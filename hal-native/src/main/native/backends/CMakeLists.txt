cmake_minimum_required(VERSION 3.5.1)


if(NOT DEFINED HAL_INTERFACE)
    message(STATUS "Interface implementation not given, adding all")
    add_subdirectory(bbb-usermode-memory)
    add_subdirectory(bbb-usermode-sysfs)
else()
    message(STATUS "Interface implementation given, adding it")

    add_subdirectory(${HAL_INTERFACE})
    set(HAL_IMPL_INCLUDES ${HAL_IMPL_INCLUDES} PARENT_SCOPE)
endif()
