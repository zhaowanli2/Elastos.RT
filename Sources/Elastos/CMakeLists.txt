
xdk_add_subdirectories(
    Runtime
    LibCore
)

if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "android")
    xdk_add_subdirectories(
        Blockchain
    )
elseif("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux" AND "$ENV{XDK_TARGET_CPU_ARCH}" STREQUAL "64")
    xdk_add_subdirectories(
        Blockchain
    )
endif()
