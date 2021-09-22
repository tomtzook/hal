
#include <hal.h>
#include "hal_stub.h"


hal_error_t hal_define_natives(hal_native_t* native) {
    native->ports_init = ports_init;

    return HAL_SUCCESS;
}
