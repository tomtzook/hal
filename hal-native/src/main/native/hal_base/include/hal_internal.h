#pragma once

#include <pthread.h>

#include "hal.h"
#include "list.h"
#include "hal_error_handling.h"


#ifdef __cplusplus
extern "C" {
#endif

#define HAL_PORT_STRUCT_MAGIC 0xea

typedef struct _hal_used_port {
    uint8_t magic;
    char port_name[PORT_NAME_MAX];
    hal_port_type_t type;
    void* native_data;
} hal_used_port_t;

struct _hal_backend {
    uint32_t (*probe)(hal_backend_t* env, const char* port_name);
    hal_error_t (*open)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void** data);
    hal_error_t (*close)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data);

    hal_error_t (*port_probe_prop)(hal_backend_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key);
    hal_error_t (*port_get_prop)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                                 hal_prop_key_t key,
                                 unsigned* value);
    hal_error_t (*port_get_prop_f)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                                 hal_prop_key_t key,
                                 float* value);
    hal_error_t (*port_set_prop)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                                hal_prop_key_t key,
                                 unsigned value);
    hal_error_t (*port_set_prop_f)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                                 hal_prop_key_t key,
                                 float value);

    hal_error_t (*dio_get)(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t* value);
    hal_error_t (*dio_set)(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t value);

    hal_error_t (*aio_get)(hal_backend_t* env, const char* port_name, void* data, hal_aio_value_t* value);
    hal_error_t (*aio_set)(hal_backend_t* env, const char* port_name, void* data, hal_aio_value_t value);

    hal_error_t (*pwm_get_duty)(hal_backend_t* env, const char* port_name, void* data, float* value);
    hal_error_t (*pwm_set_duty)(hal_backend_t* env, const char* port_name, void* data, float value);

    void* data;
    char* name;
};

struct _hal_env {
    int initialized;
    hal_list_t used_ports;
    hal_backend_t backend;
    pthread_mutex_t mutex;
};

int hal_find_port_from_handle(hal_handle_t handle, hal_used_port_t** port_out);

int hal_find_port_node(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_list_node_t** node);
int hal_find_port_node_from_handle(hal_env_t* env, hal_handle_t handle, hal_list_node_t** node);

#ifdef __cplusplus
}
#endif
