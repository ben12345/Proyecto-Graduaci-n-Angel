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
* @file      PVPInput.c
*
* @brief     This is file which contains the configuration parameters for CONVOLUTION and PEC modules of PVP.
*
* @details   Contains various configuration parameters such as filter coefficients for CONVOLUTION module,
*            threshold(s)for PEC module etc.
*
*
*/

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rules_all:"Suppress MISRA checking for example code")
#endif

#include <drivers/pvp/adi_pvp.h>


ADI_PVP_CNV_KERNEL gKernelCoef[] =
                 {

                  {
                	 -1,  -2,  0,  2, 1,
                	 -4,  -8,  0,  8, 4,
                	 -6,  -12, 0,  12,6,      /* Sobel -Vertical mask.  */
                	 -4,  -8,  0,  8, 4,
                	 -1,  -2,  0,  2, 1
                  },
                  {  1,   4,   6,   4,  1,
                     2,   8,  12,   8,  2,
                     0,   0,   0,   0,  0,    /* Sobel-Horizontal mask. */
                    -2,  -8, -12,  -8, -2,
                    -1,  -4,  -6,  -4, -1
                  },
                  {
                	   99,     397,    695,    397,    99,
                	   397,   1986,   3277,   1986,   397,
                	   695,   3277,   5362,   3277,   695,
                	   397,   1986,   3277,   1986,   397,
                	   99,     397,    695,    397,    99,
                  }

                 };

ADI_PVP_CNV_CONTROL     ogCnvControl[] =
                                         {
		                                   {false, false , 0  },  /* 16 bit saturation, Duplicate pixel , shift by 6  */
                                           {false, false , 0  },  /* 16 bit saturation,Duplicate pixel  , shift by 6 */\
                                           {false, false , 15  },  /* 16 bit saturation,Duplicate pixel  , shift by 6 */
                                         };

ADI_PVP_PEC_CONTROL    oPECControl =   { false, false, false, false};  /* First order, 8Bits per bin, */

//-----------------------------------------------------------------------------------------------
// ADI BF60x Dice-Counting Demo Developed by Berkeley Design Technology, Inc. (BDTI)
//-----------------------------------------------------------------------------------------------
ADI_PVP_PEC_THRESHOLDS  oThresholds=  {2000, 2500, 0, 0 }; //{768,1024,0,0 };

ADI_PVP_IIM_CONTROL    oIIMControl =
                                     {
		                                ADI_PVP_IIM_MODE_RECTANGLE,    /* Mode : Rectangle  */
                                        ADI_PVP_IIM_WIDTH_32BIT,       /* o/p data width  */
                                        0                              /* Shift */
                                      };

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif
