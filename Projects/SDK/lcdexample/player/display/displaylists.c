#include "types.h"
#include "exec.h"
#include "messages.h"
#include "resource.h"
#include "displaylists.h"
#ifdef CLCD
#include "displaydrv_color.h"
#include "font.h"
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int DisplayPagedList(int iSelectedItem, int iIgnored, void*ListStructure)
//
//   Type:          Function (code bank callable)
//
//   Description:   This function displays a list of items, including the selected item.
//
//   Inputs:        iSelectedItem--the index of the item selected
//                  ListStructure--a structure (PagedListStructure) that contains the list of items,
//                          the item count, etc. 
//                  iTitle -- used to add titles : really should be part of structure, but changes 
// 						too much for backwards compatibility
//
//   Outputs:       Messages to the LCD module to display the list
//
//   Notes:         This can handle string and bitmap resources.
//                  From struct PagedListStructure *ListStructure, it uses the m_iNormalDisplayMsg 
//                  and m_iSelectedDisplayMsg to display the items pionted to by m_pItems.  It will
//                  display all the items on the same page (pItem[x]->m_iPage) as the iSelectedItem
//                  using the exact coordinates in the MenuItem structure.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant int DisplayPagedList(int iSelectedItem, int iTitle, void*ListStructure)
{
    struct PagedListStructure *pListStructure = ListStructure;
    struct MenuItem *pMenuItems;
    int iPageofSelectedItem;
    int iCount;
    int uMessage;
    int i;
#ifdef CLCD
    int x_pos;
    extern int prev_x_pos[];
#endif

    iCount = pListStructure->m_iItems;
    pMenuItems = pListStructure->m_pItems;

    if(iSelectedItem < iCount)
    {//make sure the selection is within the count.
#ifdef USE_PLAYLIST3
#ifdef JPEG_APP
		if((iTitle != BITMAP_RECORD_SETTINGS_TITLE_NUM) && (iTitle != BITMAP_SETTINGS_TITLE_NUM) && (iTitle != BITMAP_ERASE_TITLE_NUM) && (iTitle != BITMAP_JPEG_DISPLAY_TITLE_NUM) && (iTitle != 0))
#else
		if((iTitle != BITMAP_RECORD_SETTINGS_TITLE_NUM) && (iTitle != BITMAP_SETTINGS_TITLE_NUM) && (iTitle != BITMAP_ERASE_TITLE_NUM) && (iTitle != 0))
#endif
			SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
#ifdef JPEG_APP
		if(iTitle == BITMAP_JPEG_DISPLAY_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,JPEG_DISPLAY_TITLE_X_POS,JPEG_DISPLAY_TITLE_Y_POS,RSRC_JPEG_DISPLAY_TITLE /*placeholder,till we get bmp file*/ );
		else
#endif

		if(iTitle == BITMAP_SETTINGS_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SETTINGS_TITLE_X_POS,SETTINGS_TITLE_Y_POS,RSRC_SETTINGS_TITLE /*placeholder,till we get bmp file*/ );
		else if(iTitle == BITMAP_ERASE_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,ERASE_TITLE_X_POS,ERASE_TITLE_Y_POS,RSRC_ERASE_TITLE /*placeholder till we get bmp file*/ );
#if defined (TEST)
		else if(iTitle == BITMAP_TEST_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,ERASE_TITLE_X_POS,ERASE_TITLE_Y_POS,RSRC_TEST_TITLE);
#endif
		else if((iTitle != BITMAP_RECORD_SETTINGS_TITLE_NUM) && (iTitle != 0))
			SysPostMessage(5,LCD_PRINT_STRING_RSRC,0,0,iTitle);
#else	// #ifdef USE_PLAYLIST3
#ifdef JPEG_APP
		if(iTitle == BITMAP_JPEG_DISPLAY_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,JPEG_DISPLAY_TITLE_X_POS,JPEG_DISPLAY_TITLE_Y_POS,RSRC_JPEG_DISPLAY_TITLE /*placeholder,till we get bmp file*/ );
#endif
		if(iTitle == BITMAP_SETTINGS_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SETTINGS_TITLE_X_POS,SETTINGS_TITLE_Y_POS,RSRC_SETTINGS_TITLE /*placeholder,till we get bmp file*/ );

		if(iTitle == BITMAP_ERASE_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,ERASE_TITLE_X_POS,ERASE_TITLE_Y_POS,RSRC_ERASE_TITLE /*placeholder till we get bmp file*/ );
		
		#if defined (TEST)
		if(iTitle == BITMAP_TEST_TITLE_NUM)
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,ERASE_TITLE_X_POS,ERASE_TITLE_Y_POS,RSRC_TEST_TITLE);
		#endif
#endif	// #ifdef USE_PLAYLIST3
        
		iPageofSelectedItem = pMenuItems[iSelectedItem].m_iPage;
        //display that page
        for(i=0;i<iCount;i++)
        {
            if(iPageofSelectedItem == pMenuItems[i].m_iPage)
            {
                if(i==iSelectedItem)
                    uMessage = pListStructure->m_iSelectedDisplayMsg;
                else
                    uMessage = pListStructure->m_iNormalDisplayMsg;
                SysPostMessage(5,uMessage,pMenuItems[i].m_ixPos,
                                                   pMenuItems[i].m_iyPos,
                                                   pMenuItems[i].m_iResource);
#ifdef CLCD
                if(iTitle == BITMAP_RECORD_SETTINGS_TITLE_NUM)
                {   x_pos = pMenuItems[i].m_ixPos + GetTextWidthResourceDBCS(pMenuItems[i].m_iResource);
                    if(x_pos < prev_x_pos[i])
                        SysPostMessage(6,LCD_CLEAR_RANGE,x_pos,pMenuItems[i].m_iyPos,LCD_X_SIZE-x_pos,CHAR_SIZE_Y);
                    prev_x_pos[i] = x_pos;                        
                }
#endif
            }
        }
    }
    return 0;


}
