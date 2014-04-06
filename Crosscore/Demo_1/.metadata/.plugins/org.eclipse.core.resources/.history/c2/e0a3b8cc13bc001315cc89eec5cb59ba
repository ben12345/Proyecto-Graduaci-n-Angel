/*****************************************************************************
Copyright (c) 2011 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9136 $
$Date: 2012-10-15 20:23:41 +0530 (Mon, 15 Oct 2012) $

Title       : adi_mem_move_bf609.c

Description : Memory Movement code for BF609

*****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include <stdio.h>
#include <stddef.h>
#include "adi_tool_chain.h"
#include "adi_mem_move.h"
#include "adi_memory.h"
#include "adi_dma_vars.h"
#include "adi_dma_func.h"
#include <blackfin.h>

/*==============  D E F I N E S  ===============*/
#define CONFIG_OFFSET                   ((uint32_t) 0x08U)
#define CURR_DESC_PTR_OFFSET            ((uint32_t) 0x24U)
#define DEST_REG_OFFSET                 ((uint32_t) 0x80U)

/* Should be Minimum 1 */
#define ADI_DMA_MAX_INPUT   2

/* Should be Minimum 1 */
#define ADI_DMA_MAX_OUTPUT  2

#define DMADESC(addr, sa, cfg, xc, xm, yc, ym)                                                          \
    asm("i0=%0;  w[i0++]=%1.l; w[i0++]=%1.h; [i0++]=%2; [i0++]=%3; [i0++]=%4; [i0++]=%5; [i0++]=%6; "   \
        : : "a"(addr),                                                                                  \
            "d"(sa),                                                                                    \
            "d"(cfg),                                                                                   \
            "d"(xc),                                                                                    \
            "d"(xm),                                                                                    \
            "d"(yc),                                                                                    \
            "d"(ym))
/* Made 16bit DMA transfer */
#define ADI_DMA_MSIZE_BYTES ENUM_DMA_CFG_MSIZE16

#define DMASIZE     (1 << (ADI_DMA_MSIZE_BYTES >> 8))   /* 2-byte DMA */
#define DDMAEN      ENUM_DMA_CFG_EN              /* Channel Enable */
#define DWNR        ENUM_DMA_CFG_WRITE              /* Channel Direction (W/R*) */
#define DWDSIZE     ADI_DMA_MSIZE_BYTES   /* Word Size 16 bits */
#define DW2D        ENUM_DMA_CFG_ADDR2D              /* Enable 2D */
#define DND_SIZE    ENUM_DMA_CFG_FETCH06              /* Next Descriptor Size */
#define DFLOW_ARRAY ENUM_DMA_CFG_DSCARRAY              /* Flow (descriptor array mode) */

#define SRCCFG      (DFLOW_ARRAY | DND_SIZE | DDMAEN)
#define DSTCFG      (DFLOW_ARRAY | DND_SIZE | DWNR | DDMAEN)
#define SRCCFG2D    (DFLOW_ARRAY | DW2D | DND_SIZE | DDMAEN)
#define DSTCFG2D    (DFLOW_ARRAY | DW2D | DND_SIZE | DWNR | DDMAEN)
#define STPSRCCFG   (DDMAEN)
#define STPDSTCFG   (DWNR | DDMAEN)
#define TOP_EDGE    0
#define BOTTOM_EDGE 1

/*=============  D A T A  =============*/

/* DMA L1 Buffers for Set 1 */
#pragma align 4
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *pInputBufL1Ping = aTempBuffer1;                                /* ADI_DMA_INPUT_BUFF1_SIZE_L1 */
#pragma align 4
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *pInputBufL1Pong = aTempBuffer1 + ADI_DMA_INPUT_BUFF1_SIZE_L1;  /* ADI_DMA_INPUT_BUFF1_SIZE_L1 */
#pragma align 4
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *pOutputBufL1 = aTempBuffer3;                                   /* ADI_DMA_OUTPUT_BUFF1_SIZE_L1 */

/* DMA L1 Buffers for Set 2 */
#pragma align 4
#pragma section("adi_appl_fastb0_prio0_rw")
int8_t      *pInputBuf2L1Ping = aTempBuffer2;                               /* ADI_DMA_INPUT_BUFF2_SIZE_L1 */
#pragma align 4
#pragma section("adi_appl_fastb0_prio0_rw")
int8_t      *pInputBuf2L1Pong = aTempBuffer2 + ADI_DMA_INPUT_BUFF2_SIZE_L1; /* ADI_DMA_INPUT_BUFF2_SIZE_L1 */
#pragma align 4
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *pOutputBuf2L1 = aTempBuffer4;                                  /* ADI_DMA_OUTPUT_BUFF2_SIZE_L1 */

#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *gpInputBufL3;
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *gpOutputBufL3;
#pragma section("adi_appl_fastb1_prio0_rw")
uint32_t    gnInputBytes;
#pragma section("adi_appl_fastb1_prio0_rw")
uint32_t    gnModeOfDataTransfer;
#pragma section("adi_appl_fastb1_prio0_rw")
uint32_t    gnInputCounter;
#pragma section("adi_appl_fastb1_prio0_rw")
uint32_t    gnOutputCounter;
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *gpInputBufCurrPing;
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *gpInputBufCurrPong;

#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *gpInputPing[ADI_DMA_MAX_INPUT];
#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *gpInputPong[ADI_DMA_MAX_INPUT];
#pragma section("adi_appl_fastb1_prio0_rw")
uint32_t    gnInputFlag[ADI_DMA_MAX_INPUT];

#pragma section("adi_appl_fastb1_prio0_rw")
int8_t      *gpOutputBuffer[ADI_DMA_MAX_OUTPUT];

#pragma section("adi_appl_fastb1_prio0_rw")
dma_desc    aDMAInSrc[ADI_DMA_MAX_INPUT + 1];
#pragma section("adi_appl_fastb1_prio0_rw")
dma_desc    aDMAInDst[ADI_DMA_MAX_INPUT + 1];

#pragma section("adi_appl_fastb1_prio0_rw")
dma_desc    aDMAOutSrc[ADI_DMA_MAX_OUTPUT + 1];
#pragma section("adi_appl_fastb1_prio0_rw")
dma_desc    aDMAOutDst[ADI_DMA_MAX_OUTPUT + 1];

/* Temp buffer for image edge replication for ADI_DATA_DMA_FROM_TO_L3 mode */
#pragma align 4
#pragma section("adi_appl_slow_prio1_rw")
int8_t      aTempBuffL3[ADI_DMA_INPUT_BUFF1_SIZE_L1];

#pragma section("adi_appl_slow_prio1_rw")
uint32_t    nValueL3 = 0;

/* Pointer to read the status of the MDMA */
#pragma align 4
#pragma section("L1_data_b")
volatile uint32_t   *pDMA_STATUS;

/*=============  C O D E  =============*/

/*
**  Function Prototype section
**  (static-scoped functions)
*/
static void DMAConfigure(void);

static void InitDMABase(uint32_t    nBaseAddr);

static void DMAWithOutDMAMgrSetDescriptors(void);

static void DMAWithOutDMAMgrSet2DChainDescriptors(
                                                  ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                                                  ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                                  uint32_t                nInput,
                                                  uint32_t                nOutput
                                                  );

/*
**  Function Definition section
*/

/******************************************************************************

  Function              : adi_MemMoveInit

  Function description  : Variable initializations required for memory movement
                          and also to set mode of transfer

  Parameters            :
    pInputBufL3(In)         -   Pointer to Input L3 Buffer
    pOutputBufL3(In)        -   Pointer to Output L3 Buffer
    nInputBytes(In)         -   Number of bytes in Input L3 Buffer
    nSizeOfBuf(In)          -   Size of data that need to be bought in for
                                the first Tile in Ping buffer for
                                ADI_DATA_DMA_FROM_TO_L3 mode
    nIncr(In)               -   Amount by which the Input L3 buffer pointer
                                need to be incremented after data transfer
    nModeOfDataTransfer(In) -   Mode of transfer chosen for memory movement
    pOutputBufCurr(Out)     -   Pointer to output buffer where the output
                                from the module will be dumped.

  Returns               : Number of Bytes transferred

  Notes                 : For ADI_DATA_DMA_FROM_TO_L3 mode, memory movement
                          to ping buffer is also done here

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
int32_t adi_MemMoveInit(
                        int8_t      *pInputBufL3,
                        int8_t      *pOutputBufL3,
                        uint32_t    nInputBytes,
                        uint16_t    nSizeOfBuf,
                        uint16_t    nIncr,
                        uint32_t    nModeOfDataTransfer,
                        int8_t      **pOutputBufCurr
                        )
{
    uint32_t    i;
    int32_t     eReturnValue;

    /* Initialize variables required for memory movement  */
    gpInputBufL3 = pInputBufL3;
    gpOutputBufL3 = pOutputBufL3;
    gnInputBytes = nInputBytes;
    gnModeOfDataTransfer = nModeOfDataTransfer;
    gnInputCounter = 0U;
    gnOutputCounter = 0U;
    eReturnValue = 0U;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DIRECT_FROM_TO_L3:
        *pOutputBufCurr = gpOutputBufL3;
        break;

    case ADI_DATA_MEMCOPY_FROM_TO_L3:
        *pOutputBufCurr = pOutputBufL1;
        gpInputBufCurrPing = pInputBufL1Ping;
        break;

    case ADI_DATA_DMA_FROM_TO_L3:

        /* Configure DMA Registers */
        DMAConfigure();
        InitDMABase(mdma_base_address_0);

        /* Disabling the MDMA  since it might  have been used by other module */
    	*MDMA_S0CFG &= ~(BITM_DMA_CFG_EN);;
    	*MDMA_D0CFG &= ~(BITM_DMA_CFG_EN);;

        /* Set Descriptors */
        DMAWithOutDMAMgrSetDescriptors();

        gpInputBufCurrPing = pInputBufL1Ping;
        gpInputBufCurrPong = pInputBufL1Pong;
    //    *pOutputBufCurr = pOutputBufL1;

        /* DMA in the Tile to Ping L1 buffer */
        if (nInputBytes < nSizeOfBuf)
        {
            nSizeOfBuf = nInputBytes;
        }

        /* DMASIZE dma */
        aDMAInSrc[0].pStartAddress = (gpInputBufL3);
        aDMAInSrc[0].XCount = (nSizeOfBuf / DMASIZE);

        aDMAInDst[0].pStartAddress = (gpInputBufCurrPing);
        aDMAInDst[0].XCount = (nSizeOfBuf / DMASIZE);

        /* start DMA */
        dmawait();
        dmastart(aDMAInSrc, aDMAInDst);
        gnInputCounter += nIncr;
        eReturnValue = nSizeOfBuf;
        break;

    default:
        break;
    }

    return (eReturnValue);
}

/******************************************************************************

  Function              : adi_MemMoveClose

  Function description  : Wait for the last DMA to get over

  Parameters            : None

  Returns               : Nothing

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMoveClose(void)
{
    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DMA_FROM_TO_L3:
        /* wait for DMA to get over */
        dmawait();
        break;
    }
}

/******************************************************************************

  Function              : adi_MemMoveReset

  Function description  : This function resets Memory Movement global variables

  Parameters            :
    pInputBufL3(In)         -   Pointer to Input L3 Buffer
    nInputBytes(In)         -   Number of bytes in Input L3 Buffer
    nSizeOfBuf(In)          -   Size of data that need to be bought in for
                                the first Tile in Ping buffer for
                                ADI_DATA_DMA_FROM_TO_L3 mode
    nIncr(In)               -   Amount by which the Input L3 buffer pointer
                                need to be incremented after data transfer

  Returns               : None

  Notes                 : For ADI_DATA_DMA_FROM_TO_L3 mode, memory movement
                          to ping buffer is also done here

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMoveReset(
                      int8_t      *pInputBufL3,
                      uint32_t    nInputBytes,
                      uint16_t    nSizeOfBuf,
                      uint16_t    nIncr
                      )
{
    /* Initialize variables required for memory movement  */
    gpInputBufL3 = pInputBufL3;
    gnInputBytes = nInputBytes;
    gnInputCounter = 0U;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DIRECT_FROM_TO_L3:
        break;

    case ADI_DATA_MEMCOPY_FROM_TO_L3:
        gpInputBufCurrPing = pInputBufL1Ping;
        break;

    case ADI_DATA_DMA_FROM_TO_L3:

        /* wait for any DMA that is happening */
        dmawait();

        gpInputBufCurrPing = pInputBufL1Ping;
        gpInputBufCurrPong = pInputBufL1Pong;

        /* DMA in the Tile to Ping L1 buffer */

        /* DMASIZE dma */
        aDMAInSrc[0].pStartAddress = (gpInputBufL3);
        aDMAInSrc[0].XCount = (nSizeOfBuf / DMASIZE);

        aDMAInDst[0].pStartAddress = (gpInputBufCurrPing);
        aDMAInDst[0].XCount = (nSizeOfBuf / DMASIZE);

        /* start DMA */
        dmastart(aDMAInSrc, aDMAInDst);
        gnInputCounter += nIncr;
        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_MemMoveModifyL3OutputBuffer

  Function description  : This function modify L3 Output Buffer Variables

  Parameters            :
    pOutputBufL3(In)        -   Pointer to Output L3 Buffer
    nIncr(In)               -   Amount by which the Output L3 buffer pointer
                                need to be incremented after initialization
    pOutputBufCurr(Out)     -   Pointer to output buffer that need to be used
                                by module to dump data next time.

  Returns               : None

  Notes                 :

******************************************************************************/
void adi_MemMoveModifyL3OutputBuffer(
                                     int8_t      *pOutputBufL3,
                                     uint32_t    nIncr,
                                     int8_t      **pOutputBufCurr
                                     )
{
    gpOutputBufL3 = pOutputBufL3;
    gnOutputCounter = nIncr;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DIRECT_FROM_TO_L3:

        /* Just update the Output Pointer */
        *pOutputBufCurr = gpOutputBufL3 + gnOutputCounter;
        break;

    case ADI_DATA_MEMCOPY_FROM_TO_L3:

        /* copy Processed Tile to L3 buffer */
        gpOutputBufL3 = gpOutputBufL3 + nIncr;
        break;

    case ADI_DATA_DMA_FROM_TO_L3:

        /* wait for any DMA that is happening */
        dmawait();
        gpOutputBufL3 = gpOutputBufL3 + nIncr;

        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_MemMoveModifyL3InputBuffer

  Function description  : This function modify L3 Input Buffer Variables

  Parameters            :
    pInputBufL3(In)         -   Pointer to Input L3 Buffer
    nInputBytes(In)         -   Number of bytes in Input L3 Buffer
    nIncr(In)               -   Amount by which the Input L3 buffer pointer
                                need to be incremented after initialization

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMoveModifyL3InputBuffer(
                                    int8_t      *pInputBufL3,
                                    uint32_t    nInputBytes,
                                    uint32_t    nIncr
                                    )
{

    /* Initialize variables required for memory movement  */
    gpInputBufL3 = pInputBufL3;
    gnInputBytes = nInputBytes;
    gnInputCounter = 0U;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DIRECT_FROM_TO_L3:
        break;

    case ADI_DATA_MEMCOPY_FROM_TO_L3:
        gpInputBufCurrPing = pInputBufL1Ping;
        break;

    case ADI_DATA_DMA_FROM_TO_L3:

        /* wait for any DMA that is happening */
        dmawait();

        /* Reset the buffers */
        gpInputBufCurrPing = pInputBufL1Ping;
        gpInputBufCurrPong = pInputBufL1Pong;

        /* Reset the counter */
        gnInputCounter += nIncr;
        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_MemMovePartialOut

  Function description  : Function for dumping processed data to L3 with configurable
                          starting address in L1.

  Parameters            :
    nSizeOfBuf(In)      -   Size of data that need to be moved to L3 Buffer.
    nModify(In)         -   Amount by which the Output L1 buffer pointer
                            need to be incremented before moving to
                            L3 buffer.
    nIncr(In)           -   Amount by which the Output L3 buffer pointer
                            need to be incremented after moving the data from
                            L1 buffer.
    pOutputBufCurr(Out) -   Pointer to output buffer that need to be used
                            by module to dump data next time.

  Returns               : None

  Notes                 : For ADI_DATA_DMA_FROM_TO_L3 mode, memory movement
                          to ping buffer is also done here

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMovePartialOut(
                           uint16_t    nSizeOfBuf,
                           int16_t     nModify,
                           int16_t     nIncr,
                           int8_t      **pOutputBufCurr
                           )
{
    uint32_t    i;
    int8_t      *pNewL1Buffer;

    /* Increment the Output L3 buffer pointer */
    gnOutputCounter += nIncr;
    pNewL1Buffer = pOutputBufL1 + nModify;
    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DIRECT_FROM_TO_L3:

        /* Just update the Output Pointer */
        *pOutputBufCurr = gpOutputBufL3 + gnOutputCounter;
        break;

    case ADI_DATA_MEMCOPY_FROM_TO_L3:

        /* copy Processed Tile to L3 buffer */
        for (i = 0; i < nSizeOfBuf; i++)
        {
            gpOutputBufL3[i] = pNewL1Buffer[i];
        }

        gpOutputBufL3 = gpOutputBufL3 + nIncr;
        break;

    case ADI_DATA_DMA_FROM_TO_L3:

        /* wait for any DMA that is happening */
        dmawait();

        /* DMA out processes Tile to L3 buffer */

        /* DMASIZE dma */
        aDMAOutSrc[0].pStartAddress = (pNewL1Buffer);
        aDMAOutSrc[0].XCount = (nSizeOfBuf / DMASIZE);

        aDMAOutDst[0].pStartAddress = (gpOutputBufL3);
        aDMAOutDst[0].XCount = (nSizeOfBuf / DMASIZE);

        /* start DMA */
        dmastart(aDMAOutSrc, aDMAOutDst);

        gpOutputBufL3 = gpOutputBufL3 + nIncr;
        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_MemMovePartialIn

  Function description  : Function to get in the Data required from L3

  Parameters            :
    nSizeOfBuf(In)      -   Size of data that need to be bought in. (Bytes)
    nModify(In)         -   Amount by which the Input L1 buffer pointer
                            need to be modified before moving in the
                            L3 buffer.
    nIncr(In)           -   Amount by which the Input L3 buffer pointer
                            need to be incremented after moving the data to
                            L1 buffer.
    pInputBufCurr(Out)  -   Pointer to input buffer that need to be
                            processed by module.

  Returns               : Number of Bytes transferred

  Notes                 : adi_MemMoveInit is called and gpInputBufL3,
                          gpOutputBufL3 buffer pointers are set.

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
int32_t adi_MemMovePartialIn(
                             uint16_t    nSizeOfBuf,
                             int16_t     nModify,
                             int16_t     nIncr,
                             int8_t      **pInputBufCurr
                             )
{
    int32_t     eReturnValue;
    uint32_t    i;
    int8_t      *pTempBuf;
    int8_t      *pNewInputBuf;

    eReturnValue = 0;
    if (gnInputCounter < gnInputBytes)
    {

        /* If data available in L3 Input Buffer */
        if ((gnInputBytes - gnInputCounter) < nSizeOfBuf)
        {

            /* data available in L3 Input Buffer, but less than what is asked */
            nSizeOfBuf = gnInputBytes - gnInputCounter;
        }

        switch (gnModeOfDataTransfer)
        {
        case ADI_DATA_DIRECT_FROM_TO_L3:

            /* Just update the Input Pointer */
            *pInputBufCurr = gpInputBufL3 + gnInputCounter;
            break;

        case ADI_DATA_MEMCOPY_FROM_TO_L3:

            /* copy Tile to L1 buffer */
            pNewInputBuf = pInputBufL1Ping + nModify;
            *pInputBufCurr = pInputBufL1Ping;
            pTempBuf = gpInputBufL3 + gnInputCounter;
            for (i = 0; i < nSizeOfBuf; i++)
            {
                pNewInputBuf[i] = pTempBuf[i];
            }

            break;

        case ADI_DATA_DMA_FROM_TO_L3:

            /* wait for any DMA that is happening */
            dmawait();
            *pInputBufCurr = gpInputBufCurrPing;
            pTempBuf = gpInputBufL3 + gnInputCounter;
            pNewInputBuf = gpInputBufCurrPong + nModify;

            /* DMA in the Tile to Pong L1 buffer */

            /* DMASIZE dma */
            aDMAInSrc[0].pStartAddress = (pTempBuf);
            aDMAInSrc[0].XCount = (nSizeOfBuf / DMASIZE);

            aDMAInDst[0].pStartAddress = (pNewInputBuf);
            aDMAInDst[0].XCount = (nSizeOfBuf / DMASIZE);

            /* start DMA */
            dmastart(aDMAInSrc, aDMAInDst);

            /* swap ping-pong buffer pointers */
            pTempBuf = gpInputBufCurrPing;
            gpInputBufCurrPing = gpInputBufCurrPong;
            gpInputBufCurrPong = pTempBuf;

            break;

        default:
            break;
        }

        /* Increment the Input L3 buffer pointer */
        gnInputCounter += nIncr;
        eReturnValue = nSizeOfBuf;
    }
    else
    {

        /* If data not available, wait for previous DMA to get over */
        dmawait();
        *pInputBufCurr = gpInputBufCurrPing;
    }

    return (eReturnValue);
}

/******************************************************************************

  Function              : adi_MemMoveIn

  Function description  : Function to get in the Data required from L3

  Parameters            :
    nSizeOfBuf(In)      -   Size of data that need to be bought in. (Bytes)
    nIncr(In)           -   Amount by which the Input L3 buffer pointer
                            need to be incremented after data transfer
    pInputBufCurr(Out)  -   Pointer to input buffer that need to be
                            processed by module.

  Returns               : Number of Bytes transferred

  Notes                 : adi_MemMoveInit is called and gpInputBufL3,
                          gpOutputBufL3 buffer pointers are set.

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
int32_t adi_MemMoveIn(
                      uint16_t    nSizeOfBuf,
                      uint16_t    nIncr,
                      int8_t      **pInputBufCurr
                      )
{
    int32_t     eReturnValue;
    uint32_t    i;
    int8_t      *pTempBuf;

    eReturnValue = 0;
    if (gnInputCounter < gnInputBytes)
    {

        /* If data available in L3 Input Buffer */
        if ((gnInputBytes - gnInputCounter) < nSizeOfBuf)
        {

            /* data available in L3 Input Buffer, but less than what is asked */
            nSizeOfBuf = gnInputBytes - gnInputCounter;
        }

        switch (gnModeOfDataTransfer)
        {
        case ADI_DATA_DIRECT_FROM_TO_L3:

            /* Just update the Input Pointer */
            *pInputBufCurr = gpInputBufL3 + gnInputCounter;
            break;

        case ADI_DATA_MEMCOPY_FROM_TO_L3:

            /* copy Tile to L1 buffer */
            *pInputBufCurr = pInputBufL1Ping;
            pTempBuf = gpInputBufL3 + gnInputCounter;
            for (i = 0; i < nSizeOfBuf; i++)
            {
                pInputBufL1Ping[i] = pTempBuf[i];
            }

            break;

        case ADI_DATA_DMA_FROM_TO_L3:

            /* wait for any DMA that is happening */
            dmawait();
            *pInputBufCurr = gpInputBufCurrPing;
            pTempBuf = gpInputBufL3 + gnInputCounter;

            /* DMA in the Tile to Pong L1 buffer */

            /* DMASIZE dma */
            aDMAInSrc[0].pStartAddress = (pTempBuf);
            aDMAInSrc[0].XCount = (nSizeOfBuf / DMASIZE);

            aDMAInDst[0].pStartAddress = (gpInputBufCurrPong);
            aDMAInDst[0].XCount = (nSizeOfBuf / DMASIZE);

            /* start DMA */
            dmastart(aDMAInSrc, aDMAInDst);

            /* swap ping-pong buffer pointers */
            pTempBuf = gpInputBufCurrPing;
            gpInputBufCurrPing = gpInputBufCurrPong;
            gpInputBufCurrPong = pTempBuf;

            break;

        default:
            break;
        }

        /* Increment the Input L3 buffer pointer */
        gnInputCounter += nIncr;
        eReturnValue = nSizeOfBuf;
    }
    else
    {

        /* If data not available, wait for previous DMA to get over */
        dmawait();
        *pInputBufCurr = gpInputBufCurrPing;
    }

    return (eReturnValue);
}

/******************************************************************************

  Function              : adi_MemMoveOut

  Function description  : Function to dump processed data to L3 buffer

  Parameters            :
    nSizeOfBuf(In)      -   Size of data that need to be dumped(In Bytes).
    nIncr(In)           -   Amount by which the Output L3 buffer pointer
                            need to be incremented after data transfer
    pOutputBufCurr(Out) -   Pointer to output buffer that need to be used
                            by module to dump data next time.

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMoveOut(
                    uint16_t    nSizeOfBuf,
                    uint16_t    nIncr,
                    int8_t      **pOutputBufCurr
                    )
{
    uint32_t    i;

    /* Increment the Output L3 buffer pointer */
    gnOutputCounter += nIncr;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DIRECT_FROM_TO_L3:

        /* Just update the Output Pointer */
        *pOutputBufCurr = gpOutputBufL3 + gnOutputCounter;
        break;

    case ADI_DATA_MEMCOPY_FROM_TO_L3:

        /* copy Processed Tile to L3 buffer */
        for (i = 0; i < nSizeOfBuf; i++)
        {
            gpOutputBufL3[i] = pOutputBufL1[i];
        }

        gpOutputBufL3 = gpOutputBufL3 + nIncr;
        break;

    case ADI_DATA_DMA_FROM_TO_L3:

        /* wait for any DMA that is happening */
        dmawait();

        /* DMA out processes Tile to L3 buffer */

        /* DMASIZE dma */
        aDMAOutSrc[0].pStartAddress = (pOutputBufL1);
        aDMAOutSrc[0].XCount = (nSizeOfBuf / DMASIZE);

        aDMAOutDst[0].pStartAddress = (gpOutputBufL3);
        aDMAOutDst[0].XCount = (nSizeOfBuf / DMASIZE);

        /* start DMA */
        dmastart(aDMAOutSrc, aDMAOutDst);

        gpOutputBufL3 = gpOutputBufL3 + nIncr;
        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_MemMoveInEdge

  Function description  : Function to get in the Edge data with number of
                          rows replicated depending on the order of filter.

  Parameters            :
    nFilterOrder(In)    -   Order of Filter
    dimX(In)            -   Image width
    pInputBufCurr(Out)  -   Pointer to input buffer that need to be
                            processed by module.
    nEdge(In)           -   Top/Bottom Edge

  Returns               : None

  Notes                 : required number of lines is replicated depending
                          on nFilterOrder.

                          For 3x3 filter, only one line is replicated
                          giving out 3 rows as a Tile.
                            Top Edge,
                                First Row <- Replicated
                                First Row
                                Second Row

                            Bottom Edge,
                                Last-1 Row
                                Last Row
                                Last Row <- Replicated

                          For 5x5 filter, two lines are replicated
                          giving out 6 rows as a Tile.
                            Top Edge,
                                First Row <- Replicated
                                First Row <- Replicated
                                First Row
                                Second Row
                                Third Row
                                Fourth Row

                            Bottom Edge,
                                Last-3 Row
                                Last-2 Row
                                Last-1 Row
                                Last Row
                                Last Row <- Replicated
                                Last Row <- Replicated

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMoveInEdge(
                       uint32_t    nFilterOrder,
                       uint32_t    dimX,
                       int8_t      **pInputBufCurr,
                       uint32_t    nEdge
                       )
{
    uint32_t    i;
    uint32_t    j;
    uint32_t    k;
    int8_t      *pTempBuf;
    int8_t      *pTempBufEdge;

    k = 0;

    if (gnModeOfDataTransfer == ADI_DATA_DIRECT_FROM_TO_L3)
    {
        *pInputBufCurr = aTempBuffL3;
    }
    else if (gnModeOfDataTransfer == ADI_DATA_MEMCOPY_FROM_TO_L3)
    {
        *pInputBufCurr = gpInputBufCurrPing;
    }

    if (nEdge == TOP_EDGE)
    {

        /* For Top Edge */
        switch (gnModeOfDataTransfer)
        {
        case ADI_DATA_DIRECT_FROM_TO_L3:
        case ADI_DATA_MEMCOPY_FROM_TO_L3:
            pTempBufEdge = *pInputBufCurr;

            /* Replicate first row (nFilterOrder>>1) times */
            for (i = 0; i < (nFilterOrder >> 1); i++)
            {
                for (j = 0; j < dimX; j++)
                {
                    pTempBufEdge[k] = gpInputBufL3[j];
                    k++;
                }
            }

            /* copy first ((nFilterOrder>>1)+(nFilterOrder>>1)) rows */
            for
            (
                j = 0;
                j < ((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX;
                j++
            )
            {
                pTempBufEdge[k] = gpInputBufL3[j];
                k++;
            }

            break;

        case ADI_DATA_DMA_FROM_TO_L3:

            /* wait for any DMA that is happening */
            dmawait();
            *pInputBufCurr = gpInputBufCurrPong;
            pTempBuf = gpInputBufCurrPong;

            /* DMA in first row (nFilterOrder>>1) times */
            for (i = 0; i < (nFilterOrder >> 1); i++)
            {

                /* DMASIZE dma */
                aDMAInSrc[0].pStartAddress = (gpInputBufL3);
                aDMAInSrc[0].XCount = (dimX / DMASIZE);

                aDMAInDst[0].pStartAddress = (pTempBuf);
                aDMAInDst[0].XCount = (dimX / DMASIZE);

                /* start DMA */
                dmastart(aDMAInSrc, aDMAInDst);

                /* wait for DMA */
                dmawait();
                pTempBuf += dimX;
            }

            /* DMASIZE dma */

            /* DMA in first ((nFilterOrder>>1)+(nFilterOrder>>1)) rows */
            aDMAInSrc[0].pStartAddress = (gpInputBufL3);
            aDMAInSrc[0].XCount = (((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX / DMASIZE);

            aDMAInDst[0].pStartAddress = (pTempBuf);
            aDMAInDst[0].XCount = (((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX / DMASIZE);

            /* start DMA */
            dmastart(aDMAInSrc, aDMAInDst);

            /* wait for DMA */
            dmawait();
            break;

        default:
            break;
        }
    }
    else
    {

        /* For Bottom Edge */
        switch (gnModeOfDataTransfer)
        {
        case ADI_DATA_DIRECT_FROM_TO_L3:
        case ADI_DATA_MEMCOPY_FROM_TO_L3:
            pTempBufEdge = *pInputBufCurr;
            pTempBuf = gpInputBufL3 + gnInputCounter;

            /* copy last ((nFilterOrder>>1)+(nFilterOrder>>1)) rows */
            for
            (
                j = 0;
                j < ((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX;
                j++
            )
            {
                pTempBufEdge[k] = *pTempBuf++;
                k++;
            }

            pTempBuf -= dimX;

            /* Replicate last row (nFilterOrder>>1) times */
            for (i = 0; i < (nFilterOrder >> 1); i++)
            {
                for (j = 0; j < dimX; j++)
                {
                    pTempBufEdge[k] = pTempBuf[j];
                    k++;
                }
            }

            break;

        case ADI_DATA_DMA_FROM_TO_L3:

            /* wait for any DMA that is happening */
            dmawait();
            *pInputBufCurr = gpInputBufCurrPong;
            gnInputCounter -= dimX;
            pTempBuf = gpInputBufL3 + gnInputCounter;

            /* DMA in last ((nFilterOrder>>1)+(nFilterOrder>>1)) rows */

            /* DMASIZE dma */
            aDMAInSrc[0].pStartAddress = (pTempBuf);
            aDMAInSrc[0].XCount = (((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX / DMASIZE);

            aDMAInDst[0].pStartAddress = (gpInputBufCurrPong);
            aDMAInDst[0].XCount = (((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX / DMASIZE);

            /* start DMA */
            dmastart(aDMAInSrc, aDMAInDst);

            /* wait for DMA */
            dmawait();

            gpInputBufCurrPong += ((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX;
            pTempBuf += ((nFilterOrder >> 1) + (nFilterOrder >> 1)) * dimX;
            pTempBuf -= dimX;

            /* DMA in last row (nFilterOrder>>1) times */
            for (i = 0; i < (nFilterOrder >> 1); i++)
            {

                /* DMASIZE dma */
                aDMAInSrc[0].pStartAddress = (pTempBuf);
                aDMAInSrc[0].XCount = (dimX / DMASIZE);

                aDMAInDst[0].pStartAddress = (gpInputBufCurrPong);
                aDMAInDst[0].XCount = (dimX / DMASIZE);

                /* start DMA */
                dmastart(aDMAInSrc, aDMAInDst);

                /* wait for DMA */
                dmawait();
                gpInputBufCurrPong += dimX;
            }

            break;

        default:
            break;
        }
    }
}

/******************************************************************************

  Function              : adi_MemMoveValue

  Function description  : Function to write user specified value to
                          given buffer

  Parameters            :
    pBuff(Out)      -   Pointer to buffer into which value has to be written
    nSizeOfBuf(In)  -   Size of data that need to be written(In Bytes)
    nValue(In)      -   Value to be written

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMoveValue(
                      int8_t      *pBuff,
                      uint32_t    nSizeOfBuf,
                      uint32_t    nValue
                      )
{
    uint32_t    i;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DMA_FROM_TO_L3:

        /* wait for any DMA that is happening */
        dmawait();

        nValueL3 = nValue;

        /* DMA In Value to buffer sent */

        /* DMASIZE dma */
        aDMAInSrc[0].pStartAddress = (&nValueL3);
        aDMAInSrc[0].XCount = (nSizeOfBuf / DMASIZE);
        aDMAInSrc[0].XModify = (0);

        aDMAInDst[0].pStartAddress = (pOutputBufL1);
        aDMAInDst[0].XCount = (nSizeOfBuf / DMASIZE);

        /* start DMA */
        dmastart(aDMAInSrc, aDMAInDst);

        /* wait for DMA */
        dmawait();
        aDMAInSrc[0].XModify = (DMASIZE);
        break;

    default:

        /* copy Value to buffer sent */
        for (i = 0; i < nSizeOfBuf; i++)
        {
            pBuff[i] = nValue;
        }

        break;
    }
}

/******************************************************************************

  Function              : adi_2DChainDesc_MemMoveInit

  Function description  : Variable initializations required for memory movement
                          and also to set mode of transfer. This method supports
                          variable number of input and output and 2D or 1D
                          transfer.

  Parameters            :
    pInputBufL3(In/Out)     -   Pointer to Array of Structure Specifying Input
                                L3 Buffer Configurations
    pOutputBufL3(In/Out)    -   Pointer to Array of Structure Specifying Output
                                L3 Buffer Configurations
    nInput(In)              -   Number of Input
    nOutput(In)             -   Number of Output
    nModeOfDataTransfer(In) -   Mode of Transfer

  Returns               : 0 if Successful else nonzero

  Notes                 : Only ADI_DATA_DMA_FROM_TO_L3 mode is currently supported
                          XCount should be a multiple of DMASIZE and the function brings
                          DMASIZE bytes at a time

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
int32_t adi_2DChainDesc_MemMoveInit(
                                    ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                                    ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                    uint32_t                nInput,
                                    uint32_t                nOutput,
                                    uint32_t                nModeOfDataTransfer
                                    )
{
    uint32_t    x;

    /* Check for Input Parameters */
    if (nInput > ADI_DMA_MAX_INPUT)
        return (1);

    if (nOutput > ADI_DMA_MAX_OUTPUT)
        return (2);

    /* Configure Ping Pong Buffers */
    gpInputPing[0] = pInputBuf2L1Ping;
    gpInputPing[1] = pInputBufL1Ping;

    gpInputPong[0] = pInputBuf2L1Pong;
    gpInputPong[1] = pInputBufL1Pong;

    gpOutputBuffer[0] = pOutputBufL1;
    gpOutputBuffer[1] = pOutputBuf2L1;

    gnModeOfDataTransfer = nModeOfDataTransfer;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DMA_FROM_TO_L3:

        /* Configure DMA Registers */
        DMAConfigure();
        InitDMABase(mdma_base_address_0);

        /* Set Descriptors */
        DMAWithOutDMAMgrSet2DChainDescriptors(pInputBufL3,
                                              pOutputBufL3,
                                              nInput,
                                              nOutput);

        /* Configure the current pointers and pingpong buffers */
        for (x = 0; x < nInput; x++)
        {
            pInputBufL3[x]->pCurrBuffer = pInputBufL3[x]->pBaseBuffer;
            pInputBufL3[x]->pInOutBuffer = gpInputPing[x];
            gnInputFlag[x] = 0;
        }

        for (x = 0; x < nOutput; x++)
        {
            pOutputBufL3[x]->pCurrBuffer = pOutputBufL3[x]->pBaseBuffer;
            pOutputBufL3[x]->pInOutBuffer = gpOutputBuffer[x];
        }

        /* DMA in the Tile to L1 buffer */

        /* DMASIZE dma */
        for (x = 0; x < nInput; x++)
        {
            /* Source Configuration */
            aDMAInSrc[x].pStartAddress = (pInputBufL3[x]->pCurrBuffer);

            aDMAInSrc[x].XCount = (pInputBufL3[x]->nInputXCount);
            aDMAInSrc[x].XModify = (pInputBufL3[x]->nInputXModify);

            aDMAInSrc[x].YCount = (pInputBufL3[x]->nInputYCount);
            aDMAInSrc[x].YModify = (pInputBufL3[x]->nInputYModify);

            if (gnInputFlag[x] == 0)
            {
                aDMAInDst[x].pStartAddress = (gpInputPong[x] + pInputBufL3[x]->nPreBufferIncrement);

                aDMAInDst[x].XCount = pInputBufL3[x]->nOutputXCount;
                aDMAInDst[x].XModify = pInputBufL3[x]->nOutputXModify;

                aDMAInDst[x].YCount = pInputBufL3[x]->nOutputYCount;
                aDMAInDst[x].YModify = pInputBufL3[x]->nOutputYModify;
            }
            else
            {
                aDMAInDst[x].pStartAddress = (gpInputPing[x] + pInputBufL3[x]->nPreBufferIncrement);

                aDMAInDst[x].XCount = pInputBufL3[x]->nOutputXCount;
                aDMAInDst[x].XModify = pInputBufL3[x]->nOutputXModify;

                aDMAInDst[x].YCount = pInputBufL3[x]->nOutputYCount;
                aDMAInDst[x].YModify = pInputBufL3[x]->nOutputYModify;
            }
        }

        /* start DMA */
        dmawait();
        dmastart(aDMAInSrc, aDMAInDst);

        /* Increment the Pointer */
        for (x = 0; x < nInput; x++)
        {
            pInputBufL3[x]->pCurrBuffer = pInputBufL3[x]->pCurrBuffer + pInputBufL3[x]->nIncrement;
        }

        break;

    default:
        return (1);
    }

    return (0);
}

/******************************************************************************

  Function              : adi_2DChainDesc_MemMoveIn

  Function description  : Function to get in the Data required from L3

  Parameters            :
    pInputBufL3(In/Out) -   Pointer to Array of Structure Specifying Input
                            L3 Buffer Configurations
    nInput(In)          -   Number of Input

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_2DChainDesc_MemMoveIn(
                               ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                               uint32_t                nInput
                               )
{
    uint32_t    x;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DMA_FROM_TO_L3:
        dmawait();

        /* Configure the current pointers and pingpong buffers */
        for (x = 0; x < nInput; x++)
        {
            if (gnInputFlag[x] == 0)
            {
                pInputBufL3[x]->pInOutBuffer = gpInputPong[x];
                gnInputFlag[x] = 1;
            }
            else
            {
                pInputBufL3[x]->pInOutBuffer = gpInputPing[x];
                gnInputFlag[x] = 0;
            }
        }

        /* DMA in the Tile to Ping L1 buffer */

        /* DMASIZE dma */
        for (x = 0; x < nInput; x++)
        {

            /* Source Configuration */
            aDMAInSrc[x].pStartAddress = (pInputBufL3[x]->pCurrBuffer);

            aDMAInSrc[x].XCount = (pInputBufL3[x]->nInputXCount);
            aDMAInSrc[x].XModify = (pInputBufL3[x]->nInputXModify);

            aDMAInSrc[x].YCount = (pInputBufL3[x]->nInputYCount);
            aDMAInSrc[x].YModify = (pInputBufL3[x]->nInputYModify);

            /* Destination Configuration */
            if (gnInputFlag[x] == 0)
            {
                aDMAInDst[x].pStartAddress = (gpInputPong[x] + pInputBufL3[x]->nPreBufferIncrement);

                aDMAInDst[x].XCount = pInputBufL3[x]->nOutputXCount;
                aDMAInDst[x].XModify = pInputBufL3[x]->nOutputXModify;

                aDMAInDst[x].YCount = pInputBufL3[x]->nOutputYCount;
                aDMAInDst[x].YModify = pInputBufL3[x]->nOutputYModify;
            }
            else
            {
                aDMAInDst[x].pStartAddress = (gpInputPing[x] + pInputBufL3[x]->nPreBufferIncrement);

                aDMAInDst[x].XCount = pInputBufL3[x]->nOutputXCount;
                aDMAInDst[x].XModify = pInputBufL3[x]->nOutputXModify;

                aDMAInDst[x].YCount = pInputBufL3[x]->nOutputYCount;
                aDMAInDst[x].YModify = pInputBufL3[x]->nOutputYModify;
            }
        }

        /* start DMA */
        dmastart(aDMAInSrc, aDMAInDst);

        /* Increment the Pointer */
        for (x = 0; x < nInput; x++)
        {
            pInputBufL3[x]->pCurrBuffer = pInputBufL3[x]->pCurrBuffer + pInputBufL3[x]->nIncrement;
        }

        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_2DChainDesc_MemMoveOut

  Function description  : Function to dump processed data to L3 buffer

  Parameters            :
    pOutputBufL3(In/Out)    -   Pointer to Array of Structure Specifying Output
                                L3 Buffer Configurations
    nOutput(In)             -   Number of Output

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_2DChainDesc_MemMoveOut(
                                ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                uint32_t                nOutput
                                )
{
    uint32_t    x;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DMA_FROM_TO_L3:
        dmawait();

        /* DMA out processes Tile to L3 buffer */
        for (x = 0; x < nOutput; x++)
        {
            pOutputBufL3[x]->pInOutBuffer = gpOutputBuffer[x];
        }

        /* DMASIZE dma */
        for (x = 0; x < nOutput; x++)
        {

            /* Source Configuration */
            aDMAOutSrc[x].pStartAddress = (gpOutputBuffer[x] + pOutputBufL3[x]->nPreBufferIncrement);

            aDMAOutSrc[x].XCount = (pOutputBufL3[x]->nInputXCount);
            aDMAOutSrc[x].XModify = (pOutputBufL3[x]->nInputXModify);
            aDMAOutSrc[x].YCount = (pOutputBufL3[x]->nInputYCount);
            aDMAOutSrc[x].YModify = (pOutputBufL3[x]->nInputYModify);

            /* Destination Configuration Configuration */
            aDMAOutDst[x].pStartAddress = (pOutputBufL3[x]->pCurrBuffer);

            aDMAOutDst[x].XCount = (pOutputBufL3[x]->nOutputXCount);
            aDMAOutDst[x].XModify = (pOutputBufL3[x]->nOutputXModify);

            aDMAOutDst[x].YCount = (pOutputBufL3[x]->nOutputYCount);
            aDMAOutDst[x].YModify = (pOutputBufL3[x]->nOutputYModify);
        }

        /* start DMA */
        dmastart(aDMAOutSrc, aDMAOutDst);

        dmawait();

        /* Increment the Pointer */
        for (x = 0; x < nOutput; x++)
        {
            pOutputBufL3[x]->pCurrBuffer = pOutputBufL3[x]->pCurrBuffer + pOutputBufL3[x]->nIncrement;
        }

        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_2DChainDesc_MemMoveCircInit

  Function description  : Variable initializations required for circular memory
                          movement. It sets mode of transfer and triggers the
                          first transfer in case of ADI_DATA_DMA_FROM_TO_L3 mode.
                          This method supports variable number of input and output.

  Parameters            :
    pInputBufL3(In/Out)     -   Pointer to Array of Structure Specifying Input
                                L3 Buffer Configurations
    pOutputBufL3(In/Out)    -   Pointer to Array of Structure Specifying Output
                                L3 Buffer Configurations
    nInput(In)              -   Number of Inputs
    nOutput(In)             -   Number of Outputs
    nModeOfDataTransfer(In) -   Mode of Transfer

  Returns               : 0 if Successful else nonzero

  Notes                 : Only ADI_DATA_DMA_FROM_TO_L3 mode is currently supported
                          XCount should be a multiple of DMASIZE, as function does
                          DMASIZE DMA.

                          nCircularBuffLen variable of pInputBufL3 need to
                          be set to Circular buffer size
                          i.e. nCircularBuffLen = Circular buffer size / data bought using adi_2DChainDesc_MemMoveCircIn

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
int32_t adi_2DChainDesc_MemMoveCircInit(
                                        ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                                        ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                        uint32_t                nInput,
                                        uint32_t                nOutput,
                                        uint32_t                nModeOfDataTransfer
                                        )
{
    uint32_t    x;

    /* Check for Input Parameters */
    if (nInput > ADI_DMA_MAX_INPUT)
        return (1);

    if (nOutput > ADI_DMA_MAX_OUTPUT)
        return (2);

    /* Configure Buffers */
    gpInputPing[0] = pInputBufL1Ping;
    gpInputPing[1] = pInputBuf2L1Ping;

    gpOutputBuffer[0] = pOutputBufL1;
    gpOutputBuffer[1] = pOutputBuf2L1;

    gnModeOfDataTransfer = nModeOfDataTransfer;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DMA_FROM_TO_L3:

        /* Configure DMA Registers */
        DMAConfigure();
        InitDMABase(mdma_base_address_0);

        /* Set Descriptors */
        DMAWithOutDMAMgrSet2DChainDescriptors(pInputBufL3,
                                              pOutputBufL3,
                                              nInput,
                                              nOutput);

        /* Configure the current pointers for buffers */
        for (x = 0; x < nInput; x++)
        {
            pInputBufL3[x]->pCurrBuffer = pInputBufL3[x]->pBaseBuffer;
            pInputBufL3[x]->pInOutBuffer = gpInputPing[x];
            pInputBufL3[x]->nCircularBuffCurr = 0;
        }

        for (x = 0; x < nOutput; x++)
        {
            pOutputBufL3[x]->pCurrBuffer = pOutputBufL3[x]->pBaseBuffer;
            pOutputBufL3[x]->pInOutBuffer = gpOutputBuffer[x];
        }

        /* DMA in the Tile to Ping L1 buffer */

        /* DMASIZE dma */
        for (x = 0; x < nInput; x++)
        {

            /* Source Configuration */
            aDMAInSrc[x].pStartAddress = (pInputBufL3[x]->pCurrBuffer);

            aDMAInSrc[x].XCount = (pInputBufL3[x]->nInputXCount);
            aDMAInSrc[x].XModify = (pInputBufL3[x]->nInputXModify);

            aDMAInSrc[x].YCount = (pInputBufL3[x]->nInputYCount);
            aDMAInSrc[x].YModify = (pInputBufL3[x]->nInputYModify);

            pInputBufL3[x]->nPreBufferIncrement = pInputBufL3[x]->nCircularBuffCurr * pInputBufL3[x]->nInputXCount * pInputBufL3[x]->nInputXModify;

            aDMAInDst[x].pStartAddress = (gpInputPing[x] + pInputBufL3[x]->nPreBufferIncrement);

            aDMAInDst[x].XCount = pInputBufL3[x]->nOutputXCount;
            aDMAInDst[x].XModify = pInputBufL3[x]->nOutputXModify;

            aDMAInDst[x].YCount = pInputBufL3[x]->nOutputYCount;
            aDMAInDst[x].YModify = pInputBufL3[x]->nOutputYModify;
        }

        /* start DMA */
        dmawait();
        dmastart(aDMAInSrc, aDMAInDst);

        /* Increment the Pointer */
        for (x = 0; x < nInput; x++)
        {
            pInputBufL3[x]->pCurrBuffer = pInputBufL3[x]->pCurrBuffer + pInputBufL3[x]->nIncrement;
            pInputBufL3[x]->nCircularBuffCurr++;
            pInputBufL3[x]->nCircularBuffCurr = pInputBufL3[x]->nCircularBuffCurr % pInputBufL3[x]->nCircularBuffLen;
        }

        break;

    default:
        return (1);
    }

    return (0);
}

/******************************************************************************

  Function              : adi_2DChainDesc_MemMoveCircIn

  Function description  : Function to get in the Data required from L3 in circular fashion

  Parameters            :
    pInputBufL3(In/Out) -   Pointer to array of structure specifying input
                            L3 buffer configurations
    nInput(In)          -   Number of Inputs

  Returns               : None

  Notes                 : nCircularBuffLen variable of pInputBufL3 need to
                          be set to Circular buffer size
                          i.e. nCircularBuffLen = Circular buffer size / data bought using adi_2DChainDesc_MemMoveCircIn

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_2DChainDesc_MemMoveCircIn(
                                   ADI_DMA_MEM_TRANSFER **pInputBufL3,
                                   uint32_t             nInput
                                   )
{
    uint32_t    x;

    switch (gnModeOfDataTransfer)
    {
    case ADI_DATA_DMA_FROM_TO_L3:
        dmawait();

        /* DMA in the Tile to L1 buffer */

        /* DMASIZE dma */
        for (x = 0; x < nInput; x++)
        {
            /* Source Configuration */
            aDMAInSrc[x].pStartAddress = (pInputBufL3[x]->pCurrBuffer);

            aDMAInSrc[x].XCount = (pInputBufL3[x]->nInputXCount);
            aDMAInSrc[x].XModify = (pInputBufL3[x]->nInputXModify);

            aDMAInSrc[x].YCount = (pInputBufL3[x]->nInputYCount);
            aDMAInSrc[x].YModify = (pInputBufL3[x]->nInputYModify);

            /* Destination Configuration */
            pInputBufL3[x]->nPreBufferIncrement = pInputBufL3[x]->nCircularBuffCurr * pInputBufL3[x]->nInputXCount * pInputBufL3[x]->nInputXModify;

            aDMAInDst[x].pStartAddress = (gpInputPing[x] + pInputBufL3[x]->nPreBufferIncrement);

            aDMAInDst[x].XCount = pInputBufL3[x]->nOutputXCount;
            aDMAInDst[x].XModify = pInputBufL3[x]->nOutputXModify;

            aDMAInDst[x].YCount = pInputBufL3[x]->nOutputYCount;
            aDMAInDst[x].YModify = pInputBufL3[x]->nOutputYModify;
        }

        /* start DMA */
        dmastart(aDMAInSrc, aDMAInDst);

        /* Increment the Pointer */
        for (x = 0; x < nInput; x++)
        {
            pInputBufL3[x]->pCurrBuffer = pInputBufL3[x]->pCurrBuffer + pInputBufL3[x]->nIncrement;
            pInputBufL3[x]->nCircularBuffCurr++;
            pInputBufL3[x]->nCircularBuffCurr = pInputBufL3[x]->nCircularBuffCurr % pInputBufL3[x]->nCircularBuffLen;
        }

        break;

    default:
        break;
    }
}

/******************************************************************************

  Function              : adi_MemMoveInEdgeCirc

  Function description  : Function to get in the Edge data into circular buffer,
                          with number of rows replicated depending on
                          the order of filter.

  Parameters            :
    nFilterOrder(In)    -   Order of Filter
    nEdge(In)           -   Top/Bottom Edge
    pInputBufL3(In/Out) -   Pointer to structure specifying input
                            L3 buffer configurations

  Returns               : None

  Notes                 : required number of lines is replicated depending
                          on nFilterOrder.

                          For 3x3 filter, only one line is replicated
                          giving out 3 rows as a Tile.
                            Top Edge,
                                First Row <- Replicated
                                First Row
                                Second Row

                            Bottom Edge,
                                Last-1 Row
                                Last Row
                                Last Row <- Replicated

                          For 5x5 filter, two lines are replicated
                          giving out 6 rows as a Tile.
                            Top Edge,
                                First Row <- Replicated
                                First Row <- Replicated
                                First Row
                                Second Row
                                Third Row
                                Fourth Row

                            Bottom Edge,
                                Last-3 Row
                                Last-2 Row
                                Last-1 Row
                                Last Row
                                Last Row <- Replicated
                                Last Row <- Replicated

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
void adi_MemMoveInEdgeCirc(
                           uint32_t             nFilterOrder,
                           uint32_t             nEdge,
                           ADI_DMA_MEM_TRANSFER *pInputBufL3
                           )
{
    uint32_t    i;

    if (nEdge == TOP_EDGE)
    {

        /* For Top Edge */
        switch (gnModeOfDataTransfer)
        {
        case ADI_DATA_DMA_FROM_TO_L3:

            /* wait for any DMA that is happening */
            dmawait();

            /* DMASIZE dma */
            /* Source Configuration */
            aDMAInSrc[0].pStartAddress = (pInputBufL3->pCurrBuffer - pInputBufL3->nInputXCount * pInputBufL3->nInputXModify);

            aDMAInSrc[0].XCount = (pInputBufL3->nInputXCount);
            aDMAInSrc[0].XModify = (pInputBufL3->nInputXModify);

            aDMAInSrc[0].YCount = (pInputBufL3->nInputYCount);
            aDMAInSrc[0].YModify = (pInputBufL3->nInputYModify);

            /* Destination Configuration */
            aDMAInDst[0].XCount = pInputBufL3->nOutputXCount;
            aDMAInDst[0].XModify = pInputBufL3->nOutputXModify;

            aDMAInDst[0].YCount = pInputBufL3->nOutputYCount;
            aDMAInDst[0].YModify = pInputBufL3->nOutputYModify;

            /* Replicate first row (nFilterOrder>>1) times */
            for (i = 0; i < (nFilterOrder >> 1); i++)
            {
                /* Destination Configuration */
                pInputBufL3->nPreBufferIncrement = pInputBufL3->nCircularBuffCurr * pInputBufL3->nInputXCount * pInputBufL3->nInputXModify;

                aDMAInDst[0].pStartAddress = (gpInputPing[0] + pInputBufL3->nPreBufferIncrement);

                /* start DMA */
                dmastart(aDMAInSrc, aDMAInDst);

                /* Increment the Pointer */
                pInputBufL3->nCircularBuffCurr++;
                pInputBufL3->nCircularBuffCurr = pInputBufL3->nCircularBuffCurr % pInputBufL3->nCircularBuffLen;

                /* wait for DMA */
                dmawait();
            }

            /* DMA in first ((nFilterOrder>>1) rows + Tile */
            /* DMASIZE dma */
            /* Source Configuration */
            aDMAInSrc[0].pStartAddress = (pInputBufL3->pCurrBuffer);

            aDMAInSrc[0].XCount += (pInputBufL3->nInputXCount) * (nFilterOrder>>1);

            /* Destination Configuration */
            pInputBufL3->nPreBufferIncrement = pInputBufL3->nCircularBuffCurr * pInputBufL3->nInputXCount * pInputBufL3->nInputXModify;

            aDMAInDst[0].pStartAddress = (gpInputPing[0] + pInputBufL3->nPreBufferIncrement);

            aDMAInDst[0].XCount += (pInputBufL3->nOutputXCount) * (nFilterOrder>>1);

            /* start DMA */
            dmastart(aDMAInSrc, aDMAInDst);

            /* Increment the Pointer */
            pInputBufL3->pCurrBuffer += pInputBufL3->nIncrement;
            pInputBufL3->pCurrBuffer += pInputBufL3->nInputXCount * pInputBufL3->nInputXModify * (nFilterOrder>>1);
            pInputBufL3->nCircularBuffCurr += (nFilterOrder>>1) + 1;
            pInputBufL3->nCircularBuffCurr = pInputBufL3->nCircularBuffCurr % pInputBufL3->nCircularBuffLen;

            /* wait for DMA */
            dmawait();
            break;

        default:
            break;
        }
    }
    else
    {

        /* For Bottom Edge */
        switch (gnModeOfDataTransfer)
        {
        case ADI_DATA_DMA_FROM_TO_L3:

            /* wait for any DMA that is happening */
            dmawait();

            /* Replicate first row (nFilterOrder>>1) times */
            /* DMASIZE dma */
            /* Source Configuration */
            aDMAInSrc[0].pStartAddress = (pInputBufL3->pCurrBuffer - pInputBufL3->nInputXCount * pInputBufL3->nInputXModify);

            aDMAInSrc[0].XCount = (pInputBufL3->nInputXCount);
            aDMAInSrc[0].XModify = (pInputBufL3->nInputXModify);

            aDMAInSrc[0].YCount = (pInputBufL3->nInputYCount);
            aDMAInSrc[0].YModify = (pInputBufL3->nInputYModify);

            /* Destination Configuration */
            aDMAInDst[0].XCount = pInputBufL3->nOutputXCount;
            aDMAInDst[0].XModify = pInputBufL3->nOutputXModify;

            aDMAInDst[0].YCount = pInputBufL3->nOutputYCount;
            aDMAInDst[0].YModify = pInputBufL3->nOutputYModify;

            for (i = 0; i < (nFilterOrder >> 1); i++)
            {
                /* Destination Configuration */
                pInputBufL3->nPreBufferIncrement = pInputBufL3->nCircularBuffCurr * pInputBufL3->nInputXCount * pInputBufL3->nInputXModify;

                aDMAInDst[0].pStartAddress = (gpInputPing[0] + pInputBufL3->nPreBufferIncrement);

                /* start DMA */
                dmastart(aDMAInSrc, aDMAInDst);

                /* Increment the Pointer */
                pInputBufL3->nCircularBuffCurr++;
                pInputBufL3->nCircularBuffCurr = pInputBufL3->nCircularBuffCurr % pInputBufL3->nCircularBuffLen;

                /* wait for DMA */
                dmawait();
            }
            break;

        default:
            break;
        }
    }
}

/******************************************************************************

  Function              : DMAConfigure

  Function description  : configuring DMA MMR addr for different processors

  Parameters            : None

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
static void DMAConfigure(void)
{
    mdma_base_address_0 = MDMA1_SRC_DSCPTR_NXT;
    pDMA_STATUS = (volatile uint32_t *)MDMA1_DST_STAT;
}

/******************************************************************************

  Function              : InitDMABase

  Function description  : Initialize the base address to DMA MMR chosen

  Parameters            :
    nBaseAddr (In)  -   base address of DMA MMR chosen

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
static void InitDMABase(uint32_t    nBaseAddr)
{
    MDMA_S0DSC = (volatile int32_t *)(nBaseAddr + CURR_DESC_PTR_OFFSET);
    MDMA_S0CFG = (volatile int32_t *)(nBaseAddr + CONFIG_OFFSET);
    MDMA_D0DSC = (volatile int32_t *)(nBaseAddr + DEST_REG_OFFSET + CURR_DESC_PTR_OFFSET);
    MDMA_D0CFG = (volatile int32_t *)(nBaseAddr + DEST_REG_OFFSET + CONFIG_OFFSET);
}

/******************************************************************************

  Function              : DMAWithOutDMAMgrSetDescriptors

  Function description  : setting Descriptors required for DMA

  Parameters            : None

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
static void DMAWithOutDMAMgrSetDescriptors(void)
{

    /* DMASIZE dma */
    DMADESC(&aDMAInSrc[0], 0, (SRCCFG + DWDSIZE), 0, DMASIZE, 0, 0);
    DMADESC(&aDMAInDst[0], 0, (DSTCFG + DWDSIZE), 0, DMASIZE, 0, 0);

    /* dma flag : 4 byte transfer with 16bit MZISE. Previous transfer is 128bit transfer. DMASIZE is defined for 128bit transfer */
    DMADESC(&aDMAInSrc[1], &dmasemL3, (STPSRCCFG + ENUM_DMA_CFG_MSIZE02), 4/2, 2, 0, 0);
    DMADESC(&aDMAInDst[1], &DMASem, (STPDSTCFG + ENUM_DMA_CFG_MSIZE02), 4/2, 2, 0, 0);

    /* DMASIZE dma*/
    DMADESC(&aDMAOutSrc[0], 0, (SRCCFG + DWDSIZE), 0, DMASIZE, 0, 0);
    DMADESC(&aDMAOutDst[0], 0, (DSTCFG + DWDSIZE), 0, DMASIZE, 0, 0);

    /* dma flag : 4 byte transfer with 16bit MZISE */
    DMADESC(&aDMAOutSrc[1], &dmasemL1, (STPSRCCFG + ENUM_DMA_CFG_MSIZE02), 4/2, 2, 0, 0);
    DMADESC(&aDMAOutDst[1], &DMASem, (STPDSTCFG + ENUM_DMA_CFG_MSIZE02), 4/2, 2, 0, 0);
}

/******************************************************************************

  Function              : DMAWithOutDMAMgrSet2DChainDescriptors

  Function description  : setting Descriptors required for DMA Transfer

  Parameters            :
      pInputBufL3(In)         -   Pointer to Array of Structure Specifying Input
                                  L3 Buffer Configurations
      pOutputBufL3(In)        -   Pointer to Array of Structure Specifying Output
                                  L3 Buffer Configurations
      nInput(In)              -   Number of Input
      nOutput(In)             -   Number of Output

  Returns               : None

  Notes                 :

******************************************************************************/
#pragma section("adi_appl_fast_prio0_code")
static void DMAWithOutDMAMgrSet2DChainDescriptors(
                                                  ADI_DMA_MEM_TRANSFER    **pInputBufL3,
                                                  ADI_DMA_MEM_TRANSFER    **pOutputBufL3,
                                                  uint32_t                nInput,
                                                  uint32_t                nOutput
                                                  )
{
    uint32_t    x;

    /* Input DMA's */

    /* DMASIZE DMA */
    for (x = 0; x < nInput; x++)
    {

        /* Source Configure */
        if (pInputBufL3[x]->bInputEnable2D == false)
        {
            DMADESC(&aDMAInSrc[x], 0, (SRCCFG + DWDSIZE), 0, DMASIZE, 0, 0);
        }
        else
        {
            DMADESC(&aDMAInSrc[x], 0, (SRCCFG2D + DWDSIZE), 0, DMASIZE, 0, 0);
        }

        /* Destination Configure */
        if (pInputBufL3[x]->bOutputEnable2D == false)
        {
            DMADESC(&aDMAInDst[x], 0, (DSTCFG + DWDSIZE), 0, DMASIZE, 0, 0);
        }
        else
        {
            DMADESC(&aDMAInDst[x], 0, (DSTCFG2D + DWDSIZE), 0, DMASIZE, 0, 0);
        }
    }

    /* dma flag */
    DMADESC(&aDMAInSrc[x], &dmasemL3, (STPSRCCFG + DWDSIZE), 4/DMASIZE, DMASIZE, 0, 0);
    DMADESC(&aDMAInDst[x], &DMASem, (STPDSTCFG + DWDSIZE), 4/DMASIZE, DMASIZE, 0, 0);

    /* Output DMA's */

    /* DMASIZE DMA*/
    for (x = 0; x < nOutput; x++)
    {
        if (pOutputBufL3[x]->bInputEnable2D == false)
        {
            DMADESC(&aDMAOutSrc[x], 0, (SRCCFG + DWDSIZE), 0, DMASIZE, 0, 0);
        }
        else
        {
            DMADESC(&aDMAOutSrc[x], 0, (SRCCFG2D + DWDSIZE), 0, DMASIZE, 0, 0);
        }

        if (pOutputBufL3[x]->bOutputEnable2D == false)
        {
            DMADESC(&aDMAOutDst[x], 0, (DSTCFG + DWDSIZE), 0, DMASIZE, 0, 0);
        }
        else
        {
            DMADESC(&aDMAOutDst[x], 0, (DSTCFG2D + DWDSIZE), 0, DMASIZE, 0, 0);
        }
    }

    /* dma flag */
    DMADESC(&aDMAOutSrc[x], &dmasemL1, (STPSRCCFG + DWDSIZE), 4/DMASIZE, DMASIZE, 0, 0);
    DMADESC(&aDMAOutDst[x], &DMASem, (STPDSTCFG + DWDSIZE), 4/DMASIZE, DMASIZE, 0, 0);
}

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_mem_move_bf609.c $
**
*/
