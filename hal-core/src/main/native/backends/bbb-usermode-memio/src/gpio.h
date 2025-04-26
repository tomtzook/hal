#pragma once

#include <hal_types.h>
#include <hal_error.h>
#include <hal_dio.h>

#include "pins.h"
#include "base.h"


typedef enum {
    DIR_OUTPUT,
    DIR_INPUT
} direction_t;

// todo: debouncing

hal_error_t gpio_get_edge(const bbb_env_t* env, const pin_t* pin, hal_dio_config_poll_edge_t* edge);
hal_error_t gpio_get_value(const bbb_env_t* env, const pin_t* pin, direction_t dir, hal_dio_value_t* value);

hal_error_t gpio_set_direction(const bbb_env_t* env, const pin_t* pin, direction_t direction);
hal_error_t gpio_set_edge(const bbb_env_t* env, const pin_t* pin, hal_dio_config_poll_edge_t edge);
hal_error_t gpio_set_value(const bbb_env_t* env, const pin_t* pin, hal_dio_value_t value);
