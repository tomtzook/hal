cmake_minimum_required(VERSION 3.5.1)

include_directories(${HAL_INCLUDES})

add_library(hal_bbb_usermode_memory STATIC
        include/bbb_gpiodef.h
        memio/peripheral.h
        memio/regs.h
        ports.c
        memio/peripheral.c)
target_include_directories(hal_bbb_usermode_memory PUBLIC include)
target_link_libraries(hal_bbb_usermode_memory ${HAL_LIBRARIES})
