#include <stdlib.h>
#include <string.h>

#include "hal.h"
#include "hal_dio.h"
#include "logging/log.h"

#include "hal_global.h"
#include "hal_modules.h"

#include "interface/interface.h"


hal_result_t hal_initialize(hal_env_t** env) {
    if (HAL_IS_INITIALIZED(*env)) {
        return HAL_ALREADY_INITIALIZED;
    }

    hal_env_t* hal_env = (hal_env_t*) malloc(sizeof(hal_env_t));
    if (NULL == hal_env) {
        return HAL_MEMORY_ALLOCATION_ERROR;
    }
    memset(hal_env, 0, sizeof(hal_env_t));

    hal_result_t init_result;

    init_result = hal_dio_init_module(hal_env);
    if (HAL_SUCCESS != init_result) {
        free(hal_env);

        LOGLN("failed to init dio module: %d", init_result);
        return init_result;
    }

    init_result = interface_init(&hal_env->interface_env);
    if (HAL_SUCCESS != init_result) {
        hal_dio_free_module(hal_env);
        free(hal_env);

        LOGLN("failed to init interface: %d", init_result);
        return init_result;
    }

    HAL_SET_INITIALIZED(hal_env);
    *env = hal_env;

    return HAL_SUCCESS;
}

void hal_clean(hal_env_t** env) {
    if (HAL_NOT_INITIALIZED(*env)) {
        return;
    }

    interface_free(&(*env)->interface_env);
    hal_dio_free_module(*env);

    HAL_SET_NOT_INITIALIZED(*env);

    free(*env);
    *env = NULL;
}

