cmake_minimum_required(VERSION 3.21)
project(Proxy)

add_library(Proxy SHARED
    src/proxy.cpp
    src/proxy.rc
)
target_compile_features(Proxy PUBLIC cxx_std_23)
set_target_properties(Proxy PROPERTIES
    PREFIX ""
	OUTPUT_NAME "XINPUT9_1_0"
)

if (MSVC)
    include(CheckLinkerFlag)
    check_linker_flag(CXX "/NOEXP" SUPPORTS_NOEXP)
    if (SUPPORTS_NOEXP)
        target_link_options(Proxy PRIVATE "/NOEXP")
    endif()

    target_link_options(Proxy PRIVATE "/NOIMPLIB /DEBUG:NONE")
endif()

target_link_libraries(Proxy PRIVATE version)