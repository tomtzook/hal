#pragma once

#include <hal_types.h>
#include <hal_error.h>


typedef struct {
    volatile void* base;
    size_t size;
} peripheral_t;

hal_error_t map_peripheral(uintptr_t address, size_t size, peripheral_t* peripheral);
void free_peripheral(peripheral_t* peripheral);
