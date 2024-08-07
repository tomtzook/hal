#pragma once

#include <hal_types.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_prop_key_t;

typedef enum {
    HAL_CONFIG_DIO_POLL_EDGE = 1, // for DIO ports, hal_dio_config_poll_edge_t
    HAL_CONFIG_DIO_RESISTOR = 2, // for DIO ports, hal_dio_config_resistor_t
    HAL_CONFIG_ANALOG_MAX_VALUE = 3, // in ADC units, for AIO ports
    HAL_CONFIG_ANALOG_MAX_VOLTAGE = 4, // in milli-volts (integer), for AIO ports
    HAL_CONFIG_ANALOG_SAMPLE_RATE = 5, // in microsecond period (integer), for AIO ports
    HAL_CONFIG_PWM_FREQUENCY = 6, // in microsecond period (integer), for PWM ports

    HAL_CONFIG_KEY_MAX = 32
} hal_config_keys_t;

typedef enum {
    HAL_CONFIG_FLAG_READABLE = (1 << 0),
    HAL_CONFIG_FLAG_WRITABLE = (1 << 1)
} hal_config_flags_t;

// only for DIO PORTS, for HAL_CONFIG_DIO_POLL_EDGE
typedef enum {
    HAL_CONFIG_DIO_EDGE_NONE,
    HAL_CONFIG_DIO_EDGE_RISING,
    HAL_CONFIG_DIO_EDGE_FALLING,
    HAL_CONFIG_DIO_EDGE_BOTH
} hal_dio_config_poll_edge_t;

// only for DIO ports, for HAL_CONFIG_DIO_RESISTOR
typedef enum {
    HAL_CONFIG_DIO_RESISTOR_NONE,
    HAL_CONFIG_DIO_RESISTOR_PULLUP,
    HAL_CONFIG_DIO_RESISTOR_PULLDOWN
} hal_dio_config_resistor_t;

hal_error_t hal_port_property_probe(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t* flags);

hal_error_t hal_port_get_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t* value);
hal_error_t hal_port_set_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t value);

#ifdef __cplusplus
}
#endif
