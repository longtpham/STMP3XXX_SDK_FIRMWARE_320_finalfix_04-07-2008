///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename:    NANDHalCorrectECC.c
// Description: Compute the ECC for the specified Sector's data and verify the ECC
//                with the fields in the Redundant Area. If the check fails, attempt
//                to correct the data.
//
//              NOTE: The "1bit" version is used for Type1 and Type2 NANDs.
//
///////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "regsecc.h"
#include "NANDRA_utils.h"

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

#define HW_ECC_CSR0_ECCSEL_SSFDC_DECODE                     (0x010000)
#define HW_ECC_CSR1_ECCEXCEPTION_SSFDC_MORE_1_ERR_SETMASK   (0x008000)

// At 6 cycles per loop & all possible digital clk freq, this value needs to give >50 microsec delay.
// At 24MHz, 1000 6 cycle loops gives 250 microsec delay.  At 75MHz, 1000 6 cycle loops is 80 usec.
#define RSECC_DELAY_COUNTER_VALUE 1000


///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Function Prototypes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Externs
///////////////////////////////////////////////////////////////////////////////
extern WORD _X SectorSize;
extern WORD _X SectorSizeInWordsPlusOne;

///////////////////////////////////////////////////////////////////////////////
// Functions
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//> Name:        NANDHalCorrectPartial
//
//  Type:        Function
//  Description: Do the actual work of correcting a block of data
//  Inputs:
//  Outputs:
//  Notes:
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NANDHalCorrectPartial( P_SECTOR_BUFFER pCompleteSectorBuf, P_SECTOR_BUFFER pECCBuf, WORD blockStartIndex )
{
    HW_ECC_CSR0.U = 0x800000;   // soft reset
    HW_ECC_CSR0.U = 0x030002;   // RS Decode, autocorrect

    HW_ECC_CSR1.U = 0x0001f0;   // clear any residual crud

    HW_ECC_RSCFG.U = 0x101ce9;  // BYTEEN = 1, 519 bytes, 9 bit symbols
    HW_ECC_BLKSTRTADDR.U = (WORD)pCompleteSectorBuf;
    HW_ECC_BLKSTRTINDEX.U = blockStartIndex;

    HW_ECC_PARSTRTADDR.U = (WORD)pECCBuf;
    HW_ECC_PARSTRTINDEX.U = 0;

    // Start the ecc decode.
    HW_ECC_CSR0.B.ECCKICK = 1;

    // Wait till finished. Sigh.
    // we need about 50 microseconds of pause to determine that the engine isn't coming back, so, we use counter
    {
        WORD counter = RSECC_DELAY_COUNTER_VALUE;
        while (HW_ECC_CSR0.B.ECCKICK && counter) 
        {
            counter--;
        }
        if (!counter)
        {
            return NANDHAL_ECC_FIX_FAILED;
        }
    }

    // check for errors
    if (HW_ECC_CSR1.B.UNCORRIN)
    {
      // ECC exception. More than 4 errors.
      // error.
      return NANDHAL_ECC_FIX_FAILED;
    }
    return SUCCESS;
}


///////////////////////////////////////////////////////////////////////////////
//> Name:        NANDHalCorrectECC_4Bit
//
//  Type:        Function
//  Description: Compute the ECC for the specified sector's data and verify
//                 the ECC fields in its redundant area. If the check fails,
//                 try to correct the data.
//
//  Inputs:      P_SECTOR_BUFFER pCompleteSectorBuf
//  Outputs:     Returns FAIL if data correction fails, else Returns SUCCESS.
//  Notes:
//<
/////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NANDHalCorrectECC_4Bit(WORD _X * pCompleteSectorBuf)
{
    short i = (SectorSize>>9);
    RETCODE status = NANDHAL_ECC_NOERROR;

    WORD wLBA, wRSI, blockStartIndex;
    P_SECTOR_BUFFER pECCBuf = &pCompleteSectorBuf[RA_ECC_POS+(3*(i-1))];

    // check for an erased block. Do this by checking the sector # field.
    // If it's all 1's then the block is erased, so just return success
    // (there's never a sector # > 0xFFFE)
    NANDRAGetLBA(pCompleteSectorBuf, WHOLE_SECTOR, &wLBA, &wRSI);
    pCompleteSectorBuf = &pCompleteSectorBuf[171*(i-1)];
    if (wRSI != 0xffff)
    {
        for ( /* i already initialized*/; (SUCCESS == status) && i; --i )
        {
            switch( i )
            {
            case 4:
                --pCompleteSectorBuf;
                blockStartIndex = 0;
                break;
            case 3:
                blockStartIndex = 8;
                break;
            case 2:
                blockStartIndex = 16;
                break;
            default:
            case 1:
                pCompleteSectorBuf += (SectorSize>512);  // increment by one if large sector
                blockStartIndex = 0;
                break;
            };
            status |= NANDHalCorrectPartial(pCompleteSectorBuf, pECCBuf, blockStartIndex );

            pCompleteSectorBuf = pCompleteSectorBuf-171;        // Really 171 here... incrementing by small sectors
            pECCBuf = pECCBuf-3;
        }
    }

    return (status);
}


/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  EOF  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
