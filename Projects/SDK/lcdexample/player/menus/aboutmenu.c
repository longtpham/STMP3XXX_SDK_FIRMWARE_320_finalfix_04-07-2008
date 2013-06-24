#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "display.h"
#include "mainmenu.h"
#include "project.h"
#include "sysmem.h"
#include "playerstatemachine.h"
#include "hwequ.h"
#include "filesystem.h"
#include "fsproj.h"
#include "musiclib_ghdr.h"

#define ABOUT_SCREEN_NUMBER_FIRST	0
#define ABOUT_SCREEN_NUMBER_LAST		3
extern _reentrant INT FileSystemPresent(INT Device);
_reentrant void DisplayAboutMenu(INT iScreenNumber);
extern long FSFreeSpace(int);

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant AboutMenu(int a, int b, int *pPtr)
//
//   Type:          Function (code bank callable)
//
//   Description:   This function is the about menu
//
//   Inputs:        none used.
//
//   Outputs:       messages
//
//   Notes:         Will stay here until menu button is pressed
//<
////////////////////////////////////////////////////////////////////////////////
int _reentrant AboutMenu(int a, int b, int *pPtr)
{
	union DisplayHints DisplayHint;

	INT  iEvent;
    BOOL bDone = FALSE;
    INT  iNextMenu = MENU_MAIN;
    BOOL bSendToPlayerStateMachine = FALSE;
	INT iAboutScreenNum=0;

    // this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

    a;b;//here to prevent parameter not used messages.

    //Lets initially clear screen and display all
    DisplayHint.I = ABOUT_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);


#if defined(MMC)  
    // Stop the decoder if needed. STMP Issue 2375 ------- 
    // Needed here to display stop icon.
    if(!(g_wDecoderSR&DECODER_STOPPED))
    {   
        SysPostMessage(2,DECODER_STOP);
        while(!(g_wDecoderSR&DECODER_STOPPED))
        {   SysWaitOnEvent(0,0,2);
        }
    }     
    DisplayHint.bits.PlayState = TRUE;  // Will update play state icon to stopped
#endif      //-------- STMP 2375 


    while(!bDone)
	{
        DisplayHint.bits.ClearDisplay=TRUE;
        
        //wait on either a EVENT_BUTTON or EVENT_MESSAGE
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);        

        bSendToPlayerStateMachine=TRUE;
		switch(iEvent)
		{
			case EVENT_TIMER:
                	SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
			if (DisplayHint.bits.ClearDisplay)
			    	{  // if cleared display, redisplay info
			       DisplayAboutMenu(iAboutScreenNum);
			    	}
                	//after displaying, clear all display hints
                	DisplayHint.I=0;
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

	            	//somebody pressed a button.
	            	//remember the last button event (mostly for coming out of FFWD and RWND)		
	            	switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
	            	{
	            	case PH_FF:
			case PR_FF:
				iAboutScreenNum++;
				if(iAboutScreenNum>ABOUT_SCREEN_NUMBER_LAST)
					iAboutScreenNum=ABOUT_SCREEN_NUMBER_FIRST;
				DisplayAboutMenu(iAboutScreenNum);
				bSendToPlayerStateMachine=FALSE;
				break;

			case PH_RW:
			case PR_RW:
				iAboutScreenNum--;
				if(iAboutScreenNum<ABOUT_SCREEN_NUMBER_FIRST)
					iAboutScreenNum=ABOUT_SCREEN_NUMBER_LAST;
				DisplayAboutMenu(iAboutScreenNum);
				bSendToPlayerStateMachine=FALSE;
				break;
					
		        case PH_STOP:
		             	//if the STOP button is held down, lets call the shutdown menu
		             	SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
		             	// if returns update display  -- if doesn't return, powered down
		             	DisplayHint.I |= (ABOUT_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
	               	bSendToPlayerStateMachine = FALSE;
		             	break;

			case PH_MENU:
				// allow user to escape to music mode
				// set flag so PR_MENU in music mode will ignore
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
		        case PR_MENU:
		             	//defaults to main menu when exiting.  No need to set a new menu.
		        default:
				bDone= TRUE;
	               	bSendToPlayerStateMachine = FALSE;
				break;
	            }// end switch (button)
	            break;
		}	// end switch (iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

 	} // end while (!bDone)
 	 
    return iNextMenu;
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void DisplayAboutMenu(void)
//
//   Type:          Function 
//
//   Description:   This function displays the about menu text.
//						System Icons are still displayed via RefreshDisplay
//
//   Inputs:        none used.
//
//   Outputs:       lcd messages  
//
//   Notes:         This call will post 65 words to LCD message queue
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void DisplayAboutMenu(INT iScreenNumber)
{
	int iMedia;			// media logical/local device number 
	int iSize;			// size of media 
	int iIntMem = 0;	// sum of internal media devices
	int iIntMemFree = 0;
	int iExtMem = 0;	// sum of external media devices
	int iExtMemFree = 0;
	long temp;
/*
#if defined(MMC) 
    // Stop the decoder if needed. STMP Issue 2375 ------- 
    // Prevents system hang/decoder crash due to media driver removal/overlay. 
    if(!(g_wDecoderSR&DECODER_STOPPED))
    {   
        SysPostMessage(2,DECODER_STOP);
        while(!(g_wDecoderSR&DECODER_STOPPED))
        {   SysWaitOnEvent(0,0,2);
        }
    }                                             
#endif
*/
	SysLoadFATWrite();

    	iIntMem = (int)FSSize(0, MEDIA_SIZE_IN_MEGABYTES);
	temp = FSFreeSpace(0)>>20;
	iIntMemFree = (int)temp;

	iExtMem = 0;

#if defined(MMC)
	// now do MMC devices 
	if(!FileSystemPresent((BYTE)1))
	{
		// FSSize returns capacity in Mb
		iExtMem = (int)FSSize(1, MEDIA_SIZE_IN_MEGABYTES);
		temp = FSFreeSpace(1)>>20;
		iIntMemFree = (int)temp;
	}
#endif // MMC

	SysUnLoadFATWrite();

	
	switch(iScreenNumber)
	{	
	case 0:
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_ABOUT_SCREEN_1+g_iLanguage);
		break;
	case 1:
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_ABOUT_SCREEN_2+g_iLanguage);
		SysPostMessage(7,LCD_PRINT_NUMBER,45,44-7,iIntMem,6,SPACE_CHAR);
		SysPostMessage(7,LCD_PRINT_NUMBER,45,59-7,iIntMemFree,6,SPACE_CHAR);
		break;
	case 2:
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_ABOUT_SCREEN_3+g_iLanguage);
		SysPostMessage(7,LCD_PRINT_NUMBER,45,44-7,iExtMem,6,SPACE_CHAR);
		SysPostMessage(7,LCD_PRINT_NUMBER,45,59-7,iExtMemFree,6,SPACE_CHAR);
		break;
	case 3:
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_ABOUT_SCREEN_4+g_iLanguage);
		break;
	default:
		break;
	}
	
#ifdef CLCD
	{
		INT iAttribute;
		WORD wLogoFile[4];

		wLogoFile[0] = 'a' | (':' << 8) | ('/' << 16);
		wLogoFile[1] = 'l' | ('o' << 8) | ('g' << 16);
		wLogoFile[2] = 'o' | ('.' << 8) | ('b' << 16);
		wLogoFile[3] = 'm' | ('p' << 8);
		
		iAttribute = filegetattrib( (_packed char*)wLogoFile );
		if( iAttribute > 0 )
		{
			SysPostMessage(6,LCD_PRINT_RANGE_FILE,SPLASH_SCREEN_X_POS,50,wLogoFile,0);
			SysWaitOnEvent(0,0,0);	//allow the display Module time to clear its Queue
		}
		else
		{
			//Print the SigmaTel Logo
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS,50,RSRC_SPLASH_SCREEN_1);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+10,50,RSRC_SPLASH_SCREEN_2);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+20,50,RSRC_SPLASH_SCREEN_3);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+30,50,RSRC_SPLASH_SCREEN_4);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+40,50,RSRC_SPLASH_SCREEN_5);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+50,50,RSRC_SPLASH_SCREEN_6);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+60,50,RSRC_SPLASH_SCREEN_7);
			SysWaitOnEvent(0,0,0);	//allow the display Module time to clear its Queue
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+70,50,RSRC_SPLASH_SCREEN_8);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+80,50,RSRC_SPLASH_SCREEN_9);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+90,50,RSRC_SPLASH_SCREEN_10);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+100,50,RSRC_SPLASH_SCREEN_11);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+110,50,RSRC_SPLASH_SCREEN_12);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SPLASH_SCREEN_X_POS+120,50,RSRC_SPLASH_SCREEN_13);
		}
	}
#endif

}


