/******************************************************************************
Copyright (c), 2012 - Analog Devices Inc. All Rights Reserved.
This software is PROPRIETARY & CONFIDENTIAL to Analog Devices, Inc.
and its licensors.
*******************************************************************************

Title       : adi_font.h

Description : Header file for fonts.

******************************************************************************/

#ifndef __ADI_FONT_H__
#define __ADI_FONT_H__
/*=============  I N C L U D E S   =============*/

/*=============  D E F I N E S  =============*/

/*=============  D A T A  =============*/
#if defined(__GNUC__) || defined(__ECC__)

/* Font Data */
extern  unsigned int    *arial_12pt_fontmap;
extern  ADICharRange    arial_12pt_range;
extern  unsigned int    arial_12pt_range_length;

extern  unsigned int    *arial_13pt_fontmap;
extern  ADICharRange    arial_13pt_range;
extern  unsigned int    arial_13pt_range_length;

extern  unsigned int    *arial_14pt_fontmap;
extern  ADICharRange    arial_14pt_range;
extern  unsigned int    arial_14pt_range_length;

extern  unsigned int    *arial_15pt_fontmap;
extern  ADICharRange    arial_15pt_range;
extern  unsigned int    arial_15pt_range_length;

extern  unsigned int    *arial_20pt_fontmap;
extern  ADICharRange    arial_20pt_range;
extern  unsigned int    arial_20pt_range_length;

extern  unsigned int    *arial_22pt_fontmap;
extern  ADICharRange    arial_22pt_range;
extern  unsigned int    arial_22pt_range_length;

extern  unsigned int    *arial_24pt_fontmap;
extern  ADICharRange    arial_24pt_range;
extern  unsigned int    arial_24pt_range_length;

extern  unsigned int    *arial_26pt_fontmap;
extern  ADICharRange    arial_26pt_range;
extern  unsigned int    arial_26pt_range_length;


/*=============  C O D E  =============*/

#endif  /* defined(__GNUC__) || defined(__ECC__) */

#endif  /* __ADI_FONT_H__ */
