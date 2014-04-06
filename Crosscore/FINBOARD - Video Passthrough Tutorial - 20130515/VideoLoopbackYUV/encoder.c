/* $Revision: 12227 $
 * $Date: 2012-11-06 13:29:05 -0500 (Tue, 06 Nov 2012) $ 
******************************************************************************
Copyright (c), 2012 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************

Title: ADV7511 Video Encoder file.

Description: This is the primary source file to configure and display video
             frames using ADV7511 Video encoder.
*****************************************************************************/
#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"Suppress all rules in the test code")
#endif

/*=============  I N C L U D E S   =============*/

/* ADV7511 Video encoder file */
#include <drivers/encoder/adv7511/adi_adv7511.h>

/* Configuration macro includes */
#include "encoder.h"

/* for printf */
#include <stdio.h>

#include <builtins.h>

/* Test result codes */
#define EXAMPLE_PASSED             0u
#define EXAMPLE_FAILED             1u

#define REPORT_ERROR(mesg)      printf(mesg); \
                                printf("Example Failed\n");\
                                return EXAMPLE_FAILED;

/*=============  D A T A  =============*/

/* Memory for ADV7511*/
uint8_t Adv7511Memory[ADI_ADV7511_MEMORY_SIZE];


/* Encoder Driver handle */
static ADI_ADV7511_HANDLE    hVideoOutDriver;

/* Configures the Softswitches on the video encoder */
//extern void ConfigSoftSwitches_Encoder(void);


/* Configures the video encoder */
uint32_t ConfigureEncoder (void)
{
    uint32_t     nResponseCount;
    uint32_t     nIndex;
    uint32_t     nResult;
    uint32_t     nSense;
    uint32_t     NumCallbacks;

    /* Program the softswitches on the Video Encoder Extender board */
    //ConfigSoftSwitches_Encoder();

    do
    {
        /* Open the ADV7511 Encoder driver */
        if(adi_adv7511_Open(ENCODER_DEVICE_NUM,
                            Adv7511Memory,
                            ADI_ADV7511_MEMORY_SIZE,
                              &hVideoOutDriver
                              ) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to open the encoder driver\n");
        }


        /* Set the PPI device number */
        if(adi_adv7511_SetPPIDevice(hVideoOutDriver,
                                        PPI_DEV_NUMBER) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the PPI device number \n");
        }

        /* Set the TWI Address */
        if(adi_adv7511_SetTWIDevice(hVideoOutDriver,
                                    TWI_DEV_NUMBER, ADI_ADV7511_TWIADDR_72, NULL) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the TWI device for the encoder \n");
        }

        /* Power down the device */
        if(adi_adv7511_PowerDown(hVideoOutDriver, true) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to power down the encoder \n");
        }

        /* clear interrupts */
        if(adi_adv7511_ClearInterrupts(hVideoOutDriver) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to clear the encoder interrupts \n");
        }

        /* Set the display mode */
#if defined(VIDEOLOOPBACKYUV_720P)
        if(adi_adv7511_ConfigDisplayMode(hVideoOutDriver, ADI_ADV7511_720P_YCBCR_60HZ) != ADI_ADV7511_SUCCESS)
#else
        if(adi_adv7511_ConfigDisplayMode(hVideoOutDriver, ADI_ADV7511_480P_YCBCR_60HZ) != ADI_ADV7511_SUCCESS)
#endif
        {
            REPORT_ERROR("Failed to set the encoder display mode \n");
        }

        if(adi_adv7511_SetPpiDmaTransferSize(hVideoOutDriver, ADI_ADV7511_PPI_DMA_256BIT) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the set the encoder PPI DMA transfer width \n");
        }

    }while(0);


    return EXAMPLE_PASSED;
}

/* Displays the given frame */
uint32_t DisplayFrame(void *pFrame)
{
    /* Submit the first frame */
    if(adi_adv7511_SubmitVideoFrame(hVideoOutDriver,
                               pFrame,
                               ENCODER_FRAME_WIDTH * ENCODER_BYTES_PER_PIXEL,
                               ENCODER_FRAME_HEIGHT
                               ) != ADI_ADV7511_SUCCESS)
    {
        REPORT_ERROR("Failed to submit the display frame \n");
    }

    return EXAMPLE_PASSED;
}

/* Gets back the displayed frame */
uint32_t GetDisplayedFrame(void **ppFrame)
{
    /* Get back the displayed frame */
    if(adi_adv7511_GetVideoFrame(hVideoOutDriver,
                                 ppFrame
                                ) != ADI_ADV7511_SUCCESS)
    {
        REPORT_ERROR("Failed to get the displayed frame \n");
    }

    return EXAMPLE_PASSED;
}

/* Enables/disables  the video output from the encoder */
uint32_t EnableDisplay(bool bEnable)
{
    /* Enable the data flow */
    if(adi_adv7511_VideoEnable(hVideoOutDriver, bEnable) != ADI_ADV7511_SUCCESS)
    {
        REPORT_ERROR("Failed to enable the encoder data flow \n");
    }

    return EXAMPLE_PASSED;
}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif
