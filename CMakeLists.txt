cmake_minimum_required (VERSION 3.8)

project(boxer CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(boxer 
src/main.h
src/main.cpp
src/parser.h
src/parser.cpp)

