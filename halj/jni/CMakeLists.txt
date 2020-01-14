cmake_minimum_required(VERSION 3.5.1)

add_subdirectory(${HAL_INTERFACE})

FIND_PACKAGE(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/../../halc/include)

add_library(haljni
        SHARED
        src/main/c/hal.c
        src/main/c/dio.c
)

target_link_libraries(haljni hal_impl)
