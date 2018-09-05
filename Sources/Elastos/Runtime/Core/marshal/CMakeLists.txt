
if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "android")
    xdk_add_subdirectories(
        p2p_rpc
    )
endif()

if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux")
    xdk_add_subdirectories(
        sock_rpc
        #MarshalRPCTest
    )
endif()

add_definitions(-D_CAR_RUNTIME)

xdk_add_subdirectories(common)

