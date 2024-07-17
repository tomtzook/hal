#pragma once

#include "hal_types.h"
#include "hal_error.h"
#include "hal_config.h"
#include "hal_dio.h"
#include "hal_aio.h"
#include "hal_pwm.h"
#include "hal_quadrature.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_init(hal_env_t** env);
void hal_shutdown(hal_env_t* env);

hal_error_t hal_probe(hal_env_t* env, hal_id_t id, hal_port_type_t type);

hal_error_t hal_open(hal_env_t* env, hal_id_t id, hal_port_type_t type, hal_handle_t* handle);
void hal_close(hal_env_t* env, hal_handle_t handle);

hal_error_t hal_get_handle(hal_env_t* env, hal_id_t id, hal_handle_t* handle);
hal_error_t hal_get_info(hal_env_t* env, hal_id_t id, hal_port_info_t* info);

hal_error_t hal_iter_port_start(hal_env_t* env, hal_port_iter_t** iter);
hal_error_t hal_iter_port_next(hal_env_t* env, hal_port_iter_t* iter);
hal_error_t hal_iter_port_end(hal_env_t* env, hal_port_iter_t* iter);

#ifdef __cplusplus
}
#endif
