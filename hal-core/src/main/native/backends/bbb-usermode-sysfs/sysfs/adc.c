
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>

#include <hal_error_handling.h>
#include <stdlib.h>
#include "common.h"
#include "adc.h"


const char* SYSFS_FILE_FORMAT = "/sys/bus/iio/devices/iio:device0/in_voltage%d_raw";


hal_error_t adc_read(unsigned number, uint32_t* value) {
    char buffer[10] = {0};

    char path[PATH_MAX] = {0};
    sprintf(path, SYSFS_FILE_FORMAT, number);
    HAL_RETURN_IF_ERROR(read_file(path, buffer, sizeof(buffer)));

    *value = (uint32_t) atoi(buffer);
    return HAL_SUCCESS;
}

