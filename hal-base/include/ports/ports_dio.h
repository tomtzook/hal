#pragma once

#include "ports_sys.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_dio_open(hal_env_t* env, hal_port_t port, hal_port_dir_t dir, hal_handle_t* handle);

void hal_dio_close(hal_env_t* env, hal_handle_t handle);

hal_error_t hal_dio_get(hal_env_t* env, hal_handle_t handle, hal_dio_value_t* value);

hal_error_t hal_dio_set(hal_env_t* env, hal_handle_t handle, hal_dio_value_t value);

#ifdef __cplusplus
}
#endif
