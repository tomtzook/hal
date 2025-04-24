
#include "memdefs.h"
#include "gpio.h"



hal_error_t gpio_get_pinmux(const bbb_env_t* env, const pin_t* pin, hal_dio_config_resistor_t* resistor) {

    return HAL_SUCCESS;
}

hal_error_t gpio_get_edge(const bbb_env_t* env, const pin_t* pin, hal_dio_config_poll_edge_t* edge) {

    // todo:
    return HAL_SUCCESS;
}

hal_error_t gpio_get_value(const bbb_env_t* env, const pin_t* pin, direction_t dir, hal_dio_value_t* value) {
    unsigned int reg_value = 0;
    if (direction == DIR_OUTPUT) {
        reg_value = ((volatile gpio_registers_t*) env->gpio_peripherals[pin->module_index].base)->dataout;
    } else if (direction == DIR_INPUT) {
        reg_value = ((volatile gpio_registers_t*) env->gpio_peripherals[pin->module_index].base)->datain;
    } else {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    *value = (reg_value & (1 << pin->pin_index)) != 0 ? HAL_DIO_HIGH : HAL_DIO_LOW;

    return HAL_SUCCESS;
}

hal_error_t gpio_set_direction(const bbb_env_t* env, const pin_t* pin, direction_t direction) {
    if (direction == DIR_OUTPUT) {
        ((volatile gpio_registers_t*) env->gpio_peripherals[pin->module_index].base)->oe &= ~(1 << pin->pin_index);
    } else if (direction == DIR_INPUT) {
        ((volatile gpio_registers_t*) env->gpio_peripherals[pin->module_index].base)->oe |= (1 << pin->pin_index);
    } else {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    return HAL_SUCCESS;
}

hal_error_t gpio_set_pinmux(const bbb_env_t* env, const pin_t* pin, hal_dio_config_resistor_t resistor) {

    return HAL_SUCCESS;
}

hal_error_t gpio_set_edge(const bbb_env_t* env, const pin_t* pin, hal_dio_config_poll_edge_t edge) {

    // todo:
    return HAL_SUCCESS;
}

hal_error_t gpio_set_value(const bbb_env_t* env, const pin_t* pin, hal_dio_value_t value) {
    if (value == HAL_DIO_HIGH) {
        ((volatile gpio_registers_t*) env->gpio_peripherals[pin->module_index].base)->setdataout |= (1 << pin->pin_index);
    } else if (value == HAL_DIO_LOW) {
        ((volatile gpio_registers_t*) env->gpio_peripherals[pin->module_index].base)->cleardataout |= (1 << pin->pin_index);
    } else {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    return HAL_SUCCESS;
}
