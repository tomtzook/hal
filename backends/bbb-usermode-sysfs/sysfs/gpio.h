#pragma once

#include <hal_types.h>
#include <hal_error.h>
#include <hal_dio.h>


typedef enum {
    DIR_OUTPUT,
    DIR_INPUT
} direction_t;

typedef enum {
    EDGE_NONE,
    EDGE_RISING,
    EDGE_FALLING,
    EDGE_BOTH
} edge_t;

hal_error_t gpio_export_pin(unsigned number);
hal_error_t gpio_unexport_pin(unsigned number);

hal_error_t gpio_set_direction(unsigned number, direction_t direction);
hal_error_t gpio_set_edge(unsigned number, edge_t edge);
hal_error_t gpio_set_value(unsigned number, hal_dio_value_t value);

hal_error_t gpio_get_value(unsigned number, hal_dio_value_t* value);
