cmake_minimum_required(VERSION 3.5.1)

FIND_PACKAGE(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../halc/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../halc/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../../halc ${CMAKE_CURRENT_BINARY_DIR}/halc)
add_subdirectory(${HAL_INTERFACE})

add_library(haljni
        SHARED
        main/jni/hal.c
        main/jni/dio.c
        $<TARGET_OBJECTS:hal_impl>
        $<TARGET_OBJECTS:hal>)
