#pragma once

#include <hal_types.h>
#include <hal_error.h>
#include <hal_dio.h>

#include "pins.h"


typedef enum {
    DIR_OUTPUT,
    DIR_INPUT
} direction_t;

hal_error_t set_pin_mode(const pin_t* pin, const char* mode);
hal_error_t get_pin_mode(const pin_t* pin, char* buffer);

hal_error_t gpio_export_pin(const pin_t* pin);
hal_error_t gpio_unexport_pin(const pin_t* pin);
int gpio_is_exported_pin(const pin_t* pin);

hal_error_t gpio_set_pinmux(const pin_t* pin, hal_dio_config_resistor_t resistor);
hal_error_t gpio_set_direction(const pin_t* pin, direction_t direction);
hal_error_t gpio_set_edge(const pin_t* pin, hal_dio_config_poll_edge_t edge);
hal_error_t gpio_set_value(const pin_t* pin, hal_dio_value_t value);

hal_error_t gpio_get_pinmux(const pin_t* pin, hal_dio_config_resistor_t* resistor);
hal_error_t gpio_get_edge(const pin_t* pin, hal_dio_config_poll_edge_t* edge);
hal_error_t gpio_get_value(const pin_t* pin, hal_dio_value_t* value);
