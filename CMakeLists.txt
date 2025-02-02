cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxs VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# module 
include_directories(
    util
    protocol
    network
    ncx_client
    ncx_server
)

# zeromq
# 查找ZeroMQ库
find_package(PkgConfig REQUIRED)
pkg_check_modules(PC_ZEROMQ REQUIRED libzmq)

# 包含ZeroMQ头文件
include_directories(${PC_ZEROMQ_INCLUDE_DIRS})

# 链接ZeroMQ库
link_directories(${PC_ZEROMQ_LIBRARY_DIRS})
link_libraries(${PC_ZEROMQ_LIBRARIES})

# protobuf
# 查找Protobuf库
find_package(Protobuf REQUIRED)
# 打印 Protobuf 信息
message(STATUS "Using Protobuf ${Protobuf_VERSION}")
message(STATUS "Protobuf include dirs: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "Protobuf libraries: ${Protobuf_LIBRARIES}")

# Boost
## Boost.UUID
find_package(Boost REQUIRED)

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

# test code set

# test for echo
# file(GLOB ECHOSERV "example/echo/serv.cpp")
# file(GLOB ECHOCLIENT "example/echo/cli.cpp")
# add_executable(echoserv ${ECHOSERV} ${NET} ${UTIL})
# add_executable(echocli ${ECHOCLIENT} ${NET} ${UTIL})

# test_for_ex
# file(GLOB EXSERV "example/ex/test_ex_serv.cpp")
# file(GLOB EXCLIENT1 "example/ex/test_ex_cli1.cpp")
# file(GLOB EXCLIENT2 "example/ex/test_ex_cli2.cpp")
# add_executable(exserv ${EXSERV} ${NET} ${UTIL} ${LOG})
# add_executable(excli1 ${EXCLIENT1} ${NET} ${UTIL} ${LOG})
# add_executable(excli2 ${EXCLIENT2} ${NET} ${UTIL} ${LOG})

# test_for_musl_channel
# file(GLOB MUSLTEST "example/mchannel/test.cpp")
# add_executable(musltest ${MUSLTEST} ${UTIL} ${NET})

# test_for_buffer
# file(GLOB BUFFER "example/buffer/test_buffer.cpp")
# add_executable(buffertest ${BUFFER} ${NET})

# test_for_protocol
# file(GLOB PROTO "example/protobuf/test_protobuf.cpp")
# add_executable(protest ${PROTO} ${PROTOCOL} ${NET})
# target_link_libraries(protest ${Protobuf_LIBRARIES})

# test_for_uuid
# file(GLOB UUID "example/uuid/test_uuid.cpp")
# add_executable(uuidtest ${UUID} ${UTIL})
# target_link_libraries(uuidtest ${Boost_LIBRARIES})

# test_for_toml
# file(GLOB TOML "example/toml/test_toml.cpp")
# add_executable(tomltest ${TOML})
# target_link_libraries(tomltest tomlplusplus::tomlplusplus)

# src 
file(GLOB NCXS "ncx_server/ncxs.cpp")
file(GLOB NCXC "ncx_client/ncxc.cpp")
add_executable(ncxs ${NCXS} ${NET} ${UTIL} ${NCX_SERVER} ${PROTOCOL})
add_executable(ncxc ${NCXC} ${NET} ${UTIL} ${NCX_CLIENT} ${PROTOCOL})

# link protobuf
target_link_libraries(ncxc ${Protobuf_LIBRARIES})
target_link_libraries(ncxs ${Protobuf_LIBRARIES})

# link boost.UUID
target_link_libraries(ncxc ${Boost_LIBRARIES})
target_link_libraries(ncxs ${Boost_LIBRARIES})

# link tomlplusplus
target_link_libraries(ncxc tomlplusplus::tomlplusplus)
target_link_libraries(ncxs tomlplusplus::tomlplusplus)