
set(CAR_FLAGS -n -u)
set(LUBE_FLAGS -n -u)

include(Elastos.CoreLibrary.cmake)
xdk_compile_car(GENERATED_SOURCES Elastos.CoreLibrary.car)

set(SOURCES
    ${GENERATED_SOURCES}
    ${CURRENT_MIRROR_SOURCES}
    AsynchronousCloseMonitor.cpp
#    Atomic.cpp
    Symbols.cpp
)

add_definitions(-DELASTOS_CORELIBRARY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../inc)

add_library(Elastos.CoreLibrary SHARED ${SOURCES})

if(ANDROID)
    # TODO: To be remove
    # Build from source or add 64-bits libraries
    add_library( icuuc SHARED IMPORTED GLOBAL )
    set_target_properties( icuuc PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../ToolChains/android/Thirdlibrary/lib/libicuuc.cr.so )
    add_library( crypto SHARED IMPORTED GLOBAL )
    set_target_properties( crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../ToolChains/android/Thirdlibrary/lib/libcrypto.so )
endif()

target_link_libraries(Elastos.CoreLibrary
    elastos.utility
    libcore.utility
    elastos.core
    elastos.io
    elastos.math
    Elastos.Runtime
    icuuc
    crypto
    z
)
