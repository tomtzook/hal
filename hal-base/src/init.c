#include <stdlib.h>

#include "hal_internal.h"
#include "ports/ports_internal.h"


hal_error_t hal_init(hal_env_t** env) {
    if (HAL_IS_INITIALIZED(*env)) {
        return HAL_SUCCESS;
    }

    hal_error_t status = HAL_SUCCESS;
    hal_env_t* make_env = NULL;
    hal_native_t native;

    make_env = (hal_env_t*) malloc(sizeof(hal_env_t));
    if (make_env == NULL) {
        status = HAL_ERROR_NOT_ALLOCATED;
        goto cleanup;
    }

    status = hal_define_natives(&native);
    if (HAL_IS_ERROR(status)) {
        goto cleanup;
    }

    // TODO: handle error
    status = hal_ports_init(make_env, native);
    if (HAL_IS_ERROR(status)) {
        goto cleanup;
    }

    make_env->initialized = true;
    *env = make_env;

cleanup:
    if (HAL_IS_ERROR(status)) {
        free(make_env);
    }

    return status;
}

void hal_quit(hal_env_t** env) {
    if (!HAL_IS_INITIALIZED(*env)) {
        return;
    }

    hal_ports_quit(*env);

    (*env)->initialized = false;
    free(*env);
    *env = NULL;
}
