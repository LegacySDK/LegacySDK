cmake_minimum_required(VERSION 3.16)
project(legacySDKLoader VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(legacysdk SHARED
    src/dllmain.cpp
    src/terminal.cpp
    src/modloader.cpp
    ../sdk/src/log.cpp
    ../sdk/src/input.cpp
    ../sdk/src/mod.cpp
)

if(WIN32)
    set_target_properties(legacysdk PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

target_include_directories(legacysdk PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/minhook/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../sdk/include
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MINHOOK_LIB "${CMAKE_CURRENT_SOURCE_DIR}/minhook/lib/libMinHook.x64.lib")
    message(STATUS "Configuring legacysdk for x64 architecture")
else()
    set(MINHOOK_LIB "${CMAKE_CURRENT_SOURCE_DIR}/minhook/lib/libMinHook.x86.lib")
    message(STATUS "Configuring legacysdk for x86 architecture")
endif()

target_link_libraries(legacysdk PRIVATE ${MINHOOK_LIB})