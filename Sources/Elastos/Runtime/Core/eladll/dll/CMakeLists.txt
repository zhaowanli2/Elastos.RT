
add_definitions(-D_USE_MY_DLLMAIN_)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../inc)

set(SOURCES
    dllmain.cpp
    dllinit.cpp
)

xdk_compile_def(DEF_GENERATED_SOURCES elastos.def)
add_library(Elastos.Runtime SHARED ${DEF_GENERATED_SOURCES} ${SOURCES})

set(MY_LINK_LIBRARIES)

list(APPEND MY_LINK_LIBRARIES
    marshalcomm
    elapi
)

if(NOT ("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux"))
    list(APPEND MY_LINK_LIBRARIES
        elcarrierapi
        elacarrier
        elacommon
        elasession
    )
endif()

add_library(uv SHARED IMPORTED GLOBAL )
set_target_properties( uv PROPERTIES IMPORTED_LOCATION /home/wanli/workspace/RaspberryPi/uv/libuv.so )
add_library(z SHARED IMPORTED GLOBAL )
set_target_properties( z PROPERTIES IMPORTED_LOCATION /home/wanli/workspace/RaspberryPi/zlib/usr/lib/libz.so )

if("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "android")
    list(APPEND MY_LINK_LIBRARIES
        servicemanager
        javacarmanager
        marshal_rpc
        log
    )
elseif("$ENV{XDK_TARGET_PLATFORM}" STREQUAL "linux")
    list(APPEND MY_LINK_LIBRARIES
        marshal_rpc
        uv
        pthread
        dl
    )
elseif(NOT "$ENV{XDK_TARGET_PLATFORM}" STREQUAL "ios")
    list(APPEND MY_LINK_LIBRARIES
        pthread
        dl
    )
endif()

list(APPEND MY_LINK_LIBRARIES
    reflection
    carapi
    lib4ela
    eltypes
    elasys
    clsinfo
    z
)

target_link_libraries(Elastos.Runtime ${XDK_START_GROUP} ${MY_LINK_LIBRARIES} ${XDK_END_GROUP})
