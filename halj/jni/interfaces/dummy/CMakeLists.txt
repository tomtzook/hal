cmake_minimum_required(VERSION 3.5.1)
project(hal_impl C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../../halc/include)
include_directories(${CMAKE_SOURCE_DIR}/../../halc/interface)

find_library(Hal libhal.a PATHS ${CMAKE_SOURCE_DIR}/../../halc/build/)

add_library(hal_impl
        STATIC
        dio_interface.c)

target_link_libraries(hal_impl ${Hal})
