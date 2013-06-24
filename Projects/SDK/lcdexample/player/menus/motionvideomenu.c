////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2007
//
// File        : motionvideomenu.c
// Description : motion video menu
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "fstypes.h"
#include "fsproj.h"
#include "fsapi.h"
#include "fserror.h"
#include "menumanager.h"
#include "messages.h"
#include "resource.h"
#include "buttons.h"
#include "syscoder.h"
#include "sysmem.h"
#include "musicmenu.h"
#include "menus.h"
#include "mainmenu.h"
#include "display.h"
#include "playerstatemachine.h"
#include "deletemenu.h"

#include "battery.h"
#include "lowresolutionadc.h"
#ifdef BATTERY_TYPE_LI_ION
#define SMV_BATTERY_THRESHOLD 3300 //3300mV
#else
#define SMV_BATTERY_THRESHOLD 1200 //1200mV
#endif

#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "jpegfileutil.h"
#include  "musiclib_ghdr.h"
#else
#ifdef USE_PLAYLIST5
#include "playlist5.h"
#endif
#endif
#endif
#endif
#include "stringlib.h"
#include "hwequ.h"
#include "FileSpec.h"
#include "playerlib.h"

#include "sysspeed.h"
#include "sysvolume.h"
#include "smvjpegdecodedisplay.h"

	#pragma asm
	nolist
	include "sysresources.inc"
	include "sysequ.inc"
	list
	#pragma endasm

#define MVIDEO_ERROR        1
#define MVIDEO_SUCCESS      0
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define HALDisplaySendCommand SMV_HALDisplaySendCommand
#define HALDisplaySendDataCommand SMV_HALDisplaySendDataCommand
#define HALDisplaySendData SMV_HALDisplaySendData
#define HALDisplaySendPixelData SMV_HALDisplaySendPixelData
#define HALDisplaySetContrast SMV_HALDisplaySetContrast
#define HALDisplayDecContrast SMV_HALDisplayDecContrast
#define HALDisplayIncContrast SMV_HALDisplayIncContrast
#define HALDisplayBacklightOn SMV_HALDisplayBacklightOn
#define HALDisplayBacklightOff SMV_HALDisplayBacklightOff
#define HALHandleMessage SMV_HALHandleMessage
#define HALDisplayReadStatus SMV_HALDisplayReadStatus
#define HALSetScreenSaver SMV_HALSetScreenSaver
#define HALScreenSaverType SMV_HALScreenSaverType

#define SMV_Display_Driver

#ifdef S6B33B0A_LCD
#define IGNORE_LCD16BIT_CONTROL
#ifdef COLOR_262K
#include "s6b33b0a_262k.c"
#else
#include "s6b33b0a_65k.c"
#endif
#endif

extern unsigned long ReadRTC(void);

void _reentrant SyncSMVFrame(void);
int _reentrant DisplaySMV(void);
int _reentrant DisplaySMVOneFrame(void);
void _reentrant SMV_DisplayBitmap(JpegControl * pJpeg);
void _reentrant UpdateBacklightOffTimer (void);

extern _asmfunc void DecoderForceInit(void);

extern int DecoderResourcePtr;
extern WORD g_wSongByteTotalHigh;
extern WORD g_wSongByteTotalLow;
extern DWORD g_SmvWave_dwAvgBytesPerSec;


#ifdef USE_PLAYLIST3
static INT g_wCurrentDirDevId_backup;
static INT g_wTotalSongCount_backup;
static INT g_wCurrentSongNumber_backup;
static INT g_wCurrentSongNumber_return;
static INT g_current_size_backup;
static INT g_current_index_backup;
int _reentrant HandlePlayerStateMachinePlaylist3(int iEvent, int iIgnored, int *pMessagePtr, int *ptr1, int *ptr2, int *ptr3);
RETCODE _reentrant StartSMVDecoder(int mode);
void _reentrant SMVBlackBackground(void);
#endif //#ifdef USE_PLAYLIST3

extern int _reentrant OpenSMVFile(int a, int b, int *pPtr);
extern int _reentrant CloseSMVFile(int a, int b, int *pPtr);

extern unsigned int (*SmvJpegInit)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct);
extern unsigned int (*SmvJpegDecode)(j_app_ptr Jpeg_dec_app_struct, j_inputbuf_ptr Jpeg_dec_input_buff_struct, j_outputbuf_ptr Jpeg_dec_output_buff_struct);
extern int _reentrant MotionVideoMenuInitState(int a, int b, int *pPtr);

extern JpegControl SmvCurrentJpeg;
extern unsigned int SmvInputBuffer[];
extern unsigned int	SmvOutputBuffer[];

#ifdef USE_PLAYLIST5
extern BOOL g_bLastDisplay;
extern BOOL g_bLastDisplaySongCnt;
#endif

//motion video application
/*static*/ INT stc_iHandle =- 1;
/*static*/ _X INT stc_iByteinJpegBlock, stc_iVideoWidth, stc_iVideoHeight, stc_iFrameHeight; // stc_iFrameHeight is aligned to 16 boundary
/*static*/ _X INT stc_iFrameDuration, stc_iFramePerSec, stc_iFrameInJpegBlock, stc_iDurationInJpegBlock;
/*static*/ _X INT stc_iFrameDrop, stc_iNumFrameStop;
/*static*/ _X LONG stc_dwLastTime, stc_dwCurrentTime;
/*static*/ _X LONG stc_dwTotalFrame, stc_dwTotalFrameBackup, stc_dwVideoStart;
//JPEG decoder interface for SMV
static _X INT stc_iByteLeave, stc_iByteDummy, stc_iCurrentFrame;

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant MotionVideoMenu(int a, int b, int *pPtr)
//
//   Type:          Function (user menu)
//
//   Description:   This function is the music menu
//
//   Inputs:        none used.
//
//   Outputs:       value of which menu to return to
//
//   Notes:         This function handles the music menu.  It loops until a reason
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
int _reentrant MotionVideoMenu(int a, int b, int *pPtr)
{
    INT iVolumeHoldCount = 0;
    INT iSkipHoldCount = 0;
	union DisplayHints DisplayHint;
	INT  iEvent;
	BOOL bDone = FALSE;
    BOOL bSendToPlayerStateMachine = FALSE;
    int RetValue=NOERROR;
    union EventTypes *pMsg;
	int  iNewMenu = MENU_MAIN;
	BOOL bStartFlag;
	BOOL bPrMenu = FALSE;

#ifdef USE_PLAYLIST3
#ifdef MMC
	BOOL bMediaChanged;
#endif
    BOOL            bSendToPlayerStateMachinePlaylist3 = FALSE;
	_packed char    szFilename[MAX_FILENAME_LENGTH];
	SONGFILEINFO    SongInfo;
#endif
#ifdef USE_PLAYLIST5
	WORD OriginEvent;
    BOOL bLastDisplaySongCnt = FALSE;
#endif

    // if battery is lower than a pre-defined voltage threshold
    if(SysLowResolutionAdcReadBin(BATT) <= MvToLradcResult(SMV_BATTERY_THRESHOLD))
    {   // if decoder is not stop, stop it.
        if(!(g_wDecoderSR&DECODER_STOPPED))
    	{   SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
#ifdef USE_PLAYLIST3
	        SysWaitOnEvent(0,0,0);
#endif
            g_iPlayerState = DECODER_STOPPED;
    	    // if it is already smv decoder, change back to the music decoder
    	    if(DecoderResourcePtr == RSRC_DEC_SMVADPCM_MOD_CODE)
    		{   SysSetDecoderResource(RSRC_DECMOD_CODE);
    	        DecoderForceInit();
    		    SysPostMessage(2, DECODER_RESET);
    		}
    	}
    	// close smv file
        SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
	    // display low battery message
	    DisplayClear();
	    DisplayLowBattery();
	    SysWaitOnEvent(0,0,2000);  // let user see it for 2 seconds
	    return(iNewMenu);   // return to mainmenu
	}
	SysSpeedIncrease(SPEED_PEAK, SPEED_CLIENT_JPEG_DEC);

	// this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;
    pMsg = (union EventTypes * )pPtr;

	a;b;//here to prevent parameter not used messages.

#ifdef USE_PLAYLIST3
	SongInfo.m_wBufferLength = MAX_FILENAME_LENGTH;
	SongInfo.m_pFilename = (_packed BYTE*)szFilename;
#endif
	//set our current menu
	g_iCurrentMenu = MENU_MVIDEO;

    SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK, MotionVideoMenuInitState, 0, 0, (int *)&DisplayHint);

#ifndef USE_PLAYLIST3
	if(Playlist_GetPlaySet() != PLAYSET_MVIDEO)
	{   //sdk2.1 (i.e. if currently not in music mode.)
		//initialize the music player state machine
        SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
        g_iPlayerState = DECODER_STOPPED;
        SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
		ChangePlaySet(PLAYSET_MVIDEO);
	}
#else
    g_wCurrentDirDevId_backup   = g_wCurrentDirDevId;
    g_wTotalSongCount_backup    = g_wTotalSongCount;
    g_wCurrentSongNumber_backup = g_wCurrentSongNumber;
    g_current_size_backup       = g_current_size;
    g_current_index_backup      = g_current_index;

    g_wCurrentDirDevId = 0;
    SysCallFunction(RSRC_JPEG_FILEUTIL_EXTRA_MENU_CODE_BANK,JpegFile_SetPlaySet,PLAYSET_MVIDEO,TRUE,0);

    g_current_size = g_wTotalSongCount;
    if(g_wDecoderSR & DECODER_STOPPED)
    {   g_wCurrentSongNumber = 0;
    }else
    {   g_wCurrentSongNumber = g_wCurrentSongNumber_return;
    }
    g_current_index = g_wCurrentSongNumber;
    g_wCurrentSongNumber_return = 0;
    JpegFile_GetCurrentSongFileInfo(0,0,(INT*)&SongInfo);
    DisplayHint.bits.SongTitle = TRUE;
#endif

	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, InitPlayerStateMachine,0,0,0);

	//***** Moved from mainmenu.c
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
	SysWaitOnEvent(0,0,0);// Let system process SOFT_TIMER_KILL_TIMER message

	//Lets initially display all & clear the screen
    DisplayHint.I = (1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
//
#ifdef USE_PLAYLIST2
		SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
#endif
//end

#ifdef USE_PLAYLIST3
    if(g_wDecoderSR & DECODER_PLAYING)
    {   DisplayHint.I &= ~(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
        SMVBlackBackground();
    }
#endif
    if (stc_iHandle == -1 && (g_iPlayerState == DECODER_PLAYING))  {
#ifdef BACKLIGHT
        SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BACKLIGHT);
#endif
        SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,OpenSMVFile,0,0,0);
        SyncSMVFrame();
    }
    else
	    DisplayHint.I |= MVIDEO_MENU_DISPLAY_ALL;

    bStartFlag = TRUE;
#ifdef USE_PLAYLIST3
#ifdef MMC
    bMediaChanged = FALSE;
#endif
#endif
	while(!bDone)
	{
		//keep only display hints we're interested in displaying in this menu.
		DisplayHint.I &= (MVIDEO_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
#ifdef USE_PLAYLIST5
        if (g_iPlayerState == DECODER_STOPPED)
        {
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
        }
#endif
		if (g_wDecoderSR & DECODER_PLAYING)
		{    //if playback is in progress, get back from system to render video frquently
		    iEvent = SysWaitOnEvent(EVENT_BUTTON|EVENT_MESSAGE, &gEventInfo.msg, 1);
		    //iEvent = SysWaitOnEvent(EVENT_BUTTON|EVENT_MESSAGE, &gEventInfo.msg, 0);
        }else
        {   iEvent = SysWaitOnEvent(EVENT_BUTTON|EVENT_MESSAGE, &gEventInfo.msg, DisplayHint.I? 1 : -1);
        }

        //Lets assume we do want to send all these to the player state machine
        bSendToPlayerStateMachine = TRUE;
#ifdef USE_PLAYLIST3
        bSendToPlayerStateMachinePlaylist3 = FALSE;
#endif
		switch(iEvent)
		{
    		case EVENT_TIMER:
				//update all UI before rendering video
				if(DisplayHint.I)   {
#ifdef USE_PLAYLIST3
                    if ((bStartFlag == TRUE) && (g_wDecoderSR & DECODER_PAUSED))
                    {   SMVBlackBackground();
                    }else
                    {   SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
    					SysWaitOnEvent(0,0,0);
                    }
#else
					SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
					SysWaitOnEvent(0,0,0);
#endif
            		if ((bStartFlag == TRUE) && (g_wDecoderSR & DECODER_PAUSED))
            		{   SyncSMVFrame();
                        DisplaySMVOneFrame();
                        bStartFlag = FALSE;
                    }
				}
				DisplayHint.I = 0;//clear out the bits

				//render video
    		    if (stc_iHandle != -1 && (g_wDecoderSR & DECODER_PLAYING))
   		            DisplaySMV();
				bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
    			break;
            case EVENT_MESSAGE:
                switch(gEventInfo.Message.wMsgCommand)
                {
					case MENU_SONG_TIME_CHNG:   // No need to update song time
                        bSendToPlayerStateMachine = FALSE;
                        break;
                    case MENU_MSG_MEDIA_CHANGED:
                        iNewMenu = MENU_MAIN;
                    	bDone = TRUE;
#ifdef USE_PLAYLIST3
#ifdef MMC
                    	bMediaChanged = TRUE;
#endif
#endif
                    	break;
#ifdef USE_PLAYLIST3
                    case PARSER_NEXT_SONG:
                    case PARSER_PREV_SONG:
                        bSendToPlayerStateMachinePlaylist3 = TRUE;
                        break;
#endif
                    case MENU_DECODER_STATE_CHNG:
#ifdef MMC
                        // Decoder will be already stopped if read MMC file is failed and the 0x800000 will be set by decoder.
                        // 0x800000 is the #FileReadError bit for x:DecoderSR, this bit is set after fileread error.
                    	if((g_wDecoderSR & 0x800000) == 0x800000)
                        {   g_iPlayerState = DECODER_STOPPED;
                            DecoderForceInit();
                            SysPostMessage(2,DECODER_RESET);
#ifdef USE_PLAYLIST5
                            SysPostMessage(2, PL5_PL_MSG_RESUME);
#endif
                            SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
                        }else
                        {
#endif
                    	   //open a new smv file
                        	if( (g_wDecoderCSR & DECODER_FILE_IS_OPEN) && (stc_iHandle == -1))
	       					{	//a new smv file is being playback
#ifdef USE_PLAYLIST5
                                SysPostMessage(2, PL5_PL_MSG_PAUSE);
#endif
                        		SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,OpenSMVFile,0,0,0);
                         		SyncSMVFrame();

#ifdef USE_PLAYLIST3
                    		    //If use black background, don't clear display by following line.
                                //DisplayHint.I = (1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
#else
                                DisplayHint.I = (1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
#endif
                            }
                        	else if( !(g_wDecoderCSR & DECODER_FILE_IS_OPEN) && (stc_iHandle != -1 ) )
	       					{	//the current smv file is finished
#ifdef USE_PLAYLIST5
	                            SysPostMessage(2, PL5_PL_MSG_RESUME);
#endif
                        		SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
                            }
#ifdef MMC
                        }

#endif

                        if(g_iPlayerState == DECODER_STOPPED)
                        {	DisplayHint.I = MVIDEO_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                        }
                        UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
            			UpdateBacklightOffTimer();
#endif
                        bSendToPlayerStateMachine = FALSE;

                        break;
                    default:
                        break;
                }
                break;
			case EVENT_BUTTON:
    			// button pressed, so restart timer if enabled
    			UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
    			UpdateBacklightOffTimer();
#endif

    			//remember the last button event (mostly for coming out of FFWD and RWND)
    			switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
    			{
                    case PH_VOL_UP:
                    case PH_VOL_DOWN:
                        // Handle volume change locally to avoid switch codebank
                        iVolumeHoldCount++;
                        if (iVolumeHoldCount>10)
                        	iVolumeHoldCount = 10;
                        g_iUserVolume += (gEventInfo.Button.wButtonEvent == PH_VOL_UP? iVolumeHoldCount : - iVolumeHoldCount);
                        SysUpdateVolume();
                        bSendToPlayerStateMachine = FALSE;
                        break;

                    case PR_VOL_UP:
                    case PR_VOL_DOWN:
                        iVolumeHoldCount = 0;
                        bSendToPlayerStateMachine = FALSE;
                        break;

                    case PH_RW:
                    case PH_FF:
                        if(g_iPlayerState == DECODER_STOPPED)
                            bSendToPlayerStateMachine = FALSE;
                        else
						{   // Update display during fast forward/rewind
							if (g_iSeeking != SEEK_NONE)
							{
								iSkipHoldCount++;
								if (iSkipHoldCount == 3)
								{
									iSkipHoldCount = 0;
										SyncSMVFrame();
                  						DisplaySMVOneFrame();
								}
                            }
                        }
                        break;

                    case PR_RW:
                    case PR_FF:
                        if (g_iSeeking != SEEK_NONE)
						{
                            iSkipHoldCount = 0;
                            SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
                        }
#ifdef USE_PLAYLIST5                        
                        SysPostMessage(2, PL5_PL_MSG_RESUME);
#endif						
#ifdef USE_PLAYLIST3
                        bSendToPlayerStateMachinePlaylist3 = TRUE;
#endif
                        break;

                    case PR_PLAY:
                        if (g_wDecoderSR & DECODER_PAUSED)
						{
							//resume from pause mode, synchronize the audio and video
                            DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
                            SysWaitOnEvent(0,0,0);
                            SyncSMVFrame();
                            bSendToPlayerStateMachine = FALSE;
                        }
#ifdef USE_PLAYLIST3
                        bSendToPlayerStateMachinePlaylist3 = TRUE;
#endif
                        break;

                    case PR_STOP:
                        DisplayHint.I |= MVIDEO_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                        break;

                    case PR_MENU:
                        iNewMenu = MENU_MAIN;
						bPrMenu = TRUE;
                    	bDone = TRUE;
#ifdef USE_PLAYLIST5						
						g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = FALSE;
#endif
                        break;
#ifdef USE_PLAYLIST5
				 case PH_MENU:
                    // Allow user to escape to music mode
                    bDone= TRUE;
                    // Set flag as a command for PR_MENU which always follows a PH_MENU sequence.
                    g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = TRUE;
                    break;
#endif                
        			case PH_STOP:
        				//if the STOP button is held down, lets call the shutdown menu
        				SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
        				// if returns update display  -- if doesn't return, powered down
        				if (g_wDecoderSR & DECODER_PLAYING)
						{
                            SyncSMVFrame();
        				}
        				else
        				    DisplayHint.I |= (MVIDEO_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
                        bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
        				break;

                    case PR_AB:
                        bSendToPlayerStateMachine = FALSE;
                        break;

        			default:
        				break;
    			}
			    break;

			default:
			    break;
		}
#ifdef USE_PLAYLIST3
        if(bSendToPlayerStateMachinePlaylist3)
        {   HandlePlayerStateMachinePlaylist3(iEvent,0,pPtr,(int*)&SongInfo,(int*)&DisplayHint,(int*)&bSendToPlayerStateMachine);
        }
#endif
#ifdef USE_PLAYLIST5
		if((bPrMenu) && (g_wDecoderSR & DECODER_PLAYING))
		{
			// If press menu and smv playing,stop the smv first before exit to main menu.
			OriginEvent = pMsg->Button.wButtonEvent;
			pMsg->Button.wButtonEvent = PR_STOP;
            SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
			pMsg->Button.wButtonEvent = OriginEvent;
		}
        else 
#endif
        if(bSendToPlayerStateMachine)
        {//only send events to the player state machine if necessary (to avoid code bank swapping)
            SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
        }
	}

    SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
	SysSpeedClockFree(SPEED_CLIENT_JPEG_DEC);
#ifdef USE_PLAYLIST3
    g_wCurrentSongNumber_return = g_wCurrentSongNumber;
#ifdef MMC
    if(bMediaChanged == FALSE)
    {
#endif
        g_wCurrentDirDevId          = g_wCurrentDirDevId_backup;
        g_wTotalSongCount           = g_wTotalSongCount_backup;
        g_wCurrentSongNumber        = g_wCurrentSongNumber_backup;
        g_current_size              = g_current_size_backup;
        g_current_index             = g_current_index_backup;
#ifdef MMC
    }
#endif
	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,ML_browsing_app_init,0,0,0);
	if( g_wDecoderSR & DECODER_STOPPED)
    {   g_iPlayerState = DECODER_STOPPED;
	    SysSetDecoderResource(RSRC_DECMOD_CODE); // Force to use other audio decoder if smv stopped.
		DecoderForceInit();
		SysPostMessage(2, DECODER_RESET);
	}
#endif
	//don't exit player state machine, cause we want to keep playing when in main menu.
	//sdk2.1
	return iNewMenu;
}

RETCODE _reentrant SMV_FillBuffer(void)
{
	if(stc_iByteLeave == 0)
	{
	    SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space = 0;
	    return JPEG_SUCCESS;
	}

	//In a single JPEG block, there is a maximum size defined in graphic header,  (g_iByteinJPEGBlock - 3)
	//The first three bytes are used to store the actual block size
	//Also, there is a actual block size defined in the beginning of each JPEG block
	//The actual JPEG data should be filled into the input buffer without any dummy
	//Otherwise, JPEG decoder will have an error

	SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space =
		Fread(stc_iHandle, (INT *)SmvCurrentJpeg.InBufDesc.pBuffPointer,
			MIN(stc_iByteLeave, JPEG_INPUT_BUFFER_SIZE*3), Y_MEMORY, -1);

    stc_iByteLeave -= SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space;

	return SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space <= 0 ? JPEG_ERROR : JPEG_SUCCESS;
}

RETCODE _reentrant SMV_InitDecoder(void) //This will update the dimensions of input image in JpegApp structure
{
	int iRet;

	//reset the JPEG decoder parameters
	SmvCurrentJpeg.DecParams.zoom_no = 1;
	SmvCurrentJpeg.DecParams.output_grayscale_flag = FALSE;

	//SmvCurrentJpeg.InBufDesc.pBuffPointer = (unsigned int*)&g_wUserScratchY;
	SmvCurrentJpeg.InBufDesc.pBuffPointer = SmvInputBuffer;
	SmvCurrentJpeg.OutBufDesc.pBuffPointer = SmvOutputBuffer;
	SmvCurrentJpeg.OutBufDesc.output_buffer_size = JPEG_OUTPUT_BUFFER_SIZE;

	//call JPEG decoder to reset
	smvj_reset_Decoder(&SmvCurrentJpeg.DecParams);

	//obtain the actual size of each JPEG block. These 3 bytes are part of JPEG block. Thus,
	if( Fread( stc_iHandle, (INT*)&stc_iByteLeave, 3, X_MEMORY, -1 ) < 0 )
	{
		SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space = -1;
		return JPEG_ERROR;
	}

	//Dummy byte will be append to the actual JPEG data in order to have constant block size defined in g_iByteinJPEGBlock
	stc_iByteDummy = stc_iByteinJpegBlock - stc_iByteLeave - 3;
	stc_iCurrentFrame = 0;

	//Fill up  the buffer
	if( SMV_FillBuffer() != JPEG_SUCCESS )
		return JPEG_ERROR;

	do
	{
		//call JPEG decoder to init the JPEG data
		iRet = (*SmvJpegInit)(&SmvCurrentJpeg.DecParams, &SmvCurrentJpeg.InBufDesc);
		if(iRet == JPEG_INIT_MOREDATA_REQUIRED)
		{
			//more data is required to initialize the JPEG data
			if( SMV_FillBuffer() != JPEG_SUCCESS )
				return JPEG_ERROR;
		}
		else if(iRet == JPEG_INIT_COMPLETE)
		{
			//initialization is completed.
			SmvCurrentJpeg.DecParams.x_pos_zoom = SmvCurrentJpeg.DecParams.image_width/2;
			SmvCurrentJpeg.DecParams.y_pos_zoom = SmvCurrentJpeg.DecParams.image_height/2;
			return JPEG_SUCCESS;
		}
		else	//any other error, return error.
			return JPEG_ERROR;

	} while (iRet != JPEG_INIT_COMPLETE);

	return JPEG_SUCCESS;
}

RETCODE _reentrant SMV_DecodeDisplay(BOOL bDisplay)
{
	int iRet;
	int iPage = 0;
	int iTemp1,iTemp2;

  //Stmp13739 Fix code start.
	if((SmvCurrentJpeg.DecParams.image_width == 0) || (SmvCurrentJpeg.DecParams.image_height == 0))
	{
		// If image_width or image_height equal to 0,should return error immediately.
		return JPEG_ERROR;
	}
  //Stmp13739 Fix code end

	if( SmvCurrentJpeg.InBufDesc.Input_buffer_filled_space < 0 )
		return JPEG_ERROR;

	//In one call, we display the whole frame, iPage will be updated once the output reach to most-right side
	while( iPage < stc_iFrameHeight )
	{
		//call JPEG decoder to decode the JPEG data in input buffer
		iRet = (*SmvJpegDecode)(&SmvCurrentJpeg.DecParams,
							&SmvCurrentJpeg.InBufDesc,
								&SmvCurrentJpeg.OutBufDesc);
		if (iRet == JPEG_RUNTIME_COMPLETE) //end of file
		{
			// the whole JPEG data is completed.
			if( bDisplay )
				SMV_DisplayBitmap(&SmvCurrentJpeg);
			//remove dummy byte appending to JPEG data
			stc_iByteLeave = stc_iByteDummy;
			while( stc_iByteLeave > 0 )
			{	if(SMV_FillBuffer() != JPEG_SUCCESS)
				    return JPEG_ERROR;
			}
			//Initialize the next JPEG block
			if( SMV_InitDecoder() != JPEG_SUCCESS )
				return JPEG_ERROR;
			break;

		}
		else if(iRet == JPEG_RUNTIME_MOREDATA_REQUIRED) //more data requested
		{
			//more data is required
			if( SMV_FillBuffer() != JPEG_SUCCESS )
				return JPEG_ERROR;
		}
		else if(iRet == JPEG_RUNTIME_MCU_BLOCK_DONE) //current decode call successful
		{
			//The output buffer is full, display on the screen
			if( bDisplay )
			{   SmvCurrentJpeg.OutBufDesc.pBuffPointer[1] %= stc_iFrameHeight;
			    iTemp1 = SmvCurrentJpeg.OutBufDesc.pBuffPointer[1] + SmvCurrentJpeg.OutBufDesc.pBuffPointer[4];
			    if(iTemp1 <= stc_iVideoHeight)
			    {   SMV_DisplayBitmap(&SmvCurrentJpeg);
			    }else
			    {   iTemp2 = iTemp1 - stc_iVideoHeight;
			        SmvCurrentJpeg.OutBufDesc.pBuffPointer[4] -= iTemp2;
			        SMV_DisplayBitmap(&SmvCurrentJpeg);
			        SmvCurrentJpeg.OutBufDesc.pBuffPointer[4] += iTemp2;
			    }

			}
			//update iPage when the output reaches the most-right side
			if( (SmvCurrentJpeg.OutBufDesc.pBuffPointer[0] + SmvCurrentJpeg.OutBufDesc.pBuffPointer[3])
			         >= SmvCurrentJpeg.DecParams.image_width )  //== SmvCurrentJpeg.DecParams.image_width )
				iPage += SmvCurrentJpeg.OutBufDesc.pBuffPointer[4];
		}
		else 				//something went wrong
			return JPEG_ERROR;
	}
	stc_iCurrentFrame++;

	return JPEG_SUCCESS;
}

void _reentrant SyncSMVFrame(void)
{
    DWORD dw_time, dw_SkipJpegBlock;
    LONG dwCurrentTime;

	//read the current RTC
   	stc_dwCurrentTime = ReadRTC() * 1000;
    if( stc_iHandle != -1)
    {
		//calculate the current time from audio decoder
        dw_time = ((DWORD)g_wSongByteTotalHigh << 24) + (DWORD)g_wSongByteTotalLow;
        dw_time *= (DWORD)1000000;
        dw_time /= (DWORD)g_SmvWave_dwAvgBytesPerSec;      // Elapsed time in us

		//find the nearest JPEG block to the current point of audio decoder
		//the next JPEG block is preferred to the previous block
		//since frame drop within one JPEG block is time consuming

		//If more preference for the next block, increase the value added to dw_time in the following equation
        dw_SkipJpegBlock = (dw_time + (DWORD)((stc_iDurationInJpegBlock * 6) / 10)) / (DWORD)stc_iDurationInJpegBlock;
		if( (dw_SkipJpegBlock * (DWORD)stc_iDurationInJpegBlock) >= dw_time )
		{
			//the next block is selected. Adjust the LastTime to the starting point of next JPEG block
			stc_dwLastTime = stc_dwCurrentTime + ((dw_SkipJpegBlock * (DWORD)stc_iDurationInJpegBlock) - dw_time);
		}
		else
		{
			//the previous block is selected. Adjust the LastTime to the starting point of previous JPEG block.
			//Then, we use frame drop machanism to fine tune the synchronization
			stc_dwLastTime = stc_dwCurrentTime - (dw_time - (dw_SkipJpegBlock * (DWORD)stc_iDurationInJpegBlock));
		}
		//update the number of frame leave for rendering
        stc_dwTotalFrame = stc_dwTotalFrameBackup - dw_SkipJpegBlock * (DWORD)stc_iFrameInJpegBlock;
		//this is the file offset for the selected JPEG block
        dw_SkipJpegBlock = (dw_SkipJpegBlock * stc_iByteinJpegBlock) + stc_dwVideoStart;
        Fseek( stc_iHandle, dw_SkipJpegBlock, SEEK_SET);

		//initialize JPEG decoder the the selected JPEG block
		SmvCurrentJpeg.iHandle = stc_iHandle;
		SmvCurrentJpeg.DisplayParams.x = 0;
		SmvCurrentJpeg.DisplayParams.y = 0;
		SmvCurrentJpeg.DecParams.output_image_width = stc_iVideoWidth;
		SmvCurrentJpeg.DecParams.output_image_height = stc_iFrameHeight*stc_iFrameInJpegBlock;
		SMV_InitDecoder();

    }
    else
	{
		//??
        stc_dwLastTime = stc_dwCurrentTime - stc_iFrameDuration;
    }

	stc_iNumFrameStop = 0;

}

int _reentrant DisplaySMVOneFrame(void)
{
	INT iRet;

	//decode the first frame in the JPEG block
    if (stc_iHandle != -1)
		SMV_DecodeDisplay(TRUE);
    return (1);
}

int _reentrant DisplaySMV(void)
{
    INT     a,b,d;
    DWORD   dwTimeDiff;

	//read the current RTC
	stc_dwCurrentTime = ReadRTC() * 1000;

	//if the current time is less than last time, no action
	//it is valid since last time will be used  to synchronize audio playback
	if( stc_dwCurrentTime < stc_dwLastTime )
		return 1;

    dwTimeDiff = stc_dwCurrentTime - stc_dwLastTime;
	//decode a frame if the time different is greater than frame duration
    if( dwTimeDiff >= stc_iFrameDuration )
    {
    	//perform frame drop machanism if the time different is greater than frame drop threshold
    	if( dwTimeDiff >= stc_iFrameDrop )
        {
    		//calculate how many frame to drop
    		dwTimeDiff = dwTimeDiff / stc_iFrameDuration;

            stc_dwTotalFrame -= dwTimeDiff;
            stc_dwLastTime += dwTimeDiff * stc_iFrameDuration;

            stc_iNumFrameStop += dwTimeDiff;    // for house keeping
            for( ;dwTimeDiff >0; dwTimeDiff-- )
            {   if( SMV_DecodeDisplay(FALSE) != JPEG_SUCCESS )
                {
        			SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
        			return 1;
                }
            }
        }

    	//update the last time
        stc_dwLastTime += stc_iFrameDuration;
        stc_dwTotalFrame--;
    	if( SMV_DecodeDisplay(TRUE) != JPEG_SUCCESS )
        {
    		SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
        }
    }
	return 1;
}

void _reentrant SMV_DisplayBitmap(JpegControl * pJpeg){
	int iSourceWidth,iSourceHeight,iSourceColorInfo;
	unsigned int x, y;

	x = pJpeg->DisplayParams.x + pJpeg->OutBufDesc.pBuffPointer[0];
	y = pJpeg->DisplayParams.y + pJpeg->OutBufDesc.pBuffPointer[1];
	iSourceColorInfo = pJpeg->OutBufDesc.pBuffPointer[2];
	iSourceWidth = pJpeg->OutBufDesc.pBuffPointer[3];
	iSourceHeight = pJpeg->OutBufDesc.pBuffPointer[4];

	HALDisplaySendData(x,y, iSourceWidth, iSourceHeight, &pJpeg->OutBufDesc.pBuffPointer[5], DISPLAY_COLOR);
}

#ifdef USE_PLAYLIST3
int _reentrant HandlePlayerStateMachinePlaylist3(int iEvent, int iIgnored, int *pMessagePtr, int *ptr1, int *ptr2, int *ptr3)
{
    union EventTypes    *pMsg = (union EventTypes * )pMessagePtr;
	SONGFILEINFO        *pSongInfo = (SONGFILEINFO*)ptr1;
    union DisplayHints  *pDisplayHint = (union DisplayHints*)ptr2;
    BOOL                *pbSendToPlayerStateMachine=(BOOL*)ptr3;
    unsigned int        iTempBackup,iTempResult;

    iIgnored;//this removes the parameter not used warning.

    switch (iEvent)
    {
    case EVENT_BUTTON:
        // button pressed, so restart timer if enabled
        UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
        UpdateBacklightOffTimer();
#endif

        switch(pMsg->Button.wButtonEvent)
        {
        case PR_RW:
        case PR_FF:
        {
            SONGTIME time;
            time = DecGetCurrentTime();
            if((g_wTotalSongCount > 0)&&(g_iSeeking != SEEK_RWND))
            {   if((g_wDecoderSR & DECODER_STOPPED)||(g_iSeeking == SEEK_NONE))
                {   iTempBackup = g_wDecoderSR;
                    if(!(iTempBackup & DECODER_STOPPED))
                    {   SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
    					SysWaitOnEvent(0,0,100);
                    }
                    if(g_wLastButton == PR_FF)
                    {   iTempResult = JpegFile_GetNextSongFileInfo(0,0,(INT*)pSongInfo);
                    }else
                    {   
                        if  ((time.wMinutes < 1) && (time.wSeconds <= PREV_SONG_THRESHOLD))
                        {
                            iTempResult = JpegFile_GetPreviousSongFileInfo(0,0,(INT*)pSongInfo);
                        }
                    }
                    if(!(iTempBackup & DECODER_STOPPED))
                    {   if(iTempResult == PLAYLIST_SUCCESS)
                        {   if(StartSMVDecoder(1) != PLAYERLIB_SUCCESS)
                            {   pDisplayHint->I |= MVIDEO_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                            }
                        }
                    }else
                    {   pDisplayHint->bits.SongTitle = TRUE;
                        pDisplayHint->bits.CurrentTrack = TRUE;
                    }
                    *pbSendToPlayerStateMachine = FALSE;
                }
            }
        }    break;
        case PR_PLAY:
            if(g_wDecoderSR & DECODER_STOPPED)
            {   if(JpegFile_GetCurrentSongFileInfo(0,0,(INT*)pSongInfo)==PLAYLIST_SUCCESS)
                {   if(StartSMVDecoder(1) != PLAYERLIB_SUCCESS)
                    {   pDisplayHint->I |= MVIDEO_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
                    }
                }
                *pbSendToPlayerStateMachine = FALSE;
            }
            break;
        }
        break;
    case EVENT_MESSAGE:
        switch(pMsg->Message.wMsgCommand)
        {
        case PARSER_NEXT_SONG:
            if(JpegFile_GetNextSongFileInfo(0,0,(INT*)pSongInfo) == PLAYLIST_SUCCESS)
            {   if(!(gEventInfo.Message.wMsgParms[0] & PREVSONG_RWND))
                {   SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
        		    SysWaitOnEvent(0,0,100);
        		}
                if((g_iSeeking == SEEK_NONE)&&(g_wCurrentSongNumber == 0))
                {   pDisplayHint->I |= MVIDEO_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
        		}else
        		{   if(StartSMVDecoder(0) == PLAYERLIB_SUCCESS)
        		    {   if(gEventInfo.Message.wMsgParms[0] & PREVSONG_RWND)
                    {   DecSetSongPosEnd();
                        SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, PlayerLib_Rewind,0,0,0);
                    }else
                    {   SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, PlayerLib_SetState, DECODER_STATE_PLAY, TRUE, NULL);
                    }
                }
            }
            }
            UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
			UpdateBacklightOffTimer();
#endif
            *pbSendToPlayerStateMachine = FALSE;
            break;
        case PARSER_PREV_SONG:
            if(JpegFile_GetPreviousSongFileInfo(0,0,(INT*)pSongInfo) == PLAYLIST_SUCCESS)
            {   if(!(gEventInfo.Message.wMsgParms[0] & PREVSONG_RWND))
                {   SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,PlayerLib_SetState,DECODER_STATE_STOP,TRUE,0);
				    SysWaitOnEvent(0,0,100);
				}
				if(StartSMVDecoder(0) == PLAYERLIB_SUCCESS)
				{   if(gEventInfo.Message.wMsgParms[0] & PREVSONG_RWND)
                {   DecSetSongPosEnd();
                    SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, PlayerLib_Rewind,0,0,0);
                }else
                {   SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, PlayerLib_SetState, DECODER_STATE_PLAY, TRUE, NULL);
                }
            }
            }
            UpdateAutoShutdownTimer();
#ifdef BACKLIGHT
			UpdateBacklightOffTimer();
#endif
            *pbSendToPlayerStateMachine = FALSE;
            break;
        }
        break;
    }
    return 0;
}

RETCODE _reentrant StartSMVDecoder(int mode)
{
    RETCODE rtn=PLAYERLIB_ERROR;

    if(mode == 0)
    {
        SysCallFunction(RSRC_MOTION_VIDEO_MENU_INITSTATE_CODE_BANK,CloseSMVFile,0,0,0);
    }
    if(SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetMetaData,0,0,0) == PLAYERLIB_SUCCESS)
    {
        DecoderForceInit();
        SysPostMessage(2,DECODER_RESET);
        SMVBlackBackground();
        if(mode == 0)
            DecSetSongPosZero();
        else
            SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, PlayerLib_SetState, DECODER_STATE_PLAY, TRUE, NULL); // mode == 1

        rtn = PLAYERLIB_SUCCESS;
    }
    return(rtn);
}

//
//
// notes:  Review: Is 8 hard code a problem here since it appears to only be for some LCDs?
//
void _reentrant SMVBlackBackground(void)
{
    int i,iTempWidth,iTempHeight;

    iTempWidth=stc_iVideoWidth; iTempHeight=stc_iVideoHeight;
    stc_iVideoWidth=LCD_SIZE_X; stc_iVideoHeight=LCD_SIZE_Y;
    _memset( SmvInputBuffer, 0, LCD_SIZE_X * 8 );
    for(i=0;i<LCD_SIZE_Y;i+=8)
	    HALDisplaySendData(0,i,LCD_SIZE_X,8,SmvInputBuffer,DISPLAY_COLOR);
    stc_iVideoWidth=iTempWidth; stc_iVideoHeight=iTempHeight;
}
#endif

#ifdef BACKLIGHT
void _reentrant UpdateBacklightOffTimer (void)
{
	if (g_iBackLightState == BACKLIGHT_ON)
    {
        SysPostMessage(2,LCD_BACKLIGHT_ON);
        if(g_wDecoderSR & (DECODER_STOPPED | DECODER_PAUSED))
        {
            SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
        }
        else
        {   // Enc and Dec are not 'both STOPPED', so kill the auto shutdown timer
            SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BACKLIGHT);
        }
        SysWaitOnEvent(0,0,0);
    }
}
#endif
