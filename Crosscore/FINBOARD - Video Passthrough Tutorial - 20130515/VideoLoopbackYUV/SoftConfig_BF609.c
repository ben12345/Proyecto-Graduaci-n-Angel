/*
 *  Software Switch Configuration for the ADSP-BF609-EZ-BOARD
 *
 *  Please see the SoftConfig documentation in CCES Help for detailed
 *  information on SoftConfig.
 *
 */

#include <drivers\twi\adi_twi.h>

/* TWI settings */
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

/* prototype */
void ConfigSoftSwitches_BF609(void);

/* disable misra diagnostics as necessary */
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_8_7:"Objects shall be defined at block scope")
#pragma diag(suppress:misra_rule_17_4:"Array indexing shall be the only allowed form of pointer arithmetic")
#endif /* _MISRA_RULES */

/************************SoftConfig Information********************************

    ~ means the signal is active low

    Please see the ADSP-BF609 EZ-KIT manual for more information on using
    Software-Controlled Switches(SoftConfig)

********************************************************************************/
/* switch 0 register settings */
static SWITCH_CONFIG SwitchConfig0[] =
{
/*
           U45 Port A                                  U45 Port B

   7---------------  RMII_CLK_EN       |       7--------------- ~TEMP_THERM_EN
   | 6------------- ~CNT0ZM_EN         |       | 6------------- ~TEMP_IRQ_EN
   | | 5----------- ~CNT0DG_EN         |       | | 5----------- ~UART0CTS_146_EN
   | | | 4--------- ~CNT0UD_EN         |       | | | 4--------- ~UART0CTS_RST_EN
   | | | | 3------- ~CAN0RX_EN         |       | | | | 3------- ~UART0CTS_RTS_LPBK
   | | | | | 2----- ~CAN0_ERR_EN       |       | | | | | 2----- ~UART0CTS_EN
   | | | | | | 1--- ~CAN_STB           |       | | | | | | 1--- ~UART0RX_EN
   | | | | | | | 0-  CAN_EN            |       | | | | | | | 0- ~UART0RTS_EN
   | | | | | | | |                     |       | | | | | | | |
   N N N N N N N N                     |       N N N N N N N N     ( Active Y or N )
   0 1 1 1 1 1 1 0                     |       1 1 1 1 1 1 1 1     ( value being set )
*/
   { 0x12u, 0x7Eu },                           { 0x13u, 0xFFu },

  /*
   * specify inputs/outputs
   */

  { 0x0u, 0x0u },   /* Set IODIRA direction (all output) */
  { 0x1u, 0x0u },   /* Set IODIRB direction (all output) */
};
/* switch 1 register settings */
static SWITCH_CONFIG SwitchConfig1[] =
{

/*
          U46 Port A                                  U46 Port B

  7--------------- ~LED4_GPIO_EN          |       7---------------  EMPTY
  | 6------------- ~LED3_GPIO_EN          |       | 6------------- ~SPI0D3_EN
  | | 5----------- ~LED2_GPIO_EN          |       | | 5----------- ~SPI0D2_EN
  | | | 4--------- ~LED1_GPIO_EN          |       | | | 4--------- ~SPIFLASH_CS_EN
  | | | | 3-------  SMC0_LP0_EN           |       | | | | 3------- ~SD_WP_EN
  | | | | | 2-----  EMPTY                 |       | | | | | 2----- ~SD_CD_EN
  | | | | | | 1---  SMC0_EPPI2_LP1_SWITCH |       | | | | | | 1--- ~PUSHBUTTON2_EN
  | | | | | | | 0-  OVERRIDE_SMC0_LP0_BOOT|       | | | | | | | 0- ~PUSHBUTTON1_EN
  | | | | | | | |                         |       | | | | | | | |
  N N N N N X Y Y                         |       X N N N N N N N     ( Active Y or N )
  1 1 1 1 1 X 1 1                         |       X 1 1 1 1 1 1 1     ( value being set )
*/
  { 0x12u, 0xFBu },                               { 0x13u, 0x7Fu },

/*
  * specify inputs/outputs
  */

  { 0x0u, 0x0u },   /* Set IODIRA direction (all output) */
  { 0x1u, 0x0u },   /* Set IODIRB direction (all output) */
};
/* switch 2 register settings */
static SWITCH_CONFIG SwitchConfig2[] =
{

/*
            U47 Port A                                                  U47 Port B

    7--------------- ~PD2_SPI0MISO_EI3_EN                   |       7---------------  EMPTY
    | 6------------- ~PD1_SPI0D3_EPPI1D17_SPI0SEL2_EI3_EN   |       | 6-------------  EMPTY
    | | 5----------- ~PD0_SPI0D2_EPPI1D16_SPI0SEL3_EI3_EN   |       | | 5-----------  EMPTY
    | | | 4--------- ~WAKE_PUSHBUTTON_EN                    |       | | | 4---------  EMPTY
    | | | | 3------- ~ETHERNET_EN                           |       | | | | 3-------  EMPTY
    | | | | | 2-----  PHYAD0                                |       | | | | | 2-----  EMPTY
    | | | | | | 1---  PHY_PWR_DWN_INT                       |       | | | | | | 1--- ~PD4_SPI0CK_EI3_EN
    | | | | | | | 0- ~PHYINT_EN                             |       | | | | | | | 0- ~PD3_SPI0MOSI_EI3_EN
    | | | | | | | |                                         |       | | | | | | | |
    Y N N N N N N N                                         |       X X X X X X N N     ( Active Y or N )
    0 1 1 1 1 0 0 1                                         |       X X X X X X 1 1     ( value being set )
*/
  { 0x12u, 0x79u },                                                 { 0x13u, 0x00u },

  /*
   * specify inputs/outputs
   */

  { 0x0u, 0x4u },    /* Set IODIRA direction (bit 2 input, all others output) */
  { 0x1u, 0x0u },    /* Set IODIRB direction (all output) */
};



/* switch configuration */
static SOFT_SWITCH SoftSwitch[] =
{
  {
    0u,
    0x21u,
    sizeof(SwitchConfig0)/sizeof(SWITCH_CONFIG),
    SwitchConfig0
  },
  {
    0u,
    0x22u,
    sizeof(SwitchConfig1)/sizeof(SWITCH_CONFIG),
    SwitchConfig1
  },
  {
    0u,
    0x23u,
    sizeof(SwitchConfig2)/sizeof(SWITCH_CONFIG),
    SwitchConfig2
  },
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
 
void ConfigSoftSwitches_BF609(void)
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
 
