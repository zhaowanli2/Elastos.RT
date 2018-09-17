

set(SOURCES
    testMain.cpp
)

add_executable(reflectionUnitTests ${SOURCES})

target_link_libraries(reflectionUnitTests
    Elastos.Runtime
    Elastos.Runtime_static
    Elastos.CoreLibrary
    cppunit
)

