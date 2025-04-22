
#include <stdio.h>

#include <hal.h>
#include <hal_error.h>
#include <hal_sim.h>
#include <string.h>

#include "platforms.h"

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

static void fprint_mask(char* buffer, uint32_t mask, const char*(*func)(uint32_t)) {
    char* buffer_ptr = buffer;
    for (int i = 0; i < sizeof(mask) * 8; ++i) {
        const uint32_t bit = mask & (1 << i);
        if (bit) {
            if (buffer_ptr > buffer) {
                buffer_ptr[0] = '|';
                buffer_ptr++;
            }

            const char* str = func(bit);
            buffer_ptr = strcpy(buffer_ptr, str);
            buffer_ptr += strlen(str);
        }
    }

    // null terminator
    buffer_ptr[0] = '\0';
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
            char types_str[256];
            fprint_mask(types_str, info.supported_types, hal_port_type_str);
            char props_str[256];
            fprint_mask(props_str, info.supported_props, hal_prop_key_str);
            char flags_str[256];
            fprint_mask(flags_str, info.flags, hal_port_flag_str);

            const char* name = info.name != NULL ? info.name : "";
            printf("\tPORT: id=0x%x name=%s, types=%s (0x%x), props=%s (0x%lx), flags=%s (0x%x), handle=0x%x\n",
                   info.identifier, name,
                   types_str, info.supported_types,
                   props_str, info.supported_props,
                   flags_str, info.flags,
                   info.open_handle);
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

int main() {
    hal_env_t* env = NULL;
    if (HAL_IS_ERROR(hal_init(&env))) {
        return 1;
    }

    const hal_id_t USR_0 = GPIO_ID(USR, 0);
    const hal_id_t P8_1 = GPIO_ID(P8, 1);
    const hal_id_t EQEP_1 = GPIO_ID(P8, 3);

    if (HAL_IS_ERROR(configure_bbb_ports(env))) {
        return 1;
    }

    /*halsim_port_handle_t sim_handle;
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
    halsim_config_add_conflicting_port(env, sim_handle, P8_1);*/

    iterports(env);

    /*hal_handle_t handle;
    hal_open(env, EQEP_1, HAL_TYPE_QUADRATURE, &handle);
    halsim_get_handle(env, EQEP_1, &sim_handle);*/

    hal_handle_t handle;
    hal_open(env, P8_1, HAL_TYPE_DIGITAL_OUTPUT, &handle);
    hal_port_set_property(env, handle, HAL_CONFIG_DIO_POLL_EDGE, HAL_CONFIG_DIO_EDGE_BOTH);

    /*halsim_quadrature_set_position(env, sim_handle, 50);
    uint32_t value;
    hal_quadrature_get_position(env, handle, &value);
    printf("POS: 0x%x\n", value);*/

end:
    hal_shutdown(env);
    return 0;
}
