/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: project_nand.c
// Description: Project-specific definitions and declarations for 
//              Internal NAND Media DDI
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
// You may have up to 4 NAND chips, used as a Concatenated MultiNAND or a Striped
// MultiNAND.  To have an external NAND drive, you must set MAX_NAND_CHIPS to less 
// than 4.  If you will never have more than "n" chips, set MAX_NAND_CHIPS to "n"
// to save RAM.
/////////////////////////////////////////////////////////////////////////////////

#define MAX_NAND_CHIPS   4




/////////////////////////////////////////////////////////////////////////////////
// A typical system has the followign System Drives:
//        bootmanager
//        USBMSC
//        StmpSys
//        resource.bin
// Adding any vendor-specific drives will require increasing MAX_NAND_SYSTEM_DRIVES.
/////////////////////////////////////////////////////////////////////////////////

#define MAX_NAND_SYSTEM_DRIVES 4



/////////////////////////////////////////////////////////////////////////////////
// The Master Bad Block Table (in RAM) must be allocated at compile-time.  Each
// Region on each NAND gets a portion of this table to track the Bad Blocks in
// that Region.  The actual size of the Region's portion depends on the actual
// number of Bad Blocks in that Region, plus other factors, so different Regions
// may get different size allocations.  The total size allocation, for the Master
// Bad Block Table allows for each NAND to get the following number of entries,
// "on average".  Your mileage may vary.
/////////////////////////////////////////////////////////////////////////////////

#define MAX_AVG_NAND_BAD_BLOCKS_PER_CHIP 50



/////////////////////////////////////////////////////////////////////////////////
//  Each chip may have one or more Data Regions.  At present, the only reason to
//  have more than one Data Region per chip is to allow MultiPlane addressing for
//  an 8-plane 128 MByte NAND.  Such a NAND is organized as two 4-plane blocks.
//  Making each block part of a different Region allows MultiPlane features to
//  be exploited.
/////////////////////////////////////////////////////////////////////////////////

#define MAX_DATA_REGIONS_PER_CHIP 2



/////////////////////////////////////////////////////////////////////////////////
//  Each Data Drive Region has an Updates Table, listing all of the UBlocks and
//  their mappings/status.  The number of UBlocks of each Region is given in the
//  Configuration Block, on the NAND.  The total number of UBlocks ther must be
//  less than or equal to the number of Data Drive Regions times the number given
//  below (AVG_UPDATE_BLOCKS_PER_REGION);  otherwise, DataDriveInit() will fail
//  and a full update (new MediaAllocate()) must be performed.  This should only
//  happen if the firmware is changed without re-allocating the NANDs.
/////////////////////////////////////////////////////////////////////////////////

#define AVG_UPDATE_BLOCKS_PER_REGION  15



/////////////////////////////////////////////////////////////////////////////////
//  Update Sectors Maps allocation
//
//  This is just a crude version, allowing for 6 maps of 256-Sector UBlocks.
/////////////////////////////////////////////////////////////////////////////////

#define NUM_OF_MAX_SIZE_USECTORS_MAPS 6
#define MAX_USECTORS_PER_BLOCK  256
#define MAX_WORDS_FOR_UPDATE_SECTOR_MAPS  (NUM_OF_MAX_SIZE_USECTORS_MAPS * ((MAX_USECTORS_PER_BLOCK+2)/3 + sizeof(UPDATE_SECTORS_MAP_DESCRIPTOR)))







/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//  SIGMATEL CUSTOMERS SHOULD NOT MODIFY ANYTING BELOW THIS LINE
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
// Each chip will have MAX_DATA_REGIONS_PER_CHIP Data Drive Regions (e.g. 2 to 
// cover 8-plane 128MB NANDs).
// Each System Drive also uses one Region
/////////////////////////////////////////////////////////////////////////////////
#define MAX_DATA_DRIVE_REGIONS (MAX_DATA_REGIONS_PER_CHIP * MAX_NAND_CHIPS)
#define MAX_NAND_REGIONS (MAX_DATA_DRIVE_REGIONS + MAX_NAND_SYSTEM_DRIVES) 

#define MAX_NAND_BAD_BLOCKS  (MAX_NAND_CHIPS * MAX_AVG_NAND_BAD_BLOCKS_PER_CHIP)

#define MAX_UPDATE_BLOCKS (AVG_UPDATE_BLOCKS_PER_REGION * MAX_NAND_REGIONS)





#ifdef FOO

//  This structure is no longer global.  It is static to MediaDiscoverAllocation(),
//  which puts a pointer to it in the NAND_MEDIA_INFO structure for the Internal
//  NAND DDI.


/////////////////////////////////////////////////////////////////////////////////
// The following is the NAND_MEDIA_INFO struct, which describes the Internal NAND 
// Media (only one exists).
//
// It will be pointed to by the LOGICAL_MEDIA_DESCRIPTOR, so the name is CRITICAL.
/////////////////////////////////////////////////////////////////////////////////

NAND_MEDIA_INFO g_InternalNANDMediaInfo = {

  NANDStateUnknown,                    // Initial state of NAND Media

  0,                                   // iNumNANDChips, initially  (filled in by MediaInit)
  s_InternalNANDDescriptors,           // array of NAND Descriptors (one per chip)(filled in by MediaInit)
  s_ConfigBlkAddrs,                    // array of Config Block addresses (one per chip)(filled in by MediaDiscover)

  0,                                   // iNumRegions, initially (filled in by MediaDiscover)
  s_InternalNANDRegionInfoStructs,     // array of Region Info structs (filled in by MediaDiscover)

  0,                                   // iNumBadBlocks, initially (filled in by MediaDiscover)
  s_InternalNANDMasterBadBlocksTable   // array of BadBlock Table entries (filled in by MediaDiscover)
};
#endif



/////////////////////////////////////////////////////////////////////////////////
// The global arrays below are referenced by MediaDiscoverAllocation(), and 
// pointers to them  are stuffed into the NAND_MEDIA_INFO structure for the Internal 
// NAND DDI. Their names are CRITICAL.
//
// The other globals below make the project-specific MAX sizes available to the 
// library routines.  Their names are CRITICAL.
/////////////////////////////////////////////////////////////////////////////////
/* The following have moved to nandhalproject.c:

NAND_PHYSICAL_MEDIA_DESCRIPTOR         g_aInternalNANDDescriptors[MAX_NAND_CHIPS];
int                                    g_iMaxNANDChips = MAX_NAND_CHIPS;

*/

/* The following was folded into the MEDIA_INFO structure, always having 4 entires.
int                                    g_aiConfigBlkPhysAddr[MAX_NAND_CHIPS];
*/



NAND_REGION_INFO                       g_aInternalNANDRegionInfoStructs[MAX_NAND_REGIONS];
int                                    g_iMaxNANDRegions = MAX_NAND_REGIONS;

P_NAND_REGION_INFO                     g_apInternalDataDriveRegions[MAX_DATA_DRIVE_REGIONS-1];
P_NAND_REGION_INFO                     g_apInternalHiddenDriveRegion[1];

int                                    g_iMaxNANDDataDriveRegions = MAX_DATA_DRIVE_REGIONS;


/*
NAND_PARAMETERS_DESCRIPTOR             g_InternalNANDSharedNANDParams;              // This doesn't have to be a Project struct
*/


NAND_BBTABLE_ENTRY                     g_aInternalNANDMasterBBTable[MAX_NAND_BAD_BLOCKS];
int                                    g_iMaxNANDBadBlocks = MAX_NAND_BAD_BLOCKS;


// TBD: In many cases, the number of cells allocated in the Master Updates Table will
// be more than is needed, because the number of actual Data Drive Regions will be less
// than the maximum allowed (because there are fewer than the maximum number of chips
// or regions per chip).  In this case, the extra space, allocated below, is wasted.
//
// It would be nice to allow that extra space to be used to increase the Update Sectors
// Map memory, to allow more USectorsMaps wo reside in RAM at the same time.  This should
// improve write performance, when you're skipping around a lot on the NANDs.

NAND_UPDATES_ENTRY                     g_aInternalNANDMasterUpdatesTable[MAX_UPDATE_BLOCKS];
int                                    g_iMaxNANDUpdateBlocks = MAX_UPDATE_BLOCKS;


WORD                                   g_UpdateSectorsMapsMemory[MAX_WORDS_FOR_UPDATE_SECTOR_MAPS];
int                                    g_iMaxWORDsForUpdateSectorsMaps = MAX_WORDS_FOR_UPDATE_SECTOR_MAPS;

NAND_DATA_DRIVE_INFO                   g_NANDDriveInfo[1];


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



