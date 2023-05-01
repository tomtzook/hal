
#include "hal_internal.h"
#include "hal_dio.h"


hal_error_t hal_dio_get(hal_env_t* env, hal_handle_t handle, hal_dio_value_t* value) {
    HAL_CHECK_INITIALIZED(env);

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        return HAL_ERROR_BAD_HANDLE;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
    if (used_port->type != HAL_TYPE_DIGITAL_OUTPUT && used_port->type != HAL_TYPE_DIGITAL_INPUT) {
        TRACE_ERROR("BACKEND does not support DIO GET");
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    if (env->backend.dio_get == NULL) {
        TRACE_ERROR("BACKEND does not support DIO GET");
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    TRACE_INFO("Reading from DIO port %s (handle %u)", used_port->port_name, handle);

    hal_error_t status = env->backend.dio_get(&env->backend, used_port->port_name, used_port->native_data, value);
    if (HAL_IS_ERROR(status)) {
        return status;
    }

    return HAL_SUCCESS;
}

hal_error_t hal_dio_set(hal_env_t* env, hal_handle_t handle, hal_dio_value_t value) {
    HAL_CHECK_INITIALIZED(env);

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        return HAL_ERROR_BAD_HANDLE;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
    if (used_port->type != HAL_TYPE_DIGITAL_OUTPUT) {
        TRACE_ERROR("BACKEND does not support DIO SET");
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    if (env->backend.dio_set == NULL) {
        TRACE_ERROR("BACKEND does not support DIO SET");
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    TRACE_INFO("Writing %d to DIO port %s (handle %u)", value, used_port->port_name, handle);

    hal_error_t status = env->backend.dio_set(&env->backend, used_port->port_name, used_port->native_data, value);
    if (HAL_IS_ERROR(status)) {
        return status;
    }

    return HAL_SUCCESS;
}
