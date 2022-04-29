cmake_minimum_required(VERSION 3.5.1)
project(hal C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(util)
add_subdirectory(hal-base)
add_subdirectory(halpp)
add_subdirectory(natives)

add_executable(test test/main.c)
target_include_directories(test PRIVATE ${HAL_INCLUDES})
target_link_libraries(test ${HAL_LIBRARIES} hal_stub)

add_executable(testbbb test/main.c)
target_include_directories(testbbb PRIVATE ${HAL_INCLUDES})
target_link_libraries(testbbb ${HAL_LIBRARIES} hal_bbb_usermode_memory)

add_executable(testpp test/testpp.cpp)
target_include_directories(testpp PRIVATE ${HALPP_INCLUDES})
target_link_libraries(testpp ${HALPP_LIBRARIES} hal_stub)
