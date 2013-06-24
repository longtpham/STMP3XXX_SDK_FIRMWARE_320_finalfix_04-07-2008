#include "exec.h"                                       
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "playmusicmenu.h"
#include "newmusicmenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "playerstatemachine.h"
#include "const.h"
#include "hwequ.h"
#ifdef USE_PLAYLIST3
#include "musiclib_ghdr.h"
#endif
#include "sysequ.h"

int _reentrant 
NewMusicMenu
(
	int a, 
	int b, 
	int *pPtr
)
{
    BOOL bDone = FALSE;
    BOOL bDisplayTitle = TRUE;
    BOOL bSendToPlayerStateMachine = FALSE;

    union DisplayHints DisplayHint;

    INT iHighLightedItem=NEWMUSICMENU_FIRST;
    INT iEvent;

    ///////////////////////////////////////////////////////////////////////////
    //These data structures are used by the DisplayPagedList function to create the
    //the main menu lists.  see displaylists.h for more info
    ///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems

    struct MenuItem aItems[NEWMUSICMENU_COUNT];   // m_iPage, m_iResource, m_ixPos, m_iyPos

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MenuListInfo.m_iItems = NEWMUSICMENU_COUNT;

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0, b = MENU_ITEM_Y_OFFSET; a < NEWMUSICMENU_COUNT; a++)
    {
		aItems[a].m_iPage = a / MENU_PAGE_ITEM_COUNT;
		aItems[a].m_iyPos = b;
		aItems[a].m_ixPos = 0;
		b += MENU_ITEM_HEIGHT;
		if (b >= MENU_PAGE_ITEM_COUNT * MENU_ITEM_HEIGHT + MENU_ITEM_Y_OFFSET)
			b = MENU_ITEM_Y_OFFSET;
    }


    // load resource numbers
    aItems[MENU_1_DAY].m_iResource = RSRC_STRING_1_DAY;
    aItems[MENU_1_WEEK].m_iResource = RSRC_STRING_1_WEEK;
    aItems[MENU_1_MONTH].m_iResource = RSRC_STRING_1_MONTH;
    aItems[MENU_NEWMUSIC_EXIT].m_iResource = RSRC_STRING_EXIT_MENU;

    MenuListInfo.m_pItems = aItems;

    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    //we need to refresh the screen since entering the menu
    DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone) 
    {   
        //mask out all display hints that aren't aimed toward the newmusic menu
        DisplayHint.I &= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        //assume every message goes to the player state machine
        bSendToPlayerStateMachine = TRUE;

        //handle the event
        switch(iEvent)
        {
        case EVENT_TIMER:
            //no other events left to process, so lets update the display
            SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
            if(bDisplayTitle == TRUE)
            {   SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,RSRC_STRING_NEW_MUSIC_MENU,(int*)&MenuListInfo);
                bDisplayTitle = FALSE;
            }else
            {   SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,0,(int*)&MenuListInfo);
            }

            DisplayHint.I = 0;//clear out the bits  
            bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
            break;
        case EVENT_BUTTON:
	        // button pressed, so restart timer if enabled
	        bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
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
                    iHighLightedItem++;
                    if(iHighLightedItem>NEWMUSICMENU_LAST)
                        iHighLightedItem = NEWMUSICMENU_FIRST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_RW:
                case PH_RW:
                    iHighLightedItem--;
                    if(iHighLightedItem<NEWMUSICMENU_FIRST)
                        iHighLightedItem = NEWMUSICMENU_LAST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_MENU:
                bDisplayTitle = TRUE;
				switch(iHighLightedItem)
				{
				   case MENU_1_DAY:
				       iHighLightedItem = SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_1DAY,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
				       break;
				   case MENU_1_WEEK:
				       iHighLightedItem = SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_1WEEK,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
				       break;
				   case MENU_1_MONTH:
				       iHighLightedItem = SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_1MONTH,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
				       break;
				   case MENU_NEWMUSIC_EXIT:
				   default:
				   	  bDone = TRUE;
				   	  DisplayHint.bits.Misc = TRUE;
				       break;
				}
                    DisplayHint.I |= MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    break;

                case PH_MENU:
                    // allow user to escape to music mode
                    // set flag so PR_MENU in music mode will ignore
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    bDone= TRUE;
                    break;

                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // when returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    break;
                default:
                    DisplayHint.bits.Misc = TRUE;
	            	break;                    
            }
        } // end switch(iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
		if ((g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC) || (g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC))
			bDone = TRUE;
    } // end while (bDone)

    return MENU_NEW_MUSIC;
}

/*================================================================================================*/
