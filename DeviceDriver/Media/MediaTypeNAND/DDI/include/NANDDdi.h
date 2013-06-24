////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: NANDDdi.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _NANDDDI_H
#define _NANDDDI_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "ddildl_internal.h"
#include "nandhal.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define ECC_TIMEOUT                         1000

// ECC & DMA get status functions return SUCCESS when completed successfully
#define ECC_STATUS_BUSY                     0x006000
#define ECC_STATUE_TIMEOUT                  0x006001
#define DMA_STATUS_BUSY                     0x006001
#define DMA_STATUE_TIMEOUT                  0x006002
#define NAND_PROGRAM_STATUS_BUSY            0x006003
#define NAND_PROGRAM_STATUS_TIMEOUT         0x006004
#define NAND_PROGRAM_STATUS_FAILED          0x006005

// Errors generated in Media Driver
#define NAND_READ_SECTOR_FAIL               0x006006
#define NAND_CONFIG_BLOCK_NOT_FOUND         0x006007


#define MAX_CHIPS                           4   // Maximum number of chips 
                                                // supported in a multi nand
                                                // configuration

#define NUM_PAGES_MARKED_BAD_IN_BLOCK       4   // Number of pages marked bad
                                                // once a block is found Bad. 
                                                // We decided to attempt to mark 
                                                // 4 pages bad in case the status
                                                // information does not stick.

///////////////////////////////////////////////////////////////////////////
// BAD BLOCK TABLE ENTRIES
//
// A BBTABLE_ENTRY may be FREE, meaning it is not associated with any Bad Block, 
// yet (but may be in the future), or it may be NOT_ASSIGNED, meaning it is
// associated with a Bad Block but no Data Drive RBlock has been assigned to
// it (e.g. it may be a System Drive block), or it may have both a Bad Block
// and an RBlock assigned to it.
///////////////////////////////////////////////////////////////////////////
#define BBTBL_FREE (-1)
#define BBTBL_NOT_ASSIGNED (-1)
//sdk3.110 requirement said to leave this as 14 and 10 bits but when we add renesas this must change to 15 and 9 bits as described in next comment.
typedef struct NandBbTableEntryStruct {  // 16k quantity of superblocks on 2Gbit renesas, so first field below needs max value of 16383. 2^14 -1 = 16383.  
    INT iBBPhysicalAddress       :   15;    // Physical address of Bad Block  // Dec19'05: changed both from INT to WORD & changed bitsz from 14 to 15 and 10 to 9 to prevent Renesas defect. Jan18'06 back to INT solves 9793 in sdk2.610 prerelease. No defect in sdk3.
                                            // (relative to Chip) || BBTBL_FREE
    INT iRblkIdx                 :   9;    // Index of Region's assigned Rblock. num R blks is 1.5% of tot blocks by default so 9 signed bits is enough to store max which is <256. 
                                            // (for Data Drive) || BBTBL_NOT_ASSIGNED
 
} NAND_BBTABLE_ENTRY_T;
// These allow declarations and externs to use the define below and we
// specify _X or _Y here only. If X, BBTable defaults to x_sys cluster.
// If _Y used below, BBTable section will default to y_extra cluster.
#define NAND_BBTABLE_ENTRY      NAND_BBTABLE_ENTRY_T _Y
#define P_NAND_BBTABLE_ENTRY    NAND_BBTABLE_ENTRY_T _Y *
#define P_NAND_BBTABLE          NAND_BBTABLE_ENTRY_T _Y *


/////////////////////////////////////////////////////////////////////////////////
//  UPDATES TABLE
//
// Each Region has an Updates Table (UTbl), which is a subset of the Data Drive's
// Master Updates Table.
//
// The Region knows the max size (iNumUBlks), which is exactly the number of UBlocks
// in the Region, and has a pointer to the table entries, an array of 
// NAND_UPDATES_ENTRYs.
//
// The NAND_UPDATES_ENTRY has two fields:
//
// iHBlkAddr is the physical address (14 bits worth, relative to the chip) of the 
// HBlock which is being Updated.  If the Update Entry is not is use, this value is
// UTBL_ERASED, if the UBlock pointed to by iUBlkIdx has been erased, or UTBL_FREE.
// 
// iUBlkIdx is the index of the UBlock which is being used to Update the HBlock.
// This value is always present and unique, even if the Update is marked as UTBL_FREE
// or UTBL_ERASED.  That is, THERE IS ALWAYS AN ENTRY FOR EVERY UBLOCK IN THE UPDATES
// TABLE.  This allows us to search for an unused UBlock, by finding a UTbl entry
// with iHBlkAddr set to UTBL_FREE or UTBL_ERASED and grabbing the iUBlkIdx field's
// value.
//
// To get the physical address of the UBlock, add UBlkIdx to pRegion->iUBlksPhysAddr.
//
// Entries in the UTbl are kept sorted by the iHBlkAddr fields (UTBL_FREE and 
// UTBL_ERASED are moved to the end of the table).  This allows fast searches for
// HBlk addresses, an operation which occurs very often.
//
// When a new entry  is added to the table, it must be inserted into the sort order,
// which may require moving some other entries down.
//
// See also: Update Sectors Map
//
/////////////////////////////////////////////////////////////////////////////////
// Any iHBlkAddr value >= UTBL_SPECIAL_CODES is a special code, instead of an address
#define UPDATE_TABLE_SPECIAL_CODES (0x3ffd)
#define UPDATE_TABLE_ERASED (0x3fff)
#define UPDATE_TABLE_FREE   (0x3ffe)
#define UPDATE_TABLE_BAD    (0x3ffd)



typedef struct {
  WORD wHBlkAddr                 :   14;    // Physiscal address of Home Block
                                            // (relative to chip) being Updated
                                            // || UPDATE_TABLE_FREE  || UPDATE_TABLE_BAD
  WORD wUBlkIdx                  :   10;    // Index of Region's assigned UBlock
                                            // ALL UBlk INDICES ARE PRESENT,
                                            // even if iHBlkAddr = UTBL_FREE || UTBL_ERASED
} NAND_UPDATES_ENTRY, * P_NAND_UPDATES_ENTRY, * P_UTABLE;



/////////////////////////////////////////////////////////////////////////////////
// NAND Region Info Struct
/////////////////////////////////////////////////////////////////////////////////
typedef struct RegionInfoStruct {

  int                                iChip;                // Index of NAND Chip containing this Region
  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR   pNANDDesc;            // NAND descriptor
  P_NAND_PARAMETERS_DESCRIPTOR       pNANDParms;           // NAND parameters (shortcut)
  P_LOGICAL_DRIVE_DESCRIPTOR         pLogicalDrive;        // Pointer back to our grandparent

  LOGICAL_DRIVE_TYPE                 eDriveType;           // Some System Drive, or Data Drive
  WORD                               wTag;                 // Drive Tag

  int                                iStartPhysAddr;       // Starting Block number for Region
                                                           // relative to chip 
  int                                iNumBlks;             // Size, in blocks, of whole Region
                                                           // Size includes embedded Bad Blocks


            ////////////////////////////////////////////////////////////////
            // The following elements, relating to HBlocks, are set and used
            // by the Data Drive, only.  They are included in this structure
            // for convenience.

  int                                iHBlksPhysAddr;       // Starting Physical Block number for 
                                                           // Home Blocks.
  int                                iNumHBlks;            // Size, in blocks, of Home Blocks area.
                                                           // Size includes embedded Bad Blocks

  int                                iHBlksLogBlkAddr;     // Starting Logical Block number

  DWORD                              dwHBlks1stLogSector;  // Staring Logical Sector Number
  DWORD                              dwNumHSectors;        // Number of Sectors in the HBlocks in
                                                           // this Region. (Includes Bad Blocks.)
            ////////////////////////////////////////////////////////////////


  int                                iUBlksPhysAddr;       // Starting Block number for UBlocks
  int                                iNumUBlks;            // For non-Data Drive, should = 0
                                                           // Size includes embedded Bad Blocks
  P_UTABLE                           pUTable;              // Pointer to Updates Table (array)
  WORD                               *pFreshList;          // Pointer to Updates Fresh List for Region
  WORD                               wFreshListHead;       // Head pointer (index) for FreshList

  int                                iRBlksPhysAddr;       // Starting Block number for RBlocks
  int                                iNumRBlks;            // For non-Data Drive, should = 0
                                                           // Size includes embedded Bad Blocks


  int                                iNumBadBlks;          // Number Bad Blocks in this Region
  int                                iMaxBadBlks;          // Max for this Region (i.e. how
                                                           // much padding is there for "new"
                                                           // Bad Blocks?)

  P_NAND_BBTABLE                     pBBTable;             // Table of Bad Blocks for this Region
                                                           // (This is just a pointer into a
                                                           // subset of the BadBlocksTable in
                                                           // a (the) NAND_MEDIA_INFO structure
                                                           // (below).
  BOOL                               bRegionInfoDirty;     // If TRUE, the bad block information
                                                           // has updates                                                           
} NAND_REGION_INFO, *P_NAND_REGION_INFO;

/////////////////////////////////////////////////////////////////////////////////
// NAND Media Physial Media Struct
//
// This structure contains all of the information about a (the) NAND Media.
// It has a table of all of the Chips' NANDDescriptors, the addresses of the
// Configuration Block for each chip, a table of all of the Regions
// on those chips, and a table of all of the Bad Blocks on those chips.
//
// There will typically be *one* of these structures, and it will be pointed 
// to by the "PhysMediaTypeStruct" element of the LOGICAL_MEDIA_DESCRIPTOR and 
// LOGICAL_DRIVE_DESCRIPTOR's for the NAND Media.
//
// Note: The array sizes, MAX_NAND_CHIPS, MAX_NAND_REGIONS and 
// NAND_MEDIA_MAX_BAD_BLOCKS are meant to be Project-specific.  So the actual
// arrays must be allocated in a Project-specific source (e.g. project.c).
// The NAND_MEDIA_INFO (assuming there's only one) can also be allocated there,
// with initial values pointing to the arrays (and initial counts of zero).
//
/////////////////////////////////////////////////////////////////////////////////

typedef struct {

  int                                   iNumNANDChips;
  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR      pNANDDesc;          // NANDDesc[MAX_NAND_CHIPS]

  int                                   ConfigBlkAddr[MAX_CHIPS];     // ConfigBlkAddr[MAX_NAND_CHIPS]

  int                                   iNumRegions;
  P_NAND_REGION_INFO                    pRegionInfo;        // RegionInfo[MAX_NAND_REGIONS];

  int                                   iNumBadBlks;
  P_NAND_BBTABLE                        pMasterBBTable;     // MasterBBTable[NAND_MEDIA_MAX_BAD_BLOCKS]

} NAND_MEDIA_INFO, * P_NAND_MEDIA_INFO;

/////////////////////////////////////////////////////////////////////////////////
// Config Block Region Info
/////////////////////////////////////////////////////////////////////////////////
typedef struct {
  LOGICAL_DRIVE_TYPE         eDriveType;       // Some System Drive, or Data Drive
  WORD                       wTag;              // Drive Tag

//  DataDriveMultiNANDConfig   eMultiNANDConfig; // CONCATENATED_MULTI_NAND | STRIPED_MULTI_NAND

//  DataDriveAddressing        eAddressing;      // NAND_LINEAR_ADDRESSING | NAND_MULTI_PLANE_ADDRESSING

  int                        iNumBlks;         // Size, in blocks, of whole Region
                                               // Size includes embedded Bad Blocks

  int                        iNumUBlks;        // For non-Data Drive, this should = 0
                                               // Size includes embedded Bad Blocks

  int                        iNumRBlks;        // For non-Data Drive, this should = 0
                                               // Size includes embedded Bad Blocks

} NAND_CFG_BLK_REGION_INFO, * P_NAND_CFG_BLK_REGION_INFO;




/////////////////////////////////////////////////////////////////////////////////
// Configuration Block Info Sector
//
// Note: The array size, NAND_MEDIA_MAX_REGIONS, is meant to be project-specfic.
// So, in this structure, a array of size, 1, is defined.  This structure will
// not typically be allocated anywhere - it's just used to cast a pointer onto
// a Sector buffer.  So, with the array at the end of the structure, we can merrily
// read beyond the stated size (1), as long as we don't go off the end of the
// Sector.  A NAND_CFG_BLK_REGION_INFO is only 4 WORDs (12 bytes) long, so we
// can go on for 41 Regions before we run off the edge.  That's way more Regions 
// than we should ever have.  (So the day this statement becomes false, I'll
// have to come back and fix this some other way.)
//
/////////////////////////////////////////////////////////////////////////////////
typedef struct {
  int                        iMagicCookie;
  int                        iVersionNum;

  int                        iNumBadBlks;        // Number Bad Blocks on this Chip

  int                        iNumRegions;
  NAND_CFG_BLK_REGION_INFO   Regions[1];         // Regions[NAND_MEDIA_MAX_REGIONS]

} NAND_CFG_BLK_INFO, * P_NAND_CFG_BLK_INFO;

#define RelativeSectorToPhysical(pNandParameters, iBlkNum, iRelSector) \
     ((iRelSector) + (iBlkNum) * ((pNandParameters)->wNumberSectorsPerBlock))

///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NANDCalculateCompleteEccSync(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc,
    P_SECTOR_BUFFER pBuffer);

#if 0
// No longer used
RETCODE _reentrant NANDCalculatePartialEccSync(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc,
    P_SECTOR_BUFFER pBuffer);

RETCODE _reentrant NANDStartEcc(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc,
    P_SECTOR_BUFFER pBuffer,
    WORD wWhich);
#endif
RETCODE _reentrant NANDGetEccStatus(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc);

RETCODE _reentrant NANDWritePageStartDmaSync(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc,
    WORD wAbsoluteSectorNum,
    P_SECTOR_BUFFER pBuffer);
RETCODE _reentrant NANDGetDmaStatus(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc);
RETCODE _reentrant NANDWriteStartPageProgramSync(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc);
RETCODE _reentrant NANDWriteStartCachePageProgramSync(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc);
RETCODE _reentrant NANDWriteStartMultiPageProgramSync(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc);
RETCODE _reentrant NANDGetPageProgramStatus(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc);
RETCODE _reentrant NANDGetCachePageProgramStatus(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc,
    WORD * wStatus);

RETCODE _reentrant NANDSetSectorLba(
    WORD wBlockNumber,
    WORD wSectorOffset,
    P_SECTOR_BUFFER pBuffer);

RETCODE NANDRecoverHardwareFromEraseFailure(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc);

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
extern LOGICAL_MEDIA_API NANDMediaApi;
extern NAND_MEDIA_INFO g_NANDMediaInfo[];

extern LOGICAL_DRIVE_API NANDDataDriveApi;
extern LOGICAL_DRIVE_API NANDSystemDriveApi;


#endif // #ifndef _NANDDDI_H
