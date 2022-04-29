
#include <hal.h>
#include <malloc.h>
#include <memory.h>

#include "include/bbb_gpiodef.h"
#include "memio/peripheral.h"
#include "memio/regs.h"


typedef struct {
    peripheral_t gpio_peripherals[4];
} context_t;

static hal_error_t ports_init(hal_env_t* env, void** data) {
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

    *data = context;
    return HAL_SUCCESS;
error:
    for (int i = 0; i < 4; ++i) {
        free_peripheral(&context->gpio_peripherals[i]);
    }
    free(context);

    return status;
}

static void ports_free(hal_env_t* env, void* data) {
    context_t* context = (context_t*) data;

    for (int i = 0; i < 4; ++i) {
        free_peripheral(&context->gpio_peripherals[i]);
    }

    free(context);
}

static hal_error_t ports_probe(hal_env_t* env, void* data, hal_port_t port, uint8_t flags) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t dio_port_init(hal_env_t* env, void* data, dio_port_t* port) {
    context_t* context = (context_t*) data;

    unsigned module = MODULE(port->port);
    unsigned pin = PIN(port->port);

    peripheral_t* peripheral = context->gpio_peripherals + module;
    if (port->dir == PORT_DIR_INPUT) {
        REG(peripheral, gpio_registers)->oe |= PIN_MASK(pin);
    } else {
        REG(peripheral, gpio_registers)->oe &= ~PIN_MASK(pin);
    }

    return HAL_SUCCESS;
}

static void dio_port_free(hal_env_t* env, void* data, dio_port_t* port) {

}

static hal_error_t dio_port_write(hal_env_t* env, void* data, dio_port_t* port, hal_dio_value_t value) {
    context_t* context = (context_t*) data;

    unsigned module = MODULE(port->port);
    unsigned pin = PIN(port->port);

    peripheral_t* peripheral = context->gpio_peripherals + module;
    if (value == HAL_DIO_HIGH) {
        REG(peripheral, gpio_registers)->setdataout |= PIN_MASK(pin);
    } else {
        REG(peripheral, gpio_registers)->cleardataout |= PIN_MASK(pin);
    }
    
    return HAL_SUCCESS;
}

static hal_error_t dio_port_read(hal_env_t* env, void* data, dio_port_t* port, hal_dio_value_t* value) {
    context_t* context = (context_t*) env;

    unsigned module = MODULE(port->port);
    unsigned pin = PIN(port->port);

    peripheral_t* peripheral = context->gpio_peripherals + module;
    if (REG(peripheral, gpio_registers)->datain & PIN_MASK(pin)) {
        *value = HAL_DIO_HIGH;
    } else {
        *value = HAL_DIO_LOW;
    }
    
    return HAL_SUCCESS;
}

static hal_error_t aio_port_init(hal_env_t* env, void* data, aio_port_t* port) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static void aio_port_free(hal_env_t* env, void* data, aio_port_t* port) {

}

static hal_error_t aio_port_write(hal_env_t* env, void* data, aio_port_t* port, hal_aio_value_t value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t aio_port_read(hal_env_t* env, void* data, aio_port_t* port, hal_aio_value_t* value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_aio_value_t aio_max_value(hal_env_t* env, void* data) {
    return 0;
}

static hal_error_t pwm_init(hal_env_t* env, void* data, pwm_port_t* port) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static void pwm_free(hal_env_t* env, void* data, pwm_port_t* port) {

}

static hal_error_t pwm_write(hal_env_t* env, void* data, pwm_port_t* port, hal_pwm_value_t value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_error_t pwm_frequency_read(hal_env_t* env, void* data, pwm_port_t* port, float* value) {
    return HAL_ERROR_UNSUPPORTED_OPERATION;
}

static hal_pwm_value_t pwm_max_value(hal_env_t* env, void* data) {
    return 0;
}

ports_native_interface _ports_native_interface = {
    .init = ports_init,
    .free = ports_free,
    .probe = ports_probe,
    .dio_init = dio_port_init,
    .dio_free = dio_port_free,
    .dio_write = dio_port_write,
    .dio_read = dio_port_read,
    .aio_init = aio_port_init,
    .aio_free = aio_port_free,
    .aio_write = aio_port_write,
    .aio_read = aio_port_read,
    .aio_max_value = aio_max_value,
    .pwm_init = pwm_init,
    .pwm_free = pwm_free,
    .pwm_write = pwm_write,
    .pwm_frequency_read = pwm_frequency_read,
    .pwm_max_value = pwm_max_value
};
