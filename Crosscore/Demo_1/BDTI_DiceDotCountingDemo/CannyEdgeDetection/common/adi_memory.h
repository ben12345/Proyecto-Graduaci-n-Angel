/*****************************************************************************
Copyright (c) 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8692 $
$Date: 2012-09-19 12:45:56 +0530 (Wed, 19 Sep 2012) $

Title       : adi_memory.h

Description : Memory declarations

*****************************************************************************/
#ifndef __ADI_MEMORY_H__
#define __ADI_MEMORY_H__

/*=============  I N C L U D E S   =============*/
#include "adi_tool_chain.h"

/*==============  D E F I N E S  ===============*/
#define ADI_DMA_INPUT_BUFF1_SIZE_L1     ((uint32_t) 0x600)
#define ADI_DMA_INPUT_BUFF2_SIZE_L1     ((uint32_t) 0x11D0)
#define ADI_TEMP_BUFFER1_SIZE_L1        ((uint32_t) ADI_DMA_INPUT_BUFF1_SIZE_L1 * 2)
#define ADI_TEMP_BUFFER2_SIZE_L1        ((uint32_t) ADI_DMA_INPUT_BUFF2_SIZE_L1 * 2)
#define ADI_DMA_OUTPUT_BUFF1_SIZE_L1    ((uint32_t) ADI_DMA_INPUT_BUFF1_SIZE_L1)
#define ADI_DMA_OUTPUT_BUFF2_SIZE_L1    ((uint32_t) 0xC00)

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

extern
int8_t  aTempBuffer1[ADI_TEMP_BUFFER1_SIZE_L1];

extern
int8_t  aTempBuffer3[ADI_DMA_OUTPUT_BUFF1_SIZE_L1];

extern
int8_t  aTempBuffer2[ADI_TEMP_BUFFER2_SIZE_L1];

extern
int8_t  aTempBuffer4[ADI_DMA_OUTPUT_BUFF2_SIZE_L1];

/*=============  D A T A    T Y P E S   =============*/

#endif /* __ADI_MEMORY_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_memory.h $
**
*/
