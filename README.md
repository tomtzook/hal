# HAL

A basic abstraction layer for Hardware IO, such as GPIO, PWM and more. The actual implementation is provided in the form of _BACKENDS_, each providing different functionalities and supporting different hardware platform (e.g. Raspberry PI, Beaglebone Black, etc.).

The following are the available backends:
- `bbb-usermode-sysfs`: a usermode implementation using sysfs for Beaglebone Black.

Available for use in native code or with Java via JNI bindings.

## Usage

#### Basic Example LED

```c
#include <hal.h>

int main() {
    hal_env_t* env;
    hal_error_t status;
    
    status = hal_init(&env);
    if (HAL_IS_ERROR(status)) {
        return 1;
    }

    hal_handle_t handle;
    status = hal_open(env, "P8_12", HAL_TYPE_DIGITAL_OUTPUT, &handle);
    if (HAL_IS_ERROR(status)) {
        goto end;
    }
    
    for (int i = 0; i < 5; i++) {
        status = hal_dio_set(handle, HAL_DIO_HIGH);
        if (HAL_IS_ERROR(status)) {
            goto end;
        }
        
        usleep(10000);
        
        status = hal_dio_set(handle, HAL_DIO_LOW);
        if (HAL_IS_ERROR(status)) {
            goto end;
        }
        
        usleep(10000);
    }
end:
    hal_shutdown(env);
    return status;
}

```
