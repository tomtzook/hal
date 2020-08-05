#include <stdlib.h>
#include <hal_types.h>

#include "interface_def.h"


hal_result_t interface_init(interface_env_t** env) {
    hal_result_t result;

    interface_env_t* interface_env = (interface_env_t*) malloc(sizeof(interface_env_t));
    if (NULL == interface_env) {
        return HAL_MEMORY_ALLOCATION_ERROR;
    }

    bbbio_rc_t bbbio_rc;
    bbbio_t* bbbio = (bbbio_t*) malloc(sizeof(bbbio_t));
    if (NULL == *env) {
        result = HAL_MEMORY_ALLOCATION_ERROR;
        goto error;
    }

    bbbio_rc = bbbio_init(bbbio);
    if (SUCCESS != bbbio_rc) {
        result = HAL_INITIALIZATION_ERROR;
        goto error;
    }

    interface_env->bbbio = bbbio;

    *env = interface_env;
    return HAL_SUCCESS;
error:
    if (NULL != bbbio) {
        free(bbbio);
    }
    free(interface_env);

    return result;
}

void interface_free(interface_env_t** env) {
    bbbio_free((*env)->bbbio);
    free((*env)->bbbio);
    free(*env);
    *env = NULL;
}
