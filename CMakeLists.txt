cmake_minimum_required(VERSION 3.12)
project(hal C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(util)
include_directories(logging)

set(BASE_INTERFACE
        include/hal.h
        include/hal_dio.h
        include/hal_types.h)

set(UTIL
        util/lookup_table.c)

add_library(hal
        ${BASE_INTERFACE}
        ${UTIL})