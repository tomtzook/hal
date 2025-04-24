cmake_minimum_required(VERSION 3.10)


if(NOT DEFINED HAL_INTERFACE)
    message(STATUS "Interface implementation not given, adding all")
    add_subdirectory(bbb-usermode-sysfs)
    add_subdirectory(bbb-usermode-memio)
    add_subdirectory(sim)
else()
    message(STATUS "Interface implementation given, adding it")

    add_subdirectory(${HAL_INTERFACE})
    set(HAL_IMPL_INCLUDES ${HAL_IMPL_INCLUDES} PARENT_SCOPE)
endif()
