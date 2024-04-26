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

    *port_handle = (halsim_port_handle_t) index;
    TRACE_INFO("Created new port %s with handle %u", port->name, *port_handle);
end:
    if (port != NULL && HAL_IS_ERROR(status)) {
        free(port);
    }

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

    TRACE_INFO("Configuring types for port %s (handle %u)", port->name, port_handle);

    port->supported_types = types;

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

hal_error_t halsim_port_get_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, hal_prop_value_t* value) {
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

    hal_prop_value_t* prop_value = port->props_values + key;
    *value = *prop_value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

hal_error_t halsim_port_set_prop(hal_env_t* env, halsim_port_handle_t port_handle, hal_prop_key_t key, hal_prop_value_t value) {
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

    hal_prop_value_t* prop_value = port->props_values + key;
    *prop_value = value;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}