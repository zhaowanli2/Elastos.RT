

set(SOURCES
    main.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../eco/elastos/hellocar)

add_executable(HelloCar ${SOURCES})

add_library(icudata SHARED IMPORTED GLOBAL )
set_target_properties( icudata PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/libicudata.so.57.1 )

add_library(stdc++ SHARED IMPORTED GLOBAL )
set_target_properties( stdc++ PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/libstdc++.so.6.0.22 )

add_library(c SHARED IMPORTED GLOBAL )
set_target_properties( c PROPERTIES IMPORTED_LOCATION /home/wanli/workspace/RaspberryPi/raspberry_lib_and_includes/lib/arm-linux-gnueabihf/libc-2.24.so )

target_link_libraries(HelloCar
    Elastos.Runtime
    Elastos.Runtime_static
    Elastos.CoreLibrary
    icudata
    stdc++
    c
)

