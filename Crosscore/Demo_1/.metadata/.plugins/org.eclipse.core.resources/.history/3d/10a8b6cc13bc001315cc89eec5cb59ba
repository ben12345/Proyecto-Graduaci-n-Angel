/*********************************************************************************

Copyright(c) 2011 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
* @file     ProcesPEC.c
*
* @brief     This is the primary source file for  processing PEC
*
* @details   This file contains all the necessary initialization function including the VSS module initialization.
*            for demonstrating the  edge extraction.
*
*/

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"Suppress MISRA checking for example code")
#endif
#include <stdio.h>
#include <stddef.h>
#include <blackfin.h>
#include <string.h>
#include <builtins.h>
#include "adi_common.h"
/* Interrupt Manager includes */
#include <services/int/adi_int.h>
/* DMA Manager includes */
#include <services/dma/adi_dma.h>
#include "glut.h"
extern     GLshort	aVertices[8];

#define PEC_EDGE_INFO  0X01

#define PEC_EDGE_VALUE 0X05

#define EDGE_DATA     0XFF80

#define NON_EDGE_DATA 0X0080

/* DMA Stream Handle */
ADI_DMA_STREAM_HANDLE   hMemDmaStreamIn;
/* Source DMA Handle */
ADI_DMA_CHANNEL_HANDLE  hSrcDmaChannelIn;
/* Destination DMA Handle */
ADI_DMA_CHANNEL_HANDLE  hDestDmaChannelIn;
/* Memory to handle DMA Stream */

static uint8_t MemDmaStreamMemIn[ADI_DMA_STREAM_REQ_MEMORY];
/* DMA Stream Handle */
ADI_DMA_STREAM_HANDLE   hMemDmaStreamOut;
/* Source DMA Handle */
ADI_DMA_CHANNEL_HANDLE  hSrcDmaChannelOut;
/* Destination DMA Handle */
ADI_DMA_CHANNEL_HANDLE  hDestDmaChannelOut;
/* Memory to handle DMA Stream */




//#pragma alignment_region (32)
//#pragma section ("L1_data_b")
//uint8_t InPingBuf[INPUT_VIDEO_WIDTH];
//#pragma align 4
//#pragma section ("L1_data_a")
//uint8_t InPongBuf[INPUT_VIDEO_WIDTH ];
//#pragma alignment_region_end

static  volatile bool  bDataIn= false;
extern int8_t aTempBuffer1[], aTempBuffer3[];
void  *pInBuffer[]= {aTempBuffer1,aTempBuffer3};

#pragma section("adi_fastb1_app_rw")
GLubyte                 aColors[] = {
                                    0,0,255,0x7F,  //-->Red colour, alpha is 7F
                                    0,255,0,0x7F,  //-->Green colour, alpha is 7F
                                    255,0,0,0x7F,  //-->Blue colour, alpha is 7F
                                    255,0,255,0x7F,//-->Magneta colour, alpha is 7F
                                    };


/*****************************************************************************

    Function: MemDmaCallback

        Callback from MDMA Manager for inbound transfer

    Parameters:
        pCBParam    - Callback parameter supplied by application
        Event       - Callback event
        pArg        - Callback argument

*****************************************************************************/
static void MemDmaCallbackIn(void *pCBParam, uint32_t Event, void *pArg)
{
    /* CASEOF (Event) */
    switch (Event)
    {
    	/* CASE (Processed a one-shot/circular buffer) */
    	case ADI_DMA_EVENT_BUFFER_PROCESSED:
    		bDataIn = true;
    		break;
        default:
            printf("Callback occured. Event code: 0x%08X\n", Event);
            break;
    }
}
uint32_t InitEdgeDraw(void)
{

	ADI_DMA_RESULT eResult;

    if ((eResult = adi_mdma_Open (ADI_DMA_MEMDMA_S1,
                                  &MemDmaStreamMemIn[0],
                                  &hMemDmaStreamIn,
                                  &hSrcDmaChannelIn,
                                  &hDestDmaChannelIn,
                                  MemDmaCallbackIn,
                                  NULL)) != ADI_DMA_SUCCESS)
    {
        printf ("Open DMA Stream, Stream Index = %d, Error code: 0x%08X\n", ADI_DMA_MEMDMA_S0, eResult);
        return(FAILURE);

    }
    if ((eResult = adi_dma_UpdateCallback (hDestDmaChannelIn,
    		                               MemDmaCallbackIn,
    		                               hMemDmaStreamIn)) != ADI_DMA_SUCCESS)
    {
    	printf ("Failed to set DMA callback, Stream Index = %d, Error code: 0x%08X\n", ADI_DMA_MEMDMA_S0, eResult);
        return(FAILURE);
    }
    return(SUCCESS);
}


void DrawEdge(void *pBufferIn)
{
	ADI_DMA_RESULT eResult;
	uint32_t i,j,nInIndex,nStatus;
    /* pointer to input buffer */
    uint8_t *pPecOut =(uint8_t *) pBufferIn;
    uint32_t nDisplayColor = 0x7FFF0000; //0x7FFFD700;
    /* Set the in-bound data transfer  flag as false since we have to wait first time inside the loop */
    bDataIn = false;
    /* Pointer for indexing  the input data*/
    uint8_t *pPtrDataIn;
    nInIndex = 0;
#if 1
    *pREG_DMA23_CFG &= ~(BITM_DMA_CFG_EN);
    *pREG_DMA24_CFG &= ~(BITM_DMA_CFG_EN);
    /* Submit the buffer inbound data transfer (L3->L1) */
    eResult = adi_mdma_Copy1D (hMemDmaStreamIn,
    		                    pInBuffer[nInIndex],
    		                    pPecOut,
    		                    ADI_DMA_MSIZE_16BYTES,
                               (INPUT_VIDEO_WIDTH)>>4);
    /* advance the input pointer by FRAME_WIDTH */
    pPecOut += INPUT_VIDEO_WIDTH;
    /* Toggle the ping-pong index  */
    nInIndex =1-nInIndex;

    aColors[0] =  nDisplayColor;
    aColors[1] = (nDisplayColor>>8);
    aColors[2] = (nDisplayColor>>16);
    aColors[3] = (nDisplayColor>>24);

    /* Set the current vertices to use from vertices array. */
    glVertexPointer(2, GL_SHORT, 0, aVertices);

    /* Set the current vertices to use color from colors array. */
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, aColors);

    /* Processs the PEC output for all the lines */
    for(i=0;i<(INPUT_VIDEO_HEIGHT-1);i++)
    {
     /* Wait till the in-bound DMA is over */
     while(bDataIn == false);

     /* Reset status as flag before starting another in-bound DMA operation  */
     bDataIn = false;
     /* Submit the buffer inbound data transfer (L3->L1) */
    eResult = adi_mdma_Copy1D (hMemDmaStreamIn,
    	                        pInBuffer[nInIndex],
    		                    pPecOut,
    		                    ADI_DMA_MSIZE_16BYTES,
                                (INPUT_VIDEO_WIDTH)>>4);
    /* Toggle the ping-pong index  */
    nInIndex =1-nInIndex;
    /* advance the input pointer by FRAME_WIDTH */
    pPecOut += INPUT_VIDEO_WIDTH;
    /* Set the pointer from where data to be read */
    pPtrDataIn =  pInBuffer[nInIndex];

    /* Process all the pixels */
    for(j=0;j< INPUT_VIDEO_WIDTH;j++)
    {
        /* Is it a strong edge ? */
    	if((pPtrDataIn[j]& PEC_EDGE_INFO) == 1 )
    	{
            /* Mark as "white" pixel */

    	    aVertices[0] = j ;
    	    aVertices[1] = i + Y_OFFSET_FOR_VIDEO_POSITION;
    	    /* Added condition check to prevent bounding rectangle draw near the boundary. Since the overlay buffer is larger than the
    	     * video area, any drawing at the edges will cause the blending to gradually spread outside the video boundary. The below values
    	     * are the limits taking into consideration the macroblock blending process */
            if(aVertices[0] < 16 || aVertices[1] <= (Y_OFFSET_FOR_VIDEO_POSITION+16) || aVertices[1] >= (Y_OFFSET_FOR_VIDEO_POSITION + INPUT_VIDEO_HEIGHT-16) || aVertices[0] >= (INPUT_VIDEO_WIDTH-16))
            	continue;
    	    glDrawArrays(GL_POINTS, 0, 1);
    	}

    }
}
#else
    aColors[0] =  nDisplayColor;
    aColors[1] = (nDisplayColor>>8);
    aColors[2] = (nDisplayColor>>16);
    aColors[3] = (nDisplayColor>>24);

    /* Set the current vertices to use from vertices array. */
    glVertexPointer(2, GL_SHORT, 0, aVertices);

    /* Set the current vertices to use color from colors array. */
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, aColors);

    for(i=0;i<(INPUT_VIDEO_HEIGHT-1);i++)
    {
       /* Process all the pixels */
       for(j=0;j< INPUT_VIDEO_WIDTH;j++)
       {
        /* Is it a strong edge ? */
    	    if((pPecOut[i*INPUT_VIDEO_WIDTH+j]& PEC_EDGE_INFO)  )
    	    {
                /* Mark as "white" pixel */

    	        aVertices[0] = j ;
    	        aVertices[1] = i+120;
       	        glDrawArrays(GL_POINTS, 0, 1);
    	    }

       }
   }

#endif

    *pREG_DMA23_CFG &= ~(BITM_DMA_CFG_EN);
    *pREG_DMA24_CFG &= ~(BITM_DMA_CFG_EN);

}
