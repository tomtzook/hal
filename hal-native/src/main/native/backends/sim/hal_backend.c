
#include <malloc.h>
#include <memory.h>

#include <hal.h>
#include <hal_error_handling.h>
#include <hal_backend.h>

#include "hal_sim_internal.h"


static hal_error_t port_iter_struct_size(hal_env_t* env) {
    return sizeof(size_t*);
}

static hal_error_t port_iter_start(hal_env_t* env, hal_port_iter_t* iter) {
    hal_backend_t* backend = hal_get_backend(env);

    size_t* index = (size_t*)(iter->_iter_data);
    *index = 0;

    halsim_port_t* port;
    if (find_next_sim_port_from_index(backend, 0, &port, index)) {
        return HAL_ERROR_NOT_FOUND;
    }

    strcpy(iter->name, port->name);
    iter->supported_types = port->supported_types;

    return HAL_SUCCESS;
}

static hal_error_t port_iter_next(hal_env_t* env, hal_port_iter_t* iter) {
    hal_backend_t* backend = hal_get_backend(env);

    size_t* index = (size_t*)(iter->_iter_data);

    halsim_port_t* port;
    if (find_next_sim_port_from_index(backend, *index + 1, &port, index)) {
        return HAL_ERROR_NOT_FOUND;
    }

    strcpy(iter->name, port->name);
    iter->supported_types = port->supported_types;

    return HAL_SUCCESS;
}

static hal_error_t probe(hal_env_t* env, const char* port_name, uint32_t* types) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    *types = port->supported_types;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t open(hal_env_t* env, const char* port_name, hal_port_type_t type, void* data) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->open_callback != NULL) {
        status = port->open_callback(env, (halsim_port_handle_t)index, type);
        HAL_JUMP_IF_ERROR(status, end);
    }

    port->is_open = 1;
    port->open_type = type;
    memset(&port->value, 0, sizeof(port->value));

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t close(hal_env_t* env, const char* port_name, hal_port_type_t type, void* data) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->close_callback != NULL) {
        status = port->close_callback(env, (halsim_port_handle_t)index, type);
        HAL_JUMP_IF_ERROR(status, end);
    }

    port->is_open = 0;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t port_probe_prop(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key, uint32_t* flags) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;

    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (key >= sizeof(port->props_config)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_prop_config_t* config = port->props_config + key;
    if (!config->configured) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    *flags = config->flags;

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t port_get_prop(hal_env_t* env, const char* port_name, hal_port_type_t type, void* data,
                          hal_prop_key_t key, hal_prop_value_t* value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;

    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }
    halsim_port_handle_t handle = (halsim_port_handle_t) index;

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (key >= sizeof(port->props_config)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_prop_config_t* config = port->props_config + key;
    if (!config->configured) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (config->get_callback != NULL) {
        hal_prop_value_t _value;
        status = config->get_callback(env, handle, key, &_value);
        if (HAL_IS_SUCCESS(status)) {
            *value = _value;
        }
    } else {
        hal_prop_value_t* prop_value = port->props_values + key;
        *value = *prop_value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t port_set_prop(hal_env_t* env, const char* port_name, hal_port_type_t type, void* data,
                          hal_prop_key_t key, hal_prop_value_t value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;

    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }
    halsim_port_handle_t handle = (halsim_port_handle_t) index;

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (key >= sizeof(port->props_config)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_prop_config_t* config = port->props_config + key;
    if (!config->configured) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (config->set_callback != NULL) {
        status = config->set_callback(env, handle, key, value);
    } else {
        hal_prop_value_t* prop_value = port->props_values + key;
        *prop_value = value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t dio_get(hal_env_t* env, const char* port_name, void* data, hal_dio_value_t* value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->dio_callbacks.get_value != NULL) {
        status = port->dio_callbacks.get_value(env, (halsim_port_handle_t)index, value);
        HAL_JUMP_IF_ERROR(status, end);
    } else {
        *value = port->value.dio_value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t dio_set(hal_env_t* env, const char* port_name, void* data, hal_dio_value_t value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->dio_callbacks.set_value != NULL) {
        status = port->dio_callbacks.set_value(env, (halsim_port_handle_t)index, value);
        HAL_JUMP_IF_ERROR(status, end);
    } else {
        port->value.dio_value = value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t aio_get(hal_env_t* env, const char* port_name, void* data, uint32_t* value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->aio_callbacks.get_value != NULL) {
        status = port->aio_callbacks.get_value(env, (halsim_port_handle_t)index, value);
        HAL_JUMP_IF_ERROR(status, end);
    } else {
        *value = port->value.aio_value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t aio_set(hal_env_t* env, const char* port_name, void* data, uint32_t value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->aio_callbacks.set_value != NULL) {
        status = port->aio_callbacks.set_value(env, (halsim_port_handle_t)index, value);
        HAL_JUMP_IF_ERROR(status, end);
    } else {
        port->value.aio_value = value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t pwm_getduty(hal_env_t* env, const char* port_name, void* data, uint32_t* value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->pwm_callbacks.get_value != NULL) {
        status = port->pwm_callbacks.get_value(env, (halsim_port_handle_t)index, value);
        HAL_JUMP_IF_ERROR(status, end);
    } else {
        *value = port->value.pwm_value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

static hal_error_t pwm_setduty(hal_env_t* env, const char* port_name, void* data, uint32_t value) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* sim_data = get_global_data(backend);

    pthread_mutex_lock(&sim_data->mutex);

    hal_error_t status = HAL_SUCCESS;
    size_t index;
    if (find_sim_port_index(backend, port_name, &index)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&sim_data->ports, index, (void**)&port)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_NOT_FOUND, end);
    }

    if (port->pwm_callbacks.set_value != NULL) {
        status = port->pwm_callbacks.set_value(env, (halsim_port_handle_t)index, value);
        HAL_JUMP_IF_ERROR(status, end);
    } else {
        port->value.pwm_value = value;
    }

end:
    pthread_mutex_unlock(&sim_data->mutex);
    return status;
}

halsim_data_t* get_global_data(hal_backend_t* env) {
    return (halsim_data_t*) env->data;
}

halsim_data_t* get_global_data_from_env(hal_env_t* env) {
    hal_backend_t* backend = hal_get_backend(env);
    return get_global_data(backend);
}

int find_sim_port_index(hal_backend_t* env, const char* port_name, size_t* index) {
    halsim_data_t* data = get_global_data(env);

    for (size_t i = 0; i < data->ports.capacity; ++i) {
        halsim_port_t* port;
        if (!hal_descriptor_table_get(&data->ports, i, (void**) &port)) {
            // has such port
            if (0 == strcmp(port->name, port_name)) {
                *index = i;
                return 0;
            }
        }
    }

    return 1;
}

int find_next_sim_port_from_index(hal_backend_t* env, size_t start_index, halsim_port_t** port_out, size_t* index) {
    halsim_data_t* data = get_global_data(env);

    size_t _index;
    if (hal_descriptor_table_find_next(&data->ports, start_index, &_index)) {
        return 1;
    }

    halsim_port_t* port;
    if (hal_descriptor_table_get(&data->ports, _index, (void**)&port)) {
        return 1;
    }

    *port_out = port;
    *index = _index;
    return 0;
}

int find_sim_port_from_handle(hal_backend_t* env, halsim_port_handle_t handle, halsim_port_t** port_out, size_t* index_out) {
    halsim_data_t* data = get_global_data(env);

    halsim_port_t* port;
    size_t index = (size_t) handle;
    if (hal_descriptor_table_get(&data->ports, index, (void**)&port)) {
        return 1;
    }

    if (port_out != NULL) {
        *port_out = port;
    }
    if (index_out != NULL) {
        *index_out = index;
    }
    return 0;
}

hal_error_t hal_backend_init(hal_env_t* env) {
    hal_backend_t* backend = hal_get_backend(env);

    backend->name = "sim";
    backend->port_iter_struct_size = port_iter_struct_size;
    backend->port_iter_start = port_iter_start;
    backend->port_iter_next = port_iter_next;
    backend->probe = probe;
    backend->open = open;
    backend->close = close;
    backend->port_probe_prop = port_probe_prop;
    backend->port_get_prop = port_get_prop;
    backend->port_set_prop = port_set_prop;
    backend->dio_get = dio_get;
    backend->dio_set = dio_set;
    backend->aio_get = aio_get;
    backend->aio_set = aio_set;
    backend->pwm_get_duty = pwm_getduty;
    backend->pwm_set_duty = pwm_setduty;

    hal_error_t status = HAL_SUCCESS;
    int table_initialized = 0;
    int mutexattr_initialized = 0;
    int mutex_initialized = 0;

    halsim_data_t* data = (halsim_data_t*) malloc(sizeof(halsim_data_t));
    HAL_CHECK_ALLOCATED(data, error);

    if (hal_descriptor_table_init(&data->ports, MAX_PORTS)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_DATA, error);
    }
    table_initialized = 1;

    if (pthread_mutexattr_init(&data->mutex_attr)) {
        TRACE_SYSTEM_ERROR();
        HAL_JUMP_IF_ERROR(HAL_ERROR_ENVIRONMENT, error);
    }
    mutexattr_initialized = 1;

    if (pthread_mutexattr_settype(&data->mutex_attr, PTHREAD_MUTEX_RECURSIVE)) {
        TRACE_SYSTEM_ERROR();
        HAL_JUMP_IF_ERROR(HAL_ERROR_ENVIRONMENT, error);
    }

    if (pthread_mutex_init(&data->mutex, &data->mutex_attr)) {
        TRACE_SYSTEM_ERROR();
        HAL_JUMP_IF_ERROR(HAL_ERROR_ENVIRONMENT, error);
    }
    mutex_initialized = 1;

    backend->data = (void*) data;

    return HAL_SUCCESS;
error:
    if (table_initialized) {
        hal_descriptor_table_free(&data->ports);
    }
    if (mutex_initialized) {
        pthread_mutex_destroy(&data->mutex);
    }
    if (mutexattr_initialized) {
        pthread_mutexattr_destroy(&data->mutex_attr);
    }
    if(data != NULL) {
        free(data);
    }

    return status;
}

void hal_backend_shutdown(hal_env_t* env) {
    hal_backend_t* backend = hal_get_backend(env);
    halsim_data_t* data = (halsim_data_t*) backend->data;
    free(data);
}
