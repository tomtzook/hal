#pragma once

#include "hal_types.h"
#include "hal_error.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef enum _hal_dio_value {
    HAL_DIO_LOW,
    HAL_DIO_HIGH
} hal_dio_value_t;

hal_error_t hal_dio_get(hal_env_t* env, hal_handle_t handle, hal_dio_value_t* value);
hal_error_t hal_dio_set(hal_env_t* env, hal_handle_t handle, hal_dio_value_t value);

#ifdef __cplusplus
}
#endif
