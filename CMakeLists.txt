cmake_minimum_required(VERSION 3.12)
project(hal C)

set(CMAKE_C_STANDARD 11)

set(BASE_INTERFACE
        include/hal.h
        include/hal_dio.h
        include/hal_types.h)

add_library(hal
        ${BASE_INTERFACE})