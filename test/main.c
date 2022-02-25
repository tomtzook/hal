#include <stdio.h>
#include <hal.h>


int main() {
    hal_env_t* env = NULL;
    hal_error_t status = hal_init(&env);
    if (HAL_IS_ERROR(status)) {
        return 1;
    }

    hal_handle_t handle;
    if (HAL_IS_ERROR(hal_dio_open(env, 1, PORT_DIR_OUTPUT, &handle))) {
        goto done;
    }

    hal_dio_set(env, handle, HAL_DIO_HIGH);

done:
    hal_quit(&env);

    return 0;
}
