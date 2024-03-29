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
    HAL_ERROR_ENVIRONMENT
} hal_error_value_t;

const char* hal_strerror(hal_error_t error);


#define HAL_IS_ERROR(error_code) (HAL_SUCCESS != (error_code))

#ifdef __cplusplus
}
#endif
