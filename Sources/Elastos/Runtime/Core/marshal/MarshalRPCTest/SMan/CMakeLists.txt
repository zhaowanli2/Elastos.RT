
set(SOURCES
    SMan.cpp
)

add_executable(SMan ${SOURCES})

target_include_directories(
    SMan PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/../SManS
)

add_library(icudata SHARED IMPORTED GLOBAL )
set_target_properties( icudata PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/libicudata.so.57.1 )

add_library(stdc++ SHARED IMPORTED GLOBAL )
set_target_properties( stdc++ PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ThirdParty/raspberry_lib_and_includes/arm-linux-gnueabihf/libstdc++.so.6.0.22 )

target_link_libraries(SMan
    Elastos.Runtime_static
    Elastos.Runtime
    Elastos.CoreLibrary
    pthread
    icudata
    stdc++
    rt
)

