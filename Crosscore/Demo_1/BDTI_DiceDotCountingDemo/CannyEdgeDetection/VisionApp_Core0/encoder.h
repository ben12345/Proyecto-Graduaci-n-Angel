/* $Revision: 8785 $
 * $Date: 2012-09-26 18:07:40 +0530 (Wed, 26 Sep 2012) $ 
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
#include "Stage_1.h"
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

/* Encoder device instance number */
#define	ENCODER_DEVICE_NUM              (0u)



#endif /* __ENCODER_H__ */
