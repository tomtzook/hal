#ifndef HAL_AIO_H
#define HAL_AIO_H

#include <stdint.h>
#include "hal_types.h"

typedef uint64_t aio_value_t;

hal_result_t hal_aio_init(hal_env_t* env, port_id_t port_id, port_dir_t port_dir, hal_handle_t* result_handle);
hal_result_t hal_aio_free(hal_env_t* env, hal_handle_t port_handle);

hal_result_t hal_aio_set(hal_env_t* env, hal_handle_t port_handle, aio_value_t value);
hal_result_t hal_aio_get(hal_env_t* env, hal_handle_t port_handle, aio_value_t* result_value);

#endif //HAL_AIO_H
