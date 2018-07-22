xdk_add_subdirectories(
    HelloCar
    HelloCarDemo
    RPC
)

if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "android")
    xdk_add_subdirectories(
        AutoGenerateJavaCodes
    )
endif()

if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux" AND "$ENV{XDK_TARGET_CPU_ARCH}" STREQUAL "64")
    xdk_add_subdirectories(
        WalletTest
    )
endif()
