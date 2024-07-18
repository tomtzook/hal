#include <malloc.h>
#include <string.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal_control.h>

#include "hal_sim_internal.h"


hal_error_t halsim_create_port(hal_env_t* env, hal_id_t id, halsim_port_handle_t* port_handle) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port = NULL;
    size_t index;
    int port_registered = 0;

    if (!find_sim_port_index(hal_get_backend(env), id, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_TAKEN, end);
    }

    status = halcontrol_register_port(env, id);
    HAL_JUMP_IF_ERROR(status, end);
    port_registered = 1;

    status = halcontrol_config_backend_allocation_size(env, id, sizeof(halsim_port_t**));
    HAL_JUMP_IF_ERROR(status, end);

    port = (halsim_port_t*) malloc(sizeof(halsim_port_t));
    HAL_CHECK_ALLOCATED(port, end);

    memset(port, 0, sizeof(halsim_port_t));
    port->identifier = id;

    if (hal_descriptor_table_add(&sim_data->ports, port, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_DATA, end);
    }

    halsim_port_handle_t handle = (halsim_port_handle_t) index;
    port->handle = handle;
    *port_handle = handle;

    TRACE_INFO("Created new port 0x%x with handle 0x%x", id, *port_handle);
end:
    if (port != NULL && HAL_IS_ERROR(status)) {
        free(port);
    }
    if (port_registered && HAL_IS_ERROR(status)) {
        halcontrol_unregister_port(env, id);
    }

    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_get_handle(hal_env_t* env, hal_id_t id, halsim_port_handle_t* port_handle) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(hal_get_backend(env), id, &index)) {
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

    TRACE_INFO("Configuring types for port 0x%x (handle 0x%x)", port->identifier, port_handle);

    HAL_JUMP_IF_ERROR(halcontrol_config_port(env, port->identifier, types, HAL_OPT_UINT64), end);

    port->supported_types = types;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_config_add_conflicting_port(hal_env_t* env, halsim_port_handle_t port_handle, hal_id_t conflicting_id) {
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

    TRACE_INFO("Adding new conflict port for 0x%x (handle 0x%x): 0x%x", port->identifier, port_handle, conflicting_id);

    status = halcontrol_config_add_conflicting_port(env, port->identifier, conflicting_id);
    HAL_JUMP_IF_ERROR(status, end);

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

    TRACE_INFO("Configuring callbacks for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Configuring prop %d for port 0x%x (handle 0x%x)",
               key, port->identifier, port_handle);

    HAL_JUMP_IF_ERROR(halcontrol_config_port_append(env, port->identifier, HAL_OPT_UINT32, key), end);

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

    TRACE_INFO("Configuring callbacks for prop %d for port 0x%x (handle 0x%x)",
               key, port->identifier, port_handle);

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

    TRACE_INFO("Getting value for prop %d for port 0x%x (handle 0x%x)",
               key, port->identifier, port_handle);

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

    TRACE_INFO("Setting value for prop %d for port 0x%x (handle 0x%x)",
               key, port->identifier, port_handle);

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

    TRACE_INFO("Setting DIO callbacks for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Getting DIO value for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Setting DIO value for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Setting AIO callbacks for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Getting AIO value for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Setting AIO value for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Setting PWM callbacks for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Getting PWM value for port 0x%x (handle 0x%x)", port->identifier, port_handle);

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

    TRACE_INFO("Setting PWM value for port 0x%x (handle 0x%x)", port->identifier, port_handle);

    port->value.pwm_value = value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_quadrature_config_callbacks(hal_env_t* env, halsim_port_handle_t port_handle,
                                               halsim_quadrature_get_position_callback_t get_position_callback,
                                               halsim_quadrature_set_position_callback_t set_position_callback,
                                               halsim_quadrature_get_period_callback_t get_period_callback) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting PWM callbacks for port 0x%x (handle 0x%x)", port->identifier, port_handle);

    port->quadrature_callbacks.get_position = get_position_callback;
    port->quadrature_callbacks.set_position = set_position_callback;
    port->quadrature_callbacks.get_period = get_period_callback;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_quadrature_get_position(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Getting QUADRATURE position for port 0x%x (handle 0x%x)", port->identifier, port_handle);

    *value = port->value.quadrature.position;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_quadrature_set_position(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Setting QUADRATURE position for port 0x%x (handle 0x%x)", port->identifier, port_handle);

    port->value.quadrature.position = value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_quadrature_get_period(hal_env_t* env, halsim_port_handle_t port_handle, uint32_t* value) {
    halsim_data_t* sim_data = get_global_data_from_env(env);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    halsim_port_t* port;
    if (find_sim_port_from_handle(hal_get_backend(env), port_handle, &port, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    TRACE_INFO("Getting QUADRATURE period for port 0x%x (handle 0x%x)", port->identifier, port_handle);

    *value = port->value.quadrature.period;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}
