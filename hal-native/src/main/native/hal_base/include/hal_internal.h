#pragma once

#include <pthread.h>

#include "hal.h"
#include "hal_list.h"
#include "hal_table.h"
#include "hal_error_handling.h"


#ifdef __cplusplus
extern "C" {
#endif

#define HAL_DESCRIPTOR_TABLE_SIZE 256

typedef struct {
    char port_name[PORT_NAME_MAX];
    hal_port_type_t type;
    uint8_t native_data[];
} hal_used_port_t;

struct _hal_backend {
    size_t (*native_data_size_for_port)(hal_backend_t* env, hal_port_type_t type);
    uint32_t (*probe)(hal_backend_t* env, const char* port_name);
    hal_error_t (*open)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data);
    hal_error_t (*close)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data);

    hal_error_t (*port_probe_prop)(hal_backend_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key, hal_config_flags_t* flags);
    hal_error_t (*port_get_prop)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                                 hal_prop_key_t key,
                                 hal_prop_value_t* value);
    hal_error_t (*port_set_prop)(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                                hal_prop_key_t key,
                                 hal_prop_value_t value);

    hal_error_t (*dio_get)(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t* value);
    hal_error_t (*dio_set)(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t value);

    hal_error_t (*aio_get)(hal_backend_t* env, const char* port_name, void* data, hal_aio_value_t* value);
    hal_error_t (*aio_set)(hal_backend_t* env, const char* port_name, void* data, hal_aio_value_t value);

    hal_error_t (*pwm_get_duty)(hal_backend_t* env, const char* port_name, void* data, uint32_t* value);
    hal_error_t (*pwm_set_duty)(hal_backend_t* env, const char* port_name, void* data, uint32_t value);

    void* data;
    char* name;
};

struct _hal_env {
    int initialized;
    hal_descriptor_table_t ports_table;
    hal_backend_t backend;
    pthread_mutex_t mutex;
};

int hal_find_port_from_handle(hal_env_t* env, hal_handle_t handle, hal_used_port_t** port_out, size_t* index_out);

#ifdef __cplusplus
}
#endif
