

set(SOURCES
    main.cpp
)

add_executable(WalletTest ${SOURCES})

target_link_libraries(WalletTest
    Elastos.Runtime
    Elastos.Runtime_static
    Elastos.CoreLibrary
    Elastos.Wallet
)

