cmake_minimum_required(VERSION 3.5)
project(rssa-simulator)
set(CMAKE_CXX_STANDARD 11)

add_executable(rssa-simulator src/main.cpp src/network.cpp)