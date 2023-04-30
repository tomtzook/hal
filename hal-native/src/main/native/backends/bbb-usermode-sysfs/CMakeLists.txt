cmake_minimum_required(VERSION 3.5.1)

set(HAL_IMPL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../bbb-includes)

add_library(hal_bbb_usermode_sysfs OBJECT
        sysfs/gpio.c
        sysfs/gpio.h
        hal_backend.c
        )
target_include_directories(hal_bbb_usermode_sysfs
        PRIVATE ${HAL_NATIVE_INCLUDES} ${HAL_IMPL_INCLUDES} ${HAL_INCLUDES})

set(HAL_IMPL_INCLUDES ${HAL_IMPL_INCLUDES} PARENT_SCOPE)
