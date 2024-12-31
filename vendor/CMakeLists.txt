file(GLOB imgui-src imgui/*.cpp imgui/*.h)
add_library(imgui STATIC
    ${imgui-src}
    imgui/backends/imgui_impl_dx11.h
    imgui/backends/imgui_impl_dx11.cpp
    imgui/backends/imgui_impl_win32.h
    imgui/backends/imgui_impl_win32.cpp
)
target_include_directories(imgui PRIVATE imgui)

set_target_properties(imgui PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(fmt)
add_subdirectory(minhook)
