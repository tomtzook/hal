#include <bbbio_debug.h>

#include "bbbio_pwmss_consts.h"
#include "bbbio_pwmss_ctrl.h"

#include "../hw.h"


static int module_clk_check(bbbio_t* bbbio, bbbio_pwmss_module_t module){
    return (HWREG(HWADD(bbbio->cm_addr, BBBIO_PWMSS_CTRL)) & (1 << module));
}

bbbio_rc_t bbbio_module_clk_enable(bbbio_t* bbbio, bbbio_pwmss_module_t module) {
    if(module_clk_check(bbbio, module)){
        HWREG(HWADD(bbbio->cm_per_addr, bbbio_module_clk_set[module])) = 0x2;
        return SUCCESS;
    }

    HWREG(HWADD(bbbio->cm_per_addr, bbbio_module_clk_set[module])) = (0x3 << 16);
    BBBIO_DEBUGF("BBBIO failed to enable PWMSS clock: %d", module);

    return EHW;
}
