#pragma once

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t hal_id_t;
typedef uint32_t hal_handle_t;

#define HAL_INVALID_IDENTIFIER ((hal_id_t)-1)
#define HAL_EMPTY_HANDLE ((hal_handle_t)-1)

#define HAL_OPT_UINT32 ((uint32_t)-1)
#define HAL_OPT_UINT64 ((uint64_t)-1)

typedef enum {
    HAL_TYPE_DIGITAL_INPUT = (0x1 << 0),
    HAL_TYPE_DIGITAL_OUTPUT = (0x1 << 1),
    HAL_TYPE_ANALOG_INPUT = (0x1 << 2),
    HAL_TYPE_ANALOG_OUTPUT = (0x1 << 3),
    HAL_TYPE_PWM_OUTPUT = (0x1 << 4),
    HAL_TYPE_QUADRATURE = (0x1 << 5)
} hal_port_type_t;

typedef enum {
    HAL_FLAG_OPEN = (0x1 << 0),
    HAL_FLAG_BLOCKED = (0x1 << 1)
} hal_port_flag_t;

typedef struct {
    hal_id_t identifier;
    uint32_t supported_types;
    uint64_t supported_props;
    uint32_t flags;

    hal_handle_t open_handle;
} hal_port_info_t;

typedef struct {
    hal_id_t identifier;
} hal_port_iter_t;

typedef struct hal__backend hal_backend_t;
typedef struct hal__env hal_env_t;

#ifdef __cplusplus
}
#endif
