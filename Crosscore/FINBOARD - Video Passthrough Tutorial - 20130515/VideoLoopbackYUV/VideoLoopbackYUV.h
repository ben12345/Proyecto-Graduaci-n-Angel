/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      VideoLoopbackYUV.h
 * @brief     Video loopback example API file.
 * @version:  $Revision: 10123 $
 * @date:     $Date: 2012-07-09 16:34:32 -0400 (Mon, 09 Jul 2012) $
 *
 * @details
 *            This is the primary API file for the video loopback example.
 *
 */

#ifndef _VIDEOLOOPBACKYUV_H_
#define _VIDEOLOOPBACKYUV_H_

/* Example return codes */
#define SUCCESS                         0
#define FAILURE                         1

/* External input clock frequency in Hz */
#define PROC_CLOCK_IN                   (25000000u)
/* Maximum core clock frequency in Hz */
#define PROC_MAX_CORE_CLOCK             (500000000u)
/* Maximum system clock frequency in Hz */
#define PROC_MAX_SYS_CLOCK              (250000000u)
/* Minimum VCO clock frequency in Hz */
#define PROC_MIN_VCO_CLOCK              (25000000u)
/* Required core clock frequency in Hz */
#define PROC_REQ_CORE_CLOCK             (500000000u)
/* Required system clock frequency in Hz */
#define PROC_REQ_SYS_CLOCK              (250000000u)

/* Adjust the macro value to increase or decrease the timeout
 *
 * Comment the macro to disable the timeout or set the macro to 0.
 * */
#define EXAMPLE_TIMEOUT                  500

#define VIDEOLOOPBACKYUV_720P

#endif /* _VIDEOLOOPBACKYUV_H_ */
