////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Sigmatel Inc 2005
// Filename: playlist3.c
//
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "menumanager.h"
#include "playlist.h"       //required include file
#include "playerlib.h"
#include "project.h"
#include "stringlib.h"
#include "resource.h"
#include "sysspeed.h"
#include "playlist3.h"
#include "playlist3internal.h"
#include "sysmem.h"
#include "FileSystem.h"
#include "fserror.h"
#include "FileSpec.h"
#include "playlist.h"
#include "syscoder.h"
//#include "..\..\msgmodules\software\musiclib\ghdr\musiclib_ghdr.h"
#include "musiclib_ghdr.h"

///////////////Extern FAT32 functions/////////////////////////////////
extern _reentrant INT Chdir(_packed char *filepath);
extern _reentrant INT FindFirst(Finddata *_finddata,_packed char *FileName);
extern _reentrant INT FindNext(INT HandleNumber,Finddata *_finddata);
extern _reentrant INT Fclose(INT HandleNumber);
extern _reentrant INT GetShortfilename(LONG Key,INT *Buffer);
extern _reentrant LONG GetRootdirkey(INT DeviceNumber);
extern _reentrant INT FastOpen(LONG Key,_packed char *mode);
extern _reentrant INT ConstructLongFileName(INT HandleNumber, INT RecordNumber, INT *LFNBuffer);
extern _reentrant INT GetCWDHandle(void);
extern INT DriveLetter[];
// for building longfilename
extern DWORD g_dwFastKey;
extern int g_iRecordNum;
extern _packed BYTE DirPath[];
extern _packed BYTE g_CurrentSongName[MAX_FILENAME_LENGTH];
extern DWORD g_FileKey;
                        

//////////////////////////////////////////////////////////////////////
int g_iPlaylistRepeat = PLAYLIST_REPEAT_OFF;
int g_bPlaylistShuffle = FALSE;
int g_bPlaylistShuffleUpdate = FALSE;
int g_iTotalTracks=0; 	//total number of songs in current playset.
int g_iPlaySet=PLAYSET_MUSIC;
int ML_UseCache = 0;
_packed BYTE g_shortname[MAX_FILESNAME];
extern RAM_SONG_INFO_T song_info;

DWORD dStart, dEnd, dDiff;	//for timing measurements
//////////////////////////////////////////////////////
//
//>  Name:          INT _reentrant Playlist_SetPlaySet(INT iPlayset, INT unused, INT *pUnused)
//
//   Type:          Function
//
//   Description:   Returns whatever is input by the user
//
//   Inputs:        INT     iPlayset    The new playset
//		    INT	    bResetBookmark - TRUE when stop is pressed twice to reset the bookmark
//
//   Outputs:       INT     iPlayset
//
//   Notes:         This function is degenerated in playlist3
//<
//////////////////////////////////////////////////////
INT _reentrant Playlist_SetPlaySet(INT iPlayset, INT bResetBookmark, INT *pUnused)
//////////////////////////////////////////////////////
{
	return iPlayset;
}

//////////////////////////////////////////////////////
//
//>  Name:          void _reentrant Playlist_ResetPlayset(void)
//
//   Type:          Function
//
//   Description:   Shuffles the play queue
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:	    Do nothing in playlist3
//<
//////////////////////////////////////////////////////
void _reentrant Playlist_ResetPlayset(){
	return;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_MarkCurrentSongPlayed(int iIgnored1,int iIgnored2,int *pIgnored)
//
//   Type:          Function
//
//   Description:   required function that marks the current song played
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:	    This function is degenerated in playlist3
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_MarkCurrentSongPlayed(int iIgnored1, int iIgnored2, int *pIgnored)
{
    return PLAYLIST_SUCCESS;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iUnused, SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename
//
//   Outputs:       none
//
//   Notes:         This function is degenerated in playlist3
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iUnused, INT*pPtr)
{
	return PLAYLIST_SUCCESS;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetCurrentSongFileInfo(int,int,SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename
//
//   Outputs:       none
//
//   Notes:         This function is degenerated in playlist3
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetCurrentSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
{
	return PLAYLIST_SUCCESS;
}

//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_GetPreviousSongFileInfo(int,int,SONGFILEINFO*SongInfo)
//
//   Type:          Function
//
//   Description:   Gets the song file info  (this is a required entry point)
//
//   Inputs:        SONGFILEINFO *      SongInfo        the pointer to the structure passed in by the PlayerLibrary.
//                                                      the PlayerLibrary must initialize some parts of the structure:
//                                                      m_wBufferLength and m_pFilename
//
//   Outputs:       none
//
//   Notes:         This function is degenerated in playlist3
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_GetPreviousSongFileInfo(INT iUnused1, INT iUnused2, INT *pPtr)
{
	return PLAYLIST_SUCCESS;
}

_reentrant RETCODE Playlist_GetLFN(INT iEntryType,INT pEntry,INT* pName){
	_packed BYTE sFileSpec[SFN_LENGTH];
	Finddata finddata;
	int find_handle ;
	int iDirHandle;
	int i;
	WORD iStrLen;
	RETCODE	rtn = PLAYLIST_TRACK_NOT_FOUND;

	sFileSpec[0] = 0;
	packed_strcpy((_packed BYTE *)pName,g_CurrentSongName);
	iStrLen= packed_strlen((_packed BYTE *)pName);
	while(iStrLen--)
	{
	    if(packed_get((_packed BYTE *)pName,iStrLen)=='/')
		{
	        for (i = 0; i < SFN_LENGTH; i++)
				packed_set(sFileSpec,i,packed_get((_packed BYTE *)pName,iStrLen+1+i));
			packed_set((_packed BYTE *)pName,iStrLen+1,0);
			break;
		}
	}

	Chdir((_packed char *)pName);
	iDirHandle = GetCWDHandle();
	_memset(&finddata,0,sizeof(finddata));
	finddata.device = 0;
	finddata.startrecord = 0;//start at the 0th record.
	find_handle = FindFirst(&finddata,(_packed char*) sFileSpec);
	if( find_handle > 0 )
	{
		if (ConstructLongFileName(iDirHandle, finddata.startrecord - 1, pName ) > 0)
			rtn = PLAYLIST_SUCCESS;
		Fclose(iDirHandle);
		Fclose(find_handle);
	}

	return rtn;
}
_reentrant RETCODE Fast_Playlist_GetLFN(INT iEntryType,INT pEntry,INT* pName){
	_packed BYTE sFileSpec[SFN_LENGTH];
	Finddata finddata;
	int find_handle ;
	int iDirHandle;
	int i;
	WORD iStrLen;
	RETCODE	rtn = PLAYLIST_TRACK_NOT_FOUND;

	sFileSpec[0] = 0;
	packed_strcpy((_packed BYTE *)pName,g_CurrentSongName);
	iStrLen= packed_strlen((_packed BYTE *)pName);
	while(iStrLen--)
	{
	    if(packed_get((_packed BYTE *)pName,iStrLen)=='/')
		{
	        for (i = 0; i < SFN_LENGTH; i++)
				packed_set(sFileSpec,i,packed_get((_packed BYTE *)pName,iStrLen+1+i));
			packed_set((_packed BYTE *)pName,iStrLen+1,0);
			break;
		}
	}

	Chdir((_packed char *)pName);
	iDirHandle = GetCWDHandle();
	_memset(&finddata,0,sizeof(finddata));
	finddata.device = 0;
	finddata.startrecord = 0;//start at the 0th record.
	find_handle = FindFirst(&finddata,(_packed char*) sFileSpec);
	if( find_handle > 0 )
	{
		g_FileKey = finddata.Key;
		song_info.g_songFastKey=g_FileKey;
        packed_strcpy((_packed BYTE *)g_shortname, (_packed BYTE *)finddata.name);

		if (ConstructLongFileName(iDirHandle, finddata.startrecord - 1, pName ) > 0)
			rtn = PLAYLIST_SUCCESS;
		Fclose(iDirHandle);
		Fclose(find_handle);
	}

	return rtn;
}

_reentrant INT Playlist_LocateEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr){
	return 0;
}
_reentrant INT Playlist_LocateDirEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr){
	return 0;
}

_reentrant RETCODE Playlist_LocateFileEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr){
	return 0;
}
//////////////////////////////////////////////////////
//
//>  Name:          RETCODE _reentrant Playlist_Initialize(void)
//
//   Type:          Function
//
//   Description:
//
//   Inputs:        none
//
//   Outputs:       Music library rebuilt and PLAYLIST_SUCCESS
//
//   Notes:         This should ideally be in playlist3init.c. But, leaving it here
//			since other codebanks call this using RSRC_PLAYLIST_CODEBANK
//<
//////////////////////////////////////////////////////
RETCODE _reentrant Playlist_Initialize(int a, int b, int *c)
{
	a;b;c;
	g_iTotalFiles = 0;	//debug purposes;
	g_iTotalDir = 0;

	// Set up the environment for rebuilding music library on Flash and SD, if available
	SysCallFunction(RSRC_PLAYLIST2INIT_CODEBANK,Playlist_BuildMusicLib,0,0,0);

	return PLAYLIST_SUCCESS;
}
