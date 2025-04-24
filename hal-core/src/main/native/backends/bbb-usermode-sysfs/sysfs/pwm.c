
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>

#include <hal_error_handling.h>
#include "common.h"
#include "pwm.h"

#include <stdlib.h>

//sys/devices/platform/ocp/{chip}.epwmss/{addr}.pwm/pwm/pwmchip{chip_index}/pwm-{chip_index}:{index}/
//                                                                                                 duty_cycle
//                                                                                                 enable
//                                                                                                 period
//                                                                                                 polarity

#define USE_OCP 0

#if USE_OCP
static const char* OCP_PWM_PATH = "/sys/devices/platform/ocp/%s.epwmss/%s.pwm/pwm/pwmchip%d/pwm-%u:%u/%s";
static const char* OCP_PWMCHIP_PATH = "/sys/devices/platform/ocp/%s.epwmss/%s.pwm/pwm/pwmchip%u/%s";
#else
static const char* CLASS_PWMCHIP_PATH = "/sys/class/pwm/pwmchip%d/%s";
static const char* CLASS_PWM_PATH = "/sys/class/pwm/pwmchip%d/pwm%d/%s";
#endif

static const char* FILE_DUTY_CYCLE = "duty_cycle";
static const char* FILE_FREQUENCY = "period";
static const char* FILE_ENABLE = "enable";


static int is_pwm_exported(const pwm_pin_t* pin) {
    char path[PATH_MAX] = {0};

#if USE_OCP
    sprintf(path, OCP_PWM_PATH, pin->chip, pin->addr, pin->chip_index, pin->chip_index, pin->index, FILE_DUTY_CYCLE);
#else
    sprintf(path, CLASS_PWM_PATH, pin->chip_index, pin->index, FILE_DUTY_CYCLE);
#endif

    return 0 == access(path, F_OK);
}

static hal_error_t write_pwm_file(const pwm_pin_t* pin, const char* file, const char* buffer) {
    char path[PATH_MAX];

#if USE_OCP
    sprintf(path, OCP_PWM_PATH, pin->chip, pin->addr, pin->chip_index, pin->chip_index, pin->index, file);
#else
    sprintf(path, CLASS_PWM_PATH, pin->chip_index, pin->index, file);
#endif

    return write_file(path, buffer);
}

static hal_error_t read_pwm_file(const pwm_pin_t* pin, const char* file, char* buffer, const size_t size) {
    char path[PATH_MAX];

#if USE_OCP
    sprintf(path, OCP_PWM_PATH, pin->chip, pin->addr, pin->chip_index, pin->chip_index, pin->index, file);
#else
    sprintf(path, CLASS_PWM_PATH, pin->chip_index, pin->index, file);
#endif

    return read_file(path, buffer, size);
}

static hal_error_t read_pwm_file_i(const pwm_pin_t* pin, const char* file, unsigned* value) {
    char buffer[8] = {0};
    HAL_RETURN_IF_ERROR(read_pwm_file(pin, file, buffer, sizeof(buffer)));

    *value = atoi(buffer);
    return HAL_SUCCESS;
}

static hal_error_t write_pwm_file_i(const pwm_pin_t* pin, const char* file, const unsigned value) {
    char buffer[32] = {0};
    sprintf(buffer, "%d", value);
    return write_pwm_file(pin, file, buffer);
}

static hal_error_t write_pwmchip_file(const pwm_pin_t* pin, const char* file, const char* buffer) {
    char path[PATH_MAX];

#if USE_OCP
    sprintf(path, OCP_PWMCHIP_PATH, pin->chip, pin->addr, pin->chip_index, file);
#else
    sprintf(path, CLASS_PWMCHIP_PATH, pin->chip_index, file);
#endif

    return write_file(path, buffer);
}


hal_error_t pwm_export(const pwm_t* pwm) {
    if (is_pwm_exported(pwm->pin)) {
        return HAL_SUCCESS;
    }

    char buffer[10] = {0};
    sprintf(buffer, "%d", pwm->pin->index);
    return write_pwmchip_file(pwm->pin, "export", buffer);
}

hal_error_t pwm_unexport(const pwm_t* pwm) {
    if (!is_pwm_exported(pwm->pin)) {
        return HAL_SUCCESS;
    }

    char buffer[10] = {0};
    sprintf(buffer, "%d", pwm->pin->index);
    return write_pwmchip_file(pwm->pin, "unexport", buffer);
}

hal_error_t pwm_reload_is_enabled(pwm_t* pwm) {
    unsigned value;
    HAL_RETURN_IF_ERROR(read_pwm_file_i(pwm->pin, FILE_ENABLE, &value));

    pwm->enabled = (value != 0);
    return HAL_SUCCESS;
}

hal_error_t pwm_enable(pwm_t* pwm) {
    const hal_error_t status = write_pwm_file_i(pwm->pin, FILE_ENABLE, 1);
    if (HAL_IS_SUCCESS(status)) {
        pwm->enabled = 1;
        return HAL_SUCCESS;
    }

    return status;
}

hal_error_t pwm_disable(pwm_t* pwm) {
    const hal_error_t status = write_pwm_file_i(pwm->pin, FILE_ENABLE, 0);
    if (HAL_IS_SUCCESS(status)) {
        pwm->enabled = 0;
        return HAL_SUCCESS;
    }

    return status;
}

hal_error_t pwm_set_duty_cycle(pwm_t* pwm, const uint32_t duty_us) {
    const uint32_t duty_ns = duty_us * 1000;
    if (duty_ns > pwm->period_ns) {
        return HAL_ERROR_BAD_ARGUMENT;
    }
    if (duty_ns == pwm->duty_ns) {
        return HAL_SUCCESS;
    }

    HAL_RETURN_IF_ERROR(write_pwm_file_i(pwm->pin, FILE_DUTY_CYCLE, duty_ns));
    pwm->duty_ns = duty_ns;

    return HAL_SUCCESS;
}

hal_error_t pwm_set_period(pwm_t* pwm, const uint32_t period_us) {
    const uint32_t period_ns = period_us * 1000;
    if (period_ns == pwm->period_ns) {
        return HAL_SUCCESS;
    }

    hal_error_t status = write_pwm_file_i(pwm->pin, FILE_FREQUENCY, period_ns);
    HAL_JUMP_IF_ERROR(status, end);

    pwm->period_ns = period_ns;

    if (pwm->duty_ns <= period_ns) {
        status = pwm_set_duty_cycle(pwm, (pwm->duty_ns / 1000));
        HAL_JUMP_IF_ERROR(status, end);
    } else {
        pwm->duty_ns = 0;
    }

end:
    return status;
}
