/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : contour_dynamic_mem_alloc.c

Description : APIs for dynamic memory allocation using system memory
              allocation methods for contours.

*****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include <stddef.h>
#include <stdlib.h>
#include "adi_contour_dynamic_mem_alloc.h"

/*==============  D E F I N E S  ===============*/

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

  Function              : DynamicMemAlloc

  Function description  : Function to allocated nCount number of memory block
                          of type nType.

  Parameters            :
    pDMA(In)    -   Pointer to Dynamic Memory Allocation Object.
    nType(In)   -   Type of memory block (ADI_CONTOUR_MEM_BLK_TYPE).
    nCount(In)  -   Number of memory blocks of type given by nType.

  Returns               : Pointer to allocated memory block,
                          NULL if no block available.

  Notes                 :

******************************************************************************/
void* DynamicMemAlloc(
                      ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                      ADI_CONTOUR_MEM_BLK_TYPE        nType,
                      uint32_t                        nCount
                      )
{
    void    *pMemory;

    pMemory = NULL;
    switch (nType)
    {
    case ADI_CONTOUR_RUN_LEN_NODE:
        pMemory = malloc(sizeof(ADI_CONT_RUN_LEN_NODE) * nCount);
        break;

    case ADI_CONTOUR_RUN_LEN_HDR:
        pMemory = malloc(sizeof(ADI_CONT_RUN_LEN_HDR) * nCount);
        break;

    case ADI_CONTOUR_BOUNDING_RECT:
        pMemory = malloc(sizeof(ADI_CONT_BOUNDING_RECTANGLE) * nCount);
        break;

    case ADI_CONTOUR_SEG_HDR:
        pMemory = malloc(sizeof(ADI_CONT_SEGMENT_HDR) * nCount);
        break;

    case ADI_CONTOUR_SEG_HDR_ROW_WISE:
        pMemory = malloc(sizeof(ADI_CONT_SEGMENT_HDR_ROW_WISE) * nCount);
        break;

    case ADI_CONTOUR_EQU_SEG_NODE:
        pMemory = malloc(sizeof(ADI_CONT_EQU_SEGMENT_NODE) * nCount);
        break;

    case ADI_CONTOUR_EQU_SEG_HDR:
        pMemory = malloc(sizeof(ADI_CONT_EQU_SEGMENT_HDR) * nCount);
        break;

    case ADI_CONTOUR_NODE:
        pMemory = malloc(sizeof(ADI_CONT_NODE) * nCount);
        break;

    case ADI_CONTOUR_HDR:
        pMemory = malloc(sizeof(ADI_CONT_HDR) * nCount);
        break;
    }

    return (pMemory);
}

/******************************************************************************

  Function              : DynamicMemFree

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
void DynamicMemFree(
                    ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                    void                            *pMem,
                    ADI_CONTOUR_MEM_BLK_TYPE        nType,
                    uint32_t                        nCount
                    )
{
    free(pMem);
}

/******************************************************************************

  Function              : DynamicMemReset

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
void DynamicMemReset(
                     ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                     void                            *pMem,
                     ADI_CONTOUR_MEM_BLK_TYPE        nType,
                     uint32_t                        nCount
                     )
{
    ADI_CONT_HDR                *pContourListHdr;
    ADI_CONT_HDR                *pContourListHdrTemp;
    ADI_CONT_NODE               *pContourNodeTemp;
    ADI_CONT_SEGMENT_HDR        *pSegmentListHdr;
    ADI_CONT_SEGMENT_HDR        *pSegmentListHdrTemp;
    ADI_CONT_EQU_SEGMENT_HDR    *pEquSegmentListHdr;
    ADI_CONT_EQU_SEGMENT_HDR    *pEquSegmentListHdrTemp;
    ADI_CONT_EQU_SEGMENT_NODE   *pEquSegmentNode;
    ADI_CONT_EQU_SEGMENT_NODE   *pEquSegmentNodeTemp;
    ADI_CONT_RUN_LEN_HDR        *pRunLenListHdr;
    ADI_CONT_RUN_LEN_NODE       *pRunLenNode;
    ADI_CONT_RUN_LEN_NODE       *pRunLenNodeTemp;
    uint32_t                    j;

    switch (nType)
    {
    case ADI_CONTOUR_HDR:
        pContourListHdr = pMem;
        while (pContourListHdr)
        {
            pContourListHdrTemp = pContourListHdr;
            pContourListHdr = pContourListHdr->pDown;
            while
            (
                pContourListHdrTemp->pLeft
            &&  (pContourListHdrTemp->pLeft != pContourListHdrTemp->pRight)
            )
            {
                pContourNodeTemp = pContourListHdrTemp->pLeft;
                pContourListHdrTemp->pLeft = pContourListHdrTemp->pLeft->pNext;
                pDMA->pMemFree(pDMA, pContourNodeTemp, ADI_CONTOUR_NODE, 1);
            }

            if (pContourListHdrTemp->pLeft)
            {
                pDMA->pMemFree(pDMA,
                               pContourListHdrTemp->pLeft,
                               ADI_CONTOUR_NODE,
                               1);
            }

            pDMA->pMemFree(pDMA, pContourListHdrTemp, ADI_CONTOUR_HDR, 1);
        }

        break;

    case ADI_CONTOUR_SEG_HDR:
        pSegmentListHdr = pMem;
        while (pSegmentListHdr)
        {
            pSegmentListHdrTemp = pSegmentListHdr;
            pSegmentListHdr = pSegmentListHdr->pDown;
            pDMA->pMemFree(pDMA, pSegmentListHdrTemp, ADI_CONTOUR_SEG_HDR, 1);
        }

        break;

    case ADI_CONTOUR_EQU_SEG_HDR:
        pEquSegmentListHdr = pMem;
        while (pEquSegmentListHdr)
        {
            pEquSegmentListHdrTemp = pEquSegmentListHdr;
            pEquSegmentListHdr = pEquSegmentListHdr->pDown;
            pEquSegmentNode = pEquSegmentListHdrTemp->pNext;
            while (pEquSegmentNode)
            {
                pEquSegmentNodeTemp = pEquSegmentNode;
                pEquSegmentNode = pEquSegmentNode->pNext;
                pDMA->pMemFree(pDMA,
                               pEquSegmentNodeTemp,
                               ADI_CONTOUR_EQU_SEG_NODE,
                               1);
            }

            pDMA->pMemFree(pDMA,
                           pEquSegmentListHdrTemp,
                           ADI_CONTOUR_EQU_SEG_HDR,
                           1);
        }

        break;

    case ADI_CONTOUR_RUN_LEN_NODE:
        pRunLenListHdr = pMem;
        for (j = 0; j < nCount; j++)
        {
            pRunLenNode = pRunLenListHdr[j].pNext;
            while (pRunLenNode)
            {
                pRunLenNodeTemp = pRunLenNode;
                pRunLenNode = pRunLenNode->pNext;
                pDMA->pMemFree(pDMA,
                               pRunLenNodeTemp,
                               ADI_CONTOUR_RUN_LEN_NODE,
                               1);
            }
        }

        break;

    default:
        pDMA->pMemFree(pDMA, pMem, nType, nCount);
        break;
    }
}

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/contour_dynamic_mem_alloc.c $
**
*/
