////////////////////////////////////////////////////////////////////////////////
// Copyright (c) SigmaTel, Inc. 2003
// Filename: playmodemenu.c
//
////////////////////////////////////////////////////////////////////////////////


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
#include "playmodemenu.h"
#include "playerstatemachine.h"
#include "const.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include "musiclib_ghdr.h"
#include  "browsemenu.h"
#else
#ifdef   USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif

#ifndef CLCD
#include "displaydrv.h"
#endif

#include "hwequ.h"

int  _reentrant SendPlayMode(int iCurrentPlayMode, int b, int *pPtr);
#ifdef USE_PLAYLIST3
int g_shuffle_start;
INT g_iShuffleSetting = SHUFFLE_OFF;
INT g_iRepeatSetting = REPEAT_OFF;
#endif	// #ifdef USE_PLAYLIST3

INT g_iPlayModeSetting=FIRST_PM;

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant PlayModeMenu(int a, int b, int *pPtr)
//
//   Type:          Function (PlayModeMenu menu entry point)
//
//   Description:   This function is the entry point for the PlayModeMenu menu manager.
//
//   Inputs:        None
//
//   Outputs:       Menu to enter upon exiting settings menu
//                      if PH_MENU: return to music menu
//                      else : return to Settings menu
//
//   Notes:
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant PlayModeMenu(int a, int b, int *pPtr)
{

    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;

    union DisplayHints DisplayHint;

    INT iHighLightedItem=g_iPlayModeSetting;
    INT iEvent;

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems

    struct MenuItem aItems[PMMENU_COUNT];

/****
    THIS METHOD MAKES A CONST SECTION in Y FOR EACH CONST
    struct PagedListStructure MenuListInfo = {
        LCD_PRINT_RANGE_RSRC,           // m_iNormalDisplayMsg
        LCD_PRINT_RANGE_INV_RSRC,       // m_iSelectedDisplayMsg
        PMMENU_COUNT,                   // m_items
        NULL};                          // MenuItem *m_pItems

    // these must match the order of items defined in PlayModeMenu.h
    struct MenuItem aItems[PMMENU_COUNT]={
        {0,RSRC_STRING_NORMAL_MENU,0,16},   // m_iPage, m_iResource, m_ixPos, m_iyPos
        {0,RSRC_STRING_REPEAT1_MENU,0,24},
        {0,RSRC_STRING_REPEATALL_MENU,0,32},
        {0,RSRC_STRING_SHUFFLE_MENU,0,40},
        {0,RSRC_STRING_SHUFFLEREPEAT_MENU,0,48},
        };
*/
    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MenuListInfo.m_iItems = PMMENU_COUNT;

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0; a < PMMENU_COUNT; a++)
    {   // m_ipage is 0 for all menu labels
        aItems[a].m_iPage = a / PMMENU_PAGE1_COUNT;
        aItems[a].m_ixPos = const_zero;
        aItems[a].m_iyPos = const_16 + (a%PMMENU_PAGE1_COUNT) * 8;
    }

    // load resource numbers
    aItems[PM_NORMAL].m_iResource = RSRC_STRING_NORMAL_MENU;
    aItems[PM_REPEAT1].m_iResource = RSRC_STRING_REPEAT1_MENU;
    aItems[PM_REPEATALL].m_iResource = RSRC_STRING_REPEATALL_MENU;
    aItems[PM_SHUFFLE].m_iResource = RSRC_STRING_SHUFFLE_MENU;
    aItems[PM_SHUFFLE_REPEAT].m_iResource = RSRC_STRING_SHUFFLEREPEAT_MENU;

    MenuListInfo.m_pItems = aItems;

    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    a;b;  // avoid warnings for unused

    //we need to refresh the screen since entering the menu
    DisplayHint.I = PM_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone)
    {
        iHighLightedItem=g_iPlayModeSetting;
        //wait until a message or button happens.
        DisplayHint.I &= (PM_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);

        //handle that EVENT_MESSAGE or EVENT_BUTTON
        bSendToPlayerStateMachine = TRUE;
        switch(iEvent)
        {
        case EVENT_TIMER:
            SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
#ifndef CLCD
            SysPostMessage(6,LCD_CLEAR_RANGE,0,8,LCD_X_SIZE,LCD_Y_SIZE-8);  //Stmp00012595
#endif
            SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,0,(int*)&MenuListInfo);
            DisplayHint.I = 0;
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
                case PR_FF:
                case PH_FF:
                    bSendToPlayerStateMachine = FALSE;
                    NextPlayMode(INCREMENT,0,0);
                    DisplayHint.bits.PlayMode = TRUE;
                    DisplayHint.bits.Misc= TRUE;
                    break;

                case PR_RW:
                case PH_RW:
                    bSendToPlayerStateMachine = FALSE;
                    NextPlayMode(DECREMENT,0,0);
                    DisplayHint.bits.PlayMode = TRUE;
                    DisplayHint.bits.Misc= TRUE;
                    break;

                case PH_MENU:
                    // allow user to escape to music mode
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                case PR_MENU:
                    bDone= TRUE;
                    bSendToPlayerStateMachine = FALSE;
                    break;

                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    bSendToPlayerStateMachine = FALSE;
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // when returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (PM_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    } // end while (bDone)

    return MENU_PLAYMODE;
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant SendPlayMode(int iCurrentPlayMode, int b, int *pPtr)
//
//   Type:          Function (codebank callable -- so can initialize to saved setting
//
//   Description:   This sends the PlayMode settings to the decoder.
//
//   Inputs:        PlayMode setting to send.
//
//   Outputs:       Messages posted to the appropriate place. Returns 0.
//
//   Notes:         This routine can be simplified if not using a menu
//                     to switch on current decoder setting and setting up
//                     messages to be the next decoder setting
//<
//////////////////////////////////////////////////////////////////////////////////////////
int  _reentrant SendPlayMode(int iCurrentPlayMode, int b, int *pPtr)
{
#ifndef USE_PLAYLIST3
    b;pPtr;


        // Valid options during music or voice playset:
        // Playset types may have different play modes. Differences handled below.
        switch(iCurrentPlayMode)
        {
            case PM_REPEATALL:
                g_iPlaylistRepeat = PLAYLIST_REPEAT_ALL;
                SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_ShuffleList,FALSE,0,0);
                break;
            case PM_REPEAT1:
                g_iPlaylistRepeat = PLAYLIST_REPEAT_ONE;
                SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_ShuffleList,FALSE,0,0);
                break;
            case PM_SHUFFLE:
                if (Playlist_GetPlaySet() == PLAYSET_MUSIC) // no shuffle in voice mode
                {   g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;
                    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_ShuffleList,TRUE,0,0);
                }
                break;
            case PM_SHUFFLE_REPEAT:
                if (Playlist_GetPlaySet() == PLAYSET_MUSIC) // no shuffle repeat in voice mode
                {   g_iPlaylistRepeat = PLAYLIST_REPEAT_ALL;
                    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_ShuffleList,TRUE,0,0);
                }
                break;

            case PM_NORMAL:
            default://nothing, so lets go to normal.
                g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;
                SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_ShuffleList,FALSE,0,0);
                break;
        }
#else	// #ifndef USE_PLAYLIST3
	INT original_shuffle = g_iShuffleSetting;
	b;pPtr;

	switch(iCurrentPlayMode)
	{
	case PM_REPEATALL:
		g_iRepeatSetting = REPEAT_ALL;
		g_iShuffleSetting = SHUFFLE_OFF;
		break;
	case PM_REPEAT1:
		g_iRepeatSetting = REPEAT_ONE;
		g_iShuffleSetting = SHUFFLE_OFF;
		break;
	case PM_SHUFFLE:
		g_iRepeatSetting = REPEAT_OFF;
		g_iShuffleSetting = SHUFFLE_ON;
		break;
	case PM_SHUFFLE_REPEAT:
		g_iRepeatSetting = REPEAT_ALL;
		g_iShuffleSetting = SHUFFLE_ON;
		break;

	case PM_NORMAL:
	default://nothing, so lets go to normal.
		g_iRepeatSetting = REPEAT_OFF;
		g_iShuffleSetting = SHUFFLE_OFF;
		break;
	}

	if (original_shuffle != g_iShuffleSetting)
	{
		SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);

		if ((g_iShuffleSetting == SHUFFLE_OFF) || (bPlayVoice == TRUE))
			SysPostMessage(3, MUSICLIB_PLAY_SETSHUFFLE, PLAY_NORMAL);
		else
		{
			BOOL bLastShuffle_YesOrNot = 0;
			if((playMode & 1<<PLAYMODE_LAST_PLAYMODE_BITPOS) != 0)  bLastShuffle_YesOrNot = 1;
			SysPostMessage(3, MUSICLIB_PLAY_SETSHUFFLE, PLAY_SHUFFLE);
			SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_NEXT_SONG,&g_shuffle_start);
			SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_PREVIOUS_SONG,&g_shuffle_start);
			if(bLastShuffle_YesOrNot == 0) playMode &= ~(1<<PLAYMODE_LAST_PLAYMODE_BITPOS);
		}
		SysWaitOnEvent(0,0,0);
		// end
	}
#endif	// #ifndef USE_PLAYLIST3

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant NextPlayMode(int iDirection, int b, int *c)
//
//   Type:          Function (code bank callable)
//
//   Description:   This updates the playmode setting and rolls it over if necessary
//
//   Inputs:        none (all parameters are ignored)
//
//   Outputs:       Calls SendPlayMode, then exits
//
//   Notes:
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant NextPlayMode(int iDirection, int b, int *c)
{
    int iLastPM;
#ifndef USE_PLAYLIST3
    int iCurrentPlaySet = Playlist_GetPlaySet();
    b;c;                        //prevent 'parameter not used'

    if (iCurrentPlaySet == PLAYSET_VOICE)
    {   iLastPM = LAST_VOICE_PM; // voice playset uses less playmodes than music.
    }
    else // PLAYSET_MUSIC
    {   iLastPM = LAST_PM;
    }
#else
	iLastPM = LAST_PM;
#endif

    if (iDirection == INCREMENT)
    {
        g_iPlayModeSetting += 1;
        if( g_iPlayModeSetting > iLastPM )    //check for roll over
            g_iPlayModeSetting = FIRST_PM;
#ifdef USE_PLAYLIST5
        if (g_iPlayModeSetting == PM_SHUFFLE)
            g_iPlayModeSetting = PM_SHUFFLE_REPEAT;
#endif
    }
    else
    {
        g_iPlayModeSetting -= 1;
        if(g_iPlayModeSetting < FIRST_PM)   //check for roll over
            g_iPlayModeSetting = iLastPM;
#ifdef USE_PLAYLIST5
        if (g_iPlayModeSetting == PM_SHUFFLE)
            g_iPlayModeSetting = PM_REPEATALL;
#endif
    }
    SendPlayMode(g_iPlayModeSetting,0,0);
    return g_iPlayModeSetting;
}



#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_iPlayModeSetting
	if (@def('USE_PLAYLIST3'))
    extern y:Fg_iShuffleSetting
    extern y:Fg_iRepeatSetting
	endif

    SaveRange Fg_iPlayModeSetting,1
	if (@def('USE_PLAYLIST3'))
    SaveRange Fg_iShuffleSetting,1
    SaveRange Fg_iRepeatSetting,1
	endif

#pragma endasm
