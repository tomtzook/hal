#pragma once

#include "hal_error.h"
#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// value is in microseconds pulse length, from 0 to a defined max. See HAL_CONFIG_PWM_FREQUENCY

hal_error_t hal_pwm_get_duty_cycle(hal_env_t* env, hal_handle_t handle, uint32_t* value);
hal_error_t hal_pwm_set_duty_cycle(hal_env_t* env, hal_handle_t handle, uint32_t value);

#ifdef __cplusplus
}
#endif
