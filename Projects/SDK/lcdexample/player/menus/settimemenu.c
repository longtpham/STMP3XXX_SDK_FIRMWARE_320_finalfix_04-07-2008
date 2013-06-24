////////////////////////////////////////////////////////////////////////////////
// SigmaTel Inc. Copyright 2003
// Filename: SetTimeMenu.c (LCD version)
// Description: User interface to change the time and set the RTC
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
#include "settimemenu.h"
//Stmp00008949
#include "FileSystem.h"
#include "playerlib.h"
#include "sysmem.h"

extern void SaveUserTime(); //Stmp00008949

// Local function prototypes
_reentrant void SetTime(int iHours,int iMinutes,int iSeconds);
_reentrant void DisplayChangeTime(int iCursorPosition ,int iPreviousCursorPosition,int iInvert, int iHours,int iMinutes,int iSeconds, int l_iTimeFormat);
_reentrant void DisplaySaveChangesTime(int iSaveOption);
_reentrant void DisplayTimeFirst(void);
_reentrant void PrintStringSetTime(int iMode,int iXpos,int iYpos,int iString);                       
_reentrant void PrintNumberSetTime(int iMode,int iXpos,int iYpos,int iNumber,int iWidth,int iFiller);




/****  Needs to be saved to user settings *****/
int g_iTimeFormat; 	// should be current time format from saved user settings, g_iTimeFormat is saved, l_iTimeFormat 
					// is used to allow	user to change TimeFormat but is not saved until user confirms						 

extern TimeDateValues_type g_iCurrentRTCValues;

unsigned long g_Usertime;
long g_UserTimeoffset;
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant SetTimeMenu(int a, int b, int *pPtr)
//
//   Type:          Function (set time menu entry point)
//
//   Description:   This function is the entry point for the set time menu manager.
//
//   Inputs:        None
//
//   Outputs:       Menu to enter upon exiting set time menu
//                      if PH_MENU: return to Music menu
//                      else : return to Settings menu selection
//
/*   Notes:         
*/
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant SetTimeMenu(int a, int b, int *pPtr)
{
	BOOL bDone = FALSE;
	BOOL bSendToPlayerStateMachine;
	union DisplayHints DisplayHint;

	INT l_iTimeFormat = g_iTimeFormat; // use local temp TimeFormat so format can have option to be saved

	INT iHours;
	INT iMinutes;
    INT iSeconds;

    INT bBlink = FALSE;
    INT bTimeModify_mode = FALSE;
	INT bTimeModified = FALSE; // Stmp00008949
    INT bSaveChanges_mode = FALSE;

    INT iEvent;
    INT iNextMenu = MENU_SET_TIME;
    INT iCursorPosition = TIMECANCEL_SELECT;
    INT iPreviousCursorPosition = TIMEOK_SELECT;
    INT iSaveChangesSelected = SETTIME_CANCEL;
    INT iCancelConfirm = FALSE;

    // This is required to get the message into the PlayerStateMachine (PSM)
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    //b;   // avoid warnings for unused

    // We need to refresh the screen since entering the menu
    DisplayHint.I = TIME_DATE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
    SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
    DisplayHint.I = 0;
    


    SysCallFunction(RSRC_TIMEDATE_MENU_CODE_BANK,UpdateTime,0,0,0);
    iHours = g_iCurrentRTCValues.B.m_iHours;
    iMinutes = g_iCurrentRTCValues.B.m_iMinutes;
    iSeconds = g_iCurrentRTCValues.B.m_iSeconds;

    DisplayTimeFirst();
    DisplayChangeTime(TIME_SELECT_ALL,iPreviousCursorPosition,TRUE,iHours,iMinutes,iSeconds,l_iTimeFormat);
    iPreviousCursorPosition = iCursorPosition;
    while(!bDone) 
    {
    // Mask out all display hints that aren't aimed toward the TIME DATE menu
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
                    if(!bSaveChanges_mode) // only blink if "Save Changes?" is not on the screen
                        DisplayChangeTime(iCursorPosition,iPreviousCursorPosition,bBlink,iHours,iMinutes,iSeconds,l_iTimeFormat);                    
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
//                case PH_VOL_UP:       // instead, let PSM adjust the volume
//                case PH_VOL_DOWN:
//                    break;

                case PR_FF:
                case PH_FF:
                    if(bSaveChanges_mode == TRUE) {
                        iSaveChangesSelected++;
                        if(iSaveChangesSelected > SETTIME_LAST_SAVE_CHANGES)
                            iSaveChangesSelected = SETTIME_FIRST_SAVE_CHANGES;
                        DisplaySaveChangesTime(iSaveChangesSelected);
                    }
                    else if(bTimeModify_mode == FALSE) {// move cursor to next time parameter
                        iCursorPosition++;
                        if(iCursorPosition > LAST_TIME_SELECT)
                        iCursorPosition = FIRST_TIME_SELECT;
                        DisplayChangeTime(iCursorPosition,iPreviousCursorPosition,TRUE,iHours,iMinutes,iSeconds,l_iTimeFormat);
                    }
                    else { //bTimeModify_mode == TRUE , MENU button already pressed to select parameter, now adjust parameter
                        switch(iCursorPosition){
                        case HOUR_SELECT:
                            iHours++;
                            if(iHours > 23)
                                iHours = 0;
                            break;
                        case MINUTE_SELECT:
                            iMinutes++;
                            if(iMinutes > 59)
                                iMinutes = 0;
                            break;
                           case TIMEFORMAT_SELECT:
                            l_iTimeFormat++;
                            if(l_iTimeFormat > TIMEFORMAT_24HOUR)
                                l_iTimeFormat = TIMEFORMAT_12HOUR;
                            break;        
                        } // end switch(iCursorPosition)
                        DisplayChangeTime(iCursorPosition,iPreviousCursorPosition,bBlink,iHours,iMinutes,iSeconds,l_iTimeFormat);
                    }
                    bSendToPlayerStateMachine = FALSE;         // no need to send to PSM
                    break;

                case PR_RW:
                case PH_RW:
                    if(bSaveChanges_mode == TRUE) {
                        iSaveChangesSelected--;
                        if(iSaveChangesSelected < SETTIME_FIRST_SAVE_CHANGES)
                            iSaveChangesSelected = SETTIME_LAST_SAVE_CHANGES;
                        DisplaySaveChangesTime(iSaveChangesSelected);
                    }
                    else if(bTimeModify_mode == FALSE) {
                        iCursorPosition--;
                        if(iCursorPosition < FIRST_TIME_SELECT)
                        iCursorPosition = LAST_TIME_SELECT;
                        DisplayChangeTime(iCursorPosition,iPreviousCursorPosition,TRUE,iHours,iMinutes,iSeconds,l_iTimeFormat);
                    }
                    else {// bTimeModify_mode == TRUE and bSaveChange_mode == FALSE;
                          // MENU button already pressed to select parameter, now adjust parameter
                        switch(iCursorPosition){
                        case HOUR_SELECT:
                            iHours--;
                            if(iHours < 0)
                                iHours = 23;
                            break;
                        case MINUTE_SELECT:
                            iMinutes--;
                            if(iMinutes < 0)
                                iMinutes = 59;
                            break;
                          case TIMEFORMAT_SELECT:
                            l_iTimeFormat--;
                            if(l_iTimeFormat < TIMEFORMAT_12HOUR)
                                l_iTimeFormat = TIMEFORMAT_24HOUR;
                            break;
                        } // end switch(iCursorPosition)
                        DisplayChangeTime(iCursorPosition,iPreviousCursorPosition,bBlink,iHours,iMinutes,iSeconds,l_iTimeFormat);
                    }
                    bSendToPlayerStateMachine = FALSE;         // no need to send to PSM
                    break;

                case PH_MENU:
                    // Allow user to escape to Music mode,
                    // but first, ask if changes need to be saved.
                                        
                    if(!bSaveChanges_mode) {
                        DisplaySaveChangesTime(SETTIME_CANCEL); // default to "Cancel"
                        bSendToPlayerStateMachine = TRUE;
                        bSaveChanges_mode=TRUE;
                        g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    }
                    else {
                        g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                        bSendToPlayerStateMachine = FALSE;
                        bDone = TRUE;    
                    }
                    break;
                                                                
                case PR_MENU:
                    if(!bSaveChanges_mode) {
                        switch(iCursorPosition) {
                        case TIMEOK_SELECT:    // exit and save
                            g_iTimeFormat = l_iTimeFormat; // save format to global
                            SetTime(iHours,iMinutes,iSeconds);
                            bDone = TRUE;
                            break;
                        case TIMECANCEL_SELECT: // exit without saving
                            bDone = TRUE;                            
                            break;
                        default:
                            // Toggle the Time-modify mode
                            if(bTimeModify_mode) { 
                                bTimeModify_mode = FALSE;
                                bBlink = FALSE;    // stop blinking
                                SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
                                if(iCursorPosition==TIMEFORMAT_SELECT)  // update the format if it changed
                                    DisplayChangeTime(TIMEFORMAT_SELECT,HOUR_SELECT,TRUE,iHours,iMinutes,iSeconds,l_iTimeFormat);
                            }
                            else {// visually indicate the time parameter, "blink" the parameter
                                bTimeModify_mode = TRUE;    // start blinking
                                bTimeModified = TRUE;   // Stmp00008949
                                bBlink = TRUE;
                                SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,500,MENU_MSG_ANIMATE);
                            }
                            SysWaitOnEvent(0,0,0);    // let system process SOFT_TIMER_xxx_TIMER msg (KILL or SET)
                            break;
            
                        }
                    } // end if(!bSaveChanges_mode)
                    else{    // bSaveChanges_mode == TRUE
                        
                        switch(iSaveChangesSelected) {
                        case SETTIME_YES:  // Exit and save
                            g_iTimeFormat = l_iTimeFormat;
                            SetTime(iHours,iMinutes,iSeconds);
                            bSendToPlayerStateMachine = TRUE;
                            g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                            g_MenuFlags.MENU_FLAG_SAVE_CHANGES = TRUE;
                            bDone = TRUE;
                            break;
                        case SETTIME_NO:  // Exit without saving
                            bSendToPlayerStateMachine = FALSE;
                            g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                            g_MenuFlags.MENU_FLAG_SAVE_CHANGES = TRUE;
                            bDone = TRUE;
                            break;
                        case SETTIME_CANCEL:  // Don't exit or save, return to set time menu
                            if(iCancelConfirm){
                                bSaveChanges_mode = FALSE;
                                iCancelConfirm = FALSE;
                                iCursorPosition = TIMECANCEL_SELECT;
                                DisplaySaveChangesTime(SETTIME_CLEAR);
                                DisplayTimeFirst();
                                DisplayChangeTime(TIME_SELECT_ALL,iPreviousCursorPosition,TRUE,iHours,iMinutes,iSeconds,l_iTimeFormat);
                            }
                            else
                                iCancelConfirm = TRUE;
                            break;
                        default:  // should never happen
                            break;
                        }
                         
                    }        // end else{ bSaveChanges_mode == TRUE
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

        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
        
        iPreviousCursorPosition = iCursorPosition;    
    } // end while(!bDone)

    if(bTimeModify_mode)     // (SDK2.520)
    {
        SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
        SysWaitOnEvent(0,0,0);      // let system process SOFT_TIMER_KILL_TIMER msg
    }
    //Stmp00008949
    if(bTimeModified == TRUE)
    {   if(g_wDecoderSR&DECODER_STOPPED)
        {   SysLoadFATWrite();
            SaveUserTime();
            SysUnLoadFATWrite();
        }
    }
    return iNextMenu;
} // end of SetTimeMenu()


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void SetTime(int iHours,int iMinutes,int iSeconds)
//
//   Type:          Function
//
//   Description:    Set RTC to values passed in   
//
//   Inputs:        iHours,iMinutes,iSeconds        
//
//   Outputs:       Sets RTC with new time
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////

_reentrant void SetTime(int iHours,int iMinutes,int iSeconds){
	
	int iTotalDays;
    long lRTCTime;
    long Usertime, Diff;
	 
	// Save date  (truncate RTC value to leave number of days passed) //
	lRTCTime = ReadRTC();
	lRTCTime /= 1000;  	//now has seconds
    
    lRTCTime += g_UserTimeoffset; // Current Time According to the user.
    
	iTotalDays = (unsigned int) (lRTCTime/86400);	//now has number of days that have passed (86400 seconds per day)
	
	// Write new value to RTC //	
    Usertime=((unsigned long) iTotalDays * 86400)+ ((unsigned long) iHours * 3600)+ ((unsigned long) iMinutes * 60)+ ((unsigned long) iSeconds);

    if(lRTCTime >= Usertime) 
    {
        Diff = -(lRTCTime - Usertime);// - (unsigned long) iTotalDays * 86400);
    } 
    else
    {
        Diff = (Usertime - lRTCTime); //- (unsigned long) iTotalDays * 86400);    
    }
    g_UserTimeoffset += Diff;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void DisplayChangeTime(int iCursorPosition ,int iInvert, int iHours,
//                                                int iMinutes,int iSeconds,int l_iTimeFormat)
//
//   Type:          Function
//
//   Description:   Displays the day of week, date and format for user to change and set.  
//
//   Inputs:        iCursorPosition - indicates which band gain to display
//                    bDisplayLabels - indicates whether or not to display dB level & Hertz text
//                    bInvert - determines whether or not to invert the gain field ??
//
//   Outputs:       Updates the display to reflect user selections
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////

_reentrant void DisplayChangeTime(int iCursorPosition ,int iPreviousCursorPosition,int iInvert, int iHours,int iMinutes,int iSeconds, int l_iTimeFormat) {
    int iHourMode =     LCD_PRINT_NUMBER;
    int iMinuteMode =   LCD_PRINT_NUMBER;
    int iSecondMode =   LCD_PRINT_NUMBER;
    int iFormatMode =   LCD_PRINT_STRING_RSRC;
    int iOkMode =       LCD_PRINT_STRING_RSRC;        
    int iCancelMode =   LCD_PRINT_STRING_RSRC;

    switch(iCursorPosition){
        case HOUR_SELECT:
            if(iInvert)    iHourMode = LCD_PRINT_NUMBER_INV;
            break;
        case MINUTE_SELECT:
            if(iInvert) iMinuteMode =  LCD_PRINT_NUMBER_INV;
            break;
        case TIMEFORMAT_SELECT:
            if(iInvert) iFormatMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        case TIMEOK_SELECT:
            if(iInvert) iOkMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        case TIMECANCEL_SELECT:
            if(iInvert) iCancelMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        case TIME_SELECT_ALL:
            iCancelMode = LCD_PRINT_STRING_INV_RSRC;
            break;
        default:
            break;
        }

    

    if(iCursorPosition==HOUR_SELECT)
    {
        switch(l_iTimeFormat) {
        case TIMEFORMAT_12HOUR:
        // Display Hours //
        
            if(iHours == 0)
                PrintNumberSetTime(iHourMode,HOUR_X_POS,HOUR_Y_POS,12,2,'0');
            else if(iHours > 12)
                PrintNumberSetTime(iHourMode,HOUR_X_POS,HOUR_Y_POS,iHours-12,2,'0');
            else                                               
               PrintNumberSetTime(iHourMode,HOUR_X_POS,HOUR_Y_POS,iHours,2,'0');
        
            // Display AM/PM //        
            if(iHours >= 12)
                PrintStringSetTime(LCD_PRINT_STRING_RSRC,HOUR_X_POS+32,HOUR_Y_POS,RSRC_STRING_PM);
            else
                PrintStringSetTime(LCD_PRINT_STRING_RSRC,HOUR_X_POS+32,HOUR_Y_POS,RSRC_STRING_AM);
            break;

        case TIMEFORMAT_24HOUR:
            // Display hours and minutes //
            PrintNumberSetTime(iHourMode,HOUR_X_POS,HOUR_Y_POS,iHours,2,'0');
            PrintNumberSetTime(iMinuteMode,HOUR_X_POS+16,HOUR_Y_POS,iMinutes,2,'0');
    
            // Clear AM/PM
            PrintStringSetTime(LCD_PRINT_STRING_RSRC,HOUR_X_POS+32,HOUR_Y_POS,RSRC_STRING_AMCLEAR);
            break;
        }
    }
    else if((iPreviousCursorPosition==HOUR_SELECT)||(iCursorPosition==TIME_SELECT_ALL))
    {
        switch(l_iTimeFormat) {
        case TIMEFORMAT_12HOUR:
        // Display Hours //
        
            if(iHours == 0)
                PrintNumberSetTime(LCD_PRINT_NUMBER,HOUR_X_POS,HOUR_Y_POS,12,2,'0');
            else if(iHours > 12)
                PrintNumberSetTime(LCD_PRINT_NUMBER,HOUR_X_POS,HOUR_Y_POS,iHours-12,2,'0');
            else                                               
               PrintNumberSetTime(LCD_PRINT_NUMBER,HOUR_X_POS,HOUR_Y_POS,iHours,2,'0');
        
            // Display AM/PM //        
            if(iHours >= 12)
                PrintStringSetTime(LCD_PRINT_STRING_RSRC,HOUR_X_POS+32,HOUR_Y_POS,RSRC_STRING_PM);
            else
                PrintStringSetTime(LCD_PRINT_STRING_RSRC,HOUR_X_POS+32,HOUR_Y_POS,RSRC_STRING_AM);
            break;

        case TIMEFORMAT_24HOUR:
            // Display hours and minutes //
            PrintNumberSetTime(LCD_PRINT_NUMBER,HOUR_X_POS,HOUR_Y_POS,iHours,2,'0');
            PrintNumberSetTime(LCD_PRINT_NUMBER,HOUR_X_POS+16,HOUR_Y_POS,iMinutes,2,'0');
    
            // Clear AM/PM
            PrintStringSetTime(LCD_PRINT_STRING_RSRC,HOUR_X_POS+32,HOUR_Y_POS,RSRC_STRING_AMCLEAR);
            break;
        }
    }
    SysWaitOnEvent(0,0,0);
    
    // Display minutes//    
    if(iCursorPosition==MINUTE_SELECT)
        PrintNumberSetTime(iMinuteMode,HOUR_X_POS+16,HOUR_Y_POS,iMinutes,2,'0');    
    else if((iPreviousCursorPosition==MINUTE_SELECT)||(iCursorPosition==TIME_SELECT_ALL))    
        PrintNumberSetTime(LCD_PRINT_NUMBER,HOUR_X_POS+16,HOUR_Y_POS,iMinutes,2,'0');

    // Display format //
    if(iCursorPosition==TIMEFORMAT_SELECT)
    {
        if(l_iTimeFormat == TIMEFORMAT_12HOUR)
            PrintStringSetTime(iFormatMode,FORMAT_X_POS+32,FORMAT_Y_POS,RSRC_STRING_12HOUR);
        else
            PrintStringSetTime(iFormatMode,FORMAT_X_POS+32,FORMAT_Y_POS,RSRC_STRING_24HOUR);
    }
    else if((iPreviousCursorPosition==TIMEFORMAT_SELECT)||(iCursorPosition==TIME_SELECT_ALL))    
    {
        if(l_iTimeFormat == TIMEFORMAT_12HOUR)
            PrintStringSetTime(LCD_PRINT_STRING_RSRC,FORMAT_X_POS+32,FORMAT_Y_POS,RSRC_STRING_12HOUR);
        else
            PrintStringSetTime(LCD_PRINT_STRING_RSRC,FORMAT_X_POS+32,FORMAT_Y_POS,RSRC_STRING_24HOUR);
    }
    
    // Display OK //  
    if(iCursorPosition==TIMEOK_SELECT)
        PrintStringSetTime(iOkMode,OK_TIME_X_POS,OK_TIME_Y_POS,RSRC_STRING_OK);
    else if((iPreviousCursorPosition==TIMEOK_SELECT)||(iCursorPosition==TIME_SELECT_ALL))    
        PrintStringSetTime(LCD_PRINT_STRING_RSRC,OK_TIME_X_POS,OK_TIME_Y_POS,RSRC_STRING_OK);
    
    // Display Cancel //
    if((iCursorPosition==TIMECANCEL_SELECT)||(iCursorPosition==TIME_SELECT_ALL))
        PrintStringSetTime(iCancelMode,OK_TIME_X_POS+32,OK_TIME_Y_POS,RSRC_STRING_CANCEL);
    else if(iPreviousCursorPosition==TIMECANCEL_SELECT) 
        PrintStringSetTime(LCD_PRINT_STRING_RSRC,OK_TIME_X_POS+32,OK_TIME_Y_POS,RSRC_STRING_CANCEL);

    SysWaitOnEvent(0,0,0);    
}
/********************************************************
/    Function:      DisplayTimeFirst(void)
/    Inputs:     
/                
/    Outputs:    
/                
/    Description:   Displays the static text on the LCD.  
/                
/                
/                
/********************************************************/
_reentrant void DisplayTimeFirst(void)
{
    // display Title                                                                                     
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SET_TIME_TITLE_X_POS,SET_TIME_TITLE_Y_POS,RSRC_SET_TIME_TITLE);    
    // Display "Format" and format selected//
    PrintStringSetTime(LCD_PRINT_STRING_RSRC,FORMAT_X_POS,FORMAT_Y_POS,RSRC_STRING_FORMAT);    
    // Display Colon
    SysPostMessage(5,LCD_PRINT_UNICODE_CHAR,HOUR_X_POS+13,HOUR_Y_POS,':');
    //PrintStringSetTime(LCD_PRINT_STRING_RSRC,HOUR_X_POS+13,HOUR_Y_POS-2,RSRC_STRING_COLON);
        
}

/********************************************************
/    Function:    DisplaySaveChangesTime
/    Inputs:        iSaveChangesSelected, the save changes option
/                the user currenly has selected
/    Outputs:    Displays a bitmap with the save option
/                highlighted
/    Description: This function displays the window to ask
/                the user if changes to time are to be saved,
/                or to just cancel. Window appears if user
/                holds MENU while in set time menu.     
/********************************************************/
_reentrant void DisplaySaveChangesTime(int iSaveOption) 
{
    int iSaveText = 0;

    switch(iSaveOption) {
    case SETTIME_YES:
        iSaveText = RSRC_SAVE_CHANGES_YES;
        break;
    case SETTIME_NO:
        iSaveText = RSRC_SAVE_CHANGES_NO;
        break;
    case SETTIME_CANCEL:
        iSaveText = RSRC_SAVE_CHANGES_CANCEL;
        break;
    case SETTIME_CLEAR:
        iSaveText = RSRC_SAVE_CHANGES_CLEAR;
        break;
    default:  // should not happen
        break;
    }
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,SAVE_CHANGES_TIME_X_POS,SAVE_CHANGES_TIME_Y_POS,iSaveText);

}
    

_reentrant void PrintStringSetTime(int iMode,int iXpos,int iYpos,int iString)
{
	SysPostMessage(5,iMode,iXpos,iYpos,iString);
}

_reentrant void PrintNumberSetTime(int iMode,int iXpos,int iYpos,int iNumber,int iWidth,int iFiller)
{
    SysPostMessage(7,iMode,iXpos,iYpos,iNumber,iWidth,iFiller);
}

