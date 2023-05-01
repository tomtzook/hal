#pragma once

#include "hal.h"
#include "list.h"
#include "hal_error_handling.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _hal_used_port {
    hal_port_t port;
    hal_port_type_t type;
    void* native_data;
} hal_used_port_t;

struct _hal_backend {
    uint32_t (*probe)(hal_backend_t* env, hal_port_t port);
    hal_error_t (*open)(hal_backend_t* env, hal_port_t port, hal_port_type_t type, void** data);
    hal_error_t (*close)(hal_backend_t* env, hal_port_t port, hal_port_type_t type, void* data);

    hal_error_t (*dio_get)(hal_backend_t* env, hal_port_t port, void* data, hal_dio_value_t* value);
    hal_error_t (*dio_set)(hal_backend_t* env, hal_port_t port, void* data, hal_dio_value_t value);

    hal_error_t (*aio_get)(hal_backend_t* env, hal_port_t port, void* data, hal_aio_value_t* value);
    hal_error_t (*aio_set)(hal_backend_t* env, hal_port_t port, void* data, hal_aio_value_t value);

    void* data;
    char* name;
};

struct _hal_env {
    hal_list_t used_ports;
    hal_backend_t backend;
};

int hal_find_port_node(hal_env_t* env, hal_port_t port, hal_port_type_t type, hal_list_node_t** node);
int hal_find_port_node_from_handle(hal_env_t* env, hal_handle_t handle, hal_list_node_t** node);

#ifdef __cplusplus
}
#endif
