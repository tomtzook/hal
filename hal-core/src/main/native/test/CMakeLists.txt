
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


add_executable(hal_test main.c
        bbb_platform.c
        ${CMAKE_CURRENT_SOURCE_DIR}/../backends/bbb-usermode-sysfs/sysfs/pins.c
        platforms.h)
target_include_directories(hal_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../backends/bbb-usermode-sysfs/include)
target_link_libraries(hal_test hal)


add_executable(hal_test_bbb test_bbb.c)
target_link_libraries(hal_test_bbb hal Threads::Threads)
