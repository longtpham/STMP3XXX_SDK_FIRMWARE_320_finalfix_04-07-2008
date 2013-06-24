#ifndef _SETTIME_H
#define _SETTIME_H

#include "types.h"
#include "timedatemenu.h"

#define SET_TIME_TITLE_X_POS  	15
#define SET_TIME_TITLE_Y_POS  	10

#define FIRST_TIME_SELECT 0
#define HOUR_SELECT 0
#define MINUTE_SELECT 1
#define TIMEFORMAT_SELECT 2
#define TIMEOK_SELECT 3
#define TIMECANCEL_SELECT 4
#define LAST_TIME_SELECT 4
#define TIME_SELECT_ALL 5

#define HOUR_X_POS	16
#define HOUR_Y_POS	32
#define FORMAT_X_POS 16
#define FORMAT_Y_POS 40
#define OK_TIME_X_POS 16
#define OK_TIME_Y_POS  48 
#define SAVE_CHANGES_TIME_X_POS	4
#define SAVE_CHANGES_TIME_Y_POS 24

#define SETTIME_FIRST_SAVE_CHANGES 0
#define SETTIME_YES		 0
#define SETTIME_NO		 1
#define SETTIME_CANCEL	 2
#define SETTIME_LAST_SAVE_CHANGES 2

#define SETTIME_CLEAR	3 // out of range because it should not be included in options

// Not Used
//#define MINUTE_X_POS 41
//#define MINUTE_Y_POS 56
//#define SECOND_X_POS 57
//#define SECOND_Y_POS 56
//#define MERIDIEM_X_POS 72
//#define MERIDIEM_Y_POS 56

// External functions
extern void SetRTC(unsigned long);
extern unsigned long ReadRTC(void);
extern _reentrant void UpdateDate(int a, int b, int *Unused);
extern _reentrant void UpdateTime(int a, int b, int *Unused);

#endif

