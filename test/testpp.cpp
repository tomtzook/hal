#include <iostream>
#include <halpp.hpp>


int main() {
    hal::hal hal;
    auto port = hal.ports().new_digital_output(1);
    port.set(HAL_DIO_HIGH);

    return 0;
}
