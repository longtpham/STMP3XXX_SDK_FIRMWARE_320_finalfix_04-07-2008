#ifndef _TIMEDATE_H
#define _TIMEDATE_H

#include "types.h"

#define TIMEDATE_REFRESH_TIME  100  // 15 seconds

enum _TIMEDATEMENU_ID
{
TIME_DATE_MENU_CLOCK = 0,
TIME_DATE_MENU_SET_TIME,
TIME_DATE_MENU_SET_DATE,
TIME_DATE_MENU_EXIT
};

#define TIME_DATE_MENU_FIRST	TIME_DATE_MENU_CLOCK
#define TIME_DATE_MENU_LAST	TIME_DATE_MENU_EXIT
#define TIME_DATE_MENU_COUNT	(TIME_DATE_MENU_LAST-1)

#define TIME_DATE_TITLE_X_POS  	2
#define TIME_DATE_TITLE_Y_POS  	8

#define TIMEFORMAT_12HOUR 0
#define TIMEFORMAT_24HOUR 1

#define DATEFORMAT_FIRST		0
#define DATEFORMAT_MMDDYYYY 0
#define DATEFORMAT_DDMMYYYY 1
#define DATEFORMAT_YYYYMMDD 2
#define DATEFORMAT_LAST		2

//Place the Date in the title of TimeDateMenu
#define DAYNAME_X_POS 	0			
#define DAYNAME_Y_POS 	48
#define DATE_X_POS 		50
#define DATE_Y_POS 		48
#define TIME_X_POS 		0
#define TIME_Y_POS 		32

#define DAYS_OF_WEEK_X_POS	0
#define DAYS_OF_WEEK_Y_POS	2
#define DATE_BEGIN_XPOS		35
#define DATE_BEGIN_YPOS		2
#define TIME_NUM_MEDIUM_WIDTH	8
#define TIME_DASH_WIDTH		8
#define TIME_MONTH_WIDTH		29

//Place the real time
#define TIME_BEGIN_X_POS	6
#define TIME_BEGIN_Y_POS	32
#define TIME_NUM_LARGE_WIDTH	13
#define TIME_COLON_WIDTH		5

// Defines for place Days of the Week
#define DAYOFWEEK_FIRST	0
#define SATURDAY		0
#define SUNDAY			1
#define MONDAY	   		2
#define TUESDAY			3
#define WEDNESDAY		4
#define THURSDAY		5
#define FRIDAY			6
#define DAYOFWEEK_LAST	6
	

#define JAN		1
#define FEB		2
#define MAR		3
#define APR		4
#define MAY		5
#define JUN		6
#define JUL		7
#define AUG		8
#define SEP		9
#define OCT		10
#define NOV		11
#define DEC		12
					
extern void SetRTC(unsigned long);
extern unsigned long ReadRTC(void);

typedef union	
{
    struct
    {
        unsigned int m_iMinutes: 6;
        unsigned int m_iHours : 5;
        unsigned int m_iDays: 5;
        unsigned int m_iMonths: 4;
        unsigned int m_iYears: 7;

        unsigned int m_iSeconds: 6;
        unsigned int m_iDayOfWeek: 3;
	}B;
	long L;
}TimeDateValues_type;

#endif
