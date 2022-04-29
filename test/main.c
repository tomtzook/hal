#include <stdio.h>
#include <hal.h>
#include <unistd.h>

#include <bbb_gpiodef.h>


#define HANDLE_ERROR(...) \
    do {                  \
        hal_error_t __status = __VA_ARGS__;\
        if (HAL_IS_ERROR(__status)) { \
            fprintf(stderr, "[%s: %d] Error: (%d) %s\n", __FILE__, __LINE__, __status, hal_strerror(__status));\
            goto done;\
        }\
    } while(0)


int main() {
    printf("Hello\n");

    hal_env_t* env = NULL;
    hal_handle_t led1 = HAL_EMPTY_HANDLE;

    HANDLE_ERROR(hal_init(&env));
    HANDLE_ERROR(hal_dio_open(env, USR3, PORT_DIR_OUTPUT, &led1));

    printf("port opened\n");

    HANDLE_ERROR(hal_dio_set(env, led1, HAL_DIO_LOW));
    for (int i = 0; i < 10; ++i) {
        HANDLE_ERROR(hal_dio_set(env, led1, HAL_DIO_HIGH));
        usleep(500000);
        HANDLE_ERROR(hal_dio_set(env, led1, HAL_DIO_LOW));
        usleep(500000);
    }

done:
    if (led1 != HAL_EMPTY_HANDLE) {
        hal_dio_close(env, led1);
    }
    if (env != NULL) {
        hal_quit(&env);
    }
    printf("Done\n");

    return 0;
}
