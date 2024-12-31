#include "di8_hook.hpp"

#include <Windows.h>
#include <dinput.h>
#include "core.hpp"
#include "hooks/vtable_hook.hpp"

namespace ts_extra_utilities
{
    using DirectInput8Create_fnt = decltype(DirectInput8Create);
    using get_device_data_fn = HRESULT __stdcall( IDirectInputDevice8W*, DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD );

    const GUID IDirectInput8W_guid{ 0xBF798031, 0x483A, 0x4DA2, 0xAA, 0x99, 0x5D, 0x64, 0xED, 0x36, 0x97, 0x00 };
    const GUID SysMouseEm_guid{ 0x6F1D2B80, 0xD5A0, 0x11CF, 0xBF, 0xC7, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 };

    std::shared_ptr< CVirtualFunctionHook > get_device_data_hook = nullptr;


    HRESULT hk_get_device_data( IDirectInputDevice8W* device, DWORD cb_object_data, LPDIDEVICEOBJECTDATA rgdod, LPDWORD pdw_in_out, DWORD dw_flags )
    {
        const auto result = get_device_data_hook->get_original< get_device_data_fn >()( device, cb_object_data, rgdod, pdw_in_out, dw_flags );

        DIDEVICEINSTANCEW instance;
        instance.dwSize = sizeof( DIDEVICEINSTANCEW );
        if ( FAILED( device->GetDeviceInfo(&instance) ) )
        {
            CCore::g_instance->error( "Could not get di8 device info" );
            return result;
        }

        const auto device_type = GET_DIDEVICE_TYPE( instance.dwDevType );

        if ( device_type == DI8DEVTYPE_MOUSE )
        {
            for ( int i = 0; i < *pdw_in_out; i++ )
            {
                CCore::g_instance->on_mouse_input( rgdod + i );
            }

            if ( CCore::g_instance->disable_in_game_mouse )
            {
                *pdw_in_out = 0;
            }
        }

        return result;
    }

    CDirectInput8Hook::~CDirectInput8Hook()
    {
        this->unhook();
    }

    bool CDirectInput8Hook::hook()
    {
        IDirectInput8W* dummy_interface;
        IDirectInputDevice8W* dummy_device;

        const HMODULE di8_module = GetModuleHandle( L"dinput8.dll" );
        if ( di8_module == nullptr )
        {
            CCore::g_instance->error( "Could not get dinput8 module" );
            return false;
        }

        const auto DirectInput8Create_address = GetProcAddress( di8_module, "DirectInput8Create" );

        if ( DirectInput8Create_address == nullptr )
        {
            CCore::g_instance->error( "Could not find the DirectInput8Create function" );
            return false;
        }

        const auto direct_input8_create = reinterpret_cast< DirectInput8Create_fnt* >( DirectInput8Create_address );

        if ( FAILED( direct_input8_create(GetModuleHandle(nullptr), 0x800, IDirectInput8W_guid, reinterpret_cast<LPVOID*>(&dummy_interface), nullptr) ) )
        {
            CCore::g_instance->error( "Could not create direct input 8 interface" );
            return false;
        }

        if ( FAILED( dummy_interface->CreateDevice(SysMouseEm_guid, &dummy_device, nullptr) ) )
        {
            dummy_interface->Release();
            CCore::g_instance->error( "Could not create direct input 8 dummy device" );
            return false;
        }

        get_device_data_hook = CCore::g_instance->get_hooks_manager()->register_virtual_function_hook(
            "di8::get_device_data",
            *reinterpret_cast< uint64_t* >( dummy_device ) + 0x08 * 10,
            reinterpret_cast< uint64_t >( &hk_get_device_data )
        );

        if ( get_device_data_hook->hook() != CHook::HOOKED )
        {
            dummy_device->Release();
            dummy_interface->Release();
            CCore::g_instance->error( "Could not hook di8::get_device_data" );
            return false;
        }

        dummy_device->Release();
        dummy_interface->Release();
        return true;
    }

    bool CDirectInput8Hook::unhook()
    {
        return get_device_data_hook->unhook() == CHook::UNHOOKED;
    }
}
