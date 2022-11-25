#pragma once

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uintptr_t hal_handle_t;

#define HAL_EMPTY_HANDLE ((hal_handle_t)0)

typedef uint32_t hal_port_t;

typedef enum _hal_port_type {
    HAL_TYPE_DIGITAL_INPUT = (0x1 << 0),
    HAL_TYPE_DIGITAL_OUTPUT = (0x1 << 1),
    HAL_TYPE_ANALOG_INPUT = (0x1 << 2),
    HAL_TYPE_ANALOG_OUTPUT = (0x1 << 3),
} hal_port_type_t;

typedef struct _hal_native hal_native_t;
typedef struct _hal_env hal_env_t;

#ifdef __cplusplus
}
#endif
