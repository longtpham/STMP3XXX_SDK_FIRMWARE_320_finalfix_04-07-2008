#include "exec.h"                                       
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "playmusicmenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "metadata.h"
#include "playerstatemachine.h"
#include "const.h"
#include "hwequ.h"
#include "sysmem.h"
#include "syscoder.h"
#include "sysspeed.h"
#include "playerlib.h"
#include "musiclib_ghdr.h"
#include "sysequ.h"
#include "font.h"
#include "playlist3.h"
#include "browsemenu.h"
#include "resource.h"

extern INT DecoderResourcePtr;

_reentrant void PSM_ResetScrollInfo(void);
_asmfunc void DecoderForceInit(void);

int _reentrant PlayMusicMenu(INT a, INT b, INT *pPtr)
{
    BOOL bDone = FALSE;
    BOOL bDisplayTitle = TRUE;
    BOOL bSendToPlayerStateMachine = FALSE;

    union DisplayHints DisplayHint;

    INT iHighLightedItem=PLAYMUSICMENU_FIRST;
    INT iEvent;

    INT iLastPage = 0; // used to determine if we should clear the screen

    ///////////////////////////////////////////////////////////////////////////
    //These data structures are used by the DisplayPagedList function to create the
    //the main menu lists.  see displaylists.h for more info
    ///////////////////////////////////////////////////////////////////////////

    struct PagedListStructure MenuListInfo;     // m_iNormalDisplayMsg,m_iSelectedDisplayMsg
                                                // m_iItems, MenuItem *m_pItems

    struct MenuItem aItems[PLAYMUSICMENU_COUNT];   // m_iPage, m_iResource, m_ixPos, m_iyPos

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;

    MenuListInfo.m_iItems = PLAYMUSICMENU_COUNT;

    // filling the structure like this takes more p mem, but it's overlaid -- not Y sys mem
    for (a = 0; a < PLAYMUSICMENU_COUNT; a++)
    {   
    	// m_ipage is 0 for all menu labels
        aItems[a].m_ixPos = 0;
        aItems[a].m_iPage = a / PLAYMUSICMENU_PAGE1_COUNT;
        aItems[a].m_iyPos = MENU_ITEM_Y_OFFSET + (a % PLAYMUSICMENU_PAGE1_COUNT) * MENU_ITEM_HEIGHT;
    }

    // load resource numbers
    aItems[MENU_PLAY_ALL].m_iResource = RSRC_STRING_PLAY_ALL;
    //aItems[MENU_ARTIST].m_iResource = RSRC_STRING_ARTIST;
    //aItems[MENU_ALBUM].m_iResource = RSRC_STRING_ALBUM;
    aItems[MENU_SONGS].m_iResource = RSRC_BROWSE_MUSIC_LIST_SONGS;
    aItems[MENU_ON_THE_FLY].m_iResource =RSRC_BROWSE_MUSIC_FAVOURITES;
	/*
    aItems[MENU_NEW_MUSIC].m_iResource = RSRC_STRING_NEW_MUSIC_MENU;
    aItems[MENU_GENRE].m_iResource = RSRC_STRING_GENRE;
    aItems[MENU_YEAR].m_iResource = RSRC_STRING_YEAR;
#if PLAYLIST_ENABLE
    aItems[MENU_PLAYLIST].m_iResource = RSRC_STRING_PLAYLIST;
#endif
	*/
    aItems[MENU_FM_REC].m_iResource = RSRC_BROWSE_MUSIC_FM_FILES;
    //aItems[MENU_LINEIN_REC].m_iResource = RSRC_STRING_LINEIN_REC;
#ifdef _FOLDER_BROWSE_
    aItems[MENU_INTERNAL].m_iResource = RSRC_BROWSE_MUSIC_FOLDER_INTERNAL;
    aItems[MENU_EXTERNAL].m_iResource = RSRC_BROWSE_MUSIC_FOLDER_EXTERNAL;
#endif  // _FOLDER_BROWSE_
    aItems[MENU_PLAYMUSIC_EXIT].m_iResource = RSRC_STRING_EXIT_MENU;
    MenuListInfo.m_pItems = aItems;

    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    //we need to refresh the screen since entering the menu
    DisplayHint.I = MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    while(!bDone) 
    {   
        //mask out all display hints that aren't aimed toward the playmusic menu
        DisplayHint.I &= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

        //wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

        //assume every message goes to the player state machine
        bSendToPlayerStateMachine = TRUE;

        //handle the event
        switch(iEvent)
        {
        case EVENT_TIMER:
		//Coment below to use bitmap menu resource
		/*
            if (iLastPage != aItems[iHighLightedItem].m_iPage)
            { 
                // remember this page
                iLastPage = aItems[iHighLightedItem].m_iPage;
                // clear the display
                DisplayHint.I |= (MAIN_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
            }
            SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
            SysWaitOnEvent(0,0,0);  // to prevent display message queue from overflow
            if(bDisplayTitle == TRUE)
            {   
            	SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,RSRC_STRING_PLAY_MUSIC,(int*)&MenuListInfo);
                bDisplayTitle = FALSE;
            }
            else
            {   
            	SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,0,(int*)&MenuListInfo);
            }
            */
            //Display with bitmap menu resource
            switch(iHighLightedItem)
		{
		case MENU_PLAY_ALL:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_MUSIC_PLAY_ALL+g_iLanguage);
			break;
		case MENU_INTERNAL:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_MUSIC_FOLDER_INTERNAL+g_iLanguage);
			break;
		case MENU_EXTERNAL:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_MUSIC_FOLDER_EXTERNAL+g_iLanguage);
			break;
		case MENU_SONGS:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_MUSIC_SONGS+g_iLanguage);
			break;
		case MENU_ON_THE_FLY:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_MUSIC_FAVORITES+g_iLanguage);
			break;		
		case MENU_FM_REC:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_MUSIC_FM_RECORD+g_iLanguage);
			break;
		case MENU_PLAYMUSIC_EXIT:
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_MUSIC_EXIT+g_iLanguage);
			break;			
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
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine = FALSE;
                    iHighLightedItem++;
                    if(iHighLightedItem>PLAYMUSICMENU_LAST)
                        iHighLightedItem = PLAYMUSICMENU_FIRST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_RW:
                case PH_RW:
                    //don't want to send these to the player state machine (that would switch songs)
                    bSendToPlayerStateMachine=FALSE;
                    iHighLightedItem--;
                    if(iHighLightedItem<PLAYMUSICMENU_FIRST)
                        iHighLightedItem = PLAYMUSICMENU_LAST;
                    DisplayHint.bits.Misc = TRUE;
                    break;

                case PR_PLAY:
                    bDisplayTitle=TRUE;   //it needs to display the title again when return my browse menu.
				switch(iHighLightedItem)
				{
				case MENU_PLAY_ALL:
						SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);	
						SysPostMessage(3,MUSICLIB_BROWSE_SETROOT, &g_current_size);
						SysWaitOnEvent(0,0,0);
				     		SysPostMessage(4,MUSICLIB_START_PLAY_ALL_SONG, &g_current_size, (INT)(DirPath));
						SysWaitOnEvent(0,0,0);
						bPlayVoice = FALSE;
						SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));
				              SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PSM_ResetScrollInfo,0,0,0);
				                    					
						if (g_current_size)
						{
							SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
							SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_SONG_CHANGE,1,100,MENU_SONG_CHANGE_TIMEOUT);
							g_iPlayerState = DECODER_PLAYING;
							
							g_current_index = 0;
							g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
							if( DecoderResourcePtr == RSRC_DEC_SMVADPCM_MOD_CODE )
							{
								SysSetDecoderResource(RSRC_DECMOD_CODE);
								DecoderForceInit();
								SysPostMessage(2, DECODER_RESET);
							}
							SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);	
						}
						else
						{
							if(!(g_wDecoderSR&DECODER_STOPPED))
							{
								SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
								g_iPlayerState = DECODER_STOPPED;
								if( DecoderResourcePtr == RSRC_DEC_SMVADPCM_MOD_CODE )
								{
									SysSetDecoderResource(RSRC_DECMOD_CODE);
									DecoderForceInit();
									SysPostMessage(2, DECODER_RESET);
								}
								SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);	
							}
							SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
							b = GetTextWidthResourceUnicode(RSRC_STRING_NO_FILES);
							a = (LCD_X_SIZE-b)/2;
							if (a < SONG_TITLE_X_POS)
								a = SONG_TITLE_X_POS;
							else if (a + b > LCD_X_SIZE - SONG_TITLE_X_POS)
								a = SONG_TITLE_X_POS + LCD_X_SIZE - b;
							SysPostMessage(5,LCD_PRINT_STRING_RSRC,a,SONG_TITLE_Y_POS,RSRC_STRING_NO_FILES);
							SysWaitOnEvent(0,0,2000);
						}
						DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
				     break;
				/*
				case MENU_NEW_MUSIC:
					SysCallFunction(RSRC_NEW_MUSIC_MENU_CODE_BANK,NewMusicMenu,0,0,0);
					break;
				case MENU_ARTIST:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_ARTIST,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
				case MENU_ALBUM:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_ALBUM,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
				case MENU_GENRE:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_GENRE,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
				case MENU_YEAR:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_YEAR,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
#if PLAYLIST_ENABLE
				case MENU_PLAYLIST:
					//SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_PLAYLIST,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
#endif
				*/
				case MENU_ON_THE_FLY:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,ITEM_ON_THE_GO,BrowseMenu,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
				case MENU_SONGS:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_TRACK,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
				case MENU_FM_REC:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_FMREC,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
				/*
				case MENU_LINEIN_REC:
					SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_PHOTO,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
					break;
					*/
#ifdef _FOLDER_BROWSE_
                case MENU_INTERNAL:
                    SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_INTERNAL,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
                    break;
                case MENU_EXTERNAL:
                    SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_EXTERNAL,MenuListInfo.m_pItems[iHighLightedItem].m_iResource,0);
                    break;
#endif  // _FOLDER_BROWSE_
				case MENU_PLAYMUSIC_EXIT:
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
//                case PR_RW:
                    bDone= TRUE;
                    break;

                case PH_STOP:
		  case PH_PLAY:
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

    return MENU_MUSIC;
}
