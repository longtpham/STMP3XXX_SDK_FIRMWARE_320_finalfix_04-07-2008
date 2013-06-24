///////////////////////////////////////////////////////////////////////////////
//!
//! \file flashtiming.c
//! \brief Functions to setup the flash port timings.
//!
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////
#include <types.h>
#include <regsemc.h>

#if defined(DEVICE_3500)
    #include <regsgpflash.h>
#endif // if defined(DEVICE_3500)

///////////////////////////////////////////////////////////////////////////////
//  CompactFlashSetPortTiming
//!
//! \param wTiming Compact Flash port timing
//!
//! \retval SUCCESS - Always success.
//!
//! Notes:         none
//!
///////////////////////////////////////////////////////////////////////////////
RETCODE CompactFlashSetPortTiming(WORD wTiming)
{
    // Set the timing
    HW_FLCFTMR1R.I = wTiming;
    
    return SUCCESS;
}


#if 0   // TOVERIFY: TODO: compare this to the new timing function from the lbmlc project checking for items to port there. Then delete this block to EOF.
#if defined(DEVICE_3500)
// JLN: This approach is cleaner but gives identical timings as previously for the 60Mhz case.
// The 48Mhz DCLK case is handled elagantly by just setting the min setup and hold times 
// here and always passing in the real integer rounded up dsp clock period.
// At 60MHz, 1T=16.66ns. At 48Mhz 1T=20.83ns.
// Start flash timing nanosecond constant defines: 
#define FLASH_TSU_MIN       (0)         // tCLS, tCS, tALS
#define FLASH_THOLD_MIN     (10)        // tCLH, tCH, tALH, tDH
#define FLASH_TDS_TDH_MIN   (80)        // tRC, tWC.  This is min(TDS+TDH)   TOVERIFY2 warning: sdk3.05 changed this from 50 to 80ns min. Consider that in nandtables.h

#if DEBUG
#ifdef NAND1
#define FLASH_TDS_MIN       (61)        // tWP, tDS.       At 60Mhz we used 3T=49.99ns. min was met. At 48Mhz we have 3T=62.5ns. we use 68MHz. sdk3.05 changed to these 2 ln from 45 30 values.
#define FLASH_TDH_MIN       (48)        // tWH, tREH, tDH. At 60Mhz we used 2T=33.3ns.  min was met. At 48Mhz we have 2T=41.66ns.
#else
#ifdef NAND2
#define FLASH_TDS_MIN       (61)        // tWP, tDS.       At 60Mhz we used 4T=66.66ns, min chosen as 5 less. At 48Mhz we have 3T=62.5ns.
#define FLASH_TDH_MIN       (48)        // tWH, tREH, tDH. At 60Mhz we used 3T=49.99ns, min chosen as 1 less. At 48Mhz we have 3T=62.5ns.
#else
#ifdef NAND3
#define FLASH_TDS_MIN       (82)        // tWP, tDS.       At 60Mz we used 5T=83.33ns, min chosen as 1 less. At 48Mhz we have 4T=83.33ns.
#define FLASH_TDH_MIN       (48)        // tWH, tREH, tDH. At 60Mz we used 3T=49.99ns, min chosen as 1 less. At 48Mhz we have 3T=62.5ns.
#else
#ifdef NAND4
#define FLASH_TDS_MIN       (82)        // tWP, tDS.       SAME AS NAND3 CASE. 
#define FLASH_TDH_MIN       (48)        // tWH, tREH, tDH.  
#else
  // DEBUG defined but not (NAND1 NAND2 NAND3 or NAND4)
#define FLASH_TDS_MIN       (61)        // tWP, tDS.       Same as NAND1 case above.   sdk3.05 changed to these values from 45 and 30 value 2 lines.
#define FLASH_TDH_MIN       (48)        // tWH, tREH, tDH. "
#endif    
#endif    
#endif    
#endif
#else
 // non DEBUG (RETAIL) builds (non engr board layouts) get one best case timing set. 
 // You may be able to reduce these two values per your flash datasheet to increase performance. 
#define FLASH_TDS_MIN       (45)        // tWP, tDS.        Same as NAND1 case above.
#define FLASH_TDH_MIN       (30)        // tWH, tREH, tDH.  "
#endif
// end flash timing constant defines

// Not enough RAM to locate largest build with this new err check. 
// With the tDH and tDS defines currently in use for 48mhz or 60mhz, err check unneeded.
#define MIN_FLASH_SETUP_AND_HOLD_ERR_CHECKING 0
///////////////////////////////////////////////////////////////////////////////
//  GPFlashSetPortTiming
//!
//! \param  wDspClockPeriod - DSP Clock Period in nanoseconds
//!
//! \retval SUCCESS - Always success.
//!                  min timing macro defines
//!
//!   Outputs:       RETCODE            SUCCESS   
//! Notes:
//!    TSU = 0ns 
//!    TH = 10ns Min 
//!      TDS = 25ns Min   (actual min TDS and TDH values used above are & were longer)
//!      TDH = 15ns Min   ("                                                     )
//!    TDS + TDH must be >= 50 nS
//!
///////////////////////////////////////////////////////////////////////////////
RETCODE GPFlashSetPortTiming(WORD wDspClockPeriod)
{
    WORD wTimingTdsTdh;
#if MIN_FLASH_SETUP_AND_HOLD_ERR_CHECKING    
    WORD wTimingTemp;
#endif    
    HW_GPFLASH_CSR0R.B.CLK_DISABLE = 0;
    
    HW_GPFLASH_TIMING1.B.TSU = FLASH_TSU_MIN;
    HW_GPFLASH_TIMING1.B.THOLD = (FLASH_THOLD_MIN / wDspClockPeriod) + 1;
    HW_GPFLASH_TIMING2.B.TDS = (FLASH_TDS_MIN / wDspClockPeriod) + 1;
    HW_GPFLASH_TIMING2.B.TDH = (FLASH_TDH_MIN / wDspClockPeriod) + 1;
    
#if MIN_FLASH_SETUP_AND_HOLD_ERR_CHECKING
    // JLN new min tds and tdh time enforcement as suggested by JCP:
    if( (wTimingTemp=((HW_GPFLASH_TIMING2.B.TDS)*wDspClockPeriod)) < FLASH_TDS_MIN)
    {   HW_GPFLASH_TIMING2.B.TDS += ((FLASH_TDS_MIN - wTimingTemp) / wDspClockPeriod) + 1;
    }

    if( (wTimingTemp=((HW_GPFLASH_TIMING2.B.TDH)*wDspClockPeriod)) < FLASH_TDH_MIN)
    {   HW_GPFLASH_TIMING2.B.TDH += ((FLASH_TDH_MIN - wTimingTemp) / wDspClockPeriod) + 1;
    }
#endif
    // Make sure (TDS + TDH) total is greater than FLASH_TDS_TDH_MIN
    wTimingTdsTdh = (HW_GPFLASH_TIMING2.B.TDS + HW_GPFLASH_TIMING2.B.TDH) 
                    * wDspClockPeriod;
                    
    if( wTimingTdsTdh < FLASH_TDS_TDH_MIN)
    {
        HW_GPFLASH_TIMING2.B.TDS+= ((FLASH_TDS_TDH_MIN - wTimingTdsTdh) / wDspClockPeriod) + 1;    
    }
    
    return SUCCESS;
}

#endif
#endif
