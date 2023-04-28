#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "peripheral.h"


hal_error_t map_peripheral(uintptr_t address, size_t size, peripheral_t* peripheral) {
    int fd = open("/dev/mem", O_RDWR);
    if (fd < 0) {
        return HAL_ERROR_PERMISSIONS;
    }

    void* mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, address);
    if (MAP_FAILED == mem) {
        close(fd);
        return HAL_ERROR_PERMISSIONS;
    }

    close(fd);

    peripheral->base = mem;
    peripheral->size = size;

    return HAL_SUCCESS;
}

void free_peripheral(peripheral_t* peripheral) {
    if (NULL != peripheral->base) {
        munmap((void*) peripheral->base, peripheral->size);
    }
}
