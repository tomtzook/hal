#pragma once

#include "hal_error.h"

#include "ports/ports_sys.h"
#include "ports/ports_dio.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _hal_native {
    hal_error_t (*ports_init)(hal_env_t* env, ports_native_t* native);
} hal_native_t;

hal_error_t hal_init(hal_env_t** env, hal_native_t native);

void hal_quit(hal_env_t** env);

#ifdef __cplusplus
}
#endif
