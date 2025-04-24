
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#include "peripheral.h"

#include "hal_error_handling.h"


static hal_error_t map_memory(const uint32_t base_addr, const size_t size, void** mem_out) {
    const int fd = open("/dev/mem", O_RDWR);
    if (fd < 0) {
        TRACE_ERROR("Failed to open /dev/mem");
        TRACE_SYSTEM_ERROR();
        return HAL_ERROR_MEMORY_ACCESS;
    }

    void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (__off_t) base_addr);
    if (mem == MAP_FAILED) {
        TRACE_SYSTEM_ERROR("failed to map address 0x%x for size 0x%x", base_addr, size);
        TRACE_SYSTEM_ERROR();
        return HAL_ERROR_MAPPING_FAILED;
    }

    close(fd);

    *mem_out = mem;
    return HAL_SUCCESS;
}

hal_error_t map_peripheral(peripheral_t* peripheral, const uint32_t base_addr, const size_t size) {
    if (peripheral->base != NULL) {
        return HAL_ERROR_ALREADY_MAPPED;
    }

    void* ptr;
    HAL_RETURN_IF_ERROR(map_memory(base_addr, size, &ptr));

    peripheral->base = ptr;
    peripheral->size = size;

    return HAL_SUCCESS;
}

void unmap_peripheral(peripheral_t* peripheral) {
    if (peripheral->base == NULL) {
        return;
    }

    if (munmap((void*) peripheral->base, peripheral->size)) {
        TRACE_SYSTEM_ERROR("failed to unmap address 0x%x", peripheral->base);
        TRACE_SYSTEM_ERROR();
    }

    peripheral->base = NULL;
    peripheral->size = 0;
}
