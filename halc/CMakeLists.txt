cmake_minimum_required(VERSION 3.5.1)
project(hal C)

set(CMAKE_C_STANDARD 11)

set(BASE_INTERFACE
        base/hal_base.c
        base/port_table.c
        base/hal_base_dio.c
        base/hal_base_aio.c)

set(UTIL
        util/lookup_table.c)

if(NOT DEFINED HAL_INTERFACE)
    message(STATUS "Interface implementation not given, defining object")

    add_library(hal
            OBJECT
            ${BASE_INTERFACE}
            ${UTIL})
else()
    message(STATUS "Interface implementation given, defining static")

    add_subdirectory(${HAL_INTERFACE})
    add_library(hal
            STATIC
            ${BASE_INTERFACE}
            ${UTIL}
            $<TARGET_OBJECTS:hal_impl>)
endif()

target_include_directories(hal PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)