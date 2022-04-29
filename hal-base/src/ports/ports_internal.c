
#include <malloc.h>

#include "../hal_internal.h"
#include "ports_internal.h"


#define HANDLE_TO_NODE(handle) ((list_node_t*) handle)
#define NODE_TO_HANDLE(node) ((hal_handle_t) node)



extern ports_interface_t _dio_interface;
extern ports_interface_t _aio_interface;
extern ports_interface_t _pwm_interface;

typedef struct {
    hal_port_t port;
} list_find_args_t;

static bool list_find_port_callback(list_node_t* node, void* arg) {
    list_find_args_t* find_args = (list_find_args_t*) arg;

    return node->data3 == find_args->port;
}

typedef struct {
    hal_env_t* env;
} list_free_args_t;

static void list_free_port_callback(list_node_t* node, void* arg) {
    list_free_args_t* free_args = (list_free_args_t*) arg;

    ports_interface_t* ports_interface = (ports_interface_t*) node->data2;
    ports_interface->close(free_args->env, node);
}


hal_error_t hal_ports_init(hal_env_t* env) {
    env->ports_sys.native.native_interface = _ports_native_interface;

    hal_error_t status = env->ports_sys.native.native_interface.init(env, &env->ports_sys.native.data);
    if (HAL_IS_ERROR(status)) {
        return status;
    }

    env->ports_sys.allocated_ports.head = NULL;
    env->ports_sys.dio = &_dio_interface;
    env->ports_sys.aio = &_aio_interface;
    env->ports_sys.pwm = &_pwm_interface;

    return HAL_SUCCESS;
}

void hal_ports_quit(hal_env_t* env) {
    list_free_args_t free_args = {.env = env};
    list_clear(&env->ports_sys.allocated_ports, list_free_port_callback, &free_args);

    env->ports_sys.native.native_interface.free(env);
}

hal_error_t hal_ports_probe(hal_env_t* env, hal_port_t port, uint8_t flags) {
    return env->ports_sys.native.native_interface.probe(env, port, flags);
}


hal_error_t hal_ports_open(hal_env_t* env, ports_interface_t* ports_interface, hal_port_t port, void* args,
                           hal_handle_t* handle_out) {
    HAL_CHECK_INITIALIZED(env);

    list_node_t* node = NULL;
    list_find_args_t find_args = {.port = port};
    if (!list_find(&env->ports_sys.allocated_ports, list_find_port_callback, &find_args, &node)) {
        // port already open
        // might be by a different interface as well
        return HAL_ERROR_TAKEN;
    }

    node = (list_node_t*) malloc(sizeof(list_node_t));
    HAL_CHECK_ALLOCATED(node);

    node->data = NULL;
    node->data2 = ports_interface;
    node->data3 = port;

    if (list_add(&env->ports_sys.allocated_ports, node)) {
        return HAL_ERROR_BAD_DATA;
    }

    hal_error_t status = ports_interface->open(env, port, node, args);
    if (HAL_IS_ERROR(status)) {
        list_remove(&env->ports_sys.allocated_ports, node);
        free(node);
        return status;
    }


    *handle_out = NODE_TO_HANDLE(node);
    return HAL_SUCCESS;
}

hal_error_t hal_ports_close(hal_env_t* env, ports_interface_t* ports_interface, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED(env);

    list_node_t* node = HANDLE_TO_NODE(handle);
    if (ports_interface != node->data2) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    ports_interface->close(env, node);
    list_remove(&env->ports_sys.allocated_ports, node);

    free(node);

    return HAL_SUCCESS;
}

hal_error_t hal_ports_action(hal_env_t* env, ports_interface_t* ports_interface, hal_handle_t handle,
                             port_action_t action, void* args) {
    HAL_CHECK_INITIALIZED(env);

    list_node_t* node = HANDLE_TO_NODE(handle);
    if (ports_interface != node->data2) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    return action(env, node, args);
}
