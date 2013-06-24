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
//#include "RAMDdi.h"
//#include "RAMHal.h"
#include "NANDddi.h"

#include "NANDDdi.h"
#include "NANDHal.h"
#include "NANDDataDrive.h"

#ifdef MMC
  #include "MMCDataDrive.h"
  #include "MMCddi.h"

  #include "regsgpio.h"
  #include "mediadetection.h"
#endif

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////


#ifndef MMC
    #define NUM_LOGICAL_MEDIA       1
#else
    #define NUM_LOGICAL_MEDIA       2     // 1 external drive if MMC.
#endif

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
    &g_MediaDescriptor[0],&NANDMediaApi
#ifdef MMC
    ,&g_MediaDescriptor[1],&MMCMediaApi
#endif
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
    }
#ifdef MMC
    ,
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
#endif
};



MEDIA_ALLOCATION_TABLE g_MediaAllocationTable[NUM_LOGICAL_MEDIA] =
{
    {
        7,
        1,DriveTypeData,DRIVE_TAG_DATA,0,FALSE,
        2,DriveTypeHidden,DRIVE_TAG_DATA_HIDDEN,0x2DD2,FALSE,      
        0,DriveTypeSystem,DRIVE_TAG_BOOTMANAGER_S,0x2DD2,FALSE,
        3,DriveTypeSystem,DRIVE_TAG_HOSTLINK_S,0x449E2,FALSE,
        4,DriveTypeSystem,DRIVE_TAG_HOSTRSC_BIN,0x3D012,FALSE,
        5,DriveTypeSystem,DRIVE_TAG_STMPSYS_S,0x32244,FALSE,
        6,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,0x4980,FALSE    
    }
#ifdef MMC
    ,
    {
        1,
        0,DriveTypeData,0,50176,FALSE,
    }
#endif
};


// Each Resource sector is actually 510 bytes
#define RAM_SYSTEM_DRIVE_1_NUM_SECTORS  99

// Assign drive labels.  In typical applications there will not be a Data Drive
//  and there will only be a single System Drive, typically containing 
//  resource.bin.  In that case you would use
//      extern Resource
//      INT * g_pRamSystemDrive0 = &Resource;
//  where Resource is defined in Resource.asm.

//extern Resource;
//
//RAM_MEDIA_ALLOCATION_TABLE g_RAMMediaAllocation[NUM_RAM_MEDIA] = 
//{
//    {
//        2,
//        {
//            {DriveTypeData,DRIVE_TAG_DATA,1,RAM_DATA_DRIVE_0,(void *)(&Resource)},    // Do not need this one
//            {DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,RAM_SYSTEM_DRIVE_1_NUM_SECTORS,RAM_SYSTEM_DRIVE_0,(void *)(&Resource)}
//        }
//    }
//};



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

WORD g_wNumDrives      = NUM_LOGICAL_MEDIA+1;
WORD _X g_wHiddenDriveIdx = NUM_LOGICAL_MEDIA;

LOGICAL_DRIVE g_Drive[MAX_LOGICAL_DRIVES]=
{
    {&g_DriveDescriptor[0],&NANDDataDriveApi},
#ifdef MMC
    {&g_DriveDescriptor[1],&MMCDataDriveApi},
#endif
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA],&NANDDataDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+1],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+2],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+3],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+4],&NANDSystemDriveApi},
    {&g_DriveDescriptor[NUM_LOGICAL_MEDIA+5],&NANDSystemDriveApi}
};

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// MMC & SPI data structures
////////////////////////////////////////////////////////////////////////////////

// The following structure allocations & initializations are based on
//      MMC_MAX_PHYSICAL_DEVICES which is defined in
//      project.h.  If the user desires a different number of devices
//      then project.h must be made local & modified.  The
//      structure initializations will auto adjust.  The max numbers supported
//      here are:
//
//      MMC_MAX_PHYSICAL_DEVICES max = 2
//
//      If the user requires more than this number then this file, mmcsalsstructs.c &
//      appropriate bus structs file (most likely spistructs.c) must be made
//      local to the project and customized to add more devices.
//
// The chip select pins are pre-defined here to be pins A4/CE3 for device 0 and
//      A5/CE2 for device 1.  If the user requires different chip selects then
//      this file must be made local to the project & customized.  If using the
//      SPI bus then the spistructs.c file must also be made local & customized.
//
// The write-protect pins are pre-defined here to be pins A7 for device 0 and
//      IORDN for device 1.  If the user requires different pins then
//      this file must be made local to the project & customized.
//
// The media detect pins are pre-defined here to be pins A0 for device 0 and 
//      IOWRN for device 1.  If the user requires different pins then
//      this file must be made local to the project & customized.

// HAL data structures
#ifdef MMC
MMC_PHYSICAL_BUS MmcPhysicalBus[MMC_MAX_PHYSICAL_DEVICES] =
                    {
                        {
                            (_X WORD *)HW_GPB1_BASEADDR,    // Chip select
                            HW_GP_B12_SETMASK,
                            HW_GP_B12_CLRMASK,
                            (_X WORD *)HW_GPB2_BASEADDR,    // Write protect
                            HW_GP_B4_SETMASK,
                            HW_GP_B4_CLRMASK,
                            (_X WORD *)HW_GPB1_BASEADDR,    // Media detect
                            HW_GP_B8_SETMASK,
                            HW_GP_B8_CLRMASK,
                            MEDIA_DETECT_NEGATIVE_POLARITY,
                            0                               // SPI Device num
                        }
#if (MMC_MAX_PHYSICAL_DEVICES > 1)
                        ,
                        {
                            (_X WORD *)HW_GPB1_BASEADDR,    // Chip select
                            HW_GP_B13_SETMASK,
                            HW_GP_B13_CLRMASK,
                            (_X WORD *)HW_GPB1_BASEADDR,    // Write protect
                            HW_GP_B9_SETMASK,
                            HW_GP_B9_CLRMASK,
                            (_X WORD *)HW_GPB2_BASEADDR,    // Media detect
                            HW_GP_B3_SETMASK,
                            HW_GP_B3_CLRMASK,
                            MEDIA_DETECT_NEGATIVE_POLARITY,
                            0                               // SPI Device num
                        }
#endif
                    };
#endif      

/////////////////////////////////////////////////////////////////////////////////
//  TEST CODE GLOBAL FLAGS
/////////////////////////////////////////////////////////////////////////////////

USECTORS_TEST g_eUSECTORS_TEST = USECTORS_TEST_NO_SECTORS;

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

