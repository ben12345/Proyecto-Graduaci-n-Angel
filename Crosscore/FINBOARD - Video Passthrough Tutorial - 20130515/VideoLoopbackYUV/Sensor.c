/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      Sensor.c
 * @brief     MT9M114 sensor configuration.
 * @version:  $Revision: 12227 $
 * @date:     $Date: 2012-11-06 13:29:05 -0500 (Tue, 06 Nov 2012) $
 *
 * @details
 *            This is the primary source file for the sensor configuration
 *            and buffer management.
 *
 */


#include "VideoLoopbackYUV.h"
#include "Sensor.h"

#include <stdio.h>

/*=============  D A T A  =============*/

/*** MT9M114 specific data ***/

/* MT9M114 Device handle */
static ADI_MT9M114_HANDLE   hMTM114Dev;

/* MT9M114 device memory */
static uint8_t MT9M114DevMem[ADI_MT9M114_MEMORY_SIZE];

/* Prepares video frames for sensor input */
static void PrepareVideoFrames(void);

/* Video frames */
#pragma align(32)
#pragma section ("sdram_bank0")
static MT9M114_VIDEO_BUF    VideoBuf0;
#pragma align(32)
#pragma section ("sdram_bank1")
static MT9M114_VIDEO_BUF    VideoBuf1;
#pragma align(32)
#pragma  section ("sdram_bank2")
static MT9M114_VIDEO_BUF    VideoBuf2;
#pragma align(32)
#pragma  section ("sdram_bank3")
static MT9M114_VIDEO_BUF    VideoBuf3;

/* Pointer to the next video frame to be submitted to the sensor */
static MT9M114_VIDEO_BUF   *pVideoBufToSubmit = NULL;

/* Number of frames currently submitted to the sensor */
static uint32_t NumBufsInUse = 0;

/*=============  L O C A L    F U N C T I O N    P R O T O T Y P E S  =============*/

/* Resets video frame status flag */
static void ResetVideoBufStatus(void);

/*=============  C O D E  =============*/

/* Configures the sensor */
uint32_t ConfigureSensor(void)
{
    /* Loop variable */
    uint8_t                 i;

    /* Return code */
    ADI_MT9M114_RESULT      eResult;

    ADI_MT9M114_FRAME_INFO  *pFrameInfo;

    /* Configure system - terminate configuration process on error */
    do
    {
        /* Open MT9M114 video sensor instance */
        if ((eResult = adi_mt9m114_Open (ADI_MT9M114_DEV_NUM,
                                         &MT9M114DevMem[0],
                                         &hMTM114Dev)) != ADI_MT9M114_SUCCESS)
        {
            printf("Failed to open MT9M114 sensor, Error Code: 0x%08X\n", eResult);
            break;
        }

        /* Configure the TWI for communicating with MT9M114 sensor */
        if ((eResult = adi_mt9m114_ConfigTWI (hMTM114Dev,
                        ADI_MT9M114_TWI_DEV_NUM,
                        ADI_MT9M114_TWI_ADDR,
                        ADI_MT9M114_TWI_BITRATE,
                        ADI_MT9M114_TWI_PRESCALE)) != ADI_MT9M114_SUCCESS)
        {
            printf("Failed to configure TWI: 0x%08X\n", eResult);
            break;
        }

        /* Set PPI Device number to get the data from MT9M114 */
        if ((eResult = adi_mt9m114_SetPPIDevice (hMTM114Dev, ADI_MT9M114_PPI_DEV_NUM)) != ADI_MT9M114_SUCCESS)
        {
            printf("Failed to set PPI Device Number, Error Code: 0x%08X\n", eResult);
            break;
        }

        /* Configure the image */
#if defined(VIDEOLOOPBACKYUV_720P)
        if((eResult = adi_mt9m114_ConfigImage(hMTM114Dev, ADI_MT9M114_RegConf_720p,
                                              ADI_MT9M114_IMAGE_CFG_ARRAY_SIZE)) != ADI_MT9M114_SUCCESS)
#else
        if((eResult = adi_mt9m114_ConfigImage(hMTM114Dev, ADI_MT9M114_RegConf_480p,
                                              ADI_MT9M114_IMAGE_CFG_ARRAY_SIZE)) != ADI_MT9M114_SUCCESS)
#endif
        {
            printf("Failed to configure the image, Error Code: 0x%08X\n", eResult);
            break;
        }

        /* Get the frame information */
        if((eResult = adi_mt9m114_GetFrameInfo(hMTM114Dev, &pFrameInfo)) != ADI_MT9M114_SUCCESS)
        {
            printf("Failed to get the capture frame information, Error Code: 0x%08X\n", eResult);
            break;
        }


        /* Set the output color format */
        if((eResult = adi_mt9m114_SetColorFormat(hMTM114Dev, ADI_MT9M114_COLOR_FORMAT_YUV, 0u, false, true)) != ADI_MT9M114_SUCCESS)
        {
            printf("Failed to set the output color format, Error Code: 0x%08X\n", eResult);
            break;
        }

        /* Change the DMA transfer width to 256 for better system performance */
        if((eResult = adi_mt9m114_SetDMATransferSize(hMTM114Dev, ADI_MT9M114_DMA_TX_256BIT)) != ADI_MT9M114_SUCCESS)
        {
            printf("Failed to set the set the DMA transfer width, Error Code: 0x%08X\n", eResult);
            break;
        }

        /* Mask the line track overflow error */
        *pREG_EPPI1_IMSK |= (1 << 2);

    }while (0);

    /* Prepare video frames for filling */
    PrepareVideoFrames();

    if (eResult != ADI_MT9M114_SUCCESS)
    {
        return FAILURE;
    }
    else
    {
        return SUCCESS;
    }
}

/*********************************************************************

    Function: GetFilledVideoFrame

        Process a video frame that has valid video data

    Returns:
        Error code

*********************************************************************/
uint32_t  GetFilledVideoFrame (uint8_t **ppVideoFrame)
{
    /* Return code */
    ADI_MT9M114_RESULT  eResult = ADI_MT9M114_SUCCESS;
    bool bAvailable;

    *ppVideoFrame = NULL;

    /* Check if a filled video frame is available */
    if((eResult = adi_mt9m114_IsFrameAvailable(hMTM114Dev, &bAvailable)) != ADI_MT9M114_SUCCESS)
    {
        printf("Failed to check if a filled video frame is available, Error Code: 0x%08X\n", eResult);
        return FAILURE;
    }

    /* Get the frame only if available to avoid the blocking */
    if(bAvailable == true)
    {
        /* Get the filled video frame */
        if((eResult = adi_mt9m114_GetFrame(hMTM114Dev, (void **)ppVideoFrame)) != ADI_MT9M114_SUCCESS)
        {
            printf("Failed to get filled video frame, Error Code: 0x%08X\n", eResult);
            return FAILURE;
        }

        ((MT9M114_VIDEO_BUF *) (*ppVideoFrame))->bIsEmpty = true;

        NumBufsInUse--;
    }

    return SUCCESS;
}

/*********************************************************************

    Function: SubmitVideoFrame

        Submits a video frames for filling

    Returns:
        none

*********************************************************************/
uint32_t SubmitEmptyVideoFrame(void)
{
    /* Return code */
    ADI_MT9M114_RESULT  eResult;

    /* IF (we've an empty video frame) */
    if (pVideoBufToSubmit->bIsEmpty)
    {
        /* IF (Sensor can accept more frame) */
        if (NumBufsInUse <= 1)
        {
                /* Submit Video frame to MT9M114 */
            if ((eResult = adi_mt9m114_SubmitFrame(hMTM114Dev, (void *) pVideoBufToSubmit)) != ADI_MT9M114_SUCCESS)
            {
                printf("Failed to Submit Video frame to MT9M114, Error Code: 0x%08X\n", eResult);
                return FAILURE;
            }
            /* ELSE (Successfully submitted video frame) */
            else
            {
                /* Update frame in use count */
                NumBufsInUse++;
                /* update video frame status flag */
                pVideoBufToSubmit->bIsEmpty = false;
                /* Move to next video frame in chain */
                pVideoBufToSubmit = pVideoBufToSubmit->pNext;
            }
        }
    }

    /* return */
    return SUCCESS;
}


/* Enables or disables  the sensor data flow */
uint32_t EnableSensor(bool bEnable)
{
    /* Return code */
    ADI_MT9M114_RESULT      eResult;

    /* Enable MT9M114 dataflow */
    if((eResult = adi_mt9m114_Enable(hMTM114Dev, bEnable)) != ADI_MT9M114_SUCCESS){
        printf("Failed to enable/disble MT9M114 dataflow, Error Code: 0x%08X\n", eResult);
        return FAILURE;
    }

    return SUCCESS;
}

/*********************************************************************

    Function: PrepareVideoFrames

        Prepares video frames for filling

    Returns:
        none

*********************************************************************/
static void PrepareVideoFrames(void)
{
    /* Reset video frame status flags */
    ResetVideoBufStatus();

    /* Chain video frames */
    VideoBuf0.pNext = &VideoBuf1;
    VideoBuf1.pNext = &VideoBuf2;
    VideoBuf2.pNext = &VideoBuf3;
    VideoBuf3.pNext = &VideoBuf0;

    /* Video frame that ready to accept new data */
    pVideoBufToSubmit = &VideoBuf0;
}

/*********************************************************************

    Function: ResetVideoBufStatus

        Resets video frame status flag

    Returns:
        none

*********************************************************************/
static void ResetVideoBufStatus(void)
{
   /* Reset video frames status */
    VideoBuf0.bIsEmpty = true;
    VideoBuf1.bIsEmpty = true;
    VideoBuf2.bIsEmpty = true;
    VideoBuf3.bIsEmpty = true;
}

