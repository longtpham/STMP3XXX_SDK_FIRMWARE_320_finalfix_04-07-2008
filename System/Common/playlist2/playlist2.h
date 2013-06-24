#ifndef _PLAYLIST2_H
#define _PLAYLIST2_H
#include "playlist.h"
#include "playlist2internal.h"
////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////
//Traversal return types
#define	PLAYLIST_END_TRAVERSAL_SUCCESS	(PLAYLIST_LAST_RETCODE + 1)
#define	PLAYLIST_END_TRAVERSAL_FAILURE	(PLAYLIST_LAST_RETCODE + 2)
#define	PLAYLIST_FILE_LOCATE_SUCCESS	(PLAYLIST_LAST_RETCODE + 3)

#define PLAYSET_MUSIC 0
#define PLAYSET_VOICE 1
#define PLAYSET_FOLDER_PLAY 2
#define PLAYSET_FAVORITES 3
#define PLAYSET_PHOTO 4
#define PLAYSET_MVIDEO	5

#define PLAYLIST_REPEAT_OFF     0
#define PLAYLIST_REPEAT_ALL     1
#define PLAYLIST_REPEAT_ONE     2

#define SELECT_TRACKS	0
#define ORDER_TRACKS	1
#define BUILD_FILE_LINKS	2
#define RESTORE_BOOKMARK 3
	//	SGTL-HK 28-10-2004
#define BUILD_DIR_LINKS		4


#define ATTR_UNACCEPTABLE  (ATTR_HIDDEN|ATTR_SYSTEM|ATTR_VOLUME_ID)

#define DIR_SEPARATOR	0x00002f	// "\"
#define ROOT_SEPARATOR	0x002f3A	// ":\" swizzled

#define DEPTH_VOICE_DIR	1	//depth of voice directory
#define DEPTH_PHOTO_DIR	1	//depth of voice directory
#define DEPTH_MVIDEO_DIR	1	//depth of motion video directory

#define TYPE_DIR 0
#define TYPE_FILE 1

#define IS_TRASH	0
#define IS_VOICE_DIR 1
#define IS_VALID_AUDIO 2
#define IS_VOICE_FILE  3
#define IS_VALID_PHOTO 4
#define IS_VALID_MVIDEO	5

//List containing audio file extensions
#define WMA_FILE_EXT	 0x414D57
#define MP3_FILE_EXT	 0x33504d
#define WAV_FILE_EXT	 0x564157
#define MP4_FILE_EXT     0x34504d
#define M4A_FILE_EXT     0x41344d
#define ASF_FILE_EXT	 0x465341
#define AUDIBLE_FILE_EXT	 0x004141
#define JPG_FILE_EXT	 0x47504a
#define BMP_FILE_EXT     0x504d42
#define SMV_FILE_EXT     0x564d53

#define ENTRY_IS_AUDIO	0
#define ENTRY_IS_PHOTO	1
#define ENTRY_IS_MVIDEO	2

#define NAME_SFN	0
#define NAME_LFN	1
//Error code for unsupported file Names or Extensions
#define META_DATA_FILE_NOT_SUPPORTED        (WORD)(0x101)

// this number is used to skip mac resource fork files when loading the playlist
// the resource SFN start with _ end with the same extension (i.e. mp3)
// a SMALL sample of test files shows that the resource file is typically 82 bytes.
#define MAC_RESOURCE_NUM_BYTES   512 

extern int g_iPlaylistRepeat;
extern int g_bPlaylistShuffle;
extern DWORD dStart, dEnd, dDiff;	//for timing measurements
INT _reentrant Playlist_GetPlaySet(void);
INT _reentrant Playlist_SetPlaySet(INT, INT, INT*);
INT _reentrant Playlist_Initialize(INT, INT, INT*);
INT _reentrant Playlist_BuildContentTree(INT iIgnored1, INT iIgnored2, INT* pIgnored);
_reentrant RETCODE Playlist_PopulateDirectory(DirEntry _X *pParentDirEntry, int depth);
_reentrant DirEntry _X* Playlist_GetFreeDirEntry(void);
_reentrant FileEntry* Playlist_GetFreeFileEntry(void);
_reentrant INT Playlist_SetPlaySet(INT , INT, INT*);
_reentrant DirEntry _X* Playlist_FindDirEntry(DirEntry _X* pDirEntry, FileEntry* pFileEntry);
FileEntry* _reentrant Playlist_GetFileEntry(INT iTrackOrder);
INT _reentrant Playlist_GetDirEntry(INT iUnused1, INT iUnused2, INT *pCurrentTrack);		//	Modified By SGTL-HK 27-10-2004
void _reentrant Playlist_ResetPlayset(void);
void _reentrant Playlist_SelectTracks(INT a, INT b, INT* pPtr);
void _reentrant Playlist_AssignTrackOrder(INT a, INT b, INT* pPtr);
_reentrant INT Playlist_ShuffleList(INT bTrueFalse, INT iIgnored, INT *pIgnored);
void _reentrant Playlist_UpdateShuffle(void);
RETCODE _reentrant Playlist_TraverseFromRoot(INT iReason, INT iPlayset, INT* pPtr);
RETCODE _reentrant Playlist_TraverseDir(INT a,INT b, INT* pPtr);
BOOL _reentrant Playlist_ValidateEntry(INT iEntryType,INT iReason, INT* pPtr);
RETCODE _reentrant Playlist_LocateFileEntry(INT a, INT b, INT* pPtr);
RETCODE _reentrant Playlist_LocateDirEntry(INT a, INT b, INT* pPtr);			//	SGTL-HK 27-10-2004
RETCODE _reentrant Playlist_FindBookMarkedSong(INT, INT, INT*);
void _reentrant Playlist_RestoreBookmark(INT, INT, INT*);
RETCODE _reentrant Playlist_BuildFileLinks(FileEntry* pFileEntry,TraverseTreeParams* TraverseParams);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////playlist2 helper functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant Playlist_IsValidAudioFile(LONG lFileSize, _packed BYTE* pFileName);
#ifdef PLAYLIST2_MULTIMEDIA									
BOOL _reentrant Playlist_IsValidPhotoFile(LONG lFileSize, _packed BYTE* pFileName);
BOOL _reentrant Playlist_IsValidMotionVideoFile(LONG lFileSize, _packed BYTE* pFileName);
#endif
_reentrant INT Playlist_GetFileExtention(INT iIgnored1, INT iIgnored2, INT* pPtr);
_reentrant BOOL IsTrashDir(_packed BYTE* pszName);
_reentrant DWORD Playlist_GenerateFastKey(INT iDevice,LONG DirSector,INT iDirOffset);
_reentrant INT ExtractDirSector1(DWORD Key);
_reentrant INT ExtractDirSector2(DWORD Key);
_reentrant INT ExtractDirOffset(DWORD Key);
_reentrant RETCODE Playlist_GetRootString(_packed BYTE* pBuffer, INT iDevice);
_reentrant RETCODE Playlist_LocateFileEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr);
_reentrant INT Playlist_LocateDirEntryFromName(INT iUnused1, INT iUnused2, INT* pPtr);
_reentrant BOOL IsVoiceDir(DirEntry _X* pDirEntry);
#ifdef PLAYLIST2_MULTIMEDIA									
_reentrant BOOL IsPhotoDir(DirEntry _X* pDirEntry);
_reentrant BOOL IsMotionVideoDir(DirEntry _X* pDirEntry);
#endif
_reentrant RETCODE Playlist_GetSFN(INT iEntryType , INT pEntry, INT* pName);
_reentrant RETCODE Playlist_GetLFN(INT iEntryType , INT pEntry, INT* pName);
_reentrant INT ShuffleTrackOrder( INT iIgnored1, INT iIgnored2, INT* pIgnored);
_reentrant RETCODE DeAllocateEmptyDir(DirEntry _X *pCurrentDirEntry, DirEntry _X* pPrevDirEntry, DirEntry _X* pParentDirEntry);
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///////playlist2 sort functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////
_reentrant void Playlist_InsertSortFile(DirEntry _X* pParentDirEntry,FileEntry* pNewDirEntry, _packed BYTE* pNewFileName);
_reentrant void Playlist_InsertSortDir(DirEntry _X* pParentDirEntry,DirEntry _X* pNewDirEntry, _packed BYTE* pNewFileName);
_reentrant void ClearSFNStorageSpace(INT iUnused1, INT iUnused2, INT* pPtr);
_reentrant RETCODE GetSFNFromTempArray(void* pEntry, _packed BYTE* pName);
_reentrant void StoreSFNInTempArray(void* pEntry, _packed BYTE* pName);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_PLAYLIST2
DWORD GetDclkCount_PL2(void);
#endif

#ifdef USE_PLAYLIST3
DWORD GetDclkCount(void);
#endif

_reentrant INT CopyFindFirst(int mDirEntry, int mfinddata, int *mFileSpec);
_reentrant INT CopyFindNext(INT HandleNumber,int mfinddata, int* ptr);
#endif	