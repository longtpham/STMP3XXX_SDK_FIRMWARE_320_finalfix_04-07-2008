#ifndef _BUTTONS_H
#define _BUTTONS_H

#include "types.h"

//These must match the definintions in buttondefs.inc, otherwise the button presses will trigger
//unexpected events.

#define PR_RW		0
#define PH_RW    		1
#define PR_FF		2
#define PH_FF		3
#define PR_MENU		4
#define PH_MENU		5
#define PR_RV 		6
#define PH_RV		7
#define PR_PLAY 		8
#define PH_PLAY         9
#define PR_HOLD         10
#define PH_HOLD         11
#define PR_VOL_DOWN 	12
#define PR_VOL_UP	13




#define PH_VOL_DOWN 	14
#define PH_VOL_UP	15
#define PR_MODE 	24
#define PR_STOP 		25
#define PH_STOP		26
#define PR_RECORD      27
#define PH_RECORD       28
#define PR_AB           33
#define PR_ERASE	34
#define PH_ERASE	35
#define PR_EQ           36
#define PH_EQ       37




extern WORD g_wLastButton;

#endif //_BUTTONS_H