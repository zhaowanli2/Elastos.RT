
add_definitions(-DELASTOS_CORELIBRARY)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../../ThirdParty/raspberry_lib_and_includes/include)

set(SOURCES
    CMatcher.cpp
    CMatcherHelper.cpp
    CPatternHelper.cpp
    CSplitter.cpp
    MatchResultImpl.cpp
    Pattern.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../corelibrary)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../inc)

add_library(elastos_utility_regex STATIC ${SOURCES})
add_dependencies(elastos_utility_regex Elastos.CoreLibrary.car)
