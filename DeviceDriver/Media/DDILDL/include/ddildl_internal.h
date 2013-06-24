////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: ddildl_internal.h
// Description: 
////////////////////////////////////////////////////////////////////////////////
#ifndef _DDILDL_INTERNAL_H
#define _DDILDL_INTERNAL_H

#include "ddildl.h"

///////////////////////////////////////
// Logical Media structures
///////////////////////////////////////

typedef struct {
    // Discovered at Run Time by MediaInit:
    WORD    wMediaNumber;
    BOOL    bInitialized;
    MEDIA_STATE eState;
    BOOL    bAllocated;
    BOOL    bWriteProtected;
    WORD    wNumberOfDrives;//on this media
    DWORD   dwSizeInBytes;
    WORD    wAllocationUnitSizeInBytes;
    PHYSICAL_MEDIA_TYPE PhysicalType;
    void * pMediaInfo;  // same as in LOGICAL_DRIVE_DESCRIPTOR
} LOGICAL_MEDIA_DESCRIPTOR, * P_LOGICAL_MEDIA_DESCRIPTOR;

// Logical Media API functions
typedef _reentrant RETCODE (*P_MEDIA_INIT_FUNCTION)(P_LOGICAL_MEDIA_DESCRIPTOR);
typedef RETCODE _reentrant (*P_MEDIA_ALLOCATE_FUNCTION)(P_LOGICAL_MEDIA_DESCRIPTOR,
    P_MEDIA_ALLOCATION_TABLE);
typedef RETCODE _reentrant (*P_MEDIA_DISCOVER_ALLOCATION_FUNCTION)(P_LOGICAL_MEDIA_DESCRIPTOR);
typedef RETCODE _reentrant (*P_MEDIA_GET_MEDIA_TABLE_FUNCTION)(P_LOGICAL_MEDIA_DESCRIPTOR,
    P_MEDIA_ALLOCATION_TABLE);
typedef RETCODE _reentrant (*P_MEDIA_GET_INFO_FUNCTION)(P_LOGICAL_MEDIA_DESCRIPTOR,
    LOGICAL_MEDIA_INFO, void * _Y);
typedef RETCODE _reentrant (*P_MEDIA_ERASE_FUNCTION)(P_LOGICAL_MEDIA_DESCRIPTOR,
    WORD wMagicNumber);

typedef struct {
    WORD wInitCodeBank;
    P_MEDIA_INIT_FUNCTION pInit;
    WORD wAllocateCodeBank;
    P_MEDIA_ALLOCATE_FUNCTION pAllocate;
    WORD wDiscoverAllocationCodeBank;
    P_MEDIA_DISCOVER_ALLOCATION_FUNCTION pDiscoverAllocation;
    WORD wGetMediaTableCodeBank;
    P_MEDIA_GET_MEDIA_TABLE_FUNCTION pGetMediatable;
    WORD wGetInfoCodeBank;
    P_MEDIA_GET_INFO_FUNCTION pGetInfo;
    WORD wEraseCodeBank;
    P_MEDIA_ERASE_FUNCTION pErase;
} LOGICAL_MEDIA_API, * P_LOGICAL_MEDIA_API;

typedef struct {
    P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor;
    P_LOGICAL_MEDIA_API pApi;
} LOGICAL_MEDIA, * P_LOGICAL_MEDIA;

///////////////////////////////////////
// Logical Drive structures
///////////////////////////////////////

typedef struct {
    // Discovered at Run Time by MediaDiscover
    BOOL    bInitialized;
    BOOL    bPresent;       // Indicates if a system drive is present
                            // wTag from Config Block matches wTag from Drive
                            // redundant area
    BOOL    bErased;
    BOOL    bWriteProtected;
    DWORD   dwNumberOfSectors;
    LOGICAL_DRIVE_TYPE Type;
    WORD    wTag;
    WORD    wDriveNumber;
    DWORD   dwSizeInBytes;
    WORD    wSectorSizeInBytes; // not included bad blocks (NAND media)
    WORD    wEraseSizeInBytes;
    SYSTEM_VERSION ProjectVersion;
    SYSTEM_VERSION ComponentVersion;
    P_LOGICAL_MEDIA_DESCRIPTOR pLogicalMediaDescriptor;
    void * pMediaInfo;  // same as in LOGICAL_MEDIA_DESCRIPTOR
    void * pDriveInfo;
} LOGICAL_DRIVE_DESCRIPTOR, * P_LOGICAL_DRIVE_DESCRIPTOR;

// Logical Drive API functions
typedef RETCODE _reentrant (*P_DRIVE_INIT_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR);
typedef RETCODE _reentrant (*P_DRIVE_GET_INFO_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR,
    LOGICAL_DRIVE_INFO, void *);
typedef RETCODE _reentrant (*P_DRIVE_SET_INFO_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR,
    LOGICAL_DRIVE_INFO, void *);
typedef RETCODE _reentrant (*P_DRIVE_READ_SECTOR_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR,
    DWORD, P_SECTOR_BUFFER);
typedef RETCODE _reentrant (*P_DRIVE_WRITE_SECTOR_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR,
    DWORD, P_SECTOR_BUFFER);
typedef RETCODE _reentrant (*P_DRIVE_MULTI_WRITE_SETUP_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR,
    DWORD, WORD, MULTI_WRITE_CALLBACK_FUNCTION);
typedef RETCODE _reentrant (*P_DRIVE_MULTI_WRITE_SECTOR_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR,
    P_SECTOR_BUFFER, LONG);
typedef RETCODE _reentrant (*P_DRIVE_ERASE_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR, WORD);
typedef RETCODE _reentrant (*P_DRIVE_FLUSH_FUNCTION)(P_LOGICAL_DRIVE_DESCRIPTOR);

typedef struct {
    WORD wInitCodeBank;
    P_DRIVE_INIT_FUNCTION pInit;
    WORD wGetInfoCodeBank;
    P_DRIVE_GET_INFO_FUNCTION pGetInfo;
    WORD wSetInfoCodeBank;
    P_DRIVE_SET_INFO_FUNCTION pSetInfo;
    WORD wReadSectorCodeBank;
    P_DRIVE_READ_SECTOR_FUNCTION pReadSector;
    WORD wWriteSectorCodeBank;
    P_DRIVE_WRITE_SECTOR_FUNCTION pWriteSector;
    WORD wMultiWriteSetupCodeBank;
    P_DRIVE_MULTI_WRITE_SETUP_FUNCTION pMultiWriteSetup;
    WORD wMultiWriteSectorCodeBank;
    P_DRIVE_MULTI_WRITE_SECTOR_FUNCTION pMultiWriteSector;
    WORD wEraseCodeBank;
    P_DRIVE_ERASE_FUNCTION pErase;
    WORD wFlushCodeBank;
    P_DRIVE_FLUSH_FUNCTION pFlush;
} LOGICAL_DRIVE_API, * P_LOGICAL_DRIVE_API;

typedef struct {
    P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor;
    P_LOGICAL_DRIVE_API pApi;
} LOGICAL_DRIVE, * P_LOGICAL_DRIVE;

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
extern MEDIA_ALLOCATION_TABLE g_MediaAllocationTable[];
extern LOGICAL_MEDIA_DESCRIPTOR g_MediaDescriptor[];
extern LOGICAL_MEDIA g_Media[];
extern WORD g_wNumMedia;
extern LOGICAL_DRIVE_DESCRIPTOR g_DriveDescriptor[];
extern LOGICAL_DRIVE g_Drive[];
extern WORD g_wNumDrives;

///////////////////////////////////////////////////////////////////////////////
// SysCallFunction Variants
///////////////////////////////////////////////////////////////////////////////
RETCODE _asmfunc SysCallFunction_P_LOGICAL_MEDIA_DESCRIPTOR_f(WORD * RESOURCE,RETCODE _reentrant (int,int,int*),  int, int, int *);
#define SysCallFunction_P_LOGICAL_MEDIA_DESCRIPTOR(rsrc,func,a,b,p) SysCallFunction_P_LOGICAL_MEDIA_DESCRIPTOR_f((void*)rsrc,(func),(a),(b),(p))

#endif