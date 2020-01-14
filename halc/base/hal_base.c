#include <hal.h>
#include <hal_dio.h>
#include <log.h>

#include "hal_global.h"


int hal_initialized = 0;

hal_initialization_result_t hal_initialize() {
    if (HAL_IS_INITIALIZED()) {
        return HAL_ALREADY_INITIALIZED;
    }

    hal_result_t init_result;

    init_result = hal_dio_init_module();
    if (init_result != HAL_SUCCESS) {
        LOGLN("failed to init dio module: %d", init_result);
        return HAL_DIO_INITIALIZATION_ERROR;
    }

    HAL_SET_INITIALIZED();
    return HAL_INITIALIZATION_SUCCESS;
}

void hal_shutdown() {
    if (HAL_NOT_INITIALIZED()) {
        return;
    }

    hal_dio_free_module();

    HAL_SET_NOT_INITIALIZED();
}

