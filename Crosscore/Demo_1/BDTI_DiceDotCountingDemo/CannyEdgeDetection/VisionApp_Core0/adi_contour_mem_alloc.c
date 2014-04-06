/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9091 $
$Date: 2012-10-14 18:20:39 +0530 (Sun, 14 Oct 2012) $

Title       : adi_contour_mem_alloc.c

Description : APIs for dynamic memory allocation init

*****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include "adi_tool_chain.h"
#include "adi_contour_mem_alloc.h"
#include "adi_contour_pseudo_dynamic_mem_alloc.h"
#include "adi_contour_dynamic_mem_alloc.h"
#include "adi_contours.h"
/*==============  D E F I N E S  ===============*/
//#define MAX_NUM_RUN_LEN_NODE            6000//100
#define MAX_NUM_SEGMENT_HDR             4000//40
#define MAX_NUM_SEGMENT_HDR_ROW_WISE    20
#define MAX_NUM_EQU_SEGMENT_HDR         2 * MAX_NUM_SEGMENT_HDR
#define MAX_NUM_EQU_SEGMENT_NODE        2 * MAX_NUM_EQU_SEGMENT_HDR
#define MAX_NUM_CONTOUR_HDR             160//10
#define MAX_NUM_BOUNDING_RECTANGLE      1

/*=============  D A T A  =============*/

/*=============  C O D E  =============*/

/*
**  Function Prototype section
**  (static-scoped functions)
*/

/*
**  Function Definition section
*/

/******************************************************************************

  Function              : adi_DynamicMemInit

  Function description  : Function to Initialize Dynamic Memory Allocation
                          for Contours.
                          It assigns required functions to function pointers
                          and creates nodes required for pseudo memory allocation.

  Parameters            :
    pDMA(Out)       -   Pointer to Dynamic Memory Allocation Object.
    pMem(In)        -   Pointer to memory that is to be used for memory allocation.
    nSizeOfMem(In)  -   Total size of the memory pointed to by pMem.
    nHeight(In)     -   Height of input binary image.

  Returns               : ADI_ITB_MODULE_STATUS

  Notes                 : By default HEAP_MEM_ALLOC code is
                          disabled; enabling pseudo memory allocation.

******************************************************************************/
#pragma section("adi_fast_prio1_code")
ADI_ITB_MODULE_STATUS adi_DynamicMemInit(
                                         ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                         int8_t                          *pMem,
                                         uint32_t                        nSizeOfMem,
                                         uint32_t                        nHeight
                                         )
{
#ifdef HEAP_MEM_ALLOC
    pDMA->pDynamicMemAllocPrivateData = NULL;
    pDMA->pMemAlloc = DynamicMemAlloc;
    pDMA->pMemFree = DynamicMemFree;
    pDMA->pMemReset = DynamicMemReset;
#else
    CONTOUR_DYNAMIC_MEM_ALLOC_PRIVATE_DATA  *pDynamicMemAllocPrivateData;
    if (nSizeOfMem < sizeof(CONTOUR_DYNAMIC_MEM_ALLOC_PRIVATE_DATA))
    {
        return ((ADI_ITB_MODULE_CONTOURS << 16) | ADI_ITB_STATUS_INSUFFICIENT_MEM);
    }

    pDMA->pMemAlloc = PseudoDynamicMemAlloc;
    pDMA->pMemFree = PseudoDynamicMemFree;
    pDMA->pMemReset = PseudoDynamicMemReset;

    pDMA->pDynamicMemAllocPrivateData = pMem;
    pDynamicMemAllocPrivateData = pDMA->pDynamicMemAllocPrivateData;

    pDynamicMemAllocPrivateData->pTotalMem = pMem;
    pDynamicMemAllocPrivateData->nTotalMemSize = nSizeOfMem;
    pDynamicMemAllocPrivateData->nTotalMemCounter = sizeof(CONTOUR_DYNAMIC_MEM_ALLOC_PRIVATE_DATA);

    /* create nodes */
    {
        uint32_t        nRunLenHdrSize;
        uint32_t        nBoundingRectangleSize;
        uint32_t        nRunLenNodeSize;
        uint32_t        nSegmentHdrSize;
        uint32_t        nEquSegmentHdrSize;
        uint32_t        nEquSegmentNodeSize;
        uint32_t        nSegmentHdrRowWiseSize;
        uint32_t        nContourHdrSize;
        uint32_t        nContourNodeSize;
        uint32_t        nTotalSize;
        uint32_t        i;
        ADI_CONT_HDR    *pContourHdr;
        ADI_CONT_NODE   *pContourNode;

        nRunLenHdrSize = sizeof(ADI_CONT_RUN_LEN_HDR) * nHeight;
        nBoundingRectangleSize = sizeof(ADI_CONT_BOUNDING_RECTANGLE) * MAX_NUM_BOUNDING_RECTANGLE;
        nRunLenNodeSize = sizeof(ADI_CONT_RUN_LEN_NODE) * MAX_NUM_RUN_LEN_NODE;
        nSegmentHdrSize = sizeof(ADI_CONT_SEGMENT_HDR) * MAX_NUM_SEGMENT_HDR;
        nEquSegmentHdrSize = sizeof(ADI_CONT_EQU_SEGMENT_HDR) * MAX_NUM_EQU_SEGMENT_HDR;
        nEquSegmentNodeSize = sizeof(ADI_CONT_EQU_SEGMENT_NODE) * MAX_NUM_EQU_SEGMENT_NODE;
        nSegmentHdrRowWiseSize = sizeof(ADI_CONT_SEGMENT_HDR_ROW_WISE) * MAX_NUM_SEGMENT_HDR_ROW_WISE;
        nContourHdrSize = sizeof(ADI_CONT_HDR) * MAX_NUM_CONTOUR_HDR;
        nTotalSize = nRunLenHdrSize +
            nBoundingRectangleSize +
            nRunLenNodeSize +
            nSegmentHdrSize +
            nEquSegmentHdrSize +
            nEquSegmentNodeSize +
            nSegmentHdrRowWiseSize +
            nContourHdrSize;

        if
        (
            (
                pDynamicMemAllocPrivateData->nTotalMemSize -
                pDynamicMemAllocPrivateData->nTotalMemCounter
            ) < nTotalSize
        )
        {
            return ((ADI_ITB_MODULE_CONTOURS << 16) | ADI_ITB_STATUS_INSUFFICIENT_MEM);
        }

        pDynamicMemAllocPrivateData->oRunLenHdr.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oRunLenHdr.nTotalNodes = nHeight;
        pDynamicMemAllocPrivateData->oRunLenHdr.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nRunLenHdrSize;

        pDynamicMemAllocPrivateData->oBoundingRectangle.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oBoundingRectangle.nTotalNodes = MAX_NUM_BOUNDING_RECTANGLE;
        pDynamicMemAllocPrivateData->oBoundingRectangle.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nBoundingRectangleSize;

        pDynamicMemAllocPrivateData->oRunLenNode.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oRunLenNode.nTotalNodes = MAX_NUM_RUN_LEN_NODE;
        pDynamicMemAllocPrivateData->oRunLenNode.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nRunLenNodeSize;

        pDynamicMemAllocPrivateData->oSegmentHdr.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oSegmentHdr.nTotalNodes = MAX_NUM_SEGMENT_HDR;
        pDynamicMemAllocPrivateData->oSegmentHdr.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nSegmentHdrSize;

        pDynamicMemAllocPrivateData->oEquSegmentHdr.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oEquSegmentHdr.nTotalNodes = MAX_NUM_EQU_SEGMENT_HDR;
        pDynamicMemAllocPrivateData->oEquSegmentHdr.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nEquSegmentHdrSize;

        pDynamicMemAllocPrivateData->oEquSegmentNode.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oEquSegmentNode.nTotalNodes = MAX_NUM_EQU_SEGMENT_NODE;
        pDynamicMemAllocPrivateData->oEquSegmentNode.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nEquSegmentNodeSize;

        pDynamicMemAllocPrivateData->oSegmentHdrRowWise.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nTotalNodes = MAX_NUM_SEGMENT_HDR_ROW_WISE;
        pDynamicMemAllocPrivateData->oSegmentHdrRowWise.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nSegmentHdrRowWiseSize;

        pDynamicMemAllocPrivateData->oContourHdr.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oContourHdr.nTotalNodes = MAX_NUM_CONTOUR_HDR;
        pDynamicMemAllocPrivateData->oContourHdr.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nContourHdrSize;
        pContourHdr = (ADI_CONT_HDR *)pDynamicMemAllocPrivateData->oContourHdr.pNodeList;
        for (i = 0; i < MAX_NUM_CONTOUR_HDR; i++)
        {
            pContourHdr->bInUse = 0;
            pContourHdr++;
        }

        pDynamicMemAllocPrivateData->oContourNode.pNodeList = pDynamicMemAllocPrivateData->pTotalMem + pDynamicMemAllocPrivateData->nTotalMemCounter;
        nContourNodeSize = pDynamicMemAllocPrivateData->nTotalMemSize - pDynamicMemAllocPrivateData->nTotalMemCounter;
        pDynamicMemAllocPrivateData->oContourNode.nTotalNodes = nContourNodeSize/sizeof(ADI_CONT_NODE);
        pDynamicMemAllocPrivateData->oContourNode.nNodeCounter = 0;
        pDynamicMemAllocPrivateData->nTotalMemCounter += nContourNodeSize;
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
    }

#endif
    return ((ADI_ITB_MODULE_CONTOURS << 16) | ADI_ITB_STATUS_SUCCESS);
}

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_contour_mem_alloc.c $
**
*/
