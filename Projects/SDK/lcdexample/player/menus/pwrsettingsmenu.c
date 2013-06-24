#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "mainmenu.h"
#include "settingsmenu.h"
#include "display.h"
#include "displaylists.h"
#include "sysmem.h"
#include "playerstatemachine.h"
#include "recorderstatemachine.h"
#include "const.h"
#include "pwrsettingsmenu.h"
#include "hwequ.h"


int _reentrant SetPwrSetting (int iCurrentPwrSetting, int b, int *pPtr);
_reentrant void EnablePwrSavings (void);


WORD g_wPowerOffTrigger=0x7fffff;
// this gets overwritten when get saved settings
WORD g_wPwrSettings = PWR_DISABLE;

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant PwrSavingsMenu(int a, int b, int *pPtr)
//
//   Type:          Function (Auto Shutdown menu entry point)
//
//   Description:   This function is the entry point for the auto shutdown menu manager.
//
//   Inputs:        None
//
//   Outputs:       Menu to enter upon exiting settings menu
//						if PH_MENU: return to music menu
//                      else : return to settings menu selection
//
//   Notes:         
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant PwrSavingsMenu(int a, int b, int *pPtr)
{
    int _reentrant (*pMenu)(int,int,int*);

    BOOL bDone = FALSE;

    BOOL bSendToPlayerStateMachine;
	union DisplayHints DisplayHint;

    INT iHighLightedItem=g_wPwrSettings;
    INT iEvent;

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the 
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////
	struct PagedListStructure MenuListInfo;	// m_iNormalDisplayMsg,m_iSelectedDisplayMsg
											// m_iItems, MenuItem *m_pItems

	// the array order must match the order of items defined in pwrmenu.h
	struct MenuItem aItems[PWRMENU_COUNT];	// m_iPage, m_iResource, m_ixPos, m_iyPos

/****
	THIS METHOD MAKES A CONST SECTION FOR EACH CONST
	struct PagedListStructure MenuListInfo = {
		LCD_PRINT_RANGE_RSRC,			// m_iNormalDisplayMsg
		LCD_PRINT_RANGE_INV_RSRC,		// m_iSelectedDisplayMsg
	    PWRMENU_COUNT,			        // m_items
	    NULL};							// MenuItem *m_pItems

	struct MenuItem aItems[PWRMENU_COUNT]={
	    {0,RSRC_STRING_DISABLE_MENU,0,16},		    
	    {0,RSRC_STRING_1MIN_MENU,0,24},
	    {0,RSRC_STRING_2MIN_MENU,0,32},
	    {0,RSRC_STRING_5MIN_MENU,0,40},
	    {0,RSRC_STRING_10MIN_MENU,0,48},
	    };
*/

	// filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
	MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
	MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MenuListInfo.m_iItems = PWRMENU_COUNT;


	// filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
	for (a = 0; a < PWRMENU_COUNT; a++)
	{	// m_ipage is 0 for all menu labels
		aItems[a].m_iPage = const_zero;
		aItems[a].m_ixPos = const_zero;
		aItems[a].m_iyPos = const_16 + a * 8;
	}

	// load resource numbers
	aItems[PWR_DISABLE].m_iResource = RSRC_STRING_DISABLE_MENU;
	aItems[PWR_1MIN].m_iResource = RSRC_STRING_1MIN_MENU;
	aItems[PWR_2MIN].m_iResource = RSRC_STRING_2MIN_MENU;
	aItems[PWR_5MIN].m_iResource = RSRC_STRING_5MIN_MENU;
	aItems[PWR_10MIN].m_iResource = RSRC_STRING_10MIN_MENU;


    MenuListInfo.m_pItems = aItems;

	a;b;pPtr;// avoid warnings for unused



    //we need to refresh the screen since entering the menu
    DisplayHint.I = PWR_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone) 
    {   
        DisplayHint.I &= (PWR_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
        
        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);

        bSendToPlayerStateMachine = TRUE;
        //handle that EVENT_MESSAGE or EVENT_BUTTON
        switch(iEvent)
        {
        case EVENT_TIMER:
            if(DisplayHint.I)
            {
	            SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
	            SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,0,(int*)&MenuListInfo);
	            DisplayHint.I = 0;
            }
            bSendToPlayerStateMachine = FALSE;
            break;
        case EVENT_BUTTON:
	        // button pressed, so restart timer if enabled
    	    UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
			// turn on backlight when button is pressed.
			if (g_iBackLightState==BACKLIGHT_ON)
			{
    	    	SysPostMessage(2,LCD_BACKLIGHT_ON);
        	    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
        	    SysWaitOnEvent(0,0,0);
 			} 
#endif
            switch(gEventInfo.Button.wButtonEvent)
            {
                //remember, the rest of the buttons and events are handled in the PlayerStateHandler below!
                case PR_FF:
                case PH_FF:
                    DisplayHint.bits.Misc = TRUE;
                    bSendToPlayerStateMachine = FALSE;
                    iHighLightedItem++;
					//wrap around
                    if(iHighLightedItem>LAST_PWR)
                        iHighLightedItem = FIRST_PWR;
                    break;

                case PR_RW:
                case PH_RW:
                    DisplayHint.bits.Misc = TRUE;
                    bSendToPlayerStateMachine = FALSE;
                    iHighLightedItem--;
                    //wrap around
                    if(iHighLightedItem<FIRST_PWR)
                        iHighLightedItem = LAST_PWR;
                    break;
				case PH_MENU:
					// set flag so PR_MENU in music mode will ignore
					g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                case PR_MENU:
                    bSendToPlayerStateMachine = FALSE;
					// allow user to escape to music mode
					bDone= TRUE;
					break;

                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                	SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
	                // when returns update display  -- if doesn't return, powered down
    				DisplayHint.I |= (PWR_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
        {//only send events to the player state machine if necessary (to avoid code bank swapping)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,(void*)&gEventInfo);
            //or the display hints returned with the current collection of hints
        }
    } // end while (bDone)

	// setup auto power down with selected value
	SetPwrSetting(iHighLightedItem,0,0);
	g_wPwrSettings = iHighLightedItem;

	return MENU_PWRSAVINGS;
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant SetPwrSetting (int iCurrentPwrSetting)
//
//   Type:          Function (code bank callable for initial setup) 
//
//   Description:   This sets up the autoshutdown timeout period and starts the 
//                     timer as required
//
//   Inputs:        Selected shutdown setting
//
//   Outputs:       Setup autoshutdown for selected amount or disable
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant SetPwrSetting (int iCurrentPwrSetting, int b, int *pPtr)
{
	b;pPtr;		// avoid warnings

    switch(iCurrentPwrSetting)
    {
        default:
        case PWR_DISABLE:
            // disable power savings
            g_wPowerOffTrigger = 0x7fffff;
            g_MenuFlags.MENU_FLAG_POWER_DOWN_ENABLED = FALSE;
            SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
            break;

        case PWR_1MIN:
            g_wPowerOffTrigger = ONEMIN;
            EnablePwrSavings();
            break;

        case PWR_2MIN:
            g_wPowerOffTrigger = TWOMIN;
            EnablePwrSavings();
            break;

        case PWR_5MIN:
            g_wPowerOffTrigger = FIVEMIN;
            EnablePwrSavings();
            break;

        case PWR_10MIN:
            g_wPowerOffTrigger = TENMIN;
            EnablePwrSavings();
            break;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void EnablePwrSavings (void)
//
//   Type:          Function 
//
//   Description:   Function sets pwr down enabled flag and updates the softtimer if 
//                  decoder and encoder status registers both indicate stopped
//
//   Inputs:        none (all parameters are ignored)
//
//   Outputs:       Sets global shutdown enable flag and sends timer set or timer kill message.
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void EnablePwrSavings (void)
{

    g_MenuFlags.MENU_FLAG_POWER_DOWN_ENABLED = TRUE;

    if( (g_wDecoderSR & DECODER_STOPPED) && (g_wEncoderSR & ENCODER_STOPPED) )
    {
        SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_AUTO_SHUTDOWN,
            1,g_wPowerOffTrigger,MENU_AUTO_SHUTDOWN);
    }
    else
    {   // Enc and Dec are not 'both STOPPED', so kill the auto shutdown timer
        SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
    }

} 



#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_wPwrSettings

    SaveRange Fg_wPwrSettings,1
    
#pragma endasm
