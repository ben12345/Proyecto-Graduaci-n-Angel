/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : adi_dma_func.h

Description : prototype for DMA functions

*****************************************************************************/
#ifndef __ADI_DMA_FUNC_H__
#define __ADI_DMA_FUNC_H__

/*=============  I N C L U D E S   =============*/
#include "adi_mem_move.h"

/*==============  D E F I N E S  ===============*/

/*=============  D A T A    T Y P E S   =============*/
#if defined(__ADSPBF609_FAMILY__)
typedef struct __dma_desc
{
    void    *pStartAddress;
    int32_t Config;
    int32_t XCount;
    int32_t XModify;
    int32_t YCount;
    int32_t YModify;
} dma_desc;
#else
typedef struct __dma_desc
{
    int16_t SAL;
    int16_t SAH;
    int16_t CFG;
    int16_t XCNT;
    int16_t XMOD;
    int16_t YCNT;
    int16_t YMOD;
} dma_desc;
#endif

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/

extern
void    dmastart(
                 dma_desc    *pDMASrc,
                 dma_desc    *pDMADst
                 );

extern
void    dmawait(void);

#endif /* __ADI_DMA_FUNC_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_dma_func.h $
**
*/
