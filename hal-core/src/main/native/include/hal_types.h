#pragma once

#include <stdint.h>

// ReSharper disable CppUnusedIncludeDirective
#include <stddef.h>
// ReSharper restore CppUnusedIncludeDirective

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_id_t;
typedef uint32_t hal_handle_t;

#define HAL_INVALID_IDENTIFIER ((hal_id_t)-1)
#define HAL_EMPTY_HANDLE ((hal_handle_t)-1)

#define HAL_OPT_UINT32 ((uint32_t)-1)
#define HAL_OPT_UINT64 ((uint64_t)-1)

#define HAL_CONFIG_KEY_MAX_COUNT 32

typedef enum {
    HAL_TYPE_DIGITAL_INPUT = (0x1 << 0),
    HAL_TYPE_DIGITAL_OUTPUT = (0x1 << 1),
    HAL_TYPE_ANALOG_INPUT = (0x1 << 2),
    HAL_TYPE_ANALOG_OUTPUT = (0x1 << 3),
    HAL_TYPE_PWM_OUTPUT = (0x1 << 4),
    HAL_TYPE_QUADRATURE = (0x1 << 5)
} hal_port_type_t;

typedef enum {
    HAL_FLAG_OPEN = (0x1 << 0),
    HAL_FLAG_BLOCKED = (0x1 << 1)
} hal_port_flag_t;

typedef struct {
    hal_id_t identifier;
    uint32_t supported_types;
    uint64_t supported_props;
    uint32_t flags;

    const char* name;

    hal_handle_t open_handle;
} hal_port_info_t;

typedef struct {
    hal_id_t identifier;
} hal_port_iter_t;

typedef uint32_t hal_prop_key_t;

typedef enum {
    HAL_CONFIG_DIO_POLL_EDGE = (0x1 << 0), // for DIO ports, hal_dio_config_poll_edge_t
    HAL_CONFIG_DIO_RESISTOR = (0x1 << 1), // for DIO ports, hal_dio_config_resistor_t
    HAL_CONFIG_ANALOG_MAX_VALUE = (0x1 << 2), // in ADC units, for AIO ports
    HAL_CONFIG_ANALOG_MAX_VOLTAGE = (0x1 << 3), // in milli-volts (integer), for AIO ports
    HAL_CONFIG_ANALOG_SAMPLE_RATE = (0x1 << 4), // in microsecond period (integer), for AIO ports
    HAL_CONFIG_PWM_FREQUENCY = (0x1 << 5), // in microsecond period (integer), for PWM ports
} hal_config_known_key_t;

typedef enum {
    HAL_CONFIG_FLAG_READABLE = (1 << 0),
    HAL_CONFIG_FLAG_WRITABLE = (1 << 1)
} hal_config_flag_t;

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

typedef struct hal__backend hal_backend_t;
typedef struct hal__env hal_env_t;

const char* hal_port_type_str(hal_port_type_t type);
const char* hal_port_flag_str(hal_port_flag_t flag);
const char* hal_prop_key_str(hal_prop_key_t key);
const char* hal_prop_flag_str(uint32_t flag);

#ifdef __cplusplus
}
#endif
