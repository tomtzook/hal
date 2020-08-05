cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(BBBIO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(BBBIO_HEADERS
        include/bbbio.h
        include/bbbio_gpio.h
        include/bbbio_pwmss.h
        include/bbbio_debug.h
        include/bbbio_rc.h
        include/bbbio_consts.h
        src/gpio/bbbio_gpio_consts.h
        src/pwmss/bbbio_pwmss_consts.h
        src/pwmss/bbbio_pwmss_ctrl.h
        src/bbbio_util.h
        src/hw.h)

set(BBBIO_SOURCES
        src/bbbio.c
        src/gpio/bbbio_gpio.c
        src/pwmss/bbbio_pwmss.c
        src/gpio/bbbio_gpio_consts.c
        src/pwmss/bbbio_pwmss_consts.c
        src/pwmss/bbbio_pwmss_ctrl.c
        )

add_library(bbbio
        SHARED
        ${BBBIO_HEADERS}
        ${BBBIO_SOURCES})

add_library(bbbio-static
        STATIC
        ${BBBIO_HEADERS}
        ${BBBIO_SOURCES})
        
add_library(bbbio-obj
        OBJECT
        ${BBBIO_HEADERS}
        ${BBBIO_SOURCES})
