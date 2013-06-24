///////////////////////////////////////////////////////////////////////////////
//  Copyright(C) SigmaTel, Inc. 2000-2005
//  File        : LowResolutionAdc.c
//  Description : Low Resolution ADC API
///////////////////////////////////////////////////////////////////////////////
//  For the 3500 device
//  LowResolutionAdc.c has been segregated per device in the 3410 and 3500 folders
///////////////////////////////////////////////////////////////////////////////

#include "types.h"

#ifdef STMP_BUILD_PLAYER
// Need to remove this header file and include only the header file
// for the specific registers needed
#include "hwequ.h"
#endif

#include "lowresolutionadc.h"


WORD wAdcBattResolution;
WORD wAdcAuxResolution;
WORD wAdcAux2Resolution;

#ifndef STMP_BUILD_PLAYER
#ifndef STMP_BUILD_RTCTEST
#ifndef STMP_BUILD_LRADCTEST
// For the player these are defined in button.asm file
WORD  LowResAdcAux2Avg, LowResAdcAux2Result;
#endif
#endif
#endif
static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}
#ifdef MTP_BUILD                               
#ifdef FUNCLET
  #pragma asm
    extern SysCallFunclet
  #pragma endasm
#endif    
#endif    


#pragma optimize 1
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysLowResolutionAdcReadVolt
//
//   Type:          Function 
//
//   Description:   Returns the instantaneous Result of the Low Resolution ADC Channel specified
//                  in mV
//
//   Inputs:        Constant specifying the Low Resolution ADC Channel
//                      BATT for Battery channel
//                      AUX for Auxiliary channel
//
//                  Constant specifying which value to return, instantaneous or average)
//
//   Outputs:       Returns the ADC channel result in mVolts
//
//   Notes:        
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        FSysLowResolutionAdcReadVolt:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_SYSLOWRESOLUTIONADCREADVOLT

        org p,"SYSFUNCLET_SysLowResolutionAdcReadVolt_P":
        dc      RSRC_FUNCLET_SYSLOWRESOLUTIONADCREADVOLT
    #pragma endasm

INT SysLowResolutionAdcReadVolt_Funclet(BYTE bAdcChannel, BOOL bAverage)
#else
INT SysLowResolutionAdcReadVolt(BYTE bAdcChannel, BOOL bAverage)
#endif    
#else	// Non-MTP Build
INT SysLowResolutionAdcReadVolt(BYTE bAdcChannel, BOOL bAverage)
#endif
{
    INT iAdcResult;

    if (bAverage == TRUE)
    {
    	iAdcResult = SysLowResolutionAdcReadAvg(bAdcChannel);
    }
    else
    {    
    	iAdcResult = SysLowResolutionAdcReadBin(bAdcChannel);
    }

    switch(bAdcChannel)
    {   // this mpy for each is actually smaller than saving to var & mpy once due to optimize pragma. 
        case BATT:
            iAdcResult = iAdcResult * wAdcBattResolution;
            break;
            
        case AUX:
            iAdcResult = iAdcResult * wAdcAuxResolution;
            break;

        case AUX2:
            iAdcResult = iAdcResult * wAdcAux2Resolution;
            break;

        default:
            break;
    }
    
    iAdcResult = iAdcResult / 10;       // Channel Resolution encoded in mV*10
        
    return(iAdcResult);

}                                             
            
#ifdef MTP_BUILD                               
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextLowResolutionAdc":
   #pragma endasm
#endif
#endif
        