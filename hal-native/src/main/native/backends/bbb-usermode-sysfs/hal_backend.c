
#include <malloc.h>
#include <memory.h>

#include <hal_error_handling.h>
#include <hal_backend.h>
#include <hal.h>

#include "sysfs/gpio.h"
#include "sysfs/adc.h"


#define PIN_NUMBER(module, index) ((module) * 32 + (index))

typedef struct {
    const char* name;
    unsigned pin_number;
    uint32_t supported_types;
} pin_t;

pin_t PINS[] = {
        {"P8_13", PIN_NUMBER(0, 23), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // EHRPWM2B
        {"P8_14", PIN_NUMBER(0, 26), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // EHRPWM2A
        {"P8_17", PIN_NUMBER(0, 27), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_19", PIN_NUMBER(0, 22), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_31", PIN_NUMBER(0, 10), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // UART5_CTSN
        {"P8_32", PIN_NUMBER(0, 11), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // UART5_RTSN
        {"P8_33", PIN_NUMBER(0, 9), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},   // UART4_RTSN
        {"P8_35", PIN_NUMBER(0, 8), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},   // UART4_CTSN
        {"P9_11", PIN_NUMBER(0, 30), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // UART4_RXD
        {"P9_13", PIN_NUMBER(0, 31), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // UART4_TXD
        {"P9_17", PIN_NUMBER(0, 5), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},   // I2C1_SCL
        {"P9_18", PIN_NUMBER(0, 4), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},   // I2C1_SDA
        {"P9_19", PIN_NUMBER(0, 13), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // I2C2_SCL
        {"P9_20", PIN_NUMBER(0, 12), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // I2C2_SDA
        {"P9_21", PIN_NUMBER(0, 3), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},   // UART2_TXD
        {"P9_22", PIN_NUMBER(0, 2), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},   // UART2_RXD
        {"P9_24", PIN_NUMBER(0, 15), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // UART1_RXD
        {"P9_26", PIN_NUMBER(0, 14), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},  // UART1_RXD
        {"P9_41A", PIN_NUMBER(0, 20), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_42A", PIN_NUMBER(0, 7), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_03", PIN_NUMBER(1, 6), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_04", PIN_NUMBER(1, 7), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_05", PIN_NUMBER(1, 2), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_06", PIN_NUMBER(1, 3), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_11", PIN_NUMBER(1, 13), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_12", PIN_NUMBER(1, 12), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_15", PIN_NUMBER(1, 15), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_16", PIN_NUMBER(1, 14), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_20", PIN_NUMBER(1, 31), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_21", PIN_NUMBER(1, 30), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_22", PIN_NUMBER(1, 5), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_23", PIN_NUMBER(1, 4), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_24", PIN_NUMBER(1, 1), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_25", PIN_NUMBER(1, 0), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_26", PIN_NUMBER(1, 29), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_12", PIN_NUMBER(1, 28), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_14", PIN_NUMBER(1, 18), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_15", PIN_NUMBER(1, 16), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // EHRPWM1A
        {"P9_16", PIN_NUMBER(1, 19), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // EHRPWM1B
        {"P9_23", PIN_NUMBER(1, 17), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"USR0", PIN_NUMBER(1, 21), HAL_TYPE_DIGITAL_OUTPUT},
        {"USR1", PIN_NUMBER(1, 22), HAL_TYPE_DIGITAL_OUTPUT},
        {"USR2", PIN_NUMBER(1, 23), HAL_TYPE_DIGITAL_OUTPUT},
        {"USR3", PIN_NUMBER(1, 24), HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_07", PIN_NUMBER(2, 2), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // TIMER4
        {"P8_08", PIN_NUMBER(2, 3), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // TIMER7
        {"P8_09", PIN_NUMBER(2, 5), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // TIMER5
        {"P8_10", PIN_NUMBER(2, 4), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // TIMER6
        {"P8_18", PIN_NUMBER(2, 1), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_27", PIN_NUMBER(2, 22), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_28", PIN_NUMBER(2, 24), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_29", PIN_NUMBER(2, 23), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_30", PIN_NUMBER(2, 25), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_34", PIN_NUMBER(2, 17), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // UART3_RTSN
        {"P8_36", PIN_NUMBER(2, 16), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // UART3_CTSN
        {"P8_37", PIN_NUMBER(2, 14), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // UART5_TXD
        {"P8_38", PIN_NUMBER(2, 15), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // UART5_RXD
        {"P8_39", PIN_NUMBER(2, 12), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_40", PIN_NUMBER(2, 13), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_41", PIN_NUMBER(2, 10), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_42", PIN_NUMBER(2, 11), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_43", PIN_NUMBER(2, 8), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_44", PIN_NUMBER(2, 9), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_45", PIN_NUMBER(2, 6), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P8_46", PIN_NUMBER(2, 7), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_25", PIN_NUMBER(3, 21), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_27", PIN_NUMBER(3, 19), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_28", PIN_NUMBER(3, 17), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // SPI1_CS0
        {"P9_29", PIN_NUMBER(3, 15), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // SPI1_d0
        {"P9_30", PIN_NUMBER(3, 16), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // SPI1_D1
        {"P9_31", PIN_NUMBER(3, 14), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT}, // SPI1_SCLK
        {"P9_41B", PIN_NUMBER(3, 20), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_42B", PIN_NUMBER(3, 18), HAL_TYPE_DIGITAL_INPUT | HAL_TYPE_DIGITAL_OUTPUT},
        {"P9_33", 4, HAL_TYPE_ANALOG_INPUT},
        {"P9_35", 6, HAL_TYPE_ANALOG_INPUT},
        {"P9_36", 5, HAL_TYPE_ANALOG_INPUT},
        {"P9_37", 2, HAL_TYPE_ANALOG_INPUT},
        {"P9_38", 3, HAL_TYPE_ANALOG_INPUT},
        {"P9_39", 0, HAL_TYPE_ANALOG_INPUT},
        {"P9_40", 1, HAL_TYPE_ANALOG_INPUT},
};


pin_t* find_pin_def_for_name(const char* port_name) {
    for (int i = 0; i < sizeof(PINS) / sizeof(pin_t); ++i) {
        pin_t* pin = PINS + i;
        if (0 == strcmp(pin->name, port_name)) {
            return pin;
        }
    }

    return NULL;
}


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

    if (type == HAL_TYPE_DIGITAL_INPUT) {
        HAL_RETURN_IF_ERROR(gpio_export_pin(pin->pin_number));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin->pin_number, DIR_INPUT));
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        HAL_RETURN_IF_ERROR(gpio_export_pin(pin->pin_number));
        HAL_RETURN_IF_ERROR(gpio_set_direction(pin->pin_number, DIR_OUTPUT));
    } else if (type == HAL_TYPE_ANALOG_INPUT) {

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
        HAL_RETURN_IF_ERROR(gpio_unexport_pin(pin->pin_number));
    } else if (type == HAL_TYPE_DIGITAL_OUTPUT){
        HAL_RETURN_IF_ERROR(gpio_unexport_pin(pin->pin_number));
    } else if (type == HAL_TYPE_ANALOG_INPUT) {

    } else {
        return HAL_ERROR_UNSUPPORTED_OPERATION;
    }

    return HAL_SUCCESS;
}

static hal_error_t dio_get(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t* value) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(gpio_get_value(pin->pin_number, value));
    return HAL_SUCCESS;
}

static hal_error_t dio_set(hal_backend_t* env, const char* port_name, void* data, hal_dio_value_t value) {
    pin_t* pin = find_pin_def_for_name(port_name);
    if (NULL == pin) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(gpio_set_value(pin->pin_number, value));
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


hal_error_t hal_backend_init(hal_backend_t* backend) {
    backend->name = "bbb-usermode-sysfs";
    backend->probe = probe;
    backend->open = open;
    backend->close = close;
    backend->dio_get = dio_get;
    backend->dio_set = dio_set;
    backend->aio_get = aio_get;
    backend->data = NULL;

    return HAL_SUCCESS;
}

void hal_backend_shutdown(hal_backend_t* backend) {

}
