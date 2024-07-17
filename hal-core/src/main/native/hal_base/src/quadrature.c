
#include <hal_quadrature.h>
#include "hal_internal.h"


hal_error_t hal_quadrature_get_position(hal_env_t* env, hal_handle_t handle, uint32_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->backend_port.type != HAL_TYPE_QUADRATURE) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.quadrature_get_pos == NULL) {
        TRACE_ERROR("BACKEND does not support QUADRATURE GETPOS");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Reading position from QUADRATURE port %u (handle %u)", port_node->port->identifier, handle);

    status = env->backend.funcs.quadrature_get_pos(env, &port_node->backend_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_quadrature_set_position(hal_env_t* env, hal_handle_t handle, uint32_t value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->backend_port.type != HAL_TYPE_QUADRATURE) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.quadrature_set_pos == NULL) {
        TRACE_ERROR("BACKEND does not support QUADRATURE SETPOS");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Writing position to QUADRATURE port %u (handle %u)", port_node->port->identifier, handle);

    status = env->backend.funcs.quadrature_set_pos(env, &port_node->backend_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_quadrature_get_period(hal_env_t* env, hal_handle_t handle, uint32_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->backend_port.type != HAL_TYPE_QUADRATURE) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.quadrature_get_period == NULL) {
        TRACE_ERROR("BACKEND does not support QUADRATURE GETPERIOD");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Reading period from QUADRATURE port %u (handle %u)", port_node->port->identifier, handle);

    status = env->backend.funcs.quadrature_get_period(env, &port_node->backend_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
