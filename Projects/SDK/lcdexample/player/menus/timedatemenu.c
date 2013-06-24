////////////////////////////////////////////////////////////////////////////////
// SigmaTel Inc. Copyright 2003
// Filename:    TimeDateMenu.c
// Description: Display time and date
// Reviews: DanhNguyen (06-2008)
////////////////////////////////////////////////////////////////////////////////

#include "exec.h"                                       
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "settingsmenu.h"
#include "display.h"
#include "displaylists.h"
#include "menus.h"
#include "mainmenu.h"
#include "project.h"
#include "sysmem.h"
#include "playerstatemachine.h"
#include "const.h"
#include "hwequ.h"
#include "timedatemenu.h"
#include "settimemenu.h"
#include "setdatemenu.h"

#include "filesystem.h"
#include "fsproj.h"

// Local function prototypes //
_reentrant void TimeDateMenuDisplayDate(int a, int b, int *pPtr);
_reentrant void TimeDateMenuRefreshTime(void);
_reentrant void UpdateTime(int a, int b, int *Unused);
_reentrant void UpdateDate(int a, int b, int *Unused);
_reentrant void PrintStringTimeDate(int iMode,int iXpos,int iYpos,int iString);
_reentrant void PrintNumberTimeDate(int iMode,int iXpos,int iYpos,int iNumber,int iWidth,int iFiller);
  
// Global Variables //
extern int g_iTimeFormat;
extern int g_iDateFormat;
extern long g_UserTimeoffset;

// Before using the values, UpdateTime() and UpdateDate() should be called
TimeDateValues_type g_iCurrentRTCValues;

////////////////////////////////////////////////////////////////////////////////
//
//>  Name: 		_reentrant void TimeDateMenuDisplayDate(int a, int b, int *pPtr)
//
//   Type:          	Function 
//
//   Description:   	This function displays the date, month & year calculated from RTC.
//				
//   Inputs:        	none used.
//
//   Outputs:       	lcd messages  
//
//   Notes:         	Place in title of the TimeDateMenu
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void TimeDateMenuDisplayDate(int a, int b, int *pPtr)
{
	int iRTCYears;
	int iRTCMonths;
	int iRTCDays;
	int iRTCDayOfWeek;
	int iTrackingXPos=DATE_BEGIN_XPOS;
	int iTrackingYPos=DATE_BEGIN_YPOS;

	SysPostMessage(6,LCD_CLEAR_RANGE,0,0,128,16);
	//Update current date	
	UpdateDate(0,0,0);
	iRTCYears   = g_iCurrentRTCValues.B.m_iYears;
	iRTCMonths  = g_iCurrentRTCValues.B.m_iMonths;
	iRTCDays    = g_iCurrentRTCValues.B.m_iDays;
	iRTCDayOfWeek = g_iCurrentRTCValues.B.m_iDayOfWeek; 

	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,DAYS_OF_WEEK_X_POS,DAYS_OF_WEEK_Y_POS,RSRC_TIME_DAY_SAT+iRTCDayOfWeek*2+g_iLanguage);
	//SysWaitOnEvent(0,0,0); 
	switch(g_iDateFormat)
	{
	case DATEFORMAT_DDMMYYYY:
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCDays/10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCDays%10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_DASH);
		iTrackingXPos+=TIME_DASH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_MONTH_1-2+(iRTCMonths-1)*2+g_iLanguage);
		iTrackingXPos+=TIME_MONTH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_DASH);
		iTrackingXPos+=TIME_DASH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_2);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCYears/10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCYears%10);		
		break;
		
	case DATEFORMAT_MMDDYYYY:
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_MONTH_1+(iRTCMonths-1)*2+g_iLanguage);
		iTrackingXPos+=TIME_MONTH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_DASH);
		iTrackingXPos+=TIME_DASH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCDays/10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCDays%10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_DASH);		
		iTrackingXPos+=TIME_DASH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_2);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCYears/10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCYears%10);
		break;
		
	case DATEFORMAT_YYYYMMDD:
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_2);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCYears/10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCYears%10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_DASH);		
		iTrackingXPos+=TIME_DASH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_MONTH_1+(iRTCMonths-1)*2+g_iLanguage);
		iTrackingXPos+=TIME_MONTH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_DASH);
		iTrackingXPos+=TIME_DASH_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCDays/10);
		iTrackingXPos+=TIME_NUM_MEDIUM_WIDTH;
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,iTrackingXPos
						,iTrackingYPos
						,RSRC_TIME_NUM_MEDIUM_0+iRTCDays%10);
		break;
		
	default:
		break;	
	}

SysWaitOnEvent(0,0,0); 

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:		_reentrant void TimeDateMenuRefreshTime(void)
//
//   Type:          	Function 
//
//   Description:   	This function update the real time to display from RTC.
//				System Icons are still displayed via TimeDateMenuDisplayDate
//
//   Inputs:        	none used.
//
//   Outputs:       	lcd messages  
//
//   Notes:         	Refresh timer are displayed.
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant void TimeDateMenuRefreshTime(void)
{

	int iRTCHours;
	int iRTCMinutes;
	int iRTCSeconds;
	char sTimeString[12];
	char * pTemp;
	int Pixel24h=0;
	
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,16,RSRC_TIME_CLEAN_DESKTOP);
	
	//Update & Refresh real time
	UpdateTime(0,0,0);
	iRTCHours   = g_iCurrentRTCValues.B.m_iHours;
	iRTCMinutes = g_iCurrentRTCValues.B.m_iMinutes;
	iRTCSeconds = g_iCurrentRTCValues.B.m_iSeconds;
	
	if(g_iTimeFormat==TIMEFORMAT_12HOUR)
	{
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+TIME_COLON_WIDTH*2+TIME_NUM_LARGE_WIDTH*6
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_AM);
		if(iRTCHours>12)
		{
			iRTCHours-=12;
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+TIME_COLON_WIDTH*2+TIME_NUM_LARGE_WIDTH*6
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_PM);			
		}
		if(iRTCHours==0)
		{
			iRTCHours=12;
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+TIME_COLON_WIDTH*2+TIME_NUM_LARGE_WIDTH*6
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_PM);
		}
		
		Pixel24h=0;
	}
	else
	{
		SysPostMessage(6,LCD_CLEAR_RANGE
						,TIME_BEGIN_X_POS+TIME_COLON_WIDTH*2+TIME_NUM_LARGE_WIDTH*6
						,TIME_BEGIN_Y_POS
						,25		//pixels in X dirrection
						,12);	//pixels in y Dirrection
		Pixel24h=11;
	}	
	
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_LARGE_0+iRTCHours/10);
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h+TIME_NUM_LARGE_WIDTH
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_LARGE_0+iRTCHours%10);
		
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h+TIME_NUM_LARGE_WIDTH*2
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_COLON);
		
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h+TIME_COLON_WIDTH+TIME_NUM_LARGE_WIDTH*2
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_LARGE_0+iRTCMinutes/10);		
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h+TIME_COLON_WIDTH+TIME_NUM_LARGE_WIDTH*3
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_LARGE_0+iRTCMinutes%10);
		
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h+TIME_COLON_WIDTH+TIME_NUM_LARGE_WIDTH*4
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_COLON);
		
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h+TIME_COLON_WIDTH*2+TIME_NUM_LARGE_WIDTH*4
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_LARGE_0+iRTCSeconds/10);
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC
						,TIME_BEGIN_X_POS+Pixel24h+TIME_COLON_WIDTH*2+TIME_NUM_LARGE_WIDTH*5
						,TIME_BEGIN_Y_POS
						,RSRC_TIME_NUM_LARGE_0+iRTCSeconds%10);	
	
		SysWaitOnEvent(0,0,0); 	// let system process its display msg

	
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant TimeDateMenu(int a, int b, int *pPtr)
//
//   Type:          Function (code bank callable)
//
//   Description:   This function is the time and date menu
//
//   Inputs:        none used.
//
//   Outputs:       messages
//
//   Notes:         Will stay here until menu button is pressed
//<
////////////////////////////////////////////////////////////////////////////////
int _reentrant TimeDateMenu(int a, int b, int *pPtr)
{
	union DisplayHints DisplayHint;

	INT  iEvent;
    	BOOL bDone = FALSE;
    	INT  iNextMenu = MENU_MAIN;
    	BOOL bSendToPlayerStateMachine;

	
	// this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

    	//a;b;//here to prevent parameter not used messages.

    	//Lets initially clear screen and display all
    	//DisplayHint.I = TIME_DATE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
	DisplayHint.bits.ClearDisplay=TRUE;
	SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
	DisplayHint.I = 0;
	

	// Call function to display all time and date values //
	TimeDateMenuDisplayDate(0,0,0);
	
    	// Start the refresh timer
	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_TIMEDATE,0,TIMEDATE_REFRESH_TIME,MENU_MSG_REFRESH_TIMEDATE);
    while(!bDone)
	{
        //DisplayHint.I &= (TIME_DATE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
        DisplayHint.bits.ClearDisplay=TRUE;
        //wait on either a EVENT_BUTTON or EVENT_MESSAGE
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1: -1);        

        bSendToPlayerStateMachine=TRUE;
		
		switch(iEvent)
		{
		case EVENT_TIMER:
                	TimeDateMenuDisplayDate(0,0,0);			  
                	DisplayHint.I=0;
	            	bSendToPlayerStateMachine = FALSE;
			break;
			
		case EVENT_MESSAGE:              // (SDK2.520)
			switch(gEventInfo.Message.wMsgCommand)
			{
	  		case MENU_MSG_REFRESH_TIMEDATE:   // process TIMEDATE display refresh
	  			TimeDateMenuRefreshTime();				
	  			break;
			}
			break;    // pass all other EVENT_MESSAGEs to PSM

	        case EVENT_BUTTON:
	        // button pressed, so restart timer if enabled
    	    UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
			// turn on backlight when button is pressed.
			if (g_iBackLightState==BACKLIGHT_ON)
			{
    	    		SysPostMessage(2,LCD_BACKLIGHT_ON);
        	    	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
        	    	SysWaitOnEvent(0,0,0);
 			} 
#endif

	            //somebody pressed a button.
	            //remember the last button event (mostly for coming out of FFWD and RWND)		
	            switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
	            {
			case PR_FF:
			case PH_FF:
			case PR_RW:
			case PH_RW:
			case PR_RV:
			case PH_RV:
    			case PR_STOP:
    				bSendToPlayerStateMachine = FALSE;
    			    	break;

			case PH_PLAY:
                    	case PH_STOP:
		              //if the STOP button is held down, lets call the shutdown menu
		              SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
		              // if returns update display  -- if doesn't return, powered down
		              DisplayHint.I |= (TIME_DATE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS)); 
                        	bSendToPlayerStateMachine=FALSE;
				break;
				
			case PH_MENU:
				// allow user to escape to music mode
				// set flag so PR_MENU in music mode will ignore
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
			case PR_PLAY:
			case PR_MENU:
				SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_TIMEDATE);
				bDone = TRUE;
		              //defaults to main menu when exiting.  No need to set a new menu.
                        	bSendToPlayerStateMachine=FALSE;
			default:					
				break;	            
					}// end switch (button)
	            break;
		}	// end switch (iEvent)
        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
 	} // end while (!bDone)
 	 
    return MENU_MAIN;
}


/********************************************************
/	Function:	UpdateTime
/	Inputs:
/	Outputs:
/	Description: Updates RTC time variables from RTC.  
/********************************************************/
_reentrant void UpdateTime(int a,int b,int *Unused) 
{	
	int iRTCMilliSeconds;
	long lRTCStartTime;
    a,b;

	lRTCStartTime = ReadRTC();
   	iRTCMilliSeconds = (unsigned int) (lRTCStartTime%1000);
	lRTCStartTime /= (long) 1000;	//Start time now has the number of seconds
    	lRTCStartTime=(long)lRTCStartTime + (long)g_UserTimeoffset;
	g_iCurrentRTCValues.B.m_iSeconds =  (unsigned int) (lRTCStartTime%60);
	lRTCStartTime /= (long) 60;		//now has number of minutes
	g_iCurrentRTCValues.B.m_iMinutes = (unsigned int) (lRTCStartTime%60);
	lRTCStartTime /= (long) 60;		//now has number of hours
	g_iCurrentRTCValues.B.m_iHours = (unsigned int) (lRTCStartTime%24);
}
  

/********************************************************
/	Function:	UpdateDate
/	Inputs:
/	Outputs:
/	Description: Updates RTC date variables from RTC. 
/********************************************************/
_reentrant void UpdateDate(int a, int b, int *Unused) {
	
	int iYears;
	int iMonths;
	int iDays;
	int iDayOfWeek;

	long lRTCStartTime;
	int iRTCTotalDays;
	
	a,b;
	lRTCStartTime = ReadRTC();
	lRTCStartTime /= (unsigned long) 1000;			 	//now has seconds
    	lRTCStartTime = lRTCStartTime+(long)g_UserTimeoffset;
	iRTCTotalDays = (unsigned int) (lRTCStartTime/86400);	//now has number of days that have passed

#if 0	
    // This calculation does not work correctly for Dec 31 in each leap year.  For example,
    // since 2008 is a leap year, Dec 31st is the 365th day of that year.  Dec 31, 2008 is 10592
    // days since Jan 1, 1980.  Using the code below you would get
    //
    // iYears = 10592 / 1461 = 7
    // iYears *= 4 = 28.
    // 
    // iDays = 10592 % 1461 = 365
    //
    // We skip the 'if(iDays>=366)' code because we have 365 days.  But then we reach
    // the 'while(iDays >= 365)' code and we fall into that loop because iDays is equal
    // to 365.  This changes iYears to 29 and iDays to 0, which is incorrect.  We
    // should have iYears set to 28 and iDays left at 365 since Dec 31, 2008 is the
    // 365th day of 2008.
    //
    // NOTE: the 'while(iDays >= 365)' loop was 'while(iDays > 365)' which would fix
    // this problem BUT CAUSES MORE!

	// Calculate years  - calculate in groups of 4 to deal with leapyears without fraction divides //

	iYears = iRTCTotalDays/1461;		
	iYears *= 4;

	iDays = iRTCTotalDays%1461;
	if(iDays >= 366) {
		iDays -= 366;
		iYears++;
	}
	while(iDays >= 365) {
		iDays -= 365;
		iYears++;
	}
#else
    // A leap year is a year that is divisible by 4 EXCEPT years that are
    // divisible by 100 but not 400.  The next year divisible by 4 that
    // is not a leap year is 2100 so we can safely assume for our
    // calculations that it's just a year divisible by 4.

	iYears = iRTCTotalDays/1461;		
	iYears *= 4;

	iDays = iRTCTotalDays%1461;

    // At this point we know we have less than 4 years of days
    // in iDays.  We also know that the first year is going
    // to be a leap year.  We need to find out how many years
    // are in iDays, add that to iyears and then subtract 
    // that many days from iDays.  The first year will be
    // a leap year so we need to add one 366 and rest 365.

	iYears = iRTCTotalDays/1461;		
	iYears *= 4;

	iDays = iRTCTotalDays%1461;

    // NOTE: At this point, if iDays < 366 then we are in
    // a leap year.

	if(iDays >= 366) 
    {
		iDays -= 366;
		iYears++;
        
        // The first year has 366 days and the
        // rest 365.
	    while(iDays >= 365) 
        {
		    iDays -= 365;
		    iYears++;
	    }
	}

#endif

	// calculate Day of Week
	g_iCurrentRTCValues.B.m_iDayOfWeek = iRTCTotalDays%7;
     
	// Calculate month and days //
	iRTCTotalDays = iDays;
	switch (iYears%4) {
	case 0:		 // leap year
		if(iRTCTotalDays < 182) {			// first half of year
			if(iRTCTotalDays < 91)	{		// first quarter
				if(iRTCTotalDays > 59 )		{iMonths = MAR; iDays -= 59;}	// MAR
				else if(iRTCTotalDays > 30) {iMonths = FEB; iDays -= 30;}	// FEB
				else {				 	 	 iMonths = JAN; iDays += 1;}   	// JAN
			} // if(totalday < 91)
			else {	   					// second quarter
				if(iRTCTotalDays > 151)		{iMonths = JUN; iDays -= 151;}	// JUN
				else if(iRTCTotalDays > 120){iMonths = MAY; iDays -= 120;}	// MAY
				else {				 	 	 iMonths = APR; iDays -= 90;}	// APR
			}
		} // end if(iRTCTotalDays < 182)
		else {							// second half of year
			if(iRTCTotalDays < 274) {		// third quarter
				if(iRTCTotalDays > 243)		{iMonths = SEP; iDays -= 243;}	// SEP
				else if(iRTCTotalDays > 212){iMonths = AUG; iDays -= 212;}	// AUG
				else {				 	 	 iMonths = JUL; iDays -= 181;}	// JUL
			} // if(iRTCTotalDays < 274)
			else {						// fourth quarter
				if(iRTCTotalDays > 334)		{iMonths = DEC; iDays -= 334;}	// DEC
				else if(iRTCTotalDays > 304){iMonths = NOV; iDays -= 304;}	// NOV
				else {				 	 	 iMonths = OCT; iDays -= 273;}	// OCT
			}
		}
		break;	// end leap year case
	
	default:	// non-leap year
		if(iRTCTotalDays < 181) {			// first half of year
			if(iRTCTotalDays < 90)	{		// first quarter
				if(iRTCTotalDays > 58 )		{iMonths = MAR; iDays -= 58;}	// MAR
				else if(iRTCTotalDays > 30) {iMonths = FEB; iDays -= 30;}	// FEB
				else {				 	  	 iMonths = JAN; iDays += 1;}   	// JAN
			}
			else {	   					// second quarter
				if(iRTCTotalDays > 150)		{iMonths = JUN; iDays -= 150;}	// JUN
				else if(iRTCTotalDays > 119){iMonths = MAY; iDays -= 119;}	// MAY
				else {				 	  	 iMonths = APR; iDays -= 89;}	// APR
			}
		} // end if(iRTCTotalDays < 181)
		else {							// second half of year
			if(iRTCTotalDays < 273) {		// third quarter
				if(iRTCTotalDays > 242)		{iMonths = SEP; iDays -= 242;}	// SEP
				else if(iRTCTotalDays > 211){iMonths = AUG; iDays -= 211;}	// AUG
				else {				 	   	 iMonths = JUL; iDays -= 180;}	// JUL
			}
			else {						// fourth quarter
				if(iRTCTotalDays > 333)		{iMonths = DEC; iDays -= 333;}	// DEC
				else if(iRTCTotalDays > 303){iMonths = NOV; iDays -= 303;}	// NOV
				else {				 	 	 iMonths = OCT; iDays -= 272;}	// OCT
			}
		}
		break;	// end non-leap year case	
		
	} // end switch (iYears%4)

    	g_iCurrentRTCValues.B.m_iDays   = iDays;
	g_iCurrentRTCValues.B.m_iMonths = iMonths;
	g_iCurrentRTCValues.B.m_iYears  = iYears;
} // end UpdateDate
 

_reentrant void PrintStringTimeDate(int iMode,int iXpos,int iYpos,int iString)
{
	SysPostMessage(5,iMode,iXpos,iYpos,iString);
}

_reentrant void PrintNumberTimeDate(int iMode,int iXpos,int iYpos,int iNumber,int iWidth,int iFiller)
{
    SysPostMessage(7,iMode,iXpos,iYpos,iNumber,iWidth,iFiller);
}

#ifdef MMC
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void UpdateDateValues(int a, int b, int* iDateArray[])
//												
//
//   Type:          Function
//
//   Description:   Retrieves and converts RTC to month,day, and year values  
//
//   Inputs:        int a - space holder for SysCallFunction
//					int b - space holder for SysCallFunction
//					int* iDateArray[] - Array holding addresses of day,month,and year 
//                    from SetDateMenu.c
//
//   Outputs:       Day, Month, and Year in iDateArray contains the correct values
//
//   Notes:         This function was placed here for overlay memory considerations.  
//					It is not used in SetTimeMenu. It is called with SysCallFunction by
//					SetDateMenu. SetTimeMenu had available memory space without increasing
//					the overlay block size.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void UpdateDateValues(int a, int b, int* iDateArray[]) {
	
	int iYears ;
	int iMonths;
	int iDays;
	
	// Get the addressed passed 
	int* months = iDateArray[0];
	int* days = iDateArray[1];
	int* years = iDateArray[2];

	long lRTCStartTime;
	int iRTCTotalDays;
	
	a,b; // prevent compiler unused warnings

	lRTCStartTime = ReadRTC();
	lRTCStartTime /= (unsigned long) 1000;			 	//now has seconds
    	lRTCStartTime =lRTCStartTime+(long)g_UserTimeoffset;//+(long)g_Dateoffset;
	iRTCTotalDays = (unsigned int) (lRTCStartTime/86400);	//now has number of days that have passed
	
	// Calculate years  - calculate in groups of 4 to deal with leapyears without fraction divides //
	iYears = iRTCTotalDays/1461;		
	iYears *= 4;

	iDays = iRTCTotalDays%1461;
	if(iDays >= 366) {
		iDays -= 366;
		iYears++;
	}
	while(iDays > 365) {
		iDays -= 365;
		iYears++;
	}

	// Calculate month and days //
	iRTCTotalDays = iDays;
	switch (iYears%4) {
	case 0:		 // leap year
		if(iRTCTotalDays < 182) {		// first half of year
			if(iRTCTotalDays < 91)	{		// first quarter
				if(iRTCTotalDays > 59 )		{iMonths = MAR; iDays -= 59;}	// MAR
				else if(iRTCTotalDays > 30) {iMonths = FEB; iDays -= 30;}	// FEB
				else {				 	 	 iMonths = JAN; iDays += 1;}   	// JAN
			} // if(totalday < 91)
			else {	   					    // second quarter
				if(iRTCTotalDays > 151)		{iMonths = JUN; iDays -= 151;}	// JUN
				else if(iRTCTotalDays > 120){iMonths = MAY; iDays -= 120;}	// MAY
				else {				 	 	 iMonths = APR; iDays -= 90;}	// APR
			}
		} // end if(iRTCTotalDays < 182)
		else {							// second half of year
			if(iRTCTotalDays < 274) {		// third quarter
				if(iRTCTotalDays > 243)		{iMonths = SEP; iDays -= 243;}	// SEP
				else if(iRTCTotalDays > 212){iMonths = AUG; iDays -= 212;}	// AUG
				else {				 	 	 iMonths = JUL; iDays -= 181;}	// JUL
			} // if(iRTCTotalDays < 274)
			else {						    // fourth quarter
				if(iRTCTotalDays > 334)		{iMonths = DEC; iDays -= 334;}	// DEC
				else if(iRTCTotalDays > 304){iMonths = NOV; iDays -= 304;}	// NOV
				else {				 	 	 iMonths = OCT; iDays -= 273;}	// OCT
			}
		}
		break;	// end leap year case
	
	default:	// non-leap year
		if(iRTCTotalDays < 181) {		// first half of year
			if(iRTCTotalDays < 90)	{		// first quarter
				if(iRTCTotalDays > 58 )		{iMonths = MAR; iDays -= 58;}	// MAR
				else if(iRTCTotalDays > 30) {iMonths = FEB; iDays -= 30;}	// FEB
				else {				 	  	 iMonths = JAN; iDays += 1;}   	// JAN
			}
			else {	   					    // second quarter
				if(iRTCTotalDays > 150)		{iMonths = JUN; iDays -= 150;}	// JUN
				else if(iRTCTotalDays > 119){iMonths = MAY; iDays -= 119;}	// MAY
				else {				 	  	 iMonths = APR; iDays -= 89;}	// APR
			}
		} // end if(iRTCTotalDays < 181)
		else {							// second half of year
			if(iRTCTotalDays < 273) {		// third quarter
				if(iRTCTotalDays > 242)		{iMonths = SEP; iDays -= 242;}	// SEP
				else if(iRTCTotalDays > 211){iMonths = AUG; iDays -= 211;}	// AUG
				else {				 	   	 iMonths = JUL; iDays -= 180;}	// JUL
			}
			else {						    // fourth quarter
				if(iRTCTotalDays > 333)		{iMonths = DEC; iDays -= 333;}	// DEC
				else if(iRTCTotalDays > 303){iMonths = NOV; iDays -= 303;}	// NOV
				else {				 	 	 iMonths = OCT; iDays -= 272;}	// OCT
			}
		}
		break;	// end non-leap year case	
		
	} // end switch (iYears%4)

	// pass back values
	*iDateArray[0] = iMonths;
	*iDateArray[1] = iDays;
	*iDateArray[2] = iYears;

} // end UpdateDateValues
#endif
