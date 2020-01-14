#ifndef HAL_HAL_GLOBAL_H
#define HAL_HAL_GLOBAL_H

#include <stdbool.h>

#include "interface/dio_interface.h"
#include "util/lookup_table.h"

#define HAL_IS_INITIALIZED(__ENV) \
    NULL != __ENV && (__ENV)->initialized
#define HAL_NOT_INITIALIZED(__ENV) \
    NULL == __ENV || !(__ENV)->initialized


#define HAL_SET_INITIALIZED(__ENV) \
    (__ENV)->initialized = true
#define HAL_SET_NOT_INITIALIZED(__ENV) \
    (__ENV)->initialized = false

typedef struct hal_env {
    bool initialized;
    lookup_table_t dio_table;
    dio_env_t* dio_env;
} hal_env_t;

#endif //HAL_HAL_GLOBAL_H
