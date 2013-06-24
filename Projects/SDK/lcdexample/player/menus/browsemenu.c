#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "settingsmenu.h"
#include "browsemenu.h"
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
#include "playerlib.h"
#include "FileSystem.h" // for testing purpose
#include "musiclib_ghdr.h"
#include "sysequ.h"
#include "font.h"
#include "stringlib.h"
#include "playmodemenu.h"
#include "playlist3.h"

// Darwin 2005/8/10 11:52
#pragma optimize 1
// Darwin 2005/8/10 11:52

#define _PLAY_ALL_IN_BROWSE_

extern uint24		 	window_merge_index;
uint24           g_iBrowseTotalNum;


INT scrolling_offset;
BOOL g_bDeleteBrowse = FALSE;
BOOL bPlayVoice = FALSE;
BOOL bFavoritesUpdate = FALSE;
FLASH_GROUP_NAME_T	browse_item_name[BROWSE_WINDOW_SIZE];
uint24				merge_id_list_flash[BROWSE_WINDOW_SIZE];
uint24				merge_id_list_sd[BROWSE_WINDOW_SIZE];
INT	_X				g_iBrowseWindowSize = MENU_PAGE_ITEM_COUNT;	// range from 4 - 8

_reentrant void PSM_ResetScrollInfo(void);
INT _reentrant on_the_fly_submenu(INT display_base_message, INT confirm_insturction, INT *insturction_parameter_arg);
int _reentrant display_year_list(int a, int b, int *pPtr);
int _reentrant browsemenu_init(int a, int b, int *pPtr);

int _reentrant on_the_fly_submenu_wrapper(INT display_base_message, INT confirm_insturction, INT insturction_parameter)
{
	INT	insturction_parameter_arg = insturction_parameter;

	return (SysCallFunction(RSRC_BROWSE_MENU_EXTRA_CODE_BANK, on_the_fly_submenu, display_base_message, confirm_insturction, &insturction_parameter_arg));
}

int _reentrant display_year_list_wrapper(int iHighLightedItem, FLASH_GROUP_NAME_T* browse_item_name, int num_of_item, int window_offset, BOOL bHavePlayAll)
{
    DISP_YEAR_LIST_ARG_T    arg;

    arg.iHighLightedItem = iHighLightedItem;
    arg.browse_item_name = browse_item_name;
    arg.num_of_item      = num_of_item;
    arg.window_offset    = window_offset;
    arg.bHavePlayAll     = bHavePlayAll;

    return (SysCallFunction(RSRC_BROWSE_MENU_EXTRA_CODE_BANK, display_year_list, 0, 0, (int*)&arg));
}

int _reentrant display_browse_list(int iHighLightedItem, FLASH_GROUP_NAME_T* browse_item_name, int num_of_item, int window_offset, BOOL bHavePlayAll)
{
	int i,start_item;
	int x_pos,y_pos, string_width;
	int iResNo,iXPos;

	browse_item_name += window_offset;
#ifdef _PLAY_ALL_IN_BROWSE_
	if (bHavePlayAll)
	{
		browse_item_name--;
		num_of_item++;
		start_item = 1;

		if (iHighLightedItem)
			SysPostMessage(5,LCD_PRINT_STRING_RSRC,MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET,RSRC_STRING_PLAY_ALL);
		else
		{
			//SysPostMessage(5,LCD_PRINT_RANGE_RSRC,MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET,RSRC_HIGHLIGHT_BACK);
          	SysPostMessage(5,LCD_PRINT_STRING_INV_RSRC,MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET,RSRC_STRING_PLAY_ALL);
          	string_width = GetTextWidthResourceDBCS(RSRC_STRING_PLAY_ALL);
            SysPostMessage(6,LCD_CLEAR_RANGE,string_width,MENU_ITEM_Y_OFFSET,LCD_X_SIZE-string_width,CHAR_SIZE_Y);
		}
	}
	else
#endif
		start_item = 0;

	if (num_of_item > MENU_PAGE_ITEM_COUNT)
		num_of_item = MENU_PAGE_ITEM_COUNT;

	for (i=start_item; i<num_of_item; i++)
	{
		y_pos = MENU_ITEM_Y_OFFSET+MENU_ITEM_HEIGHT*i;
		if (i==iHighLightedItem)
		{
			x_pos = MENU_ITEM_X_OFFSET;
			//SysPostMessage(5,LCD_PRINT_RANGE_RSRC,MENU_ITEM_X_OFFSET,y_pos,RSRC_HIGHLIGHT_BACK);
			if (browse_item_name[i].unicode == 1) //unicode
			{
				string_width = GetTextWidthAddressUnicode(browse_item_name[i].name);
				if ((string_width > LCD_SIZE_X - MENU_ITEM_X_OFFSET) && (scrolling_offset > 0))
					x_pos -= scrolling_offset;				
				//SysPostMessage(5,LCD_PRINT_STRING_UNICODE_INV_ADDR,x_pos,y_pos,browse_item_name[i].name);
				iResNo = LCD_PRINT_STRING_UNICODE_INV_ADDR; iXPos = x_pos;
			}
			else
			{
				string_width = GetTextWidthAddressDBCS((_packed BYTE *)browse_item_name[i].name);
				if ((string_width > LCD_SIZE_X - MENU_ITEM_X_OFFSET) && (scrolling_offset > 0))
					x_pos -= scrolling_offset;
				//SysPostMessage(5,LCD_PRINT_STRING_INV_ADDR,x_pos,y_pos,browse_item_name[i].name);
				iResNo = LCD_PRINT_STRING_INV_ADDR; iXPos = x_pos;
			}
            if (x_pos < -string_width)
                scrolling_offset = -5;
		}
		else
		{
			if (browse_item_name[i].unicode == 1) //unicode
			{
				string_width = GetTextWidthAddressUnicode(browse_item_name[i].name);
			    //SysPostMessage(5,LCD_PRINT_STRING_UNICODE_ADDR,MENU_ITEM_X_OFFSET,y_pos,browse_item_name[i].name);
			    iResNo = LCD_PRINT_STRING_UNICODE_ADDR; iXPos = MENU_ITEM_X_OFFSET;
			}else
			{
				string_width = GetTextWidthAddressDBCS((_packed BYTE *)browse_item_name[i].name);
			    //SysPostMessage(5,LCD_PRINT_STRING_ADDR,MENU_ITEM_X_OFFSET,y_pos,browse_item_name[i].name);
			    iResNo = LCD_PRINT_STRING_ADDR; iXPos = MENU_ITEM_X_OFFSET;
            	}
            x_pos = MENU_ITEM_X_OFFSET;
		}
		SysPostMessage(5,iResNo,iXPos,y_pos,browse_item_name[i].name);
        	SysPostMessage(6,LCD_CLEAR_RANGE,x_pos+string_width,y_pos,LCD_X_SIZE-(x_pos+string_width),CHAR_SIZE_Y);
	}
	return 0;
}

void _reentrant browsemenu_down(INT browse_type, INT browse_lv, INT* iHighLightedItem, INT* num_of_item, INT* window_offset, BOOL* bHavePrev, BOOL* bHaveNext, BOOL* bHavePlayAll)
{
	INT current_highlight;
	BOOL bWrapAround = FALSE;

	//case of wrap around
	if ((!bHaveNext[browse_lv]) &&
		( (bHavePlayAll[browse_lv]   && (iHighLightedItem[browse_lv] == num_of_item[browse_lv])) ||
		((!bHavePlayAll[browse_lv]) && (iHighLightedItem[browse_lv] == num_of_item[browse_lv]-1)) ))
		bWrapAround = TRUE;
	else if (iHighLightedItem[browse_lv] < MENU_PAGE_ITEM_COUNT-1)
		iHighLightedItem[browse_lv]++;
	else if (bHavePlayAll[browse_lv] && (num_of_item[browse_lv] >= MENU_PAGE_ITEM_COUNT))
		bHavePlayAll[browse_lv] = FALSE;
	else if (bHaveNext[browse_lv])
	{
		//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
        if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
			SysPostMessage(4,MUSICLIB_BROWSE_DOWNLIST, 1, (INT*)(bHaveNext+browse_lv));
		else
			SysPostMessage(4,MUSICLIB_VOICE_DOWNLIST, 1, (INT*)(bHaveNext+browse_lv));

		SysWaitOnEvent(0,0,0);
		bHavePrev[browse_lv] = TRUE;
	}
	else
		iHighLightedItem[browse_lv]++;
	if ((bWrapAround) || (iHighLightedItem[browse_lv] >= MENU_PAGE_ITEM_COUNT))
	{
		current_highlight = iHighLightedItem[browse_lv] + window_offset[browse_lv];
		if (bHavePlayAll[browse_lv])
			current_highlight--;

		if ((current_highlight < num_of_item[browse_lv]) && (window_offset[browse_lv] < num_of_item[browse_lv] - MENU_PAGE_ITEM_COUNT))
		{
			if (bHavePlayAll[browse_lv])
				bHavePlayAll[browse_lv] = FALSE;
			else
			{
				window_offset[browse_lv]++;
				iHighLightedItem[browse_lv] = MENU_PAGE_ITEM_COUNT - 1;
			}
		}
		else
		{
			//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
			if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
				SysPostMessage(6,MUSICLIB_BROWSE_UPMENU, (INT*)(num_of_item+browse_lv), browse_type, iHighLightedItem[browse_lv], (INT*)(&g_iBrowseTotalNum));
			else
				SysPostMessage(6,MUSICLIB_VOICE_UPMENU, (INT*)(num_of_item+browse_lv), browse_type, iHighLightedItem[browse_lv], (INT*)(&g_iBrowseTotalNum));

			SysWaitOnEvent(0,0,0);
			if (browse_lv == 0)
				current_highlight = 0;
			else
			{
				current_highlight = iHighLightedItem[browse_lv-1];
				if (bHavePlayAll[browse_lv-1])
					current_highlight--;
				current_highlight += window_offset[browse_lv-1];
			}
			//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
			if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
				SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, current_highlight, (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv));
			else
				SysPostMessage(7,MUSICLIB_VOICE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, current_highlight, (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv));

			SysWaitOnEvent(0,0,0);

			iHighLightedItem[browse_lv] = 0;
			window_offset[browse_lv] = 0;
			bHavePrev[browse_lv] = FALSE;
#ifdef _PLAY_ALL_IN_BROWSE_
			//if ((browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO) && (!g_bDeleteBrowse))
			if ((browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (!g_bDeleteBrowse))
				bHavePlayAll[browse_lv] = TRUE;
			else
#endif
				bHavePlayAll[browse_lv] = FALSE;
		}
	}
}


void _reentrant browsemenu_up(INT browse_type, INT browse_lv, INT* iHighLightedItem, INT* num_of_item, INT* window_offset, BOOL* bHavePrev, BOOL* bHaveNext, BOOL* bHavePlayAll)
{
	if ((window_offset[browse_lv]) && (iHighLightedItem[browse_lv] == 0))
		window_offset[browse_lv]--;
	else
#ifdef _PLAY_ALL_IN_BROWSE_
		//case of shown "*Play All"
		//if ((!bHavePlayAll[browse_lv]) && (browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO) && (!g_bDeleteBrowse) && (!bHavePrev[browse_lv]) && (iHighLightedItem[browse_lv] == 0))
		if ((!bHavePlayAll[browse_lv]) && (browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (!g_bDeleteBrowse) && (!bHavePrev[browse_lv]) && (iHighLightedItem[browse_lv] == 0))
			bHavePlayAll[browse_lv] = TRUE;
		else
#endif
			//case of not highlight the first item, highlight move up
			if (iHighLightedItem[browse_lv] > 0)
				iHighLightedItem[browse_lv]--;
			//case of uplist (not wrap around)
			else if (bHavePrev[browse_lv])
			{
				//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
				if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
					SysPostMessage(5,MUSICLIB_BROWSE_UPLIST, 1, (INT*)(bHavePrev+browse_lv), (INT*)(num_of_item+browse_lv));
				else
					SysPostMessage(5,MUSICLIB_VOICE_UPLIST, 1, (INT*)(bHavePrev+browse_lv), (INT*)(num_of_item+browse_lv));

				SysWaitOnEvent(0,0,0);
				bHaveNext[browse_lv] = TRUE;
			}
			else
			{
				if (bHaveNext[browse_lv])
				{
					//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
					if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
						SysPostMessage(5,MUSICLIB_BROWSE_UPLIST, 1, (INT*)(bHavePrev+browse_lv), (INT*)(num_of_item+browse_lv));
					else
						SysPostMessage(5,MUSICLIB_VOICE_UPLIST, 1, (INT*)(bHavePrev+browse_lv), (INT*)(num_of_item+browse_lv));

					SysWaitOnEvent(0,0,0);
				}
				bHaveNext[browse_lv] = FALSE;
				if (num_of_item[browse_lv] >= MENU_PAGE_ITEM_COUNT)
					bHavePlayAll[browse_lv] = FALSE;
				if (bHavePlayAll[browse_lv])
					iHighLightedItem[browse_lv] = num_of_item[browse_lv];
				else
					iHighLightedItem[browse_lv] = num_of_item[browse_lv]-1;

				if (iHighLightedItem[browse_lv] >= MENU_PAGE_ITEM_COUNT)
					iHighLightedItem[browse_lv] = MENU_PAGE_ITEM_COUNT - 1;

				window_offset[browse_lv] = num_of_item[browse_lv] - MENU_PAGE_ITEM_COUNT;
				if (window_offset[browse_lv] < 0)
					window_offset[browse_lv] = 0;
			}
}

#ifdef _FOLDER_BROWSE_
_reentrant BOOL BrowseMenu_IsDirItem(INT iIndex, INT iLevel)
{
    if (iIndex < g_u24BrowseNumOfDirInDir[iLevel])
        return TRUE;
    return FALSE;
}
#endif  // _FOLDER_BROWSE_

int _reentrant BrowseMenu(int a, int b, int *pPtr)
{
	BOOL bDone = FALSE;
	BOOL bSendToPlayerStateMachine = TRUE;

	union DisplayHints DisplayHint;
	INT iEvent;
	INT iTitleResource = b;
	INT browse_type = a;
	INT browse_max_lv = 0;
	INT browse_lv = 0;
	INT status = 0;

    INT iHighLightedItem[BROWSE_MAX_LEVEL];
    INT num_of_item[BROWSE_MAX_LEVEL];
    INT window_offset[BROWSE_MAX_LEVEL];

    BOOL bHavePrev[BROWSE_MAX_LEVEL];
    BOOL bHaveNext[BROWSE_MAX_LEVEL];
    BOOL bHavePlayAll[BROWSE_MAX_LEVEL];

	BOOL bDisplayEmpty = FALSE;
	INT hold_count = 0;
	BOOL bSizeChanged = FALSE;
	BOOL bDeleteFavourite;
    BOOL bIsDirItem;
    BOOL bHavePlayAllCurLvl;
    INT  iHighLightedItemCurLvl;

	BROWSEMENU_INIT_ARG_T   tBrowseinitArg;

	iHighLightedItem[0] = 0;
	window_offset[0] = 0;
	bHavePrev[0] = FALSE;
#ifdef _PLAY_ALL_IN_BROWSE_
	//if ((browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO) && (!g_bDeleteBrowse))
	if ((browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (!g_bDeleteBrowse))
		bHavePlayAll[0] = TRUE;
	else
#endif
		bHavePlayAll[0] = FALSE;

	///////////////////////////////////////////////////////////////////////////
	//These data structures are used by the DisplayPagedList function to create the
	//the main menu lists.  see displaylists.h for more info
	///////////////////////////////////////////////////////////////////////////

	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
	SysWaitOnEvent(0,0,0);
	scrolling_offset = -5;

	// this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;
	//ML_browsing_app_init();

    tBrowseinitArg.browse_type = browse_type;
    tBrowseinitArg.browse_lv = browse_lv;
    tBrowseinitArg.pbrowse_max_lv = &browse_max_lv;
    tBrowseinitArg.num_of_item = num_of_item;
    tBrowseinitArg.bHaveNext = bHaveNext;
    SysCallFunction(RSRC_BROWSE_MENU_EXTRA_CODE_BANK, browsemenu_init, 0, 0, (int*)&tBrowseinitArg);
    
 	//SysPostMessage(3,MUSICLIB_BROWSE_SETROOT, (INT*)(num_of_item+browse_lv));
	//SysWaitOnEvent(0,0,0);
	//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
	//	SysPostMessage(6,MUSICLIB_BROWSE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, 0, (INT*)(bHaveNext+browse_lv));
	//else
	//	SysPostMessage(6,MUSICLIB_VOICE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, 0, (INT*)(bHaveNext+browse_lv));
    //
	//SysWaitOnEvent(0,0,0);

	//we need to refresh the screen since entering the menu
	DisplayHint.I = BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

	while(!bDone)
	{
		if (num_of_item[browse_lv] <= 0)
		{
			//if ((browse_type == ITEM_ON_THE_GO) || (browse_type == ITEM_VOICE) || (browse_type == ITEM_FMREC) || (browse_type == ITEM_PHOTO) || (browse_type == ITEM_MVIDEO))
            if ((browse_type == ITEM_ON_THE_GO) || (browse_type == ITEM_VOICE) || (browse_type == ITEM_FMREC) || (browse_type == ITEM_PHOTO))
			{
				bDisplayEmpty = TRUE;
				bHavePlayAll[0] = FALSE;
			}
			else
			{
				SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
				b = GetTextWidthResourceUnicode(RSRC_STRING_NO_FILES);
				a = (LCD_X_SIZE-b)/2;
				if (a < SONG_TITLE_X_POS)
					a = SONG_TITLE_X_POS;
				else if (a + b > LCD_X_SIZE - SONG_TITLE_X_POS)
					a = SONG_TITLE_X_POS + LCD_X_SIZE - b;
				SysPostMessage(5,LCD_PRINT_STRING_RSRC,a,SONG_TITLE_Y_POS,RSRC_STRING_NO_FILES);
				bDone = TRUE;
				SysWaitOnEvent(0,0,2000);
				if (g_bDeleteBrowse)
					g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                else if(browse_type == ITEM_AUDIBLE)    //STMP00013551
                    g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
				break;
			}
		}

		//mask out all display hints that aren't aimed toward the restore default menu
		DisplayHint.I &= (BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

		//wait until a message or button happens.
		iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);

		bSendToPlayerStateMachine = TRUE;

		//handle the event
		switch(iEvent)
		{
		case EVENT_TIMER:
			SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,iTitleResource+g_iLanguage);

			//display_empty
			if (bDisplayEmpty)
			{
				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,MENU_ITEM_X_OFFSET,MENU_ITEM_Y_OFFSET,RSRC_EMPTY_FAVOURITE);
			}
			else
			{
				if ((browse_type == ITEM_YEAR) && (browse_lv == 0))
					display_year_list_wrapper(iHighLightedItem[browse_lv], browse_item_name, num_of_item[browse_lv], window_offset[browse_lv], bHavePlayAll[browse_lv]);
				else
					display_browse_list(iHighLightedItem[browse_lv], browse_item_name, num_of_item[browse_lv], window_offset[browse_lv], bHavePlayAll[browse_lv]);
				SysWaitOnEvent(0,0,0);
			}
			DisplayHint.I = 0;//clear out the bits
			break;
		case EVENT_MESSAGE:
			switch (gEventInfo.Message.wMsgCommand)
			{
			case MENU_MSG_MEDIA_CHANGED:
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
				bDone = TRUE;
				break;
			case MENU_MSG_ANIMATE:
				scrolling_offset++;
				DisplayHint.bits.Misc = TRUE;
				bSendToPlayerStateMachine = FALSE;
				break;
			}
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
			bSendToPlayerStateMachine = FALSE;
			scrolling_offset = -5;

			switch(gEventInfo.Button.wButtonEvent)
			{
			case PR_RECORD:
				if ((browse_type == ITEM_VOICE) && (!g_bDeleteBrowse))
				{
					g_MenuFlags.MENU_FLAG_ESCAPE_TO_RECORD = TRUE;
					bDone = TRUE;
				}
				break;
				
			case PR_RV:
				if (!bDisplayEmpty)
				{
					browsemenu_down(browse_type, browse_lv, iHighLightedItem, num_of_item, window_offset, bHavePrev, bHaveNext, bHavePlayAll);
					DisplayHint.bits.Misc = TRUE;
				}
				break;

			case PR_MENU:
				if (!bDisplayEmpty)
				{
					browsemenu_up(browse_type, browse_lv, iHighLightedItem, num_of_item, window_offset, bHavePrev, bHaveNext, bHavePlayAll);
					DisplayHint.bits.Misc = TRUE;
				}
				break;

			//display OnTheGo menu
			case PR_FF:
			//if ((browse_type != ITEM_FMREC) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
                	if ((browse_type != ITEM_FMREC) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_PHOTO))
                	{
#ifdef _FOLDER_BROWSE_
                    	if ((browse_type == ITEM_INTERNAL) || (browse_type == ITEM_EXTERNAL))
                    	{
#ifdef _PLAY_ALL_IN_BROWSE_
                        if (bHavePlayAll[browse_lv])
                            bIsDirItem = BrowseMenu_IsDirItem((window_merge_index+iHighLightedItem[browse_lv]-1),browse_lv);
                        else
#endif
                            bIsDirItem = BrowseMenu_IsDirItem((window_merge_index+iHighLightedItem[browse_lv]),browse_lv);
                    	}
                    	else
#endif  // _FOLDER_BROWSE_
                    {
                        if ((!bDisplayEmpty) && (browse_lv == browse_max_lv) && (!g_bDeleteBrowse) &&
					       	((!bHavePlayAll[browse_lv]) || (iHighLightedItem[browse_lv] != 0)))			 	    
                            bIsDirItem = FALSE;
                        else
                            bIsDirItem = TRUE;
                    }
                    
					if (!bIsDirItem)
					{
#if 0 //if there is animate timer
						SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
#endif
						SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
						SysWaitOnEvent(0,0,0);
						bDeleteFavourite = FALSE;
						if (browse_type == ITEM_ON_THE_GO)
							bDeleteFavourite = TRUE;
						else
						{
							//post message to check on the go list
							//prompt 1)full 2)already in the list 3)ask if add
#ifdef _PLAY_ALL_IN_BROWSE_
							if (bHavePlayAll[browse_lv])
								SysPostMessage(4, MUSICLIB_BROWSE_ON_THE_FLY_GET_STATUS, (INT)iHighLightedItem[browse_lv]-1, (INT*)&status);
							else
#endif
								SysPostMessage(4, MUSICLIB_BROWSE_ON_THE_FLY_GET_STATUS, (INT)iHighLightedItem[browse_lv], (INT*)&status);
							SysWaitOnEvent(0,0,0);

							if (status == ON_THE_GO_FREE)
							{
								//ask if save
					 			on_the_fly_submenu_wrapper(RSRC_FAVOURITES_LIST_ADD+g_iLanguage, MUSICLIB_BROWSE_ON_THE_FLY_ADD_ITEM, 0);
								if (IsPlayOnTheGo)
									bSizeChanged = TRUE;
								bFavoritesUpdate = TRUE;
								hold_count = 0;
					 			DisplayHint.I = BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
							}
							else
							{
								if (status == ON_THE_GO_EXIST)
									bDeleteFavourite = TRUE;
								//SysPostMessage(5,LCD_PRINT_RANGE_RSRC,2,0,RSRC_LIST_EXIST_MESSAGE);
								else //if (status == ON_THE_GO_FULL)
								{
									SysPostMessage(5,LCD_PRINT_RANGE_RSRC,2,0,RSRC_FAVOURITES_LIST_IS_FULL+g_iLanguage);
									SysWaitOnEvent(0,0,1000);
								}
							}
						}

						if (bDeleteFavourite)
						{
#ifdef _PLAY_ALL_IN_BROWSE_
							if (bHavePlayAll[browse_lv])
					 			status = on_the_fly_submenu_wrapper(RSRC_FAVOURITES_LIST_REMOVE+g_iLanguage, MUSICLIB_BROWSE_ON_THE_FLY_DELETE_ITEM, iHighLightedItem[browse_lv]-1);
							else
#endif
			 					status = on_the_fly_submenu_wrapper(RSRC_FAVOURITES_LIST_REMOVE+g_iLanguage, MUSICLIB_BROWSE_ON_THE_FLY_DELETE_ITEM, iHighLightedItem[browse_lv]);
                            DisplayHint.I = BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
							if (status>=0)
							{
								//update display only if browsing "Favourite"
								if (browse_type == ITEM_ON_THE_GO)
								{
									iHighLightedItem[0] = 0;
									bHavePrev[0] = FALSE;
#ifdef _PLAY_ALL_IN_BROWSE_
									//if ((browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO) && (!g_bDeleteBrowse))
									if ((browse_type != ITEM_AUDIBLE) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (!g_bDeleteBrowse))
										bHavePlayAll[0] = TRUE;
									else
#endif
										bHavePlayAll[0] = FALSE;

									SysPostMessage(3,MUSICLIB_BROWSE_SETROOT, (INT*)(num_of_item+browse_lv));
									SysWaitOnEvent(0,0,0);
									//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
									if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
										SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, 0, (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv));
									else
										SysPostMessage(7,MUSICLIB_VOICE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, 0, (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv));
									SysWaitOnEvent(0,0,0);
								}
								if (IsPlayOnTheGo)
								{
									bSizeChanged = TRUE;
									if (g_iShuffleSetting)
									{
										if (status > 0)
											SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_NEXT_SONG,&g_current_index);
										SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_PREVIOUS_SONG,&g_current_index);
									}
									else
									{
										if (status > 0)
											SysPostMessage(3,MUSICLIB_PLAYNEXTSONG,&g_current_index);
										SysPostMessage(3,MUSICLIB_PLAYPREVIOUSSONG,&g_current_index);
									}
									SysWaitOnEvent(0,0,0);
									if (status==1) //delete playing song, so update the playback
									{
										//SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,(INT*)(DirPath),0,0);
										SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));

										SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PSM_ResetScrollInfo,0,0,0);
										SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
										SysPostMessage(2,MENU_SONG_CHANGE_TIMEOUT);
										//g_iPlayerState = DECODER_PLAYING;
									}
								}
								bFavoritesUpdate = TRUE;
							}
							hold_count = 0;
						}
						if(bFavoritesUpdate)
						{
       					 	int button = PR_STOP;
        						SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,EVENT_BUTTON,0,&button);	
						}
						if ((bFavoritesUpdate) && (g_wDecoderSR & DECODER_STOPPED) && (g_ML_save_on_exit == 1))
						{
							SysCallFunction(RSRC_MAIN_MENU_CODE_BANK, ML_building_engine_init,0,0,0);
							SysLoadFATWrite();
							ML_SaveOnTheGo();
							SysUnLoadFATWrite();
							SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);
							bFavoritesUpdate = FALSE;
						}

						if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC)
							bDone = TRUE;
						DisplayHint.bits.Misc = TRUE;
#if 0 //if there is animate timer
						SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
						SysWaitOnEvent(0,0,0);
#endif
					}
				}
				break;

			//Play current song
			case PR_PLAY:
				if (hold_count)
				{
					hold_count = 0;
					break;
				}
				
			case PR_VOL_DOWN:
				if (!bDisplayEmpty)
				{
#ifdef _PLAY_ALL_IN_BROWSE_
					if ((bHavePlayAll[browse_lv]) && (iHighLightedItem[browse_lv] == 0))
					{
						g_current_index = 0;
						switch (browse_type)
						{
						case ITEM_1DAY:
						case ITEM_1WEEK:
						case ITEM_1MONTH:
						case ITEM_ON_THE_GO:
						case ITEM_AUDIBLE:
						case ITEM_VOICE:
						case ITEM_FMREC:
						case ITEM_PHOTO:
						//case ITEM_MVIDEO:
							SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
							SysCallFunction(RSRC_METADATA_CODEBANK,GetRelativePath,0,browse_type,(INT*)(DirPath));
							SysWaitOnEvent(0,0,0);
							break;
						default:
							SysPostMessage(4,MUSICLIB_START_PLAY_ALL_SONG, &g_current_size, (INT)(DirPath));
							SysWaitOnEvent(0,0,0);
							//SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,(INT*)(DirPath),0,0);
							SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));
							bPlayVoice = FALSE;
							break;
						}
						g_wTotalSongCount = g_current_size;
						SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PSM_ResetScrollInfo,0,0,0);

						if (g_iShuffleSetting == SHUFFLE_ON)
						{
							SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_NEXT_SONG,&g_shuffle_start);
							SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_PREVIOUS_SONG,&g_shuffle_start);
							SysWaitOnEvent(0,0,0);
						}
						SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
						//need to prepare a valid path before posting message - MENU_SONG_CHANGE_TIMEOUT
						// the timeout time is modified by Raymond
						SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_SONG_CHANGE,1,100,MENU_SONG_CHANGE_TIMEOUT);
						g_iPlayerState = DECODER_PLAYING;

						DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything

						// set flag so PR_MENU in music mode will ignore
						g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
						bDone = TRUE;
						bSizeChanged = FALSE;
					}
					else
#endif
                    {
#ifdef _FOLDER_BROWSE_
                        if ((browse_type == ITEM_INTERNAL) || (browse_type == ITEM_EXTERNAL))
                        {
#ifdef _PLAY_ALL_IN_BROWSE_
                            if (bHavePlayAll[browse_lv])
                                bIsDirItem = BrowseMenu_IsDirItem((window_merge_index+iHighLightedItem[browse_lv]-1),browse_lv);
                            else
#endif
                                bIsDirItem = BrowseMenu_IsDirItem((window_merge_index+iHighLightedItem[browse_lv]),browse_lv);
                        }
                        else
#endif  // _FOLDER_BROWSE_
                        {
						if (browse_lv < browse_max_lv)
                                bIsDirItem = TRUE;
                            else
                                bIsDirItem = FALSE;
                        }

                        if (bIsDirItem)
						{
#ifdef _PLAY_ALL_IN_BROWSE_
							if (bHavePlayAll[browse_lv])
								SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)(num_of_item+browse_lv+1), browse_type, iHighLightedItem[browse_lv]+window_offset[browse_lv]-1, (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv+1));
							else
#endif
								SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)(num_of_item+browse_lv+1), browse_type, iHighLightedItem[browse_lv]+window_offset[browse_lv], (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv+1));
							browse_lv++;
							SysWaitOnEvent(0,0,0);
#ifdef _FOLDER_BROWSE_
                            if (browse_type == ITEM_INTERNAL)
                            {
                                iTitleResource = RSRC_BROWSE_MUSIC_FOLDER_INTERNAL;
                            }
                            else if (browse_type == ITEM_EXTERNAL)
                            {
                                iTitleResource = RSRC_BROWSE_MUSIC_FOLDER_EXTERNAL;
                            }
                            else
#endif  // _FOLDER_BROWSE_
                            {
    							if (browse_lv == browse_max_lv)
	       							iTitleResource = RSRC_STRING_SONGS;
		      					else if (browse_type == ITEM_ARTIST)
			     					iTitleResource = RSRC_STRING_ALBUM;
                            }
							iHighLightedItem[browse_lv] = 0;
							window_offset[browse_lv] = 0;
#ifdef _PLAY_ALL_IN_BROWSE_
							bHavePlayAll[browse_lv] = TRUE;
#else
							bHavePlayAll[browse_lv] = FALSE;
#endif
							bHavePrev[browse_lv] = FALSE;
							DisplayHint.bits.Misc = TRUE;
							DisplayHint.bits.ClearDisplay = TRUE;
						}
						else
						{
							SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
							//play song
							//iHighLightedItemCurLvl = iHighLightedItem[browse_max_lv];
#ifdef _PLAY_ALL_IN_BROWSE_
#ifdef _FOLDER_BROWSE_
                            if ((browse_type == ITEM_INTERNAL) || (browse_type == ITEM_EXTERNAL))
                            {
                                bHavePlayAllCurLvl = bHavePlayAll[browse_lv];
                                iHighLightedItemCurLvl = iHighLightedItem[browse_lv];
                            }
                            else
#endif  // _FOLDER_BROWSE_
                            {
                                bHavePlayAllCurLvl = bHavePlayAll[browse_max_lv];
                                iHighLightedItemCurLvl = iHighLightedItem[browse_max_lv];
                            }
#else   // _PLAY_ALL_IN_BROWSE_
#ifdef _FOLDER_BROWSE_
                            if ((browse_type == ITEM_INTERNAL) || (browse_type == ITEM_EXTERNAL))
                            {
                                iHighLightedItemCurLvl = iHighLightedItem[browse_lv];
                            }
                            else
#endif  // _FOLDER_BROWSE_
                                iHighLightedItemCurLvl = iHighLightedItem[browse_max_lv];
#endif  // _PLAY_ALL_IN_BROWSE_
#ifdef _PLAY_ALL_IN_BROWSE_
							if (bHavePlayAllCurLvl)
								SysCallFunction(RSRC_METADATA_CODEBANK,GetRelativePath,iHighLightedItemCurLvl+window_offset[browse_lv]-1,browse_type,(INT*)(DirPath));
							else
#endif
								SysCallFunction(RSRC_METADATA_CODEBANK,GetRelativePath,iHighLightedItemCurLvl+window_offset[browse_lv],browse_type,(INT*)(DirPath));

							SysWaitOnEvent(0,0,0);
							//siuman
							packed_strcpy(g_CurrentSongName, DirPath);
							g_wTotalSongCount = g_current_size;

							SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PSM_ResetScrollInfo,0,0,0);

							//if ((g_iShuffleSetting == SHUFFLE_ON) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO) && (!g_bDeleteBrowse))
							if ((g_iShuffleSetting == SHUFFLE_ON) && (browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (!g_bDeleteBrowse))
							{
								SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_NEXT_SONG,&g_shuffle_start);
								SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_PREVIOUS_SONG,&g_shuffle_start);
								SysWaitOnEvent(0,0,0);
							}

							//need to prepare a valid path before posting message - MENU_SONG_CHANGE_TIMEOUT
							// the timeout time is modified by Raymond
							if (!g_bDeleteBrowse)
							{
								SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_SONG_CHANGE,1,100,MENU_SONG_CHANGE_TIMEOUT);
								g_iPlayerState = DECODER_PLAYING;
								// set flag so PR_MENU in music mode will ignore
								g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
							}
							bDone = TRUE;
							bSizeChanged = FALSE;
							DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
						}
					}
				}
				break;

			//Previous Menu
			case PR_RW:
				if (browse_lv == 0)
				{
					if (g_bDeleteBrowse)
						g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
					bDone = TRUE;
				}
				else
				{
					browse_lv--;
					//voice only have one level, won't go into this case
                    SysPostMessage(6,MUSICLIB_BROWSE_UPMENU, (INT*)(num_of_item+browse_lv), browse_type, iHighLightedItem[browse_lv], (INT*)(&g_iBrowseTotalNum));

					SysWaitOnEvent(0,0,0);
#ifdef _FOLDER_BROWSE_
                    if (browse_type == ITEM_INTERNAL)
                    {
                        iTitleResource = RSRC_BROWSE_MUSIC_FOLDER_INTERNAL;
                    }
                    else if (browse_type == ITEM_EXTERNAL)
                    {
                        iTitleResource = RSRC_BROWSE_MUSIC_FOLDER_EXTERNAL;
                    }
                    else
#endif  // _FOLDER_BROWSE_
                    {
			     		if (browse_lv == 1)
		      				iTitleResource = RSRC_STRING_ALBUM;
	       				else
    					{
					       	switch (browse_type)
				    		{
			     			case ITEM_ARTIST:
		      					iTitleResource = RSRC_STRING_ARTIST;
	       						break;
    						case ITEM_ALBUM:
					       		iTitleResource = RSRC_STRING_ALBUM;
				    			break;
			     			case ITEM_GENRE:
		      					iTitleResource = RSRC_STRING_GENRE;
	       						break;
    						case ITEM_YEAR:
						      	iTitleResource = RSRC_STRING_YEAR;
					       		break;
				    		case ITEM_TRACK:
			     			default:
		      					break;
	       					}
    					}
                    }
					DisplayHint.bits.Misc = TRUE;
					DisplayHint.bits.ClearDisplay = TRUE;
				}
				break;
				
			case PH_MENU:
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
				bDone = TRUE;
				break;

			case PH_PLAY:
			case PH_STOP:
				//if the STOP button is held down, lets call the shutdown menu
				SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
				// when returns update display  -- if doesn't return, powered down
				DisplayHint.I |= (BROWSE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
				break;
				
			default:
				//DisplayHint.bits.Misc = TRUE;
				break;
			}
		} // end switch(iEvent)
		if(bSendToPlayerStateMachine)
			DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
		if ((g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC) || (g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC) || (g_MenuFlags.MENU_FLAG_ESCAPE_TO_RECORD))
			bDone = TRUE;
	} // end while (bDone)
	if (bSizeChanged)
	{
		SysPostMessage(3,MUSICLIB_PLAY_CURRENT_SIZE, (INT*)(&g_current_size));
		SysWaitOnEvent(0,0,0);
		if (g_current_size == 0)
		{
			SysPostMessage(3,MUSICLIB_BROWSE_SETROOT, (INT*)&g_current_size);
			SysWaitOnEvent(0,0,0);
			SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)&g_current_size, ITEM_TRACK, 0, (INT*)(&g_iBrowseTotalNum), (INT*)&g_current_index);
			SysWaitOnEvent(0,0,0);
			SysCallFunction(RSRC_METADATA_CODEBANK,GetRelativePath,0,browse_type,(INT*)(DirPath));
		}
	}
//	SysPostMessage(3,LCD_SET_FONT,0);
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
	SysWaitOnEvent(0,0,0);
	return 0;
}

#pragma asm
    nolist
    include "resource.inc"
    include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:FbPlayVoice

    SaveRange FbPlayVoice,1

#pragma endasm
