// Copyright (C) SigmaTel, Inc. 2001-2005
// Filename: voicemenu.c (LCDexample)
//
//
#include "types.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "voicemenu.h"
#include "menus.h"
#include "mainmenu.h"
#include "display.h"
#include "playerstatemachine.h"
#include "recorderstatemachine.h"
#include "recordsettingsmenu.h"
#include "deletemenu.h"
#include "sysmem.h"
#include "playerlib.h"
#include "hwequ.h"
#include "stringlib.h"
#include "persist_bits.h"
#include "regsrtc.h"

#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#else
#ifdef   USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif

#include "filesystem.h"
#include "project.h"  // BATT_SAFETY_MARGIN
#include "battery.h"  // SysBatteryGetLevel()
#include "micbias.h"
#include "lowresolutionadc.h"
#include "battery_config.h"
#include "ddildl.h"
#ifdef USE_PLAYLIST3


#pragma asm
    nolist
    include "sysresources.inc"
    include "sysequ.inc"
    list
#pragma endasm
#endif	// #ifdef USE_PLAYLIST3

_asmfunc INT DecoderForceInit(void);
_reentrant void ChangePlaySet(INT mode);
LONG FSFreeSpace(INT Device);
#ifdef USE_PLAYLIST5
extern BOOL g_bLastDisplay;
extern BOOL g_bLastDisplaySongCnt;
#endif

#ifdef USE_PLAYLIST3

_packed BYTE g_LastEncName[30];

void _reentrant ML_voice_menu_init(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextvoicemenu":
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

void _reentrant chop_name(_packed char *p_source, _packed char seperator)
{
	_packed char *p_start = p_source;
	_packed char *p_last = 0;
	_packed char temp;
	int byte_offset = 0, i;

	while (1)
	{
		temp = *p_start;
		for (i=0; i<3; i++)
		{
			if ((temp & 0xff) == seperator)
			{
				if (i<2)
				{
					p_last = p_start;
					byte_offset = i+1;
				}
				else
				{
					p_last = p_start+1;
					byte_offset = 0;
				}
			}
			if ((temp & 0xff) == 0)
				break;
			temp = temp >> 8;
		}
		if (i<3)
			break;
		p_start++;
	}

	if (p_last)
	{
		i = 0;
		while (1)
		{
			switch (byte_offset)
			{
			case 0:
				p_source[i] = p_last[i] & 0xffffff;
				break;
			case 1:
				p_source[i] = ((p_last[i] >> 8) & 0xffff) | ((p_last[i+1] << 16) & 0xff0000);
				break;
			case 2:
				p_source[i] = ((p_last[i] >> 16) & 0xff) | ((p_last[i+1] << 8) & 0xffff00);
				break;
			}
			if (	((p_source[i] & 0x0000ff) == 0)	||
				((p_source[i] & 0x00ff00) == 0)	||
				((p_source[i] & 0xff0000) == 0)	)
				break;
			i++;
		}
	}
}

void _reentrant AddVoiceEntry(uint24 type)
{
	if (g_LastEncName[0])
	{
		ML_voice_menu_init();
		//ML_InitVoiceParameter();
		ML_LoadVoiceParameter(type);
		song_info.unicode = 0x05; //0x00000101;
		song_info.song_name[0] = 0;
		song_info.artist_name[0] = 0;
		song_info.album_name[0] = 0;
		song_info.genre_name[0] = 0;
        song_info.g_songFastKey=g_CurrentSongFastkey;   //		song_info.g_songFastKey=0;
		song_info.year = 2036;
		g_file_time = 18;
		g_file_time |= 7 << 6;
		g_file_time |= 2005 << 12;
		song_info.path_name[0] = 0x3a41; // : A
		packed_strcat((_packed BYTE*)song_info.path_name, g_LastEncName);
		packed_strcpy((_packed BYTE*)song_info.song_name, g_LastEncName);
		chop_name((_packed char*)song_info.song_name, '/');
		ML_AddEntryToVoice(type, &song_info,VOICE_FILE_ADD);
		ML_FlushVoiceToFlash(type);
		g_LastEncName[0] = 0;
	}
	//load browse apps after finish add entry
	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);
}
#endif	// #ifdef USE_PLAYLIST3
//added to refresh bad resource.bin blocks for 70nm Toshiba nand.
_reentrant WORD SequentialRefreshSystemDriveErrorBlock(void);
_reentrant int EraseOneSystemDriveBlock(void);
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant VoiceMenu(int a, int b, int *pPtr)
//
//   Type:          Function (user menu)
//
//   Description:   This function is the voice menu
//
//   Inputs:        none used.
//
//   Outputs:       value of which menu to return to
//
//   Notes:         This function handles the voice menu.  It loops until a reason
//                  to exit is reached (PH_STOP held long enough will return MENU_EXIT, and
//                  PR_MENU will return MENU_MAIN)
//
//                  The basic loop is...
//
//                  while (not done)
//                  {
//                      Update display if necessary
//                      Wait for event
//                      Pass event to player state handler
//                      handle event locally
//                  }
//
//                  Sometimes an event may be handled entirely in the player state handler,
//                  sometimes entirely locally, sometimes in both places.
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant INT VoiceMenu(INT a, INT b, INT *pPtr)
{
    union DisplayHints DisplayHint;
    INT  iEvent;
    BOOL bDone = FALSE;
    INT  iNewMenu;
    INT  iSaveRecSource;
    BOOL bSendToPlayerStateMachine = FALSE;
#ifdef USE_PLAYLIST5
    BOOL bLastDisplaySongCnt = FALSE;
#endif
    a;b;  //here to prevent parameter not used messages.

    // This is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    iNewMenu = MENU_MAIN;
    g_iCurrentMenu = MENU_VOICE;
    DisplayHint.I = VOICE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);

    #if ENABLE_INTERNALLY_GENERATED_MICBIAS // Pass config values from player's project.inc file
    EnableStmpGeneratedMicBias(LRADC_NUM_FOR_MIC_BIAS, MICBIAS_CONFIGURED_RESISTOR_KOHMS);
    #endif // enabling the bias here allows for the needed pre-record-press mic bias settling time ~ 400 ms.

#ifndef USE_PLAYLIST3
    if(Playlist_GetPlaySet()!= PLAYSET_VOICE)
    {//sdk2.1 (check if currently in voice mode)
        //make sure we're stopped
#endif // #ifdef USE_PLAYLIST3
        SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
        g_iPlayerState = DECODER_STOPPED;
#ifndef USE_PLAYLIST3
        ChangePlaySet(PLAYSET_VOICE);
    }
#endif // #ifdef USE_PLAYLIST3
    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,InitPlayerStateMachine,1,0,0);

    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,100,MENU_MSG_ANIMATE);

    while(!bDone)
    {

        DisplayHint.I &= (VOICE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
#ifdef USE_PLAYLIST5
	if(!g_PL5_Playback.foundall)
	{
		DisplayHint.I |= 1<<DISPLAY_CURRENTTRACK_BITPOS;
		DisplayHint.I |= 1<<DISPLAY_SONGARTISTALBUM_BITPOS;
		g_bLastDisplaySongCnt = TRUE;
	}
	else if(g_bLastDisplaySongCnt == TRUE)
	{
		DisplayHint.I |= 1<<DISPLAY_CURRENTTRACK_BITPOS;
		DisplayHint.I |= 1<<DISPLAY_SONGARTISTALBUM_BITPOS;
		g_bLastDisplaySongCnt = FALSE;
	}
#endif
#if (defined USE_PLAYLIST3) || (defined USE_PLAYLIST5)
		if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_RECORD)
		{
			iEvent = EVENT_BUTTON;
			gEventInfo.Button.wButtonEvent = PR_RECORD;
			g_wLastButton = gEventInfo.Button.wButtonEvent;
			g_MenuFlags.MENU_FLAG_ESCAPE_TO_RECORD = FALSE;
		}
		else
#endif
        iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,DisplayHint.I ? 1 : -1);
        //this will return when we get a message
        //a button has been pressed.

        bSendToPlayerStateMachine = TRUE;


        //Things handled below are menu specific reactions to buttons/events
        switch(iEvent)
        {
            case EVENT_TIMER:
                SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,TRUE,0);
                DisplayHint.I = 0;
                bSendToPlayerStateMachine=FALSE;
                break;

        	case EVENT_MESSAGE:
                switch(gEventInfo.Message.wMsgCommand)
                {
                 case MENU_MSG_ANIMATE:
                     if(g_bSongStringScroll)
                     {
                         DisplayHint.bits.SongTitle = TRUE;
                         g_iSongStringOffset+=1;
                         if(g_iSongStringOffset>g_iSongStringLength)
                             g_iSongStringOffset=-SONG_ARTIST_X_SIZE;
                     }

                     bSendToPlayerStateMachine = FALSE;
                     break;
                }
                break;

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
                //remember the last button event (mostly for coming out of FFWD and RWND)
                switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
                {
                case PH_STOP:
                    //if the STOP button is held down, lets call the shutdown menu
                    SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
                    // if returns update display  -- if doesn't return, powered down
                    DisplayHint.I |= (VOICE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    bSendToPlayerStateMachine=FALSE;
                    break;

                case PH_MENU:
                    // Allow user to escape to music mode
                    bDone= TRUE;
                    // Set flag as a command for PR_MENU which always follows a PH_MENU sequence.
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    break;

                case PR_MENU:
                    if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC == TRUE)  // escape to music?
                    {   // exit from sub menu
                        iNewMenu = MENU_MUSIC;
                    }
                    bDone = TRUE;
                    bSendToPlayerStateMachine=FALSE;
                    break;

                case PR_RECORD:
                    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ExitPlayerStateMachine,1,0,0);
                    iSaveRecSource = g_ADCsource;   //save temp copy of global source
                    g_ADCsource = SOURCE_MIC;       //set source to MIC
                    RecordVoiceFile(0,0,0);
                    DisplayHint.I |= (VOICE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    g_ADCsource = iSaveRecSource;   //restore global record source
                    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,InitPlayerStateMachine,1,0,0);
                    bSendToPlayerStateMachine=FALSE;
#ifdef USE_PLAYLIST3
				AddVoiceEntry(REC_VOICE_TYPE);
				//after record should go back to browse voice
				iNewMenu = MENU_VOICE;
				bDone = TRUE;
#endif	// #ifdef USE_PLAYLIST3
                    break;

                case PR_ERASE: //sdk2.1, should go in the deletemenu (now callable from any menu)
                    SysCallFunction(RSRC_DELETE_MENU_CODE_BANK,DeleteMenu,1,PLAYSET_VOICE,0);
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = FALSE; //cause, in voice menu now, and flag=TRUE in deletemenu.c
                    DisplayHint.I |= (VOICE_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                    bSendToPlayerStateMachine=FALSE;
                    break;

                default:
                    break;
                }
                break; // end EVENT_BUTTON case
        }// end switch iEvent

        if(bSendToPlayerStateMachine)
            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);

    }

    //*****
    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);


    #if (ENABLE_INTERNALLY_GENERATED_MICBIAS) // project.inc
    // Reduces current consumption through mic by removing mic bias while not recording.
    DisableStmpGeneratedMicBias(0,0,0);
    #endif

    //don't exit player state machine, cause we want it to continue playing voice when back in main menu.
    //sdk2.1
    return iNewMenu;
}


////////////////////////////////////////////////////////////////////////////////
// Helper Function: PowerSufficiencyCheck
// Description: Tuned for DEVICE_3500 family with caller specified Activity Identifier
// Inputs: iDesiredActivityId is an encoder number
//         (or can be expanded with IDs for other power profiled activities.)
//         DCDC converter mode register is read for bat type check.
// Output: A positive int as Pre Encode Min Vbat Mv threshhold if power was sufficient
//         or a negative error code if insufficient power for requested activity.
//
// Vbat threshholds tuned with HW apps engr by profiling max bat current for 3 HW cases:
// 1 GigaByte MMC, NAND flash only, & LiIon Buck mode.
// Various _MIN_V_BAT defines used below are in project.inc. project.h is generated
//WORD wPreEncMinVbatMv=0; // a global when profiling, else a local var.
//WORD g_wHalt=FALSE; // only uncomment when profiling. Set to 1 in boxview to halt at Vbat comparison.

_reentrant int PowerSufficiencyCheck(int iDesiredActivityId)
{   int iRetCode=0; // SUCCESS by default
    int wPreEncMinVbatMv=0; //a global when profiling, else a local.
#ifdef DEVICE_3500
    int iBatteryCurrentAvg;
    // Don't attempt to record if the battery is below our safety threshold.
    // Prevents possible FAT corruption and bad file generation that could hang the decoder during decode later.
#if defined CLCD && BACKLIGHT && !(BATTERY_TYPE_LI_ION)
    if (( iDesiredActivityId == ENCODER_MP3 ) || (g_ADCsource == SOURCE_FM))
#else
    if ( iDesiredActivityId == ENCODER_MP3 )
#endif
    {
        // In MP3 encode case, prevent brownout / Vrail droop crash in a different way than wav enc case.
        // This more robust approach handles each battery type and enforces vBat limits.

        // if !BUCK mode, enforce min Vbat to allow mp3 enc, else handle buck
        if( (HW_REVR.B.DCDCMODE == DCDCMODE_2_CONVERTER1_OFF_CONVERTER2_OFF) ||
            (HW_REVR.B.DCDCMODE > DCDCMODE_3_CONVERTER1_1CH_BUCK_CONVERTER2_OFF)) //regsrevision.h
        {   //boost. Alkaline or NiMH batteries have same lower end of range.
           #ifdef MMC
            wPreEncMinVbatMv = MP3_ENC_MMC_MIN_V_BAT_MV; //global when profiling, else a local.
           #else // NAND only build has this lower Vbat thresh to allow more mp3 encoding.
            wPreEncMinVbatMv = MP3_ENC_NANDONLY_MIN_V_BAT_MV;
           #endif
        }
        else // DCDC mode is buck. This mode always has one threshhold.
        {   // lradc api SysBatteryGetLevel & SysLowResolutionAdcReadAvg don't give reasonable value
            // in buck mode so no threshholding is possible yet for buck mode (LiIon). Cause: alkaline build. See stmp5101
      //#if 0 // or #ifdef BATTERY_TYPE_LI_ION use this after Li Ion player build reverified.
            wPreEncMinVbatMv = MP3_ENC_BUCKMODE_MIN_V_BAT_MV;
      //#else
      //     return 0;// remove this line when LiIon player build is used. Then use & verify LiIon threshholding.
      //#endif
        }

        // Get the battery's most recent running average voltage
        iBatteryCurrentAvg = SysLowResolutionAdcReadAvg(BATT);
        if (iBatteryCurrentAvg == 0) iBatteryCurrentAvg = SysLowResolutionAdcReadBin(BATT);
        //DebugBuildAssert(iBatteryCurrentAvg <= x);// could check for max valid lradc sample value
        //DebugBuildAssert(g_wHalt==FALSE); // inspect threshhold, its lradc conversion, & its compare to Vbat (in lradc format)
        if(iBatteryCurrentAvg < MvToLradcResult(wPreEncMinVbatMv)) // buck had: 155h <  1a0h in A. (same vals at 4.2V or 3.7V) SysLowResolutionAdcReadAvg errored during buck mode. cause: alkaline build. See stmp5101
        {   // display low battery message
    	    DisplayClear();
    	    DisplayLowBattery();
    	    SysWaitOnEvent(0,0,2000);  // let user see it for 2 seconds
            iRetCode = -1;
        }
    }// end mp3 encode battery checks
    else// (AD)PCM wav encode has a lower Vbat threshhold
    {   if ( SysBatteryGetLevel() < BATT_SAFETY_MARGIN )//unit: % nominal range
        {   // Could display notification for the user: "Battery Too Low"
            iRetCode = -2;
        }
    }
    if(iRetCode>=0) // Tells caller Vbat threshhold passed, and which if any thresh was used
        iRetCode = (INT)wPreEncMinVbatMv;
#else
    // iRetCode is 0; // stub passes sufficiency check for non 3500 fam future devices
    #pragma asm
    WARN 'Note: non 3500 device needs pre audio encode Vbat voltage threshhold'
    #pragma endasm
#endif // ifdef DEVICE_3500
    return iRetCode;
}

extern WORD g_bEraseComplete;
extern WORD g_bResourceGetDirty;
////////////////////////////////////////////////////////////////////////////////
// Function: RecordVoiceFile
// Description: Make a voice recording using the configured encoder & g_ADCsource.
// Inputs:   int a, int b, int *pPtr     a and b are unused currently
// Output:   int next menu recommendation
//
#ifdef TUNER_STFM1000
int _reentrant RecordVoiceFile(int bDontStopDecoder, int b, int *pPtr)
#else
int _reentrant RecordVoiceFile(int a, int b, int *pPtr)
#endif
{
    INT iEvent;
	INT CallBackTimeout;
    union DisplayHints DisplayHints;
    BOOL bDone = FALSE;
    INT iNextMenu=MENU_MAIN;
    WORD wSeparator = DIRECTORY_SEPARATOR;
    BOOL bSendToRecorderStateMachine;
    _packed BYTE RecordDirectory[MAX_FOLDER_SIZE];
    _packed BYTE *pDirPtr;
    _packed BYTE wTempStr[MAX_FOLDER_SIZE];

    // this is required to get the message into the playerstatemachine
    // casted to avoid pointer mismatch warnings
    pPtr = (int *) &gEventInfo;

    // stmp5071 Checks against one of several vBat threshholds considering battery type,
    // per mmc worst case power loading, mp3/wav encode type. Provides stable low V SOC.
    if( PowerSufficiencyCheck(g_RecorderSettings[g_ADCsource].m_EncoderNo) < 0 )
        return iNextMenu;

	if(FSFreeSpace(0) <= 8192)
	{
		SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_DISK_FULL,1000,0);
		return iNextMenu;
	}

    // clear MTP database clean flag; a new object is being created, which needs to be accounted for
    ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);

    // Stmp3026 addition prevents hang: Stop the decoder per the player state.
    // Makes RecordVoiceFile a valid external entry point as
    // with calls from other menus such as the main menu.
    // Stopping the decoder allows record. ExitPSM() stops decode and sets player state.
#ifdef TUNER_STFM1000
    if( !bDontStopDecoder ) // Don't stop the decoder for STFM
#endif
    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ExitPlayerStateMachine,1,0,0);
    // Decoder must be stopped before file system operations below.

    pDirPtr = RecordDirectory;
    pDirPtr[0] = 0;

    //Add Slash
    packed_strcat(pDirPtr,(_packed BYTE*)(&wSeparator));

#ifndef USE_PLAYLIST3
    if (g_RecorderSettings[g_ADCsource].m_EncoderNo != ENCODER_MP3)


	{
#endif
        // load MP3 encoded files to root so skip this part
        //Load Proper Directory
        switch(g_ADCsource)
        {
            case SOURCE_FM:
                ((WORD*)wTempStr)[0]=FMDIR_0;    //word 0 (3 chars)
        	    break;

            case SOURCE_LINEIN:
                ((WORD*)wTempStr)[0]=LINE1DIR_0;    //word 0 (3 chars)
                ((WORD*)wTempStr)[1]=LINE1DIR_1;      //word 1
                ((WORD*)wTempStr)[2]=LINE1DIR_2;      //word 2
        	    break;

            //default to voice directory or SOURCE_MIC
            default:
                ((WORD*)wTempStr)[0]=VOICEDIR_0;      //word 0 (3 chars)
                ((WORD*)wTempStr)[1]=VOICEDIR_1;      //word 1
        	    break;
        }
        packed_strcat(pDirPtr,(_packed BYTE*)(wTempStr));//add the directory name
        packed_strcat(pDirPtr,(_packed BYTE*)(&wSeparator)); //add the trailing slash
#ifndef USE_PLAYLIST3
    }
#endif

   SysLoadFATWrite(); // Locates NandForegroundGarbageCollector overlaid func in P. Also needed for FSCreateDir below.

    // Attempt to change to the Voice directory
    if(FSChangeDir(RecordDirectory,g_RecorderSettings[g_ADCsource].m_iDestinationDevice)<0)
    {
        //couldn't change to the voice directory, lets try to create it.

        //end slash must be removed so re-initialize RecordDirectory and pointer.
        pDirPtr = RecordDirectory;
        pDirPtr[0] = 0;
        //Re-build RecordDirectory w/o end slash
        packed_strcat(pDirPtr,(_packed BYTE*)(&wSeparator));
        packed_strcat(pDirPtr,(_packed BYTE*)(wTempStr));//add the directory name

        if(FSCreateDir(RecordDirectory,g_RecorderSettings[g_ADCsource].m_iDestinationDevice)<0)
        {//this verifies the creation did not fail.
            bDone = TRUE;
        }

    }
   SysUnLoadFATWrite(); // also flushes drives (NandForegroundGarbageCollector frees blocks & closeUpdate blocks)
                        // for optimal state for min latency encoder writes. (pushes out enc overflows)

    if(!bDone)//if bDone is already TRUE, then we can't open the file.
    {
    	SysCallFunction(RSRC_RECORDER_STATE_MACHINE_CODE_BANK,InitRecorderStateMachine,0,0,0);
        DisplayHints.I = RECORDING_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
        while(!bDone)
        {
            DisplayHints.I &= (RECORDING_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));

            if(g_bEraseComplete  && g_bResourceGetDirty)
				CallBackTimeout = 1;
			else
				CallBackTimeout = 0;

			iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg,(DisplayHints.I) ? 1: (CallBackTimeout?1:-1));
            // Above call returns when we get a message or button event

            if((!g_bEraseComplete) && g_bResourceGetDirty)
				SysCallFunction(RSRC_RESTORESYSDRIVE_P_CODE,EraseOneSystemDriveBlock,0,0,0);
			else
			{	if(g_bEraseComplete  && g_bResourceGetDirty)
					SysCallFunction(RSRC_RESTORESYSDRIVE_P_CODE,SequentialRefreshSystemDriveErrorBlock,0,0,0);
			}

            bSendToRecorderStateMachine = TRUE; // send events to recorder state machine by default
            switch(iEvent)
            {
                case EVENT_TIMER:
                    SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHints.I,TRUE,0);
                    DisplayHints.I= 0;
                    bSendToRecorderStateMachine = FALSE;
                    break;

                case EVENT_MESSAGE:
                    switch(gEventInfo.Message.wMsgCommand)
                    {
                        case MENU_RECORDER_STATE_CHNG:
                            //If hint is anything other than RECORDER_START, we have stopped recording
                            switch (gEventInfo.Message.wMsgParms[0])
                            {
                                case RECORDER_START:
                                case RECORDER_PAUSE:
                                case RECORDER_RESUME:
                		            DisplayHints.bits.PlayState = TRUE;
                                    break;

                                default:
                                    bDone = TRUE;
                                    break;
                            }
                            break;

                        case MENU_SONG_TIME_CHNG:
                            // Once per second, conditionally display "Device Full!" warning.  DVRWARN
                            DisplayHints.bits.RecordMode = TRUE;
                            break;

                        case MENU_RECORDER_WRITE_DATA:
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
                    //remember the last button event (mostly for coming out of FFWD and RWND)
                    switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
                    {
                        case PR_MENU:
                        case PR_STOP:
#ifdef USE_PLAYLIST3
					packed_strcpy(g_LastEncName, g_EncFileNameString);
#endif	// #ifdef USE_PLAYLIST3

                              bDone = TRUE;
                            break;
                        default:
                            break;
                    }
                    break;
            } // end switch(iEvent)

            if(bSendToRecorderStateMachine)
                DisplayHints.I |= SysCallFunction(RSRC_RECORDER_STATE_MACHINE_CODE_BANK,HandleRecorderStateMachine,iEvent,0,pPtr);

        } // end while(!bDone)


        SysCallFunction(RSRC_RECORDER_STATE_MACHINE_CODE_BANK,ExitRecorderStateMachine,0,0,0);

    }
    else
    {//Put up a message about an error.
    }


    return iNextMenu; // if called from mainmenu, then return MENU_MAIN.
}

#ifdef USE_PLAYLIST3
#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_iHighestVoiceNumber
    extern y:Fg_iHighestFMNumber
    extern y:Fg_iHighestLineNumber

    SaveRange Fg_iHighestVoiceNumber,1
    SaveRange Fg_iHighestFMNumber,1
    SaveRange Fg_iHighestLineNumber,1

#pragma endasm
#endif	// #ifdef USE_PLAYLIST3
