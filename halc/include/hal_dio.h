#ifndef HAL_DIO_H
#define HAL_DIO_H

#include "hal_types.h"

typedef enum {
    HIGH,
    LOW
} dio_value_t;

hal_result_t hal_dio_init_module(hal_env_t* env);
void hal_dio_free_module(hal_env_t* env);

hal_result_t hal_dio_init(hal_env_t* env, port_id_t port_id, port_dir_t port_dir, hal_handle_t* port_handle);
hal_result_t hal_dio_free(hal_env_t* env, hal_handle_t port_handle);

hal_result_t hal_dio_set(hal_env_t* env, hal_handle_t port_handle, dio_value_t dio_value);
hal_result_t hal_dio_get(hal_env_t* env, hal_handle_t port_handle, dio_value_t* dio_value);
hal_result_t hal_dio_pulse(hal_env_t* env, hal_handle_t port_handle, uint64_t length_us);

#endif //HAL_DIO_H
