#ifndef _TEST_MENU_H
#define _TEST_MENU_H

// menus in settings menu
// defines order of selection
#define TESTMENU_FIRST  	0
#define MENU_RECORDTEST			0
#define MENU_ENCODER        1
#define MENU_DEVICE			2
#define MENU_TIMEDURATION	3
//#define MENU_TEST_EXIT 		4
#define TESTMENU_LAST   	3
#define TESTMENU_COUNT  	TESTMENU_LAST+1


#define SEC_FIRST       0
#define SEC_5       	0
#define SEC_10          1    
#define SEC_30          2
#define SEC_60          3
#define SEC_300         4
#define SEC_600         5
#define SEC_LAST        5
#define SEC_COUNT       SEC_LAST+1
void _reentrant RecordTestFile(void);
extern struct RecorderSettings g_RecorderSettings[];

#endif
