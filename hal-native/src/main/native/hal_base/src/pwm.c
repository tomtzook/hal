
#include <pthread.h>

#include <hal_error_handling.h>
#include <hal_internal.h>
#include <hal_pwm.h>



hal_error_t hal_pwm_get_duty_cycle(hal_env_t* env, hal_handle_t handle, float* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
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

hal_error_t hal_pwm_get_frequency(hal_env_t* env, hal_handle_t handle, float* value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status = HAL_SUCCESS;

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
    if (used_port->type != HAL_TYPE_PWM_OUTPUT) {
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    if (env->backend.pwm_get_frequency == NULL) {
        TRACE_ERROR("BACKEND does not support PWM GETFREQ");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Reading from PWM port %s (handle %u)", used_port->port_name, handle);

    status = env->backend.pwm_get_frequency(&env->backend, used_port->port_name, used_port->native_data, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}

hal_error_t hal_pwm_set_duty_cycle(hal_env_t* env, hal_handle_t handle, float value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status;

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
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

hal_error_t hal_pwm_set_frequency(hal_env_t* env, hal_handle_t handle, float value) {
    HAL_CHECK_INITIALIZED(env);

    pthread_mutex_lock(&env->mutex);

    hal_error_t status;

    hal_list_node_t* node;
    if (hal_find_port_node_from_handle(env, handle, &node)) {
        status = HAL_ERROR_BAD_HANDLE;
        goto end;
    }

    hal_used_port_t* used_port = (hal_used_port_t*) node->data;
    if (used_port->type != HAL_TYPE_PWM_OUTPUT) {
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    if (env->backend.pwm_set_frequency == NULL) {
        TRACE_ERROR("BACKEND does not support PWM SETFREQ");
        status = HAL_ERROR_UNSUPPORTED_OPERATION;
        goto end;
    }

    TRACE_INFO("Writing %f to PWM port %s (handle %u)", value, used_port->port_name, handle);

    status = env->backend.pwm_set_frequency(&env->backend, used_port->port_name, used_port->native_data, value);

end:
    pthread_mutex_unlock(&env->mutex);
    return status;
}
