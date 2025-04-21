#pragma once

#include <hal_error.h>

#include "pins.h"


typedef struct {
    const pwm_pin_t* pin;
    uint32_t period_ns;
    uint32_t duty_ns;
} pwm_t;


hal_error_t pwm_export(const pwm_t* pwm);
hal_error_t pwm_unexport(const pwm_t* pwm);

hal_error_t pwm_enable(const pwm_t* pwm);
hal_error_t pwm_disable(const pwm_t* pwm);

hal_error_t pwm_set_duty_cycle(pwm_t* pwm, uint32_t duty);
hal_error_t pwm_set_frequency(pwm_t* pwm, uint32_t frequency);
