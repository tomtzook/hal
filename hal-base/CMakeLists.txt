cmake_minimum_required(VERSION 3.5.1)

include_directories(include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../util)

add_library(hal STATIC
        src/init.c
        src/ports/ports_dio.c
        src/ports/ports_internal.c src/ports/ports_aio.c include/ports/ports_pwm.h src/ports/ports_pwm.c src/ports/ports_pwm.c src/error.c)
target_link_libraries(hal hal_util)

set(HAL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
set(HAL_LIBRARIES hal PARENT_SCOPE)
