#pragma once

#include <hal_types.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_prop_key_t;
typedef uint32_t hal_prop_value_t;

typedef enum {
    HAL_CONFIG_GPIO_POLL_EDGE = 1,
    HAL_CONFIG_GPIO_RESISTOR = 2
} hal_config_keys_t;

typedef enum {
    HAL_GPIO_CONFIG_EDGE_NONE,
    HAL_GPIO_CONFIG_EDGE_RISING,
    HAL_GPIO_CONFIG_EDGE_FALLING,
    HAL_GPIO_CONFIG_EDGE_BOTH
} hal_gpio_config_poll_edge_t;

typedef enum {
    HAL_GPIO_CONFIG_RESISTOR_NONE,
    HAL_GPIO_CONFIG_RESISTOR_PULLUP,
    HAL_GPIO_CONFIG_RESISTOR_PULLDOWN
} hal_gpio_config_resistor_t;


hal_error_t hal_set_port_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, hal_prop_value_t value);

#ifdef __cplusplus
}
#endif
