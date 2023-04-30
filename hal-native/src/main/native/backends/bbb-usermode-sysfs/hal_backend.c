
#include <malloc.h>
#include <memory.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal.h>
#include <bbb_gpiodef.h>

#include "sysfs/gpio.h"


static uint32_t probe(hal_backend_t* env, hal_port_t port) {
    return HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT;
}

static hal_error_t open(hal_backend_t* env, hal_port_t port, hal_port_type_t type, void** data) {
    if (type == HAL_TYPE_DIGITAL_INPUT) {
        HAL_RETURN_IF_ERROR(gpio_export_pin(port));
        HAL_RETURN_IF_ERROR(gpio_set_direction(port, DIR_INPUT));
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        HAL_RETURN_IF_ERROR(gpio_export_pin(port));
        HAL_RETURN_IF_ERROR(gpio_set_direction(port, DIR_OUTPUT));
    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

static hal_error_t close(hal_backend_t* env, hal_port_t port, hal_port_type_t type, void* data) {
    if (type == HAL_TYPE_DIGITAL_INPUT) {
        HAL_RETURN_IF_ERROR(gpio_unexport_pin(port));
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        HAL_RETURN_IF_ERROR(gpio_unexport_pin(port));
    }

    return HAL_SUCCESS;
}

static hal_error_t dio_get(hal_backend_t* env, hal_port_t port, void* data, hal_dio_value_t* value) {
    HAL_RETURN_IF_ERROR(gpio_get_value(port, value));
    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_backend_t* env, hal_port_t port, void* data, hal_dio_value_t value) {
    HAL_RETURN_IF_ERROR(gpio_set_value(port, value));
    return HAL_SUCCESS;
}


hal_error_t hal_backend_init(hal_backend_t* backend) {
    backend->name = "bbb-usermode-sysfs";
    backend->probe = probe;
    backend->open = open;
    backend->close = close;
    backend->dio_get = dio_get;
    backend->dio_set = dio_set;
    backend->data = NULL;

    return HAL_SUCCESS;
}

void hal_backend_shutdown(hal_backend_t* backend) {

}
