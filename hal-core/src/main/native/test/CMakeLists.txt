

add_executable(hal_test main.c
        bbb_platform.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../backends/bbb-usermode-sysfs/sysfs/pins.c
        platforms.h)
target_include_directories(hal_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../backends/bbb-usermode-sysfs/include)
target_link_libraries(hal_test hal)
