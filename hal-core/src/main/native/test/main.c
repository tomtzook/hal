
#include <stdio.h>

#include <hal.h>
#include <hal_error.h>
#include <hal_sim.h>

#define P8 (0x1 << 6)
#define P9 (0x1 << 7)
#define USR (0x3 << 6)
#define GPIO_ID(header, number) (header | number)

static hal_error_t open_callback(hal_env_t* env, halsim_port_handle_t handle, hal_port_type_t type) {
    printf("port open, handle 0x%x\n", handle);
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
    hal_error_t status;

    hal_port_iter_t* iter;
    status = hal_iter_port_start(env, &iter);
    if (HAL_IS_ERROR(status)) {
        return;
    }

    printf("Listing all ports:\n");
    do {
        hal_port_info_t info;
        status = hal_get_info(env, iter->identifier, &info);
        if (HAL_IS_SUCCESS(status)) {
            printf("\tPORT: id=0x%x, types=0x%x, props=0x%lx, flags=0x%x, handle=0x%x\n",
                   info.identifier, info.supported_types, info.supported_props, info.flags, info.open_handle);
        } else {
            printf("\tPORT: id=0x%x (failed to get more info)\n", iter->identifier);
        }

        status = hal_iter_port_next(env, iter);
        if (HAL_IS_ERROR(status)) {
            break;
        }
    } while (iter->identifier != HAL_INVALID_IDENTIFIER);

    hal_iter_port_end(env, iter);
}

void a(int b[]) {

}

int main() {
    hal_env_t* env = NULL;
    if (HAL_IS_ERROR(hal_init(&env))) {
        return 1;
    }

    const hal_id_t USR_0 = GPIO_ID(USR, 0);
    const hal_id_t P8_1 = GPIO_ID(P8, 1);
    const hal_id_t EQEP_1 = GPIO_ID(P8, 3);

    halsim_port_handle_t sim_handle;
    halsim_create_port(env, USR_0, &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_DIGITAL_OUTPUT | HAL_TYPE_DIGITAL_INPUT);
    halsim_config_port_prop(env, sim_handle, HAL_CONFIG_DIO_POLL_EDGE, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE);
    halsim_config_port_prop(env, sim_handle, HAL_CONFIG_DIO_RESISTOR, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE);

    halsim_create_port(env, P8_1, &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_DIGITAL_OUTPUT | HAL_TYPE_DIGITAL_INPUT);
    halsim_config_port_prop(env, sim_handle, HAL_CONFIG_DIO_POLL_EDGE, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE);
    halsim_config_port_prop(env, sim_handle, HAL_CONFIG_DIO_RESISTOR, HAL_CONFIG_FLAG_WRITABLE | HAL_CONFIG_FLAG_READABLE);

    halsim_create_port(env, EQEP_1, &sim_handle);
    halsim_config_port_types(env, sim_handle, HAL_TYPE_QUADRATURE);
    halsim_config_add_conflicting_port(env, sim_handle, USR_0);
    halsim_config_add_conflicting_port(env, sim_handle, P8_1);

    iterports(env);

    hal_handle_t handle;
    hal_open(env, EQEP_1, HAL_TYPE_QUADRATURE, &handle);
    halsim_get_handle(env, EQEP_1, &sim_handle);

    iterports(env);

    halsim_quadrature_set_position(env, sim_handle, 50);

    uint32_t value;
    hal_quadrature_get_position(env, handle, &value);
    printf("POS: 0x%x\n", value);

end:
    hal_shutdown(env);
    return 0;
}
