#ifndef BBBIO_UTIL_H
#define BBBIO_UTIL_H

#define CLOSE_FD(fd_var) \
    if (fd_var >= 0) { \
        close(fd_var); \
        fd_var = -1; \
    }

#define MUNMAP(ptr, len) \
    if (NULL != ptr) { \
        munmap((void*)ptr, len); \
        ptr = NULL; \
    }

#endif //BBBIO_UTIL_H
