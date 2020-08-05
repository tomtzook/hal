#ifndef BBBIO_PWMSS_CTRL_H
#define BBBIO_PWMSS_CTRL_H

#include <bbbio.h>
#include <bbbio_pwmss.h>

#include "bbbio_rc.h"

#ifdef __cplusplus
extern "C"{
#endif

bbbio_rc_t bbbio_module_clk_enable(bbbio_t* bbbio, bbbio_pwmss_module_t module);

#ifdef __cplusplus
}
#endif

#endif //BBBIO_PWMSS_CTRL_H
