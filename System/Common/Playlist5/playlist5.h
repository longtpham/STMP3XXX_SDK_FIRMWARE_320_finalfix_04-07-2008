#ifndef _PLAYLIST5_H
#define _PLAYLIST5_H

#include "types.h"
#include "filespec.h"
#include "filesystem.h"
#include "playlist5internal.h"
#include "mainmenu.h"

// DEFINITIONS

#define SFN_LENGTH	13	//8.3\ + NULL
#define LFN_LENGTH  256
#define MAX_DIR_DEPTH   8	// Max dir depth which playback and browse can reach to. It should be <= 12.
#define MAX_RECORDS_PER_DIRECTORY   4096    //calculated from the bits in the m_iRecordNum field in the following data structures.
											//An entry can have multiple records in the FAT for LFNs.
											//Records more than this number will not be included in the playlist content

#define PL5_BR_PAGE_SIZE	                4       // total line number in a browse view
#define PL5_BR_MARGIN                       4       // must be >= 1. If browse PAGE_BY_PAGE, recommend this value to be at least (PL5_BR_PAGE_SIZE + 1).
#define PL5_BR_QUEUE_SIZE                   (PL5_BR_PAGE_SIZE+PL5_BR_MARGIN+1)     // total items in a browse queue.
                                                                                 // queue size must be more than (Size(page) + Margin)
#define PL5_TMP_BUFFER_SIZE 	            (PL5_BR_PAGE_SIZE)

#define PL5_BR_VIEWSTACK_SIZE               MAX_DIR_DEPTH     // stack[0] corresponds to Virtual folder

#define PL5_BR_SEARCH_TIME_SLICE            200     // time slice for one search
#define PL5_BR_SEARCH_INTERVAL              100     // enter background search per 100ms


#define PL5_PL_QUEUE_SIZE             3 //4	//PL5_BR_QUEUE_SIZE
#define PL5_PL_STACK_SIZE             (MAX_DIR_DEPTH - 1)

#define PL5_PL_SEARCH_TIME_SLICE      200     // time slice for one search
#define PL5_PL_SEARCH_INTERVAL        100     // enter background search per 100ms

// reserve some margin when insert previous or next records into playback queue
#define PL5_PL_NEXT_MARGIN    (PL5_PL_QUEUE_SIZE/2)
#define PL5_PL_PREV_MARGIN    (PL5_PL_QUEUE_SIZE-PL5_PL_NEXT_MARGIN - 1)

#define PL5_PL_RAND_BUF_SIZE  10

#define MENU_BROWSE (MENU_MAIN_EXIT+1)

#define	NO_SD		0
#define	HAS_SD		1

#define TYPE_DIR    0
#define TYPE_FILE   1

#define FORWARD     0
#define BACKWARD    1

#define BACKGROUND  0
#define FOREGROUND  1

#define ATTR_UNACCEPTABLE  (ATTR_HIDDEN|ATTR_SYSTEM|ATTR_VOLUME_ID)
#define DIR_SEPARATOR	0x00002f	// "\"
#define ROOT_SEPARATOR	0x002f3A	// ":\" swizzled

#define MP3_FILE_EXT    0x33504D
#define WMA_FILE_EXT    0x414D57
#define WAV_FILE_EXT    0x564157
#define JPG_FILE_EXT	0x47504A
#define BMP_FILE_EXT    0x504D42
#define SMV_FILE_EXT    0x564D53
#define AUD_FILE_EXT    0x004141
#define MAX_EXTS        4           // Max types of supported file extension for one browse type

#define META_DATA_FILE_NOT_SUPPORTED        (WORD)(0x101)

#define PLAYSET_MUSIC           0
#define PLAYSET_VOICE           1
#define PLAYSET_FOLDER_PLAY     2
#define PLAYSET_FAVORITES       3
#define PLAYSET_PHOTO           4
#define PLAYSET_MVIDEO	        5

#define PLAYLIST_REPEAT_OFF     0
#define PLAYLIST_REPEAT_ALL     1
#define PLAYLIST_REPEAT_ONE     2
#define PL5_PL_RANDOM_MOD 4

#define PL5_PLAY_CURFOLDER_ALL_SONG	0
#define PL5_PLAY_SUBFOLDER_ALL_SONG	1
#define PL5_PLAY_ALL_SUBFOLDER  	2


// TYPE DEFINITIONS
typedef struct _pl5_folder_record {
    int iDevice:                6;      // device number, 0: internal, 1: external
    int bIsRoot:                1;      // root flag, TRUE: root, FALSE: not root
    int bIsDir:                 1;      // folder flag, TRUE: folder, FALSE: not folder
    int iRecordNum:             16;     // Directory record returned by FindFirst/FindNext
    LONG    lFastKey;                   // Directory sector/offset returned by FindFirst/FindNext
} PL5_FOLDER_RECORD;

typedef struct {
    int status;     //:                 5;                         // IDLE, RUN, PAUSE
    int phase;      //:                  6;                          // NEXT_FILE, PREV_FILE, NEXT_FOLDER, PREV_FOLDER, LAST, LASTINLAST
    BOOL foregnd;   //:               1;                       // 0: background search; 1: foreground search
    int start;      //:                  12;                          // offset to start searching
    int * piExtSupported;               // Pointer to supported extension list
} PL5_BR_SEARCH;

typedef struct {
    PL5_FOLDER_RECORD * pos;
    int size:                   8;
    int head:                   8;
    int tail:                   8;
} PL5_BR_QUEUE;

typedef struct {
    PL5_FOLDER_RECORD   folder;         // the folder that view display
    int firstfile;  //:              12;     // -1: no valid file; other: unknown or the first valid file in current folder
    int firstfolder;    //:            12;     // -1: no valid dir; other: unknown or the first valid folder in current folder
    int lastfile;   //:               12;		// the last valid file found already in current folder
    int lastfolder; //:             12;     // the last valid folder found already in current folder
    int folders;    //:                12;     // total num of folders found in current folder
    int files;  //:                  12;     // total num of files found in current folder
    BOOL foundall;  //;                      // All items in current folder are found?
    int start_cnt;  //:              12;                      // last position when count total items
    int top_recordnum;  //:          12;                  // record number in current folder of top line
    int pagesize:               8;                       // Current page size for display
    int highlight:              8;                      // highlighted line number, 0 based
    int top_index_in_queue:     8;             // position of top line in browse queue
} PL5_BR_VIEW;

typedef struct {
    PL5_BR_VIEW * pos;
    int size:                   8;
    int top:                    8;
    int bottom:                 8;
} PL5_BR_VIEWSTACK;

typedef struct {
    PL5_BR_QUEUE * queue;
    PL5_BR_VIEWSTACK * viewstack;
    PL5_BR_VIEW * current;
    PL5_BR_SEARCH * search;
    _packed BYTE * path;
    int type;
} PL5_BROWSE;


typedef struct {
    PL5_FOLDER_RECORD record;           // current folder record
    int firstfile;  //:              12;                      // -1: no valid file; other: first valid file in current folder
    int firstfolder;    //:            12;                    // -1: no valid dir; other: first valid directory in current folder
    int lastfile;   //:               12;                       // the furthest valid file found already in current folder (not in subfolder)
    int lastfolder; //:             12;                     // the furthest valid folder found already in current folder (not in subfolder)
    BOOL foundall;
} PL5_PL_FOLDER;

typedef struct {
    PL5_PL_FOLDER folder[MAX_DIR_DEPTH+1];// folders through the full path
    int depth;
    PL5_FOLDER_RECORD file;             // file
    int tracknum;
} PL5_PL_QITEM;

typedef struct {
    PL5_PL_QITEM * pos;
    int size;   //:                   8;
    int head;   //:                   8;
    int tail;   //:                   8;
} PL5_PL_QUEUE;

typedef struct {
    int status; //:                 11;                         // IDLE, RUN, PAUSE
    int phase;  //:                  12;                          // NEXT_FILE, PREV_FILE, NEXT_FOLDER, PREV_FOLDER, LAST, LASTINLAST, COUNT
    BOOL foregnd;   //:               1;                       // 0: background search 1: foreground search
    int * piExtSupported;               // Pointer to supported extension list
    PL5_PL_FOLDER * tPath;              // Searching path in folder array format
    _packed BYTE * sPath;               // Searching path in packed string format
    int depth;  //:                  12;                          // End of searching path in folder array
    int start;  //:                  12;                          // offset to start searching
    PL5_PL_FOLDER * tPath_cnt;          // Searching path in folder array format
    _packed BYTE * sPath_cnt;           // Counting path in packed string format
    int depth_cnt;  //:              12;                      // end of counting path in folder array
    int start_cnt;  //:              12;                      // offset to start counting
    int top;                            // Top of the folder array
    int tracknum;		                // track number of song found
} PL5_PL_SEARCH;

typedef struct {
    PL5_PL_QUEUE * queue;
    PL5_PL_SEARCH * search;
    int type;   //:                   8;                           // music, voice, fm, line-in ...
    int mode;   //:                   7;                           // playfolder, playfile, ...
    BOOL foundall;  //:              1;                      // All files are found?
    int currentplay;    //:            8;                    // current playing song in playback queue
    int total;                          // total number of files
} PL5_PLAYBACK;

typedef int (*PL5_BR_SUBMENU)(int a, int b, int * browse);

typedef struct {
    int rsrc;
    PL5_BR_SUBMENU pfunction;
} PL5_BR_SUBMENU_ITEM;

typedef enum {
    PL5_MUSICTYPE = 0,
    PL5_VOICETYPE,
#ifndef REMOVE_FM
    PL5_FMTYPE,
#endif
    PL5_LINEINTYPE,
#ifdef JPEG_APP
    PL5_PHOTOTYPE,
#endif
#ifdef MOTION_VIDEO
    PL5_VIDEOTYPE,
#endif
#ifdef AUDIBLE
    PL5_AUDIBLETYPE,
#endif
    PL5_DELETETYPE,
    PL5_BROWSEALL
} e_PL5BrowseType;

typedef enum {
    PL5_STATE_IDLE,
    PL5_STATE_RUN,
    PL5_STATE_PAUSE
} e_PL5State;

typedef enum {
    PL5_NEXT_FILE,
    PL5_PREV_FILE,
    PL5_NEXT_FOLDER,
    PL5_PREV_FOLDER,
    PL5_LAST,
    PL5_LASTINLAST,
    PL5_COUNT
} e_PL5Phase;

typedef enum {
    PL5_MODE_PLAYFOLDERS,
    PL5_MODE_PLAYFILES
} e_PL5Mode;

typedef enum {
    PLAYMODE_NORMAL,
    PLAYMODE_REPEAT_ONE,
    PLAYMODE_REPEAT_ALL,
    PLAYMODE_SHUFFLE,
    PLAYMODE_MAX
} e_playMode;


struct Bookmark{
	INT	  m_iTracknum;
	DWORD m_dwTrackPosBytes;
};

// DECLARATIONS OF EXTERNAL GLOBAL VARIABLE
extern PL5_PLAYBACK    g_PL5_Playback;
extern PL5_PL_QUEUE    g_PL5_PL_queue;
extern PL5_PL_SEARCH   g_PL5_PL_search;
extern PL5_PL_SEARCH   g_PL5_PL_search_cnt;
extern PL5_PL_QITEM    g_PL5_PL_buffer[];
extern PL5_PL_FOLDER   g_PL5_PL_searchingfolders[];
extern _packed BYTE    g_PL5_PL_searchingpath[];
extern PL5_PL_FOLDER   g_PL5_PL_searchingfolders_cnt[];
extern _packed BYTE    g_PL5_PL_searchingpath_cnt[];
extern int             g_piPlayExtSupported[];

extern int g_PL5_iCurFileInCurFolder;
extern int g_RealCurrenNum;
extern int g_iRandomNum;
extern e_PL5BrowseType g_ePL5BrowseType;
extern int g_iTitleResource;
extern BOOL g_Rebuild;

extern int g_iPlaylistRepeat;
extern int g_bPlaylistShuffle;
extern int g_iPlaySet;
extern int g_iTotalTracks;


// functions in playlist5.c
INT _reentrant Playlist_SetPlaySet(INT iPlaysetHint, INT iEntry, INT *piExtList);
INT _reentrant Playlist_GetPlaySet(void);
INT _reentrant Playlist_ShuffleList(INT bTrueFalse, INT iIgnored, INT *pIgnored);
void _reentrant Playlist_UpdateShuffle(void);
RETCODE _reentrant Playlist_MarkCurrentSongPlayed(int iIgnored1, int iIgnored2, int *pIgnored);
RETCODE _reentrant Playlist_GetNextSongFileInfo(INT iMode, INT iRepeat, INT*pPtr);
RETCODE _reentrant Playlist_GetCurrentSongFileInfo(INT iUnused1, INT iOffset, INT *pPtr);
RETCODE _reentrant Playlist_GetPreviousSongFileInfo(INT iUnused1, INT iRepeat, INT *pPtr);
_reentrant RETCODE Playlist_GetLFN(INT pFastKey,int RecordNum,INT* pName);
_reentrant RETCODE Playlist_GetSFN (PL5_FOLDER_RECORD * FolderRec, _packed BYTE * pszDirName);
RETCODE _reentrant Playlist_Initialize(int a, int b, int *c);
_reentrant RETCODE Playlist_GetRootString(_packed BYTE* pBuffer, INT iDevice);
_reentrant INT PL5_GetFileExtention(_packed BYTE* pFileName);
// functions in playlist5init.c
_reentrant RETCODE Playlist_InitRoot(int a, int b, int *c);
_reentrant INT Playlist_GetFileExtention(INT iIgnored1, INT iIgnored2, INT* pPtr);


#endif //_PLAYLIST5_H
