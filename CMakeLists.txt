cmake_minimum_required(VERSION 3.5.1)
project(hal C CXX)

add_subdirectory(util)
add_subdirectory(hal-base)
add_subdirectory(halpp)
add_subdirectory(natives)

add_executable(test test/main.c)
target_include_directories(test PRIVATE ${HAL_INCLUDES})
target_link_libraries(test ${HAL_LIBRARIES} hal_stub)

add_executable(testpp test/testpp.cpp)
target_include_directories(testpp PRIVATE ${HALPP_INCLUDES})
target_link_libraries(testpp ${HALPP_LIBRARIES} hal_stub)
