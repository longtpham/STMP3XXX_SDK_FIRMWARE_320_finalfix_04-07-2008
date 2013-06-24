/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: NandRA_utils.c
// Description: Redundant Area Utility (access) routine
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//  Include Files
/////////////////////////////////////////////////////////////////////////////////

#include "types.h"
//#include "ddildl.h"
#include "..\..\COMMON\nandstructs.h"
#include "NANDRA_utils.h"
//#include "nanddevdrvapi.h"


extern WORD _X SectorSize;
extern WORD _X SectorSizeInWordsPlusOne;
/////////////////////////////////////////////////////////////////////////////////
//  Functions
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//  Globals
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
//  Defines
/////////////////////////////////////////////////////////////////////////////////


//#define BLOCK_STATUS_MAJORITY    1  // not enough room for this.

// Get Byte from Array macro.

_asmfunc BYTE GetByteFromArrayInX( void _X * p,int n);
_asmfunc void PutByteInArrayInX( void _X * p,int n, int newchar);

/////////////////////////////////////////////////////////////////////////////////
//  Layout 2
//
//  Layout 2 is a new Redundant Area Layout, which is no longer SSFDC-compliant (SmartMedia).
//
//  Layout 2 always uses Reed-Solomon ECC, which uses a 9 byte parity code.  The ECC is 
//  performed over the first 519 bytes of the 528-byte sector;  the remaining 9 bytes are
//  the parity code, itself.  This provides ECC protection for the "data" bytes in the 
//  Redundant Area (e.g. Block Status and LBA).
//
//  Since applications just read the Redundant Area in many cases (e.g. to search for
//  STMP blocks ot to build a USectors Map), additional protection of the "data" in the
//  R.A. is provided by a CRC byte.  This byte provides a data integrity check for the other
//  6 "data" bytes.  If you read an R.A. and the CRC check fails, you can read the whole 
//  528-byte sector and use the Reed Solomon ECC to correct the errors.
//
//  The ECC hardware requires that the data and parity bytes each be in contiguous blocks
//  of RAM (they don't have to be adjacent to each other).  But the STMP3500 Boot ROM expects
//  the STMP Tag to be in bytes 518 and 519 of the sector, which does not leave a block of 9
//  contiguous bytes to hold the ECC parity.  So bytes have to be "juggled", one way or another.
//  An efficient way of doing this is to swap bytes 513-515 with bytes 519-521.  In one form,
//  Layout 2S ("Standard"), the 519 data bytes are contiguous, followed by 9 contiguous ECC
//  parity bytes. In the other form, Layout 2N (the image stored on NAND), the STMP Tag bytes
//  and Block Status bytes are in the positions expected by the Boot ROM, bootmanager, and
//  Boot EEPROM.  The nice feature of these two layouts is that switching between them requires
//  just a single 24-bit WORD swap (WORD 171 with WORD 173), which is much quicker than swapping
//  individual bytes around.
//
//  Another advantage of Layout 2S is that the CRC calculation can be done on two WORDS (171 & 
//  172), instead of extracting individual bytes.  The CRC "check" will normally be done from
//  a 16-byte version of the R.A., in the Layout 2S form;  so it will require addressing 
//  individual bytes.
//
//  THE RULE HERE IS THAT WE DON'T DO ANY OPERATIONS EXCEPT *SWAP* ON THE "2N" FORM OF LAYOUT 2.
//
//  This rule might not be followed in the bootmanager, Boot EPROM, and Boot ROM code.  That 
//  remains to be seen.
//
//
//  Offset is from 512 for small sectors, 2048 for large sectors
//
//  Layout 2S ("Standard")
//   0  	RA CRC
//   1      LBA 1 / STMP Tag 1
//   2      RSI 0
//   3      RSI 1
//   4      Customer Byte
//   5      Block Status
//   6      LBA 0 / STMP Tag 0
//   7      RS_ECC 0
//   8      RS_ECC 1
//   9      RS_ECC 2
//  10      RS_ECC 3
//  11      RS_ECC 4
//  12      RS_ECC 5
//  13      RS_ECC 6
//  14      RS_ECC 7
//  15      RS_ECC 8
//
//  Layout 2N ("NAND")
//   0      RA CRC
//   1      RS_ECC 0
//   2      RS_ECC 1
//   3      RS_ECC 2
//   4      Customer Byte
//   5      Block Status
//   6      LBA 0 / STMP Tag 0
//   7      LBA 1 / STMP Tag 1
//   8      RSI 0
//   9      RSI 1
//  10      RS_ECC 3
//  11      RS_ECC 4
//  12      RS_ECC 5
//  13      RS_ECC 6
//  14      RS_ECC 7
//  15      RS_ECC 8
//
//
//  Definitions:
//      RA CRC - CRC parity for 6 "data" bytes in the Redundant Area
//		LBA    - Logical Block Address (which block in the Region, starting at 0)
//      RSI    - Relative Sector Index (which sector in the block, starting at 0)
//      RS_ECC - Reed-Solomon Error Correction Code parity bytes (0-8)
//
/////////////////////////////////////////////////////////////////////////////////





// Defines of offsets, relative to the beginning of the Redundant Area
// Layout 2


// The RA CRC covers bytes 1-6
#define RA_CRC_DATA_OFFSET      1
#define RA_CRC_SIZE             6

/////////////////////////////////////////////////////////////////////////////////
//  IMPORTANT: The SWAP definitions below assume Layout 2S and 2N, as described 
//  above.  If the layouts are changed, these values, or even the whole NANDRaSwap()
//  routine may need to be changed.
/////////////////////////////////////////////////////////////////////////////////

#define RA_SWAP_WORD_OFFSET_1   0
#define RA_SWAP_WORD_OFFSET_2   2
#define RA_SWAP_BYTE_OFFSET_1   1
#define RA_SWAP_BYTE_OFFSET_2   7





/////////////////////////////////////////////////////////////////////////////////
//> Name: NANDRA_ToStandardForm
//
//  Type: Function
//  Description: 	Convert from "NAND" form to "Standard" (internal) form
//  Inputs:         pRA  - Pointer to whole Sector (528 bytes) or to isolated 
//                         Redundant Area buffer (16 bytes)
//                  bWholeSector - TRUE if pRA points to a whole Sector
//  Outputs:        SUCCESS/?
//  Notes:          Just swap the Redundant Area between Layout 2S and Layout 2N.
//                  ASSUMES RA IS IN "NAND" FROM - DOESN'T CHECK.
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant NANDRA_ToStandardForm(WORD _X * pRA, BOOL bWholeSector) {

  WORD wTmp1, wTmp2;
  int i;

#if 0
  if (bWholeSector) {      // Assuming 1 WORD, aligned
    short sIndex1, sIndex2;
    sIndex1 = sIndex2 = SectorSizeInWordsPlusOne;
    sIndex1 += RA_SWAP_WORD_OFFSET_1;
    sIndex2 += RA_SWAP_WORD_OFFSET_2;

    wTmp1 = pRA[sIndex1];
    pRA[sIndex1] = pRA[sIndex2];
    pRA[sIndex2] = wTmp1;
  }
  else {  // 16-byte R.A.  Access byte-by-byte
    for (i=0; i<3; i++) {  // Assuming 3 consecutive bytes!!
      wTmp1 = GetByteFromArrayInX(pRA, RA_SWAP_BYTE_OFFSET_1 + i );
      wTmp2 = GetByteFromArrayInX(pRA, RA_SWAP_BYTE_OFFSET_2 + i );

      PutByteInArrayInX(pRA, RA_SWAP_BYTE_OFFSET_1 + i, wTmp2);
      PutByteInArrayInX(pRA, RA_SWAP_BYTE_OFFSET_2 + i, wTmp1);
    }
  }
#else
  if (bWholeSector) {      // Assuming 1 WORD, aligned
    short sIndex1, sIndex2;
    sIndex1 = sIndex2 = SectorSizeInWordsPlusOne;
    sIndex1 += RA_SWAP_WORD_OFFSET_1;
    sIndex2 += RA_SWAP_WORD_OFFSET_2;

    wTmp1 = pRA[sIndex1];
    pRA[sIndex1] = pRA[sIndex2];
    pRA[sIndex2] = wTmp1;
  }
  else {  // 16-byte R.A.  Access byte-by-byte
    wTmp1 = pRA[0] & 0x00FFFF;
    pRA[0] = (pRA[0] & 0xFF0000) | (pRA[3] & 0x00FFFF);
    pRA[3] = (pRA[3] & 0xFF0000) | wTmp1;

    wTmp1 = pRA[1] & 0xFF0000;
    pRA[1] = (pRA[1] & 0x00FFFF) | (pRA[4] & 0xFF0000);
    pRA[4] = (pRA[4] & 0x00FFFF) | wTmp1;
  }
#endif
  return SUCCESS;
}





/////////////////////////////////////////////////////////////////////////////////
//> Name: NANDRAGetByte
//
//  Type: Function
//  Description:	Get a specified byte from the Redundant Area
//  Inputs:         pRA  - Pointer to whole Sector or to isolated
//                         Redundant Area buffer
//                  wByteOffsetInRA - 0=beginning of the RA
//                  bWholeSector - TRUE if pRA points to a whole Sector
//                  pwValue - WORD to return byte value in
//  Outputs: 		SUCCESS/?
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant NANDRAGetByte(WORD _X * pRA, BOOL bWholeSector, WORD wByteOffsetInRA,
                         WORD *pwValue) {

  int iOffset = bWholeSector ? SectorSize : 0;

  *pwValue = GetByteFromArrayInX(pRA, wByteOffsetInRA + iOffset);

  return SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////
//> Name: NANDRAGetBlockStatus
//
//  Type: Function
//  Description:	Get Block Status (Good Block = 0xFF, else BAD) 
//  Inputs:         pRA  - Pointer to whole Sector (528 bytes) or to isolated 
//                         Redundant Area buffer (16 bytes)
//                  bWholeSector - TRUE if pRA points to a whole Sector
//                  pwBlockStatus - WORD to return Block Status byte in
//  Outputs: 		SUCCESS/?
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant NANDRAGetBlockStatus(WORD _X * pRA, BOOL bWholeSector,
                         WORD *pwBlockStatus){

  NANDRAGetByte(pRA, bWholeSector, RA_BLOCK_STATUS_OFFSET, pwBlockStatus);

  return SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////
//> Name:  NANDRAGetLBA
//
//  Type: Function
//  Description: 	Get Logical Block Address and "Relative" Sector Index
//  Inputs:         pRA  - Pointer to whole Sector or to isolated
//                         Redundant Area buffer
//                  bWholeSector - TRUE if pRA points to a whole Sector
//                  pwLBA - WORD to return the Logical Block Address in
//                  pwLSA - WORD to return the Sector Index, within the block
//  Outputs:        SUCCESS/?
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant NANDRAGetLBA(WORD _X * pRA, BOOL bWholeSector,
                 WORD *pwLBA, WORD *pwLSA) {

  int iOffset = bWholeSector ? SectorSize : 0;

  *pwLBA = GetByteFromArrayInX(pRA, RA_LBA_0_OFFSET + iOffset) +
           GetByteFromArrayInX(pRA, RA_LBA_1_OFFSET + iOffset) * 0x100;

  *pwLSA = GetByteFromArrayInX(pRA, RA_RSI_0_OFFSET + iOffset) +
           GetByteFromArrayInX(pRA, RA_RSI_1_OFFSET + iOffset) * 0x100;

  return SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////
//> Name: NANDRAGetStmpCode
//
//  Type: Function
//  Description:	Get System Block Stmp code (e.g. "ST", "MP", etc.) 
//  Inputs:         pRA  - Pointer to whole Sector or to isolated
//                         Redundant Area buffer
//                  bWholeSector - TRUE if pRA points to a whole Sector
//                  pwStmpCode - Pointer to WORD to return the STMP code (in lower two bytes)
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant NANDRAGetStmpCode(WORD _X * pRA, BOOL bWholeSector,
                          WORD *pwStmpCode) {

  int iOffset = bWholeSector ? SectorSize : 0;

  *pwStmpCode = GetByteFromArrayInX(pRA, RA_STMP_TAG_0_OFFSET + iOffset) + 
                (GetByteFromArrayInX(pRA, RA_STMP_TAG_1_OFFSET + iOffset) << 8);

  return SUCCESS;
}



/*
ComputeCRC
CheckCRC
ComputeECC
CorrectECC
MarkBad
*/

#if 1
#pragma asm

OneThird equ 0.333333333333

    ;
    ; A contains the byteCounter  (It will not work if A2 is non zero!)
    ; return value: A1 contains the result. 
    ; A contains the byteCounter
GetByteFromArrayInX:
FGetByteFromArrayInX:
    clr B     A,x0       #>OneThird,y1                
    mpy  x0,y1,A  #>$200000,x0
    move A0,B1
    lsr  B  A1,n0                    ; Avoid sign.
    cmp  x0,B #>$ff,y1
    jlt Get_CaseA                   
    move #>$400000,x0
    cmp x0,B X:(r0+n0),x0
    jlt Get_CaseB

    ; Case C follows.
    ; In this case data is in upper 8 bits
Get_CaseC
    mpy #16,x0,A  
    and y1,A
    move    A1,A
    rts
 
Get_CaseB    ; Case B: Middle byte
    mpy  #8,x0,A 
    and y1,A  
    move    A1,A
    rts
 
Get_CaseA
    ;Case A
    ;Lower Byte
    move    X:(r0+n0),A
    and y1,A
    move    A1,A
    rts

    

#pragma endasm
#endif
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  EOF  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
