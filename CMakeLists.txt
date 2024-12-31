cmake_minimum_required(VERSION 3.15)
project(ts-extra-utilities VERSION 1.0.0)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE ${CMAKE_PROJECT_NAME}-src src/*.hpp src/*.cpp)

set(PLUGIN_VERSION_MAJOR 1)
set(PLUGIN_VERSION_MINOR 0)
set(PLUGIN_VERSION_PATCH 0)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.rc
)

foreach(_source IN ITEMS ${${CMAKE_PROJECT_NAME}-src})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

add_library(${CMAKE_PROJECT_NAME} SHARED ${${CMAKE_PROJECT_NAME}-src} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE TS_EXTRA_UTILITIES_DLL_EXPORTS UNICODE _UNICODE)

target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(vendor)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src vendor/scs_sdk/include vendor/imgui vendor/minhook/include)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE fmt::fmt-header-only imgui minhook)
