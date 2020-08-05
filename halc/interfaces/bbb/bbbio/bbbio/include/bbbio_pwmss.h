#ifndef BBBIO_PWMSS_H
#define BBBIO_PWMSS_H

#include <bbbio.h>

#include "bbbio_rc.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef enum {
    BBBIO_PWMSS_0 = 0,
    BBBIO_PWMSS_1 = 1,
    BBBIO_PWMSS_2 = 2,
} bbbio_pwmss_module_t;

typedef enum {
    BBBIO_PWMSS_PIN_A = 0,
    BBBIO_PWMSS_PIN_B = 1,
} bbbio_pwmss_pin_t;

typedef float bbbio_pwmss_frequency_t;

typedef float bbbio_pwmss_duty_t;

typedef struct {
    bbbio_pwmss_frequency_t frequency;
    bbbio_pwmss_duty_t duty_a;
    bbbio_pwmss_duty_t duty_b;
} bbbio_pwmss_setting_t;

bbbio_rc_t bbbio_pwmss_enable(bbbio_t* bbbio, bbbio_pwmss_module_t module);
bbbio_rc_t bbbio_pwmss_disable(bbbio_t* bbbio, bbbio_pwmss_module_t module);

bbbio_rc_t bbbio_pwmss_set_frequency(bbbio_t* bbbio, bbbio_pwmss_module_t module, bbbio_pwmss_setting_t setting);

#ifdef __cplusplus
}
#endif

#endif //BBBIO_PWMSS_H
