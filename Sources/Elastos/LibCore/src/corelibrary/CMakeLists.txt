
set(CAR_FLAGS -n -u)
set(LUBE_FLAGS -n -u)

include(Elastos.CoreLibrary.cmake)
xdk_export_car_headers(export_headers ${CURRENT_MIRROR_HEADERS})
xdk_compile_car(CAR_GENERATED_SOURCES Elastos.CoreLibrary.car ${export_headers})

set(SOURCES
    ${CAR_GENERATED_SOURCES}
    ${CURRENT_MIRROR_SOURCES}
    AsynchronousCloseMonitor.cpp
#    Atomic.cpp
    Symbols.cpp
)

add_definitions(-DELASTOS_CORELIBRARY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../inc)

add_library(icuuc SHARED IMPORTED GLOBAL )
set_target_properties( icuuc PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/libicuuc.so )
add_library(icui18n SHARED IMPORTED GLOBAL )
set_target_properties( icui18n PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/libicui18n.so.57.1 )
add_library(crypto SHARED IMPORTED GLOBAL )
set_target_properties( crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/libcrypto.a )
#add_library(rt SHARED IMPORTED GLOBAL )
#set_target_properties( rt PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/librt.a )

add_library(Elastos.CoreLibrary SHARED ${SOURCES})

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
    rt
)

add_dependencies(Elastos.CoreLibrary libcore_headers)

if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux")
    target_link_libraries(Elastos.CoreLibrary icui18n)
endif()
