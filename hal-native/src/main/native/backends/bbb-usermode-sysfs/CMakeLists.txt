cmake_minimum_required(VERSION 3.5.1)

set(HAL_IMPL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

add_library(hal_bbb_usermode_sysfs OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/../bbb-includes/bbb_gpiodef.h
        sysfs/gpio.c
        sysfs/gpio.h
        hal_backend.c
        )
target_include_directories(hal_bbb_usermode_sysfs
        PUBLIC ../bbb-includes ${HAL_INCLUDES}
        PRIVATE ${HAL_NATIVE_INCLUDES})