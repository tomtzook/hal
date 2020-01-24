#ifndef BBBIO_GPIOS_H
#define BBBIO_GPIOS_H

#include <_G_config.h>

#include <bbbio_consts.h>

extern const __off_t bbbio_gpio_addr_offset[BBBIO_GPIO_MODULES_COUNT];

extern const signed char bbbio_module_mapping[BBBIO_GPIO_HEADERS_COUNT][BBBIO_GPIO_HEADER_PIN_COUNT];
extern const unsigned char bbbio_pin_mapping[BBBIO_GPIO_HEADERS_COUNT][BBBIO_GPIO_HEADER_PIN_COUNT];

#endif //BBBIO_GPIOS_H
