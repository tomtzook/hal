cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_STANDARD 11)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../hal-base/include)
include_directories(${HAL_INCLUDES})

add_library(hal_stub STATIC
        hal.c
        ports.c)
target_link_libraries(hal_stub ${HAL_LIBRARIES})
