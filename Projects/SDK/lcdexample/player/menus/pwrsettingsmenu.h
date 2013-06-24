#ifndef _PWR_H
#define _PWR_H

#include "types.h"

// order of Power Settings selection
// must match order of struct MenuItem defined in eqmenu.c
#define FIRST_PWR    	0
#define PWR_DISABLE   	0
#define PWR_1MIN	  	1
#define PWR_2MIN	    2   
#define PWR_5MIN		3   
#define PWR_10MIN		4 
#define LAST_PWR     	4
#define PWRMENU_COUNT   LAST_PWR+1

#define ONEMIN			(1*60000)
#define TWOMIN			(2*60000)
#define FIVEMIN			(5*60000)
#define TENMIN			(10*60000)

extern WORD g_wPowerOffTrigger;
extern WORD g_wPwrSettings;

#endif