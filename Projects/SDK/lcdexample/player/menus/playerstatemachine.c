////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
// Reviews	: DanhNguyen (06-2008)
// File        	: playerstatemachine.c (LCD)
// Description : Code bank that handles common system functions based on button
//               presses or messages. Display functions are not handled here.
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "project.h"
#include "exec.h"
#include "menumanager.h"
#include "messages.h"
#include "playerstatemachine.h"
#include "menus.h"
#include "mainmenu.h"
#ifdef WOW
#include "wowmenu.h"
#endif
#include "resource.h"
#include "buttons.h"
#include "syscoder.h"
#include "sysmem.h"
#include "playerlib.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#include "playlist1internal.h"
#else
#ifdef   USE_PLAYLIST2
#include "playlist2.h"
#include "playlist2internal.h"
#else
#ifdef   USE_PLAYLIST3
#include "playlist3.h"
#include "playlist3internal.h"
#include "browsemenu.h"
#else
#ifdef   USE_PLAYLIST5
#include "playlist5.h"
#include "sysspeed.h"
#endif
#endif
#endif
#endif
#include "font.h"
#include "stringlib.h"
#include "metadata.h"
#include "battery.h"
#include "sysvolume.h"
#include "filesystem.h"
#include "display.h"
#include "Fsproj.h"
#ifdef CLCD
#include "displaymodule_color.h"
#else
#include "displaymodule.h"
#endif
#include "ddildl.h"
#include "batterycharge.h"
#include "playmodemenu.h"
//#include "sysequ.h"
#define DECODER_TYPE_WMA 1
#include "RestoreDriveFromBackup.h"
#ifdef USE_PLAYLIST3
#include "musiclib_ghdr.h"

#pragma asm
    nolist
    include "sysresources.inc"
    include "sysequ.inc"
    list
#pragma endasm
#endif	// #ifdef USE_PLAYLIST3
//#include "chkdsk.h"      TT- TODO - Investigate why this isn't resolving.
#ifdef WOW
#include "wow_ed.h"
#endif
#ifdef AUDIBLE
#include "audiblesongposition.h"
#include <limits.h>
#endif
#ifdef SYNC_LYRICS
#include "synclyrics.h"

extern void _reentrant LyricsSetTimer(WORD wPeriod);
#endif

_X INT g_iSeeking=SEEK_NONE;
extern WORD g_wSecondsToSkip;
extern WORD g_wAdpcmSecondstoSkip;

extern JanusDRMLicExpired;
static int holdcount=0;
#ifdef USE_PLAYLIST5
extern bDRMLicExpired;
BOOL g_bWait;
BOOL g_bShuffle = FALSE;
extern g_LastCurrentMenu;
#endif

static _X INT stc_iPressHoldCount=0;
static _X long stc_lTimeABWasLastPressed=0;
static _X WORD g_bRewoundToBeginning= FALSE;

_asmfunc DecoderForceInit(void);
extern INT  FSInit(INT _X *bufx, INT _Y *bufy, INT maxdevices, INT maxhandles, INT maxcaches);
extern _reentrant INT Computefreeluster(INT DeviceNumber);

extern _asmfunc void UsbConnected(void);

void _reentrant HandleMediaChange(void);
_reentrant int PSM_GetABMode(void);
_reentrant void PSM_ResetScrollInfo(void);
extern _reentrant void  FSClearDriveBuf(INT DriveNumber,INT maxhandles, INT maxcaches);

extern void TestSkipCheckDisk(WORD wDiskNum);
#ifdef USE_PLAYLIST5
extern INT  FSDriveInit(INT DeviceNumber);
#endif

extern g_iMediaErrorFlag;
extern g_FSinitErrorCode;

extern  filest_bHasJanusDRM;

#ifdef USE_PLAYLIST5
BOOL g_bLastDisplay = FALSE;
BOOL g_bLastDisplaySongCnt = FALSE;
#endif

#ifdef USE_PLAYLIST3
extern int24 g_iBrowseTotalNum;
int g_iVolume_Control_Mode=FALSE;
int g_iAB_Control_Mode=FALSE;
BOOL g_Loading_displayed = FALSE;
uint24 g_is_SD_inserted = NO_SD;
BOOL bSystemInit = FALSE;
BOOL bResume = TRUE;

void _reentrant ML_browsing_app_init(void)
{
#pragma asm
	include "resource.inc"
	org     p,".ptextplayerstatemachine":
	extern	F_lc_u_e_MusicLibBrowse_P
	extern	F_lc_u_b_MusicLibBrowse_P
	extern	SysLoadResource

	; Load the MusicLib browsing engine into P memory
	move    #>RSRC_MUSICLIB_BROWSE_APP,x0                      ; Resource number
	move    #>RSRC_TYPE_DATA,x1                        ; Resource type is data
	move    #>(F_lc_u_e_MusicLibBrowse_P-F_lc_u_b_MusicLibBrowse_P)*3,y1 ; overlay area sz
	move    #>F_lc_u_b_MusicLibBrowse_P,y0                      ; Address of the overlay area
	move    #TARGET_MEM_P,a1                           ; Overlay area is in P memory
	jsr     SysLoadResource
#pragma endasm
}
#else
#ifdef USE_PLAYLIST5
INT g_is_SD_inserted = NO_SD;
#endif
#endif	// #ifdef USE_PLAYLIST3

#ifdef AUDIBLE
extern INT DecoderResourcePtr;
extern UCS3 g_wSongAlbum[ALBUM_SIZE];
extern unsigned int _X  g_bAudibleCurrentChapterNum, g_bAudibleTotalChapterCount;
char * _itoa( char *p, unsigned int num, unsigned char radix );
static _X WORD g_bAllowAudibleSave=FALSE;
_X AudibleFlags_t g_AudibleFlags = { 0, 0, 0 };
#endif
extern WORD g_wNumMedia;
extern int CleanDataStoreFlag;
#ifdef WOW
extern void DisablePluginModule(int *);
extern void EnablePluginModule(int *);
#endif

#ifdef JPEG_ALBUM_ART
extern BOOL g_AlbumArtInProgress;
int _reentrant StopAlbumArtProcess(int a, int b, int *pPtr);
extern _X BOOL	g_bApicFrameFound;	// declared in "attachedpictureframe.c"
#ifdef USE_PLAYLIST5
extern int g_ReturnValueForPlayerstatemachine;
#endif
RETCODE _reentrant ApicPostMessage(INT a, INT b, INT *c);	// defined in "attachedpictureframe.c"
#endif
#ifdef USE_PLAYLIST5
int LicExpireCount=0;
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant CheckDecoderInit(void )
//
//   Type:          Function
//
//   Description:   Makes sure the decoder is loaded
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         Moved into a function to save code space
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant CheckDecoderInit(void )
{
	if(g_wDecoderSR & DECODER_STOPPED)
	{
		DecoderForceInit();
		SysPostMessage(2,DECODER_RESET);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant InitPlayerStateMachine(int a, int b, int *c)
//
//   Type:          Function (code bank callable)
//
//   Description:   Does everything required to init the player state machine
//
//   Inputs:        none
//
//   Outputs:       always returns 0
//
//   Notes:         Available to call from outside this codebank for menus that
//                  need to initialize the player
//<
////////////////////////////////////////////////////////////////////////////////
INT _reentrant InitPlayerStateMachine(INT a, INT b, INT *c)
{
	a;b;c;
#ifndef USE_PLAYLIST5	// g_wTotalSongCount equal to 0 and g_wCurrentSongNumber equal to 1 when init.
	if(g_wCurrentSongNumber>g_wTotalSongCount)
		g_wCurrentSongNumber=g_wTotalSongCount;
#endif

	if( (g_wDecoderSR & g_wDecoderCSR & DECODER_STOPPED) && (g_iPlaylistRepeat==PLAYLIST_REPEAT_OFF) && (!PSM_GetABMode()) )

	{
		SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetBookmarkSongInfo,g_iPlaySet,0,0);
		DecoderForceInit();
		SysPostMessage(2,DECODER_RESET);
		PSM_ResetScrollInfo();
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant ExitPlayerStateMachine(int a, int b, int *c)
//
//   Type:          Function (code bank callable)
//
//   Description:   Does everything required to stop the player state machine
//
//   Inputs:        none
//
//   Outputs:       always returns 0
//
//   Notes:         Available to call from outside this codebank for menus that
//                  need to shut down the player state machine
//<
////////////////////////////////////////////////////////////////////////////////
INT _reentrant ExitPlayerStateMachine(INT a, INT b, INT *c)
{
	a;b;c;

	PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
#ifdef USE_PLAYLIST3
	SysWaitOnEvent(0,0,0);
#endif
	g_iPlayerState = DECODER_STOPPED;
	return 0;
}


#define QUICK_SWITCH_NEXT       1
#define QUICK_SWITCH_PREVIOUS   -1


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          void _reentrant QuickSwitchSong(INT iDirection)
//
//   Type:          Function
//
//   Description:   Increments/decrements track number and sets up a timer to
//                  tell the player state machine to change tracks.
//
//   Inputs:        INT iDirection:   QUICK_SWITCH_NEXT or QUICK_SWITCH_PREVIOUS
//
//   Outputs:       none
//
//   Notes:         If this is repeatedly called (quicker than
//                  QUICK_SWITCH_TIMEOUT_TIME), then the user can rapidly switch
//                  tracks.  once the timeout occurs, the player state  machine
//                  will change songs, update the display, and begin playing (if
//                  already playing).
//
//         QUICK_SWITCH_TIMEOUT_TIME is configurable/overrideable in project.inc
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant QuickSwitchSong(INT iDirection)
{
#ifndef USE_PLAYLIST3
	void _P *pFunction;
	SONGFILEINFO SongInfo;
	union DisplayHints hints;
	BYTE INTERSONG_DELAY_MSG[] = " Loading...";

	///////////////////////////////////////////////
	// MLP: 8-15-05
	// Fix for defect 8153.
	//
	// We only want to display the INTERSONG_DELAY_MSG when
	// we are in the music mode.  If we have another menu
	// on the LCD screen our message will over write it.
	// In other words, We want to be able to control the
	// play back when the user is surfing through the
	// menus.
	if (g_iCurrentMenu == MENU_MUSIC)
	{
		////////////////////////////////////////////////////////////////////////////
		// MLP 8-5-05
		// For some DRM/Janus content the intersong delay is greater than 3 seconds.
		// As per the SDK3.10 requirements, since we are greater than 3 seconds, we
		// need to put up a message indicating we are skipping to the next/previous
		// song.  To display the message we are "hijacking" the g_wSongAlbum,
		// g_wSongArtist and g_wSongTitle and using those buffers to display our
		// message since they will be changing anyway.  The Album and Artist strings
		// are cleared.  The message string is placed in g_wSongTitle.  The message
		// is a 3 byte per character string created above called INTERSONG_DELAY_MSG.
		// Once the intersong  delay meets the specified requirements we can remove
		// this code.

		// Set the Artist and Album to NULL so they are left blank.
		g_wSongArtist[0] = 0;
		g_wSongAlbum[0] = 0;

		// Copy our message into the song title buffer.  This message does not
		// scroll so make sure it's small enough to fit on the LCD screen.
		strcpy(g_wSongTitle, (UCS3 _Y *)INTERSONG_DELAY_MSG);

		// Make sure scrolling is disabled.  If not our message will get
		// clobbered.
		g_bSongStringScroll = FALSE;

		// Zero out all bit fields in the hints union
		hints.I = 0;

		// Set the bits for ArtistAlbum and Title so only those parts of
		// the LCD are updated.
		hints.bits.SongArtistAlbum = 1;
		hints.bits.SongTitle = 1;

		// This variable is used when the song title is scrollong across the screen.
		// If the song title we scrolling then this value is probably not going to
		// be zero which would cause our message to be displayed at some random
		// horizontal location.
		g_iSongStringOffset=0;

		// Refresh the LCD screen.
		// Arguments:
		//     hints.I is our union indicating we only want to change Album, Arist and Title
		//     1 means we want to update the fiels in hints.I. A 0 would just clear them.
		//     0 is ignored.
		SysCallFunction(RSRC_DISPLAY_CODE_BANK, RefreshDisplay, hints.I, 1, 0);

		// MLP
		// 8-0-05
		// End
		///////////////////////////////////////////////////////////////////////

		// MLP
		// 8-15-05
		// End.
	}
	SongInfo.m_wBufferLength = MAX_FILENAME_LENGTH;
	SongInfo.m_pFilename = g_CurrentSongName;
#ifdef USE_PLAYLIST5
    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
    PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
#endif
	if(iDirection == QUICK_SWITCH_NEXT)
		pFunction = Playlist_GetNextSongFileInfo;
	else
		pFunction = Playlist_GetPreviousSongFileInfo;

	// only switch to next track if found.
	if (SysCallFunction(RSRC_PLAYLIST_CODEBANK,pFunction,NEXTSONG_PLAY_BUTTON,0,(void*)&SongInfo)!=PLAYLIST_TRACK_NOT_FOUND)
	{
		g_wCurrentDirDevId = SongInfo.m_wDeviceID;
#ifndef USE_PLAYLIST5
		g_wCurrentSongNumber = SongInfo.m_wTrack;
#endif
		SysWaitOnEvent(0,0,0);
		SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_SONG_CHANGE,1,QUICK_SWITCH_TIMEOUT_TIME,MENU_SONG_CHANGE_TIMEOUT);
	}
#ifdef USE_PLAYLIST5
    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
	SysWaitOnEvent(0,0,0);
	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
	SysWaitOnEvent(0,0,0);
	UpdateAutoShutdownTimer();
#endif
#else // #ifndef USE_PLAYLIST3
	union DisplayHints hints;
	BYTE INTERSONG_DELAY_MSG[] = " Loading...";

	// Moved from the message handler of MENU_SONG_CHANGE_TIMEOUT
	PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);

	// have to get a full path here
	if ((g_iShuffleSetting) && (bPlayVoice == FALSE))
	{
		if(iDirection == QUICK_SWITCH_NEXT)
			SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_NEXT_SONG,&g_current_index);
		else
			SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_PREVIOUS_SONG,&g_current_index);
	}
	else
	{
		if(iDirection == QUICK_SWITCH_NEXT)
			SysPostMessage(3,MUSICLIB_PLAYNEXTSONG,&g_current_index);
		else
			SysPostMessage(3,MUSICLIB_PLAYPREVIOUSSONG,&g_current_index);
	}
	// only switch to next track if found.
	//g_wCurrentDirDevId = 0;
	//packed_strcpy(g_CurrentSongName, DirPath);   // copy path and filename
	SysWaitOnEvent(0,0,0);
	SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));

	///////////////////////////////////////////////
	// MLP: 8-15-05
	// Fix for defect 8153.
	//
	// We only want to display the INTERSONG_DELAY_MSG when
	// we are in the music mode.  If we have another menu
	// on the LCD screen our message will over write it.
	// In other words, We want to be able to control the
	// play back when the user is surfing through the
	// menus.
	if (g_iCurrentMenu == MENU_MUSIC)
	{
		////////////////////////////////////////////////////////////////////////////
		// MLP 8-5-05
		// For some DRM/Janus content the intersong delay is greater than 3 seconds.
		// As per the SDK3.10 requirements, since we are greater than 3 seconds, we
		// need to put up a message indicating we are skipping to the next/previous
		// song.  To display the message we are "hijacking" the g_wSongAlbum,
		// g_wSongArtist and g_wSongTitle and using those buffers to display our
		// message since they will be changing anyway.  The Album and Artist strings
		// are cleared.  The message string is placed in g_wSongTitle.  The message
		// is a 3 byte per character string created above called INTERSONG_DELAY_MSG.
		// Once the intersong  delay meets the specified requirements we can remove
		// this code.

		// Set the Artist and Album to NULL so they are left blank.
		g_wSongArtist[0] = 0;
		g_wSongAlbum[0] = 0;

		// Copy our message into the song title buffer.  This message does not
		// scroll so make sure it's small enough to fit on the LCD screen.
		strcpy(g_wSongTitle, (UCS3 _Y *)INTERSONG_DELAY_MSG);

		// Make sure scrolling is disabled.  If not our message will get
		// clobbered.
		g_bSongStringScroll = FALSE;

		// Zero out all bit fields in the hints union
		hints.I = 0;

		// Set the bits for ArtistAlbum and Title so only those parts of
		// the LCD are updated.
		hints.bits.SongArtistAlbum = 1;
		hints.bits.SongTitle = 1;

		// This variable is used when the song title is scrollong across the screen.
		// If the song title we scrolling then this value is probably not going to
		// be zero which would cause our message to be displayed at some random
		// horizontal location.
		g_iSongStringOffset=0;

		// Refresh the LCD screen.
		// Arguments:
		//     hints.I is our union indicating we only want to change Album, Arist and Title
		//     1 means we want to update the fiels in hints.I. A 0 would just clear them.
		//     0 is ignored.
		SysCallFunction(RSRC_DISPLAY_CODE_BANK, RefreshDisplay, hints.I, 1, 0);
		g_Loading_displayed = TRUE;
		// MLP
		// 8-0-05
		// End
		///////////////////////////////////////////////////////////////////////

		// MLP
		// 8-15-05
		// End.
	}

	//update the alignment of the ARTIST, TITLE, ALBUM
	PSM_ResetScrollInfo();
	//need to prepare a valid path before posting message - MENU_SONG_CHANGE_TIMEOUT
	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_SONG_CHANGE,1,QUICK_SWITCH_TIMEOUT_TIME,MENU_SONG_CHANGE_TIMEOUT);
#endif // #ifndef USE_PLAYLIST3
}

#ifdef	AUDIBLE
/*
static BOOL IsAudibleFile(void)
{
if ( (DecoderResourcePtr == RSRC_AUDIBLE_ACELPDEC_CODE) || (DecoderResourcePtr == RSRC_AUDIBLE_DECMOD_CODE) )
return TRUE;

return FALSE;
}
*/

void _reentrant AudibleStopAndSave(INT fStop)
{
	if ( fStop )
		PlayerLib_SetState(DECODER_STATE_STOP, TRUE, NULL);

	// Make sure that we don't continue until the decoder is good and done with the audio file.
	while ( !(g_wDecoderCSR & DECODER_STOPPED) || (g_wDecoderCSR & DECODER_FILE_IS_OPEN) ) {
		SysWaitOnEvent(0,0,0);
	}
	// Save file position if currently playing an Audible file
	if( g_bAllowAudibleSave ) {
		SysCallFunction(RSRC_AUDIBLE_SONG_POSITION_P, AudibleSetFilePosition, (INT)(&g_CurrentSongName), (BYTE)g_wCurrentDirDevId, 0);
		g_bAllowAudibleSave = FALSE;
	}
}
#endif	// AUDIBLE

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant HandlePlayerStateMachine(int iEvent,
//                                                          int iIgnored,
//                                                          int *pMessagePtr)
//
//   Type:          Function (code bank callable)
//
//   Description:   Does everything required to stop the player state machine
//
//   Inputs:        int iEvent     Either EVENT_MESSAGE,EVENT_BUTTON,EVENT_TIMER
//                  struct CMessage Contains a structure (see exec.h) that has
//                                  event information returned by SysWaitOnEvent
//
//   Outputs:       returns the iEvent passed in
//
//   Notes:         Put into its own code bank to remove the player state machine
//                  handling from menus that need it (like eq, contrast, etc)
//<
////////////////////////////////////////////////////////////////////////////////
int _reentrant HandlePlayerStateMachine(int iEvent, int iIgnored, int *pMessagePtr)
{
	INT iCurrentSong;
	INT i;
	int MMCDriveNumber;
	UCS3 * pChapterInfo;
    RETCODE rtn;

	union EventTypes *pMsg = (union EventTypes * )pMessagePtr;

	union DisplayHints DisplayHint;

	iIgnored;//this removes the parameter not used warning.

	DisplayHint.I = 0;

	MMCDriveNumber = g_wNumMedia-1;

	switch (iEvent)
	{
	case EVENT_BUTTON:
		// button pressed, so restart timer if enabled
		UpdateAutoShutdownTimer();

		if(pMsg->Button.wButtonEvent != PR_RW)
			g_bRewoundToBeginning= FALSE;

		if((g_wDecoderSR & DECODER_STOPPED) && (g_iPlaylistRepeat==PLAYLIST_REPEAT_OFF) && (!PSM_GetABMode()) )
			SysWaitOnEvent(0,0,0);


		switch(pMsg->Button.wButtonEvent)
		{
		case PR_RV:
			if (g_iAB_Control_Mode==FALSE)
			{
			g_iVolume_Control_Mode=!g_iVolume_Control_Mode;
			DisplayHint.bits.Volume = TRUE;
			}
			else
			{
				DisplayHint.bits.ABIcon = TRUE;
 				//Decoder might be stopped and moving back to 'A' so wait.
				if(g_wDecoderSR & DECODER_STOPPED)
					SysWaitOnEvent(0,0,0);
				
				//Do not send AB if in a stop state.  This can cause problems since a 
				//DecoderForceInit() or DECODER_RESET might be called and put the player
				//in an unknown state (STMP3114, STMP3261, STMP3232).
				//AND do not send if A has been send within 1 second.  A tight loop can cause Menu MSG overflow.
				if((!(g_wDecoderSR & DECODER_STOPPED))	 && (SysGetCurrentTime() > (stc_lTimeABWasLastPressed+900)))
				{			
	        	    	switch (PSM_GetABMode())
    	        		{
        	        	case DECODER_A_SET:
					// this will set B
	            	        	SysPostMessage(2,DECODER_AB_MODE);
            	    			break;

	                	case DECODER_B_SET:
    	                		// this will clear AB mode
        	                	SysPostMessage(2,DECODER_AB_MODE);
	        	        	break;

    	         	   	default:
	                	    	// no AB mode started -- set A and start timer
                        		SysPostMessage(2,DECODER_AB_MODE);
					stc_lTimeABWasLastPressed = SysGetCurrentTime();                       
        	        	break;
	            		}	//end AB switch
    	     			}//end if stop
			}
			break;
			
		case PR_EQ:
			DisplayHint.bits.EQ = TRUE;
		#ifndef WOW
			SysCallFunction(RSRC_EQ_MENU_CODE_BANK,NextEQ,INCREMENT,0,0);
		#else

			// toggle WOW processing
			if (g_bWOWEnable)
			{   // To enable GEQ and disable WOW:
				SysPostMessage(3,GEQ_SET_EQ,ENABLE);
				// Set Q=0.99 and center frequencies at: 62,250,1k,4k,16k)
				SysPostMessage(12,GEQ_SET_PARAM,0.99,0.99,0.99,0.99,0.99,62,250,1000,4000,16000);
				SysCallFunction(RSRC_WOW_CTRL,SetProcessEnable,(int)&sWowChannel,(int)FALSE,0);
				g_bWOWEnable = FALSE;
			}
			else
			{   // To enable SRS WOW and disable GEQ:
				SysPostMessage(3,GEQ_SET_EQ,DISABLE);  // send disable GEQ message
				EnablePluginModule(&WowModuleBase);    // enable and Init WOW
				g_bWOWEnable = TRUE;
				SysCallFunction(RSRC_WOW_CTRL,SetProcessEnable,(int)&sWowChannel,(int)TRUE,0);
				//      SysCallFunction(RSRC_WOW_MENU_CODE_BANK,WowInit,0,0,0);
			}
			SysWaitOnEvent(0,0,0);      //let GEW process its queues, to prevent overflow

		#endif
			break;

		case PR_MODE:
			SysCallFunction(RSRC_PLAYMODE_MENU_CODE_BANK,NextPlayMode,INCREMENT,0,0);
			DisplayHint.bits.PlayMode = TRUE;
			break;
		case PH_HOLD:
		case PR_HOLD:
			DisplayHint.bits.LockIcon = TRUE;
			break;
		case PR_PLAY:
			if(g_wDecoderSR&DECODER_STOPPED)
			{

				RETCODE rtn;
#ifdef USE_PLAYLIST3
				rtn = SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong_ML,0,0,0);
                if (rtn != PLAYERLIB_SUCCESS)
                {
                    rtn = PlayerLib_SkipToNextSong(TRUE, NEXTSONG_PLAY_EOF,0);
                }
				if (rtn == PLAYERLIB_SUCCESS)
#else
				rtn = SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
#ifdef USE_PLAYLIST2
				while (((rtn != META_DATA_FOUND)&(rtn != PLAYERLIB_END_OF_LIST)) && !(g_iPlayModeSetting & PM_REPEAT1) )
				{
					rtn = PlayerLib_SkipToNextSong(TRUE, NEXTSONG_PLAY_EOF,0);
				}
#endif
				if (rtn == META_DATA_FOUND)
#endif
				{
#ifdef JPEG_ALBUM_ART
					if (g_bApicFrameFound)
					{
						// Post a message to update display art
						SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicPostMessage, 0, 0, NULL);
					}
					else
#endif
					{
#ifdef USE_PLAYLIST5
						if((g_wTotalSongCount == 0))	// g_PL5_PL_queue.head must be not equal to  g_PL5_PL_queue.tail here
						    g_wTotalSongCount = 1;
#endif									
						PlayerLib_SetState (DECODER_STATE_PLAY,TRUE,NULL);
					}
#ifdef USE_PLAYLIST3
					SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath)); //STMP00014061,14169
#endif
					PSM_ResetScrollInfo();
#ifdef	AUDIBLE
					g_bAllowAudibleSave = TRUE;
					g_AudibleFlags.bits.ChapterNavMode = FALSE;
#endif
					DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
				}
			}
			else
			{
				PlayerLib_SetState (DECODER_STATE_TOGGLE,TRUE,NULL);
			}
			g_iSeeking=SEEK_NONE;
			break;

		case PR_FF:
			if (g_iVolume_Control_Mode==FALSE)
			{
			holdcount=0;
			g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP;
			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_FFRWND);  // Kill the timer

			g_wSecondsToSkip=SECONDS_TO_SKIP;
			DisplayHint.bits.PlayState = TRUE;
			if(g_iSeeking)
			{//if we were fast forwarding, lets tell the player to play
				if(g_iPlayerState == DECODER_PLAYING)
				{
					PlayerLib_SetState (DECODER_STATE_PLAY, TRUE, NULL);
				}
			}
			else
			{
#ifdef AUDIBLE
				if ( g_AudibleFlags.bits.ChapterNavMode ) {
					PlayerLib_SetState (DECODER_STATE_PAUSE,TRUE,NULL);
					SysPostMessage(2,DECODER_NEXT_CHAPTER);
					PlayerLib_SetState(DECODER_STATE_PLAY, TRUE, NULL);
					// Update chapter number
				}
				else
#endif
				{
					// We want to skip to next song
#ifdef	AUDIBLE
					if ( g_AudibleFlags.bits.IsAudibleFile )
						AudibleStopAndSave(TRUE);
#endif
					QuickSwitchSong(QUICK_SWITCH_NEXT);
					// Update the track number
					DisplayHint.bits.CurrentTrack = TRUE;
				}
			}
			g_iSeeking=SEEK_NONE;
			}
			else
			//In Volume Control Mode
			{
				SysIncrementVolume();
				stc_iPressHoldCount=0;
				DisplayHint.bits.Volume = TRUE;
			}
			break;

		case PH_FF:
			if (g_iVolume_Control_Mode==FALSE)
			{
			DisplayHint.bits.PlayState = TRUE;
			if(!g_iSeeking)
			{//if we're not seeking, lets enter the seek state
				g_iSeeking = SEEK_FFWD;
			}

			if (PlayerLib_FastForward(FALSE, 0,0) == PLAYERLIB_SUCCESS) //now lets seek
			{
				// This timer was added to speed up the scanning speed, it sets up two call backs to the menu module which in turn will send
				// FF/RWND decoder messages.  This will cause the seconds display to be updated twice as often (depending on the equate values).
				//   To go back to the original configuration:
				//                1) comment out the Syspostmessage function below so the timers won't be set.
				//                2) change the SECONDS_TO_SKIP values, all three, to 3 seconds.
				SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_FFRWND,2,200,MENU_MSG_SEND_FF);  // Sets up the Decoder FF messages between the PH_FFs.
			} // TOVERIFY    sdk2.6 used 100 here rather than sdk3.0 & 3.05's 200 value. Which is best?



			holdcount++;
#ifdef AUDIBLE
			//Audible Check
			if( g_AudibleFlags.bits.IsAudibleFile ) {
				if(holdcount<LEVEL1_BOUNDARY) {
					// Level 1 (b/w 1 to 5 sec)  --> 3X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_3X_RATE;
				} else if((holdcount>LEVEL1_BOUNDARY) && (holdcount<LEVEL2_BOUNDARY)) {
					// Level 2 (b/w 5 to 10 sec) --> 60X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_60X_RATE;
				} else if((holdcount>LEVEL2_BOUNDARY) && (holdcount<LEVEL3_BOUNDARY)) {
					// Level 3 (b/w 10 to 15 sec)--> 600X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_600X_RATE;
				} else if(holdcount>LEVEL3_BOUNDARY) {
					// Level 4 (above 15 sec)    --> 1800X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_1800X_RATE;
				}
			}
			else
#endif
			{
				if(holdcount < SECOND_TIME_BOUNDARY && holdcount > FIRST_TIME_BOUNDARY ) {
					g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP1;
					g_wSecondsToSkip = SECONDS_TO_SKIP1;
				}
				if(holdcount > SECOND_TIME_BOUNDARY) {
					g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP2;
					g_wSecondsToSkip=SECONDS_TO_SKIP2;
				}
			}
#ifdef SYNC_LYRICS
			if ((holdcount & 0x3) == 0)
				LyricsSetTimer(1);
#endif
			}
			else
			//In Volume Control Mode
			{
				DisplayHint.bits.Volume = TRUE;
				stc_iPressHoldCount++;
				if(stc_iPressHoldCount > 10)
					stc_iPressHoldCount = 10;
				for(i=0;i<stc_iPressHoldCount;i++)
				{
					SysIncrementVolume();
					// mixer queue is only 32 words
					// menu will post 6 words when SysXXXVolume is called
					// GEQ module loads after menus but before mixer module
					//     and posts 6 words
					// so assuming the system module or menu doesn't post any mixer
					//   messages the max mixer words that can be posted here before
					//   need to process that queue is 26 -- so let the modules run
					//   after every 4 calls to the SysXXXVolume function

					// this may cause problems with AB because more decoder messages
					// can be posted.
					if ((i==4) || (i==8))
					{
						SysWaitOnEvent(0,0,0);
					}
				}
			}
			break;
			
		case PR_RW:
			if (g_iVolume_Control_Mode==FALSE)
			{
			DisplayHint.bits.PlayState = TRUE;
			holdcount=0;
			g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP;
			g_wSecondsToSkip=SECONDS_TO_SKIP;
			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_FFRWND);  // Kill the timer
			if(g_iSeeking)
			{//if we were fast forwarding, lets tell the player to play
				if(g_iPlayerState == DECODER_PLAYING)
				{
					PlayerLib_SetState (DECODER_STATE_PLAY, TRUE, NULL);
				}
			}
			else
			{
#ifdef AUDIBLE
				if ( g_AudibleFlags.bits.ChapterNavMode ) {
					PlayerLib_SetState (DECODER_STATE_PAUSE,TRUE,NULL);
					SysPostMessage(2,DECODER_PREV_CHAPTER);
					PlayerLib_SetState(DECODER_STATE_PLAY, TRUE, NULL);
					// Update Chapter number
				}
				else
#endif
				{
					//We want to skip to previous song, or go back to the beginning of the song
					//if we're more than PREV_SONG_THRESHOLD (project.h)
					SONGTIME time;
					time = DecGetCurrentTime();
#ifdef	AUDIBLE
					if( g_bRewoundToBeginning ||
					    ((time.wMinutes <= 0) && (time.wSeconds <= PREV_SONG_THRESHOLD)) ||
					    g_AudibleFlags.bits.IsAudibleFile )
#else
                if( g_bRewoundToBeginning ||
                    ((time.wMinutes <= 0) && (time.wSeconds <= PREV_SONG_THRESHOLD)))
#endif
					{
#ifdef	AUDIBLE
						if ( g_AudibleFlags.bits.IsAudibleFile )
							AudibleStopAndSave(TRUE);
#endif
						QuickSwitchSong(QUICK_SWITCH_PREVIOUS);
						// Update the track number
						DisplayHint.bits.CurrentTrack = TRUE;
					}
					else
					{
						g_bRewoundToBeginning = TRUE;
						SysWaitOnEvent(0,0,1);
						SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_SONG_CHANGE,1,QUICK_SWITCH_TIMEOUT_TIME,MENU_SONG_CHANGE_TIMEOUT);
					}
				}
			}
			g_iSeeking=SEEK_NONE;
			}
			else
			//In Volume Control Mode
			{
				SysDecrementVolume();
				stc_iPressHoldCount=0;
				DisplayHint.bits.Volume = TRUE;
			}
			break;

		case PH_RW:
			if (g_iVolume_Control_Mode==FALSE)
			{
			DisplayHint.bits.PlayState = TRUE;
			if(!g_iSeeking)
			{//if we're not seeking, lets enter the seek state
				g_iSeeking = SEEK_RWND;
			}

			if (PlayerLib_Rewind(FALSE, 0,0) == PLAYERLIB_SUCCESS)  //now lets seek
			{
				// This timer was added to speed up the scanning speed, it sets up two call backs to the menu module which in turn will send
				// FF/RWND decoder messages.  This will cause the seconds display to be updated twice as often (depending on the equate values).
				//   To go back to the original configuration:
				//                1) comment out the Syspostmessage function below so the timers won't be set.
				//                2) change the SECONDS_TO_SKIP values, all three, to 3 seconds.
				SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_FFRWND,2,100,MENU_MSG_SEND_RWND);
			}

			holdcount++;

#ifdef AUDIBLE
			if ( g_AudibleFlags.bits.IsAudibleFile ) {
				if(holdcount<LEVEL1_BOUNDARY) {
					// Level 1 (b/w 1 to 5 sec)  --> 3X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_3X_RATE;
				} else if((holdcount>LEVEL1_BOUNDARY) && (holdcount<LEVEL2_BOUNDARY)) {
					// Level 2 (b/w 5 to 10 sec) --> 60X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_60X_RATE;
				} else if((holdcount>LEVEL2_BOUNDARY) && (holdcount<LEVEL3_BOUNDARY)) {
					// Level 3 (b/w 10 to 15 sec)--> 600X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_600X_RATE;
				} else if(holdcount>LEVEL3_BOUNDARY) {
					// Level 4 (above 15 sec)    --> 1800X rate
					g_wSecondsToSkip = SECONDS_TO_SKIP_FOR_1800X_RATE;
				}
			}
			else
#endif
			{
				if(holdcount < SECOND_TIME_BOUNDARY && holdcount > FIRST_TIME_BOUNDARY ) {
					g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP1;
					g_wSecondsToSkip=SECONDS_TO_SKIP1;
				}
				if(holdcount > SECOND_TIME_BOUNDARY) {
					g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP2;
					g_wSecondsToSkip=SECONDS_TO_SKIP2;
				}
			}
#ifdef SYNC_LYRICS
			if ((holdcount & 0x3) == 0)
				LyricsSetTimer(1);
#endif
			}
			else
			//In Volume Control Mode
			{
				DisplayHint.bits.Volume = TRUE;
				//this PressHoldCount stuff is for volume control accelleration
				stc_iPressHoldCount++;
				if(stc_iPressHoldCount > 10)
					stc_iPressHoldCount = 10;
				for(i=0;i<stc_iPressHoldCount;i++)
				{
					SysDecrementVolume();
					// see description above
					if ((i==4) || (i==8))
					{
						SysWaitOnEvent(0,0,0);
					}

				}
			}
			break;

		case PR_STOP:
#ifdef USE_PLAYLIST5		
#ifdef SYNC_LYRICS
    #if 1
			DisplayHint.I |=MUSIC_MENU_DISPLAY_ALL | (1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
    #else

				if(g_iPlaySet == PLAYSET_MUSIC)
				{
					SysPostMessage(6,LCD_CLEAR_RANGE,SONG_TITLE_X_POS,SONG_TITLE_Y_POS-8,LCD_SIZE_X,32);
					SysWaitOnEvent(0,0,0);
				}
    #endif
#else
			DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
#endif
#else
			DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
#endif  //#ifdef USE_PLAYLIST5
#ifdef USE_PLAYLIST3
			if((g_wDecoderSR & DECODER_STOPPED) == 0)
#else
			if(g_iPlayerState != DECODER_STOPPED)
#endif
			{
				ExitPlayerStateMachine(0,0,0);
				//PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
				//g_iPlayerState = DECODER_STOPPED;
			}
#ifndef USE_PLAYLIST3
			else
			{//Playlist_SetPlaySet will reset the current list to the beginning of the list.
				//It will also reshuffle if shuffle is on.
				SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_SetPlaySet,Playlist_GetPlaySet(),TRUE,0);
				SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
			}
#endif
			g_iSeeking=SEEK_NONE;
#ifdef	AUDIBLE
			g_AudibleFlags.bits.ChapterNavMode = FALSE;
#endif
			break;

		case PH_PLAY:
		case PH_STOP:
#ifdef JANUS
			CleanDataStoreFlag = 1;
#endif
			//if the STOP button is held down, lets call the shutdown menu
			SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
			// when returns update display  -- if doesn't return, powered down
			DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;
			break;

		case PR_VOL_UP:
			SysIncrementVolume();
			stc_iPressHoldCount=0;
			DisplayHint.bits.Volume = TRUE;
			break;

		case PR_VOL_DOWN:
			SysDecrementVolume();
			stc_iPressHoldCount=0;
			DisplayHint.bits.Volume = TRUE;
			break;

			// SDK2.105
			// the for loop in each of the PH_VOL_UP/PH_VOL_DOWN is used to speed up the
			// volume adjustment so it will move 10 ticks each time a PH event occurs
			//
			// with the addition of the multi-stage volume, mixer messages are sent with
			// each call to the volume function; the for loop will cause a mixer queue overflow
			// SDK2.104 called SysWaitOnEvent() to handle this, but the problem with this solution
			// is that the GEQ module also calls the volume function and a menu is not necessarily loaded
			// Also, by letting the other modules run each time, the A-B mode can crash because the decoder
			// will post messages to the menu, but the menu is not processing new messages yet.
			//
			// The fix below adds the SysWaitOnEvent() call where needed to avoid overflowing the mixer
			// queue.  A cleaner solution could be to remove the for loop but the volume adjust will be slower.


		case PH_VOL_UP:
			DisplayHint.bits.Volume = TRUE;
			stc_iPressHoldCount++;
			if(stc_iPressHoldCount > 10)
				stc_iPressHoldCount = 10;
			for(i=0;i<stc_iPressHoldCount;i++)
			{
				SysIncrementVolume();
				// mixer queue is only 32 words
				// menu will post 6 words when SysXXXVolume is called
				// GEQ module loads after menus but before mixer module
				//     and posts 6 words
				// so assuming the system module or menu doesn't post any mixer
				//   messages the max mixer words that can be posted here before
				//   need to process that queue is 26 -- so let the modules run
				//   after every 4 calls to the SysXXXVolume function

				// this may cause problems with AB because more decoder messages
				// can be posted.
				if ((i==4) || (i==8))
				{
					SysWaitOnEvent(0,0,0);
				}
			}
			break;

		case PH_VOL_DOWN:
			DisplayHint.bits.Volume = TRUE;
			//this PressHoldCount stuff is for volume control accelleration
			stc_iPressHoldCount++;
			if(stc_iPressHoldCount > 10)
				stc_iPressHoldCount = 10;
			for(i=0;i<stc_iPressHoldCount;i++)
			{
				SysDecrementVolume();
				// see description above
				if ((i==4) || (i==8))
				{
					SysWaitOnEvent(0,0,0);
				}

			}
			break;

#ifdef PR_AB
		case PR_AB:
#ifdef	AUDIBLE
			// For Audible file, the A-B Mode button is interpreted as a chapter
			// navigation mode button. Only go to section navigation mode if
			// playing or paused.
			if ( g_AudibleFlags.bits.IsAudibleFile ) {
				if ( g_wDecoderSR & (DECODER_PLAYING | DECODER_PAUSED) ) {
					g_AudibleFlags.bits.ChapterNavMode ^= 1;
					DisplayHint.bits.ABIcon = TRUE;
				}
			}
			else
#endif
			{
				DisplayHint.bits.ABIcon = TRUE;
				//Decoder might be stopped and moving back to 'A' so wait.
				if(g_wDecoderSR & DECODER_STOPPED)
					SysWaitOnEvent(0,0,0);

				//Do not send AB if in a stop state.  This can cause problems since a
				//DecoderForceInit() or DECODER_RESET might be called and put the player
				//in an unknown state (STMP3114, STMP3261, STMP3232).
				//AND do not send if A has been send within 1 second.  A tight loop can cause Menu MSG overflow.
				if((!(g_wDecoderSR & DECODER_STOPPED))   && (SysGetCurrentTime() > (stc_lTimeABWasLastPressed+900)))
				{
					switch (PSM_GetABMode())
					{
					case DECODER_A_SET:
						// this will set B
						SysPostMessage(2,DECODER_AB_MODE);
						break;

					case DECODER_B_SET:
						// this will clear AB mode
						SysPostMessage(2,DECODER_AB_MODE);
						break;

					default:
						// no AB mode started -- set A and start timer
						SysPostMessage(2,DECODER_AB_MODE);
						stc_lTimeABWasLastPressed = SysGetCurrentTime();
						break;
					}		//end AB switch
				}//end if stop
			}
			break;
#endif
		}
		break;

	case EVENT_MESSAGE:
		switch(pMsg->Message.wMsgCommand)
		{
#ifdef USE_PLAYLIST5
        case MENU_MSG_PL5_PLAY_SONG:
#ifdef JPEG_ALBUM_ART
// !! IMPORTANT - Do not remove this from the first statement for this case
// ensure that before switching to next/prev song we were not in the middle of displaying album art for the current one
			if(g_AlbumArtInProgress)
				SysCallFunction(RSRC_MUSIC_MENU_CODE_BANK,StopAlbumArtProcess, 0, 0, 0);
#endif

		    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);
			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
		    SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
			SysWaitOnEvent(0,0,0);

            if (SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0) == META_DATA_FOUND)
			{
#ifdef JPEG_ALBUM_ART
				if (g_bApicFrameFound)
				{
					// Post a message to update display art
					SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicPostMessage, 0, 0, NULL);
				}
				else
#endif
				{
					BOOL bWait = TRUE;
#ifdef USE_PLAYLIST5
					if((g_wTotalSongCount == 0) && (g_PL5_PL_queue.head != g_PL5_PL_queue.tail))
					    g_wTotalSongCount = 1;
#endif					
#ifdef USE_PLAYLIST5
					if(g_LastCurrentMenu == MENU_VOICE)	
						bWait = FALSE;
#endif
					PlayerLib_SetState (DECODER_STATE_PLAY,bWait,NULL);
				}

				PSM_ResetScrollInfo();
#ifdef	AUDIBLE
				g_bAllowAudibleSave = TRUE;
				g_AudibleFlags.bits.ChapterNavMode = FALSE;
#endif
				DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
			}

		    SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
			SysWaitOnEvent(0,0,0);
			SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
			SysWaitOnEvent(0,0,10);
			// button pressed, so restart timer if enabled
			UpdateAutoShutdownTimer();

		    g_iSeeking=SEEK_NONE;
			iEvent = EVENT_BUTTON;
			pMsg->Button.wButtonEvent = PR_PLAY;
            break;
#endif  // #ifdef USE_PLAYLIST5

		case MENU_SONG_CHANGE_TIMEOUT:
#ifdef JPEG_ALBUM_ART
// !! IMPORTANT - Do not remove this from the first statement for this case
// ensure that before switching to next/prev song we were not in the middle of displaying album art for the current one
			if(g_AlbumArtInProgress)
				SysCallFunction(RSRC_MUSIC_MENU_CODE_BANK,StopAlbumArtProcess, 0, 0, 0);
#endif
			PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
#ifdef USE_PLAYLIST3
			SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong_ML,0,0,0);
              SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath)); //STMP00014061,14169
#else // #ifdef USE_PLAYLIST3
			SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
			DecoderForceInit();//needed to prevent WMA decoder from crashing.
			SysPostMessage(2,DECODER_RESET);
#endif // #ifdef USE_PLAYLIST3
			// Force the decoder to re-open the file, the next time the song is played.
#ifdef	AUDIBLE
			// mmiu - Commented out if statement to always force a Stop which closes the file.
			//                if(g_iPlayerState != DECODER_PLAYING)
#else
			if(g_iPlayerState != DECODER_PLAYING)
#endif
				SysPostMessage(2,DECODER_STOP);

#ifdef USE_PLAYLIST3
			if (g_Loading_displayed == FALSE)
#endif
			{
				PSM_ResetScrollInfo();
				DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK; //update everything
#ifdef USE_PLAYLIST5				
#ifdef SYNC_LYRICS
				if(g_iPlaySet == PLAYSET_MUSIC)
				SysPostMessage(6,LCD_CLEAR_RANGE,SONG_TITLE_X_POS,SONG_TITLE_Y_POS-8,LCD_SIZE_X,32);
#endif
#endif  //#ifdef USE_PLAYLIST5
			}

			if(g_iPlayerState == DECODER_PLAYING) // As in PR_FF during play.
			{   // Now includes decoder settling time. Stmp00003027
#ifdef JPEG_ALBUM_ART
				if (g_bApicFrameFound)
				{
					// Post a message to display album art
					SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicPostMessage, 0, 0, NULL);
				}
				else
#endif
				{
					PlayerLib_SetState (DECODER_STATE_PLAY,TRUE,NULL);
#ifdef	AUDIBLE
					g_bAllowAudibleSave = TRUE;
#endif
				}
			}
#ifdef USE_PLAYLIST3
			if (g_Loading_displayed == TRUE) {
				SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));
				PSM_ResetScrollInfo();
				DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
				g_Loading_displayed = FALSE;
			}
#endif
			g_bRewoundToBeginning = FALSE;
			break;

		case MENU_SONG_TIME_CHNG:
			DisplayHint.bits.TrackTime = TRUE;
			break;
#ifdef BACKLIGHT
		case MENU_MSG_TURN_OFF_BACKLIGHT:
			SysPostMessage(2,LCD_BACKLIGHT_OFF);
			SysWaitOnEvent(0,0,0);
			break;
#endif

		case MENU_MSG_SEND_RWND:
			DisplayHint.bits.PlayState = TRUE;

			if (PlayerLib_Rewind(FALSE, 0,0) == PLAYERLIB_SUCCESS)//now lets seek
			{
				if(!g_iSeeking)
					PlayerLib_SetState (DECODER_STATE_PLAY,TRUE,NULL);
			}
			break;

		case MENU_MSG_SEND_FF:
			DisplayHint.bits.PlayState = TRUE;

			if (PlayerLib_FastForward(FALSE, 0,0) == PLAYERLIB_SUCCESS) //now lets seek
			{
				if(!g_iSeeking)
					PlayerLib_SetState (DECODER_STATE_PLAY,TRUE,NULL);    //sometimes there's a timer still on after the PR_FF,
			}
			break;

#ifdef BATTERY_CHARGE
		case MENU_BATTERY_CHARGER_TIMEOUT:
			//check on battery charging.
			SysCallFunction(RSRC_BATTERY_CHARGE_CODEBANK,BatteryChargeStateMachine,0,0,0);
			break;
#endif  //BATTERY_CHARGE

		case MENU_DECODER_STATE_CHNG:
			// if decoder stopped, start timer. If started playing disable it
			UpdateAutoShutdownTimer();
			DisplayHint.bits.PlayState = TRUE;
			break;

		case MENU_AUTO_SHUTDOWN:
			SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,FORCESHUTDOWN,0,0);
			// will not return
			break;

		case MENU_MSG_MEDIA_CHANGED:
#ifdef USE_PLAYLIST3
			g_Rebuild = TRUE;
#endif
			PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
			//Allow the decoder to stop before continuing
			SysWaitOnEvent(0,0,0);
#ifdef AUDIBLE
			// Save file position if Audible file
			if ( g_AudibleFlags.bits.IsAudibleFile )
				AudibleStopAndSave(FALSE);
#endif
#ifdef MMC
#ifdef USE_PLAYLIST3
			bResume = TRUE;		// reset to initial value
			// MMC is media 1
			// if MediaInit or DriveInit not success for MMC
			if ( ( MediaInit(MMCDriveNumber) != SUCCESS )  || ( DriveInit(MMCDriveNumber) != SUCCESS ) )
			{   // SystemHalt(); _asm(" nop"); //testpoint
				if ((g_is_SD_inserted != NO_SD) && ((playMode&8) != 0 ) && (bSystemInit))
					//			if ((g_is_SD_inserted != NO_SD) && (bSystemInit))
					bResume = FALSE;
				g_is_SD_inserted = NO_SD;
				i = RSRC_SD_REMOVE;
			}
			else
			{
				/*
				if (g_is_SD_inserted != HAS_SD)
				bResume = FALSE;
				*/
				g_is_SD_inserted = HAS_SD;
				i = RSRC_SD_INSERT;
			}
			if (bSystemInit)
			{
				SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,i);
				SysWaitOnEvent(0,0,0);
			}
#else // #ifdef USE_PLAYLIST3
			if(MediaInit(MMCDriveNumber) != SUCCESS)
			{
				// stmp00006762
				// When the Player is warm started from MTP/MSC by disconnecting USB,
				// the MMC can be in a state that may not reset properly the first time.
				// Exciting the MMC and trying again clears this condition.
				// If this happens, wait a short time, and try again.

				// Wait 10ms, then try again
				SysWaitOnEvent(EVENT_TIMER,0,10);

				if(MediaInit(MMCDriveNumber) != SUCCESS)
				{
#ifdef USE_PLAYLIST5
					g_is_SD_inserted = NO_SD;
#endif
					goto MMCBadInit;
				}
			}
#ifdef USE_PLAYLIST5
            SysSpeedIncrease(SPEED_PEAK,SPEED_CLIENT_PLAYLIST1);
            g_is_SD_inserted = HAS_SD;
#endif
            // DriveInit requires SysLoadFATWrite() to be loaded
    		SysLoadFATWrite();
			if(DriveInit(MMCDriveNumber) != SUCCESS)
			{
				;
			}
			else // DriveInit MMC success so run checkdisk on the MMC.
			{
				// Run checkdisk each time media is inserted.
				TestSkipCheckDisk(MMCDriveNumber); // do here, not in main.asm since MMC not init til here.
			}
			SysUnLoadFATWrite();
//			if(DriveInit(MMCDriveNumber) != SUCCESS)
//			{
//				;
//			}
//			else // DriveInit MMC success so run checkdisk on the MMC.
//			{
//				// Run checkdisk each time media is inserted.
//				SysLoadFATWrite();
//				TestSkipCheckDisk(MMCDriveNumber); // do here, not in main.asm since MMC not init til here.
//				SysUnLoadFATWrite();
//			}

#endif // #ifdef USE_PLAYLIST3
MMCBadInit:

#endif


			// Always must call FSInit so FS file count is updated [comment stale]. FSInit does both media but we only needed to init MMC
			// Handle media error


            // DriveInit requires SysLoadFATWrite() to be loaded
			SysLoadFATWrite();
			if (FSDriveInit(MMCDriveNumber) == 0)
			{
				FSClearDriveBuf(MMCDriveNumber,maxhandles,maxcaches);
#if (NUM_REMOVABLE_MEDIA > 0) // There is at least one external media
				Computefreeluster(MMCDriveNumber);
#endif
			}
			SysUnLoadFATWrite();
#ifdef MMC
#ifdef USE_PLAYLIST5
			SysSpeedClockFree(SPEED_CLIENT_PLAYLIST1);
#endif
#endif
//			if (FSDriveInit(MMCDriveNumber) == 0)
//			{
//				FSClearDriveBuf(MMCDriveNumber,maxhandles,maxcaches);
//#if (NUM_REMOVABLE_MEDIA > 0) // There is at least one external media
//				SysLoadFATWrite();
//				Computefreeluster(MMCDriveNumber);
//				SysUnLoadFATWrite();
//#endif
//			}

			if(g_FSinitErrorCode)//check if there is some error related to media
			{
				// There is media error
				SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);

				//SystemHalt(); // FSInit returned error. Check err code
#if (NUM_REMOVABLE_MEDIA > 0) // There is at least one external media

				if (g_FSinitErrorCode & EXTERNAL_MEDIA_NOT_SIGMATEL_FORMAT)
					SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,RSRC_EXTERNAL_MEDIA_ERROR2);

				else if (g_FSinitErrorCode & ERROR_ON_EXTERNAL_MEDIA)
					SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,8,RSRC_EXTERNAL_MEDIA_ERROR1);

				else if (g_FSinitErrorCode &
					(ERROR_ON_INTERNAL_MEDIA_SHIFTED | INTERNAL_MEDIA_NOT_SIGMATEL_FORMAT_SHIFTED))//
					SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,8,RSRC_INTERNAL_MEDIA_ERROR1);

#else // NAND only
				if (g_FSinitErrorCode & (ERROR_ON_INTERNAL_MEDIA | INTERNAL_MEDIA_NOT_SIGMATEL_FORMAT)) //
					SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,8,RSRC_INTERNAL_MEDIA_ERROR1);

#endif

				SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE); // Kill animate timer to provent queue from filling up.
				SysWaitOnEvent(0,0,5000);  // Show for 5 second
				SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,FORCESHUTDOWN,0,0); //<<< SHUTS DOWN PLAYER
			}

			//don't put a break here. Update playlist with any supported new audio files
		case PARSER_DEVICE_ENUMERATE:
#ifdef USE_PLAYLIST3
			// Let the other messages queues be cleared first
			SysWaitOnEvent(0,0,0);
			// Suspending the auto power down timer and battery check timer to prevent
			//  sudden power down in long sessions of music library rebuild
			// Suspending the animation timer off to save system resource
			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_BATT_CHK);
			SysWaitOnEvent(0,0,0);
			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_AUTO_SHUTDOWN);
			SysWaitOnEvent(0,0,0);
			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
			SysWaitOnEvent(0,0,0);
#endif // #ifndef USE_PLAYLIST3

			g_iPlayerState = DECODER_STOPPED;
			g_iSeeking = FALSE;
#ifdef USE_PLAYLIST5			
			if(g_bPlaylistShuffle == TRUE)
			{
				g_bShuffle = TRUE;
				g_bPlaylistShuffle = FALSE;
			}
#endif			
			SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
#ifndef USE_PLAYLIST3
#ifdef USE_PLAYLIST5
			while((g_PL5_PL_queue.head == g_PL5_PL_queue.tail) && (!g_PL5_Playback.foundall))
			{
				SysWaitOnEvent(0,0,50);
			}
			if(g_bShuffle == TRUE)
			{
				g_bShuffle = FALSE;
				g_bPlaylistShuffle = TRUE;
			}
			DisplayHint.I |= 1<<DISPLAY_SONGTITLE_BITPOS;
			DisplayHint.I |= 1<<DISPLAY_SONGARTISTALBUM_BITPOS;
#endif
			SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
#endif // #ifndef USE_PLAYLIST3

#ifndef USE_PLAYLIST5
			if(!g_wTotalSongCount)
			{   g_wCurrentSongNumber = 0;
			//force reset of current song number else old value used
			}
#endif			
			PSM_ResetScrollInfo();
			DecoderForceInit();
			SysPostMessage(2,DECODER_RESET);
			DisplayHint.I |= 0xffffff;//update everything
#ifdef USE_PLAYLIST3
			// Let the decoder messages cleared first
			SysWaitOnEvent(0,0,0);

			ML_browsing_app_init();
#ifdef MMC
			if (g_is_SD_inserted)
				SysPostMessage(4,MUSICLIB_MERGE_SONG, (INT*) &g_current_size, (INT*)&g_current_index);
#endif
			if (bPlayVoice)
				bResume = FALSE;
			if (bResume)
			{
				SysPostMessage(5,MUSICLIB_PLAY_RESUME, (INT*)&g_current_size, (INT*)&g_current_index, g_is_SD_inserted);
				SysWaitOnEvent(0,0,0);
#ifdef _FOLDER_BROWSE_				
				g_current_size = 0;
				g_current_index = 0;
#endif				
				if (g_current_size > 0)
					SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));
				else
					bResume = FALSE;
			}
			if (!bResume)
			{
				IsPlayOnTheGo = FALSE;
				SysPostMessage(3,MUSICLIB_BROWSE_SETROOT, (INT*)&g_current_size);
				SysPostMessage(7,MUSICLIB_BROWSE_BROWSEMENU, (INT*)&g_current_size, ITEM_TRACK, 0, (INT*)(&g_iBrowseTotalNum), (INT*)&g_current_index);
				SysWaitOnEvent(0,0,0);
				g_current_index = 0;
//				if (g_current_size > 0)
					SysCallFunction(RSRC_METADATA_CODEBANK,GetRelativePath,0,ITEM_TRACK,(INT*)(DirPath));
				bPlayVoice = FALSE;
			}

			//have to get a full path here
			//SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong_ML,0,0,0);
			DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything

			if (g_current_size == 0)
			{
				g_wSongTitle[0] = 0;
				g_wSongArtist[0] = 0;
				g_wSongAlbum[0] = 0;
			}

			g_wTotalSongCount = g_current_size;
			SysWaitOnEvent(0,0,0);
			SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong_ML,0,0,0);
#ifdef USE_PLAYLIST3
            SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath)); //STMP00014061,14169
#endif
			PSM_ResetScrollInfo();
			bSystemInit = TRUE;
			//end of rebuild code
			if ((g_iShuffleSetting == SHUFFLE_OFF) || (bPlayVoice == TRUE))
				SysPostMessage(3, MUSICLIB_PLAY_SETSHUFFLE, PLAY_NORMAL);
			else
				SysPostMessage(3, MUSICLIB_PLAY_SETSHUFFLE, PLAY_SHUFFLE);

			// Music library rebuild completed, resume the suspended timers
			SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,ANIMATE_PERIOD,MENU_MSG_ANIMATE);
			SysWaitOnEvent(0,0,0);
			SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BATT_CHK,0,2000,MENU_BATTERY_CHNG);
			SysWaitOnEvent(0,0,0);
			UpdateAutoShutdownTimer();

#endif
			break;

		case MENU_BATTERY_CHNG:

#ifdef BATTERY_TYPE_LI_ION
#define JANUS_BATTERY_THRESHOLD 22
#else
#define JANUS_BATTERY_THRESHOLD 19
#endif

#ifdef JANUS_BUILD   //stmp8602 Battery level monitoring needed while playing back DRM tracks.
			// Voltage levels:
			// Alkaline:     17%== 0.99v ;  18% == 1.06V ; 19% == 1.08v
			// Lithium Ion   19% = 3.0v  ;  20% == 3.15V
			if ((WMA_TYPE == g_iSongType) &&        // WMA File
				(filest_bHasJanusDRM) &&            // AND Janus (MUST FOLLOW WMA FILE TEST)
				(SysBatteryGetLevel() <= JANUS_BATTERY_THRESHOLD) &&     // AND Remaining level below threshold
				(g_iPlayerState == DECODER_PLAYING))// AND Playing a song
			{
				ExitPlayerStateMachine(0,0,0);
				// display low battery message
				DisplayClear();
				DisplayLowBattery();
				SysWaitOnEvent(0,0,2000);  // let user see it for 2 seconds
				DisplayClear();
				DisplayHint.I |= 0xffffff;//update everything
			}
#endif
			DisplayHint.bits.Battery = TRUE;
			break;

		case PARSER_NEXT_SONG:
            if (IsSystemDriveDirty() != FALSE)
            {

                SysCallFunction(RSRC_RESTORESYSDRIVE_P_CODE, RefreshSystemDriveErrorBlock, 0, 0, NULL);
                //RefreshSystemDriveErrorBlock();

                // This call has been moved into RefreshSystemDriveErrorBlock();
                // Switch back to the cleaned drive
                //ToggleSystemDrive();
            }
	        holdcount=0;
            g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP;
			g_wSecondsToSkip=SECONDS_TO_SKIP;

#ifdef AUDIBLE
			if( g_AudibleFlags.bits.IsAudibleFile ) {
				g_wSecondsToSkip = 0;
			}
#endif

#ifndef USE_PLAYLIST5
			if(JanusDRMLicExpired)
			{
				SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_LIC_EXPIRED,1000,0);
				JanusDRMLicExpired=0;
			}
			SysWaitOnEvent(0,0,0);
			PlayerLib_SkipToNextSong(TRUE, pMsg->Message.wMsgParms[0],0);			
#else
			if((bDRMLicExpired))
			{
				SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,SplashScreen,RSRC_LIC_EXPIRED,1000,0);
			}


			if(bDRMLicExpired)
			{
                if (g_iPlaylistRepeat == PLAYLIST_REPEAT_ONE)
                    LicExpireCount = 1;
                else
                    LicExpireCount++;

				g_bWait = FALSE;
			}
			else
				LicExpireCount = 0;


#ifdef MOTION_VIDEO
			if(g_PL5_PL_search.status == PL5_STATE_PAUSE)
			{
                SysPostMessage(2, PL5_PL_MSG_RESUME);
				while((g_wTotalSongCount == 0) && (!g_PL5_Playback.foundall))
					SysWaitOnEvent(0,0,50);
			}
#endif

//            if (((g_iPlaylistRepeat == PLAYLIST_REPEAT_ONE) && (LicExpireCount < 1)) ||
//                ((g_iPlaylistRepeat == PLAYLIST_REPEAT_ALL) &&(LicExpireCount < (g_wTotalSongCount - g_wCurrentSongNumber))))
				PlayerLib_SkipToNextSong(TRUE, pMsg->Message.wMsgParms[0],0);


			if((g_wCurrentSongNumber == 1) && (g_iPlaylistRepeat != PLAYLIST_REPEAT_ALL) && (g_iPlaylistRepeat !=PLAYLIST_REPEAT_ONE) && (!g_bPlaylistShuffle))
			{

				if(g_iPlayerState == DECODER_PLAYING)
				{
		          // Set player state to stopped.
		          g_iPlayerState = DECODER_STOPPED;
		          PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
		          SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);

		          SysPostMessage(2,MENU_DECODER_STATE_CHNG);
				}
			}
#endif  //#ifndef USE_PLAYLIST5
#ifdef USE_PLAYLIST3
			SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));//STMP00014061,14169
#endif
			DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
			DisplayHint.I |= (1<<DISPLAY_VOLUME_BITPOS);
#ifdef USE_PLAYLIST5			
#ifdef SYNC_LYRICS
			if(g_iPlaySet == PLAYSET_MUSIC)
    			SysPostMessage(6,LCD_CLEAR_RANGE,SONG_TITLE_X_POS,SONG_TITLE_Y_POS-8,LCD_SIZE_X,32);
#endif
#endif
			PSM_ResetScrollInfo();
			break;

		case PARSER_PREV_SONG:
	            holdcount=0;
				g_wAdpcmSecondstoSkip=SECONDS_TO_SKIP;
				g_wSecondsToSkip=SECONDS_TO_SKIP;

#ifdef AUDIBLE
			if( g_AudibleFlags.bits.IsAudibleFile ) {
				g_wSecondsToSkip = 0;
			}
#endif
#ifdef USE_PLAYLIST5
//			if(g_iPlaylistRepeat == PLAYLIST_REPEAT_ONE)
//		          SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
//			else
			PlayerLib_SkipToPreviousSong(TRUE, pMsg->Message.wMsgParms[0],0);
#else
			PlayerLib_SkipToPreviousSong(TRUE, pMsg->Message.wMsgParms[0],0);
#endif
#ifdef USE_PLAYLIST3
            SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));//STMP00014061,14169
#endif
			DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
			PSM_ResetScrollInfo();
			break;

#ifdef MMC
		case MENU_MSG_MEDIA_OVER_CUR:
#ifdef USE_PLAYLIST5
			PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
			SysWaitOnEvent(0,0,0);
#endif			

			SysPostMessage(6,LCD_CLEAR_RANGE,0,0,LCD_SIZE_X,LCD_SIZE_Y);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE1_X,WARNING_MSG_LINE1_Y,RSRC_LOW_PWR_EXT_DEV_LINE1);
			SysPostMessage(5,LCD_PRINT_RANGE_RSRC,WARNING_MSG_LINE2_X,WARNING_MSG_LINE2_Y,RSRC_LOW_PWR_EXT_DEV_LINE2);

			SysWaitOnEvent(0,0,2000);  // Show for 2 second

			ExitPlayerStateMachine(0,0,0);
			//PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);

			// Parser device enumerate
			//g_iPlayerState = DECODER_STOPPED;
			g_iSeeking = FALSE;
			SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_Initialize,0,0,0);
#ifndef USE_PLAYLIST3
#ifdef USE_PLAYLIST5
			while((g_PL5_PL_queue.head == g_PL5_PL_queue.tail) && (!g_PL5_Playback.foundall))
			{
				SysWaitOnEvent(0,0,50);
			}
			DisplayHint.I |= 1<<DISPLAY_SONGTITLE_BITPOS;
			DisplayHint.I |= 1<<DISPLAY_SONGARTISTALBUM_BITPOS;
#endif
			SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
#endif // #ifndef USE_PLAYLIST3

#ifndef USE_PLAYLIST5
			if(!g_wTotalSongCount)
			{   g_wCurrentSongNumber = 0;
			//force reset of current song number else old value used
			}
#endif			
			PSM_ResetScrollInfo();
			DecoderForceInit();
			SysPostMessage(2,DECODER_RESET);
			DisplayHint.I |= 0xffffff;//update everything
			break;
#endif
#ifdef AUDIBLE
		case MENU_SET_SONG_POSITION:
			if( g_bAllowAudibleSave && g_AudibleFlags.bits.IsAudibleFile ) {
				SysCallFunction(RSRC_AUDIBLE_SONG_POSITION_P, AudibleSetFilePosition, (INT)(&g_CurrentSongName), (BYTE)g_wCurrentDirDevId, 0);
				g_bAllowAudibleSave = FALSE;
			}
			break;
		case MENU_UPDATE_CHAPTER_INFO:

			pChapterInfo = (UCS3 *)g_wSongAlbum;

			// Null terminate string
			*pChapterInfo = '\0';

			// pChapter --> Chapter:
			strcat(pChapterInfo,"Chapter:");
			pChapterInfo += strlen(pChapterInfo);

			// pChapterInfo --> Chapter:g_bAudibleCurrentChapterNum
			_itoa((char *)pChapterInfo, g_bAudibleCurrentChapterNum, 10);

			// pChapterInfo --> Chapter:g_bAudibleCurrentChapterNum/
			pChapterInfo += strlen(pChapterInfo);
			*pChapterInfo = '/';
			pChapterInfo++;

			// pChapterInfo --> Chapter:g_bAudibleCurrentChapterNum/g_bAudibleTotalChapterCount
			_itoa((char *)pChapterInfo, g_bAudibleTotalChapterCount, 10);

    	    DisplayHint.I |= MUSIC_MENU_DISPLAY_NEW_TRACK;//update everything
#ifdef JPEG_ALBUM_ART
            // Follow would cause the screen flashes
    	    DisplayHint.I &= ~(1<<DISPLAY_ALBUMART_BITPOS);
#endif
			break;
		case MENU_USB_CONNECTED:
			PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
			//Allow the decoder to stop before continuing
			SysWaitOnEvent(0,0,0);
			SysCallFunction(RSRC_AUDIBLE_SONG_POSITION_P, AudibleSetFilePosition, (INT)(&g_CurrentSongName), (BYTE)g_wCurrentDirDevId, 0);
			UsbConnected();
			break;
#endif
#ifdef JPEG_ALBUM_ART
		case MENU_MSG_APIC_UPDATE:
			// Not handled by high level event handler, start to play
			PlayerLib_SetState(DECODER_STATE_PLAY, TRUE, NULL);
			break;
#endif
		}
		break;
	}
#ifdef USE_PLAYLIST3
	if ((bFavoritesUpdate) && (g_wDecoderSR & DECODER_STOPPED) && (g_ML_save_on_exit == 1)) {
		SysCallFunction(RSRC_MAIN_MENU_CODE_BANK, ML_building_engine_init,0,0,0);
		SysLoadFATWrite();
		ML_SaveOnTheGo();
		SysUnLoadFATWrite();
		SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);
		bFavoritesUpdate = FALSE;
	}
#endif

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


#ifdef JPEG_ALBUM_ART
	g_ReturnValueForPlayerstatemachine = DisplayHint.I;
#endif
#endif

	return DisplayHint.I;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int PSM_GetABMode(void)
//
//   Type:          Function
//
//   Description:   Returns state of AB mode using decoder status register
//
//   Inputs:        none
//
//   Outputs:       AB status - A set, B set, nothing set
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
_reentrant int PSM_GetABMode(void)
{
	int rtn;
	if (g_wDecoderCSR & DECODER_B_SET)
		rtn = DECODER_B_SET;
	else if (g_wDecoderCSR & DECODER_A_SET)
		rtn = DECODER_A_SET;
	else
		return 0;
	return rtn;
}

_reentrant void PSM_ResetScrollInfo(void)
{
	g_bSongStringScroll=FALSE;
	g_iSongStringOffset=0;

	g_bArtistStringScroll=FALSE;
	g_iArtistStringOffset=0;

	g_bAlbumStringScroll=FALSE;
	g_iAlbumStringOffset=0;


	g_iSongStringLength  =GetTextWidthAddressUnicode(g_wSongTitle);
	g_iArtistStringLength=GetTextWidthAddressUnicode(g_wSongArtist);
	g_iAlbumStringLength =GetTextWidthAddressUnicode(g_wSongAlbum);

	if(g_iSongStringLength>LCD_SIZE_X)
	{
		g_bSongStringScroll = TRUE;
		g_iSongStringOffset=-20;
	}

	if(g_iArtistStringLength>LCD_SIZE_X)
	{
		g_bArtistStringScroll = TRUE;
		g_iArtistStringOffset= -20;
	}

	if(g_iAlbumStringLength>LCD_SIZE_X)
	{
		g_bAlbumStringScroll = TRUE;
		g_iAlbumStringOffset= -20;
	}

	// also uncomment #define in musicmenu.c
	//#define DISPLAY_LONG_DIRECTORY_NAME 1
#ifdef DISPLAY_LONG_DIRECTORY_NAME
	// Clear previous display.  stmp7096 fix
	SysPostMessage(6, LCD_CLEAR_RANGE, 0, (SONG_TITLE_Y_POS - 8), SCREEN_WIDTH, 8);
#endif

}

#ifdef AUDIBLE
///////////////////////////////////////////////////////////////////////////
//  FILE        :  _itoa.c
//
//  DESCRIPTION :  Source file for _itoa() routine
//                 This routine converts the given integer to an ascii
//                 string (buffer must be given as an argument).
//                 This routine was formerly used by _doprint() routines.
//
//  Copyright 1996-2002 Altium BV
//
///////////////////////////////////////////////////////////////////////////
char * _itoa( char *p, unsigned int num, unsigned char radix ) {

	register unsigned int   digit;
	register unsigned int   newnum;
	register char *         q;
	register char *         hold;   /* Remember address of destination buffer */

	hold = p;

	q = p + INT_DIGITS - 1 + 2;

	*--q = '\0';    /* be sure to terminate the string with a NULL character        */

	do {
		newnum = num / radix;
		digit = num - (newnum * radix); /* avoid modulo operation */
		if ( digit > 9 )
			digit += 'A'-'0'-10;    /* Convert to hex A-F */
		*--q = digit + '0';
		num = newnum;
	} while ( newnum != 0 );

	while ( (*p++ = *q++) );    /* move the string to the left, including the NULL character    */

	return hold;

}
#endif

#pragma asm
    nolist
    include "resource.inc"
    include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_iUserVolume

    SaveRange Fg_iUserVolume,1

#pragma endasm
