///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2004
//
// Filename:    NANDHalStartComputeECC.c for SDK2.4
// Description: FOR SDK2.4 -- THIS ROUTINE WILL RETURN AFTER
//              it has completed the ECC calculation and the populating of the 
//              redundant area.
//
///////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "nandhal.h"
#include "regsecc.h"
#include "stringlib.h"
#include "NANDRA_utils.h"



///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

#define HW_ECC_CSR0_ECCSEL_SSFDC_ENCODE             (0x000000)

///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Function Prototypes
///////////////////////////////////////////////////////////////////////////////
#ifdef ENABLE_1_BIT_ECC
void EccHamming256Encode(WORD _X * data, unsigned dataByteOffset, 
    WORD _X * parity, unsigned parityByteOffset);
#endif
///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_1_BIT_ECC
///////////////////////////////////////////////////////////////////////////////
//> Name:        NANDHalStartComputeECC_1Bit
//
//  Type:        Function
//  Description: FOR SDK2.4 -- Does the ECC calculation and then populates the
//                redundant area, before returning.
//
//  Inputs:      P_SECTOR_BUFFER pCompleteSectorBuf, WORD wWhichHalf.
//  Outputs:     Returns SUCCESS.
//  Notes:       
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NANDHalStartComputeECC_1Bit(P_SECTOR_BUFFER pCompleteSectorBuf, 
    WORD wWhichHalf)
{
    if(wWhichHalf == 0)
    {

        // Doing the even side (first 256 bytes)
        //g_wEccCount++;
        EccHamming256Encode(pCompleteSectorBuf +  0, 0, 
            pCompleteSectorBuf +  175, 0);
        // Doing the odd side (last 256 bytes)
        EccHamming256Encode(pCompleteSectorBuf + 85, 1, 
            pCompleteSectorBuf +  173, 1);
    
    }
    return (SUCCESS);
}
#endif

///////////////////////////////////////////////////////////////////////////////
//> Name:        NANDHalStartComputeECC_4Bit
//
//  Type:        Function
//  Description: FOR SDK2.4 -- JUST RETURN SUCCESS.
//
//  Inputs:      P_SECTOR_BUFFER pCompleteSectorBuf, WORD wWhichHalf.
//  Outputs:     Returns SUCCESS.
//  Notes:       
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NANDHalStartComputeECC_4Bit(P_SECTOR_BUFFER pCompleteSectorBuf, 
    WORD wWhichHalf)
{


    if (wWhichHalf)
    {
        // we never do the second half of this.....
        return SUCCESS;
    }


    HW_ECC_RSCFG.U = 0x800000;	//soft reset
	HW_ECC_RSCFG.U = 0x000000;	//out of soft reset

    HW_ECC_CSR0.U = 0x020002;   		// RS Encode, autocorrect
    HW_ECC_CSR1.U = 0x0001f0;   		// clear any residual crud
    HW_ECC_RSCFG.U = 0x101ce9;  		// BYTEEN = 1, 519 bytes, 9 bit symbols
    HW_ECC_BLKSTRTADDR.U = (WORD)&pCompleteSectorBuf[0];
	HW_ECC_BLKSTRTINDEX.U = 0;

    HW_ECC_PARSTRTADDR.U = (WORD)&pCompleteSectorBuf[RA_ECC_POS];
	HW_ECC_PARSTRTINDEX.U = 0;

    // Start the ecc encode.
    HW_ECC_CSR0.B.ECCKICK = 1;

    // Don't wait for it to finish!  This is asynch!

    return(SUCCESS);
}


#ifdef ENABLE_1_BIT_ECC
///////////////////////////////////////////////////////////////////////////////
//> Name:        EccHamming256Encode
//
//  Type:        Function
//  Description: Hamming 256 ECC encode for STMP3500 hardware ECC.
//
//  Inputs:      data - Points to start of 256 byte data block
//               dataByteOffset - Byte offset of data. 0, 1, or 2.
//               parity - Points to start of 3 byte paraity block
//               parityByteOffset - Byte offset of parity. 0, 1, or 2.
//  Outputs:     None
//  Notes:       
//<
/////////////////////////////////////////////////////////////////////////////////
void EccHamming256Encode(WORD _X * data, unsigned dataByteOffset, 
    WORD _X * parity, unsigned parityByteOffset)
{
	HW_ECC_SSFDCCFG.U = 0x800000;	//soft reset
	HW_ECC_SSFDCCFG.U = 0x000000;	//out of soft reset

    // Disable all interrupts and set SSFDC ECC encode function.
    HW_ECC_CSR0.U = HW_ECC_CSR0_ECCSEL_SSFDC_ENCODE;
    HW_ECC_CSR1.U = 0;

    // Set up all the registers.
    HW_ECC_BLKSTRTADDR.U  = (unsigned)data;
	HW_ECC_BLKSTRTINDEX.U = (dataByteOffset&0x3)<<3;
	HW_ECC_PARSTRTADDR.U  = (unsigned)parity;
	HW_ECC_PARSTRTINDEX.U = (parityByteOffset&0x3)<<3;

    // Wait till finished.
    //while (HW_ECC_CSR0.B.ECCKICK);

    // Start the ecc encode.
    HW_ECC_CSR0.U = HW_ECC_CSR0.U | HW_ECC_CSR0_ECCKICK_SETMASK;

    // Wait till finished.
    while (HW_ECC_CSR0.B.ECCKICK);

    // Clear bits 16 and 17, then invert all bits.  This is needed to get
    // the STMP3500 hardware SSFDC ECC to match the Samsung software SSFDC
    // ECC.
    if (parityByteOffset == 0)
    {
        *(parity) = ~(*parity & 0xFCFFFF);
    } else if (parityByteOffset == 1) {
        *parity = *parity ^ 0xFFFF00;
        *(parity + 1) = (*(parity + 1) & 0xFFFFFC) ^ 0x0000FF;
    } else {
        *(parity) = *parity ^ 0xFF0000;
        *(parity + 1) = (*(parity + 1) & 0xFFFCFF) ^ 0x00FFFF;
    }
}
#endif




/////////////////////////////////////////////////////////////////////////////////
//  CRC8 Lookup Table.
//  This version is "packed", with three one-byte entries per WORD.
/////////////////////////////////////////////////////////////////////////////////

static WORD crcvalues[(256+2)/3] = 
{
  0x0E0700,  0x1B1C09,  0x381512,  0x31363F,  0x2A2324,  0x77702D,  0x6C797E,  0x65626B,
  0x464F48,  0x535441,  0xE05D5A,  0xE9EEE7,  0xF2FBFC,  0xDFD8F5,  0xC4D1D6,  0xCDCAC3,
  0x9E9790,  0x8B8C99,  0xA88582,  0xA1A6AF,  0xBAB3B4,  0xC0C7BD,  0xDBCEC9,  0xD2D5DC,
  0xF1F8FF,  0xE4E3F6,  0xB7EAED,  0xBEB9B0,  0xA5ACAB,  0x888FA2,  0x938681,  0x9A9D94,
  0x292027,  0x3C3B2E,  0x1F3235,  0x161118,  0x0D0403,  0x50570A,  0x4B5E59,  0x42454C,
  0x61686F,  0x747366,  0x897A7D,  0x80878E,  0x9B9295,  0xB6B19C,  0xADB8BF,  0xA4A3AA,
  0xF7FEF9,  0xE2E5F0,  0xC1ECEB,  0xC8CFC6,  0xD3DADD,  0x6E69D4,  0x756067,  0x7C7B72,
  0x5F5651,  0x4A4D58,  0x194443,  0x10171E,  0x0B0205,  0x26210C,  0x3D282F,  0x34333A,
  0x40494E,  0x555247,  0x765B5C,  0x7F7871,  0x646D6A,  0x393E63,  0x223730,  0x2B2C25,
  0x080106,  0x1D1A0F,  0xAE1314,  0xA7A0A9,  0xBCB5B2,  0x9196BB,  0x8A9F98,  0x83848D,
  0xD0D9DE,  0xC5C2D7,  0xE6CBCC,  0xEFE8E1,  0xF4FDFA,  0x0000F3
}; 

///////////////////////////////////////////////////////////////////////////////////
//> Name:        CRC8
//
//  Type:        Function
//  Description: returns the CRC-8 for a given sequence of bytes
//
//  Inputs:      wData - Points to start of byte stream (packed into words in _X)
//               wOffset - Byte offset into data buffer (may b>2, to skip over stuff)
//               wBcount - the number of bytes over which this routine should execute
//  Outputs:     an unsigned 8 bit CRC8 value
//  Notes:       
//               This is typically used to detect errors in bytes in the Redundant Area,
//               but nothing is assumed about layout.  The "data" is assumed to be 
//               contiguous packed bytes, but they may start at any point after the
//               WORD data pointer (wData).  So you can pass it a WHOLE SECTOR or just
//               a Redundant Area buffer, with wOffset set appropriately.
//               
//<
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant CRC8(WORD _X * wData, WORD wOffset, WORD wBcount)
{
    int
        i;
    WORD
      temp,
      crc,
      newindex;
    WORD wFFcnt = 0;

    for(crc=0, i=0; i < wBcount; i++)
    {
        temp = GetByteFromArrayInX(wData, i + wOffset);
        newindex = crc ^ temp;
        crc = GetByteFromArrayInY(crcvalues, newindex);

        if (temp == 0xFF)
          wFFcnt++;
    }



    /////////////////////////////////////////////////////////////////////////////////
    //  If the RA is all FFs, it's probably erased, so the CRC byte will contain 0xFF.
    //  To match that, we force the computation to 0xFF, here.
    /////////////////////////////////////////////////////////////////////////////////

    if (wFFcnt == wBcount)
      crc = 0xFF;

    return (crc);
}



/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  EOF  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
