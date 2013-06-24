////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2006
//
// Filename:    task0.c
// Description: main() Program for obtaining info from NANDs for STMP35xx
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel.h"
#include "ddildl.h"
#include "NANDDdi.h"
#include "NANDDataDrive.h"
#include "NANDMedia.h"
#include "stringlib.h"
#include "sysserialnumber.h"
#include "NANDRA_utils.h"


////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

#define MAX_SYSTEM_BLOCKS_DISPLAYED 60
#define MAX_BAD_BLOCKS_DISPLAYED    200
#define TEST_BUFFER_SIZE_IN_WORDS   176
#define MAX_REGIONS 100


// Normally, you shouldn't need to use the ECC correction, but there may be
// some cases where the ECC is needed (Type3)  // type3 no longer supported.
// #define USE_ECC   1

typedef enum {
    BlockTypeBad = 0,
    BlockTypeGood = 1,
    BlockTypeSystem = 2,
    BlockTypeUnknown = 3
} BLOCK_TYPE;

typedef struct {
    WORD wBadBlocksCount;
    WORD wBadBlockAddr[MAX_BAD_BLOCKS_DISPLAYED];
} BAD_BLOCK_LIST, *P_BAD_BLOCK_LIST;

typedef struct {
    WORD wSystemBlocksCount;
    WORD wSystemBlockAddr[MAX_SYSTEM_BLOCKS_DISPLAYED];
} SYSTEM_BLOCK_LIST, *P_SYSTEM_BLOCK_LIST;

typedef struct {
    WORD wConfigBlockAddr;
    SYSTEM_BLOCK_LIST SystemBlocks;
    WORD wHBlocksCount;
    WORD wUBlocksCount;
    WORD wRBlocksCount;
    BAD_BLOCK_LIST BadBlocks;
} NAND_INFO_TYPE, *P_NAND_INFO_TYPE;

typedef struct {
    int  iChip;
    int  iHBlksPhysAddr;
    int  iNumHBlks;
    int  iUBlksPhysAddr;
    int  iNumUBlks;
    int  iRBlksPhysAddr;
    int  iNumRBlks;
} REGION_INFO_TYPE, *P_REGION_INFO_TYPE;

typedef struct {
    BOOL bCompleted;
    WORD wTotalBlocksChecked;
    WORD wSystemBlockCount;
    WORD wBadBlockCount;
} TEST_NAND_TYPE, *P_TEST_NAND_TYPE;

typedef struct {
  USHORT usSectorNumFld1;
  USHORT usSectorNumFld2;
  BYTE   btReserved;
  BYTE   btBlockStatus;  // Good/Bad/System
  USHORT usBlockAddrFld1;
  WORD   wECCField1;
  USHORT usBlockAddrFld2;
  WORD   wECCField2;
} NAND_REDUNDANT_AREA, *P_NAND_REDUNDANT_AREA;


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

RETCODE GetInfoForAllNands(void);
RETCODE GetNandInfo(WORD wChip);
RETCODE GetBlockType(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
    WORD wBlockAddr,
    BLOCK_TYPE * eBlockType);
RETCODE InitNandInfo(WORD wChip);
void NandRedundantAreaSwizzler(INT i);

extern NANDHAL_MEM NAND_PHYSICAL_MEDIA_DESCRIPTOR g_NandDescriptor[];


void ChipUpdateCapability(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

// NANDInfo Variables
NAND_INFO_TYPE g_NandInfo[MAX_CHIPS];
REGION_INFO_TYPE g_RegionInfo[MAX_REGIONS];
TEST_NAND_TYPE g_Nand[MAX_CHIPS];
SECTOR_BUFFER SectorBuffer[TEST_BUFFER_SIZE_IN_WORDS];

NAND_REDUNDANT_AREA _X g_NandRedundantInfo[256];
SECTOR_BUFFER RedundantBuffer[16];
//SECTOR_BUFFER g_NandRedundantInfo[256];
WORD g_NandRAStartBlock = 0;

INT g_JanusDrive = -1;

WORD g_ResourceRead = 0;
short g_ResourceByteIndex = 0;
short g_ResourceByteLength = 0;

//BOOL g_bWriteBmngr512Bytes=TRUE; // unused stub 

// nand info util and any util that is not allowed to edit the flash will need to set this flag to FALSE in the application. 
extern BOOL g_bAllowedToEditTheFlash; 


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

long int  FSGetDWord(void *buffer, int Offset,int MemoryType)
{
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Main
//
//   Type:          Function
//
//   Description:   Main entry point
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant Task0EntryPoint(void)
{
  WORD wSector;
  INT  i;	
  INT  ret;
  

    // Call NandMediaInit
    // This function will fill in parameters for:
    //     LOGICAL_MEDIA_DESCRIPTOR  (g_MediaDescriptor : declared in ddiproject.c)
    //     NAND_MEDIA_INFO   (NandMediaInfo : declared in NANDMediaInit.c)
    //     NAND_PHYSICAL_MEDIA_DESCRIPTOR[] via NandHalInit (g_NandDescriptor[4] : declared in nandhalproject.c)
    //     NAND_PARAMETERS_DESCRIPTOR via NandHalInit (g_NandParams : declared in nandhalproject.c)

    if ( SUCCESS != NANDMediaInit(g_MediaDescriptor) )
        SystemHalt();

    // Each app that can't convert factory blocks bad to sgtl bad blocks in case of no config block found, 
    // must set this BOOL to avoid trying to convert any factory blocks bad in case of config block not found
    g_bAllowedToEditTheFlash = FALSE;   // declared in nandmediadiscover.c
        
    if ( SUCCESS != NANDMediaDiscoverAllocation(g_MediaDescriptor) )
        SystemHalt();
    
    for ( i = 0; i < g_wNumDrives; ++i )
    {
        ret = DriveInit(i);
        
        if ( SUCCESS != ret )
            SystemHalt();        // If MediaInit fails, HALT.
    }
        
    GetInfoForAllNands();
    
    // SUCCESS!!!!! DONE!!!!!
    SystemHalt();
    SystemHalt();
    // SUCCESS!!!!! DONE!!!!!

    // To Get Info On The Redundant Areas, Continue from here.
    // First set block number to start from.
    //    g_NandRAStartBlock  = set this to physical block number to begin at
    //    Hit GO (F5).
    while (1)
    {
      wSector = (g_NandRAStartBlock) * (g_NandDescriptor[0].pNANDParams->wNumberSectorsPerBlock);
      for (i = 0; i < g_NandDescriptor[0].pNANDParams->wNumberSectorsPerBlock; i++)
      { // for each sector in the block
#ifdef USE_ECC
        g_NandDescriptor[0].pNANDParams->pNandHalApi->pReadSectorRedundant(&g_NandDescriptor[0], wSector+i, &RedundantBuffer[0], TRUE);
        // Put back into NAND form
        NANDRA_ToNANDForm(&RedundantBuffer[0], RA_ONLY); 
#else
        g_NandDescriptor[0].pNANDParams->pNandHalApi->pReadSectorRedundant(&g_NandDescriptor[0], wSector+i, &RedundantBuffer[0], FALSE);
#endif
        NandRedundantAreaSwizzler(i); // swizzle the data into readable format
      }
      g_NandRAStartBlock++;
      SystemHalt();
      // Look at g_NandRedundantInfo now to see the redundant areas for each sector of this block.
      // If you want to jump to a new block -- change g_NandRAStartBlock to the new location.
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetInfoForAllNands
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
extern NAND_MEDIA_INFO NandMediaInfo;
RETCODE GetInfoForAllNands(void)
{
    INT i, iChip;
    INT iCount=0;
    
    for(i=0;i<NandMediaInfo.iNumNANDChips;i++)
    {
        GetNandInfo(i);
        // Init some values
        g_NandInfo[i].wHBlocksCount = 0;
        g_NandInfo[i].wUBlocksCount = 0;
        g_NandInfo[i].wRBlocksCount = 0;

    }

    for (i=0;i<MAX_REGIONS;i++)
    {
      g_RegionInfo[i].iChip = 0;
      g_RegionInfo[i].iHBlksPhysAddr = 0;
      g_RegionInfo[i].iNumHBlks = 0;
      g_RegionInfo[i].iUBlksPhysAddr = 0;
      g_RegionInfo[i].iNumUBlks = 0;
      g_RegionInfo[i].iRBlksPhysAddr = 0;
      g_RegionInfo[i].iNumRBlks = 0;
    }

    // Now, add up the HBlocks/UBlocks/RBlocks on each chip
    for (i=0; i<NandMediaInfo.iNumRegions;i++)
    {
      iChip = g_aInternalNANDRegionInfoStructs[i].iChip;
      if (g_aInternalNANDRegionInfoStructs[i].eDriveType == DriveTypeData)
      {
        g_NandInfo[iChip].wHBlocksCount += g_aInternalNANDRegionInfoStructs[i].iNumHBlks;
        g_NandInfo[iChip].wUBlocksCount += g_aInternalNANDRegionInfoStructs[i].iNumUBlks;
        g_NandInfo[iChip].wRBlocksCount += g_aInternalNANDRegionInfoStructs[i].iNumRBlks;

        g_RegionInfo[iCount].iChip = g_aInternalNANDRegionInfoStructs[i].iChip;
        g_RegionInfo[iCount].iHBlksPhysAddr = g_aInternalNANDRegionInfoStructs[i].iHBlksPhysAddr;
        g_RegionInfo[iCount].iNumHBlks = g_aInternalNANDRegionInfoStructs[i].iNumHBlks;
        g_RegionInfo[iCount].iUBlksPhysAddr = g_aInternalNANDRegionInfoStructs[i].iUBlksPhysAddr;
        g_RegionInfo[iCount].iNumUBlks = g_aInternalNANDRegionInfoStructs[i].iNumUBlks;
        g_RegionInfo[iCount].iRBlksPhysAddr = g_aInternalNANDRegionInfoStructs[i].iRBlksPhysAddr;
        g_RegionInfo[iCount].iNumRBlks = g_aInternalNANDRegionInfoStructs[i].iNumRBlks;
        iCount ++;
      }
    }

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetNandInfo
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

RETCODE GetNandInfo(WORD wChip)
{
    INT i;
    WORD wSystemBlockIndex = 0;
    WORD wBadBlockIndex = 0;
    BOOL bConfigBlockFound = FALSE;
    BLOCK_TYPE eBlockType;
    P_NAND_PARAMETERS_DESCRIPTOR pNANDParams;
    P_NANDHAL_API_DESCRIPTOR pNandHalApi;
    
    // Get some local pointers
    pNANDParams = g_NandDescriptor[wChip].pNANDParams;
    pNandHalApi = pNANDParams->pNandHalApi;
    
    // Init stuff
    g_Nand[wChip].bCompleted = FALSE;
    g_Nand[wChip].wTotalBlocksChecked = 0;
    g_Nand[wChip].wSystemBlockCount = 0;
    g_Nand[wChip].wBadBlockCount = 0;
    g_NandInfo[wChip].wConfigBlockAddr = 0xFFFF;
    InitNandInfo(wChip);

    // Loop through all the blocks on this chip
    for(i=0;i<g_NandDescriptor[wChip].wTotalBlocks;i++)
    {
        g_Nand[wChip].wTotalBlocksChecked++;

        if(GetBlockType(
            &g_NandDescriptor[wChip],
            i,
            &eBlockType) != SUCCESS)
        {
            return ERROR;
        }
        
        // Save off information on this block type.
        switch(eBlockType)
        {
            case BlockTypeBad :
            {
              if (wBadBlockIndex < MAX_BAD_BLOCKS_DISPLAYED)
              {
                g_NandInfo[wChip].BadBlocks.wBadBlockAddr[wBadBlockIndex] = i;
                wBadBlockIndex ++;
              }
                g_Nand[wChip].wBadBlockCount++;
                g_NandInfo[wChip].BadBlocks.wBadBlocksCount++;
            } break;
            case BlockTypeSystem :
            {
              if (wSystemBlockIndex < MAX_SYSTEM_BLOCKS_DISPLAYED)
              {
                g_NandInfo[wChip].SystemBlocks.wSystemBlockAddr[wSystemBlockIndex] = i;
                wSystemBlockIndex ++;
              }
                g_Nand[wChip].wSystemBlockCount++;
                g_NandInfo[wChip].SystemBlocks.wSystemBlocksCount++;
            }
            case BlockTypeGood :
            {
              if (bConfigBlockFound == FALSE)
              {
                g_NandInfo[wChip].wConfigBlockAddr = i;
                bConfigBlockFound = TRUE;
              }
            } break;
            case BlockTypeUnknown :
            default :
            {
            } break;
        }
    }

    g_Nand[wChip].bCompleted = TRUE;
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          InitNandInfo
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none

//<
////////////////////////////////////////////////////////////////////////////////
RETCODE InitNandInfo(WORD wChip)
{
    INT i;
    
    g_NandInfo[wChip].SystemBlocks.wSystemBlocksCount = 0;
    g_NandInfo[wChip].BadBlocks.wBadBlocksCount = 0;
    for(i=0;i<MAX_BAD_BLOCKS_DISPLAYED;i++)
    {
        g_NandInfo[wChip].BadBlocks.wBadBlockAddr[i] = 0;
    }
    for(i=0;i<MAX_SYSTEM_BLOCKS_DISPLAYED;i++)
    {
        g_NandInfo[wChip].SystemBlocks.wSystemBlockAddr[i] = 0;
    }
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetBlockType
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE GetBlockType(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
    WORD wBlockAddr,
    BLOCK_TYPE * eBlockType)
{
    INT i;
    P_NANDHAL_API_DESCRIPTOR pNandHalApi; 
    WORD wBaseSectorAddr;
    WORD wSTMPCode;

    pNandHalApi = pNANDDescriptor->pNANDParams->pNandHalApi;
    wBaseSectorAddr = wBlockAddr * 
        pNANDDescriptor->pNANDParams->wNumberSectorsPerBlock;
    
    // Assume it's a good block
    *eBlockType = BlockTypeGood;
    
    for(i=0;i<pNANDDescriptor->pNANDParams->wNumberSectorsPerBlock;i++)
    {
        // read page
#ifdef USE_ECC
        if(pNandHalApi->pReadSectorRedundant(
            pNANDDescriptor,
            wBaseSectorAddr + i,
            SectorBuffer, TRUE) != SUCCESS)
        {
            SystemHalt();
        }
        
        // Is this marked as a bad block?
        if (!bBlockMarkedBadInRA(SectorBuffer, RA_ONLY, 0))
        {
           continue;
        }

        NANDRAGetStmpCode(SectorBuffer, RA_ONLY, &wSTMPCode);

        if (wSTMPCode == ST_TAG)
        {
            *eBlockType = BlockTypeSystem;
            return SUCCESS;
        }

        // To maintain Backward compatibility, we need to
        // perform an additional test if prior to TA6.

        // In order to remain backward compatible with TA4 & TA5, the bootmanager is
        // written as 512+16 chunks instead of the normal 2K for Type2, 4, 5 and 6. 
        {             
            WORD wTempNANDDataSize;
            WORD wTempAuxDataSize;

            WORD wRomRevision = ROM_REV_ID;
            wTempNANDDataSize = pNANDDescriptor->pNANDParams->wSectorDataSize;
            wTempAuxDataSize = g_NandAuxData.SectorDataSizeInBytes;
    
            if ((wRomRevision < TA6_ROM_REV_ID) && 
                (pNANDDescriptor->pNANDParams->NandType != 0x01)) // if not Type1.
            {            
                pNANDDescriptor->pNANDParams->wSectorDataSize = 512;
                g_NandAuxData.SectorDataSizeInBytes = 512;
            }                            
    
            if(pNandHalApi->pReadSectorRedundant(
            pNANDDescriptor,
            wBaseSectorAddr + i,
            SectorBuffer, TRUE) != SUCCESS)
            {
                SystemHalt();
            }
            
            // Is this marked as a bad block?
            if (!bBlockMarkedBadInRA(SectorBuffer, RA_ONLY, 0))
            {
               // Revert to original Data Size.
                pNANDDescriptor->pNANDParams->wSectorDataSize = wTempNANDDataSize;
                g_NandAuxData.SectorDataSizeInBytes = wTempAuxDataSize;
               continue;
            }
    
            NANDRAGetStmpCode(SectorBuffer, RA_ONLY, &wSTMPCode);

            // Revert to original Data Size.
            pNANDDescriptor->pNANDParams->wSectorDataSize = wTempNANDDataSize;
            g_NandAuxData.SectorDataSizeInBytes = wTempAuxDataSize;
    
            if (wSTMPCode == ST_TAG)
            {
                *eBlockType = BlockTypeSystem;
                return SUCCESS;
            }            
        }
#else        
        if(pNandHalApi->pReadSectorRedundant(
            pNANDDescriptor,
            wBaseSectorAddr + i,
            SectorBuffer, FALSE) != SUCCESS)
        {
            SystemHalt();
        }
        NANDRA_ToNANDForm(SectorBuffer, RA_ONLY);

        if((SectorBuffer[1] & 0xFF0000) == 0xFF0000)
        {
            continue;
        }        

        // It may be a bad block or a system block
        if((SectorBuffer[2] & 0x00FFFF) == 0x005453)
        {
            *eBlockType = BlockTypeSystem;
            return SUCCESS;
        }


        // To maintain Backward compatibility, we may need to
        // perform an additional test if prior to TA6.

        // In order to remain backward compatible with TA4 & TA5, the bootmanager is
        // written as 512+16 byte chunks instead of the normal 2KB sectors for Type2, 4, 5 and 6. 
        {             
            WORD wTempNANDDataSize;
            WORD wTempAuxDataSize;

            WORD wRomRevision = ROM_REV_ID;
            wTempNANDDataSize = pNANDDescriptor->pNANDParams->wSectorDataSize;
            wTempAuxDataSize = g_NandAuxData.SectorDataSizeInBytes;
    
            if ((wRomRevision < TA6_ROM_REV_ID) && 
                (pNANDDescriptor->pNANDParams->NandType != 0x01)) // if not Type1.
            {            
                pNANDDescriptor->pNANDParams->wSectorDataSize = 512;
                g_NandAuxData.SectorDataSizeInBytes = 512;
            }                            
    
            if(pNandHalApi->pReadSectorRedundant(
                pNANDDescriptor,
                wBaseSectorAddr + i,
                SectorBuffer, FALSE) != SUCCESS)
            {
                SystemHalt();
            }

            NANDRA_ToNANDForm(SectorBuffer, RA_ONLY);

            // Revert to original Data Size.
            pNANDDescriptor->pNANDParams->wSectorDataSize = wTempNANDDataSize;
            g_NandAuxData.SectorDataSizeInBytes = wTempAuxDataSize;
    
            if((SectorBuffer[1] & 0xFF0000) == 0xFF0000)
            {
                continue;
            }
            
            // It may be a bad block or a system block
            if((SectorBuffer[2] & 0x00FFFF) == 0x005453)
            {
                *eBlockType = BlockTypeSystem;
                return SUCCESS;
            }
        }

#endif
        *eBlockType = BlockTypeBad;
        return SUCCESS;
    }
    return SUCCESS;
}

void NandRedundantAreaSwizzler(INT i)
{        // BRUTE FORCE
  BYTE btCurrentByte;
          // Write the Sector Number Field 1
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 0);
          PutByteInArrayInX(&g_NandRedundantInfo[i],0,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 1);
          PutByteInArrayInX(&g_NandRedundantInfo[i],1,btCurrentByte);
          btCurrentByte = 0;
          PutByteInArrayInX(&g_NandRedundantInfo[i],2,btCurrentByte);
          // Write the Sector Number Field 2
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 2);
          PutByteInArrayInX(&g_NandRedundantInfo[i],3,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 3);
          PutByteInArrayInX(&g_NandRedundantInfo[i],4,btCurrentByte);
          btCurrentByte = 0;
          PutByteInArrayInX(&g_NandRedundantInfo[i],5,btCurrentByte);
          // Write the Reserved byte
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 4);
          PutByteInArrayInX(&g_NandRedundantInfo[i],6,btCurrentByte);
          btCurrentByte = 0;
          PutByteInArrayInX(&g_NandRedundantInfo[i],7,btCurrentByte);
          PutByteInArrayInX(&g_NandRedundantInfo[i],8,btCurrentByte);
          // Write the Block Status Byte
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 5);
          PutByteInArrayInX(&g_NandRedundantInfo[i],9,btCurrentByte);
          btCurrentByte = 0;
          PutByteInArrayInX(&g_NandRedundantInfo[i],10,btCurrentByte);
          PutByteInArrayInX(&g_NandRedundantInfo[i],11,btCurrentByte);
          // Write Block Address Field 1
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 6);
          PutByteInArrayInX(&g_NandRedundantInfo[i],12,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 7);
          PutByteInArrayInX(&g_NandRedundantInfo[i],13,btCurrentByte);
          btCurrentByte = 0;
          PutByteInArrayInX(&g_NandRedundantInfo[i],14,btCurrentByte);
          // Write ECC Field 2
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 8);
          PutByteInArrayInX(&g_NandRedundantInfo[i],15,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 9);
          PutByteInArrayInX(&g_NandRedundantInfo[i],16,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 10);
          PutByteInArrayInX(&g_NandRedundantInfo[i],17,btCurrentByte);
          // Write Block Address Field 2
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 11);
          PutByteInArrayInX(&g_NandRedundantInfo[i],18,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 12);
          PutByteInArrayInX(&g_NandRedundantInfo[i],19,btCurrentByte);
          btCurrentByte = 0;
          PutByteInArrayInX(&g_NandRedundantInfo[i],20,btCurrentByte);
          // Write ECC Field 2
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 13);
          PutByteInArrayInX(&g_NandRedundantInfo[i],21,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 14);
          PutByteInArrayInX(&g_NandRedundantInfo[i],22,btCurrentByte);
          btCurrentByte = GetByteFromArrayInX(&RedundantBuffer, 15);
          PutByteInArrayInX(&g_NandRedundantInfo[i],23,btCurrentByte);
}


void ChipUpdateCapability(void)
{
	return;
}

// use this for tests that must halt instead of ver that resets the DSP in RETAIL build. 
#pragma asm
 global FatalError
FatalError:
    debug
    nop
#pragma endasm


