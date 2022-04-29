#include <stdio.h>
#include <hal.h>
#include <unistd.h>


#define HANDLE_ERROR(status) \
    do {                     \
        if (HAL_IS_ERROR(status)) { \
            fprintf(stderr, "Error (%s: %d): %d\n", __FILE__, __LINE__, status);\
            goto done;\
        }\
    } while(0)


// usr3
#define PORT (1 * 32 + 24)

int main() {
    printf("Hello\n");

    hal_env_t* env = NULL;
    hal_error_t status = hal_init(&env);
    HANDLE_ERROR(status);

    hal_handle_t handle;
    status = hal_dio_open(env, PORT, PORT_DIR_OUTPUT, &handle);
    HANDLE_ERROR(status);

    printf("port opened\n");

    status = hal_dio_set(env, handle, HAL_DIO_LOW);
    HANDLE_ERROR(status);
    for (int i = 0; i < 10; ++i) {
        status = hal_dio_set(env, handle, HAL_DIO_HIGH);
        HANDLE_ERROR(status);
        usleep(500000);
        status = hal_dio_set(env, handle, HAL_DIO_LOW);
        HANDLE_ERROR(status);
        usleep(500000);
    }

done:
    if (env != NULL) {
        hal_quit(&env);
    }
    printf("Done\n");

    return 0;
}
