cmake_minimum_required(VERSION 3.5.1)
project(hal_impl C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../../halc/include)
include_directories(${CMAKE_SOURCE_DIR}/../../halc/interface)

add_library(hal_impl
        OBJECT
        dio_interface.c)
