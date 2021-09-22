cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_library(hal STATIC
        src/init.c
        src/ports/ports_dio.c
        src/ports/ports_internal.c)



add_executable(test test/main.c)
target_link_libraries(test hal)
