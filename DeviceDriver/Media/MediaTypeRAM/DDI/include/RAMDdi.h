////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: RAMDdi.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _RAM_DDI_H
#define _RAM_DDI_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "ddildl_internal.h"
///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define MAX_RAM_MEDIA_TABLE_ENTRIES         5

#define RAM_SECTOR_SIZE_IN_WORDS            170

#define RAM_DATA_DRIVE_0        0
#define RAM_SYSTEM_DRIVE_0      1
#define RAM_SYSTEM_DRIVE_1      2
#define RAM_SYSTEM_DRIVE_2      3
#define RAM_SYSTEM_DRIVE_3      4
#define RAM_SYSTEM_DRIVE_4      5

///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    P_SECTOR_BUFFER pSector[32];
} RAW_MEDIA, * P_RAW_MEDIA;

typedef struct {
    // This struct is currently not used.
    DWORD dwSizeInBytes;
} RAM_MEDIA_INFO, * P_RAM_MEDIA_INFO;

typedef struct {
    void * pStartAddress;
    P_RAM_MEDIA_INFO pRAMMediaInfo;
} RAM_DRIVE_INFO, * P_RAM_DRIVE_INFO;

typedef struct {
    LOGICAL_DRIVE_TYPE Type;
    WORD wTag;
    WORD wNumberOfSectors;
    WORD wRamDriveType;
    void * pMemoryAddress;
} RAM_MEDIA_ALLOCATION_TABLE_ENTRY, * P_RAM_MEDIA_ALLOCATION_TABLE_ENTRY;

typedef struct {
    WORD wNumEntries;
    RAM_MEDIA_ALLOCATION_TABLE_ENTRY Entry[MAX_RAM_MEDIA_TABLE_ENTRIES];
} RAM_MEDIA_ALLOCATION_TABLE, * P_RAM_MEDIA_ALLOCATION_TABLE;

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant RAMMediaInit(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor);
RETCODE _reentrant RAMMediaDiscoverAllocation(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor);
RETCODE _reentrant RAMMediaAllocate(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
    P_MEDIA_ALLOCATION_TABLE pTable);
RETCODE _reentrant RAMMediaGetMediaTable(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
    P_MEDIA_ALLOCATION_TABLE pTable);
RETCODE _reentrant RAMMediaGetInfo(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
    LOGICAL_MEDIA_INFO Type, void * Info);
RETCODE _reentrant RAMMediaErase(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
    WORD wMagicNumber);
RETCODE _reentrant RAMDataDriveInit(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor);
RETCODE _reentrant RAMDataDriveGetInfo(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    LOGICAL_DRIVE_INFO Type, void * Info);
RETCODE _reentrant RAMDataDriveSetInfo(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    LOGICAL_DRIVE_INFO Type, void * Info);
RETCODE _reentrant RAMDataDriveReadSector(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwSectorNumber, P_SECTOR_BUFFER pSectorData);
RETCODE _reentrant RAMDataDriveWriteSector(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwSectorNumber, P_SECTOR_BUFFER pSectorData);
RETCODE _reentrant RAMDataDriveMultiWriteSetup(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwStartSectorNumber, WORD wSectorCount, void * pCallBackFunction);
RETCODE _reentrant RAMDataDriveMultiWriteSector(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    P_SECTOR_BUFFER pSectorData, LONG lCallBackPassThru);
RETCODE _reentrant RAMDataDriveErase(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    WORD wMagicNumber);
RETCODE _reentrant RAMDataDriveFlush(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor);

RETCODE _reentrant RAMSystemDriveInit(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor);
RETCODE _reentrant RAMSystemDriveGetInfo(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    LOGICAL_DRIVE_INFO Type, void * Info);
RETCODE _reentrant RAMSystemDriveSetInfo(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    LOGICAL_DRIVE_INFO Type, void * Info);
RETCODE _reentrant RAMSystemDriveReadSector(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwSectorNumber, P_SECTOR_BUFFER pSectorData);
RETCODE _reentrant RAMSystemDriveWriteSector(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwSectorNumber, P_SECTOR_BUFFER pSectorData);
RETCODE _reentrant RAMSystemDriveMultiWriteSetup(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwStartSectorNumber, WORD wSectorCount, void * pCallBackFunction);
RETCODE _reentrant RAMSystemDriveMultiWriteSector(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    P_SECTOR_BUFFER pSectorData, LONG lCallBackPassThru);
RETCODE _reentrant RAMSystemDriveErase(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    WORD wMagicNumber);
RETCODE _reentrant RAMSystemDriveFlush(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor);


///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
extern LOGICAL_MEDIA_API RAMMediaApi;
extern RAM_MEDIA_INFO g_RAMMediaInfo[];

extern LOGICAL_DRIVE_API RAMDataDriveApi;
extern LOGICAL_DRIVE_API RAMSystemDriveApi;
extern RAM_DRIVE_INFO g_RAMDriveInfo[];

extern RAM_MEDIA_ALLOCATION_TABLE g_RAMMediaAllocation[];
extern WORD g_wNumRAMMedia;

#endif // #ifndef _RAM_DDI_H
