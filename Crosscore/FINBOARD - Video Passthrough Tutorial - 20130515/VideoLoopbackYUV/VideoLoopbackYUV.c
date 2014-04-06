/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/
/*!
 * @file      VideoLoopbackYUV
 * @brief     Video loopback example source file.
 * @version:  $Revision: 10187 $
 * @date:     $Date: 2012-07-11 16:52:06 -0400 (Wed, 11 Jul 2012) $
 *
 * @details
 *            This is the primary source file for the video loopback example.
 *
 */


/*=============  I N C L U D E S   =============*/
/* Power Service includes */
#include <services/pwr/adi_pwr.h>
/* Interrupt Manager includes */
#include <services/int/adi_int.h>
/* SEC includes */
#include <services/int/adi_sec.h>
/* TWI driver includes */
#include <drivers/twi/adi_twi.h>
/* PPI driver includes */
#include <drivers/ppi/adi_ppi.h>
#include <blackfin.h>

#include <stdio.h>
#include <string.h>
#include "encoder.h"
#include "Sensor.h"
#include "VideoLoopbackYUV.h"
#include "finboard_bsp.h"

/* ADI initialization header */
#include "system/adi_initialize.h"
/*=============  D E F I N E S  =============*/

uint32_t NumFilledFrames = 0;
//extern void ConfigSoftSwitches_BF609(void);

void main(void)
{
    uint8_t *pFilledFrame   = NULL;
    uint8_t *pDisplayFrame  = NULL;
    bool     bDisplayEnabled = false;

    adi_initComponents(); /* auto-generated code */

    /* Adjust the core frequency */
    if (adi_pwr_Init (PROC_CLOCK_IN, PROC_MAX_CORE_CLOCK, PROC_MAX_SYS_CLOCK, PROC_MIN_VCO_CLOCK) != ADI_PWR_SUCCESS)
    {
        printf ("Failed to initialize Power service\n");
        return;
    }
    if(adi_pwr_SetFreq(PROC_REQ_CORE_CLOCK, PROC_REQ_SYS_CLOCK)!= ADI_PWR_SUCCESS )
    {
        printf ("Failed to initialize Power service\n");
        return;
    }

    /* Configure the Software controlled switches on BF609 EZ-Board */
    //ConfigSoftSwitches_BF609();
#if defined(VIDEOLOOPBACKYUV_720P)
    FINBOARD_CLK_Synth_Config_OUT4_74_25_MHz();
#else
    FINBOARD_CLK_Synth_Config_OUT4_27_00_MHz();
#endif
    FINBOARD_LED_Drivers_Init();
    FINBOARD_LED_Drivers_Config(1);

    /* Configure the sensor */
    if(ConfigureSensor() != SUCCESS)
    {
        printf("Failed to configure Sensor \n");
        return;
    }

    /* Configure the sensor for display */
    if(ConfigureEncoder() != SUCCESS)
    {
        printf("Failed to configure LCD \n");
        return;
    }
    FINBOARD_ADV7511_16bit_Mode();

    /* Submit the frame for filling */
    if(SubmitEmptyVideoFrame() != SUCCESS)
    {
        printf("Failed to submit empty video frame to the sensor \n");
        return;
    }

    /* Submit the next frame for filling */
    if(SubmitEmptyVideoFrame() != SUCCESS)
    {
        printf("Failed to submit empty video frame to the sensor \n");
        return;
    }

    /* Enable the sensor to capture the frames */
    if(EnableSensor(true) != SUCCESS)
    {
        printf("Failed to enable sensor \n");
        return;
    }

#if (EXAMPLE_TIMEOUT > 0)
    while(NumFilledFrames < EXAMPLE_TIMEOUT)
#else
    while(1)
#endif
    {
        /* Get filled frame from sensor */
        if(GetFilledVideoFrame(&pFilledFrame) != SUCCESS)
        {
            printf("Failed to get filled frame from the sensor \n");
            return;
        }

        if(pFilledFrame != NULL)
        {
            NumFilledFrames++;

            pDisplayFrame = pFilledFrame;

            /* Submit the next frame for filling */
            if(SubmitEmptyVideoFrame() != SUCCESS)
            {
                printf("Failed to submit empty video frame to the sensor \n");
                return;
            }

            if(bDisplayEnabled == false)
            {
                /* Submit the filled frame to encoder for display */
                if(DisplayFrame(pDisplayFrame) != SUCCESS)
                {
                    printf("Failed to submit the filled frame to LCD for display \n");
                    return;
                }
            }

            if(NumFilledFrames == 2)
            {
                /* Enable video display after submitting two frames */
                if(EnableDisplay(true) != SUCCESS)
                {
                    printf("Failed to enable video display \n");
                    return;
                }

                bDisplayEnabled = true;
            }
        }

        if(bDisplayEnabled == true)
        {
              uint8_t *pFrame;

              if(GetDisplayedFrame((void **)&pFrame) != SUCCESS)
              {
                  printf("Failed to get the displayed frame \n");
              }

              /* Submit the filled frame to the encoder for display */
              if(DisplayFrame(pDisplayFrame) != SUCCESS)
              {
                  printf("Failed to submit the filled frame to encoder for display \n");
                  return;
              }
          }
       }

   FINBOARD_LED_Drivers_Config(0);
    printf("All done \n");
}
