#ifndef _SETDATE_H
#define _SETDATE_H

#include "types.h"
#include "timedatemenu.h"

#define SET_DATE_TITLE_X_POS  	15
#define SET_DATE_TITLE_Y_POS  	10


#define DAYOFWEEK_X_POS  	16
#define DAYOFWEEK_Y_POS  	24
#define MONTH_X_POS		16
#define MONTH_Y_POS		32
#define FORMAT_DATE_X_POS	16
#define FORMAT_DATE_Y_POS	40
#define OK_DATE_X_POS		16
#define OK_DATE_Y_POS		48
#define SAVE_CHANGES_DATE_X_POS	4
#define SAVE_CHANGES_DATE_Y_POS 24

#define SETDATE_FIRST_SAVE_CHANGES 0
#define SETDATE_YES		0
#define SETDATE_NO		1
#define SETDATE_CANCEL	2
#define SETDATE_LAST_SAVE_CHANGES 2
#define SETDATE_CLEAR	3	// out of range because it should not be included in the options, just a label

// Numbered in MMDDYYYY format
#define FIRST_DATE_SELECT	0
#define MONTH_SELECT 0	 
#define DAY_SELECT 1		 
#define YEAR_SELECT 2
#define DATEFORMAT_SELECT 3
#define DATEOK_SELECT 4
#define DATECANCEL_SELECT 5
#define LAST_DATE_SELECT 5
#define DATE_SELECT_ALL 6	
					
// External functions
extern _reentrant void UpdateDate(int a, int b,int *Unused);
extern void SetRTC(unsigned long);
extern unsigned long ReadRTC(void);

#endif

