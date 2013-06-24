#ifndef _SYSTEM_MENU_H
#define _SYSTEM_MENU_H

// menus in settings menu
// defines order of selection
#define DELETEMENU_FIRST  		0
//#define MENU_DELETE_MUSIC		0
//#define MENU_DELETE_MUSIC_ALL	1
#define MENU_DELETE_VOICE		0
#define MENU_DELETE_VOICE_ALL   1
#define MENU_DELETE_EXIT 		2
#define DELETEMENU_LAST   		2
#define DELETEMENU_COUNT  	DELETEMENU_LAST+1
// TOVERIFY2 ln below should be changed to 3 right?
#define MENU_DELETE_MAIN 		5
#ifdef USE_PLAYLIST3
#define MENU_DELETE_FM	 		6
#define MENU_DELETE_FM_ALL 		7
#define MENU_DELETE_LINEIN      8
#define MENU_DELETE_LINEIN_ALL  9

typedef enum{
    VOICE_SUBMENU,
    FMREC_SUBMENU,
    LINEIN_SUBMENU,
    MAXSUBMENU
} DELVOICESUBMENU;

#endif	// #ifdef USE_PLAYLIST3

#ifndef NO
#define NO	0
#endif

#ifndef	YES
#define YES 1
#endif
#endif
