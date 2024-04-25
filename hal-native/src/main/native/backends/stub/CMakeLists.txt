cmake_minimum_required(VERSION 3.5.1)

set(HAL_IMPL_INCLUDES )

add_library(hal_stub OBJECT
        hal_backend.c)
target_include_directories(hal_stub
        PRIVATE ${HAL_NATIVE_INCLUDES} ${HAL_IMPL_INCLUDES} ${HAL_INCLUDES})

set(HAL_IMPL_INCLUDES ${HAL_IMPL_INCLUDES} PARENT_SCOPE)
