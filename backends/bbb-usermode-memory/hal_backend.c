
#include <malloc.h>
#include <memory.h>

#include <hal_backend.h>
#include <hal.h>

#include "include/bbb_gpiodef.h"
#include "memio/peripheral.h"
#include "memio/regs.h"


typedef struct {
    peripheral_t gpio_peripherals[4];
} context_t;


static uint32_t probe(hal_backend_t* env, hal_port_t port) {
    return HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT;
}

static hal_error_t open(hal_backend_t* env, hal_port_t port, hal_port_type_t type, void** data) {
    context_t* context = (context_t*) env->data;

    if (type == HAL_TYPE_DIGITAL_INPUT) {
        unsigned module = MODULE(port);
        unsigned pin = PIN(port);

        peripheral_t* peripheral = context->gpio_peripherals + module;
        REG(peripheral, gpio_registers)->oe |= PIN_MASK(pin);
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        unsigned module = MODULE(port);
        unsigned pin = PIN(port);

        peripheral_t* peripheral = context->gpio_peripherals + module;
        REG(peripheral, gpio_registers)->oe &= ~PIN_MASK(pin);
    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

static hal_error_t close(hal_backend_t* env, hal_port_t port, hal_port_type_t type, void* data) {
    return HAL_SUCCESS;
}

static hal_error_t dio_get(hal_backend_t* env, hal_port_t port, void* data, hal_dio_value_t* value) {
    context_t* context = (context_t*) env->data;

    unsigned module = MODULE(port);
    unsigned pin = PIN(port);

    peripheral_t* peripheral = context->gpio_peripherals + module;
    if (REG(peripheral, gpio_registers)->datain & PIN_MASK(pin)) {
        *value = HAL_DIO_HIGH;
    } else {
        *value = HAL_DIO_LOW;
    }

    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_backend_t* env, hal_port_t port, void* data, hal_dio_value_t value) {
    context_t* context = (context_t*) env->data;

    unsigned module = MODULE(port);
    unsigned pin = PIN(port);

    peripheral_t* peripheral = context->gpio_peripherals + module;
    if (value == HAL_DIO_HIGH) {
        REG(peripheral, gpio_registers)->setdataout |= PIN_MASK(pin);
    } else {
        REG(peripheral, gpio_registers)->cleardataout |= PIN_MASK(pin);
    }

    return HAL_SUCCESS;
}


hal_error_t hal_backend_init(hal_backend_t* backend) {
    context_t* context = (context_t*) malloc(sizeof(context_t));
    HAL_CHECK_ALLOCATED(context);
    memset(context, 0, sizeof(context_t));

    hal_error_t status;
    // init gpio peripherals
    status = map_peripheral(GPIO0, GPIO_CTRL_SIZE, &context->gpio_peripherals[0]);
    HAL_JUMP_IF_ERROR(status, error);
    status = map_peripheral(GPIO1, GPIO_CTRL_SIZE, &context->gpio_peripherals[1]);
    HAL_JUMP_IF_ERROR(status, error);
    status = map_peripheral(GPIO2, GPIO_CTRL_SIZE, &context->gpio_peripherals[2]);
    HAL_JUMP_IF_ERROR(status, error);
    status = map_peripheral(GPIO3, GPIO_CTRL_SIZE, &context->gpio_peripherals[3]);
    HAL_JUMP_IF_ERROR(status, error);

    backend->probe = probe;
    backend->open = open;
    backend->close = close;
    backend->dio_get = dio_get;
    backend->dio_set = dio_set;
    backend->data = context;
    return HAL_SUCCESS;
error:
    for (int i = 0; i < 4; ++i) {
        free_peripheral(&context->gpio_peripherals[i]);
    }
    free(context);

    return status;
}

void hal_backend_shutdown(hal_backend_t* backend) {
    context_t* context = (context_t*) backend->data;

    for (int i = 0; i < 4; ++i) {
        free_peripheral(&context->gpio_peripherals[i]);
    }

    free(context);
}
