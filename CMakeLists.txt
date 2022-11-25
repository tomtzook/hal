cmake_minimum_required(VERSION 3.5.1)
project(hal C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(HAL_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(hal_base)
add_subdirectory(halpp)
add_subdirectory(natives)

if(NOT DEFINED HAL_INTERFACE)
    message(STATUS "Interface implementation not given")
else()
    message(STATUS "Interface implementation given, defining static library")

    string(REPLACE "-" "_" HAL_INTERFACE_TARGET ${HAL_INTERFACE})
    set(HAL_INTERFACE_TARGET hal_${HAL_INTERFACE_TARGET})

    add_library(hal
            STATIC
            $<TARGET_OBJECTS:hal_base>
            $<TARGET_OBJECTS:${HAL_INTERFACE_TARGET}>)
    target_include_directories(hal
            PRIVATE $<BUILD_INTERFACE:${HAL_NATIVE_INCLUDES}>
            PUBLIC $<BUILD_INTERFACE:${HAL_INCLUDES}> $<BUILD_INTERFACE:${HAL_IMPL_INCLUDES}>)

    add_library(halpp
            STATIC
            $<TARGET_OBJECTS:halpp_base>)
    target_include_directories(hal PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/halpp/include>)
    target_link_libraries(halpp hal)
endif()


add_executable(test test.cpp)
target_link_libraries(test halpp hal)
