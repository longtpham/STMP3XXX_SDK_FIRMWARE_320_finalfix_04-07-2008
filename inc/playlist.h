#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "types.h"

typedef struct {
    WORD    m_wTrack;                       
    WORD    m_wDeviceID;
    WORD    m_wBufferLength;            //this is in BYTES, not WORDS!
    _packed BYTE *m_pFilename;
} SONGFILEINFO;

#define PLAYLIST_SUCCESS				0
#define PLAYLIST_FILE_ERROR				1
#define PLAYLIST_END_OF_LIST			2
#define PLAYLIST_TRACK_NOT_FOUND		3
#define PLAYLIST_DIR_NOT_EMPTY          4 
#define PLAYLIST_REBUILD                5
#define PLAYLIST_LAST_RETCODE			5		//The value should always be the same as the last RETCODE


#ifdef  USE_PLAYLIST1
RETCODE ChangeDIRtoFileEntryDir(struct FileEntry *pCurrentEntry, _packed BYTE * buffer);
#endif
RETCODE _reentrant Playlist_LFNGetFileName(int,int,UCS3*);			//UCS3 is actually typdef WORD
RETCODE _reentrant Playlist_MarkCurrentSongPlayed(int,int,int*);
RETCODE _reentrant Playlist_GetCurrentSongFileInfo(int, int, int*);
RETCODE _reentrant Playlist_GetNextSongFileInfo(int, int , int*);
RETCODE _reentrant Playlist_GetPreviousSongFileInfo(int,int,int*);
#ifdef USE_PLAYLIST1
_reentrant INT Playlist_LocateEntryFromName(INT,INT,INT*);
#endif

#endif 
