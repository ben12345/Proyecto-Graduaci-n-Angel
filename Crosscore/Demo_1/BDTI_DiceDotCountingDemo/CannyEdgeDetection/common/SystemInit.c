/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      SystemInit.c
 * @brief     Contain the Intialization furnction.
 * @version:  $ $
 * @date:     $ $
 *
 * @details
 *            
 *
 */

#include <blackfin.h>
#include <builtins.h>

/*=============  D E F I N E S  =============*/
#define BITM_DDR_CFG_IFWID_16       (BITM_DMC_CFG_IFWID & ((uint32_t)2 << BITP_DMC_CFG_IFWID)) /* 16 bit interface width*/
#define BITM_DDR_CFG_SDRSIZE_1G  	(BITM_DMC_CFG_SDRSIZE & ((uint32_t)4 << BITP_DMC_CFG_SDRSIZE))/* 1Gb SDRAM size*/
#define BITM_DDR_CFG_SDRWID_16      (BITM_DMC_CFG_SDRWID & ((uint32_t)2 << BITP_DMC_CFG_SDRWID))/* 16 bit SDRAM width*/
#define BITM_DDR_CFG_EXTBANK_1      (BITM_DMC_CFG_EXTBANK & ((uint32_t)0 << BITP_DMC_CFG_EXTBANK)) /* 1 external bank*/

/* TR0 bits*/
#define BITM_DDR_TR0_TRCD4       	(BITM_DMC_TR0_TRCD & ((uint32_t)4<< BITP_DMC_TR0_TRCD))
#define BITM_DDR_TR0_TWTR2 			(BITM_DMC_TR0_TWTR & ((uint32_t)2 << BITP_DMC_TR0_TWTR))
#define BITM_DDR_TR0_TRP4			(BITM_DMC_TR0_TRP & ((uint32_t)4 << BITP_DMC_TR0_TRP))
#define BITM_DDR_TR0_TRAS12			(BITM_DMC_TR0_TRAS & ((uint32_t)12 << BITP_DMC_TR0_TRAS))
#define BITM_DDR_TR0_TRC15			(BITM_DMC_TR0_TRC & ((uint32_t)15 << BITP_DMC_TR0_TRC))
#define BITM_DDR_TR0_TMRD2			(BITM_DMC_TR0_TMRD & ((uint32_t)2 << BITP_DMC_TR0_TMRD))

/*TR1 bits*/
#define BITM_DDR_TR1_TRRD3			(BITM_DMC_TR1_TRRD & ((uint32_t)3 << BITP_DMC_TR1_TRRD))
#define BITM_DDR_TR1_TRFC32			(BITM_DMC_TR1_TRFC & ((uint32_t)32 << BITP_DMC_TR1_TRFC))
#define BITM_DDR_TR1_TREFVAL		(BITM_DMC_TR1_TREF & ((uint32_t)1934 << BITP_DMC_TR1_TREF))

/*TR2 bits*/
#define BITM_DDR_TR2_TCKE3			(BITM_DMC_TR2_TCKE & ((uint32_t)3 << BITP_DMC_TR2_TCKE))
#define BITM_DDR_TR2_TXP2			(BITM_DMC_TR2_TXP & ((uint32_t)2 << BITP_DMC_TR2_TXP))
#define BITM_DDR_TR2_TRTP2			(BITM_DMC_TR2_TRTP & ((uint32_t)2 <<BITP_DMC_TR2_TRTP))
#define BITM_DDR_TR2_TFAW13 		(BITM_DMC_TR2_TFAW & ((uint32_t)13 << BITP_DMC_TR2_TFAW))

/* MR bits*/
#define BITM_DDR_MR_WRRECOV3   		(BITM_DMC_MR_WRRECOV & ((uint32_t)3 << BITP_DMC_MR_WRRECOV ))
#define BITM_DDR_MR_WRRECOV4   		(BITM_DMC_MR_WRRECOV & ((uint32_t)4 << BITP_DMC_MR_WRRECOV ))
#define BITM_DDR_MR_CL4 			(BITM_DMC_MR_CL & ((uint32_t)4 << BITP_DMC_MR_CL))
#define BITM_DDR_MR_BLEN4			(BITM_DMC_MR_BLEN & ((uint32_t)2 << BITP_DMC_MR_BLEN))

/*EMR1 bits*/
#define BITM_DDR_EMR1_AL0			(BITM_DMC_EMR1_AL & ((uint32_t)0 << BITP_DMC_EMR1_AL))
#define BITM_DDR_EMR1_AL3			(BITM_DMC_EMR1_AL & ((uint32_t)3 << BITP_DMC_EMR1_AL))

/* CTL bits*/
#define BITM_DDR_CTL_RDTOWR1 		(BITM_DMC_CTL_RDTOWR & ((uint32_t)1 << BITP_DMC_CTL_RDTOWR))
#define BITM_DDR_CTL_RDTOWR2 		(BITM_DMC_CTL_RDTOWR & ((uint32_t)2 << BITP_DMC_CTL_RDTOWR))
#define DLL_LOCK_PERIOD (4500)

uint32_t adi_DMCamInit(void)
{
	uint32_t i;
	uint32_t nResult = 0;
	uint32_t  DSELval = (uint32_t)0;
	uint32_t CSELval = (uint32_t)0;
	uint32_t Dll_cycles = (uint32_t)0;

	/* Check for DDR2 IDLE*/
	while(!(*pREG_DMC0_STAT & BITM_DMC_STAT_IDLE))
	{

	}
	/* put the part into self refresh only if already initialized*/
	/*if((*pREG_DMC0_STAT & (uint32_t)BITM_DMC_STAT_MEMINITDONE)&&(*pREG_DMC0_STAT & (uint32_t)BITP_DMC_STAT_DLLCALDONE))*/
	{
		/*Enable self Refresh*/
		*pREG_DMC0_CTL |= BITM_DMC_CTL_SRREQ;
		/*Wait for SRACK to get set*/
		while(!(*pREG_DMC0_STAT & BITM_DMC_STAT_SRACK))
		{
			/* Do Nothing */
		}
	}

	/*calculate the DLL wait cycles*/
	DSELval = (*pREG_CGU0_DIV & BITM_CGU_DIV_DSEL) >> BITP_CGU_DIV_DSEL;
	CSELval = (*pREG_CGU0_DIV & BITM_CGU_DIV_CSEL) >> BITP_CGU_DIV_CSEL;
	Dll_cycles =(uint32_t)DLL_LOCK_PERIOD * DSELval/CSELval;            /* DLL Lock*/

	/* DLL Lock*/
	for (i=(uint32_t)0; i<Dll_cycles; i++)
	{
		/* Do Nothing*/
	}

	/*Wait for DLL to lock*/
	for (i=(uint32_t)0; i<Dll_cycles; i++)
	{
		/* Do Nothing*/
	}

	/*Get the DMC2 out of self refresh only if already initialized*/
	if((*pREG_DMC0_STAT & (uint32_t)BITM_DMC_STAT_MEMINITDONE)&&(*pREG_DMC0_STAT & (uint32_t)BITP_DMC_STAT_DLLCALDONE))
	{
		/*Disable self refresh*/
		*pREG_DMC0_CTL ^= (uint32_t)BITM_DMC_CTL_SRREQ;
		/*Wait for SRACK to clear*/
		while((*pREG_DMC0_STAT & (uint32_t)BITM_DMC_STAT_SRACK))
		{
			/* Do Nothing*/
		}
	}

	*pREG_DMC0_CFG 	= (BITM_DDR_CFG_IFWID_16)|(BITM_DDR_CFG_SDRWID_16)|(BITM_DDR_CFG_SDRSIZE_1G)|(BITM_DDR_CFG_EXTBANK_1);

	ssync();
	*pREG_DMC0_TR0 	= (BITM_DDR_TR0_TRCD4)|(BITM_DDR_TR0_TWTR2)|(BITM_DDR_TR0_TRP4 )|( BITM_DDR_TR0_TRAS12 )|(BITM_DDR_TR0_TRC15 )|(BITM_DDR_TR0_TMRD2);

	ssync();
	*pREG_DMC0_TR1 	= (BITM_DDR_TR1_TRRD3)|( BITM_DDR_TR1_TRFC32 )|(BITM_DDR_TR1_TREFVAL); /*(975)*/
	ssync();
	*pREG_DMC0_TR2 	= (BITM_DDR_TR2_TCKE3)|( BITM_DDR_TR2_TXP2)|(BITM_DDR_TR2_TRTP2)|(BITM_DDR_TR2_TFAW13);
	ssync();
	*pREG_DMC0_MR 	= (BITM_DDR_MR_WRRECOV4 )|( BITM_DDR_MR_CL4 )|(BITM_DDR_MR_BLEN4);
	ssync();
	*pREG_DMC0_EMR1 = (BITM_DMC_EMR1_RTT0 )|( BITM_DDR_EMR1_AL0);
	ssync();
	*pREG_DMC0_CTL 	= (BITM_DMC_CTL_INIT)|(BITM_DDR_CTL_RDTOWR2);
	ssync();
	/*Wait for initialization to complete*/
	while(!(*pREG_DMC0_STAT & BITM_DMC_STAT_MEMINITDONE))
	{
		/* Do Nothing*/
	}
	/*Wait for calibration to complete*/
	while(!(*pREG_DMC0_STAT & (uint32_t)BITM_DMC_STAT_DLLCALDONE))
	{
		/* Do Nothing*/
	}

	/*keep the DLLCALRDCNT reset value and only modify DATACYC*/
	*pREG_DMC0_DLLCTL= 0x54Bu;

#if 1
	(*pREG_SCB0_ARBW0)  = (0 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (1 << 24)  | 12;
    (*pREG_SCB0_ARBW0)  = (2 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (3 << 24)  | 0;
    (*pREG_SCB0_ARBW0)  = (4 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (5 << 24)  | 12;
    (*pREG_SCB0_ARBW0)  = (6 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (7 << 24)  | 4;
    (*pREG_SCB0_ARBW0)  = (8 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (9 << 24)  | 12;
    (*pREG_SCB0_ARBW0)  = (10 << 24)  | 2;
    (*pREG_SCB0_ARBW0)  = (11 << 24)  | 0;
    (*pREG_SCB0_ARBW0)  = (12 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (13 << 24)  | 7;
    (*pREG_SCB0_ARBW0)  = (14 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (15 << 24)  | 4;
    (*pREG_SCB0_ARBW0)  = (16 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (17 << 24)  | 12;
    (*pREG_SCB0_ARBW0)  = (18 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (19 << 24)  | 2;
    (*pREG_SCB0_ARBW0)  = (20 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (21 << 24)  | 12;
    (*pREG_SCB0_ARBW0)  = (22 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (23 << 24)  | 4;
    (*pREG_SCB0_ARBW0)  = (24 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (25 << 24)  | 12;
    (*pREG_SCB0_ARBW0)  = (26 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (27 << 24)  | 0;
    (*pREG_SCB0_ARBW0)  = (28 << 24)  | 5;
    (*pREG_SCB0_ARBW0)  = (29 << 24)  | 12;
    (*pREG_SCB0_ARBW0)  = (30 << 24)  | 2;
    (*pREG_SCB0_ARBW0)  = (31 << 24)  | 4;

    (*pREG_SCB0_ARBR0)  = (0 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (1 << 24)  | 12;
    (*pREG_SCB0_ARBR0)  = (2 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (3 << 24)  | 0;
    (*pREG_SCB0_ARBR0)  = (4 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (5 << 24)  | 12;
    (*pREG_SCB0_ARBR0)  = (6 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (7 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (8 << 24)  | 2;
    (*pREG_SCB0_ARBR0)  = (9 << 24)  | 12;
    (*pREG_SCB0_ARBR0)  = (10 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (11 << 24)  | 0;
    (*pREG_SCB0_ARBR0)  = (12 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (13 << 24)  | 7;
    (*pREG_SCB0_ARBR0)  = (14 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (15 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (16 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (17 << 24)  | 12;
    (*pREG_SCB0_ARBR0)  = (18 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (19 << 24)  | 2;
    (*pREG_SCB0_ARBR0)  = (20 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (21 << 24)  | 12;
    (*pREG_SCB0_ARBR0)  = (22 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (23 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (24 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (25 << 24)  | 12;
    (*pREG_SCB0_ARBR0)  = (26 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (27 << 24)  | 0;
    (*pREG_SCB0_ARBR0)  = (28 << 24)  | 5;
    (*pREG_SCB0_ARBR0)  = (29 << 24)  | 12;
    (*pREG_SCB0_ARBR0)  = (30 << 24)  | 2;
    (*pREG_SCB0_ARBR0)  = (31 << 24)  | 5;
#endif
	return nResult;

}

