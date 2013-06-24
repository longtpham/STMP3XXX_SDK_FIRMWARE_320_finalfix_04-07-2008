////////////////////////////////////////////////////////////////////////////////
// Copyright (C) SigmaTel, Inc. 2002-2004
// Filename:     recorderstatemachine.c (LCD player)
// Description : Starts up/shuts down mixer and encoder module.
// 			     Handles common encoding functionality.
// InitRecorderStatMachine enables SOC internally generated mic Vbias for 35xx- moved to voicemenu.c
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "mainmenu.h"
#include "resource.h"
#include "recorderstatemachine.h"
#include "recordsettingsmenu.h"
#include "hwequ.h"
#include "buttons.h"
#include "sysmem.h"
#include "encoderproperties.h"
#include "playerlib.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "playerstatemachine.h"
#else
#ifdef   USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif
#include "stringlib.h"
#include "display.h"
#include "menus.h"
#include "ddildl.h"
#include "project.h" // mic bias config macros as input params for API call
#include "micbias.h"
#include "batterycharge.h"

extern int g_ADCsource;

int g_iRbrSetting = 0;
_reentrant INT StopEncoder(void);
_reentrant INT StartEncoderRecord(void);
_reentrant void SysSetEncoder(WORD);


_asmfunc void EncoderForceInit(void);
_asmfunc void DecoderForceInit(void);

_asmfunc MMCCheckWriteProtectWrapper(void*);

extern _packed BYTE g_LastEncName[];

volatile int g_bEncoderLowSpace = FALSE;
WORD g_bWriteProtected = FALSE;

////////////////////////////////////////////////////////////////////////////////
// Function:    InitRecorderStateMachine()
// Description:
// Inputs:      int a, int b, int c  Input params are currently unused.
//              Mic bias macros from project.inc
// Outputs:     int SUCCESS
// Notes:
////////////////////////////////////////////////////////////////////////////////
_reentrant int InitRecorderStateMachine(int a, int b, int *c)
{
    int LeftSource, RightSource;
    a; b; c;

    #if ENABLE_INTERNALLY_GENERATED_MICBIAS // project.inc
    // Can enable mic bias here, but we now do that on voice menu entry
    // to allow bias settling time. To record outside of voicemenu,
    // its here but does nothing if already biased. Similar for disable in function below.
    // Note that the mainmenu can also enable the mic bias & leave it on, if configured in project file.
    // API internal mic bias counting semaphore check & increment is the mechanism.
    // Pass config values from player's project.inc file
    EnableStmpGeneratedMicBias(LRADC_NUM_FOR_MIC_BIAS, MICBIAS_CONFIGURED_RESISTOR_KOHMS);
    #endif

    SysPostMessage(3,MIXER_ADC_SETLVL, CURRENTADCGAIN );

    // SysPostMessage(2,MIXER_ADC_UNMUTE);  moved this unmute until after both L&R channels are enabled.
	// power up adc
    HW_MIXPWRDNR.B.PR0 = 0;
	HW_ADCCSR.B.CLKGT = 0;


    switch(g_ADCsource)
    {
        case SOURCE_FM:
        	LeftSource = FM_LEFT_SOURCE;
        	RightSource = FM_RIGHT_SOURCE;
#ifndef TUNER_STFM1000
        	SysPostMessage(2,MIXER_FM_UNMUTE);
			//Re-set ADC gain to reduce distortion when recording from FM
			// SysPostMessage(3,MIXER_ADC_SETLVL,HW_MIXADCGAINR_GL_06P0_SETMASK|HW_MIXADCGAINR_GR_06P0_SETMASK);
			SysPostMessage(3,MIXER_ADC_SETLVL,HW_MIXADCGAINR_GL_00P0_SETMASK|HW_MIXADCGAINR_GR_00P0_SETMASK);
			//Can set record level here, but will override volume setting from FM Tuner
        	//SysPostMessage(3,MIXER_FM_SETLVL,HW_MIXLINE2INVR_GN_ZERO_SETMASK);
#endif
        break;

        case SOURCE_LINEIN:
        	LeftSource = LINE1_LEFT_SOURCE;
        	RightSource = LINE1_RIGHT_SOURCE;
        	SysPostMessage(2,MIXER_LINE_UNMUTE);
           #if defined(DEVICE_3500)
        	//3500 define
           #else
        	SysPostMessage(3,MIXER_LINE_SETLVL,HW_MIXLINE1INVR_GN_ZERO_SETMASK);
           #endif
			//Re-set ADC gain to reduce distortion when recording from Line-In
			// SysPostMessage(3,MIXER_ADC_SETLVL,HW_MIXADCGAINR_GL_06P0_SETMASK|HW_MIXADCGAINR_GR_06P0_SETMASK);
			SysPostMessage(3,MIXER_ADC_SETLVL,HW_MIXADCGAINR_GL_00P0_SETMASK|HW_MIXADCGAINR_GR_00P0_SETMASK);
            break;

        default:	//Default to Mic
			// Re-set ADC gain to reduce distortion when recording from mic.
			// stmp5204: Was 22.5dB but clipped; 16P5 dB clipped. 15.0 dB adc amp gain gives full scale w/o clipping.
			SysPostMessage(3,MIXER_ADC_SETLVL,HW_MIXADCGAINR_GL_15P0_SETMASK|HW_MIXADCGAINR_GR_15P0_SETMASK);

        	LeftSource = MIC_LEFT_SOURCE;
        	RightSource = MIC_RIGHT_SOURCE;
        	SysPostMessage(3,MIXER_MIC_SETLVL,ENCODERCURRENTMICROPHONEVOLUME);
        	SysPostMessage(2,MIXER_MIC_UNMUTE);
        	SysPostMessage(2,MIXER_MIC_BOOST);
        	break;
     }

#ifdef TUNER_STFM1000
	if (g_ADCsource != SOURCE_FM)
#endif
    SysPostMessage(4,MIXER_REC_SELECT,LeftSource,RightSource);
	SysWaitOnEvent(0,0,1); // let the modules run to process the messages

    g_iEncCurrentSeconds = 0;
    StartEncoderRecord();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
// Function:    ExitRecorderStateMachine()
// Description: Powers down adc. Mutes adc source. Disables mic bias if requested.
// Inputs:      int a, int b, int *c  Input params are currently unused.
// Outputs:     int SUCCESS
// Notes:
////////////////////////////////////////////////////////////////////////////////
_reentrant int ExitRecorderStateMachine(int a, int b, int *c)
{
    a; b; c;

	// power down adc
    HW_MIXPWRDNR.B.PR0 = 1;
	HW_ADCCSR.B.CLKGT = 0;

	StopEncoder();

    switch(g_ADCsource)
    {
        case SOURCE_FM:
#ifdef TUNER_STFM1000
        	if(g_iCurrentMenu != MENU_FMTUNER) //i.e. if recording from fmtuner then don't mute as
											   //		you like to continue listening
				SysPostMessage(2,MIXER_FM_MUTE);
#endif
			break;

        case SOURCE_LINEIN:
        	SysPostMessage(2,MIXER_LINE_MUTE);
            break;

        default:	//Default to Mic
        	SysPostMessage(2,MIXER_MIC_MUTE);
        	break;
     }

    SysPostMessage(2,MIXER_ADC_MUTE);
    SysPostMessage(2,DISABLE_RIGHT_CH);


    #if ENABLE_INTERNALLY_GENERATED_MICBIAS // project.inc
    // Can disable internally generated mic bias here although we currently disable on voice menu exit
    // per long bias settling time. API internal counting semaphore decrements & checks to handle all cases.
    // Reduces current consumption through mic by removing mic bias while not recording (if not api users remain).
    DisableStmpGeneratedMicBias(0,0,0);
    #endif

#ifdef TUNER_STFM1000
#ifndef USE_PLAYLIST3
    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,((g_ADCsource==SOURCE_FM) &&(g_iCurrentMenu == MENU_FMTUNER)),0,0);
#else
	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ML_browsing_app_init,((g_ADCsource==SOURCE_FM) &&(g_iCurrentMenu == MENU_FMTUNER)),0,0);
#endif
#else
#ifndef USE_PLAYLIST3
    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
#else
	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ML_browsing_app_init,0,0,0);
#endif
#endif

//#ifndef USE_PLAYLIST3
//#ifdef TUNER_STFM1000
//    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,((g_ADCsource==SOURCE_FM) &&(g_iCurrentMenu == MENU_FMTUNER)),0,0);
//#else
//    SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
//#endif
//#endif
	return 0;
}


_reentrant int HandleRecorderStateMachine(int iEvent, int b, int *pMessagePtr)
{
    union DisplayHints DisplayHint;
    int rtn=0;
    union EventTypes *pMsg = (union EventTypes * )pMessagePtr;
    b;
    DisplayHint.I = 0;
    switch (iEvent)
    {
        case EVENT_BUTTON:
        	switch(pMsg->Button.wButtonEvent)
    		{
            case PR_MENU:
            case PR_STOP:
 				if (g_wEncoderSR & ENCODER_PAUSED)
				{
	    			SysPostMessage(2,ENCODER_RECORD);
            		SysWaitOnEvent(0,0,0);
				}
                 break;
            case PR_RECORD:
				if (g_wEncoderSR & ENCODER_RECORDING)	 // mute to reduce clicking
				{
				    SysPostMessage(2,MIXER_MIC_MUTE);
            		SysWaitOnEvent(0,0,0);
				}
	    		SysPostMessage(2,ENCODER_RECORD);
            	SysWaitOnEvent(0,0,0);
                 break;
            default:
                 break;
            }
        case EVENT_MESSAGE:
            switch(pMsg->Message.wMsgCommand)   // DVRWARN
            {
            case MENU_RECORDER_STATE_CHNG:
				switch (pMsg->Message.wMsgParms[0])
				{
					case RECORDER_RESUME:
                		DisplayHint.bits.PlayState = TRUE;
                        if (g_ADCsource == SOURCE_MIC)
                        {
                          SysPostMessage(2,MIXER_MIC_UNMUTE);
                          SysWaitOnEvent(0,0,0);
                        }
						break;
					case RECORDER_PAUSE:
                		DisplayHint.bits.PlayState = TRUE;
						break;
                    case RECORDER_STOP_OUT_OF_SPACE:
#ifdef USE_PLAYLIST3
                         packed_strcpy(g_LastEncName, g_EncFileNameString);
#endif	// #ifdef USE_PLAYLIST3
                         SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_DISK_FULL,1000,0);
						break;
					default:
						break;
				}
				break;
            case MENU_MSG_ENC_LOW_SPACE:
                g_bEncoderLowSpace = TRUE;
                break;
            case RECORDER_STOP_LOW_BATTERY:    // Encoder stopped due to low battery.
                DisplayHint.bits.Battery = TRUE;            // Refresh battery level icon.
                break;
            case MENU_SONG_TIME_CHNG:
	            DisplayHint.bits.EncoderTrackTime = TRUE;
                break;
            case MENU_SONG_TRACK_CHNG:
	            DisplayHint.bits.EncoderTrackName = TRUE;
                break;
#ifdef BACKLIGHT
            case MENU_MSG_TURN_OFF_BACKLIGHT:
    	    	SysPostMessage(2,LCD_BACKLIGHT_OFF);
        	    SysWaitOnEvent(0,0,0);
			   	break;
#endif
#ifdef BATTERY_CHARGE
			case MENU_BATTERY_CHARGER_TIMEOUT:
				//check on battery charging.
				SysCallFunction(RSRC_BATTERY_CHARGE_CODEBANK,BatteryChargeStateMachine,0,0,0);
				break;
#endif	//BATTERY_CHARGE

            default:
                break;
            }
        default:
            break;
    }
    return DisplayHint.I;
}

_reentrant int StopEncoder(void)
{

	if(g_wEncoderSR & ENCODER_RECORDING)
	{
	    SysPostMessage(2,ENCODER_STOP);
        while(g_wEncoderSR & ENCODER_RECORDING){
            SysWaitOnEvent(0,0,0);
            }
	}
	return 0;
}

#ifdef MP3_ENCODE
_reentrant RETCODE MP3_GetNextFilename(int iDevice, int iFilenameBufferLength, int*pPtr);
#endif
_reentrant RETCODE Record_GetNextFilename(int iDevice, int iFilenameBufferLength, int*pPtr);

_reentrant int StartEncoderRecord(void)
{
   //int iSource;

    int iDevice = g_RecorderSettings[g_ADCsource].m_iDestinationDevice;
    _packed BYTE sVoiceFileName[20];
    EncProperties props;

#ifdef TUNER_STFM1000
    if( g_ADCsource != SOURCE_FM )  // Don't stop decoder for STFM encode
#endif
    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
    props.device = iDevice;
    props.samplingRateInHz = g_RecorderSettings[g_ADCsource].m_iSampleRateInHz;
#ifdef MMC
//    DriveGetInfo(1, DriveInfoIsWriteProtected, &bWriteProtected);
    if (iDevice == DESTINATION_EXTERNAL)
    {
//        DriveGetInfo(1, DriveInfoIsWriteProtected, &g_bWriteProtected);
      // g_bWriteProtected is updated when checkremovablemedia sees a media change.
        if (g_bWriteProtected)
          return 1;
    }

#endif
    if(g_RecorderSettings[g_ADCsource].m_EncoderNo == ENCODER_MP3) {
#ifdef MP3_ENCODE

	    SysSetEncoder(ENCODER_TYPE_MP3);
        g_wEncADCGain = DIGITALGAINFORMP3RECORD;
        props.FormatTag = ENCODER_MP3;
        props.bitRateInKbps   =   g_RecorderSettings[g_ADCsource].m_iBitRateInKbps;

        if(props.bitRateInKbps == 96)
            g_iRbrSetting = BITRATE_96000HZ;
        else if(props.bitRateInKbps == 112)
            g_iRbrSetting = BITRATE_112000HZ;
        else if(props.bitRateInKbps == 128)
            g_iRbrSetting = BITRATE_128000HZ;
        else if(props.bitRateInKbps == 160)
            g_iRbrSetting = BITRATE_160000HZ;
        else if(props.bitRateInKbps == 192)
            g_iRbrSetting = BITRATE_192000HZ;
        else if(props.bitRateInKbps == 224)
            g_iRbrSetting = BITRATE_224000HZ;
        else if(props.bitRateInKbps == 256)
            g_iRbrSetting = BITRATE_256000HZ;
        else if(props.bitRateInKbps == 320)
            g_iRbrSetting = BITRATE_320000HZ;

	    SysCallFunction(RSRC_MP3_IMPLEMENTATION,MP3_GetNextFilename,iDevice,20,(void*)sVoiceFileName);
#endif
    }
    else {
        if(g_RecorderSettings[g_ADCsource].m_EncoderNo == ENCODER_IMADPCM)
           props.FormatTag = IMADPCM;
        else if(g_RecorderSettings[g_ADCsource].m_EncoderNo == ENCODER_MSADPCM)
           props.FormatTag = MSADPCM;
        else if(g_RecorderSettings[g_ADCsource].m_EncoderNo == ENCODER_PCM)
           props.FormatTag = WPCM;

		SysSetEncoder(ENCODER_TYPE_ADPCM_IMA);

		props.EncPCMBits = g_RecorderSettings[g_ADCsource].m_ibits;
        SysCallFunction(RSRC_SYSRECORD_CODE,Record_GetNextFilename,iDevice,20,(void*)sVoiceFileName);
    }
    // this must follow the xxxx_GetNextFilename() to be loaded in EncSetProperties()
    props.pFilename = sVoiceFileName;

    // turn on right channel if recording stereo
#ifdef TUNER_STFM1000
	if (g_ADCsource != SOURCE_FM)
#endif
    if ((props.EncNumberOfChannels = g_RecorderSettings[g_ADCsource].m_iChannels) == CHANNELS_STEREO) {
       SysPostMessage(2,ENABLE_RIGHT_CH);
	   SysWaitOnEvent(0,0,0); // to yeild to mixer module
	}


	// Note: Always unmute Left and Right channel ADC's after powering up right channel to avoid
	// blip at the begining of the right channel of the recorded file.
#ifdef TUNER_STFM1000
	if (g_ADCsource != SOURCE_FM)
#endif
	SysPostMessage(2,MIXER_ADC_UNMUTE);

	SysWaitOnEvent(0,0,0); // to yeild to mixer module

    EncSetProperties(&props);
    EncoderForceInit();
    SysPostMessage(2,ENCODER_RECORD);
    SysWaitOnEvent(0,0,1);//must have this wait so that the encoder can begin recording.
	return 0;
}

_reentrant void SysSetEncoder(unsigned int EncoderType)
{
    EncoderType = 1<<EncoderType ;
    __asm(" extern SysSetEncoder \n move %0,x0 \n jsr SysSetEncoder"::"r"(EncoderType));
}




