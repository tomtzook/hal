
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <errno.h>

#include "gpio.h"


static const char* SYSFS_EXPORT = "/sys/class/gpio/export";
static const char* SYSFS_UNEXPORT = "/sys/class/gpio/unexport";

static const char* SYSFS_FILE_FORMAT = "/sys/class/gpio/gpio%d/%s";

static const char* FILE_DIRECTION = "direction";
static const char* FILE_EDGE = "edge";
static const char* FILE_VALUE = "value";

static const char* STR_DIRECTION_INPUT = "in";
static const char* STR_DIRECTION_OUTPUT = "out";

static const char* STR_EDGE_NONE = "none";
static const char* STR_EDGE_RISING = "rising";
static const char* STR_EDGE_FALLING = "falling";
static const char* STR_EDGE_BOTH = "both";

static const char* STR_VALUE_LOW = "0";
static const char* STR_VALUE_HIGH = "1";


static hal_error_t errno_to_hal_error() {
    switch (errno) {
        case EPERM:
            return HAL_ERROR_PERMISSIONS;
        default:
            return HAL_ERROR_UNKNOWN;
    }
}

static hal_error_t read_numbered_file(unsigned number, const char* file, const char* buffer, size_t size) {
    char path[PATH_MAX] = {0};
    sprintf(path, SYSFS_FILE_FORMAT, number, file);

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return errno_to_hal_error();
    }

    hal_error_t status = HAL_SUCCESS;
    ssize_t amount = read(fd, buffer, size);
    if (amount < 0) {
        status = errno_to_hal_error();
        goto done;
    }

done:
    close(fd);
    return status;
}

static hal_error_t write_file(const char* path, const char* buffer) {
    int fd = open(path, O_WRONLY);
    if (fd < 0) {
        return errno_to_hal_error();
    }

    hal_error_t status = HAL_SUCCESS;
    ssize_t amount = write(fd, buffer, strlen(buffer));
    if (amount < 0) {
        status = errno_to_hal_error();
        goto done;
    }

done:
    close(fd);
    return status;
}

static hal_error_t write_numbered_file(unsigned number, const char* file, const char* buffer) {
    char path[PATH_MAX] = {0};
    sprintf(path, SYSFS_FILE_FORMAT, number, file);
    return write_file(path, buffer);
}


hal_error_t gpio_export_pin(unsigned number) {
    char str[3];
    sprintf(str, "%d", number);
    HAL_RETURN_IF_ERROR(write_file(SYSFS_EXPORT, str));
    return HAL_SUCCESS;
}

hal_error_t gpio_unexport_pin(unsigned number) {
    char str[3];
    sprintf(str, "%d", number);
    HAL_RETURN_IF_ERROR(write_file(SYSFS_UNEXPORT, str));
    return HAL_SUCCESS;
}

hal_error_t gpio_set_direction(unsigned number, direction_t direction) {
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

    HAL_RETURN_IF_ERROR(write_numbered_file(number, FILE_DIRECTION, to_write));
    return HAL_SUCCESS;
}

hal_error_t gpio_set_edge(unsigned number, edge_t edge) {
    const char* to_write;
    switch (edge) {
        case EDGE_NONE:
            to_write = STR_EDGE_NONE;
            break;
        case EDGE_RISING:
            to_write = STR_EDGE_RISING;
            break;
        case EDGE_FALLING:
            to_write = STR_EDGE_FALLING;
            break;
        case EDGE_BOTH:
            to_write = STR_EDGE_BOTH;
            break;
        default:
            return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(write_numbered_file(number, FILE_EDGE, to_write));
    return HAL_SUCCESS;
}

hal_error_t gpio_set_value(unsigned number, hal_dio_value_t value) {
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

    HAL_RETURN_IF_ERROR(write_numbered_file(number, FILE_VALUE, to_write));
    return HAL_SUCCESS;
}

hal_error_t gpio_get_value(unsigned number, hal_dio_value_t* value) {
    char buffer[16] = {0};
    HAL_RETURN_IF_ERROR(read_numbered_file(number, FILE_VALUE, buffer, sizeof(buffer)));

    if (0 == strcmp(buffer, STR_VALUE_HIGH)) {
        *value = HAL_DIO_HIGH;
    } else if (0 == strcmp(buffer, STR_VALUE_LOW)) {
        *value = HAL_DIO_LOW;
    } else {
        return HAL_ERROR_BAD_DATA;
    }

    return HAL_SUCCESS;
}
