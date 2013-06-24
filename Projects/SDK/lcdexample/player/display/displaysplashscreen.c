/*;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : displaysplashscreen.c
;  Description : LCD based reference design
;///////////////////////////////////////////////////////////////////////////////
*/
#include "types.h"
#include "resource.h"
#include "display.h"
#include "messages.h"
#include "exec.h"
#include "menus.h"
#include "hwequ.h"

extern _asmfunc void ExecuteModules(void);

#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm    
static void EmptyFunction (void){}  // this is required to get the 1st funclet org to work 
                             		// it costs 1 word in system space
#endif    

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void  DisplaySplashScreen (void)
//
//   Type:          Function 
//
//   Description:   Updates the display
//
//   Inputs:        bDisplay--display or not
//                  iIgnored2-- ignored
//                  pPtr --currently ignored
//
//  Notes:          Some of these functions do not clear the range prior to drawing for memory
//                  savings.  See the code to verify that bDisplay is actually used.
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        global FDisplaySplashScreen
        FDisplaySplashScreen:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_DISPLAYSPLASHSCREEN

        org p,"SYSFUNCLET_DisplaySplashScreen_P":
        dc      RSRC_FUNCLET_DISPLAYSPLASHSCREEN
    #pragma endasm

_reentrant void  DisplaySplashScreen_Funclet (void)
{
#else
_reentrant void  DisplaySplashScreen (void)
{
#endif

#ifdef BACKLIGHT
		//Turn on backlight if saved as on
		if (g_iBackLightState == BACKLIGHT_ON)
		{
            SysPostMessage(2,LCD_BACKLIGHT_ON);
			SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
		}
#endif


  	SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
#ifdef CLCD
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS,SPLASH_SCREEN_Y_POS,RSRC_SPLASH_SCREEN_ALL);
#else
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS,SPLASH_SCREEN_Y_POS,RSRC_SPLASH_SCREEN);
#endif
    ExecuteModules(); 
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextdisplaysplashscreen":
    #pragma endasm
#endif
