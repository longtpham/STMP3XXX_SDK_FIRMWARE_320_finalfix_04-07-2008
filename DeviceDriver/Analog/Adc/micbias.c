// Copyright (C) SigmaTel, Inc. 2003-2004
// Filename: micbias.c         JLN
//

#include "types.h"
#include "hwequ.h"
#include "micbias.h"

#ifdef FUNCLET
  #pragma asm
    extern SysCallFunclet
  #pragma endasm
#endif    

// Counting semaphore incremented on bias enable call, decrement on disable call.
// Prevents disabling bias when mic bias still desired by other caller. 
INT g_iMicBiasEnableCount = 0; // used by this file only.

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}

////////////////////////////////////////////////////////////////////////////////
// Function:    DisableStmpGeneratedMicBias()
// Description: Disable the STMP internally generated mic bias voltage at LRADC1/2 
//              pin so the pin can be used for A to D or temperature sensing 
//              (STMP35xx LRADC2 pin can optionally be used for temperature sensing)
//              LRADC1 has been used for player remote control.
// Inputs:      int a, int b, int c  All unused. 
// Outputs:     SUCCESS
// Notes:       STMP3410 version is to be done.
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        global FDisableStmpGeneratedMicBias
        FDisableStmpGeneratedMicBias:

        nolist
        include "sysmacro.asm"
        include "resource.inc"        
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_DISABLE_STMP_GENERATED_MICBIAS
        CallFunclet RSRC_FUNCLET_DISABLE_STMP_GENERATED_MICBIAS

        org p,"SYSFUNCLET_DISABLE_STMP_GENERATED_MIC_BIAS_P":
        dc      RSRC_FUNCLET_DISABLE_STMP_GENERATED_MICBIAS
    #pragma endasm
    
//Use the exact same prototype of the static function to convert
_reentrant int DisableStmpGeneratedMicBias_Funclet(int a, int b, int c)
#else
_reentrant int DisableStmpGeneratedMicBias(int a, int b, int c)
#endif
{

    if( (g_iMicBiasEnableCount--) <= 0) 
    {
        // After this, no internally generated mic bias voltage is present since 
        // no bias resistor is switched onto an LRADC pin.  
        HW_MIXTBR.B.MICBIAS1 = 0;
        HW_MIXTBR.B.MICBIAS_LSBITPOS = 0;
    }
    return SUCCESS;
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextmicbias":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
// Private Function: EnableStmpGeneratedMicBias()
//
// Description: Sets up the STMP to generate a mic bias voltage at the requested lradc
//              number's pin, and configures a bias circuit resistor as requested.
//
// Inputs:      int iLradcNum = {1 or 2} for STMP3500               
//              int iMicBiasConfiguredResistorKOhms = {2, 4, or 8} for STMP3500
//
// Outputs:     int SUCCESS
//
// Notes:       Debug build halts if the bias output pin or resistor value is invalid.
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        global FEnableStmpGeneratedMicBias
        FEnableStmpGeneratedMicBias:

        nolist
        include "sysmacro.asm"
        include "resource.inc"        
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_ENABLE_STMP_GENERATED_MICBIAS
        CallFunclet RSRC_FUNCLET_ENABLE_STMP_GENERATED_MICBIAS

        org p,"SYSFUNCLET_ENABLE_STMP_GENERATED_MIC_BIAS_P":
        dc      RSRC_FUNCLET_ENABLE_STMP_GENERATED_MICBIAS
    #pragma endasm
    
//Use the exact same prototype of the static function to convert
_reentrant int EnableStmpGeneratedMicBias_Funclet(int iLradcNum, int iMicBiasConfiguredResistorKOhms)
#else
_reentrant int EnableStmpGeneratedMicBias(int iLradcNum, int iMicBiasConfiguredResistorKOhms)
#endif
{
	/////// SOC INTERNALLY GENERATED MIC V BIAS FROM LRADCx pin /////////////////
    //  STMP35xx has different register names & has mic bias enabled Nov 2003.
    //  STMP3410 internally generated Vmicbias was not enabled here as of Nov 2003.
    //
	//	There are 4 different regulated supply settings available for internally 
	//  generated MIC Vbias depending on the two MICBIAS bits. 
	//  	00 - MIC Bias OFF	   10 - 4kohm
	//		01 - 2kohm			   11 - 8kohm
	//  Internally generated mic bias voltage must be enabled at LRADC1|2 pin by:
	//	Applying pwr with the AUXPWD from the LRADC_CTRL.  Must be turned on.
	///////////////////////////////////////////////////////////////////////////////

	// JN Nov13 2003. Enable 35xx. Internally generated mic Vbias wasn't used on either SOC. 
    g_iMicBiasEnableCount++; // counting semaphore
    if(g_iMicBiasEnableCount > 1) 
        return SUCCESS; // The internally generated mic bias voltage is already present.

    // Enforce valid project HW selection in \lcd|ledexample\project.inc
    DebugBuildAssert( (iLradcNum>=1) && (iLradcNum<=2) );
    // Vmicbias and SOC internal resistor selection depend on mic's internal 
    // resistance and mic's Vbias spec.
    // Setup Vmicbias SOC internal resistor choice with 2 register bits. 
    // bits==01 gives SOC internal 2 kOhms in series with mic's internal resistance of x kOhm.
    // This voltage divider gives a DC Voltage bias at the microphone.
    if( iMicBiasConfiguredResistorKOhms == 2)
    {  	HW_MIXTBR.B.MICBIAS1 = 0;
    	HW_MIXTBR.B.MICBIAS_LSBITPOS = 1;    
    }else
    if( iMicBiasConfiguredResistorKOhms == 4)
    {   HW_MIXTBR.B.MICBIAS1 = 1;
        HW_MIXTBR.B.MICBIAS_LSBITPOS = 0;    
    }else
    if( iMicBiasConfiguredResistorKOhms == 8)
    {   HW_MIXTBR.B.MICBIAS1 = 1;
        HW_MIXTBR.B.MICBIAS_LSBITPOS = 1;    
    }else DebugBuildAssert(0); //invalid mic bias resistor parm. Check value in project file & caller.

    // Selects pin LRADC2 or LRADC1 (determined by board layout) & OEM choice in \lcdexample\project.inc
    // Selection bit=0 for lradc1 pin, or selecion bit=1 for lradc2 pin as mic's Vbias source.
    HW_MIXTBR.B.MIC_BIAS_OUT_SEL = iLradcNum - 1; 
    
    // LRADC Aux Power is not required for the 35xx. 
    // Now mic bias voltage is set at requested pin.       

    // Wait for ~400 ms to allow the mic bias & related ADC input voltage to settle. 
    // Prevents audible pop at start of recording due to DC offset. Can adjust delay in project.inc. 
    // Or instead: enable earlier at voicemenu entry.
//    SysWaitOnEvent(0, 0, REC_BTN_INTERNAL_MIC_BIAS_SETTLING_DELAY_MS);//only for rec outside voicemenu

    return SUCCESS; 
}


#ifdef FUNCLET
    #pragma asm
        org p,".ptextmicbias":
    #pragma endasm
#endif


