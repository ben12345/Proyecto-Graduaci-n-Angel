/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : adi_contour_dynamic_mem_alloc.h

Description : Objects and function prototype declarations for
              memory alloc using system mem alloc methods required by contours

*****************************************************************************/
#ifndef __ADI_CONTOUR_DYNAMIN_MEM_ALLOC_H__
#define __ADI_CONTOUR_DYNAMIN_MEM_ALLOC_H__

/*=============  I N C L U D E S   =============*/
#include "adi_tool_chain.h"
#include "adi_contour_mem_alloc.h"

/*==============  D E F I N E S  ===============*/

/*=============  D A T A    T Y P E S   =============*/

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/
extern
void*    DynamicMemAlloc(
                         ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                         ADI_CONTOUR_MEM_BLK_TYPE        nType,
                         uint32_t                        nCount
                         );

extern
void    DynamicMemFree(
                       ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                       void                            *pMem,
                       ADI_CONTOUR_MEM_BLK_TYPE        nType,
                       uint32_t                        nCount
                       );

extern
void    DynamicMemReset(
                        ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                        void                            *pMem,
                        ADI_CONTOUR_MEM_BLK_TYPE        nType,
                        uint32_t                        nCount
                        );
#endif /* __ADI_CONTOUR_DYNAMIN_MEM_ALLOC_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_contour_dynamic_mem_alloc.h $
**
*/
