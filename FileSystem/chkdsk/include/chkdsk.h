////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: chkdsk.h (historic name fattest.h)
// Description: 
////////////////////////////////////////////////////////////////////////////////
#include "types.h"

#ifndef _CHKDSK_H
#define _CHKDSK_H

extern WORD _X cachedSectorSize;
extern WORD _X cachedSectorSizeInWords;
extern WORD _X cachedClusterEntryPerSectorShift;
extern WORD _X cachedDirRecordsPerSector;

#define BYTE_POS_SIGNATURE          0x1fe
#define BYTE_POS_NUM_SECT_PER_CX    0x0d
#define BYTE_POS_NUM_RES_SECT       0x0e
#define BYTE_POS_NUM_FAT            0x10
#define BYTE_POS_NUM_ROOT_SECT      0x11
#define BYTE_POS_NUM_FAT_SECT       0x16
#define BYTE_POS_NUM_HIDDEN_SECT    0x1C
#define BYTE_POS_NUM_FAT_SECT_32	0x24
#define BYTE_POS_ROOT_DIR_CX     	0x2C
#define BYTE_POS_TOTAL_SECTS	    0x13
#define BYTE_POS_TOTAL_SECTS_32	    0x20

#define DIR_REC_ATT_POS             0x0b
#define DIR_REC_FIRST_CX_POS        0x1a
#define DIR_REC_SIZE_POS            0x1c       
#define DIR_REC_FIRST_CX_HIGH_POS	0x14
#define SIGN_WORD_VALUE     0x00AA55
#define FAT_WORD            18
#define FAT_1ST_WORD        0x544146       
#define FAT12_SIGN_VALUE    0x203231
#define FAT16_SIGN_VALUE    0x203631
#define FAT32_SIGN_VALUE    0x203233

#define DIR_REC_PER_SECTOR  (cachedDirRecordsPerSector)     // Number of directory records per sector

#define MAX_ENTRIES_LONG_FILE_NAME  20      // Any long file name can be up to 255 bytes long
                                            // and each entry can hold up to 13 characters
                                            // there can only be up to 20 entries of LFN
                                            // per file.
#define BYTES_PER_DIR_RECORD    32
#define BYTES_PER_SECT          (cachedSectorSize)

#define FILE_DELETED_CODE       0xe5
#define CLUSTER_ENTRY_PER_SECTOR_SHIFT  (cachedClusterEntryPerSectorShift)

#define  MAX_CACHES             5   // 9
#define WORDS_IN_CACHE_ENTRY  705   // 705
#define     X_MEMORY            0
#define     Y_MEMORY            2
#define     P_MEMORY            4
#define READCOUNTER         105
#define WRITECOUNTER        100
#define FLUSHCOUNTER        200


#define DEFAULT_MEMORY      Y_MEMORY

#include "sectordef.h"
// exported defines that were here into sectordef.h    


//#define BOOL    unsigned int        // bVarName
//#define FALSE 0
//#define TRUE  1
//#define NOERROR 0

// A sector group is the number of sectors in the FAT buffer
// To improve speed and program writting this buffer 
// should be a multiple of 3 sectors
#define FAT12_ENTRIES_PER_SECT_GROUP  ((cachedSectorSize*3)*2/3)  // was 1024   (NumberBufferSect*sectorSize)/1.5
#define FAT16_ENTRIES_PER_SECT_GROUP  ((cachedSectorSize*3)/2)    // was  768   (NumberBufferSect*sectorSize)/2
#define FAT32_ENTRIES_PER_SECT_GROUP  ((cachedSectorSize*3)/4)    // was  384   (NumberBufferSect*sectorSize)/4

#define  SET_BIT     1
#define  FREE_BIT    2
#define  GET_BIT     0

typedef enum {
    FS_FAT12 = 0,
    FS_FAT16,
	FS_FAT32,
    FATUNSUPPORTED
    }FAT_TYPE;
    
typedef enum {
    CLEAN = 0,
    DIRTY
    } SECT_CTRL;    
    
typedef struct {
    FAT_TYPE    TypeFileSystem;
    BYTE        bSectPerCx;        
    BYTE        bNumberFats;
    WORD        wNumberRootDirEntries;
    DWORD        wNumberFatSectors;
    DWORD        wStartSectDataArea;
    DWORD        wStartSectPrimaryFat;
    DWORD        wStartSectSecondaryFat;
    DWORD        wStartSectRootDir;
    DWORD        wStartSectData;
    DWORD        dwNumHiddenSectors;
    DWORD        wNumberRootDirSectors;
	DWORD        Rootdirstartcx;
	DWORD 		 dwTotalsectors;
	DWORD 		 dwTotalclusters;
} PARTITION_BOOT_SECTOR;

typedef struct {
    BYTE        Device;                     // Logical device number
    DWORD        StartSectCurDir;            // Start Sector for the current directory
    DWORD        wStartCxCurDir;             // Start Cluster Number for current directory
    SECT_CTRL   Control;                                            // 0 means Root Directory
    DWORD        CurSect;                    // Current Sector Number Loaded in Buffer
    WORD        NumberFiles;                // Number of files in the current dir
    WORD _X     *pwBuffer;                  // Buffer to read device
} DIR_CTRL_BLK;

typedef struct {
    BYTE        StartNameCharacter;        
    WORD        Attribut;
    DWORD        StartCluster;
    DWORD       Size;
} FILE_CTRL_BLK;

typedef struct {
    BYTE        Device;
    WORD        FatSectorCached;    // Absolute sector cached in Fat buffer
    SECT_CTRL   Control;
    DWORD        FirstPrimaryFatSect;
    DWORD        FirstSecondaryFatSect;
    WORD _Y     *pwBuffer;
    } FAT_STRUCT;

typedef struct {
    BYTE        RecordNumber;
    DWORD        SectorNumber;
} DIR_REC_LOCATION;


typedef struct {
	INT CacheValid;
	LONG SectorNumber;
	INT WriteAttribute;
	INT CacheCounter;
} tCACHEDESCR_checkdisk;

    

void chkdskall(void);
void CheckDisk(BYTE bDiskNum);
BOOL _reentrant InitPartitionBootSectorStruct(BYTE bLogDevNumber);
BOOL _reentrant ScanFilesAndSubDirs(DIR_CTRL_BLK *pstDirCtrlBlk);
void _reentrant HandleFailReadSector(void);
BOOL _reentrant DeleteFileRecord(BYTE bRecordNumber, DWORD wSectNumber, DIR_CTRL_BLK *pstDirCtrlBlk);
BOOL _reentrant GetFileCtrlBlk(BYTE bRecordNumber, DWORD wSectNumber, DIR_CTRL_BLK *pstDirCtrlBlk, FILE_CTRL_BLK *pstFileCtrlBlk);
//BOOL _reentrant ScanFileList(DIR_CTRL_BLK *pstDirCtrlBlk, DIR_REC_LOCATION *pstFileList);
DWORD _reentrant CxToSect(DWORD wCx);
BOOL _reentrant ReadDirSector(DWORD wSectNumber, DIR_CTRL_BLK *pstDirCtrlBlk);
LONG _reentrant CheckCrossLinkFile(DWORD wStartCluster,BYTE blogdevicenumber);
void _reentrant ReserveCluster(DWORD wStartCluster,BYTE blogdevicenumber);
BOOL _reentrant ScanDirectory(DIR_CTRL_BLK *pstDirCtrlBlk);
BOOL _reentrant ScanAndUpdateFat(BYTE blogdevicenumber,FAT_TYPE TypeFileSystem);
#ifdef STMP_BUILD_DCC
void InitTime(void);
DWORD GetTime(void);
#endif
#endif 
