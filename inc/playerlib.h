#ifndef _PLAYERLIB_H
#define _PLAYERLIB_H

#include "types.h"
#include "playlist.h"

#define DECODER_STATE_STOP	    0
#define DECODER_STATE_PLAY	    1
#define DECODER_STATE_PAUSE	    2
#define DECODER_STATE_TOGGLE	4

#define PLAYERLIB_SUCCESS       0
#define PLAYERLIB_BAD_FILE      1
#define PLAYERLIB_ERROR         2
#define PLAYERLIB_END_OF_LIST   3

//These are in the DecoderSR/DecoderCSR
#define DECODER_PAUSED          1<<5
#define DECODER_STOPPED         1<<6
#define DECODER_SYNCED          1<<10
#define DECODER_PLAYING         1<<12
#define DECODER_SONG_INFO       1<<15
#define DECODER_FILE_IS_OPEN    1<<16
#define DECODER_A_SET           1<<18
#define DECODER_B_SET           1<<19
#define DECODER_BAD_FILE        1<<21
#define DECODER_LOOKING_FOR_SYNC 1<<22

//PrevSong Message Parameters
#define PREVSONG_STOP        0             //PrevSong + Stopped
#define PREVSONG_PLAY        1<<0          //PrevSong + Play
#define PREVSONG_RWND        1<<1          //PrevSong + Rwnd
//NextSong Message Parameters
#define NEXTSONG_STOP        0             //NextSong + Stopped
#define NEXTSONG_PLAY_EOF    1             //NextSong + Play + EOF reached
#define NEXTSONG_PLAY_BUTTON 3             //NextSong + Play + BUTTON pressed
#define NEXTSONG_FFWD        4             //NextSong + Ffwd
#define NEXTSONG_DELETE_MENU 5             //NextSong via delete menu
//CurrentSong Message Parameters
#define CURRENTSONG_DELETE_MENU 1          //CurrentSong via delete menu

#ifdef USE_PLAYLIST3
extern DWORD    g_CurrentSongFastkey;
#endif  // USE_PLAYLIST3

#ifdef USE_PLAYLIST5
extern long g_CurrentFastKey;
#endif //USE_PLAYLIST5
RETCODE _reentrant PlayerLib_SetState (int iState,int bWait,int*);  
RETCODE _reentrant PlayerLib_FastForward (int bPlayDuring,int,int*);
RETCODE _reentrant PlayerLib_Rewind(int bPlayDuring,int,int*);
RETCODE _reentrant PlayerLib_SetSongName(int bStartPlaying, int, SONGFILEINFO*);
RETCODE _reentrant PlayerLib_GetCurrentSong(WORD wMode,int ignored1,int*ignored2);
RETCODE _reentrant PlayerLib_SkipToNextSong(WORD,WORD,int*);
RETCODE _reentrant PlayerLib_SkipToPreviousSong(WORD,WORD,int*);
RETCODE _reentrant PlayerLib_EnablePlaylist(int bTrueFalse,int,int*);  
RETCODE _reentrant PlayerLib_GetMetaData(WORD wMode,int ignored1,int*ignored2);
DWORD _reentrant PlayerLib_GetSongPosition(void);
RETCODE _reentrant PlayerLib_SetSongPosition(DWORD dwSongPos);
////////////////////Bookmarking functions///////////////////////////////
RETCODE _reentrant PlayerLib_ResetBookmark(int iCurrentPlayset, int ignored, int* pPtr);
RETCODE _reentrant PlayerLib_SetBookmark(int iCurrentPlayset, int iTracknum,int* pPtr);
RETCODE _reentrant PlayerLib_GotoTrack(int iTracknum, int ignored, int* ptr);
RETCODE _reentrant PlayerLib_GetBookmarkSongInfo(int iCurrentPlayset, int ignored2, int* pPtr);
#ifdef USE_PLAYLIST3
RETCODE _reentrant PlayerLib_GetCurrentSong_ML(WORD wMode,int ignored1,int*ignored2);
#endif // #ifdef USE_PLAYLIST3

#endif 
