////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2006
//
// File        : s6b33b0ainitstatic.c
// Description : LCD module driver
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "regsgpio.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "SysResourceApi.h"
#include "..\displaydriver_color.h"
#include "s6b33b0a.h"

_reentrant void HALDisplayZStateEnableStatic(void)
{
//    LCD_RESET_GPEN  = FALSE;
#if 0
    LCD_DC_GPEN     = FALSE;

    LCD_CS1_GPEN     = FALSE;

    LCD_RDE_GPEN    = FALSE;

    LCD_RW_GPEN     = FALSE;

    LCD_DATA_EN     = 0x00;
#endif
}

_reentrant void HALDisplayZStateDisableStatic(void)
{
//    LCD_RESET_GPEN  = TRUE;
#if 0
    LCD_DC_GPEN     = TRUE;

    LCD_CS1_GPEN     = TRUE;

    LCD_RDE_GPEN    = TRUE;

    LCD_RW_GPEN     = TRUE;

    LCD_DATA_EN     = 0xff;
#endif
}
