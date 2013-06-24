///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
// File        : PersistBits.c 
// Description : Persistent Bits API
///////////////////////////////////////////////////////////////////////////////
// This file handles the Persistent Bits ("Sticky Bits") on the STMP35xx 
// parts.
///////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "regsrtc.h"
#include "persist_bits.h"


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          StickyBitInit
//
//   Type:          Function 
//
//   Description:   Initialize the Persistent Bit functionality. 
//
//   Inputs:        none
//
//   Outputs:       return code.
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
void _reentrant StickyBitInit(void)
{    
    
    // Remove SoftReset so copy controller can run
    HW_RTC_CSR.B.SFTRST = 0;
    // Force an update
//    HW_RTC_CSR.B.FORCE = 1;  //REVIEW: why was this commented out? Is it needed? JLN
}

// Inputs none
// Description: force the sync between analog and digital domains. 
void _reentrant StickyBitForceUpdate(void)
{    
    HW_RTC_CSR.B.FORCE = 1; // Force an update
}


/////////////////////////////////////////////////////////////////////////////////////////
//>  Name:          ReadStickyBit
//
//   Type:          Function 
//
//   Description:   Reads the present value of the given Persistent bit and returns 
//                  the result.
//
//   Inputs:        WORD * pSBPointer
//                  WORD BitPosition
//                  BOOL bResult
//
//   Outputs:       return code.
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant ReadStickyBit(volatile _X WORD * pSBPointer, WORD BitPosition, 
                                 BOOL * bResult)
{    
    WORD wStickyBitMask = 1 << BitPosition;
    WORD wRetValue = !SUCCESS;
    WORD i;

    if (HW_RTC_CSR.B.SFTRST)
    {
       // Copy Controller Disabled - Soft Reset
       SystemHalt();
       HW_RTC_CSR.B.SFTRST = 0;
    }
    for (i=0;i<PERSIST_WAIT_TIME;i++)
    {
      if (!HW_RTC_CSR.B.STALEREGS)
      {
          // Set the new value of the Sticky Bit.
          *bResult = (*pSBPointer & wStickyBitMask);
          // shift result bit to lsb to convert to BOOL. Note that TRUE is lsb set.    
          if( *bResult != 0 )
            *bResult = TRUE;
          // else *bResult is 0 which is FALSE.
          wRetValue = SUCCESS; 
          break;
      }
    }
	return wRetValue;
}

          
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SetStickyBit
//
//   Type:          Function 
//
//   Description:   Set the given Persistent bit.
//   Inputs:        WORD * pSBPointer
//                  WORD BitPosition
//
//   Outputs:       return code.
//
//   Notes:         Warning, we found that this function required a SysDelay after 
//                  it before system was intentionally reset, in order for bit to 
//                  persist to the next player boot. 
//<
//////////////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SetStickyBit(volatile _X WORD * pSBPointer, WORD BitPosition)
{
    WORD wStickyBitMask = 1 << BitPosition;
    WORD wRetValue = !SUCCESS;
    WORD i;

    for (i=0;i<PERSIST_WAIT_TIME;i++)
    {
      if (!HW_RTC_CSR.B.NEWREGS)
      {
          // Set the new value of the Sticky Bit.
          *pSBPointer |= wStickyBitMask;
          wRetValue = SUCCESS;
      }
    }
	return wRetValue;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ClearStickyBit
//
//   Type:          Function 
//
//   Description:   Clear the given Persistent bit.
//
//   Inputs:        WORD * pSBPointer
//                  WORD BitPosition
//
//   Outputs:       return code.
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant ClearStickyBit(volatile _X WORD * pSBPointer, WORD BitPosition)
{
    WORD wStickyBitMask = 1 << BitPosition;
    WORD wRetValue = !SUCCESS;
    WORD i;

    for (i=0;i<PERSIST_WAIT_TIME;i++)
    {
      if (!HW_RTC_CSR.B.NEWREGS)
      {
          // Set the new value of the Sticky Bit.
          *pSBPointer &= ~wStickyBitMask;
          wRetValue = SUCCESS;
      }
    }
	return wRetValue;
}
