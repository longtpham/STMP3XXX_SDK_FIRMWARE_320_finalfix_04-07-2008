#include "types.h"
#include "resource.h"
#include "messages.h"
#include "exec.h"
#include "menumanager.h"
#include "menus.h"
#include "display.h"
#include "buttons.h"
#include "musiclib_ghdr.h"
#include "browsemenu.h"
#include "font.h"

#define _PLAY_ALL_IN_BROWSE_
extern int24 g_iBrowseTotalNum;
// Description: 
// Inputs:      
// Outputs:     
// Notes:      Defect 12440 solved aug31 2006 in this function for sdk3.120 in development. 
//
int _reentrant on_the_fly_submenu(INT display_base_message, INT confirm_insturction, INT *insturction_parameter_arg)
{
	union DisplayHints DisplayHint;
	BOOL bDone = FALSE;
	INT iEvent;
	INT del_status = 0;
	BOOL bHoldSelect = TRUE;
	BOOL bSelectChanged = TRUE;
	int a;
	INT insturction_parameter = *insturction_parameter_arg;

	
	while (!bDone)
	{
        // 2 lines that were here are moved to right before while loop to avoid hang when adding fav while playing mp3. stmp12440
        
        if(bSelectChanged == TRUE)
        {   
        	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,2,0,display_base_message);
            SysWaitOnEvent(0,0,0); // goes to kernel to give all other tasks a chance to run before returning here. 
            bSelectChanged = FALSE;
        }

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
#ifdef BACKLIGHT
			//UpdateBackLightTimer();
			//SysCallFunction(RSRC_BACKLIGHT_MENU_CODE_BANK,UpdateBacklightTimer,0,0,0);
#endif
			UpdateAutoShutdownTimer();
			DisplayHint.bits.Misc = TRUE;
			switch(gEventInfo.Button.wButtonEvent)
			{
			case PR_FF:
				SysPostMessage(4, confirm_insturction, insturction_parameter, &del_status);
				SysWaitOnEvent(0,0,0);				
				bDone = TRUE;
				break;
				
			case PR_RW:
				bDone = TRUE;
				break;
			}
		}
		if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC)
			bDone = TRUE;
	}
	/*
	if (a)	//select cancel
		return -1;
	else if (del_status == ON_THE_GO_DEL_SMALLER_ID)	//del song before playing one
		return 0;
	else if (del_status == ON_THE_GO_DEL_PLAYING)	//del playing song
		return 1;
	else 									//del song after playing one
		return 2;
	*/
}

int _reentrant display_year_list(int a, int b, int *pPtr)
{
	int i,string_width;
	int start_item;
	int y_pos;
	DISP_YEAR_LIST_ARG_T*  arg;

    int                 iHighLightedItem;
    FLASH_GROUP_NAME_T* browse_item_name;
    int                 num_of_item;
    int                 window_offset;
    BOOL                bHavePlayAll;
    
    a;b;
    arg = (DISP_YEAR_LIST_ARG_T*) pPtr;
    iHighLightedItem = arg->iHighLightedItem;
    browse_item_name = arg->browse_item_name;
    num_of_item      = arg->num_of_item;     
    window_offset    = arg->window_offset;   
    bHavePlayAll     = arg->bHavePlayAll;    

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
			//SysPostMessage(5,LCD_PRINT_RANGE_RSRC,MENU_ITEM_X_OFFSET,y_pos,RSRC_HIGHLIGHT_BACK);
			if (browse_item_name[i].name[0] != UNKNOWN_YEAR_CODE)
			{	SysPostMessage(7,LCD_PRINT_NUMBER_INV,MENU_ITEM_X_OFFSET,y_pos,browse_item_name[i].name[0],4,' ');
			    string_width = GetTextWidthResourceDBCS(RSRC_STRING_YEAR_WIDTH);
		    }else
			{	SysPostMessage(5,LCD_PRINT_STRING_INV_RSRC,MENU_ITEM_X_OFFSET,y_pos,RSRC_STRING_UNKNOWN_YEAR);
			    string_width = GetTextWidthResourceDBCS(RSRC_STRING_UNKNOWN_YEAR);
			}	
		}
		else
		{
			if (browse_item_name[i].name[0] != UNKNOWN_YEAR_CODE)
			{	SysPostMessage(7,LCD_PRINT_NUMBER,MENU_ITEM_X_OFFSET,y_pos,browse_item_name[i].name[0],4,' ');
			    string_width = GetTextWidthResourceDBCS(RSRC_STRING_YEAR_WIDTH);
			}else
			{	SysPostMessage(5,LCD_PRINT_STRING_RSRC,MENU_ITEM_X_OFFSET,y_pos,RSRC_STRING_UNKNOWN_YEAR);
			    string_width = GetTextWidthResourceDBCS(RSRC_STRING_UNKNOWN_YEAR);
            }
		}
        SysPostMessage(6,LCD_CLEAR_RANGE,MENU_ITEM_X_OFFSET+string_width,y_pos,LCD_X_SIZE-(MENU_ITEM_X_OFFSET+string_width),CHAR_SIZE_Y);				
	}
	return 0;
}

int _reentrant browsemenu_init(int a, int b, int *pPtr)
{
    INT     browse_type;
    INT     browse_lv;
    INT*    pbrowse_max_lv;
    INT*    num_of_item;
    BOOL*   bHaveNext;
    BROWSEMENU_INIT_ARG_T*  arg;
    
    a;b;
    arg = (BROWSEMENU_INIT_ARG_T*) pPtr;
    browse_type = arg->browse_type;
    browse_lv = arg->browse_lv;
    pbrowse_max_lv = arg->pbrowse_max_lv;
    num_of_item = arg->num_of_item;
    bHaveNext = arg->bHaveNext;

	switch (browse_type)
	{
	case ITEM_ARTIST:
		*pbrowse_max_lv = 2;
		break;
	case ITEM_ALBUM:
		*pbrowse_max_lv = 1;
		break;
	case ITEM_GENRE:
		*pbrowse_max_lv = 1;
		break;
	case ITEM_YEAR:
		*pbrowse_max_lv = 1;
		break;
#ifdef _FOLDER_BROWSE_
    case ITEM_INTERNAL:
    case ITEM_EXTERNAL:
        *pbrowse_max_lv = _MAX_DIR_DEPTH-1;
        break;
#endif  // _FOLDER_BROWSE_
	default:
		*pbrowse_max_lv = 0;
		break;
	}

 	SysPostMessage(3,MUSICLIB_BROWSE_SETROOT, (INT*)(num_of_item+browse_lv));
	SysWaitOnEvent(0,0,0);
	//if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO) && (browse_type != ITEM_MVIDEO))
	if ((browse_type != ITEM_VOICE) && (browse_type != ITEM_FMREC) && (browse_type != ITEM_PHOTO))
		SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, 0, (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv));
	else
		SysPostMessage(7,MUSICLIB_VOICE_BROWSEMENU, (INT*)(num_of_item+browse_lv), browse_type, 0, (INT*)(&g_iBrowseTotalNum), (INT*)(bHaveNext+browse_lv));

	SysWaitOnEvent(0,0,0);
	return 0;
}
