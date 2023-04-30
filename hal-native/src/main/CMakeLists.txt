cmake_minimum_required(VERSION 3.5.1)
project(hal C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(native)
add_subdirectory(jnikit)

add_library(hal_jni SHARED
        jni/hal_jni.cpp
        jni/hal_except.h
        jni/hal_except.cpp
        jni/hal_dio_jni.cpp)
target_link_libraries(hal_jni hal jnikit)
target_include_directories(hal_jni PRIVATE ${JNI_INCLUDE_DIRS} ${HAL_INCLUDES})
