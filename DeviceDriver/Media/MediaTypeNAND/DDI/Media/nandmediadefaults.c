
#include "types.h"
#include "NANDDdi.h"
#include "nanddatadrive.h"
/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2005
//
// Filename: nandmediadefaults.c 
// Description: Project-specific definitions and declarations for 
//              Internal NAND Media DDI
/////////////////////////////////////////////////////////////////////////////////


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

/* The following was folded into the MEDIA_INFO structure, always having 4 entries.
int                                    g_aiConfigBlkPhysAddr[MAX_NAND_CHIPS];
*/


NAND_REGION_INFO                       g_aInternalNANDRegionInfoStructs[MAX_NAND_REGIONS];
int                                    g_iMaxNANDRegions = MAX_NAND_REGIONS;

P_NAND_REGION_INFO                     g_apInternalDataDriveRegions[MAX_DATA_DRIVE_REGIONS];
P_NAND_REGION_INFO                     g_apInternalHiddenDriveRegion[1];

int                                    g_iMaxNANDDataDriveRegions = MAX_DATA_DRIVE_REGIONS;



WORD                                   g_NANDDataDriveNumFreshListees = NUM_FRESH_LISTEES;


/*
NAND_PARAMETERS_DESCRIPTOR             g_InternalNANDSharedNANDParams;              // This doesn't have to be a Project struct
*/

WORD                                   g_NANDDataDriveFreshList[MAX_DATA_DRIVE_REGIONS*NUM_FRESH_LISTEES];
WORD                                   g_NANDHiddenDriveFreshList[NUM_FRESH_LISTEES];
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
//WORD                                   g_wGuardWordToCheckOflow=0xeedeaf; // comment out unless debugging

WORD                                   g_UpdateSectorsMapsMemory[MAX_WORDS_FOR_UPDATE_SECTOR_MAPS];

#ifdef MMC
NAND_DATA_DRIVE_INFO                   g_NANDDriveInfo[3];	//CHANGE FROM 2 TO 3 taken care of janus drive in MMC build
#else
NAND_DATA_DRIVE_INFO                   g_NANDDriveInfo[2];	//non MMC
#endif

P_UPDATE_SECTORS_MAP_DESCRIPTOR        g_pUpdateSectorsMapsMemory = (P_UPDATE_SECTORS_MAP_DESCRIPTOR)g_UpdateSectorsMapsMemory;
int                                    g_iMaxNANDUpdateBlocks = MAX_UPDATE_BLOCKS;
int                                    g_iMaxWORDsForUpdateSectorsMaps = MAX_WORDS_FOR_UPDATE_SECTOR_MAPS;


FLOAT g_fMinFractionRBlocks = FRACTION_RBLOCKS;


INT g_iMinSpareRBlocksPerRegion = MIN_SPARE_RBLOCKS_PER_REGION;
INT g_MaxAvgUBlksPerRegion = AVG_UPDATE_BLOCKS_PER_REGION;
INT g_MinUBlksPerRegion = MIN_UPDATE_BLOCKS_PER_REGION;
INT g_MinDataDriveSize = MIN_DATA_DRIVE_SIZE;
FLOAT g_fHiddenDriveFraction = HIDDEN_DRIVE_PERCENTAGE;


// Renesas NAND uses the WP# line to drive both the WP# and RES# lines.  If
// this behavior is not desired, change the following line to FALSE.
// Note - parts other than Renesas are not affected by this flag.
#define USE_WP_FOR_RESET    TRUE
WORD g_UseWP_asRenesasReset = USE_WP_FOR_RESET;
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


