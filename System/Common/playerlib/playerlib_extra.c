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
#else
#ifdef   USE_PLAYLIST5
#include  "playlist5.h"
#endif
#endif
#endif
#endif
#ifdef WOW
#include "wowmenu.h"
#endif
#ifdef AUDIBLE
#include "audiblesongposition.h"

extern INT DecoderResourcePtr;
#endif

///////////////////////////////////////////////////////////////////////////////
//  Prototypes
///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
WORD g_wBitRateFound = 0;
BOOL g_bInitBookmarkActive = FALSE;
extern _packed BYTE g_CurrentSongName[MAX_FILENAME_LENGTH];
extern BOOL bCurrentSongShort;
#ifdef USE_PLAYLIST3
extern _packed BYTE DirPath[];
extern BOOL g_Rebuild;
extern int g_current_index;
extern int g_current_size;
extern int g_shuffle_start;
#endif

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////
_asmfunc DecoderForceInit(void);

#ifdef USE_PLAYLIST3
// Stmp00013553
extern RAM_SONG_INFO_T song_info;
// Stmp00013553
#endif

static _reentrant void TrimUnicodeString(UCS3* pString)
{
	int i;
	i = strlen(pString)-1;
	while((i>=0) && (pString[i]==' '))
	{
		pString[i]=0;
		i--;
	}
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant PlayerLib_GotoTrack(int iTracknum, int ignored, int* ptr)
//
//   Type:          Function
//
//   Description:   This advances g_iInternalTrack and makes it point to the offset in the playlist which stores iTracknum
//
//   Inputs:        iTracknum you want to advance to
//
//   Outputs:       RETCODE
//
//   Notes:
//<
//////////////////////////////////////////////////////

RETCODE _reentrant PlayerLib_GotoTrack(int iTracknum, int ignored, int* ptr){
#ifdef USE_PLAYLIST1
	RETCODE rtn=PLAYERLIB_ERROR;
	int i;

	//first search g_InternalList for the track number matching with iTracknum and
	//then set g_iInternaTrack = index of that track
	if(g_iTotalTracks)
	{
		for(i=0;i<g_iTotalTracks;i++)
			if(g_iInternalList[i] == iTracknum){
				g_iInternalTrack = i;
				rtn=PLAYERLIB_SUCCESS;
				break;
			}
	}
	return rtn;
#else
	// Ken 2005/8/2 02:26
#if (defined USE_PLAYLIST2) || (defined USE_PLAYLIST3) || (defined USE_PLAYLIST5)
	return PLAYERLIB_SUCCESS;
#endif
	// Ken 2005/8/2 02:26
#endif

}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant PlayerLib_ResetBookmark(int iCurrentPlayset, int ignored, int* pPtr)
//
//   Type:          Function
//
//   Description:   This resets bookmark for the current playset to zero
//
//   Inputs:        iCurrentPlayset
//
//   Outputs:       RETCODE
//
//   Notes:
//<
//////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_ResetBookmark(int iCurrentPlayset, int ignored, int* pPtr){

	RETCODE rtn=PLAYERLIB_SUCCESS;
#ifndef USE_PLAYLIST3
#ifndef USE_PLAYLIST5
	switch(iCurrentPlayset)
	{
	case PLAYSET_MUSIC:
		g_MarkerMusic.m_iTracknum = 0;
		break;
	case PLAYSET_VOICE:
		g_MarkerVoice.m_iTracknum = 0;
		break;
	default:
		rtn = PLAYERLIB_ERROR;
		break;

	}
#endif // #ifndef USE_PLAYLIST5
#endif // #ifndef USE_PLAYLIST3
	return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant PlayerLib_SetBookmark(int iCurrentPlayset, int iTracknum,int* pPtr)
//
//   Type:          Function
//
//   Description:   This sets the bookmark for the current playset
//
//   Inputs:        iCurrentPlayset
//
//   Outputs:       RETCODE
//
//   Notes:
//<
//////////////////////////////////////////////////////

RETCODE _reentrant PlayerLib_SetBookmark(int iCurrentPlayset, int iTracknum,int* pPtr){

	RETCODE rtn=PLAYERLIB_SUCCESS;
#ifndef USE_PLAYLIST3
#ifndef USE_PLAYLIST5
	switch(iCurrentPlayset)
	{
	case PLAYSET_MUSIC:
		g_MarkerMusic.m_iTracknum = iTracknum;
		break;
	case PLAYSET_VOICE:
		g_MarkerVoice.m_iTracknum = iTracknum;
		break;
	default:
		rtn = PLAYERLIB_ERROR;
		break;
	}
#endif // #ifndef USE_PLAYLIST5
#endif // #ifndef USE_PLAYLIST3
	return rtn;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant PlayerLib_GetBookmarkSongInfo(int iCurrentPlayset, int ignored2, int* pPtr)
//
//   Type:          Function
//
//   Description:   This gets the bookmarked track for the current playset and advances to that track
//
//   Inputs:        current playset
//
//   Outputs:       RETCODE
//
//   Notes:
//<
//////////////////////////////////////////////////////


RETCODE _reentrant PlayerLib_GetBookmarkSongInfo(int iCurrentPlayset, int ignored2, int* pPtr){

	RETCODE rtn=PLAYERLIB_SUCCESS;
#ifndef USE_PLAYLIST3
#ifndef USE_PLAYLIST5
	INT iTracknum=0;

	switch(iCurrentPlayset)
	{
	case PLAYSET_MUSIC:
		iTracknum = g_MarkerMusic.m_iTracknum;
		break;
	case PLAYSET_VOICE:
		iTracknum = g_MarkerVoice.m_iTracknum;
		break;
	default:
		break;
	}

	rtn = PlayerLib_GotoTrack(iTracknum,0,0);

	g_bInitBookmarkActive = TRUE;		//to tell shuffle that bookmarks are initiliasing after rebuilding a playlist
	PlayerLib_GetCurrentSong(0,0,0);
	g_bInitBookmarkActive = FALSE;		//to tell shuffle that bookmarks initiliasing is over
#endif // #ifndef USE_PLAYLIST5
#endif // #ifndef USE_PLAYLIST3
	return rtn;

}


///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_SetSongName
//
//  Type:        Function
//
//  Description: TBD: THIS FUNCTION NOT YET IMPLEMENTED!!!!!
//
//  Inputs:      TBD.
//
//  Outputs:     TBD.
//
//  Notes:
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_SetSongName(int bStartPlaying, int ignored, SONGFILEINFO*pSongInfo)
{

/*    _packed char *pNewSong = pSongInfo->m_pFilename;
    char c;
    int i;
    //this could be changed to use a function packed_strncpy() if we had it...
    for(i=0;i<_MAX_FILENAME-1;i++)
    {
      c= packed_get(pNewSong,i);
      packed_set(g_CurrentSongName,i,c);
      if(!c)
        break;
    }
    packed_set(g_CurrentSongName,i,0);         // put the terminator in.
    if(bStartPlaying)
    {
      PlayerLib_SetState (DECODER_STOP,FALSE,NULL);
      PlayerLib_SetState (DECODER_PLAY,FALSE,NULL);
    }

*/
    return PLAYERLIB_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_SetSongPosition
//
//  Type:        Function
//
//  Description: Used for BOOKMARKS. Menu should call PlayerLibSetSongName followed
//               by PlayerLib_SetSongPosition to use the bookmark feature.
//               The input to this function is the DWORD byte position that should be
//               stored off when the user presses the bookmark button.
//
//  Inputs:      DWORD SongBytePosition
//  Outputs:
//
//  Notes:
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_SetSongPosition(DWORD dwSongPos)
{
    RETCODE rtn;

    rtn = DecSetSongPos(dwSongPos);               // set the song position specific byte number

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_GetSongPosition
//
//  Type:        Function
//
//  Description: Used for BOOKMARKS. Menu should call PlayerLib_GetSongPos to
//               get the current 48-bit byte position in the file. This is stored
//               off as the BOOKMARK.
//
//  Inputs:
//  Outputs:     DWORD SongBytePosition
//
//  Notes:
//<
///////////////////////////////////////////////////////////////////////////////
DWORD _reentrant PlayerLib_GetSongPosition(void)
{
    DWORD rtn;

    rtn = DecGetSongPos();                        // returns the song byte position (48 bits)

    return rtn;
}



///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_GetCurrentSong
//
//  Type:        Function
//
//  Description: This function should be used to get the important info about the
//               current song. It will get the device number, song name & path,
//               track number, and the meta-data.
//
//  Inputs:      None used.
//  Outputs:     Returns success or failures. Fills in memory locations mentioned above.
//
//  Notes:
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_GetCurrentSong(WORD wMode,int ignored1,int*ignored2)
{
#ifdef USE_PLAYLIST3
	RETCODE rtn = META_DATA_FOUND;
#else	// #ifdef USE_PLAYLIST3
    RETCODE rtn;
    _packed BYTE szFilename[MAX_FILENAME_LENGTH];
    SONGFILEINFO SongInfo;
    SongInfo.m_wBufferLength = MAX_FILENAME_LENGTH;
    SongInfo.m_pFilename = szFilename;

    // Use C Speed client Increase call to up the Dclock freq.
    // Defines are in SysSpeed.inc. Sysspeed.h is generated
    SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYERLIB);

    if(SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_GetCurrentSongFileInfo,0,0,(void*)&SongInfo)==PLAYLIST_SUCCESS)
    {
#ifdef USE_PLAYLIST2
      g_wCurrentDirDevId = SongInfo.m_wDeviceID;      // set device number
#endif
      packed_strcpy(g_CurrentSongName, szFilename);   // copy path and filename
#ifdef USE_PLAYLIST2
      g_wCurrentSongNumber = SongInfo.m_wTrack;       // set track number
#endif
      rtn = PlayerLib_GetMetaData(0,0,0);             // get the meta-data           RETURNS 0 on first call.

      // If an invalid (i.e., metadata doesn't match file extension) song is first in the list,
      // but we're here through the delete menu, allow it to be displayed for deletion.
      if((wMode == CURRENTSONG_DELETE_MENU) && (rtn == PLAYERLIB_ERROR))
      {
        rtn = META_DATA_FOUND; // = 0
      }
    }
    else
    {
        g_wSongTitle[0]=0;
        g_wSongArtist[0]=0;
        g_wSongAlbum[0]=0;
        rtn = PLAYERLIB_BAD_FILE;
        //  This path is followed when there is a bad file or
        //  there are no files on the media. When there are no files on the media,
        //  playerstatemachine will take care of resetting the Current Song Info.
    }

    // C Speed client frees the shared speed resource
    SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB);

    if (rtn == META_DATA_FOUND)
    {
      DecoderForceInit();                               // update decoder and force init
      SysPostMessage(2,DECODER_RESET);
    }

#endif	// #ifdef USE_PLAYLIST3
    return rtn;
}

#ifdef USE_PLAYLIST3
///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_GetCurrentSong_ML
//
//  Type:        Function
//
//  Description: This function should be used to get the important info about the
//               current song. It will get the device number, song name & path,
//               track number, and the meta-data.
//
//  Inputs:      None used.
//  Outputs:     Returns success or failures. Fills in memory locations mentioned above.
//
//  Notes: 	Add by PD
//<
///////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant PlayerLib_GetCurrentSong_ML(WORD wMode,int ignored,int*ignored2)
{
	RETCODE rtn;

	_packed BYTE *szFilename = DirPath;
	int i;

	// Use C Speed client Increase call to up the Dclock freq.
	// Defines are in SysSpeed.inc. Sysspeed.h is generated
	//    SysSpeedIncrease(SPEED_MAX, SPEED_CLIENT_PLAYERLIB);

	if ( (((int*)DirPath)[0] == 0x2f3a62) || (((int*)DirPath)[0] == 0x2f3a42))
		g_wCurrentDirDevId = 1;
	else
		g_wCurrentDirDevId = 0;

	//g_CurrentTrack.pFileEntry = (void*) SysCallFunction(RSRC_PLAYLIST_CODEBANK,Playlist_LocateEntryFromName,0,0,(INT*)DirPath);

	packed_strcpy(g_CurrentSongName, szFilename);   // copy path and filename
	//packed_strcpy(g_CurrentSongName, DirPath);   // copy path and filename

	rtn = PlayerLib_GetMetaData(0,0,0);             // get the meta-data           RETURNS 0 on first call.

	// C Speed client frees the shared speed resource
	//SysSpeedClockFree(SPEED_CLIENT_PLAYERLIB);

#ifndef USE_PLAYLIST3
	if (rtn == META_DATA_FOUND)
#else
	if (rtn == PLAYERLIB_SUCCESS)
#endif
	{
		DecoderForceInit();                               // update decoder and force init
		SysPostMessage(2,DECODER_RESET);
		SysWaitOnEvent(0,0,0);
#ifdef USE_PLAYLIST3
		DecSetSongPosZero();                       // reset byte positions to beginning of file
#endif
	}

	return rtn;
}
#endif

///////////////////////////////////////////////////////////////////////////////
//> Name:        PlayerLib_GetMetaData
//
//  Type:        Function
//
//  Description: This function is responsible for storing the information found
//               in the meta-data to the appropriate locations.
//
//  Inputs:      none used.
//
//  Outputs:     PLAYERLIB_SUCCESS if meta-data found
//               PLAYERLIB_ERROR otherwise
//
//  Notes:       Not complete. Need to add Table of Contents for Xing VBR mp3
//               songs, album name, etc.
//<
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PlayerLib_GetMetaData(WORD wMode,int ignored1,int*ignored2)
{
    RETCODE rtn = PLAYERLIB_ERROR;

    FILE_META_DATA MetaData;

#ifdef USE_PLAYLIST3
	if (g_Rebuild)
	{
#endif
    MetaData.wTitle=g_wSongTitle;
    MetaData.iTitleBufferLength=TITLE_SIZE;
    MetaData.wArtist=g_wSongArtist;
    MetaData.iArtistBufferLength=ARTIST_SIZE;
    MetaData.wAlbum=g_wSongAlbum;
    MetaData.iAlbumBufferLength=ALBUM_SIZE;
#ifdef USE_PLAYLIST3
		MetaData.wGenre=g_wSongGenre;
		MetaData.iGenreBufferLength=GENRE_SIZE;
		MetaData.wYear=g_wSongYear;
		MetaData.iYearBufferLength=YEAR_SIZE;
	}
	else
	{
		MetaData.wTitle=0;
		MetaData.iTitleBufferLength=0;
		MetaData.wArtist=0;
		MetaData.iArtistBufferLength=0;
		MetaData.wAlbum=0;
		MetaData.iAlbumBufferLength=0;
		MetaData.wGenre=0;
		MetaData.iGenreBufferLength=0;
		MetaData.wYear=0;
		MetaData.iYearBufferLength=0;
	}
#endif

#ifdef USE_PLAYLIST3
      g_wSongTrackNum = MetaData.wTrackNum = UNKNOWN_TRACK_NUMBER;  // track number is 12bit because in musiclib track number is 12bit.
#endif

#ifdef AUDIBLE
	// For Audible content, current minutes and seconds may be returned, so
	// initialize g_wSongCurrentMinutes and g_wSongCurrentSeconds here.
    g_wSongCurrentMinutes = (WORD) 0;
    g_wSongCurrentSeconds = (WORD) 0;
    MetaData.wArtist=g_wSongArtist;             //Stmp00013553
    MetaData.iArtistBufferLength=ARTIST_SIZE;   //Stmp00013553
#endif

    // stmp6855
    // On invalid file, (getfilemetadata returned META_DATA_NOT_FOUND), the code inside this if statement will be circumvented.
    // this can happen when the metadata does not match the filename extension. This function then returns PLAYERLIB_ERROR.

    if(SysCallFunction(RSRC_METADATA_CODEBANK,GetFileMetaData,(INT)(&g_CurrentSongName), (BYTE)g_wCurrentDirDevId, (void*)&MetaData) == SUCCESS)
    {
      g_wSongByteLengthLow = (WORD) (MetaData.dwNumBytes);
      g_wSongByteLengthHigh = (WORD) (MetaData.dwNumBytes>>24);
      g_wSongTotalMinutes = (WORD) (MetaData.iTime / 60);
      g_wSongTotalSeconds = (WORD) (MetaData.iTime - ((INT) g_wSongTotalMinutes * 60));
#ifndef	AUDIBLE
      g_wSongCurrentMinutes = (WORD) 0;
      g_wSongCurrentSeconds = (WORD) 0;
#endif
      g_wAccumulatedSecs = (WORD) 0;
      g_wAccumulatedMSecs = (WORD) 0;
      g_wBitRateFound = MetaData.wBitRate;
      g_iSongType = MetaData.iFileType;

#ifdef USE_PLAYLIST3
      if (MetaData.wTrackNum <= MAX_TRACK_NUMBER)
          g_wSongTrackNum = MetaData.wTrackNum;
#endif

#ifdef AUDIBLE
      // 11/1/2004 mmiu -	Temporary. Set temp global var with start position.
	  g_wSongStartOffsetHigh = (WORD) (MetaData.dwStartPos >> 24);
	  g_wSongStartOffsetLow  = (WORD)  MetaData.dwStartPos;
#endif

      // next line calculates a normalized (by 2^18) value of seconds per byte
      // which is used by WMA decoder overlay to calculate the current elapsed play time
      if (MetaData.dwDataBytes)
        g_frSongSecondsPerByte =(((float)MetaData.iTime * 512.0)/(float)MetaData.dwDataBytes);
      else
        g_frSongSecondsPerByte = 0;

      if (MetaData.iResourceNum != -1)                       // iResourceNum = -1 if file type not supported
        SysSetDecoderResource(MetaData.iResourceNum);        // load the correct resource ptr into ModuleTableDecoder

      //by Sarah, for the 0 byte file deadloop in REPEAT_1/REPEAT_ALL mode, solving the ticket #68703
      //Correction on Mar-21-2005, correct the the compiled bit operation.
      g_wDecoderCSR &= ~(WORD)(0x00000|DECODER_BAD_FILE);	//clear the current song as a bad song

	  // By Sarah on Mar-09-2005, for song shorter than the min. skip size, solving the ticket #71685
	  if ((g_wSongTotalMinutes <= 0) && (g_wSongTotalSeconds <= SHORT_SONG_TIME))
      	bCurrentSongShort = TRUE;
      else
      	bCurrentSongShort = FALSE;

      rtn = PLAYERLIB_SUCCESS;

#ifdef WOW
    SysCallFunction(RSRC_WOW_MENU_CODE_BANK,WowSetProperties,0,0,(INT*)&MetaData);
#endif
#ifdef AUDIBLE
	  if( (DecoderResourcePtr == RSRC_AUDIBLE_ACELPDEC_CODE) || (DecoderResourcePtr == RSRC_AUDIBLE_DECMOD_CODE) ) {
		  // For audible files, tell UI to update chapter info after retreveing metadata
		  SysPostMessage(2, MENU_UPDATE_CHAPTER_INFO);
		  SysPostMessage(2, MENU_SONG_TIME_CHNG);
	  }
#ifdef USE_PLAYLIST3
      if(g_AudibleFlags.bits.IsAudibleFile == TRUE) //Stmp00013553
      {   TrimUnicodeString(g_wSongArtist);
          strcpy(song_info.artist_name, g_wSongArtist);
      }
#endif
#endif
    }
    //for the 0 byte file deadloop in REPEAT_1/REPEAT_ALL mode, solving the ticket #68703
    else
	{		//reset all the META DATA
	      g_wSongByteLengthLow = (WORD) 0;
	      g_wSongByteLengthHigh = (WORD) 0;
	      g_wSongTotalMinutes = (WORD) 0;
	      g_wSongTotalSeconds = (WORD) 0;
#ifndef	AUDIBLE
	      g_wSongCurrentMinutes = (WORD) 0;
	      g_wSongCurrentSeconds = (WORD) 0;
#endif
	      g_wAccumulatedSecs = (WORD) 0;
          g_wAccumulatedMSecs = (WORD) 0;
	      g_wBitRateFound = 0;
	      g_iSongType = 0;
          g_frSongSecondsPerByte = 0;
#ifdef AUDIBLE
    	  g_wSongStartOffsetHigh = (WORD) 0;
    	  g_wSongStartOffsetLow  = (WORD) 0;
#endif

          g_wDecoderCSR |= DECODER_BAD_FILE;	//set the current song as a bad song
	  	  bCurrentSongShort = TRUE;		// By Sarah on Mar-09-2005, for song shorter than the min. skip size, solving the ticket #71685
    }

    return rtn;
}