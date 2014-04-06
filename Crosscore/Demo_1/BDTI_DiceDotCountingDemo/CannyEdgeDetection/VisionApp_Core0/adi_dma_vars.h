/*****************************************************************************
Copyright (c) 2009 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : adi_dma_vars.h

Description : DMA variables declaration required by rotate module

*****************************************************************************/
#ifndef __ADI_DMA_VARS_H__
#define __ADI_DMA_VARS_H__

/*=============  I N C L U D E S   =============*/

/*==============  D E F I N E S  ===============*/

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/
extern
uint32_t            mdma_base_address_0;
extern
uint32_t            mdma_base_address_1;

extern
int32_t             dmasemL3;
extern
int32_t             dmasemL1;
extern
int32_t             DMASem;

#if defined(__ADSPBF609_FAMILY__)
extern
volatile int32_t    *MDMA_D0CFG;
extern
volatile int32_t    *MDMA_D0DSC;
extern
volatile int32_t    *MDMA_S0CFG;
extern
volatile int32_t    *MDMA_S0DSC;
#else
extern
volatile int16_t    *MDMA_D0CFG;
extern
volatile int32_t    *MDMA_D0DSC;
extern
volatile int16_t    *MDMA_S0CFG;
extern
volatile int32_t    *MDMA_S0DSC;
#endif

/*
**  External Function Prototypes
*/

/*=============  D A T A    T Y P E S   =============*/
#endif /* __ADI_DMA_VARS_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_dma_vars.h $
**
*/
