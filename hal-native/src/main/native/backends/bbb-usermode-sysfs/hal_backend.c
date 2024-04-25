
#include <malloc.h>
#include <memory.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal.h>

#include "sysfs/gpio.h"
#include "sysfs/adc.h"
#include "sysfs/pins.h"
#include "sysfs/pwm.h"


static uint32_t probe(hal_backend_t* env, const char* port_name) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    return pin->supported_types;
}

static hal_error_t open(hal_backend_t* env, const char* port_name, hal_port_type_t type, void** data) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    if ((pin->supported_types & type) != type) {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    if (type == HAL_TYPE_DIGITAL_INPUT) {
        HAL_RETURN_IF_ERROR(gpio_export_pin(pin));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin, DIR_INPUT));
        HAL_RETURN_IF_ERROR(gpio_set_pinmux(pin, HAL_GPIO_CONFIG_RESISTOR_PULLDOWN));
        HAL_RETURN_IF_ERROR(gpio_set_edge(pin, HAL_GPIO_CONFIG_EDGE_RISING));
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        HAL_RETURN_IF_ERROR(gpio_export_pin(pin));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin, DIR_OUTPUT));
    } else if (type == HAL_TYPE_ANALOG_INPUT) {
        // no action needed
    } else if (type == HAL_TYPE_PWM_OUTPUT) {
        const char* module_name = get_pwm_module_name_for_pin(pin);
        if (module_name == NULL) {
            return HAL_ERROR_BAD_ARGUMENT;
        }

        pwm_pin_t* pwm_pin = get_pwm_pin_for_module(module_name);
        pwm_t* pwm = (pwm_t*) malloc(sizeof(pwm_t));
        HAL_CHECK_ALLOCATED(pwm);
        pwm->pin = pwm_pin;
        pwm->duty_ns = 0;
        pwm->period_ns = 0;

        hal_error_t status;
        status = set_pin_mode(pin, "pwm");
        HAL_JUMP_IF_ERROR(status, pwm_error);

        status = pwm_export(pwm);
        HAL_JUMP_IF_ERROR(status, pwm_error);

        status = pwm_set_frequency(pwm, 1000000);
        HAL_JUMP_IF_ERROR(status, pwm_error);

        *data = pwm;
        return HAL_SUCCESS;
pwm_error:
        free(pwm);
        return status;
    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

static hal_error_t close(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    if (type == HAL_TYPE_DIGITAL_INPUT) {
        gpio_unexport_pin(pin);
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        gpio_unexport_pin(pin);
    } else if (type == HAL_TYPE_ANALOG_INPUT) {
        // no action needed
    } else if (type == HAL_TYPE_PWM_OUTPUT) {
        pwm_t* pwm = (pwm_t*) data;

        pwm_disable(pwm);
        pwm_unexport(pwm);

        free(data);
    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

hal_error_t port_probe_prop(hal_backend_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key, hal_config_flags_t* flags) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    hal_config_flags_t _flags;
    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            if (type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }
            _flags |= HAL_CONFIG_FLAG_READABLE | HAL_CONFIG_FLAG_WRITABLE;
            break;
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            if (type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }
            _flags |= HAL_CONFIG_FLAG_READABLE | HAL_CONFIG_FLAG_WRITABLE;
            break;
        }
        case HAL_CONFIG_ANALOG_MAX_VALUE: {
            if (type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }
            _flags |= HAL_CONFIG_FLAG_READABLE;
            break;
        }
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE: {
            if (type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }
            _flags |= HAL_CONFIG_FLAG_READABLE;
            break;
        }
        case HAL_CONFIG_ANALOG_SAMPLE_RATE: {
            if (type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }
            _flags |= HAL_CONFIG_FLAG_READABLE;
            break;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            if (type != HAL_TYPE_PWM_OUTPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }
            _flags |= HAL_CONFIG_FLAG_READABLE | HAL_CONFIG_FLAG_WRITABLE;
            break;
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }

    *flags = _flags;
    return HAL_SUCCESS;
}

hal_error_t port_get_prop(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                          hal_prop_key_t key, hal_prop_value_t* value) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            if (type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            return gpio_get_edge(pin, value);
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            if (type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            return gpio_get_pinmux(pin, value);
        }
        case HAL_CONFIG_ANALOG_MAX_VALUE: {
            if (type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            *value = ANALOG_MAX_VALUE;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE: {
            if (type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            *value = ANALOG_MAX_VOLTAGE_MV;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_SAMPLE_RATE: {
            if (type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            *value = ANALOG_SAMPLE_RATE_PERIOD_US;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            if (type != HAL_TYPE_PWM_OUTPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pwm_t* pwm = (pwm_t*) data;
            *value = pwm->period_ns * 10e-3;
            return HAL_SUCCESS;
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }
}

hal_error_t port_set_prop(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                          hal_prop_key_t key, hal_prop_value_t value) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            if (type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            return gpio_set_edge(pin, value);
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            if (type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            return gpio_set_pinmux(pin, value);
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            if (type != HAL_TYPE_PWM_OUTPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pwm_t* pwm = (pwm_t*) data;
            return pwm_set_frequency(pwm, value);
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }
}

static hal_error_t dio_get(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t* value) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(gpio_get_value(pin, value));
    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t value) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(gpio_set_value(pin, value));
    return HAL_SUCCESS;
}

static hal_error_t aio_get(hal_backend_t* env, const char* port_name, void* data, hal_aio_value_t* value) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(adc_read(pin->pin_number, value));
    return HAL_SUCCESS;
}

static hal_error_t pwm_getduty(hal_backend_t* env, const char* port_name, void* data, uint32_t* value) {
    pwm_t* pwm = (pwm_t*) data;
    *value = pwm->duty_ns * 10e-3;

    return HAL_SUCCESS;
}

static hal_error_t pwm_setduty(hal_backend_t* env, const char* port_name, void* data, uint32_t value) {
    pwm_t* pwm = (pwm_t*) data;
    return pwm_set_duty_cycle(pwm, value);
}

hal_error_t hal_backend_init(hal_backend_t* backend) {
    backend->name = "bbb-usermode-sysfs";
    backend->probe = probe;
    backend->open = open;
    backend->close = close;
    backend->port_probe_prop = port_probe_prop;
    backend->port_get_prop = port_get_prop;
    backend->port_set_prop = port_set_prop;
    backend->dio_get = dio_get;
    backend->dio_set = dio_set;
    backend->aio_get = aio_get;
    backend->pwm_get_duty = pwm_getduty;
    backend->pwm_set_duty = pwm_setduty;
    backend->data = NULL;

    return HAL_SUCCESS;
}

void hal_backend_shutdown(hal_backend_t* backend) {

}
