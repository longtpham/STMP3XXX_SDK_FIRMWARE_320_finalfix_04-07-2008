// Filename: fmtunermenu.c


#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "display.h"
#include "menus.h"
#include "mainmenu.h"
#include "fmtunermenu.h"
#include "recorderstatemachine.h"
#include "playerstatemachine.h"
#include "sysspeed.h"
#include "eqmenu.h"
#include "pwrsettingsmenu.h"
#include "hwequ.h"
#include "sysvolume.h"
#include "battery.h"
#include "project.h"
#include "recordsettingsmenu.h"
#include "batterycharge.h"
#include "SysResourceApi.h"
#ifdef TUNER_STFM1000
#include "syscoder.h"
#endif
#include <filesystem.h>
#include "RestoreDriveFromBackup.h"


#ifdef USE_PLAYLIST2
#include "playlist2.h"
#endif
#ifdef USE_PLAYLIST3
#include "playlist3.h"
#include "voicemenu.h"
#include "stringlib.h"
#include "musiclib_ghdr.h"
#endif
#if !defined(NUMBER_OF_PRESETS)
#define NUMBER_OF_PRESETS	10
#endif


#if 0
#if !defined(FM_BAND_LIMIT_HIGH_FREQ)
#ifdef FM_JAPAN_REGION
#ifdef SD_USE_200KHZ_TUNING_GRID 
#define	FM_BAND_LIMIT_HIGH_FREQ	89900   //stmp10809
#else
#define	FM_BAND_LIMIT_HIGH_FREQ	90000 
#endif
#else
#ifdef SD_USE_200KHZ_TUNING_GRID 
#define	FM_BAND_LIMIT_HIGH_FREQ	107900
#else
#define	FM_BAND_LIMIT_HIGH_FREQ	108000 
#endif
#endif
#endif
#if !defined(FM_BAND_LIMIT_LOW_FREQ)
#ifdef FM_JAPAN_REGION
#define	FM_BAND_LIMIT_LOW_FREQ	76000
#else
#define	FM_BAND_LIMIT_LOW_FREQ	87500
#endif
#endif

#endif
#if !defined(FM_BAND_LIMIT_HIGH_FREQ)
    #ifdef  FM_USA_REGION               //USA
                #define FM_BAND_LIMIT_HIGH_FREQ    107900 
                #define FM_BAND_LIMIT_LOW_FREQ     88100
    #else             //  !(USA)
        #ifdef  FM_EUROPE_REGION      //Europe and China
                #define FM_BAND_LIMIT_HIGH_FREQ    108000 
                #define FM_BAND_LIMIT_LOW_FREQ      87500
        #else         //  !(EUROPE)
            #ifdef FM_JAPAN_REGION    //Japan
                #define FM_BAND_LIMIT_HIGH_FREQ	    90000
                #define FM_BAND_LIMIT_LOW_FREQ	    76000
            #endif   // end (JAPAN)
        #endif      // end (EUROPE)
    #endif          // end (USA)
#endif
/* FM TUNER GRID is defaulted to 100KHz. Override this value by defining a new value in project.inc */
#if !defined(FM_TUNER_GRID)
#ifdef SD_USE_200KHZ_TUNING_GRID
#define	FM_TUNER_GRID	200
#else
#define	FM_TUNER_GRID	100
#endif
#endif

#define DCDC1_750KHZ  0     //3120-mark
   // In order to reduce EMI, customer can define this macro as "1"
   // However this change may cause less stability of DCDC converter.  
  //Customer should determine this value to be "1" or "0".
  //If customer set DCDC1_750KHz to "1" to let DCDC1 works on 750KHz,  then customer should change the DCDC1 power inductor to 15uH or 22 uH power inductor with max. allowable DC current larger than 500mA
  //If customer set DCDC1_750KHZ to "0", then the DCDC1 power inductor is 4.7uH, customer could refer to the "STMP35xx Reference Schematics"


// Minimum settings for mixer volume registers
#define MIN_LVOL	0x001F00
#define MIN_RVOL	0x00001F

#define SmartMediaTimer1Reg (*((volatile WORD _X *) (HW_EMC_BASEADDR+17)))
#define CompactFlashTimer1Reg (*((volatile WORD _X *) (HW_EMC_BASEADDR+9)))
#define dcdcVdddReg (*((volatile WORD _X *) (HW_DCDC_BASEADDR+3)))
#define dcdcVddioReg (*((volatile WORD _X *) (HW_DCDC_BASEADDR+2)))

#ifdef TUNER_STFM1000
// Lower the Vddio to conserve the power
#define STFM_VDDIO_BO_CHANGE 0x300      // 0.24V 
#define STFM_VDDIO_VDC_CHANGE 0x004     // 0.25V
#define STFM_VDDIO_CONTRAST_ADJUST 5
#endif

#define FMTUNER_MENU_DISPLAY_ALL (1<<DISPLAY_LOCK_BITPOS)|\
                                 (1<<DISPLAY_DISK_BITPOS)|\
								 (1<<DISPLAY_BATTERY_BITPOS)|\
							     (1<<DISPLAY_VOLUME_BITPOS)|\
                                 (1<<DISPLAY_PLAYSET_BITPOS)|\
                                 (1<<DISPLAY_EQ_BITPOS)|\
                                 (1<<DISPLAY_FMTUNER_FREQUENCY)


_reentrant void InitializeFMTuner(void);
_reentrant void TurnOffFMTuner(void);
_reentrant void ForceFMTInit (void);
_reentrant void DisableExecSpeedClient(void);  // See SysExecSpeedClient.asm
_reentrant void ReEnableExecSpeedClient(void);
WORD _reentrant DisplayGetContrast(void);

extern INT gTUNER_STFM1000;
extern int g_iEqSetting;

extern WORD g_bEraseComplete;
extern WORD g_bResourceGetDirty;
#ifdef TUNER_STFM1000
_reentrant void LoadSTFMDecoder(void);
#endif

// Filter the battery level to minimize the ADC noise
INT _X iBattlevel;


/* Added for removing nand flash noise in the FM Tuner mode */
void FadeOutFMTuner(void);
_reentrant WORD RefreshSystemDriveErrorBlock(void);
_reentrant int EraseOneSystemDriveBlock(void);
/* Tuner muted states:
	0 - idle
	1 - Tuner muted and Faded out. Unmute the tuner when tuner is tuned
	2 - Tuner muted and Faded out. Mute the tuner when tuner is tuned
	3 - Wating for audio fade in
*/
INT _X iTuningMuted;	  
/* These global variables are used to communicate the tuner data with the display routine */
WORD _X g_wTunedFrequency;   //
BOOL	g_bStereoBroadcast;
INT _X iCurrentPreset = 0;   //
WORD  _X  s_wTimeCount = 0;  //
#ifdef TUNER_STFM1000
WORD _X g_wDCDCTBR;          //
// Have to be defined as pointers
extern _P int  _lc_u_b_FM_TUNER_SECOND_RSRC_P_buf[];
extern _P int  _lc_u_e_FM_TUNER_SECOND_RSRC_P_buf[];
BOOL _X g_bEnable100msTimer = FALSE;  //
// Wrap function
_reentrant void LoadSTFM10000SecondResource(int rsrc_num,     // resource number
                                        int rsrc_type,     // resource type
                                        int rsrc_size,     // resource size
                                        int start_addr,    // resource start addr
                                        int target_mem);   // Memory type
#endif
//INT  g_iStoredContrastPercentage = 0;  //

#if 0
#ifdef USE_PLAYLIST3
extern RAM_SONG_INFO_T song_info;
extern int g_file_time;
extern _packed BYTE g_LastEncName[30];

#pragma asm
    nolist
    include "sysresources.inc"
    include "sysequ.inc"
    list
#pragma endasm

void _reentrant ML_fmrec_menu_init(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextfmtunermenu":
	extern	F_lc_u_e_MusicLibVoice_P
	extern	F_lc_u_b_MusicLibVoice_P
	extern	SysLoadResource

	; Load the MusicLib voice build engine into P memory
	move    #>RSRC_MUSICLIB_VOICE_APP,x0                      ; Resource number
	move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
	move    #>(F_lc_u_e_MusicLibVoice_P-F_lc_u_b_MusicLibVoice_P)*3,y1 ; overlay area sz
	move    #>F_lc_u_b_MusicLibVoice_P,y0                      ; Address of the overlay area
	move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
	jsr     SysLoadResource
#pragma endasm
}
#endif
#endif


 

#ifndef BATTERY_TYPE_LI_ION
#ifdef TUNER_STFM1000
void IncreaseVDDIO(void) {    // Helper function to increse r  ecrease VDDIO and VDDIO_BO
    dcdcVddioReg += STFM_VDDIO_VDC_CHANGE;				// increase the Vddio first
    SysPostMessage(3,LCD_TEMP_CONTRAST,0);
	SysWaitOnEvent(0,0,1);
    dcdcVddioReg += STFM_VDDIO_BO_CHANGE;          // then raise the BO level
    return;
}
void DecreaseVDDIO(void) {    // Helper function to increse r  ecrease VDDIO and VDDIO_BO
	dcdcVddioReg -= STFM_VDDIO_BO_CHANGE;          // decrease the BO level first
	SysPostMessage(3,LCD_TEMP_CONTRAST,LCD_STEPSIZE_CONTRAST*STFM_VDDIO_CONTRAST_ADJUST);
	SysWaitOnEvent(0,0,1);
	dcdcVddioReg -= STFM_VDDIO_VDC_CHANGE; 
    return;
}
#else
// Display is too dark using LIION battery if above code is used, so do nothing for LIION battery
#define IncreaseVDDIO() 
#define DecreaseVDDIO()
#endif
#else 
#define IncreaseVDDIO() 
#define DecreaseVDDIO()
#endif

#if 0
static void Illegal_Instruction_Int_Init (void) 
{
  #pragma asm
 ;Set GP12 as trigger if Invalid Instruction
    move    #>$0A702C,x0             ; opcode for  bset #HW_GP_B12_BITPOS,x:HW_GP1DOR (bclr is $0A700C)
    move    #>$00F411 ,n1
    move    #$0038,r1                ; Invalid DSP Insrution Vector
    move    x0,p:(r1)+
    move    n1,p:(r1) 
  #pragma endasm
    
    return();
}
#endif	            
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant FMTunerMenu(int a, int b, int *pPtr )
//
//   Type:          Function (user menu)
//
//   Description:   This function is the FM Tuner menu
//
//   Inputs:        None used.
//
//   Outputs:       value of which menu to return to
//
//   Notes:         This function handles the FM Tuner menu.  It loops until a reason
//                  to exit is reached (PR_MENU will return MENU_MAIN)
//
//                  The basic loop is...
//
//                  while (not done)
//                  {
//                      Update display if necessary
//                      Wait for event
//                      handle event locally
//                  }
//                  
//                  Sometimes an event may be handled entirely in the player state handler,
//                  sometimes entirely locally, sometimes in both places.
//<
//////////////////////////////////////////////////////////////////////////////////////////
int _reentrant FMTunerMenu(int a, int b, int *pPtr)
{
    union DisplayHints DisplayHint;
    INT	i ;
    INT  iNewMenu;
	
    INT  iEvent;
    INT  iEventSave = 0;  //Used to record a "media state change" and save the event.

    BOOL bDone = FALSE;
    BOOL bMuted;
    BOOL bResetPreset = FALSE;
    INT  iSeekCount;
    INT  iSaveRecSource;
    INT iVolumeRepeat = 0;
    INT	iFlashAccessTimer;
    INT iTempEqSetting;

// This structure is used to save the relevant members of EventInfo each time
// a MENU_MSG_MEDIA_CHANGED message is received.
    struct EventInfoSave {
	WORD wMsgLength;
	WORD wMsgCommand;
    } EventInfoSave;

    a;b;//prevent parameter not used warnings.
    // This is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

#ifdef REMOVE_FM
    return MENU_MUSIC;		// Exit if No FM Support
    }
#else
    
#ifdef TUNER_STFM1000 
	if (0 == gTUNER_STFM1000) return MENU_MUSIC;		// Exit if STFM1000 is not present
#endif

#ifdef  FM_PHILIPS    
   	 if (1 == gTUNER_STFM1000) return MENU_MUSIC;		// Exit if STFM1000 is present
#endif

    // Turn on the mixer first then mute the DAC to avoid the pop
#ifndef TUNER_STFM1000
    SysPostMessage(3,MIXER_POWER_DOWN,MIXER_ON);
    SysWaitOnEvent(0,0,1);		
#endif

    SysPostMessage(2,MIXER_DAC_MUTE);
    SysWaitOnEvent(0,0,1);	

    // stop the decoder
    SysPostMessage(2,DECODER_STOP);
    SysWaitOnEvent(0,0,1);

#ifdef TUNER_STFM1000
    if(EQ_NORMAL != g_iEqSetting)
    {
          SysSpeedIncrease(SPEED_FM_EQ, SPEED_CLIENT_STFM_EQ);   // Increase speed for equalizer
//4-17        SysSpeedIncrease(SPEED_MP3, SPEED_CLIENT_STFM_EQ);  //DEBUG ONLY Try higher clock speed to fix DAC underflow
        SysPostMessage(3,GEQ_SET_EQ,ENABLE_EQ);   // Make sure Equalizer is Enabled for STFM1000
    }
#ifdef CLCD_16BIT    
    SysPostMessage(2,LCD_16BIT_OFF);
    SysWaitOnEvent(0,0,1);
#endif //#ifdef CLCD_16BIT
  
    // Force loading of STFM decoder
    // NOTE: MAKE SURE THE DECODER_STOP MESSAGE HAS BEEN SENT
    // TO THE CURRENTLY LOADED DECODER BEFORE LOADING THE STFM DECODER
    LoadSTFMDecoder();
    // Load FAT write to restore the system drive
    SysLoadFATWrite();

    // start the decoder
    SysPostMessage(2,DECODER_PLAY);
    SysWaitOnEvent(0,0,1);

#endif

    // Stop the Force Battery update every 2 seconds, and the animation
    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);    
    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);    

    // disable auto powerdown while in this menu
    g_MenuFlags.MENU_FLAG_POWER_DOWN_ENABLED = FALSE;
    UpdateAutoShutdownTimer();

#ifdef TUNER_STFM1000

    // set DCLK to SPEED FM
    SysSpeedIncrease(SPEED_FM, SPEED_CLIENT_STFM_DEC);
//4-17        SysSpeedIncrease(SPEED_MP3, SPEED_CLIENT_STFM_DEC);   //DEBUG ONLY Try higher clock speed to fix DAC underflow
 #else
 
    // Increases vddd Voltage if its not already above the mixer's needed vddd voltage  
    SysSpeedIncrease(SPEED_MIXER, SPEED_CLIENT_MIXER);
#endif
    // Disable the exec speed client upon entering FM menu so DCLK speed does not increase 
    // on button press introducing noise due to higher DCLK freq. Reenable on FM menu exit.
    DisableExecSpeedClient();

    //set the initial conditions here, rather than in the declaration--here it takes code 
    //(which is overlayed) to initialize the variables  rather than y space (which isn't)
    iSeekCount = 0;
    DisplayHint.I = FMTUNER_MENU_DISPLAY_ALL;
    bDone = FALSE;

    g_iCurrentMenu = MENU_FMTUNER;
    iNewMenu = MENU_MAIN;
    bMuted = FALSE;
#ifdef TUNER_LV24000     
    // Change the Vddio for testing.
	// Disable the brownout
	//HW_DCDC_VDDIO.B.BROWNOUT_ENABLE = 0;
	dcdcVddioReg &= 0x0301f1f;
	// incease the Level
	//	Vddio = 3.0 V => set VOLTAGE_LEVEL = 0x0f
	//  Vddio = 3.3 V => set VOLTAGE_LEVEL = 0x14
	//  Vddio = 3.3 V => set VOLTAGE_LEVEL = 0x17
	//HW_DCDC_VDDIO.B.VOLTAGE_LEVEL = 0x14;
	dcdcVddioReg = (dcdcVddioReg & 0x301f00) | 0x14;
	// wait for Vddio to settle
	// note: do not wait HW_DCDC_VDDIO.B.VDDIO_OK. It may not set in buck mode
	SysWaitOnEvent(0,0,30);
	// increase the brownout level
	//HW_DCDC_VDDIO.B.BROWNOUT_LEVEL = 0x0c;
	dcdcVddioReg = (dcdcVddioReg & 0x3001F) | 0x0c00;
	// enable brownout
	//HW_DCDC_VDDIO.B.BROWNOUT_ENABLE = 1;
	dcdcVddioReg |= 0x10000;
	// compensate the LCD contrast due to the change in Vddio
	// Vddio = 3.0 V, no compensation
	// Vddio = 3.3 V, decrease by 4 steps
	// Vddio = 3.5 V, decrease by 7 steps
	//g_iStoredContrastPercentage = DisplayGetContrast();
	SysPostMessage(3,LCD_SET_CONTRAST, DisplayGetContrast()-LCD_STEPSIZE_CONTRAST*4);  //
#endif

	//clear the screen
    SysCallFunction(RSRC_DISPLAY_CODE_BANK,DisplayClearDisplay,0,0,0);
	SysWaitOnEvent(0,0,10);

    InitializeFMTuner();

	iTuningMuted=4;
#ifdef TUNER_STFM1000
    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_TUNER,1,100,MENU_TUNER_READY); 
  
#else 
    // set stage for tuner ready
    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_TUNER,1,2000,MENU_TUNER_READY); 
#endif
	// set the tuner sensitivity
#ifdef TUNER_STFM1000  
    SysPostMessage(3,TUNER_SET_SENSITIVITY,27);
#else
    SysPostMessage(3,TUNER_SET_SENSITIVITY,20); 
#endif
    SysWaitOnEvent(0,0,1);
	

    // read the battery level
    iBattlevel =  SysBatteryGetLevel();
	
    // slow the flash accessing to reduce the radio interference noise
    //SmartMediaTimer1Reg = 0x18098c;
	//iFlashAccessTimer = CompactFlashTimer1Reg;
	//CompactFlashTimer1Reg = 0x302306;		   // Reduce the flash noise picked up by FM Tuner

#ifdef TUNER_STFM1000
    // Save power by lowering the Vddio, but it won't work on a single channel buck mode because the Vddio runs on external linear reg
    DecreaseVDDIO();
#endif

    //Completely refresh screen upon entering menu
    DisplayHint.I = (FMTUNER_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

    while(!bDone)//loop until we're done
    {
#ifndef TUNER_STFM1000
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,(DisplayHint.I? 1 : 2000));
#else
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,-1);
#endif
#ifdef TUNER_STFM1000    
        g_bEnable100msTimer = TRUE;  // rssi gets called every 100ms - a big trigger for fm processing
#endif
         //this will return when we get a message
         //a button has been pressed.

	
        //Things handled below are menu specific reactions to buttons/events
        switch(iEvent)
        {
            case EVENT_TIMER:
#ifdef TUNER_STFM1000
#else
                // Uncommenting the following optional if segment will allow Mono/Stereo and current frequency to be dynamically updated, 
                // but may cause excessive NAND interference on the tuner. Commented out by default.
		        //if ((g_wFMTunerError.m_bTUNER_BUSY==FALSE)&&(g_wFMTunerError.m_bSCANNING_STATION==FALSE))
                //{
			    //    SysPostMessage(2,TUNER_GET_STATES);	   // get the tuner's stereo pilot and possible new frequency
                //}
		        // update battery level only when necessary
                if ((iBattlevel=((iBattlevel+SysBatteryGetLevel())/2)) < 10)
                    battlevel = 0;
                else if (iBattlevel >= 100)
                    battlevel = 10;
                else 
                    battlevel = iBattlevel/10;            
                if (battlevel != LastBattLevel)
                {
                    LastBattLevel = battlevel;
					DisplayHint.bits.Battery = TRUE;
                }

				// Updating the FM Stereo pilot and/or frequency display may cause interference noise from accessing the nand flash!!!
				// These conditions will not evaluate to TRUE if the TUNER_GET_STATES message post above is commented out
                if (g_bTunedStereoStation != g_bStereoBroadcast)	// update the FM Stereo Pilot display
                {
                    DisplayHint.I |= FMTUNER_MENU_DISPLAY_ALL;
                    g_bStereoBroadcast = g_bTunedStereoStation;
                }

                // update frequency
                if (g_wTunedFrequency != FM_TUNER_GRID*((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID))   
                {
                    DisplayHint.I |= FMTUNER_MENU_DISPLAY_ALL;
                    g_wTunedFrequency = FM_TUNER_GRID*((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID);
                }
				
				if(DisplayHint.I)
				{
					SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
					DisplayHint.I = 0;
				}
#endif


            break; /* end EVENT_TIMER */

            case EVENT_MESSAGE:
            switch(gEventInfo.Message.wMsgCommand)
            {
#ifdef BATTERY_CHARGE
				case MENU_BATTERY_CHARGER_TIMEOUT:
					//check on battery charging.	  
					SysCallFunction(RSRC_BATTERY_CHARGE_CODEBANK,BatteryChargeStateMachine,0,0,0);	  
					break;
#endif	//BATTERY_CHARGE

                case MENU_BATTERY_CHNG:
				    DisplayHint.bits.Battery = TRUE;
                    break;

                case MENU_TUNER_REFRESH_DISPLAY:
                    //DisplayHint.I |= FMTUNER_MENU_DISPLAY_ALL;
                    DisplayHint.I |= gEventInfo.Message.wMsgParms[0];
                    break;

                case MENU_TUNER_DIRTY_FLASH:
                    if ((!g_bEraseComplete) && (IsSystemDriveDirty()))
    		        {
    			        SysCallFunction(RSRC_RESTORESYSDRIVE_P_CODE,EraseOneSystemDriveBlock,0,0,0);
                    }
                    else
                    {
                        if(g_bEraseComplete && (IsSystemDriveDirty()))
                        {
                            SysCallFunction(RSRC_RESTORESYSDRIVE_P_CODE,SequentialRefreshSystemDriveErrorBlock,0,0,0);
                        }
                    }
                    break;

                case MENU_TUNER_TUNED:	/* received the tuner tuned message, if preset scanning is in progress then keep scanning until done */
                    g_wTunedFrequency = FM_TUNER_GRID*((g_wCurrentFrequency+FM_TUNER_GRID/2)/FM_TUNER_GRID); /* Tuned frequency */
                    g_bStereoBroadcast = (g_bTunedStereoStation==0? FALSE:TRUE); /* Tuned stereo indicator */
                    if(g_wFMTunerError.m_bSCANNING_STATION==FALSE)	 /* skip if scanning the preset */
                    {
                        if ((g_wLastButton == PH_FF) || (g_wLastButton == PH_RW))
							iSeekCount = 1;	// the next search for station cannot begin for another 1 second
                        if (iTuningMuted==1)
                        {
#ifdef TUNER_STFM1000
                            SysPostMessage(2,MIXER_DAC_UNMUTE);
#else
                            SysPostMessage(2,MIXER_FM_UNMUTE);
#endif  
                        }
                        if ((iTuningMuted > 0)&& (iTuningMuted<3))
                        {
                            /* set stage for tuner ready */
                            SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_TUNER,1,300,MENU_TUNER_READY); 
                            iTuningMuted = 3;
                        }
                    }
                    DisplayHint.I |= (FMTUNER_MENU_DISPLAY_ALL);
                    break;

                case MENU_TUNER_READY:	/* tuner has tuned for 0.5 seconds */
                    if (iTuningMuted == 3)
                    {
                        SysPostMessage(2,TUNER_GET_STATES);
                        SysPostMessage(3,MIXER_MASTER_FADE_IN,5);
                        iTuningMuted =0;
                    }
                    if (iTuningMuted == 4)
                    {
                        // check if preset scanning is needed
                        for (i = NUMBER_OF_PRESETS; (i > 0) && (g_astPresetStations[i-1].Frequency == 0)&&((g_astPresetStations[i-1].Frequency < FM_BAND_LIMIT_LOW_FREQ)||(g_astPresetStations[i-1].Frequency > FM_BAND_LIMIT_HIGH_FREQ)); i--);

                        if (i==0)	// need to do scanning for preset stations
                        { 
                            g_bSearchEndAtBandLimit = TRUE;
                            iCurrentPreset = 1;	// set the menu level preset number
                            SysPostMessage(2,TUNER_PRESET_STATION);    
                            SysWaitOnEvent(0,0,1);
                        }
                        else
                        {
                            SysPostMessage(3,TUNER_TUNE_FREQUENCY,g_wCurrentFrequency);
                            SysWaitOnEvent(0,0,1);
                            if ((g_wCurrentPreset >= 1) && (g_wCurrentPreset <= NUMBER_OF_PRESETS))
                                iCurrentPreset = g_wCurrentPreset; 
                        }    
                        SysWaitOnEvent(0,0,2);
#ifdef TUNER_STFM1000
                        SysPostMessage(2,MIXER_DAC_MUTE);// Changed from "UNMUTE" to "MUTE"
#else
                        SysPostMessage(2,MIXER_FM_MUTE);// Changed from "UNMUTE" to "MUTE"
#endif

                        iTuningMuted = 1;
                    }  
                   break; // end MENU_TUNER_READY


                case MENU_MSG_MEDIA_CHANGED:            
 	               iEventSave =  iEvent;	//Save event for SysCallFunction @ end of while(!bDone) loop.   
                   EventInfoSave.wMsgLength = gEventInfo.Message.wMsgLength ; 
                   EventInfoSave.wMsgCommand = gEventInfo.Message.wMsgCommand  ;	
		           DisplayHint.bits.Disk ^= TRUE;	//Toggle icon screen display	   
                   break;		

                default:
                    break;
            }
            break; // end EVENT_MESSAGE

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
            switch(gEventInfo.Button.wButtonEvent)
            {
            case PR_PLAY://this will mute and unmute the FM Tuner
#ifdef TUNER_STFM1000    
                if(bMuted)
                   SysPostMessage(2,MIXER_DAC_UNMUTE);
                else
                   SysPostMessage(2,MIXER_DAC_MUTE);
#else
                if(bMuted)
                   SysPostMessage(2,MIXER_FM_UNMUTE);
                else
                   SysPostMessage(2,MIXER_FM_MUTE);
#endif
                bMuted = !bMuted;
                break;

            case PH_HOLD:
            case PR_HOLD:
                DisplayHint.bits.LockIcon = TRUE;
                break;

            case PH_VOL_UP:
            case PH_VOL_DOWN:
                iVolumeRepeat++;
                if (iVolumeRepeat>10)
                	iVolumeRepeat = 10;
                g_iUserVolume += (gEventInfo.Button.wButtonEvent == PH_VOL_UP? iVolumeRepeat : - iVolumeRepeat);
                SysUpdateVolume();
                DisplayHint.bits.Volume=TRUE;
                SysWaitOnEvent(0,0,0);			
 			break;
            case PR_VOL_UP:
            case PR_VOL_DOWN:
                iVolumeRepeat = 0;
                //this is the only event that we want handled by the player state machine...much easier this way
                SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
                DisplayHint.bits.Volume=TRUE;
				break;

            case PR_FF:
                if(g_wLastButton!=PH_FF)
                {//if we're releasing from a scan (a scan will probably have PH_FF held down)
                    g_wCurrentPreset = 0;//if we move off our current value, lets undo the preset
                    FadeOutFMTuner();
                    SysPostMessage(2,TUNER_TUNE_MANUAL_UP);
                }
                iSeekCount = 0;
                DisplayHint.bits.FMTunerFrequency=TRUE;
                break;

            case PH_FF:
                if(!iSeekCount)
                {//if this is the first count of the seek (first PH_FF received)
                    iSeekCount++;
                    if (g_wFMTunerError.m_bSCANNING_STATION==FALSE)
                    {
                    	g_wCurrentPreset = 0;//if we move off our current value, lets undo the preset
                    	FadeOutFMTuner();
                        g_bSearchEndAtBandLimit = FALSE;
                        SysPostMessage(2,TUNER_TUNE_SEARCH_UP);
                    }
                }
                else
                {//otherwise...count 5 PH_FFs, which should wait a second
                    iSeekCount++;
                    if(iSeekCount > 5)
                    {
                        iSeekCount = 0;
                    }
                    DisplayHint.bits.FMTunerFrequency=TRUE;
                }
                break;

            case PR_RW:
                        
                if(g_wLastButton!=PH_RW)
                {//if we're releasing from a scan (a scan will probably have PH_RW held down)
                    g_wCurrentPreset = 0;//if we move off our current value, lets undo the preset
                    FadeOutFMTuner();
                    SysPostMessage(2,TUNER_TUNE_MANUAL_DOWN);
                }
                DisplayHint.bits.FMTunerFrequency = TRUE;
                iSeekCount= 0;
                break;

            case PH_RW:
                if(!iSeekCount)
                {//if this is the first count of the seek (first PH_RW received)
                    iSeekCount++;
                    if (g_wFMTunerError.m_bSCANNING_STATION==FALSE)
                    {
                    	g_wCurrentPreset = 0;//if we move off our current value, lets undo the preset
                    	FadeOutFMTuner();
                        g_bSearchEndAtBandLimit = FALSE;
                        SysPostMessage(2,TUNER_TUNE_SEARCH_DOWN);
                    }
                }
                else
                {//otherwise...count 5 PH_RWs, which should wait a second
                    iSeekCount++;
                    if(iSeekCount > 5)
                    {
                        iSeekCount = 0;
                    }
                }
                DisplayHint.bits.FMTunerFrequency = TRUE;
                break;
            case PR_MODE:				    
                if (++iCurrentPreset > NUMBER_OF_PRESETS)
                    iCurrentPreset = 1;
                FadeOutFMTuner();
                iTuningMuted += (g_astPresetStations[iCurrentPreset-1].Level == 0? 1 : 0);
                SysPostMessage(3,TUNER_TUNE_TO_PRESET,iCurrentPreset);
				DisplayHint.I |= (FMTUNER_MENU_DISPLAY_ALL);
               break;
            case PR_AB:
                if (--iCurrentPreset <1)
                    iCurrentPreset = NUMBER_OF_PRESETS;
                FadeOutFMTuner();
                iTuningMuted += (g_astPresetStations[iCurrentPreset-1].Level == 0? 1 : 0);
                SysPostMessage(3,TUNER_TUNE_TO_PRESET,iCurrentPreset);
				DisplayHint.I |= (FMTUNER_MENU_DISPLAY_ALL);
             
                break;				   
                
            case PH_EQ: // store the current station into the current preset (was PR_EQ , now Press and HOLD)
                if ((iCurrentPreset>= 1) && (iCurrentPreset <= NUMBER_OF_PRESETS))
                    SysPostMessage(3,TUNER_SET_PRESET,iCurrentPreset);
				DisplayHint.I |= (FMTUNER_MENU_DISPLAY_ALL);
                bResetPreset = TRUE;
                break;	
                                			   
            case PR_EQ: // Change EQ (equalizer) mode
                if (TRUE == bResetPreset)   // Do not change EQ mode immediately after storing current preset
                {
                    bResetPreset = FALSE;
                }
                else    // Normal case
                {
                    DisplayHint.bits.EQ = TRUE;                    
                    SysCallFunction(RSRC_EQ_MENU_CODE_BANK,NextEQ,INCREMENT,0,0);

                    if(EQ_NORMAL != g_iEqSetting)
                    {
               SysSpeedIncrease(SPEED_FM_EQ, SPEED_CLIENT_STFM_EQ);  // Increase speed for EQ processing
//4-17                    SysSpeedIncrease(SPEED_MP3, SPEED_CLIENT_STFM_EQ);   //DEBUG ONLY Try higher clock speed to fix DAC underflow
                    }
                    else //Going back to no equalizer mode
                    {
                        SysSpeedClockFree(SPEED_CLIENT_STFM_EQ);    // Speed goes back to SPEED_FM_EQ
                    }
                }
                break;                	  
	 		   							  			
            case PR_RECORD:
				iSaveRecSource = g_ADCsource;	//save temp copy of global source
//stmp6930 moved test for FM tuner source from MainMenu.c
#ifdef FMTUNER_ON_LINE2_IN 
            	g_ADCsource = SOURCE_LINEIN;		//set source to LINE-IN	if 144 pin package
#else 
            	g_ADCsource = SOURCE_FM;		    //use fm if 100 pin package to do line-in function
#endif	          				
#ifdef TUNER_STFM1000
                
                 // undo Save power by lowering the Vddio
                IncreaseVDDIO();

                // increase the speed to 36 MHz since 24 MHz is too slow when exiting the FM Recording
				SysSpeedIncrease(SPEED_MP3, SPEED_CLIENT_MP3_DEC);
				SysWaitOnEvent(0,0,2);    //was 0,0,100
                // Use first arg to RecordVoiceFile to flag that the decoder
                // should not be stopped

				SysCallFunction(RSRC_VOICE_MENU_CODE_BANK,RecordVoiceFile,g_ADCsource==SOURCE_FM,0,0); // Call blocks until recording is stopped
                // stay at 36 MHz for a while to close out all recording activities
    			
				SysWaitOnEvent(0,0,100);
				// drop the DCLK to save power
				SysSpeedClockFree(SPEED_CLIENT_MP3_DEC);

				SysWaitOnEvent(0,0,100);  //was 0,0,1
				// Save power by lowering the Vddio
                DecreaseVDDIO();             
#else
				SysCallFunction(RSRC_VOICE_MENU_CODE_BANK,RecordVoiceFile,0,0,0);
#endif

#ifdef USE_PLAYLIST3
				//after record, rebuild fm part
                SysCallFunction(RSRC_VOICE_MENU_CODE_BANK,AddVoiceEntry,REC_FMREC_TYPE,0,0);
#endif
				DisplayHint.I |= (FMTUNER_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
				g_ADCsource = iSaveRecSource;	//restore global record source
                break;				   
            case PH_STOP:
#ifdef TUNER_STFM1000
                // undo Save power by lowering the Vddio
                IncreaseVDDIO();
#endif

#if 0
                // Here are the test code to debug the monitor signal quality
                // Use the following BoxView command to dump sdc_ctl_TableSdnom_dw to file MyData in 2 columns
                // OUTPUT MyData -2 ## have to do it before log    
                // OUTPUT OFF       ## closes all files after log
                // OUTPUT           ## lists open files
                // close all        ## closes all files
               // __asm(" push x0");  // Save x0, r0, r1
               // __asm(" push r0");
               // __asm(" push r1");
                
                __asm(" move #$010500,x0");     // 0x500 words to file #1
                __asm(" move #$004fb9,r0");     // read from address 0xx4fb9
                __asm(" move #$004001,r1");     // read from space X


                SystemHalt();   // at address P:E5D3
               // __asm(" pop r1");    // Restore r1,r0,x0
              //  __asm(" pop r0");
              //  __asm(" pop x0");
                
                //SystemHalt();
               // SystemHalt();

#else            
                SysPostMessage(3,MIXER_MASTER_FADE_OUT,10); // Mute the mixer 
                //if the STOP button is held down, lets call the shutdown menu
                SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                // if returns update display  -- if doesn't return, powered down
                DisplayHint.I |= (FMTUNER_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                SysPostMessage(3,MIXER_MASTER_FADE_IN,1); // unmute the Tuner
#endif 

#ifdef TUNER_STFM1000
                // Save power by lowering the Vddio
                DecreaseVDDIO();
#endif
                break;

	     case PH_MENU:
                 // Allow user to escape to music mode
                 bDone= TRUE;
                 // Set flag as a command for PR_MENU which always follows a PH_MENU sequence.
                 g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                break;
                
	     case PR_MENU:
                if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC == TRUE)
                {   // exit from sub menu
                    iNewMenu = MENU_MUSIC;
                }
                bDone = TRUE;
   
                break;

	     case PH_ERASE:
	     case PR_ERASE:
         	  // Erase the latest presets
                if ((iCurrentPreset>= 1) && (iCurrentPreset <= NUMBER_OF_PRESETS)&&(g_wCurrentPreset==iCurrentPreset))
                    SysPostMessage(3,TUNER_ERASE_PRESET,iCurrentPreset);
				DisplayHint.I |= (FMTUNER_MENU_DISPLAY_ALL);
         
                break;
            default:
                break;
            }

            //remember the last button event (mostly for coming out of FFWD and RWND)
            g_wLastButton=gEventInfo.Button.wButtonEvent; 

            break;  // end EVENT_BUTTON case
        } //end switch iEvent           

        if( DisplayHint.I != 0 )
        {
    	    SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
    	    DisplayHint.I = 0;
        }
    }
#ifdef TUNER_STFM1000    
    g_bEnable100msTimer = FALSE; 
#endif

#ifdef TUNER_STFM1000
#ifdef CLCD_16BIT
    //***********************************************************
    //* Following is used to make the I2S_BCLK returns stable
    //***********************************************************
    STFM1000TunerSafePowerDown();
#endif //#ifdef CLCD_16BIT
#endif
    TurnOffFMTuner();

    if (iEventSave != 0)
      DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEventSave,0,(int *)&EventInfoSave);          

#ifdef TUNER_STFM1000
    // undo Save power by lowering the Vddio
    IncreaseVDDIO();
#endif

	ReEnableExecSpeedClient(); // Re-enable the Executive's master DCLK speed client for fast button response.

#ifdef TUNER_STFM1000
    // Remove the STFM clock speed
    SysSpeedClockFree(SPEED_CLIENT_STFM_DEC);
#else
    // Remove the Mixer specific speed/voltage client.
    SysSpeedClockFree(SPEED_CLIENT_MIXER);
#endif

    // setup auto shutdown if enabled setting saved
    if (g_wPwrSettings != PWR_DISABLE)
    {
        g_MenuFlags.MENU_FLAG_POWER_DOWN_ENABLED = TRUE;
        UpdateAutoShutdownTimer();
    }
	// restore the flash access timer
	//CompactFlashTimer1Reg = iFlashAccessTimer;
    // Restore the Force Battery update every 2 seconds
    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);

#ifdef TUNER_STFM1000
    // Restore the EQ if it was enabled
    if(iTempEqSetting != EQ_NORMAL)
    {
        SysPostMessage(3,GEQ_SET_EQ,ENABLE_EQ);
        g_iEqSetting = iTempEqSetting;  
        SysCallFunction(RSRC_EQ_MENU_CODE_BANK,SendEQ,g_iEqSetting,0,0);
    }
#ifdef CLCD_16BIT    
    SysWaitOnEvent(0,0,1);  // let the decoder to stop the STFM1000
    SysPostMessage(2,LCD_16BIT_ON);
#endif //#ifdef CLCD_16BIT
#endif //#ifdef TUNER_STFM1000

    SysWaitOnEvent(0,0,1);
    return iNewMenu;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void InitializeFMTuner(void)
//
//   Type:          Function 
//
//   Description:   Initializes the analog sections of the FM tuner
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void InitializeFMTuner(void)
{
#ifdef TUNER_STFM1000
#ifdef BATTERY_TYPE_LI_ION
#if DCDC1_750KHZ
    //; The following code to set DCDC1 converter frequency to 750kHz 
    //; by setting the following bits in the HW_DCDCTBR register: 
    //; DCDC1_DIS_5BIT (bit 18), 
    //; DCDC1_CLK2X (bit 7), and 
    //; DCDC1_CLK4X (bit 6) 
    g_wDCDCTBR = HW_DCDC_TBR.I; // to reduce EMI. DW w/ input from JaH.
    HW_DCDC_TBR.B.DCDC1_DIS_5BIT = 1;
    HW_DCDC_TBR.B.DCDC1_CLK2X = 1;
    HW_DCDC_TBR.B.DCDC1_CLK4X = 1;
    // turning off half of the DCDC FETs. This can be done 
    // by setting the DCDC1_HALF_FETS bit (bit 17) of HW_DCDCTBR register
    HW_DCDC_TBR.B.DCDC1_HALF_FETS = 1;
    SysWaitOnEvent(0,0,500);
    
    // for 100 pins STMP35xx, there is only DCDC1 in chip. 
    // for 144 pins STMP35xx, there are both DCDC1 and DCDC2 in chip
    // if needed to set DCDC2 converter frequency, please refer to datasheet. It is similar to DCDC1
#endif
#endif    
    // Load the tuner code resided in the app space to reduce NAND access in FM mode
    LoadSTFM10000SecondResource(RSRC_FM_TUNER_SECOND_RSRC,
                            RSRC_TYPE_DATA,
                            (_lc_u_e_FM_TUNER_SECOND_RSRC_P_buf-_lc_u_b_FM_TUNER_SECOND_RSRC_P_buf)*3,
                            (int)_lc_u_b_FM_TUNER_SECOND_RSRC_P_buf,
                            TARGET_MEM_P);
    SysWaitOnEvent(0,0,500); 
#endif


#ifdef TUNER_STFM1000
	SysPostMessage(2,MIXER_DAC_UNMUTE);						// Unmute the line1 or line2 driver
#else
	SysPostMessage(3,MIXER_FM_SETLVL,MIN_LVOL+MIN_RVOL);	// Turn down the line1 or line2 volume
	SysPostMessage(2,MIXER_FM_UNMUTE);						// Unmute the line1 or line2 driver
#endif
	HW_HPCTRL.B.HPCLASSAB = 0;								// Switch headphones to Class A drive temporarily

#ifdef  TUNER_STFM1000 
    //	SysWaitOnEvent(0,0,1000);								// wait for a while then power-on
	SysPostMessage(2,TUNER_POWER_ON);						// Take the tuner out of standby
	SysWaitOnEvent(0,0,100);							    // Wait for the initial impulse from tuner output to settle
#else
	SysPostMessage(2,TUNER_POWER_ON);						// Take the tuner out of standby
	SysWaitOnEvent(0,0,500);								// Wait for the initial impulse from tuner output to settle

#endif
	HW_HPCTRL.B.HPCLASSAB = 1;								// Return headphones to Class AB
	SysWaitOnEvent(0,0,10);
#ifdef TUNER_STFM1000
	SysPostMessage(2,MIXER_DAC_MUTE);						// Mute the line1/line2 input
	SysWaitOnEvent(0,0,10);
#else
	SysPostMessage(2,MIXER_FM_MUTE);						// Mute the line1/line2 input
	SysWaitOnEvent(0,0,10);
#endif

#ifdef TUNER_STFM1000
    g_iVolumeMode = VOLUME_MODE_DAC;
    SysUpdateVolume();										// Update the Volume based on the Tuner's Volume Model
    SysPostMessage(2,MIXER_DAC_MUTE);
    SysWaitOnEvent(0,0,1);
    SysPostMessage(3,MIXER_MASTER_FADE_OUT,0);
    SysWaitOnEvent(0,0,30);
#else

#ifdef FMTUNER_ON_LINE2_IN
	g_iVolumeMode = VOLUME_MODE_LINE2;
#else
	g_iVolumeMode = VOLUME_MODE_LINE1;
#endif

    SysUpdateVolume();										// Update the Volume based on the Tuner's Volume Model

#ifndef  TUNER_STFM1000 
    SysPostMessage(2,MIXER_FM_MUTE);
    SysWaitOnEvent(0,0,1);
#endif
    SysPostMessage(3,MIXER_MASTER_FADE_OUT,0);
    SysWaitOnEvent(0,0,30);
#endif
#ifndef  TUNER_STFM1000 
    // DWang - I don't think we need to tune channel at this point	
    SysPostMessage(3,TUNER_TUNE_FREQUENCY,g_wCurrentFrequency);
    SysWaitOnEvent(0,0,500);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void TurnOffFMTuner(void)
//
//   Type:          Function 
//
//   Description:   Sets the analog back to being able to output from the DAC
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void TurnOffFMTuner(void)
{
#ifdef TUNER_STFM1000
    SysPostMessage(3,MIXER_MASTER_FADE_OUT,10);  
    // Power down the tuner
    SysPostMessage(2,TUNER_POWER_OFF);					   // ride the double barrel shot gun to force the tuner
    SysWaitOnEvent(0,0,1);    
    SysPostMessage(2,TUNER_POWER_OFF);					   // off.
    SysWaitOnEvent(0,0,1); 
    // Stop decoder and turn VDDD and VDDIO for Steely Dan off   
    SysPostMessage(2,DECODER_STOP);
    SysWaitOnEvent(0,0,1); 
       
    g_iPlayerState = DECODER_STOPPED;
    g_iVolumeMode = VOLUME_MODE_DAC;
    // Take the mixer off
    SysPostMessage(3,MIXER_POWER_DOWN,MIXER_OFF);
    SysWaitOnEvent(0,0,1);		
    
    SysUpdateVolume();                                      // Update the Volume based on the DAC's Volume Model  
#else
    g_iVolumeMode = VOLUME_MODE_DAC;
    SysUpdateVolume();                                      // Update the Volume based on the DAC's Volume Model  
    // Power down the tuner
    SysPostMessage(2,TUNER_POWER_OFF);					   // ride the double barrel shot gun to force the tuner
    SysPostMessage(2,TUNER_POWER_OFF);					   // off.
#endif
    // Unload FAT write  
    SysUnLoadFATWrite();
#ifdef TUNER_STFM1000
#ifdef BATTERY_TYPE_LI_ION  
#if DCDC1_750KHZ
    // Restore HW_DCDC_TBR register settings
    HW_DCDC_TBR.I = g_wDCDCTBR;
#endif
#endif
#endif
    

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void FadeOutFMTuner( void )
//
//   Type:          Function 
//
//   Description:   Gradually turn off the audio
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
void FadeOutFMTuner(void)
{
	if (iTuningMuted==0)
	{
#ifdef TUNER_STFM1000
		SysPostMessage(2,MIXER_FM_MUTE);
#else
		SysPostMessage(2,MIXER_DAC_MUTE);
#endif
		SysPostMessage(3,MIXER_MASTER_FADE_OUT,10);
		SysWaitOnEvent(0,0,2);
	}
	iTuningMuted = 1;
    
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:        _reentrant void LoadSTFM10000SecondResource(int rsrc_num, 
//										 int rsrc_type,
//										 int rsrc_size,
//										 int start_addr,
//										 int target_mem)
//
//   Type:          Function 
//
//   Description:   Load the code to app space to reduce the NAND access in FM mode
//                  to improve the STFM1000 sensitivity by over 10dB                  
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef TUNER_STFM1000
_reentrant void LoadSTFM10000SecondResource(int rsrc_num, 
                                        int rsrc_type,
                                        int rsrc_size,
                                        int start_addr,
                                        int target_mem)
{
//caution : do not put anything before this #pragma block or else some registers will get trashed.
#pragma asm
    extern	SysLoadResource
    movec SSH,y:(r7)+
    move x0,y1
    move a1,x0
    move x1,a1
    move b1,x1
    move y0,y0
    jsr	SysLoadResource
    movec y:-(r7),SSH
    nop
#pragma endasm
}
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:         _reentrant void ForceFMTInit (void)
//
//   Type:          Function 
//
//   Description:   Forces FM tuner module to initialize
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void ForceFMTInit (void)
{
#pragma asm
    include "msgequ.inc"
    extern  MODULE_NUM_FMTUNER
    extern  SignalModule
    move    #>MODULE_NUM_FMTUNER,x1
    move    #>EVENT_INIT,x0
    jsr     SignalModule
#pragma endasm
}

#ifdef TUNER_STFM1000
_reentrant void LoadSTFMDecoder(void)
{
    SysSetDecoderResource(RSRC_DEC_STFM_MOD_CODE);
#pragma asm
    include "msgequ.inc"
    extern  MODULE_NUM_DECODER
    extern  SignalModule
    move    #>MODULE_NUM_DECODER,x1
    move    #>EVENT_INIT,x0
    jsr     SignalModule
#pragma endasm
}
#endif

#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_wCurrentFrequency
    extern y:Fg_wCurrentPreset
    extern y:Fg_astPresetStations
	extern x:Fg_bSearchEndAtBandLimit

    SaveRange Fg_bSearchEndAtBandLimit,1
    SaveRange Fg_wCurrentFrequency,1
    SaveRange Fg_wCurrentPreset,1
    SaveRange Fg_astPresetStations,NUMBER_OF_PRESETS*2
#pragma endasm

#endif // #ifdef REMOVE_FM
