/*
 **
 ** pinmux_config.c source file generated on April 17, 2013 at 10:42:49.	
 **
 ** Copyright (C) 2000-2013 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the Pin Multiplexing configuration editor. Changes to the Pin Multiplexing
 ** configuration should be made by changing the appropriate options rather
 ** than editing this file.
 **
 ** Selected Peripherals
 ** --------------------
 ** EPPI0 (D00, D01, D02, D03, D04, D05, D06, D07, D08, D09, D10, D11, D12, D13, D14, D15, CLK, FS1, FS2)
 ** EPPI2 (D00, D01, D02, D03, D04, D05, D06, D07, CLK, FS1, FS2)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** PA00, PA01, PA02, PA03, PA04, PA05, PA06, PA07, PB00, PB01, PB02, PE07, PE08,
 ** PE09, PF00, PF01, PF02, PF03, PF04, PF05, PF06, PF07, PF08, PF09, PF10, PF11,
 ** PF12, PF13, PF14, PF15
 */

#include <sys/platform.h>
#include <stdint.h>

#define EPPI0_D00_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<0))
#define EPPI0_D01_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<2))
#define EPPI0_D02_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<4))
#define EPPI0_D03_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<6))
#define EPPI0_D04_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<8))
#define EPPI0_D05_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<10))
#define EPPI0_D06_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<12))
#define EPPI0_D07_PORTF_MUX  ((uint16_t) ((uint16_t) 1<<14))
#define EPPI0_D08_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<16))
#define EPPI0_D09_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<18))
#define EPPI0_D10_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<20))
#define EPPI0_D11_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<22))
#define EPPI0_D12_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<24))
#define EPPI0_D13_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<26))
#define EPPI0_D14_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<28))
#define EPPI0_D15_PORTF_MUX  ((uint32_t) ((uint32_t) 1<<30))
#define EPPI0_CLK_PORTE_MUX  ((uint32_t) ((uint32_t) 1<<18))
#define EPPI0_FS1_PORTE_MUX  ((uint32_t) ((uint32_t) 1<<16))
#define EPPI0_FS2_PORTE_MUX  ((uint16_t) ((uint16_t) 1<<14))
#define EPPI2_D00_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<0))
#define EPPI2_D01_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<2))
#define EPPI2_D02_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<4))
#define EPPI2_D03_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<6))
#define EPPI2_D04_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<8))
#define EPPI2_D05_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<10))
#define EPPI2_D06_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<12))
#define EPPI2_D07_PORTA_MUX  ((uint16_t) ((uint16_t) 1<<14))
#define EPPI2_CLK_PORTB_MUX  ((uint16_t) ((uint16_t) 1<<0))
#define EPPI2_FS1_PORTB_MUX  ((uint16_t) ((uint16_t) 1<<2))
#define EPPI2_FS2_PORTB_MUX  ((uint16_t) ((uint16_t) 1<<4))

#define EPPI0_D00_PORTF_FER  ((uint16_t) ((uint16_t) 1<<0))
#define EPPI0_D01_PORTF_FER  ((uint16_t) ((uint16_t) 1<<1))
#define EPPI0_D02_PORTF_FER  ((uint16_t) ((uint16_t) 1<<2))
#define EPPI0_D03_PORTF_FER  ((uint16_t) ((uint16_t) 1<<3))
#define EPPI0_D04_PORTF_FER  ((uint16_t) ((uint16_t) 1<<4))
#define EPPI0_D05_PORTF_FER  ((uint16_t) ((uint16_t) 1<<5))
#define EPPI0_D06_PORTF_FER  ((uint16_t) ((uint16_t) 1<<6))
#define EPPI0_D07_PORTF_FER  ((uint16_t) ((uint16_t) 1<<7))
#define EPPI0_D08_PORTF_FER  ((uint32_t) ((uint32_t) 1<<8))
#define EPPI0_D09_PORTF_FER  ((uint32_t) ((uint32_t) 1<<9))
#define EPPI0_D10_PORTF_FER  ((uint32_t) ((uint32_t) 1<<10))
#define EPPI0_D11_PORTF_FER  ((uint32_t) ((uint32_t) 1<<11))
#define EPPI0_D12_PORTF_FER  ((uint32_t) ((uint32_t) 1<<12))
#define EPPI0_D13_PORTF_FER  ((uint32_t) ((uint32_t) 1<<13))
#define EPPI0_D14_PORTF_FER  ((uint32_t) ((uint32_t) 1<<14))
#define EPPI0_D15_PORTF_FER  ((uint32_t) ((uint32_t) 1<<15))
#define EPPI0_CLK_PORTE_FER  ((uint32_t) ((uint32_t) 1<<9))
#define EPPI0_FS1_PORTE_FER  ((uint32_t) ((uint32_t) 1<<8))
#define EPPI0_FS2_PORTE_FER  ((uint16_t) ((uint16_t) 1<<7))
#define EPPI2_D00_PORTA_FER  ((uint16_t) ((uint16_t) 1<<0))
#define EPPI2_D01_PORTA_FER  ((uint16_t) ((uint16_t) 1<<1))
#define EPPI2_D02_PORTA_FER  ((uint16_t) ((uint16_t) 1<<2))
#define EPPI2_D03_PORTA_FER  ((uint16_t) ((uint16_t) 1<<3))
#define EPPI2_D04_PORTA_FER  ((uint16_t) ((uint16_t) 1<<4))
#define EPPI2_D05_PORTA_FER  ((uint16_t) ((uint16_t) 1<<5))
#define EPPI2_D06_PORTA_FER  ((uint16_t) ((uint16_t) 1<<6))
#define EPPI2_D07_PORTA_FER  ((uint16_t) ((uint16_t) 1<<7))
#define EPPI2_CLK_PORTB_FER  ((uint16_t) ((uint16_t) 1<<0))
#define EPPI2_FS1_PORTB_FER  ((uint16_t) ((uint16_t) 1<<1))
#define EPPI2_FS2_PORTB_FER  ((uint16_t) ((uint16_t) 1<<2))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *pREG_PORTA_MUX = EPPI2_D00_PORTA_MUX | EPPI2_D01_PORTA_MUX
     | EPPI2_D02_PORTA_MUX | EPPI2_D03_PORTA_MUX | EPPI2_D04_PORTA_MUX
     | EPPI2_D05_PORTA_MUX | EPPI2_D06_PORTA_MUX | EPPI2_D07_PORTA_MUX;
    *pREG_PORTB_MUX = EPPI2_CLK_PORTB_MUX | EPPI2_FS1_PORTB_MUX
     | EPPI2_FS2_PORTB_MUX;
    *pREG_PORTE_MUX = EPPI0_CLK_PORTE_MUX | EPPI0_FS1_PORTE_MUX
     | EPPI0_FS2_PORTE_MUX;
    *pREG_PORTF_MUX = EPPI0_D00_PORTF_MUX | EPPI0_D01_PORTF_MUX
     | EPPI0_D02_PORTF_MUX | EPPI0_D03_PORTF_MUX | EPPI0_D04_PORTF_MUX
     | EPPI0_D05_PORTF_MUX | EPPI0_D06_PORTF_MUX | EPPI0_D07_PORTF_MUX
     | EPPI0_D08_PORTF_MUX | EPPI0_D09_PORTF_MUX | EPPI0_D10_PORTF_MUX
     | EPPI0_D11_PORTF_MUX | EPPI0_D12_PORTF_MUX | EPPI0_D13_PORTF_MUX
     | EPPI0_D14_PORTF_MUX | EPPI0_D15_PORTF_MUX;

    /* PORTx_FER registers */
    *pREG_PORTA_FER = EPPI2_D00_PORTA_FER | EPPI2_D01_PORTA_FER
     | EPPI2_D02_PORTA_FER | EPPI2_D03_PORTA_FER | EPPI2_D04_PORTA_FER
     | EPPI2_D05_PORTA_FER | EPPI2_D06_PORTA_FER | EPPI2_D07_PORTA_FER;
    *pREG_PORTB_FER = EPPI2_CLK_PORTB_FER | EPPI2_FS1_PORTB_FER
     | EPPI2_FS2_PORTB_FER;
    *pREG_PORTE_FER = EPPI0_CLK_PORTE_FER | EPPI0_FS1_PORTE_FER
     | EPPI0_FS2_PORTE_FER;
    *pREG_PORTF_FER = EPPI0_D00_PORTF_FER | EPPI0_D01_PORTF_FER
     | EPPI0_D02_PORTF_FER | EPPI0_D03_PORTF_FER | EPPI0_D04_PORTF_FER
     | EPPI0_D05_PORTF_FER | EPPI0_D06_PORTF_FER | EPPI0_D07_PORTF_FER
     | EPPI0_D08_PORTF_FER | EPPI0_D09_PORTF_FER | EPPI0_D10_PORTF_FER
     | EPPI0_D11_PORTF_FER | EPPI0_D12_PORTF_FER | EPPI0_D13_PORTF_FER
     | EPPI0_D14_PORTF_FER | EPPI0_D15_PORTF_FER;
    return 0;
}

