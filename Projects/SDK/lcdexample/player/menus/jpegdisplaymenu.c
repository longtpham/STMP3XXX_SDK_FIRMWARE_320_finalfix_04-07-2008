////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2007
//
// File        : jpegdisplaymenu.c
// Description : Jpeg application top menu
////////////////////////////////////////////////////////////////////////////////

#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "jpegdisplaymenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "playerlib.h"
#include "playerstatemachine.h"
#include "sysmem.h"
#include "syscoder.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include "playlist3.h"
#include "jpegfileutil.h"
#else
#ifdef   USE_PLAYLIST5
#include "playlist5.h"
#endif
#endif
#endif
#endif
#include "const.h"
#include "hwequ.h"

extern int DecoderResourcePtr;

_reentrant int EnterJpegSubMenu(int iSelectedItem);
extern _asmfunc void DecoderForceInit(void);

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant jpegdisplay(int a, int b, int *pPtr)
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
int _reentrant JpegDisplayMenu(int a, int b, int *pPtr)
{
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;

    union DisplayHints DisplayHint;

    INT iHighLightedItem=JPEGDISPLAYMENU_FIRST;
    INT iEvent;
    INT iNextMenu = MENU_MAIN;  // default return (main menu)

///////////////////////////////////////////////////////////////////////////
//These data structures are used by the DisplayPagedList function to create the
//the main menu lists.  see displaylists.h for more info
///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems

    struct MenuItem aItems[JPEGDISPLAYMENU_COUNT];   // m_iPage, m_iResource, m_ixPos, m_iyPos

/****
    THIS METHOD MAKES A CONST SECTION FOR EACH CONST
    struct MenuItem aItems[JPEGDISPLAYMENU_COUNT]={
        LCD_PRINT_RANGE_RSRC,           // m_iNormalDisplayMsg
        LCD_PRINT_RANGE_INV_RSRC,       // m_iSelectedDisplayMsg
        JPEGDISPLAYMENU_COUNT,             // m_items
        NULL};                          // MenuItem *m_pItems

    struct MenuItem aItems[JPEGDISPLAYMENU_COUNT]={
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
    MenuListInfo.m_iItems = JPEGDISPLAYMENU_COUNT;


    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0; a < JPEGDISPLAYMENU_COUNT; a++)
    {   // m_ipage is 0 for all menu labels
        aItems[a].m_iPage = const_zero;
    }

    // load resource numbers
    aItems[MENU_MANUAL_MODE].m_iResource = RSRC_STRING_JPEG_MANUAL_MENU;
    aItems[MENU_SLIDESHOW_MODE].m_iResource = RSRC_STRING_JPEG_SLIDESHOW_MENU;
    aItems[MENU_THUMBNAIL_MODE].m_iResource = RSRC_STRING_JPEG_THUMBNAIL_MENU;
	aItems[MENU_JPEG_DISPLAY_EXIT].m_iResource = RSRC_STRING_EXIT_MENU;

    for (a = 0; a < JPEGDISPLAYMENU_COUNT; a++)
    {   // m_ixPos is 0 for all menu labels -- start at same X loc
        aItems[a].m_ixPos = const_zero;
    }
    // load Y position  m_iyPos -- each line is 8 pixels lower
    aItems[MENU_MANUAL_MODE].m_iyPos =        24;
    aItems[MENU_SLIDESHOW_MODE].m_iyPos =     32;
    aItems[MENU_THUMBNAIL_MODE].m_iyPos =     40;
	aItems[MENU_JPEG_DISPLAY_EXIT].m_iyPos =  48;


    MenuListInfo.m_pItems = aItems;

    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    a;b;  // avoid warnings for unused

    //we need to refresh the screen since entering the menu
    DisplayHint.I = JPEG_DISPLAY_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

	if(!(g_wDecoderSR&DECODER_STOPPED))
	{
		SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
#ifdef USE_PLAYLIST3
	    SysWaitOnEvent(0,0,0);
#endif
		if( DecoderResourcePtr == RSRC_DEC_SMVADPCM_MOD_CODE )
		{   SysSetDecoderResource(RSRC_DECMOD_CODE);
			DecoderForceInit();
			SysPostMessage(2, DECODER_RESET);
		}
	}
    g_iPlayerState = DECODER_STOPPED;

#ifdef USE_PLAYLIST5
    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
	SysWaitOnEvent(0,0,10);
#endif

	// Flush the message in queue
	do	{
    	iEvent = SysWaitOnEvent(EVENT_MESSAGE,&gEventInfo.msg, 500);
    }while(iEvent != EVENT_TIMER);

#ifdef USE_PLAYLIST5
    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
	SysWaitOnEvent(0,0,10);
#endif


#ifdef USE_PLAYLIST3
    SysCallFunction(RSRC_JPEG_FILEUTIL_EXTRA_MENU_CODE_BANK,JpegFile_SetPlaySet,PLAYSET_PHOTO,TRUE,0);
#else
    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_SetPlaySet,PLAYSET_PHOTO,TRUE,0);
#endif

    while(!bDone)
    {
        //mask out all display hints that aren't aimed toward the settings menu
        DisplayHint.I &= (JPEG_DISPLAY_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

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
            if(DisplayHint.bits.Misc)
                SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,BITMAP_JPEG_DISPLAY_TITLE_NUM,(int*)&MenuListInfo);
            DisplayHint.I = 0;//clear out the bits
            bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
            break;
        case EVENT_MESSAGE:
            if( gEventInfo.Message.wMsgCommand == MENU_MSG_MEDIA_CHANGED )
                bDone = TRUE;
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
            	case PR_PLAY:
            		bSendToPlayerStateMachine=FALSE;
            		break;
                case PR_FF:
                case PH_FF:
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine=FALSE;
                    iHighLightedItem++;
                    if(iHighLightedItem>JPEGDISPLAYMENU_LAST)
                        iHighLightedItem = JPEGDISPLAYMENU_FIRST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_RW:
                case PH_RW:
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine=FALSE;
                    iHighLightedItem--;
                    if(iHighLightedItem<JPEGDISPLAYMENU_FIRST)
                        iHighLightedItem = JPEGDISPLAYMENU_LAST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_MENU:
#ifdef  USE_PLAYLIST5  // Maybe we can use other ways to judge whether there is jpg files.
                    if(g_wTotalSongCount > 0)
#else
                    if( g_iTotalTracks > 0 )
#endif
					{
	                    iHighLightedItem = EnterJpegSubMenu(iHighLightedItem);
					}
                    DisplayHint.I |= JPEG_DISPLAY_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    if (iHighLightedItem == MENU_JPEG_DISPLAY_EXIT || iHighLightedItem == MENU_MAIN )
                    {   // exit from sub menu
                        bDone = TRUE;
                    }
                    if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC == TRUE)
                    {   // exit from sub menu
                        bDone = TRUE;
                        iNextMenu = MENU_MUSIC;
                    }
#ifdef USE_PLAYLIST5		
					if(iHighLightedItem == MENU_JPEG_DISPLAY_EXIT )
						g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = FALSE;
#endif
                    break;

                case PH_MENU:
                    // allow user to escape to music mode
                    bDone= TRUE;
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
#ifdef USE_PLAYLIST5
					iNextMenu = MENU_MUSIC;
#endif
                    break;

                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // when returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (JPEG_DISPLAY_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    } // end while (bDone)

#ifdef USE_PLAYLIST3
	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ML_browsing_app_init,0,0,0);
#endif
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
//   Outputs:       Selection to highlight upon exiting jpeg display sub menu
//                      if MENU_EXIT: return to main menu selection
//                      else return to jpeg display menu with returned menu highlighted
//
//   Notes:         Return value from sub menu must be supported menu item in jpegdisplay
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int EnterJpegSubMenu(int iSelectedItem)
{

    int _reentrant (*pMenu)(int,int,int*);

    INT iResource=RSRC_JPEG_DISPLAY_MENU_CODE_BANK;
    // local because not saving place at this level,
    // otherwise need another global var
    INT iCurrentMenu=MENU_JPEG_DISPLAY_EXIT;

    while( iSelectedItem != MENU_JPEG_DISPLAY_EXIT )
    {
    pMenu = NULL;  // Make sure that the last menu is cleared out, for the exit case
    switch(iSelectedItem)
    {
        case MENU_MANUAL_MODE:
            iCurrentMenu  = MENU_MANUAL_MODE;
            iResource = RSRC_JPEG_MANUAL_MENU_CODE_BANK;
            pMenu = JpegManualMenu;
            break;
        case MENU_THUMBNAIL_MODE:
            iCurrentMenu  = MENU_THUMBNAIL_MODE;
            iResource = RSRC_JPEG_THUMBNAIL_MENU_CODE_BANK;
            pMenu = JpegThumbnailMenu;
#ifdef USE_PLAYLIST5            
		    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
			SysWaitOnEvent(0,0,0);
			while(!g_PL5_Playback.foundall)
				SysWaitOnEvent(0,0,100);
#endif				
            break;
        case MENU_SLIDESHOW_MODE:
            iCurrentMenu  = MENU_SLIDESHOW_MODE;
            iResource = RSRC_JPEG_SLIDESHOW_MENU_CODE_BANK;
            pMenu = JpegSlideshowMenu;
            break;
/*
        case MENU_EMBEDDED_ART_MODE:
            iCurrentMenu  = MENU_EMBEDDED_ART_MODE;
            iResource = RSRC_EMBEDDED_ART_MENU_CODE_BANK;
            pMenu = EmbeddedArtMenu;
            break;
*/
            case MENU_MAIN:
                iCurrentMenu = MENU_MAIN;
                iSelectedItem = MENU_JPEG_DISPLAY_EXIT;
                break;
        default:
            break;
    }


    if(pMenu)   //only enter a new menu if pMenu is not NULL.
    {
#ifdef USE_PLAYLIST5
					SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_JPEG_DECODER_CALLBACK);
					SysWaitOnEvent(0,0,0);
#endif
            iSelectedItem = SysCallFunction(iResource,pMenu,0,0,0);
        }
    }

    return iCurrentMenu;
}

