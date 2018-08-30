
include_directories(/home/wanli/workspace/RaspberryPi/zlib/usr/include)

set(SOURCES
    clserr.cpp
    comprcls.cpp
    reloc.cpp
    datatype.cpp
)

add_library(clsinfo STATIC ${SOURCES})

