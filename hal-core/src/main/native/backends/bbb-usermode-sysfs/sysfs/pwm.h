#pragma once

#include <hal_error.h>
#include <hal_error_handling.h>

#include "pins.h"


typedef struct {
    pwm_pin_t* pin;
    uint32_t period_ns;
    uint32_t duty_ns;
} pwm_t;


hal_error_t pwm_export(pwm_t* pwm);
hal_error_t pwm_unexport(pwm_t* pwm);

hal_error_t pwm_enable(pwm_t* pwm);
hal_error_t pwm_disable(pwm_t* pwm);

hal_error_t pwm_set_duty_cycle(pwm_t* pwm, uint32_t duty);
hal_error_t pwm_set_frequency(pwm_t* pwm, uint32_t frequency);
