#pragma once

#include <hal_error.h>


typedef struct _peripheral {
    volatile void* base;
    size_t size;
} peripheral_t;

#define PREFREG(peripheral, offset, type) ((volatile type * ) ((peripheral)->base + (offset)))

hal_error_t map_peripheral(peripheral_t* peripheral, uint32_t base_addr, size_t size);
void unmap_peripheral(peripheral_t* peripheral);
