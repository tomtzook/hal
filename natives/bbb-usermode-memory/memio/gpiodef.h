#pragma once

#define MODULE(pin_number) ((pin_number) / 32)
#define PIN(pin_number) ((pin_number) % 32)
#define PIN_NUMBER(module, index) ((module) * 32 + (index))
#define PIN_MASK(pin) (1u << (pin))

// Defines for GPIO mem-access
#define GPIO0 0x44e07000
#define GPIO1 0x4804c000
#define GPIO2 0x481ac000
#define GPIO3 0x481ae000
#define GPIO_CTRL_SIZE 0x1000

// Defines for pins on GPIO0
#define P8_13 (23) // EHRPWM2B
#define P8_14 (26)
#define P8_17 (27)
#define P8_19 (22) // EHRPWM2A
#define P8_31 (10) // UART5_CTSN
#define P8_32 (11) // UART5_RTSN
#define P8_33 (9) // UART4_RTSN
#define P8_35 (8) // UART4_CTSN

#define P9_11 (30) // UART4_RXD
#define P9_13 (31) // UART4_TXD
#define P9_17 (5) // I2C1_SCL
#define P9_18 (4) // I2C1_SDA
#define P9_19 (13) // I2C2_SCL
#define P9_20 (12) // I2C2_SDA
#define P9_21 (3) // UART2_TXD
#define P9_22 (2) // UART2_RXD
#define P9_24 (15) // UART1_RXD
#define P9_26 (14) // UART1_RXD
#define P9_41A (20)
#define P9_42A (7)

// Defines for pins on GPIO1
#define P8_03 (6)
#define P8_04 (7)
#define P8_05 (2)
#define P8_06 (3)
#define P8_11 (13)
#define P8_12 (12)
#define P8_15 (15)
#define P8_16 (14)
#define P8_20 (31)
#define P8_21 (30)
#define P8_22 (5)
#define P8_23 (4)
#define P8_24 (1)
#define P8_25 (0)
#define P8_26 (29)

#define P9_12 (28)
#define P9_14 (18) // EHRPWM1A
#define P9_15 (16)
#define P9_16 (19) // EHRPWM1B
#define P9_23 (17)

#define USR0 (21)
#define USR1 (22)
#define USR2 (23)
#define USR3 (24)

// Defines for pins on GPIO2
#define P8_07 (2) // TIMER4
#define P8_08 (3) // TIMER7
#define P8_09 (5) // TIMER5
#define P8_10 (4) // TIMER6
#define P8_18 (1)
#define P8_27 (22)
#define P8_28 (24)
#define P8_29 (23)
#define P8_30 (25)
#define P8_34 (17) // UART3_RTSN
#define P8_36 (16) // UART3_CTSN
#define P8_37 (14) // UART5_TXD
#define P8_38 (15) // UART5_RXD
#define P8_39 (12)
#define P8_40 (13)
#define P8_41 (10)
#define P8_42 (11)
#define P8_43 (8)
#define P8_44 (9)
#define P8_45 (6)
#define P8_46 (7)

// Defines for pins on GPIO3
#define P9_25 (21)
#define P9_27 (19)
#define P9_28 (17) // SPI1_CS0
#define P9_29 (15) // SPI1_d0
#define P9_30 (16) // SPI1_D1
#define P9_31 (14) // SPI1_SCLK
#define P9_41B (20)
#define P9_42B (18)
