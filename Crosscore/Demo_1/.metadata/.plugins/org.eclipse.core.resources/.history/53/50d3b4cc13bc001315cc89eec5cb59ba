/*****************************************************************************
Copyright (c) 2009 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : adi_dma_vars.c

Description : variables required for data DMAing

*****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include "adi_tool_chain.h"

/*==============  D E F I N E S  ===============*/

/*=============  D A T A  =============*/
#pragma section("adi_appl_fastb1_prio0_rw")
uint32_t            mdma_base_address_0;
#pragma section("adi_appl_fastb1_prio0_rw")
uint32_t            mdma_base_address_1;

#pragma section("adi_appl_slow_noprio_rw")
int32_t             dmasemL3 = 1;
#pragma section("adi_appl_fastb0_prio0_rw")
int32_t             dmasemL1 = 1;
#pragma section("adi_appl_fastb0_prio0_rw")
int32_t             DMASem = 1;

#if defined(__ADSPBF609_FAMILY__)
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int32_t    *MDMA_D0CFG;
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int32_t    *MDMA_D0DSC;
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int32_t    *MDMA_S0CFG;
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int32_t    *MDMA_S0DSC;
#else
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int16_t    *MDMA_D0CFG;
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int32_t    *MDMA_D0DSC;
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int16_t    *MDMA_S0CFG;
#pragma section("adi_appl_fastb0_prio0_rw")
volatile int32_t    *MDMA_S0DSC;
#endif

/*=============  C O D E  =============*/

/*
**  Function Prototype section
**  (static-scoped functions)
*/

/*
**  Function Definition section
*/

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_dma_vars.c $
**
*/
