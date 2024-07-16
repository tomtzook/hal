#pragma once

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_handle_t;

#define HAL_EMPTY_HANDLE ((hal_handle_t)-1)
#define HAL_PORT_NAME_MAX 32

typedef enum {
    HAL_TYPE_DIGITAL_INPUT = (0x1 << 0),
    HAL_TYPE_DIGITAL_OUTPUT = (0x1 << 1),
    HAL_TYPE_ANALOG_INPUT = (0x1 << 2),
    HAL_TYPE_ANALOG_OUTPUT = (0x1 << 3),
    HAL_TYPE_PWM_OUTPUT = (0x1 << 4),
    HAL_TYPE_QUADRATURE = (0x1 << 5),

    HAL_TYPE_BLOCKED = 0xffff
} hal_port_type_t;

typedef struct _hal_backend hal_backend_t;
typedef struct _hal_env hal_env_t;

typedef struct {
    char name[HAL_PORT_NAME_MAX];
    uint32_t supported_types;

    uint8_t _iter_data[];
} hal_port_iter_t;

typedef struct {
    hal_handle_t handle;
    char name[HAL_PORT_NAME_MAX];
    hal_port_type_t type;
} hal_open_port_info_t;

#ifdef __cplusplus
}
#endif
