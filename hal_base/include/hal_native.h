#pragma once

#include "hal.h"
#include "hal_internal.h"


#ifdef __cplusplus
extern "C" {
#endif

hal_error_t hal_native_init(hal_native_t* native);
void hal_native_shutdown(hal_native_t* native);

#ifdef __cplusplus
}
#endif
