cmake_minimum_required(VERSION 3.10)
project(hal C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(native)
