#include <stdlib.h>

#include <hal_types.h>

#include "interface_def.h"

hal_result_t interface_init(interface_env_t** env) {
    interface_env_t* interface_env = (interface_env_t*) malloc(sizeof(interface_env_t));
    if (NULL == interface_env) {
        return HAL_MEMORY_ALLOCATION_ERROR;
    }

    interface_env->some_value_in_env = rand();

    *env = interface_env;
    return HAL_SUCCESS;
}

void interface_free(interface_env_t** env) {
    free(*env);
    *env = NULL;
}
