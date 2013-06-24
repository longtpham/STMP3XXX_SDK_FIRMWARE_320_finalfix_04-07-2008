////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: ddiproject.c
// Description: DDI Test
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "ddildl.h"
#include "RAMDdi.h"
#include "RAMHal.h"

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

#define NUM_RAM_MEDIA               1

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////
// Setup the RAM media & drives
///////////////////////////////////////

// Assign drive labels.  In typical applications there will not be a Data Drive
//  and there will only be a single System Drive, typically containing 
//  resource.bin.  In that case you would use
//      g_pRamSystemDrive0 = (void)(&Resource);
//  where Resource is defined in Resource.asm.

#define RAM_DATA_DRIVE_0_NUM_SECTORS    8
#define RAM_SYSTEM_DRIVE_0_NUM_SECTORS  10
#define RAM_SYSTEM_DRIVE_1_NUM_SECTORS  4
#define RAM_SYSTEM_DRIVE_2_NUM_SECTORS  6
#define RAM_SYSTEM_DRIVE_3_NUM_SECTORS  6

RAM_SECTOR_BUFFER RawSectorArrayRamDataDrive0[RAM_DATA_DRIVE_0_NUM_SECTORS][RAM_SECTOR_SIZE_IN_WORDS];
RAM_SECTOR_BUFFER RawSectorArrayRamSystemDrive0[RAM_SYSTEM_DRIVE_0_NUM_SECTORS][RAM_SECTOR_SIZE_IN_WORDS];
RAM_SECTOR_BUFFER RawSectorArrayRamSystemDrive1[RAM_SYSTEM_DRIVE_1_NUM_SECTORS][RAM_SECTOR_SIZE_IN_WORDS];
RAM_SECTOR_BUFFER RawSectorArrayRamSystemDrive2[RAM_SYSTEM_DRIVE_2_NUM_SECTORS][RAM_SECTOR_SIZE_IN_WORDS];
RAM_SECTOR_BUFFER RawSectorArrayRamSystemDrive3[RAM_SYSTEM_DRIVE_3_NUM_SECTORS][RAM_SECTOR_SIZE_IN_WORDS];

WORD g_wNumRamMedia = NUM_RAM_MEDIA;

#ifdef NAND2
WORD g_wNumNANDChips = 2;
#else
#ifdef NAND3
WORD g_wNumNANDChips = 3;
#else
#ifdef NAND4
WORD g_wNumNANDChips = 4;
#else
WORD g_wNumNANDChips = 1;
#endif
#endif
#endif

RAM_MEDIA_ALLOCATION_TABLE g_RAMMediaAllocation[NUM_RAM_MEDIA] = 
{
    {
        5,
        {
            {DriveTypeData,DRIVE_TAG_DATA,RAM_DATA_DRIVE_0_NUM_SECTORS,RAM_DATA_DRIVE_0,(void *)(&RawSectorArrayRamDataDrive0)},
            {DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,RAM_SYSTEM_DRIVE_0_NUM_SECTORS,RAM_SYSTEM_DRIVE_0,(void *)(&RawSectorArrayRamSystemDrive0)},
            {DriveTypeSystem,DRIVE_TAG_BOOTMANAGER_S,RAM_SYSTEM_DRIVE_1_NUM_SECTORS,RAM_SYSTEM_DRIVE_1,(void *)(&RawSectorArrayRamSystemDrive1)},
            {DriveTypeSystem,DRIVE_TAG_STMPSYS_S,RAM_SYSTEM_DRIVE_2_NUM_SECTORS,RAM_SYSTEM_DRIVE_2,(void *)(&RawSectorArrayRamSystemDrive2)},
            {DriveTypeSystem,DRIVE_TAG_HOSTLINK_S,RAM_SYSTEM_DRIVE_3_NUM_SECTORS,RAM_SYSTEM_DRIVE_3,(void *)(&RawSectorArrayRamSystemDrive3)}
        }
    }
};

///////////////////////////////////////
// Setup the global Media structs
///////////////////////////////////////

LOGICAL_MEDIA_DESCRIPTOR g_MediaDescriptor[NUM_LOGICAL_MEDIA] =
{
    {
        0,                      // wMediaNumber
        FALSE,                  // bInitialized
        MediaStateUnknown,      // eState
        FALSE,                  // bWriteProtected
        0,                      // wNumberOfDrives
        0,                      // dwSizeInBytes
        0,                      // wAllocationUnitSizeInBytes
        0,                      // PhysicalType
        0                       // pMediaInfo
    }
};

LOGICAL_MEDIA g_Media[NUM_LOGICAL_MEDIA] = 
{
    &g_MediaDescriptor[0],&RAMMediaApi
};

WORD g_wNumMedia = NUM_LOGICAL_MEDIA;

MEDIA_ALLOCATION_TABLE g_MediaAllocationTable[NUM_LOGICAL_MEDIA] =
{
    5,
    0,DriveTypeData,DRIVE_TAG_DATA,50,TRUE,
    0,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,5120,TRUE,
    0,DriveTypeSystem,DRIVE_TAG_BOOTMANAGER_S,2048,TRUE,
    0,DriveTypeSystem,DRIVE_TAG_STMPSYS_S,3072,TRUE,
    0,DriveTypeSystem,DRIVE_TAG_HOSTLINK_S,3072,TRUE
};

///////////////////////////////////////
// Setup the global Drive structs
///////////////////////////////////////
// New global instead of just the MAX_LOGICAL_DRIVES define in driver lib
WORD g_wMaxLogicalDrives = MAX_LOGICAL_DRIVES; 

LOGICAL_DRIVE_DESCRIPTOR g_DriveDescriptor[MAX_LOGICAL_DRIVES];

WORD g_wNumDrives = NUM_LOGICAL_MEDIA+1;
WORD _X g_wHiddenDriveIdx = NUM_LOGICAL_MEDIA;

LOGICAL_DRIVE g_Drive[MAX_LOGICAL_DRIVES] = 
{
    &g_DriveDescriptor[0],&RAMDataDriveApi,
    &g_DriveDescriptor[1],&RAMSystemDriveApi,
    &g_DriveDescriptor[2],&RAMSystemDriveApi,
    &g_DriveDescriptor[3],&RAMSystemDriveApi,
    &g_DriveDescriptor[4],&RAMSystemDriveApi
};


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

