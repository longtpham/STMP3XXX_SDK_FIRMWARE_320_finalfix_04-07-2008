=======================================================================================
NANDInfo Utility: This utility works in the BoxView debugger.
=======================================================================================
Information Provided:
g_Nand -- this structure contains (for each Nand):
    BOOL bCompleted;           // =1 when complete
    WORD wTotalBlocksChecked;  // When complete, this contains the total blocks checked in the Nand (all of them)
    WORD wSystemBlockCount;    // When complete, this contains the total number of system blocks on the Nand
    WORD wBadBlockCount;       // When complete, this contains the total number of bad blocks on the Nand

g_NandInfo -- this structure contains (for each Nand):
    WORD wConfigBlockAddr;          // When complete, this contains the block number of the config block (inits to $00FFFF)
    SYSTEM_BLOCK_LIST SystemBlocks; // This contains:
                                           WORD wSystemBlocksCount;  // The total system blocks on this Nand
                                           WORD wSystemBlockAddr[];  // The individual addresses of the system blocks (in order)
    WORD wHBlocksCount;             // Total HBlocks on this NAND Chip
    WORD wUBlocksCount;             // Total UBlocks on this NAND Chip
    WORD wRBlocksCount;             // Total RBlocks on this NAND Chip
    BAD_BLOCK_LIST BadBlocks;       // This contains:
                                           WORD wBadBlocksCount;     // The total bad blocks on this Nand
                                           WORD wBadBlockAddr[];     // The individual addresses of the bad blocks (in order)

g_RegionInfo -- this structure contains more detailed information about the H/U/R blocks.
For each region on the chip(s), it contains this information:
    int  iChip;
    int  iHBlksPhysAddr;
    int  iNumHBlks;
    int  iUBlksPhysAddr;
    int  iNumUBlks;
    int  iRBlksPhysAddr;
    int  iNumRBlks;



==============================================================================================
More detailed information about size of drives
==============================================================================================


/////////////////////////////////////////////////////////////////////////////////
// NAND Region Info Struct
/////////////////////////////////////////////////////////////////////////////////

g_aInternalNANDRegionInfoStructs -- contains more specific information about each drive regions

The drive tags are defined as:
#define DRIVE_TAG_STMPSYS_S         0x00        //!< player drive tag.
#define DRIVE_TAG_HOSTLINK_S        0x01        //!< UsbMscMtp drive tag, old name was DRIVE_TAG_USBMSC_S. 
#define DRIVE_TAG_RESOURCE_BIN      0x02        //!< player resource drive tag.
#define DRIVE_TAG_EXTRA_S           0x03        //!< the host has 0x03 reserved for an extra system drive.
#define DRIVE_TAG_RESOURCE1_BIN     0x04        //!< the host has 0x04 reserved for an extra system drive.
#define DRIVE_TAG_OTGHOST_S         0x05        //!< the host has 0x05 reserved for OTG drive.
#define DRIVE_TAG_HOSTRSC_BIN       0x06        //!< UsbMscMtp resource drive tag, old name was DRIVE_TAG_MTP_BIN.
#define DRIVE_TAG_DATA              0x0A        //!< data drive tag.
#define DRIVE_TAG_DATA_HIDDEN       0x0B        //!< hidden data drive tag, old name was DRIVE_TAG_HIDDEN
#define DRIVE_TAG_BOOTMANAGER_S     0x50        //!< boot manager drive tag
#define DRIVE_TAG_UPDATER_S         0xFF        //!< the host has 0xFF reserved for usbmsc.sb file used in recovery mode operation only.
 

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

=======================================================================================

To BUILD:
 in the Make directory, compile with:   nandinfo ALL DEBUG LIBS PLIBS

For Multi-NAND: nandinfo ALL NAND2 DEBUG LIBS PLIBS  (or NAND3, NAND4, etc)
=======================================================================================

To RUN: 
 in the Boxview debugger, load the file:  nandinfo.abs
 after loading, press F5 to run
 It should run until it hits "// SUCCESS!!!!! DONE!!!!!" in the code.

 At this point, in the WATCH window, verify the values in g_Nand, g_NandInfo, and g_RegionInfo for each Nand.

=======================================================================================
