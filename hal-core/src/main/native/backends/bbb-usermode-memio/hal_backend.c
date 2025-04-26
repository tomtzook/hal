
#include <malloc.h>
#include <memory.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal_control.h>

#include "pins.h"
#include "peripheral.h"
#include "base.h"
#include "gpio.h"
#include "pinmux.h"


#define max(a,b) (((a)>(b)) ? (a) : (b))

#define TRACE_TITLE "BBB_BACKEND: "

#define ANALOG_MAX_VALUE 4095
#define ANALOG_MAX_VOLTAGE 3.3f
#define ANALOG_MAX_VOLTAGE_MV 3300
#define ANALOG_SAMPLE_RATE 200000.0f // 200khz
#define ANALOG_SAMPLE_RATE_PERIOD_US 5


static uint32_t get_supported_types(const pin_t* pin) {
    const pinmux_t* pinmux = get_pinmux_for_pin(pin);
    if (pinmux == NULL) {
        return 0;
    }

    uint32_t types = 0;
    for (int i = 0; i < 8; i++) {
        const unsigned type = pinmux->modes[i];
        if (type == PIN_TYPE_GPIO) {
            types |= HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT;
        }
        if (type == PIN_TYPE_AIN) {
            types |= HAL_TYPE_ANALOG_INPUT;
        }
        if (type == PIN_TYPE_EHRPWM) {
            types |= HAL_TYPE_PWM_OUTPUT;
        }
    }

    return types;
}

uint32_t get_supported_props(const uint32_t supported_types) {
    uint32_t props = 0;

    if (supported_types & (HAL_TYPE_DIGITAL_INPUT)) {
        props |= HAL_CONFIG_DIO_POLL_EDGE | HAL_CONFIG_DIO_RESISTOR;
    }
    if (supported_types & (HAL_TYPE_ANALOG_INPUT | HAL_TYPE_ANALOG_OUTPUT)) {
        props |= HAL_CONFIG_ANALOG_MAX_VALUE | HAL_CONFIG_ANALOG_MAX_VOLTAGE | HAL_CONFIG_ANALOG_SAMPLE_RATE;
    }
    if (supported_types & HAL_TYPE_PWM_OUTPUT) {
        props |= HAL_CONFIG_PWM_FREQUENCY;
    }

    return props;
}

static const bbb_env_t* get_bbb_env(hal_env_t* env) {
    const hal_backend_t* backend = hal_get_backend(env);
    return (bbb_env_t*) backend->data;
}

static const pin_t* get_pin_from_data(void* data) {
    pin_t** pin_out = (pin_t**) data;
    return *pin_out;
}

static const pwm_pin_t* get_pwm_from_data(void* data) {
    pwm_pin_t** pin_out = (pwm_pin_t**) data;
    return *pin_out;
}

static hal_error_t open(hal_env_t* env, const hal_backend_port_t* port) {
    const pin_t* pin = find_pin_by_id(port->identifier);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    if ((get_supported_types(pin) & port->type) != port->type) {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    const bbb_env_t* bbb_env = get_bbb_env(env);
    if (bbb_env == NULL) {
        return HAL_ERROR_BAD_DATA;
    }

    if (port->type == HAL_TYPE_DIGITAL_INPUT) {
        const pin_t** pin_out = (const pin_t**) port->data;
        if (pin_out == NULL) {
            return HAL_ERROR_BAD_DATA;
        }

        HAL_RETURN_IF_ERROR(set_pinmux_to_type(bbb_env, pin, PIN_TYPE_GPIO));
        HAL_RETURN_IF_ERROR(gpio_set_direction(bbb_env, pin, DIR_INPUT));
        HAL_RETURN_IF_ERROR(set_pinmux_gpio_resistor(bbb_env, pin, HAL_CONFIG_DIO_RESISTOR_PULLDOWN));

        *pin_out = pin;
        return HAL_SUCCESS;
    } else if (port->type == HAL_TYPE_DIGITAL_OUTPUT) {
        const pin_t** pin_out = (const pin_t**) port->data;
        if (pin_out == NULL) {
            return HAL_ERROR_BAD_DATA;
        }

        HAL_RETURN_IF_ERROR(set_pinmux_to_type(bbb_env, pin, PIN_TYPE_GPIO));
        HAL_RETURN_IF_ERROR(gpio_set_direction(bbb_env, pin, DIR_OUTPUT));
        HAL_RETURN_IF_ERROR(set_pinmux_gpio_resistor(bbb_env, pin, HAL_CONFIG_DIO_RESISTOR_NONE));

        *pin_out = pin;
        return HAL_SUCCESS;
    } else if (port->type == HAL_TYPE_ANALOG_INPUT) {
        const pin_t** pin_out = (const pin_t**) port->data;
        if (pin_out == NULL) {
            return HAL_ERROR_BAD_DATA;
        }

        *pin_out = pin;

        // todo
    } else if (port->type == HAL_TYPE_PWM_OUTPUT) {
        /*const char* module_name = get_pwm_module_name_for_pin(pin);
        if (module_name == NULL) {
            TRACE_ERROR(TRACE_TITLE "PWM module was not found for pin %s", pin->name);
            return HAL_ERROR_BAD_ARGUMENT;
        }

        hal_error_t status;

        const pwm_pin_t* pwm_pin = get_pwm_pin_for_module(module_name);
        if (pwm_pin == NULL) {
            TRACE_ERROR(TRACE_TITLE "PWM Pin was not found for module %s", module_name);
            return HAL_ERROR_BAD_ARGUMENT;
        }

        const pwm_pin_t** pin_out = (const pwm_pin_t**) port->data;
        if (pin_out == NULL) {
            return HAL_ERROR_BAD_DATA;
        }

        *pin_out = pwm_pin;*/

        // todo
        return HAL_SUCCESS;
    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

static hal_error_t close(hal_env_t* env, const hal_backend_port_t* port) {
    const pin_t* pin = find_pin_by_id(port->identifier);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    if (port->type == HAL_TYPE_DIGITAL_INPUT || port->type == HAL_TYPE_DIGITAL_OUTPUT) {
        // todo
    } else if (port->type == HAL_TYPE_ANALOG_INPUT) {
        // todo
    } else if (port->type == HAL_TYPE_PWM_OUTPUT) {
        // todo
    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

static hal_error_t port_probe_prop(hal_env_t* env, const hal_backend_port_t* port,
        const hal_prop_key_t key, uint32_t* flags) {
    uint32_t _flags = 0;
    switch (key) {
        case HAL_CONFIG_DIO_POLL_EDGE:
        case HAL_CONFIG_DIO_RESISTOR:
        case HAL_CONFIG_PWM_FREQUENCY:
            _flags |= HAL_CONFIG_FLAG_READABLE | HAL_CONFIG_FLAG_WRITABLE;
            break;
        case HAL_CONFIG_ANALOG_MAX_VALUE:
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE:
        case HAL_CONFIG_ANALOG_SAMPLE_RATE:
            _flags |= HAL_CONFIG_FLAG_READABLE;
            break;
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }

    *flags = _flags;
    return HAL_SUCCESS;
}

static hal_error_t port_get_prop(hal_env_t* env, const hal_backend_port_t* port,
                          const hal_prop_key_t key, uint32_t* value) {
    switch (key) {
        case HAL_CONFIG_DIO_POLL_EDGE: {
            const pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }
            // todo
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_DIO_RESISTOR: {
            const pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }
            // todo
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_MAX_VALUE: {
            *value = ANALOG_MAX_VALUE;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE: {
            *value = ANALOG_MAX_VOLTAGE_MV;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_SAMPLE_RATE: {
            *value = ANALOG_SAMPLE_RATE_PERIOD_US;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            // todo
            return HAL_SUCCESS;
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }
}

static hal_error_t port_set_prop(hal_env_t* env, const hal_backend_port_t* port,
                          const hal_prop_key_t key, const uint32_t value) {
    switch (key) {
        case HAL_CONFIG_DIO_POLL_EDGE: {
            if (port->type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            const pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            // todo
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_DIO_RESISTOR: {
            if (port->type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            const pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            // todo
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            if (port->type != HAL_TYPE_PWM_OUTPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            const pwm_pin_t* pin = get_pwm_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            // todo
            return HAL_SUCCESS;
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }
}

static hal_error_t dio_get(hal_env_t* env, const hal_backend_port_t* port, hal_dio_value_t* value) {
    const pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        TRACE_ERROR(TRACE_TITLE "Pin was not found for port %u", port->identifier);
        return HAL_ERROR_BAD_ARGUMENT;
    }

    // todo
    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_env_t* env, const hal_backend_port_t* port, hal_dio_value_t value) {
    const pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        TRACE_ERROR(TRACE_TITLE "Pin was not found for port %u", port->identifier);
        return HAL_ERROR_BAD_ARGUMENT;
    }

    // todo
    return HAL_SUCCESS;
}

static hal_error_t aio_get(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value) {
    const pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        TRACE_ERROR(TRACE_TITLE "Pin was not found for port %u", port->identifier);
        return HAL_ERROR_BAD_ARGUMENT;
    }

    // todo
    return HAL_SUCCESS;
}

static hal_error_t pwm_getduty(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value) {
    const pwm_pin_t* pwm = get_pwm_from_data(port->data);
    if (pwm == NULL) {
        return HAL_ERROR_BAD_DATA;
    }

    // todo
    return HAL_SUCCESS;
}

static hal_error_t pwm_setduty(hal_env_t* env, const hal_backend_port_t* port, const uint32_t value) {
    const pwm_pin_t* pwm = get_pwm_from_data(port->data);
    if (pwm == NULL) {
        return HAL_ERROR_BAD_DATA;
    }

    // todo
    return HAL_SUCCESS;
}

hal_error_t hal_backend_init(hal_env_t* env) {
    hal_backend_t* backend = hal_get_backend(env);

    backend->name = "bbb-usermode-memio";
    backend->funcs.open = open;
    backend->funcs.close = close;
    backend->funcs.port_probe_prop = port_probe_prop;
    backend->funcs.port_get_prop = port_get_prop;
    backend->funcs.port_set_prop = port_set_prop;
    backend->funcs.dio_get = dio_get;
    backend->funcs.dio_set = dio_set;
    backend->funcs.aio_get = aio_get;
    backend->funcs.pwm_get_duty = pwm_getduty;
    backend->funcs.pwm_set_duty = pwm_setduty;
    backend->data = NULL;

    const size_t allocation_size = max(sizeof(pin_t**), sizeof(pwm_pin_t**));

    for (int i = 0; i < get_pin_count(); ++i) {
        const pin_t* pin = get_pin(i);
        if (pin == NULL) {
            continue;
        }
        const uint32_t supported_types = get_supported_types(pin);
        if (supported_types == 0) {
            continue;
        }

        const uint32_t supported_props = get_supported_props(supported_types);

        HAL_RETURN_IF_ERROR(halcontrol_register_port(env, pin->id));
        HAL_RETURN_IF_ERROR(halcontrol_config_port(env, pin->id, supported_types, supported_props));
        HAL_RETURN_IF_ERROR(halcontrol_config_port_name(env, pin->id, pin->name));
        HAL_RETURN_IF_ERROR(halcontrol_config_backend_allocation_size(env, pin->id, allocation_size));
    }

    bbb_env_t* our_env;
    HAL_RETURN_IF_ERROR(initialize_backend(&our_env));
    backend->data = our_env;

    return HAL_SUCCESS;
}

void hal_backend_shutdown(hal_env_t* env) {
    hal_backend_t* backend = hal_get_backend(env);
    if (backend->data != NULL) {
        free_backend((bbb_env_t*) backend->data);
    }
}
