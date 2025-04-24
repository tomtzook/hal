#pragma once

#include <hal_error.h>

#include "peripheral.h"



typedef struct _bbb_env {
    peripheral_t control_module;
    peripheral_t gpio_peripherals[4];
} bbb_env_t;

// todo: rename
hal_error_t initialize_backend(bbb_env_t** env);
void free_backend(bbb_env_t* env);
