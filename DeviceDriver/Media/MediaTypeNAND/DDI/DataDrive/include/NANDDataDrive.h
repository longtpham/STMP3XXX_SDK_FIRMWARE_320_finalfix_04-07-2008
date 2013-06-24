////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename: NANDDataDrive.h
// Description: Contains OEM customizable flash driver options that were previously
//              in nandmediadefaults.c (see that file also)
////////////////////////////////////////////////////////////////////////////////

#ifndef _NANDDATADRIVE_H
#define _NANDDATADRIVE_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// You may have up to 4 NAND chips, used as a Concatenated MultiNAND or a Striped
// MultiNAND.  To have an external NAND drive, you must set MAX_NAND_CHIPS to less 
// than 4.  If you will never have more than "n" chips, set MAX_NAND_CHIPS to "n"
// to save RAM, unless this is MLC which needs high # of BB handling.
/////////////////////////////////////////////////////////////////////////////////
// Set to 1 to enable or 0 to disable. Only enable one of 2 defines below. 
// If the following 3 defines are all 0, config is auto set to 2 die per chipSelect & 4 chips max, 
// so 8 data drive regions (NAND4). 
// 46 words of data RAM used for each additional die's datadrive region & region array +)

// The 8 die config below uses 108 words of additional data RAM beyond 6 die config.  
#define SUPPORT_QUAD_DIE_NAND2          0  /* Untested but expected to function. 8 total die config for 2 Chip Enables  */
#define SUPPORT_OCTAL_DIE_NAND1         0  /* Only enable if you wish to support 8-die single-ChipEnable cases. */

#define EIGHTDIEMAX 1   /* to support 8 die total, uncomment this line or for 6 die max to save ram, comment out this line */
// ifdef allow 8 total die, else default limit to 6 die max to save ram
#ifdef EIGHTDIEMAX
#define LIMIT_TO_6_DIE_CONFIG_MAX       0  /* 8 total die supported */
#elif SUPPORT_QUAD_DIE_NAND2
#define LIMIT_TO_6_DIE_CONFIG_MAX       0  /* 8 total die supported */
#elif SUPPORT_OCTAL_DIE_NAND1
#define LIMIT_TO_6_DIE_CONFIG_MAX       0  /* 8 total die supported */
#else
// LIMIT_TO_6_DIE_CONFIG_MAX when defined as 1, supports a single quad die (1 Chip Enable), or 
// dual die in 1x/2x/3x physical config but not 4x physical config since 8 dice unsupported. 
#define LIMIT_TO_6_DIE_CONFIG_MAX       1  /* Emulate previous configs & uses no additional data RAM. */
#endif


// Note: if plib has hard coded references to this, you may need to use our 6 die max default above.
#if SUPPORT_QUAD_DIE_NAND2
 // If 4 die nand packages (or needs to be treated as one like toshiba w/ inter-zone page copy restriction), 
 // this supports 2 ChipEnables max to save ram. 
 #define MAX_NAND_CHIPS 2  /* 2 here gives 8 dice config max & 8 data drives for dice */
#elif SUPPORT_OCTAL_DIE_NAND1
 // OCTAL die       multinand not supported by default due to desired ram usage.
 #define MAX_NAND_CHIPS 1  /* 1 here gives 8 dice config max & 8 data drives for dice */
#else
 // default: 2 die packages can use 3 or 4 chips here (issue: ram amount used. SDK2.521 had MAX_NAND_CHIPS 4)
 #define MAX_NAND_CHIPS 4
#endif // since we added a hidden data drive and 1 system drive for sdk3.0 suggest 3 2die chips max instead of 4 to save ram.

// if the limit define above is disabled, nandmediadefaults auto multiplies macros (this file 3 pages down):
// numDiePerChip *  chosenMaxNumChipsForYramDesired   =  max num data drive regions resulting
//  2                   4     as previously.          =  8
//  4 (new)             2                             =  8
//  8 (new)             1                             =  8

/////////////////////////////////////////////////////////////////////////////////
//  Each chip may have one or more Data Regions.  One reason to
//  have more than one Data Region per chip is to allow MultiPlane addressing for
//  an 8-plane 128 MByte NAND. TODO UPDATE PREVIOUS COMMENT. Such a NAND is organized as two 4-plane blocks.
//  Making each block part of a different Region allows MultiPlane features to
//  be exploited.
//  Note that the value following is not used if LIMIT_TO_6_DICE_CONFIG_MAX is enabled.
//  A very few flash packages need 8 here since they have 8 die/districts/regions per chip enable. 
//  Sets 4 here if you enabled support of 2 quad-die configurations each with a chipEnable (multnand). 
//  Or default sets 2 for 2-die packages (except: toshiba 512MB w/ page copy restriction that needs 4 here)
#if SUPPORT_QUAD_DIE_NAND2
#define MAX_DATA_REGIONS_PER_CHIP 4
#elif SUPPORT_OCTAL_DIE_NAND1
#define MAX_DATA_REGIONS_PER_CHIP 8
#else
// default as in sdk 2.521 (most nand flash today have 2 die per ChipEnable (march 2005)
#define MAX_DATA_REGIONS_PER_CHIP 2 
#endif

////////////////////////////////////////////////////////////////////////////////////////
// A typical SDK3.0 system has the following System Drives:
//        bootmanager.sb    hostlink.sb   hostrsc.bin   StmpSys.sb  resource.bin
//        Note: updater.sb is not flashed & is only used during usb boot (firmware recovery mode). 
// Adding any vendor-specific drives will require increasing MAX_NAND_SYSTEM_DRIVES.
// There are 5 system drives in sdk3.x, 4 sys drives in sdk 2.521. 
#define MAX_NAND_SYSTEM_DRIVES 9 

////////////////////////////////////////////////////////////////////////////////////////
// A typical system will have just one hidden data Drives:
// Adding any vendor-specific drives will require increasing MAX_NAND_HIDDEN_DRIVES.
#define MAX_NAND_HIDDEN_DRIVES 1 

/////////////////////////////////////////////////////////////////////////////////
// The Master Bad Block Table (in RAM) must be allocated at compile-time.  Each
// Region on each NAND gets a portion of this table to track the Bad Blocks in
// that Region.  The actual size of the Region's portion depends on the actual
// number of Bad Blocks in that Region, plus other factors, so different Regions
// may get different size allocations.  The total size allocation, for the Master
// Bad Block Table allows for each NAND to get the following number of entries,
// "on average".  Your mileage may vary. 
/////////////////////////////////////////////////////////////////////////////////
// JLN: Toshiba MLC 512 MByte datasheet says <= 720 Bad Blocks guaranteed for new device. 
//      Toshiba MLC 256 MByte datasheet says <= 360 Bad Blocks guaranteed. 
//      Toshiba MLC 128 MByte datasheet says <= 180 Bad Blocks guaranteed. 
//                2x256 MByte                <= 720 Bad Blocks guaranteed. 
//                2x128 MByte                <= 360 Bad Blocks guaranteed.
// Note that that we instead specify quad blocks as bad if one of the quad is bad.
// 360 BB case is fully handled by BB per chip*4 below, but if you want to completely handle 
// the 720 bad quad block case, more y mem can be allocated here if you export more 
// data (y_extra) sections to elsewhere.
// To reduce data mem usage, set this define to 0 if you won't use MLC. Only used here currently.
// SDK 3.000 CAN NOT SUPPORT MLC SO LEAVE 0. 3.1 planned to support large block MLC aka type 5 flash.

//DS: Enabled the directive below, in order to increase the number of Bad Block entries. 
#define SUPPORT_MLC 1

#if SUPPORT_MLC
// Uses much data memory. Likely multiplied by 4 chips max when allocating actual BB table.
//the following are the number of Bad Block entries currently allocated for the player and mtp builds.
#ifdef PLAYER
#define MAX_AVG_NAND_BAD_BLOCKS_PER_CHIP 56 // 56*4=224  for 4 chips
#else
#define MAX_AVG_NAND_BAD_BLOCKS_PER_CHIP 76 // 76*4=304  for 4 chips
#endif
#else
#define MAX_AVG_NAND_BAD_BLOCKS_PER_CHIP 38 // 38*4=152 word 
#endif


/////////////////////////////////////////////////////////////////////////////////
//  FRACTION_RBLOCKS tells what minimal part of each Region should be allocated as 
//  Replacement Blocks - blocks used to substitute for Home Blocks (HBlocks) which 
//  are bad.  This number is typically something like 10/1024, i.e. reserve 10 
//  blocks out of every 1024.  This determines the initial number of RBlocks allocated
//  when there are few known Bad Blocks.  Later, MIN_SPARE_RBLOCKS_PER_REGION (below)
//  kicks in.
//  As Type 3 NANDs (MLC) typically need more RBlocks to function well, for
//  Type 3s, the number of RBlocks will be scaled up by TYPE3_RBLOCKS_SCALER.
//  This define allows Type 3s to be scaled up without punishing other NAND types
//  by forcing them to have more RBlocks than they need.
/////////////////////////////////////////////////////////////////////////////////
#define FRACTION_RBLOCKS 0.025
#define TYPE3_RBLOCKS_SCALER 6.7

/////////////////////////////////////////////////////////////////////////////////
//  MIN_SPARE_RBLOCKS_PER_REGION specifies the minimum number of "spare" RBlocks
//  get allocated for a Region when the number of known Bad Blocks in that Region,
//  plus MIN_SPARE_RBLOCKS_PER_REGION, is greater than FRACTION_RBLOCKS * RegionSize.
//
//  As the NAND ages and new Bad Blocks are discovered, the initial pool of Rblocks
//  (FRACTION_RBLOCKS * RegionSize) gets used up.  A new allocation will see that
//  there are few spare RBlocks (or none!), and will steal blocks from the HBlocks
//  to increase the RBlock pool.
//
//  The minimum spare RBlocks allocation will be scaled up by TYPE3_RBLOCKS_SCALER,
//  for Type 3 NANDs.
/////////////////////////////////////////////////////////////////////////////////
#define MIN_SPARE_RBLOCKS_PER_REGION 4

/////////////////////////////////////////////////////////////////////////////////
//  Each Data Drive Region has an Updates Table, listing all of the UBlocks and
//  their mappings/status.  The number of UBlocks of each Region is given in the
//  Configuration Block, on the NAND.  The total number of UBlocks there must be
//  less than or equal to the number of Data Drive Regions times the number given
//  below (AVG_UPDATE_BLOCKS_PER_REGION);  otherwise, DataDriveInit() will fail
//  and a full update (new MediaAllocate()) must be performed.  This should only
//  happen if the firmware is changed without re-allocating the NANDs.
//
//  As Type 3 NANDs (SB-MLC now unsupported) typically need more UBlocks to function well 
//  (as do type5 LB-MLC), for Type 3s, the number of UBlocks will be scaled up by TYPE3_UBLOCKS_SCALER.
//  This define allows Type 3s to be scaled up without punishing other NAND types
//  by forcing them to have more UBlocks than they need.
/////////////////////////////////////////////////////////////////////////////////
// more u block allocation. For 2 year player life.
#define AVG_UPDATE_BLOCKS_PER_REGION  31 

// This is unused in the SDK currently. 
#define TYPE3_UBLOCKS_SCALER 3.
/////////////////////////////////////////////////////////////////////////////////
// REVIEW: comment below appears to be in error:
//  AVG_UPDATE_BLOCKS_PER_REGION is good for drives that have blocks greater than
//  2*(AVG_UPDATE_BLOCKS_PER_REGION + MIN_SPARE_RBLOCKS_PER_REGION) but for drives
//  that are too small with fewer blocks we need another define for minimum blocks
//  per region.  (this is basically just for 8MByte and smaller flash or regions like the hidden drive)
/////////////////////////////////////////////////////////////////////////////////
#define MIN_UPDATE_BLOCKS_PER_REGION  MIN_SPARE_RBLOCKS_PER_REGION

/////////////////////////////////////////////////////////////////////////////////
//  Using MIN_UPDATE_BLOCKS_PER_REGION and MIN_SPARE_RBLOCKS_PER_REGION we can 
//  define minimum size required for a small size data drive.
/////////////////////////////////////////////////////////////////////////////////
#define MIN_DATA_DRIVE_SIZE ( 2 * ( MIN_UPDATE_BLOCKS_PER_REGION + MIN_SPARE_RBLOCKS_PER_REGION ) )

/////////////////////////////////////////////////////////////////////////////////
//  HIDDEN_DRIVE_PERCENTAGE gives the percentage of data drive to be allocated
//  for hidden drive.
/////////////////////////////////////////////////////////////////////////////////
//#define HIDDEN_DRIVE_PERCENTAGE (0.01) 

// Making the HIDDEN_DRIVE_PERCENTAGE 0 will cause it 
// to default to a minimum block size.  Right now that
// block size is 16 blocks.
#define HIDDEN_DRIVE_PERCENTAGE 0

/////////////////////////////////////////////////////////////////////////////////
//  Update Sectors Maps allocation
//  This is just a crude version, allowing for 6 maps of 256-Sector UBlocks.
/////////////////////////////////////////////////////////////////////////////////
#define NUM_OF_MAX_SIZE_USECTORS_MAPS 6
#define MAX_USECTORS_PER_BLOCK  256
#define MAX_WORDS_FOR_UPDATE_SECTOR_MAPS  (NUM_OF_MAX_SIZE_USECTORS_MAPS * ((MAX_USECTORS_PER_BLOCK+2)/3 + sizeof(UPDATE_SECTORS_MAP_DESCRIPTOR)))


/////////////////////////////////////////////////////////////////////////////////
//  Garbage Collector Configuration
//
/////////////////////////////////////////////////////////////////////////////////

#define NUM_FRESH_LISTEES 6



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//  SIGMATEL CUSTOMERS SHOULD NOT MODIFY ANYTHING BELOW THIS LINE *********
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
// Each chip will have MAX_DATA_REGIONS_PER_CHIP Data Drive Regions (e.g. 2 to 
// cover 8-plane 128MB NANDs).
// Each System Drive also uses one Region
/////////////////////////////////////////////////////////////////////////////////
// DO NOT CHANGE THIS. ENABLE/DISABLE DEFs AT TOP OF FILE TO SUPPORT 2, 4, OR 8 DIE FLASH.
// SDK3.0 IS CAN DEFAULT ABOVE TO ENABLE ONLY 6 DICE TOTAL OR A CALCULATED TOTAL SUCH AS 8. 
//    DON'T CHANGE THIS TO RM A WARNING UNLESS .ybssnandmediadefaults size is unchanged.
#if LIMIT_TO_6_DIE_CONFIG_MAX
// SDK3.0 default stays at 6 max data drive regions so no RAM growth                          
// 6 supports 3 dual-die ChipEnables max. Or 1 4-die configured nand.
#define MAX_DATA_DRIVE_REGIONS (                          6               ) 
#else
// sdk2.5x                            2                   *     4        =  8
// sdk3.0 default:                   {2,2,4,8}            *    {4,3,2,1} = {8,6,8,8}
#define MAX_DATA_DRIVE_REGIONS (MAX_DATA_REGIONS_PER_CHIP * MAX_NAND_CHIPS)
#endif

// sdk2.5x                               8       +           4            + 0                    =  12
// sdk3.0 default:                {8,6,8,8}      +           5            + 1                    = {14,12,14,14}
#define MAX_NAND_REGIONS (MAX_DATA_DRIVE_REGIONS + MAX_NAND_SYSTEM_DRIVES + MAX_NAND_HIDDEN_DRIVES) 



#define MAX_NAND_BAD_BLOCKS  (MAX_NAND_CHIPS * MAX_AVG_NAND_BAD_BLOCKS_PER_CHIP)

#if 1 
// update line: in sdk3.110 this change to no longer allocate ram for u block for sys drives (which don't use u blocks)
// is just a mem savings since we kept just 20 u blocks per drive region. 3.120 is higher. 
// New approach for sdk3.110 (and 2.610) excludes sys drives & allocates no more ram.                 
// This can give at least 50% more u blocks per region in the same RAM. In 3.120 & 2.610 more % increase above this is done in nandmediaallocate at stupdater time.
// HDS is said to be only 4 u blocks. Verify if 4 u blocks is enough. Some said it is written infrequently so probably ok. 
#define MAX_UPDATE_BLOCKS ((AVG_UPDATE_BLOCKS_PER_REGION*(MAX_DATA_DRIVE_REGIONS+MAX_NAND_HIDDEN_DRIVES))+MAX_DATA_DRIVE_REGIONS)
#else
// Add a constant MAX_DATA_DRIVE_REGIONS for growth.
#define MAX_UPDATE_BLOCKS ((AVG_UPDATE_BLOCKS_PER_REGION * MAX_NAND_REGIONS)+MAX_DATA_DRIVE_REGIONS)
#endif

#define NUM_BBTABLE_ENTRIES_PER_REGION  12
#define NUM_BAD_BLOCKS_REGION_0         9
#define NUM_BAD_BLOCKS_REGION_1         9
#define NUM_BAD_BLOCKS_REGION_2         9
#define NUM_BAD_BLOCKS_REGION_3         9

///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

#include "errors.h"


///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    P_NAND_REGION_INFO pRegion;
    WORD wUBlkAddr;
} ASYNC_ERASE_CALLBACK_PARAMS, * P_ASYNC_ERASE_CALLBACK_PARAMS;

    #ifdef NDD_STRESS_TEST
// This is used by the nand data drive stress test.
// It is externed in nddAddBlkToBBTbl.c
typedef struct {
    WORD wNumBadBlocks;
    WORD wBadBlockAddr[50];
    WORD wChip[50];
} STRESS_TEST_UPDATE_BB, * P_STRESS_TEST_UPDATE_BB;
    #endif

/////////////////////////////////////////////////////////////////////////////////
//  UPDATE SECTORS MAPS
//
//  An array of Update Sector Maps are shared by all Data Drive Regions, to hold
//  a mapping between HBlock Sectors and UBlock Sectors, in RAM, for HBlocks which 
//  have open Updates.
//
//  Some Update Sectors Maps can consume a large amount of memory (depending on the
//  number of Sectors per Block), so we have only a limited number of them.  There 
//  may be many more Open Updates than Update Sectors Maps, so only a few Updates 
//  are mapped, in RAM, at any one time.  Other Updates are mapped only by their 
//  Logical Block Address (LBA) and Logical Sector Address (LSA) fields in the 
//  Redundant Areas of the USectors on NAND.  
//
//  When a needed Update Sectors Map is not in RAM, it can built built, on demand,
//  by reading the Redundant Areas off NAND and filling in one of the shared Update 
//  Sectors Maps in the RAM array.  (Another map may have to be discarded to make
//  room;  it can be rebuilt when needed.  Obviously, you want to have as many
//  Update Sectors Maps in RAM as you can afford, to reduce "thrashing".)
//
//  An Update Sectors Map in the array is associated with a particular Update through
//  the pRegion and iUBlkIdx elements.  These identify the Region and give the 
//  index of the UBlock used for the Update.  To find the Updates Sector Map for a 
//  particular Update for a given Region, you have to search the shared Update 
//  Sectors Maps for a match;  there is no forward pointer (from the Updates Table), 
//  to save memory.  Since there are a small number of Update Sectors Maps, this 
//  should not be too many CPU cycles.  
//
//  Note the Updates Table gets resorted whenever an Update gets opened or closed, 
//  so the index into the Updates Table is not a robust way to link the USectors Maps 
//  to the Updates Table.  Instead, we use the UBlock index, which is fixed until the 
//  Update gets closed.  Setting the Block Index to USECTORS_MAP_NOT_MAPPED clears the
//  USectorMap entry.
//
//  pSectorsMap points at the actual Sector mappings.  It is an array of WORDS, but
//  it is accessed on a byte basis, using a "mod 3" function.  Each byte, by index,
//  corresponds to a particular HSector.  The contents of the byte is either a USector
//  index or iSectorsPerBlock (which is greater than any USector index).  A value of
//  iSectorsPerBlock (MAX+1) means that the HBlock has not been updated.
//
//  USectors are written sequentially into the UBlock, and iUpdatedSectorsCnt keeps 
//  track of how many have been written (and what the next write index is).
//
//  iSectorsPerBlock will be a power of two, ranging from 32 to 256.
//
//  The Sector Maps can be defined, dynamically, from a fixed-sized reserved area,
//  such that the number of maps depends on the Sectors per Block.  For large blocks,
//  you'd have a small number of maps, but for small blocks, you could have more
//  maps.  More maps means less "trashing".
//
//  The DataDrive needs to keep a pointer to the array of Sector Maps, and it probably 
//  defines them, in DataDriveInit.
//
/////////////////////////////////////////////////////////////////////////////////

#define USECTORS_MAP_FREE  (0xffffff)

// Using the last valid index here is ok because we never completely fill an update block
#define USECTORS_MAP_MAX_SECTOR_INDEX (0xFF)
#define USECTORS_THREE_UNMAPPED_SECTOR_INDICES (0xFFFFFF)

typedef WORD * UPDATE_SECTORS_MAP_BYTES, * P_UPDATE_SECTORS_MAP_BYTES;

typedef struct {

  P_NAND_REGION_INFO       pRegion;                // Which Region and
  WORD                     wUBlkIdx;               // which Update Block this Map goes with OR USECTORS_MAP_NOT_MAPPED

  int                      iUpdatedSectorsCnt;     // The number of actual USectors that have been 
                                                   // written to the UBlock.  They are written 
                                                   // sequentially, so this is also the index for
                                                   // the next USector write

  P_UPDATE_SECTORS_MAP_BYTES pSectorsMapBytes;     // array of Update Sector Mappings

} UPDATE_SECTORS_MAP_DESCRIPTOR, * P_UPDATE_SECTORS_MAP_DESCRIPTOR;






typedef struct {
  WORD wNumRegions;
  P_NAND_REGION_INFO *ppRegion;
    
  WORD iNumUSectorsMaps;
  P_UPDATE_SECTORS_MAP_DESCRIPTOR pUSectorsMaps;
  int iLastUSectorsMapIdxTouched;

} NAND_DATA_DRIVE_INFO, * P_NAND_DATA_DRIVE_INFO;


    #ifdef NDD_COUNT_ROUTINES
typedef struct {
    WORD AddUSector;
    WORD BuildUSectorsMap;
    WORD CloseUpdate;
    WORD CloseUpdate_PreserveHSectors;
    WORD GetFreeErasedUBlock;
    WORD GetRecycledUBlock;
    WORD GetUSectorsMap;
    WORD KillUpdate;
    WORD KillUSector;
    WORD LookupUBlock;
    WORD LookupUSector;
    WORD ProtectUBlock;
    WORD RecoverBadUBlock;
    WORD StartNewUpdate;
    WORD AddBlkToBBTbl;
    WORD CopySectors;
    WORD GetFreeErasedRBlock;
    WORD LookupRBlock;
    WORD RecoverBadHBlock;
    WORD RecoverBadRBlock;
    WORD SearchBBTableForPhysBlock;
    WORD write_in_place;
    WORD pin_allocation;
} NDD_ROUTINE_COUNT, * P_NDD_ROUTINE_COUNT;
    #endif



///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NANDDataDriveInit(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor);

RETCODE _reentrant NANDDataDriveGetInfo(
    P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    LOGICAL_DRIVE_INFO Type,
    void * pInfo);

RETCODE NANDDataDriveConvertLogicalToRelative(
    P_NAND_DATA_DRIVE_INFO pDriveInfo,
    DWORD dwLogicalSectorNum,
    P_NAND_REGION_INFO *hRegion,
    WORD *pwRelativeSectorNum);
    
RETCODE NANDDataDriveConvertRelativeToPhysical(
    P_NAND_REGION_INFO pRegion,
    WORD wRelativeSectorNum,
    WORD *pwHBlockAddr,
    WORD *pwHSectorIdx);
    
RETCODE NANDDataDriveLinearConvertRelativeToPhysical(
    P_NAND_REGION_INFO pRegion,
    WORD wRelativeSectorNum,
    WORD *pwHBlkAddr,
    WORD *pwHSectorIdx);
    
RETCODE NANDDataDriveMPConvertRelativeToPhysical(
    P_NAND_REGION_INFO pRegion,
    WORD wRelativeSectorNum,
    WORD *pwHBlkAddr,
    WORD *pwHSectorIdx);
    
RETCODE NANDDataDriveLookupRBlock(
    P_NAND_REGION_INFO pRegion,
    WORD HBlkAddr,
    WORD *pRBlkAddr);
    
RETCODE NANDDataDriveSearchBBTableForPhysBlock(
    P_NAND_REGION_INFO pRegion,
    WORD wPhysBlkNum, 
    WORD *pwBBIdx);

RETCODE NANDDataDriveGetFreeErasedRBlock(
    P_NAND_REGION_INFO pRegion,
    WORD wOldBlockAddr,
    WORD * pwRBlockAddr);

RETCODE NANDDataDriveLinearGetFreeErasedRBlock(
    P_NAND_REGION_INFO pRegion,
    WORD wOldBlockAddr,
    WORD * pwRBlockAddr);
    
RETCODE NANDDataDriveMPGetFreeErasedRBlock(
    P_NAND_REGION_INFO pRegion,
    WORD wOldBlockAddr,
    WORD * pwRBlockAddr);

RETCODE NANDDataDriveAddBlkToBBTbl(
    P_NAND_REGION_INFO pRegion,
    int iBlkAddr,
    int iMapping);


RETCODE NANDDataDriveRecoverBadHBlock(
    P_NAND_REGION_INFO pRegion,
    WORD wHBlockAddr,
    WORD wNumSectors,
    WORD *pwNewRBlockAddr);

RETCODE NANDDataDriveRecoverBadRBlock(
    P_NAND_REGION_INFO pRegion,
    WORD wHBlockAddr,
    WORD wRBlockAddr,
    WORD wNumSectors,
    WORD *pwNewRBlockAddr);

RETCODE NANDDataDriveEraseBlock(
    P_NAND_REGION_INFO pRegion, 
    WORD wBlockAddr);

RETCODE _reentrant NANDDataDriveWriteSector(
    P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwSectorNumber,
    P_SECTOR_BUFFER pSectorData);

RETCODE _reentrant nddWriteSector(
    P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwSectorNumber,
    P_SECTOR_BUFFER pSectorData);

//#ifdef k_opt_performance_enhancement
// this perf opt code block stays on even when perf opt is disabled.
// the reason is to keep the stub of ndd_write_sector_in_place
// in the build to prevent having to build with different .dsc files.
RETCODE _reentrant ndd_write_sector_in_place(
                                            P_LOGICAL_DRIVE_DESCRIPTOR p_descriptor,
                                            DWORD dw_sector_number,
                                            P_SECTOR_BUFFER p_sector_data);
//#endif // k_opt_performance_enhancement


RETCODE _reentrant NANDDataDriveReadSector(
    P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
    DWORD dwSectorNumber,
    P_SECTOR_BUFFER pSectorData);


RETCODE NANDDataDriveCopySectors(P_NAND_REGION_INFO pRegion, 
                                 WORD wSrcBlkAddr, 
                                 WORD wSrcSectorIdx, 
                                 WORD wDestBlkAddr, 
                                 WORD wDestSectorIdx, 
                                 WORD wNumSectorsToCopy);

_reentrant BOOL NANDDataDriveAsyncEraseUBlkCallback(int iStatus, void *pObj);

// Garbage Collecotr API
RETCODE NANDForegroundGarbageCollector(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);
RETCODE NANDBackgroundGarbageCollector(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);
RETCODE NANDBackgroundGarbageCollectorEnable(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);
RETCODE NANDBackgroundGarbageCollectorDisable(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);



///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
extern BOOL g_bMultiWriteStarted;
extern NAND_DATA_DRIVE_INFO g_NANDDriveInfo[];



extern NAND_REGION_INFO                g_aInternalNANDRegionInfoStructs[];
extern int                             g_iMaxNANDRegions;

extern P_NAND_REGION_INFO              g_apInternalDataDriveRegions[];
extern P_NAND_REGION_INFO              g_apInternalHiddenDriveRegion[];
extern int                             g_iMaxNANDDataDriveRegions;

extern NAND_BBTABLE_ENTRY              g_aInternalNANDMasterBBTable[];
extern int                             g_iMaxNANDBadBlocks;

extern NAND_UPDATES_ENTRY              g_aInternalNANDMasterUpdatesTable[];
extern int                             g_iMaxNANDUpdateBlocks;

extern WORD                            g_UpdateSectorsMapsMemory[];
extern int                             g_iMaxWORDsForUpdateSectorsMaps;

    #ifdef NDD_COUNT_ROUTINES
extern NDD_ROUTINE_COUNT               g_NddRoutineCount;
    #endif

/////////////////////////////////////////////////////////////////////////////////
//  TEST CODE
/////////////////////////////////////////////////////////////////////////////////

typedef enum {
  USECTORS_TEST_NO_SECTORS,
  USECTORS_TEST_FIRST_HSECTOR,
  USECTORS_TEST_LAST_HSECTOR,
  USECTORS_TEST_MIDDLE_HSECTOR,
  USECTORS_TEST_ALL_HSECTORS,
  USECTORS_TEST_HALF_HSECTORS,
  USECTORS_TEST_FAIL
} USECTORS_TEST;

// TT - use MJ's method to debug MultiWrites. 
// JN: Change default to undefined by commenting line below.
//  #define DEBUG_MULTIWRITE 1
  #ifdef DEBUG_MULTIWRITE
    #define DebugMultiWrite() SystemHalt()
  #else
    #define DebugMultiWrite()
  #endif

extern USECTORS_TEST g_eUSECTORS_TEST;
extern ASYNC_ERASE_CALLBACK_PARAMS g_AsyncEraseCallBackParam;

// Must be a power of 2
#define MAX_MULTI_WRITE_INFO    8
#define MULTI_WRITE_FIFO_MODULO (MAX_MULTI_WRITE_INFO-1)

typedef enum {
    MultiWriteBlockTypeHBlock = 0,
    MultiWriteBlockTypeUBlock = 1,
    MultiWriteBlockTypeRBlock = 2
} MULTI_WRITE_BLOCK_TYPE;
    
typedef enum {
    MultiWritePipelineSectorFree = 0,
    MultiWritePipelineSectorEccInProgress = 1,
    MultiWritePipelineSectorDmaInProgress = 2,
    MultiWritePipelineSectorProgramInProgress = 3,
    MultiWritePipelineSectorReleasePending = 4
} MULTI_WRITE_SECTOR_STATUS, * P_MULTI_WRITE_SECTOR_STATUS;

typedef struct {
    P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor;
    P_NAND_MEDIA_INFO pNandMediaInfo;
    P_NAND_DATA_DRIVE_INFO pNandDriveInfo;
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc;
    P_NAND_PARAMETERS_DESCRIPTOR pNANDParms;
    P_NANDHAL_API_DESCRIPTOR pNandHalApi;
    DWORD dwSectorNumber;
    P_SECTOR_BUFFER pSectorData;
    LONG lCallBackParameter;
    // also need the block number?
    MULTI_WRITE_SECTOR_STATUS eStatus;
    RETCODE rtCode;
} MULTI_WRITE_INFO, * P_MULTI_WRITE_INFO;

typedef RETCODE (*DUMB_MULTIWRITE_PIPELINE_FUNCTION)(void);

//#define DEBUG_MWRITE_CACHE 1
#ifdef DEBUG_MWRITE_CACHE
    #define DebugMwriteCache() SystemHalt()
#else
    #define DebugMwriteCache()
#endif

RETCODE _reentrant NANDDataDriveRecoverMultiWriteBlock(void);
RETCODE _reentrant MultiWriteSectorRelease(
    WORD wOffset,
    RETCODE rtCode);
RETCODE _reentrant MultiWriteCreateSectorInfo(P_MULTI_WRITE_INFO *hMultiWriteSectorInfo);
RETCODE _reentrant MultiWriteRemoveSectorInfo(void);
RETCODE _reentrant MultiWriteFetchSectorInfo(
    WORD wOffset,
    P_MULTI_WRITE_INFO * hMultiWriteSectorInfo);
RETCODE _reentrant MWDCleanupFatalError(
    BOOL bCallBacks,
    LONG lCallBackParam);

#endif // #ifndef _NANDDATADRIVE_H
