
#include <stdio.h>

#include <hal.h>
#include <hal_error.h>
#include <hal_sim.h>

static hal_error_t open_callback(hal_env_t* env, halsim_port_handle_t handle, hal_port_type_t type) {
    printf("port open, handle %u\n", handle);
    return HAL_SUCCESS;
}

static hal_error_t get_prop_callback(hal_env_t* env,
                                     halsim_port_handle_t port_handle,
                                     hal_prop_key_t key,
                                     uint32_t* value) {
    *value = 10;
    return HAL_SUCCESS;
}

int main() {
    hal_env_t* env = NULL;
    if (HAL_IS_ERROR(hal_init(&env))) {
        return 1;
    }

    halsim_port_handle_t sim_handle;
    halsim_create_port(env, "PORT1", &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_DIGITAL_OUTPUT);
    halsim_config_port_prop(env, sim_handle, HAL_CONFIG_DIO_RESISTOR, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE);
    halsim_config_port_callbacks(env, sim_handle, open_callback, NULL);
    halsim_port_set_prop(env, sim_handle, HAL_CONFIG_DIO_RESISTOR, HAL_CONFIG_DIO_RESISTOR_PULLUP);
    halsim_config_port_prop_callbacks(env, sim_handle, HAL_CONFIG_DIO_RESISTOR, get_prop_callback, NULL);

    halsim_create_port(env, "PORT2", &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_DIGITAL_OUTPUT);
    halsim_config_port_prop(env, sim_handle, HAL_CONFIG_DIO_RESISTOR, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE);

    hal_port_iter_t* iter;
    hal_error_t iter_status;
    hal_iter_port_start(env, &iter);
    do {
        printf("PORT: name=%s, types=%d\n", iter->name, iter->supported_types);
        iter_status = hal_iter_port_next(env, iter);
    } while (HAL_IS_SUCCESS(iter_status));

    hal_handle_t handle;
    hal_open(env, "PORT1", HAL_TYPE_DIGITAL_OUTPUT, &handle);
    uint32_t value;
    hal_port_get_property(env, handle, HAL_CONFIG_DIO_RESISTOR, &value);
    printf("%u\n", value);

    /*hal_handle_t handle;
    if (HAL_IS_ERROR(hal_open(env, "NAME", HAL_TYPE_DIGITAL_OUTPUT, &handle))) {
        goto end;
    }

    hal_config_flags_t flags;
    hal_port_property_probe(env, handle, HAL_CONFIG_DIO_RESISTOR, &flags);

    hal_dio_set(env, handle, HAL_DIO_HIGH);*/

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

    //hal_close(env, handle);
end:
    hal_shutdown(env);
    return 0;
}
