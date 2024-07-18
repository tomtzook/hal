#pragma once

#include "hal_error.h"
#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_quadrature_get_position(hal_env_t* env, hal_handle_t handle, uint32_t* value);
hal_error_t hal_quadrature_set_position(hal_env_t* env, hal_handle_t handle, uint32_t value);
hal_error_t hal_quadrature_get_period(hal_env_t* env, hal_handle_t handle, uint32_t* value);

#ifdef __cplusplus
}
#endif