#ifndef HAL_TYPES_H
#define HAL_TYPES_H

#include <stdint.h>

#define HAL_INVALID_HANDLE -1

typedef struct hal_env hal_env_t;

typedef int16_t hal_handle_t;
typedef int16_t port_id_t;

typedef enum port_dir {
    PORT_DIR_OUTPUT,
    PORT_DIR_INPUT
} port_dir_t;

typedef enum hal_result {
    HAL_SUCCESS = 0,
    HAL_NOT_INITIALIZED = 1,
    HAL_ARGUMENT_ERROR = 2,
    HAL_PORT_NOT_INITIALIZED = 3,
    HAL_HARDWARE_ERROR = 4,
    HAL_STORE_ERROR = 5,
    HAL_MEMORY_ALLOCATION_ERROR = 6,
    HAL_INITIALIZATION_ERROR = 7,
    HAL_ALREADY_INITIALIZED = 8,
    HAL_UNSUPPORTED_OPERATION = 9,
    HAL_PORT_NOT_FOUND = 10,

    HAL_UNKNOWN_ERROR = 1000
} hal_result_t;

#endif //HAL_HAL_TYPES_H
