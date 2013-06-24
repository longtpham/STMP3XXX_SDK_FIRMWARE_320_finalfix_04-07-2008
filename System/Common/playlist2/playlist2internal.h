#ifndef __PLAYLIST2INTERNAL_H
#define __PLAYLIST2INTERNAL_H

#include "project.h"

#include "filesystem.h"

#ifdef  SMALL_PLAYLIST
#define  PLAYLIST_SIZE   300
#else
#define  PLAYLIST_SIZE   380
#endif

#define MAX_NUM_DIR     150     //max supported directories including all subdirectories

#define MAX_NUM_FILES   PLAYLIST_SIZE // largest build 338    //235 max supported files     reduced in 2.6 to 3.0 merge (to locate DRM JANUS LIION build). TOVERIFY

#define MAX_SORT_ENTRIES_PER_DIRECTORY	300		//max entries (files + directories) per directory that will be sorted. Review: Should this have been dirs+files? TOVERIFY
											//Entries more than this number will be inserted towards the end of the sorted list

#define MAX_RECORDS_PER_DIRECTORY   4096	//calculated from the bits in the m_iRecordNum field in the following data structures.
											//An entry can have multiple records in the FAT for LFNs.
											//Records more than this number will not be included in the playlist content

#define SFN_LENGTH	13	//8.3\ + NULL

#define  _MAX_DIR_DEPTH ((MAX_FILENAME_LENGTH-1)/SFN_LENGTH)

struct Bookmark{
	INT	  m_iTracknum;
	DWORD m_dwTrackPosBytes;
}; //this will save the num and position of the song selected for bookmark


typedef struct{
    unsigned int    m_pNext:16;			//points to the next inline DirEntry. 
	unsigned int	m_bAllocated:1;	//1=allocated, 0=free.
    unsigned int    m_iDevice:2;
	unsigned int	m_bIsRoot:1;	//1=ROOT directory, 0= Any directory except ROOT.
////////////////////////////////////////////////////////////////////////////////////////////
    unsigned int    m_pDirContents:16;	//points to the first content which is a dir.
///////////////////////////////////////////////////////////////////////////////////////////
	unsigned int    m_pFileContents:16; //pointer in FileEntryPool where the file contents for this directory begin.
////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int	m_iDirSector1:21;
////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int	m_iDirOffset:12;
	unsigned int	m_iRecordNum:12;	//record number of the directory record (LFN use).

	unsigned int    m_iDirSector2:11;
	//!! The above are implicitly used as a struct*. Using INT bitfields to save memory !!!
////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_SFN
	_packed BYTE name[SFN_LENGTH];
#endif
}DirEntry;

typedef struct{
	unsigned int	m_bAllocated:1;	//1=allocated, 0=free.
	unsigned int	m_bCurrentPlayset:1;	//1=Belongs to the current playset.
	unsigned int	m_bTrackPlayed:1;		//Indicates if the track was played atleast once. Currently not used.
	unsigned int	m_iDirSector1:21;
//////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int	m_iTrackOrder:10;		//Order of this track in the current playset.
	unsigned int	m_iDirOffset:12;
	unsigned int	m_iDevice:2;
//////////////////////////////////////////////////////////////////////////////////////////////////////
	unsigned int	m_iRecordNum:12;	//Record number of the file record (LFN use).
	unsigned int	m_iDirSector2:11;
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PLAYLIST2_MULTIMEDIA
	unsigned int    m_iMediaType:2;   // Indicate it is media type
#endif // PLAYLIST2_MULTIMEDIA
	unsigned int	m_pNext:16;		// //points to the next inline file.
//////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG_SFN
	_packed BYTE name[SFN_LENGTH];
#endif
}FileEntry;

typedef struct{
	DirEntry _X* pDirEntry;	//Start traversing from this directory entry
	unsigned int	iReason;
	unsigned int iDevice;
	unsigned int iPlayset;
	unsigned int iDepth;
	unsigned int iTrackOrder;
	unsigned int iTrackNum;
	FileEntry* pFileEntry;
	DirEntry _X* DirLinks[_MAX_DIR_DEPTH];	//maintains a dynamic list of directory links within the recursion
	unsigned int	iTotalLinks;
}TraverseTreeParams;

typedef struct{
	unsigned int EntryType;				//TYPE_DIR or TYPE_FILE
	FileEntry* pFileEntry;		//used if TYPE_FILE
	DirEntry _X* pDirEntry;			//used if TYPE_DIR
	unsigned int iTrackNum;
	unsigned int iTrackOrder;
	DirEntry _X* DirLinks[_MAX_DIR_DEPTH];
	unsigned int iTotalLinks;
}EntryAccessInfo;

typedef struct{
	void* pEntry;
	_packed BYTE name[SFN_LENGTH];
}SFNStorage;

extern DirEntry _X g_DirEntryPerDevice[];	//ROOT(s)
extern DirEntry _X g_DirEntryPool[];		//Pool of directory entries, Does not contain files.
extern FileEntry g_FileEntryPool[];
extern EntryAccessInfo g_CurrentTrack;
extern int g_iPlaylistRepeat;
extern int g_bPlaylistShuffle;
extern SFNStorage g_TempSFNSpace[];
extern int g_iTotalTracks;

extern int g_iPlaySet;

extern struct Bookmark g_MarkerMusic, g_MarkerVoice;
#ifdef PLAYLIST2_MULTIMEDIA
extern struct Bookmark g_MarkerMVideo, g_MarkerPhoto;
#endif
extern int g_iTotalDir;
extern int g_iTotalFiles;

#endif
