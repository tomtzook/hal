
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

static void iterports(hal_env_t* env) {
    hal_port_iter_t* iter;
    hal_error_t iter_status;
    hal_iter_port_start(env, &iter);
    do {
        hal_port_info_t info;
        hal_get_info(env, iter->identifier, &info);
        printf("PORT: id=%u, types=%u, props=%lu, flags=%u, handle=%u\n", info.identifier, info.supported_types, info.supported_props, info.flags, info.open_handle);
        iter_status = hal_iter_port_next(env, iter);
    } while (HAL_IS_SUCCESS(iter_status));
    hal_iter_port_end(env, iter);
}

int main() {
    hal_env_t* env = NULL;
    if (HAL_IS_ERROR(hal_init(&env))) {
        return 1;
    }

    halsim_port_handle_t sim_handle;
    halsim_create_port(env, 1, &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_DIGITAL_OUTPUT | HAL_TYPE_DIGITAL_INPUT);

    halsim_create_port(env, 2, &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_DIGITAL_OUTPUT | HAL_TYPE_DIGITAL_INPUT);

    halsim_create_port(env, 3, &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_QUADRATURE);
    halsim_config_add_conflicting_port(env, sim_handle, 1);
    halsim_config_add_conflicting_port(env, sim_handle, 2);

    iterports(env);

    hal_handle_t handle;
    hal_open(env, 3, HAL_TYPE_QUADRATURE, &handle);
    halsim_get_handle(env, 3, &sim_handle);

    iterports(env);

    halsim_quadrature_set_position(env, sim_handle, 50);

    uint32_t value;
    hal_quadrature_get_position(env, handle, &value);
    printf("POS: %u\n", value);

    /*hal_handle_t handle;
    if (HAL_IS_ERROR(hal_open(env, "NAME", HAL_TYPE_DIGITAL_OUTPUT, &handle))) {
        goto end;
    }

    hal_config_flag_t flags;
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
