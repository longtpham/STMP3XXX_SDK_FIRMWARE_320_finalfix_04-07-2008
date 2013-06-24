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
// #include "project.h"
#include "ddildl.h"
#include "RAMDdi.h"
#include "RAMHal.h"
#include "NANDDdi.h"

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

#define NUM_LOGICAL_MEDIA       2
#define NUM_RAM_MEDIA           1

#define DRIVE_TAG_RESOURCE_BIN_RAM 0x10

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
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

LOGICAL_MEDIA g_Media[NUM_LOGICAL_MEDIA] = 
{
    &g_MediaDescriptor[0],&RAMMediaApi,
    &g_MediaDescriptor[1],&NANDMediaApi
};

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
    },
    {
        1,                      // wMediaNumber
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



MEDIA_ALLOCATION_TABLE g_MediaAllocationTable[NUM_LOGICAL_MEDIA] =
{
    7,
    0,DriveTypeData,0,33000,FALSE,
   1,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,TRUE,
   2,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,FALSE,
   3,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,FALSE,
   4,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,FALSE,
   5,DriveTypeSystem,DRIVE_TAG_HOSTRSC_BIN,50176,FALSE,
    6,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN_RAM,33000,TRUE
};


// Each Resource sector is actually 510 bytes
#define RAM_SYSTEM_DRIVE_1_NUM_SECTORS  65

// Assign drive labels.  In typical applications there will not be a Data Drive
//  and there will only be a single System Drive, typically containing 
//  resource.bin.  In that case you would use
//      extern Resource
//      INT * g_pRamSystemDrive0 = &Resource;
//  where Resource is defined in Resource.asm.

extern Resource;

RAM_MEDIA_ALLOCATION_TABLE g_RAMMediaAllocation[NUM_RAM_MEDIA] = 
{
    {
        2,
        {
            {DriveTypeData,DRIVE_TAG_DATA,1,RAM_DATA_DRIVE_0,(void *)(&Resource)},    // Do not need this one
            {DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN_RAM,RAM_SYSTEM_DRIVE_1_NUM_SECTORS,RAM_SYSTEM_DRIVE_0,(void *)(&Resource)}
        }
    }
};


#define NUM_LOGICAL_DRIVES  2


///////////////////////////////////////
// Setup the RAM media & drives
///////////////////////////////////////



WORD g_wNumRamMedia = NUM_RAM_MEDIA;


///////////////////////////////////////
// Setup the global Media structs
///////////////////////////////////////

WORD g_wNumMedia = NUM_LOGICAL_MEDIA;


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
    {&g_DriveDescriptor[1],&NANDDataDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA],&NANDDataDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+1],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+2],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+3],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+4],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+5],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+6],&RAMSystemDriveApi}
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

