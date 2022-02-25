cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(${HAL_INCLUDES})

add_library(halpp STATIC
        src/halpp.cpp
        src/ports/ports_dio.cpp
        src/ports/ports_aio.cpp)
target_link_libraries(halpp hal)

set(HALPP_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/include ${HAL_INCLUDES} PARENT_SCOPE)
set(HALPP_LIBRARIES halpp ${HAL_LIBRARIES} PARENT_SCOPE)
