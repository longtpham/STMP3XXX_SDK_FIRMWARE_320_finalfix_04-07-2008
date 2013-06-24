#ifndef _PLAYLIST3_H
#define _PLAYLIST3_H
#include "playlist.h"
#include "playlist3internal.h"
#include "musiclib_ghdr.h"
#include "fsapi.h"
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

#define TYPE_DIR 0
#define TYPE_FILE 1

#define IS_TRASH	0
#define IS_VOICE_DIR 1
#define IS_VALID_AUDIO 2
#define IS_VOICE_FILE  3

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

#define VOICE_PATH_0	0x2f3a61	// a:/ in dyslexical order
#define VOICE_PATH_1	0x494f56	// VOI in dyslexical order
#define VOICE_PATH_2	0x2f4543	// CE/ in dyslexical order
#define VOICE_PATH_3	0x000000	// terminating 0

#define FM_PATH_0		0x2f3a61	// a:/ in dyslexical order
#define FM_PATH_1		0x2f4d46	// FM/ in dyslexical order
#define FM_PATH_2		0x000000	// terminating 0

#define LINEIN_PATH_0	0x2f3a61	// a:/ in dyslexical order
#define LINEIN_PATH_1	0x4e494c	// LIN in dyslexical order
#define LINEIN_PATH_2	0x492d45	// E-I in dyslexical order
#define LINEIN_PATH_3	0x002f4e	// N/  in dyslexical order

#define NAME_SFN	0
#define NAME_LFN	1
//Error code for unsupported file Names or Extensions
#define META_DATA_FILE_NOT_SUPPORTED        (WORD)(0x101)

#define FINDDATA_CACHE_SIZE   20

// this number is used to skip mac resource fork files when loading the playlist
// the resource SFN start with _ end with the same extension (i.e. mp3)
// a SMALL sample of test files shows that the resource file is typically 82 bytes.
#define MAC_RESOURCE_NUM_BYTES   512

extern int g_iPlaylistRepeat;
extern int g_bPlaylistShuffle;
extern DWORD dStart, dEnd, dDiff;	//for timing measurements
extern _packed BYTE DirPath[MAX_DIRNAME_LENGTH];
extern BOOL g_Rebuild;
extern RAM_SONG_INFO_T  song_info;
extern INT  g_file_time;
extern INT  g_unicode;
extern DWORD    g_dwFastKey;
extern INT  g_iRecordNum;
extern DWORD    g_FileKey;
extern DIR_DATE g_dirdate;
extern DIR_TIME g_dirtime;
extern INT  *pHighestNumber;
extern INT  g_iHighestVoiceNumber;
extern INT  g_iHighestFMNumber;
extern INT  g_iHighestLineNumber;

INT _reentrant Playlist_GetPlaySet(void);
INT _reentrant Playlist_SetPlaySet(INT, INT, INT*);
INT _reentrant Playlist_Initialize(INT, INT, INT*);
INT _reentrant Playlist_BuildMusicLib(INT iIgnored1, INT iIgnored2, INT* pIgnored);
_reentrant RETCODE Playlist_PopulateMusicLib(int iDevice, int iDepth, DWORD dwDirKey,INT RecordNumber);
_reentrant INT Playlist_SetPlaySet(INT , INT, INT*);
void _reentrant Playlist_ResetPlayset(void);
BOOL _reentrant Playlist_ValidateEntry(INT iEntryType,INT iReason, INT* pPtr);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/////playlist3 helper functions
//////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant Playlist_IsValidAudioFile(LONG lFileSize, _packed BYTE* pFileName);
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
_reentrant RETCODE Playlist_GetSFN(INT iEntryType , INT pEntry, INT* pName);
_reentrant RETCODE Playlist_GetLFN(INT iEntryType , INT pEntry, INT* pName);
INT _reentrant PathFormation(_packed BYTE* dst, _packed BYTE* src, int iDepth);

DWORD GetDclkCount(void);

_reentrant INT CopyFindFirst(int mDirEntry, int mfinddata, int *mFileSpec);
_reentrant INT CopyFindNext(INT HandleNumber,int mfinddata, int* ptr);

_reentrant void BuildVoiceFilePath(void);
_reentrant void BuildFMFilePath(void);
_reentrant void BuildLINEINFilePath(void);
_reentrant void GetVoiceFastKey(WORD wFastKeyBitField);
_reentrant INT AddSongToLibrary(int a, int drive, void* b);
#endif
