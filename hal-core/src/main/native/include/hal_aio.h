#pragma once

#include <hal_error.h>
#include <hal_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// value is in ADC units, from 0 to a defined max. See HAL_CONFIG_ANALOG_MAX_VALUE

hal_error_t hal_aio_get(hal_env_t* env, hal_handle_t handle, uint32_t* value);
hal_error_t hal_aio_set(hal_env_t* env, hal_handle_t handle, uint32_t value);

#ifdef __cplusplus
}
#endif
