//-----------------------------------------------------------------------------------------------
//
//     BBBBBBBBBBBBBBBBB   DDDDDDDDDDDDD       TTTTTTTTTTTTTTTTTTTTTTT  iiii
//     B::::::::::::::::B  D::::::::::::DDD    T:::::::::::::::::::::T i::::i
//     B::::::BBBBBB:::::B D:::::::::::::::DD  T:::::::::::::::::::::T  iiii
//     BB:::::B     B:::::BDDD:::::DDDDD:::::D T:::::TT:::::::TT:::::T
//       B::::B     B:::::B  D:::::D    D:::::DTTTTTT  T:::::T  TTTTTTiiiiiii
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T        i:::::i
//       B::::BBBBBB:::::B   D:::::D     D:::::D       T:::::T         i::::i
//       B:::::::::::::BB    D:::::D     D:::::D       T:::::T         i::::i
//       B::::BBBBBB:::::B   D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D     D:::::D       T:::::T         i::::i
//       B::::B     B:::::B  D:::::D    D:::::D        T:::::T         i::::i
//     BB:::::BBBBBB::::::BDDD:::::DDDDD:::::D       TT:::::::TT      i::::::i
//     B:::::::::::::::::B D:::::::::::::::DD        T:::::::::T      i::::::i
//     B::::::::::::::::B  D::::::::::::DDD          T:::::::::T      i::::::i
//     BBBBBBBBBBBBBBBBB   DDDDDDDDDDDDD             TTTTTTTTTTT      iiiiiiii
//
//-----------------------------------------------------------------------------------------------
// ADI BF60x Dice-Counting Demo Developed by Berkeley Design Technology, Inc. (BDTI)
//
// BDTI provides expert engineering services, including algorithm design and implementation of
// software for embedded vision applications. For questions regarding this demo or information
// on BDTI’s engineering services, contact BDTI:
//
// info@BDTI.com                      +1 510-451-1800                   http://www.BDTI.com
//
// Berkeley Design Technology, Inc.(BDTI) makes no warranty for the use of this software, which
// has been developed for demonstration purposes only.  This software is provided  "as is".  BDTI
// assumes no responsibility for any errors which may appear in this software, nor does it make
// a commitment to update the information contained herein. BDTI specifically disclaims any implied
// warranties of fitness for a particular purpose.  You are solely responsible for determining
// the suitability of this software for your purposes and you use it at your own risk. In no event
// shall BDTI be held liable for any damages of any kind arising out of your use of or inability to
// use this software.
//-----------------------------------------------------------------------------------------------

/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
* @file     PVPInit.c
*
* @brief     This is the primary source file for  initializing the PVP device.
*
* @details   This file contains all the necessary initialization function including the VSS module initialization.
*            for demonstrating the  edge extraction.
*
*/

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"Suppress MISRA checking for example code")
#endif

#include <stddef.h>
#include <blackfin.h>
#include <builtins.h>
#include <string.h>
#include <stdio.h>
#include <drivers/pvp/adi_pvp.h>
#include <drivers/vss/adi_vss.h>
#include "EdgeDetection.h"
#include "PVPInput.c"

#define PEC_OUT_ELEM_WIDTH    1

/* Form the chain by connecting modules */
static uint32_t ConnectModules(void);

/* PVP Device related callback function */
static void PVPCallback (void *pCBParam, ADI_PVP_EVENT  eEven, void  *pArg);

/* Pipe related callback function */
static void PipeCallback(void *pCBParam,ADI_PVP_EVENT  eEvent,void *pArg);


/* PEC output stream related callback */
static void PECOutStreamCallback(void *pCBParam, ADI_PVP_EVENT   eEvent,void  *pArg);


/* Functions which initialize and configure modules */
static uint32_t InitIPF0Module( uint32_t width, uint32_t height);
static uint32_t InitCONVModule(void);
static uint32_t InitPMAModule(void);
static uint32_t InitPECModule(void);

/* Creates the configuration */
static uint32_t CreateConfiguration(uint32_t width, uint32_t height);

/* Enable/Disable pipe */
static uint32_t EnablePipe(bool bEnable);

#ifdef ENABLE_IIM_MODULE
/* THC output stream related callback */
static void IIM0OutStreamCallback(
        void            *pCBParam,
        ADI_PVP_EVENT   eEvent,
        void            *pArg);
static uint32_t InitIIM0Module(void);
static uint8_t  IIM0OutStreamMem[ADI_PVP_STREAM_MEM_SIZE];   /* Memory required for THC Output Stream */
static uint8_t  OPF1ConfMem[ADI_PVP_OPF_MEM_SIZE];      /* Memory required for OPF module configuration */
static uint8_t  IIM0ModuleMem[ADI_PVP_IIM_MEM_SIZE];       /* Memory required for IIM module configuration */
#endif



/*
 * Memory Required for PVP
 */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      PipeMem[ADI_PVP_PIPE_MEM_SIZE];             /* Memory for Pipe */
/*
 * Memory required for configuration of modules
 */
/* Memory required for creating the configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      ConfigMem[ADI_PVP_CONF_MEM_SIZE];
/* Memory required for IPF module configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      IPF0ConfMem[ADI_PVP_IPF_MEM_SIZE];
/* Memory required for OPF module configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      OPF0ConfMem[ADI_PVP_OPF_MEM_SIZE];
/* Memory required for PMA module configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      PMAModuleMem[ADI_PVP_PMA_MEM_SIZE];
/* Memory required for CNV0 module configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      CONV0ModuleMem[ADI_PVP_CNV_MEM_SIZE];
/* Memory required for CNV1 module configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      CONV1ModuleMem[ADI_PVP_CNV_MEM_SIZE];
/* Memory required for CNV1 module configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      CONV2ModuleMem[ADI_PVP_CNV_MEM_SIZE];
/* Memory required for PEC module configuration */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      PECModuleMem[ADI_PVP_PEC_MEM_SIZE];
/* Memory required for PEC Output Stream */
#pragma section ("adi_fastb0_prio1_rw")
static uint8_t      PECOutStreamMem[ADI_PVP_STREAM_MEM_SIZE];
#pragma section ("adi_fastb0_prio1_rw")
static PVP_BUF_INFO *aPecFreeBuffers[NUM_OF_OUTPUT_BUFFER];
#pragma section ("adi_fastb0_prio1_rw")
static PVP_BUF_INFO *aPecFilledBuffers[NUM_OF_OUTPUT_BUFFER];
#pragma section ("adi_fastb0_prio1_rw")
static int32_t nPecFreeIndex;
#pragma section ("adi_fastb0_prio1_rw")
static int32_t nPecfilledIndex;
#pragma section ("adi_fastb0_prio1_rw")
/* PVP Device Handle */
static ADI_PVP_HANDLE           ghPVPDevice;

/* PVP Camera Pipe handle */
#pragma section ("adi_fastb0_prio1_rw")
static ADI_PVP_PIPE_HANDLE      ghCameraPipe;

/* PEC output stream handle */
#pragma section ("adi_fastb0_prio1_rw")
static ADI_PVP_STREAM_HANDLE    ghPECOutStream;

/* configuration handle */
#pragma section ("sdram_bank6")
static ADI_PVP_CONFIG_HANDLE    ghConfig;

/* Number of processed frames, updated by PEC output callback */
#pragma section ("adi_fastb0_prio1_rw")
static volatile uint32_t    gNumPECProcessedFrames;

#ifdef ENABLE_IIM_MODULE
#pragma alignment_region (32)
#pragma section ("sdram_bank6")
static PVP_BUF_INFO  PECBuff0;
#pragma section ("sdram_bank7")
static PVP_BUF_INFO  PECBuff1;
#pragma section ("sdram_bank6")
static PVP_BUF_INFO  PECBuff2;
#pragma section ("sdram_bank7")
static PVP_BUF_INFO  PECBuff3;
#pragma alignment_region_end

#pragma alignment_region (32)
#pragma section ("sdram_bank7")
static uint32_t  IIM0Buff0[IIM_BUFFER_SIZE];
#pragma section ("sdram_bank4_A")
static uint32_t  IIM0Buff1[IIM_BUFFER_SIZE];
#pragma section ("sdram_bank7")
static uint32_t  IIM0Buff2[IIM_BUFFER_SIZE];
#pragma section ("sdram_bank6")
static uint32_t  IIM0Buff3[IIM_BUFFER_SIZE];
#pragma alignment_region_end
#pragma section ("L2_sram")
static uint32_t *aIIMFreeBuffers[NUM_OF_OUTPUT_BUFFER];
#pragma section ("L2_sram")
static uint32_t *aIIMFilledBuffers[NUM_OF_OUTPUT_BUFFER];
#pragma section ("L2_sram")
static int32_t nIIMFreeIndex;
#pragma section ("L2_sram")
static int32_t nIIMfilledIndex;
/* IIM output stream handle */
#pragma section ("L2_sram")
static ADI_PVP_STREAM_HANDLE    ghIIM0OutStream;
#pragma section ("L2_sram")
uint32_t gNumIIM0ProcessedFrames=0;
#pragma section ("sdram_bank6")
static PVP_BUF_INFO  PECBuff_temp;
#else
#pragma alignment_region (32)
#pragma section ("sdram_bank6")
static PVP_BUF_INFO  PECBuff0;
#pragma section ("sdram_bank4_A")
static PVP_BUF_INFO  PECBuff1;
#pragma section ("sdram_bank6")
static PVP_BUF_INFO  PECBuff2;
#pragma section ("sdram_bank4_A")
static PVP_BUF_INFO  PECBuff3;
#pragma section ("sdram_bank6")
static PVP_BUF_INFO  PECBuff_temp;

#pragma alignment_region_end
#endif

/**
 * @brief       Initializes PVP device  and  sets up the processing pipe, submit the buffer to the output stream.
 *
 * @details     Initializes and configures the PVP device
 *
 * @param       None
 *
 * @return      SUCCESS : successfully initialized the device
 *              FAILURE : Failed to initialize the device.
 */

uint32_t InitPVP()
{
    /* PVP Result code */
    ADI_PVP_RESULT eResult;
     /* Reset number of processed frames */
    gNumPECProcessedFrames  =0;

    /* Open the PVP Device */
    if((eResult = adi_pvp_Open(
                  0,                          /* Device instance to be opened */
                  PVPCallback,                /* PVP device related callback */
                  NULL,                       /* Callback parameter */
                  &ghPVPDevice                /* Pointer to Device Handle */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to open PVP Device 0x%08X\n", eResult);
        return FAILURE;
    }

    /* Open the camera pipe */
    if((eResult = adi_pvp_OpenPipe (
                  ghPVPDevice,               /* Handle to the PVP Device */
                  ADI_PVP_PIPE_CAMERA,       /* Pipe type to be opened  */
                  PipeCallback,              /* Pipe related callback */
                  NULL,                      /* Callback parameter */
                  PipeMem,                   /* Memory required for the pipe operation */
                  ADI_PVP_PIPE_MEM_SIZE,     /* Size of the given memory in bytes */
                  &ghCameraPipe              /* Pointer to the pipe handle */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to open PVP Pipe 0x%08X\n", eResult);
        return FAILURE;
    }
    /* create the configurations */
    if(CreateConfiguration( INPUT_VIDEO_WIDTH , INPUT_VIDEO_HEIGHT) != SUCCESS)
    {
        return FAILURE;
    }
    if((eResult = adi_pvp_SubmitBuffer(
                    ghPECOutStream,                            /* Handle to the stream */
                  aPecFreeBuffers[nPecFreeIndex]->VideoData,   /* Pointer to the buffer */
                  ((INPUT_VIDEO_WIDTH * INPUT_VIDEO_HEIGHT)/4) /* Number of output elements */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to submit buffer 0 to  output stream 0x%08X\n", eResult);
        return FAILURE;
    }

    aPecFreeBuffers[nPecFreeIndex--] = NULL;

    /*Mark the status and  Advance the pointer */
    if((eResult = adi_pvp_SubmitBuffer(
                  ghPECOutStream,                                /* Handle to the stream */
                  aPecFreeBuffers[nPecFreeIndex]->VideoData,      /* Pointer to the buffer */
                  ((INPUT_VIDEO_WIDTH * INPUT_VIDEO_HEIGHT)/4)    /* Number of  output elements */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to submit buffer 1 to  output 0x%08X\n", eResult);
        return FAILURE;
    }
    aPecFreeBuffers[nPecFreeIndex--] = NULL;
#ifdef ENABLE_IIM_MODULE
    if((eResult = adi_pvp_SubmitBuffer(
                  ghIIM0OutStream,                            /* Handle to the stream */
                  aIIMFreeBuffers[nIIMFreeIndex],             /* Pointer to the buffer */
                  ((INPUT_VIDEO_WIDTH * INPUT_VIDEO_HEIGHT))  /* Number of output elements */
                 )) != ADI_PVP_SUCCESS)
     {
        REPORT_ERROR("Failed to submit buffer 0 to  output stream 0x%08X\n", eResult);
        return FAILURE;
    }

    aIIMFreeBuffers[nIIMFreeIndex--] = NULL;

    /*Mark the status and  Advance the pointer */
    if((eResult = adi_pvp_SubmitBuffer(
                  ghIIM0OutStream,                             /* Handle to the stream */
                  aIIMFreeBuffers[nIIMFreeIndex],              /* Pointer to the buffer */
                  ((INPUT_VIDEO_WIDTH * INPUT_VIDEO_HEIGHT))   /* Number of  output elements */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to submit buffer 1 to  output 0x%08X\n", eResult);
        return FAILURE;
    }
    aIIMFreeBuffers[nIIMFreeIndex--] = NULL;
#endif

    /* Apply the created configuration  ghConfig */
    if((eResult = adi_pvp_ApplyPipeConfig(ghConfig)) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to apply the pipe configuration 0x%08X\n", eResult);
        return FAILURE;
    }
   return  SUCCESS;
}

/**
 * @brief       Enables the  PVP device , PVP pipe and  video subsystem.
 *
 * @details     Enable the PVP device and initialize the VSS.
 *
 * @param       None
 *
 * @return      SUCCESS : successfully initialized the device
 *              FAILURE : Failed to initialize the device.
 */

uint32_t enablePVP()
{
    /* Setup pixel cross bar (Initializes the VSS )*/
    if(adi_vss_Init() != ADI_VSS_SUCCESS)
    {
        return FAILURE;
    }
    /* Sets the source from which the PVP device receives the pixels. */
    if(adi_vss_SetupPVPInput(0, ADI_VSS_DEVICE_PPIRX, INPUT_PPI_DEV_NUM) != ADI_VSS_SUCCESS)
    {
        return FAILURE;
    }
    /* Sets the source from which the PVP device receives the pixels. */
    if(adi_vss_EnablePPIRxBcast(INPUT_PPI_DEV_NUM,true) != ADI_VSS_SUCCESS)
    {
        return FAILURE;
    }


    /* Enable the pipe */
    if(EnablePipe(true) != SUCCESS)
    {
        return FAILURE;
    }

    /* Enable PVP device */
   if(adi_pvp_Enable(ghPVPDevice,true) != ADI_PVP_SUCCESS)
   {
       REPORT_ERROR("Failed to enable PVP \n");
       return FAILURE;
   }
   return(SUCCESS);
}
/**
 * @brief      Disable the  the  PVP device  and clear the configuration close the device.
 *
 * @param       None
 *
 * @return      SUCCESS : successfully disabled  the device
 *              FAILURE : Failed to disable the device.
 */

uint32_t ClosePVP(void )
{
       /* PVP Result code */
       ADI_PVP_RESULT eResult;

       /* Disable the pipe */
       if(EnablePipe(false) != SUCCESS)
       {
           return FAILURE;
       }
       /* Destroy the configuration */
       if((eResult = adi_pvp_DestroyPipeConfig(ghConfig)) != ADI_PVP_SUCCESS)
       {
           REPORT_ERROR("Failed to destroy the configuration 0x%08X\n", eResult);
           return FAILURE;
       }

       /* Disable  PVP */
       if((eResult = adi_pvp_Enable(
                     ghPVPDevice,         /* Disable the device  */
                     false
                     )) != ADI_PVP_SUCCESS)
       {
           REPORT_ERROR("Failed to disable  PVP: 0x%08X\n",eResult);
           return FAILURE;
       }
       return SUCCESS;
}

/**
 * @brief       Create the configuration. Initializes the required blocks of PVP. .
 *
 * @details     Initializes and configures the PVP device
 *
 * @param       None
 *
 * @return      SUCCESS : successfully Created the configuration.
 *              FAILURE : Failed to create the configuration.
 */

static uint32_t CreateConfiguration(uint32_t width, uint32_t height)
{
    /* PVP Result code */
    ADI_PVP_RESULT eResult;

    /* Create the pipe configuration for test */
    if((eResult = adi_pvp_CreatePipeConfig (
                  ghCameraPipe,              /* Pipe handle for which configuration to be created */
                  ConfigMem,                 /* Memory for the configuration */
                  ADI_PVP_CONF_MEM_SIZE,     /* Size of the given memory in bytes */
                  &ghConfig                   /* Pointer to the configuration handle */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to create the configuration 0x%08X\n", eResult);
        return FAILURE;
    }


    /* Initialize IPF0 module */
    if(InitIPF0Module(width, height) != SUCCESS)
    {
        return FAILURE;
    }

    /* Initialize Convolution module */
    if(InitCONVModule() != SUCCESS)
    {
        return FAILURE;
    }
    /* Initialize PMA module.  */
     if(InitPMAModule() != SUCCESS)
     {
         return FAILURE;
     }
     /* Initialize PEC module */
     if(InitPECModule() != SUCCESS)
     {
         return FAILURE;
     }
#ifdef ENABLE_IIM_MODULE
     /* Initialize IIM0 module */
     if(InitIIM0Module() != SUCCESS)
     {
         return FAILURE;
     }
#endif

    /* Form the module chain */
    if(ConnectModules() != SUCCESS)
    {
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * @brief       Enable/Disable  the pipe and output streams .
 *
 * @param       None
 *
 * @return      SUCCESS : successfully enable/disable the pipe.
 *              FAILURE : Failed enable/disable the pipe.
 */

static uint32_t EnablePipe(bool bEnable)
{
    /* PVP Result code */
     ADI_PVP_RESULT eResult;
     /* Disable PVP pipe */
     if(adi_pvp_EnablePipe(ghCameraPipe, bEnable) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to  enable/disable PVP pipe \n");
         return FAILURE;
     }

    if((eResult = adi_pvp_EnableStream(ghPECOutStream, bEnable)) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to enable/disable PEC output stream 0x%08X\n", eResult);
        return FAILURE;
    }
#ifdef ENABLE_IIM_MODULE
    if((eResult = adi_pvp_EnableStream(ghIIM0OutStream, bEnable)) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to enable/disable PEC output stream 0x%08X\n", eResult);
        return FAILURE;
    }
#endif
    return SUCCESS;
}
/**
 * @brief       Initializes and configures IPF0 module
 *
 * @param       None
 *
  * @return     SUCCESS : successfully initialized the IPF0 module.
 *              FAILURE : Failed initialize the IPF0 module.

 */
static uint32_t InitIPF0Module(uint32_t width, uint32_t height)
{
    ADI_PVP_RESULT           eResult;
    ADI_PVP_IPF_CONTROL      oIPFControl;    /* IPF control word initialization object */
    ADI_PVP_IPF_PIPE_CONTROL oIPFPipeControl;/* IPF pipe control word initialization object */
    /*
     * Initialize and configure the modules required for the test configuration
     */
    if((eResult = adi_pvp_ipf_Init (
                  ghConfig,                  /* Configuration handle in which IPF module to be initialized */
                  ADI_PVP_MODULE_IPF0,       /* Module ID for the module to be initialized (IPF 0) */
                  IPF0ConfMem,               /* Memory for IPF 1 configuration */
                  ADI_PVP_IPF_MEM_SIZE,      /* Size of the given memory in bytes */
                   NULL                       /* Reference configuration */
                   )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to initialize IPF0 module 0x%08X\n", eResult);
        return FAILURE;
    }


    /* Initialize the IPF control word */
    memset (&oIPFControl,0,sizeof(ADI_PVP_IPF_CONTROL));
    oIPFControl.bOutPort0       =   true;    /* Enable Output port 0 */
    oIPFControl.eOutPort1       =   ADI_PVP_IPF_PORT1_DISABLE;  /* Disable Output port 1 */
    oIPFControl.bOutPort2       =   false;   /* Disable Output port 2 */
    oIPFControl.eColorFormat    =   ADI_PVP_COLOR_FORMAT_YUV422_8BIT; /* input 16 bits */
    oIPFControl.bEnableUnpack   =   true;    /* Disable incoming data unpack */
    oIPFControl.bExtractRed     =   false;   /* Disable Red extraction */
    oIPFControl.bEnableSignExtn =   false;   /* Sign extension is not required */
    oIPFControl.bEnableQFormat  =   false;   /* No Q format for incoming data */

    if((eResult = adi_pvp_ipf_SetControlWord(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_IPF0,       /* IPF 0 module ID */
                  &oIPFControl               /* Control word to be set */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set IPF control word 0x%08X\n", eResult);
        return FAILURE;
    }

    /* Initialize IPF input control word */
    oIPFPipeControl.bEnableDrain     = false; /* Drain after the end of processing */
    oIPFPipeControl.bEnableStatusDMA = false;/* No status DMA */

    if((eResult = adi_pvp_ipf_SetPipeControlWord(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_IPF0,       /* Module ID of IPF 0 */
                  &oIPFPipeControl           /* Pipe control word to be set */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set IPF Pipe control word 0x%08X\n", eResult);
        return FAILURE;
    }

    /* Set the number of frames the test should operate on */
    if((eResult = adi_pvp_ipf_SetFrameCount(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_IPF0,       /* Module ID of IPF 0 */
                  NUM_OF_FRAMES              /* Number of test frames */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set frame count 0x%08X\n", eResult);
        return FAILURE;
    }

    /* Set frame size */
    if((eResult = adi_pvp_ipf_SetROISize(
                  ghConfig,                /* Configuration handle */
                  ADI_PVP_MODULE_IPF0,     /* Module ID of IPF 0 */
                  width,             /* Frame Width */
                  height             /* Frame Height */
                  )) != ADI_PVP_SUCCESS)
      {
          REPORT_ERROR("Failed to set frame size 0x%08X\n", eResult);
          return FAILURE;
      }
    return SUCCESS;
}

/**
 * @brief       Initializes and configures CONV module
 *
 * @param       None
 *
 * @return      SUCCESS : Successfully initialized CONVOLUTION module of PVP device.
 *              FAILURE : Failed to initialized CONVOLUTION module.
 */
static uint32_t InitCONVModule(void)
{
    ADI_PVP_RESULT          eResult;
    /* Initialize CONV module */
    if((eResult = adi_pvp_cnv_Init (
                  ghConfig,                  /* Configuration handle in which CONV module to be initialized */
                  ADI_PVP_MODULE_CNV0,       /* Module ID for the module to be initialized (CONV-0) */
                  CONV0ModuleMem,            /* Memory for CONV configuration */
                  ADI_PVP_CNV_MEM_SIZE,      /* Size of the given memory in bytes */
                  NULL                       /* Reference configuration */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to initialize CONV0 module 0x%08X\n", eResult);
        return FAILURE;
    }
    if((eResult = adi_pvp_cnv_SetControlWord(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV0,       /* Module ID of CNV */
                  &ogCnvControl[0]           /* CNV Control word */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set CONV control word 0x%08X\n", eResult);
        return FAILURE;
    }
    /* Set the coefficients for CNV0 */
    if((eResult = adi_pvp_cnv_SetCoef(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV0,       /* Module ID of CONV */
                  &gKernelCoef[0]            /* Coefficient matrix */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set the coefficients for CNV block 0x%08X\n", eResult);
        return FAILURE;
    }

    /* No scaling */
    if((eResult = adi_pvp_cnv_SetScaleValue(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV0,       /* Module ID of CONV */
                  0,                         /* Horizontal Scaling factor */
                  0                          /* Vertical Scaling factor */
              )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set scaling  value 0x%08X\n", eResult);
        return FAILURE;
    }

    /* Initialize CONV module */
    if((eResult = adi_pvp_cnv_Init (
                  ghConfig,                  /* Configuration handle in which CONV module to be initialized */
                  ADI_PVP_MODULE_CNV1,       /* Module ID for the module to be initialized (CONV 1) */
                  CONV1ModuleMem,            /* Memory for CONV configuration */
                  ADI_PVP_CNV_MEM_SIZE,      /* Size of the given memory in bytes */
                  NULL                       /* Reference configuration */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to initialize CONV1 module 0x%08X\n", eResult);
        return FAILURE;
    }
    if((eResult = adi_pvp_cnv_SetControlWord(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV1,       /* Module ID of CNV */
                  &ogCnvControl[1]           /* CNV Control word */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set CONV1 control word 0x%08X\n", eResult);
        return FAILURE;
    }
    /*  Set the coefficients for CNV1 */
    if((eResult = adi_pvp_cnv_SetCoef(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV1,       /* Module ID of CONV */
                  &gKernelCoef[1]            /* Coefficient matrix */
                  )) != ADI_PVP_SUCCESS)
    {
       REPORT_ERROR("Failed to set the coefficients for CNV block 0x%08X\n", eResult);
       return FAILURE;
    }

    /* No scaling  */
    if((eResult = adi_pvp_cnv_SetScaleValue(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV1,       /* Module ID of CONV-1 */
                  0,                         /* Horizontal Scaling factor */
                  0                          /* Vertical Scaling factor */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to scaling  value 0x%08X\n", eResult);
        return FAILURE;
    }
    /* Initialize CONV module */
    if((eResult = adi_pvp_cnv_Init (
                  ghConfig,                 /* Configuration handle in which CONV module to be initialized */
                  ADI_PVP_MODULE_CNV2,      /* Module ID for CONV-2 */
                  CONV2ModuleMem,           /* Memory for CONV configuration */
                  ADI_PVP_CNV_MEM_SIZE,     /* Size of the given memory in bytes */
                  NULL                      /* Reference configuration */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to initialize CONV2 module 0x%08X\n", eResult);
        return FAILURE;
    }
    if((eResult = adi_pvp_cnv_SetControlWord(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV2,       /* Module ID of CNV */
                  &ogCnvControl[2]           /* CNV Control word */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set CONV2 control word 0x%08X\n", eResult);
        return FAILURE;
     }
     /* Set the coefficients for CNV2 */
     if((eResult = adi_pvp_cnv_SetCoef(
                   ghConfig,                /* Configuration handle */
                   ADI_PVP_MODULE_CNV2,     /* Module ID of CONV */
                   &gKernelCoef[2]          /* Coefficient matrix */
                    )) != ADI_PVP_SUCCESS)
  {
   REPORT_ERROR("Failed to set the coefficients for CNV2 block 0x%08X\n", eResult);
    return FAILURE;
  }

    /* Set the shift value to 0, to skip the shifter block */
    if((eResult = adi_pvp_cnv_SetScaleValue(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV2,       /* Module ID of CONV */
                  0,                         /* Horizontal Scaling factor */
                  0                          /* Vertical Scaling factor */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to scaling  value 0x%08X\n", eResult);
        return FAILURE;
    }

    return SUCCESS;
}

/**
 * @brief       Initializes and configures PMA module
 *
 * @param       None
 *
 * @return      SUCCESS : Successfully initialized PMA module of PVP device.
 *              FAILURE : Failed to initialized PMA module.
 */
static uint32_t InitPMAModule(void)
{
    ADI_PVP_RESULT          eResult;
    /* Initialize PMA module */
    if((eResult = adi_pvp_pma_Init (
                  ghConfig,                  /* Configuration handle in which PMA module to be initialized */
                  ADI_PVP_MODULE_PMA,        /* Module ID for the module to be initialized (PMA ) */
                  PMAModuleMem,              /* Memory for PMA configuration */
                  ADI_PVP_PMA_MEM_SIZE,      /* Size of the given memory in bytes */
                  NULL                       /* Reference configuration */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to initialize PMA module 0x%08X\n", eResult);
        return FAILURE;
    }
    return SUCCESS;
}
/**
 * @brief       Initializes and configures PEC module
 *
 * @param       None
 *
 * @return      SUCCESS : Successfully initialized PEC module of PVP device.
 *              FAILURE : Failed to open/configure PEC.

 */
static uint32_t InitPECModule(void)
{
    ADI_PVP_RESULT          eResult;
    ADI_PVP_OPF_CONTROL     oOPFControl;
    /* Initialize  queue for PEC  output buffer management */
    aPecFreeBuffers[0]=&PECBuff0;
    aPecFreeBuffers[1]=&PECBuff1;
    aPecFreeBuffers[2]=&PECBuff2;
    aPecFreeBuffers[3]=&PECBuff3;
    aPecFilledBuffers[0]=aPecFilledBuffers[1]=aPecFilledBuffers[2]=aPecFilledBuffers[3]=NULL;
    nPecFreeIndex = NUM_OF_OUTPUT_BUFFER-1;
    nPecfilledIndex =-1;
    /* Initialize OPF0 module */
    if((eResult = adi_pvp_opf_Init (
                  ghConfig,                  /* Configuration handle in which OPF module to be initialized */
                  ADI_PVP_MODULE_OPF0,       /* Module ID for the module to be initialized (OPF 0) */
                  OPF0ConfMem,               /* Memory for OPF 0 configuration */
                  ADI_PVP_OPF_MEM_SIZE,      /* Size of the given memory in bytes */
                  NULL                       /* Reference configuration */
                 )) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to initialize OPF0 module 0x%08X\n", eResult);
         return FAILURE;
     }

     /* Initialize OPF control word */
     oOPFControl.eInDataWidth    =  ADI_PVP_OPF_IN_DATA_8BIT;  /* Input data width set to 32 bit */
     oOPFControl.eOutDataWidth   =  ADI_PVP_OPF_OUT_DATA_32BIT; /* Output data width set to 32 bit */

     oOPFControl.bUseUpper16bits =  false;   /* Use only lower 16 bits (Not valid for 32 bit) */
     oOPFControl.bFinishSignal   =  true;    /* Enable finish signaling */

     if((eResult = adi_pvp_opf_SetControlWord(
                   ghConfig,                 /* Configuration handle */
                   ADI_PVP_MODULE_OPF0,      /* Module ID of OPF 0 */
                   &oOPFControl              /* OPF Control word */
                   )) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to set OPF0 control word 0x%08X\n", eResult);
         return FAILURE;
    }
    /* Open stream for PEC output */
    if((eResult = adi_pvp_OpenStream (
                  ghCameraPipe,              /* Pipe in which the stream to be opened */
                  ADI_PVP_STREAM_CAMERA0,    /* Stream to be opened */
                  ADI_PVP_STREAM_DIR_OUT,    /* Stream direction is output */
                  PECOutStreamMem,           /* Memory required for stream operation */
                  ADI_PVP_STREAM_MEM_SIZE,   /* Size of the given memory in bytes */
                  PEC_OUT_ELEM_WIDTH*4,      /* Element width in bytes */
                  &ghPECOutStream            /* Pointer to the Stream Handle */
                )) != ADI_PVP_SUCCESS)
    {
         REPORT_ERROR("Failed to open the PEC output stream 0x%08X\n", eResult);
         return FAILURE;
     }
     /* Register callback for Output stream */
     if((eResult = adi_pvp_RegisterStreamCallback(
                   ghPECOutStream,           /* Stream Handle */
                   PECOutStreamCallback,     /* Callback function */
                   ghPECOutStream            /* Callback parameter */
                   )) != ADI_PVP_SUCCESS)
     {
        REPORT_ERROR("Failed to register callback for output stream 0x%08X\n", eResult);
        return FAILURE;
     }

     /* Register callback for Output stream */
     if((eResult = adi_pvp_EnableStreaming(ghPECOutStream,true)) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to enable the streaming for  output stream 0x%08X\n", eResult);
         return FAILURE;
     }

    if((eResult = adi_pvp_pec_Init (
                  ghConfig,                  /* Configuration handle in which PEC module to be initialized */
                  ADI_PVP_MODULE_PEC,        /* Module ID for the module to be initialized (PEC ) */
                  PECModuleMem,              /* Memory for PEC configuration */
                  ADI_PVP_PEC_MEM_SIZE,      /* Size of the given memory in bytes */
                  NULL                       /* Reference configuration */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to initialize PEC module 0x%08X\n", eResult);
        return FAILURE;
    }
    /* Initialize PEC Control word */
    if((eResult = adi_pvp_pec_SetControlWord(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_PEC,        /* Module ID for PEC */
                  &oPECControl               /* PEC control word to be set */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set PEC control word 0x%08X\n", eResult);
        return FAILURE;
    }
    if((eResult = adi_pvp_pec_SetThresholds(
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_PEC,        /* Module ID for PEC */
                  &oThresholds               /* Threshold setting. */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to set PEC threshold 0x%08X\n", eResult);
        return FAILURE;
    }
    return SUCCESS;
}

#ifdef ENABLE_IIM_MODULE
/**
 * @brief       Initializes and configures IIM0 module, and corresponding output stream.
 *
 * @param       None
 *
 * @return     SUCCESS : successfully initialized the IIM0   module.
 *              FAILURE : Failed initialize the IIM0  module.

 */
static uint32_t InitIIM0Module()
{
     ADI_PVP_RESULT          eResult;
     ADI_PVP_OPF_CONTROL     oOPFControl;

     aIIMFreeBuffers[0]=&IIM0Buff0[0];
     aIIMFreeBuffers[1]=&IIM0Buff1[0];
     aIIMFreeBuffers[2]=&IIM0Buff2[0];
     aIIMFreeBuffers[3]=&IIM0Buff3[0];
     aIIMFilledBuffers[0]=aIIMFilledBuffers[1]=aIIMFilledBuffers[2]=aIIMFilledBuffers[3]=NULL;
     nIIMFreeIndex = NUM_OF_OUTPUT_BUFFER-1;
     nIIMfilledIndex =-1;

     /* Initialize IIM module */
     if((eResult = adi_pvp_iim_Init (
                   ghConfig,                 /* Configuration handle in which IIM module to be initialized */
                   ADI_PVP_MODULE_IIM0,      /* Module ID for the module to be initialized (IIM 0) */
                   IIM0ModuleMem,            /* Memory for IIM configuration */
                   ADI_PVP_IIM_MEM_SIZE,     /* Size of the given memory in bytes */
                   NULL                      /* Reference configuration */
                   )) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to initialize IIM0 module 0x%08X\n", eResult);
         return FAILURE;
     }
    if((eResult = adi_pvp_iim_SetControlWord (
                   ghConfig,                 /* Configuration handle in which IIM module to be initialized */
                  ADI_PVP_MODULE_IIM0,      /* Module ID for the module to be initialized (IIM 0) */
                  &oIIMControl
                  )) != ADI_PVP_SUCCESS)
    {
          REPORT_ERROR("Failed to set the control word for  IIM0 module 0x%08X\n", eResult);
          return FAILURE;
    }


    /* Initialize OPF3 module */
     if((eResult = adi_pvp_opf_Init (
                   ghConfig,                 /* Configuration handle in which OPF module to be initialized */
                   ADI_PVP_MODULE_OPF1,      /* Module ID for the module to be initialized (OPF 1) */
                   OPF1ConfMem,              /* Memory for OPF 3 configuration */
                   ADI_PVP_OPF_MEM_SIZE,     /* Size of the given memory in bytes */
                   NULL                      /* Reference configuration */
                   )) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to initialize OPF1 module 0x%08X\n", eResult);
         return FAILURE;
     }

     /* Initialize OPF control word */
     oOPFControl.eInDataWidth    =  ADI_PVP_OPF_IN_DATA_32BIT;  /* Input data width set to 32 bit */
     oOPFControl.eOutDataWidth   =  ADI_PVP_OPF_OUT_DATA_32BIT; /* Output data width set to 32 bit */
     oOPFControl.bUseUpper16bits =  false;   /* Use only lower 16 bits (Not valid for 32 bit) */
     oOPFControl.bFinishSignal   =  true;    /* Enable finish signaling */

     if((eResult = adi_pvp_opf_SetControlWord(
                   ghConfig,                 /* Configuration handle */
                   ADI_PVP_MODULE_OPF1,      /* Module ID of OPF 1 */
                   &oOPFControl              /* OPF Control word */
                   )) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to set OPF1 control word 0x%08X\n", eResult);
         return FAILURE;
     }
     /* Open stream for THC output */
     if((eResult = adi_pvp_OpenStream (
                   ghCameraPipe,             /* Pipe in which the stream to be opened */
                   ADI_PVP_STREAM_CAMERA1,   /* Stream to be opened */
                   ADI_PVP_STREAM_DIR_OUT,   /* Stream direction is output */
                   IIM0OutStreamMem,         /* Memory required for stream operation */
                   ADI_PVP_STREAM_MEM_SIZE,  /* Size of the given memory in bytes */
                   IIM_OUT_ELEM_WIDTH,       /* Element width in bytes */
                   &ghIIM0OutStream          /* Pointer to the Stream Handle */
                   )) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to open the IIM0 output stream 0x%08X\n", eResult);
         return FAILURE;
     }
     /* Register callback for Output stream */
     if((eResult = adi_pvp_RegisterStreamCallback(
                   ghIIM0OutStream,          /* Stream Handle */
                   IIM0OutStreamCallback,    /*Callback function pointer */
                   ghIIM0OutStream           /*callback parameter*/
                   )) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed to register callback function to  output stream 0x%08X\n", eResult);
         return FAILURE;
     }

     /* Enabling the streaming*/
     if((eResult = adi_pvp_EnableStreaming(ghIIM0OutStream,true)) != ADI_PVP_SUCCESS)
     {
         REPORT_ERROR("Failed  Enable streaming  0x%08X\n", eResult);
         return FAILURE;
     }

     return SUCCESS;
}

/**
 * @brief       IIM0 stream related callback
 *
 * @param [in]  pCBParam    Callback parameter that was supplied to the PVP driver
 * @param [in]  eEvent      Event to be notified
 * @param [in]  pArg        Argument related to the event. Please see the
 *                          enumeration ADI_PVP_EVENT to know more about the
 *                          arguments related to each event.
 *
 * @return      None
 */
static void IIM0OutStreamCallback(
        void            *pCBParam,
        ADI_PVP_EVENT    eEvent,
        void            *pArg)
{
    uint32_t i;
    static uint32_t IIM0buffError=0,nStatus;
    uint32_t *ptemp;
    switch(eEvent)
    {
        case ADI_PVP_EVENT_BUFFER_PROCESSED:
            nStatus= cli();
            if(nIIMFreeIndex < 0 )
            {
                ptemp =(uint32_t *)pArg;
            }
            else
            {
                ptemp = aIIMFreeBuffers[0];
                for(i=0;i<(nIIMFreeIndex);i++)
                {
                    aIIMFreeBuffers[i]= aIIMFreeBuffers[i+1];
                }
                aIIMFreeBuffers[nIIMFreeIndex] = NULL;
                if(nIIMFreeIndex >=0 )
                {
                    nIIMFreeIndex--;
                }
                aIIMFilledBuffers[++nIIMfilledIndex] =(uint32_t *)pArg;
            }
            sti(nStatus);
            adi_pvp_SubmitBuffer(pCBParam, ptemp, (INPUT_VIDEO_WIDTH * INPUT_VIDEO_HEIGHT));
            gNumIIM0ProcessedFrames++;
            break;

        default:
            break;
    }
     return;
}
/**
 * @brief       Gets the address of the buffer containing the PEC output.
 *
 * @param       None
 *
 * @return      None

 */

void   GetIIMOutputBuffer (void **const ppVideoBuffer)
{

    ADI_PVP_RESULT eResult;
    int32_t i;
    uint32_t nStatus;
    *ppVideoBuffer = NULL;
    if(nIIMfilledIndex < 0 )
    {
        return;
    }
    else
    {
        nStatus= cli();
        *ppVideoBuffer = aIIMFilledBuffers[0];
        for(i=0;i<(nIIMfilledIndex);i++)
        {
            aIIMFilledBuffers[i]= aIIMFilledBuffers[i+1];
        }
        aIIMFilledBuffers[nIIMfilledIndex] = NULL;
        if(nIIMfilledIndex >= 0 )
        {
            nIIMfilledIndex--;
        }
        sti(nStatus);
    }
}
/**
 * @brief      Move the  buffer  to free queue
 *
 * @param
 *           Pbuffer : Buffer to be moved to free queue.
 *
 * @return      none

 */
void FreeIIMOutputBuffer(void *Pbuffer)
{
        /* Mark the buffer associated with the node as free */
        uint32_t i,nStatus;
        nStatus= cli();
        if( nIIMFreeIndex <  NUM_OF_OUTPUT_BUFFER)
        {
             nIIMFreeIndex++;
             aIIMFreeBuffers[nIIMFreeIndex] =(uint32_t *)Pbuffer;

        }
        sti(nStatus);
}

#endif
/**
 * @brief       Connects the modules to form the processing chain.
 *
 * @param       None
 *
 * @return      SUCCESS : Successfully connected the modules.
 *              FAILURE : Failed to connect the modules.

 */
static uint32_t ConnectModules(void)
{
    ADI_PVP_RESULT  eResult;


    /*
                                           +=======+
                                           |       |
                                    +----->|       |------+
                                    |      | CNV0  |      |
                                    |      |       |      |
                   +=======+        |      +=======+      |P0
EPPI              |        |        |                +========+      +========+      +========+
----->IPF0--+-- > |        |------->|                |        |      |        | P0   |        |
                  |  CNV2  |        |                |        |----->|        |----->|        |-----> Memory [OUTPUT BUFFER]
                  |        |        |                |  PMA   |      |  PEC   |      |  OPF0  |
                  +========+        |                |        |      |        |      |        |
                                    |                +========+      +========+      +========+
                                    |      +=======+      | P1
                                    |      |       |      |
                                    +----->|       |------+
                                           | CNV1  |
                                           |       |
                                           +=======+


     */


    /* Connect IPF0 and CNV2 */
    if((eResult = adi_pvp_ConnectModule (
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV2 ,      /* Destination module */
                  0u,                        /* Destination port number */
                  ADI_PVP_MODULE_IPF0,       /* Source module */
                  0u                         /* Source port number */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to connect IPF0 and CONV2 modules 0x%08X\n", eResult);
        return FAILURE;
    }
    /* Connect CNV2 and CNV0 */
    if((eResult = adi_pvp_ConnectModule (
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV0 ,      /* Destination module */
                  0u,                        /* Destination port number */
                  ADI_PVP_MODULE_CNV2,       /* Source module */
                  0u                         /* Source port number */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to connect CONV2 and CONV0 modules 0x%08X\n", eResult);
        return FAILURE;
    }


    /* Connect CNV2 and CNV1 */
    if((eResult = adi_pvp_ConnectModule (
                    ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_CNV1,       /* Destination module */
                  0u,                        /* Destination port number */
                  ADI_PVP_MODULE_CNV2 ,      /* Source module */
                  0u                         /* Source port number */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to connect CONV2 and CONV1 modules 0x%08X\n", eResult);
        return FAILURE;
    }
    /* Connect CNV0 and PMA( port 0) */
    if((eResult = adi_pvp_ConnectModule (
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_PMA,        /* Destination module */
                  0u,                        /* Destination port number */
                  ADI_PVP_MODULE_CNV0 ,      /* Source module */
                  0u                         /* Source port number */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to connect CONV0 and PMA modules 0x%08X\n", eResult);
        return FAILURE;
    }
    /* Connect CNV1 and PMA( port 1) */
    if((eResult = adi_pvp_ConnectModule (
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_PMA,        /* Destination module */
                  1u,                        /* Destination port number */
                  ADI_PVP_MODULE_CNV1 ,      /* Source module */
                   0u                         /* Source port number */
                 )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to connect CONV1 and PMA modules 0x%08X\n", eResult);
        return FAILURE;
    }

    /* Connect PMA and PEC */
    if((eResult = adi_pvp_ConnectModule (
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_PEC,        /* Destination module */
                  0u,                        /* Destination port number */
                  ADI_PVP_MODULE_PMA ,       /* Source module */
                  2u                         /* Source port number */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to connect PMA and PEC modules 0x%08X\n", eResult);
        return FAILURE;
    }
#ifdef ENABLE_IIM_MODULE
    /* Connect PEC and OPF0 */
   if((eResult = adi_pvp_ConnectModule (
                 ghConfig,                  /* Configuration handle */
                 ADI_PVP_MODULE_IIM0,       /* Destination module */
                 0u,                        /* Destination port number */
                 ADI_PVP_MODULE_PEC ,       /* Source module */
                 1u                         /* Source port number */
                 )) != ADI_PVP_SUCCESS)
   {
       REPORT_ERROR("Failed to connect PEC and OPF1 modules 0x%08X\n", eResult);
       return FAILURE;
   }
   /* Connect PEC and OPF0 */
  if((eResult = adi_pvp_ConnectModule (
                ghConfig,                  /* Configuration handle */
                ADI_PVP_MODULE_OPF1,       /* Destination module */
                0u,                        /* Destination port number */
                ADI_PVP_MODULE_IIM0 ,       /* Source module */
                0u                         /* Source port number */
                )) != ADI_PVP_SUCCESS)
  {
      REPORT_ERROR("Failed to connect PEC and OPF1 modules 0x%08X\n", eResult);
      return FAILURE;
  }

#endif
     /* Connect PEC and OPF0 */
    if((eResult = adi_pvp_ConnectModule (
                  ghConfig,                  /* Configuration handle */
                  ADI_PVP_MODULE_OPF0,       /* Destination module */
                  0u,                        /* Destination port number */
                  ADI_PVP_MODULE_PEC ,       /* Source module */
                  0u                         /* Source port number */
                  )) != ADI_PVP_SUCCESS)
    {
        REPORT_ERROR("Failed to connect PEC and OPF0 modules 0x%08X\n", eResult);
        return FAILURE;
    }
    return SUCCESS;
}
/**
 * @brief       PVP Device related callback
 *
 * @details     Callback through which PVP device related events are notified.
 *              Application can choose the the events that it is interested in
 *              and set the Mask accordingly using the function
 *              adi_pvp_SetIRQMask.
 *
 * @param [in]  pCBParam    Callback parameter that was supplied to the driver
 * @param [in]  eEvent      Event to be notified
 * @param [in]  pArg        Argument related to the event. Please see the
 *                          enumeration ADI_PVP_EVENT to know more about the
 *                          arguments related to each event.
 *
 * @return      None
 *
 * @sa          adi_pvp_SetIRQMask
 *
 */
static void PVPCallback (
        void            *pCBParam,
        ADI_PVP_EVENT    eEvent,
        void            *pArg
)
{
    /* PVP device related events here. Events which are independent of
     * PIPE
     */
    return;
}


/**
 * @brief       Pipe related callback
 *
 * @details     Callback through which PVP pipe related events are notified.
 *              Events such as configuration DMA complete will be notified
 *              using this callback (Not the status events)
 *
 * @param [in]  pCBParam    Callback parameter that was supplied to the PVP driver
 * @param [in]  eEvent      Event to be notified
 * @pArg  [in]  pArg        Argument related to the event. Please see the
 *                          enumeration ADI_PVP_EVENT to know more about the
 *                          arguments related to each event.
 *
 * @return      None
 */
static void PipeCallback(
        void            *pCBParam,
        ADI_PVP_EVENT   eEvent,
        void            *pArg)
{

    /* Handle the events */
    switch(eEvent)
    {
        case ADI_PVP_EVENT_CONFIG_PROCESSED:
            break;

        default:
            /* We are not expecting any PVP pipe related events. It should be an error */
            REPORT_ERROR("Unexpected PVP Pipe related event \n");
            break;
    }

    return;
}

/**
 * @brief       PECOuput stream related callback
 *
 * @param [in]  pCBParam    Callback parameter that was supplied to the PVP driver
 * @param [in]  eEvent      Event to be notified
 * @param [in]  pArg        Argument related to the event. Please see the
 *                          enumeration ADI_PVP_EVENT to know more about the
 *                          arguments related to each event.
 *
 * @return      None
 */
static void PECOutStreamCallback(
        void            *pCBParam,
        ADI_PVP_EVENT    eEvent,
        void            *pArg)
{
    static uint32_t PECBuffError=0;
    PVP_BUF_INFO   *ptemp;
    uint32_t nStatus;
    int32_t i;
    switch(eEvent)
    {
        case ADI_PVP_EVENT_BUFFER_PROCESSED:
            nStatus= cli();
            if(nPecFreeIndex < 0 )
            {
            	 ptemp =(PVP_BUF_INFO *)&PECBuff_temp;
            }
            else
            {
                ptemp = aPecFreeBuffers[0];
                for(i=0;i<(nPecFreeIndex);i++)
                {
                    aPecFreeBuffers[i]= aPecFreeBuffers[i+1];
                }
                aPecFreeBuffers[nPecFreeIndex] = NULL;
                if(nPecFreeIndex >=0 )
                {
                    nPecFreeIndex--;
                }
            }
            sti(nStatus);
            if((uint32_t)&PECBuff_temp !=(uint32_t)pArg)
            aPecFilledBuffers[++nPecfilledIndex] =(PVP_BUF_INFO *)pArg;
            adi_pvp_SubmitBuffer(pCBParam, ptemp->VideoData, (INPUT_VIDEO_WIDTH * INPUT_VIDEO_HEIGHT)/4);
            gNumPECProcessedFrames++;

            break;
        default:
            break;
    }
   return;
}

/**
 * @brief       Gets the address of the buffer containing the PEC output.
 *
 * @param       None
 *
 * @return      None

 */

void   GetPECOutputBuffer (void **const ppVideoBuffer)
{

    ADI_PVP_RESULT eResult;
    int32_t i;
    uint32_t nStatus;
    *ppVideoBuffer = NULL;
    if(nPecfilledIndex < 0 )
    {
        return;
    }
    else
    {
        nStatus= cli();
        *ppVideoBuffer = aPecFilledBuffers[0];
        for(i=0;i<(nPecfilledIndex);i++)
        {
            aPecFilledBuffers[i]= aPecFilledBuffers[i+1];
        }
        aPecFilledBuffers[nPecfilledIndex] = NULL;
        if(nPecfilledIndex >= 0 )
        {
            nPecfilledIndex--;
        }
        sti(nStatus);
    }
}
/**
 * @brief       Mark the buffer status an free so that it can be re used.
 *
 * @param       None
 *
 * @return      none

 */
void FreePECOutputBuffer(void *Pbuffer)
{

       /* Process the PEC output data */
        /* Mark the buffer associated with the node as free */
        uint32_t i,nStatus;
        nStatus= cli();
        if( nPecFreeIndex <  NUM_OF_OUTPUT_BUFFER)
        {
             nPecFreeIndex++;
             aPecFreeBuffers[nPecFreeIndex] =(PVP_BUF_INFO *)Pbuffer;

        }
        sti(nStatus);
}
