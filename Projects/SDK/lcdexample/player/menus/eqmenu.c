// Filename: EqMenu.c (LCD version)

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

// Local function prototypes
void _reentrant DisplayEqBar(INT iSelected, INT bInvert);
void _reentrant DisplayEqStats(INT iSelected, INT bDisplayLabels);

// Global variables
int g_iEqSetting = FIRST_EQ;   // user-selected EQ setting (saved in Settings.dat)
//       org     y,".ydataeqmenu",init:     ; (cluster: Y_Extra)
// Fg_iEqSetting:
//       dc      $000000       ; initialization data (zero) located in Y memory

INT _Y g_iCustomEqBand_gain[NUM_EQ_BANDS] = { 0,0,0,0,0 };  // five EQ Bands supported
//       org     y,".ydataeqmenu",init:     ; (cluster: Y_Extra)
// Fg_iCustomEqBand_gain:
//       dc      $000000,$000000,$000000,$000000,$000000   ; initialization data (zero)


#define FIRST_X_POS      5
#define SLIDER_SPACING   18
#define GEQ_NORMAL       FALSE
#define GEQ_INVERT       TRUE


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant EqMenu(int a, int b, int *pPtr)
//
//   Type:          Function (EQ menu entry point)
//
//   Description:   This function is the entry point for the EQ menu manager.
//
//   Inputs:        None
//
//   Outputs:       Menu to enter upon exiting EQ menu
//                      if PH_MENU: return to Music menu
//                      else : return to Settings menu selection (predefined modes only)
//
/*   Notes:         
  For custom GEQ mode where five EqBand dB gain levels are user-selectable:
   =========================================
   ||  <first row icons>                  || <--play state,shuffle mode,EQ mode,lock,battery level,..
   ||  <second row icons>                 || <--volume level, playlist,..
   ||     |      |      |      |      |   ||
   ||     |      |      |      |      |   ||
   ||    -+-     |      |      |     ===  || <--selected gain will appear in Inverse
   ||     |     -+-     |     -+-     |   ||    (selected gain will "blink"
   ||     |      |     -+-     |      |   ||     when in Band-gain-modify mode)
   ||     |      |      |      |      |   ||
   ||     |      |      |      |      |   ||
   ||     |      |      |      |      |   ||
   || dB: -nn.n                 120000 Hz || <--(-14)dB to 14dB, 80 to 120000Hz
   =========================================

  For predefined GEQ mode where five EqBand dB gain levels are fixed:
   =========================================
   ||  <first row icons>                  ||
   ||  <second row icons>                 ||
   ||     |      |      |      |      |   ||
   ||     |      |      |      |      |   ||
   ||    -+-     |      |      |     -+-  ||
   ||     |     -+-     |     -+-     |   ||
   ||     |      |     -+-     |      |   ||
   ||     |      |      |      |      |   ||
   ||     |      |      |      |      |   ||
   ||     |      |      |      |      |   ||
   ||                                     || <--no levels displayed
   =========================================

  Changes in EQ settings can be heard in real-time if a song is playing.
  The Play, Stop, Volume up, and Volume down buttons are not blocked. */
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant EqMenu(int a, int b, int *pPtr)
{
	BOOL bDone = FALSE;
	BOOL bSendToPlayerStateMachine;
	union DisplayHints DisplayHint;

	INT iSelectedBand = EQ_BAND_FIRST;    // (SDK2.520)
	INT bBandGainModify_mode = FALSE;
	INT bBlink = FALSE;
	INT iEvent;

	// This is required to get the message into the PlayerStateMachine (PSM)
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

	b;   // avoid warnings for unused

	// We need to refresh the screen since entering the menu
	DisplayHint.I = EQ_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
	SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
	DisplayHint.I = 0;
	
	SendEQ(g_iEqSetting,0,0);
	
	// Display all five EQ slider bars (do not invert)
	for(a = (NUM_EQ_BANDS - 1); a >= 0; a--)   // testing against '0' is more efficient
	{
            DisplayEqBar(a,GEQ_NORMAL);
	}
	if(g_iEqSetting == EQ_CUSTOM)
	{
            // Display dB level for the selected EQ slider bar (invert). 
            //warning: in sdk2.6 the following ln was removed so selected doesn't jump to 1st after mmc eject
            iSelectedBand = EQ_BAND_FIRST;
            DisplayEqBar(iSelectedBand,GEQ_INVERT);
            // Display bottom row info: dB level, center freq Hz (display labels)
            DisplayEqStats(iSelectedBand,TRUE);
	}

	while(!bDone) 
	{
		// Mask out all display hints that aren't aimed toward the EQ menu
		DisplayHint.I &= (EQ_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

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
		 // TOVERIFY2: This code is different from 2.600 in that it assumes that we will not support MMC and hence we do not need to fix
		 // the MMC removal problem on eq menu.

                 #if 0 // This segment is enabled in sdk2.600 and prevents an mmc bug but since we don't have mmc in sdk3.05 it is left off for now.
                  for(a = (NUM_EQ_BANDS - 1); a >= 0; a--)   // testing against '0' is more efficient
	          {
		         DisplayEqBar(a,GEQ_NORMAL);
	          }

	          if(g_iEqSetting == EQ_CUSTOM)
	          {
		         DisplayEqBar(iSelectedBand,GEQ_INVERT);
		         // Display bottom row info: dB level, center freq Hz (display labels)
		         DisplayEqStats(iSelectedBand,TRUE);
	          }
                 #endif

			bSendToPlayerStateMachine = FALSE;
			break;

		case EVENT_MESSAGE:              // (SDK2.520)
			switch(gEventInfo.Message.wMsgCommand)
			{
				case MENU_MSG_ANIMATE:   // process expiration of Blink timer
					bBlink ^= 1;         // toggle mode
					DisplayEqBar(iSelectedBand,bBlink);
					bSendToPlayerStateMachine = FALSE;
					break;
                  #if 0 // handle mmc eject like sdk2.6. Segment not enabled currently since we don't support MMC in sdk3.05
                  case MENU_MSG_MEDIA_CHANGED: 
  		            DisplayHint.bits.Disk ^= TRUE;	//Toggle icon screen display
                      break;
                  #endif
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
				case PR_EQ:      // (SDK2.520)
					// Allow the user to change the selected EQ (Rock, Pop, Custom, etc.)
					DisplayHint.bits.EQ = TRUE;

					// Make sure TIMER_ANIMATE is not running
					if(bBandGainModify_mode)
					{
						bBandGainModify_mode = FALSE;
						SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
//						SysWaitOnEvent(0,0,0);   // let system process its timer msg
						// ^--not needed here since called in following DisplayEqBar()
					}

					NextEQ(INCREMENT,0,0);       // select the next EQ
					for(a = (NUM_EQ_BANDS - 1); a >= 0; a--)
					{
						// Display all five EQ slider bars (do not invert)
						DisplayEqBar(a,GEQ_NORMAL);
					}

					if(g_iEqSetting == EQ_CUSTOM)
					{	// Prepare the display for user-definable EQ band gain mode
						// Display dB level for the first EQ slider bar (invert)
						iSelectedBand = EQ_BAND_FIRST;
						DisplayEqBar(iSelectedBand,GEQ_INVERT);
						// Display bottom row info: dB level, center freq Hz (display labels)
						DisplayEqStats(iSelectedBand,TRUE);
					}
					else
					{	// Clear the entire bottom row
						SysPostMessage(6,LCD_CLEAR_RANGE,0,56,LCD_SIZE_X,8);
						SysWaitOnEvent(0,0,0);    // let system process its display msg
					}
					bSendToPlayerStateMachine = FALSE;		
					break;

//				case PH_VOL_UP:       // instead, let PSM adjust the volume
//				case PH_VOL_DOWN:
//					break;

				case PR_FF:
				case PH_FF:
					// If EQ mode is not Custom, then ignore requests to change dB gain levels!
					if(g_iEqSetting == EQ_CUSTOM)
					{	// Depending on the EQ sub-mode, either increm dB gain level or select next EqBand!
						if(bBandGainModify_mode)
						{
//							DisplayEqBar(iSelectedBand,GEQ_NORMAL);  // redraw the old level
							g_iCustomEqBand_gain[iSelectedBand]++;
							if(g_iCustomEqBand_gain[iSelectedBand] > EQ_MAX_GAIN) {
								g_iCustomEqBand_gain[iSelectedBand] = EQ_MAX_GAIN;
							}
							SendEQ(EQ_CUSTOM,0,0);        // update EQ module in real time
						}
						else
						{
							DisplayEqBar(iSelectedBand,GEQ_NORMAL);  // redraw the old band
							iSelectedBand++;
							if(iSelectedBand > EQ_BAND_LAST) {
								iSelectedBand = EQ_BAND_FIRST;
							}
						}
						DisplayEqBar(iSelectedBand,GEQ_INVERT);      // draw the new level/band

						// Display bottom row info: dB level, center freq Hz
						DisplayEqStats(iSelectedBand,FALSE);   // assume labels have already been displayed
					}
					bSendToPlayerStateMachine = FALSE;         // no need to send to PSM
					break;

				case PR_RW:
				case PH_RW:
					// If sub-mode is not Custom, then ignore requests to change dB gain levels!
					if(g_iEqSetting == EQ_CUSTOM)
					{	// Depending on the EQ sub-mode, either decrem dB gain level or select previous EqBand!
						if(bBandGainModify_mode)
						{
//							DisplayEqBar(iSelectedBand,GEQ_NORMAL);  // redraw the old level
							g_iCustomEqBand_gain[iSelectedBand]--;
							if(g_iCustomEqBand_gain[iSelectedBand] < EQ_MIN_GAIN) {
								g_iCustomEqBand_gain[iSelectedBand] = EQ_MIN_GAIN;
							}					
							SendEQ(EQ_CUSTOM,0,0);        // update EQ module in real time
						}
						else
						{
							DisplayEqBar(iSelectedBand,GEQ_NORMAL);  // redraw the old band
							iSelectedBand--;
							if(iSelectedBand < EQ_BAND_FIRST){
								iSelectedBand = EQ_BAND_LAST;
							}
						}
						DisplayEqBar(iSelectedBand,GEQ_INVERT);      // draw the new level/band

						// Display bottom row info: dB level, center freq Hz
						DisplayEqStats(iSelectedBand,FALSE);   // assume labels have already been displayed
					}
					bSendToPlayerStateMachine = FALSE;         // no need to send to PSM
					break;

				case PH_MENU:
					// Allow user to escape to Music mode
					// Set flag so PR_MENU in Music mode will ignore
					g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
					bDone = TRUE;
					bSendToPlayerStateMachine = FALSE;
					break;

				case PR_MENU:
					if(g_iEqSetting == EQ_CUSTOM)      // (SDK2.520)
					{
						// Toggle the Band-gain-modify mode
						DisplayEqBar(iSelectedBand,GEQ_INVERT);
						if(bBandGainModify_mode)
						{	// Visually indicate the normal Band-select mode, invert the slider bar
							bBandGainModify_mode = FALSE;   // stop blinking  (ignore bBlink)
							SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
						}
						else
						{	// Visually indicate the Band-gain-modify mode, "blink" the slider bar!
							bBandGainModify_mode = TRUE;    // start blinking  (toggle bBlink)
							bBlink = TRUE;
							SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,500,MENU_MSG_ANIMATE);
						}
						SysWaitOnEvent(0,0,0);    // let system process SOFT_TIMER_xxx_TIMER msg (KILL or SET)

						// Display bottom row info: dB level, center freq Hz
						DisplayEqStats(iSelectedBand,FALSE);   // assume labels have already been displayed
					}
					else
					{
						bDone = TRUE;    // exit EqMenu()
					}
					bSendToPlayerStateMachine = FALSE;
					break;

				case PR_STOP:
					bSendToPlayerStateMachine = FALSE;
				    break;

				case PH_STOP:
					// If the STOP button is held down, let's call the Shutdown menu
					SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
					// If user chooses to return then update display.  If doesn't return, player was powered down.
					DisplayHint.I |= (EQ_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

					bDone = TRUE;
					bSendToPlayerStateMachine = FALSE;
					break;
			} // end switch(wButtonEvent)
		} // end switch(iEvent)

		if(bSendToPlayerStateMachine)
			DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
	} // end while(!bDone)

	if(bBandGainModify_mode)     // (SDK2.520)
	{
		SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
		SysWaitOnEvent(0,0,0);      // let system process SOFT_TIMER_KILL_TIMER msg
	}
	return MENU_EQ;
} // end of EqMenu()


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant SendEQ(int iCurrentEQ, int b, int *pPtr)
//
//   Type:          Function 
//
//   Description:   This sends the EQ settings to the decoder.
//
//   Inputs:        Which EQ setting to send.
//
//   Outputs:       Messages posted to the appropriate place
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant SendEQ(int iCurrentEQ, int b, int *pPtr)
{
    // check GeqModuleBase to see if eq is off, if so, turn it on.
    SysPostMessage(7,GEQ_SET_EQ,1);

	// Post the appropriate set of five band gains to the EQ module
	// (use hard-coded values in order to save global data memory)
	switch(iCurrentEQ)
	{
		case EQ_ROCK     ://ROCK
			SysPostMessage(7,GEQ_SET_ALL_BAND_GAINS,
										EQ_ROCK_GAIN1,
										EQ_ROCK_GAIN2,
										EQ_ROCK_GAIN3,
										EQ_ROCK_GAIN4,
										EQ_ROCK_GAIN5 );
			break;

		case EQ_JAZZ     ://JAZZ
			SysPostMessage(7,GEQ_SET_ALL_BAND_GAINS,
										EQ_JAZZ_GAIN1,
										EQ_JAZZ_GAIN2,
										EQ_JAZZ_GAIN3,
										EQ_JAZZ_GAIN4,
										EQ_JAZZ_GAIN5 );
			break;

		case EQ_CLASSIC  ://CLASSIC
			SysPostMessage(7,GEQ_SET_ALL_BAND_GAINS,
										EQ_CLASS_GAIN1,
										EQ_CLASS_GAIN2,
										EQ_CLASS_GAIN3,
										EQ_CLASS_GAIN4,
										EQ_CLASS_GAIN5 );
			break;

		case EQ_POP     ://POP
			SysPostMessage(7,GEQ_SET_ALL_BAND_GAINS,
										EQ_POP_GAIN1,
										EQ_POP_GAIN2,
										EQ_POP_GAIN3,
										EQ_POP_GAIN4,
										EQ_POP_GAIN5 );
			break;

		case EQ_CUSTOM   ://User-defined EQ setting
			SysPostMessage(7,GEQ_SET_ALL_BAND_GAINS,
									g_iCustomEqBand_gain[EQ_BAND1],
									g_iCustomEqBand_gain[EQ_BAND2],
									g_iCustomEqBand_gain[EQ_BAND3],
									g_iCustomEqBand_gain[EQ_BAND4],
									g_iCustomEqBand_gain[EQ_BAND5] );
			break;

//		case EQ_NORMAL   ://Normal (same as "default")  (SDK2.520)
		default:
			SysPostMessage(7,GEQ_SET_ALL_BAND_GAINS,
										EQ_NORM_GAIN1,
										EQ_NORM_GAIN2,
										EQ_NORM_GAIN3,
										EQ_NORM_GAIN4,
										EQ_NORM_GAIN5 );

            // May23'05 addition to disable EQ when gains are 0. Gives 3-4% pwr reduction in player.
            // if all gains are zero, we could also disable the equalizer processing to save 3.76 MIPS
            if( (EQ_NORM_GAIN1==0)  &&
                (EQ_NORM_GAIN2==0)  &&
                (EQ_NORM_GAIN3==0)  &&
                (EQ_NORM_GAIN4==0)  &&
                (EQ_NORM_GAIN5==0)  )
            {
                // Allow the equalizer processing to smoothly transition to 0dB gains on all bands.
                SysWaitOnEvent(0,0,100); // wait 100 msec
                // disable
                SysPostMessage(7,GEQ_SET_EQ,0);
            }

			break;
	}
	return 0; // SUCCESS
} // end of SendEQ()


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant NextEQ(int iDirection, int b, int *c)
//
//   Type:          Function (code bank callable)
//
//   Description:   This updates the eq setting and rolls it over if necessary
//
//   Inputs:        none (all parameters are ignored)
//
//   Outputs:       Calls SendEQ, then exits
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant NextEQ(int iDirection, int b, int *c)
{
	b;c;                        //prevent 'parameter not used'

    if(iDirection == INCREMENT)
    {
        g_iEqSetting += 1;
        if(g_iEqSetting > LAST_EQ)   //check for roll over
            g_iEqSetting = FIRST_EQ;
    }
    else
    {
        g_iEqSetting -= 1;
        if(g_iEqSetting < FIRST_EQ)  //check for roll over
            g_iEqSetting = LAST_EQ;
    }  

    SendEQ(g_iEqSetting,0,0);
    return g_iEqSetting;
} // end of NextEQ()


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant DisplayEqBar(int iSelected, int bInvert)
//
//   Type:          Function
//
//   Description:   Draws iSelected band slider bar and updates the slider position
//
//   Inputs:        iSelected - indicates which band slider to display
//					bInvert - determines whether or not to invert the slider bar
//
//   Outputs:       Updates the display to reflect user selections
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant DisplayEqBar(INT iSelected, INT bInvert)    // (SDK2.520)
{
	INT  iBand_gain[NUM_EQ_BANDS];
	INT  idB_level;
	WORD wSlider_rsrc;

	// Assign the appropriate set of five band gains
	// (use hard-coded values in order to save global data memory)
	switch(g_iEqSetting)
	{
		case EQ_ROCK:  // ROCK
			iBand_gain[EQ_BAND1] = EQ_ROCK_GAIN1;
			iBand_gain[EQ_BAND2] = EQ_ROCK_GAIN2;
			iBand_gain[EQ_BAND3] = EQ_ROCK_GAIN3;
			iBand_gain[EQ_BAND4] = EQ_ROCK_GAIN4;
			iBand_gain[EQ_BAND5] = EQ_ROCK_GAIN5;
			break;
		case EQ_JAZZ:  // JAZZ
			iBand_gain[EQ_BAND1] = EQ_JAZZ_GAIN1;
			iBand_gain[EQ_BAND2] = EQ_JAZZ_GAIN2;
			iBand_gain[EQ_BAND3] = EQ_JAZZ_GAIN3;
			iBand_gain[EQ_BAND4] = EQ_JAZZ_GAIN4;
			iBand_gain[EQ_BAND5] = EQ_JAZZ_GAIN5;
			break;
		case EQ_CLASSIC:  // CLASSIC
			iBand_gain[EQ_BAND1] = EQ_CLASS_GAIN1;
			iBand_gain[EQ_BAND2] = EQ_CLASS_GAIN2;
			iBand_gain[EQ_BAND3] = EQ_CLASS_GAIN3;
			iBand_gain[EQ_BAND4] = EQ_CLASS_GAIN4;
			iBand_gain[EQ_BAND5] = EQ_CLASS_GAIN5;
			break;
		case EQ_POP:  // POP
			iBand_gain[EQ_BAND1] = EQ_POP_GAIN1;
			iBand_gain[EQ_BAND2] = EQ_POP_GAIN2;
			iBand_gain[EQ_BAND3] = EQ_POP_GAIN3;
			iBand_gain[EQ_BAND4] = EQ_POP_GAIN4;
			iBand_gain[EQ_BAND5] = EQ_POP_GAIN5;
			break;
		case EQ_CUSTOM:  // User-defined EQ setting
			iBand_gain[EQ_BAND1] = g_iCustomEqBand_gain[EQ_BAND1];
			iBand_gain[EQ_BAND2] = g_iCustomEqBand_gain[EQ_BAND2];
			iBand_gain[EQ_BAND3] = g_iCustomEqBand_gain[EQ_BAND3];
			iBand_gain[EQ_BAND4] = g_iCustomEqBand_gain[EQ_BAND4];
			iBand_gain[EQ_BAND5] = g_iCustomEqBand_gain[EQ_BAND5];
			break;
//		case EQ_NORMAL:  // Normal (same as "default")
		default:
			iBand_gain[EQ_BAND1] = EQ_NORM_GAIN1;
			iBand_gain[EQ_BAND2] = EQ_NORM_GAIN2;
			iBand_gain[EQ_BAND3] = EQ_NORM_GAIN3;
			iBand_gain[EQ_BAND4] = EQ_NORM_GAIN4;
			iBand_gain[EQ_BAND5] = EQ_NORM_GAIN5;
			break;
	}

	idB_level = iBand_gain[iSelected] / 2;    // perform integer math (fraction is lost)

	// Change the level of selected EqBand
	// First, display the vertical slider track
	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,
					(FIRST_X_POS + SLIDER_SPACING * iSelected),16,RSRC_SLIDER_TRACK);

	// Second, display the horizontal slider bar representing the gain
	// (due to integer math, the slider bar will only move for whole dB levels)
	if(bInvert)
		wSlider_rsrc = RSRC_SLIDER_BAR_INV;
	else
		wSlider_rsrc = RSRC_SLIDER_BAR;

	SysPostMessage(5,LCD_PRINT_RANGE_RSRC,
					(FIRST_X_POS + SLIDER_SPACING * iSelected + 3),
					(32 - idB_level), wSlider_rsrc);

	// Make sure the display (LCD) is updated before returning
	SysWaitOnEvent(0,0,0);
} // end of DisplayEQBar()


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant DisplayEqStats(int iSelected, int bDisplayLabels)
//
//   Type:          Function
//
//   Description:   Displays selected band gain value and center frequency on bottom row
//
//   Inputs:        iSelected - indicates which band gain to display
//					bDisplayLabels - indicates whether or not to display dB level & Hertz text
//
//   Outputs:       Updates the display to reflect user selections
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant DisplayEqStats(INT iSelected, INT bDisplayLabels)    // (SDK2.520)
{
	INT idB_level;
	INT iCustomEqBand_freq[NUM_EQ_BANDS];   // five EQ Bands supported

	iCustomEqBand_freq[0] = CENTER_FREQ1;   // use hard-coded freqs
	iCustomEqBand_freq[1] = CENTER_FREQ2;
	iCustomEqBand_freq[2] = CENTER_FREQ3;
	iCustomEqBand_freq[3] = CENTER_FREQ4;
	iCustomEqBand_freq[4] = CENTER_FREQ5;

	if(bDisplayLabels)   // in case user selected a different band
	{
		SysPostMessage(5,LCD_PRINT_STRING_RSRC,0,56,RSRC_DB_STRING);    // "dB:" (String_db.txt)
		SysPostMessage(5,LCD_PRINT_STRING_RSRC,85,56,RSRC_HZ2_STRING);  // "Hz"
	}

	// Display center frequency of selected band
	SysPostMessage(6,LCD_CLEAR_RANGE,55,56,30,8);   // clear the old CenterFreq
	SysPostMessage(7,LCD_PRINT_NUMBER,55,56,iCustomEqBand_freq[iSelected],5,' ');

	// Display gain level of selected band
	SysPostMessage(6,LCD_CLEAR_RANGE,16,56,18,8);   // clear the old GainLevel and Neg sign
//	SysWaitOnEvent(0,0,0);	     // give display module time to process its msgs

	idB_level = g_iCustomEqBand_gain[iSelected];     // get global var value
	if(idB_level < 0)
	{	// Display the negative sign  (Negative_sign.bmp)
		SysPostMessage(5,LCD_PRINT_RANGE_RSRC,16,56,RSRC_NEGATIVE_SIGN);
		idB_level *= -1;     // make local var positive
	}
	if((idB_level % 2) != 0)
	{	// Display ".5" fraction string  (String_dec_pt5.txt)
		SysPostMessage(5,LCD_PRINT_STRING_RSRC,32,56,RSRC_DEC_PT5_STRING);
	}
	else
	{	// Display ".0" fraction string  (String_dec_pt0.txt)
		SysPostMessage(5,LCD_PRINT_STRING_RSRC,32,56,RSRC_DEC_PT0_STRING);
	}

	idB_level /= 2;          // convert gain level to "dB"
	SysPostMessage(7,LCD_PRINT_NUMBER,20,56,idB_level,2,' ');

	// Make sure the display (LCD) is updated before returning
	SysWaitOnEvent(0,0,0);
} // end of DisplayEQStats()


// Use SaveRange macro to save a copy of global variable addresses as well as
// reserve space for their values in section "Player_settings_Y".  All of which
// will be used at shutdown to save user settings in the Settings.dat file.
#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_iEqSetting
	if (@def('USE_PLAYLIST3'))
    extern y:Fg_MarkerMusic
    extern y:Fg_MarkerVoice
    endif

    SaveRange Fg_iEqSetting,1
    SaveRange Fg_iCustomEqBand_gain,5
	if (@def('USE_PLAYLIST3'))
    SaveRange Fg_MarkerMusic,9
    SaveRange Fg_MarkerVoice,9
    endif
    
#pragma endasm
