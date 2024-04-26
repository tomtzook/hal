#pragma once

#include <stdint.h>

#include "hal_types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_error_t;

typedef enum _hal_error_value {
    HAL_SUCCESS = 0,
    HAL_ERROR_NOT_INITIALIZED,
    HAL_ERROR_ALLOCATION_FAILED,
    HAL_ERROR_NOT_FOUND,
    HAL_ERROR_UNSUPPORTED_OPERATION,
    HAL_ERROR_TAKEN,
    HAL_ERROR_BAD_DATA,
    HAL_ERROR_BAD_ARGUMENT,
    HAL_ERROR_PERMISSIONS,
    HAL_ERROR_BAD_HANDLE,
    HAL_ERROR_TYPE_NOT_SUPPORTED,
    HAL_ERROR_ENVIRONMENT,
    HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED,
    HAL_ERROR_OPERATION_NOT_SUPPORTED_FOR_TYPE,
    HAL_ERROR_ITER_END,

    HAL_CUSTOM_ERROR_STATE = 0xffff
} hal_error_value_t;

const char* hal_strerror(hal_error_t error);


#define HAL_IS_SUCCESS(error_code) (HAL_SUCCESS == (error_code))
#define HAL_IS_ERROR(error_code) (HAL_SUCCESS != (error_code))

#ifdef __cplusplus
}
#endif
