cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxs VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# module 
include_directories(
    epoll
    error
    net_util
    server
    socket
    util
)

aux_source_directory(epoll EPOLL)
aux_source_directory(error ERROR)
aux_source_directory(net_util NET_UTIL)
aux_source_directory(server SERVER)
aux_source_directory(socket SOCKET)
aux_source_directory(util, UTIL)

# bin
file(GLOB SERV "serv.cpp")
file(GLOB CLIENT "cli.cpp")
add_executable(ncxs ${SERV} ${EPOLL} ${ERROR} ${NET_UTIL} ${SERVER} ${SOCKET} ${UTIL})
add_executable(ncxc ${CLIENT} ${NET_UTIL} ${ERROR} ${SOCKET} ${UTIL})
