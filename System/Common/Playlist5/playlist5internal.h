#ifndef __PLAYLIST5INTERNAL_H
#define __PLAYLIST5INTERNAL_H

#include "project.h"

#include "filesystem.h"

//#define DISPLAY_LINES_PER_SCREEN 6 //replaced by MENU_PAGE_ITEM_COUNT


//#define MAX_RECORDS_PER_DIRECTORY   4096    //calculated from the bits in the m_iRecordNum field in the following data structures.
//											//An entry can have multiple records in the FAT for LFNs.
//											//Records more than this number will not be included in the playlist content

#define NUM_FOLDER_RECORD_FOR_WINDOW   30
#define NUM_FOLDER_MARKER                           300
#define FOLDER_MARKER_DISTANCE                  10 //totally record (NUM_FOLDER_MARKER * FOLDER_MARKER_DISTANCE)
#define NUM_TRACK_INFO_BUFFER                    3
#define NUM_RANDOM_HISTORY                         50

#define LEN_OF_NAME_IN_BYTE (120) /* must be an integer multiple of 3 */
#define NUM_OF_BYTE_IN_ONE_WORD (3)

#define DIR_RECURCIVE_ON 0
#define DIR_RECURCIVE_OFF 1

enum {
    E_TRACK_PREVIOUS = 0,
    E_TRACK_CURRENT,
    E_TRACK_NEXT
};

union PlaySetHint
{
    struct {
        unsigned int PlaySet             :3;
        int Changed           :1;
        int CurrentPos      :20;
    } detail;
    WORD I;
};

typedef struct  {
    int iEntry;
    int name[LEN_OF_NAME_IN_BYTE/NUM_OF_BYTE_IN_ONE_WORD];
} tPageItem;

typedef struct {
    int m_iItems;
    int m_iPage;

    tPageItem *m_pItems;
} tPagedInfoStructure;

//typedef struct{
//    int    iDevice:6;
//    int    bIsRoot:1;
//    int    bIsDir:1;
//    int    iRecordNum:16;
//    LONG lFastKey;
//#ifdef DEBUG_SFN
//    _packed BYTE name[SFN_LENGTH];
//#endif
//}PL5_FOLDER_RECORD;

////for sort compliant
//typedef struct{
//#ifdef PERFORM_SORT
//    int    iNext;
//#endif
//    PL5_FOLDER_RECORD tContent;
//} pl_folderRecord2;


//struct Bookmark{
//    INT	  m_iTracknum;
//    DWORD m_dwTrackPosBytes;
//}; //this will save the num and position of the song selected for bookmark


#ifdef PERFORM_SORT
typedef struct{
    void* pEntry;
    _packed BYTE name[SFN_LENGTH];
}SFNStorage;


extern int g_iNumStoredSFN;
extern SFNStorage g_TempSFNSpace[];
#endif

extern int g_iBeginLevel;
//extern PL5_FOLDER_RECORD g_DirEntryPerDevice[];
//extern PL5_FOLDER_RECORD g_tCurFolderRecord[];
//extern PL5_FOLDER_RECORD g_tSubFolderRecord[];
//extern pl_folderRecord2 g_tSubFolderRecord[];
//extern PL5_FOLDER_RECORD g_tFolderWindow[];
//extern PL5_FOLDER_RECORD g_tFolderSecondWindow[];
//extern PL5_FOLDER_RECORD g_tFolderMarker[];
//extern pl_buildModuleParam g_tBuildModuleParam;
//extern PL5_TRACKINFO g_tTrackInfo[];
extern LONG g_lRandomHistory[];


extern int g_iPlaylistRepeat;
extern int g_bPlaylistShuffle;

extern int g_iTotalTracks;
extern int g_iPlaySet;

extern int g_iCurrentTrack;

//extern struct Bookmark g_MarkerMusic, g_MarkerVoice;



extern int g_iTotalDir;
extern int g_iTotalFiles;

#endif
