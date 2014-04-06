/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      adi_sigprocess.h
 * @brief     Common definitions for both graphics and contour
 * @version:  $Revision: 8662 $
 * @date:     $Date: 2012-09-13 18:51:00 +0530 (Thu, 13 Sep 2012) $
 *
 * @details
 *            This is the common include file used by both graphics and contour.
 *
 */

#ifndef __ADI_SIGPROCESS_H__
#define __ADI_SIGPROCESS_H__

/*=============  I N C L U D E S   =============*/

/* OSAL includes */
#include <adi_osal.h>
/* Power Service includes */
#include <services/pwr/adi_pwr.h>
/* Interrupt Manager includes */
#include <services/int/adi_int.h>
/* SEC includes */
#include <services/int/adi_sec.h>
#include <stdio.h>
#include <string.h>
#include "adi_image_tool_box.h"

/*=============  FUNCTION PROTOTYPES =============*/
void itoa(int32_t DotCount,char *aString2);
void DrawEdge(void *pInput);
void display_count(void );

/*=============  D E F I N E S  =============*/
/* Maximum number of dots that can be marked */
#define MAX_OBJ_INFO 50

/*** structure to hold the Information about the dot in the dice ***/
typedef struct  CONTOUR_INFO
{
    /*Bounding rectangle information to draw rectangle around the detected dots by the graphics */
	ADI_CONT_BOUNDING_RECTANGLE  aCONTOURInfo[MAX_OBJ_INFO];
    /* Coulour to be used to draw the bounding rectangle */
	uint32_t   nColour[MAX_OBJ_INFO];
    /* Number of detected objects */
	uint32_t   nNumberOfObjects;
	/*Number of detected dots*/
	uint32_t nCONTOURCount;

}CONTOUR_INFO;

/*************Function prototypes *****************/
uint32_t ADI_FindContours( uint8_t *pImage, uint32_t nWidth, uint32_t nHeight );


#endif /* __ADI_SIGPROCESS_H__ */
