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
// Module Name:         finboard_bsp.c
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

#include <stdio.h>
#include <string.h>

#include "finboard_bsp.h"

#include <drivers\twi\adi_twi.h>

/* TWI settings */
#define TWI_ADDRESS   (0x0u)
#define TWI_PRESCALE  (8u)
#define TWI_BITRATE   (100u)
#define TWI_DUTYCYCLE (50u)

#define BUFFER_SIZE (32u)

/* TWI hDevice handle */
static ADI_TWI_HANDLE hDevice;

/* TWI data buffer */
static uint8_t twiBuffer[BUFFER_SIZE];

/* TWI device memory */
static uint8_t deviceMemory[ADI_TWI_MEMORY_SIZE];


/* disable misra diagnostics as necessary */
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_8_7:"Objects shall be defined at block scope")
#pragma diag(suppress:misra_rule_17_4:"Array indexing shall be the only allowed form of pointer arithmetic")
#endif /* _MISRA_RULES */

/************************ LED Driver Information********************************

    ~ means the signal is active low

    Please see the BF609 EVSK hardware manual for more information on using
    LED Drivers

********************************************************************************/

/* ADV7511 fix for supporting 16bit YCbCr 4:2:2 mode on BF609 EVSK */

FINBOARD_I2C_CONFIG Adv7511Fix[] =
{
  { 0x16u, 0xECu }, // Change Input Color Depth from '11' (8 bits) to '10' (12 bits)
  { 0x48u, 0x08u }  // Change Video Input Justification from '01' (left justified) to '01' (right justified)
};

/* LED Driver I2C settings */

FINBOARD_I2C_CONFIG InitLedDriver[] =
{
  { 0x08u, 0xC0u }, // MODE_SEL
					//    TORCH_EN_PD = 1 (enable pull-down on TORCH_EN pin)
					//    FLASH_STB_PD = 1 (enable pull-down on FLASH_STB pin)
					//    TORCH_MODE = 000 (torch mode disabled)
					//    FLASH_MODE = 000 (flash mode disabled)
  { 0x15u, 0xC0u }, // DCDC_CNTL1
                    //    OVP_TH[1:0] = 11 (5.4V)
                    //    DCDC_MODE = 00 (adaptive mode)
  { 0x0Cu, 0x44u }, // TORCH_RAMP_SEL
                    //    TORCH_RU[1:0] = 262.152ms
                    //    TORCH_RD[1:0] = 262.152ms
  { 0x0Eu, 0x80u }, // TORCH_TMR_CNTL
                    //    TORCH_TMR_CNTL = 1 (timer mode disabled)
  { 0x10u, 0x00u }, // MAXFLASH1 (default value)
                    //    MAXFLASH_HYS[2:0] = 000
                    //    MAXFLASH_TH[4:0] = 00000
  { 0x11u, 0x00u }, // MAXFLASH2 (default value)
                    //    LB_TMR_R[3:0] = 0000
                    //    LB_TMR_F[3:0] = 0000
  { 0x14u, 0x00u }, // NTC (default value)
  { 0x16u, 0x04u }, // DCDC_CNTL2
                    //    DCDC_GAIN = 0
                    //    4MHz Auto Adjust, 3.125% D.C., Skip Mode
  { 0x17u, 0x00u }, // DCDC_LIM
  { 0x06u, 0x80u }, // ITORCH1
                    //    TORCH1_EN = 1 (FLED1 enabled in torch mode)
                    //    TORCH1[5:0] = 000000 (0mA per LED x2)
                    //    TORCH1_DIM = 0 (DAC Mode)
  { 0x07u, 0x80u }, // ITORCH2
                    //    TORCH2_EN = 1 (FLED2 enabled in torch mode)
                    //    TORCH2[5:0] = 000000 (0mA per LED x2)
                    //    TORCH2_DIM = 0 (DAC Mode)
  { 0x15u, 0xC1u }, // DCDC_CNTL1
                    //    OVP_TH[1:0] = 11 (5.4V)
                    //    DCDC_MODE = 01 (prebiased adaptive mode)
  { 0x08u, 0xF8u }  // MODE_SEL
                    //    TORCH_EN_PD = 1 (enable pull-down on TORCH_EN pin)
                    //    FLASH_STB_PD = 1 (enable pull-down on FLASH_STB pin)
                    //    TORCH_MODE = 111 (torch mode enabled regardless of logic inputs)
                    //    FLASH_MODE = 000 (flash mode disabled)
};

uint8_t IlluminationValues[8] =
{
  // Level 0 - OFF
  0x00u, // ITORCH#
         //    TORCH#_EN = 0 (FLED1 disabled in torch mode)
         //    TORCH#[5:0] = 000000 (0mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
  // Level 1
  0x80u, // ITORCH#
         //    TORCH#_EN = 1 (FLED1 enabled in torch mode)
         //    TORCH#[5:0] = 000000 (3.9mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
  // Level 2
  0x88u, // ITORCH#
         //    TORCH#_EN = 1 (FLED1 enabled in torch mode)
         //    TORCH#[5:0] = 000100 (19.5mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
  // Level 3
  0x90u, // ITORCH#
         //    TORCH#_EN = 1 (FLED1 enabled in torch mode)
         //    TORCH#[5:0] = 001000 (35.1mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
  // Level 4
  0x98u, // ITORCH#
         //    TORCH#_EN = 1 (FLED1 enabled in torch mode)
         //    TORCH#[5:0] = 001100 (50.7mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
  // Level 5
  0xA0u, // ITORCH#
         //    TORCH#_EN = 1 (FLED1 enabled in torch mode)
         //    TORCH#[5:0] = 010000 (66.4mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
  // Level 6
  0xA8u, // ITORCH#
         //    TORCH#_EN = 1 (FLED1 enabled in torch mode)
         //    TORCH#[5:0] = 010100 (82.0mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
  // Level 7
  0xB0u // ITORCH#
         //    TORCH#_EN = 1 (FLED1 enabled in torch mode)
         //    TORCH#[5:0] = 011100 (97.6mA per LED x2)
         //    TORCH#_DIM = 0 (DAC Mode)
};

FINBOARD_I2C_CONFIG ConfigLedDriver[] =
{
  { 0x08u, 0xC0u }, // MODE_SEL
					//    TORCH_EN_PD = 1 (enable pull-down on TORCH_EN pin)
					//    FLASH_STB_PD = 1 (enable pull-down on FLASH_STB pin)
					//    TORCH_MODE = 000 (torch mode disabled)
					//    FLASH_MODE = 000 (flash mode disabled)
  { 0x06u, 0xA4u }, // ITORCH1
	                //    TORCH1_EN = 1 (FLED1 enabled in torch mode)
					//    TORCH1[5:0] = 010010 (70mA per LED x2)
                    //    TORCH1_DIM = 0 (DAC Mode)
  { 0x07u, 0xA4u }, // ITORCH2
                    //    TORCH2_EN = 1 (FLED2 enabled in torch mode)
					//    TORCH2[5:0] = 010010 (70mA per LED x2)
                    //    TORCH2_DIM = 0 (DAC Mode)
  { 0x08u, 0xF8u }  // MODE_SEL
                    //    TORCH_EN_PD = 1 (enable pull-down on TORCH_EN pin)
                    //    FLASH_STB_PD = 1 (enable pull-down on FLASH_STB pin)
                    //    TORCH_MODE = 111 (torch mode enabled regardless of logic inputs)
                    //    FLASH_MODE = 000 (flash mode disabled)
};


/* Clock Synthezerr I2C settings */


// BF609 EVSK 480P60 Settings:
//    OUT1 = 25.00 MHz (DSP)
//    OUT2 = 48.00 MHz (USB)
//    OUT3 = 24.00 MHz (Camera Reference Clock)
//    OUT4 = 74.25 MHz (Video Clock for 720P60 resolution)
//    OUT5 = 50.00 MHz (Ethernet)
//    OUT6 =  0.05 MHz (unused)
FINBOARD_I2C_CONFIG CLKSynt720P60Config[] =
{
  // Read-Only
	{ 0x00, 0x00 },
	{ 0x01, 0x00 },
	{ 0x02, 0x00 },
	{ 0x03, 0x00 },
  // Manual Frequency Control Mode
	{ 0x04, 0x00 },
	{ 0x05, 0xFF },
  // Crystal Driver Strength
	{ 0x06, 0x30 },
  // Crystal Load Cap
	{ 0x07, 0x00 },
  // PLL0 Loop Filtering Setting
	{ 0x08, 0x15 },
	{ 0x09, 0x00 },
	{ 0x0A, 0x00 },
	{ 0x0B, 0x00 },
	{ 0x0C, 0xFF },
	{ 0x0D, 0x00 },
	{ 0x0E, 0x00 },
	{ 0x0F, 0x00 },
  // PLL0 Input Divider D0 Setting
	{ 0x10, 0x19 },
	{ 0x11, 0x00 },
	{ 0x12, 0x00 },
	{ 0x13, 0x00 },
  // PLL0 Multiplier Setting
	{ 0x14, 0x2E },
	{ 0x15, 0x00 },
	{ 0x16, 0x00 },
	{ 0x17, 0x00 },
	{ 0x18, 0x30 },
	{ 0x19, 0x00 },
	{ 0x1A, 0x00 },
	{ 0x1B, 0x00 },
  //
	{ 0x1C, 0x20 },
	{ 0x1D, 0x40 },
	{ 0x1E, 0x00 },
	{ 0x1F, 0x10 },
  // PLL1 Loop Filtering Setting
	{ 0x20, 0x3E },
	{ 0x21, 0x00 },
	{ 0x22, 0x00 },
	{ 0x23, 0x00 },
	{ 0x24, 0x74 },
	{ 0x25, 0x00 },
	{ 0x26, 0x00 },
	{ 0x27, 0x00 },
  // PLL1 Input Divider D0 Setting
	{ 0x28, 0x0A },
	{ 0x29, 0x00 },
	{ 0x2A, 0x00 },
	{ 0x2B, 0x00 },
  // PLL1 Multiplier Setting
	{ 0x2C, 0x93 },
	{ 0x2D, 0x00 },
	{ 0x2E, 0x00 },
	{ 0x2F, 0x00 },
	{ 0x30, 0x20 },
	{ 0x31, 0x00 },
	{ 0x32, 0x00 },
	{ 0x33, 0x00 },
  //
	{ 0x34, 0x47 },
	{ 0x35, 0xB9 },
	{ 0x36, 0x00 },
	{ 0x37, 0x00 },
  // PLL2 Loop Filtering Setting
	{ 0x38, 0x63 },
	{ 0x39, 0x00 },
	{ 0x3A, 0x00 },
	{ 0x3B, 0x00 },
	{ 0x3C, 0x22 },
	{ 0x3D, 0x00 },
	{ 0x3E, 0x00 },
	{ 0x3F, 0x00 },
  // PLL2 Input Divider D0 Setting
	{ 0x40, 0x01 },
	{ 0x41, 0x00 },
	{ 0x42, 0x00 },
	{ 0x43, 0x00 },
  // PLL2 Multiplier Setting
	{ 0x44, 0x30 },
	{ 0x45, 0x00 },
	{ 0x46, 0x00 },
	{ 0x47, 0x00 },
	{ 0x48, 0x00 },
	{ 0x49, 0x00 },
	{ 0x4A, 0x00 },
	{ 0x4B, 0x00 },
  // OUT2 Configuration
	{ 0x4C, 0x10 },
	{ 0x4D, 0xBD },
	{ 0x4E, 0x01 },
	{ 0x4F, 0x00 },
  // OUT3 Configuration
	{ 0x50, 0x10 },
	{ 0x51, 0xBA },
	{ 0x52, 0x03 },
	{ 0x53, 0x00 },
  // OUT4 Configuration
	{ 0x54, 0x10 },
	{ 0x55, 0xBF },
	{ 0x56, 0x00 },
	{ 0x57, 0x00 },
  // OUT5 Configuration
	{ 0x58, 0x10 },
	{ 0x59, 0xBB },
	{ 0x5A, 0x02 },
	{ 0x5B, 0x00 },
  // OUT6 Configuration
	{ 0x5C, 0x13 },
	{ 0x5D, 0xBB },
	{ 0x5E, 0xEF },
	{ 0x5F, 0x00 },
  // PLL0 Spread Spectrum Settings
	{ 0x60, 0x00 },
	{ 0x61, 0x00 },
	{ 0x62, 0x00 },
	{ 0x63, 0x00 },
	{ 0x64, 0x00 },
	{ 0x65, 0x00 },
	{ 0x66, 0x00 },
	{ 0x67, 0x00 },
  // PLL1 Spread Spectrum Settings
	{ 0x68, 0x00 },
	{ 0x69, 0x00 },
	{ 0x6A, 0x00 },
	{ 0x6B, 0x00 },
	{ 0x6C, 0x00 },
	{ 0x6D, 0x00 },
	{ 0x6E, 0x00 },
	{ 0x6F, 0x00 }
};

// BF609 EVSK 480P60 Settings:
//    OUT1 = 25.00 MHz (DSP)
//    OUT2 = 48.00 MHz (USB)
//    OUT3 = 24.00 MHz (Camera Reference Clock)
//    OUT4 = 27.00 MHz (Video Clock for 480P60 resolution)
//    OUT5 = 50.00 MHz (Ethernet)
//    OUT6 =  0.05 MHz (unused)
FINBOARD_I2C_CONFIG CLKSynt480P60Config[] =
{
  // Read-Only
	{ 0x00, 0x00 },
	{ 0x01, 0x00 },
	{ 0x02, 0x00 },
	{ 0x03, 0x00 },
  // Manual Frequency Control Mode
	{ 0x04, 0x00 },
	{ 0x05, 0xFF },
  // Crystal Driver Strength
	{ 0x06, 0x30 },
  // Crystal Load Cap
	{ 0x07, 0x00 },
  // PLL0 Loop Filtering Setting
	{ 0x08, 0x1F },
	{ 0x09, 0x00 },
	{ 0x0A, 0x00 },
	{ 0x0B, 0x00 },
	{ 0x0C, 0xFF },
	{ 0x0D, 0x00 },
	{ 0x0E, 0x00 },
	{ 0x0F, 0x00 },
  // PLL0 Input Divider D0 Setting
	{ 0x10, 0x19 },
	{ 0x11, 0x00 },
	{ 0x12, 0x00 },
	{ 0x13, 0x00 },
  // PLL0 Multiplier Setting
	{ 0x14, 0xAE },
	{ 0x15, 0x00 },
	{ 0x16, 0x00 },
	{ 0x17, 0x00 },
	{ 0x18, 0x31 },
	{ 0x19, 0x00 },
	{ 0x1A, 0x00 },
	{ 0x1B, 0x00 },
  //
	{ 0x1C, 0x20 },
	{ 0x1D, 0x40 },
	{ 0x1E, 0x00 },
	{ 0x1F, 0x10 },
  // PLL1 Loop Filtering Setting
	{ 0x20, 0x41 },
	{ 0x21, 0x00 },
	{ 0x22, 0x00 },
	{ 0x23, 0x00 },
	{ 0x24, 0xB5 },
	{ 0x25, 0x00 },
	{ 0x26, 0x00 },
	{ 0x27, 0x00 },
  // PLL1 Input Divider D0 Setting
	{ 0x28, 0x01 },
	{ 0x29, 0x00 },
	{ 0x2A, 0x00 },
	{ 0x2B, 0x00 },
  // PLL1 Multiplier Setting
	{ 0x2C, 0x02 },
	{ 0x2D, 0x00 },
	{ 0x2E, 0x00 },
	{ 0x2F, 0x00 },
	{ 0x30, 0x00 },
	{ 0x31, 0x00 },
	{ 0x32, 0x00 },
	{ 0x33, 0x00 },
  //
	{ 0x34, 0x47 },
	{ 0x35, 0xA5 },
	{ 0x36, 0x00 },
	{ 0x37, 0x00 },
  // PLL2 Loop Filtering Setting
	{ 0x38, 0x63 },
	{ 0x39, 0x00 },
	{ 0x3A, 0x00 },
	{ 0x3B, 0x00 },
	{ 0x3C, 0x22 },
	{ 0x3D, 0x00 },
	{ 0x3E, 0x00 },
	{ 0x3F, 0x00 },
  // PLL2 Input Divider D0 Setting
	{ 0x40, 0x00 },
	{ 0x41, 0x00 },
	{ 0x42, 0x00 },
	{ 0x43, 0x00 },
  // PLL2 Multiplier Setting
	{ 0x44, 0x00 },
	{ 0x45, 0x00 },
	{ 0x46, 0x00 },
	{ 0x47, 0x00 },
	{ 0x48, 0x00 },
	{ 0x49, 0x00 },
	{ 0x4A, 0x00 },
	{ 0x4B, 0x00 },
  // OUT2 Configuration
	{ 0x4C, 0x10 },
	{ 0x4D, 0xBF },
	{ 0x4E, 0x01 },
	{ 0x4F, 0x00 },
  // OUT3 Configuration
	{ 0x50, 0x10 },
	{ 0x51, 0xB3 },
	{ 0x52, 0x04 },
	{ 0x53, 0x00 },
  // OUT4 Configuration
	{ 0x54, 0x10 },
	{ 0x55, 0xBB },
	{ 0x56, 0x03 },
	{ 0x57, 0x00 },
  // OUT5 Configuration
	{ 0x58, 0x10 },
	{ 0x59, 0xB9 },
	{ 0x5A, 0x02 },
	{ 0x5B, 0x00 },
  // OUT6 Configuration
	{ 0x5C, 0x13 },
	{ 0x5D, 0xBB },
	{ 0x5E, 0xF9 },
	{ 0x5F, 0x00 },
  // PLL0 Spread Spectrum Settings
	{ 0x60, 0x00 },
	{ 0x61, 0x00 },
	{ 0x62, 0x00 },
	{ 0x63, 0x00 },
	{ 0x64, 0x00 },
	{ 0x65, 0x00 },
	{ 0x66, 0x00 },
	{ 0x67, 0x00 },
  // PLL1 Spread Spectrum Settings
	{ 0x68, 0x00 },
	{ 0x69, 0x00 },
	{ 0x6A, 0x00 },
	{ 0x6B, 0x00 },
	{ 0x6C, 0x00 },
	{ 0x6D, 0x00 },
	{ 0x6E, 0x00 },
	{ 0x6F, 0x00 }
};

#if defined(ADI_DEBUG)
#include <stdio.h>
#define CHECK_RESULT(result, message) \
    do { \
        if((result) != ADI_TWI_SUCCESS) \
        { \
            printf((message)); \
            printf("\n"); \
        } \
    } while (0)  /* do-while-zero needed for Misra Rule 19.4 */
#else
#define CHECK_RESULT(result, message)
#endif

//
// I2C routines
//

uint32_t FINBOARD_I2C_Write( uint8_t twiAddr, uint8_t deviceAddr, uint8_t regAddr, uint8_t regData )
{
    ADI_TWI_RESULT result;
    uint32_t bDetected = 0;

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* read register value */
		twiBuffer[0] = regAddr;
		twiBuffer[1] = regData;
		result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)2, false);
		CHECK_RESULT(result, "adi_twi_Write failed");
#if defined(FINBOARD_I2C_VERBOSE)
		printf( "\tI2C Write : TWI%d, Device=0x%02X, Register=0x%02X, Data=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr, regData );
#endif
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C Write : TWI%d, Device=0x%02X, Register=0x%02X, Data=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr, regData );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return 1;
}

uint32_t FINBOARD_I2C_Read( uint8_t twiAddr, uint8_t deviceAddr, uint8_t regAddr, uint8_t *pRegData )
{
    ADI_TWI_RESULT result;
    uint32_t bSuccess = 0;

    *pRegData = 0x00;

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* read register value */
		twiBuffer[0] = regAddr;
		result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)1, true);
		CHECK_RESULT(result, "adi_twi_Write failed");
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C Write : TWI%d, Device=0x%02X, Register=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr );
		result = adi_twi_Read(hDevice, twiBuffer, (uint32_t)1, false);
		CHECK_RESULT(result, "adi_twi_Read failed");
		if((result) == ADI_TWI_SUCCESS)
		{
			bSuccess = 1;
			*pRegData = twiBuffer[0];
#if defined(FINBOARD_I2C_VERBOSE)
			printf( "\tI2C Read  : TWI%d, Device=0x%02X, Register=0x%02X, Data=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr, *pRegData );
#endif
		}
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C Read : TWI%d, Device=0x%02X, Register=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return bSuccess;
}

void FINBOARD_I2C_Scan(void)
{
    uint8_t deviceAddr;
    uint8_t regAddr = 0x00;
    uint8_t regData;
    uint32_t bSuccess;

    printf( "Scanning TWI %d for I2C devices\n", TWI_ADDRESS );

    for (deviceAddr=0x00; deviceAddr<0x80; deviceAddr++)
    {
    	regAddr = 0x00;
  		bSuccess = FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, regAddr, &regData );
    	if ( bSuccess == 1 )
    	{
           printf( "\tDetected Device at address 0x%02X/0x%02X\n", (deviceAddr<<1), (deviceAddr<<1)+1 );
#if 0
           for ( regAddr = 0x00; regAddr < 0xFF; regAddr++ )
           {
               if ( FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, regAddr, &regData ) == 1 )
               {
                   printf( "\t\t0x%02X => 0x%02X\n", regAddr, regData );
               }
               else
               {
                   printf( "\t\t0x%02X => error\n", regAddr );
               }
           }
           regAddr = 0xFF;
           FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, regAddr, &regData );
           printf( "\t\t0x%02X => 0x%02X\n", regAddr, regData );
#endif
#if 0
           if ( deviceAddr == 0x39 ) // ADV7511 (MAIN)
           {
           }
           if ( deviceAddr == 0x70 ) // PCA9450
           {
              FINBOARD_I2C_Mux_Read( TWI_ADDRESS, deviceAddr, &regData );
              printf( "\t\t0x%02X\n", regData );
           }
#endif
#if 0
           if ( (deviceAddr == 0x94>>1) || (deviceAddr == 0xD4>>1) ) // MAX77387 or IDT
           {
               for ( regAddr = 0x00; regAddr < 0x20; regAddr++ )
               {
                   if ( FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, regAddr, &regData ) == 1 )
                   {
                       printf( "\t\t0x%02X => 0x%02X\n", regAddr, regData );
                   }
                   else
                   {
                       printf( "\t\t0x%02X => error\n", regAddr );
                   }
               }
           }
#endif
        }
    }

}

void FINBOARD_I2C_Configuration( uint8_t twiAddr, uint8_t deviceAddr, FINBOARD_I2C_CONFIG *pConfigSeq, uint32_t configLen )
{
    uint32_t i;
    uint8_t regAddr;
    uint8_t regData;
    FINBOARD_I2C_CONFIG *pConfigReg;

	/* switch register settings */
	for (i=0u; i < configLen; i++)
	{
		pConfigReg = &pConfigSeq[i];

		regAddr = pConfigReg->Register;
		regData = pConfigReg->Value;
		FINBOARD_I2C_Write( TWI_ADDRESS, deviceAddr, regAddr, regData );

#if defined(FINBOARD_I2C_READBACK_VERIFICATION)
		FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, regAddr, &regData );
		if ( regData != pConfigReg->Value )
		{
		   printf( "\t\tFailed to write 0x%02X at 0x%02X\n", pConfigReg->Value, pConfigReg->Register );
		}
#endif
	}

}

//
// ADV7511 Fix
//

void FINBOARD_ADV7511_16bit_Mode(void)
{
   uint8_t deviceAddr;
   uint8_t regAddr;
   uint8_t regData;

   printf( "Configuring ADV7511 for 16bit YCbCr Mode\n" );

   deviceAddr=0x39; // ADV7511 (MAIN)
   FINBOARD_I2C_Configuration( TWI_ADDRESS, deviceAddr, Adv7511Fix, sizeof(Adv7511Fix)/sizeof(FINBOARD_I2C_CONFIG) );
}

//
// I2C MUX routines
//

uint32_t FINBOARD_I2C_Mux_Write( uint8_t twiAddr, uint8_t deviceAddr, uint8_t muxData )
{
    ADI_TWI_RESULT result;
    uint32_t bDetected = 0;

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* write control word */
		twiBuffer[0] = muxData;
		result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)1, false);
#if defined(FINBOARD_I2C_VERBOSE)
        printf( "\tI2C Write : TWI%d, Device=0x%02X, Control=0x%02X\n", twiAddr, (deviceAddr)<<1, muxData );
#endif
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C Mux Write : TWI%d, Device=0x%02X, Control=0x%02X\n", twiAddr, (deviceAddr)<<1, muxData );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return 1;
}

uint32_t FINBOARD_I2C_Mux_Read( uint8_t twiAddr, uint8_t deviceAddr, uint8_t *pMuxData )
{
    ADI_TWI_RESULT result;
    uint32_t bSuccess = 0;

    *pMuxData = 0x00;
    twiBuffer[0] = 0x00;

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* read control word */
		result = adi_twi_Read(hDevice, twiBuffer, (uint32_t)1, false);
		CHECK_RESULT(result, "adi_twi_Read failed");
		if((result) == ADI_TWI_SUCCESS)
		{
			bSuccess = 1;
			*pMuxData = twiBuffer[0];
#if defined(FINBOARD_I2C_VERBOSE)
            printf( "\tI2C Read  : TWI%d, Device=0x%02X, Control=0x%02X\n", twiAddr, (deviceAddr)<<1, *pMuxData );
#endif
		}
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C Mux Read : TWI%d, Device=0x%02X\n", twiAddr, (deviceAddr)<<1 );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return bSuccess;
}

//
// LED Drivers Init/Config
//

void FINBOARD_LED_Drivers_Init(void)
{
    uint8_t deviceAddr;
    uint8_t regData;

    printf( "LED Drivers Initialization\n" );
	if ( FINBOARD_I2C_Mux_Read( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, &regData ) == 0 )
	{
		printf( "\tFailed to communicate with I2C Multiplexer !\n" );
		return;
	}

	// Enable Channel 0 on I2C Multiplexer
	regData = FINBOARD_I2C_MUX_CHANNEL0;
	FINBOARD_I2C_Mux_Write( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, regData );

	// LED Driver #1 Initialization
    printf( "\tLED Driver #1 Initialization\n" );
    deviceAddr=FINBOARD_LED_DRV1_ADDR;
    // Clean any pending fault status by reading STATUS1 register
    FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, 0x02, &regData );
    printf( "\t\tSTATUS1 = 0x%02X\n", regData );
    // Initialize device in TORCH mode
    FINBOARD_I2C_Configuration( TWI_ADDRESS, deviceAddr, InitLedDriver, sizeof(InitLedDriver)/sizeof(FINBOARD_I2C_CONFIG) );

	// Enable Channel 1 on I2C Multiplexer
	regData = FINBOARD_I2C_MUX_CHANNEL1;
	FINBOARD_I2C_Mux_Write( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, regData );

	// LED Driver #2 Initialization
    printf( "\tLED Driver #2 Initialization\n" );
    deviceAddr=FINBOARD_LED_DRV2_ADDR;
    // Clean any pending fault status by reading STATUS1 register
    FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, 0x02, &regData );
    printf( "\t\tSTATUS1 = 0x%02X\n", regData );
    // Initialize device in TORCH mode
    FINBOARD_I2C_Configuration( TWI_ADDRESS, deviceAddr, InitLedDriver, sizeof(InitLedDriver)/sizeof(FINBOARD_I2C_CONFIG) );
}

void FINBOARD_LED_Drivers_Config( uint8_t intensity )
{
    uint8_t deviceAddr;
    uint8_t regData;
    int i;

#if defined(FINBOARD_SHOW_LIGHTING_IN_CONSOLE)
    printf( "LED Drivers Configuration (intensity = %d)\n", intensity );
#endif

    // Modify ConfigLedDriver data structure with desired configuration
    for ( i = 0; i < sizeof(ConfigLedDriver)/sizeof(FINBOARD_I2C_CONFIG); i++ )
    {
    	if ( (ConfigLedDriver[i].Register == 0x06) || (ConfigLedDriver[i].Register == 0x07) )
    	{
    	    ConfigLedDriver[i].Value = IlluminationValues[intensity];
    	}
    }
    for ( i = 0; i < sizeof(InitLedDriver)/sizeof(FINBOARD_I2C_CONFIG); i++ )
    {
    	if ( (InitLedDriver[i].Register == 0x06) || (InitLedDriver[i].Register == 0x07) )
    	{
    		InitLedDriver[i].Value = IlluminationValues[intensity];
    	}
    }
#if defined(FINBOARD_SHOW_LIGHTING_IN_CONSOLE)
    printf( "\tITORCH1/ITORCH2 = 0x%02X\n", IlluminationValues[intensity] );
#endif

	if ( FINBOARD_I2C_Mux_Read( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, &regData ) == 0 )
	{
		printf( "\tFailed to communicate with I2C Multiplexer !\n" );
		return;
	}

	// Enable Channel 0 on I2C Multiplexer
	regData = FINBOARD_I2C_MUX_CHANNEL0;
	FINBOARD_I2C_Mux_Write( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, regData );
	//printf( "\tI2C Write : PCA9540 <= 0x%02X\n", regData );
	//FINBOARD_I2C_Mux_Read( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, &regData );
	//printf( "\tI2C Read  : PCA9540 => 0x%02X\n", regData );

	// LED Driver #1 Configuration
#if defined(FINBOARD_SHOW_LIGHTING_IN_CONSOLE)
    printf( "\tLED Driver #1 Configuration\n" );
#endif
    deviceAddr=FINBOARD_LED_DRV1_ADDR;
    // Clean any pending fault status by reading STATUS1 register
    FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, 0x02, &regData );
#if defined(FINBOARD_SHOW_LIGHTING_IN_CONSOLE)
    printf( "\t\tSTATUS1 = 0x%02X\n", regData );
#endif
    // Configure LED Driver #1
    //FINBOARD_I2C_Configuration( TWI_ADDRESS, deviceAddr, ConfigLedDriver, sizeof(ConfigLedDriver)/sizeof(FINBOARD_I2C_CONFIG) );
    FINBOARD_I2C_Configuration( TWI_ADDRESS, deviceAddr, InitLedDriver, sizeof(InitLedDriver)/sizeof(FINBOARD_I2C_CONFIG) );

	// Enable Channel 1 on I2C Multiplexer
	regData = FINBOARD_I2C_MUX_CHANNEL1;
	FINBOARD_I2C_Mux_Write( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, regData );
	//printf( "\tI2C Write : PCA9540 <= 0x%02X\n", regData );
	//FINBOARD_I2C_Mux_Read( TWI_ADDRESS, FINBOARD_I2C_MUX_ADDR, &regData );
	//printf( "\tI2C Read  : PCA9540 => 0x%02X\n", regData );

	// LED Driver #2 Configuration
#if defined(FINBOARD_SHOW_LIGHTING_IN_CONSOLE)
    printf( "\tLED Driver #2 Configuration\n" );
#endif
    deviceAddr=FINBOARD_LED_DRV2_ADDR;
    // Clean any pending fault status by reading STATUS1 register
    FINBOARD_I2C_Read( TWI_ADDRESS, deviceAddr, 0x02, &regData );
#if defined(FINBOARD_SHOW_LIGHTING_IN_CONSOLE)
    printf( "\t\tSTATUS1 = 0x%02X\n", regData );
#endif
    // Configure LED Driver #2
    //FINBOARD_I2C_Configuration( TWI_ADDRESS, deviceAddr, ConfigLedDriver, sizeof(ConfigLedDriver)/sizeof(FINBOARD_I2C_CONFIG) );
    FINBOARD_I2C_Configuration( TWI_ADDRESS, deviceAddr, InitLedDriver, sizeof(InitLedDriver)/sizeof(FINBOARD_I2C_CONFIG) );
}

//
// Video Clock Synthesizer Config
//

uint32_t FINBOARD_CLK_Synth_Write( uint8_t regAddr, uint8_t regData )
{
	uint8_t twiAddr = TWI_ADDRESS;
	uint8_t deviceAddr = FINBOARD_CLK_SYNTH_ADDR;
    ADI_TWI_RESULT result;
    uint32_t bDetected = 0;

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* write CommandCode, register address & data */
		twiBuffer[0] = 0x00; // CommandCode = Write
		twiBuffer[1] = regAddr;
		twiBuffer[2] = regData;
		result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)3, false);
		CHECK_RESULT(result, "adi_twi_Write failed");
#if defined(FINBOARD_I2C_VERBOSE)
		printf( "\tIDT Write : TWI%d, Device=0x%02X, Command=0x00, Register=0x%02X, Data=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr, regData );
#endif
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C IDT Write : TWI%d, Device=0x%02X, Command=0x00, Register=0x%02X, Data=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr, regData );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return 1;
}

uint32_t FINBOARD_CLK_Synth_Read( uint8_t regAddr, uint8_t *pRegData )
{
	uint8_t twiAddr = TWI_ADDRESS;
	uint8_t deviceAddr = FINBOARD_CLK_SYNTH_ADDR;
    ADI_TWI_RESULT result;
    uint32_t bSuccess = 0;

    *pRegData = 0x00;

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* write CommandCode, register address */
		twiBuffer[0] = 0x00; // CommandCode = Read/Write
		twiBuffer[1] = regAddr;
		result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)2, true);
		CHECK_RESULT(result, "adi_twi_Write failed");
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C IDT Write : TWI%d, Device=0x%02X, Command=0x00, Register=0x%02X\n", twiAddr, (deviceAddr)<<1, regAddr );
        /* read IdByte, register data */
		result = adi_twi_Read(hDevice, twiBuffer, (uint32_t)2, false);
		CHECK_RESULT(result, "adi_twi_Read failed");
		if((result) == ADI_TWI_SUCCESS)
		{
			bSuccess = 1;
			*pRegData = twiBuffer[1];
#if defined(FINBOARD_I2C_VERBOSE)
			printf( "\tIDT Read  : TWI%d, Device=0x%02X, IdByte=0x%02X, Data=0x%02X\n", twiAddr, (deviceAddr)<<1, twiBuffer[0], *pRegData );
#endif
		}
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C IDT Read : TWI%d, Device=0x%02X\n", twiAddr, (deviceAddr)<<1 );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return bSuccess;
}

uint32_t FINBOARD_CLK_Synth_Save( void )
{
	uint8_t twiAddr = TWI_ADDRESS;
	uint8_t deviceAddr = FINBOARD_CLK_SYNTH_ADDR;
    ADI_TWI_RESULT result;
    uint32_t bDetected = 0;

    printf( "Saving current Clock Synthesizer settings to firmware\n");

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* read register value */
		twiBuffer[0] = 0x01; // CommandCode = Save
		result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)1, false);
		CHECK_RESULT(result, "adi_twi_Write failed");
#if defined(FINBOARD_I2C_VERBOSE)
		printf( "\tIDT Save : TWI%d, Device=0x%02X\n", twiAddr, (deviceAddr)<<1 );
#endif
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C IDT Write : TWI%d, Device=0x%02X, Command=0x01\n", twiAddr, (deviceAddr)<<1 );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return 1;
}

uint32_t FINBOARD_CLK_Synth_Restore( void )
{
	uint8_t twiAddr = TWI_ADDRESS;
	uint8_t deviceAddr = FINBOARD_CLK_SYNTH_ADDR;
    ADI_TWI_RESULT result;
    uint32_t bDetected = 0;

    printf( "Restoring Clock Synthesizer settings from firmware\n");

	result = adi_twi_Open(twiAddr, ADI_TWI_MASTER,
		deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
	CHECK_RESULT(result, "adi_twi_Open failed");

	result = adi_twi_SetHardwareAddress(hDevice, deviceAddr);
	CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

	result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
	CHECK_RESULT(result, "adi_twi_Prescale failed");

	result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
	CHECK_RESULT(result, "adi_twi_SetBitRate failed");

	result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
	CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

	{
		/* read register value */
		twiBuffer[0] = 0x02; // CommandCode = Restore
		result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)1, false);
		CHECK_RESULT(result, "adi_twi_Write failed");
#if defined(FINBOARD_I2C_VERBOSE)
		printf( "\tIDT Restore : TWI%d, Device=0x%02X\n", twiAddr, (deviceAddr)<<1 );
#endif
		if(result != ADI_TWI_SUCCESS)
			printf( "\tERROR during I2C IDT Write : TWI%d, Device=0x%02X, Command=0x01\n", twiAddr, (deviceAddr)<<1 );
	}

	result = adi_twi_Close(hDevice);
	CHECK_RESULT(result, "adi_twi_Close failed");

	return 1;
}

void FINBOARD_CLK_Synth_Dump( void )
{
    uint8_t regAddr;
    uint8_t regData;

    printf( "Dumping Clock Synthesizer registers\n" );

    for (regAddr=0x00; regAddr<=0x88; regAddr++)
    {
    	if ( FINBOARD_CLK_Synth_Read( regAddr, &regData ) == 1 )
    	{
           printf( "\t0x%02X => 0x%02X\n", regAddr, regData );
        }
    }
}

uint32_t FINBOARD_CLK_Synth_Config_OUT4_74_25_MHz( void )
{
   uint32_t i;
   uint8_t regAddr;
   uint8_t regData;

   printf( "Configuring Clock Synthesizer OUT4 for 720P60 resolution\n");

   for ( i = 0; i < sizeof(CLKSynt720P60Config)/sizeof(FINBOARD_I2C_CONFIG); i++ )
   {
      regAddr = CLKSynt720P60Config[i].Register;
      regData = CLKSynt720P60Config[i].Value;
   	  if ( FINBOARD_CLK_Synth_Write( regAddr, regData ) == 1 )
   	  {
          //printf( "\t0x%02X => 0x%02X\n", regAddr, regData );
      }
   	  else
   	  {
   		  printf( "\tERROR : Failed to configure Clock Synthesizer\n");
   		  return 0;
   	  }
   }

   return 1;
}

uint32_t FINBOARD_CLK_Synth_Config_OUT4_27_00_MHz( void )
{
   uint32_t i;
   uint8_t regAddr;
   uint8_t regData;

   printf( "Configuring Clock Synthesizer OUT4 for 480P60 resolution\n");

   for ( i = 0; i < sizeof(CLKSynt480P60Config)/sizeof(FINBOARD_I2C_CONFIG); i++ )
   {
	  regAddr = CLKSynt480P60Config[i].Register;
	  regData = CLKSynt480P60Config[i].Value;
	  if ( FINBOARD_CLK_Synth_Write( regAddr, regData ) == 1 )
	  {
		  //printf( "\t0x%02X => 0x%02X\n", regAddr, regData );
	  }
	  else
	  {
		  printf( "\tERROR : Failed to configure Clock Synthesizer\n");
		  return 0;
	  }
   }

   return 1;

}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */
