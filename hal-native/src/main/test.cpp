
#include <unistd.h>
#include <cstdio>
#include <halpp.h>
#include <bbb_gpiodef.h>


int main() {
    auto env = hal::init();
    auto port0 = env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR0);
    auto port1 = env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR1);
    auto port2 = env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR2);
    auto port3 = env.open<HAL_TYPE_DIGITAL_OUTPUT>(USR3);
    for (int i = 0; i < 20; ++i) {
        printf("on");
        port0.set(true);
        port1.set(true);
        port2.set(true);
        port3.set(true);
        sleep(1);
        printf("off");
        port0.set(false);
        port1.set(false);
        port2.set(false);
        port3.set(false);
        sleep(1);
    }

    return 0;
}
