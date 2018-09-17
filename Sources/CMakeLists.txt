
cmake_minimum_required(VERSION 2.8)

project(ElastosRuntime)

include($ENV{XDK_COMMON_CMAKE})

if("$ENV{XDK_TARGET_PRODUCT}" STREQUAL "devtools")
    xdk_add_subdirectories(
        Elastos/Runtime/Library/inc
        Elastos/Runtime/Library/clsmodule
        ToolChains
    )
else()
    xdk_add_subdirectories(
        ThirdParty
        Elastos
        Sample
        Tests
    )
endif()
