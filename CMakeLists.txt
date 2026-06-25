cmake_minimum_required(VERSION 4.2)

project(BombekLauncher)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD ON)

set(CMAKE_C_FLAGS_INIT                "-Wall -std=c23")
set(CMAKE_C_FLAGS_DEBUG_INIT          "-g")
set(CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

set(CMAKE_CXX_FLAGS_INIT                "-Wall -std=c++23")
set(CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE_INIT        "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

message(STATUS "Cmake build type:")
message(STATUS ${CMAKE_BUILD_TYPE})

add_subdirectory(vendor)
add_subdirectory(lib)
add_subdirectory(cmd)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_target_properties(BombekLauncher PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/BombekLauncher/"
)
