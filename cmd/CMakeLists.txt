cmake_minimum_required(VERSION 4.2)

file(GLOB_RECURSE _SOURCES *.c *.cpp *.cxx)
file(GLOB_RECURSE _HEADERS *.h *.hpp *.hxx)
file(GLOB_RECURSE _MODULES *.ixx *.cppm)

set(INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(LIBRARIES 
    "BombekLauncher_lib"
)

add_executable(BombekLauncher)

target_sources(BombekLauncher
    PUBLIC
        FILE_SET cxx_modules TYPE CXX_MODULES FILES
            ${_MODULES}
    PRIVATE
        ${_SOURCES}
        ${_HEADERS}
)

target_compile_features(BombekLauncher
    PUBLIC
        cxx_std_23
)

target_include_directories(BombekLauncher
    PUBLIC
        ${INCLUDES}
)

target_link_libraries(BombekLauncher
    PUBLIC
        ${LIBRARIES}
)