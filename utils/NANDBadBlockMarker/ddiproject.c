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
#include "ddildl.h"
#include "NANDDdi.h"
#include "NANDHal.h"
#include "NANDDataDrive.h"


////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////
extern NAND_PHYSICAL_MEDIA_DESCRIPTOR NANDHAL_MEM g_NandDescriptor[];
extern NAND_PARAMETERS_DESCRIPTOR NANDHAL_MEM g_NandParams;
////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

//#define MAX_LOGICAL_DRIVES 1

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
// HACK!! These two belong in project_nand.c
//NAND_REGION_INFO   g_InternalNANDRegionInfoStructs[8];
//NAND_BBTABLE_ENTRY g_InternalNANDMasterBBTable[40];

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

// These things are stubs for the NAND HAL:
RETCODE NandHalStub(void);

NANDHAL_MEM NANDHAL_API_DESCRIPTOR NANDHALApiDescriptorStub = {
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub,
    0,NandHalStub
};
// Assumed physical layout:
//  4 Nands, each with 1024 blocks, 32 Pages/Block, 1 Sector/Page
//  All 4 Nands combine to make a single Data Drive
//  4 Regions, each Region assigned to a separate Nand
//  1000 HBlocks per Region, 12 UBlocks & 12 RBlocks.
//
//  Block Map:
//            Bad Blocks
//  Chip      HBlocks    UBlocks         RBlocks           Config Block
//  -------|---------------------------------------------|----------------
//    0       0,100,200, 1000,1004,1008, 1012,1016,1020    1
//    1       1,101,201, 1001,1005,1009, 1013,1017,1021    0
//    2       5,102,202, 1002,1006,1010, 1014,1018,1022    0
//    3       9,103,203, 1003,1007,1011, 1015,1019,1023    0
//           
#define NUM_BBTABLE_ENTRIES_PER_REGION  12
#define NUM_BAD_BLOCKS_REGION_0         9
#define NUM_BAD_BLOCKS_REGION_1         9
#define NUM_BAD_BLOCKS_REGION_2         9
#define NUM_BAD_BLOCKS_REGION_3         9
NAND_BBTABLE_ENTRY g_BBTbl[4*NUM_BBTABLE_ENTRIES_PER_REGION] =
{
/*
    0x255,0x4064,0x80C8,0xFFC3E8,0xFFC3EC,0xFFC3F0,0xFFC3F4,0xFFC3F8,0xFFC3FC,0,0,0,
    0x8001,0x65,0x40C9,0xFFC3E9,0xFFC3ED,0xFFC3F1,0xFFC3F5,0xFFC3F9,0xFFC3FD,0,0,0,
    0x4005,0x8066,0xCA,0xFFC3EA,0xFFC3EE,0xFFC3F2,0xFFC3F6,0xFFC3FA,0xFFC3FE,0,0,0,
    0x9,0x4067,0x80CB,0xFFC3EB,0xFFC3EF,0xFFC3F3,0xFFC3F7,0xFFC3FB,0xFFC3FF,0,0,0,
*/
    {0,1},{100,2},{200,3},{1000,-1},{1004,-1},{1008,-1},{1012,-1},{1016,-1},{1020,-1},{0,0},{0,0},{0,0},
    {1,3},{101,2},{201,0},{1001,-1},{1005,-1},{1009,-1},{1013,-1},{1017,-1},{1021,-1},{0,0},{0,0},{0,0},
    {5,3},{102,0},{202,1},{1002,-1},{1006,-1},{1010,-1},{1014,-1},{1018,-1},{1022,-1},{0,0},{0,0},{0,0},
    {9,1},{103,2},{203,0},{1003,-1},{1007,-1},{1011,-1},{1015,-1},{1019,-1},{1023,-1},{0,0},{0,0},{0,0}
};

#define NUM_LOGICAL_MEDIA 1

///////////////////////////////////////
// Setup the global Media structs
///////////////////////////////////////

LOGICAL_MEDIA_DESCRIPTOR g_MediaDescriptor[NUM_LOGICAL_MEDIA] =
{
    {
        0,          // wMediaNumber
        FALSE,      // bInitialized
        FALSE,      // bErased
        FALSE,      // bAllocated
        FALSE,      // bWriteProtected
        0,          // wNumberOfDrives
        0,          // dwSizeInBytes
        0,          // wAllocationUnitSizeInBytes
        0,          // PhysicalType
        0           // pMediaInfo
    }
};

LOGICAL_MEDIA g_Media[NUM_LOGICAL_MEDIA] = 
{
    0,0
//    &g_MediaDescriptor[0],&NandMediaApi
};

WORD g_wNumMedia = NUM_LOGICAL_MEDIA;

MEDIA_ALLOCATION_TABLE g_MediaAllocationTable[NUM_LOGICAL_MEDIA] =
{  4,
  0,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,16385,TRUE,
  1,DriveTypeData,DRIVE_TAG_DATA,100,FALSE,
  2,DriveTypeSystem,DRIVE_TAG_STMPSYS_S,16385,TRUE,
  3,DriveTypeSystem,DRIVE_TAG_HOSTLINK_S,16385,TRUE
};


///////////////////////////////////////
// Setup the global Drive structs
///////////////////////////////////////
// New global instead of just the MAX_LOGICAL_DRIVES define in driver lib
WORD g_wMaxLogicalDrives = MAX_LOGICAL_DRIVES; 

LOGICAL_DRIVE_DESCRIPTOR g_DriveDescriptor[MAX_LOGICAL_DRIVES];

WORD g_wNumDrives = NUM_LOGICAL_MEDIA;

LOGICAL_DRIVE g_Drive[MAX_LOGICAL_DRIVES] = 
{
  {&g_DriveDescriptor[0],&NANDSystemDriveApi},
  {&g_DriveDescriptor[1],&NANDDataDriveApi},
  {&g_DriveDescriptor[2],&NANDSystemDriveApi},
  {&g_DriveDescriptor[3],&NANDSystemDriveApi}

};


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

RETCODE NandHalStub(void)
{
    return SUCCESS;
}
