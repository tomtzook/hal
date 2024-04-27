#include <malloc.h>
#include <string.h>

#include <hal_error_handling.h>
#include <hal_backend.h>

#include "hal_sim_internal.h"


hal_error_t halsim_create_port(hal_env_t* env, const char* name, halsim_port_handle_t* port_handle) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port = NULL;
    size_t index;

    if (!find_sim_port_index(hal_get_backend(env), name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_TAKEN, end);
    }

    port = (halsim_port_t*) malloc(sizeof(halsim_port_t));
    HAL_CHECK_ALLOCATED(port, end);

    memset(port, 0, sizeof(halsim_port_t));
    strcpy(port->name, name);

    if (hal_descriptor_table_add(&sim_data->ports, port, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_DATA, end);
    }

    halsim_port_handle_t handle = (halsim_port_handle_t) index;
    port->handle = handle;
    *port_handle = handle;
    TRACE_INFO("Created new port %s with handle %u", port->name, *port_handle);
end:
    if (port != NULL && HAL_IS_ERROR(status)) {
        free(port);
    }

    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_get_handle(hal_env_t* env, const char* name, halsim_port_handle_t* port_handle) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(hal_get_backend(env), name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    *port_handle = (halsim_port_handle_t) index;
end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_config_port_types(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t types) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port->is_open) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_BAD_STATE, end);
    }

    TRACE_INFO("Configuring types for port %s (handle %u)", port->name, port_handle);

    port->supported_types = types;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_config_port_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                         halsim_open_callback_t open_callback,
                                         halsim_close_callback_t close_callback) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Configuring callbacks for port %s (handle %u)", port->name, port_handle);

    port->open_callback = open_callback;
    port->close_callback = close_callback;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_config_port_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, uint32_t flags) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (key >= sizeof(port->props_config)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    TRACE_INFO("Configuring prop %d for port %s (handle %u)",
               key, port->name, port_handle);

    halsim_port_prop_config_t* config = port->props_config + key;
    if (config->configured) {
        TRACE_DEBUG("Prop %d already configured, overriding settings", key);
    }

    config->configured = 1;
    config->key = key;
    config->flags = flags;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_config_port_prop_callbacks(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key,
                                              halsim_get_prop_callback_t get_callback,
                                              halsim_set_prop_callback_t set_callback) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (key >= sizeof(port->props_config)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    TRACE_INFO("Configuring callbacks for prop %d for port %s (handle %u)",
               key, port->name, port_handle);

    halsim_port_prop_config_t* config = port->props_config + key;
    if (!config->configured) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    config->get_callback = get_callback;
    config->set_callback = set_callback;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_port_get_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, uint32_t* value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (key >= sizeof(port->props_config)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    TRACE_INFO("Getting value for prop %d for port %s (handle %u)",
               key, port->name, port_handle);

    halsim_port_prop_config_t* config = port->props_config + key;
    if (!config->configured) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    uint32_t* prop_value = port->props_values + key;
    *value = *prop_value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_port_set_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, uint32_t value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (key >= sizeof(port->props_config)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    TRACE_INFO("Setting value for prop %d for port %s (handle %u)",
               key, port->name, port_handle);

    halsim_port_prop_config_t* config = port->props_config + key;
    if (!config->configured) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    uint32_t* prop_value = port->props_values + key;
    *prop_value = value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_dio_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                        halsim_dio_get_value_callback_t get_value_callback,
                                        halsim_dio_set_value_callback_t set_value_callback) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting DIO callbacks for port %s (handle %u)", port->name, port_handle);

    port->dio_callbacks.get_value = get_value_callback;
    port->dio_callbacks.set_value = set_value_callback;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_dio_get_value(hal_env_t* env, halsim_port_handle_t port_handle, hal_dio_value_t* value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Getting DIO value for port %s (handle %u)", port->name, port_handle);

    *value = port->value.dio_value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_dio_set_value(hal_env_t* env, halsim_port_handle_t port_handle, hal_dio_value_t value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting DIO value for port %s (handle %u)", port->name, port_handle);

    port->value.dio_value = value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_aio_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                        halsim_aio_get_value_callback_t get_value_callback,
                                        halsim_aio_set_value_callback_t set_value_callback) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting AIO callbacks for port %s (handle %u)", port->name, port_handle);

    port->aio_callbacks.get_value = get_value_callback;
    port->aio_callbacks.set_value = set_value_callback;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_aio_get_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Getting AIO value for port %s (handle %u)", port->name, port_handle);

    *value = port->value.aio_value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_aio_set_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting AIO value for port %s (handle %u)", port->name, port_handle);

    port->value.aio_value = value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_pwm_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                        halsim_pwm_get_value_callback_t get_value_callback,
                                        halsim_pwm_set_value_callback_t set_value_callback) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting PWM callbacks for port %s (handle %u)", port->name, port_handle);

    port->pwm_callbacks.get_value = get_value_callback;
    port->pwm_callbacks.set_value = set_value_callback;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_pwm_get_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Getting PWM value for port %s (handle %u)", port->name, port_handle);

    *value = port->value.pwm_value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_pwm_set_value(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting PWM value for port %s (handle %u)", port->name, port_handle);

    port->value.pwm_value = value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}
