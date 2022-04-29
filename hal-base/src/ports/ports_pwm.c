
#include <malloc.h>

#include "../hal_internal.h"
#include "ports/ports_pwm.h"


#define HANDLE_TO_NODE(handle) ((aio_port_node_t*) handle)
#define NODE_TO_HANDLE(node) ((hal_handle_t) node)

typedef struct {
    hal_pwm_value_t* value_out;
} pwm_action_get_args_t;

typedef struct {
    float* value_out;
} pwm_action_get_frequency_args_t;

typedef struct {
    hal_pwm_value_t value_in;
} pwm_action_set_args_t;

static hal_error_t internal_open(hal_env_t* env, hal_port_t port, list_node_t* node, void* args) {
    pwm_port_t* port_struct = (pwm_port_t*) malloc(sizeof(pwm_port_t));
    HAL_CHECK_ALLOCATED(port_struct);

    port_struct->port = port;
    port_struct->value = 0;
    port_struct->native_data = NULL;

    hal_error_t status = env->ports_sys.native.native_interface.pwm_init(env, port_struct);
    if (HAL_IS_ERROR(status)) {
        free(port_struct);
        return status;
    }

    node->data = port_struct;
    return HAL_SUCCESS;
}

static hal_error_t internal_close(hal_env_t* env, list_node_t* node) {
    env->ports_sys.native.native_interface.pwm_free(env, node->data);
    free(node->data);
    return HAL_SUCCESS;
}

static hal_error_t action_get(hal_env_t* env, list_node_t* node, void* args) {
    pwm_port_t* port = (pwm_port_t*) node->data;
    pwm_action_get_args_t* get_args = (pwm_action_get_args_t*) args;

    *get_args->value_out = port->value;

    return HAL_SUCCESS;
}

static hal_error_t action_set(hal_env_t* env, list_node_t* node, void* args) {
    pwm_port_t* port = (pwm_port_t*) node->data;
    pwm_action_set_args_t* set_args = (pwm_action_set_args_t*) args;

    hal_error_t status = env->ports_sys.native.native_interface.pwm_write(env, port, set_args->value_in);
    if (HAL_IS_ERROR(status)) {
        return status;
    }

    port->value = set_args->value_in;
    return HAL_SUCCESS;
}

static hal_error_t action_get_frequency(hal_env_t* env, list_node_t* node, void* args) {
    pwm_port_t* port = (pwm_port_t*) node->data;
    pwm_action_get_frequency_args_t* get_args = (pwm_action_get_frequency_args_t*) args;

    hal_error_t status = env->ports_sys.native.native_interface.pwm_frequency_read(env, port, get_args->value_out);
    if (HAL_IS_ERROR(status)) {
        return status;
    }

    return HAL_SUCCESS;
}

hal_error_t hal_pwm_open(hal_env_t* env, hal_port_t port, hal_handle_t* handle) {
    HAL_CHECK_INITIALIZED(env);

    return hal_ports_open(env, env->ports_sys.pwm, port, NULL, handle);
}

void hal_pwm_close(hal_env_t* env, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED_VOID(env);

    hal_ports_close(env, env->ports_sys.pwm, handle);
}

hal_error_t hal_pwm_get(hal_env_t* env, hal_handle_t handle, hal_aio_value_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pwm_action_get_args_t args = {.value_out = value};
    return hal_ports_action(env, env->ports_sys.pwm, handle, action_get, &args);
}

hal_error_t hal_pwm_set(hal_env_t* env, hal_handle_t handle, hal_aio_value_t value) {
    HAL_CHECK_INITIALIZED(env);

    pwm_action_set_args_t args = {.value_in = value};
    return hal_ports_action(env, env->ports_sys.pwm, handle, action_set, &args);
}

hal_error_t hal_pwm_get_frequency(hal_env_t* env, hal_handle_t handle, float* value) {
    HAL_CHECK_INITIALIZED(env);

    pwm_action_get_frequency_args_t args = {.value_out = value};
    return hal_ports_action(env, env->ports_sys.pwm, handle, action_get_frequency, &args);
}

hal_pwm_value_t hal_pwm_max_value(hal_env_t* env) {
    return env->ports_sys.native.native_interface.pwm_max_value(env);
}

ports_interface_t _pwm_interface = {
        .open = internal_open,
        .close = internal_close
};
