#ifndef __PLAYLIST1INTERNAL_H
#define __PLAYLIST1INTERNAL_H

#include "project.h"

#include "filesystem.h"

#ifndef PLAYLIST_MAX_FILES
#define PLAYLIST_MAX_FILES 256
#endif

//MAX_DIR_DEPTH is going to be the (MAX_FILENAME_LENGTH-1)/13 ((MAXLENGTH-1)/8+.+3+/ or null)
#define  _MAX_DIR_DEPTH ((MAX_FILENAME_LENGTH-1)/13)

#define FILE_ENTRY_UNUSED -1
#define FILE_ENTRY_DIR    0

struct FileEntry
{
    int                 m_iDecoder:16;//resource # of decoder, or FILE_ENTRY_DIR if its a directory, or FILE_ENTRY_UNUSED if not used
    int                 m_iDevice:4;
    int                 m_iMode:4;
    _packed BYTE        m_pszFilename[8+1+3+1];//8+3+dot+terminator
    struct FileEntry    *m_pNext;//points to the next in the directory
    struct FileEntry    *m_pContents;//points to the first in the directory contents.
    struct FileEntry    *m_pContainer;//used for creating the fully qualified filenames
	int m_iFCBEntry;
};


//these are special file entries for the roots of the logical devices.
extern struct FileEntry g_FileEntryPerDevice[MAX_LOGICAL_DEVICES];

//this is a pool for all the file entries
extern struct FileEntry g_FileEntryPool[];

//This contains a list of pointers to file entry's in the g_FileEntryPool
extern struct FileEntry *g_PlayList[];

//This contains a list of indexes to the g_PlayList Array
extern int    g_iInternalList[PLAYLIST_MAX_FILES];

extern int g_iPlaylistRepeat;
extern BOOL g_bPlaylistShuffle;

extern int g_iInternalTrack;
extern int g_iTotalTracks;

extern int g_iPlaySet;


#endif