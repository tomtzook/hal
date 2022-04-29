#include <stdio.h>
#include <hal.h>


int main() {
    hal_env_t* env = NULL;
    hal_error_t status = hal_init(&env);
    if (HAL_IS_ERROR(status)) {
        return 1;
    }

    if (HAL_IS_ERROR(hal_ports_probe(env, 1, HAL_PORT_PROBE_ANALOG | HAL_PORT_PROBE_OUTPUT))) {
        goto done;
    }

    hal_handle_t handle;
    if (HAL_IS_ERROR(hal_aio_open(env, 1, PORT_DIR_OUTPUT, &handle))) {
        goto done;
    }
    printf("port opened");

    hal_aio_set(env, handle, 0);

done:
    hal_quit(&env);

    return 0;
}
