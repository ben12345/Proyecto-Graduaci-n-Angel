/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      sensor.h
 * @brief     MT9M114 sensor configuration.
 * @version:  $Revision: 10123 $
 * @date:     $Date: 2012-07-09 16:34:32 -0400 (Mon, 09 Jul 2012) $
 *
 * @details
 *            This is the primary API file for the sensor driver configuration
 *            and buffer management.
 *
 */

#ifndef __SENSOR_H__
#define __SENSOR_H__

#include <drivers/sensor/aptina/mt9m114/adi_mt9m114.h>
#include <stdint.h>

/*=============  D E F I N E S  =============*/

/* MT9M114 Device instance to open */
#define ADI_MT9M114_DEV_NUM             (0u)

/* PPI Device number to receive MT91M114 sensor Data */
//#define ADI_MT9M114_PPI_DEV_NUM         (1u)
#define ADI_MT9M114_PPI_DEV_NUM         (2u)

/* TWI Device number to access MT9M114 sensor registers */
#define ADI_MT9M114_TWI_DEV_NUM         (0u)

/* 7-bit TWI Address for communicating with sensor */
#define ADI_MT9M114_TWI_ADDR            ADI_MT9M114_TWI_ADDR_PRI

/* TWI Bitrate */
#define ADI_MT9M114_TWI_BITRATE         (100u)

/* TWI Prescale value (Assuming 300Mhz system clock) */
#define ADI_MT9M114_TWI_PRESCALE        (8u)

/* Video frame Size (Max it to hold an entire 1280x780x2 frame) */
#define VIDEO_BUF_SIZE                  (1996800)


/*** MT9M114 video frame structure ***/
typedef struct  MT9M114_VIDEO_BUF
{
    /* Video frame to accommodate maximum video resolution */
    uint8_t                         VideoBuf[VIDEO_BUF_SIZE];

    /* TRUE when the frame is empty */
    bool                            bIsEmpty;

    /* Next frame in chain */
    struct MT9M114_VIDEO_BUF   *pNext;

} MT9M114_VIDEO_BUF;


/* Configures the video sensor */
uint32_t ConfigureSensor(void);

/* Submits a video frames for decoding */
uint32_t SubmitEmptyVideoFrame(void);

/* Returns the filled video frame */
uint32_t  GetFilledVideoFrame (uint8_t **ppVideoFrame);

/* Disables or disables the video sensor data flow */
uint32_t EnableSensor(bool bEnable);

#endif /* __SENSOR_H__ */
