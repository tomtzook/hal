#include <stdlib.h>

#include "hal_aio.h"
#include "hal_global.h"
#include "logging/log.h"
#include "interface/aio_interface.h"

#include "hal_modules.h"

typedef struct {
    port_id_t port_id;
    port_dir_t port_dir;

    aio_value_t last_value;
} aio_port_t;

hal_result_t hal_aio_init_module(hal_env_t* env) {
    lookup_table_result_t table_init_result = lookup_table_init(&env->aio_table);
    if (LOOKUP_TABLE_SUCCESS != table_init_result) {
        LOGLN("failed to initialize lookup table: %d", table_init_result);
        return HAL_INITIALIZATION_ERROR;
    }

    return HAL_SUCCESS;
}

void hal_aio_free_module(hal_env_t* env) {
    if (HAL_NOT_INITIALIZED(env)) {
        return;
    }

    lookup_table_free(&env->aio_table);
}

