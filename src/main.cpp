#include <Windows.h>
#include <scssdk_telemetry.h>
#include "scssdk.h"

#include "./core.hpp"

thread_local std::unique_ptr< ts_extra_utilities::CCore > g_core;

__declspec(dllexport) SCSAPI_RESULT scs_telemetry_init( const scs_u32_t version, const scs_telemetry_init_params_t* const params )
{
    auto* init_params = static_cast< const scs_telemetry_init_params_v100_t* >( params );
    g_core = std::make_unique< ts_extra_utilities::CCore >( init_params );

    if ( !g_core->init() )
    {
        return SCS_RESULT_generic_error;
    }

    return SCS_RESULT_ok;
}

__declspec(dllexport) SCSAPI_VOID scs_telemetry_shutdown( void )
{
    if ( g_core != nullptr )
    {
        g_core.reset();
    }
}

BOOL WINAPI DllMain( HMODULE module, DWORD reason_for_call, LPVOID reserved )
{
    return TRUE;
}
