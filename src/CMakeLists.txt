cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxs VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# lib
add_subdirectory(error)
add_subdirectory(net_util)
add_subdirectory(socket)
add_subdirectory(epoll)

# bin
file(GLOB SERVER "serv.cpp")
file(GLOB CLIENT "cli.cpp")
add_executable(ncxs ${SERVER})
add_executable(ncxc ${CLIENT})
target_link_libraries(ncxs error)
target_link_libraries(ncxs net_util)
