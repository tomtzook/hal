cmake_minimum_required(VERSION 3.12)
project(hal C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(util)
include_directories(logging)
include_directories(interface)

set(BASE_INTERFACE
        base/hal_base.c
        base/hal_base_dio.c)

set(UTIL
        util/lookup_table.c)

add_library(hal
        ${BASE_INTERFACE}
        ${UTIL})