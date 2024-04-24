
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "common.h"
#include "pwm.h"

//sys/devices/platform/ocp/{chip}.epwmss/{addr}.pwm/pwm/pwmchip{chip_index}/pwm-{chip_index}:{index}/
//                                                                                                 duty_cycle
//                                                                                                 enable
//                                                                                                 period
//                                                                                                 polarity

static const char* OCP_PWM_PATH = "/sys/devices/platform/ocp/%s.epwmss/%s.pwm/pwm/pwmchip%d/pwm-%u:%u/%s";
static const char* OCP_PWMCHIP_PATH = "/sys/devices/platform/ocp/%s.epwmss/%s.pwm/pwm/pwmchip%u/%s";

static const char* FILE_DUTY_CYCLE = "duty_cycle";
static const char* FILE_FREQUENCY = "period";
static const char* FILE_ENABLE = "enable";


int is_pwm_exported(pwm_pin_t* pin) {
    char path[PATH_MAX] = {0};
    sprintf(path, OCP_PWM_PATH, pin->chip, pin->addr, pin->chip_index, pin->chip_index, pin->index, FILE_DUTY_CYCLE);
    return 0 == access(path, F_OK);
}

hal_error_t write_pwm_file(pwm_pin_t* pin, const char* file, const char* buffer) {
    char path[PATH_MAX];
    sprintf(path, OCP_PWM_PATH, pin->chip, pin->addr, pin->chip_index, pin->chip_index, pin->index, file);

    return write_file(path, buffer);
}

hal_error_t write_pwm_file_i(pwm_pin_t* pin, const char* file, unsigned value) {
    char buffer[32] = {0};
    sprintf(buffer, "%d", value);
    return write_pwm_file(pin, file, buffer);
}

hal_error_t write_pwmchip_file(pwm_pin_t* pin, const char* file, const char* buffer) {
    char path[PATH_MAX];
    sprintf(path, OCP_PWMCHIP_PATH, pin->chip, pin->addr, pin->chip_index, file);

    return write_file(path, buffer);
}


hal_error_t pwm_export(pwm_t* pwm) {
    if (is_pwm_exported(pwm->pin)) {
        return HAL_SUCCESS;
    }

    char buffer[10] = {0};
    sprintf(buffer, "%d", pwm->pin->index);
    return write_pwmchip_file(pwm->pin, "export", buffer);
}

hal_error_t pwm_unexport(pwm_t* pwm) {
    if (!is_pwm_exported(pwm->pin)) {
        return HAL_SUCCESS;
    }

    char buffer[10] = {0};
    sprintf(buffer, "%d", pwm->pin->index);
    return write_pwmchip_file(pwm->pin, "unexport", buffer);
}

hal_error_t pwm_enable(pwm_t* pwm) {
    return write_pwm_file_i(pwm->pin, FILE_ENABLE, 1);
}

hal_error_t pwm_disable(pwm_t* pwm) {
    return write_pwm_file_i(pwm->pin, FILE_ENABLE, 0);
}

hal_error_t pwm_set_duty_cycle(pwm_t* pwm, uint32_t duty) {
    uint32_t duty_ns = (uint32_t)(duty * 10e3);
    if (duty_ns > pwm->period_ns) {
        return HAL_ERROR_BAD_ARGUMENT;
    }

    HAL_RETURN_IF_ERROR(write_pwm_file_i(pwm->pin, FILE_DUTY_CYCLE, duty_ns));
    pwm->duty_ns = duty_ns;

    return HAL_SUCCESS;
}

hal_error_t pwm_set_frequency(pwm_t* pwm, uint32_t frequency) {
    pwm_disable(pwm);

    hal_error_t status;
    uint32_t duty = pwm->duty_ns * 10e-3;

    uint32_t period_ns = (uint32_t)(frequency * 10e3);
    status = write_pwm_file_i(pwm->pin, FILE_FREQUENCY, period_ns);
    HAL_JUMP_IF_ERROR(status, end);
    pwm->period_ns = period_ns;

    status = pwm_set_duty_cycle(pwm, duty);
    HAL_JUMP_IF_ERROR(status, end);

end:
    pwm_enable(pwm);
    return status;
}
