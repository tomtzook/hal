
#include <malloc.h>
#include <syslog.h>
#include <string.h>

#include <hal.h>

#include "hal_control.h"
#include "hal_backend.h"
#include "hal_internal.h"


static hal_error_t check_backend(const hal_backend_t* backend) {
    if (backend->name == NULL ||
            backend->funcs.open == NULL ||
            backend->funcs.close == NULL ||
            backend->funcs.port_probe_prop == NULL ||
            backend->funcs.port_get_prop == NULL ||
            backend->funcs.port_set_prop == NULL) {
        return HAL_ERROR_BAD_DATA;
    }

    return HAL_SUCCESS;
}

static int is_port_config_supported_for_type(hal_prop_key_t key, hal_port_type_t type) {
    switch (key) {
        case HAL_CONFIG_DIO_POLL_EDGE:
        case HAL_CONFIG_DIO_RESISTOR:
            if ((type & (HAL_TYPE_DIGITAL_OUTPUT | HAL_TYPE_DIGITAL_INPUT)) == 0) {
                return 1;
            }
            break;
        case HAL_CONFIG_ANALOG_MAX_VALUE:
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE:
        case HAL_CONFIG_ANALOG_SAMPLE_RATE:
            if ((type & (HAL_TYPE_ANALOG_OUTPUT | HAL_TYPE_ANALOG_INPUT)) == 0) {
                return 1;
            }
            break;
        case HAL_CONFIG_PWM_FREQUENCY:
            if ((type & (HAL_TYPE_PWM_OUTPUT)) == 0) {
                return 1;
            }
            break;
        default:
            return 1;
    }

    return 0;
}

static void close_port(hal_env_t* env, hal_open_port_node_t* port_node, size_t index) {
    TRACE_INFO("closing port %u of type %d (handle %u)",
               port_node->backend_port.identifier, port_node->backend_port.type, port_node->handle);

    if (env->backend.funcs.close != NULL) {
        env->backend.funcs.close(env, &port_node->backend_port);
    } else {
        TRACE_ERROR("BACKEND does not support CLOSE");
    }

    hal_descriptor_table_remove(&env->handle_table, index);

    port_node->port->open_handle = HAL_EMPTY_HANDLE;
    port_node->port->flags &= ~HAL_FLAG_OPEN;

    free(port_node);
}

hal_backend_t* hal_get_backend(hal_env_t* env) {
    return &env->backend;
}

int hal_find_port_by_id(hal_env_t* env, hal_id_t id, hal_port_t** port_out) {
    if(id == HAL_INVALID_IDENTIFIER) {
        return 1;
    }

    size_t index = (size_t) id;
    hal_port_t* port;
    if (hal_descriptor_table_get(&env->port_table, index, (void**)&port)) {
        return 1;
    }

    *port_out = port;

    return 0;
}

int hal_find_next_id(hal_env_t* env, hal_id_t* id) {
    hal_id_t _id = *id;
    size_t index;
    if (_id == HAL_INVALID_IDENTIFIER) {
        index = 0;
    } else {
        index = (size_t) _id + 1;
    }

    hal_port_t* port;
    while (index < env->port_table.capacity) {
        if (!hal_descriptor_table_get(&env->port_table, index, (void**)&port)) {
            *id = (hal_id_t) index;
            return 0;
        }

        index++;
    }

    return 1;
}

int hal_find_port_from_handle(hal_env_t* env, hal_handle_t handle, hal_open_port_node_t** port_out, size_t* index_out) {
    if(handle == HAL_EMPTY_HANDLE) {
        return 1;
    }

    size_t index = (size_t) handle;
    hal_open_port_node_t* used_port;
    if (hal_descriptor_table_get(&env->handle_table, index, (void**)&used_port)) {
        return 1;
    }

    if (port_out != NULL) {
        *port_out = used_port;
    }

    if (index_out != NULL) {
        *index_out = index;
    }

    return 0;
}

hal_error_t hal_init(hal_env_t** env) {
    hal_error_t status;
    int mutex_initialized = 0;
    int table_initialized = 0;
    int backend_initialized = 0;

    openlog("HAL", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    TRACE_INFO("Initializing HAL");

    hal_env_t* _env = (hal_env_t*) malloc(sizeof(hal_env_t));
    HAL_CHECK_ALLOCATED(_env, error);

    memset(_env, 0, sizeof(hal_env_t));

    if (pthread_mutex_init(&_env->mutex, NULL)) {
        TRACE_SYSTEM_ERROR();
        HAL_JUMP_IF_ERROR(HAL_ERROR_ENVIRONMENT, error);
    }

    mutex_initialized = 1;

    _env->handle_table.elements = NULL;
    _env->port_table.elements = NULL;
    if (hal_descriptor_table_init(&_env->handle_table, HAL_DESCRIPTOR_TABLE_SIZE) ||
        hal_descriptor_table_init(&_env->port_table, HAL_DESCRIPTOR_TABLE_SIZE)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_DATA, error);
    }

    table_initialized = 1;

    memset(&_env->backend, 0, sizeof(_env->backend));
    _env->backend.name = "N/A";

    TRACE_INFO("Initializing BACKEND");
    status = hal_backend_init(_env);
    HAL_JUMP_IF_ERROR(status, error);
    backend_initialized = 1;

    status = check_backend(&_env->backend);
    HAL_JUMP_IF_ERROR(status, error);

    TRACE_INFO("Using BACKEND %s", _env->backend.name);

    __sync_fetch_and_or(&_env->initialized, 1);
    *env = _env;
    return HAL_SUCCESS;
error:
    *env = NULL;
    if (backend_initialized) {
        hal_backend_shutdown(_env);
    }
    if (table_initialized) {
        if (_env->handle_table.elements != NULL) {
            hal_descriptor_table_free(&_env->handle_table);
        }
        if (_env->port_table.elements != NULL) {
            hal_descriptor_table_free(&_env->port_table);
        }
    }
    if (mutex_initialized) {
        pthread_mutex_destroy(&_env->mutex);
    }
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

    pthread_mutex_lock(&env->mutex);

    for (size_t i = 0; i < env->handle_table.capacity; ++i) {
        hal_open_port_node_t* port_node;
        if (!hal_descriptor_table_get(&env->handle_table, i, (void**) &port_node)) {
            // has such port,
            close_port(env, port_node, i);
        }
    }

    for (size_t i = 0; i < env->port_table.capacity; ++i) {
        hal_port_t* port;
        if (!hal_descriptor_table_get(&env->port_table, i, (void**) &port)) {
            // has such port,
            halcontrol_unregister_port(env, i);
        }
    }

    hal_backend_shutdown(env);

    __sync_fetch_and_and(&env->initialized, 0);

    pthread_mutex_unlock(&env->mutex);

    hal_descriptor_table_free(&env->handle_table);
    hal_descriptor_table_free(&env->port_table);
    pthread_mutex_destroy(&env->mutex);
    free(env);

    closelog();
}

hal_error_t hal_probe(hal_env_t* env, hal_id_t id, hal_port_type_t type) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_TAKEN, end);
    }

    TRACE_INFO("Probing port %u for type %d", id, type);

    if ((port->supported_types & type) != type) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_TYPE_NOT_SUPPORTED, end);
    }

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_open(hal_env_t* env, hal_id_t id, hal_port_type_t type, hal_handle_t* handle) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status;
    int opened = 0;
    hal_open_port_node_t* port_node = NULL;

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_TAKEN, error);
    }

    if ((port->flags & HAL_FLAG_BLOCKED) != 0) {
        TRACE_ERROR("Cannot open a blocked port");
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE, error);
    }

    if (env->backend.funcs.open == NULL) {
        TRACE_ERROR("BACKEND does not support OPEN");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, error);
    }

    if ((port->supported_types & type) != type) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_TYPE_NOT_SUPPORTED, error);
    }

    TRACE_INFO("Opening port %u of type %d", id, type);

    size_t extra_allocation_size = 0;
    if (env->backend.funcs.native_data_size_for_port != NULL) {
        extra_allocation_size = env->backend.funcs.native_data_size_for_port(env, type);
    }

    size_t total_allocation_size = sizeof(hal_open_port_node_t) + extra_allocation_size;
    port_node = (hal_open_port_node_t*) malloc(total_allocation_size);
    HAL_CHECK_ALLOCATED(port_node, error);

    memset(port_node, 0, sizeof(total_allocation_size));
    port_node->backend_port.identifier = id;
    port_node->backend_port.type = type;
    port_node->backend_port.data = port_node->native_data;

    status = env->backend.funcs.open(env, &port_node->backend_port);
    HAL_JUMP_IF_ERROR(status, error);
    opened = 1;

    size_t index;
    if (hal_descriptor_table_add(&env->handle_table, port_node, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_DATA, error);
    }

    hal_handle_t new_handle = (hal_handle_t) index;
    port_node->handle = new_handle;
    port->open_handle = new_handle;
    port->flags |= HAL_FLAG_OPEN;

    *handle = new_handle;
    TRACE_INFO("New port %u assigned handle %u", id, new_handle);

    pthread_mutex_unlock(&env->mutex);
    return HAL_SUCCESS;
error:
    if (opened) {
        env->backend.funcs.close(env, &port_node->backend_port);
    }
    if (NULL != port_node) {
        free(port_node);
    }

    pthread_mutex_unlock(&env->mutex);

    return status;
}

void hal_close(hal_env_t* env, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED_VOID(env);

    pthread_mutex_lock(&env->mutex);

    hal_open_port_node_t* port_node;
    size_t index;
    if (hal_find_port_from_handle(env, handle, &port_node, &index)) {
        goto end;
    }

    close_port(env, port_node, index);

end:
    pthread_mutex_unlock(&env->mutex);
}

hal_error_t hal_get_handle(hal_env_t* env, hal_id_t id, hal_handle_t* handle) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        *handle = HAL_EMPTY_HANDLE;
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if ((port->flags & HAL_FLAG_OPEN) != 0) {
        *handle = port->open_handle;
    } else {
        *handle = HAL_EMPTY_HANDLE;
    }
end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_get_info(hal_env_t* env, hal_id_t id, hal_port_info_t* info) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_port_t* port;
    if (hal_find_port_by_id(env, id, &port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    info->identifier = id;
    info->flags = port->flags;
    info->supported_types = port->supported_types;
    info->supported_props = port->supported_props;
    info->open_handle = info->open_handle;

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_iter_port_start(hal_env_t* env, hal_port_iter_t** iter) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;
    hal_port_iter_t* _iter;

    if (iter == NULL) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_ARGUMENT, end);
    }

    _iter = (hal_port_iter_t*) malloc(sizeof(hal_port_iter_t));
    HAL_CHECK_ALLOCATED(_iter, end);

    memset(_iter, 0, sizeof(hal_port_iter_t));

    _iter->identifier = HAL_INVALID_IDENTIFIER;
    if (hal_find_next_id(env, &_iter->identifier)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE, end);
    } else {
        *iter = _iter;
    }

end:
    pthread_mutex_unlock(&env->mutex);
    if (_iter != NULL && HAL_IS_ERROR(status)) {
        free(_iter);
    }

    return status;
}

hal_error_t hal_iter_port_next(hal_env_t* env, hal_port_iter_t* iter) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    if (iter == NULL) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_ARGUMENT, end);
    }

    if (hal_find_next_id(env, &iter->identifier)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE, end);
    }

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_iter_port_end(hal_env_t* env, hal_port_iter_t* iter) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    if (iter == NULL) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_ARGUMENT, end);
    }

    free(iter);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_port_property_probe(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t* flags) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (is_port_config_supported_for_type(key, port_node->backend_port.type)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.port_probe_prop == NULL) {
        TRACE_ERROR("BACKEND does not support PROBE PROP");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    if ((port_node->port->supported_props & key) == 0) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED, end);
    }

    TRACE_INFO("Probing port configuration for port %u with type %d (handle %u) for key %d",
               port_node->port->identifier, port_node->backend_port.type, handle, key);

    status = env->backend.funcs.port_probe_prop(env, &port_node->backend_port, key, flags);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_port_get_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (is_port_config_supported_for_type(key, port_node->backend_port.type)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.port_get_prop == NULL) {
        TRACE_ERROR("BACKEND does not support GET PROP");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    if ((port_node->port->supported_props & key) == 0) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED, end);
    }

    if (env->backend.funcs.port_probe_prop != NULL) {
        uint32_t flags;
        status = env->backend.funcs.port_probe_prop(env, &port_node->backend_port, key, &flags);
        HAL_JUMP_IF_ERROR(status, end);

        if ((flags & HAL_CONFIG_FLAG_READABLE) == 0) {
            HAL_JUMP_IF_ERROR(HAL_ERROR_CONFIG_KEY_NOT_READABLE, end);
        }
    }

    TRACE_INFO("Reading configuration of port %u of type %d (handle %u): key=%d",
               port_node->port->identifier, port_node->backend_port.type, handle,
               key);

    status = env->backend.funcs.port_get_prop(env,
                                        &port_node->backend_port,
                                        key, value);
end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_port_set_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, uint32_t value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (is_port_config_supported_for_type(key, port_node->backend_port.type)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.port_set_prop == NULL) {
        TRACE_ERROR("BACKEND does not support SET PROP");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    if ((port_node->port->supported_props & key) == 0) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED, end);
    }

    if (env->backend.funcs.port_probe_prop != NULL) {
        uint32_t flags;
        status = env->backend.funcs.port_probe_prop(env, &port_node->backend_port, key, &flags);
        HAL_JUMP_IF_ERROR(status, end);

        if ((flags & HAL_CONFIG_FLAG_WRITABLE) == 0) {
            HAL_JUMP_IF_ERROR(HAL_ERROR_CONFIG_KEY_NOT_WRITABLE, end);
        }
    }

    TRACE_INFO("Configuring port %u of type %d (handle %u): key=%d value=0x%x",
               port_node->port->identifier, port_node->backend_port.type, handle,
               key, value);

    status = env->backend.funcs.port_set_prop(env,
                                        &port_node->backend_port,
                                        key, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
