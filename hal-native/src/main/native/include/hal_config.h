#pragma once

#include <hal_types.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_prop_key_t;

typedef enum {
    HAL_CONFIG_GPIO_POLL_EDGE = 1, // for DIO ports
    HAL_CONFIG_GPIO_RESISTOR = 2, // for DIO ports
    HAL_CONFIG_ANALOG_MAX_VALUE = 3, // in ADC units, for AIO ports
    HAL_CONFIG_ANALOG_MAX_VOLTAGE = 4, // in volts (float), for AIO ports
    HAL_CONFIG_ANALOG_SAMPLE_RATE = 5, // in HZ (float), for AIO ports
    HAL_CONFIG_PWM_FREQUENCY = 6 // in HZ (float), for PWM ports
} hal_config_keys_t;

// only for DIO PORTS, for HAL_CONFIG_GPIO_POLL_EDGE
typedef enum {
    HAL_GPIO_CONFIG_EDGE_NONE,
    HAL_GPIO_CONFIG_EDGE_RISING,
    HAL_GPIO_CONFIG_EDGE_FALLING,
    HAL_GPIO_CONFIG_EDGE_BOTH
} hal_gpio_config_poll_edge_t;

// only for DIO ports, for HAL_CONFIG_GPIO_RESISTOR
typedef enum {
    HAL_GPIO_CONFIG_RESISTOR_NONE,
    HAL_GPIO_CONFIG_RESISTOR_PULLUP,
    HAL_GPIO_CONFIG_RESISTOR_PULLDOWN
} hal_gpio_config_resistor_t;


hal_error_t hal_get_port_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, unsigned* value);
hal_error_t hal_get_port_property_f(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, float* value);

hal_error_t hal_set_port_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, unsigned value);
hal_error_t hal_set_port_property_f(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, float value);

#ifdef __cplusplus
}
#endif
