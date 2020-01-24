#ifndef BBBIO_GPIO_H
#define BBBIO_GPIO_H

#include <bbbio.h>

#include "bbbio_rc.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum bbbio_gpio_header {
    BBB_P8_HEADER = 0,
    BBB_P9_HEADER = 1
} bbbio_gpio_header_t;

typedef enum bbbio_gpio_value {
    BBB_GPIO_LOW = 0,
    BBB_GPIO_HIGH = 1
} bbbio_gpio_value_t;

typedef enum bbbio_gpio_dir {
    BBB_DIR_INPUT = 0,
    BBB_DIR_OUTPUT = 1
} bbbio_gpio_dir_t;

typedef char bbbio_gpio_pin_t;

bbbio_rc_t bbbio_gpio_setdir(bbbio_t* bbbio, bbbio_gpio_header_t header, bbbio_gpio_pin_t pin, bbbio_gpio_dir_t dir);

bbbio_rc_t bbbio_gpio_set(bbbio_t* bbbio, bbbio_gpio_header_t header, bbbio_gpio_pin_t pin, bbbio_gpio_value_t value);
bbbio_rc_t bbbio_gpio_high(bbbio_t* bbbio, bbbio_gpio_header_t header, bbbio_gpio_pin_t pin);
bbbio_rc_t bbbio_gpio_low(bbbio_t* bbbio, bbbio_gpio_header_t header, bbbio_gpio_pin_t pin);

bbbio_rc_t bbbio_gpio_get(bbbio_t* bbbio, bbbio_gpio_header_t header, bbbio_gpio_pin_t pin, bbbio_gpio_value_t *value);

#ifdef __cplusplus
}
#endif

#endif //BBBIO_GPIO_H
