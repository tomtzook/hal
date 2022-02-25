cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_STANDARD 11)

include_directories(${HAL_INCLUDES})

add_library(hal_stub STATIC
        ports.c)
target_link_libraries(hal_stub ${HAL_LIBRARIES})
