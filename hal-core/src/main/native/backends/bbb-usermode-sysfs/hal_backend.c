
#include <malloc.h>
#include <memory.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal.h>

#include "sysfs/gpio.h"
#include "sysfs/adc.h"
#include "sysfs/pins.h"
#include "sysfs/pwm.h"


static pin_t* get_pin_from_data(void* data) {
    pin_t** pin_out = (pin_t**) data;
    return *pin_out;
}

static hal_error_t port_iter_struct_size(hal_env_t* env) {
    return sizeof(size_t);
}

static hal_error_t port_iter_start(hal_env_t* env, hal_port_iter_t* iter) {
    size_t* index = (size_t*)(iter->_iter_data);
    *index = 0;

    pin_t* pin = find_pin_def_for_index(*index);
    iter->supported_types = pin->supported_types;
    strcpy(iter->name, pin->name);

    return HAL_SUCCESS;
}

static hal_error_t port_iter_next(hal_env_t* env, hal_port_iter_t* iter) {
    size_t* index = (size_t*)(iter->_iter_data);
    (*index)++;

    pin_t* pin = find_pin_def_for_index(*index);
    if (pin == NULL) {
        return HAL_ERROR_ITER_END;
    }

    iter->supported_types = pin->supported_types;
    strcpy(iter->name, pin->name);

    return HAL_SUCCESS;
}

static size_t native_data_size_for_port(hal_env_t* env, hal_port_type_t type) {
    if (type == HAL_TYPE_PWM_OUTPUT) {
        return sizeof(pwm_t);
    } else if (type == HAL_TYPE_DIGITAL_INPUT || type == HAL_TYPE_DIGITAL_OUTPUT || type == HAL_TYPE_ANALOG_INPUT) {
        return sizeof(pin_t**);
    }

    return 0;
}

static hal_error_t probe(hal_env_t* env, const char* port_name, uint32_t* types) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    *types = pin->supported_types;
    return HAL_SUCCESS;
}

static hal_error_t open(hal_env_t* env, const char* port_name, hal_port_type_t type, void* data) {
    // TODO: BETTER TEARDOWN ON FAILURES
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    if ((pin->supported_types & type) != type) {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    if (type == HAL_TYPE_DIGITAL_INPUT) {
        pin_t** pin_out = (pin_t**) data;
        *pin_out = pin;

        HAL_RETURN_IF_ERROR(gpio_export_pin(pin));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin, DIR_INPUT));
        HAL_RETURN_IF_ERROR(gpio_set_pinmux(pin, HAL_GPIO_CONFIG_RESISTOR_PULLDOWN));
        HAL_RETURN_IF_ERROR(gpio_set_edge(pin, HAL_GPIO_CONFIG_EDGE_RISING));
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        pin_t** pin_out = (pin_t**) data;
        *pin_out = pin;

        HAL_RETURN_IF_ERROR(gpio_export_pin(pin));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin, DIR_OUTPUT));
    } else if (type == HAL_TYPE_ANALOG_INPUT) {
        pin_t** pin_out = (pin_t**) data;
        *pin_out = pin;

        // no action needed
    } else if (type == HAL_TYPE_PWM_OUTPUT) {
        const char* module_name = get_pwm_module_name_for_pin(pin);
        if (module_name == NULL) {
            return HAL_ERROR_BAD_ARGUMENT;
        }

        hal_error_t status;

        pwm_pin_t* pwm_pin = get_pwm_pin_for_module(module_name);
        pwm_t* pwm = (pwm_t*) data;
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

static hal_error_t close(hal_env_t* env, const hal_open_port_t* port) {
    pin_t* pin = find_pin_def_for_name(port->name);
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

static hal_error_t port_probe_prop(hal_env_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key, uint32_t* flags) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    uint32_t _flags;
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

static hal_error_t port_get_prop(hal_env_t* env, const hal_open_port_t* port,
                          hal_prop_key_t key, hal_prop_value_t* value) {
    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            if (port->type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            return gpio_get_edge(pin, value);
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            if (port->type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            return gpio_get_pinmux(pin, value);
        }
        case HAL_CONFIG_ANALOG_MAX_VALUE: {
            if (port->type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            *value = ANALOG_MAX_VALUE;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE: {
            if (port->type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            *value = ANALOG_MAX_VOLTAGE_MV;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_SAMPLE_RATE: {
            if (port->type != HAL_TYPE_ANALOG_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            *value = ANALOG_SAMPLE_RATE_PERIOD_US;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            if (port->type != HAL_TYPE_PWM_OUTPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pwm_t* pwm = (pwm_t*) port->data;
            *value = pwm->period_ns / 1000;
            return HAL_SUCCESS;
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }
}

static hal_error_t port_set_prop(hal_env_t* env, const hal_open_port_t* port,
                          hal_prop_key_t key, hal_prop_value_t value) {
    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            if (port->type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pin_t* pin = get_pin_from_data(port->data);
            if (NULL == pin) {
                return HAL_ERROR_BAD_ARGUMENT;
            }

            return gpio_set_edge(pin, value);
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            if (port->type != HAL_TYPE_DIGITAL_INPUT) {
                return HAL_ERROR_UNSUPPORTED_OPERATION;
            }

            pin_t* pin = get_pin_from_data(port->data);
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

static hal_error_t dio_get(hal_env_t* env, const hal_open_port_t* port, hal_dio_value_t* value) {
    pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(gpio_get_value(pin, value));
    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_env_t* env, const hal_open_port_t* port, hal_dio_value_t value) {
    pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(gpio_set_value(pin, value));
    return HAL_SUCCESS;
}

static hal_error_t aio_get(hal_env_t* env, const hal_open_port_t* port, uint32_t* value) {
    pin_t* pin = get_pin_from_data(port->data);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(adc_read(pin->pin_number, value));
    return HAL_SUCCESS;
}

static hal_error_t pwm_getduty(hal_env_t* env, const hal_open_port_t* port, uint32_t* value) {
    pwm_t* pwm = (pwm_t*) port->data;
    *value = pwm->duty_ns / 1000;

    return HAL_SUCCESS;
}

static hal_error_t pwm_setduty(hal_env_t* env, const hal_open_port_t* port, uint32_t value) {
    pwm_t* pwm = (pwm_t*) port->data;
    return pwm_set_duty_cycle(pwm, value);
}

hal_error_t hal_backend_init(hal_env_t* env) {
    hal_backend_t* backend = hal_get_backend(env);

    backend->name = "bbb-usermode-sysfs";
    backend->port_iter_struct_size = port_iter_struct_size;
    backend->port_iter_start = port_iter_start;
    backend->port_iter_next = port_iter_next;
    backend->native_data_size_for_port = native_data_size_for_port;
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

void hal_backend_shutdown(hal_env_t* env) {
    
}
