#pragma once

#include "hal_types.h"
#include "hal_error.h"
#include "hal_config.h"
#include "hal_dio.h"
#include "hal_aio.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_init(hal_env_t** env);
void hal_shutdown(hal_env_t* env);

hal_error_t hal_probe(hal_env_t* env, const char* port_name, hal_port_type_t type);

hal_error_t hal_open(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_handle_t* handle);
void hal_close(hal_env_t* env, hal_handle_t handle);

#ifdef __cplusplus
}
#endif
