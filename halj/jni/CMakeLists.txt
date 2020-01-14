cmake_minimum_required(VERSION 3.5.1)

FIND_PACKAGE(JNI REQUIRED)

include_directories(${JNI_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/libs/include)

find_library(Mad360 Mad360.lib PATHS ${CMAKE_SOURCE_DIR}/libs)

add_library(mcl
        SHARED
        src/main/c/mcljni.c
)

target_link_libraries(mcl ${Mad360})
