cmake_minimum_required(VERSION 3.16)
project(example_mod VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LEGACYSDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../loader)

add_library(example_mod SHARED
    src/main.cpp
)

target_include_directories(example_mod PRIVATE
    ${LEGACYSDK_DIR}/src/sdk/include
    ${LEGACYSDK_DIR}/src/sdk/enums
)

target_link_libraries(example_mod PRIVATE
    ${LEGACYSDK_DIR}/build/Release/legacysdk.lib
)

if(WIN32)
    set_target_properties(example_mod PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()
