#pragma once

#include "hal_error.h"
#include "hal_types.h"
#include "hal.h"

#include "ports/ports_internal.h"


struct _hal_env {
    bool initialized;
    ports_env_t ports_sys;
};

#define HAL_IS_INITIALIZED(env) \
    (NULL != (env) && (env)->initialized)

#define HAL_CHECK_INITIALIZED(env) \
    do {                            \
        if(!HAL_IS_INITIALIZED(env)) { \
            return HAL_ERROR_NOT_INITIALIZED; \
        } \
    } while(0)

#define HAL_CHECK_INITIALIZED_VOID(env) \
    do {                            \
        if(!HAL_IS_INITIALIZED(env)) { \
            return; \
        } \
    } while(0)
