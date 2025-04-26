#pragma once

#include <stdint.h>


// gpio_registers_t
#define GPIO0_PERIPHERAL_BASE 0x44e07000
#define GPIO1_PERIPHERAL_BASE 0x4804c000
#define GPIO2_PERIPHERAL_BASE 0x481ac000
#define GPIO3_PERIPHERAL_BASE 0x481ae000
#define GPIO_PERIPHERAL_SIZE 0x1000

// adc_registers_t
#define ADCTSC_PERIPHERAL_BASE 0x44e0d000
#define ADCTSC_PERIPHERAL_SIZE 0x2000

// controlmodule_registers_t
#define CONTROL_MODULE_PERIPHERAL_BASE 0x44e10000
#define CONTROL_MODULE_PERIPHERAL_SIZE 0x2000

#define REG(ptr, type) ((volatile type *) (ptr))

#pragma pack(push, 1)

// todo: pwmss, cm_per, cm, pruss

typedef enum _idle_mode : uint32_t {
    idle_mode_force = 0x0,
    idle_mode_no = 0x1,
    idle_mode_smart = 0x2,
    idle_mode_smart_wakeup = 0x3
} idle_mode_t;

typedef enum : uint32_t {
    step_id_idle = 0x10,
    step_id_charge = 0x11,
    step_id_1 = 0x0,
    step_id_2 = 0x1,
    step_id_3 = 0x2,
    step_id_4 = 0x3,
    step_id_5 = 0x4,
    step_id_6 = 0x5,
    step_id_7 = 0x6,
    step_id_8 = 0x7,
    step_id_9 = 0x8,
    step_id_10 = 0x9,
    step_id_11 = 0xa,
    step_id_12 = 0xb,
    step_id_13 = 0xc,
    step_id_14 = 0xd,
    step_id_15 = 0xe,
    step_id_16 = 0xf,
} adc_step_id_t;

typedef enum : uint32_t {
    mode_sw_enabled_oneshot = 0x0,
    mode_sw_enabled_continuous = 0x1,
    mode_hw_sync_oneshot = 0x2,
    mode_hw_sync_continuous = 0x3,
} adc_step_mode_t;
typedef enum : uint32_t {
    averaging_no = 0x0,
    averaging_2 = 0x1,
    averaging_4 = 0x2,
    averaging_8 = 0x3,
    averaging_16 = 0x4,
} adc_averaging_t;

typedef enum : uint32_t {
    sel_rfp_t_vdda_adc = 0x0,
    sel_rfp_t_vdda_xpul = 0x1,
    sel_rfp_t_vdda_ypll = 0x2,
    sel_rfp_t_vdda_vrefp = 0x3,
} adc_sel_rfp_t;

typedef enum : uint32_t {
    sel_inm_channel_1 = 0x0,
    sel_inm_channel_8 = 0x7,
    sel_inm_adcrefm = 0x8,
} adc_sel_inm_t;

typedef enum : uint32_t {
    sel_inp_channel_1 = 0x0,
    sel_inp_channel_8 = 0x7,
    sel_inp_vrefn = 0x8,
} adc_sel_inp_t;

typedef enum : uint32_t {
    sel_rfm_vssa_adc = 0x0,
    sel_rfm_xnur = 0x1,
    sel_rfm_ynlr = 0x2,
    sel_rfm_vrefn = 0x3
} adc_sel_rfm_t;

typedef union {
    uint32_t data;
    struct {
        adc_step_mode_t mode : 2;
        adc_averaging_t averaging : 3;
        uint32_t xppsw_swc : 1;
        uint32_t xnnsw_swc : 1;
        uint32_t yppsw_swc : 1;
        uint32_t ynnsw_swc : 1;
        uint32_t xnpsw_swc : 1;
        uint32_t ypnsw_swc : 1;
        uint32_t wpnsw_swc : 1;
        adc_sel_rfp_t sel_rfp_swc_2_0 : 3;
        adc_sel_inm_t sel_inm_swc_3_0 : 4;
        adc_sel_inp_t sel_inp_swc_3_0 : 4;
        adc_sel_rfm_t sel_rfm_swc_1_0 : 2;
        uint32_t diff_cntrl : 1;
        uint32_t fifo_select : 1;
        uint32_t range_check : 1;
    } bits;
} adc_stepconfig_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t open_delay : 18;
        uint32_t reserved0 : 6;
        uint32_t sample_delay : 8;
    } bits;
} adc_stepdelay_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t words : 7;
    } bits;
} adc_fifocount_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t level : 6;
    } bits;
} adc_fifothreshold_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t dma_request_level : 6;
    } bits;
} adc_dmareq_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t adcdata : 12;
        uint32_t reserved0 : 4;
        uint32_t adcchnlid : 4;
    } bits;
} adc_fifodata_t;

typedef enum {
    pinmux_mode0 = 0b000,
    pinmux_mode1 = 0b001,
    pinmux_mode2 = 0b010,
    pinmux_mode3 = 0b011,
    pinmux_mode4 = 0b100,
    pinmux_mode5 = 0b101,
    pinmux_mode6 = 0b110,
    pinmux_mode7 = 0b111,
} cm_conf_pin_muxmode_t;

typedef enum {
    pull_pd_sel_act = 0b00,
    pull_pd_sel_nact = 0b01,
    pull_pu_sel_act = 0b10,
    pull_pu_sel_nact = 0b11,
} cm_conf_pin_pullsel_t;

typedef union  {
    uint32_t value;
    struct {
        uint32_t mmode : 3;
        uint32_t puden : 1;
        uint32_t putypesel : 1;
        uint32_t rxactive : 1;
        uint32_t slewctrl : 1;
        uint32_t reserved0 : 13;
        uint32_t reserved1 : 12;
    } bits;
} cm_conf_pin_reg_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t cm_pwrdn : 1;
        uint32_t otg_pwrdn : 1;
        uint32_t chgdet_dis : 1;
        uint32_t chgdet_rsrt : 1;
        uint32_t srcondm : 1;
        uint32_t sinkondp : 1;
        uint32_t chgisink_en : 1;
        uint32_t chgvsrv_en : 1;
        uint32_t dmpulldn : 1;
        uint32_t dppullup : 1;
        uint32_t cdet_extcl : 1;
        uint32_t reserved0 : 1;
        uint32_t gpiomode : 1;
        uint32_t gpio_sig_inv : 1;
        uint32_t gpio_sig_cross : 1;
        uint32_t reserved1 : 1;
        uint32_t reserved2 : 1;
        uint32_t dpgpio_pd : 1;
        uint32_t dmgpio_pd : 1;
        uint32_t otgvdet_en : 1;
        uint32_t otgsessenden : 1;
        uint32_t reserved3 : 1;
        uint32_t reserved4 : 1;
        uint32_t datapolarity_inv : 1;
        uint32_t reserved5 : 8;
    } bits;
} cm_usb_ctrl_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t chgdetdone : 1;
        uint32_t chgdetect : 1;
        uint32_t cdet_datadet : 1;
        uint32_t cdet_dpdet : 1;
        uint32_t cdet_dmdet : 1;
        uint32_t chgdetsts : 3;
        uint32_t reserved0 : 24;
    } bits;
} cm_usb_sts_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t macaddr_39_32 : 8;
        uint32_t macaddr_47_40 : 8;
        uint32_t reserved0 : 16;
    } bits;
} cm_mac_id_lo_t;

typedef union {
    uint32_t data;
    struct {
        uint32_t macaddr_7_0 : 8;
        uint32_t macaddr_15_8 : 8;
        uint32_t macaddr_23_16 : 8;
        uint32_t macaddr_31_24 : 8;
    } bits;
} cm_mac_id_hi_t;

typedef struct _gpio_registers {
    union {
        uint32_t data;
        struct {
            uint32_t minor : 6;
            uint32_t custom : 2;
            uint32_t major : 3;
            uint32_t rtl : 5;
            uint32_t func : 12;
            uint32_t reserved0 : 2;
            uint32_t scheme : 2;
        } bits;
    } revision;
    uint8_t reserved0[0xc];
    union {
        uint32_t data;
        struct {
            uint32_t auto_idle : 1;
            uint32_t softreset : 1;
            uint32_t ena_wakeup : 1;
            idle_mode_t idle_mode : 2;
        } bits;
    } sysconfig;
    uint8_t reserved1[0xc];
    union {
        uint32_t data;
        struct {
            uint32_t dma_event_ack : 1;
        } bits;
    } eoi;
    uint32_t irqstatus_raw0;
    uint32_t irqstatus_raw1;
    uint32_t irqstatus0;
    uint32_t irqstatus1;
    uint32_t irqstatus_set0;
    uint32_t irqstatus_set1;
    uint32_t irqstatus_clr0;
    uint32_t irqstatus_clr1;
    uint32_t irqwaken0;
    uint32_t irqwaken1;
    uint8_t reserved2[0xc8];
    uint32_t sysstatus;
    uint8_t reserved3[0x18];
    union {
        uint32_t data;
        struct {
            uint32_t disable_module : 1;
            uint32_t gating_ratio : 2;
        } bits;
    } ctrl;
    uint32_t oe;
    uint32_t datain;
    uint32_t dataout;
    uint32_t leveldetect0;
    uint32_t leveldetect1;
    uint32_t risingdetect;
    uint32_t fallingdetect;
    uint32_t debouncingenable;
    union {
        uint32_t data;
        struct {
            uint32_t debouncingtime : 8;
        } bits;
    } debouncingtime;
    uint8_t reserved5[0x38];
    uint32_t cleardataout;
    uint32_t setdataout;
} gpio_registers_t;

typedef struct _adc_registers {
    union {
        uint32_t data;
        struct {
            uint32_t y_minor : 6;
            uint32_t custom : 2;
            uint32_t x_major : 3;
            uint32_t r_rtl : 5;
            uint32_t func : 12;
            uint32_t reserved0 : 2;
            uint32_t scheme : 2;
        } bits;
    } revision;
    uint8_t reserved0[0xc];
    union {
        uint32_t data;
        struct {
            uint32_t reserved0 : 2;
            idle_mode_t idle_mode : 2;
        } bits;
    } sysconfig;
    uint8_t reserved1[0x10];
    union {
        uint32_t data;
        struct {
            uint32_t hw_pen_event_async : 1;
            uint32_t end_of_sequence : 1;
            uint32_t fifo0_threshold : 1;
            uint32_t fifo0_overrun : 1;
            uint32_t fifo0_underflow : 1;
            uint32_t fifo1_threshold : 1;
            uint32_t fifo1_overrun : 1;
            uint32_t fifo1_underflow : 1;
            uint32_t out_of_range : 1;
            uint32_t pen_up_event : 1;
            uint32_t pen_irq_sync : 1;
        } bits;
    } irqstatus_raw;
    union {
        uint32_t data;
        struct {
            uint32_t hw_pen_event_async : 1;
            uint32_t end_of_sequence : 1;
            uint32_t fifo0_threshold : 1;
            uint32_t fifo0_overrun : 1;
            uint32_t fifo0_underflow : 1;
            uint32_t fifo1_threshold : 1;
            uint32_t fifo1_overrun : 1;
            uint32_t fifo1_underflow : 1;
            uint32_t out_of_range : 1;
            uint32_t pen_up_event : 1;
            uint32_t pen_irq_sync : 1;
        } bits;
    } irqstatus;
    union {
        uint32_t data;
        struct {
            uint32_t hw_pen_event_async : 1;
            uint32_t end_of_sequence : 1;
            uint32_t fifo0_threshold : 1;
            uint32_t fifo0_overrun : 1;
            uint32_t fifo0_underflow : 1;
            uint32_t fifo1_threshold : 1;
            uint32_t fifo1_overrun : 1;
            uint32_t fifo1_underflow : 1;
            uint32_t out_of_range : 1;
            uint32_t pen_up_event : 1;
            uint32_t pen_irq_sync : 1;
        } bits;
    } irqenable_set;
    union {
        uint32_t data;
        struct {
            uint32_t hw_pen_event_async : 1;
            uint32_t end_of_sequence : 1;
            uint32_t fifo0_threshold : 1;
            uint32_t fifo0_overrun : 1;
            uint32_t fifo0_underflow : 1;
            uint32_t fifo1_threshold : 1;
            uint32_t fifo1_overrun : 1;
            uint32_t fifo1_underflow : 1;
            uint32_t out_of_range : 1;
            uint32_t pen_up_event : 1;
            uint32_t pen_irq_sync : 1;
        } bits;
    } irqstatus_clr;
    union {
        uint32_t data;
        struct {
            uint32_t wakeen0 : 1;
        } bits;
    } irqwakeup;
    union {
        uint32_t data;
        struct {
            uint32_t enable_0 : 1;
            uint32_t enable_1 : 1;
        } bits;
    } dmaenable_set;
    union {
        uint32_t data;
        struct {
            uint32_t enable_0 : 1;
            uint32_t enable_1 : 1;
        } bits;
    } dmaenable_clr;
    union {
        uint32_t data;
        struct {
            uint32_t enable : 1;
            uint32_t step_id_tag : 1;
            uint32_t stepconfig_writeprotect_n_active_low : 1;
            uint32_t adc_bias_select : 1;
            uint32_t power_down : 1;
            uint32_t afe_pen_ctrl : 2;
            uint32_t touch_screen_enable : 1;
            uint32_t hw_event_mapping : 1;
            uint32_t hw_preempt : 1;
        } bits;
    } ctrl;
    union {
        uint32_t data;
        struct {
            adc_step_id_t step_id : 5;
            uint32_t fsm_busy : 1;
            uint32_t pen_irq0 : 1;
            uint32_t pen_irq1 : 1;
        } bits;
    } adcstat;
    union {
        uint32_t data;
        struct {
            uint32_t low_range_data : 12;
            uint32_t reserved0 : 4;
            uint32_t high_range_data : 12;
        } bits;
    } adcrange;
    union {
        uint32_t data;
        struct {
            uint32_t adc_clkdiv : 16;
        } bits;
    } adc_clkdiv;
    union {
        uint32_t data;
        struct {
            uint32_t afe_spare_input : 4;
            uint32_t afe_spare_output : 4;
        } bits;
    } adc_misc;
    union {
        uint32_t data;
        struct {
            uint32_t ts_charge : 1;
            uint32_t step1 : 1;
            uint32_t step2 : 1;
            uint32_t step3 : 1;
            uint32_t step4 : 1;
            uint32_t step5 : 1;
            uint32_t step6 : 1;
            uint32_t step7 : 1;
            uint32_t step8 : 1;
            uint32_t step9 : 1;
            uint32_t step10 : 1;
            uint32_t step11 : 1;
            uint32_t step12 : 1;
            uint32_t step13 : 1;
            uint32_t step14 : 1;
            uint32_t step15 : 1;
            uint32_t step16 : 1;
        } bits;
    } stepenable;
    union {
        uint32_t data;
        struct {
            uint32_t reserved : 5;
            uint32_t xppsw_swc : 1;
            uint32_t xnnsw_swc : 1;
            uint32_t yppsw_swc : 1;
            uint32_t ynnsw_swc : 1;
            uint32_t xnpsw_swc : 1;
            uint32_t ypnsw_swc : 1;
            uint32_t wpnsw_swc : 1;
            adc_sel_rfp_t sel_rfp_swc_2_0 : 3;
            adc_sel_inm_t sel_inm_swc_3_0 : 4;
            adc_sel_inp_t sel_inp_swc_3_0 : 4;
            adc_sel_rfm_t sel_rfm_swc_1_0 : 2;
            uint32_t diff_cntrl : 1;
        } bits;
    } idleconfig;
    union {
        uint32_t data;
        struct {
            uint32_t reserved : 5;
            uint32_t xppsw_swc : 1;
            uint32_t xnnsw_swc : 1;
            uint32_t yppsw_swc : 1;
            uint32_t ynnsw_swc : 1;
            uint32_t xnpsw_swc : 1;
            uint32_t ypnsw_swc : 1;
            uint32_t wpnsw_swc : 1;
            uint32_t sel_rfp_swc_2_0 : 3;
            uint32_t sel_inm_swc_3_0 : 4;
            uint32_t sel_inp_swc_3_0 : 4;
            uint32_t sel_rfm_swc_1_0 : 2;
            uint32_t diff_cntrl : 1;
        } bits;
    } ts_charge_stepconfig;
    union {
        uint32_t data;
        struct {
            uint32_t open_delay : 18;
        } bits;
    } ts_charge_delay;
    adc_stepconfig_t stepconfig1;
    adc_stepdelay_t stepdelay1;
    adc_stepconfig_t stepconfig2;
    adc_stepdelay_t stepdelay2;
    adc_stepconfig_t stepconfig3;
    adc_stepdelay_t stepdelay3;
    adc_stepconfig_t stepconfig4;
    adc_stepdelay_t stepdelay4;
    adc_stepconfig_t stepconfig5;
    adc_stepdelay_t stepdelay5;
    adc_stepconfig_t stepconfig6;
    adc_stepdelay_t stepdelay6;
    adc_stepconfig_t stepconfig7;
    adc_stepdelay_t stepdelay7;
    adc_stepconfig_t stepconfig8;
    adc_stepdelay_t stepdelay8;
    adc_stepconfig_t stepconfig9;
    adc_stepdelay_t stepdelay9;
    adc_stepconfig_t stepconfig10;
    adc_stepdelay_t stepdelay10;
    adc_stepconfig_t stepconfig11;
    adc_stepdelay_t stepdelay11;
    adc_stepconfig_t stepconfig12;
    adc_stepdelay_t stepdelay12;
    adc_stepconfig_t stepconfig13;
    adc_stepdelay_t stepdelay13;
    adc_stepconfig_t stepconfig14;
    adc_stepdelay_t stepdelay14;
    adc_stepconfig_t stepconfig15;
    adc_stepdelay_t stepdelay15;
    adc_stepconfig_t stepconfig16;
    adc_stepdelay_t stepdelay16;
    adc_fifocount_t fifo0count;
    adc_fifothreshold_t fifo0threshold;
    adc_dmareq_t dma0req;
    adc_fifocount_t fifo1count;
    adc_fifothreshold_t fifo1threshold;
    adc_dmareq_t dma1req;
    adc_fifodata_t fifo0data;
    adc_fifodata_t fifo1data;
} adc_registers_t;

typedef struct _controlmodule_registers {
    union {
        uint32_t data;
        struct {
            uint32_t ip_rev_minor : 6;
            uint32_t ip_rev_custom : 2;
            uint32_t ip_rev_major : 3;
            uint32_t ip_rev_rtl : 5;
            uint32_t ip_rev_func : 12;
            uint32_t reserved : 2;
            uint32_t ip_rev_scheme : 2;
        } bits;
    } control_revision;
    uint32_t ip_hwinfo;
    uint8_t reserved0[8];
    union {
        uint32_t data;
        struct {
            uint32_t reserved0 : 1;
            uint32_t freemu : 1;
            uint32_t idlemode : 2;
            uint32_t standby : 2;
            uint32_t reserved1 : 26;
        } bits;
    } control_sysconfig;
    uint8_t reserved1[0x2c];
    union {
        uint32_t data;
        struct {
            uint32_t sysboot0 : 8;
            uint32_t devtype : 3;
            uint32_t reserved0 : 5;
            uint32_t bw : 1;
            uint32_t waiten : 1;
            uint32_t admux : 2;
            uint32_t testmd : 2;
            uint32_t sysboot1 : 2;
            uint32_t reserved1 : 7;
        } bits;
    } control_status;
    uint8_t reserved2[0xcc];
    union {
        uint32_t data;
        struct {
            uint32_t pagesize : 3;
            uint32_t ebank : 1;
            uint32_t ibank : 3;
            uint32_t rowsize : 3;
            uint32_t cl : 4;
            uint32_t narrow_mode : 2;
            uint32_t cwl : 2;
            uint32_t sdram_drive : 2;
            uint32_t reserved0 : 1;
            uint32_t dyn_odt : 2;
            uint32_t ddr2_ddqs : 1;
            uint32_t ddr_term : 3;
            uint32_t ibank_pos : 2;
            uint32_t sdram_type : 3;
        } bits;
    } control_emif_sdram_config;
    uint8_t reserved3[0x314];
    union {
        uint32_t data;
        struct {
            uint32_t reserved0 : 16;
            uint32_t vset : 10;
            uint32_t reserved1 : 6;
        } bits;
    } core_sldo_ctrl;
    union {
        uint32_t data;
        struct {
            uint32_t reserved0 : 16;
            uint32_t vset : 10;
            uint32_t reserved1 : 6;
        } bits;
    } mpu_sldo_ctrl;
    uint8_t reserved4[0x14];
    union {
        uint32_t data;
        struct {
            uint32_t clkdivopp50_en : 1;
            uint32_t reserved0 : 31;
        } bits;
    } clk32divratio_ctrl;
    union {
        uint32_t data;
        struct {
            uint32_t tshut : 1;
            uint32_t ecoz : 1;
            uint32_t contcov : 1;
            uint32_t clrz : 1;
            uint32_t soc : 1;
            uint32_t tempsoff : 1;
            uint32_t bgroff : 1;
            uint32_t cbiassel : 1;
            uint32_t dtemp : 8;
            uint32_t reserved0 : 16;
        } bits;
    } bandgap_ctrl;
    union {
        uint32_t data;
        struct {
            uint32_t dtrtempsc : 8;
            uint32_t dtrtemps : 8;
            uint32_t dtrbgapv : 8;
            uint32_t dtrbgapc : 8;
        } bits;
    } bandgap_trim;
    uint8_t reserved5[0x8];
    union {
        uint32_t data;
        struct {
            uint32_t mpu_dpll_clkinpulow_set : 1;
            uint32_t disp_pll_clkinplulow_sel : 1;
            uint32_t ddr_pll_clkinpulow_sel : 1;
            uint32_t reserved0 : 29;
        } bits;
    } pll_clkinplulow;
    uint8_t reserved6[0xc];
    union {
        uint32_t data;
        struct {
            uint32_t resselect : 1;
            uint32_t reserved0 : 31;
        } bits;
    } mosc_ctrl;
    uint8_t reserved7[0x4];
    union {
        uint32_t data;
        struct {
            uint32_t dscount : 16;
            uint32_t reserved0 : 1;
            uint32_t dsenable : 1;
            uint32_t reserved1 : 14;
        } bits;
    } deepsleep_ctrl;
    uint8_t reserved8[0x98];
    union {
        uint32_t data;
        struct {
            uint32_t reserved0 : 8;
            uint32_t ponout_per : 1;
            uint32_t pgoodout_per : 1;
            uint32_t reserved1 : 6;
            uint32_t ponout_disp : 1;
            uint32_t pgoodout_disp : 1;
            uint32_t reserved2 : 6;
            uint32_t ponout_ddr : 1;
            uint32_t pgoodout_ddr : 1;
            uint32_t reserved3 : 6;
        } bits;
    } dpll_pwr_sw_status;
    uint8_t reserved9[0xf0];
    union {
        uint32_t data;
        struct {
            uint32_t reserved0 : 1;
            uint32_t mgfr : 11;
            uint32_t partnum : 16;
            uint32_t devrev : 4;
        };
    } device_id;
    union {
        uint32_t data;
        struct {
            uint32_t dev_feature_bits : 32;
        } bits;
    } dev_feature;
    union {
        uint32_t data;
        struct {
            uint32_t host_arm : 2;
            uint32_t reserved0 : 2;
            uint32_t pru_icss : 2;
            uint32_t mmu : 2;
            uint32_t reserved1 : 6;
            uint32_t p1500 : 2;
            uint32_t tcrd0 : 2;
            uint32_t tcwr0 : 2;
            uint32_t tcrd1 : 2;
            uint32_t tcwr1 : 2;
            uint32_t tcrd2 : 2;
            uint32_t tcwr2 : 2;
            uint32_t reserved2 : 4;
        } bits;
    } init_priority_0;
    union {
        uint32_t data;
        struct {
            uint32_t cpsw : 2;
            uint32_t reserved0 : 2;
            uint32_t usb_dma : 2;
            uint32_t usb_qmgr : 2;
            uint32_t reserved1 : 8;
            uint32_t reserved2 : 2;
            uint32_t reserved3 : 2;
            uint32_t sgx : 2;
            uint32_t lcd : 2;
            uint32_t debug : 2;
            uint32_t reserved4 : 6;
        } bits;
    } init_priority_1;
    uint8_t reserved10[0x4];
    union {
        uint32_t data;
        struct {
            uint32_t tc0dbs : 2;
            uint32_t tc1dbs : 2;
            uint32_t tc2dbs : 2;
            uint32_t reserved0 : 26;
        };
    } tptc_cfg;
    uint8_t reserved11[0x8];
    cm_usb_ctrl_t usb_ctrl0;
    cm_usb_sts_t usb_sts0;
    cm_usb_ctrl_t usb_ctrl1;
    cm_usb_sts_t usb_sts1;
    cm_mac_id_lo_t mac_id0_lo;
    cm_mac_id_hi_t mac_id0_hi;
    cm_mac_id_lo_t mac_id1_lo;
    cm_mac_id_hi_t mac_id1_hi;
    uint8_t reserved12[0x4];


    /*union {
        uint32_t data;
        struct {

        } bits;
    } name;*/
    // todo
} controlmodule_registers_t;

#pragma pack(pop)
