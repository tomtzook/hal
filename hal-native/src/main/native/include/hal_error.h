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
    HAL_ERROR_UNKNOWN
} hal_error_value_t;

const char* hal_strerror(hal_error_t error);


#define HAL_IS_ERROR(error_code) (HAL_SUCCESS != (error_code))

#define HAL_JUMP_IF_ERROR(error_code, label) \
    do { \
         if (HAL_IS_ERROR(error_code)) goto label;  \
    } while(0)

#define HAL_RETURN_IF_ERROR(...) \
    do {                         \
         hal_error_t _status = __VA_ARGS__;  \
         if (HAL_IS_ERROR(_status)) return _status;  \
    } while(0)

#define HAL_CHECK_ALLOCATED(ptr) \
    do {                            \
        if(NULL == ptr) { \
            return HAL_ERROR_ALLOCATION_FAILED; \
        } \
    } while(0)

#define HAL_CHECK_INITIALIZED(ptr) \
    do {                            \
        if(NULL == ptr) { \
            return HAL_ERROR_NOT_INITIALIZED; \
        } \
    } while(0)

#define HAL_CHECK_INITIALIZED_VOID(ptr) \
    do {                            \
        if(NULL == ptr) { \
            return; \
        } \
    } while(0)

#ifdef __cplusplus
}
#endif
