/*****************************************************************************
Copyright (c) 2009 - 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 8503 $
$Date: 2012-08-23 23:06:21 +0530 (Thu, 23 Aug 2012) $

Title       : adi_dma_func.asm

Description : DMA functions

*****************************************************************************/

/*=============  I N C L U D E S   =============*/

/*=============  D E F I N E S  =============*/

/*=============  E X T E R N A L S  =============*/
.extern _MDMA_D0CFG;
.extern _MDMA_D0DSC;
.extern _MDMA_S0CFG;
.extern _MDMA_S0DSC;
.extern _DMASem;
.extern _pDMA_STATUS;

/*=============  D A T A  =============*/

/*=============  C O D E  =============*/

/******************************************************************************

  Function              : dmastart

  Function description  : Clear the Semaphone DMASem and start the DMA

  Parameters            :
    pSrcDesc(In)    -   Pointer to Source Descriptor        - R0
    pDesDesc(In)    -   Pointer to Destination Descriptor   - R1

  Returns               : None

  Registers             : R0,R1,R2,P0,P1,P2

  Notes                 :

******************************************************************************/
#if defined(__ADSPBF609_FAMILY__)
.section    adi_appl_fast_prio0_code;
.align      4;
.global     _dmastart;
_dmastart:
    P1 = R0;
    R0 = 0 ;
    P0.l = _DMASem;
    P0.h = _DMASem;
    [P0] = R0;
    CLI R2;
    P0.l = _MDMA_S0DSC;
    P0.h = _MDMA_S0DSC;
    P2.l = _MDMA_S0CFG;
    P2.h = _MDMA_S0CFG;
    P0 = [P0];
    P2 = [P2];
    R0 = [P1 + 4];
    [P0] = P1;
    [P2] = R0;
    P1 = R1;
    P0.l = _MDMA_D0DSC;
    P0.h = _MDMA_D0DSC;
    P2.l = _MDMA_D0CFG;
    P2.h = _MDMA_D0CFG;
    P0 = [P0];
    P2 = [P2];
    R0 = [P1 + 4];
    [P0] = P1;
    [P2] = R0;
    SSYNC;
    STI R2;
    RTS;
_dmastart.END:
#else
.section    adi_appl_fast_prio0_code;
.align      4;
.global     _dmastart;
_dmastart:
    P1 = R0;
    R0 = 0 ;
    P0.l = _DMASem;
    P0.h = _DMASem;
    [P0] = R0;
    CLI R2;
    P0.l = _MDMA_S0DSC;
    P0.h = _MDMA_S0DSC;
    P2.l = _MDMA_S0CFG;
    P2.h = _MDMA_S0CFG;
    P0 = [P0];
    P2 = [P2];
    R0 = W[P1 + 4] (Z);
    [P0] = P1;
    W[P2] = R0;
    P1 = R1;
    P0.l = _MDMA_D0DSC;
    P0.h = _MDMA_D0DSC;
    P2.l = _MDMA_D0CFG;
    P2.h = _MDMA_D0CFG;
    P0 = [P0];
    P2 = [P2];
    R0 = W[P1 + 4] (Z);
    [P0] = P1;
    W[P2] = R0;
    SSYNC;
    STI R2;
    RTS;
_dmastart.END:
#endif

/******************************************************************************

  Function              : dmawait

  Function description  : waits for DMA variable DMASem

  Parameters            : None

  Returns               : None

  Registers             : P0, R0, R2

  Notes                 :

******************************************************************************/
.section    adi_appl_fast_prio0_code;
.align      4;
.global     _dmawait;
_dmawait:
    P0.l = _DMASem;
    P0.h = _DMASem;
    R0 = 0;         /* timeout error */
$wait_here:
    R2 = [P0];      /* DMASem value */
    CC = R2 == 0;   /* DMA not done? */
    IF CC JUMP $wait_here;
#if defined(__ADSPBF609__)
    P0.l = _pDMA_STATUS;
    P0.h = _pDMA_STATUS;
    p0 = [p0];
wait_dma_status:    
    R0 = [p0];
    R1 = 0x700;
    R0 = R0 & R1;
    CC = R0 == 0;
    if !cc jump wait_dma_status; 
#endif

    RTS;

_dmawait.END:

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_dma_func.asm $
**
*/
