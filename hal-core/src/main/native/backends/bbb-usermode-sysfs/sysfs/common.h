#pragma once

#include <hal_error.h>


hal_error_t os_error_to_hal_error(int error);

hal_error_t read_file(const char* path, char* buffer, size_t size);
hal_error_t write_file(const char* path, const char* buffer);
