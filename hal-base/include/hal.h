#pragma once

#include "hal_error.h"

#include "ports/ports_sys.h"
#include "ports/ports_dio.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_init(hal_env_t** env);
void hal_quit(hal_env_t** env);

#ifdef __cplusplus
}
#endif
