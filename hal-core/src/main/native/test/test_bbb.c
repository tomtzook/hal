
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <hal.h>
#include <bbb_port_ids.h>

volatile sig_atomic_t continue_running = 1;
void stop_run(int sig) {
    continue_running = 0;
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
            printf("\tPORT: id=0x%x name=%s, types=%s (0x%x), props=%s (0x%llx), flags=%s (0x%x), handle=0x%x\n",
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
    signal(SIGINT, stop_run);

    hal_env_t* env = NULL;
    if (HAL_IS_ERROR(hal_init(&env))) {
        return 1;
    }

    iterports(env);

    hal_handle_t handle;
    hal_error_t status;

    status = hal_open(env, P8_13, HAL_TYPE_DIGITAL_OUTPUT, &handle);
    if (HAL_IS_ERROR(status)) {
        printf("failed open: 0x%x\n", status);
        goto end;
    }
    hal_dio_set(env, handle, HAL_DIO_HIGH);

    /*status = hal_open(env, EHRPWM2B, HAL_TYPE_PWM_OUTPUT, &handle);
    if (HAL_IS_ERROR(status)) {
        printf("failed open: 0x%x\n", status);
        goto end;
    }

    hal_port_set_property(env, handle, HAL_CONFIG_PWM_FREQUENCY, 10000);
    hal_pwm_set_duty_cycle(env, handle, 5000);*/

    for (int i = 0; i < 10000; i++) {
        if (!continue_running) {
            break;
        }

        usleep(100000000);
    }

    hal_close(env, handle);

end:
    hal_shutdown(env);
    return 0;
}
