////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename:    task0.c
// Description: main() Program for NANDBadBlockMarker
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
#include "sysserialnumber.h"
#include "nandra_utils.h"


////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////
extern NAND_MEDIA_INFO MEDIA_DDI_MEM NandMediaInfo;
extern SECTOR_BUFFER * NANDMediaSectorBuffer;
extern SECTOR_BUFFER   NANDRedundantArea[];

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////
#define MAX_BAD_BLOCKS 300
#define MAX_NANDS 4

typedef struct {
      int    iNumBadBlocks;         // number of bad blocks to set
      WORD   wBadBlockAddr[MAX_BAD_BLOCKS];         // block address to mark bad
} BAD_BLOCK_MAKER_TABLE;
  
////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
RETCODE Test1(void);
RETCODE _reentrant NandMediaEraseALL(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor, WORD wMagicNumber);
RETCODE _reentrant NandMediaSetBadBlocks(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor);
void ChipUpdateCapability(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
BOOL g_bLimitedVDDD = FALSE;
SECTOR_BUFFER   *pRedundBufTemp=NANDRedundantArea;
BAD_BLOCK_MAKER_TABLE g_NandBadBlocks[MAX_CHIPS];
SERIAL_NUMBER g_InternalMediaSerialNumber;           // Kludged here to make it compile...
WORD g_ResourceRead = 0;
short g_ResourceByteIndex = 0;
short g_ResourceByteLength = 0;

/* This global variable will tell the HAL driver to treat a system sector in the type5
nand as a 512 bytes sector device */

//BOOL g_bWriteBmngr512Bytes = FALSE;                 // Kludged here to make it compile

INT g_JanusDrive = -1;

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

    Test1();
    
    // SUCCESS!!!!!  DONE!!!!
    SystemHalt();
}

////////////////////////////////////////////////////////////////////////////////
//>  Name:          Test1
//   Description:
//
//   Inputs:        none
//   Outputs:       none
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE Test1(void)
{
    WORD wMagicNumber = 0;                // We don't have this implemented yet.
    INT i, iBadBlockCounter = 0;
    WORD iNumNands;

    // Call NANDMediaInit
    // This function will fill in parameters for:
    //     LOGICAL_MEDIA_DESCRIPTOR  (g_MediaDescriptor : declared in ddiproject.c)
    //     NAND_MEDIA_INFO   (NandMediaInfo : declared in NANDMediaInit.c)
    //     NAND_PHYSICAL_MEDIA_DESCRIPTOR[] via NandHalInit (g_NandDescriptor[4] : declared in nandhalproject.c)
    //     NAND_PARAMETERS_DESCRIPTOR via NandHalInit (g_NandParams : declared in nandhalproject.c)
    if (NANDMediaInit(g_MediaDescriptor))
      SystemHalt();        // If MediaInit fails, HALT.

    iNumNands = NandMediaInfo.iNumNANDChips;

    // Clear out the Nands that do not exist
    for (i=iNumNands; i<MAX_CHIPS; i++)
      g_NandBadBlocks[i].iNumBadBlocks = 0;

    // Count up total bad blocks to mark
    for (i=0; i<iNumNands; i++)
      iBadBlockCounter += g_NandBadBlocks[i].iNumBadBlocks;

    if (iBadBlockCounter)
    {
      // Call NandMediaEraseALL
      // DANGER!!! This is a test program only. It erases all blocks including bad blocks. It does not check them!
      // NandMediaEraseALL commented out for now. I advise using NANDRecover.
      //        if (NandMediaEraseALL(g_MediaDescriptor, wMagicNumber))
      //          SystemHalt();        // If MediaEraseALL fails, HALT.

        // Setup Bad Blocks.
        // This routine will mark the redundant area to signify that blocks are bad.
      if (NandMediaSetBadBlocks(g_MediaDescriptor))
          SystemHalt(); // inspect error return code in register A
    }

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! !
// D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! !
//>  Name:          NandMediaEraseALL
//
//   Type:          Function
//
//   Description:   This function erases ALL blocks, good or bad. It doesn't care.
//
//   Inputs:        pDescriptor:    Pointer to the logical media descriptor
//                                  structure
//                  wMagicNumber:   Magic Number - used to prevent accidental call to function.

//
//   Outputs:       SUCCESS or error code if fail
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NandMediaEraseALL(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
    WORD wMagicNumber)
{
    INT iNandNumber, iBlockCounter, i, j;
    INT iBadBlockCounter=0;                     // counts number of bad blocks across all NAND media
    INT iBlockPhysAddr;
    INT iNumSectorsPerPage;
    RETCODE Status;
    WORD wNumberSectorsPerBlock;
    DWORD dwSectorNumber;

    NAND_MEDIA_INFO *pNandMediaInfoDesc;
    NAND_PHYSICAL_MEDIA_DESCRIPTOR *pNandPhysicalMediaDesc;
    NAND_PARAMETERS_DESCRIPTOR *pNandParamsDesc;
    NANDHAL_API_DESCRIPTOR *pNandHalApiDesc;

    // Setup some pointers to use later
    pNandMediaInfoDesc = pDescriptor->pMediaInfo;

    // For each NAND in the system
    for (iNandNumber = 0; iNandNumber < pNandMediaInfoDesc->iNumNANDChips; iNandNumber++)
    {
        // Setup some pointers to use later
        pNandPhysicalMediaDesc = &(pNandMediaInfoDesc->pNANDDesc[iNandNumber]);
        pNandParamsDesc = pNandPhysicalMediaDesc->pNANDParams;
        pNandHalApiDesc = pNandParamsDesc->pNandHalApi;
        // Get Sectors Per Block and Sectors Per Page (to use later)
        wNumberSectorsPerBlock = pNandParamsDesc->wNumberSectorsPerBlock;
        iNumSectorsPerPage = 1 << pNandParamsDesc->wSectorToPageShift; 

        // LOCK the NAND to prevent other routines from interfering
        NANDHalLockNand(pNandPhysicalMediaDesc);

        // Enable Writes on the write protect line
        (*pNandHalApiDesc->pEnableWrites)(WP_NAND_INTERNAL);

        // For every block of the individual NAND chip
        for(iBlockCounter = 0 ; iBlockCounter < pNandPhysicalMediaDesc->wTotalBlocks; iBlockCounter++)
        {
            // Calculate the absolute physical address of the block
            iBlockPhysAddr = iBlockCounter;      
            
            // Erase the block
            Status = (*pNandHalApiDesc->pEraseBlock)(pNandPhysicalMediaDesc, (WORD)iBlockCounter);
 
            if( Status != SUCCESS )
            {
                if(Status == NANDHAL_ERROR_WRITE_FAILED)
                {   INT iNumPagesMarkedBadHere=0;
                    // The block is BAD and erase failed
                    // Calculate the 0th sector in the block
                    dwSectorNumber = iBlockCounter * wNumberSectorsPerBlock;

                    // Mark pages bad within the block until two read back bad
                    for(i = 0 ; i < pNandParamsDesc->wPagesPerBlock; i++)
                    {
                        // Write zeros to the entire sector (including redundant area)
                        pNandHalApiDesc->pWriteSector(pNandPhysicalMediaDesc, (WORD)dwSectorNumber, NANDMediaSectorBuffer);
                        // Read back the redundant area (to check the Bad Block Byte)
                        pNandHalApiDesc->pReadSectorRedundant(pNandPhysicalMediaDesc,(WORD)dwSectorNumber, pRedundBufTemp, FALSE);
                        NANDRA_ToNANDForm(pRedundBufTemp, RA_ONLY);
                        // IF Block Status Byte != 0xFF, the block has been successfully marked bad
                        if((NANDRedundantArea[RDNT_BAD_BLOCK_WORD_STATUS_POS] & RDNT_GOOD_BLOCK_MASK) !=  RDNT_GOOD_BLOCK_MASK)
                            if( ++iNumPagesMarkedBadHere == 2 ) 
                              break; // as soon as 2 pages marked bad, we are done. Note nand driver marks sector 0 & 1 bad so now did that here!
                        dwSectorNumber += (WORD)iNumSectorsPerPage; //increments by 1 for 2kByte sector / page flash
                    }
                } // endif(Status == NANDHAL_ERROR_WRITE_FAILED)
            } // endif ( Satus != SUCCESS )


        }   // End For Every Block on the NAND

      // Release the NAND
      NANDHalReleaseNand(pNandPhysicalMediaDesc);

      // Disable writes to chip
      (*pNandHalApiDesc->pDisableWrites)(WP_NAND_INTERNAL);
  
    } // endfor (i = 0; i < pDescriptor->pMediaInfo->iNumNANDChips; i++)

    return SUCCESS;
}

// Inputs: 
// Outputs: 
// Description: 
// Notes:
//
RETCODE _reentrant NandMediaSetBadBlocks(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor)
{
    int i, j, k;
    int iNumChips = NandMediaInfo.iNumNANDChips;
    INT iNumSectorsPerPage, iBadBlockNumber;
    WORD wNumberSectorsPerBlock;
    DWORD dwSectorNumber;
    RETCODE iRetCode;

    NAND_MEDIA_INFO *pNandMediaInfoDesc;
    NAND_PHYSICAL_MEDIA_DESCRIPTOR *pNandPhysicalMediaDesc;
    NAND_PARAMETERS_DESCRIPTOR *pNandParamsDesc;
    NANDHAL_API_DESCRIPTOR *pNandHalApiDesc;

    // Setup some pointers to use later
    pNandMediaInfoDesc = pDescriptor->pMediaInfo;

    for ( i = 0; i < iNumChips; i++)
    {
        // Setup some pointers to use later
        pNandPhysicalMediaDesc = &(pNandMediaInfoDesc->pNANDDesc[i]);
        pNandParamsDesc = pNandPhysicalMediaDesc->pNANDParams;
        pNandHalApiDesc = pNandParamsDesc->pNandHalApi;
        // Get Sectors Per Block and Sectors Per Page (to use later)
        wNumberSectorsPerBlock = pNandParamsDesc->wNumberSectorsPerBlock;
        iNumSectorsPerPage = 1 << pNandParamsDesc->wSectorToPageShift;

        // LOCK the NAND to prevent other routines from interfering
        NANDHalLockNand(pNandPhysicalMediaDesc);

        // Enable Writes on the write protect line
        (*pNandHalApiDesc->pEnableWrites)(WP_NAND_INTERNAL);

        // FOR each of the blocks listed in the table to mark bad
        for (j = 0; j < g_NandBadBlocks[i].iNumBadBlocks; j++)
        {
            // Get Block Number to mark bad
            iBadBlockNumber = g_NandBadBlocks[i].wBadBlockAddr[j];

             // Calculate the 0th sector in the block
            dwSectorNumber = (DWORD)iBadBlockNumber * (DWORD)wNumberSectorsPerBlock;

#if 1       // This fixed ver passed testing of stmp00009540.  
            // Ver in #else below uses same API as player & usbmsc to mark block pages bad. (but test showed it marked blocks bad sometimes)
            // Mark all pages bad within the block until two read back as bad
            {INT iNumPagesMarkedBadHere=0;
            for(k = 0 ; k < pNandParamsDesc->wPagesPerBlock ; k++)
            {
                // Write zeros to the entire sector (including redundant area)
                pNandHalApiDesc->pWriteSector(pNandPhysicalMediaDesc, (WORD)dwSectorNumber, NANDMediaSectorBuffer);

                // Read back the redundant area (need to check the Bad Block Byte)
                pNandHalApiDesc->pReadSectorRedundant(pNandPhysicalMediaDesc,(WORD)dwSectorNumber, pRedundBufTemp, FALSE);
                NANDRA_ToNANDForm(pRedundBufTemp, RA_ONLY);

                // old:  dwSectorNumber += (WORD)iNumSectorsPerPage; // 
                dwSectorNumber++; // go try to mark next sector in block as bad.

                // IF Bad Block Byte != FF, then the low sector number(s) (of this block) have been successfully marked bad
                // if SUCCESS check removed June 26 '06 since we can't handle the err anyway.
		//if (NANDRedundantArea[RDNT_BAD_BLOCK_WORD_STATUS_POS] & RDNT_GOOD_BLOCK_MASK) !=  RDNT_GOOD_BLOCK_MASK) 
              
                if (k==1)  /* page 0 and 1 marked bad already, now jump to last page and mark it bad also */
                {                                              
                    // Mark the last sector of the block as BAD
                    dwSectorNumber = ((DWORD)iBadBlockNumber * (DWORD)wNumberSectorsPerBlock) + (wNumberSectorsPerBlock-1);
                    // loop termination condition here so loop runs once more to mark last page bad.
                    k = pNandParamsDesc->wPagesPerBlock - 2;                    
                }

            }}
#else       // Ver simulates player and usbmsc case by using same function to mark a block bad. (but did not reliably mark some blocks in testing)
            // boxview showed that RA write buf had 0xFF in first byte. Datasheets say this should be non 0xFF value instead. 
            // Note: usbmsc/player don't write all zero's to the sector so I don't in this ver either (but above ver did). 
            // Marks some sectors bad for this block (just like player and usbmsc do it). 
            // Jan23'06: marks 1st 2 sectors bad that it can & last one in block that it can.
            if( (iRetCode = HALMarkBlockBadInRA(pNandPhysicalMediaDesc, iBadBlockNumber)) != SUCCESS ) 
            {   DebugBuildAssert(0); // future possible error handling.  Try to mark it bad again or return iRetCode now?
            }        
#endif

        } // for (j = 0; j < g_NandBadBlocks[i].iNumBadBlocks; j++)

        // Release the NAND
        NANDHalReleaseNand(pNandPhysicalMediaDesc);

        // Disable writes to chip
        (*pNandHalApiDesc->pDisableWrites)(WP_NAND_INTERNAL);

    } // for ( i = 0; i < iNumChips; i++)

    return iRetCode;
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



