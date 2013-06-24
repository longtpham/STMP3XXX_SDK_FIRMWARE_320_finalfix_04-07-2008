#ifndef PLAYMUSICMENU_H
#define PLAYMUSICMENU_H

#include "menus.h"
#include "musiclib_ghdr.h"

/*
#define PLAYMUSICMENU_FIRST		0
#define MENU_PLAY_ALL			0
#define MENU_ARTIST				1
#define MENU_ALBUM				2
#define MENU_SONGS				3
#define MENU_ON_THE_FLY		4
#define MENU_NEW_MUSIC			5
#define MENU_GENRE					6
#define MENU_YEAR					7

#if PLAYLIST_ENABLE
#define MENU_PLAYLIST			8
#define MENU_FM_REC				9
#define MENU_LINEIN_REC 		10

#ifdef _FOLDER_BROWSE_
#define MENU_INTERNAL           11
#define MENU_EXTERNAL           12
#define MENU_PLAYMUSIC_EXIT		13
#define PLAYMUSICMENU_LAST		13
#else
#define MENU_PLAYMUSIC_EXIT		11
#define PLAYMUSICMENU_LAST		11
#endif  // _FOLDER_BROWSE_

#else	//not PLAYLIST_ENABLE
#define MENU_FM_REC			8
#define MENU_LINEIN_REC         	9

#ifdef _FOLDER_BROWSE_
#define MENU_INTERNAL           	10
#define MENU_EXTERNAL           	11
#define MENU_PLAYMUSIC_EXIT		12
#define PLAYMUSICMENU_LAST		12
#else
#define MENU_PLAYMUSIC_EXIT		10
#define PLAYMUSICMENU_LAST		10
#endif  // _FOLDER_BROWSE_
#endif
*/
//Review by DanhNguyen (06-2008) with Using STFM1000. 
//Modify below if you dont use FM tuner
#define PLAYMUSICMENU_FIRST	0
#define MENU_PLAY_ALL			0
#ifdef _FOLDER_BROWSE_
#define MENU_INTERNAL           	1
#define MENU_EXTERNAL           	2
#define MENU_SONGS				3
#define MENU_ON_THE_FLY		4
#define MENU_FM_REC			5	//comment this line if you dont use FM tuner
#define MENU_PLAYMUSIC_EXIT	6
#define PLAYMUSICMENU_LAST	6
#else
#define MENU_SONGS				1
#define MENU_ON_THE_FLY		2
#define MENU_FM_REC			3	//comment this line if you dont use FM tuner
#define MENU_PLAYMUSIC_EXIT	4
#define PLAYMUSICMENU_LAST	4
#endif	//_FOLDER_BROWSE_

//dont use some MENU define
#define MENU_ARTIST				11
#define MENU_ALBUM				12
#define MENU_NEW_MUSIC		15
#define MENU_GENRE				16
#define MENU_YEAR				17
#define MENU_PLAYLIST			18

#define PLAYMUSICMENU_COUNT  (PLAYMUSICMENU_LAST+1)

#ifdef S6B33B0A_LCD
#define PLAYMUSICMENU_PAGE1_COUNT	PLAYMUSICMENU_COUNT
#endif

#ifdef SED15XX_LCD
#define PLAYMUSICMENU_PAGE1_COUNT	MENU_PAGE_ITEM_COUNT
#endif

#endif  /* PLAYMUSICMENU_H */
