cmake_minimum_required(VERSION 3.5.1)

set(HAL_IMPL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(hal_bbb_usermode_sysfs OBJECT
        hal_backend.c
        sysfs/gpio.c
        sysfs/gpio.h
        sysfs/common.h
        sysfs/common.c
        sysfs/adc.h
        sysfs/adc.c
        sysfs/pins.c
        sysfs/pwm.h
        sysfs/pwm.c)
target_include_directories(hal_bbb_usermode_sysfs
        PRIVATE ${HAL_NATIVE_INCLUDES} ${HAL_IMPL_INCLUDES} ${HAL_INCLUDES})

set(HAL_IMPL_INCLUDES ${HAL_IMPL_INCLUDES} PARENT_SCOPE)
