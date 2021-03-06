/*********************************************************************************

Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

This software is proprietary and confidential.  By using this software you agree
to the terms of the associated Analog Devices License Agreement.

*********************************************************************************/

/*
* Use the GPIO service to Toggle LED's when the push buttons are pressed
* on the BF609 EZ-Board.
*/

#include <services/gpio/adi_gpio.h>
#include "EdgeDetection.h"
#include "finboard_bsp.h"
#include <stdio.h>
/* memory for two callbacks */
#define GPIO_MEMORY_SIZE (ADI_GPIO_CALLBACK_MEM_SIZE*5u)

#pragma section("adi_appl_slow_noprio_rw")
static uint8_t gpioMemory[GPIO_MEMORY_SIZE];
volatile uint32_t nBoundingRectFlag;
#if defined(FINBOARD)
volatile uint32_t nIllumination;
#endif

static void pinIntCallback(ADI_GPIO_PIN_INTERRUPT ePinInt, uint32_t PinIntData,  void *pCBParam)
{
    switch(ePinInt)
	{
#if !defined(FINBOARD)
	case ADI_GPIO_PIN_INTERRUPT_0:
		/* push button 1 */
		if (PinIntData & ADI_GPIO_PIN_10)
		{
			/* mark the flag */
			nBoundingRectFlag++;
			if(nBoundingRectFlag > 2 )
			{
				nBoundingRectFlag =0;
			}
		}
		break;
	case ADI_GPIO_PIN_INTERRUPT_3:
		/* push button 2 */
		if (PinIntData & ADI_GPIO_PIN_1)
		{
			/* mark the flag */
			nIllumination++;
			if(nIllumination > 7 )
			{
				nIllumination =0;
			}
		}
		break;
 #else // !defined(FINBOARD)
	case ADI_GPIO_PIN_INTERRUPT_1:
		/* push button 1 */
		if (PinIntData & ADI_GPIO_PIN_9)
		{
			/* mark the flag */
			nBoundingRectFlag++;
			if(nBoundingRectFlag > 2 )
			{
				nBoundingRectFlag =0;
			}
		}
		/* push button 2 */
		if (PinIntData & ADI_GPIO_PIN_10)
		{
			/* mark the flag */
			nIllumination++;
			if(nIllumination > 7 )
			{
				nIllumination =0;
			}
		}
		break;
#endif // #if defined(FINBOARD)
	default:
		break;
	}
}

uint32_t Init_GPIO(void)
{


    uint32_t gpioMaxCallbacks;
 	ADI_GPIO_RESULT eResult;
     /* init the GPIO service */
	if((eResult = adi_gpio_Init(
			(void*)gpioMemory,
			GPIO_MEMORY_SIZE,
			&gpioMaxCallbacks))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	}

#if !defined(FINBOARD)
	/*
	 * Setup Push Button 1
	 */
	/* set GPIO input */
	if((eResult = adi_gpio_SetDirection(
			ADI_GPIO_PORT_B,
	    ADI_GPIO_PIN_10,
	    ADI_GPIO_DIRECTION_INPUT))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	}


	/* set edge sense mode (PORT B is connected to Pin Interrupt 0) */
	if((eResult = adi_gpio_SetPinIntEdgeSense(
			ADI_GPIO_PIN_INTERRUPT_0,
	    ADI_GPIO_PIN_10,
	    ADI_GPIO_SENSE_RISING_EDGE))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};

	/* register pinint 0 callback */
	if((eResult = adi_gpio_RegisterCallback(
			ADI_GPIO_PIN_INTERRUPT_0,
	    ADI_GPIO_PIN_10,
	    pinIntCallback,
	    (void*)0))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};;

	/* set pin interrupt mask */
	if((eResult = adi_gpio_EnablePinInterruptMask(
			ADI_GPIO_PIN_INTERRUPT_0,
	    ADI_GPIO_PIN_10,
	    true))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	};

	/*
	 * Setup Push Button 2
	 */
	/* set GPIO input */
	if((eResult = adi_gpio_SetDirection(
			ADI_GPIO_PORT_E,
	    ADI_GPIO_PIN_1,
	    ADI_GPIO_DIRECTION_INPUT))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	}


	/* set edge sense mode (PORT E is connected to Pin Interrupt 3) */
	if((eResult = adi_gpio_SetPinIntEdgeSense(
			ADI_GPIO_PIN_INTERRUPT_3,
	    ADI_GPIO_PIN_1,
	    ADI_GPIO_SENSE_RISING_EDGE))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};

	/* register pinint 0 callback */
	if((eResult = adi_gpio_RegisterCallback(
			ADI_GPIO_PIN_INTERRUPT_3,
	    ADI_GPIO_PIN_1,
	    pinIntCallback,
	    (void*)0))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};;

	/* set pin interrupt mask */
	if((eResult = adi_gpio_EnablePinInterruptMask(
			ADI_GPIO_PIN_INTERRUPT_3,
	    ADI_GPIO_PIN_1,
	    true))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	};
 #else // !defined(FINBOARD)
	/*
	 * Setup Push Button 1
	 */
	/* set GPIO input */
	if((eResult = adi_gpio_SetDirection(
			ADI_GPIO_PORT_C,
	    ADI_GPIO_PIN_9,
	    ADI_GPIO_DIRECTION_INPUT))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	}


	/* set edge sense mode (PORT C is connected to Pin Interrupt 1) */
	if((eResult = adi_gpio_SetPinIntEdgeSense(
			ADI_GPIO_PIN_INTERRUPT_1,
	    ADI_GPIO_PIN_9,
	    ADI_GPIO_SENSE_RISING_EDGE))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};

	/* register pinint 0 callback */
	if((eResult = adi_gpio_RegisterCallback(
			ADI_GPIO_PIN_INTERRUPT_1,
	    ADI_GPIO_PIN_9,
	    pinIntCallback,
	    (void*)0))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};;

	/* set pin interrupt mask */
	if((eResult = adi_gpio_EnablePinInterruptMask(
			ADI_GPIO_PIN_INTERRUPT_1,
	    ADI_GPIO_PIN_9,
	    true))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	};

	/*
	 * Setup Push Button 2
	 */
	/* set GPIO input */
	if((eResult = adi_gpio_SetDirection(
			ADI_GPIO_PORT_C,
	    ADI_GPIO_PIN_10,
	    ADI_GPIO_DIRECTION_INPUT))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	}


	/* set edge sense mode (PORT C is connected to Pin Interrupt 1) */
	if((eResult = adi_gpio_SetPinIntEdgeSense(
			ADI_GPIO_PIN_INTERRUPT_1,
	    ADI_GPIO_PIN_10,
	    ADI_GPIO_SENSE_RISING_EDGE))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};

	/* register pinint 0 callback */
	if((eResult = adi_gpio_RegisterCallback(
			ADI_GPIO_PIN_INTERRUPT_1,
	    ADI_GPIO_PIN_10,
	    pinIntCallback,
	    (void*)0))!=ADI_GPIO_SUCCESS)
		{
			return(FAILURE);
		};;

	/* set pin interrupt mask */
	if((eResult = adi_gpio_EnablePinInterruptMask(
			ADI_GPIO_PIN_INTERRUPT_1,
	    ADI_GPIO_PIN_10,
	    true))!=ADI_GPIO_SUCCESS)
	{
		return(FAILURE);
	};
#endif // #if defined(FINBOARD)


	return (SUCCESS);
}
