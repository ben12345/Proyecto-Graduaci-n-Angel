/*****************************************************************************
Copyright (c) 2010 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
$Revision: 9091 $
$Date: 2012-10-14 18:20:39 +0530 (Sun, 14 Oct 2012) $

Title       : adi_contours.h

Description : declarations specific to contour modules

*****************************************************************************/
#ifndef __ADI_CONTOURS_H__
#define __ADI_CONTOURS_H__

/*=============  I N C L U D E S   =============*/
#include "adi_image_tool_box.h"

/*==============  D E F I N E S  ===============*/
#define ADI_CONTOURS_NUM_LINES_IN_TILE  1
#define ADI_IMAGE_WIDTH                 ((uint32_t) 20)
#define ADI_IMAGE_HEIGHT                ((uint32_t) 20)
#define MAX_NUM_RUN_LEN_NODE            6000//100
/*=============  E X T E R N A L S  ============*/

/*
**  External Data section
*/

/*
**  External Function Prototypes
*/
extern
ADI_ITB_MODULE_STATUS   ADIContoursRLEWrapper(
                                              ADI_CONTOUR_DYNAMIC_MEM_ALLOC   *pDMA,
                                              int8_t                          *pIn,
                                              uint32_t                        nInputBytes,
                                              ADI_CONT_RUN_LEN_HDR            *pRunLenListHdr,
                                              uint32_t                        dimX,
                                              uint32_t                        dimY
                                              );

/*=============  D A T A    T Y P E S   =============*/
#endif /* __ADI_CONTOURS_H__ */

/*
**
** EOF: $URL: https://ipdc-video-s03.ad.analog.com/svn/ipdc-ptg/trunk/avcodecs/BDTi-II/DiceDemoApp/common/adi_contours.h $
**
*/
