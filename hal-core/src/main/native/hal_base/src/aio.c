
#include <hal_aio.h>
#include "hal_internal.h"


hal_error_t hal_aio_get(hal_env_t* env, hal_handle_t handle, uint32_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->backend_port.type != HAL_TYPE_ANALOG_OUTPUT && port_node->backend_port.type != HAL_TYPE_ANALOG_INPUT) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.aio_get == NULL) {
        TRACE_ERROR("BACKEND does not support AIO GET");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Reading from AIO port %u (handle %u)", port_node->port->identifier, handle);

    status = env->backend.funcs.aio_get(env, &port_node->backend_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_aio_set(hal_env_t* env, hal_handle_t handle, uint32_t value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->backend_port.type != HAL_TYPE_ANALOG_OUTPUT) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.aio_set == NULL) {
        TRACE_ERROR("BACKEND does not support AIO SET");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Writing %d to AIO port %u (handle %u)", value, port_node->port->identifier, handle);

    status = env->backend.funcs.aio_set(env, &port_node->backend_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
