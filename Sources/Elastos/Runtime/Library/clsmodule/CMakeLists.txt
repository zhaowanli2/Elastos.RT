
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../ThirdParty/raspberry_lib_and_includes/zlib/usr/include)

set(SOURCES
    clserr.cpp
    comprcls.cpp
    reloc.cpp
    datatype.cpp
)

add_library(clsinfo STATIC ${SOURCES})

