cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxs VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# module 
include_directories(
    util
    log
    src
    protocol
    network
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


# source code
aux_source_directory(network NET)
aux_source_directory(util UTIL)
aux_source_directory(log LOG)
aux_source_directory(src SRC)
aux_source_directory(protocol PROTOCOL)

# test code set

# test for echo
file(GLOB ECHOSERV "example/echo/serv.cpp")
file(GLOB ECHOCLIENT "example/echo/cli.cpp")
add_executable(echoserv ${ECHOSERV} ${NET} ${UTIL} ${LOG})
add_executable(echocli ${ECHOCLIENT} ${NET} ${UTIL} ${LOG})

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
file(GLOB PROTO "protocol/test_protocol.cpp")
add_executable(protest ${PROTO} ${PROTOCOL} ${NET})
target_link_libraries(protest ${Protobuf_LIBRARIES})

# src 
# file(GLOB NCXS "src/server.cpp")
# file(GLOB NCXC "src/client.cpp")
# add_executable(ncxs ${NCXS} ${NET} ${UTIL} ${LOG} ${PROTOCOL})
# add_executable(ncxc ${NCXC} ${NET} ${UTIL} ${LOG} ${PROTOCOL})

# 链接ZeroMQ库到可执行文件
# target_link_libraries(exserv ${PC_ZEROMQ_LIBRARIES})
# target_link_libraries(musltest ${PC_ZEROMQ_LIBRARIES})
# target_link_libraries(ncxs ${PC_ZEROMQ_LIBRARIES})
# target_link_libraries(ncxc ${PC_ZEROMQ_LIBRARIES})