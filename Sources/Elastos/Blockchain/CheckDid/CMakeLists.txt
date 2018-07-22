
set(CAR_FLAGS -n -u)
set(LUBE_FLAGS -n -u)

include(Elastos.CheckDID.cmake)
xdk_export_car_headers(export_headers ${CURRENT_MIRROR_HEADERS})
xdk_compile_car(CAR_GENERATED_SOURCES Elastos.CheckDID.car ${export_headers})

set(SOURCES
    ${CAR_GENERATED_SOURCES}
    ${CURRENT_MIRROR_SOURCES}
    CCheckDID.cpp
    CheckDid.cpp
    Utils.cpp
    Key.cpp
    ByteStream.cpp
    BTCKey.cpp
    BTCBase58.cpp
    BRInt.c
    BRCrypto.c
    BRAddress.c
)

add_library(Elastos.CheckDID SHARED ${SOURCES})

target_link_libraries(Elastos.CheckDID
    Elastos.Runtime
    Elastos.Runtime_static
    Elastos.CoreLibrary
    crypto
)
