cmake_minimum_required(VERSION 3.5.1)

FIND_PACKAGE(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../halc/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../halc/)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../halc ${CMAKE_CURRENT_BINARY_DIR}/halc)

if(DEFINED HAL_INTERFACE)
    message(STATUS "Build interfaces ${HAL_INTERFACE}")

    foreach(INT ${HAL_INTERFACE})
        add_library(haljni_${INT}
                SHARED
                main/jni/hal.c
                main/jni/dio.c
                main/jni/aio.c)

        set_property(TARGET haljni_${INT} PROPERTY POSITION_INDEPENDENT_CODE ON)
        target_link_libraries(haljni_${INT} hal_${INT})
    endforeach()
else()
    message(ERROR "No hal interface specified")
endif()
