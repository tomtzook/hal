
#include <pthread.h>

#include <hal_error_handling.h>
#include <hal_internal.h>
#include <hal_pwm.h>



hal_error_t hal_pwm_get_duty_cycle(hal_env_t* env, hal_handle_t handle, float* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_used_port_t* used_port;
    if (hal_find_port_from_handle(handle, &used_port)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    if (used_port->type != HAL_TYPE_PWM_OUTPUT) {
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    if (env->backend.pwm_get_duty == NULL) {
        TRACE_ERROR("BACKEND does not support PWM GETDUTY");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Reading from PWM port %s (handle %u)", used_port->port_name, handle);

    status = env->backend.pwm_get_duty(&env->backend, used_port->port_name, used_port->native_data, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_pwm_set_duty_cycle(hal_env_t* env, hal_handle_t handle, float value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status;

    hal_used_port_t* used_port;
    if (hal_find_port_from_handle(handle, &used_port)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    if (used_port->type != HAL_TYPE_PWM_OUTPUT) {
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    if (env->backend.pwm_set_duty == NULL) {
        TRACE_ERROR("BACKEND does not support PWM SETDUTY");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Writing %f to PWM port %s (handle %u)", value, used_port->port_name, handle);

    status = env->backend.pwm_set_duty(&env->backend, used_port->port_name, used_port->native_data, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
