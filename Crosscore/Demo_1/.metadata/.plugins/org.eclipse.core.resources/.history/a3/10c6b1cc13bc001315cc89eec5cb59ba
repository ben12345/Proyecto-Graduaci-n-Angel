/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : adi_contour_pseudo_dynamic_mem_alloc.h

Description : Objects and function prototype declarations for
              pseudo memory alloc required by contours

*****************************************************************************/
#ifndef __ADI_CONTOUR_PSEUDO_DYNAMIN_MEM_ALLOC_H__
#define __ADI_CONTOUR_PSEUDO_DYNAMIN_MEM_ALLOC_H__

/*=============  I N C L U D E S   =============*/
#include "adi_tool_chain.h"
#include "adi_contour_mem_alloc.h"

/*==============  D E F I N E S  ===============*/

/*=============  D A T A    T Y P E S   =============*/

/* Object type to represent memory blocks and its details */
typedef struct _MemBlock
{
    int8_t      *pNodeList;
    uint32_t    nTotalNodes;
    uint32_t    nNodeCounter;
} ADI_CONTOUR_MEM_BLOCK;

/* Structure to Private data required by pseudo memory alloc methods */
typedef struct _DynamicMemAllocPrivateData
{
    int8_t                  *pTotalMem;
    uint32_t                nTotalMemSize;
    uint32_t                nTotalMemCounter;
    ADI_CONTOUR_MEM_BLOCK   oRunLenHdr;
    ADI_CONTOUR_MEM_BLOCK   oBoundingRectangle;
    ADI_CONTOUR_MEM_BLOCK   oRunLenNode;
    ADI_CONTOUR_MEM_BLOCK   oSegmentHdr;
    ADI_CONTOUR_MEM_BLOCK   oEquSegmentHdr;
    ADI_CONTOUR_MEM_BLOCK   oEquSegmentNode;
    ADI_CONTOUR_MEM_BLOCK   oSegmentHdrRowWise;
    ADI_CONTOUR_MEM_BLOCK   oContourHdr;
    ADI_CONTOUR_MEM_BLOCK   oContourNode;
} CONTOUR_DYNAMIC_MEM_ALLOC_PRIVATE_DATA;

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/
extern
void*    PseudoDynamicMemAlloc(
                               ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                               ADI_CONTOUR_MEM_BLK_TYPE        nType,
                               uint32_t                        nCount
                               );

extern
void    PseudoDynamicMemFree(
                             ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                             void                            *pMem,
                             ADI_CONTOUR_MEM_BLK_TYPE        nType,
                             uint32_t                        nCount
                             );

extern
void    PseudoDynamicMemReset(
                              ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                              void                            *pMem,
                              ADI_CONTOUR_MEM_BLK_TYPE        nType,
                              uint32_t                        nCount
                              );
#endif /* __ADI_CONTOUR_PSEUDO_DYNAMIN_MEM_ALLOC_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_contour_pseudo_dynamic_mem_alloc.h $
**
*/
