
if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "ios")
    include(cmake/openssl.cmake)
    include(cmake/icu.cmake)
elseif("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "android")
    include(cmake/ndk-toolchain.cmake)
    include(cmake/icu.cmake)
    include(cmake/openssl.cmake)
endif()

if(NOT ("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux"))
    include(cmake/elacarrier.cmake)
endif()

if(("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux"))
    if("$ENV{XDK_TARGET_CPU_ARCH}" STREQUAL "32")
        xdk_add_subdirectories(
            raspberry_lib_and_includes
        )
    endif()
endif()

