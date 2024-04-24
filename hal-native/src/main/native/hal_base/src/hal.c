
#include <malloc.h>
#include <syslog.h>
#include <string.h>

#include "hal.h"
#include "hal_internal.h"
#include "hal_backend.h"


static hal_error_t check_backend(const hal_backend_t* backend) {
    if (backend->name == NULL ||
            backend->probe == NULL ||
            backend->open == NULL ||
            backend->close == NULL ||
            backend->port_probe_prop == NULL ||
            backend->port_get_prop == NULL ||
            backend->port_set_prop == NULL) {
        return HAL_ERROR_BAD_DATA;
    }

    return HAL_SUCCESS;
}

static int check_port_config_supported_for_type(hal_config_keys_t key, hal_port_type_t type) {
    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE:
        case HAL_CONFIG_GPIO_RESISTOR:
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

int hal_find_port_from_handle(hal_handle_t handle, hal_used_port_t** port_out) {
    hal_used_port_t* used_port = (hal_used_port_t*) handle;
    if (NULL == used_port) {
        return 1;
    }

    if (used_port->magic != HAL_PORT_STRUCT_MAGIC) {
        return 1;
    }

    *port_out = used_port;
    return 0;
}

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
    hal_used_port_t* used_port;
    if (hal_find_port_from_handle(handle, &used_port)) {
        return 1;
    }

    hal_list_node_t* node;
    if (hal_find_port_node(env, used_port->port_name, used_port->type, &node)) {
        return 1;
    }

    *node_out = node;
    return 0;
}

hal_error_t hal_init(hal_env_t** env) {
    hal_error_t status;
    int mutex_initialized = 0;
    int backend_initialized = 0;

    openlog("HAL", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    TRACE_INFO("Initializing HAL");

    hal_env_t* _env = (hal_env_t*) malloc(sizeof(hal_env_t));
    HAL_CHECK_ALLOCATED(_env);

    if (pthread_mutex_init(&_env->mutex, NULL)) {
        TRACE_SYSTEM_ERROR();
        status = HAL_ERROR_ENVIRONMENT;
        goto error;
    }

    mutex_initialized = 1;

    _env->used_ports.head = NULL;
    memset(&_env->backend, 0, sizeof(_env->backend));
    _env->backend.name = "N/A";

    TRACE_INFO("Initializing BACKEND");
    status = hal_backend_init(&_env->backend);
    HAL_JUMP_IF_ERROR(status, error);
    backend_initialized = 1;

    status = check_backend(&_env->backend);
    HAL_JUMP_IF_ERROR(status, error);

    TRACE_INFO("Using BACKEND %s", _env->backend.name);

    _env->initialized = 1;
    *env = _env;
    return HAL_SUCCESS;
error:
    *env = NULL;
    if (backend_initialized) {
        hal_backend_shutdown(&_env->backend);
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

    hal_list_node_t* node = env->used_ports.head;
    hal_list_node_t* next_node;
    while (NULL != node) {
        hal_used_port_t* used_port = (hal_used_port_t*)node->data;
        next_node = node->next;

        env->backend.close(&env->backend, used_port->port_name, used_port->type, used_port->native_data);
        hal_list_remove(&env->used_ports, node);

        node = next_node;
    }

    env->initialized = 0;

    pthread_mutex_unlock(&env->mutex);

    hal_backend_shutdown(&env->backend);
    pthread_mutex_destroy(&env->mutex);
    free(env);
    closelog();
}

hal_error_t hal_probe(hal_env_t* env, const char* port_name, hal_port_type_t type) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    if (env->backend.probe == NULL) {
        TRACE_ERROR("BACKEND does not support PROBE");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Probing port %s for type %d", port_name, type);

    uint32_t flags = env->backend.probe(&env->backend, port_name);
    if ((flags & type) != type) {
        status = HAL_ERROR_TYPE_NOT_SUPPORTED;
    }

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_open(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_handle_t* handle) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status;
    void* native_data = NULL;
    hal_list_node_t* node = NULL;

    if (!hal_find_port_node(env, port_name, type, &node)) {
        status = HAL_ERROR_TAKEN;
        goto error;
    }

    if (env->backend.probe == NULL) {
        TRACE_ERROR("BACKEND does not support PROBE");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto error;
    }

    uint32_t flags = env->backend.probe(&env->backend, port_name);
    if ((flags & type) != type) {
        status = HAL_ERROR_TYPE_NOT_SUPPORTED;
        goto error;
    }

    TRACE_INFO("Opening port %s of type %d", port_name, type);

    node = (hal_list_node_t*) malloc(sizeof(hal_list_node_t) + sizeof(hal_used_port_t));
    HAL_CHECK_ALLOCATED(node);

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
    used_port->magic = HAL_PORT_STRUCT_MAGIC;
    strncpy(used_port->port_name, port_name, PORT_NAME_MAX);
    used_port->type = type;

    if (env->backend.open == NULL) {
        TRACE_ERROR("BACKEND does not support OPEN");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto error;
    }

    status = env->backend.open(&env->backend, port_name, type, &native_data);
    HAL_JUMP_IF_ERROR(status, error);

    used_port->native_data = native_data;
    if (hal_list_add(&env->used_ports, node)) {
        status = HAL_ERROR_BAD_DATA;
        goto error;
    }

    *handle = ((hal_handle_t) used_port);
    TRACE_INFO("New port %s assigned handle %u", port_name, *handle);

    pthread_mutex_unlock(&env->mutex);
    return HAL_SUCCESS;
error:
    if (NULL != node) {
        free(node);
    }
    if (NULL != native_data) {
        env->backend.close(&env->backend, port_name, type, native_data);
    }

    pthread_mutex_unlock(&env->mutex);

    return status;
}

void hal_close(hal_env_t* env, hal_handle_t handle) {
    HAL_CHECK_INITIALIZED_VOID(env);

    pthread_mutex_lock(&env->mutex);

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        goto end;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;

    if (env->backend.close == NULL) {
        TRACE_ERROR("BACKEND does not support CLOSE");
        goto end;
    }

    TRACE_INFO("closing port %s of type %d (handle %u)", used_port->port_name, used_port->type, handle);

    env->backend.close(&env->backend, used_port->port_name, used_port->type, used_port->native_data);
    hal_list_remove(&env->used_ports, node);

end:
    pthread_mutex_unlock(&env->mutex);
}

hal_error_t hal_port_property_probe(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    if (check_port_config_supported_for_type(key, type)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED, end);
    }

    if (env->backend.port_probe_prop == NULL) {
        TRACE_ERROR("BACKEND does not support PROBE PROP");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Probing port configuration for %s with type %d for key %d",
               port_name, type, key);

    status = env->backend.port_probe_prop(&env->backend, port_name, type, key);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_port_property_probe_handle(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_used_port_t* used_port;
    if (hal_find_port_from_handle(handle, &used_port)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    if (check_port_config_supported_for_type(key, used_port->type)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED, end);
    }

    if (env->backend.port_probe_prop == NULL) {
        TRACE_ERROR("BACKEND does not support PROBE PROP");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Probing port configuration for port %s with type %d (handle %u) for key %d",
               used_port->port_name, used_port->type, handle, key);

    status = env->backend.port_probe_prop(&env->backend, used_port->port_name, used_port->type, key);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_get_port_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, hal_prop_value_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_used_port_t* used_port;
    if (hal_find_port_from_handle(handle, &used_port)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    if (env->backend.port_get_prop == NULL) {
        TRACE_ERROR("BACKEND does not support GET PROP");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Reading configuration of port %s of type %d (handle %u): key=%d",
               used_port->port_name, used_port->type, handle,
               key);

    status = env->backend.port_get_prop(&env->backend,
                                        used_port->port_name,
                                        used_port->type,
                                        used_port->native_data,
                                        key, value);
end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_set_port_property(hal_env_t* env, hal_handle_t handle, hal_prop_key_t key, hal_prop_value_t value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_used_port_t* used_port;
    if (hal_find_port_from_handle(handle, &used_port)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    if (env->backend.port_set_prop == NULL) {
        TRACE_ERROR("BACKEND does not support SET PROP");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Configuring port %s of type %d (handle %u): key=%d value=0x%x",
               used_port->port_name, used_port->type, handle,
               key, value);

    status = env->backend.port_set_prop(&env->backend,
                                          used_port->port_name,
                                          used_port->type,
                                          used_port->native_data,
                                          key, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
