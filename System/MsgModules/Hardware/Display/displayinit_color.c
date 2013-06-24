////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : displayinit_color.c
// Description : LCD display driver
////////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "displaydriver.h"
#include "displaydrv.h"
#include "sal\display_sal.h"
#include "resource.h"

void _reentrant DisplayInit(void)
{
    SALDisplayInit();
    HALDisplayInit();
    DisplaySetFont(0);
}
