cmake_minimum_required(VERSION 3.5.1)

add_library(halpp_base OBJECT
        include/halpp.h
        include/halpp_except.h

        src/empty.cpp
        src/except.cpp
        src/hal.cpp)
target_include_directories(halpp_base PUBLIC ${HAL_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/include)
