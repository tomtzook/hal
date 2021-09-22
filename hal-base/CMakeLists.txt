cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_library(hal STATIC
        src/init.c
        src/ports/ports_dio.c
        src/ports/ports_internal.c)


set(HAL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set(HAL_LIBRARIES hal PARENT_SCOPE)
