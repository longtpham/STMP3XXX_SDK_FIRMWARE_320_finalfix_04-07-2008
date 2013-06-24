#ifndef _PLAYMODE_H
#define _PLAYMODE_H

#include "types.h"

// order of PLAYMODE selection
// must match order of struct MenuItem defined in eqmenu.c
#define FIRST_PM    	0
#define PM_NORMAL   	0
#define PM_REPEAT1  	1
#define PM_REPEATALL    2
#define LAST_VOICE_PM   2
#define PM_SHUFFLE		3   
#define PM_SHUFFLE_REPEAT 4 
#define LAST_PM     4
#define PMMENU_COUNT   (LAST_PM+1)

#ifdef S6B33B0A_LCD
#define PMMENU_PAGE1_COUNT	PMMENU_COUNT
#endif

#ifdef SED15XX_LCD
#define PMMENU_PAGE1_COUNT	4
#endif

//Repeat Message Parameters
#define REPEAT_OFF         0
#define REPEAT_ONE         1
#define REPEAT_ALL         2
//#define REPEAT_SONG        2

//Shuffle Message Parameters
#define SHUFFLE_OFF        0
#define SHUFFLE_ON         1

extern INT g_iPlayModeSetting;
#ifdef USE_PLAYLIST3
extern INT  g_shuffle_start;
extern INT  g_iShuffleSetting;   // Shuffle Setting
extern INT  g_iRepeatSetting;    // Repeat Setting
#endif  // USE_PLAYLIST3

#endif