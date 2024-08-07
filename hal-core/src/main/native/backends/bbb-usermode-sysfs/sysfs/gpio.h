#pragma once

#include <hal_types.h>
#include <hal_error.h>
#include <hal_dio.h>
#include <hal_config.h>

#include "pins.h"


typedef enum {
    DIR_OUTPUT,
    DIR_INPUT
} direction_t;

hal_error_t gpio_export_pin(pin_t* pin);
hal_error_t gpio_unexport_pin(pin_t* pin);
int gpio_is_exported_pin(pin_t* pin);

hal_error_t gpio_set_pinmux(pin_t* pin, hal_dio_config_resistor_t resistor);
hal_error_t gpio_set_direction(pin_t* pin, direction_t direction);
hal_error_t gpio_set_edge(pin_t* pin, hal_dio_config_poll_edge_t edge);
hal_error_t gpio_set_value(pin_t* pin, hal_dio_value_t value);

hal_error_t gpio_get_pinmux(pin_t* pin, hal_dio_config_resistor_t* resistor);
hal_error_t gpio_get_edge(pin_t* pin, hal_dio_config_poll_edge_t* edge);
hal_error_t gpio_get_value(pin_t* pin, hal_dio_value_t* value);
