
#include <malloc.h>

#include "../hal_internal.h"
#include "ports/ports_aio.h"


#define HANDLE_TO_NODE(handle) ((aio_port_node_t*) handle)
#define NODE_TO_HANDLE(node) ((hal_handle_t) node)

typedef struct {
    hal_port_dir_t dir;
} aio_open_args_t;

typedef struct {
    hal_aio_value_t* value_out;
} aio_action_get_args_t;

typedef struct {
    hal_aio_value_t value_in;
} aio_action_set_args_t;

static hal_error_t internal_open(hal_env_t* env, hal_port_t port, list_node_t* node, void* args) {
    aio_open_args_t* open_args = (aio_open_args_t*) args;

    aio_port_t* port_struct = (aio_port_t*) malloc(sizeof(aio_port_t));
    HAL_CHECK_ALLOCATED(port_struct);

    port_struct->port = port;
    port_struct->dir = open_args->dir;
    port_struct->value = 0;
    port_struct->native_data = NULL;

    hal_error_t status = env->ports_sys.native.native_interface.aio_init(env, port_struct);
    if (HAL_IS_ERROR(status)) {
        free(port_struct);
        return status;
    }

    node->data = port_struct;
    return HAL_SUCCESS;
}

static hal_error_t internal_close(hal_env_t* env, list_node_t* node) {
    env->ports_sys.native.native_interface.aio_free(env, node->data);
    free(node->data);
    return HAL_SUCCESS;
}

static hal_error_t action_get(hal_env_t* env, list_node_t* node, void* args) {
    aio_port_t* port = (aio_port_t*) node->data;
    aio_action_get_args_t* get_args = (aio_action_get_args_t*) args;

    if (PORT_DIR_INPUT == port->dir) {
        hal_error_t status = env->ports_sys.native.native_interface.aio_read(env, port, get_args->value_out);
        if (HAL_IS_ERROR(status)) {
            return status;
        }
    } else {
        *get_args->value_out = port->value;
    }

    return HAL_SUCCESS;
}

static hal_error_t action_set(hal_env_t* env, list_node_t* node, void* args) {
    aio_port_t* port = (aio_port_t*) node->data;
    aio_action_set_args_t* set_args = (aio_action_set_args_t*) args;

    if (PORT_DIR_INPUT == port->dir) {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    hal_error_t status = env->ports_sys.native.native_interface.aio_write(env, port, set_args->value_in);
    if (HAL_IS_ERROR(status)) {
        return status;
    }

    port->value = set_args->value_in;
    return HAL_SUCCESS;
}

hal_error_t hal_aio_open(hal_env_t* env, hal_port_t port, hal_port_dir_t dir, hal_handle_t* handle) {
    HAL_CHECK_INITIALIZED(env);

    aio_open_args_t args = {.dir = dir};
    return hal_ports_open(env, env->ports_sys.aio, port, &args, handle);
}

void hal_aio_close(hal_env_t* env, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED_VOID(env);

    hal_ports_close(env, env->ports_sys.aio, handle);
}

hal_error_t hal_aio_get(hal_env_t* env, hal_handle_t handle, hal_aio_value_t* value) {
    HAL_CHECK_INITIALIZED(env);

    aio_action_get_args_t args = {.value_out = value};
    return hal_ports_action(env, env->ports_sys.aio, handle, action_get, &args);
}

hal_error_t hal_aio_set(hal_env_t* env, hal_handle_t handle, hal_aio_value_t value) {
    HAL_CHECK_INITIALIZED(env);

    aio_action_set_args_t args = {.value_in = value};
    return hal_ports_action(env, env->ports_sys.aio, handle, action_set, &args);
}

hal_aio_value_t hal_aio_max_value(hal_env_t* env) {
    return env->ports_sys.native.native_interface.aio_max_value(env);
}

ports_interface_t _aio_interface = {
        .open = internal_open,
        .close = internal_close
};
