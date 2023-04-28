cmake_minimum_required(VERSION 3.5.1)


set(HAL_NATIVE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(hal_base OBJECT
        include/hal_internal.h
        include/hal_backend.h
        include/list.h

        src/hal.c
        src/hal_error.c
        src/list.c
        src/dio.c
        )
target_include_directories(hal_base
        PUBLIC ${HAL_INCLUDES}
        PRIVATE ${HAL_NATIVE_INCLUDES}
        )

set(HAL_NATIVE_INCLUDES ${HAL_NATIVE_INCLUDES} PARENT_SCOPE)
