#include <interface.h>
#include "bbbio.h"

#define HEADER(port) (port & 0xff)
#define PIN(port) ((port >> 8) & 0xff)
//#define PORT(header, pin) (header & 0xff) | ((pin & 0xff) << 8)

struct interface_env {
    bbbio_t* bbbio;
};

static inline hal_result_t __bbbio_error_to_hal(bbbio_rc_t code) {
    switch (code) {
        case SUCCESS: return HAL_SUCCESS;
        case EINITIALIZED: return HAL_INITIALIZATION_ERROR;
        case EMAP: return HAL_SUCCESS;
        case EARG: return HAL_SUCCESS;
        case EHW: return HAL_SUCCESS;
    }
}

#define BBBIO2HAL_RESULT(bbbio) __bbbio_error_to_hal((bbbio))
