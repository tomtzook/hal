
#include <stdlib.h>
#include <string.h>
#include <hal_error_handling.h>

#include "memdefs.h"
#include "base.h"


hal_error_t initialize_backend(bbb_env_t** env) {
    bbb_env_t* _env = (bbb_env_t*) malloc(sizeof(bbb_env_t));
    if (_env == NULL) {
        HAL_RETURN_IF_ERROR(HAL_ERROR_ALLOCATION_FAILED);
    }

    memset(_env, 0, sizeof(bbb_env_t));

    hal_error_t status = HAL_SUCCESS;
    const uint32_t gpio_base_addrs[4] {GPIO0_PERIPHERAL_BASE, GPIO1_PERIPHERAL_BASE, GPIO2_PERIPHERAL_BASE, GPIO3_PERIPHERAL_BASE};
    for (int i = 0; i < 4; ++i) {
        status = map_peripheral(&_env->gpio_peripherals[i], gpio_base_addrs[i], GPIO_PERIPHERAL_SIZE);
        HAL_JUMP_IF_ERROR(status, end);
    }

    status = map_peripheral(&_env->control_module, CM_PERIPHERAL_BASE, GCM_PERIPHERAL_SIZE);
    HAL_JUMP_IF_ERROR(status, end);

    *env = _env;
end:
    if (HAL_IS_ERROR(status)) {
        free_backend(_env);
    }

    return status;
}

void free_backend(bbb_env_t* env) {
    if (env == NULL) {
        return;
    }

    for (int i = 0; i < 4; ++i) {
        unmap_peripheral(&env->gpio_peripherals[i]);
    }

    unmap_peripheral(&env->control_module);

    free(env);
}
