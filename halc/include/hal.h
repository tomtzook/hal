#ifndef HAL_H
#define HAL_H

typedef enum {
    HAL_INITIALIZATION_SUCCESS = 0,
    HAL_ALREADY_INITIALIZED = 1,
    HAL_DIO_INITIALIZATION_ERROR = 2
} hal_initialization_result_t;

hal_initialization_result_t hal_initialize();
void hal_shutdown();

#endif //HAL_HAL_H
