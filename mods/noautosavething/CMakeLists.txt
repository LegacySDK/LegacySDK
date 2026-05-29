cmake_minimum_required(VERSION 3.16)
project(noautosave VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LEGACYSDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../loader)

add_library(noautosave SHARED
    src/main.cpp
)

target_include_directories(noautosave PRIVATE
    ${LEGACYSDK_DIR}/src/sdk/include
    ${LEGACYSDK_DIR}/src/sdk/enums
)

target_link_libraries(noautosave PRIVATE
    ${LEGACYSDK_DIR}/build/Release/legacysdk.lib
)

if(WIN32)
    set_target_properties(noautosave PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()
