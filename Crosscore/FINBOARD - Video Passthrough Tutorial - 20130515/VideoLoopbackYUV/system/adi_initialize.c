/*
** adi_initialize.c source file generated on November 2, 2012 at 14:28:53.
**
** Copyright (C) 2000-2012 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically. You should not modify this source file,
** as your changes will be lost if this source file is re-generated.
*/

#include "adi_initialize.h"

extern int32_t adi_initpinmux(void);

int32_t adi_initComponents(void)
{
	int32_t result = 0;

#ifdef __HAS_SEC__
	result = adi_sec_Init();
#endif

	if (result == 0) {
		result = adi_initpinmux(); /* auto-generated code (order:0) */
	}

	return result;
}

