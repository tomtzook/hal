cmake_minimum_required(VERSION 3.5.1)


set(HAL_NATIVE_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(hal_base OBJECT
        include/hal_internal.h
        include/hal_backend.h
        include/list.h

        src/hal.c
        src/hal_error.c
        src/list.c
        src/dio.c
        include/hal_error_handling.h src/aio.c)
target_include_directories(hal_base
        PUBLIC ${HAL_INCLUDES}
        PRIVATE ${HAL_NATIVE_INCLUDES}
        )
target_link_libraries(hal_base PRIVATE Threads::Threads)

set(HAL_NATIVE_INCLUDES ${HAL_NATIVE_INCLUDES} PARENT_SCOPE)
