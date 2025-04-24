
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <hal_error_handling.h>

#include "common.h"


hal_error_t os_error_to_hal_error(int error) {
    // todo: implement
    return HAL_ERROR_ENVIRONMENT;
}

hal_error_t read_file(const char* path, char* buffer, const size_t size) {
    const int fd = open(path, O_RDONLY);
    if (fd < 0) {
        TRACE_ERROR("failed to open %s", path);
        TRACE_SYSTEM_ERROR();
        return os_error_to_hal_error(errno);
    }

    hal_error_t status = HAL_SUCCESS;
    const ssize_t amount = read(fd, buffer, size);
    if (amount < 0) {
        TRACE_ERROR("failed to read from %s", path);
        TRACE_SYSTEM_ERROR();
        status = os_error_to_hal_error(errno);
        goto done;
    }

done:
    close(fd);
    return status;
}

hal_error_t write_file(const char* path, const char* buffer) {
    const int fd = open(path, O_WRONLY);
    if (fd < 0) {
        TRACE_ERROR("failed to open %s", path);
        TRACE_SYSTEM_ERROR();
        return os_error_to_hal_error(errno);
    }

    hal_error_t status = HAL_SUCCESS;
    const ssize_t amount = write(fd, buffer, strlen(buffer));
    if (amount < 0) {
        TRACE_ERROR("failed to write to %s", path);
        TRACE_SYSTEM_ERROR();
        status = os_error_to_hal_error(errno);
        goto done;
    }

done:
    close(fd);
    return status;
}
