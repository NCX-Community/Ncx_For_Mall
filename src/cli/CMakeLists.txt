cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(ncxc VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件目录
add_subdirectory(error)
add_subdirectory(net_util)
file(GLOB SOURCES "cli.cpp")
add_executable(ncxc ${SOURCES})
target_link_libraries(ncxc error)
target_link_libraries(ncxc net_util)