#pragma once

#include "hal_types.h"
#include "hal_error.h"


#ifdef __cplusplus
extern "C" {
#endif

#define HAL_PORT_PROBE_EXISTS 0x1
#define HAL_PORT_PROBE_INPUT 0x2
#define HAL_PORT_PROBE_OUTPUT 0x4
#define HAL_PORT_PROBE_DIGITAL 0x8
#define HAL_PORT_PROBE_ANALOG 0x16
#define HAL_PORT_PROBE_PWM 0x32

typedef enum _hal_dio_value {
    HAL_DIO_LOW,
    HAL_DIO_HIGH
} hal_dio_value_t;

typedef uint16_t hal_aio_value_t;

typedef uint16_t hal_pwm_value_t;

typedef enum _hal_port_dir {
    PORT_DIR_OUTPUT,
    PORT_DIR_INPUT
} hal_port_dir_t;

typedef uint32_t hal_port_t;

typedef struct _dio_port {
    hal_port_t port;
    hal_port_dir_t dir;
    hal_dio_value_t value;

    void* native_data;
} dio_port_t;

typedef struct _aio_port {
    hal_port_t port;
    hal_port_dir_t dir;
    hal_aio_value_t value;

    void* native_data;
} aio_port_t;

typedef struct _pwm_port { // only output
    hal_port_t port;
    hal_pwm_value_t value;

    void* native_data;
} pwm_port_t;

typedef struct _ports_native_interface {
    hal_error_t (*init)(hal_env_t* env, void** data);
    void (*free)(hal_env_t* env, void* data);
    hal_error_t (*probe)(hal_env_t* env, void* data, hal_port_t port, uint8_t flags);

    hal_error_t (*dio_init)(hal_env_t* env, void* data, dio_port_t* port);
    void (*dio_free)(hal_env_t* env, void* data, dio_port_t* port);
    hal_error_t (*dio_write)(hal_env_t* env, void* data, dio_port_t* port, hal_dio_value_t value);
    hal_error_t (*dio_read)(hal_env_t* env, void* data, dio_port_t* port, hal_dio_value_t* value);

    hal_error_t (*aio_init)(hal_env_t* env, void* data, aio_port_t* port);
    void (*aio_free)(hal_env_t* env, void* data, aio_port_t* port);
    hal_error_t (*aio_write)(hal_env_t* env, void* data, aio_port_t* port, hal_aio_value_t value);
    hal_error_t (*aio_read)(hal_env_t* env, void* data, aio_port_t* port, hal_aio_value_t* value);
    hal_aio_value_t (*aio_max_value)(hal_env_t* env, void* data);

    hal_error_t (*pwm_init)(hal_env_t* env, void* data, pwm_port_t* port);
    void (*pwm_free)(hal_env_t* env, void* data, pwm_port_t* port);
    hal_error_t (*pwm_write)(hal_env_t* env, void* data, pwm_port_t* port, hal_pwm_value_t value);
    hal_error_t (*pwm_frequency_read)(hal_env_t* env, void* data, pwm_port_t* port, float* value);
    hal_pwm_value_t (*pwm_max_value)(hal_env_t* env, void* data);
} ports_native_interface;

typedef struct _ports_native {
    ports_native_interface native_interface;
    void* data;
} ports_native_t;

extern ports_native_interface _ports_native_interface;

hal_error_t hal_ports_probe(hal_env_t* env, hal_port_t port, uint8_t flags);

#ifdef __cplusplus
}
#endif
