// Filename: musicmenu.c
// Description: passes messages to playerstatemachine mostly but handles some messages.
// This screen displays songs in playlist and player status. It is the home screen for the player application.

#include "types.h"
#include "exec.h"
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
#include "musicmenu_extra.h"
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include "browsemenu.h"
#else
#ifdef   USE_PLAYLIST5
#include "playlist5.h"
#endif
#endif
#endif
#endif
#include "stringlib.h"
#include "hwequ.h"
#ifdef JPEG_ALBUM_ART
#include "JpegDecodeDisplay.h"
extern _X BOOL	g_bApicFrameFound;					// declared in "attachedpictureframe.c"
#ifdef USE_PLAYLIST5
int g_ReturnValueForPlayerstatemachine=0;
#endif
#endif
#ifdef SYNC_LYRICS
#include "synclyrics.h"
#endif

typedef unsigned int   U24;

#if defined CLCD && BACKLIGHT && JANUS && !(BATTERY_TYPE_LI_ION)
#include "lowresolutionadc.h"
#define WMA_LOW_BATTERY_THRESHOLD   1150    //1150 mV
BOOL bWMALowBattery = FALSE;
#endif

#ifdef PLAYER_STRESS
#include "playmodemenu.h"
//#define REBOOT_COUNTDOWN_MS (30*1000L) //30*1000 gives 30 seconds until it reboots. for development
#define REBOOT_COUNTDOWN_MS (6*60*60*1000L) //6 Hours
extern INT  g_iNumberOfResourceDriveRefreshes; // defined in restoredrivefrombackup.c
extern INT _Y g_iSysSaveSettingsDone;
extern _asmfunc void SysSaveSettings(void);
#endif

#ifdef JANUS
extern int CleanDataStoreFlag;
#endif
#ifdef USE_PLAYLIST5
extern BOOL g_bLastDisplay;
extern BOOL g_bLastDisplaySongCnt;


extern BOOL g_bWait;
#endif

#ifdef USE_PLAYLIST2
#ifdef AUDIBLE
extern BOOL g_bStartOfList;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          int _reentrant MusicMenu(int a, int b, int *pPtr)
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
int _reentrant MusicMenu(int a, int b, int *pPtr)
{
#ifdef PLAYER_STRESS
	int iRefreshFlag = 0;
#endif
	//	SGTL-HK 28-10-2004
#define DEPLAY_LONG_DIRECTORY_NAME

	union DisplayHints DisplayHint;

	INT  iCurrentSong= -1;
	INT  iEvent;
	BOOL bDone = FALSE;
	INT  iNewMenu = MENU_MAIN;
	BOOL bSendToPlayerStateMachine = FALSE;
#ifdef JPEG_ALBUM_ART
	BOOL	bDecodeProgress = FALSE;
#endif
#ifdef USE_PLAYLIST5
    BOOL bLastDisplaySongCnt = FALSE;
#endif
	// this is required to get the message into the playerstatemachine
	// casted to avoid pointer mismatch warnings
	pPtr = (int *) &gEventInfo;

	a;b;//here to prevent parameter not used messages.

	//set our current menu
	g_iCurrentMenu = MENU_MUSIC;
#if (defined USE_PLAYLIST3) || (defined USE_PLAYLIST5)
	g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = FALSE;
#endif	// #if (defined USE_PLAYLIST3) || (defined USE_PLAYLIST5)

#ifndef USE_PLAYLIST3
	if(Playlist_GetPlaySet() != PLAYSET_MUSIC)
	{   //sdk2.1 (i.e. if currently not in music mode.)
		//initialize the music player state machine
		ChangePlaySet(PLAYSET_MUSIC);
	}
#endif	// #ifdef USE_PLAYLIST3

	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, InitPlayerStateMachine,0,0,0);
#ifdef USE_PLAYLIST3
	SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK, ML_browsing_app_init,0,0,0);
#endif

#ifdef SYNC_LYRICS
	if (g_iPlayerState == DECODER_PLAYING)
		SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsStart,0,0,0);
#endif
	//***** Moved from mainmenu.c
	SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,100,MENU_MSG_ANIMATE);

	#ifdef PLAYER_STRESS
	{
	   int button = PR_PLAY;
	   SysCallFunction(RSRC_PLAYMODE_MENU_CODE_BANK,SendPlayMode,PM_REPEATALL,0,0);
	   SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,EVENT_BUTTON,0,&button);
	}
	#endif

#ifdef USE_PLAYLIST5
#ifdef SYNC_LYRICS
	DisplayHint.I = MUSIC_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
	DisplayHint.I &= ~((WORD)(1<<DISPLAY_SONGARTISTALBUM_BITPOS));
	DisplayHint.I &= ~((WORD)(1<<DISPLAY_SONGTITLE_BITPOS));
	SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
	DisplayHint.I = MUSIC_MENU_DISPLAY_ALL;
#else
	//Lets initially display all & clear the screen
	DisplayHint.I = MUSIC_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
#endif

	g_bWait = TRUE;
#else
	//Lets initially display all & clear the screen
	DisplayHint.I = MUSIC_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
#endif  //#ifdef USE_PLAYLIST5	
	while(!bDone)
	{

		//keep only display hints we're interested in displaying in this menu.
		DisplayHint.I &= (MUSIC_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
#ifdef JPEG_ALBUM_ART
        if(g_iSeeking != SEEK_NONE)
        {   DisplayHint.I &= ~(1<<DISPLAY_ALBUMART_BITPOS);
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
#endif
		iEvent = SysWaitOnEvent(EVENT_MESSAGE|EVENT_BUTTON,&gEventInfo.msg, DisplayHint.I? 1 : -1);

		//Lets assume we do want to send all these to the player state machine
		bSendToPlayerStateMachine = TRUE;

		switch(iEvent)
		{
		case EVENT_TIMER:
			//we've gotten a timeout event, so we've processed all our other events and
			//should update the display
			if(DisplayHint.I)
			{
#ifdef USE_PLAYLIST2
#ifdef AUDIBLE
				if(g_bStartOfList)
				{
					SysWaitOnEvent(0,0,0);
					g_bStartOfList = FALSE;
				}
#endif
#endif
				SysCallFunction(RSRC_DISPLAY_CODE_BANK,RefreshDisplay,DisplayHint.I,1,0);
			}
			DisplayHint.I = 0;//clear out the bits
			bSendToPlayerStateMachine = FALSE;//don't need to send this to the player state machine
                #ifdef PLAYER_STRESS
                    if(iRefreshFlag == 0) // only post this message on first timer event
                    {   iRefreshFlag = 1;
                        SysPostMessage(7,LCD_PRINT_NUMBER,40,16,g_iNumberOfResourceDriveRefreshes,3,ZERO_CHAR);
                    }
                #endif
			break;
		case EVENT_MESSAGE:
			switch(gEventInfo.Message.wMsgCommand)
			{
#ifdef SYNC_LYRICS
		case MENU_MSG_LYRICS_SYNC_AB:		// Song arrived B position and Lyrics have to re-sync. to A
			SysWaitOnEvent(0,0,100);
			SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsStart,0,0,0);
			bSendToPlayerStateMachine = FALSE;
			break;

		case MENU_MSG_LYRICS_UPDATE:		// It's time to display sycn. lyrics.
			SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
			SysWaitOnEvent(0,0,0);// Let system process SOFT_TIMER_KILL_TIMER message
			if (DisplaySongLyrics(1,0,0) == TRUE)
			{
#ifdef CLCD
				DisplayHint.I &= ~((WORD)(1<<DISPLAY_SONGARTISTALBUM_BITPOS));
#else
				DisplayHint.I &= ~((WORD)((1<<DISPLAY_SONGARTISTALBUM_BITPOS)|(1<<DISPLAY_SONGTITLE_BITPOS)));
#endif
			}
			else
			{
			    DisplayHint.I |= MUSIC_MENU_DISPLAY_ALL;
			    if(g_iSeeking == SEEK_NONE)
			    {   DisplayHint.I |= (1<<DISPLAY_CLEAR_DISPLAY_BITPOS);
			    }
			}
			SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,100,MENU_MSG_ANIMATE);
			SysWaitOnEvent(0,0,0);
			bSendToPlayerStateMachine = FALSE;
			break;
#endif
		case MENU_MSG_ANIMATE:
		#ifdef PLAYER_STRESS
                    HW_GP0DOR.B.B9 = !HW_GP0DOR.B.B9; //toggle backlight on EVK for countdown indication.  TOVERIFY6 is this pin available always?
                    if (SysGetCurrentTime() > REBOOT_COUNTDOWN_MS)  // in the eastech stress test they shutdown & reboot player every 6 hours. This just reboots every 6 hrs.
                    {
		        SysPostMessage(2,DECODER_STOP);
			SysWaitOnEvent(0,0,2);
			SysSaveSettings(); // to save g_iNumberOfResourceDriveRefreshes
		        while ( g_iSysSaveSettingsDone == 0 ); // tested ok by MD. UPDATE: JUNE 26 '06:  STUCK IN THIS LOOP WE THINK DURING PLAYER_STRESS.
                        SystemHalt(); // resets in RETAIL build so make sure that when player.mk defines PLAYER_STRESS, that RETAIL should be forced to also be defined in player.mk
                        // system will gracefully shutdown and reset now.
                    }
		#endif // PLAYER_STRESS
#ifdef SYNC_LYRICS
			if (LyricsIsValid() == FALSE || g_iPlayerState == DECODER_STOPPED)
#endif
			{
				if(g_bSongStringScroll)
				{
					DisplayHint.bits.SongTitle = TRUE;
					g_iSongStringOffset+=1;
					if(g_iSongStringOffset>g_iSongStringLength)
						g_iSongStringOffset=-SONG_ARTIST_X_SIZE;
				}
				if(g_bArtistStringScroll)
				{
					DisplayHint.bits.SongArtistAlbum = TRUE;
					g_iArtistStringOffset+=1;
					if(g_iArtistStringOffset>g_iArtistStringLength)
						g_iArtistStringOffset=-SONG_ARTIST_X_SIZE;
				}

				if(g_bAlbumStringScroll)
				{
					DisplayHint.bits.SongArtistAlbum = TRUE;
					g_iAlbumStringOffset+=1;
					if(g_iAlbumStringOffset>g_iAlbumStringLength)
						g_iAlbumStringOffset=-SONG_ALBUM_X_SIZE;
				}
			}
#ifdef SYNC_LYRICS
			else
			{
#ifdef CLCD
				if (g_bSongStringScroll)
					g_iSongStringOffset = 0;
				if (g_bArtistStringScroll)
					g_iArtistStringOffset = 0;
				if (g_bAlbumStringScroll)
					g_iAlbumStringOffset = 0;
#endif
			}
#endif
			bSendToPlayerStateMachine = FALSE;
			break;
#ifdef JPEG_ALBUM_ART
		case MENU_MSG_JPEG_CALLBACK:
			if(bDecodeProgress) {
				if(Jpeg_DecodeDisplay() != JPEG_SUCCESS ||
					Jpeg_DecodeDisplay() != JPEG_SUCCESS ||
					Jpeg_DecodeDisplay() != JPEG_SUCCESS) {
						//either we reached end of file or there was an error in decoding
						DisplayAlbumArtClose(0,0,0);
						bDecodeProgress = FALSE;
				}
			}
			bSendToPlayerStateMachine = FALSE;
			break;

		case MENU_MSG_APIC_UPDATE:
			// Album art is found, setup JPEG decoder to display the picture
			if (g_bApicFrameFound) {
				if (DisplayHint.I) {
					SysCallFunction(RSRC_DISPLAY_CODE_BANK, RefreshDisplay, DisplayHint.I, TRUE, NULL);
					DisplayHint.I = 0;
				}
				if (DisplayAlbumArtInit(0,0,0) == SUCCESS) {
					bDecodeProgress = TRUE;	//To keep calling Jpeg Decode Display.
					bSendToPlayerStateMachine = FALSE;
				}
			}
			break;

		case PARSER_DEVICE_ENUMERATE:
#ifdef MMC
		case MENU_MSG_MEDIA_OVER_CUR:
#endif
			if (bDecodeProgress) {
				// JPEG decoding process stopped by event message
				DisplayAlbumArtClose(0,0,0);
				bDecodeProgress = FALSE;
			}
			break;
#endif
			}
			break;
		case EVENT_BUTTON:
			// button pressed, so restart timer if enabled
			UpdateAutoShutdownTimer();
#if defined CLCD && BACKLIGHT && JANUS && !(BATTERY_TYPE_LI_ION)
            if(SysLowResolutionAdcReadBin(BATT) <= MvToLradcResult(WMA_LOW_BATTERY_THRESHOLD))
            {   bWMALowBattery = TRUE;
            }else
            {   bWMALowBattery = FALSE;
            }
#endif
#ifdef BACKLIGHT
			// turn on backlight when button is pressed.
			if (g_iBackLightState==BACKLIGHT_ON)
			{
				SysPostMessage(2,LCD_BACKLIGHT_ON);
				SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_BACKLIGHT,1,BACKLIGHT_TIME,MENU_MSG_TURN_OFF_BACKLIGHT);
				SysWaitOnEvent(0,0,0);
			}
#endif
#ifdef JPEG_ALBUM_ART
			if (bDecodeProgress) {
				// JPEG decoding process stopped by event button
				DisplayAlbumArtClose(0,0,0);
				bDecodeProgress = FALSE;
			}
#endif

			//remember the last button event (mostly for coming out of FFWD and RWND)
			switch(g_wLastButton = gEventInfo.Button.wButtonEvent)
			{
			case PH_PLAY:
			case PH_STOP:
#ifdef SYNC_LYRICS
				if (g_iPlayerState == DECODER_PLAYING)
					SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsPause,0,0,0);
#endif
#ifdef JANUS
				CleanDataStoreFlag = 1;
#endif
				//if the STOP button is held down, lets call the shutdown menu
				SysCallFunction(RSRC_MAIN_MENU_CODE_BANK,ShutdownMenu,USERSHUTDOWN,0,0);
				// if returns update display  -- if doesn't return, powered down
				DisplayHint.I |= (MUSIC_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
				bSendToPlayerStateMachine = FALSE;
#ifdef SYNC_LYRICS
				if (g_iPlayerState == DECODER_PLAYING)
					SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsStart,0,0,0);
#endif
				break;

			case PR_MENU:
				// because SaveChanges message interrupts PH_MENU, we need to make
				// sure the next PR_MENU in musicmenu is not ignored.
				if (g_MenuFlags.MENU_FLAG_SAVE_CHANGES)
				{
					bDone = TRUE;
					g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = FALSE;
					g_MenuFlags.MENU_FLAG_SAVE_CHANGES = FALSE;
				}
				// was this the case where PH_MENU from other menus allowed escape??
				// if so ignore
				if (g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC)
				{
					g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = FALSE;
				}
				else
				{  //defaults to main menu when exiting.  No need to set a new menu.
					bDone = TRUE;
				}
				bSendToPlayerStateMachine = FALSE;
				break;


				/*
				case PR_ERASE: //sdk2.1 (should go in the deletemenu, and return)
				//*****
				SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
				SysWaitOnEvent(0,0,0);// Let system process SOFT_TIMER_KILL_TIMER message


				SysCallFunction(RSRC_DELETE_MENU_CODE_BANK,DeleteMenu,1,PLAYSET_MUSIC,0);
				g_MenuFlags.MENU_FLAG_ESCAPE_TO_MUSIC = FALSE; //cause, already in music menu
				DisplayHint.I |= (MUSIC_MENU_DISPLAY_ALL|(1<<DISPLAY_CLEAR_DISPLAY_BITPOS));
				bSendToPlayerStateMachine = FALSE;

				//*****
				SysPostMessage(6,SOFT_TIMER_SET_TIMER,TIMER_ANIMATE,0,100,MENU_MSG_ANIMATE);
				SysWaitOnEvent(0,0,0);// Let system process SOFT_TIMER_KILL_TIMER message
				break;
				*/

				// uncomment "#define DISPLAY_LONG_DIRECTORY_NAME" below and in playerstatemachine.c
				// to allow PR_RECORD to display long file names
//#define DISPLAY_LONG_DIRECTORY_NAME 1
#ifdef DISPLAY_LONG_DIRECTORY_NAME

			case PR_RECORD:
				{
					WORD		wDirectoryName[50];
					DirEntry _X*	pEntry;
					SysPostMessage(6, LCD_CLEAR_RANGE, 0, (SONG_TITLE_Y_POS - 8), SCREEN_WIDTH, 8);
					pEntry = (DirEntry _X *)SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetDirEntry, 0, 0, (INT *)&g_CurrentTrack);
					if (SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetLFN, TYPE_DIR, (INT)pEntry, (INT *)wDirectoryName) == PLAYLIST_SUCCESS)
					{
						SysPostMessage(5, LCD_PRINT_STRING_UNICODE_ADDR, 0, (SONG_TITLE_Y_POS - 8), wDirectoryName);
						SysWaitOnEvent(0, 0, 500);
					}
					else
					{
						SysCallFunction(RSRC_PLAYLIST_CODEBANK, Playlist_GetSFN, TYPE_DIR, (INT)pEntry, (INT *)wDirectoryName);
						SysPostMessage(5, LCD_PRINT_STRING_ADDR, 0, (SONG_TITLE_Y_POS - 8), wDirectoryName);
						SysWaitOnEvent(0, 0, 500);
					}
				}
				bSendToPlayerStateMachine = FALSE;
				break;

#endif

			default:

				break;
			}
			break;
		}
		if(bSendToPlayerStateMachine)
		{//only send events to the player state machine if necessary (to avoid code bank swapping)
#ifdef USE_PLAYLIST5		
#ifdef JPEG_ALBUM_ART
			SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
			DisplayHint.I |= g_ReturnValueForPlayerstatemachine;
#else
			DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
#endif
#else
			DisplayHint.I |= SysCallFunction(RSRC_PLAY_STATE_MACHINE_CODE_BANK,HandlePlayerStateMachine,iEvent,0,pPtr);
#endif  //#ifdef USE_PLAYLIST5  
			//or the display hints returned with the current collection of hints
		}

	}
	//*****
#ifdef SYNC_LYRICS
	if (g_iPlayerState == DECODER_PLAYING)
		SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsPause,0,0,0);
#endif
	SysPostMessage(3,SOFT_TIMER_KILL_TIMER,TIMER_ANIMATE);
	SysWaitOnEvent(0,0,0);// Let system process SOFT_TIMER_KILL_TIMER message

	//don't exit player state machine, cause we want to keep playing when in main menu.
	//sdk2.1
	return iNewMenu;
}

#ifdef PLAYER_STRESS
// Use SaveRange macro to save a copy of global variable addresses as well as
// reserve space for their values in section "Player_settings_Y".  All of which
// will be used at shutdown to save user settings in the Settings.dat file.
#pragma asm
    nolist
        include "resource.inc"
        include "sysmacro.asm"
    list
    org y,"Player_settings_Y":
    extern y:Fg_iNumberOfResourceDriveRefreshes

    SaveRange Fg_iNumberOfResourceDriveRefreshes,1

#pragma endasm
#endif

