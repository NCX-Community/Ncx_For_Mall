cmake_minimum_required(VERSION 3.10)

# 设置项目名称和版本
project(net_util VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(net_util STATIC endpoint.cpp)