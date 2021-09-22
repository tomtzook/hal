#include <stdlib.h>

#include "hal_internal.h"
#include "ports/ports_internal.h"


hal_error_t hal_init(hal_env_t** env, hal_native_t native) {
    if (HAL_IS_INITIALIZED(*env)) {
        return HAL_SUCCESS;
    }

    hal_error_t status = HAL_SUCCESS;

    hal_env_t* make_env = (hal_env_t*) malloc(sizeof(hal_env_t));
    HAL_CHECK_ALLOCATED(make_env);

    // TODO: handle error
    status = hal_ports_init(make_env, native);
    if (status) {
        goto cleanup;
    }

    make_env->initialized = true;
    *env = make_env;

cleanup:
    if (status) {
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
