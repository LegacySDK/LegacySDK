cmake_minimum_required(VERSION 3.16)
project(legacySDKLoader VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(legacysdk SHARED
    src/dllmain.cpp
    src/terminal.cpp
    src/modloader.cpp
    src/loader/UI.cpp
    src/crashlog/crashlog.cpp
    src/crashlog/crashmanager.cpp
    src/sdk/main/log.cpp
    src/sdk/main/input.cpp
    src/sdk/main/mod.cpp
    src/sdk/main/ui/uicontroller.cpp
    src/sdk/main/ui/hooks.cpp
    src/sdk/main/ui/mainmenu.cpp
)

if(WIN32)
    set_target_properties(legacysdk PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()

target_include_directories(legacysdk PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/minhook_src/include # aded a patch for linux
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/enums
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include_fix # added a patch for linux 
)

# If it's not Windows import the correct library accordin gto the compiler or system 
if(NOT WIN32 OR MINGW)
   #for linux on VSCode
   set(MINHOOK_LIB "${CMAKE_CURRENT_SOURCE_DIR}/includes/minhook_src/build/minhook.a")
   message(STATUS "Configuring legacysdk for Linux/MinGW architecture")
else()
    # For Windows native (Visual Studio)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(MINHOOK_LIB "${CMAKE_CURRENT_SOURCE_DIR}/includes/minhook/lib_old/libMinHook.x64.lib")
            message(STATUS "Configuring legacysdk for Windows x64 architecture")
        else()
            set(MINHOOK_LIB "${CMAKE_CURRENT_SOURCE_DIR}/includes/minhook/lib_old/libMinHook.x86.lib")
            message(STATUS "Configuring legacysdk for Windows x86 architecture")
    endif()
endif()

target_link_libraries(legacysdk PRIVATE "${MINHOOK_LIB}")