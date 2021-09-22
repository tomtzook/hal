
#include <malloc.h>

#include "../hal_internal.h"
#include "ports/ports_dio.h"


#define HANDLE_TO_NODE(handle) ((dio_port_node_t*) handle)
#define NODE_TO_HANDLE(node) ((hal_handle_t) node)


static hal_error_t find_port_node(hal_env_t* env, hal_port_t port, hal_handle_t* handle) {
    dio_port_node_t* node = env->ports_sys.dio_head;
    while (NULL != node) {
        if (port == node->port.port) {
            *handle = NODE_TO_HANDLE(node);
            return HAL_SUCCESS;
        }

        node = node->next;
    }

    return HAL_NOT_FOUND;
}

hal_error_t hal_dio_open(hal_env_t* env, hal_port_t port, hal_port_dir_t dir, hal_handle_t* handle) {
    HAL_CHECK_INITIALIZED(env);

    hal_error_t status = find_port_node(env, port, handle);
    if (!HAL_IS_ERROR(status)) {
        // port already open
        return HAL_SUCCESS;
    }

    dio_port_node_t* node = (dio_port_node_t*) malloc(sizeof(dio_port_node_t));
    HAL_CHECK_ALLOCATED(node);

    node->port.port = port;
    node->port.value = HAL_DIO_LOW;
    node->port.dir = dir;
    node->prev = NULL;
    node->next = NULL;

    status = env->ports_sys.native.dio_init(env, &node->port);
    if (HAL_IS_ERROR(status)) {
        free(node);
        return status;
    }

    if (NULL == env->ports_sys.dio_head) {
        env->ports_sys.dio_head = node;
        env->ports_sys.dio_tail = node;
    } else {
        dio_port_node_t* prev = env->ports_sys.dio_tail;
        env->ports_sys.dio_tail = node;

        prev->next = node;
        node->prev = prev;
    }

    return HAL_SUCCESS;
}

void hal_dio_close(hal_env_t* env, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED_VOID(env);

    dio_port_node_t* node = HANDLE_TO_NODE(handle);

    hal_dio_close_port(env, &node->port);

    if (env->ports_sys.dio_head == node) {
        // no prev
        env->ports_sys.dio_head = node->next;
        node->next->prev = NULL;
    } else if (env->ports_sys.dio_tail == node) {
        // no next
        env->ports_sys.dio_tail = node->prev;
        node->prev->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    free(node);
}

void hal_dio_close_port(hal_env_t* env, dio_port_t* port) {
    HAL_CHECK_INITIALIZED_VOID(env);

    env->ports_sys.native.dio_free(env, port);
}

hal_error_t hal_dio_get(hal_env_t* env, hal_handle_t handle, hal_dio_value_t* value) {
    HAL_CHECK_INITIALIZED(env);

    dio_port_node_t* node = HANDLE_TO_NODE(handle);

    if (PORT_DIR_INPUT == node->port.dir) {
        return env->ports_sys.native.dio_read(env, &node->port, value);
    } else {
        *value = node->port.value;
        return HAL_SUCCESS;
    }
}

hal_error_t hal_dio_set(hal_env_t* env, hal_handle_t handle, hal_dio_value_t value) {
    HAL_CHECK_INITIALIZED(env);

    dio_port_node_t* node = HANDLE_TO_NODE(handle);

    if (PORT_DIR_INPUT == node->port.dir) {
        return HAL_UNSUPPORTED_OPERATION;
    }

    hal_error_t status = env->ports_sys.native.dio_write(env, &node->port, value);
    if (status) {
        return status;
    }

    node->port.value = value;
    return HAL_SUCCESS;
}
