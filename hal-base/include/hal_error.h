#pragma once

#include <stdint.h>

#include "hal_types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_error_t;

typedef enum _error_value {
    HAL_SUCCESS = 0,
    HAL_ERROR_NOT_INITIALIZED,
    HAL_ERROR_NOT_ALLOCATED,
    HAL_ERROR_NOT_FOUND,
    HAL_ERROR_UNSUPPORTED_OPERATION,
    HAL_ERROR_TAKEN,
    HAL_ERROR_BAD_DATA,
    HAL_ERROR_BAD_ARGUMENT
} error_value_t;

#define HAL_IS_ERROR(error_code) (HAL_SUCCESS != (error_code))

#define HAL_CHECK_ALLOCATED(ptr) \
    do {                            \
        if(NULL == ptr) { \
            return HAL_ERROR_NOT_ALLOCATED; \
        } \
    } while(0)

#ifdef __cplusplus
}
#endif
