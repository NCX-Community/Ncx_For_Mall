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
aux_source_directory(util, UTIL)
aux_source_directory(log, LOG)


# bin
file(GLOB SERV "example/serv.cpp")
file(GLOB CLIENT "example/cli.cpp")
add_executable(ncxs ${SERV} ${TCP} ${UTIL} ${LOG})
add_executable(ncxc ${CLIENT} ${TCP} ${UTIL} ${LOG})
