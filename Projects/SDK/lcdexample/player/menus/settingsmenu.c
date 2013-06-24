////////////////////////////////////////////////////////////////////////////////
// SigmaTel Inc. Copyright 2003
// Filename:    	Settingsmenu.c
// Reviews:		DanhNguyen (06-2008)
// Description: 	Digital Audio Player Settings such as EQ mode, play mode, contrast,  
// inactivity shutdown timeout, record settings.
////////////////////////////////////////////////////////////////////////////////

#include "exec.h"                                       
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "settingsmenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "playerstatemachine.h"
#include "const.h"
#include "hwequ.h"
#include "sysmem.h"
#ifdef WOW
#include "wowmenu.h" 
#include "wow_ed.h"
#endif
#ifndef CLCD
#include "displaydrv.h"
#endif

INT g_iLanguage=LANGUAGES_FIRST;

_reentrant int EnterSubMenu(int iSelectedItem);

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant SettingsMenu(int a, int b, int *pPtr)
//
//   Type:          Function (settings menu entry point)
//
//   Description:   This function is the enrty point for the settings menu manager.
//
//   Inputs:        None used.
//
//   Outputs:       Menu to enter upon exiting settings menu
//                      if PH_MENU: return to music menu
//                      else : return to main menu selection
//
//   Notes:         
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant SettingsMenu(int a, int b, int *pPtr)
{
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;

    union DisplayHints DisplayHint;

    INT iHighLightedItem=SETTINGSMENU_FIRST;
    INT iEvent;
    INT iNextMenu = MENU_MAIN;  // default return (main menu)

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the 
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems

    struct MenuItem aItems[SETTINGSMENU_COUNT];   // m_iPage, m_iResource, m_ixPos, m_iyPos

/****
    THIS METHOD MAKES A CONST SECTION FOR EACH CONST
    struct MenuItem aItems[SETTINGSMENU_COUNT]={
        LCD_PRINT_RANGE_RSRC,           // m_iNormalDisplayMsg
        LCD_PRINT_RANGE_INV_RSRC,       // m_iSelectedDisplayMsg
        SETTINGSMENU_COUNT,             // m_items
        NULL};                          // MenuItem *m_pItems

    struct MenuItem aItems[SETTINGSMENU_COUNT]={
        {0,RSRC_STRING_EQ_MENU,0,16},   
        {0,RSRC_STRING_PLAYMODE_MENU,0,24},
        {0,RSRC_STRING_CONTRAST_MENU,0,32},
        {0,RSRC_STRING_PWRSAVINGS_MENU,0,40},
		{0,RSRC_STRING_TIME_DATE_MENU,0,48},
		{0,RSRC_STRING_EXIT_MENU,0,56},
        };
*/

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MenuListInfo.m_iItems = SETTINGSMENU_COUNT;


    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0; a < SETTINGSMENU_COUNT; a++)
    {   // m_ipage is 0 for all menu labels
        aItems[a].m_iPage = a / SETTINGSMENU_PAGE1_COUNT;
        aItems[a].m_ixPos = const_zero;
        aItems[a].m_iyPos = const_24 + (a%SETTINGSMENU_PAGE1_COUNT) * 8;
    }

    // load resource numbers
    aItems[MENU_EQ].m_iResource = RSRC_STRING_EQ_MENU;
    aItems[MENU_PLAYMODE].m_iResource = RSRC_STRING_PLAYMODE_MENU;
    aItems[MENU_CONTRAST].m_iResource = RSRC_STRING_CONTRAST_MENU;
#if (defined(BACKLIGHT))
	aItems[MENU_BACKLIGHT].m_iResource = RSRC_STRING_BACKLIGHT_MENU;
#endif
    aItems[MENU_PWRSAVINGS].m_iResource = RSRC_STRING_PWRSAVINGS_MENU;

#if (defined(WOW))
    aItems[MENU_WOW].m_iResource = RSRC_STRING_WOW_MENU;
#endif
    aItems[MENU_RECORD_SETTINGS].m_iResource =RSRC_STRING_RECORD_SETTINGS;
	aItems[MENU_SET_TIME].m_iResource = RSRC_STRING_SET_TIME_MENU;
	aItems[MENU_SET_DATE].m_iResource = RSRC_STRING_SET_DATE_MENU;
    aItems[MENU_SETTINGS_EXIT].m_iResource = RSRC_STRING_EXIT_MENU;



    MenuListInfo.m_pItems = aItems;

    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    a;b;  // avoid warnings for unused

    //we need to refresh the screen since entering the menu
    DisplayHint.I = SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone) 
    {   
        //mask out all display hints that aren't aimed toward the settings menu
        DisplayHint.I &= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        //assume every message goes to the player state machine
        bSendToPlayerStateMachine = TRUE;


        //handle the event
        switch(iEvent)
        {
        case EVENT_TIMER:
            //no other events left to process, so lets update the display
            if(DisplayHint.I)
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
#ifndef CLCD
            SysPostMessage(6,LCD_CLEAR_RANGE,0,SETTINGS_TITLE_Y_POS,LCD_X_SIZE,LCD_Y_SIZE-SETTINGS_TITLE_Y_POS);    //Stmp00012595
#endif
            SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,BITMAP_SETTINGS_TITLE_NUM,(int*)&MenuListInfo);

            DisplayHint.I = 0;//clear out the bits  
            bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
            break;

        case EVENT_MESSAGE:
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
                case PR_FF:
                case PH_FF:
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine=FALSE;
                    iHighLightedItem++;
                    if(iHighLightedItem>SETTINGSMENU_LAST)
                        iHighLightedItem = SETTINGSMENU_FIRST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_RW:
                case PH_RW:
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine=FALSE;
                    iHighLightedItem--;
                    if(iHighLightedItem<SETTINGSMENU_FIRST)
                        iHighLightedItem = SETTINGSMENU_LAST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_MENU:
                    iHighLightedItem = EnterSubMenu(iHighLightedItem);
                    DisplayHint.I |= SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    if (iHighLightedItem == MENU_SETTINGS_EXIT)
                    {   // exit from sub menu
                        bDone = TRUE;                           
                    }
                    if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC == TRUE)
                    {   // exit from sub menu
                        bDone = TRUE; 
                        iNextMenu = MENU_MUSIC;
                    }
                    break;

                case PR_STOP:
                    if((g_wDecoderSR & DECODER_STOPPED) != 0)
                        bSendToPlayerStateMachine = FALSE;
					else
	                    DisplayHint.I |= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
                
                case PH_MENU:
                    // allow user to escape to music mode
                    bDone= TRUE;
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    break;

                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // when returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (SETTINGS_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    } // end while (bDone)

    return iNextMenu;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant EnterSubMenu(int iSelectedItem)
//
//   Type:          Function 
//
//   Description:   This function calls a submenu based upon the user selection from the
//                      previous menu.
//
//   Inputs:        None
//
//   Outputs:       Selection to highlight upon exiting settings sub menu
//                      if MENU_EXIT: return to main menu selection
//                      else return to settings menu with returned menu highlighted
//
//   Notes:         Return value from sub menu must be supported menu item in settingsmenu
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int EnterSubMenu(int iSelectedItem)
{

    int _reentrant (*pMenu)(int,int,int*);

    INT iResource=RSRC_SETTINGS_MENU_CODE_BANK;
    // local because not saving place at this level, 
    // otherwise need another global var
    INT iCurrentMenu=SETTINGSMENU_FIRST;

    pMenu = NULL;  // Make sure that the last menu is cleared out, for the exit case
    switch(iSelectedItem)
    {
        case MENU_EQ:
#ifdef WOW
            if (g_bWOWEnable)
                // don't display EQ menu if WOW enabled
                break;
#endif

            iCurrentMenu  = MENU_EQ;
            iResource = RSRC_EQ_MENU_CODE_BANK;
            pMenu = EqMenu;
            break;

        case MENU_PLAYMODE:
            iCurrentMenu  = MENU_PLAYMODE;
            iResource = RSRC_PLAYMODE_MENU_CODE_BANK;
            pMenu = PlayModeMenu; //JN: func ptr corrected to PlayModeMenu instead of VoiceMenu
            break;

        case MENU_CONTRAST:
            iCurrentMenu  = MENU_CONTRAST;
            iResource = RSRC_CONTRAST_MENU_CODE_BANK;
            pMenu = ContrastMenu;
            break;
#if (defined(BACKLIGHT))
        case MENU_BACKLIGHT:
            iCurrentMenu  = MENU_BACKLIGHT;
            iResource = RSRC_BACKLIGHT_MENU_CODE_BANK;
            pMenu = BackLightMenu;
            break;
#endif
        case MENU_PWRSAVINGS:
            iCurrentMenu = MENU_PWRSAVINGS;
            iResource = RSRC_PWRSETTINGS_MENU_CODE_BANK;
            pMenu = PwrSavingsMenu;
            break;

        case MENU_RECORD_SETTINGS:
            iCurrentMenu = MENU_RECORD_SETTINGS;
            iResource = RSRC_RECORD_SETTINGS_MENU_CODE_BANK;
            pMenu = RecordSettingsMenu;
            break;
		
		case MENU_SET_TIME:
			iCurrentMenu = MENU_SET_TIME;
            iResource = RSRC_SETTIME_MENU_CODE_BANK;
            pMenu = SetTimeMenu;
			break;

		case MENU_SET_DATE:
			iCurrentMenu = MENU_SET_DATE;
            iResource = RSRC_SETDATE_MENU_CODE_BANK;
            pMenu = SetDateMenu;
			break;

#ifdef	WOW
        case MENU_WOW:
            iCurrentMenu = MENU_WOW;
            iResource = RSRC_WOW_MENU_CODE_BANK;
            pMenu = WowMenu;
            break;
#endif

				
        default:
        case MENU_SETTINGS_EXIT:
            // pMenu = NULL so will not call a menu
            iCurrentMenu = MENU_SETTINGS_EXIT;  // break out, return to main menu using init value
            break;
    }

    if(pMenu)   //only enter a new menu if pMenu is not NULL.
    {
        iCurrentMenu = SysCallFunction(iResource,pMenu,0,0,0);
    }

    return iCurrentMenu;
}

