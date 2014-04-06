/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : adi_mem_move.h

Description : Memory Movement API prototypes

*****************************************************************************/
#ifndef __ADI_MEM_MOVE_H__
#define __ADI_MEM_MOVE_H__

/*=============  I N C L U D E S   =============*/
#include "adi_tool_chain.h"

/*==============  D E F I N E S  ===============*/
#define ADI_DATA_DIRECT_FROM_TO_L3      ((uint32_t) 0)
#define ADI_DATA_MEMCOPY_FROM_TO_L3     ((uint32_t) 1)
#define ADI_DATA_DMA_FROM_TO_L3         ((uint32_t) 2)

#if defined(__ADSPBF609_FAMILY__)
typedef struct _adi_dma_mem_transfer
{
    int8_t      *pBaseBuffer;   /* Base Pointer */
    int32_t     nIncrement;     /* Increment after Transfer in L3 */

    /* Increment before Transfer applied to L1 Buffer */
    int32_t     nPreBufferIncrement;

    /* Input Configuration */
    int32_t     bInputEnable2D;
    uint32_t    nInputXCount;
    int32_t     nInputXModify;
    uint32_t    nInputYCount;
    int32_t     nInputYModify;

    /* Output Configuration */
    int32_t     bOutputEnable2D;
    uint32_t    nOutputXCount;
    int32_t     nOutputXModify;
    uint32_t    nOutputYCount;
    int32_t     nOutputYModify;

    /* Circular buffer variables */
    int32_t     nCircularBuffLen;   /* Circular buffer size / data bought using adi_2DChainDesc_MemMoveCircIn */

    /* Internal Variables */
    int8_t      *pCurrBuffer;
    int8_t      *pInOutBuffer;
    int32_t     nCircularBuffCurr;  /* Counter to hold the current place in the circular buffer */
} ADI_DMA_MEM_TRANSFER;
#else
typedef struct _adi_dma_mem_transfer
{
    int8_t      *pBaseBuffer;   /* Base Pointer */
    int32_t     nIncrement;     /* Increment after Transfer in L3 */

    /* Increment before Transfer applied to L1 Buffer */
    int32_t     nPreBufferIncrement;

    /* Input Configuration */
    int8_t      bInputEnable2D;
    uint16_t    nInputXCount;
    int16_t     nInputXModify;
    uint16_t    nInputYCount;
    int16_t     nInputYModify;

    /* Output Configuration */
    int8_t      bOutputEnable2D;
    uint16_t    nOutputXCount;
    int16_t     nOutputXModify;
    uint16_t    nOutputYCount;
    int16_t     nOutputYModify;

    /* Circular buffer variables */
    int32_t     nCircularBuffLen;   /* Circular buffer size / data bought using adi_2DChainDesc_MemMoveCircIn */

    /* Internal Variables */
    int8_t      *pCurrBuffer;
    int8_t      *pInOutBuffer;
    int32_t     nCircularBuffCurr;  /* Counter to hold the current place in the circular buffer */
} ADI_DMA_MEM_TRANSFER;
#endif

/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/

/* 1D DMA Functions */
extern
int32_t adi_MemMoveInit(
                        int8_t      *pInputBufL3,
                        int8_t      *pOutputBufL3,
                        uint32_t    nInputBytes,
                        uint16_t    nSizeOfBuf,
                        uint16_t    nIncr,
                        uint32_t    nModeOfDataTransfer,
                        int8_t      **pOutputBufCurr
                        );

extern
void    adi_MemMoveClose(void);

extern
void    adi_MemMoveReset(
                         int8_t      *pInputBufL3,
                         uint32_t    nInputBytes,
                         uint16_t    nSizeOfBuf,
                         uint16_t    nIncr
                         );

extern
int32_t adi_MemMoveIn(
                      uint16_t    nSizeOfBuf,
                      uint16_t    nIncr,
                      int8_t      **pInputBufCurr
                      );

extern
void    adi_MemMoveOut(
                       uint16_t    nSizeOfBuf,
                       uint16_t    nIncr,
                       int8_t      **pOutputBufCurr
                       );
extern                       
void adi_MemMoveOut_Spl(
                        uint16_t    nSizeOfBuf,
                        uint16_t    nIncr,
                        int8_t      *pOutputBuffer
                       );                       

extern
void    adi_MemMoveInEdge(
                          uint32_t    nFilterOrder,
                          uint32_t    dimX,
                          int8_t      **pInputBufCurr,
                          uint32_t    nEdge
                          );

extern
void    adi_MemMoveValue(
                         int8_t      *pBuff,
                         uint32_t    nSizeOfBuf,
                         uint32_t    nValue
                         );

extern
void    adi_MemMovePartialOut(
                              uint16_t    nSizeOfBuf,
                              int16_t     nModify,
                              int16_t     nIncr,
                              int8_t      **pOutputBufCurr
                              );

extern
int32_t adi_MemMovePartialIn(
                             uint16_t    nSizeOfBuf,
                             int16_t     nModify,
                             int16_t     nIncr,
                             int8_t      **pInputBufCurr
                             );

extern
void    adi_MemMoveModifyL3InputBuffer(
                                       int8_t      *pInputBufL3,
                                       uint32_t    nInputBytes,
                                       uint32_t    nIncr
                                       );

extern
void    adi_MemMoveModifyL3OutputBuffer(
                                        int8_t      *pOutputBufL3,
                                        uint32_t    nIncr,
                                        int8_t      **pOutputBufCurr
                                        );

/* 2D DMA Function with variable number of input and output */
extern
int32_t adi_2DChainDesc_MemMoveInit(
                                    ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                                    ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                    uint32_t                nInput,
                                    uint32_t                nOutput,
                                    uint32_t                nModeOfDataTransfer
                                    );

extern
void    adi_2DChainDesc_MemMoveIn(
                                  ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                                  uint32_t                nInput
                                  );

extern
void    adi_2DChainDesc_MemMoveOut(
                                   ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                   uint32_t                nOutput
                                   );

extern
int32_t adi_2DChainDesc_MemMoveCircInit(
                                        ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                                        ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                        uint32_t                nInput,
                                        uint32_t                nOutput,
                                        uint32_t                nModeOfDataTransfer
                                        );

extern
void    adi_2DChainDesc_MemMoveCircIn(
                                      ADI_DMA_MEM_TRANSFER  **pInputBufL3,
                                      uint32_t              nInput
                                      );

extern
void    adi_MemMoveInEdgeCirc(
                              uint32_t              nFilterOrder,
                              uint32_t              nEdge,
                              ADI_DMA_MEM_TRANSFER  *pInputBufL3
                              );

/*=============  D A T A    T Y P E S   =============*/
#endif /* __ADI_MEM_MOVE_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_mem_move.h $
**
*/
