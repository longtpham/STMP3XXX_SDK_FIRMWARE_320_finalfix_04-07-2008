////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: NANDMedia.h
// Description:
////////////////////////////////////////////////////////////////////////////////

#ifndef _NAND_MEDIA_H
#define _NAND_MEDIA_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////
#include "NANDDdi.h"

///////////////////////////////////////////////////////////////////////////////
// Externs
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

// Those definitions are based to begin of sector
#define SECT_GOOD_BLOCK_MASK                0x00ff00

// Those definitions are based to begin of redundant area
#define RDNT_BAD_BLOCK_WORD_STATUS_POS      1
#define RDNT_GOOD_BLOCK_MASK                0xff0000
#define RDNT_STMP_BLOCK_WORD_STATUS_POS     2
#define RDNT_STMP_MASK                      0x005453

#define NAND_CONFIG_BLOCK_MAGIC_COOKIE  0x010203
#define NAND_CONFIG_BLOCK_VERSION       0x000003
#define NAND_MAGIC_COOKIE_WORD_POS      0
#define NAND_VERSION_WORD_POS           1


///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////
typedef struct MultiNandBbTableEntryStruct {    //Stmp00010049  
    INT BBPhysicalAddress       :   15;    // Physical address of Bad Block. Was 14 now 15. Fix Renesas. Must be INT not WORD.
                                            // (relative to the chip origin.
    INT ChipNumber              :   9;    // Chip number where bad block is located. Now 9 was 10. 

} MULTI_NAND_BBTABLE_ENTRY_T;
// These allow declarations and externs to use the define below and we
// specify _X or _Y here only. If X, BBTable will default to x_sys cluster.
// If _Y, used below, section containing BBTable will default to y_extra cluster.
#define MULTI_NAND_BBTABLE_ENTRY    MULTI_NAND_BBTABLE_ENTRY_T _Y
#define P_MULTI_NAND_BBTABLE_ENTRY  MULTI_NAND_BBTABLE_ENTRY_T _Y *
#define P_MULTI_NAND_BBTABLE        MULTI_NAND_BBTABLE_ENTRY_T _Y *


///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
BOOL IsBlockBad(WORD wBlock1stSector,
            NANDHAL_API_DESCRIPTOR *pNandHalApi,
            NAND_PHYSICAL_MEDIA_DESCRIPTOR *pNandPhysMediaDesc,
            INT iNumSectorsPerPage,
            LOGICAL_DRIVE_TYPE eDriveType);


#ifdef USBMSC_BUILD
    // Caller is in nandmediadiscover.c  
    // This func should only be called for detecting factory marked bad blocks before anything was written to the flash ever.
    // unused array elements set to -1 and assume all elements after that are unused.
   #define MAX_PAGES_IN_ARRAY  (3)

   BOOL IsBlockBadFromFactory(WORD wBlock1stSector,
            NANDHAL_API_DESCRIPTOR *pNandHalApi,
            NAND_PHYSICAL_MEDIA_DESCRIPTOR *pNandPhysMediaDesc,
            INT iNumSectorsPerPage,
            INT aPageNumListArray[ /* MAX_PAGES_IN_ARRAY */ ]);
#endif

RETCODE _reentrant NANDMediaAllocate(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
                                     P_MEDIA_ALLOCATION_TABLE pTable);
RETCODE _reentrant NANDMediaCloseMedia(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor);
RETCODE _reentrant NANDMediaDiscoverAllocation(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor);
RETCODE _reentrant NANDMediaErase(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
                                  WORD wMagicNumber);
RETCODE _reentrant NANDMediaGetInfo(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
                                    LOGICAL_MEDIA_INFO Type, void * pInfo);
RETCODE _reentrant NANDMediaGetMediaTable(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
                                          P_MEDIA_ALLOCATION_TABLE *pTable);
RETCODE _reentrant NANDMediaInit(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor);

///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////

#endif // #ifndef NANDSystemDrive
