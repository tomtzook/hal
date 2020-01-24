cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)

set(CMAKE_C_STANDARD 11)

set(BBBIO_INCLUDE_DIR ../bbbio/include)

include_directories(${BBBIO_INCLUDE_DIR})

add_executable(button-example button.c)
target_link_libraries(button-example bbbio-static)

add_executable(led-example led.c)
target_link_libraries(led-example bbbio-static)