cmake_minimum_required(VERSION 3.16)
project(noautosave VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(noautosave SHARED
    src/main.cpp
)
target_include_directories(noautosave PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ROOT/sdk_dist
)

target_link_libraries(noautosave PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../loader/build/Release/legacysdk.lib
)
if(WIN32)
    set_target_properties(noautosave PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
endif()
