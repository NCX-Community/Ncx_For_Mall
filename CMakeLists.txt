cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxs VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

# module 
include_directories(
    cli
    util
    protocol
    network
    ncx_client
    ncx_server
    http
)

# Json
find_package(nlohmann_json REQUIRED)
if(NLOHMANN_JSON_FOUND)
    include_directories(${NLOHMANN_JSON_INCLUDE_DIRS})
    message(STATUS "Using nlohmann_json ${NLOHMANN_JSON_VERSION}")
    message(STATUS "nlohmann_json include dirs: ${NLOHMANN_JSON_INCLUDE_DIRS}")
    message(STATUS "nlohmann_json libraries: ${NLOHMANN_JSON_LIBRARIES}")
else()
    message(FATAL_ERROR "nlohmann_json not found")
endif()

# protobuf
# 查找Protobuf库
find_package(Protobuf REQUIRED)
# 打印 Protobuf 信息
if(PROTOBUF_FOUND)
    include_directories(${PROTOBUF_INCLUDE_DIRS})
    message(STATUS "Using Protobuf ${Protobuf_VERSION}")
    message(STATUS "Protobuf include dirs: ${PROTOBUF_INCLUDE_DIRS}")
    message(STATUS "Protobuf libraries: ${PROTOBUF_LIBRARIES}")
else()
    message(FATAL_ERROR "Protobuf not found")
endif()

# Boost
find_package(Boost REQUIRED COMPONENTS program_options)
if(BOOST_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Using Boost ${Boost_VERSION}")
    message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
else()
    message(FATAL_ERROR "Boost not found")
endif()

# TomlPlusPlus
include(FetchContent)
FetchContent_Declare(
    TomlPlusPlus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(TomlPlusPlus)

# source code
aux_source_directory(network NET)
aux_source_directory(util UTIL)
aux_source_directory(ncx_client NCX_CLIENT)
aux_source_directory(ncx_server NCX_SERVER)
aux_source_directory(protocol PROTOCOL)
aux_source_directory(cli CLI)
aux_source_directory(http HTTP)

# src 
file(GLOB NCXS "ncx_server/ncxs.cpp")
file(GLOB NCXC "ncx_client/ncxc.cpp")
add_executable(ncxs ${NCXS} ${NET} ${UTIL} ${NCX_SERVER} ${PROTOCOL} ${CLI} ${HTTP})
add_executable(ncxc ${NCXC} ${NET} ${UTIL} ${NCX_CLIENT} ${PROTOCOL} ${CLI} ${HTTP})

# link boost
target_link_libraries(ncxc ${Boost_LIBRARIES})
target_link_libraries(ncxs ${Boost_LIBRARIES})

# link boost program_options
target_link_libraries(ncxc Boost::program_options)
target_link_libraries(ncxs Boost::program_options)

# link tomlplusplus
target_link_libraries(ncxc tomlplusplus::tomlplusplus)
target_link_libraries(ncxs tomlplusplus::tomlplusplus)

# link protobuf
target_link_libraries(ncxc ${Protobuf_LIBRARIES})
target_link_libraries(ncxs ${Protobuf_LIBRARIES})

# link nlohmann_json
target_link_libraries(ncxc ${NLOHMANN_JSON_LIBRARIES})
target_link_libraries(ncxs ${NLOHMANN_JSON_LIBRARIES})