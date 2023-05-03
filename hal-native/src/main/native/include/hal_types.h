#pragma once

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uintptr_t hal_handle_t;

#define HAL_EMPTY_HANDLE ((hal_handle_t)0)
#define PORT_NAME_MAX 32

typedef enum _hal_port_type {
    HAL_TYPE_DIGITAL_INPUT = (0x1 << 0),
    HAL_TYPE_DIGITAL_OUTPUT = (0x1 << 1),
    HAL_TYPE_ANALOG_INPUT = (0x1 << 2),
    HAL_TYPE_ANALOG_OUTPUT = (0x1 << 3),
    HAL_TYPE_PWM_OUTPUT = (0x1 << 4),
} hal_port_type_t;

typedef struct _hal_backend hal_backend_t;
typedef struct _hal_env hal_env_t;

#ifdef __cplusplus
}
#endif
