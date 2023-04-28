#pragma once

#include "hal.h"
#include "hal_internal.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_backend_init(hal_backend_t* backend);
void hal_backend_shutdown(hal_backend_t* backend);

#ifdef __cplusplus
}
#endif
