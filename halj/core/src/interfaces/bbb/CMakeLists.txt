cmake_minimum_required(VERSION 3.5.1)
project(hal_impl C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/bbbio/bbbio)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../halc/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../halc/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../../halc/interface)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/bbbio/bbbio/include)

add_library(hal_impl
        OBJECT
        dio_interface.c
        ${BBBIO_SOURCES})
