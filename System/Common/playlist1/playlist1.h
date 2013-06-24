#ifndef _PLAYLIST1_H
#define _PLAYLIST1_H

extern int g_iPlaylistRepeat;
extern BOOL g_bPlaylistShuffle;

#define PLAYSET_VOICE      0
#define PLAYSET_MUSIC      1

#define PLAYLIST_REPEAT_OFF     0
#define PLAYLIST_REPEAT_ALL     1
#define PLAYLIST_REPEAT_ONE     2

// this number is used to skip mac resource fork files when loading the playlist
// the resource SFN start with _ end with the same extension (i.e. mp3)
// a SMALL sample of test files shows that the resource file is typically 82 bytes.
#define MAC_RESOURCE_NUM_BYTES   512 

INT _reentrant Playlist_GetPlaySet(void);
INT _reentrant Playlist_SetPlaySet(INT, INT, INT*);
RETCODE _reentrant Playlist_Initialize(INT, INT, INT*);
void _reentrant Playlist_ShuffleList(INT, INT, INT*);

struct Bookmark{
	INT	  m_iTracknum;
	DWORD m_dwTrackPosBytes;
}; //this will save the num and position of the song selected for bookmark
extern struct Bookmark g_MarkerMusic, g_MarkerVoice;


#endif