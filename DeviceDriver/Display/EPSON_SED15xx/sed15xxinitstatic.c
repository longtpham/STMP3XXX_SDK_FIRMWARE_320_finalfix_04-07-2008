#include "types.h"
#include "regsgpio.h"
#include "messages.h"
#include "project.h"
#include "resource.h"
#include "SysResourceApi.h"
#include "..\displaydriver.h"
#include "SED15xx.h"

_reentrant void HALDisplayZStateEnableStatic(void)
{
//    LCD_RESET_GPEN  = FALSE;

    LCD_DC_GPEN     = FALSE;

    LCD_CS1_GPEN     = FALSE;

    LCD_RDE_GPEN    = FALSE;

    LCD_RW_GPEN     = FALSE;

    LCD_DATA_EN     = 0x00;

}

_reentrant void HALDisplayZStateDisableStatic(void)
{
//    LCD_RESET_GPEN  = TRUE;

    LCD_DC_GPEN     = TRUE;

    LCD_CS1_GPEN     = TRUE;

    LCD_RDE_GPEN    = TRUE;

    LCD_RW_GPEN     = TRUE;

    LCD_DATA_EN     = 0xff;

}
