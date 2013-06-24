////////////////////////////////////////////////////////////////////////////////
// SigmaTel Inc. Copyright 2003
// Filename: SetDateMenu.c (LCD version)
// Description: User interface to change the date and set the RTC
////////////////////////////////////////////////////////////////////////////////

#include "exec.h"
#include "types.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "menus.h"
#include "display.h"
#include "displaylists.h"
#include "eqmenu.h"
#include "mainmenu.h"
#include "settingsmenu.h"
#include "const.h"
#include "hwequ.h"
#include "setdatemenu.h"
#include "timedatemenu.h"
//Stmp00008949
#include "FileSystem.h"
#include "playerlib.h"
#include "sysmem.h"

extern void SaveUserTime(); //Stmp00008949

// Local function prototypes
_reentrant void SetDate(int iMonths,int iDays, int iYears);
_reentrant int DaysInMonth(int iMonths,int iYears);
_reentrant int DaysFromDate(int iDays,int iMonths,int iYears);
_reentrant void DisplaySaveChangesDate(int iSaveOption);
_reentrant void DisplayDateFirst(int iDateFormat);
_reentrant void DisplayChangeDate(int iCursorPosition,int iPreviousCursorPosition ,int iInvert, int iMonths,int iDays,int iYears,int iDateFormat);
_reentrant void PrintStringSetDate(int iMode,int iXpos,int iYpos,int iString);
_reentrant void PrintNumberSetDate(int iMode,int iXpos,int iYpos,int iNumber,int iWidth,int iFiller);

// Global variables
int g_iDateFormat;	// Current date format from saved user settings; g_iDateFormat is saved, iDateFormat 
				  	//  is used to allow user to change DateFormat in this menu but is not saved until user confirms
extern TimeDateValues_type g_iCurrentRTCValues;

extern long g_UserTimeoffset;
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant SetDateMenu(int a, int b, int *pPtr)
//
//   Type:          Function (set date menu entry point)
//
//   Description:   This function is the entry point for the set date menu manager.
//
//   Inputs:        None
//
//   Outputs:       Menu to enter upon exiting set date menu
//                      if PH_MENU: return to Music menu
//                      else : return to Settings menu selection
//
/*   Notes:         
*/
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant SetDateMenu(int a, int b, int *pPtr)
{
	BOOL bDone = FALSE;
	BOOL bSendToPlayerStateMachine;
	union DisplayHints DisplayHint;

	INT iDateFormat = g_iDateFormat; // use local temp DateFormat so format can have "option" to be saved

	INT iMonths;
	INT iDays;
	INT iYears;

	BOOL bBlink = FALSE;
	BOOL bDateModify_mode = FALSE;
	BOOL bDateModified = FALSE; // Stmp00008949
	BOOL bSaveChanges_mode = FALSE;
	
	INT iEvent;
	INT iCursorPosition = DATECANCEL_SELECT;
    INT iPreviousCursorPosition = DATEOK_SELECT;
	INT iSaveChangesSelected = SETDATE_CANCEL;
	INT iCancelConfirm = FALSE;
	INT iNextMenu = MENU_SET_DATE;

	// This is required to get the message into the PlayerStateMachine (PSM)
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

	b;   // avoid warnings for unused

	// We need to refresh the screen since entering the menu
	DisplayHint.I = TIME_DATE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
	SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
	DisplayHint.I = 0;
	

    SysCallFunction(RSRC_TIMEDATE_MENU_CODE_BANK,UpdateDate,0,0,0);
    iMonths = g_iCurrentRTCValues.B.m_iMonths;
	iDays   = g_iCurrentRTCValues.B.m_iDays;
    iYears  = g_iCurrentRTCValues.B.m_iYears;
 
    DisplayDateFirst(iDateFormat);
    DisplayChangeDate(DATE_SELECT_ALL,iPreviousCursorPosition,TRUE,iMonths,iDays,iYears,iDateFormat);
    iPreviousCursorPosition = iCursorPosition;
	while(!bDone) 
	{
		// Mask out all display hints that aren't aimed toward the EQ menu
		DisplayHint.I &= (TIME_DATE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

		// Wait until a Message or Button happens
		iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);

		// Assume every message goes to the PSM
		bSendToPlayerStateMachine = TRUE;

		        
        // Handle the event:  EVENT_TIMER, EVENT_MESSAGE, or EVENT_BUTTON
		switch(iEvent)
		{
		case EVENT_TIMER:
			if(DisplayHint.I != 0)
			{
				SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
				DisplayHint.I = 0;
			}
			bSendToPlayerStateMachine = FALSE;
			break;

		case EVENT_MESSAGE:              // (SDK2.520)
			switch(gEventInfo.Message.wMsgCommand)
			{
				case MENU_MSG_ANIMATE:   // process expiration of Blink timer
					bBlink ^= 1;         // toggle mode
					if(!bSaveChanges_mode)  // only blink if "Save Changes?" is not on the screen
						DisplayChangeDate(iCursorPosition,iPreviousCursorPosition,bBlink,iMonths,iDays,iYears,iDateFormat);					
					bSendToPlayerStateMachine = FALSE;
					break;
			}
			break;    // pass all other EVENT_MESSAGEs to PSM

		case EVENT_BUTTON:
			// A button was pressed, so restart timers if enabled
			UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
			// Turn ON the Backlight when any button is pressed
			if(g_iBackLightState == BACKLIGHT_ON)
			{
    	    	SysPostMessage(2,LCD_BACKLIGHT_ON);
        	    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
        	    SysWaitOnEvent(0,0,0);
 			} 
#endif
			switch(gEventInfo.Button.wButtonEvent)
			{
//				case PH_VOL_UP:       // instead, let PSM adjust the volume
//				case PH_VOL_DOWN:
//					break;

				case PR_FF:
				case PH_FF:
					if(bSaveChanges_mode == TRUE) {
						iSaveChangesSelected++;
						if(iSaveChangesSelected > SETDATE_LAST_SAVE_CHANGES)
							iSaveChangesSelected = SETDATE_FIRST_SAVE_CHANGES;
						DisplaySaveChangesDate(iSaveChangesSelected);
					}
					else if(bDateModify_mode == FALSE) {	// move cursor to next date parameter
						switch(iDateFormat) {		// Note: progresses left to right
						case DATEFORMAT_MMDDYYYY:	// Normal numeric progression: 0,1,2,3,4
							iCursorPosition++;
							if(iCursorPosition > LAST_DATE_SELECT)
							iCursorPosition = FIRST_DATE_SELECT;
							break;
						
						case DATEFORMAT_DDMMYYYY:	// Day and month reversed: 1,0,2,3,4 
						 	if(iCursorPosition == DAY_SELECT) iCursorPosition = MONTH_SELECT;  // 1 to 0
							else if(iCursorPosition == MONTH_SELECT) iCursorPosition = YEAR_SELECT;	// 0 to 2
							else if(iCursorPosition == LAST_DATE_SELECT) iCursorPosition = DAY_SELECT; // 4 to 1
							else iCursorPosition++;
							break;	
						  
						case DATEFORMAT_YYYYMMDD:	// Year first, then month, then day: 2,0,1,3,4
						  	if(iCursorPosition == YEAR_SELECT) iCursorPosition = MONTH_SELECT; // 2 to 0
							else if(iCursorPosition == DAY_SELECT) iCursorPosition = DATEFORMAT_SELECT;	 // 1 to 3
							else if(iCursorPosition == LAST_DATE_SELECT) iCursorPosition = YEAR_SELECT; // 4 to 2
							else iCursorPosition++;
							break;	
							
						} // end switch(iDateFormat)
					DisplayChangeDate(iCursorPosition,iPreviousCursorPosition,TRUE,iMonths,iDays,iYears,iDateFormat);
						
					} // end if
					else { //bDateModify_mode == TRUE , MENU button already pressed to select parameter, now adjust parameter
						switch(iCursorPosition){
						case MONTH_SELECT:
							iMonths++;
							if(iMonths > DEC)
								iMonths = JAN;
							if(iDays > DaysInMonth(iMonths,iYears))
							iDays = DaysInMonth(iMonths,iYears);
							break;
						case DAY_SELECT:
							iDays++;
							if(iDays > DaysInMonth(iMonths,iYears))
								iDays = 1;
							break;
						case YEAR_SELECT:
							iYears++;
							if(iYears > 99)
								iYears = 0;
							if(iDays > DaysInMonth(iMonths,iYears))
								iDays = DaysInMonth(iMonths,iYears);
							break;
						case DATEFORMAT_SELECT:
							iDateFormat++;
							if(iDateFormat > DATEFORMAT_YYYYMMDD)
							iDateFormat = DATEFORMAT_MMDDYYYY;
							break;	
						} //end switch(iCursorPosition)
					DisplayChangeDate(iCursorPosition,iPreviousCursorPosition,TRUE,iMonths,iDays,iYears,iDateFormat);
                    }
                    bSendToPlayerStateMachine = FALSE;         // no need to send to PSM
					break;

				case PR_RW:
				case PH_RW:
					if(bSaveChanges_mode == TRUE) {
						iSaveChangesSelected--;
						if(iSaveChangesSelected < SETDATE_FIRST_SAVE_CHANGES)
							iSaveChangesSelected = SETDATE_LAST_SAVE_CHANGES;
						DisplaySaveChangesDate(iSaveChangesSelected);
					}
					else if(bDateModify_mode == FALSE) {	// move cursor to next date parameter
						
                        switch(iDateFormat) {		// Note: Progress right to left
						
                        case DATEFORMAT_MMDDYYYY:	// Normal numeric progression: 0,1,2,3,4
							iCursorPosition--;			
							if(iCursorPosition < FIRST_DATE_SELECT)
							    iCursorPosition = LAST_DATE_SELECT;
							break;
						case DATEFORMAT_DDMMYYYY:	// Day and month reversed: 1,0,2,3,4 
							if(iCursorPosition == DAY_SELECT) iCursorPosition = LAST_DATE_SELECT;  // 1 to 4
							else if(iCursorPosition == MONTH_SELECT) iCursorPosition = DAY_SELECT;	// 0 to 1
							else if(iCursorPosition == YEAR_SELECT) iCursorPosition = MONTH_SELECT; // 2 to 0
							else iCursorPosition--;
							break;
						case DATEFORMAT_YYYYMMDD:	// Year first, then month, then day: 2,0,1,3,4
							if(iCursorPosition == YEAR_SELECT) iCursorPosition = LAST_DATE_SELECT; // 2 to 4
							else if(iCursorPosition == MONTH_SELECT) iCursorPosition = YEAR_SELECT;	 // 0 to 2
							else if(iCursorPosition == DATEFORMAT_SELECT) iCursorPosition = DAY_SELECT; // 3 to 1
							else iCursorPosition--;
							break;
						}
                        DisplayChangeDate(iCursorPosition,iPreviousCursorPosition,TRUE,iMonths,iDays,iYears,iDateFormat);
                    } 
					else {// bDateModify_mode == TRUE , MENU button already pressed to select parameter, now adjust parameter
						switch(iCursorPosition){
						case MONTH_SELECT:
							iMonths--;
							if(iMonths < JAN)
								iMonths = DEC;
							if(iDays > DaysInMonth(iMonths,iYears))
								iDays = DaysInMonth(iMonths,iYears);
							break;
						case DAY_SELECT:
							iDays--;
							if(iDays == 0)
			   					iDays = DaysInMonth(iMonths,iYears);
							break;
						case YEAR_SELECT:
							iYears--;
							if(iYears < 0)
								iYears = 99;
							if(iDays > DaysInMonth(iMonths,iYears))
								iDays = DaysInMonth(iMonths,iYears);
							break;
						case DATEFORMAT_SELECT:
							iDateFormat--;
							if(iDateFormat < DATEFORMAT_MMDDYYYY)
								iDateFormat = DATEFORMAT_YYYYMMDD;
							break;
						} // end switch(iCursorPosition)
                        DisplayChangeDate(iCursorPosition,iPreviousCursorPosition,TRUE,iMonths,iDays,iYears,iDateFormat);
					}
					bSendToPlayerStateMachine = FALSE;         // no need to send to PSM
					break;

				case PH_MENU:
					// Allow user to escape to Music mode,
					// but first, ask if changes need to be saved.
					
					if(!bSaveChanges_mode) 
                    {
						DisplaySaveChangesDate(SETDATE_CANCEL); // default to "Cancel"
						bSaveChanges_mode = TRUE;
					}
					else 
                    {
						g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
						bDone = TRUE;	
					}
                    bSendToPlayerStateMachine = FALSE;
					break;

				case PR_MENU:
					if(!bSaveChanges_mode) {
						switch(iCursorPosition) {
						case DATEOK_SELECT:
							g_iDateFormat = iDateFormat;
							SetDate(iMonths,iDays,iYears);
							bDone = TRUE;
							break;
					    case DATECANCEL_SELECT:
							bDone = TRUE;
							break;
						default:
							// Toggle the Date-modify mode
							if(bDateModify_mode) { // visually indicate the time parameter, invert the parameter
								bDateModify_mode = FALSE;	
								bBlink = FALSE;  // stop blinking
								SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
                                if(iCursorPosition==DATEFORMAT_SELECT) // update the format if it changed
                                {
                                    DisplayDateFirst(iDateFormat);
                                    DisplayChangeDate(DATEFORMAT_SELECT,MONTH_SELECT,TRUE,iMonths,iDays,iYears,iDateFormat);
							    }
                            }
							else {// visually indicate the time parameter, "blink" the parameter
								bDateModify_mode = TRUE;	// start blinking
								bDateModified = TRUE;   //Stmp00008949
								bBlink = TRUE;
								SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,500,MENU_MSG_ANIMATE);
							}
							SysWaitOnEvent(0,0,0);    // let system process SOFT_TIMER_xxx_TIMER msg (KILL or SET)
				   			break;
						}
					} // end if(!bSaveChanges_mode)
					else { // bSaveChanges_mode == TRUE;
						switch(iSaveChangesSelected) {
						case SETDATE_YES: // Exit and save
							g_iDateFormat = iDateFormat;
							SetDate(iMonths,iDays,iYears);
							g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
							g_MenuFlags.MENU_FLAG_SAVE_CHANGES = TRUE;
                            bDone = TRUE;
							bSendToPlayerStateMachine = FALSE;
							break;
						case SETDATE_NO: // Exit without saving
							g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
							g_MenuFlags.MENU_FLAG_SAVE_CHANGES = TRUE;
                            bDone = TRUE;	
							bSendToPlayerStateMachine = FALSE;
							break;
						case SETDATE_CANCEL: // Don't exit or save, return to set date menu
							if(iCancelConfirm) {
								bSaveChanges_mode = FALSE;
								iCancelConfirm = FALSE;
                                iCursorPosition = DATECANCEL_SELECT;
                                iPreviousCursorPosition = DATEOK_SELECT;
                                DisplaySaveChangesDate(SETDATE_CLEAR);
								DisplayDateFirst(iDateFormat);
                                DisplayChangeDate(DATE_SELECT_ALL,iPreviousCursorPosition,TRUE,iMonths,iDays,iYears,iDateFormat);
							}
							else
							    iCancelConfirm = TRUE;
							break;		
						default:  // should never happen
							break;
						}
					
					}		// end else {bSaveChanges_mode == TRUE
					break;

				case PR_STOP:
					bSendToPlayerStateMachine = FALSE;
				    break;

				case PH_STOP:
                    // If the STOP button is held down, let's call the Shutdown menu
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // If user chooses to return then update display.  If doesn't return, player was powered down.
                    DisplayHint.I |= (TIME_DATE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

                    bDone = TRUE;
                    bSendToPlayerStateMachine = FALSE;
                    break;
			} // end switch(wButtonEvent)
	    } // end switch(iEvent)

        //Update Previous Cursor
        iPreviousCursorPosition = iCursorPosition;

        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
    } // end while(!bDone)

    if(bDateModify_mode)     // (SDK2.520)
    {
        SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
        SysWaitOnEvent(0,0,0);      // let system process SOFT_TIMER_KILL_TIMER msg
    }
    //Stmp00008949
    if(bDateModified == TRUE)
    {   if(g_wDecoderSR&DECODER_STOPPED)
        {   SysLoadFATWrite();
            SaveUserTime();
            SysUnLoadFATWrite();
        }
    }
    return iNextMenu;
} // end of SetDateMenu()


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void SetDate(int iMonths,int iDays,int iYears)
//
//   Type:          Function
//
//   Description:    Set RTC to values passed in   
//
//   Inputs:        iMonths,iDays,iYears        
//
//   Outputs:       Sets RTC with new date
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void SetDate(int iMonths,int iDays, int iYears){
    
    int iTotalDays=0;
    long lRTCTime,RTCTimeinDay=0,NewTime,Diff;

	// Calculate the total number of days from date //
	// First, calculate days from number of years //
	iTotalDays = iYears*365;
	
	// First, add number of leap days that have passed in previous years //
	// Will not add leap day if current year is a leap year //
	if(iYears != 0)		// add the leapdays that have passed
		iTotalDays += ((iYears-1)/4) + 1;
	
	// Next, call function DaysFromDate to add number of days in current year //
	// Will add leap day if necessary //
	iTotalDays += DaysFromDate(iDays,iMonths,iYears);	
																		
	// Last, adjust total days to number of days passed for RTC					
	iTotalDays--;  
												
	// Save current time //
	lRTCTime = ReadRTC();				  
	lRTCTime /= (unsigned long) 1000;	 //now has seconds
    lRTCTime += g_UserTimeoffset;
    
    //RTCTimeinDay %= 86400;				 //now has total seconds in current day
    RTCTimeinDay= lRTCTime% 86400;
	
	// Add new date to RTC //
	NewTime = ((unsigned long) iTotalDays * 86400) + RTCTimeinDay;
    
    if(lRTCTime >= NewTime) {
        Diff = -(lRTCTime - NewTime);
    } else
    {
        Diff = (NewTime - lRTCTime);    
    }

    g_UserTimeoffset += Diff;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void DisplayChangeDate(int iCursorPosition ,int iInvert, int iMonths,
//                                                int iDays,int iYears,int iDateFormat)
//   Type:          Function
//
//   Description:   Displays the day of week, date and format for user to change and set.  
//
//   Inputs:        iCursorPosition - indicates which item to highlight or flash display
//                    iInvert - determines whether or not to invert the text
//                    iMonths - Current month count (JAN-DEC)
//                    iDays - Current day count (1 - Max Days in Month)
//                    iYear - Current year count (2000-2099)
//                    iDateFormat - Date format used in this display before it is saved to global
//
//   Outputs:       Updates the display to reflect user selections
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////

_reentrant void DisplayChangeDate(int iCursorPosition ,int iPreviousCursorPosition,int iInvert, int iMonths,int iDays,int iYears,int iDateFormat) {
    int iMonthMode =     LCD_PRINT_NUMBER;
    int iDayMode =         LCD_PRINT_NUMBER;
    int iYearMode =     LCD_PRINT_NUMBER;
    int iFormatMode =     LCD_PRINT_STRING_RSRC;
    int iOkMode =         LCD_PRINT_STRING_RSRC;
    int iCancelMode =     LCD_PRINT_STRING_RSRC;
    int iTotalDays;
    int iDayofWeek;
    int iDayOfWeekText = 0;
    

     int iLeftMode = 0; 
       int iLeftNum = iMonths;
       int iLeftLength    = 2;
       int iSlashPos = MONTH_X_POS+12;
       int iCenterMode = 0; 
       int iCenterNum = iDays;
       int iRightMode = 0; 
       int iRightNum = 0;
       int iRightLength = 4;
      int iFormatString = 0;
          
    switch(iCursorPosition){
        case MONTH_SELECT:
            if(iInvert) iMonthMode = LCD_PRINT_NUMBER_INV;
            break;
        case DAY_SELECT:
            if(iInvert) iDayMode =  LCD_PRINT_NUMBER_INV;
            break;
        case YEAR_SELECT:
            if(iInvert) iYearMode =  LCD_PRINT_NUMBER_INV;
            break;
        case DATEFORMAT_SELECT:
            if(iInvert) iFormatMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        case DATEOK_SELECT:
            if(iInvert) iOkMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        case DATECANCEL_SELECT:
            if(iInvert) iCancelMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        case DATE_SELECT_ALL:
            iCancelMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        default:
            break;
        }
    
    
    //SysPostMessage(6,LCD_CLEAR_RANGE,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,DAYOFWEEK_X_POS+48,DAYOFWEEK_Y_POS+16);
    
    
    // calcualte and draw day of week // 
    iTotalDays = DaysFromDate(iDays,iMonths,iYears);    
    iTotalDays += iYears*365;
    if(iYears != 0)        // add the leapdays that have passed
        iTotalDays += ((iYears-1)/4) + 1;        
    iTotalDays--;  // to adjust to number of days passed for RTC
    
    // Displays the day of the week, clears range first //
    /*
    switch(iTotalDays%7) {
        case SUNDAY:     SysPostMessage(5,LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,RSRC_STRING_SUNDAY); break;
        case MONDAY:     SysPostMessage(5,LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,RSRC_STRING_MONDAY); break;
        case TUESDAY:     SysPostMessage(5,LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,RSRC_STRING_TUESDAY); break;
        case WEDNESDAY: SysPostMessage(5,LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,RSRC_STRING_WEDNESDAY); break;
        case THURSDAY:     SysPostMessage(5,LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,RSRC_STRING_THURSDAY); break;
        case FRIDAY:     SysPostMessage(5,LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,RSRC_STRING_FRIDAY); break;
        case SATURDAY:     SysPostMessage(5,LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,RSRC_STRING_SATURDAY); break;                                                
    }
    */
     
    switch(iTotalDays%7) {
    case SUNDAY:    iDayOfWeekText = RSRC_STRING_SUNDAY; break;
    case MONDAY:    iDayOfWeekText = RSRC_STRING_MONDAY; break;
    case TUESDAY:   iDayOfWeekText = RSRC_STRING_TUESDAY; break;
    case WEDNESDAY: iDayOfWeekText = RSRC_STRING_WEDNESDAY; break;
    case THURSDAY:  iDayOfWeekText = RSRC_STRING_THURSDAY; break;
    case FRIDAY:    iDayOfWeekText = RSRC_STRING_FRIDAY; break;
    case SATURDAY:  iDayOfWeekText = RSRC_STRING_SATURDAY; break;
    }
    
        PrintStringSetDate(LCD_PRINT_STRING_RSRC,DAYOFWEEK_X_POS,DAYOFWEEK_Y_POS,iDayOfWeekText);                                                                                                                           
        
        //  Display the month, day, and year in the correct format // 
        //SysPostMessage(6,LCD_CLEAR_RANGE,MONTH_X_POS+32,MONTH_Y_POS,40,16);
        iYears = iYears + 2000;
        switch(iDateFormat) {
        case DATEFORMAT_MMDDYYYY:
            iLeftMode = iMonthMode;           
            iCenterMode = iDayMode;
            iRightMode = iYearMode;
            iRightNum = iYears;
            /*      
              SysPostMessage(7,iMonthMode,MONTH_X_POS,MONTH_Y_POS,iMonths,2,'0');
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,    MONTH_X_POS+12,    MONTH_Y_POS,    RSRC_STRING_SLASH);
            SysPostMessage(7,iDayMode,MONTH_X_POS+16,MONTH_Y_POS,iDays,2,'0');
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,    MONTH_X_POS+28,    MONTH_Y_POS,    RSRC_STRING_SLASH);
            SysPostMessage(7,iYearMode,MONTH_X_POS+32,MONTH_Y_POS,iYears+2000,4,'0');
             */
            break;
      
        case DATEFORMAT_DDMMYYYY:
            iLeftMode = iDayMode;
            iLeftNum = iDays;
            iCenterMode = iMonthMode;
            iCenterNum = iMonths;
            iRightMode = iYearMode;
            iRightNum = iYears;
            /*    
               SysPostMessage(7,iDayMode,MONTH_X_POS,MONTH_Y_POS,iDays,2,'0');
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,    MONTH_X_POS+12,    MONTH_Y_POS,    RSRC_STRING_SLASH);
            SysPostMessage(7,iMonthMode,MONTH_X_POS+16,MONTH_Y_POS,iMonths,2,'0');
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,    MONTH_X_POS+28,    MONTH_Y_POS,    RSRC_STRING_SLASH);
            SysPostMessage(7,iYearMode,MONTH_X_POS+32,MONTH_Y_POS,iYears+2000,4,'0');
            */     
            break;
        
        case DATEFORMAT_YYYYMMDD:
            iLeftMode = iYearMode;
            iLeftNum = iYears;
            iLeftLength    = 4;
            iSlashPos = MONTH_X_POS+24;
            iCenterMode = iMonthMode;
            iCenterNum = iMonths;
            iRightMode = iDayMode;
            iRightNum = iDays;
            iRightLength = 2;
            /*
               SysPostMessage(7,iYearMode,MONTH_X_POS,MONTH_Y_POS,iYears+2000,4,'0');
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,    MONTH_X_POS+24,    MONTH_Y_POS,    RSRC_STRING_SLASH);
            SysPostMessage(7,iMonthMode,MONTH_X_POS+28,MONTH_Y_POS,iMonths,2,'0');
            SysPostMessage(5,LCD_PRINT_STRING_RSRC,    MONTH_X_POS+40,    MONTH_Y_POS,    RSRC_STRING_SLASH);
            SysPostMessage(7,iDayMode,MONTH_X_POS+44,MONTH_Y_POS,iDays,2,'0');
           */    break;
        }
    if((iCursorPosition==MONTH_SELECT) || (iCursorPosition==DAY_SELECT) || (iCursorPosition==YEAR_SELECT))
    {    
        PrintNumberSetDate(iLeftMode,MONTH_X_POS,MONTH_Y_POS,iLeftNum,iLeftLength,'0');
        PrintNumberSetDate(iCenterMode,iSlashPos+4,MONTH_Y_POS,iCenterNum,2,'0');
        PrintNumberSetDate(iRightMode,iSlashPos+22,MONTH_Y_POS,iRightNum,iRightLength,'0');
        SysWaitOnEvent(0,0,0);
    }
    else if((iPreviousCursorPosition==MONTH_SELECT) || (iPreviousCursorPosition==DAY_SELECT) || (iPreviousCursorPosition==YEAR_SELECT) ||(iCursorPosition==DATE_SELECT_ALL))           
    {
        PrintNumberSetDate(iLeftMode,MONTH_X_POS,MONTH_Y_POS,iLeftNum,iLeftLength,'0');
        PrintNumberSetDate(iCenterMode,iSlashPos+4,MONTH_Y_POS,iCenterNum,2,'0');
        PrintNumberSetDate(iRightMode,iSlashPos+22,MONTH_Y_POS,iRightNum,iRightLength,'0');   
        SysWaitOnEvent(0,0,0);
    }

    
    // Display format selected by user //
    switch(iDateFormat) { 
        case DATEFORMAT_MMDDYYYY:
            iFormatString = RSRC_STRING_MMDDYYYY;
            break;
        case DATEFORMAT_DDMMYYYY:
            iFormatString = RSRC_STRING_DDMMYYYY;
            break;
        case DATEFORMAT_YYYYMMDD:
            iFormatString = RSRC_STRING_YYYYMMDD;
            break;
    }
    if(iCursorPosition==DATEFORMAT_SELECT)
        PrintStringSetDate(iFormatMode,FORMAT_DATE_X_POS+32,FORMAT_DATE_Y_POS,iFormatString);  // inverted text
    else if((iPreviousCursorPosition==DATEFORMAT_SELECT)||(iCursorPosition==DATE_SELECT_ALL))
        PrintStringSetDate(LCD_PRINT_STRING_RSRC,FORMAT_DATE_X_POS+32,FORMAT_DATE_Y_POS,iFormatString);  // will be non-inverted from init
    
        
    // Display OK //
    if(iCursorPosition==DATEOK_SELECT)
        PrintStringSetDate(iOkMode,OK_DATE_X_POS,OK_DATE_Y_POS,RSRC_STRING_OK);  // inverted text
    else if((iPreviousCursorPosition==DATEOK_SELECT)||(iCursorPosition==DATE_SELECT_ALL))
        PrintStringSetDate(LCD_PRINT_STRING_RSRC,OK_DATE_X_POS,OK_DATE_Y_POS,RSRC_STRING_OK);  // non-inverted from init
    
    // Display Cancel //
    if((iCursorPosition==DATECANCEL_SELECT)||(iCursorPosition==DATE_SELECT_ALL)) // we want this to be initial position
        PrintStringSetDate(iCancelMode,OK_DATE_X_POS+32,OK_DATE_Y_POS,RSRC_STRING_CANCEL);  // inverted text
    else if(iPreviousCursorPosition==DATECANCEL_SELECT)
        PrintStringSetDate(LCD_PRINT_STRING_RSRC,OK_DATE_X_POS+32,OK_DATE_Y_POS,RSRC_STRING_CANCEL);  // non-inverted from init
    SysWaitOnEvent(0,0,0);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void DisplayDateFirst(void)                            
//
//   Type:          Function
//
//   Description:   Displays the static text and the initial values
//
//   Inputs:        
//
//   Outputs:       
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void DisplayDateFirst(int iDateFormat)
{
    int iSlashPos;
    
    if(iDateFormat==DATEFORMAT_YYYYMMDD)
        iSlashPos = MONTH_X_POS+24;
    else
        iSlashPos = MONTH_X_POS+12;
    // display Title
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SET_DATE_TITLE_X_POS,SET_DATE_TITLE_Y_POS,RSRC_SET_DATE_TITLE);
    // Display Slashes
    SysPostMessage(6,LCD_CLEAR_RANGE,MONTH_X_POS,MONTH_Y_POS,72,8);
    PrintStringSetDate(LCD_PRINT_STRING_RSRC,iSlashPos,MONTH_Y_POS,RSRC_STRING_SLASH);
    PrintStringSetDate(LCD_PRINT_STRING_RSRC,iSlashPos+16,MONTH_Y_POS,RSRC_STRING_SLASH);
    // Display "Format"  //
    PrintStringSetDate(LCD_PRINT_STRING_RSRC,FORMAT_DATE_X_POS,FORMAT_DATE_Y_POS,RSRC_STRING_FORMAT);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant DaysInMonth(int iMonths,int iYears)                            
//
//   Type:          Function
//
//   Description:   returns the number of days in a month, given month and year
//
//   Inputs:        iMonths, iYears
//
//   Outputs:       Returns the number of days in a month
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////

_reentrant int DaysInMonth(int iMonths,int iYears) {
    int iDays = 0;

    switch(iMonths) {
        case JAN: iDays = 31; break;
        case FEB: if(iYears%4 == 0) iDays = 29;
                  else iDays = 28; break;
        case MAR: iDays = 31; break;
        case APR: iDays = 30; break;
        case MAY: iDays = 31; break;
        case JUN: iDays = 30; break;
        case JUL: iDays = 31; break;
        case AUG: iDays = 31; break;
        case SEP: iDays = 30; break;
        case OCT: iDays = 31; break;
        case NOV: iDays = 30; break;
        case DEC: iDays = 31; break;
    }

    return iDays;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant DaysFromDate(int iDays,int iMonths, iYears)
//
//   Type:          Function
//
//   Description:   returns the number of the day in the year from the date
//
//   Inputs:        iDays,iMonths,iYears
//
//   Outputs:       Returns the number of the day in the year
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////      

_reentrant int DaysFromDate(int iDays,int iMonths,int iYears) {
    int iTotalDays = 0;

    switch(iYears%4) {
    case 0:                    // leapyear
        switch(iMonths) {
        case JAN: iTotalDays = iDays;    break;
        case FEB: iTotalDays = iDays + 31; break;
        case MAR: iTotalDays = iDays + 60; break;
        case APR: iTotalDays = iDays + 91; break;
        case MAY: iTotalDays = iDays + 121; break;
        case JUN: iTotalDays = iDays + 152; break;
        case JUL: iTotalDays = iDays + 182; break;
        case AUG: iTotalDays = iDays + 213; break;
        case SEP: iTotalDays = iDays + 244; break;
        case OCT: iTotalDays = iDays + 274; break;
        case NOV: iTotalDays = iDays + 305; break;
        case DEC: iTotalDays = iDays + 335; break;
        }
    break;
    
    default:                // non-leap year
        switch(iMonths) {
        case JAN: iTotalDays = iDays;    break;
        case FEB: iTotalDays = iDays + 31; break;
        case MAR: iTotalDays = iDays + 59; break;
        case APR: iTotalDays = iDays + 90; break;
        case MAY: iTotalDays = iDays + 120; break;
        case JUN: iTotalDays = iDays + 151; break;
        case JUL: iTotalDays = iDays + 181; break;
        case AUG: iTotalDays = iDays + 212; break;
        case SEP: iTotalDays = iDays + 243; break;
        case OCT: iTotalDays = iDays + 273; break;
        case NOV: iTotalDays = iDays + 304; break;
        case DEC: iTotalDays = iDays + 334; break; 
        }
    break;
    }
    return iTotalDays;
}

/********************************************************
/    Function:    DisplaySaveChangesDate
/    Inputs:        iSaveChangesSelected, the save changes option
/                the user currenly has selected
/    Outputs:    Displays a bitmap with the save option
/                highlighted
/    Description: This function displays the window to ask
/                the user if changes to date are to be saved,
/                or to just cancel. Window appears if user
/                holds MENU while in set date menu.     
/********************************************************/
_reentrant void DisplaySaveChangesDate(int iSaveOption) 
{
    int DisplayItem = 0;
    switch(iSaveOption) {
    case SETDATE_YES:
        //SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SAVE_CHANGES_DATE_X_POS,SAVE_CHANGES_DATE_Y_POS,RSRC_SAVE_CHANGES_YES);
        DisplayItem = RSRC_SAVE_CHANGES_YES;
        break;
    case SETDATE_NO:
        //SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SAVE_CHANGES_DATE_X_POS,SAVE_CHANGES_DATE_Y_POS,RSRC_SAVE_CHANGES_NO);
        DisplayItem = RSRC_SAVE_CHANGES_NO;
        break;
    case SETDATE_CANCEL:
        //SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SAVE_CHANGES_DATE_X_POS,SAVE_CHANGES_DATE_Y_POS,RSRC_SAVE_CHANGES_CANCEL);
        DisplayItem = RSRC_SAVE_CHANGES_CANCEL;
        break;
    case SETDATE_CLEAR:
        //SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SAVE_CHANGES_DATE_X_POS,SAVE_CHANGES_DATE_Y_POS,RSRC_SAVE_CHANGES_CLEAR);
        DisplayItem = RSRC_SAVE_CHANGES_CLEAR;
        break;
    default:  // should not happen
        break;
    }
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SAVE_CHANGES_DATE_X_POS,SAVE_CHANGES_DATE_Y_POS,DisplayItem);
}

_reentrant void PrintStringSetDate(int iMode,int iXpos,int iYpos,int iString)
{
	SysPostMessage(5,iMode,iXpos,iYpos,iString);
}

_reentrant void PrintNumberSetDate(int iMode,int iXpos,int iYpos,int iNumber,int iWidth,int iFiller)
{
    SysPostMessage(7,iMode,iXpos,iYpos,iNumber,iWidth,iFiller);
}


// Use SaveRange macro to save a copy of global variable addresses as well as
// reserve space for their values in section "Player_settings_Y".  All of which
// will be used at shutdown to save user settings in the Settings.dat file.

#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_iDateFormat

    SaveRange Fg_iDateFormat,1         
    
#pragma endasm
 