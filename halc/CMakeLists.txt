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

if(DEFINED HAL_INTERFACE)
    message(STATUS "Interface implementations given ${HAL_INTERFACE}")

    foreach(INT ${HAL_INTERFACE})
        message(STATUS "Interface ${INT}")

        add_subdirectory(interfaces/${INT})
        add_library(hal_${INT}
                STATIC
                ${BASE_INTERFACE}
                ${UTIL})
        target_link_libraries(hal_${INT} hal_impl_${INT})

        target_include_directories(hal_${INT} PRIVATE
                $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
                $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
    endforeach()
else()
    message(STATUS "Interface implementation not given, defining object")

    add_library(hal
            OBJECT
            ${BASE_INTERFACE}
            ${UTIL})

    target_include_directories(hal PRIVATE
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
endif()
