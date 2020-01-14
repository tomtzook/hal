cmake_minimum_required(VERSION 3.5.1)
project(hal C)

set(CMAKE_C_STANDARD 11)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

set(BASE_INTERFACE
        base/hal_base.c
        base/hal_base_dio.c)

set(UTIL
        util/lookup_table.c)

add_library(hal
        STATIC
        ${BASE_INTERFACE}
        ${UTIL})