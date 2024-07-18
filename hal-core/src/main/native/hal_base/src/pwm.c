
#include <pthread.h>

#include <hal_error_handling.h>
#include <hal_pwm.h>

#include "hal_internal.h"



hal_error_t hal_pwm_get_duty_cycle(hal_env_t* env, hal_handle_t handle, uint32_t* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->backend_port.type != HAL_TYPE_PWM_OUTPUT) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.pwm_get_duty == NULL) {
        TRACE_ERROR("BACKEND does not support PWM GETDUTY");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Reading from PWM port 0x%x (handle 0x%x)", port_node->port->identifier, handle);

    status = env->backend.funcs.pwm_get_duty(env, &port_node->backend_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_pwm_set_duty_cycle(hal_env_t* env, hal_handle_t handle, uint32_t value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_open_port_node_t* port_node;
    if (hal_find_port_from_handle(env, handle, &port_node, NULL)) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_BAD_HANDLE, end);
    }

    if (port_node->backend_port.type != HAL_TYPE_PWM_OUTPUT) {
        HAL_JUMP_IF_ERROR(HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE, end);
    }

    if (env->backend.funcs.pwm_set_duty == NULL) {
        TRACE_ERROR("BACKEND does not support PWM SETDUTY");
        HAL_JUMP_IF_ERROR(HAL_ERROR_UNSUPPORTED_OPERATION, end);
    }

    TRACE_INFO("Writing 0x%x to PWM port 0x%x (handle 0x%x)", value, port_node->port->identifier, handle);

    status = env->backend.funcs.pwm_set_duty(env, &port_node->backend_port, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
