#ifndef BBBIO_DEBUG_H
#define BBBIO_DEBUG_H

#include <stdio.h>

#ifdef BBBIO_DEBUG

#define BBBIO_DEBUG(msg) \
    printf(msg)

#define BBBIO_DEBUGF(fmt, args...) \
    printf(fmt, args)

#else

#define BBBIO_DEBUG(msg)
#define BBBIO_DEBUGF(fmt, args...)

#endif

#endif //BBBIO_DEBUG_H
