/*
 *  Software Switch Configuration for the Video Encoder EI3 Extender Board
 *
 *
 */

#include <drivers\twi\adi_twi.h>
#include "EdgeDetection.h"

/* TWI settings */
#define TWI_ADDRESS   (0x25u)
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

/* switch register structure */
typedef struct {
	uint8_t Register;
	uint8_t Value;
} SWITCH_CONFIG;

/* switch parameter structure */
typedef struct {
	uint32_t TWIDevice;
	uint16_t HardwareAddress;
	uint32_t NumConfigSettings;
	SWITCH_CONFIG *ConfigSettings;
} SOFT_SWITCH;

/* external functions */

void ConfigSoftSwitches_Encoder(void);

/* disable misra diagnostics as necessary */
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_8_7:"Objects shall be defined at block scope")
#pragma diag(suppress:misra_rule_17_4:"Array indexing shall be the only allowed form of pointer arithmetic")
#endif /* _MISRA_RULES */

/************************SoftConfig Information********************************

    ~ means the signal is active low

    Please see the Video Encoder EI3 Extender manual for more information on using
    Software-Controlled Switches(SoftConfig)

********************************************************************************/


/* Video Encoder EI3 Extender Software switch for ADV7511 16bit 74MHz PPI clock */
static SWITCH_CONFIG SwitchConfigEncoder[] =
{
    /*
                U20 Port A                                  U20 Port B

        7--------------- ~SPORT_ENABLE          |       7--------------- ~ADV7511_INT_GPIO7
        | 6------------- ~ 8_BIT_ADV7341        |       | 6------------- ~ADV7511_INT_GPIO6
        | | 5----------- ~74MHZ_CLK_EN          |       | | 5----------- ~ADV7511_INT_GPIO5
        | | | 4--------- ~27MHZ_CLK_EN          |       | | | 4--------- ~ADV7511_INT_GPIO4
        | | | | 3------- ~16_BIT_ADV7341        |       | | | | 3------- ~ADV7511_INT_GPIO3
        | | | | | 2----- ~16_BIT_ADV7511        |       | | | | | 2----- ~ADV7511_INT_GPIO2
        | | | | | | 1--- ~24_BIT_ADV7341        |       | | | | | | 1--- ~ADV7511_INT_GPIO1
        | | | | | | | 0- ~24_BIT_ADV7511        |       | | | | | | | 0- ~ADV7511_INT_GPIO0
        | | | | | | | |                         |       | | | | | | | |
        Y N N Y N Y N N                         |       N N N Y N N N N     ( Active Y or N )
        0 1 1 0 1 0 1 1                         |       1 1 1 0 1 1 1 1     ( value being set )
    */
#ifdef DISPLAY_720P
        { 0x12u, 0x5Bu },                               { 0x13u, 0xEFu },
#else
        { 0x12u, 0x6Bu },                               { 0x13u, 0xEFu },
#endif
        /*
         * specify inputs/outputs
         */

        { 0x00u, 0x00u }, /* Set IODIRA direction (all outputs) */
        { 0x01u, 0x00u }  /* Set IODIRB direction (all output) */
};

/* switch configuration */
static SOFT_SWITCH SoftSwitch[] =
{
  {
    0u,
    TWI_ADDRESS,
    sizeof(SwitchConfigEncoder)/sizeof(SWITCH_CONFIG),
    SwitchConfigEncoder
  }
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

void ConfigSoftSwitches_Encoder(void)
{
    uint32_t switchNum;
    uint32_t configNum;
    uint32_t settings;
    uint32_t switches;

    SOFT_SWITCH *ss;
    SWITCH_CONFIG *configReg;
    ADI_TWI_RESULT result;

    switches = (uint32_t)(sizeof(SoftSwitch)/sizeof(SOFT_SWITCH));
    for (switchNum=0u; switchNum<switches; switchNum++)
    {
        ss = &SoftSwitch[switchNum];

        result = adi_twi_Open(ss->TWIDevice, ADI_TWI_MASTER,
            deviceMemory, ADI_TWI_MEMORY_SIZE, &hDevice);
        CHECK_RESULT(result, "adi_twi_Open failed");

        result = adi_twi_SetHardwareAddress(hDevice, ss->HardwareAddress);
        CHECK_RESULT(result, "adi_twi_SetHardwareAddress failed");

        result = adi_twi_SetPrescale(hDevice, TWI_PRESCALE);
        CHECK_RESULT(result, "adi_twi_Prescale failed");

        result = adi_twi_SetBitRate(hDevice, TWI_BITRATE);
        CHECK_RESULT(result, "adi_twi_SetBitRate failed");

        result = adi_twi_SetDutyCycle(hDevice, TWI_DUTYCYCLE);
        CHECK_RESULT(result, "adi_twi_SetDutyCycle failed");

        /* switch register settings */
        for (configNum=0u; configNum<ss->NumConfigSettings; configNum++)
        {
            configReg = &ss->ConfigSettings[configNum];

            /* write register value */
            twiBuffer[0] = configReg->Register;
            twiBuffer[1] = configReg->Value;
            result = adi_twi_Write(hDevice, twiBuffer, (uint32_t)2, false);
            CHECK_RESULT(result, "adi_twi_Write failed");
        }

        result = adi_twi_Close(hDevice);
        CHECK_RESULT(result, "adi_twi_Close failed");
    }
}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */
 
