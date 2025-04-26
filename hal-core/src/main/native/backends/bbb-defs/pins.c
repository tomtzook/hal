
#include <stddef.h>

#include "pins.h"


pin_t PINS[] = {
    {"P8_3", P8_03, 1, 6},
    {"P8_4", P8_04, 1, 7},
    {"P8_5", P8_05, 1, 2},
    {"P8_6", P8_06, 1, 3},
    {"P8_7", P8_07, 2, 2},
    {"P8_8", P8_08, 2, 3},
    {"P8_9", P8_09, 2, 5},
    {"P8_10", P8_10, 2, 4},
    {"P8_11", P8_11, 1, 13},
    {"P8_12", P8_12, 1, 12},
    {"P8_13", P8_13, 0, 23},
    {"P8_14", P8_14, 0, 26},
    {"P8_15", P8_15, 1, 15},
    {"P8_16", P8_16, 1, 14},
    {"P8_17", P8_17, 0, 27},
    {"P8_18", P8_18, 2, 1},
    {"P8_19", P8_19, 0, 22},
    {"P8_20", P8_20, 1, 31},
    {"P8_21", P8_21, 1, 30},
    {"P8_22", P8_22, 1, 5},
    {"P8_23", P8_23, 1, 4},
    {"P8_24", P8_24, 1, 1},
    {"P8_25", P8_25, 1, 0},
    {"P8_26", P8_26, 1, 29},
    {"P8_27", P8_27, 2, 22},
    {"P8_28", P8_28, 2, 24},
    {"P8_29", P8_29, 2, 23},
    {"P8_30", P8_30, 2, 25},
    {"P8_31", P8_31, 0, 10},
    {"P8_32", P8_32, 0, 11},
    {"P8_33", P8_33, 0, 9},
    {"P8_34", P8_34, 2, 17},
    {"P8_35", P8_35, 0, 8},
    {"P8_36", P8_36, 2, 16},
    {"P8_37", P8_37, 2, 14},
    {"P8_38", P8_38, 2, 15},
    {"P8_39", P8_39, 2, 12},
    {"P8_40", P8_40, 2, 13},
    {"P8_41", P8_41, 2, 10},
    {"P8_42", P8_42, 2, 11},
    {"P8_43", P8_43, 2, 8},
    {"P8_44", P8_44, 2, 9},
    {"P8_45", P8_45, 2, 6},
    {"P8_46", P8_46, 2, 7},
    {"P9_11", P9_11, 0, 30},
    {"P9_12", P9_12, 1, 28},
    {"P9_13", P9_13, 0, 31},
    {"P9_14", P9_14, 1, 18},
    {"P9_15", P9_15, 1, 16},
    {"P9_16", P9_16, 1, 19},
    {"P9_17", P9_17, 0, 5},
    {"P9_18", P9_18, 0, 4},
    {"P9_19", P9_19, 0, 13},
    {"P9_20", P9_20, 0, 12},
    {"P9_21", P9_21, 0, 3},
    {"P9_22", P9_22, 0, 2},
    {"P9_23", P9_23, 1, 17},
    {"P9_24", P9_24, 0, 15},
    {"P9_25", P9_25, 3, 21},
    {"P9_26", P9_26, 0, 14},
    {"P9_27", P9_27, 3, 19},
    {"P9_28", P9_28, 3, 17},
    {"P9_29", P9_29, 3, 15},
    {"P9_30", P9_30, 3, 16},
    {"P9_31", P9_31, 3, 14},
    {"P9_33", P9_33, 0, 4}, // AIN4
    {"P9_35", P9_35, 0, 6}, // AIN6
    {"P9_36", P9_36, 0, 5}, // AIN5
    {"P9_37", P9_37, 0, 2}, // AIN2
    {"P9_38", P9_38, 0, 3}, // AIN3
    {"P9_39", P9_39, 0, 0}, // AIN0
    {"P9_40", P9_40, 0, 1}, // AIN1
    {"P9_41A", P9_41A, 0, 20},
    {"P9_41B", P9_41B, 3, 20},
    {"P9_42A", P9_42A, 0, 7},
    {"P9_42B", P9_42B, 3, 18},
};
static const unsigned PIN_COUNT = sizeof(PINS) / sizeof(pin_t);

pinmux_t PINMUX[] = {
    {P8_03, 0x818, {0 /*gpmc_ad6*/, 0 /*mmc1_dat6*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_6*/}},
    {P8_04, 0x81c, {0 /*gpmc_ad7*/, 0 /*mmc1_dat7*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_7*/}},
    {P8_05, 0x808, {0 /*gpmc_ad2*/, 0 /*mmc1_dat2*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_2*/}},
    {P8_06, 0x80c, {0 /*gpmc_ad3*/, 0 /*mmc1_dat3*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_3*/}},
    {P8_07, 0x890, {0 /*gpmc_advn_ale*/, 0 /**/, 0 /*timer4*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio2_2*/}},
    {P8_08, 0x894, {0 /*gpmc_oen_ren*/, 0 /**/, 0 /*timer7*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio2_3*/}},
    {P8_09, 0x89c, {0 /*gpmc_be0n_cle*/, 0 /**/, 0 /*timer5*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio2_5*/}},
    {P8_10, 0x898, {0 /*gpmc_wen*/, 0 /**/, 0 /*timer6*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio2_4*/}},
    {P8_11, 0x834, {0 /*gpmc_ad13*/, 0 /*lcd_data18*/, 0 /*mmc1_dat5*/, 0 /*mmc2_dat1*/, 0 /*eQEP2B_in*/, 0 /*pr1_mii0_txd1*/, 0 /*pr1_pru0_pru_r30_15*/, PIN_TYPE_GPIO /*gpio1_13*/}},
    {P8_12, 0x830, {0 /*gpmc_ad12*/, 0 /*lcd_data19*/, 0 /*mmc1_dat4*/, 0 /*mmc2_dat0*/, 0 /*eQEP2A_in*/, 0 /*pr1_mii0_txd2*/, 0 /*pr1_pru0_pru_r30_14*/, PIN_TYPE_GPIO /*gpio1_12*/}},
    {P8_13, 0x824, {0 /*gpmc_ad9*/, 0 /*lcd_data22*/, 0 /*mmc1_dat1*/, 0 /*mmc2_dat5*/, 0 /*ehrpwm2B*/, 0 /*pr1_mii0_col*/, 0 /**/, PIN_TYPE_GPIO /*gpio0_23*/}},
    {P8_14, 0x828, {0 /*gpmc_ad10*/, 0 /*lcd_data21*/, 0 /*mmc1_dat2*/, 0 /*mmc2_dat6*/, 0 /*ehrpwm2_tripzone_input*/, 0 /*pr1_mii0_txen*/, 0 /**/, PIN_TYPE_GPIO /*gpio0_26*/}},
    {P8_15, 0x83C, {0 /*gpmc_ad15*/, 0 /*lcd_data16*/, 0 /*mmc1_dat7*/, 0 /*mmc2_dat3*/, 0 /*eQEP2_strobe*/, 0 /*pr1_ecap0_ecap_capin_apwm_o*/, 0 /*pr1_pru0_pru_r31_15*/, PIN_TYPE_GPIO /*gpio1_15*/}},
    {P8_16, 0x838, {0 /*gpmc_ad14*/, 0 /*lcd_data17*/, 0 /*mmc1_dat6*/, 0 /*mmc2_dat2*/, 0 /*eQEP2_index*/, 0 /*pr1_mii0_txd0*/, 0 /*pr1_pru0_pru_r31_14*/, PIN_TYPE_GPIO /*gpio1_14*/}},
    {P8_17, 0x82C, {0 /*gpmc_ad11*/, 0 /*lcd_data20*/, 0 /*mmc1_dat3*/, 0 /*mmc2_dat7*/, 0 /*ehrpwm0_synco*/, 0 /*pr1_mii0_txd3*/, 0 /**/, PIN_TYPE_GPIO /*gpio0_27*/}},
    {P8_18, 0x88C, {0 /*gpmc_clk*/, 0 /*lcd_memory_clk*/, 0 /*gpmc_wait1*/, 0 /*mmc2_clk*/, 0 /*ehrpwm2A*/, 0 /*pr1_mdio_mdclk*/, 0 /*mcasp0_fsr*/, PIN_TYPE_GPIO /*gpio2_1*/}},
    {P8_19, 0x820, {0 /*gpmc_ad8*/, 0 /*lcd_data23*/, 0 /*mmc1_dat0*/, 0 /*mmc2_dat4*/, 0 /*pr1_edio_data_out7*/, 0 /*pr1_mii_mt0_clk*/, 0 /**/, PIN_TYPE_GPIO /*gpio0_22*/}},
    {P8_20, 0x884, {0 /*gpmc_csn2*/, 0 /*gpmc_be1n*/, 0 /*mmc1_cmd*/, 0 /*pr1_edio_data_in7*/, 0 /*pr1_edio_data_out7*/, 0 /*pr1_pru1_pru_r30_13*/, 0 /*pr1_pru1_pru_r31_13*/, PIN_TYPE_GPIO /*gpio1_31*/}},
    {P8_21, 0x880, {0 /*gpmc_csn1*/, 0 /*gpmc_clk*/, 0 /*mmc1_clk*/, 0 /*pr1_edio_data_in6*/, 0 /*pr1_edio_data_out6*/, 0 /*pr1_pru1_pru_r30_12*/, 0 /*pr1_pru1_pru_r31_12*/, PIN_TYPE_GPIO /*gpio1_30*/}},
    {P8_22, 0x814, {0 /*0x814*/, 0 /*gpmc_ad5*/, 0 /*mmc1_dat5*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_5*/}},
    {P8_23, 0x810, {0 /*gpmc_ad4*/, 0 /*mmc1_dat4*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_4*/}},
    {P8_24, 0x804, {0 /*gpmc_ad1*/, 0 /*mmc1_dat1*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_1*/}},
    {P8_25, 0x800, {0 /*gpmc_ad0*/, 0 /*mmc1_dat0*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_0*/}},
    {P8_26, 0x87C, {0 /*gpmc_csn0*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, PIN_TYPE_GPIO /*gpio1_29*/}},
    {P8_27, 0x8E0, {0 /*lcd_vsync*/, 0 /*gpmc_a8*/, 0 /*gpmc_a1*/, 0 /*pr1_edio_data_in2*/, 0 /*pr1_edio_data_out2*/, 0 /*pr1_pru1_pru_r30_8*/, 0 /*pr1_pru1_pru_r31_8*/, PIN_TYPE_GPIO /*gpio2_22*/}},
    {P8_28, 0x8E8, {0 /*lcd_pclk*/, 0 /*gpmc_a10*/, 0 /*pr1_mii0_crs*/, 0 /*pr1_edio_data_in4*/, 0 /*pr1_edio_data_out4*/, 0 /*pr1_pru1_pru_r30_10*/, 0 /*pr1_pru1_pru_r31_10*/, PIN_TYPE_GPIO /*gpio2_24*/}},
    {P8_29, 0x8E4, {0 /*lcd_hsync*/, 0 /*gpmc_a9*/, 0 /*gpmc_a2*/, 0 /*pr1_edio_data_in3*/, 0 /*pr1_edio_data_out3*/, 0 /*pr1_pru1_pru_r30_9*/, 0 /*pr1_pru1_pru_r31_9*/, PIN_TYPE_GPIO /*gpio2_23*/}},
    {P8_30, 0x8EC, {0 /*lcd_ac_bias_en*/, 0 /*gpmc_a11*/, 0 /*pr1_mii1_crs*/, 0 /*pr1_edio_data_in5*/, 0 /*pr1_edio_data_out5*/, 0 /*pr1_pru1_pru_r30_11*/, 0 /*pr1_pru1_pru_r31_11*/, PIN_TYPE_GPIO /*gpio2_25*/}},
    {P8_31, 0x8D8, {0 /*lcd_data14*/, 0 /*gpmc_a18*/, 0 /*eQEP1_index*/, 0 /*mcasp0_axr1*/, 0 /*uart5_rxd*/, 0 /*pr1_mii_mr0_clk*/, 0 /*uart5_ctsn*/, PIN_TYPE_GPIO /*gpio0_10*/}},
    {P8_32, 0x8DC, {0 /*lcd_data15*/, 0 /*gpmc_a19, 0 /*eQEP1_strobe*/, 0 /*mcasp0_ahclkx*/, 0 /*mcasp0_axr3*/, 0 /*pr1_mii0_rxdv*/, 0 /*uart5_rtsn*/, PIN_TYPE_GPIO /*gpio0_11*/}},
    {P8_33, 0x8D4, {0 /*lcd_data13*/, 0 /*gpmc_a17*/, 0 /*eQEP1B_in*/, 0 /*mcasp0_fsr*/, 0 /*mcasp0_axr3*/, 0 /*pr1_mii0_rxer*/, 0 /*uart4_rtsn*/, PIN_TYPE_GPIO /*gpio0_9*/}},
    {P8_34, 0x8CC, {0 /*lcd_data11*/, 0 /*gpmc_a15*/, 0 /*ehrpwm1B*/, 0 /*mcasp0_ahclkr*/, 0 /*mcasp0_axr2*/, 0 /*pr1_mii0_rxd0*/, 0 /*uart3_rtsn*/, PIN_TYPE_GPIO /*gpio2_17*/}},
    {P8_35, 0x8D0, {0 /*lcd_data12*/, 0 /*gpmc_a16*/, 0 /*eQEP1A_in*/, 0 /*mcasp0_aclkr*/, 0 /*mcasp0_axr2*/, 0 /*pr1_mii0_rxlink*/, 0 /*uart4_ctsn*/, PIN_TYPE_GPIO /*gpio0_8*/}},
    {P8_36, 0x8C8, {0 /*lcd_data10*/, 0 /*gpmc_a14*/, 0 /*ehrpwm1A*/, 0 /*mcasp0_axr0*/, 0 /**/, 0 /*pr1_mii0_rxd1*/, 0 /*uart3_ctsn*/, PIN_TYPE_GPIO /*gpio2_16*/}},
    {P8_37, 0x8C0, {0 /*lcd_data8*/, 0 /*gpmc_a12*/, 0 /*ehrpwm1_tripzone_input*/, 0 /*mcasp0_aclkx*/, 0 /*uart5_txd*/, 0 /*pr1_mii0_rxd3*/, 0 /*uart2_ctsn*/, PIN_TYPE_GPIO /*gpio2_14*/}},
    {P8_38, 0x8C4, {0 /*lcd_data9*/, 0 /*gpmc_a13*/, 0 /*ehrpwm0_synco*/, 0 /*mcasp0_fsx*/, 0 /*uart5_rxd*/, 0 /*pr1_mii0_rxd2*/, 0 /*uart2_rtsn*/, PIN_TYPE_GPIO /*gpio2_15*/}},
    {P8_39, 0x8B8, {0 /*lcd_data6*/, 0 /*gpmc_a6*/, 0 /*pr1_edio_data_in6*/, 0 /*eQEP2_index*/, 0 /*pr1_edio_data_out6*/, 0 /*pr1_pru1_pru_r30_6*/, 0 /*pr1_pru1_pru_r31_6*/, PIN_TYPE_GPIO /*gpio2_12*/}},
    {P8_40, 0x8BC, {0 /*lcd_data7*/, 0 /*gpmc_a7*/, 0 /*pr1_edio_data_in7*/, 0 /*eQEP2_strobe*/, 0 /*pr1_edio_data_out7*/, 0 /*pr1_pru1_pru_r30_7*/, 0 /*pr1_pru1_pru_r31_7*/, PIN_TYPE_GPIO /*gpio2_13*/}},
    {P8_41, 0x8B0, {0 /*lcd_data4*/, 0 /*gpmc_a4*/, 0 /*pr1_mii0_txd1*/, 0 /*eQEP2A_in*/, 0 /**/, 0 /*pr1_pru1_pru_r30_4*/, 0 /*pr1_pru1_pru_r31_4*/, PIN_TYPE_GPIO /*gpio2_10*/}},
    {P8_42, 0x8B4, {0 /*lcd_data5*/, 0 /*gpmc_a5*/, 0 /*pr1_mii0_txd0*/, 0 /*eQEP2B_in*/, 0 /**/, 0 /*pr1_pru1_pru_r30_5*/, 0 /*pr1_pru1_pru_r31_5*/, PIN_TYPE_GPIO /*gpio2_11*/}},
    {P8_43, 0x8A8, {0 /*lcd_data2*/, 0 /*gpmc_a2*/, 0 /*pr1_mii0_txd3*/, 0 /*ehrpwm2_tripzone_input*/, 0 /**/, 0 /*pr1_pru1_pru_r30_2*/, 0 /*pr1_pru1_pru_r31_2*/, PIN_TYPE_GPIO /*gpio2_8*/}},
    {P8_44, 0x8AC, {0 /*lcd_data3*/, 0 /*gpmc_a3*/, 0 /*pr1_mii0_txd2*/, 0 /*ehrpwm0_synco*/, 0 /**/, 0 /*pr1_pru1_pru_r30_3*/, 0 /*pr1_pru1_pru_r31_3*/, PIN_TYPE_GPIO /*gpio2_9*/}},
    {P8_45, 0x8A0, {0 /*lcd_data0*/, 0 /*gpmc_a0*/, 0 /*pr1_mii_mt0_clk*/, 0 /*ehrpwm2A*/, 0 /**/, 0 /*pr1_pru1_pru_r30_0*/, 0 /*pr1_pru1_pru_r31_0*/, PIN_TYPE_GPIO /*gpio2_6*/}},
    {P8_46, 0x8A4, {0 /*lcd_data1*/, 0 /*gpmc_a1*/, 0 /*pr1_mii0_txen*/, 0 /*ehrpwm2B*/, 0 /*emp*/, 0 /*pr1_pru1_pru_r30_1*/, 0 /*pr1_pru1_pru_r31_1*/, PIN_TYPE_GPIO /*gpio2_7*/}},
    {P9_11, 0x870, {0 /*gpmc_wait0*/, 0 /*gmii2_crs*/, 0 /*gpmc_csn4*/, 0 /*rmii2_crs_dv*/, 0 /*mmc1_sdcd*/, 0 /*pr1_mii1_col*/, 0 /*uart4_rxd*/, PIN_TYPE_GPIO /*gpio0_30*/}},
    {P9_12, 0x878, {0 /*gpmc_be1n*/, 0 /*gmii2_col*/, 0 /*gpmc_csn6*/, 0 /*mmc2_dat3*/, 0 /*gpmc_dir*/, 0 /*pr1_mii1_rxlink*/, 0 /*mcasp0_aclkr*/, PIN_TYPE_GPIO /*gpio1_28*/}},
    {P9_13, 0x874, {0 /*gpmc_wpn*/, 0 /*gmii2_rxerr*/, 0 /*gpmc_csn5*/, 0 /*rmii2_rxerr*/, 0 /*mmc2_sdcd*/, 0 /*pr1_mii1_txen*/, 0 /*uart4_txd*/, PIN_TYPE_GPIO /*gpio0_31*/}},
    {P9_14, 0x848, {0 /*gpmc_a2*/, 0 /*gmii2_txd3*/, 0 /*rgmii2_td3*/, 0 /*mmc2_dat1*/, 0 /*gpmc_a18*/, 0 /*pr1_mii1_txd2*/, 0 /*ehrpwm1A*/, PIN_TYPE_GPIO /*gpio1_18*/}},
    {P9_15, 0x840, {0 /*gpmc_a0*/, 0 /*gmii2_txen*/, 0 /*rgmii2_tctl*/, 0 /*rmii2_txen*/, 0 /*gpmc_a16*/, 0 /*pr1_mii_mt1_clk*/, 0 /*ehrpwm1_tripzone_input*/, PIN_TYPE_GPIO /*gpio1_16*/}},
    {P9_16, 0x84C, {0 /*gpmc_a3*/, 0 /*gmii2_txd2*/, 0 /*rgmii2_td2*/, 0 /*mmc2_dat2*/, 0 /*gpmc_a19*/, 0 /*pr1_mii1_txd1*/, 0 /*ehrpwm1B*/, PIN_TYPE_GPIO /*gpio1_19*/}},
    {P9_17, 0x95C, {0 /*spi0_cs0*/, 0 /*mmc2_sdwp*/, 0 /*I2C1_SCL*/, 0 /*ehrpwm0_synci*/, 0 /*pr1_uart0_txd*/, 0 /*pr1_edio_data_in1*/, 0 /*pr1_edio_data_out1*/, PIN_TYPE_GPIO /*gpio0_5*/}},
    {P9_18, 0x958, {0 /*spi0_d1*/, 0 /*mmc1_sdwp*/, 0 /*I2C1_SDA*/, 0 /*ehrpwm0_tripzone_input*/, 0 /*pr1_uart0_rxd*/, 0 /*pr1_edio_data_in0*/, 0 /*pr1_edio_data_out0*/, PIN_TYPE_GPIO /*gpio0_4*/}},
    {P9_19, 0x97C, {0 /*uart1_rtsn*/, 0 /*timer5*/, 0 /*dcan0_rx*/, 0 /*I2C2_SCL*/, 0 /*spi1_cs1*/, 0 /*pr1_uart0_rts_n*/, 0 /*pr1_edc_latch1_in*/, PIN_TYPE_GPIO /*gpio0_13*/}},
    {P9_20, 0x978, {0 /*uart1_ctsn*/, 0 /*timer6*/, 0 /*dcan0_tx*/, 0 /*I2C2_SDA*/, 0 /*spi1_cs0*/, 0 /*pr1_uart0_cts_n*/, 0 /*pr1_edc_latch0_in*/, PIN_TYPE_GPIO /*gpio0_12*/}},
    {P9_21, 0x954, {0 /*spi0_d0*/, 0 /*uart2_txd*/, 0 /*I2C2_SCL*/, 0 /*ehrpwm0B*/, 0 /*pr1_uart0_rts_n*/, 0 /*pr1_edio_latch_in*/, 0 /*EMU3*/, PIN_TYPE_GPIO /*gpio0_3*/}},
    {P9_22, 0x950, {0 /*spi0_sclk*/, 0 /*uart2_rxd*/, 0 /*I2C2_SDA*/, 0 /*ehrpwm0A*/, 0 /*pr1_uart0_cts_n*/, 0 /*pr1_edio_sof*/, 0 /*EMU2*/, PIN_TYPE_GPIO /*gpio0_2*/}},
    {P9_23, 0x844, {0 /*gpmc_a1*/, 0 /*gmii2_rxdv*/, 0 /*rgmii2_rctl*/, 0 /*mmc2_dat0*/, 0 /*gpmc_a17*/, 0 /*pr1_mii1_txd3*/, 0 /*ehrpwm0_synco*/, PIN_TYPE_GPIO /*gpio1_17*/}},
    {P9_24, 0x984, {0 /*uart1_txd*/, 0 /*mmc2_sdwp*/, 0 /*dcan1_rx*/, 0 /*I2C1_SCL*/, 0 /**/, 0 /*pr1_uart0_txd*/, 0 /*pr1_pru0_pru_r31_16*/, PIN_TYPE_GPIO /*gpio0_15*/}},
    {P9_25, 0x9AC, {0 /*mcasp0_ahclkx*/, 0 /*eQEP0_strobe*/, 0 /*mcasp0_axr3*/, 0 /*mcasp1_axr1*/, 0 /*EMU4*/, 0 /*pr1_pru0_pru_r30_7*/, 0 /*pr1_pru0_pru_r31_7*/, PIN_TYPE_GPIO /*gpio3_21*/}},
    {P9_26, 0x980, {0 /*uart1_rxd*/, 0 /*mmc1_sdwp*/, 0 /*dcan1_tx*/, 0 /*I2C1_SDA*/, 0 /**/, 0 /*pr1_uart0_rxd*/, 0 /*pr1_pru1_pru_r31_16*/, PIN_TYPE_GPIO /*gpio0_14*/}},
    {P9_27, 0x9A4, {0 /*mcasp0_fsr*/, 0 /*eQEP0B_in*/, 0 /*mcasp0_axr3*/, 0 /*mcasp1_fsx*/, 0 /*EMU2*/, 0 /*pr1_pru0_pru_r30_5*/, 0 /*pr1_pru0_pru_r31_5*/, PIN_TYPE_GPIO /*gpio3_19*/}},
    {P9_28, 0x99C, {0 /*mcasp0_ahclkr*/, 0 /*ehrpwm0_synci*/, 0 /*mcasp0_axr2*/, 0 /*spi1_cs0*/, 0 /*eCAP2_in_PWM2_out*/, 0 /*pr1_pru0_pru_r30_3*/, 0 /*pr1_pru0_pru_r31_3*/, PIN_TYPE_GPIO /*gpio3_17*/}},
    {P9_29, 0x994, {0 /*mcasp0_fsx*/, 0 /*ehrpwm0B*/, 0 /**/, 0 /*spi1_d0*/, 0 /*mmc1_sdcd*/, 0 /*pr1_pru0_pru_r30_1*/, 0 /*pr1_pru0_pru_r31_1*/, PIN_TYPE_GPIO /*gpio3_15*/}},
    {P9_30, 0x998, {0 /*mcasp0_axr0*/, 0 /*ehrpwm0_tripzone_input*/, 0 /**/, 0 /*spi1_d1*/, 0 /*mmc2_sdcd*/, 0 /*pr1_pru0_pru_r30_2*/, 0 /*pr1_pru0_pru_r31_2*/, PIN_TYPE_GPIO /*gpio3_16*/}},
    {P9_31, 0x990, {0 /*mcasp0_aclkx*/, 0 /*ehrpwm0A*/, 0 /**/, 0 /*spi1_sclk*/, 0 /*mmc0_sdcd*/, 0 /*pr1_pru0_pru_r30_0*/, 0 /*pr1_pru0_pru_r31_0*/, PIN_TYPE_GPIO /*gpio3_14*/}},
    {P9_33, 0x00, {PIN_TYPE_AIN /*AIN4*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/}},
    {P9_35, 0x00, {PIN_TYPE_AIN /*AIN6*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/}},
    {P9_36, 0x00, {PIN_TYPE_AIN /*AIN5*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/}},
    {P9_37, 0x00, {PIN_TYPE_AIN /*AIN2*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/}},
    {P9_38, 0x00, {PIN_TYPE_AIN /*AIN3*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/}},
    {P9_39, 0x00, {PIN_TYPE_AIN /*AIN0*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/}},
    {P9_40, 0x00, {PIN_TYPE_AIN /*AIN1*/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/, 0 /**/}},
    {P9_41A, 0x9B4, {0 /*xdma_event_intr1*/, 0 /**/, 0 /*tclkin*/, 0 /*clkout2*/, 0 /*timer7*/, 0 /*pr1_pru0_pru_r31_16*/, 0 /*EMU3*/, PIN_TYPE_GPIO /*gpio0_20*/}},
    {P9_41B, 0x9A8, {0 /*mcasp0_axr1*/, 0 /*eQEP0_index*/, 0 /**/, 0 /*mcasp1_axr0*/, 0 /*EMU3*/, 0 /*pr1_pru0_pru_r30_6*/, 0 /*pr1_pru0_pru_r31_6*/, PIN_TYPE_GPIO /*gpio3_20*/}},
    {P9_42A, 0x964, {0 /*eCAP0_in_PWM0_out*/, 0 /*uart3_txd*/, 0 /*spi1_cs1*/, 0 /*pr1_ecap0_ecap_capin_apwm_o*/, 0 /*spi1_sclk*/, 0 /*mmc0_sdwp*/, 0 /*xdma_event_intr2*/, PIN_TYPE_GPIO /*gpio0_7*/}},
    {P9_42B, 0x9A0, {0 /*mcasp0_aclkr*/, 0 /*eQEP0A_in*/, 0 /*mcasp0_axr2*/, 0 /*mcasp1_aclkx*/, 0 /*mmc0_sdwp*/, 0 /*pr1_pru0_pru_r30_4*/, 0 /*pr1_pru0_pru_r31_4*/, PIN_TYPE_GPIO /*gpio3_18*/}}

};
static const unsigned PINMUX_COUNT = sizeof(PINMUX) / sizeof(pinmux_t);

pwm_chip_t PWM_CHIPS[] = { // todo: what about tripzone_input, synco/synci or other related?
    {"EHRPWM0", 1, 48300000},
    {"EHRPWM1", 4, 48302000},
    {"EHRPWM2", 7, 48304000},
};

pwm_pin_t PWM_PINS[] = {
    {"EHRPWM0A", P9_22, 1, 0},
    {"EHRPWM0B", P9_21, 1, 1},
    {"EHRPWM1A", P9_14, 4, 0},
    {"EHRPWM1B", P9_16, 4, 1},
    {"EHRPWM1A", P8_36, 4, 0},
    {"EHRPWM1B", P8_34, 4, 1},
    {"EHRPWM2A", P8_19, 7, 0},
    {"EHRPWM2B", P8_13, 7, 1},
    {"EHRPWM2A", P8_45, 7, 0},
    {"EHRPWM2B", P8_46, 7, 1},
};


const pin_t* get_pin(unsigned index) {
    if (index >= PIN_COUNT) {
        return NULL;
    }

    return &PINS[index];
}

unsigned get_pin_count() {
    return PIN_COUNT;
}

const pin_t* find_pin(const pin_predicate predicate) {
    for (unsigned i = 0; i < PIN_COUNT; i++) {
        const pin_t* pin = &PINS[i];
        if (predicate(pin)) {
            return pin;
        }
    }

    return NULL;
}

const pin_t* find_pin_by_id(const unsigned id) {
    for (unsigned i = 0; i < PIN_COUNT; i++) {
        const pin_t* pin = &PINS[i];
        if (id == pin->id) {
            return pin;
        }
    }

    return NULL;
}

const pinmux_t* get_pinmux_for_pin(const pin_t* pin) {
    for (unsigned i = 0; i < PINMUX_COUNT; i++) {
        const pinmux_t* pinmux = &PINMUX[i];
        if (pinmux->id == pin->id) {
            return pinmux;
        }
    }

    return NULL;
}
