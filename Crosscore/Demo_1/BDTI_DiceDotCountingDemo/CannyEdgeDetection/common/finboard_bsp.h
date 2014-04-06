//----------------------------------------------------------------
//      _____
//     /     \
//    /____   \____
//   / \===\   \==/
//  /___\===\___\/  AVNET
//       \======/
//        \====/
//---------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2013 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------
//
// Create Date:         Feb 26, 2013
// Project Name:        FinBoard : Avnet's BF609 Embedded Vision Starter Kit
// Module Name:         finboard_bsp.h
// Target Devices:      BF609
// Hardware Boards:     FinBoard
//
// Tool versions:       CCES 1.0.1.1
//
// Description:         This file provides the board support package (BSP)
//                      for devices specific to the BF609 Embedded Vision Starter Kit
//                      Including:
//                      - PC9546 I2C Multiplexer
//                      - MAX77387 LED Drivers
//                      - IDT5V9885 Clock Synthesizer
//                      The BF609 EVSK is hard-wired for 24 bit RGB mode.
//                      This file provides an alternate configuration for the ADV7511
//                      allowing to use the 16 bit YCbCr 4:2:2 mode with this hardware.
//
// Dependencies:        The I2C transactions in this code depend on the following library:
//                      - drivers\twi\adi_twi.h
//
// Revision:            Feb 26, 2013: 1.00 Initial Release
//
//----------------------------------------------------------------

#ifndef __FINBOARD_BSP_H__
#define __FINBOARD_BSP_H__

#include <drivers\twi\adi_twi.h> // for uint8_t/uint32_t data types

// use this define to identify all code specific to BF609 EVSK
#define FINBOARD

// use this define to display all BF609 EVSK specific I2C transactions
//#define FINBOARD_I2C_VERBOSE
//#define FINBOARD_I2C_READBACK_VERIFICATION


// BF609 EVSK specific I2C devices
#define FINBOARD_I2C_MUX_ADDR	  (0x70u) /* device PCA9540  (U26) at r/w address 0xE0/0xE1 */
#define FINBOARD_LED_DRV1_ADDR  (0x4Au) /* device MAX77387 (U22) at r/w address 0x94/0x95 */
#define FINBOARD_LED_DRV2_ADDR  (0x4Au) /* device MAX77387 (U25) at r/w address 0x94/0x95 */
#define FINBOARD_CLK_SYNTH_ADDR (0x6Au) /* device IDT5V9985 (U3) at r/w address 0xD4/0xD5 */

// I2C Multiplexer control works
#define FINBOARD_I2C_MUX_DISABLED (0x00u)
#define FINBOARD_I2C_MUX_CHANNEL0 (0x04u)
#define FINBOARD_I2C_MUX_CHANNEL1 (0x05u)

// I2C config structure
typedef struct {
    uint8_t Register;
    uint8_t Value;
} FINBOARD_I2C_CONFIG;

//
// I2C routines
//

uint32_t FINBOARD_I2C_Mux_Write( uint8_t twiAddr, uint8_t deviceAddr, uint8_t muxData );
uint32_t FINBOARD_I2C_Mux_Read( uint8_t twiAddr, uint8_t deviceAddr, uint8_t *pMuxData );
uint32_t FINBOARD_I2C_Write( uint8_t twiAddr, uint8_t deviceAddr, uint8_t regAddr, uint8_t regData );
uint32_t FINBOARD_I2C_Read( uint8_t twiAddr, uint8_t deviceAddr, uint8_t regAddr, uint8_t *pRegData );
void FINBOARD_I2C_Scan(void);
void FINBOARD_I2C_Configuration( uint8_t twiAddr, uint8_t deviceAddr, FINBOARD_I2C_CONFIG *pConfigSeq, uint32_t configLen );

//
// ADV7511 Fix
//

void FINBOARD_ADV7511_16bit_Mode(void);

//
// I2C MUX routines
//

uint32_t FINBOARD_I2C_Mux_Write( uint8_t twiAddr, uint8_t deviceAddr, uint8_t muxData );
uint32_t FINBOARD_I2C_Mux_Read( uint8_t twiAddr, uint8_t deviceAddr, uint8_t *pMuxData );

//
// LED Drivers Init/Config
//

void FINBOARD_LED_Drivers_Init(void);
void FINBOARD_LED_Drivers_Config( uint8_t intensity );
// intensity can be 0-7, corresponding to following settings
//    0 : TORCH_EN = OFF
//    1 : TORCH_EN = ON, ITORCH[5:0] = 000000 =   3.9mA
//    2 : TORCH_EN = ON, ITORCH[5:0] = 000100 =  19.5mA
//    3 : TORCH_EN = ON, ITORCH[5:0] = 001000 =  35.1mA
//    4 : TORCH_EN = ON, ITORCH[5:0] = 001100 =  50.7mA
//    5 : TORCH_EN = ON, ITORCH[5:0] = 010000 =  66.4mA
//    6 : TORCH_EN = ON, ITORCH[5:0] = 010100 =  82.0mA
//    7 : TORCH_EN = ON, ITORCH[5:0] = 011000 =  97.6mA

//
// Video Clock Synthesizer Config
//

uint32_t FINBOARD_CLK_Synth_Write( uint8_t regAddr, uint8_t regData );
uint32_t FINBOARD_CLK_Synth_Read( uint8_t regAddr, uint8_t *pRegData );
uint32_t FINBOARD_CLK_Synth_Save( void );
uint32_t FINBOARD_CLK_Synth_Restore( void );
void FINBOARD_CLK_Synth_Dump( void );
uint32_t FINBOARD_CLK_Synth_Config_OUT4_74_25_MHz( void );
uint32_t FINBOARD_CLK_Synth_Config_OUT4_27_00_MHz( void );

#endif // __FINBOARD_BSP_H__
