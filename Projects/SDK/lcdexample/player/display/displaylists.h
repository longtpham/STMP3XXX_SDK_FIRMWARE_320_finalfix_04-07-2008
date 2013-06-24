#ifndef _DISPLAY_LISTS_H
#define _DISPLAY_LISTS_H

#include "display.h"

//Display lists is very LCD centric


//Each menu item contains which page it belongs on, the resource required to display this, and the
//X and Y position.  All Items on the same page as the selected item will be displayed using the
//coordinates listed below.
struct MenuItem
{
    int m_iPage;
    int m_iResource;
    int m_ixPos;
    int m_iyPos;
};

//The PagedListStructure contains the message used to display items in the list, 
//the message used to display the selected item in the list, the number of items
//in the list, and a pointer to an array of these items.
struct PagedListStructure
{
    int m_iNormalDisplayMsg;
    int m_iSelectedDisplayMsg;
    int m_iItems;
    struct MenuItem *m_pItems;
};

_reentrant int DisplayPagedList(int iSelectedItem, int iTitle, void*ListStructure);

// iTitle values
#define BITMAP_SETTINGS_TITLE_NUM 1 //sdk2.1 for displaying title bitmap SETTINGS. 
#define BITMAP_ERASE_TITLE_NUM    2 //sdk2.1 for displaying title bitmap ERASE. 
#define BITMAP_TEST_TITLE_NUM    3 
#define BITMAP_RECORD_SETTINGS_TITLE_NUM 4

#ifdef JPEG_APP
#define BITMAP_JPEG_DISPLAY_TITLE_NUM 5 //sdk2.1 for displaying title bitmap JPEG DISPLAY. 
#endif

#endif