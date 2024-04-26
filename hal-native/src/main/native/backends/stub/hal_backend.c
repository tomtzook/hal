
#include <malloc.h>
#include <memory.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal.h>


static hal_error_t port_iter_struct_size(hal_backend_t* env) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t port_iter_start(hal_backend_t* env, hal_port_iter_t* iter) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t port_iter_next(hal_backend_t* env, hal_port_iter_t* iter) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static uint32_t probe(hal_backend_t* env, const char* port_name) {
    return HAL_TYPE_PWM_OUTPUT | HAL_TYPE_ANALOG_INPUT | HAL_TYPE_ANALOG_OUTPUT | HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT;
}

static hal_error_t open(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data) {
    return HAL_SUCCESS;
}

static hal_error_t close(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data) {
    return HAL_SUCCESS;
}

hal_error_t port_probe_prop(hal_backend_t* env, const char* port_name, hal_port_type_t type, hal_prop_key_t key, hal_config_flags_t* flags) {
    hal_config_flags_t _flags;
    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            _flags |= HAL_CONFIG_FLAG_READABLE | HAL_CONFIG_FLAG_WRITABLE;
            break;
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            _flags |= HAL_CONFIG_FLAG_READABLE | HAL_CONFIG_FLAG_WRITABLE;
            break;
        }
        case HAL_CONFIG_ANALOG_MAX_VALUE: {
            _flags |= HAL_CONFIG_FLAG_READABLE;
            break;
        }
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE: {
            _flags |= HAL_CONFIG_FLAG_READABLE;
            break;
        }
        case HAL_CONFIG_ANALOG_SAMPLE_RATE: {
            _flags |= HAL_CONFIG_FLAG_READABLE;
            break;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
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
    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            *value = HAL_GPIO_CONFIG_EDGE_NONE;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            *value = HAL_GPIO_CONFIG_RESISTOR_NONE;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_MAX_VALUE: {
            *value = 0;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_MAX_VOLTAGE: {
            *value = 0;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_ANALOG_SAMPLE_RATE: {
            *value = 0;
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            *value = 0;
            return HAL_SUCCESS;
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }
}

hal_error_t port_set_prop(hal_backend_t* env, const char* port_name, hal_port_type_t type, void* data,
                          hal_prop_key_t key, hal_prop_value_t value) {
    switch (key) {
        case HAL_CONFIG_GPIO_POLL_EDGE: {
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_GPIO_RESISTOR: {
            return HAL_SUCCESS;
        }
        case HAL_CONFIG_PWM_FREQUENCY: {
            return HAL_SUCCESS;
        }
        default:
            return HAL_ERROR_CONFIG_KEY_NOT_SUPPORTED;
    }
}

static hal_error_t dio_get(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t* value) {
    *value = HAL_DIO_LOW;
    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t value) {
    return HAL_SUCCESS;
}

static hal_error_t aio_get(hal_backend_t* env, const char* port_name, void* data, hal_aio_value_t* value) {
    *value = 0;
    return HAL_SUCCESS;
}

static hal_error_t pwm_getduty(hal_backend_t* env, const char* port_name, void* data, uint32_t* value) {
    *value = 0;
    return HAL_SUCCESS;
}

static hal_error_t pwm_setduty(hal_backend_t* env, const char* port_name, void* data, uint32_t value) {
    return HAL_SUCCESS;
}

hal_error_t hal_backend_init(hal_backend_t* backend) {
    backend->name = "stub";
    backend->port_iter_struct_size = port_iter_struct_size;
    backend->port_iter_start = port_iter_start;
    backend->port_iter_next = port_iter_next;
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
