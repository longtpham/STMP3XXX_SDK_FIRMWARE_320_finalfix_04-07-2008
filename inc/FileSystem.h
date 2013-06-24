////////////////////////////////////////////////////////////////////////////////
//  Copyright(C) SigmaTel, Inc. 2001
//
//  File        : FileSystem.h
//  Description : Header File for File System
////////////////////////////////////////////////////////////////////////////////

#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H

#include "types.h"


// File attributes
#ifndef _FS_ATTRIBUTES
#define _FS_ATTRIBUTES
#define READ        1
#define WRITE       2
#define WRITE_PLUS  3
#define APPEND      4
#define TRUNCATE    8
#define CREATE      16
#endif

//#ifndef FAT12
//#define FAT12   1
//#endif
//
#ifndef FAT16
#define FAT16   2
#endif

#define MEM_SPACE_P 0x100000
#define MEM_SPACE_Y 0x400000
#define MEM_SPACE_X 0x800000

#define FILE_SYS_MODE_READ  0
#define FILE_SYS_MODE_WRITE 1
 
#define ATTR_READ_ONLY      0x01
#define ATTR_HIDDEN         0x02
#define ATTR_SYSTEM         0x04
#define ATTR_VOLUME_ID      0x08
#define ATTR_DIRECTORY      0x10
#define ATTR_ARCHIVE        0x20
#define ATTR_LONG_NAME      ATTR_READ_ONLY|ATTR_HIDDEN|ATTR_SYSTEM|ATTR_VOLUME_ID

#define SEEK_SET           -1
#define SEEK_CUR            0
#define SEEK_END            1

#define DEVICE_INSTALLED              (WORD)(0)
#define DEVICE_NOT_FOUND              (WORD)(2)
#define INVALID_FILESYSTEM			  (WORD)(3)
#define DEVICE_INVALID                (WORD)(-1)
#define DEVICE_ERROR_WRITE_PROTECTED  (WORD)(-2)

#define MEDIA_SIZE_TOTAL_SECTORS    0
#define MEDIA_SIZE_TOTAL_BYTES      1
#define MEDIA_SIZE_BYTES_PER_SECTOR 2
#define MEDIA_SIZE_IN_MEGABYTES     3     

#define READ_TYPE_NORMAL            0
#define READ_TYPE_FAT               1
#define READ_TYPE_RAW               2


#ifdef MEDIA_DDI_PARTIAL_SEQ_ENABLE
    #define WRITE_TYPE_RANDOM                   0
    #define WRITE_TYPE_SEQ_FIRST                1
    #define WRITE_TYPE_SEQ_NEXT                 2
    #define WRITE_TYPE_RESET_CLEAN_UP           3
    #define WRITE_TYPE_PARTIAL_SEQ_FIRST        4
    #define WRITE_TYPE_PARTIAL_SEQ_NEXT         5
    #define WRITE_TYPE_PARTIAL_SEQ_LAST         6
    #ifdef MEDIA_DDI_RAW_SECTOR_SERVER_ENABLE
        #define WRITE_TYPE_RANDOM_RAW               7
        #define WRITE_TYPE_SEQ_FIRST_RAW            8
        #define WRITE_TYPE_SEQ_NEXT_RAW             9
        #define WRITE_TYPE_PARTIAL_SEQ_FIRST_RAW    10
        #define WRITE_TYPE_PARTIAL_SEQ_NEXT_RAW     11
        #define WRITE_TYPE_PARTIAL_SEQ_LAST_RAW     12
    #endif
#else
    #define WRITE_TYPE_RANDOM                   0
    #define WRITE_TYPE_SEQ_FIRST                1
    #define WRITE_TYPE_SEQ_NEXT                 2
    #ifdef MEDIA_DDI_RAW_SECTOR_SERVER_ENABLE
        #define WRITE_TYPE_RANDOM_RAW               3
        #define WRITE_TYPE_SEQ_FIRST_RAW            4
        #define WRITE_TYPE_SEQ_NEXT_RAW             5
    #endif
#endif
    #define WRITE_TYPE_UNDEFINED 0xFFFFFF


#ifndef MAX_FILENAME_LENGTH
#define MAX_FILENAME_LENGTH 256
#endif

typedef struct {
    WORD wNumberOfZones;
    WORD wSizeInMegaBytes;
} SMARTMEDIA_CHIP_INFO;

typedef struct {
    WORD wBootIdentification;
    WORD wStartHeadNumber;
    WORD wStartSectorNumber;
    WORD wStartCylinderNumber;
    WORD wSystemIdentification;
    WORD wEndHeadNumber;
    WORD wEndSectorNumber;
    WORD wEndCylinderNumber;
    WORD wStartLogicalSectorNumberHigh;
    WORD wStartLogicalSectorNumberLow;
    WORD wPartitionSizeHigh;
    WORD wPartitionSizeLow;
} SMARTMEDIA_PARTITION_TABLE;

typedef struct {
    WORD wWord0;
    WORD wWord1;
    WORD wWord2;
} SMARTMEDIA_CIS_IDI_MANUFACTURER_NAME;

typedef struct {
    WORD wWord0;
    WORD wWord1;
} SMARTMEDIA_CIS_IDI_PRODUCT_NAME;

typedef struct {
    WORD wWord0;
    WORD wWord1;
} SMARTMEDIA_CIS_IDI_PRODUCT_VERSION;

typedef struct {
    WORD wWord0;
    WORD wWord1;
    WORD wWord2;
    WORD wWord3;
} SMARTMEDIA_CIS_IDI_SERIAL_NUMBER;

typedef struct {
    WORD wWord0;
} SMARTMEDIA_CIS_IDI_MODEL_NUMBER;

typedef struct {
    SMARTMEDIA_CIS_IDI_MANUFACTURER_NAME ManufacturerName;
    SMARTMEDIA_CIS_IDI_PRODUCT_NAME ProductName;
    SMARTMEDIA_CIS_IDI_PRODUCT_VERSION ProductVersion;
    SMARTMEDIA_CIS_IDI_SERIAL_NUMBER SerialNumber;
    SMARTMEDIA_CIS_IDI_MODEL_NUMBER ModelNumber;
} SMARTMEDIA_CIS_IDI_TABLE;
   
typedef struct {
    WORD wPageSizeInBytes;
    WORD wRedundantAreaSizeInBytes;
    WORD wNumberOfPagesPerBlock;
    WORD wNumberOfBlocksPerZone;
    WORD wNumberOfZonesInMedia;
    WORD wMediaSizeInMBytes;
    SMARTMEDIA_PARTITION_TABLE * pPartitionTable;
    SMARTMEDIA_CIS_IDI_TABLE * pCisIdiTable;
    WORD wMediaFlagStatus;
    WORD wNumberOfBlocksToTheCisIdiBlock;
    WORD wTotalNumberOfPhysicalBlocks;
    WORD wNumberOfSystemBlocks;
} SMARTMEDIA_ENTRY_TABLE;

typedef struct {
    WORD wDevice;        
    WORD wDirtyBlockFlag;
    WORD wCleanTailFlag; 
    WORD wLogDOSPage;    
    WORD wSrcLogBlock;   
    WORD wSrcPhyBlock;   
    WORD wDestPhyBlock;  
    WORD wStartSrcPage;  
    WORD wStartDestPage; 
    WORD wPagesToCopy;   
    WORD wReplaceBuff;   
    WORD wReplaceRdnt;
    #ifdef MEDIA_DDI_PARTIAL_SEQ_ENABLE
        WORD wFirstCount;
        WORD wNextCount;
        WORD wLastCount;
    #endif
} SMARTMEDIA_FWPPS_TABLE;

typedef struct {
    WORD wWord0;
    WORD wWord1;
    WORD wWord2;
    WORD wWord3;
} DIRECTORY_NAME;

typedef struct {
    WORD wWord0;
    WORD wWord1;
} DIRECTORY_EXTENSION;

typedef struct {
    WORD wWord0;
    WORD wWord1;
} DIRECTORY_SIZE;

typedef struct {
    DIRECTORY_NAME Name;
    DIRECTORY_EXTENSION Extension;
    WORD wAttribute;
    WORD wReserved[4];
    WORD wCreationTime;
    WORD wCreationData;
    WORD wFirstCluster;
    DIRECTORY_SIZE Size;
    WORD wCurrentCluster;
    WORD wPointer;
    WORD wRecord;
    WORD wRd;
} DIRECTORY_FILE_CONTROL_BLOCK;

// TODO:  clean this up.  There are two versions.
struct FCB
{
    _packed BYTE m_szFileName[9];       //0-2
    int     m_wReserved;                //3
    _packed BYTE m_szExt[4];            //4-5
    int     m_wAttributes;              //6
    int     m_wReserved2[4];            //7,8,9,a
    int     m_wTimeofCreation;          //b
    int     m_wDateofCreation;          //c
    int     m_wFirstCluster;            //d
    int     m_wFileSizeHigh;            //e
    int     m_wFileSizeLow;             //f
};


typedef struct {
    WORD wFirstClusterCurrentDirectory;
    WORD wFirstClusterParentDirectory;
    WORD wAbsSectorCurrentlyCached;
    WORD wCurrentRelativeSector;
    WORD wNumberOfSectors;
    WORD wCurrentRecordLoadedInDcb;
    WORD wBufferedRecord;
    WORD wMaxNumberRecordsCurrentDirectory;
    WORD * pwPointerToBuffer;
    WORD * pwPointerToPath;
    DIRECTORY_FILE_CONTROL_BLOCK * pDirFcb;
} DIRECTORY_CONTROL_BLOCK;

typedef struct {
    WORD wWord0;
    WORD wWord1;
    WORD wWord2;
    WORD wWord3;
} FILE_NAME;

typedef struct {
    WORD wWord0;
    WORD wWord1;
} FILE_EXTENSION;

typedef struct {
    WORD wWord0;
    WORD wWord1;
} FILE_SIZE;

typedef union {
    struct {
        int Read        :1;
        int Write       :1;
        int Append      :1;
        int Truncate    :1;
        int Create      :1;
        int Rsrv        :3;
        int Mode        :8;
        int Device      :8;
    } B;
    int I;
} FILE_FLAGS;

typedef struct {
    WORD wWord0;
    WORD wWord1;
} FILE_BYTE_CURRENT;

typedef struct {
    FILE_NAME Name;
    FILE_EXTENSION Extension;
    WORD wAttributes;
    WORD wReserved[4];
    WORD wCreationTime;
    WORD wCreationData;
    WORD wFirstCluster;
    FILE_SIZE Size;
    WORD wCurrentCluster;
    WORD wPointer;
    WORD wRecord;
    WORD wRd;
    FILE_FLAGS Flags;
    FILE_BYTE_CURRENT FcbByteCurrent;
    WORD wFcbFlagEndOfCx;
} FILE_CONTROL_BLOCK;    

typedef struct {
    WORD wWord0;
    WORD wWord1;
    WORD wWord2;
    WORD wWord3;
} VOLUME_LABEL;

typedef struct {
    WORD wFATPhysicalBlock1;
    WORD wFATPhysicalBlock2;
    WORD wFATPhysicalBlock3;
    WORD wFATPhysicalBlock4;
} FAT_PHYSICAL_BLOCK_LIST;

typedef struct {
    WORD wFATSectorInCache;
    WORD wLastClusterFree;
    WORD wNumberOfUsedClusters;
    WORD wNumberOfFreeClusters;
    WORD wNumberOfBadClusters;
    WORD wNumberOfReservedClusters;
    WORD wControl;
    WORD * pwSectorCache;
    FAT_PHYSICAL_BLOCK_LIST FATPhysicalLocationList;
} FAT_TABLE;

typedef struct {
    WORD wStateMediaTable;
    WORD wTypeFs;
    WORD wBytesPerSector;
    WORD wSectorsPerCluster;
    WORD wNumberOfReservedSectors;
    WORD wMaximumNumberOfFATs;
    WORD wMaxRootDirectoryEntries;
    WORD wTotalSectors;
    WORD wNumberOfFATSectors;
    WORD wNumberOfSectorsPerTrack;
    WORD wNumberOfHeads;
    WORD wNumberOfHiddenSectorsMSB;
    WORD wNumberOfHiddenSectorsLSB;
    WORD wTotalSectors32MSB;
    WORD wTotalSectors32LSB;
    WORD wDriverNumber;
    WORD wExtendedBootSignature;
    WORD wVolumeIDMSB;
    WORD wVolumeIDLSB;
    VOLUME_LABEL VolumeLabel;
    WORD * pwWriteBuffer;
    WORD wPrimaryFATRelativeSector;
    WORD wSecondaryFATRelativeSector;
    WORD wRootDirectoryRelativeSector;
    WORD wFirstSectorNumberDataZone;
    WORD wMaxNumberOfFATEntries;
    WORD wRootDirectorySizeInSectors;
    WORD wDataAreaSizeInSectors;
} MEDIA_TABLE;

typedef struct {
    MEDIA_TABLE * pMediaTable;
    DIRECTORY_CONTROL_BLOCK * pDirectoryControlBlock;
    FAT_TABLE * pFATTable;
} DEVICE_CONTROL_TABLE;
    
typedef struct {
    WORD dwTotalSizeInMegaBytes;        // dwTotalSizeInMegaBytes is limited
                                        //  to 2-bytes for compatibility with
                                        //  older host drivers.
    DWORD dwTotalNumberOfSectors;
    DWORD dwTotalNumberOfBytes;
    WORD wSectorSizeInBytes;
} MEDIA_SIZE;

typedef struct {
    BOOL    bInstalled;
    INT     iPbsSectorOffset;   // from the beginning of the data drive 
    DWORD   dwSize;
} DATA_DRIVE_PBS_LOC;
extern  INT _reentrant FSFileOpen(_packed BYTE *fname,INT attribute, INT DeviceNumber);
extern  INT _reentrant FSFastOpen(DWORD Key, INT attribute);
extern  INT FSFileDelete(_packed BYTE *fname,INT DeviceNumber);
extern  INT FSFileCreate(_packed BYTE *fname,INT DeviceNumber);
extern  INT _reentrant FSChangeDir(_packed BYTE *dirname,INT DeviceNumber);
extern  INT _reentrant FSCreateDir(_packed BYTE *dirname,INT DeviceNumber);
extern  INT _reentrant FSDeleteDir(_packed BYTE *dirname,INT DeviceNumber);
extern  INT _reentrant FSFileAppend(_packed BYTE *fname,INT DeviceNumber);
extern  INT _reentrant FSFileRead(INT BytestoRead, INT Handle, INT memory_type, INT modulo, WORD *buffer);
extern  INT _reentrant FSFileWrite(INT wNumberBytes, INT Handle, INT memory_type, INT iModulo, WORD *buffer);
extern  INT _reentrant FSFileClose(INT Handle);
extern  LONG _reentrant FSFileSize(INT HandleNumber);
extern  LONG  FSSize(INT DeviceNum, INT TYPE);	 
extern  INT FSFreeClusters(INT Device);
extern  INT BytesPerCluster(INT Device);



//extern  INT FSFileCreate(_packed BYTE *pbFileName, BYTE bDevice);
//extern  INT FSFileDelete(_packed BYTE *pbFileName, BYTE bDevice);
extern  INT FSFileRename(_packed BYTE *pbCurFilename, _packed BYTE *pbNewFilename, BYTE bDevice);
//extern  INT _reentrant FSFileAttribute(_packed BYTE *pbFilename, WORD wAttributes, BYTE bDevice);
//extern  INT FSFileAppend(_packed BYTE *pbFileName, BYTE bDevice);
//extern  INT FSFileOpen(_packed BYTE *pbFileName, WORD wAttributes, BYTE bDevice);
//extern  INT FSFileClose(INT bHandle);
//extern  INT FSFileRead(WORD wNumberBytes, BYTE bHandle, WORD wMemSpace, INT iModulo, WORD *pwBuffer);
//extern  INT FSFileWrite(WORD wNumberBytes, BYTE bHandle, WORD wMemSpace, INT iModulo, WORD *pwBuffer);
extern  INT _reentrant fseek( INT handle , LONG offset, INT iOrigin );
extern  INT _reentrant FSFileEof(INT Handle);
extern INT _reentrant FSFileAttribute(INT Attributes, _packed BYTE *fname, INT DeviceNumber);
extern INT _reentrant FSFileAttributeClear(INT Attributes, _packed BYTE *fname, INT DeviceNumber);
//extern  LONG FSFileSize(INT bHandle);
extern _asmfunc INT FSPresent(BYTE bDevice);
extern _asmfunc INT FSType(BYTE bDevice);
//extern LONG FSSize(WORD wDevice, WORD wReturnType);
//extern INT FSChangeDir(_packed BYTE *pbDirName, BYTE bDevice);
//extern INT FSCreateDir(_packed BYTE *pbDirName, BYTE bDevice);
//extern INT FSDeleteDir(_packed BYTE *pbDirName, BYTE bDevice);
extern _asmfunc void SysLoadFATWrite(void);
extern _asmfunc INT SysUnLoadFATWrite(void);
extern LONG _reentrant ftell(INT iHandle);
extern _asmfunc struct FCB * ReadDirectory(int iDevice, int iEntry);

#endif
