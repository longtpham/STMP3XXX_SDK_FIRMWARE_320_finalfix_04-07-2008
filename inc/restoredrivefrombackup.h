// filename restoredrivefrombackup.h
#define FIRST_SYSTEM_DRIVE_TAG      (0x02)
#define SECOND_SYSTEM_DRIVE_TAG     (0x12)
#define MASTER_SYSTEM_DRIVE_TAG     (0x22) //????

#ifdef MMC
#define FIRST_SYSTEM_DRIVE_NUMBER   (6)
#define SECOND_SYSTEM_DRIVE_NUMBER  (7)
#define MASTER_SYSTEM_DRIVE_NUMBER  (8) //????
#else
#define FIRST_SYSTEM_DRIVE_NUMBER   (5)
#define SECOND_SYSTEM_DRIVE_NUMBER  (6)
#define MASTER_SYSTEM_DRIVE_NUMBER  (7) //????
#endif

extern WORD g_wPlayerResourceTag;
extern WORD wResourceDrive;
extern WORD wResourceTag;
extern BOOL g_bResourceGetDirty;
extern BOOL g_bEraseComplete;

_reentrant WORD RefreshSystemDriveErrorBlock(void);
_reentrant WORD SequentialRefreshSystemDriveErrorBlock(void);
_reentrant WORD EraseBadSystemBlock(void);
_reentrant WORD RefreshBadBlockSector(void);
_reentrant WORD IsSystemDriveDirty(void);
_reentrant WORD MarkSystemDriveDirty(void);
_reentrant WORD MarkSystemDriveClean(void);
_reentrant WORD ToggleSystemDrive(void);
_reentrant void RebootNow(void);

_reentrant int RestoreDriveFromBackup(int iDestinationDriveNum, int iSourceDriveNum); // logical
_reentrant int GetLogicalDriveNumForActiveResourceBin(void);
_reentrant int EraseSystemDriveBlock(void);
_reentrant int PartialRestoreDriveFromBackup(BOOL bSequentialReplace); // logical



