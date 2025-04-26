#pragma once

#include <hal_error.h>

#include "pins.h"
#include "peripheral.h"



typedef struct _bbb_env {
    peripheral_t control_module;
    peripheral_t gpio_peripherals[4];
} bbb_env_t;

// todo: rename
hal_error_t initialize_backend(bbb_env_t** env);
void free_backend(bbb_env_t* env);

hal_error_t get_pinmux(const bbb_env_t* env, const pin_t* pin, unsigned* mode);
hal_error_t set_pinmux(const bbb_env_t* env, const pin_t* pin, unsigned mode);
