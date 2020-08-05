#include <stdlib.h>
#include <unistd.h>

#include "hal_types.h"
#include "hal_dio.h"
#include "dio_interface.h"
#include "logging/log.h"

#include "bbbio.h"
#include "bbbio_gpio.h"

#include "interface_def.h"

hal_result_t dio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    bbbio_rc_t result = bbbio_gpio_setdir(env->bbbio, HEADER(port_id), PIN(port_id),
            port_dir == OUTPUT ? BBB_DIR_OUTPUT : BBB_DIR_INPUT);
    return BBBIO2HAL_RESULT(result);
}
void dio_port_free(interface_env_t* env, port_id_t port_id) {

}

hal_result_t dio_port_write(interface_env_t* env, port_id_t port_id, dio_value_t value) {
    bbbio_rc_t result = bbbio_gpio_set(env->bbbio, HEADER(port_id), PIN(port_id),
            value == HIGH ? BBB_GPIO_HIGH : BBB_GPIO_LOW);
    if (SUCCESS != result) {
        LOGLN("Error dio write %d", result);
        return BBBIO2HAL_RESULT(result);
    }

    return HAL_SUCCESS;
}
hal_result_t dio_port_read(interface_env_t* env, port_id_t port_id, dio_value_t* value) {
    bbbio_gpio_value_t bbbio_value;
    bbbio_rc_t result = bbbio_gpio_get(env->bbbio, HEADER(port_id), PIN(port_id), &bbbio_value);
    if (SUCCESS != result) {
        LOGLN("Error dio write %d", result);
        return BBBIO2HAL_RESULT(result);
    }

    *value = bbbio_value == BBB_GPIO_HIGH ? HIGH : LOW;
    return HAL_SUCCESS;
}
hal_result_t dio_port_pulse(interface_env_t* env, port_id_t port_id, uint64_t length_us) {
    hal_result_t result = dio_port_write(env, port_id, HIGH);
    if (HAL_SUCCESS != result) {
        return result;
    }

    usleep(length_us); // bad, should be concurrent

    result = dio_port_write(env, port_id, LOW);
    if (HAL_SUCCESS != result) {
        return result;
    }

    return HAL_SUCCESS;
}
