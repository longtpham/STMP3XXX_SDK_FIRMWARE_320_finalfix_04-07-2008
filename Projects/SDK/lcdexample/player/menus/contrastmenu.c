#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "display.h"
#include "mainmenu.h"
#include "settingsmenu.h"
#include "hwequ.h"

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant ContrastMenu(int a, int b, int *pPtr)
//
//   Type:          Function (code bank callable)
//
//   Description:   This function is the cotrast menu
//
//   Inputs:        none used.
//
//   Outputs:       messages
//
//   Notes:         Will stay here until menu button is pressed
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant ContrastMenu(int a, int b, int *pPtr)
{
	union DisplayHints DisplayHint;
	INT  iEvent;
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine;

    // this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

    a;b;//here to prevent parameter not used messages.

    //Lets initially clear screen and display all
    DisplayHint.I = CONTRAST_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone)
	{
        
        DisplayHint.I &= (CONTRAST_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
        //wait on either a EVENT_BUTTON or EVENT_MESSAGE
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);        

        //This block is for handling the player state machine related messages.  
        bSendToPlayerStateMachine=TRUE;
		switch(iEvent)
		{
			case EVENT_TIMER:
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayContrast,TRUE, 0,0);

                //after displaying, clear all display hints
                DisplayHint.I=0;
                bSendToPlayerStateMachine=FALSE;
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
	            //remember the last button event (mostly for coming out of FFWD and RWND)		
	            switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
	            {
		            case PH_FF:
		            case PR_FF:
                        bSendToPlayerStateMachine=FALSE;
		                SysPostMessage(2,LCD_INC_CONTRAST);
						DisplayHint.I |= CONTRAST_MENU_DISPLAY_ALL;
		                break;
		            case PH_RW:
		            case PR_RW:
                        bSendToPlayerStateMachine=FALSE;
		                SysPostMessage(2,LCD_DEC_CONTRAST);
						DisplayHint.I |= CONTRAST_MENU_DISPLAY_ALL;
		                break;

                    case PH_STOP:
		                //if the STOP button is held down, lets call the shutdown menu
		                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
		                // if returns update display  -- if doesn't return, powered down
		                DisplayHint.I |= (MUSIC_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS)); 
                        bSendToPlayerStateMachine=FALSE;
		                break;
					case PH_MENU:
						// allow user to escape to music mode
						// set flag so PR_MENU in music mode will ignore
						g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
		            case PR_MENU:
		                //defaults to main menu when exiting.  No need to set a new menu.
		                bDone = TRUE;
                        bSendToPlayerStateMachine=FALSE;
						break;
	            }// end switch (button)
	            break;
		}	// end switch (iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

 	} // end while (!bDone)
 	 
    return MENU_CONTRAST;
}
