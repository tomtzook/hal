#pragma once

#include "hal_types.h"
#include "hal_error.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum _hal_dio_value {
    HAL_DIO_LOW,
    HAL_DIO_HIGH
} hal_dio_value_t;

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

typedef struct _ports_native_interface {
    hal_error_t (*init)(hal_env_t* env, void** data);
    void (*free)(hal_env_t* env);

    hal_error_t (*dio_init)(hal_env_t* env, dio_port_t* port);
    void (*dio_free)(hal_env_t* env, dio_port_t* port);
    hal_error_t (*dio_write)(hal_env_t* env, dio_port_t* port, hal_dio_value_t value);
    hal_error_t (*dio_read)(hal_env_t* env, dio_port_t* port, hal_dio_value_t* value);
} ports_native_interface;

typedef struct _ports_native {
    ports_native_interface native_interface;
    void* data;
} ports_native_t;

extern ports_native_interface _ports_native_interface;

#ifdef __cplusplus
}
#endif
