/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9136 $
$Date: 2012-10-15 20:23:41 +0530 (Mon, 15 Oct 2012) $

Title       : adi_contours_wrapper.c

Description : wrapper for Run-Length Encoding of binary images for contour
              tracing

*****************************************************************************/
#if 1
/*=============  I N C L U D E S   =============*/
//#include "demo_application.h"
#include "adi_contours.h"
//#include "profile.h"
#include "adi_mem_move.h"
#include "adi_image_tool_box.h"
#include "adi_dma_vars.h"
#include <blackfin.h>

/*==============  D E F I N E S  ===============*/

/*=============  D A T A  =============*/
extern uint32_t                    nTotalRLENodes;
/*=============  C O D E  =============*/

/*
**  Function Prototype section
**  (static-scoped functions)
*/

/*
**  Function Definition section
*/

/******************************************************************************

  Function              : ADIContoursRLEWrapper

  Function description  : Wrapper function to Run-Length Encode input
                          binary image for contour tracing

  Parameters            :
    pDMA(In)            -   Pointer to Dynamic Memory Allocation Object.
    pIn(In)             -   Pointer to L3 Input buffer of binary image.
    nInputBytes(In)     -   Number of bytes in L3 input buffer.
    pRunLenListHdr(Out) -   Pointer to List of Run-Length Encoded nodes.
    dimX(In)            -   X dimension of input binary image.
    dimY(In)            -   Y dimension of input binary image.

  Returns               : ADI_ITB_MODULE_STATUS

  Notes                 :

******************************************************************************/

#pragma section("L1_code")
ADI_ITB_MODULE_STATUS ADIContoursRLEWrapper(
                                            ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                            int8_t                          *pIn,
                                            uint32_t                        nInputBytes,
                                            ADI_CONT_RUN_LEN_HDR            *pRunLenListHdr,
                                            uint32_t                        dimX,
                                            uint32_t                        dimY
                                            )
{
    int32_t                 nNumInputChannel;
    uint32_t                nNumLinesProcessed;
    int8_t                  *pInputBufCurr;
    uint32_t                nLineNo;
    uint32_t                j;
    ADI_ITB_MODULE_STATUS   nReturnValue = ADI_ITB_STATUS_SUCCESS;


    nNumInputChannel = 1;
    nLineNo = 0;


    adi_MemMoveInit(pIn,
                    0,
                    nInputBytes,
                    ADI_CONTOURS_NUM_LINES_IN_TILE * dimX * nNumInputChannel,
                    ADI_CONTOURS_NUM_LINES_IN_TILE * dimX * nNumInputChannel,
                    ADI_DATA_DMA_FROM_TO_L3,
                    0);

    nNumLinesProcessed = 0;
    do
    {
        adi_MemMoveIn(ADI_CONTOURS_NUM_LINES_IN_TILE * dimX * nNumInputChannel,
                      ADI_CONTOURS_NUM_LINES_IN_TILE * dimX * nNumInputChannel,
                      &pInputBufCurr);

        for (j = 0; j < ADI_CONTOURS_NUM_LINES_IN_TILE; j++)
        {
            pRunLenListHdr[nLineNo].pNext = 0;
            nReturnValue = adi_contour_RLE(pDMA,
                                           (uint8_t *)pInputBufCurr + j * dimX,
                                           dimX,
                                           nLineNo,
                                           &pRunLenListHdr[nLineNo]);
            nLineNo++;
            if (((nReturnValue & 0xFFFF) != (ADI_ITB_STATUS_SUCCESS)) || nTotalRLENodes > MAX_NUM_RUN_LEN_NODE)
            {
                adi_MemMoveClose();
            	*MDMA_S0CFG &= ~(BITM_DMA_CFG_EN); ;
            	*MDMA_D0CFG &= ~(BITM_DMA_CFG_EN); ;
                return (ADI_ITB_STATUS_FAILURE);
            }
        }


        nNumLinesProcessed += ADI_CONTOURS_NUM_LINES_IN_TILE;
    } while (nNumLinesProcessed < dimY);

    //if(nTotalRLENodes>6000)
    	//nReturnValue =  ADI_ITB_STATUS_FAILURE;
    //else
    	nReturnValue =  ADI_ITB_STATUS_SUCCESS;
    adi_MemMoveClose();
	*MDMA_S0CFG &= ~(BITM_DMA_CFG_EN); ;
	*MDMA_D0CFG &= ~(BITM_DMA_CFG_EN); ;
    return (nReturnValue);
}
#endif
/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_contours_wrapper.c $
**
*/
