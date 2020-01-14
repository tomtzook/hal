#ifndef HAL_H
#define HAL_H

#include "hal_types.h"

hal_result_t hal_initialize(hal_env_t* env);
void hal_shutdown(hal_env_t* env);

#endif //HAL_HAL_H
