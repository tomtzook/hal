
#include <malloc.h>

#include "../hal_internal.h"
#include "ports_internal.h"


#define HANDLE_TO_NODE(handle) ((list_node_t*) handle)
#define NODE_TO_HANDLE(node) ((hal_handle_t) node)


typedef struct {
    hal_env_t* env;
    ports_interface_t* ports_interface;
    hal_port_t port;
} list_find_args_t;

static bool list_find_port_callback(list_node_t* node, void* arg) {
    list_find_args_t* find_args = (list_find_args_t*) arg;

    return find_args->ports_interface->is_same(
            find_args->env,
            find_args->port,
            node
            );
}

typedef struct {
    hal_env_t* env;
    ports_interface_t* ports_interface;
} list_free_args_t;

static void list_free_port_callback(list_node_t* node, void* arg) {
    list_free_args_t* free_args = (list_free_args_t*) arg;

    free_args->ports_interface->close(free_args->env, node);
}


hal_error_t hal_ports_init(hal_env_t* env, hal_native_t hal_native) {
    hal_error_t status = hal_native.ports_init(env, &env->ports_sys.native);
    if (HAL_IS_ERROR(status)) {
        return status;
    }

    hal_ports_dio_init_interface(env, &env->ports_sys.dio);

    return HAL_SUCCESS;
}

void hal_ports_quit(hal_env_t* env) {
    list_free_args_t free_args = {.env = env, .ports_interface = &env->ports_sys.dio};
    list_clear(&env->ports_sys.dio.ports, list_free_port_callback, &free_args);

    env->ports_sys.native.free(env, &env->ports_sys.native);
}


hal_error_t hal_ports_open(hal_env_t* env, ports_interface_t* ports_interface, hal_port_t port, void* args,
                           hal_handle_t* handle_out) {
    HAL_CHECK_INITIALIZED(env);

    list_node_t* node = NULL;
    list_find_args_t find_args = {.env = env, .ports_interface = ports_interface, .port = port};

    if (!list_find(&ports_interface->ports, list_find_port_callback, &find_args, &node)) {
        // port already open
        return HAL_ERROR_TAKEN;
    }

    node = (list_node_t*) malloc(sizeof(list_node_t));
    HAL_CHECK_ALLOCATED(node);

    if (list_add(&ports_interface->ports, node)) {
        return HAL_DATA_ERROR;
    }

    hal_error_t status = ports_interface->open(env, port, node, args);
    if (HAL_IS_ERROR(status)) {
        list_remove(&ports_interface->ports, node);
        free(node);
        return status;
    }


    *handle_out = NODE_TO_HANDLE(node);
    return HAL_SUCCESS;
}

hal_error_t hal_ports_close(hal_env_t* env, ports_interface_t* ports_interface, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED(env);

    list_node_t* node = HANDLE_TO_NODE(handle);

    ports_interface->close(env, node);
    list_remove(&ports_interface->ports, node);

    free(node);

    return HAL_SUCCESS;
}

hal_error_t hal_ports_action(hal_env_t* env, ports_interface_t* ports_interface, hal_handle_t handle,
                             port_action_t action, void* args) {
    HAL_CHECK_INITIALIZED(env);

    list_node_t* node = HANDLE_TO_NODE(handle);

    return action(env, node, args);
}
