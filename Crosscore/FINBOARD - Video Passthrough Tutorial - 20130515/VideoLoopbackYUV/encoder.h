/* $Revision: 10123 $
 * $Date: 2012-07-09 16:34:32 -0400 (Mon, 09 Jul 2012) $ 
******************************************************************************
Copyright (c), 2012 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
Title: ADV7511 Video encoder header file.

Description: This is the primary header file for .

*****************************************************************************/

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <drivers/encoder/adv7511/adi_adv7511.h>

#include "VideoLoopbackYUV.h"

/*
 * Local APIs
 */

/* Configures the video encoder */
uint32_t ConfigureEncoder (void);

/* Submits the given frame for display */
uint32_t DisplayFrame(void *pFrame);

/* Enables or disables the display */
uint32_t EnableDisplay(bool bEnable);

/* Gets back the displayed frame */
uint32_t GetDisplayedFrame(void **ppFrame);

/******************************************************************************
**
** Encoder specific Macros
**
******************************************************************************/

/* Video Encoder frame dimensions */
#if defined(VIDEOLOOPBACKYUV_720P)
#define ENCODER_FRAME_WIDTH              1280u
#define ENCODER_FRAME_HEIGHT             720u
#else
#define ENCODER_FRAME_WIDTH              720u
#define ENCODER_FRAME_HEIGHT             480u
#endif
#define ENCODER_BYTES_PER_PIXEL          2u

/* Encoder device instance number */
#define	ENCODER_DEVICE_NUM              (0u)

/* PPI peripheral parameters */
#define PPI_DEV_NUMBER                  (0u)

/* TWI device number */
#define TWI_DEV_NUMBER                  (0u)


#endif /* __ENCODER_H__ */
