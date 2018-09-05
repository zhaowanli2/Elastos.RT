
set(SOURCES
    core.cpp
    Thread.cpp
    NativeThread.cpp
    Object.cpp
    Singleton.cpp
    Runnable.cpp
    ClassLoader.cpp

    exportext.cpp
    CString.cpp
    CBoolean.cpp
    CByte.cpp
    CChar32.cpp
    CInteger16.cpp
    CInteger32.cpp
    CInteger64.cpp
    CFloat.cpp
    CDouble.cpp
    CArrayOf.cpp
#    CThrowable.cpp
    Init.cpp

    ThreadGroup.cpp
    CThread.cpp
    CThreadGroup.cpp
    CPathClassLoader.cpp
#    Os.cpp
#    BlockGuard.cpp
#    CBlockGuard.cpp
#    CCloseGuard.cpp
#    CCloseGuardHelper.cpp
    Enum.cpp
#    CSystem.cpp

    Math.cpp
    StrictMath.cpp
    cbigint.cpp
#    CoreUtils.cpp
    StringUtils.cpp
    HexStringParser.cpp
    StringToIntegral.cpp
    IntegralToString.cpp
    StringToReal.cpp
    RealToString.cpp
    Character.cpp
    AbstractStringBuilder.cpp
    StringBuffer.cpp
    StringBuilder.cpp
#    CaseMapper.cpp
)

add_definitions(-DELASTOS_CORELIBRARY)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../corelibrary)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../ThirdParty/raspberry_lib_and_includes/include)

add_library(elastos.core STATIC ${SOURCES})
add_dependencies(elastos.core Elastos.CoreLibrary.car)



