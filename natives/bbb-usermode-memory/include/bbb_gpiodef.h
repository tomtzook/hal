#pragma once

#define MODULE(pin_number) ((pin_number) / 32)
#define PIN(pin_number) ((pin_number) % 32)
#define PIN_NUMBER(module, index) ((module) * 32 + (index))
#define PIN_MASK(pin) (1u << (pin))

// Defines for pins on GPIO0
#define P8_13 PIN_NUMBER(0, 23) // EHRPWM2B
#define P8_14 PIN_NUMBER(0, 26)
#define P8_17 PIN_NUMBER(0, 27)
#define P8_19 PIN_NUMBER(0, 22) // EHRPWM2A
#define P8_31 PIN_NUMBER(0, 10) // UART5_CTSN
#define P8_32 PIN_NUMBER(0, 11) // UART5_RTSN
#define P8_33 PIN_NUMBER(0, 9) // UART4_RTSN
#define P8_35 PIN_NUMBER(0, 8) // UART4_CTSN

#define P9_11 PIN_NUMBER(0, 30) // UART4_RXD
#define P9_13 PIN_NUMBER(0, 31) // UART4_TXD
#define P9_17 PIN_NUMBER(0, 5) // I2C1_SCL
#define P9_18 PIN_NUMBER(0, 4) // I2C1_SDA
#define P9_19 PIN_NUMBER(0, 13) // I2C2_SCL
#define P9_20 PIN_NUMBER(0, 12) // I2C2_SDA
#define P9_21 PIN_NUMBER(0, 3) // UART2_TXD
#define P9_22 PIN_NUMBER(0, 2) // UART2_RXD
#define P9_24 PIN_NUMBER(0, 15) // UART1_RXD
#define P9_26 PIN_NUMBER(0, 14) // UART1_RXD
#define P9_41A PIN_NUMBER(0, 20)
#define P9_42A PIN_NUMBER(0, 7)

// Defines for pins on GPIO1
#define P8_03 PIN_NUMBER(1, 6)
#define P8_04 PIN_NUMBER(1, 7)
#define P8_05 PIN_NUMBER(1, 2)
#define P8_06 PIN_NUMBER(1, 3)
#define P8_11 PIN_NUMBER(1, 13)
#define P8_12 PIN_NUMBER(1, 12)
#define P8_15 PIN_NUMBER(1, 15)
#define P8_16 PIN_NUMBER(1, 14)
#define P8_20 PIN_NUMBER(1, 31)
#define P8_21 PIN_NUMBER(1, 30)
#define P8_22 PIN_NUMBER(1, 5)
#define P8_23 PIN_NUMBER(1, 4)
#define P8_24 PIN_NUMBER(1, 1)
#define P8_25 PIN_NUMBER(1, 0)
#define P8_26 PIN_NUMBER(1, 29)

#define P9_12 PIN_NUMBER(1, 28)
#define P9_14 PIN_NUMBER(1, 18) // EHRPWM1A
#define P9_15 PIN_NUMBER(1, 16)
#define P9_16 PIN_NUMBER(1, 19) // EHRPWM1B
#define P9_23 PIN_NUMBER(1, 17)

#define USR0 PIN_NUMBER(1, 21)
#define USR1 PIN_NUMBER(1, 22)
#define USR2 PIN_NUMBER(1, 23)
#define USR3 PIN_NUMBER(1, 24)

// Defines for pins on GPIO2
#define P8_07 PIN_NUMBER(2, 2) // TIMER4
#define P8_08 PIN_NUMBER(2, 3) // TIMER7
#define P8_09 PIN_NUMBER(2, 5) // TIMER5
#define P8_10 PIN_NUMBER(2, 4) // TIMER6
#define P8_18 PIN_NUMBER(2, 1)
#define P8_27 PIN_NUMBER(2, 22)
#define P8_28 PIN_NUMBER(2, 24)
#define P8_29 PIN_NUMBER(2, 23)
#define P8_30 PIN_NUMBER(2, 25)
#define P8_34 PIN_NUMBER(2, 17) // UART3_RTSN
#define P8_36 PIN_NUMBER(2, 16) // UART3_CTSN
#define P8_37 PIN_NUMBER(2, 14) // UART5_TXD
#define P8_38 PIN_NUMBER(2, 15) // UART5_RXD
#define P8_39 PIN_NUMBER(2, 12)
#define P8_40 PIN_NUMBER(2, 13)
#define P8_41 PIN_NUMBER(2, 10)
#define P8_42 PIN_NUMBER(2, 11)
#define P8_43 PIN_NUMBER(2, 8)
#define P8_44 PIN_NUMBER(2, 9)
#define P8_45 PIN_NUMBER(2, 6)
#define P8_46 PIN_NUMBER(2, 7)

// Defines for pins on GPIO3
#define P9_25 PIN_NUMBER(3, 21)
#define P9_27 PIN_NUMBER(3, 19)
#define P9_28 PIN_NUMBER(3, 17) // SPI1_CS0
#define P9_29 PIN_NUMBER(3, 15) // SPI1_d0
#define P9_30 PIN_NUMBER(3, 16) // SPI1_D1
#define P9_31 PIN_NUMBER(3, 14) // SPI1_SCLK
#define P9_41B PIN_NUMBER(3, 20)
#define P9_42B PIN_NUMBER(3, 18)
