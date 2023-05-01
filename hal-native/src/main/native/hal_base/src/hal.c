
#include <malloc.h>
#include <syslog.h>
#include <string.h>

#include "hal.h"
#include "hal_internal.h"
#include "hal_backend.h"


int hal_find_port_node(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_list_node_t** node_out) {
    hal_list_node_t* node = env->used_ports.head;
    while (NULL != node) {
        hal_used_port_t* node_data = (hal_used_port_t*)node->data;
        if (0 == strcmp(node_data->port_name, port_name) && node_data->type == type) {
            *node_out = node;
            return 0;
        }

        node = node->next;
    }

    return 1;
}

int hal_find_port_node_from_handle(hal_env_t* env, hal_handle_t handle, hal_list_node_t** node_out) {
    hal_used_port_t* used_port = (hal_used_port_t*) handle;
    if (NULL == used_port) {
        return 1;
    }

    hal_list_node_t* node = env->used_ports.head;
    while (NULL != node) {
        hal_used_port_t* node_data = (hal_used_port_t*)node->data;
        if (0 == strcmp(node_data->port_name, used_port->port_name) && node_data->type == used_port->type) {
            *node_out = node;
            return 0;
        }

        node = node->next;
    }

    return 1;
}

hal_error_t hal_init(hal_env_t** env) {
    hal_error_t status;

    openlog("HAL", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    TRACE_INFO("Initializing HAL");

    hal_env_t* _env = (hal_env_t*) malloc(sizeof(hal_env_t));
    HAL_CHECK_ALLOCATED(_env);

    _env->used_ports.head = NULL;
    _env->backend.name = "N/A";
    _env->backend.dio_get = NULL;
    _env->backend.dio_set = NULL;
    _env->backend.aio_get = NULL;
    _env->backend.aio_set = NULL;

    TRACE_INFO("Initializing BACKEND");
    status = hal_backend_init(&_env->backend);
    HAL_JUMP_IF_ERROR(status, error);

    TRACE_INFO("Using BACKEND %s", _env->backend.name);

    *env = _env;
    return HAL_SUCCESS;
error:
    *env = NULL;
    if (NULL != _env) {
        free(_env);
    }
    closelog();

    return status;
}

void hal_shutdown(hal_env_t* env) {
    if (NULL == env) {
        return;
    }

    TRACE_INFO("Shutting down HAL");

    hal_backend_shutdown(&env->backend);
    free(env);
    closelog();
}

hal_error_t hal_probe(hal_env_t* env, const char* port_name, hal_port_type_t type) {
    HAL_CHECK_INITIALIZED(env);

    uint32_t flags = env->backend.probe(&env->backend, port_name);
    if ((flags & type) != type) {
        return HAL_ERROR_TYPE_NOT_SUPPORTED;
    }

    return HAL_SUCCESS;
}

hal_error_t hal_open(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_handle_t* handle) {
    HAL_CHECK_INITIALIZED(env);

    hal_error_t status;
    void* native_data = NULL;
    hal_list_node_t* node;

    if (!hal_find_port_node(env, port_name, type, &node)) {
        return HAL_ERROR_TAKEN;
    }

    status = hal_probe(env, port_name, type);
    if (HAL_IS_ERROR((status))) {
        return status;
    }

    TRACE_INFO("Opening port %s of type %d", port_name, type);

    node = (hal_list_node_t*) malloc(sizeof(hal_list_node_t) + sizeof(hal_used_port_t));
    HAL_CHECK_ALLOCATED(node);

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
    strncpy(used_port->port_name, port_name, PORT_NAME_MAX);
    used_port->type = type;

    status = env->backend.open(&env->backend, port_name, type, &native_data);
    HAL_JUMP_IF_ERROR(status, error);

    used_port->native_data = native_data;
    if (hal_list_add(&env->used_ports, node)) {
        status = HAL_ERROR_BAD_DATA;
        goto error;
    }

    *handle = ((hal_handle_t) used_port);
    TRACE_INFO("New port %s assigned handle %u", port_name, *handle);

    return HAL_SUCCESS;
error:
    if (NULL != node) {
        free(node);
    }
    if (NULL != native_data) {
        env->backend.close(&env->backend, port_name, type, native_data);
    }

    return status;
}

void hal_close(hal_env_t* env, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED_VOID(env);

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        return;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;

    TRACE_INFO("closing port %s of type %d (handle %u)", used_port->port_name, used_port->type, handle);

    env->backend.close(&env->backend, used_port->port_name, used_port->type, used_port->native_data);
    hal_list_remove(&env->used_ports, node);
}
