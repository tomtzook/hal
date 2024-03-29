#pragma once

#include "hal_error.h"
#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_pwm_get_duty_cycle(hal_env_t* env, hal_handle_t handle, float* value);
hal_error_t hal_pwm_get_frequency(hal_env_t* env, hal_handle_t handle, float* value);

hal_error_t hal_pwm_set_duty_cycle(hal_env_t* env, hal_handle_t handle, float value);
hal_error_t hal_pwm_set_frequency(hal_env_t* env, hal_handle_t handle, float value);

#ifdef __cplusplus
}
#endif
