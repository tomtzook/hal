#ifndef HAL_TYPES_H
#define HAL_TYPES_H

#include <stdint.h>

typedef int16_t hal_handle_t;

#define HAL_INVALID_HANDLE -1

typedef int16_t port_connection_t;

typedef enum {
    OUTPUT,
    INPUT
} port_dir_t;

typedef struct {
    port_connection_t connection;
} port_id_t;

typedef enum {
    HAL_SUCCESS = 0,
    HAL_NOT_INITIALIZED = 1,
    HAL_ARGUMENT_ERROR = 2,
    HAL_PORT_NOT_INITIALIZED = 3,
    HAL_IO_ERROR = 4,
    HAL_STORE_ERROR = 5,
    HAL_MEMORY_ALLOCATION_ERROR = 6,
    HAL_INITIALIZATION_ERROR = 7,
    HAL_ALREADY_INITIALIZED = 8,
    HAL_DIO_INITIALIZATION_ERROR = 9
} hal_result_t;

#endif //HAL_HAL_TYPES_H
