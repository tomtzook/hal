
#include <unistd.h>

#include <hal.h>
#include <bbb_gpiodef.h>
#include <halpp.h>


int main() {
    auto hal_env = hal::init();

    auto usr0 = hal_env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR0);
    usr0.set(false);
    auto usr1 = hal_env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR1);
    usr1.set(false);
    auto usr2 = hal_env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR2);
    usr2.set(false);
    auto usr3 = hal_env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR3);
    usr3.set(false);

    for (int i = 0; i < 20; ++i) {
        usr0.set(true);
        usleep(1000 * 500);
        usr1.set(true);
        usleep(1000 * 500);
        usr2.set(true);
        usleep(1000 * 500);
        usr3.set(true);
        usleep(1000 * 500);

        usr0.set(false);
        usleep(1000 * 500);
        usr1.set(false);
        usleep(1000 * 500);
        usr2.set(false);
        usleep(1000 * 500);
        usr3.set(false);
        usleep(1000 * 500);
    }

    return 0;
}
