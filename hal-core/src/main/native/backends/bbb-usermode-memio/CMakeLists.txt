cmake_minimum_required(VERSION 3.10)

set(HAL_IMPL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(hal_bbb_usermode_memio OBJECT
        hal_backend.c
        src/pins.c
        src/peripheral.h
        src/peripheral.c
        src/gpio.h
        src/memdefs.h
        src/base.h
        src/gpio.c
        src/base.c)
target_include_directories(hal_bbb_usermode_memio
        PRIVATE ${HAL_NATIVE_INCLUDES} ${HAL_IMPL_INCLUDES} ${HAL_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HAL_IMPL_INCLUDES ${HAL_IMPL_INCLUDES} PARENT_SCOPE)
