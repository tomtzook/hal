#include "bbbio_consts.h"
#include "bbbio_gpio_consts.h"

const __off_t bbbio_gpio_addr_offset[BBBIO_GPIO_MODULES_COUNT] = {BBBIO_GPIO0_ADDR, BBBIO_GPIO1_ADDR, BBBIO_GPIO2_ADDR, BBBIO_GPIO3_ADDR};

const signed char bbbio_module_mapping[BBBIO_GPIO_HEADERS_COUNT][BBBIO_GPIO_HEADER_PIN_COUNT] = {
        {
                -1, -1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0, 1, 1,
                0, 2, 0, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0, 0,
                0, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
        },
        {
                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 0, 1, 1,
                1, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 3, 3, 3, 3, 3, -1, -1,
                -1, -1, -1, -1, -1, -1, -1, 0, 0, -1, -1, -1, -1
        }
};
const unsigned char bbbio_pin_mapping[BBBIO_GPIO_HEADERS_COUNT][BBBIO_GPIO_HEADER_PIN_COUNT] = {
        {
                0, 0, 6, 7, 2,	3, 2, 3,
                5, 4, 13, 12, 23, 26, 15,
                14, 27, 1, 22, 31, 30, 5,
                4, 1, 0, 29, 22, 24, 23,
                25, 10, 11, 9, 17, 8, 16,
                14, 15, 12, 13, 10, 11, 8,
                9, 6, 7
        },
        {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 30, 28, 31, 18, 16, 19,
                5, 4, 13, 12, 3, 2, 17,
                15, 21, 14, 19, 17, 15, 16,
                14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20,
                7, 0, 0, 0, 0
        }
};
