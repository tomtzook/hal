
#include <hal_dio.h>
#include "hal_internal.h"


hal_error_t hal_dio_get(hal_env_t* env, hal_handle_t handle, hal_dio_value_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->open_port.type != HAL_TYPE_DIGITAL_OUTPUT && port_node->open_port.type != HAL_TYPE_DIGITAL_INPUT) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.dio_get == NULL) {
        TRACE_ERROR("BACKEND does not support DIO GET");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Reading from DIO port %s (handle %u)", port_node->open_port.name, handle);

    status = env->backend.dio_get(env, &port_node->open_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_dio_set(hal_env_t* env, hal_handle_t handle, hal_dio_value_t value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->open_port.type != HAL_TYPE_DIGITAL_OUTPUT) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.dio_set == NULL) {
        TRACE_ERROR("BACKEND does not support DIO SET");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Writing %d to DIO port %s (handle %u)", value, port_node->open_port.name, handle);

    status = env->backend.dio_set(env, &port_node->open_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
