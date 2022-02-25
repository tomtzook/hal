cmake_minimum_required(VERSION 3.5.1)
project(hal C)

add_subdirectory(util)
add_subdirectory(hal-base)
add_subdirectory(natives)

include_directories(${HAL_INCLUDES})
add_executable(test test/main.c)
target_link_libraries(test ${HAL_LIBRARIES} hal_stub)
