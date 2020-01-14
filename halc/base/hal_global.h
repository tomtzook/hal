#ifndef HAL_HAL_GLOBAL_H
#define HAL_HAL_GLOBAL_H

#include <stdbool.h>
#include "util/lookup_table.h"

#define HAL_IS_INITIALIZED(__ENV) \
    __ENV->initialized == 1
#define HAL_NOT_INITIALIZED(__ENV) \
    __ENV->initialized == 0


#define HAL_SET_INITIALIZED(__ENV) \
    __ENV->initialized = 1
#define HAL_SET_NOT_INITIALIZED(__ENV) \
    __ENV->initialized = 0

typedef struct hal_env {
    bool initialized;
    lookup_table_t dio_table;
} hal_env_t;

#endif //HAL_HAL_GLOBAL_H
