
#include <malloc.h>
#include <memory.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal_control.h>

#include "sysfs/gpio.h"
#include "sysfs/adc.h"
#include "include/pins.h"
#include "sysfs/pwm.h"

// todo: quad/counter support
// todo: set pinmode based on need

#define max(a,b) (((a)>(b)) ? (a) : (b))

#define TRACE_TITLE "BBB_BACKEND: "


static const pin_t* get_pin_from_data(void* data) {
    pin_t** pin_out = (pin_t**) data;
    return *pin_out;
}

static hal_error_t open(hal_env_t* env, const hal_backend_port_t* port) {
    // TODO: BETTER TEARDOWN ON FAILURES
    const pin_t* pin = find_pin_def_for_id(port->identifier);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    if ((pin->supported_types & port->type) != port->type) {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    if (port->type == HAL_TYPE_DIGITAL_INPUT) {
        const pin_t** pin_out = (const pin_t**) port->data;
        if (pin_out == NULL) {
            return HAL_ERROR_BAD_DATA;
        }

        *pin_out = pin;

        HAL_RETURN_IF_ERROR(gpio_export_pin(pin));
        HAL_RETURN_IF_ERROR(gpio_set_pinmux(pin, HAL_CONFIG_DIO_RESISTOR_PULLDOWN));
        HAL_RETURN_IF_ERROR(gpio_set_edge(pin, HAL_CONFIG_DIO_EDGE_RISING));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin, DIR_INPUT));
    } else if (port->type == HAL_TYPE_DIGITAL_OUTPUT){
        const pin_t** pin_out = (const pin_t**) port->data;
        if (pin_out == NULL) {
            return HAL_ERROR_BAD_DATA;
        }

        *pin_out = pin;

        HAL_RETURN_IF_ERROR(gpio_export_pin(pin));
        HAL_RETURN_IF_ERROR(gpio_set_pinmux(pin, HAL_CONFIG_DIO_RESISTOR_NONE));
        HAL_RETURN_IF_ERROR(gpio_set_edge(pin, HAL_CONFIG_DIO_EDGE_NONE));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin, DIR_OUTPUT));
    } else if (port->type == HAL_TYPE_ANALOG_INPUT) {
        const pin_t** pin_out = (const pin_t**) port->data;
        if (pin_out == NULL) {
            return HAL_ERROR_BAD_DATA;
        }

        *pin_out = pin;

        // no action needed
    } else if (port->type == HAL_TYPE_PWM_OUTPUT) {
        const char* module_name = get_pwm_module_name_for_pin(pin);
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

        pwm_t* pwm = (pwm_t*) port->data;
        HAL_CHECK_ALLOCATED(pwm, pwm_error);

        pwm->pin = pwm_pin;
        pwm->duty_ns = 0;
        pwm->period_ns = 0;

        status = set_pin_mode(pin, "pwm");
        HAL_JUMP_IF_ERROR(status, pwm_error);

        status = pwm_export(pwm);
        HAL_JUMP_IF_ERROR(status, pwm_error);

        status = pwm_set_frequency(pwm, 1000000);
        HAL_JUMP_IF_ERROR(status, pwm_error);

        return HAL_SUCCESS;
pwm_error:
        return status;
    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

static hal_error_t close(hal_env_t* env, const hal_backend_port_t* port) {
    const pin_t* pin = find_pin_def_for_id(port->identifier);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    if (port->type == HAL_TYPE_DIGITAL_INPUT || port->type == HAL_TYPE_DIGITAL_OUTPUT) {
        gpio_unexport_pin(pin);
    } else if (port->type == HAL_TYPE_ANALOG_INPUT) {
        // no action needed
    } else if (port->type == HAL_TYPE_PWM_OUTPUT) {
        pwm_t* pwm = (pwm_t*) port->data;

        pwm_disable(pwm);
        pwm_unexport(pwm);
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

            hal_dio_config_poll_edge_t config_value;
            const hal_error_t error = gpio_get_edge(pin, &config_value);
            if (HAL_IS_SUCCESS(error)) {
                *value = config_value;
            }
            return error;
        }
        case HAL_CONFIG_DIO_RESISTOR: {
            const pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            hal_dio_config_resistor_t config_value;
            const hal_error_t error = gpio_get_pinmux(pin, &config_value);
            if (HAL_IS_SUCCESS(error)) {
                *value = config_value;
            }
            return error;
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
            const pwm_t* pwm = (pwm_t*) port->data;
            *value = pwm->period_ns / 1000;
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

            return gpio_set_edge(pin, value);
        }
        case HAL_CONFIG_DIO_RESISTOR: {
            if (port->type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            const pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            return gpio_set_pinmux(pin, value);
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            if (port->type != HAL_TYPE_PWM_OUTPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pwm_t* pwm = (pwm_t*) port->data;
            return pwm_set_frequency(pwm, value);
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

    HAL_RETURN_IF_ERROR(gpio_get_value(pin, value));
    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_env_t* env, const hal_backend_port_t* port, hal_dio_value_t value) {
    const pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        TRACE_ERROR(TRACE_TITLE "Pin was not found for port %u", port->identifier);
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(gpio_set_value(pin, value));
    return HAL_SUCCESS;
}

static hal_error_t aio_get(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value) {
    const pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        TRACE_ERROR(TRACE_TITLE "Pin was not found for port %u", port->identifier);
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(adc_read(pin->pin_number, value));
    return HAL_SUCCESS;
}

static hal_error_t pwm_getduty(hal_env_t* env, const hal_backend_port_t* port, uint32_t* value) {
    const pwm_t* pwm = (pwm_t*) port->data;
    if (pwm == NULL) {
        return HAL_ERROR_BAD_DATA;
    }

    *value = pwm->duty_ns / 1000;

    return HAL_SUCCESS;
}

static hal_error_t pwm_setduty(hal_env_t* env, const hal_backend_port_t* port, const uint32_t value) {
    pwm_t* pwm = (pwm_t*) port->data;
    if (pwm == NULL) {
        return HAL_ERROR_BAD_DATA;
    }
    if ((value * 1000) > pwm->period_ns) {
        TRACE_ERROR(TRACE_TITLE "Requested duty cycle is longer than set period");
        return HAL_ERROR_BAD_ARGUMENT;
    }

    return pwm_set_duty_cycle(pwm, value);
}

hal_error_t hal_backend_init(hal_env_t* env) {
    hal_backend_t* backend = hal_get_backend(env);

    backend->name = "bbb-usermode-sysfs";
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

    const size_t allocation_size = max(sizeof(pin_t**), sizeof(pwm_t));

    for (int i = 0; i < get_pin_def_count(); ++i) {
        const pin_t* pin = get_pin_def_for_index(i);
        HAL_RETURN_IF_ERROR(halcontrol_register_port(env, pin->id));
        HAL_RETURN_IF_ERROR(halcontrol_config_port(env, pin->id, pin->supported_types, get_supported_props(pin)));
        HAL_RETURN_IF_ERROR(halcontrol_config_port_name(env, pin->id, pin->name));
        HAL_RETURN_IF_ERROR(halcontrol_config_backend_allocation_size(env, pin->id, allocation_size));
    }

    return HAL_SUCCESS;
}

void hal_backend_shutdown(hal_env_t* env) {
    
}
