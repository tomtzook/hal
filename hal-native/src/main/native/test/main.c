
#include <hal.h>
#include <hal_error.h>
#include <stdio.h>

int main() {
    hal_env_t* env = NULL;
    if (HAL_IS_ERROR(hal_init(&env))) {
        return 1;
    }

    hal_handle_t handle;
    if (HAL_IS_ERROR(hal_open(env, "NAME", HAL_TYPE_DIGITAL_OUTPUT, &handle))) {
        goto end;
    }

    hal_config_flags_t flags;
    hal_port_property_probe_handle(env, handle, HAL_CONFIG_GPIO_RESISTOR, &flags);

    hal_dio_set(env, handle, HAL_DIO_HIGH);

    /*hal_handle_t iter_handle = HAL_EMPTY_HANDLE;
    hal_error_t iter_status = HAL_SUCCESS;
    do {
        iter_status = hal_iter_open_port_next(env, &iter_handle);
        if (!HAL_IS_ERROR(iter_status)) {
            hal_open_port_info_t info;
            hal_error_t _status = hal_open_port_get_info(env, iter_handle, &info);
            if (!HAL_IS_ERROR(_status)) {
                printf("OPEN PORT: handle=%u, name=%s, type=%d\n", info.handle, info.name, info.type);
            }
        }
    } while (iter_handle != HAL_EMPTY_HANDLE);*/

    hal_close(env, handle);
end:
    hal_shutdown(env);
    return 0;
}
