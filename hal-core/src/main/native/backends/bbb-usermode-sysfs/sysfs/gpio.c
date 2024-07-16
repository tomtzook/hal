
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <unistd.h>

#include <hal_error_handling.h>

#include "common.h"
#include "pins.h"
#include "gpio.h"


static const char* SYSFS_EXPORT = "/sys/class/gpio/export";
static const char* SYSFS_UNEXPORT = "/sys/class/gpio/unexport";

static const char* SYSFS_FILE_FORMAT = "/sys/class/gpio/gpio%d/%s";

static const char* FILE_DIRECTION = "direction";
static const char* FILE_EDGE = "edge";
static const char* FILE_VALUE = "value";

static const char* STR_RESISTOR_NONE = "gpio";
static const char* STR_RESISTOR_PULLDOWN = "gpio_pd";
static const char* STR_RESISTOR_PULLUP = "gpio_pu";

static const char* STR_DIRECTION_INPUT = "in";
static const char* STR_DIRECTION_OUTPUT = "out";

static const char* STR_EDGE_NONE = "none";
static const char* STR_EDGE_RISING = "rising";
static const char* STR_EDGE_FALLING = "falling";
static const char* STR_EDGE_BOTH = "both";

static const char* STR_VALUE_LOW = "0";
static const char* STR_VALUE_HIGH = "1";


static hal_error_t read_numbered_file(unsigned number, const char* file, char* buffer, size_t size) {
    char path[PATH_MAX] = {0};
    sprintf(path, SYSFS_FILE_FORMAT, number, file);
    return read_file(path, buffer, size);
}

static hal_error_t write_numbered_file(unsigned number, const char* file, const char* buffer) {
    char path[PATH_MAX] = {0};
    sprintf(path, SYSFS_FILE_FORMAT, number, file);
    return write_file(path, buffer);
}


hal_error_t gpio_export_pin(pin_t* pin) {
    if (gpio_is_exported_pin(pin)) {
        return HAL_SUCCESS;
    }

    char str[3];
    sprintf(str, "%d", pin->pin_number);
    HAL_RETURN_IF_ERROR(write_file(SYSFS_EXPORT, str));

    return HAL_SUCCESS;
}

hal_error_t gpio_unexport_pin(pin_t* pin) {
    if (!gpio_is_exported_pin(pin)) {
        return HAL_SUCCESS;
    }

    char str[3];
    sprintf(str, "%d", pin->pin_number);
    HAL_RETURN_IF_ERROR(write_file(SYSFS_UNEXPORT, str));

    return HAL_SUCCESS;
}

int gpio_is_exported_pin(pin_t* pin) {
    char path[PATH_MAX] = {0};
    sprintf(path, SYSFS_FILE_FORMAT, pin->pin_number, FILE_VALUE);
    return 0 == access(path, F_OK);
}

hal_error_t gpio_set_pinmux(pin_t* pin, hal_dio_config_resistor_t resistor) {
    const char* to_write;
    switch (resistor) {
        case HAL_CONFIG_DIO_RESISTOR_NONE:
            to_write = STR_RESISTOR_NONE;
            break;
        case HAL_CONFIG_DIO_RESISTOR_PULLDOWN:
            to_write = STR_RESISTOR_PULLDOWN;
            break;
        case HAL_CONFIG_DIO_RESISTOR_PULLUP:
            to_write = STR_RESISTOR_PULLUP;
            break;
        default:
            return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(set_pin_mode(pin, to_write));

    return HAL_SUCCESS;
}

hal_error_t gpio_set_direction(pin_t* pin, direction_t direction) {
    const char* to_write;
    switch (direction) {
        case DIR_INPUT:
            to_write = STR_DIRECTION_INPUT;
            break;
        case DIR_OUTPUT:
            to_write = STR_DIRECTION_OUTPUT;
            break;
        default:
            return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(write_numbered_file(pin->pin_number, FILE_DIRECTION, to_write));
    return HAL_SUCCESS;
}

hal_error_t gpio_set_edge(pin_t* pin, hal_dio_config_poll_edge_t edge) {
    const char* to_write;
    switch (edge) {
        case HAL_CONFIG_DIO_EDGE_NONE:
            to_write = STR_EDGE_NONE;
            break;
        case HAL_CONFIG_DIO_EDGE_RISING:
            to_write = STR_EDGE_RISING;
            break;
        case HAL_CONFIG_DIO_EDGE_FALLING:
            to_write = STR_EDGE_FALLING;
            break;
        case HAL_CONFIG_DIO_EDGE_BOTH:
            to_write = STR_EDGE_BOTH;
            break;
        default:
            return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(write_numbered_file(pin->pin_number, FILE_EDGE, to_write));
    return HAL_SUCCESS;
}

hal_error_t gpio_set_value(pin_t* pin, hal_dio_value_t value) {
    const char* to_write;
    switch (value) {
        case HAL_DIO_LOW:
            to_write = STR_VALUE_LOW;
            break;
        case HAL_DIO_HIGH:
            to_write = STR_VALUE_HIGH;
            break;
        default:
            return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(write_numbered_file(pin->pin_number, FILE_VALUE, to_write));
    return HAL_SUCCESS;
}

hal_error_t gpio_get_pinmux(pin_t* pin, hal_dio_config_resistor_t* resistor) {
    char buffer[32] = {0};
    HAL_RETURN_IF_ERROR(get_pin_mode(pin, buffer));

    if (0 == strcmp(buffer, STR_RESISTOR_PULLDOWN)) {
        *resistor = HAL_CONFIG_DIO_RESISTOR_PULLDOWN;
    } else if (0 == strcmp(buffer, STR_RESISTOR_PULLUP)) {
        *resistor = HAL_CONFIG_DIO_RESISTOR_PULLUP;
    } else if (0 == strcmp(buffer, STR_RESISTOR_NONE)) {
        *resistor = HAL_CONFIG_DIO_RESISTOR_NONE;
    } else {
        return HAL_ERROR_BAD_DATA;
    }

    return HAL_SUCCESS;
}

hal_error_t gpio_get_edge(pin_t* pin, hal_dio_config_poll_edge_t* edge) {
    char buffer[32] = {0};
    HAL_RETURN_IF_ERROR(read_numbered_file(pin->pin_number, FILE_EDGE, buffer, sizeof(buffer)));

    if (0 == strcmp(buffer, STR_EDGE_FALLING)) {
        *edge = HAL_CONFIG_DIO_EDGE_FALLING;
    } else if (0 == strcmp(buffer, STR_EDGE_RISING)) {
        *edge = HAL_CONFIG_DIO_EDGE_RISING;
    } else if (0 == strcmp(buffer, STR_EDGE_BOTH)) {
        *edge = HAL_CONFIG_DIO_EDGE_BOTH;
    } else if (0 == strcmp(buffer, STR_EDGE_NONE)) {
        *edge = HAL_CONFIG_DIO_EDGE_NONE;
    } else {
        return HAL_ERROR_BAD_DATA;
    }

    return HAL_SUCCESS;
}

hal_error_t gpio_get_value(pin_t* pin, hal_dio_value_t* value) {
    char buffer[1] = {0};
    HAL_RETURN_IF_ERROR(read_numbered_file(pin->pin_number, FILE_VALUE, buffer, sizeof(buffer)));

    if (buffer[0] == '0') {
        *value = HAL_DIO_LOW;
    } else if (buffer[0] == '1') {
        *value = HAL_DIO_HIGH;
    } else {
        return HAL_ERROR_BAD_DATA;
    }

    return HAL_SUCCESS;
}
