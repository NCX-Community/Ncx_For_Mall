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
)

aux_source_directory(tcp TCP)
aux_source_directory(util UTIL)
aux_source_directory(log LOG)

# test for echo
file(GLOB ECHOSERV "example/echo/serv.cpp")
file(GLOB ECHOCLIENT "example/echo/cli.cpp")
add_executable(echoserv ${ECHOSERV} ${TCP} ${UTIL} ${LOG})
add_executable(echocli ${ECHOCLIENT} ${TCP} ${UTIL} ${LOG})

# test_for_ex
# file(GLOB EXSERV "example/ex/test_ex_serv.cpp")
# file(GLOB EXCLIENT "example/ex/test_ex_cli.cpp")
# add_executable(exserv ${EXSERV} ${TCP} ${UTIL} ${LOG})
# add_executable(excli ${EXCLIENT} ${TCP} ${UTIL} ${LOG})




