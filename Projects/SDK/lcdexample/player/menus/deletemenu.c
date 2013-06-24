////////////////////////////////////////////////////////////////////////////////
// Copyright (C) SigmaTel, Inc. 2002-2005
// Filename:    deletemenu.c (LCD audio player)
// Description: Delete screen offers file deletion options to users.
////////////////////////////////////////////////////////////////////////////////

#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "display.h"
#include "displaylists.h"
#include "mainmenu.h"
#include "deletemenu.h"
#include "const.h"
#include "FileSystem.h"
#include "playerstatemachine.h"
#include "sysmem.h"
#include "fsapi.h"
#include "persist_bits.h"

#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#include "playlist1internal.h"
#else
#ifdef USE_PLAYLIST2
#include "playlist2.h"
#include "playlist2internal.h"
#else
#ifdef USE_PLAYLIST3
#include "playlist3.h"
#include "playlist3internal.h"
#include "browsemenu.h"
#else
#ifdef  USE_PLAYLIST5
#include "playlist5.h"
#endif
#endif
#endif
#endif

#include "playerlib.h"
#include "syscoder.h"   //for extern unsigned int _Y g_wCurrentSongNumber;
#include "metadata.h"
#include "hwequ.h"
#ifdef USE_PLAYLIST3
#include "musiclib_ghdr.h"
extern BOOL g_FlashWriteOccurFlag;
extern int24 g_iBrowseTotalNum;
static BOOL bWaitforDisplay = FALSE;
#endif	// #ifdef USE_PLAYLIST3

#pragma optimize 1

int _reentrant DeleteFile(void);
_reentrant void DeleteAllFiles(void);

#ifdef USE_PLAYLIST3
#pragma asm
	nolist
	include "sysresources.inc"
	include "sysequ.inc"
	list
#pragma endasm

void _reentrant ML_browsing_delete_init(void)
{
#pragma asm
    include "resource.inc"
    org     p,".ptextdeletemenu":
	extern	F_lc_u_e_MusicLibBrowse_P
	extern	F_lc_u_b_MusicLibBrowse_P
	extern	SysLoadResource

	; Load the MusicLib building engine into P memory
    move    #>RSRC_MUSICLIB_BROWSE_APP,x0                      ; Resource number
    move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
    move    #>(F_lc_u_e_MusicLibBrowse_P-F_lc_u_b_MusicLibBrowse_P)*3,y1 ; overlay area sz
    move    #>F_lc_u_b_MusicLibBrowse_P,y0                      ; Address of the overlay area
    move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
    jsr     SysLoadResource
#pragma endasm
}

int _reentrant select_delete_type_submenu(void)
{
	union DisplayHints DisplayHint;
	BOOL bDone = FALSE;
	INT iEvent;
	int result = 0;
	INT display_type[3];

    SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);    // To fix defect 10915
	while (!bDone)
	{
        display_type[0] = display_type[1] = display_type[2] = LCD_PRINT_STRING_RSRC;
        display_type[result] = LCD_PRINT_STRING_INV_RSRC;

		//SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);  // To fix defect 10915
		SysPostMessage(5,display_type[0],MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET,RSRC_STRING_VOICE_MENU);
		SysPostMessage(5,display_type[1],MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET+MENU_ITEM_HEIGHT,RSRC_STRING_FM_REC);
        SysPostMessage(5,display_type[2],MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET+MENU_ITEM_HEIGHT*2,RSRC_STRING_LINEIN_REC);

		iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,(DisplayHint.I? 1 : -1));
		DisplayHint.I = 0;
		if (iEvent == EVENT_MESSAGE)
		{
			if (gEventInfo.Message.wMsgCommand == MENU_MSG_MEDIA_CHANGED)
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
			SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,(int *) &gEventInfo);
		}
		else if (iEvent == EVENT_BUTTON)
		{
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
			DisplayHint.bits.Misc = TRUE;
			switch(gEventInfo.Button.wButtonEvent)
			{
            case PR_FF:
                result++;
                if (result>=MAXSUBMENU)
                    result = 0;
                break;
            case PR_RW:
                result--;
                if (result<0)
                    result = MAXSUBMENU-1;
                break;
			case PR_MENU:
				bDone = TRUE;
				break;
			case PR_VOL_UP:
			case PH_MENU:
				// allow user to escape to music mode
				bDone = TRUE;
				// set flag so PR_MENU in music mode will ignore
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
				break;
			case PH_STOP:
				//if the STOP button is held down, lets call the shutdown menu
				SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
				break;
			}
		}
		if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC)
		{
			bDone = TRUE;
			result = -1;
		}
	}
	return result;
}
#endif	// #ifdef USE_PLAYLIST3
/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          	void clear_display
//
//   Type:          Function (clear display message sent to display task)
//
//   Description:   This function sends the clear display message to display task.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         SDK3.0 used macro but SDK2.6 used func. Consider if inline is appropriate.
//
//
//////////////////////////////////////////////////////////////////////////////////////////

// #define clear_display() SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayClearDisplay,0,0,0)
void clear_display(void)
{
    SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayClearDisplay,0,0,0);
}



/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          int  hndl_delete_main_menu(WORD button, int * iHighLightedItem)
//
//   Type:          Function (hndl buttons that are processed in delete main menu)
//
//   Description:   This function allows selection of the delete sub-menus/functions (Music, Voice,
//					All_music, All_voice, exit). It processes the button and returns to  DeleteMenu.
//
//   Inputs:        button - to process (	PR_FF, PH_FF,  PR_RW, PH_RW, PR_MENU)
//				    iHighLightedItem -- sub-menu that is currently selected.
//
//   Outputs:       iHighLightedItem - set to the current selection
//                  return next_menu -- delete sub_menu or the next menu to go to.
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////

int _reentrant hndl_delete_main_menu(WORD button, int * iHighLightedItem)
{
	int inext_menu = MENU_DELETE_MAIN;

	// check which button and process accordingly, caller assures valid buttons passed
    switch(button)
	{
      	case PR_FF:
    	case PH_FF:
        	(*iHighLightedItem)++;
        	if(*iHighLightedItem>DELETEMENU_LAST)
            	*iHighLightedItem = DELETEMENU_FIRST;
        	break;

    	case PR_RW:
   		case PH_RW:
        	(*iHighLightedItem)--;
        	if(*iHighLightedItem<DELETEMENU_FIRST)
            	*iHighLightedItem = DELETEMENU_LAST;
        	break;

    	case PR_MENU:
            inext_menu = *iHighLightedItem;
#ifdef USE_PLAYLIST3
		g_bDeleteBrowse = TRUE;
		switch (*iHighLightedItem)
		{
		case MENU_DELETE_VOICE:
			switch(select_delete_type_submenu())
			{
            case VOICE_SUBMENU:
                SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_VOICE,RSRC_STRING_VOICE_MENU,0);
                break;
            case FMREC_SUBMENU:
                SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_FMREC,RSRC_STRING_FM_REC,0);
                inext_menu = MENU_DELETE_FM;
                break;
            case LINEIN_SUBMENU:
                SysCallFunction(RSRC_BROWSE_MENU_CODE_BANK,BrowseMenu,ITEM_PHOTO,RSRC_STRING_LINEIN_REC,0);
                inext_menu = MENU_DELETE_LINEIN;
                break;
			default:
				inext_menu = MENU_DELETE_MAIN;
				break;
			}
			break;
		case MENU_DELETE_VOICE_ALL:
			switch(select_delete_type_submenu())
			{
            case VOICE_SUBMENU:
                break;
            case FMREC_SUBMENU:
                inext_menu = MENU_DELETE_FM_ALL;
                break;
            case LINEIN_SUBMENU:
                inext_menu = MENU_DELETE_LINEIN_ALL;
                break;
			default:
				inext_menu = MENU_DELETE_MAIN;
				break;
			}
			break;
		}
		g_bDeleteBrowse = FALSE;
#else	// #ifdef USE_PLAYLIST3
#if 0 // Delete all songs is very slow (like minutes), so this feature is removed
            if((*iHighLightedItem == MENU_DELETE_MUSIC_ALL) || (*iHighLightedItem == MENU_DELETE_MUSIC))
            {
                ChangePlaySet(PLAYSET_MUSIC);
                //Playlist_SetPlaySet will reset the current list to the beginning of the list.
                if (*iHighLightedItem == MENU_DELETE_MUSIC_ALL)
                SysCallFunction(RSRC_PLAYERLIST_CODEBANK,Playlist_SetPlaySet,Playlist_GetPlaySet(),TRUE,0);

                if (SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,CURRENTSONG_DELETE_MENU,0,0) != PLAYERLIB_SUCCESS)	//stmp7120 Delete even bad files.
                {
					// stay in Delete main
					inext_menu = MENU_DELETE_MAIN;
                }
            }
            else
 #endif
            if((*iHighLightedItem == MENU_DELETE_VOICE_ALL) || (*iHighLightedItem == MENU_DELETE_VOICE))
            {
                ChangePlaySet(PLAYSET_VOICE);
                //Playlist_SetPlaySet will reset the current list to the beginning of the list.
                if (*iHighLightedItem == MENU_DELETE_VOICE_ALL)
                    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_SetPlaySet,Playlist_GetPlaySet(),TRUE,0);

                if (SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0) != PLAYERLIB_SUCCESS) //stmp7120 Delete even bad files
                {
                    // handle boundry cases, stay in DELETE main
                    inext_menu = MENU_DELETE_MAIN;
                }
            }

#endif // #ifndef USE_PLAYLIST3
            break;

        default:
            // error.
           break;
        } // end switch

	return inext_menu;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          	void display_question_track_yes_or_no
//
//   Type:          Function (display question, track to delete, yes or no option)
//
//   Description:   This function displays the question: whether file should be deleted?
//					displays the track/title (for the file that will be deleted)
//					displays yes or no (one of the options is highlighted)
//
//
//   Inputs:        y_or_no:  highlight either yes or no option
//
//   Outputs:       none
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////

void _reentrant display_question_track_yes_or_no(WORD y_or_no )
{
	// Question
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE1_X,WARNING_MSG_LINE1_Y,RSRC_WARNING_MSG_LINE1);
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE2_X,WARNING_MSG_LINE2_Y,RSRC_WARNING_MSG_LINE2);

	//Track
	SysPostMessage(6,LCD_CLEAR_RANGE,WARNING_MSG_FILE_X,WARNING_MSG_FILE_Y,SONG_TITLE_X_SIZE,SONG_TITLE_Y_SIZE);
	SysPostMessage(5,LCD_PRINT_STRING_UNICODE_ADDR,WARNING_MSG_FILE_X,WARNING_MSG_FILE_Y,g_wSongTitle);

	// YES or NO
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_YES_X,WARNING_MSG_YES_Y,y_or_no);

	 SysWaitOnEvent(0,0,0); // allow lcd to process messages
 }

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          	void display_question_del_all_yes_or_no
//
//   Type:          Function (display question - whether to delete all files (music or voice)
//
//   Description:   This function displays the question: whether all files should be deleted?
//					displays yes or no (one of the options is highlighted)
//
//
//   Inputs:        y_or_no:  highlight either yes or no option
//
//   Outputs:       none
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////

void _reentrant display_question_del_all_yes_or_no(WORD y_or_no )
{
	// Question
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE1_X,WARNING_MSG_LINE1_Y,RSRC_WARNING_MSG_LINE1);
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE2_X,WARNING_MSG_LINE2_Y,RSRC_WARNING_MSG_ALL_LINE2);

	// Yes or NO
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_YES_X,WARNING_MSG_YES_Y,y_or_no);

	 SysWaitOnEvent(0,0,0); // allow lcd to process messages
 }

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          int  hndl_del_music_voice_file_menu(int inext_menu, WORD button, int * iHighLightedItem)
//
//   Type:          Function (handle buttons that are processed in delete file (music or voice) sub-menu
//
//   Description:   This function allows processing of the delete file sub-menus/functions (yes or no toggle
//                  and selection of yes or no and associated processing).
//
//   Inputs:        button - to process (	PR_FF, PH_FF,  PR_RW, PH_RW, PR_MENU)
//				    iHighLightedItem -- yes or no that is currently selected.
//					inext_menu - current sub-menu
//
//   Outputs:
//                  return next_menu -- delete main or the current delete file sub-menu (music or voice).
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////

int _reentrant hndl_del_music_voice_file_menu(int inext_menu, WORD button, int * iHighLightedItem)
{
#ifdef USE_PLAYLIST3
	int iError;
#endif	// #ifdef USE_PLAYLIST3
	// check which button and process accordingly
    switch(button)
	{
    	case PR_FF:
		case PH_FF:
		case PR_RW:
    	case PH_RW:
        	if (*iHighLightedItem == NO)
			{
				*iHighLightedItem = YES;
			}
			else
			{
            	*iHighLightedItem = NO;
			}
        	break;

    	case PR_MENU:
#ifdef USE_PLAYLIST3
		if(*iHighLightedItem != NO)
		{
			// clear MTP database clean flag; an object is being deleted, which needs to be accounted for
			ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);

			SysLoadFATWrite();
			iError = Fremove((_packed char *)g_CurrentSongName);
			SysUnLoadFATWrite();     // allow other apps to overwrite Fat Write App

			if (iError == NOERROR)
			{
				if ((g_CurrentSongName[0] == 0x2f3a61) || (g_CurrentSongName[0] == 0x2f3a41))
				{
					ClearStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG,
					HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS);
					g_FlashWriteOccurFlag = TRUE;
				}
				bSystemInit = FALSE;
				g_Rebuild = TRUE;
				SysPostMessage(2,PARSER_DEVICE_ENUMERATE);
			}
			bWaitforDisplay = TRUE; // To fix defect 10915
		}
		inext_menu = MENU_DELETE_MAIN;
#else	// #ifdef USE_PLAYLIST3

			if(*iHighLightedItem == NO)
			{
				if (SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SkipToNextSong,FALSE,NEXTSONG_DELETE_MENU,0) != PLAYERLIB_SUCCESS)
				{
					// handle boundary cases.
					inext_menu = MENU_DELETE_MAIN;
				}
				else
				{
					*iHighLightedItem = NO;
			   }
			}
			else
			{

 				if (DeleteFile() == NOERROR)
 				{
					if (SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SkipToNextSong,FALSE,NEXTSONG_DELETE_MENU,0) != PLAYERLIB_SUCCESS)
					{
						// handle boundry cases.
						if ( g_wCurrentSongNumber == 1)
						{

						    if (SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,CURRENTSONG_DELETE_MENU,0,0) == PLAYERLIB_SUCCESS) //stmp7120 Delete even bad files
						    {
								// don't leave
								*iHighLightedItem = NO;
							}
							else
							{
    							inext_menu = MENU_DELETE_MAIN;
						   	}
						}
						else
						{
							inext_menu = MENU_DELETE_MAIN;
						}

					}
					else
					{
						*iHighLightedItem = NO;
					}
				}
				else
				{
					   // error
						inext_menu = MENU_DELETE_MAIN;
				}
			}

#endif	// #ifdef USE_PLAYLIST3
         	break;

		default:
			// error
			break;
	} // end switch
#ifndef USE_PLAYLIST3
	if (inext_menu == MENU_DELETE_MAIN)
	        SysPostMessage(2,PARSER_DEVICE_ENUMERATE);
#endif	// #ifndef USE_PLAYLIST3
	return inext_menu;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          int  hndl_del_music_voice_all_menu(int inext_menu, WORD button, int * iHighLightedItem)
//
//   Type:          Function (handle buttons that are processed in delete all files (music or voice) sub-menu
//
//   Description:   This function allows processing of the delete all file sub-menus/functions (yes or no toggle
//                  and selection of yes or no and associated processing).
//
//   Inputs:        button - to process (	PR_FF, PH_FF,  PR_RW, PH_RW, PR_MENU)
//				    iHighLightedItem -- yes or no that is currently selected.
//					inext_menu - current sub-menu
//
//   Outputs:
//                  return next_menu -- delete main or the current delete all file sub-menu (music or voice).
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant hndl_del_music_voice_all_menu(int inext_menu, WORD button, int * iHighLightedItem)
{
#ifdef USE_PLAYLIST3
	INT browse_type;
	INT highlight;
	INT num_of_item;
	BOOL bHaveNext;
	int iDisplayProgress=0;
#endif	// #ifdef USE_PLAYLIST3
	// check which button and process accordingly
    switch(button)
	{
    	case PR_FF:
		case PH_FF:
		case PR_RW:
    	case PH_RW:
        	if (*iHighLightedItem == NO)
			{
				*iHighLightedItem = YES;
			}
			else
			{
            	*iHighLightedItem = NO;
			}
        	break;

    	case PR_MENU:
#ifdef USE_PLAYLIST3
		if(*iHighLightedItem != NO)
		{
            if (inext_menu == MENU_DELETE_VOICE_ALL)
                browse_type = ITEM_VOICE;
            else if (inext_menu == MENU_DELETE_FM_ALL)
                browse_type = ITEM_FMREC;
            else
                browse_type = ITEM_PHOTO;
			highlight = 0;

			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);


			SysPostMessage(3,MUSICLIB_BROWSE_SETROOT, (INT*)(&num_of_item));
			SysWaitOnEvent(0,0,0);

			if (browse_type == ITEM_TRACK)
                SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)(&num_of_item), browse_type, 0, (INT*)(&g_iBrowseTotalNum), (INT*)(&bHaveNext));
			else
                SysPostMessage(7,MUSICLIB_VOICE_BROWSEMENU, (INT*)(&num_of_item), browse_type, 0, (INT*)(&g_iBrowseTotalNum), (INT*)(&bHaveNext));
			SysWaitOnEvent(0,0,0);

			SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
			if (num_of_item == 0) {
				SysPostMessage(5,LCD_PRINT_STRING_RSRC,0,20,RSRC_STRING_NO_FILES);
				SysWaitOnEvent(0,0,200);
				inext_menu = MENU_DELETE_MAIN;
				break;
			}

			// clear MTP database clean flag; an object is being deleted, which needs to be accounted for
			ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);

			while (highlight < num_of_item)
			{
				SysCallFunction(RSRC_METADATA_CODEBANK,GetRelativePath,highlight,browse_type,(INT*)(g_CurrentSongName));

				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_FILE_X+8*iDisplayProgress,WARNING_MSG_FILE_Y,RSRC_FILE_DELETE_STAR);
				iDisplayProgress++;
				if (iDisplayProgress>5)
				{
					iDisplayProgress=0;
					SysPostMessage(6,LCD_CLEAR_RANGE,WARNING_MSG_FILE_X,WARNING_MSG_FILE_Y,SONG_TITLE_X_SIZE,SONG_TITLE_Y_SIZE);
				}
				SysWaitOnEvent(0,0,0);
				SysLoadFATWrite();
				Fremove((_packed char *)g_CurrentSongName);
				SysUnLoadFATWrite();     // allow other apps to overwrite Fat Write App

				if (bHaveNext)
				{
					if (browse_type == ITEM_TRACK)
						SysPostMessage(4,MUSICLIB_BROWSE_DOWNLIST, 1, (INT*)(&bHaveNext));
					else
						SysPostMessage(4,MUSICLIB_VOICE_DOWNLIST, 1, (INT*)(&bHaveNext));
					SysWaitOnEvent(0,0,0);
				}
				else
					highlight++;
			}
			ClearStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG,
			HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS);
			g_FlashWriteOccurFlag = TRUE;
			bSystemInit = FALSE;
			g_Rebuild = TRUE;
			SysPostMessage(2,PARSER_DEVICE_ENUMERATE);
			SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
			bWaitforDisplay = TRUE; // To fix defect 10915
		}
		inext_menu = MENU_DELETE_MAIN;
#else	// #ifdef USE_PLAYLIST3

			if(*iHighLightedItem != NO)
			{

				DeleteAllFiles();

			}
			inext_menu = MENU_DELETE_MAIN;

#endif	// #ifdef USE_PLAYLIST3
         	break;

		default:
			// error
			break;
	} // end switch

	return inext_menu;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          int _reentrant DeleteMenu(int a, int b, int *pPtr)
//
//   Type:          Function (Delete menu entry point)
//
//   Description:   This function is the entry point for the Delete menu.  It presents
//                  the user with the choice to delete either Voice or Music files.
//
//   Inputs:        a  is set by the caller to be non-zero if need to enter muisc or voice file delete sub-menu directly
//					b is set to PLAYSET_MUSIC or PLAYSET_VOICE if a is non-zero by the caller (which files to delete).
//
//   Outputs:       return to main menu
//                      if PH_MENU button received, set up global g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC
//						so that on return from the delete menu main menu can initiate music menu.
//
//   Notes:
//
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant DeleteMenu(int a, int b, int *pPtr)
{
    BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;
    INT iHighLightedItem=DELETEMENU_FIRST;
	INT iDeleteQuestionHighLightedItem = NO;
    INT iNextMenu = MENU_DELETE_MAIN;  // default start menu assigned
	INT iCurrentMenu = -1; // check if a new menu initilaztion is required.
	INT iEvent;
	struct PagedListStructure MenuListInfo;
    struct MenuItem aItems[DELETEMENU_COUNT];
	union DisplayHints DisplayHint;
	INT i;
	 int old_iPlaylistRepeat = 	  g_iPlaylistRepeat;
	 int old_bPlaylistShuffle =   g_bPlaylistShuffle;

#ifdef USE_PLAYLIST3
    bWaitforDisplay = FALSE;
#endif
 	MenuListInfo.m_iNormalDisplayMsg = LCD_PRINT_STRING_RSRC;
    MenuListInfo.m_iSelectedDisplayMsg = LCD_PRINT_STRING_INV_RSRC;
    MenuListInfo.m_iItems = DELETEMENU_COUNT;

    for (i = 0; i < DELETEMENU_COUNT; i++)
    {   // m_ipage is 0 for all menu labels
        aItems[i].m_iPage = 0; // all on page 0
        aItems[i].m_ixPos = 0;	 // all @x = 0, start at same X loc
		aItems[i].m_iyPos = const_24 + i*8; // each line is 8 pixels lower and start at 24 pixels
    }

    // load resource numbers
//    aItems[MENU_DELETE_MUSIC].m_iResource		= RSRC_STRING_MUSIC_MENU;
//	aItems[MENU_DELETE_MUSIC_ALL].m_iResource	= RSRC_STRING_MUSIC_MENU_DELETE_ALL;
    aItems[MENU_DELETE_VOICE].m_iResource		= RSRC_STRING_VOICE_MENU;
	aItems[MENU_DELETE_VOICE_ALL].m_iResource	= RSRC_STRING_VOICE_MENU_DELETE_ALL;
    aItems[MENU_DELETE_EXIT].m_iResource		= RSRC_STRING_EXIT_MENU;


    MenuListInfo.m_pItems = aItems;

        // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
     pPtr = (int *) &gEventInfo;

	// stop the decoder
    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
#ifdef USE_PLAYLIST3
	SysWaitOnEvent(0,0,0);
#endif

    g_iPlayerState = DECODER_STOPPED;

	g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;  // so that cycling of files is successful.
	g_bPlaylistShuffle = FALSE;

	// check if we need to enter music or voice file delete directly
	if (a)
	{
		// Request to enter music file or voice file delete directly
/*
        if (b == PLAYSET_MUSIC)
		{
			iHighLightedItem =  MENU_DELETE_MUSIC ;
			iNextMenu = hndl_delete_main_menu(PR_MENU,&iHighLightedItem);
		}
		else
*/
        if (b == PLAYSET_VOICE)
		{
			iHighLightedItem =  MENU_DELETE_VOICE;
			iNextMenu = hndl_delete_main_menu(PR_MENU,&iHighLightedItem);
		}
#ifdef USE_PLAYLIST3
		if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC)
		{
			iNextMenu = MENU_DELETE_MAIN;
			g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = FALSE;
		}
#endif //#ifdef USE_PLAYLIST3
	}

    do // need to execute do-while loop at least once
    {

		// check if the new menu is different from the old menu ... perform initialization setps.

		if (iCurrentMenu != iNextMenu) {

			switch (iNextMenu)
				{
					case MENU_DELETE_MAIN:
						DisplayHint.I = DELETE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
						break;

//					case MENU_DELETE_MUSIC :
					case MENU_DELETE_VOICE:
//					case MENU_DELETE_MUSIC_ALL :
					case MENU_DELETE_VOICE_ALL:
#ifdef USE_PLAYLIST3
			case MENU_DELETE_FM:
			case MENU_DELETE_FM_ALL:
            case MENU_DELETE_LINEIN:
            case MENU_DELETE_LINEIN_ALL:
#endif	// #ifdef USE_PLAYLIST3
						iDeleteQuestionHighLightedItem = NO;
						// clear the display
	    				clear_display();
						DisplayHint.I = DELETE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
						break;

					default :
						break;
				} // end switch

			iCurrentMenu = iNextMenu;

		} // end if
#ifdef USE_PLAYLIST3
		if ((bWaitforDisplay) && (bSystemInit))
		{
			bWaitforDisplay = FALSE;
			clear_display();
			DisplayHint.I = DELETE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
		}
#endif	// #ifdef USE_PLAYLIST3

		if (DisplayHint.I)
		{
			SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay, DisplayHint.I, 1, 0);
    		if (iNextMenu == MENU_DELETE_MAIN) {
#ifdef USE_PLAYLIST3
    			SysPostMessage(6,LCD_CLEAR_RANGE,MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET,LCD_SIZE_X,MENU_ITEM_Y_OFFSET+MENU_ITEM_HEIGHT*3);  //John_test
#endif
				SysCallFunction(RSRC_DISPLAY_LISTS_CODE_BANK,DisplayPagedList,iHighLightedItem,BITMAP_ERASE_TITLE_NUM,(int*)&MenuListInfo);
			}
#ifdef USE_PLAYLIST3
			else if ((iNextMenu == MENU_DELETE_VOICE)|| (iNextMenu == MENU_DELETE_FM)|| (iNextMenu == MENU_DELETE_LINEIN)) {
#else	// #ifdef USE_PLAYLIST3
			else if (iNextMenu == 	MENU_DELETE_VOICE) {
//			else if ((iNextMenu == MENU_DELETE_MUSIC)|| (iNextMenu == 	MENU_DELETE_VOICE)) {
#endif	// #ifdef USE_PLAYLIST3
				if (iDeleteQuestionHighLightedItem == NO)
					display_question_track_yes_or_no (	RSRC_DELETE_NO);
				else
					display_question_track_yes_or_no (	RSRC_DELETE_YES);

			}
#ifdef USE_PLAYLIST3
			else if ((iNextMenu == MENU_DELETE_VOICE_ALL)|| (iNextMenu == MENU_DELETE_FM_ALL)|| (iNextMenu == MENU_DELETE_LINEIN_ALL)) {
#else	// #ifdef USE_PLAYLIST3
			else if (iNextMenu ==    MENU_DELETE_VOICE_ALL) {
//			else if ((iNextMenu ==   MENU_DELETE_MUSIC_ALL)|| (iNextMenu ==    MENU_DELETE_VOICE_ALL)) {
#endif	// #ifdef USE_PLAYLIST3
				if (iDeleteQuestionHighLightedItem == NO)
					display_question_del_all_yes_or_no (RSRC_DELETE_NO);
				else
					display_question_del_all_yes_or_no (RSRC_DELETE_YES);
			}

			DisplayHint.I = 0;
		}

		//wait until a message or button happens.
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg, -1);

        bSendToPlayerStateMachine = TRUE;

		switch (iEvent) {

    		//check for EVENT_BUTTON
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
				bSendToPlayerStateMachine = FALSE;
				// check which button and process accordingly
	       		 switch(gEventInfo.Button.wButtonEvent)
	        	{
	            	case PH_HOLD:
	           		case PR_HOLD:
	                	DisplayHint.bits.LockIcon = TRUE;

	                break;

	            	case PH_MENU:
	                	// allow user to escape to music mode
	               		 iNextMenu = MENU_DELETE_EXIT;
	                	// set flag so PR_MENU in music mode will ignore
	                	g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
	                	break;

	            	case PH_STOP:
	                	//if the STOP button is held down, lets call the shutdown menu
	                	SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
	                	// when returns update display  -- if doesn't return, powered down
	                	DisplayHint.I |= DELETE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
	                	break;

					case PR_FF:
					case PH_FF:
					case PR_RW:
					case PH_RW:
					case PR_MENU:
						bSendToPlayerStateMachine = FALSE;
#ifdef USE_PLAYLIST3
			if (iNextMenu == MENU_DELETE_MAIN) {
				iNextMenu = hndl_delete_main_menu(gEventInfo.Button.wButtonEvent,&iHighLightedItem);
			}
			else if ((iNextMenu == MENU_DELETE_VOICE)|| (iNextMenu == MENU_DELETE_FM)|| (iNextMenu == MENU_DELETE_LINEIN)) {
				iNextMenu = hndl_del_music_voice_file_menu(iNextMenu, gEventInfo.Button.wButtonEvent, &iDeleteQuestionHighLightedItem);
				//bWaitforDisplay = TRUE;   // To fix defect 10915
			}
			else if ((iNextMenu ==    MENU_DELETE_VOICE_ALL)|| (iNextMenu == MENU_DELETE_FM_ALL)|| (iNextMenu == MENU_DELETE_LINEIN_ALL)) {
				iNextMenu = hndl_del_music_voice_all_menu(iNextMenu, gEventInfo.Button.wButtonEvent, &iDeleteQuestionHighLightedItem);
				//bWaitforDisplay = TRUE;   // To fix defect 10915
			}
			DisplayHint.bits.Misc = TRUE;

			if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC)
			{
				iNextMenu = MENU_DELETE_MAIN;
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = FALSE;
			}
#else	// #ifdef USE_PLAYLIST3

    					if (iNextMenu == MENU_DELETE_MAIN) {
							iNextMenu =
								hndl_delete_main_menu(gEventInfo.Button.wButtonEvent,&iHighLightedItem);
						}
						else if (iNextMenu == MENU_DELETE_VOICE) {
//						else if ((iNextMenu == MENU_DELETE_MUSIC)|| (iNextMenu == 	MENU_DELETE_VOICE)) {

							iNextMenu =
								hndl_del_music_voice_file_menu(iNextMenu, gEventInfo.Button.wButtonEvent, &iDeleteQuestionHighLightedItem);
						}
						else if (iNextMenu == MENU_DELETE_VOICE_ALL) {
//						else if ((iNextMenu ==   MENU_DELETE_MUSIC_ALL)|| (iNextMenu ==    MENU_DELETE_VOICE_ALL)) {
								iNextMenu =
									hndl_del_music_voice_all_menu(iNextMenu, gEventInfo.Button.wButtonEvent, &iDeleteQuestionHighLightedItem);
						}
	                	DisplayHint.bits.Misc = TRUE;

#endif	// #ifdef USE_PLAYLIST3
						break;

					case PR_PLAY:
						// Prevent the user from restarting the Decoder,
						// ignore/consume this button event (fix defect #stmp7488)
						break;

					default:
					bSendToPlayerStateMachine = TRUE; // 3.05 sets this to TRUE. 2.6 had it as false.
						break;
				  }	// end switch
				  break;

	        case EVENT_MESSAGE:
				if(gEventInfo.Message.wMsgCommand == MENU_MSG_MEDIA_CHANGED)
				{
							// In this case, we need to go back to the delete main menu
							// (the song might not be there any more)
							iNextMenu = MENU_DELETE_MAIN;
#ifdef USE_PLAYLIST3
			bWaitforDisplay = TRUE;
#endif
				}
#ifdef MMC
				else if(gEventInfo.Message.wMsgCommand == MENU_MSG_MEDIA_OVER_CUR)
				{
							// In this case, we need to go back to the delete main menu
							// (the song might not be there any more)
							iNextMenu = MENU_DELETE_MAIN;
				}

#endif
				else
				{
	            	//These all need to be sent to the player state machine
	            	bSendToPlayerStateMachine = TRUE;
				}
	            break;

				default:
				  	//Let PSM handle the object in this case
					bSendToPlayerStateMachine = TRUE;
				  	break;
			}	//switch(iEvent)


       if(bSendToPlayerStateMachine)
	       SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

	}    while(	 iNextMenu != MENU_DELETE_EXIT );

	// clear the display
	clear_display();

	g_iPlaylistRepeat = old_iPlaylistRepeat;  // restore globals
	g_bPlaylistShuffle = old_bPlaylistShuffle;

    return MENU_MAIN;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          	void DisplayDeleteResultBitmap (int iBitmapResourceNum)
//
//   Type:          Function
//
//   Description:   This function was defined to save X memory in DeleteAllFiles
//
//
//   Inputs:        iBitmapResourceNum
//
//   Outputs:       none
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////

void DisplayDeleteResultBitmap(int iBitmapResourceNum)
{
    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,iBitmapResourceNum,1000,0);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          	void DeleteAllFiles (void)
//
//   Type:          Function (delete all files - music or voice)
//
//   Description:   This function deletes all files and updates display accordingly
//
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////

void _reentrant DeleteAllFiles(void)
{
    int iError = NOERROR;
	BOOL done = FALSE;
	Message msg;
	int iDisplayProgress=0;

    // prevent msg queue overflow
#ifdef USE_PLAYLIST3
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
#else
	// SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
#endif
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);

    // Clear MTP database clean flag; an object is being deleted, which needs to be accounted for
    ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);

	// clear display
	clear_display();

	//Display Success (would be nice to show deleting files)
    //SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_DELETE_SUCCESSFUL,1000,0);
    DisplayDeleteResultBitmap(RSRC_DELETE_SUCCESSFUL);

	while (!done)
	{
		// Load FAT write module into application space and block other apps
		SysLoadFATWrite();

		iError = Fremove((_packed char *)g_CurrentSongName);

		SysUnLoadFATWrite();


		if (iError == NOERROR)
		{

			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_FILE_X+8*iDisplayProgress,WARNING_MSG_FILE_Y,RSRC_FILE_DELETE_STAR);
			iDisplayProgress++;
			if (iDisplayProgress>5)
			{
				iDisplayProgress=0;
				SysPostMessage(6,LCD_CLEAR_RANGE,WARNING_MSG_FILE_X,WARNING_MSG_FILE_Y,SONG_TITLE_X_SIZE,SONG_TITLE_Y_SIZE);
			 }
#ifdef USE_PLAYLIST3
			if (PLAYERLIB_END_OF_LIST == SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SkipToNextSong,FALSE,NEXTSONG_DELETE_MENU,0))	//stmp7120 Delete even bad files
#else
			if (SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SkipToNextSong,FALSE,NEXTSONG_DELETE_MENU,0) != PLAYERLIB_SUCCESS) //stmp7120 Delete even bad files
#endif
			{
				done = TRUE;
			}
			SysWaitOnEvent(0,0,0);
		}
		else
		{
			//Something has gone wrong.  Perhaps the media is bad, or the user has removed the external media
        	//in the middle of the operation.
        	//SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_DELETE_ERROR,1000,0);
            DisplayDeleteResultBitmap(RSRC_DELETE_ERROR);
			done = TRUE;
		}

	} // end while

	// re-enumerate devices
    SysPostMessage(2,PARSER_DEVICE_ENUMERATE);

	// clear display
	clear_display();

	// re-enable soft timers
	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//  Name:          	int _reentrant DeleteFile (void)
//
//   Type:          Function (delete currently selected file - music or voice)
//
//   Description:   This function deletes the currently selected file and updates display accordingly
//
//
//   Inputs:        none
//
//   Outputs:       int error code from Fremove or if that was ok, an error code
//                  from SysUnloadFatWrite or NOERROR.
//
//   Notes:
//
//
//////////////////////////////////////////////////////////////////////////////////////////

int _reentrant DeleteFile(void)

{
    int iBitmapResourceNum = RSRC_DELETE_ERROR;
    int iError;

//	Message msg;

    // clear MTP database clean flag; an object is being deleted, which needs to be accounted for
    ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);

	// clear display
	clear_display();

    // Load FAT write module into application space and block other apps
    SysLoadFATWrite();
    // delete file
    iError = Fremove((_packed char *)g_CurrentSongName);

    if ( SUCCESS != iError ) // Fremove result

    {   //Something has gone wrong.  Perhaps the media is bad, or the user has removed the external media
        //in the middle of the operation.
        //SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_DELETE_ERROR,1000,0);
        //iBitmapResourceNum = RSRC_DELETE_ERROR;
        SysUnLoadFATWrite();     // allow other apps to overwrite Fat Write App
    }
    else // Fremove SUCCESS
        if( SUCCESS == (iError = SysUnLoadFATWrite()) )  //allow other apps to overwrite Fat Write App
        {
              	// re-enumerate devices after successful delete
                SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);

        		SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SkipToPreviousSong,FALSE,0,0);

        		//Display Success
                //SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_DELETE_SUCCESSFUL,1000,0);
                iBitmapResourceNum = RSRC_DELETE_SUCCESSFUL;
         }

   // SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,iBitmapResourceNum,1000,0);
     DisplayDeleteResultBitmap( iBitmapResourceNum );
	// clear display
 	clear_display();

	SysWaitOnEvent(0,0,0);

    return iError;
}
