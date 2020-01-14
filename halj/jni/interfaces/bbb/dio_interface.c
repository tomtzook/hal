#include <stdlib.h>

#include "hal_types.h"
#include "hal_dio.h"
#include "dio_interface.h"
#include "logging/log.h"

#include "bbbio.h"
#include "bbbio_gpio.h"

#define HEADER(port) (port & 0xff)
#define PIN(port) ((port << 8) & 0xff)

typedef struct dio_env {
    bbbio_t* bbbio;
} dio_env_t;

int dio_init(dio_env_t** env) {
    *env = (dio_env_t*) malloc(sizeof(dio_env_t));
    if (NULL == *env) {
        return 1;
    }

    bbbio_rc_t bbbio_rc;
    bbbio_t* bbbio = (bbbio_t*) malloc(sizeof(bbbio_t));
    if (NULL == *env) {
        goto error;
    }

    bbbio_rc = bbbio_init(bbbio);
    if (SUCCESS != bbbio_rc) {
        goto error;
    }

    (*env)->bbbio = bbbio;

    return 0;
error:
    if (NULL != bbbio) {
        free(bbbio);
    }
    free(*env);

    return 1;
}
void dio_free(dio_env_t** env) {
    bbbio_free((*env)->bbbio);
    free((*env)->bbbio);

    free(*env);
    *env = NULL;
}

int dio_port_init(dio_env_t* env, port_id_t port_id, port_dir_t port_dir) {
    return bbbio_gpio_setdir(env->bbbio, HEADER(port_id), PIN(port_id),
            port_dir == OUTPUT ? BBB_DIR_OUTPUT : BBB_DIR_INPUT);
}
void dio_port_free(dio_env_t* env, port_id_t port_id) {

}

void dio_port_write(dio_env_t* env, port_id_t port_id, dio_value_t value) {
    bbbio_rc_t result = bbbio_gpio_set(env->bbbio, HEADER(port_id), PIN(port_id),
            value == HIGH ? BBB_GPIO_HIGH : BBB_GPIO_LOW);
    if (SUCCESS != result) {
        LOGLN("Error dio write %d", result);
    }
}
dio_value_t dio_port_read(dio_env_t* env, port_id_t port_id) {
    bbbio_gpio_value_t value = BBB_GPIO_LOW;
    bbbio_rc_t result = bbbio_gpio_get(env->bbbio, HEADER(port_id), PIN(port_id),
            &value);
    if (SUCCESS != result) {
        LOGLN("Error dio write %d", result);
        return LOW;
    }

    return value == BBB_GPIO_HIGH ? HIGH : LOW;
}