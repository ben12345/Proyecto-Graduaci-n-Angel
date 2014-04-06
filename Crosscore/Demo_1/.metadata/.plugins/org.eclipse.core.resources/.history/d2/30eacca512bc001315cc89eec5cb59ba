/*
 ******************************************************************************
 Copyright (c) 2012 - Analog Devices Inc. All Rights Reserved.
 This software is proprietary & confidential to Analog Devices, Inc.
 and its licensors.
 ******************************************************************************
 $Revision: 8621 $
 $Date: 2012-09-12 09:52:49 +0530 (Wed, 12 Sep 2012) $

 Title: Edge Trace Demo

 Description:
 This file contains example on how to use ADI VAT Edge Detectors module to trace PVP-PEC1 output

 *****************************************************************************/

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_5_6 :"Using same names for identifiers,for easy readability and understanding.")
#pragma diag(suppress:misra_rule_17_4:"Pointer arithmetic.")
#pragma diag(suppress:misra_rule_8_7:"Global variables are all placed in a single place.")
#endif /* _MISRA_RULES */

#define ETSI_SOURCE

/*=============  I N C L U D E S   =============*/
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <adi_types.h>
#include <stdio.h>
#include "adi_vat.h"
#include "adi_codec.h"
#include "EdgeDetection.h"

/*=============  D E F I N E S  =============*/
/* Bits per pixel. */

#define ED_DEMO_MAX_EDGE_PERCENT           1u

#define ED_MEM_BLOCK0_NUM_BYTES            ADI_VAT_ED_MEM_BLOCK0_NUM_BYTES
#define ED_MEM_BLOCK1_NUM_BYTES            ADI_VAT_ED_MEM_BLOCK1_NUM_BYTES(ED_DEMO_MAX_WIDTH)
#define ED_MEM_BLOCK2_NUM_BYTES_PVP_MODE   ADI_VAT_ED_MEM_BLOCK2_NUM_BYTES_PVP_MODE(ED_DEMO_MAX_WIDTH)
#define ED_MEM_BLOCK4_NUM_BYTES            ADI_VAT_ED_MEM_BLOCK4_NUM_BYTES(ED_DEMO_MAX_EDGE_PERCENT,ED_DEMO_MAX_WIDTH,ED_DEMO_MAX_HEIGHT)
#define ED_MEM_BLOCK5_NUM_BYTES_PVP_MODE   ADI_VAT_ED_MEM_BLOCK5_NUM_BYTES_PVP_MODE(ED_DEMO_MAX_WIDTH)

#pragma section ("L2_sram")
ADIVATConfigImage oConfigImage;
#pragma section ("L2_sram")
ADIVATROIInfo oROIInfo;
#pragma section ("L2_sram")
ADICodecHandle hEdgeDetector;
#pragma section ("L2_sram")
ADIVATConfigImage *pConfigImage = &oConfigImage;
#pragma section ("L2_sram")
ADIVATConfigDMA oConfigDMA;

#pragma section("adi_fastb0_prio0_rw")
static ADIMem goMemBlock[ADI_VAT_ED_NUM_MEMORY_BLOCKS];
#pragma section("adi_fastb0_prio0_rw")
static int32_t gaMemBlock0L1A[ED_MEM_BLOCK1_NUM_BYTES/4];
#pragma section("adi_fastb0_prio0_rw")
static int32_t gaMemBlock1L1A[ED_MEM_BLOCK2_NUM_BYTES_PVP_MODE/4];
#pragma section("adi_fastb1_prio0_rw")
#pragma alignment_region (32)
static int32_t gaMemBlock1L1B[ED_MEM_BLOCK2_NUM_BYTES_PVP_MODE/4];
#pragma section("adi_slow_prio0_rw")
#pragma alignment_region_end
static int32_t gaMemBlock0L3Cached[ED_MEM_BLOCK4_NUM_BYTES];
/*=============  D A T A  =============*/

/* State buffers for Edge detector module. */
#pragma section("adi_fastb0_prio0_rw")
static int32_t gaVATEDMemBlock0[ADI_VAT_ED_MEM_BLOCK0_NUM_BYTES];

/* Edge Detect instance array. */
#pragma section("adi_slow_prio0_rw")
static ADICodecInstance oEdgeDetect;


int32_t EdgeDetectCreatePVPMode(ADICodecHandle *phEdgeDetector, ADIVATConfigDMA *pConfigDMA,
                         ADIVATConfigImage *pConfigImage);



/*=============  C O D E  =============*/

/*
 **  Function Prototype section
 */


static void AllocateMemoryPVPMode(ADIMemMap *poMemMap);


static int32_t EdgeDetectSetConfigPVPMode(ADICodecHandle hEdgeDetector,
                                          ADIVATConfigDMA *pConfigDMA,
                                          ADIVATConfigImage *pConfigImage);


/*****************************************************************************
 Function: InitEdgeTrace

 Function description: Initialization function for edge trace.

 Parameters: None

 Returns:
 SUCCESS  - Successfully initialized the edge trace .
 FAILURE  - Failed to initialize the edge trace .
 *****************************************************************************/
uint32_t InitEdgeTrace(void)
{
    pConfigImage->nSize = sizeof(ADIVATConfigImage);
    pConfigImage->nVersion.nVersion  = ADI_VAT_VERSION;
    pConfigImage->nBitsPerInputSample = ED_DEMO_BITS_PER_PIXEL;
    pConfigImage->nStride = ED_DEMO_INPUT_STRIDE;
    pConfigImage->nHeight = ED_DEMO_MAX_HEIGHT;
    pConfigImage->nWidth = ED_DEMO_MAX_WIDTH;
    oConfigDMA.nSize = sizeof(ADIVATConfigDMA);
    oConfigDMA.nVersion.nVersion  = ADI_VAT_VERSION;
    oConfigDMA.nDMAType = 1;
    oConfigDMA.pBaseAddress = (void *)REG_DMA21_DSCPTR_NXT;
    oConfigDMA.eDMAWidth = ADI_VAT_DMA_128BIT;
    if(EdgeDetectCreatePVPMode(&hEdgeDetector,&oConfigDMA,pConfigImage) != 0)
    {
    	return(FAILURE);
    }
    return(SUCCESS);
}

/*****************************************************************************
 Function: EdgeDetectCreate

 Function description:
 Creates the module.

 Parameters:
 poEDDemo     - Pointer to array of demo info struture.
 pConfigDMA   - Pointer to DMA Config structure.
 pConfigImage - Pointer to image config structure.

 Returns:
 0 - Success.
 1 - Failure.
 *****************************************************************************/
int32_t EdgeDetectCreatePVPMode(ADICodecHandle *phEdgeDetector, ADIVATConfigDMA *pConfigDMA,
                         ADIVATConfigImage *pConfigImage)

{
    int32_t nResult = 0;
    ADIMemDMAService oDMABase;
    ADIMemDMAService *pDMABase;
    uint32_t nInstID;
    ADICodecHandle hEdgeDetector;
    ADIMemMap oMemMap;


    AllocateMemoryPVPMode(&oMemMap);

    if (pConfigDMA != NULL)
    {
        pDMABase = &oDMABase;
        pDMABase->nSize = sizeof(ADIMemDMAService);
        pDMABase->nVersion.nVersion = ADI_VAT_VERSION;
        pDMABase->nMDMA1BaseAddress = (uint32_t) pConfigDMA->pBaseAddress;
        pDMABase->nMDMA2BaseAddress = 0u;
    }
    else
    {
        pDMABase = NULL;
    }

    /* Create instance. */
    oEdgeDetect.nSize = sizeof(ADICodecInstance);
    oEdgeDetect.nVersion.nVersion = ADI_VAT_VERSION;
    hEdgeDetector = ADIVATEdgeDetectCodecNew(&oEdgeDetect,
                                              &oMemMap,
                                              pDMABase,
                                              &nInstID);
    if (hEdgeDetector == NULL)
    {
        printf("Error in CodecNew at %s %d\n", __FILE__, __LINE__);
        nResult = 1;
    }
    else
    {
        /* Store. */
        *phEdgeDetector = hEdgeDetector;
        /* Set configuration parameters */
        nResult = EdgeDetectSetConfigPVPMode(hEdgeDetector,
        									pConfigDMA,
                                             pConfigImage);

    }

    return nResult;
}

/*****************************************************************************
 Function: EdgeDetect

 Function description:
 This function detects the edge. Calls main processing function


 Parameters:
  pInput  - Pointer to input pixels.
  pOutput  - Pointer to output buffer.

 Returns:
 SUCCESS  - Success.
 FAILURE  - Failure.
 *****************************************************************************/

int32_t EdgeTracePVPOut(uint8_t *pInput, uint8_t *pOutput)
{
    ADICodecData oInData;
    ADICodecData oOutData;
    ADIVATEdgeDetectInputMetaData oInMetaData;

    ADIVATROIInfo ROIInfo;
    ROIInfo.nROIHeight = INPUT_VIDEO_HEIGHT;
    ROIInfo.nROIWidth = INPUT_VIDEO_WIDTH;
    ROIInfo.nROIStartY =0;
    ROIInfo.nROIStartX = 0;
    /* Assign input meta data. Not relevant for PVP Mode*/
    oInMetaData.nThresholdLo = 0;
    oInMetaData.nThresholdHi = 0;
    oInMetaData.pROIInfo = NULL;/// &ROIInfo;

    oInData.pData = (void *) pInput;
    oOutData.pData = (void *) pOutput;
    oInData.nNumBytes = INPUT_VIDEO_WIDTH*INPUT_VIDEO_HEIGHT;

    if (hEdgeDetector->pProcess(hEdgeDetector,
                                &oInData,
                                &oInMetaData,
                                &oOutData,
                                NULL) != ADI_CODEC_STATUS_OK)
    {
        return(FAILURE);
    }
    return(SUCCESS);
}


/*****************************************************************************
 Function: EdgeDetectSetConfigPVPMode.

 Function description:
 Initializes the edge detector module by calling the set config functions
 with suitable value.

 Parameters:
 poEDDemo     - Pointer to array of demo info struture.
 pConfigDMA   - Pointer to DMA Config structure.
 pConfigImage - Pointer to image config structure.

 Returns:
 0 - Success.
 1 - Failure.
 *****************************************************************************/
static int32_t EdgeDetectSetConfigPVPMode(ADICodecHandle hEdgeDetector,
                                          ADIVATConfigDMA *pConfigDMA,
                                          ADIVATConfigImage *pConfigImage)
{
    ADICodecStatus eResult = ADI_CODEC_STATUS_OK;
    int32_t nResult = 0;
    ADIVATConfigPVP oConfigPVP;

    /* Initialize. */

    oConfigPVP.nSize = sizeof(ADIVATConfigPVP);
    oConfigPVP.nVersion.nVersion = ADI_VAT_VERSION;




    eResult = hEdgeDetector->pSetConfig(hEdgeDetector,
                                        ADI_CODEC_CONFIG_INPUT,
                                        pConfigImage);

    if (eResult != ADI_CODEC_STATUS_OK)
    {
        printf("Error in SetConfig at %s %d\n", __FILE__, __LINE__);
    }
    else
    {
        /* Input set config. */
        oConfigPVP.nMode = 1;

        eResult = hEdgeDetector->pSetConfig(hEdgeDetector,
                                            ADI_CODEC_CONFIG_PVP,
                                            &oConfigPVP);

    }

    if (eResult != ADI_CODEC_STATUS_OK)
    {
        printf("Error in SetConfig at %s %d\n", __FILE__, __LINE__);
    }
    else
    {

        if (pConfigDMA != NULL)
        {
            eResult = hEdgeDetector->pSetConfig(hEdgeDetector,
                                                ADI_CODEC_CONFIG_DMA,
                                                pConfigDMA);

            if (eResult != ADI_CODEC_STATUS_OK)
            {
                printf("Error in SetConfig at %s %d\n", __FILE__, __LINE__);
            }

        }

    }

    if (eResult != ADI_CODEC_STATUS_OK)
    {
        nResult = 1;
    }

    return nResult;
}

/*****************************************************************************
 Function: AllocateMemoryPVPMode.

 Function description:
 Allocates memory for Edge detector module. Fills memory
 map input and output structures with appropriate buffer pointers

 Parameters:
 poMemMap - Pointer to memory map structures.
 poEDDemo - Pointer to  demo info structure.

 Returns:
 None.
 *****************************************************************************/
static void AllocateMemoryPVPMode(ADIMemMap *poMemMap)
{

    /* Initialize instance memory required for decoder */
    poMemMap->nNumMemBlocks = ADI_VAT_ED_NUM_MEMORY_BLOCKS;
    poMemMap->nSize = sizeof(ADIMemMap);
    poMemMap->nVersion.nVersion = ADI_VAT_VERSION;
    poMemMap->pMemBlocks = &goMemBlock[0];

    /* Mem block 0 */
    poMemMap->pMemBlocks[0].nMemSize = ED_MEM_BLOCK0_NUM_BYTES;
    poMemMap->pMemBlocks[0].pMem = &gaVATEDMemBlock0[0];

    /* Mem block 1*/
    poMemMap->pMemBlocks[1].nMemSize = ED_MEM_BLOCK1_NUM_BYTES;
    poMemMap->pMemBlocks[1].pMem = &gaMemBlock0L1A[0];

    /* Mem block 2*/
    poMemMap->pMemBlocks[2].nMemSize = ED_MEM_BLOCK2_NUM_BYTES_PVP_MODE;
    poMemMap->pMemBlocks[2].pMem = &gaMemBlock1L1A[0];

    /* Mem block 3, Not need for PVP mode. */
    poMemMap->pMemBlocks[3].nMemSize = 0u;
    poMemMap->pMemBlocks[3].pMem = NULL;

    /* Mem block 4 */
    poMemMap->pMemBlocks[4].nMemSize = ED_MEM_BLOCK4_NUM_BYTES;
    poMemMap->pMemBlocks[4].pMem = &gaMemBlock0L3Cached[0];

    /* Mem block 5 for DMA . This shall be in L1 */
    poMemMap->pMemBlocks[5].nMemSize = ED_MEM_BLOCK5_NUM_BYTES_PVP_MODE;
    poMemMap->pMemBlocks[5].pMem = &gaMemBlock1L1B[0];

}


#ifdef _MISRA_RULES
/* MISRA rules 17.4 and 8.4. */
#pragma diag(pop)
#endif /* _MISRA_RULES */

/*
 **
 ** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/vat_ed_pvp.c $
 **
 */
