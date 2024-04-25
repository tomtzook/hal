
#include <hal.h>
#include <hal_error.h>

int main() {
    hal_env_t* env;
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

    hal_close(env, handle);
end:
    hal_shutdown(env);
    return 0;
}
