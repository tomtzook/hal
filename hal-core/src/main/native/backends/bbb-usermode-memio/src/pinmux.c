
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <hal_error_handling.h>

#include "files.h"
#include "memdefs.h"
#include "pinmux.h"


#define PINMUX_IMPL_SYSFS 0
#define PINMUX_IMPL_MEMIO 1
#define PINMUX_IMPL PINMUX_IMPL_SYSFS

#if PINMUX_IMPL == PINMUX_IMPL_SYSFS

static const char* PINMUX_FILE_FORMAT = "/sys/devices/platform/ocp/ocp:%s_pinmux/state";

// see config-pin
static const char* STR_PINMUX_DEFAULT = "default";
static const char* STR_PINMUX_GPIO = "gpio";
static const char* STR_PINMUX_GPIO_PULLDOWN = "gpio_pd";
static const char* STR_PINMUX_GPIO_PULLUP = "gpio_pu";
static const char* STR_PINMUX_PWM = "pwm";
static const char* STR_PINMUX_AIN = "adc";


static unsigned get_type_from_mode_str(const char* mode_str) {
    if (strcmp(mode_str, STR_PINMUX_DEFAULT) == 0) {
        return 0;
    }
    if (strcmp(mode_str, STR_PINMUX_GPIO) == 0) {
        return PIN_TYPE_GPIO;
    }
    if (strcmp(mode_str, STR_PINMUX_PWM) == 0) {
        return PIN_TYPE_EHRPWM;
    }
    if (strcmp(mode_str, STR_PINMUX_AIN) == 0) {
        return PIN_TYPE_AIN;
    }

    return (unsigned) -1;
}

static const char* get_mode_str_from_type(const unsigned type) {
    switch (type) {
        case PIN_TYPE_GPIO:
            return STR_PINMUX_GPIO;
        case PIN_TYPE_EHRPWM:
            return STR_PINMUX_PWM;
        case PIN_TYPE_AIN:
            return STR_PINMUX_AIN;
        default:
            return NULL;
    }
}

static hal_error_t get_pinumux_via_sysfs(const pin_t* pin, const pinmux_t* pinmux, unsigned* mode) {
    char path[PATH_MAX];
    sprintf(path, PINMUX_FILE_FORMAT, pin->name);

    char buffer[16];
    HAL_RETURN_IF_ERROR(read_file(path, buffer, sizeof(buffer)));

    const unsigned mode_type = get_type_from_mode_str(buffer);
    if (mode_type == 0) {
        // default mode
        *mode = 0;
    } else if (mode_type == (unsigned) -1) {
        return HAL_ERROR_BAD_DATA;
    } else {
        int found = 0;
        int idx = 0;
        for (int i = 0; i < 8; ++i) {
            if (pinmux->modes[i] == mode_type) {
                found = 1;
                idx = i;
                break;
            }
        }

        if (!found) {
            return HAL_ERROR_BAD_DATA;
        }

        *mode = idx;
    }

    return HAL_SUCCESS;
}

static hal_error_t set_pinumux_via_sysfs(const pin_t* pin, const pinmux_t* pinmux, const unsigned mode) {
    if (mode > 7) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    const char* raw_mode;
    if (mode == 0) {
        raw_mode = STR_PINMUX_DEFAULT;
    } else {
        const unsigned mode_type = pinmux->modes[mode];
        raw_mode = get_mode_str_from_type(mode_type);
        if (raw_mode == NULL) {
            return HAL_ERROR_BAD_ARGUMENT;
        }
    }

    char path[PATH_MAX];
    sprintf(path, PINMUX_FILE_FORMAT, pin->name);

    HAL_RETURN_IF_ERROR(write_file(path, raw_mode));

    return HAL_SUCCESS;
}

#elif PINMUX_IMPL == PINMUX_IMPL_MEMIO

static hal_error_t get_pinumux_via_memio(const peripheral_t* control_module, const pinmux_t* pinmux, unsigned* mode) {
    const unsigned raw_mode = REG(control_module->base + pinmux->cm_offset, cm_conf_pin_reg_t)->bits.mmode;
    switch (raw_mode) {
        case pinmux_mode0:
            *mode = 0;
            break;
        case pinmux_mode1:
            *mode = 1;
            break;
        case pinmux_mode2:
            *mode = 2;
            break;
        case pinmux_mode3:
            *mode = 3;
            break;
        case pinmux_mode4:
            *mode = 4;
            break;
        case pinmux_mode5:
            *mode = 5;
            break;
        case pinmux_mode6:
            *mode = 6;
            break;
        case pinmux_mode7:
            *mode = 7;
            break;
        default:
            return HAL_ERROR_BAD_DATA;
    }

    return HAL_SUCCESS;
}

static hal_error_t set_pinumux_via_memio(const peripheral_t* control_module, const pinmux_t* pinmux, const unsigned mode) {
    unsigned raw_mode;
    switch (mode) {
        case 0:
            raw_mode = pinmux_mode0;
            break;
        case 1:
            raw_mode = pinmux_mode1;
            break;
        case 2:
            raw_mode = pinmux_mode2;
            break;
        case 3:
            raw_mode = pinmux_mode3;
            break;
        case 4:
            raw_mode = pinmux_mode4;
            break;
        case 5:
            raw_mode = pinmux_mode5;
            break;
        case 6:
            raw_mode = pinmux_mode6;
            break;
        case 7:
            raw_mode = pinmux_mode7;
            break;
        default:
            return HAL_ERROR_BAD_ARGUMENT;
    }

    REG(control_module->base + pinmux->cm_offset, cm_conf_pin_reg_t)->bits.mmode = raw_mode;

    return HAL_SUCCESS;
}

#else
#error "PINMUX_IMPL has invalid value"
#endif


static hal_error_t get_mode_with_support_for_type(const pinmux_t* pinmux, const unsigned type, unsigned* mode) {
    for (int i = 0; i < 8; ++i) {
        if (pinmux->modes[i] == type) {
            *mode = i;
            return HAL_SUCCESS;
        }
    }

    return HAL_ERROR_NOT_FOUND;
}

hal_error_t get_pinmux(const bbb_env_t* env, const pin_t* pin, unsigned* mode) {
    const pinmux_t* pinmux = get_pinmux_for_pin(pin);
    if (pinmux == NULL) {
        return HAL_ERROR_NOT_FOUND;
    }

#if PINMUX_IMPL == PINMUX_IMPL_SYSFS
    HAL_RETURN_IF_ERROR(get_pinumux_via_sysfs(pin, pinmux, mode));
#elif PINMUX_IMPL == PINMUX_IMPL_MEMIO
    HAL_RETURN_IF_ERROR(get_pinumux_via_memio(env->control_module, pinmux, mode));
#endif

    return HAL_SUCCESS;
}

hal_error_t set_pinmux(const bbb_env_t* env, const pin_t* pin, const unsigned mode) {
    const pinmux_t* pinmux = get_pinmux_for_pin(pin);
    if (pinmux == NULL) {
        return HAL_ERROR_NOT_FOUND;
    }

#if PINMUX_IMPL == PINMUX_IMPL_SYSFS
    HAL_RETURN_IF_ERROR(set_pinumux_via_sysfs(pin, pinmux, mode));
#elif PINMUX_IMPL == PINMUX_IMPL_MEMIO
    HAL_RETURN_IF_ERROR(set_pinumux_via_memio(env->control_module, pinmux, mode));
#endif

    return HAL_SUCCESS;
}

hal_error_t set_pinmux_to_type(const bbb_env_t* env, const pin_t* pin, const unsigned type) {
    const pinmux_t* pinmux = get_pinmux_for_pin(pin);
    if (pinmux == NULL) {
        return HAL_ERROR_NOT_FOUND;
    }

    unsigned mode;
    HAL_RETURN_IF_ERROR(get_mode_with_support_for_type(pinmux, type, &mode));
    HAL_RETURN_IF_ERROR(set_pinmux(env, pin, mode));

    return HAL_SUCCESS;
}

hal_error_t get_pinmux_gpio_resistor(const bbb_env_t* env, const pin_t* pin, hal_dio_config_resistor_t* resistor) {
#if PINMUX_IMPL == PINMUX_IMPL_SYSFS
    char path[PATH_MAX];
    sprintf(path, PINMUX_FILE_FORMAT, pin->name);

    char buffer[16];
    HAL_RETURN_IF_ERROR(read_file(path, buffer, sizeof(buffer)));

    if (strcmp(buffer, STR_PINMUX_GPIO_PULLUP) == 0) {
        *resistor = HAL_CONFIG_DIO_RESISTOR_PULLUP;
    } else if (strcmp(buffer, STR_PINMUX_GPIO_PULLDOWN) == 0) {
        *resistor = HAL_CONFIG_DIO_RESISTOR_PULLDOWN;
    } else {
        *resistor = HAL_CONFIG_DIO_RESISTOR_NONE;
    }

    return HAL_SUCCESS;
#elif PINMUX_IMPL == PINMUX_IMPL_MEMIO
#error "resistor access not implemented for memio"
#endif
}

hal_error_t set_pinmux_gpio_resistor(const bbb_env_t* env, const pin_t* pin, const hal_dio_config_resistor_t resistor) {
#if PINMUX_IMPL == PINMUX_IMPL_SYSFS
    const char* mode;
    switch (resistor) {
        case HAL_CONFIG_DIO_RESISTOR_PULLUP:
            mode = STR_PINMUX_GPIO_PULLUP;
            break;
        case HAL_CONFIG_DIO_RESISTOR_PULLDOWN:
            mode = STR_PINMUX_GPIO_PULLDOWN;
            break;
        case HAL_CONFIG_DIO_RESISTOR_NONE:
            mode = STR_PINMUX_GPIO;
            break;
        default:
            return HAL_ERROR_BAD_ARGUMENT;
    }

    char path[PATH_MAX];
    sprintf(path, PINMUX_FILE_FORMAT, pin->name);

    HAL_RETURN_IF_ERROR(write_file(path, mode));

    return HAL_SUCCESS;
#elif PINMUX_IMPL == PINMUX_IMPL_MEMIO
#error "resistor access not implemented for memio"
#endif
}
