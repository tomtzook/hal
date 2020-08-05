#include <stdlib.h>
#include <unistd.h>

#include "hal_types.h"
#include "hal_dio.h"
#include "dio_interface.h"
#include "logging/log.h"

#include "bbbio.h"
#include "bbbio_gpio.h"

#include "interface_def.h"

#define HEADER(port) (port & 0xff)
#define PIN(port) ((port >> 8) & 0xff)
//#define PORT(header, pin) (header & 0xff) | ((pin & 0xff) << 8)


int dio_port_init(interface_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    return bbbio_gpio_setdir(env->bbbio, HEADER(port_id), PIN(port_id),
            port_dir == OUTPUT ? BBB_DIR_OUTPUT : BBB_DIR_INPUT);
}
void dio_port_free(interface_env_t* env, port_id_t port_id) {

}

void dio_port_write(interface_env_t* env, port_id_t port_id, dio_value_t value) {
    bbbio_rc_t result = bbbio_gpio_set(env->bbbio, HEADER(port_id), PIN(port_id),
            value == HIGH ? BBB_GPIO_HIGH : BBB_GPIO_LOW);
    if (SUCCESS != result) {
        LOGLN("Error dio write %d", result);
    }
}
dio_value_t dio_port_read(interface_env_t* env, port_id_t port_id) {
    bbbio_gpio_value_t value = BBB_GPIO_LOW;
    bbbio_rc_t result = bbbio_gpio_get(env->bbbio, HEADER(port_id), PIN(port_id),
            &value);
    if (SUCCESS != result) {
        LOGLN("Error dio write %d", result);
        return LOW;
    }

    return value == BBB_GPIO_HIGH ? HIGH : LOW;
}
void dio_port_pulse(interface_env_t* env, port_id_t port_id, uint64_t length_us) {
    dio_port_write(env, port_id, HIGH);
    usleep(length_us); // bad, should be concurrent
    dio_port_write(env, port_id, LOW);
}


