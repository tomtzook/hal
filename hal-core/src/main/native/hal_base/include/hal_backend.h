#pragma once

#include "hal.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char name[HAL_PORT_NAME_MAX];
    hal_port_type_t type;
    void* data;
} hal_open_port_t;

struct _hal_backend {
    hal_error_t (*port_iter_struct_size)(hal_env_t* env);
    hal_error_t (*port_iter_start)(hal_env_t* env, hal_port_iter_t* iter);
    hal_error_t (*port_iter_next)(hal_env_t* env, hal_port_iter_t* iter);

    size_t (*native_data_size_for_port)(hal_env_t* env, hal_port_type_t type);
    hal_error_t (*probe)(hal_env_t* env, const char* port_name, uint32_t* types);
    hal_error_t (*open)(hal_env_t* env, const char* port_name, hal_port_type_t type, void* data);
    hal_error_t (*close)(hal_env_t* env, const hal_open_port_t* port);

    hal_error_t (*port_probe_prop)(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key, uint32_t* flags);
    hal_error_t (*port_get_prop)(hal_env_t* env, const hal_open_port_t* port,
                                 hal_prop_key_t key,
                                 uint32_t* value);
    hal_error_t (*port_set_prop)(hal_env_t* env, const hal_open_port_t* port,
                                 hal_prop_key_t key,
                                 uint32_t value);

    hal_error_t (*dio_get)(hal_env_t* env, const hal_open_port_t* port, hal_dio_value_t* value);
    hal_error_t (*dio_set)(hal_env_t* env, const hal_open_port_t* port, hal_dio_value_t value);

    hal_error_t (*aio_get)(hal_env_t* env, const hal_open_port_t* port, uint32_t* value);
    hal_error_t (*aio_set)(hal_env_t* env, const hal_open_port_t* port, uint32_t value);

    hal_error_t (*pwm_get_duty)(hal_env_t* env, const hal_open_port_t* port, uint32_t* value);
    hal_error_t (*pwm_set_duty)(hal_env_t* env, const hal_open_port_t* port, uint32_t value);

    void* data;
    char* name;
};

hal_backend_t* hal_get_backend(hal_env_t* env);

hal_error_t hal_backend_init(hal_env_t* env);
void hal_backend_shutdown(hal_env_t* env);

#ifdef __cplusplus
}
#endif
