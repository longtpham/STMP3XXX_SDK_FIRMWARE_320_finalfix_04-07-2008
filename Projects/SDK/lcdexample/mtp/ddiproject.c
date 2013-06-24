////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename: ddiproject.c
// Description: 
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "ddildl.h"
#include "NANDddi.h"
#include "NANDHal.h"
#include "NANDDataDrive.h"
#ifdef MMC
  #include "MMCDataDrive.h"
  #include "MMCddi.h"
  #include "regsgpio.h"
  #include "mediadetection.h"
#endif

#ifdef USE_PLAYLIST3_HOST
#include "persist_bits.h"	//added by samuel on 31/1/05 for flash write detection
#endif
////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
WORD g_wHostResourceTag = DRIVE_TAG_HOSTRSC_BIN;
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
		FALSE,					// bAllocated
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
		FALSE,					// bAllocated
        FALSE,                  // bWriteProtected
        1,                      // wNumberOfDrives
        0,                      // dwSizeInBytes
        0,                      // wAllocationUnitSizeInBytes
        MediaTypeMMC,           // PhysicalType
        (void *)(&MmcMediaInfo) // pMediaInfo
    }
#endif
};


MEDIA_ALLOCATION_TABLE g_MediaAllocationTable[NUM_LOGICAL_MEDIA] =
{
    {
        7,
        0,DriveTypeData,0,50176,FALSE,
        1,DriveTypeData,0,50176,FALSE,            
        2,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,FALSE,
        3,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,FALSE,
        4,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,FALSE,
        5,DriveTypeSystem,DRIVE_TAG_RESOURCE_BIN,50176,FALSE,
        6,DriveTypeSystem,DRIVE_TAG_HOSTRSC_BIN,50176,FALSE
    }
#ifdef MMC
    ,
    {
        1,
        0,DriveTypeData,0,50176,FALSE,
    }
#endif
};

///////////////////////////////////////
// Setup the global Media structs
///////////////////////////////////////

WORD g_wNumMedia = NUM_LOGICAL_MEDIA;

///////////////////////////////////////
// Setup the global Drive structs
///////////////////////////////////////
// New global instead of just the MAX_LOGICAL_DRIVES define in driver lib, allows 
// required customization by making previous macro refs in libs now use global. March 1 2005
WORD g_wMaxLogicalDrives = MAX_LOGICAL_DRIVES; 

LOGICAL_DRIVE_DESCRIPTOR g_DriveDescriptor[MAX_LOGICAL_DRIVES];

WORD g_wNumDrives = NUM_LOGICAL_MEDIA+1;
WORD _X g_wHiddenDriveIdx = NUM_LOGICAL_MEDIA;
INT g_JanusDrive = -1;
WORD g_protectedMode = 0;


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

#ifdef USE_PLAYLIST3_HOST
#define STORED_BIT_VALUE_MASK		(1)
#define DISABLE_DETECTION_MASK	(2)
#define STORED_BIT_VALID_MASK		(4)

int g_FlashWriteOccurFlag = 0;
#endif
////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#ifdef USE_PLAYLIST3_HOST
/*
 *access the write detection flag
 *(option = [0,1,2,3,4]->[read the flag, set the flag, clear the flag,disable detection, enable detection])
 *return current bit value
 */ 
_reentrant int write_detection_access(int option)
{

	RETCODE returnCode;
	BOOL bBitResult;
	int return_value = -1;

	//if disabled detection, nothing will be changed
	if( (g_FlashWriteOccurFlag  & DISABLE_DETECTION_MASK) && (option != 4) )
	{
		//return the stored bit no matter valid or not
			option = 0;
	}

	//detect the bit value and set the return_value variable if valid
	if(g_FlashWriteOccurFlag  & STORED_BIT_VALID_MASK)
	{
		if(g_FlashWriteOccurFlag & STORED_BIT_VALUE_MASK)
			return_value = 1;
		else
			return_value = 0;
	}
	
	switch(option)
	{
		case 0:	//read
			// bit stored is valid? if yes, just return it
			if(!(g_FlashWriteOccurFlag  & STORED_BIT_VALID_MASK))
			{
				//check whether a write to flash occurred before, if yes, rebuild the library
				returnCode = ReadStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG,
										HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS,
										&bBitResult);
			
				if(returnCode == SUCCESS)	//able to read the bit
				{
					if(bBitResult)
						return_value = 1;
					else
						return_value = 0;
				}
				else
					return_value = 1;		//if unable to read the bit, treat it as written before
			}
			break;
					
		case 1:	//set the flag -> clear the bit
			if(return_value != 0)
			{
				//Even if hardware is not able to record the bit, nothing can do!
				ClearStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG, 
							HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS);		
				return_value = 0;
			}		
			break;
			
		case 2:	//clear the flag -> set the bit
			if(return_value != 1)
			{
				//Even if hardware is not able to record the bit, nothing can do!
				SetStickyBit((volatile _X WORD *)&HW_RTC_SKIP_REBUILD_REG, 
							HW_RTC_PERSISTENT1_SKIP_REBUILD_BITPOS);
				return_value = 1;
			}
			break;	
					
		case 3:	//disable
			g_FlashWriteOccurFlag |= DISABLE_DETECTION_MASK;
			break;
			
		case 4:	//enable
			g_FlashWriteOccurFlag &= (~DISABLE_DETECTION_MASK);
			break;		
	}
	
	//set the valid bit and set the stored bit value at the same time
	g_FlashWriteOccurFlag |= (STORED_BIT_VALID_MASK | STORED_BIT_VALUE_MASK);

	//by the return value, we set the correct value of the stored bit
	if(return_value == 0)
		g_FlashWriteOccurFlag &= (~STORED_BIT_VALUE_MASK);

	//if the bit is set -> flag is off
	//if the bit is clear -> flag is on
	if(return_value != 0)
		return 0;
	else
		return 1;
}
#else
_reentrant int write_detection_access(int option)
{
    return 0;
}
#endif
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