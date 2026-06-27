cmake_minimum_required(VERSION 3.16)
project(legacySDKLoader VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# adding a subdirectory new for minhook
add_subdirectory(minhook)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/minhook/include # patch for linux
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sdk/enums
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include_fix # patch for linux
    ${CMAKE_CURRENT_SOURCE_DIR}/src # Fuck the git accions
)

# If it's not Windows import the correct library according to the compiler or system 
if(NOT WIN32 OR MINGW)
   message(STATUS "Configuring legacysdk for Linux/MinGW")
else()
   message(STATUS "Configuring legacysdk for Windows Native")
endif()

# New change after a boring and painful afternoon :
# Instead of using the variable ${MINHOOK_LIB}, use the target 'minhook'
# That CMake created when reading the 'minhook' folder.
# if this detects are you using MinGW, this constructs minhook.a from the src
# if this detects are you using Visual Studio, this constructs minhook.lib from the src
target_link_libraries(legacysdk PRIVATE minhook)