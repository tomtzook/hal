#pragma once

#include "ports_sys.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_pwm_open(hal_env_t* env, hal_port_t port, hal_handle_t* handle);
void hal_pwm_close(hal_env_t* env, hal_handle_t handle);
hal_error_t hal_pwm_get(hal_env_t* env, hal_handle_t handle, hal_pwm_value_t* value);
hal_error_t hal_pwm_set(hal_env_t* env, hal_handle_t handle, hal_pwm_value_t value);
hal_error_t hal_pwm_get_frequency(hal_env_t* env, hal_handle_t handle, float* value);

hal_pwm_value_t hal_pwm_max_value(hal_env_t* env);

#ifdef __cplusplus
}
#endif
