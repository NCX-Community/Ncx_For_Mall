cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxs VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# module 
include_directories(
    tcp
    util
    log
    src
    protocol
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

# source code
aux_source_directory(tcp TCP)
aux_source_directory(util UTIL)
aux_source_directory(log LOG)
aux_source_directory(src SRC)
aux_source_directory(protocol PROTOCOL)

# test code set

# test for echo
# file(GLOB ECHOSERV "example/echo/serv.cpp")
# file(GLOB ECHOCLIENT "example/echo/cli.cpp")
# add_executable(echoserv ${ECHOSERV} ${TCP} ${UTIL} ${LOG})
# add_executable(echocli ${ECHOCLIENT} ${TCP} ${UTIL} ${LOG})

# test_for_ex
# file(GLOB EXSERV "example/ex/test_ex_serv.cpp")
# file(GLOB EXCLIENT1 "example/ex/test_ex_cli1.cpp")
# file(GLOB EXCLIENT2 "example/ex/test_ex_cli2.cpp")
# add_executable(exserv ${EXSERV} ${TCP} ${UTIL} ${LOG})
# add_executable(excli1 ${EXCLIENT1} ${TCP} ${UTIL} ${LOG})
# add_executable(excli2 ${EXCLIENT2} ${TCP} ${UTIL} ${LOG})

# test_for_musl_channel
# file(GLOB MUSLTEST "example/mchannel/test.cpp")
# add_executable(musltest ${MUSLTEST} ${UTIL} ${TCP})

# test_for_buffer
file(GLOB BUFFER "example/buffer/test_buffer.cpp")
add_executable(buffertest ${BUFFER} ${TCP})

# src 
# file(GLOB NCXS "src/server.cpp")
# file(GLOB NCXC "src/client.cpp")
# add_executable(ncxs ${NCXS} ${TCP} ${UTIL} ${LOG} ${PROTOCOL})
# add_executable(ncxc ${NCXC} ${TCP} ${UTIL} ${LOG} ${PROTOCOL})

# 链接ZeroMQ库到可执行文件
# target_link_libraries(exserv ${PC_ZEROMQ_LIBRARIES})
# target_link_libraries(musltest ${PC_ZEROMQ_LIBRARIES})
# target_link_libraries(ncxs ${PC_ZEROMQ_LIBRARIES})
# target_link_libraries(ncxc ${PC_ZEROMQ_LIBRARIES})