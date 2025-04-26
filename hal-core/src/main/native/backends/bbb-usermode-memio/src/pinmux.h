#pragma once

#include <hal_types.h>
#include <hal_error.h>

#include "base.h"


hal_error_t get_pinmux(const bbb_env_t* env, const pin_t* pin, unsigned* mode);
hal_error_t set_pinmux(const bbb_env_t* env, const pin_t* pin, unsigned mode);

hal_error_t set_pinmux_to_type(const bbb_env_t* env, const pin_t* pin, unsigned type);

hal_error_t get_pinmux_gpio_resistor(const bbb_env_t* env, const pin_t* pin, hal_dio_config_resistor_t* resistor);
hal_error_t set_pinmux_gpio_resistor(const bbb_env_t* env, const pin_t* pin, hal_dio_config_resistor_t resistor);
