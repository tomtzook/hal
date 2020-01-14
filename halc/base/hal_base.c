#include <stdlib.h>

#include "hal.h"
#include "hal_dio.h"
#include "logging/log.h"

#include "hal_global.h"


hal_result_t hal_initialize(hal_env_t** env) {
    if (HAL_IS_INITIALIZED(*env)) {
        return HAL_ALREADY_INITIALIZED;
    }

    *env = (hal_env_t*) malloc(sizeof(hal_env_t));
    if (NULL == *env) {
        return HAL_ENV_MALLOC_ERROR;
    }

    (*env)->dio_env = NULL;
    (*env)->dio_table.table = NULL;

    hal_result_t init_result;

    init_result = hal_dio_init_module(*env);
    if (init_result != HAL_SUCCESS) {
        free(*env);

        LOGLN("failed to init dio module: %d", init_result);
        return HAL_DIO_INITIALIZATION_ERROR;
    }

    HAL_SET_INITIALIZED(*env);
    return HAL_SUCCESS;
}

void hal_clean(hal_env_t** env) {
    if (HAL_NOT_INITIALIZED(*env)) {
        return;
    }

    hal_dio_free_module(*env);

    HAL_SET_NOT_INITIALIZED(*env);

    free(*env);
    *env = NULL;
}

