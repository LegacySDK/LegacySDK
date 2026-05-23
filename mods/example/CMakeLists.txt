cmake_minimum_required(VERSION 3.16)
project(example_mod VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(example_mod SHARED
    src/main.cpp
)
target_include_directories(example_mod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ROOT/sdk_dist
)

target_link_libraries(example_mod PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../loader/build/Release/legacysdk.lib
)
if(WIN32)
    set_target_properties(example_mod PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()
