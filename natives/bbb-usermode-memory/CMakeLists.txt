cmake_minimum_required(VERSION 3.5.1)

include_directories(${HAL_INCLUDES})

add_library(hal_bbb_usermode_memory STATIC
        memio/gpiodef.h
        memio/peripheral.h
        ports.c memio/peripheral.c memio/regs.h)
target_link_libraries(hal_bbb_usermode_memory ${HAL_LIBRARIES})
