/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: nanddevdrv.c
// Description: C Support functions for NAND Device Driver
/////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "smequ.h"

#include "nandstructs.h"
#include "nanddevdrvapi.h"

#include "gpflash_hal.h"

#include "nandra_utils.h"


///////////////////////////////////////////////////////////////////////////////
// Exports
_reentrant WORD NandSearchForSTMPBlock(void /*struct NANDDescriptorStruct _P *pNANDDevice,
                                         WORD iDeviceNum, 
                                         WORD iComponentCode*/);
_reentrant WORD NandIncBlockNumber(void);
_reentrant WORD NandIsBlockBoundary(void);

#ifndef BOOT_SEQUENCE_ORG
_reentrant WORD NandSearchForStmpBlockReverse(/*struct NANDDescriptorStruct _P *pNANDDevice,
                                                WORD iDeviceNum,
                                                WORD iComponentCode*/);
#endif


///////////////////////////////////////////////////////////////////////////////
// Private Functions

extern _X WORD CurrentNANDBusWidth;

/////////////////////////////////////////////////////////////////////////////////
//> Name: NandSearchForStmpBlock
//
//  Type: Function
//  Description: 
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NandSearchForSTMPBlock(/*struct NANDDescriptorStruct _P *pNANDDevice,
                                         WORD iDeviceNum,
                                         WORD iComponentCode*/) {


  // In the non-ROM version, the following would be passed in as args: (review: actually used as external statics in sdk)

  extern struct NANDDescriptorStruct _P * _X pCurrentNANDDescriptor;
  struct NANDDescriptorStruct _P * _X pNANDDevice = pCurrentNANDDescriptor;
  extern WORD _X NandCurDevNum;
  extern WORD _X SmBlockTag;
  WORD iDeviceNum = NandCurDevNum;

  WORD iComponentCode = SmBlockTag;   
  // WORD iComponentCode = 0x000001;   // 1 = DCC, for testing. DCC is no longer used in 35xx SDKs.



  extern WORD _X NandBlockNum;
  int  iSectorsPerBlock = pNANDDevice->pNandDescriptorSubStruct->pBlockDescriptor->NumSectorsPerBlock;
  int  iBlockMax = pNANDDevice->TotalBlocks;
  int  iBlockNum;
  int  iSectorNum;
  extern WORD _X NandSectorNum;
  extern WORD _X FlashPageBuf;
  WORD _X *pFlashPageBuf = &FlashPageBuf;
  int iSearchPattern1, iSearchPattern2;
  WORD wSTMPCode,wBlockStatus,wMatchPattern;


  for (    ; NandBlockNum < pNANDDevice->TotalBlocks; NandIncBlockNumber()) 
  {
      iSectorNum = NandBlockNum * iSectorsPerBlock + 1;      // Get the second Sector/page in the Block [CHECKS SECTOR 1 THEN SECTOR 0 IN BLK]
      //  iSearchPattern1 = 0x00004D;  // ascii code for 'M' which is stored on 2nd page of each system block. 
      //  iSearchPattern2 = iComponentCode;
      // Put together the search pattern that needs to be matched.
      wMatchPattern = (iComponentCode<<8) | 0x00004D; // firmware component's character code OR'd with the 'M' code. See nandsystemdrivewritesector.c
      // now match pattern is set for sector/page 1

      while(1)
      {   // This while loop executes only twice (2nd and 1st page of each block)
          // It searches for pattern1 and pattern2 in the 2nd and then the 1st page
          // of each block      
          NandReadSectorDataWrapper(pNANDDevice, iDeviceNum, iSectorNum, pFlashPageBuf);    

          NANDRAGetBlockStatus(pFlashPageBuf, TRUE, &wBlockStatus);
          // If any bits are cleared, we'll continue to check, otherwise skip to next block.
          if (wBlockStatus == NAND_GOOD_BLOCK_VALUE)
          {   break;
          }

          NANDRAGetStmpCode(pFlashPageBuf, TRUE, &wSTMPCode);
          // If this is the first time through the loop, see if sector/page 1 stmpcode matches system component block match pattern.
          if(wMatchPattern != 0x005453) // "ST" is on each sys block's sector 1 in the RA (0 based).
          {
              if (wSTMPCode != wMatchPattern) // Check 2nd page for "M*" 2 character codes
              {   break;  // sector/page 1 RA field does not match desired component's stmpcode, so break while loop and for loop will test next block.
              }
          } 
          else // now need to find a matching "ST" on sector/page 0.
          {   if (wSTMPCode == wMatchPattern)
              {  NandSectorNum = iSectorNum;         // Success!
                 return (0);
              }
              else
              { // sector 0 RA field does not match stmpcode, break out of while loop so we can test next block in for loop
                break;
              }
          }
          // Page 1 RA field matched the system block code pattern, lets compare the 0th page RA stmp code next
          iSectorNum--;  // change from sector/page 1 to 0.
          //    iSearchPattern1 = 0x000053; // 'S'
          //    iSearchPattern2 = 0x000054; // 'T'
          // Put together the search pattern that needs to be matched.
          wMatchPattern = (0x005453);
        } // end while 1
    } // end for

  return(1);                                                 // Failed to find STMP block
}




/////////////////////////////////////////////////////////////////////////////////
//> Name: NandIncBlockNumber
//
//  Type: Function
//  Description: 
//  Inputs:       None
//  Outputs:     Status: 0=SUCCESS, else "maxed out"
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NandIncBlockNumber() {
  extern WORD g_wNandBlockNum;
  extern struct NANDDescriptorStruct _P *pCurrentNANDDescriptor;
  extern WORD BootInfo;


  NandBlockNum++;

  if (NandBlockNum >= pCurrentNANDDescriptor->TotalBlocks) {
    BootInfo |= SM_INFO_END_OF_BLOCKS;
    return(1);                        // Maxed out. (Past last block)
  }

  return(0);                        // Success
}


#ifndef BOOT_SEQUENCE_ORG
/////////////////////////////////////////////////////////////////////////////////
//> Name: NandSearchForSTMPBlockReverse
//
//  Type: Function
//  Description: 
//  Inputs:   No func params. extern static WORD _X NandSectorNum, and 5 other externs below.
//
//  Outputs:  1 if failed to find STMP block, otherwise ... and static(s) _____ set. Returns 0 when ___
//
//  Notes: originally from sgtl korea: 1st added in stmp35xx sdk2.610
//         Derived from NandSearchForSTMPBlock func above. Ver below port into 3.120 is logically same as 2.610. 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NandSearchForSTMPBlockReverse(/*struct NANDDescriptorStruct _P *pNANDDevice,
                                                WORD iDeviceNum,
                                                WORD iComponentCode*/)
{
  // In the non-ROM version, the following would be passed in as args:

  extern struct NANDDescriptorStruct _P * _X pCurrentNANDDescriptor;
  struct NANDDescriptorStruct _P * _X pNANDDevice = pCurrentNANDDescriptor;
  extern WORD _X NandCurDevNum;
  extern WORD _X SmBlockTag;
  WORD iDeviceNum = NandCurDevNum;

  WORD iComponentCode = SmBlockTag;   

  extern WORD _X NandBlockNum;
  int  iSectorsPerBlock = pNANDDevice->pNandDescriptorSubStruct->pBlockDescriptor->NumSectorsPerBlock;
  WORD  wBlockNum; // REVIEW: needs a default value here! see use below that depends on global NandSectorNum to avoid read of this before this is set. 
  int  iSectorNum;
  extern WORD _X NandSectorNum;
  extern WORD _X FlashPageBuf;
  WORD _X *pFlashPageBuf = &FlashPageBuf;
  WORD wSTMPCode,wBlockStatus,wMatchPattern;


  // Start from last block on this NANDDevice. REVIEW NOTE: firmware we want is at last blocks of last nand chip enable device.  
  // REVIEW: How does this pNANDDevice->TotalBlocks work in multinand case with >1 chip enable? This version passed 2 chip enable cases in QA for SDK 2.610. 

  for (NandBlockNum = (pNANDDevice->TotalBlocks - 1);  
       0 <= (INT) NandBlockNum;    NandBlockNum--) 
  {
      iSectorNum = NandBlockNum * iSectorsPerBlock + 1;      // Get the second Sector in the Block

      // Put together the search pattern that needs to be matched.
      wMatchPattern = (iComponentCode<<8) | 0x00004D; // firmware component code OR'd with 'M" char code

      while(1)
      {    // This while loop executes only twice (or once) 2nd and 1st page/sector of each block checked)
           // It searches for pattern1 and pattern2 in the 2nd and then the 1st page
           // of each block      
           NandReadSectorDataWrapper(pNANDDevice, iDeviceNum, iSectorNum, pFlashPageBuf);    

           NANDRAGetBlockStatus(pFlashPageBuf, TRUE, &wBlockStatus); // BB_REVIEW: better to consider calling IsBlockBad central function
           // If any bits are cleared (indicates bad or system blocK), we'll continue to check if this is a system block, otherwise skip to next block.
           if (wBlockStatus == NAND_GOOD_BLOCK_VALUE)// defect9540 solved by ensuring page 1 appears bad here when block is bad, instead of only page 0 as before.
           {
               if (NandSectorNum)   // REVIEW: comment use of this global to prove correctness. If this is non-zero on first block checked good case, defect occurs (wBlockNum un-init junk value)
               {   
                   NandBlockNum = wBlockNum; // REVIEW: compiler warning: possibly uninitialized wBlockNum here!!!! Sgtl Korea author here says its safe but agrees. We should give it a default init value. 
                   return (0);
               }
               break;
           }

           NANDRAGetStmpCode(pFlashPageBuf, TRUE, &wSTMPCode);
           // If this is the first time through the loop, see if the 2nd sector's RA field matches the "ST" pattern for STMP to indicate system block.
           if(wMatchPattern != 0x005453)  // "ST"
           {   if (wSTMPCode != wMatchPattern)
               {   break;  // 2nd sector does not match, break so we can test next block.
               }
           } 
           else // check for "ST" since we are on page 0 (this will confirm it is also a system block)
           {   if (wSTMPCode == wMatchPattern)
               {   wBlockNum = NandBlockNum;
                   NandSectorNum = iSectorNum;         // Success
                   break;
               }
               else
               {   // 1st sector does not match, break so we can test next block
                   break;
               }
           }
           // Second Page Matched, lets search the 1st page
           iSectorNum--;
           // Put together the search pattern that needs to be matched on the 1st page.
           wMatchPattern = (0x005453); // "ST"

        } // end while sector 1 and 0 check

    }// end for(last block; block>=0; block--

    if (pCurrentNANDDescriptor->TotalBlocks)
    {   extern WORD BootInfo;
        BootInfo |= SM_INFO_END_OF_BLOCKS;
    }
    return(1);                                                 // Failed to find STMP block
}
#endif


/////////////////////////////////////////////////////////////////////////////////
//> Name: NandIsBlockBoundary
//
//  Type: Function
//  Description:   Return TRUE (1) if current Sector index is at the beginning of a block.
//  Inputs:     None
//  Outputs:     boolean
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NandIsBlockBoundary() {
  extern struct NANDDescriptorStruct _P *pCurrentNANDDescriptor;
  extern WORD NandSectorNum;
  WORD iBlockSizeModuloMask;

  iBlockSizeModuloMask = pCurrentNANDDescriptor->pNandDescriptorSubStruct->pBlockDescriptor->NumSectorsPerBlock - 1;

  if (NandSectorNum & iBlockSizeModuloMask)
    return(0);                  // FALSE - not on Block Boundary
  else
    return(1);
}

/////////////////////////////////////////////////////////////////////////////////
//> Name: checkPadPower
//
//  Type: Function
//  Description:     Turn on extra pads for 16-bit NANDs
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////


WORD checkPadPower(void) {
  extern struct NANDDescriptorStruct _P *pCurrentNANDDescriptor;

  if (CurrentNANDBusWidth == 16)
    return (PowerUpFlashPads_16bit());

  return 0;
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
