/* $Revision: 9399 $
 * $Date: 2012-11-09 10:39:43 +0530 (Fri, 09 Nov 2012) $ 
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

#include "finboard_bsp.h"

/*=============  D A T A  =============*/

/* Memory for ADV7511*/
/* Video frames */
#pragma section ("adi_fastb1_prio0_rw")
uint8_t Adv7511Memory[ADI_ADV7511_MEMORY_SIZE];

static uint32_t NumFramesDisplayed =0;
/* Encoder Driver handle */
static ADI_ADV7511_HANDLE    hVideoEncAdv7511;

/* Configures the Softswitches on the video encoder */
//extern void ConfigSoftSwitches_Encoder(void);
static void VideoEncAdv7511Callback(void *pCBParam, uint32_t nEvent, void *pArg);

/* Configures the video encoder */
uint32_t ConfigureEncoder (void)
{
    uint32_t     nResponseCount;
    uint32_t     nIndex;
    uint32_t     nResult;
    uint32_t     nSense;
    uint32_t     NumCallbacks;
    ADI_ADV7511_RESULT eResult;

    do
    {
        /* Open the ADV7511 Encoder driver */
        if(adi_adv7511_Open(ENCODER_DEVICE_NUM,
                            Adv7511Memory,
                            ADI_ADV7511_MEMORY_SIZE,
                              &hVideoEncAdv7511
                              ) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to open the encoder driver\n");
        }
        /* Set the TWI Address */
        if(adi_adv7511_SetTWIDevice(hVideoEncAdv7511,
                                    ENC_TWI_DEV_NUM, ADI_ADV7511_TWIADDR_72, NULL) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the TWI device for the encoder \n");
        }
        /* Set the PPI device number */
        if(adi_adv7511_SetPPIDevice(hVideoEncAdv7511,
                                        ENC_PPI_DEV_NUM) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the PPI device number \n");
        }

        /* Power down the device */
        if(adi_adv7511_PowerDown(hVideoEncAdv7511, true) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to power down the encoder \n");
        }

        /* clear interrupts */
        if(adi_adv7511_ClearInterrupts(hVideoEncAdv7511) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to clear the encoder interrupts \n");
        }
#if !defined(FINBOARD)
        /* Program the softswitches on the Video Encoder Extender board */
        ConfigSoftSwitches_Encoder();
#endif // !defined(FINBOARD)
#ifdef DISPLAY_720P
        /* Set the display mode */
        if(adi_adv7511_ConfigDisplayMode(hVideoEncAdv7511, ADI_DOTCOUNT_DISP_MODE) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the encoder display mode \n");
        }
#else
        /* Set the display mode */
        if(adi_adv7511_ConfigDisplayMode(hVideoEncAdv7511, ADI_ADV7511_480P_YCBCR_60HZ) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the encoder display mode \n");
        }

#endif

        if(adi_adv7511_SetPpiDmaTransferSize(hVideoEncAdv7511,ADI_ADV7511_PPI_DMA_256BIT) != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the PPI device number \n");
        }

        if(adi_adv7511_SetHotPlugDetectMode(hVideoEncAdv7511, ADI_ADV7511_HPD_HIGH)   != ADI_ADV7511_SUCCESS)
        {
            REPORT_ERROR("Failed to set the HOT PlugDetect Mode \n");
        }
        /* Enable callbacks for ADV7511 */
         if ((eResult = adi_adv7511_RegisterVideoCallback(hVideoEncAdv7511, VideoEncAdv7511Callback, NULL)) != ADI_ADV7511_SUCCESS)
         {
             return (eResult);
         }

    }while(0);


    return EXAMPLE_PASSED;
}


/* Enables/disables  the video output from the encoder */
uint32_t EnableDisplay(bool bEnable)
{
    /* Enable the data flow */
    if(adi_adv7511_VideoEnable(hVideoEncAdv7511, bEnable) != ADI_ADV7511_SUCCESS)
    {
        REPORT_ERROR("Failed to enable the encoder data flow \n");
    }

    return EXAMPLE_PASSED;
}
/*********************************************************************
*
*   Submit buffer to video encoder for display
*
*   Parameters
*       - pBuffer       Buffer address to submit
*
*   Returns:
*       - ADV734x/ADV7511 driver return code
*
*********************************************************************/
uint32_t SubmitEncBuf(MT9M114_VIDEO_BUF    *pBuffer)
{
    /* Return code */
    uint32_t    Result;

    /* IF (Encoder can accept more buffer) */
    if (NumEncBufsInQ <= 1)
    {
        /* Submit video frame to ADV7511 encoder */
        if((Result = (uint32_t) adi_adv7511_SubmitVideoFrame(hVideoEncAdv7511, (void *) pBuffer->VideoBuf, OUTPUT_VIDEO_WIDTH*ENCODER_BYTES_PER_PIXEL, OUTPUT_VIDEO_HEIGHT)) != ADI_ADV7511_SUCCESS)
        {
            return (Result);
        }
        /* Update number of encoder buffers in queue count */
        NumEncBufsInQ++;
        /* Update usage count for the submitted buffer */
        pBuffer->EncUsageCount++;
    }

    /* return success */
    return (SUCCESS);

}

/*********************************************************************
*
*   Callback for ADV7511 video encoder
*
*   Parameters
*    - [in]  AppHandle   ADV7511 instance to work on.
*    - [in]  Event       ADV7511 Event ID
*    - [in]  pArg        Event specific argument
*
*   Returns  None
*
*********************************************************************/
static void VideoEncAdv7511Callback(void *pCBParam, uint32_t nEvent, void *pArg)
{
    uint32_t  Result;

    /* IF (ADV7511 Encoder video frame processed) */
    if (nEvent == ADI_ADV7511_VIDEO_FRAME_PROCESSED)
    {
        /* Get the processed encoder buffer address */
        pProcessedEncBuf = (MT9M114_VIDEO_BUF *) pArg;
        pProcessedEncBuf->EncUsageCount--;
        if(pProcessedEncBuf->EncUsageCount == 0)
        {
        	pProcessedEncBuf->eStatus =CONTOUR_COUNT_BUFFER_FREE;
        }
        NumEncBufsInQ--;
        GetProcessedGfxBuf((void *) &pNextEncBuf);
        /* IF (we've no valid buffer to display) */
        if (pNextEncBuf == NULL)
        {
            /* Re-submit the last buffer in encoder display queue */
            pNextEncBuf = pLastEncBuf;
        }

        /* Submit the next buffer to encoder */
        Result = SubmitEncBuf (pNextEncBuf);

        /* Update the aDMCess of last encoder buffer submitted */
        pLastEncBuf = pNextEncBuf;

        /* Update number of frames displayed count */
        NumFramesDisplayed++;
    }
}

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif
