#ifndef HAL_INTERFACE_H
#define HAL_INTERFACE_H

#include "hal_types.h"

typedef struct interface_env interface_env_t;

hal_result_t interface_init(interface_env_t** env);
void interface_free(interface_env_t** env);

#endif //HAL_INTERFACE_H
