#include "dx11_hook.hpp"

#include <Windows.h>
#include <d3d11.h>

#include "imgui.h"

#include "core.hpp"
#include "hooks/vtable_hook.hpp"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

namespace ts_extra_utilities
{
    using D3D11CreateDeviceAndSwapChain_fnt = decltype(D3D11CreateDeviceAndSwapChain);
    using present_fn = HRESULT __stdcall( IDXGISwapChain*, UINT, UINT );

    std::shared_ptr< CVirtualFunctionHook > present_hook = nullptr;

    bool imgui_initialized;

    ID3D11Device* device;
    ID3D11DeviceContext* device_context;
    HWND prism_hwnd;
    WNDPROC original_wndproc;

    LRESULT __stdcall WndProcD3D( HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam )
    {
        if ( CCore::g_instance->on_wnd_proc( hwnd, umsg, wparam, lparam ) ) return 0;
        return CallWindowProc( original_wndproc, hwnd, umsg, wparam, lparam );
    }

    HRESULT hk_present( IDXGISwapChain* swap_chain, const UINT sync_interval, const UINT flags )
    {
        if ( !imgui_initialized )
        {
            if ( SUCCEEDED( swap_chain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&device)) ) )
            {
                ImGui::CreateContext();

                device->GetImmediateContext( &device_context );

                DXGI_SWAP_CHAIN_DESC desc;
                swap_chain->GetDesc( &desc );
                prism_hwnd = desc.OutputWindow;

                ImGui_ImplWin32_Init( prism_hwnd );
                ImGui_ImplDX11_Init( device, device_context );
                ImGui_ImplDX11_CreateDeviceObjects();
                original_wndproc = reinterpret_cast< WNDPROC >(
                    SetWindowLongPtr( prism_hwnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( WndProcD3D ) )
                );
                imgui_initialized = true;
            }
        }

        CCore::g_instance->render();

        return present_hook->get_original< present_fn >()( swap_chain, sync_interval, flags );
    }


    CDirectX11Hook::~CDirectX11Hook()
    {
        this->unhook_present();
    }

    bool CDirectX11Hook::hook_present()
    {
        const HMODULE d3d11_module = GetModuleHandle( L"d3d11.dll" );
        if ( d3d11_module == nullptr )
        {
            CCore::g_instance->error( "Could not get dx11 module" );
            return false;
        }

        WNDCLASSEX window_class{ 0 };
        window_class.cbSize = sizeof( WNDCLASSEX );
        window_class.lpfnWndProc = DefWindowProc;
        window_class.hInstance = GetModuleHandle( nullptr );
        window_class.lpszClassName = L"dummy_hook_window";
        if ( !RegisterClassEx( &window_class ) )
        {
            CCore::g_instance->error( "Could not register window class" );
            return false;
        }

        const HWND hwnd = CreateWindow( window_class.lpszClassName, L"directx_hook_window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, window_class.hInstance, NULL );
        if ( hwnd == nullptr )
        {
            UnregisterClass( window_class.lpszClassName, window_class.hInstance );
            CCore::g_instance->error( "Could not create window" );
            return false;
        }

        DXGI_SWAP_CHAIN_DESC swap_chain_desc{ 0 };
        swap_chain_desc.BufferCount = 1;
        swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swap_chain_desc.OutputWindow = hwnd;
        swap_chain_desc.SampleDesc.Count = 1;
        swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swap_chain_desc.Windowed = 1;

        constexpr D3D_FEATURE_LEVEL feature_levels[ ] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };
        D3D_FEATURE_LEVEL feature_level;

        IDXGISwapChain* dummy_swap_chain = nullptr;
        ID3D11Device* dummy_device = nullptr;
        ID3D11DeviceContext* dummy_context = nullptr;

        const auto D3D11CreateDeviceAndSwapChain_address = GetProcAddress( d3d11_module, "D3D11CreateDeviceAndSwapChain" );

        if ( D3D11CreateDeviceAndSwapChain_address == nullptr )
        {
            DestroyWindow( hwnd );
            UnregisterClass( window_class.lpszClassName, window_class.hInstance );
            CCore::g_instance->error( "Could not find the D3D11CreateDeviceAndSwapChain function" );
            return false;
        }

        const auto d3d11_create_device_and_swap_chain = reinterpret_cast< D3D11CreateDeviceAndSwapChain_fnt* >( D3D11CreateDeviceAndSwapChain_address );

        const auto hr = d3d11_create_device_and_swap_chain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, feature_levels, 2, D3D11_SDK_VERSION, &swap_chain_desc, &dummy_swap_chain, &dummy_device, &feature_level, &dummy_context );

        if ( FAILED( hr ) )
        {
            DestroyWindow( hwnd );
            UnregisterClass( window_class.lpszClassName, window_class.hInstance );
            CCore::g_instance->error( "Could not create dx11 device and swap chain (hresult = 0x{:x})", static_cast< uint32_t >( hr ) );
            return false;
        }

        present_hook = CCore::g_instance->get_hooks_manager()->register_virtual_function_hook(
            "dx11::present",
            *reinterpret_cast< uint64_t* >( dummy_swap_chain ) + 0x08 * 8,
            reinterpret_cast< uint64_t >( &hk_present )
        );

        if ( present_hook->hook() != CHook::HOOKED )
        {
            CCore::g_instance->error( "Could not hook dx11::present" );
            return false;
        }

        dummy_swap_chain->Release();
        dummy_device->Release();
        dummy_context->Release();
        DestroyWindow( hwnd );
        UnregisterClass( window_class.lpszClassName, window_class.hInstance );
        return true;
    }

    bool CDirectX11Hook::unhook_present()
    {
        SetWindowLongPtr( prism_hwnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( original_wndproc ) );

        return present_hook->unhook() == CHook::UNHOOKED;
    }
}
