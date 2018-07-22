
set(SOURCES
    Utils.cpp
    Key.cpp
    ByteStream.cpp
    BTCKey.cpp
    BTCBase58.cpp
    BRInt.c
    BRCrypto.c
    BRAddress.c
)

add_library(didinspector.utils STATIC ${SOURCES})
