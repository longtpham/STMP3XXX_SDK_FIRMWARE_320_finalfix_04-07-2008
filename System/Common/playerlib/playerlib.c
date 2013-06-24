////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// File : playerlib.c
// Description : Player Library Routines
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "exec.h"
#include "resource.h"
#include "menumanager.h"
#include "messages.h"
#include "playerlib.h"
#include "playlist.h"
#include "sysmem.h"
#include "stringlib.h"
#include "syscoder.h"
#include "project.h"
#include "metadata.h"
#include "sysspeed.h"
#include "filesystem.h"
#ifdef WOW
#include "wowmenu.h"
#endif
#ifdef   USE_PLAYLIST1
#include "playlist1.h"
#include "playlist1internal.h"
#else
#ifdef   USE_PLAYLIST2
#include  "playlist2.h"
#include  "playlist2internal.h"
#else
#ifdef   USE_PLAYLIST3
#include  "playlist3.h"
#include  "playlist3internal.h"
#include  "musiclib_ghdr.h"
#include "playmodemenu.h"
#include "mainmenu.h"
#include "browsemenu.h"
#include "menus.h"
#else
#ifdef   USE_PLAYLIST5
#include "playlist5.h"
#endif
#endif
#endif
#endif
#ifdef WOW
#include "wow_ed.h"
#endif

#ifdef AUDIBLE
#include "audiblesongposition.h"

extern INT DecoderResourcePtr;
#endif
#include "menus.h"
#ifdef JPEG_ALBUM_ART
extern _X BOOL	g_bApicFrameFound;	// declared in "attachedpictureframe.c"

RETCODE _reentrant ApicPostMessage(INT a, INT b, INT *c);	// defined in "attachedpictureframe.c"
#endif
#ifdef SYNC_LYRICS
#include "synclyrics.h"
#endif

#ifdef   USE_PLAYLIST3
#include  "jpegfileutil.h"
extern   INT  iJpegPlaySet;
#endif

#ifdef USE_PLAYLIST5
extern JanusDRMLicExpired;  
extern HasJanusDRMLICFail;
extern bDRMLicExpired;

extern BOOL g_bWait;

extern int   _Y filest_bHasDRM;
extern  filest_bHasJanusDRM;
#endif

#ifdef USE_PLAYLIST2
#ifdef AUDIBLE
BOOL g_bStartOfList = FALSE;
#endif
#endif

///////////////////////////////////////////////////////////////////////////////
//  Prototypes
///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
BOOL g_bPlayListEnabled = TRUE;
_packed BYTE g_CurrentSongName[MAX_FILENAME_LENGTH];
#ifdef USE_PLAYLIST5
long g_CurrentFastKey=0;
#endif
DWORD g_CurrentSongFastkey;

// Stmp00010296 Ticket #71685: for song shorter than the min. skip size
BOOL bCurrentSongShort = FALSE;		//local variable only
////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////
_X INT g_iPlayerState = DECODER_STOPPED;

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////
_asmfunc DecoderForceInit(void);


///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_SetState
//
//  Type:        Function
//
//  Description: Use the function to set the decoder to a specified state:
//                  STOP, PLAY, PAUSE, TOGGLE (switches between PLAY & PAUSE,
//                                             PLAYs if STOPPED)
//
//  Inputs:      iState = state to set decoder to
//               bWait  = if TRUE, function will not return until state is set
//  Outputs:     Success returned if successful, Error if not.
//
//  Notes:
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_SetState (int iState,int bWait,int*ignored)
{
    RETCODE rtn=PLAYERLIB_SUCCESS;
    ignored;
    switch (iState)                              // switch on the desired decoder state
    {
    case DECODER_STATE_STOP:                     // set Decoder to STOP state
      if(!(g_wDecoderSR & DECODER_STOPPED))      // if Decoder NOT stopped-->
      {
#ifdef SYNC_LYRICS
          SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsStop,0,0,0);
#endif
          SysPostMessage(2,DECODER_STOP);        // -->then post a message to STOP
          if(bWait)                              // check if we need to WAIT for STOP to occur
          {
            while(!(g_wDecoderSR & DECODER_STOPPED))
              SysWaitOnEvent(EVENT_TIMER,0,10);  // wait, 10ms ...
          }
      }
	  // For fix FF hang
      DecoderForceInit();                               // update decoder and force init
      SysPostMessage(2,DECODER_RESET);
#ifdef AUDIBLE
		  if( (DecoderResourcePtr != RSRC_AUDIBLE_ACELPDEC_CODE) && (DecoderResourcePtr != RSRC_AUDIBLE_DECMOD_CODE) ) {
			DecSetSongPosZero();                       // reset byte positions to beginning of file
		  }
#else
        DecSetSongPosZero();                       // reset byte positions to beginning of file
#endif

#ifdef WOW
	  SysSpeedClockFree(SPEED_CLIENT_MENU_WOW);  // let speed return to IDLE if not playing
#endif

      break;

    case DECODER_STATE_PLAY:                     // set Decoder to PLAY state
      //it is possible that the player is already heading into play state and our play message will cause it to
      //go to pause state.   The decoder should be fixed to avoid this problem. See decoder settling interval.
      g_iPlayerState = DECODER_PLAYING;

      // Wait decoder settling interval (ms), before checking decoder status.
      SysWaitOnEvent(EVENT_TIMER,0,100); // Prevents hang by keeping playerlib & decoder synchronized.
      // Now check the Decoder's updated status.

      if(!(g_wDecoderSR & DECODER_PLAYING))      // if Decoder NOT playing-->
      {
#ifdef WOW
        // Calling SysSpeedIncrease() will decrease battery life,
        // so only call for WOW to increase battery life. (Stmp00008309)
        // But inter-song delay will be increased if speed is not increased.
        SysSpeedIncrease(SPEED_MAX,SPEED_CLIENT_MENU_WOW);  // boost speed on MP3s
#endif
#ifdef SYNC_LYRICS
		SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsStart,0,0,0);
#endif

#ifndef USE_PLAYLIST5
        SysPostMessage(2,DECODER_PLAY);          // -->then post a message to PLAY
#else        
		if((!g_bWait)|| filest_bHasDRM || filest_bHasJanusDRM)
			bWait = FALSE;

        SysPostMessage(2,DECODER_PLAY);          // -->then post a message to PLAY
//         bWait = FALSE;
#endif
#ifdef MOTION_VIDEO
#ifdef USE_PLAYLIST3
        if(iJpegPlaySet == PLAYSET_MVIDEO)              bWait = FALSE;
#else
#ifndef USE_PLAYLIST5
        if(Playlist_GetPlaySet() == PLAYSET_MVIDEO) bWait = FALSE;
#endif		
#endif
#endif
        if(bWait)                                // check if we need to WAIT for PLAY to occur
        {
          while(!(((g_wDecoderSR & (DECODER_PLAYING|DECODER_BAD_FILE))||g_wCurrentSongBad)))
          {   SysWaitOnEvent(EVENT_TIMER,0,10);    // wait, 10 ms ...
#ifdef USE_PLAYLIST5          
                if (bDRMLicExpired || filest_bHasDRM || filest_bHasJanusDRM)
                    break;
#endif
          }
          if((g_wDecoderSR & DECODER_BAD_FILE) || g_wCurrentSongBad)
          {
              g_iPlayerState = DECODER_STOPPED;
              rtn = PLAYERLIB_BAD_FILE;          // return error code if file will not play
          }
          else // else, mark the current song as played in playlist
          {   SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_MarkCurrentSongPlayed,0,0,0);
          }
        }
      }
      break;

    case DECODER_STATE_PAUSE:
      //it is possible that the player is already heading into pause state and our play message will cause it to
      //go to play state.   The decoder should be fixed to avoid this problem.

#ifdef SYNC_LYRICS
      SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsPause,0,0,0);
#endif
      // Decoder settling time wait (ms) prevents hang after PLAY, PR_FF, quickly followed by PH_FF.
      // Keeps playerlib & decoder synchronized. Stmp00003027/2947  Verified with decoder bitrate limits.
      // Note: Decoder settling time of 30 ms is insufficient for some MPEG rates. 100 ms prevents hang.
      if (bWait)  // If we haven't yet waited for decoder settling time to expire, do so now.
      {  SysWaitOnEvent(EVENT_TIMER,0,100);
      }

      if(!(g_wDecoderSR & DECODER_PAUSED))       // if Decoder NOT paused-->
      {
        if(g_wDecoderSR & DECODER_STOPPED)       // if Decoder stopped==>
          SysPostMessage(2,DECODER_PLAY);        // ==>then post a message to PLAY
        SysPostMessage(2,DECODER_PLAY);          // -->then post a message to PLAY/PAUSE
        if(bWait)                                // check if we need to WAIT for PAUSE to occur
        {
          while(!((g_wDecoderSR & (DECODER_PLAYING|DECODER_BAD_FILE)||g_wCurrentSongBad)))
            SysWaitOnEvent(EVENT_TIMER,0,10);    // wait, 10 ms ...
          if(g_wDecoderSR & DECODER_BAD_FILE)
            rtn = PLAYERLIB_BAD_FILE;            // return error code if file will not play
        }
      }
      break;

    case DECODER_STATE_TOGGLE:                  // this function simply toggles between PLAY and PAUSE
#ifdef SYNC_LYRICS
      if(!(g_wDecoderSR & DECODER_PAUSED))       // if Decoder NOT paused-->
         SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsPause,0,0,0);
      else
         SysCallFunction(RSRC_LYRICS_API_CODEBANK,LyricsStart,0,0,0);
#endif
      // As with state pause, decoder settling time wait (ms) prevents hang. Keeps playerlib & decoder synchronized.
      // Note: See comments for case decoder state pause above.
      SysWaitOnEvent(EVENT_TIMER,0,100);
      SysPostMessage(2,DECODER_PLAY);           // if STOPPED -- toggle will cause a play
      if(bWait)
      {
        //ignoring wait for now.
      }
      break;
    default:
        _asm ( " debug");
        break;
    }
    return PLAYERLIB_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_FastForward
//
//  Type:        Function
//
//  Description: Use the function to FFWD through the song
//                  For SDK: Works from STOP, PLAY, or PAUSE.
//                  For FFWD garble: Works from PLAY or PAUSE.
//
//  Inputs:      None (currently)
//
//  Outputs:     Success returned if successful, Error if not.
//
//  Notes:
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_FastForward (int bPlayDuring,int ignored1,int*ignored2)
{
    RETCODE rtn=PLAYERLIB_SUCCESS;
    ignored1;ignored2;
    //bPlayDuring is ignored for now.

	// Stmp00010296 Ticket #68703: for bad file(s) handling
	// Stmp00010296 Ticket #71685 for song shorter than the min. skip size
	if (!(g_wDecoderCSR & DECODER_BAD_FILE) &&
	    !bCurrentSongShort)
	{
    SysWaitOnEvent(EVENT_TIMER,0,0); // Wait decoder settling time, prevents hang.Stmp00003027/2947
    // Note: decoder settling time + PH_FF cycle rate vs quickswitch timeout affects sequencing.

    //set into pause state
#ifndef USE_PLAYLIST3
    if((g_wDecoderSR & DECODER_PLAYING))        // if PLAYING --> then set to PAUSE state
#endif
      rtn = PlayerLib_SetState (DECODER_STATE_PAUSE,FALSE,NULL);
    SysPostMessage(2,DECODER_FFWD);             // post message to FFWD
	}
	else
		rtn = PLAYERLIB_BAD_FILE;

    return rtn;                                 // return success or failure
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_Rewind
//
//  Type:        Function
//
//  Description: Use the function to rewind through the song
//                  For SDK: Works from STOP, PLAY, or PAUSE.
//                  For FFWD garble: Works from PLAY or PAUSE.
//
//  Inputs:      None (currently)
//
//  Outputs:     Success returned if successful, Error if not.
//
//  Notes:
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_Rewind(int bPlayDuring,int ignored1,int*ignored2)
{
    RETCODE rtn=PLAYERLIB_SUCCESS;
    ignored1;ignored2;
    //bPlayingDuring is ignored for now.

      SysWaitOnEvent(EVENT_TIMER,0,0); // Wait decoder settling time, prevents hang. stmp7634
	// Stmp00010296 Ticket #68703: for bad file(s) handling
	// Stmp00010296 Ticket #71685 for song shorter than the min. skip size
	if (!(g_wDecoderCSR & DECODER_BAD_FILE) && !bCurrentSongShort)
	{
	    if((g_wDecoderSR & DECODER_PLAYING))        // if PLAYING --> then set to PAUSE state
	      rtn = PlayerLib_SetState (DECODER_STATE_PAUSE,FALSE,NULL);
	    SysPostMessage(2,DECODER_RWND);             // post message to rewind
	}
	else
		rtn = PLAYERLIB_BAD_FILE;
//    SysPostMessage(2,DECODER_RWND);             // post message to rewind
    return rtn;;                                // return success or failure
}


///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_SkipToNextSong
//
//  Type:        Function
//
//  Description: This function is responsible for advancing to the next song in
//               the playlist and then returning the decoder to the proper state
//               for the new song (play, stop, ffwd, etc.)
//
//  Inputs:      wMode : NEXT song parameter (i.e. button push, end of song,
//                       ffwd over boundary ...)
//
//  Outputs:     PLAYERLIB_SUCCESS if next song is successfully found and loaded
//               PLAYERLIB_END_OF_LIST if end of list reached (**currently
//                                results in song stopped on first song in list)
//
//  Notes:       Need to devise a complete method for passing the cause for the
//               NEXT song on to the playlist.
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_SkipToNextSong(WORD bUseMode,WORD wMode,int*ignored2)
{
#ifdef USE_PLAYLIST3
	_packed BYTE *szFilename = DirPath; // use *szFilename_temp point to SongInfo[1]
	RETCODE rtn = PLAYERLIB_ERROR;

    INT  iTemp = 0;

    if (g_iRepeatSetting != REPEAT_ALL)
        iTemp = g_current_index;


	//This is a bit of a hack to remember what we were doing prior to stopping
	if(!bUseMode && (g_wDecoderSR & (DECODER_PLAYING|DECODER_PAUSED)))
		wMode = NEXTSONG_PLAY_BUTTON;

	// First, we make sure decoder is in STOP state.
	PlayerLib_SetState(DECODER_STATE_STOP,TRUE,NULL);

	// Goto Next Song
	while (rtn != PLAYERLIB_SUCCESS)
	{
		if (iTemp++ >= g_current_size)
		{
		    rtn = PLAYERLIB_END_OF_LIST;
		    break;
		}

		// Use C Speed client Dclock freq Increase call.
		// Defines are in SysSpeed.inc.  Sysspeed.h is generated
		SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYERLIB);

		if (g_iRepeatSetting != REPEAT_ONE)
		{
			//SysCallFunction(RSRC_METADATA_CODEBANK,GetRelativePath,g_current_index,(INT)(DirPath),0);
			if ((g_iShuffleSetting) && (bPlayVoice == FALSE))
				SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_NEXT_SONG,&g_current_index);
			else
				SysPostMessage(3,MUSICLIB_PLAYNEXTSONG,&g_current_index);
			SysWaitOnEvent(0,0,0);

			SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));

			// Fill in card number, song name (& path), and song number
			if ( (szFilename[0] == 0x2f3a62) || (szFilename[0] == 0x2f3a42))
				g_wCurrentDirDevId = 1;
			else
				g_wCurrentDirDevId = 0;
			packed_strcpy(g_CurrentSongName, szFilename);

			// Find meta-data information for new song
			if (wMode < 256)	 // do not update metadata while deleting file
            			rtn = SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetMetaData,0,0,0);
			else
				rtn = PLAYERLIB_SUCCESS;		  // Fake the rtn code while skiping alot of file. ms
		}
		else
		    rtn = PLAYERLIB_SUCCESS;		  // Fake the rtn code while skiping alot of file. ms

		// stmp6855
		// If PlayerLib_GetMetaData() returns PLAYERLIB_ERROR for invalid song (extension doesn't match metadata)
		// and we are here because of the delete menu, we should still allow an invalid file to be shown and deleted.
		if((rtn == PLAYERLIB_ERROR) && (wMode == NEXTSONG_DELETE_MENU))
		{
			rtn = PLAYERLIB_SUCCESS;
		}

		// Make sure it is a valid song
		if (rtn == PLAYERLIB_SUCCESS)
		{
			//check and see if the decoder changed.  If so, update the decoder and force init.
			DecoderForceInit();
			SysPostMessage(2,DECODER_RESET);
#ifdef AUDIBLE
		  	if( (DecoderResourcePtr != RSRC_AUDIBLE_ACELPDEC_CODE) && (DecoderResourcePtr != RSRC_AUDIBLE_DECMOD_CODE) ) {
				DecSetSongPosZero();                       // reset byte positions to beginning of file
			}
#else
			DecSetSongPosZero();                       // reset file byte positions to 0
#endif
			SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB); // C Speed client frees the shared speed resource

			switch (wMode)
			// wMode contains the parameter attached to next song
			// this needs more work to ensure that all cases are covered and handled properly <<<<
			{
			case NEXTSONG_PLAY_EOF:
			case NEXTSONG_PLAY_BUTTON:
#ifdef JPEG_ALBUM_ART
			if (g_bApicFrameFound) {
				// Post a message to display album art
				SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicPostMessage, 0, 0, NULL);
			} else
#endif
			{
				// decoder was playing and NEXT button was pressed
				PlayerLib_SetState (DECODER_STATE_PLAY,TRUE,NULL);
            }
				SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_MarkCurrentSongPlayed,0,0,0);
				break;
			case NEXTSONG_FFWD:
				// decoder was fast forwarding and reached the end of a song
				PlayerLib_FastForward(0,0,0);
				break;
			default:
				// else -- decoder is stopped, do nothing
				break;
			} // switch (wMode)

			//no repeat and already wrap around
			if (	(g_iRepeatSetting == REPEAT_OFF) &&
				(((g_iShuffleSetting == SHUFFLE_OFF)&&(g_current_index == 0)) ||
				((g_iShuffleSetting == SHUFFLE_ON)&&(g_current_index == g_shuffle_start)))	)
			{
				if (bPlayVoice)
					SysPostMessage(2,DECODER_STOP);
				else
					PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
				g_iPlayerState = DECODER_STOPPED;
				SysWaitOnEvent(0,0,0);
#if 0	//enable only if want to go back to music menu after playback without repeat
				if (wMode != NEXTSONG_FFWD)
					g_MenuFlags.MENU_FLAG_RETURN_TO_MUSIC = TRUE;
#endif
			}

        	}
        	else
           		SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB); // C Speed client frees the shared speed resource
	} // while loop
#else	// #ifdef USE_PLAYLIST3
		INT  iTemp=0;

    RETCODE rtn = PLAYERLIB_ERROR;
	RETCODE TmpRtn;
    _packed BYTE szFilename[MAX_FILENAME_LENGTH];
    SONGFILEINFO SongInfo;
    SONGTIME time;
    SongInfo.m_wBufferLength = MAX_FILENAME_LENGTH;
    SongInfo.m_pFilename = szFilename;

    //This is a bit of a hack to remember what we were doing prior to stopping
    if(!bUseMode
        && (g_wDecoderSR & (DECODER_PLAYING|DECODER_PAUSED)))
        wMode = NEXTSONG_PLAY_BUTTON;
    // First, we make sure decoder is in STOP state.
    PlayerLib_SetState(DECODER_STATE_STOP,TRUE,NULL);

    // Goto Next Song

    while (rtn != PLAYERLIB_SUCCESS)
    {
      // Stmp00010296 Ticket #68703: for the 0 byte file deadloop in REPEAT_1/REPEAT_ALL mode
#ifdef USE_PLAYLIST5

	  TmpRtn = SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetNextSongFileInfo,wMode,0,(void*)&SongInfo);

	  if(
	  	
#else
		TmpRtn = SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetNextSongFileInfo,wMode,0,(void*)&SongInfo);
      if ( (iTemp++ <= g_iTotalTracks) &&
#endif
           (TmpRtn == PLAYLIST_SUCCESS) )
      {

        // Use C Speed client Dclock freq Increase call.
        // Defines are in SysSpeed.inc.  Sysspeed.h is generated
        SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYERLIB);

        // Fill in card number, song name (& path), and song number
        g_wCurrentDirDevId = SongInfo.m_wDeviceID;
        packed_strcpy(g_CurrentSongName, szFilename);
#ifndef USE_PLAYLIST5		
        g_wCurrentSongNumber = SongInfo.m_wTrack;
#endif

        // Find meta-data information for new song
        if (wMode < 256)	 // do not update metadata while deleting file
            rtn = SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetMetaData,0,0,0);
		else
		    rtn = PLAYERLIB_SUCCESS;		  // Fake the rtn code while skiping alot of file. ms


        // stmp6855
        // If PlayerLib_GetMetaData() returns PLAYERLIB_ERROR for invalid song (extension doesn't match metadata)
        // and we are here because of the delete menu, we should still allow an invalid file to be shown and deleted.
        if((rtn == PLAYERLIB_ERROR) && (wMode == NEXTSONG_DELETE_MENU))
        {
            rtn = PLAYERLIB_SUCCESS;
        }

        // Make sure it is a valid song
        if (rtn == PLAYERLIB_SUCCESS)
        {
          //check and see if the decoder changed.  If so, update the decoder and force init.
          DecoderForceInit();
          SysPostMessage(2,DECODER_RESET);
          DecSetSongPosZero();  // Stmp00010296 reset file byte positions to 0
#ifdef AUDIBLE
		  if( (DecoderResourcePtr != RSRC_AUDIBLE_ACELPDEC_CODE) && (DecoderResourcePtr != RSRC_AUDIBLE_DECMOD_CODE) ) {
			DecSetSongPosZero();                       // reset byte positions to beginning of file
		  }
#else
		  DecSetSongPosZero();                       // reset byte positions to beginning of file
#endif

          SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB); // C Speed client frees the shared speed resource

          switch (wMode)
            // wMode contains the parameter attached to next song
            // this needs more work to ensure that all cases are covered and handled properly <<<<
          {
          case NEXTSONG_PLAY_EOF:
            // decoder was playing and reached the end of a file
          case NEXTSONG_PLAY_BUTTON:
#ifdef JPEG_ALBUM_ART
			if (g_bApicFrameFound) {
				// Post a message to display album art
				SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicPostMessage, 0, 0, NULL);
			} else
#endif
			{
#ifdef USE_PLAYLIST5
				if((g_wTotalSongCount == 0) && (g_PL5_PL_queue.head != g_PL5_PL_queue.tail))
				    g_wTotalSongCount = 1;
#endif
            // decoder was playing and NEXT button was pressed
        	PlayerLib_SetState (DECODER_STATE_PLAY,TRUE,NULL);

			}
            SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_MarkCurrentSongPlayed,0,0,0);
            break;
          case NEXTSONG_FFWD:
            // decoder was fast forwarding and reached the end of a song
            PlayerLib_FastForward(0,0,0);
            break;
          default:
            // else -- decoder is stopped, do nothing
            break;
          } // switch (wMode)
        }
        else
           SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB); // C Speed client frees the shared speed resource
      }
      else
      {
          // PlayList had no more songs -- reset to first song
          // Set player state to stopped.
          g_iPlayerState = DECODER_STOPPED;
          SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
#ifdef USE_PLAYLIST5          
          PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);
#endif          

          SysPostMessage(2,MENU_DECODER_STATE_CHNG);
#ifdef USE_PLAYLIST2
#ifdef AUDIBLE
		  g_bStartOfList = TRUE;
#endif
#endif
          rtn = PLAYERLIB_END_OF_LIST;
          break;
      }
    } // while loop
#endif	// #ifdef USE_PLAYLIST3

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_SkipToPreviousSong
//
//  Type:        Function
//
//  Description: This function is responsible for backing up to the previous
//               song and then returning the decoder to the proper state (stop,
//               play, rwnd, etc.)
//
//  Inputs:      wMode : PREV song parameter (i.e. "PREV" button push, rewind
//                       over boundary, etc.)
//
//  Outputs:     PLAYERLIB_SUCCESS if prev song is successfully found and loaded
//               PLAYERLIB_END_OF_LIST if no songs available
//
//  Notes:       Need to devise a complete method for passing the cause for the
//               PREV song on to the playlist.
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_SkipToPreviousSong(WORD bUseMode, WORD wMode,int*ignored2)
{
#ifdef USE_PLAYLIST3
	_packed BYTE *szFilename = DirPath; // use *szFilename_temp point to SongInfo[1]
	RETCODE rtn = PLAYERLIB_ERROR;

	// Go to PREVIOUS song ...
	if(!(wMode & PREVSONG_RWND))
		PlayerLib_SetState(DECODER_STATE_STOP,TRUE,NULL);
	// Use C Speed client Dclock freq Increase call.
	// Defines are in SysSpeed.inc.  Sysspeed.h is generated
	SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYERLIB);

//	//get the previous track by using PD musiclib
	if ((g_iShuffleSetting) && (bPlayVoice == FALSE))
		SysPostMessage(4,MUSICLIB_SHUFFLE_NEXT,SHUFFLE_PREVIOUS_SONG,&g_current_index);
	else
		SysPostMessage(3,MUSICLIB_PLAYPREVIOUSSONG,&g_current_index);
	SysWaitOnEvent(0,0,0);
	SysCallFunction(RSRC_METADATA_CODEBANK,RetrieveSongInfo,0,0,(INT*)(DirPath));

	if ( (szFilename[0] == 0x2f3a62) || (szFilename[0] == 0x2f3a42))
		g_wCurrentDirDevId = 1;
	else
		g_wCurrentDirDevId = 0;
	packed_strcpy(g_CurrentSongName, DirPath);
        SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetMetaData,0,0,0);

	SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB); // C Speed client frees the shared speed resource

	//check and see if the decoder changed.  If so, update the decoder and force init.
	DecoderForceInit();
	SysPostMessage(2,DECODER_RESET);
#ifdef AUDIBLE
		  if( (DecoderResourcePtr != RSRC_AUDIBLE_ACELPDEC_CODE) && (DecoderResourcePtr != RSRC_AUDIBLE_DECMOD_CODE) ) {
			DecSetSongPosZero();                       // reset byte positions to beginning of file
		  }
#else
      DecSetSongPosZero();                       // reset byte positions to beginning of file
#endif
	if(wMode & PREVSONG_PLAY)      // decoder was playing and PREV button pressed
	{
#ifdef JPEG_ALBUM_ART
		if (g_bApicFrameFound) {
			// Post a message to display album art
			SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicPostMessage, 0, 0, NULL);
		} else
#endif
		{
		PlayerLib_SetState(DECODER_STATE_PLAY,TRUE,NULL);
	    }
	}
	else if (wMode & PREVSONG_RWND)  // decoder was rewinding and hit beginning of song
	{
		DecSetSongPosEnd();
		rtn =PlayerLib_Rewind(0,0,0);
		if (rtn != PLAYERLIB_SUCCESS)	//return with error
    		    return rtn;
	}
	else
	{
		// else -- decoder is stopped, do nothing
	}
	rtn = PLAYERLIB_SUCCESS;
#else	// #ifdef USE_PLAYLIST3
    RETCODE rtn = PLAYERLIB_END_OF_LIST;
    _packed BYTE szFilename[MAX_FILENAME_LENGTH];
	RETCODE TmpRtn; 	

    SONGFILEINFO SongInfo;
    SONGTIME time;
    SongInfo.m_wBufferLength = MAX_FILENAME_LENGTH;
    SongInfo.m_pFilename = szFilename;

     // Go to PREVIOUS song ...
    if(!(wMode & PREVSONG_RWND))
        PlayerLib_SetState(DECODER_STATE_STOP,TRUE,NULL);

#ifdef USE_PLAYLIST5	  		
	if((g_iPlaylistRepeat == PLAYLIST_REPEAT_ONE) && (wMode !=PREVSONG_PLAY) )
	{
		TmpRtn = PLAYLIST_SUCCESS;
        SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetCurrentSongFileInfo,0,0,(int*)&SongInfo);

	}
	else
	{
		TmpRtn = SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo,0,0,(void*)&SongInfo);
	}
    if(TmpRtn==PLAYLIST_SUCCESS)
#else
    if(SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetPreviousSongFileInfo,0,0,(void*)&SongInfo)==PLAYLIST_SUCCESS)
#endif    
    {
      // Use C Speed client Dclock freq Increase call.
      // Defines are in SysSpeed.inc.  Sysspeed.h is generated
      SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYERLIB);

      g_wCurrentDirDevId = SongInfo.m_wDeviceID;
      packed_strcpy(g_CurrentSongName, szFilename);
#ifndef USE_PLAYLIST5	  
      g_wCurrentSongNumber = SongInfo.m_wTrack;
#endif
      rtn = SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetMetaData,0,0,0); // Stmp00010296

      SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB); // C Speed client frees the shared speed resource

		if (rtn == META_DATA_FOUND)     // Stmp00010296
    	{
          //check and see if the decoder changed.  If so, update the decoder and force init.
          DecoderForceInit();
          SysPostMessage(2,DECODER_RESET);
     	  DecSetSongPosZero();              // Stmp00010296 reset file byte positions to 0.

#ifdef AUDIBLE
		  if( (DecoderResourcePtr != RSRC_AUDIBLE_ACELPDEC_CODE) && (DecoderResourcePtr != RSRC_AUDIBLE_DECMOD_CODE) ) {
			DecSetSongPosZero();                       // reset byte positions to beginning of file
		  }
#else
      DecSetSongPosZero();                       // reset byte positions to beginning of file
#endif

      if(wMode & PREVSONG_PLAY)      // decoder was playing and PREV button pressed
      {
#ifdef JPEG_ALBUM_ART
		if (g_bApicFrameFound) {
			// Post a message to display album art
			SysCallFunction(RSRC_APIC_FRAME_CODEBANK, ApicPostMessage, 0, 0, NULL);
		} else
#endif
		{
        PlayerLib_SetState(DECODER_STATE_PLAY,TRUE,NULL);
		}
      }
      else if (wMode & PREVSONG_RWND)  // decoder was rewinding and hit beginning of song
      {
        DecSetSongPosEnd();
        PlayerLib_Rewind(0,0,0);
      }
      else
      {
          // else -- decoder is stopped, do nothing
      }
      rtn = PLAYERLIB_SUCCESS;
	    }   // Stmp00010296
    }
#ifdef USE_PLAYLIST5    
	else
	{
      g_iPlayerState = DECODER_STOPPED;
      SysCallFunction(RSRC_PLAYERLIB_EXTRA_CODE_BANK,PlayerLib_GetCurrentSong,0,0,0);
      PlayerLib_SetState (DECODER_STATE_STOP,TRUE,NULL);

      SysPostMessage(2,MENU_DECODER_STATE_CHNG);
      rtn = PLAYERLIB_END_OF_LIST;
	}
#endif	
#endif	// #ifdef USE_PLAYLIST3
    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_EnablePlaylist
//
//  Type:        Function
//
//  Description: Enable the playlist.
//
//  Inputs:      None used.
//
//  Outputs:     PLAYERLIB_SUCCESS
//
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_EnablePlaylist(int bTrueFalse,int ignored1,int*ignored2)
{
    g_bPlayListEnabled = bTrueFalse;
    return PLAYERLIB_SUCCESS;
}

