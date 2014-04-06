/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9084 $
$Date: 2012-10-14 12:38:57 +0530 (Sun, 14 Oct 2012) $

Title       : contour_pseudo_dynamic_mem_alloc.c

Description : APIs for pseudo dynamic memory allocation for contours.

*****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include <stddef.h>
#include <stdlib.h>
#include "adi_contour_pseudo_dynamic_mem_alloc.h"

/*==============  D E F I N E S  ===============*/

/*=============  D A T A  =============*/
extern uint32_t                        nTotalRLENodes;

/*=============  C O D E  =============*/

/*
**  Function Prototype section
**  (static-scoped functions)
*/
static void *SearchEmptyNode(
                             int8_t      *pNodeList,
                             uint32_t    nCount,
                             uint32_t    nOffset
                             );

/*
**  Function Definition section
*/

/******************************************************************************

  Function              : PseudoDynamicMemAlloc

  Function description  : Function to allocated nCount number of memory block
                          of type nType.

  Parameters            :
    pDMA(In)    -   Pointer to Dynamic Memory Allocation Object.
    nType(In)   -   Type of memory block (ADI_CONTOUR_MEM_BLK_TYPE).
    nCount(In)  -   Number of memory blocks of type given by nType.

  Returns               : Pointer to allocated memory block,
                          NULL if no block is available.

  Notes                 :

******************************************************************************/

#pragma section("adi_fast_prio0_code")
void *PseudoDynamicMemAlloc(
                            ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                            ADI_CONTOUR_MEM_BLK_TYPE        nType,
                            uint32_t                        nCount
                            )
{
    void                                    *pMemory;
    CONTOUR_DYNAMIC_MEM_ALLOC_PRIVATE_DATA  *pDynamicMemAllocPrivateData;
    ADI_CONT_HDR                            *pContourHdr;
    ADI_CONT_NODE                           *pContourNode;

    pMemory = 0;
    pDynamicMemAllocPrivateData = pDMA->pDynamicMemAllocPrivateData;

    switch (nType)
    {
    case ADI_CONTOUR_RUN_LEN_HDR:
        if (nCount <= (pDynamicMemAllocPrivateData->oRunLenHdr.nTotalNodes - pDynamicMemAllocPrivateData->oRunLenHdr.nNodeCounter))
        {
            pMemory = pDynamicMemAllocPrivateData->oRunLenHdr.pNodeList + (pDynamicMemAllocPrivateData->oRunLenHdr.nNodeCounter*sizeof(ADI_CONT_RUN_LEN_HDR));
            pDynamicMemAllocPrivateData->oRunLenHdr.nNodeCounter += nCount;
        }

        break;

    case ADI_CONTOUR_BOUNDING_RECT:
        if (nCount <= (pDynamicMemAllocPrivateData->oBoundingRectangle.nTotalNodes - pDynamicMemAllocPrivateData->oBoundingRectangle.nNodeCounter))
        {
            pMemory = pDynamicMemAllocPrivateData->oBoundingRectangle.pNodeList + (pDynamicMemAllocPrivateData->oBoundingRectangle.nNodeCounter*sizeof(ADI_CONT_BOUNDING_RECTANGLE));
            pDynamicMemAllocPrivateData->oBoundingRectangle.nNodeCounter += nCount;
        }

        break;

    case ADI_CONTOUR_RUN_LEN_NODE:
        if (nCount <= (pDynamicMemAllocPrivateData->oRunLenNode.nTotalNodes - pDynamicMemAllocPrivateData->oRunLenNode.nNodeCounter))
        {
            pMemory = pDynamicMemAllocPrivateData->oRunLenNode.pNodeList + (pDynamicMemAllocPrivateData->oRunLenNode.nNodeCounter*sizeof(ADI_CONT_RUN_LEN_NODE));
            pDynamicMemAllocPrivateData->oRunLenNode.nNodeCounter += nCount;
            nTotalRLENodes +=  nCount; // added to check run-length nodes and limit
        }

        break;

    case ADI_CONTOUR_SEG_HDR:
        if (nCount <= (pDynamicMemAllocPrivateData->oSegmentHdr.nTotalNodes - pDynamicMemAllocPrivateData->oSegmentHdr.nNodeCounter))
        {
            pMemory = pDynamicMemAllocPrivateData->oSegmentHdr.pNodeList + (pDynamicMemAllocPrivateData->oSegmentHdr.nNodeCounter*sizeof(ADI_CONT_SEGMENT_HDR));
            pDynamicMemAllocPrivateData->oSegmentHdr.nNodeCounter += nCount;
        }

        break;

    case ADI_CONTOUR_EQU_SEG_HDR:
        if (nCount <= (pDynamicMemAllocPrivateData->oEquSegmentHdr.nTotalNodes - pDynamicMemAllocPrivateData->oEquSegmentHdr.nNodeCounter))
        {
            pMemory = pDynamicMemAllocPrivateData->oEquSegmentHdr.pNodeList + (pDynamicMemAllocPrivateData->oEquSegmentHdr.nNodeCounter*sizeof(ADI_CONT_EQU_SEGMENT_HDR));
            pDynamicMemAllocPrivateData->oEquSegmentHdr.nNodeCounter += nCount;
        }

        break;

    case ADI_CONTOUR_EQU_SEG_NODE:
        if (nCount <= (pDynamicMemAllocPrivateData->oEquSegmentNode.nTotalNodes - pDynamicMemAllocPrivateData->oEquSegmentNode.nNodeCounter))
        {
            pMemory = pDynamicMemAllocPrivateData->oEquSegmentNode.pNodeList + (pDynamicMemAllocPrivateData->oEquSegmentNode.nNodeCounter*sizeof(ADI_CONT_EQU_SEGMENT_NODE));
            pDynamicMemAllocPrivateData->oEquSegmentNode.nNodeCounter += nCount;
        }

        break;

    case ADI_CONTOUR_SEG_HDR_ROW_WISE:
        if (nCount <= (pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nTotalNodes - pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nNodeCounter))
        {
            pMemory = pDynamicMemAllocPrivateData->oSegmentHdrRowWise.pNodeList + (pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nNodeCounter*sizeof(ADI_CONT_SEGMENT_HDR_ROW_WISE));
            pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nNodeCounter += nCount;
        }

        break;

    case ADI_CONTOUR_HDR:

        /* only 1 node is allowed */
        pContourHdr = (ADI_CONT_HDR *)(pDynamicMemAllocPrivateData->oContourHdr.pNodeList+pDynamicMemAllocPrivateData->oContourHdr.nNodeCounter*sizeof(ADI_CONT_HDR));
        if (pContourHdr->bInUse)
        {
            pMemory = SearchEmptyNode(pDynamicMemAllocPrivateData->oContourHdr.pNodeList,
                                      pDynamicMemAllocPrivateData->oContourHdr.nTotalNodes,
                                      sizeof(ADI_CONT_HDR));
        }
        else
        {
            pMemory = pContourHdr;
            pContourHdr->bInUse = 1;
            pDynamicMemAllocPrivateData->oContourHdr.nNodeCounter += 1;
        }

        break;

    case ADI_CONTOUR_NODE:

        /* only 1 node is allowed */
        pContourNode = (ADI_CONT_NODE *)(pDynamicMemAllocPrivateData->oContourNode.pNodeList+pDynamicMemAllocPrivateData->oContourNode.nNodeCounter*sizeof(ADI_CONT_NODE));
        if (pContourNode->bInUse)
        {
            pMemory = SearchEmptyNode(pDynamicMemAllocPrivateData->oContourNode.pNodeList,
                                      pDynamicMemAllocPrivateData->oContourNode.nTotalNodes,
                                      sizeof(ADI_CONT_NODE));
        }
        else
        {
            pMemory = pContourNode;
            pContourNode->bInUse = 1;
            pDynamicMemAllocPrivateData->oContourNode.nNodeCounter += 1;
        }

        break;

    default:
        exit(0);
    }

    return (pMemory);
}

/******************************************************************************

  Function              : PseudoDynamicMemFree

  Function description  : Function to free/collect nCount number of memory
                          block of type nType; pointed to by pMem.

  Parameters            :
    pDMA(In)        -   Pointer to Dynamic Memory Allocation Object.
    pMem(In/Out)    -   Pointer to memory block to be freed.
    nType(In)       -   Type of memory block (ADI_CONTOUR_MEM_BLK_TYPE)
    nCount(In)      -   Number of memory blocks of type given by nType.

  Returns               : Nothing

  Notes                 :

******************************************************************************/

#pragma section("adi_fast_prio0_code")
void PseudoDynamicMemFree(
                          ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                          void                            *pMem,
                          ADI_CONTOUR_MEM_BLK_TYPE        nType,
                          uint32_t                        nCount
                          )
{
    CONTOUR_DYNAMIC_MEM_ALLOC_PRIVATE_DATA  *pDynamicMemAllocPrivateData;
    ADI_CONT_HDR                            *pContourHdr;
    ADI_CONT_NODE                           *pContourNode;

    pDynamicMemAllocPrivateData = pDMA->pDynamicMemAllocPrivateData;

    switch (nType)
    {
    case ADI_CONTOUR_RUN_LEN_HDR:
        pDynamicMemAllocPrivateData->oRunLenHdr.nNodeCounter -= nCount;
        break;

    case ADI_CONTOUR_BOUNDING_RECT:
        pDynamicMemAllocPrivateData->oBoundingRectangle.nNodeCounter -= nCount;
        break;

    case ADI_CONTOUR_RUN_LEN_NODE:
        pDynamicMemAllocPrivateData->oRunLenNode.nNodeCounter -= nCount;
        break;

    case ADI_CONTOUR_SEG_HDR:
        pDynamicMemAllocPrivateData->oSegmentHdr.nNodeCounter -= nCount;
        break;

    case ADI_CONTOUR_EQU_SEG_HDR:
        pDynamicMemAllocPrivateData->oEquSegmentHdr.nNodeCounter -= nCount;
        break;

    case ADI_CONTOUR_EQU_SEG_NODE:
        pDynamicMemAllocPrivateData->oEquSegmentNode.nNodeCounter -= nCount;
        break;

    case ADI_CONTOUR_SEG_HDR_ROW_WISE:
        pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nNodeCounter -= nCount;
        break;

    case ADI_CONTOUR_HDR:

        /* only 1 node is allowed */
        pContourHdr = pMem;
        pContourHdr->bInUse = 0;
        break;

    case ADI_CONTOUR_NODE:

        /* only 1 node is allowed */
        pContourNode = pMem;
        pContourNode->bInUse = 0;
        break;

    default:
        exit(0);
    }
}

/******************************************************************************

  Function              : PseudoDynamicMemReset

  Function description  : Function to free/collect nCount number of memory
                          block of type nType; pointed to by pMem and
                          its sub-nodes.

  Parameters            :
    pDMA(In)        -   Pointer to Dynamic Memory Allocation Object.
    pMem(In/Out)    -   Pointer to parent memory block to be reset.
    nType(In)       -   Type of memory block (ADI_CONTOUR_MEM_BLK_TYPE)
    nCount(In)      -   Number of memory blocks of type given by nType.

  Returns               : Nothing

  Notes                 :

******************************************************************************/

#pragma section("adi_fast_prio0_code")
void PseudoDynamicMemReset(
                           ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                           void                            *pMem,
                           ADI_CONTOUR_MEM_BLK_TYPE        nType,
                           uint32_t                        nCount
                           )
{
    CONTOUR_DYNAMIC_MEM_ALLOC_PRIVATE_DATA  *pDynamicMemAllocPrivateData;
    uint32_t                                i;
    ADI_CONT_HDR                            *pContourHdr;
    ADI_CONT_NODE                           *pContourNode;

    pDynamicMemAllocPrivateData = pDMA->pDynamicMemAllocPrivateData;

    switch (nType)
    {
    case ADI_CONTOUR_BOUNDING_RECT:
        pDynamicMemAllocPrivateData->oBoundingRectangle.nNodeCounter = 0;
        break;

    case ADI_CONTOUR_SEG_HDR_ROW_WISE:
        pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nNodeCounter = 0;
        break;

    case ADI_CONTOUR_HDR:
        pDynamicMemAllocPrivateData->oContourNode.nNodeCounter = 0;
        pContourNode = (ADI_CONT_NODE *)pDynamicMemAllocPrivateData->oContourNode.pNodeList;
        for
        (
            i = 0;
            i < pDynamicMemAllocPrivateData->oContourNode.nTotalNodes;
            i++
        )
        {
            pContourNode->bInUse = 0;
            pContourNode++;
        }

        pDynamicMemAllocPrivateData->oContourHdr.nNodeCounter = 0;

        pContourHdr = (ADI_CONT_HDR *)pDynamicMemAllocPrivateData->oContourHdr.pNodeList;
        for
        (
            i = 0;
            i < pDynamicMemAllocPrivateData->oContourHdr.nTotalNodes;
            i++
        )
        {
            pContourHdr->bInUse = 0;
            pContourHdr++;
        }

        break;

    case ADI_CONTOUR_SEG_HDR:
        pDynamicMemAllocPrivateData->oSegmentHdr.nNodeCounter = 0;
        break;

    case ADI_CONTOUR_EQU_SEG_HDR:
        pDynamicMemAllocPrivateData->oEquSegmentHdr.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->oEquSegmentNode.nNodeCounter = 0;
        break;

    case ADI_CONTOUR_RUN_LEN_NODE:
        pDynamicMemAllocPrivateData->oRunLenNode.nNodeCounter = 0;
        break;
    }
}

/******************************************************************************

  Function              : SearchEmptyNode

  Function description  : Function to search an empty node in the list pNodeList.

  Parameters            :
    pNodeList(In)   -   Pointer to memory block list.
    nCount(In)      -   Total number of memory blocks in list pNodeList.
    nOffset(In)     -   size of memory bloack.

  Returns               : Pointer to free memory block,
                          NULL if no block available.

  Notes                 :

******************************************************************************/

#pragma section("adi_fast_prio0_code")
static void *SearchEmptyNode(
                             int8_t      *pNodeList,
                             uint32_t    nCount,
                             uint32_t    nOffset
                             )
{
    uint32_t    i;

    for (i = 0; i < nCount; i++)
    {
        if (*(pNodeList + i * nOffset) == 0)
        {
            *(pNodeList + i * nOffset) = 1;
            return (pNodeList + i * nOffset);
        }
    }

    return (NULL);
}

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/contour_pseudo_dynamic_mem_alloc.c $
**
*/
