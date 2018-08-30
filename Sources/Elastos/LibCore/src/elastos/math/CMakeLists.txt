
set(SOURCES
    NativeBN.cpp
    BigInt.cpp
    BitLevel.cpp
    Logical.cpp
    Primality.cpp
    Conversion.cpp
    Division.cpp
    Multiplication.cpp
    CBigDecimal.cpp
    CBigDecimalHelper.cpp
    CBigInteger.cpp
    CBigIntegerHelper.cpp
    CMathContext.cpp
    CMathContextHelper.cpp
)

add_definitions(-DELASTOS_CORELIBRARY)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../corelibrary)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../inc)
include_directories(/home/wanli/workspace/RaspberryPi/raspberry_lib_and_includes/include)
include_directories(/home/wanli/workspace/RaspberryPi/raspberry_lib_and_includes/include/arm-linux-gnueabihf)

add_library(elastos.math STATIC ${SOURCES})
add_dependencies(elastos.math Elastos.CoreLibrary.car)



