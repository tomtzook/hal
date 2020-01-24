#ifndef BBBIO_CONSTS_H
#define BBBIO_CONSTS_H

/***********************************************************************\
 * GPIO
\***********************************************************************/

#define BBBIO_GPIO_MODULES_COUNT    (4)
#define BBBIO_GPIO_HEADERS_COUNT    (2)
#define BBBIO_GPIO_HEADER_PIN_COUNT (46)

#define BBBIO_GPIO0_ADDR	        (0x44e07000)
#define BBBIO_GPIO1_ADDR 	        (0x4804c000)
#define BBBIO_GPIO2_ADDR 	        (0x481ac000)
#define BBBIO_GPIO3_ADDR 	        (0x481ae000)
#define BBBIO_GPIO_LEN 	            (0x1000)

#define BBBIO_GPIO_OE			    (0x134)
#define BBBIO_GPIO_DATAIN 		    (0x138)
#define BBBIO_GPIO_DATAOUT 		    (0x13c)
#define BBBIO_GPIO_CLEARDATAOUT		(0x190)
#define BBBIO_GPIO_SETDATAOUT		(0x194)

#define BBBIO_GPIO_CTRL			    (0x130)
#define BBBIO_GPIO_DEBOUNCENABLE	(0x150)
#define BBBIO_GPIO_DEBOUNCINGTIME	(0x154)


/***********************************************************************\
 * PWMSS
\***********************************************************************/

#define BBBIO_PWMSS_MODULES_COUNT      (3)
#define BBBIO_PWMSS_MODULE_PIN_COUNT   (2)

#define BBBIO_PWMSS0_ADDR	        (0x48300000)
#define BBBIO_PWMSS1_ADDR	        (0x48302000)
#define BBBIO_PWMSS2_ADDR	        (0x48304000)
#define BBBIO_PWMSS_LEN		        (0x1000)

#define BBBIO_PWMSS_EPWM_OFFSET     (0x200)

#define BBBIO_PWMSS_CTRL	        (0x664)

#define BBBIO_EPWM_TBCTL	        (0x0)
#define BBBIO_EPWM_TBPRD	        (0xa)
#define BBBIO_EPWM_TBCNT	        (0x8)

#define BBBIO_TBCTL_CTRMODE_UP      (0x0)
#define BBBIO_TBCTL_CTRMODE_DOWN    (0x1)
#define BBBIO_TBCTL_CTRMODE_UPDOWN  (0x2)
#define BBBIO_TBCTL_CTRMODE_FREEZE  (0x3)

#define BBBIO_EPWM_CMPA	            (0x12)
#define BBBIO_EPWM_CMPB	            (0x14)

#define BBBIO_EPWM_AQCTLA	        (0x16)
#define BBBIO_EPWM_AQCTLB	        (0x18)

/***********************************************************************\
 * CONTROL MODULE
\***********************************************************************/

#define BBBIO_CM_PER_ADDR		    (0x44e00000)
#define BBBIO_CM_PER_LEN		    (0x4000)

#define BBBIO_CONTROL_MODULE        (0x44e10000)
#define BBBIO_CONTROL_LEN           (0x2000)

#define BBBIO_CM_GPIO1_CLKCTRL	    (0xac)
#define BBBIO_CM_GPIO2_CLKCTRL	    (0xb0)
#define BBBIO_CM_GPIO3_CLKCTRL	    (0xb4)

#define BBBIO_CM_EPWMSS0_CLKCTRL	(0xd4)
#define BBBIO_CM_EPWMSS1_CLKCTRL	(0xcc)
#define BBBIO_CM_EPWMSS2_CLKCTRL	(0xd8)

#endif //BBBIO_CONSTS_H
