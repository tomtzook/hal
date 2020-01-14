cmake_minimum_required(VERSION 3.5.1)
project(hal C)

set(CMAKE_C_STANDARD 11)

set(BASE_INTERFACE
        base/hal_base.c
        base/hal_base_dio.c)

set(UTIL
        util/lookup_table.c)

add_library(hal
        OBJECT
        ${BASE_INTERFACE}
        ${UTIL})

target_include_directories(hal PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)