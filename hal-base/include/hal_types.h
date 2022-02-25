#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _hal_env hal_env_t;

typedef uintptr_t hal_handle_t;

#define HAL_EMPTY_HANDLE ((hal_handle_t)0)

#ifdef __cplusplus
}
#endif
