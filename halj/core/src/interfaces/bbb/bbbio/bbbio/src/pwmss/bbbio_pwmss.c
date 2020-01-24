#include <bbbio_pwmss.h>
#include <bbbio_debug.h>

#include "../hw.h"


const int clkdiv_vals[] = {1, 2, 4, 8, 16, 32, 64, 128};
const int hspclkdiv_vals[] = {1, 2, 4 ,6, 8, 10, 12, 14};

typedef struct {
    int tbprd;
    int clkdiv;
    int hspclkdiv;
} clk_tbprd_t;

static bbbio_rc_t module_calc_tbprd(clk_tbprd_t* tbprd, bbbio_pwmss_frequency_t frequency) {
    tbprd->tbprd = 0;
    tbprd->clkdiv = 7;
    tbprd->hspclkdiv = 7;

    float cycles = 1000000000.0f / frequency;
    float divisor = (cycles / 655350.0f);

    if(divisor > (128 * 14)) {
        BBBIO_DEBUGF("BBBIO given frequency is too high %f", frequency);
        return EARG;
    }

    int possible_divisor = 0;
    int last_best_divisor = 0;

    for(size_t i = 0 ; i < 8 ; i ++) {
        for(size_t j = 0 ; j < 8 ; j ++) {
            possible_divisor = clkdiv_vals[i] * hspclkdiv_vals[j];
            if (possible_divisor < last_best_divisor && possible_divisor > divisor) {
                last_best_divisor = possible_divisor;

                tbprd->clkdiv = i;
                tbprd->hspclkdiv = j;
            }
        }
    }

    tbprd->tbprd = (int) (cycles / (10.0f * (clkdiv_vals[tbprd->clkdiv] * hspclkdiv_vals[tbprd->hspclkdiv])));

    return SUCCESS;
}

bbbio_rc_t bbbio_pwmss_enable(bbbio_t* bbbio, bbbio_pwmss_module_t module) {
    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_AQCTLA)) = (0x2 | (0x3 << 4));
    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_AQCTLB)) = (0x2 | (0x3 << 8));

    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_TBCNT)) = 0;
    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_TBCTL)) &= ~0x3;

    return SUCCESS;
}

bbbio_rc_t bbbio_pwmss_disable(bbbio_t* bbbio, bbbio_pwmss_module_t module) {
    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_TBCTL)) |= 0x3;

    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_AQCTLA)) = (0x1 | (0x3 << 4));
    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_AQCTLB)) = (0x1 | (0x3 << 8));

    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_TBCNT)) = 0;

    return SUCCESS;
}

bbbio_rc_t bbbio_pwmss_set_frequency(bbbio_t* bbbio, bbbio_pwmss_module_t module, bbbio_pwmss_setting_t setting) {
    bbbio_rc_t return_code;

    clk_tbprd_t tbprd;
    return_code = module_calc_tbprd(&tbprd, setting.frequency);
    if (return_code != SUCCESS) {
        return return_code;
    }

    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_TBCTL)) = (unsigned short) (BBBIO_TBCTL_CTRMODE_FREEZE | (tbprd.clkdiv << 10) | (tbprd.hspclkdiv << 7));

    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_CMPA)) = (unsigned short) ((float) tbprd.tbprd * setting.duty_a);
    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_CMPB)) = (unsigned short) ((float) tbprd.tbprd * setting.duty_b);

    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_TBPRD)) = (unsigned short) tbprd.tbprd;

    HWREGH(HWADD(bbbio->epwm_module_addr[module], BBBIO_EPWM_TBCNT)) = 0;

    return SUCCESS;
}
