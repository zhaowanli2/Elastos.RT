
set(CAR_FLAGS -n -u)
set(LUBE_FLAGS -n -u)

xdk_add_subdirectories(
    Utils
)

include(Elastos.DIDInspector.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Utils)
xdk_export_car_headers(export_headers ${CURRENT_MIRROR_HEADERS})
xdk_compile_car(CAR_GENERATED_SOURCES Elastos.DIDInspector.car ${export_headers})

set(SOURCES
    ${CAR_GENERATED_SOURCES}
    ${CURRENT_MIRROR_SOURCES}
    CDIDInspector.cpp
)

add_library(Elastos.DIDInspector SHARED ${SOURCES})

target_link_libraries(Elastos.DIDInspector
    Elastos.Runtime
    Elastos.Runtime_static
    Elastos.CoreLibrary
    didinspector.utils
    crypto
)
