#pragma once

#include <hal_error.h>
#include <hal_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_aio_value_t;

hal_error_t hal_aio_get(hal_env_t* env, hal_handle_t handle, hal_aio_value_t* value);
hal_error_t hal_aio_set(hal_env_t* env, hal_handle_t handle, hal_aio_value_t value);

#ifdef __cplusplus
}
#endif
