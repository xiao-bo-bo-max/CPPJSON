cmake_minimum_required(VERSION 3.26)
project(CPPJSON)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPJSON main.cpp
        cppJSON.cpp
        cppJSON.h)
