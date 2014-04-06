/*****************************************************************************
Copyright (c) 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9082 $
$Date: 2012-10-13 16:48:31 +0530 (Sat, 13 Oct 2012) $

Title       : adi_memory.c

Description : Memory declarations

*****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include "adi_memory.h"

/*==============  D E F I N E S  ===============*/

/*=============  D A T A  =============*/

/* L1 Input Buffers bank 1 */
#pragma alignment_region (16)

#pragma section("adi_appl_fastb1_prio0_rw")
int8_t  aTempBuffer1[ADI_TEMP_BUFFER1_SIZE_L1];

/* L1 Output Buffers bank 1 */

#pragma section("adi_appl_fastb0_prio0_rw")
int8_t  aTempBuffer3[ADI_DMA_OUTPUT_BUFF1_SIZE_L1];

/* L1 Input Buffers bank 2 */

#pragma section("adi_appl_fastb0_prio0_rw")
int8_t  aTempBuffer2[ADI_TEMP_BUFFER2_SIZE_L1];

/* L1 Output Buffers bank 2 */

#pragma section("adi_appl_fastb1_prio0_rw")
int8_t  aTempBuffer4[ADI_DMA_OUTPUT_BUFF2_SIZE_L1];
#pragma alignment_region_end
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
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_memory.c $
**
*/
