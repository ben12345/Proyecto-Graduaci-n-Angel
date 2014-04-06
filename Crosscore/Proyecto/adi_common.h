/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      adi_common.h
 * @brief     Common definitions for both cores
 * @version:  $Revision: 8662 $
 * @date:     $Date: 2012-09-13 18:51:00 +0530 (Thu, 13 Sep 2012) $
 *
 * @details
 *            This is the common include file used by both cores.
 *
 */

#ifndef __ADI_COMMON_H__
#define __ADI_COMMON_H__

/*=============  I N C L U D E S   =============*/

#include <stdio.h>
#include <string.h>

/*=============  D E F I N E S  =============*/
//#define DEBUG_INFO
/* Example return codes */
/*  Called function return successfully */
#define SUCCESS                         0
/* Called function failed */
#define FAILURE                         1

/* Macro to manage the display size. We support only 720p */
#define DISPLAY_720P
/* UYVY data format. Number of bytes per pixel is 2 */
#define ENCODER_BYTES_PER_PIXEL          2u
#ifdef DISPLAY_720P

/* Width of input video */
#define INPUT_VIDEO_WIDTH                720

/* Height of input video */
#define INPUT_VIDEO_HEIGHT               480

/* Width of output video */
#define OUTPUT_VIDEO_WIDTH               1280

/* Height of output video */
#define OUTPUT_VIDEO_HEIGHT              720

#define Y_OFFSET_FOR_VIDEO_POSITION    ((OUTPUT_VIDEO_HEIGHT-INPUT_VIDEO_HEIGHT)/2)

#else

#define INPUT_VIDEO_WIDTH                320

#define INPUT_VIDEO_HEIGHT               240

#define OUTPUT_VIDEO_WIDTH               720

#define OUTPUT_VIDEO_HEIGHT              480

#endif

/* Video buffer Size - to hold UYVY422 720p frame */
#define    VIDEO_BUF_SIZE                    (OUTPUT_VIDEO_WIDTH*OUTPUT_VIDEO_HEIGHT*ENCODER_BYTES_PER_PIXEL)
/* Offset to position (from top left corner) to place input video in 1280X720 buffer */
#define	VIDEO_BUF_POSITION_OFFSET	    (OUTPUT_VIDEO_WIDTH * ((OUTPUT_VIDEO_HEIGHT-INPUT_VIDEO_HEIGHT)/2) * ENCODER_BYTES_PER_PIXEL)

/* Stride to place the input video in the 1280X720 frame buffer */
#define DISPLAY_BUFFER_Y_STRIDE          (OUTPUT_VIDEO_WIDTH-INPUT_VIDEO_WIDTH)*ENCODER_BYTES_PER_PIXEL+32
/*
 * Video buffer structure specific to this example
 */

/*
  *  Enumeration for  status of the buffer.
 */
typedef enum CONTOUR_COUNT_BUFFER_STATUS
{

    CONTOUR_COUNT_BUFFER_FREE,     /*Buffer is ready to submit for the sensor */

    CONTOUR_COUNT_BUFFER_FILLED,   /* Buffer is returned from sensor. But Dotcount is not updated*/

    CONTOUR_COUNT_BUFFER_READY     /* Buffer is ready to display */

} CONTOUR_COUNT_BUFFER_STATUS;

typedef struct  VIDEO_BUF
{
    /* Video buffer to accommodate maximum video resolution */
    uint8_t             Data[VIDEO_BUF_SIZE];
    /* TRUE when the buffer has valid data to display */
    CONTOUR_COUNT_BUFFER_STATUS eStatus;
    /* counter to track buffer usage in encoder queue */
    uint8_t             EncUsageCount;
    /* counter to track buffer usage in decoder queue */
    uint8_t             DecUsageCount;
    /* Next buffer in chain */
    struct VIDEO_BUF    *pNext;
} VIDEO_BUF;

/*** MT9M114 video frame structure ***/
typedef struct  MT9M114_VIDEO_BUF
{
    /* Video buffer to accommodate maximum video resolution */
    uint8_t             VideoBuf[VIDEO_BUF_SIZE];
    /* TRUE when the buffer has valid data to display */
    CONTOUR_COUNT_BUFFER_STATUS    eStatus;
    /* counter to track buffer usage in encoder queue */
    uint8_t             EncUsageCount;
    /* counter to track buffer usage in camera  queue */
    uint8_t             CameraUsageCount;
    /* Next buffer in chain */
    struct MT9M114_VIDEO_BUF    *pNext;
}MT9M114_VIDEO_BUF;

/*** structure to hold the information passed between 2 cores ***/
typedef struct  MESSAGE_INFO
{
     /* Hold address of EdgeTrace output buffer */
    uint8_t *pBuf1;

    /* Hold address of Input buffer */
    uint8_t *pBuf2;

    /* Width of image */
    uint32_t nWidth;

    /* Height of image */
    uint32_t nHeight;

    /* Bounding rectangle flag */
    uint32_t nBoundingRectFlag;

    /* Completion Flag */
    volatile uint32_t nCompletionFlag;

}MESSAGE_INFO;

#endif /* __ADI_COMMON_H__ */
