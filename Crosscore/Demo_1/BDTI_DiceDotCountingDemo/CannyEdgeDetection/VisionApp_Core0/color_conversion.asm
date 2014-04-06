/******************************************************************************
Copyright (c), 2009-2010 - Analog Devices Inc. All Rights Reserved.
This software is PROPRIETARY & CONFIDENTIAL to Analog Devices, Inc.
and its licensors.
*******************************************************************************

Title       : color_conversion.asm

Description : Captures the different color conversion codes.

******************************************************************************/

/*=============  I N C L U D E S   =============*/

/*=============  D E F I N E S  =============*/
#define STACK_PUSH_2    (4*2)
#define STACK_PUSH_3    (4*3)
#define STACK_PUSH_4    (4*4)
#define STACK_PUSH_5    (4*5)
#define STACK_PUSH_7    (4*7)
#define STACK_PUSH_8    (4*8)
#define ARGOFFSET       (4*3)
#define ARG4            (ARGOFFSET)
#define ARG5            (ARGOFFSET + 4)
#define ARG6            (ARGOFFSET + 8)

/*=============  D A T A  =============*/
.section adi_fastb1_app_r;

.global RGB2YCbCrPara;
.align 4;
RGB2YCbCrPara:
.byte2 _RGB2YCbCrPara[12]= -4850,    // -0.148 
                          14385,   // 0.439
                          -9535,   // -0.291
                          -12059,  // -0.368
                          14385,   // 0.439
                          -2327,   // -0.071
                          128,
                          128,
                          8421,       // 0.257
                          16515,   // 0.504
                          16,
                          3211;       // 0.098

RGB2YCbCrPara.end:

.global _opt_buffer;
.align 4;
.byte4 _opt_buffer[4] = 0x0,0x0,0x0,0x0;

/*=============  C O D E  =============*/
/**********************************************************************************

  Function              : rgb_yuv_656_blk

  Function description  : ARGB to UYVY.


***********************************************************************************/
.section adi_fast_app_code;
.align 8;
.global _rgb_yuv_656_blk;
_rgb_yuv_656_blk:
//
//  Input Data Format:
//          A R G B (A - Alpha, R - Red, G - Green, B - Blue )
// 
//  RGB -> YUV
//             Y  =  0.257*R + 0.504*G + 0.098*B + 16
//             Cr =  0.439*R - 0.368*G - 0.071*B + 128
//             Cb = -0.148*R - 0.291*G + 0.439*B + 128 
//   
// 
//  Output Data Format:
//         Y2 Cr Y1 Cb
//         where,
//            Cb = (Cb1 + Cb2)/2  for 4:2:2 conversion
//            Cr = (Cr1 + Cr2)/2
//
//reg      H       L
//----------------------
// R1     Cb0     Cr0  
// R4       x      Y0   
// R5     Cb1     Cr1  
// R4      Y1      x 
// R1 (Cb0+Cb1)/2 (Cr0+Cr1)/2    
//


       [--SP] = R4;
       [--SP] = R5;
       [--SP] = M2;         

       I0 = R0;                               /* input row    */
       I1 = R1;                               /* o/p          */
       P2 = [SP + STACK_PUSH_3 + ARG4];                /* height       */
       P1 = R2;                               /* width        */ 
       I2.H = RGB2YCbCrPara; I2.L = RGB2YCbCrPara;
       M2 = -24;                              /* Modifier to top of para */
       R0  = [I0++];
       (R3,R2) = BYTEUNPACK R1:0                              || R0 = [I2++];
       A0   = R3.L * R0.H, A1  = R3.L * R0.L                  || R0 = [I2++];
       LSETUP ($rgbyuv_blk_hght_st, $rgbyuv_blk_wdth_end) LC0 = P2;
$rgbyuv_blk_hght_st:
       LSETUP ($rgbyuv_blk_wdth_st, $rgbyuv_blk_wdth_end) LC1 = P1 >> 1;
$rgbyuv_blk_wdth_st:
       A0  += R2.H * R0.H, A1 += R2.H * R0.L                  || R0 = [I2++];
       R1.L = (A0 += R2.L * R0.H), R1.H = (A1 += R2.L * R0.L) || R0 = [I2++];
       R1   = R1+|+R0 (s)                                     || R0 = [I2++];
       A0   = R3.L * R0.L                                     || NOP;
       A0  += R2.H * R0.H                                     || R5 = [I2++];
       R4.L = (A0 += R2.L * R5.H)                             || R0 = [I0++] || I2 += M2;
      (R3,R2) = BYTEUNPACK R1:0                               || NOP;
       R4.L = R4.L + R5.L (S)                                 || R0 = [I2++]; 
       A0   = R3.L * R0.H, A1  = R3.L * R0.L                  || R0 = [I2++];
       A0  += R2.H * R0.H, A1 += R2.H * R0.L                  || R0 = [I2++];
       R5.L = (A0 += R2.L * R0.H), R5.H = (A1 += R2.L * R0.L) || R0 = [I2++];
       R5   = R5+|+R0 (s)                                     || R0 = [I2++];
       A0   = R3.L * R0.L                                     || NOP;
       A0  += R2.H * R0.H                                     || R0 = [I2++];
       R3.L = (A0 += R2.L * R0.H)                             || I2 += M2;
       R4.H = R3.L + R0.L (S)                                 || NOP;
       R1 = R1+|+R5                                           || NOP;
       R1 = R1 >> 1 (v)                                       || NOP;
       R5 = PACK(R4.L,R1.H)                                   || NOP;
       R4 = PACK(R4.H,R1.L)                                   || R0 = [I0++];
      (R3,R2) = BYTEUNPACK R1:0                               || R0 = [I2++];
       R4 = BYTEPACK(R5,R4)                                   || NOP;          /* Y2CrY1Cb */
$rgbyuv_blk_wdth_end:A0 = R3.L * R0.H, A1 = R3.L * R0.L       || R0 = [I2++] || [I1++] = R4;
               

       M2 = [SP++]; 
       R5 = [SP++];
       R4 = [SP++];
       RTS;

_rgb_yuv_656_blk.END:

