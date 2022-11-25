cmake_minimum_required(VERSION 3.5.1)

set(HAL_IMPL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

add_library(hal_bbb_usermode_memory OBJECT
        include/bbb_gpiodef.h
        memio/peripheral.h
        memio/regs.h

        hal_native.c
        memio/peripheral.c
        )
target_include_directories(hal_bbb_usermode_memory
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${HAL_INCLUDES}
        PRIVATE ${HAL_NATIVE_INCLUDES})
