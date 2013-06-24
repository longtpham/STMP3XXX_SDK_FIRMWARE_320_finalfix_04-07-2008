#include "types.h"
#include "exec.h"
#include "display.h"
#include "messages.h"
#include "resource.h"

_reentrant void DisplayClear (void)
{
    SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);     
}


_reentrant void DisplayLowBattery (void)
{
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,(LCD_FIRST_ROW_NUM+2)*LCD_SIZE_ROW,RSRC_BROWNOUT_BITMAP);

}