#pragma once

#include "hal.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    hal_id_t identifier;
    hal_port_type_t type;

    void* data;
} hal_backend_port_t;

struct hal__backend {
    struct {
        hal_error_t (*open)(hal_env_t* env, const hal_backend_port_t* port);
        hal_error_t (*close)(hal_env_t* env, const hal_backend_port_t* port);

        hal_error_t (*port_probe_prop)(hal_env_t* env, const hal_backend_port_t* port, hal_prop_key_t key, uint32_t* flags);
        hal_error_t (*port_get_prop)(hal_env_t* env, const hal_backend_port_t* port,
                                     hal_prop_key_t key,
                                     uint32_t* value);
        hal_error_t (*port_set_prop)(hal_env_t* env, const hal_backend_port_t* port,
                                     hal_prop_key_t key,
                                     uint32_t value);

        hal_error_t (*dio_get)(hal_env_t* env, const hal_backend_port_t* port, hal_dio_value_t* value);
        hal_error_t (*dio_set)(hal_env_t* env, const hal_backend_port_t* port, hal_dio_value_t value);

        hal_error_t (*aio_get)(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value);
        hal_error_t (*aio_set)(hal_env_t* env, const hal_backend_port_t* port, uint32_t value);

        hal_error_t (*pwm_get_duty)(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value);
        hal_error_t (*pwm_set_duty)(hal_env_t* env, const hal_backend_port_t* port, uint32_t value);

        hal_error_t (*quadrature_get_pos)(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value);
        hal_error_t (*quadrature_set_pos)(hal_env_t* env, const hal_backend_port_t* port, uint32_t value);
        hal_error_t (*quadrature_get_period)(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value);
    } funcs;

    void* data;
    char* name;
};

hal_backend_t* hal_get_backend(hal_env_t* env);

hal_error_t hal_backend_init(hal_env_t* env);
void hal_backend_shutdown(hal_env_t* env);

#ifdef __cplusplus
}
#endif
