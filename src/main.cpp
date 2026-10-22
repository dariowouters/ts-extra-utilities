#include <Windows.h>
#include <stdio.h>
#include <scssdk_telemetry.h>
#include "scssdk.h"

#include "./core.hpp"

thread_local std::unique_ptr< ts_extra_utilities::CCore > g_core;

namespace
{
    HMODULE g_module = nullptr;

    // The game's own crash handler is already gone when a plugin crashes during
    // process teardown, so those faults never reach game.crash.txt. This writes
    // the faulting address/module to a file next to the DLL instead.
    void trace_close( const char* fmt, ... )
    {
        wchar_t path[ MAX_PATH ];
        if ( g_module == nullptr || GetModuleFileNameW( g_module, path, MAX_PATH ) == 0 ) return;
        wchar_t* slash = wcsrchr( path, L'\\' );
        if ( slash == nullptr ) return;
        wcscpy_s( slash + 1, path + MAX_PATH - slash - 1, L"ts-shutdown.log" );

        FILE* f = _wfopen( path, L"a" );
        if ( f == nullptr ) return;
        va_list args;
        va_start( args, fmt );
        vfprintf( f, fmt, args );
        va_end( args );
        fputc( '\n', f );
        fclose( f );
    }

    // The game can hang in its own teardown after the save completes — a
    // known engine bug spins forever unregistering loaders and the process
    // never exits (see the endless "[mem] Attempt to unregister" spam).
    // WndProcD3D arms the watchdog on WM_CLOSE/WM_DESTROY; it only fires if
    // the render loop is also dead, so a window recreation (fullscreen
    // toggle) never triggers it. A normal shutdown disarms it.
    std::atomic< uint64_t > g_close_armed_ms{ 0 };
    std::atomic< uint64_t > g_last_present_ms{ 0 };

    DWORD WINAPI exit_watchdog( LPVOID )
    {
        for ( ;; )
        {
            Sleep( 500 );
            const uint64_t armed = g_close_armed_ms.load( std::memory_order_acquire );
            if ( armed == 0 ) continue;
            const uint64_t now = GetTickCount64();
            const uint64_t last = g_last_present_ms.load( std::memory_order_acquire );
            if ( now - armed >= 15000 && ( last == 0 || now - last >= 5000 ) )
            {
                trace_close( "watchdog: teardown stalled %llums after close, forcing exit",
                             static_cast< unsigned long long >( now - armed ) );
                TerminateProcess( GetCurrentProcess(), 0 );
            }
        }
    }

    LONG WINAPI close_crash_trace( EXCEPTION_POINTERS* ep )
    {
        const DWORD code = ep->ExceptionRecord->ExceptionCode;
        // during shutdown every exception is suspicious; outside it only log
        // the fatal ones so normal first-chance noise doesn't fill the file
        const bool fatal = code == EXCEPTION_ACCESS_VIOLATION
            || code == 0xC0000374 /* heap corruption */
            || code == 0xC0000409 /* fast fail */
            || code == 0x80000003 /* breakpoint / abort */
            || code == 0xC00000FD /* stack overflow */
            || code == 0xE06D7363 /* C++ exception */;
        if ( !fatal && !ts_extra_utilities::g_shutting_down.load() ) return EXCEPTION_CONTINUE_SEARCH;

        static std::atomic< int > logged{ 0 };
        if ( logged.fetch_add( 1 ) > 400 ) return EXCEPTION_CONTINUE_SEARCH;

        const auto addr = ep->ExceptionRecord->ExceptionAddress;
        HMODULE mod = nullptr;
        GetModuleHandleExA( GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            reinterpret_cast< LPCSTR >( addr ), &mod );
        char name[ MAX_PATH ] = "<no module>";
        uintptr_t offset = 0;
        if ( mod != nullptr )
        {
            GetModuleFileNameA( mod, name, MAX_PATH );
            offset = reinterpret_cast< uintptr_t >( addr ) - reinterpret_cast< uintptr_t >( mod );
        }
        const char* file = strrchr( name, '\\' );
        trace_close( "EXC %08lx at %p (%s+0x%llx) tid=%lu%s", code, addr,
                     file != nullptr ? file + 1 : name,
                     static_cast< unsigned long long >( offset ),
                     GetCurrentThreadId(),
                     ts_extra_utilities::g_shutting_down.load() ? " [during shutdown]" : "" );
        return EXCEPTION_CONTINUE_SEARCH;
    }
}

namespace ts_extra_utilities
{
    void ts_shutdown_mark( const char* step )
    {
        trace_close( "shutdown: %s", step );
    }

    void ts_note_present()
    {
        g_last_present_ms.store( GetTickCount64(), std::memory_order_release );
    }

    void ts_arm_exit_watchdog()
    {
        g_close_armed_ms.store( GetTickCount64(), std::memory_order_release );
    }

    void ts_disarm_exit_watchdog()
    {
        g_close_armed_ms.store( 0, std::memory_order_release );
    }
}

__declspec(dllexport) SCSAPI_RESULT scs_telemetry_init( const scs_u32_t version, const scs_telemetry_init_params_t* const params )
{
    static bool handler_registered = false;
    if ( !handler_registered )
    {
        handler_registered = true;
        AddVectoredExceptionHandler( 0, close_crash_trace );
        CreateThread( nullptr, 0, exit_watchdog, nullptr, 0, nullptr );
    }

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
    // Flag first: every detour and the per-frame trailer pass-through checks it
    // and stops touching game/plugin state. Then wait briefly for calls already
    // in flight so destroy() never frees objects mid-use (quit crash/hang).
    ts_extra_utilities::g_shutting_down.store( true, std::memory_order_release );
    ts_extra_utilities::ts_disarm_exit_watchdog();
    trace_close( "shutdown: flag set, draining detours" );
    for ( int i = 0; i < 100 && ts_extra_utilities::g_detour_depth.load( std::memory_order_acquire ) > 0; ++i )
    {
        Sleep( 10 );
    }
    trace_close( "shutdown: draining done (depth=%d)", ts_extra_utilities::g_detour_depth.load() );
    if ( g_core != nullptr )
    {
        g_core.reset();
    }
    trace_close( "shutdown: core destroyed" );
}

BOOL WINAPI DllMain( HMODULE module, DWORD reason_for_call, LPVOID reserved )
{
    if ( reason_for_call == DLL_PROCESS_ATTACH )
    {
        g_module = module;
    }
    return TRUE;
}
