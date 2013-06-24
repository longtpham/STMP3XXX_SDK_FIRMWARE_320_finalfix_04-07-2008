#ifndef NEWMUSICMENU_H
#define NEWMUSICMENU_H

// menus in newmusic menu
// defines order of selection
#define NEWMUSICMENU_FIRST		0
#define MENU_1_DAY				0
#define MENU_1_WEEK				1
#define MENU_1_MONTH			2
#define MENU_NEWMUSIC_EXIT		3
#define NEWMUSICMENU_LAST		3
#define NEWMUSICMENU_COUNT  NEWMUSICMENU_LAST+1

extern int _reentrant 
NewMusicMenu
(
	int a, 
	int b, 
	int *pPtr
);

#endif  /* NEWMUSICMENU_H */
