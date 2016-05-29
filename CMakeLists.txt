cmake_minimum_required(VERSION 3.5)
project(iron)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES lexer.cpp parser.cpp)
add_executable(iron ${SOURCE_FILES})