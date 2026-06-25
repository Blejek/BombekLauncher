cmake_minimum_required(VERSION 4.2)

file(GLOB_RECURSE _SOURCES *.c *.cpp *.cxx)
file(GLOB_RECURSE _HEADERS *.h *.hpp *.hxx)
file(GLOB_RECURSE _MODULES *.ixx *.cppm)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(LIBRARIES 
    "soalib::soalib"
    "spdlog::spdlog_header_only"
)

add_library(BombekLauncher_lib STATIC)

target_sources(BombekLauncher_lib
    PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
            ${_MODULES}

    PRIVATE
        ${_SOURCES}
        ${_HEADERS}
)

target_compile_features(BombekLauncher_lib
    PUBLIC
        cxx_std_23
)

target_include_directories(BombekLauncher_lib
    PUBLIC
        ${INCLUDES}
)

target_link_libraries(BombekLauncher_lib
    PUBLIC
        ${LIBRARIES}
)

