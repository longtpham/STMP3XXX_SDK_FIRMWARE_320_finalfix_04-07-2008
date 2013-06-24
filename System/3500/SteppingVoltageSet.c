////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004
//
// Filename:      SteppingVoltageSet.c
// Description :  Functions to step the voltage up and down. Use only SteppingVoltageSet().
// History & Notes: Supports 35xx VDDD & VDDIO voltage stepping (VDDIO mode not attempted yet). 
//                These functions make convergent V steps for either voltage type
//                in order to have reduced undershoot in the step down case and 
//                reduced overshoot in the step up case. 
////////////////////////////////////////////////////////////////////////////////

#include "SteppingVoltageSet.h" // has input parameter macros, error codes, public prototypes. 
#include "regsrevision.h"
#include "regsdcdc.h"
#include "misc.h"
#include "exec.h"
#include "regsgpio.h"
#include "lowresolutionadc.h"
// Use absolute path so i2ctest.bat will work
#include "..\..\Projects\SDK\lcdexample\battery_config.h"

#define POST_OK_SETTLING_DELAY_STEPDOWN_VALUE 3200
#define POST_OK_SETTLING_DELAY_STEPUP_VALUE 1000
#define VDD_OK_SETUP_CYCLES_AT_70MHZ 160
#define DCDCTBR_SETTLING_DELAY_VALUE 3200

//void SysDelay(int iMs);

//int VStepHalt=0; //  only uncomment during debug

    // Single public entry point function:
//------------------------------------------------------------------------------
// Public Function Name: SteppingVoltageSet
// Description: Performs intermediate V step(s) if needed. Steps iStepSize. 
//              For VDDD, polls VDD_OK register bit after each intermediate step if used. 
//              This possible polling adds inline delay that may be on the order 
//              of 1 ms per intermediate step. 
// Inputs:  WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD sStepSize, /* 3 parms are V bit codes */
//          int iVoltageType      /* VOLTAGE_TYPE_VDDD or VOLTAGE_TYPE_VDDIO (VDDIO untested)   */
// 
// Returns: RETCODE: SUCCESS if no change needed, ERR_INVALID_VOLTAGE_CODE, 
//          or passes back the RETCODE from VoltageStepUp or VoltageStepDown. 
// Notes:   When called from asm, the registers with these params are: a,b,x0,y0
//------------------------------------------------------------------------------
//int g_iErr=0;
#ifdef FUNCLET
RETCODE SteppingVoltageSetFunclet(WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize, 
                                       int iVoltageType) 
#else
RETCODE SteppingVoltageSet(WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize, 
                                       int iVoltageType) 
#endif
{
    INT iAvgBatteryVoltageMV;
    WORD wNewDesiredVoltageCode;
  
    if( wDesiredVoltageCode == wCurrentVoltageCode ) 
        return SUCCESS; 

    #ifdef DEVICE_3500 // DEVICE_3600 has a 5 bit field also. 
    if( (wDesiredVoltageCode > 31) || (wCurrentVoltageCode > 31) ) // STMP35xx has 5 lsb V codes. 
        return ERR_INVALID_VOLTAGE_CODE; // Max v code is 31 == 0x1F

    //8 is min register settable voltage code of 1.28V, below which we disallow here.
    if( (wDesiredVoltageCode < 8) ) 
    {   //g_iErr = iDesiredVoltageCode;
        return ERR_INVALID_VOLTAGE_CODE_LOW;
    }    
    #endif

    if( (iVoltageType != VOLTAGE_TYPE_VDDD) && (iVoltageType != VOLTAGE_TYPE_VDDIO) )//can comment out for codespace
        return ERR_INVALID_VOLTAGE_TYPE;

#ifdef BATTERY_TYPE_ALKALINE
// Special processing for Oxyride battery
    iAvgBatteryVoltageMV = SysLowResolutionAdcReadVolt(BATT, 0); // Read battery voltage (Don't use avg!!!)

    if (OXYRIDE_THRESHOLD_MV < iAvgBatteryVoltageMV)   // Oxyride if battery voltage above threshold (max alkaline + delta)  
    { 
         // Convert between Millivolts (MV) and Voltage Code (VC) : MV = (190/6) * VC + 1028 ;  VC = ...
        wNewDesiredVoltageCode = (((iAvgBatteryVoltageMV - OXYRIDE_SETBACK_MV - 1028) * 6) / 190);
        if (wDesiredVoltageCode < wNewDesiredVoltageCode)  // Use higher of Desired and Oxyride for Vddd
        {
            wDesiredVoltageCode = wNewDesiredVoltageCode;
        }
     }
#endif

    if( wDesiredVoltageCode > wCurrentVoltageCode )
    {      
        return( VoltageStepUp(wCurrentVoltageCode, wDesiredVoltageCode, wStepSize, 
                              iVoltageType) );
    }
    else           // desired < current V
    {
        //SystemHalt();   
        return( VoltageStepDown(wCurrentVoltageCode, wDesiredVoltageCode, wStepSize, 
                                iVoltageType) );
    }
}

// FUNCTIONS BELOW THIS LINE ARE PRIVATE AND BEHIND THE INTERFACE ABOVE.

//Amount of time recommended by HW
//engineer for waiting for HW to
//settle is 1ms per step per tick.
//The exact value is 3ms, and
//due to granularity of System Timer,
//we need to wait one more millisecond
//than 3ms to make sure that in worst
//case we wait at least 3ms.
#define SETTLING_WAIT_TIME_MS 4

void WaitForVDD_OKOrTimeOut(void)
{
    LONG iFinishTime;

    iFinishTime = (LONG)SETTLING_WAIT_TIME_MS + SysGetCurrentTime();

    while( HW_DCDC_VDDD.B.VDD_OK != 1 )    
    { // nop();
        if ( SysGetCurrentTime() >= iFinishTime )
        {    break;
        }
    }
} /* WaitForVDD_OKOrTimeOut */

//------------------------------------------------------------------------------
// Private Function Name: VoltageStepDown
// Description: Performs Voltage intermediate step(s) down if needed. Steps iStepSize. 
//              Polls VDD_OK register bit after each intermediate step if used. 
//              This possible polling adds inline delay that may be on the order 
//              of 1 ms per intermediate step but varies with system HW state. 
// Inputs:  WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize, /* these are V bit codes */
//          int iVoltageType     /* VOLTAGE_TYPE_VDDD or VOLTAGE_TYPE_VDDIO (VDDIO untested) */
//
// Returns: RETCODE: SUCCESS 
//                    
// Notes:   
//------------------------------------------------------------------------------
//int VStepHalt=0; //debug int only

RETCODE VoltageStepDown(WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize, 
                             int iVoltageType)
{
    WORD wRemainder; 
    //int retCode=SUCCESS;
    int iDelayCounter;
    WORD wSavedDcDcControlReg = HW_DCDC1_CTRL1.I;
    LONG iFinishTime;

    // All params except iStepSize are already qualified in calling function. 
    
    // if(VStepHalt)  { _asm( " debug"); _asm( " nop"); _asm( " nop"); _asm( " nop");}

    #if 0 // input verification removed for codespace    
    if( wDesiredVoltageCode > wCurrentVoltageCode )
    {   return ERR_STEPDOWN_NOT_NEEDED; //Notifies caller: this function is only for stepping down, not up. 
    }
    #endif

    //Changing the RC value of the DCDC1_CTRL1 register will stabalize the transients on VDDIO and VDDD when
	//the voltage is changed.  However, it will also close the DCDC control loop for the 5V to DCDC Transfer.
	//This may crash the player when disconnecting from 5V.  Only change the RC values when 5V is not connected.
    if(!(HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT))
	{
    	HW_DCDC1_CTRL1.I = 0x000098;  // with 0x94 had best results yet. Resistance is nibble1. Capacitor is nibble0.
	}

    while( 1 ) // break near end exits the V stepping loop.
    {
        if(iVoltageType == VOLTAGE_TYPE_VDDD)
            wCurrentVoltageCode = (WORD)HW_DCDC_VDDD.B.VOLTAGE_LEVEL; // a 5 bit bitfield of the register
        else // (iVoltageType == VOLTAGE_TYPE_VDDIO)
            wCurrentVoltageCode = (WORD)HW_DCDC_VDDIO.B.VOLTAGE_LEVEL; // a 5 bit bitfield of the register

        wRemainder = wCurrentVoltageCode - wDesiredVoltageCode; // V code bits remaining of total decrease

        if( wRemainder > wStepSize )
        {   
            // step down V by stepSize. Write the voltage control register.
            if(iVoltageType == VOLTAGE_TYPE_VDDD)
            {   
                HW_DCDC_VDDD.B.VOLTAGE_LEVEL -= wStepSize;  // set the new voltage
                SetDCDCTestBitRegister();
                // _asm( " bset #HW_GP_B17_BITPOS,x:HW_GP0DOR ");    //make pin 5 high for scope profiling VDD_OK.
            }
            else if (iVoltageType == VOLTAGE_TYPE_VDDIO)
            {   
                HW_DCDC_VDDIO.B.VOLTAGE_LEVEL -= wStepSize;
            }
            // above are all cases since calling func qualified the inputs

            // poll for voltage convergence
            if(iVoltageType == VOLTAGE_TYPE_VDDD)
            {                
                //while( HW_DCDC_VDDD.B.VDD_OK != 0 ) _nop(); // must be 0 or fall to 0. Designer required. 
                //When TBR func was separate funclet, DSP slower than OK bit setup time so delay count for worst case. 
                //          Designer specifies 3T at 1.5Mhz so 72cycles@36MHz; 144cycles@72MHz. For safety use 160.
                //          as seen on scope.  
                iDelayCounter = VDD_OK_SETUP_CYCLES_AT_70MHZ;
                while( iDelayCounter-- > 0 ) _nop();
                
                WaitForVDD_OKOrTimeOut();

                // _asm( " bclr #HW_GP_B17_BITPOS,x:HW_GP0DOR "); //make pin 5 low for scope profiling VDD_OK.

                // SysDelay(1); // spin for 1 ms to let the VDDD and VDDIO voltages settle. Not a reliable ms. 
                iDelayCounter = POST_OK_SETTLING_DELAY_STEPDOWN_VALUE; 
                // 3200 best yet. 1800 didn't prevent vddd brownout w/ stepsize 3. 
                // 1100 w/ stepsize 1 resulted in: undershoot issues so avoid. 
                while(iDelayCounter-- > 0) _nop();
            }
            else if(iVoltageType == VOLTAGE_TYPE_VDDIO)
            {   
                //while( HW_DCDC_VDDIO.B.VDDIO_OK != 0 ) _nop(); // must be 0 or fall to 0. Designer requires. 
                //When TBR func was separate funclet, DSP slower than OK bit setup time so delay count for worst case. 
                iDelayCounter = VDD_OK_SETUP_CYCLES_AT_70MHZ;
                while( iDelayCounter-- > 0 ) _nop();

                iFinishTime = (LONG)SETTLING_WAIT_TIME_MS + SysGetCurrentTime();

                while( HW_DCDC_VDDIO.B.VDDIO_OK != 1 )    
                {   _nop(); // poll for V converged state. 
                    if ( SysGetCurrentTime() >= iFinishTime )
                    {    break;
                    }
                }
                
            }    
        }
        else // No full steps left. Just set the target voltage & wait for it to converge. 
        {   if(wRemainder>0)
            {
                if(iVoltageType == VOLTAGE_TYPE_VDDD) 
                {   HW_DCDC_VDDD.B.VOLTAGE_LEVEL = wDesiredVoltageCode;  // set target voltage 
                    SetDCDCTestBitRegister();
                }
                else if(iVoltageType == VOLTAGE_TYPE_VDDIO)    
                {   
                    HW_DCDC_VDDIO.B.VOLTAGE_LEVEL = wDesiredVoltageCode;  // set target voltage
                }

                #if 1 // delay may not be needed here since we're not changing VDDD again til next call. 
                // Poll for voltage convergence. Since this is last change, 
                //  we probably don't need to wait here. fDCLK is set previously. 
                if(iVoltageType == VOLTAGE_TYPE_VDDD)
                {
                    //while( HW_DCDC_VDDD.B.VDD_OK != 0 ) _nop(); // must be 0 or fall to 0. Designer required. 
                    //When TBR func was separate funclet, DSP slower than OK bit setup time so delay count for worst case. 
                    iDelayCounter = VDD_OK_SETUP_CYCLES_AT_70MHZ;
                    while( iDelayCounter-- > 0 ) _nop();

                    WaitForVDD_OKOrTimeOut();

                }               
                else if(iVoltageType == VOLTAGE_TYPE_VDDIO)
                {   
                    //while( HW_DCDC_VDDIO.B.VDDIO_OK != 0 ) _nop(); // must be 0 or fall to 0. Designer required. 
                    //When TBR func was separate funclet, DSP slower than OK bit setup time so delay count for worst case. 
                    iDelayCounter = VDD_OK_SETUP_CYCLES_AT_70MHZ;
                    while( iDelayCounter-- > 0 ) _nop();

                    iFinishTime = (LONG)SETTLING_WAIT_TIME_MS + SysGetCurrentTime();

                    while( HW_DCDC_VDDIO.B.VDDIO_OK != 1 )    
                    {   _nop(); // poll for V converged state. 
                        if ( SysGetCurrentTime() >= iFinishTime )
                        {    break;
                        }
                    }

                }    
                #endif                
            }            
            break; // done. The desired voltage is set and converged upon. LOOP's ONLY EXIT POINT
        }// endif full step remaining?
   
    }// loop

    HW_DCDC1_CTRL1.I = wSavedDcDcControlReg;

    // continue normal system operation.
    return SUCCESS;
}

//------------------------------------------------------------------------------
// Private Function Name:    VoltageStepUp  
// Description: Raising the voltage in steps is requested also but is less critical.   
//              Initial version just sets voltage per type as in sysspeed without stepping. 
// Inputs:  WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize, /* 3 parms are V bit codes */
//          int iVoltageType      /* VOLTAGE_TYPE_VDDD or VOLTAGE_TYPE_VDDIO (VDDIO untested) */
// Returns: SUCCESS
// Notes:   
//------------------------------------------------------------------------------
RETCODE VoltageStepUp(WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize,
                      int iVoltageType)  
{
    WORD wRemainder;
	WORD iDelayCounter;

    #if 0 // input verification removed for codespace    
    if( wDesiredVoltageCode < wCurrentVoltageCode )
    {   return ERR_STEPUP_NOT_NEEDED; //Notifies caller: this function is only for stepping up, not down. 
    }
    #endif

    // All params except iStepSize are already qualified by caller.
	// Step-up core voltage
    if(iVoltageType == VOLTAGE_TYPE_VDDD)
    {
       while(1)
       {
       	   wCurrentVoltageCode = (WORD)HW_DCDC_VDDD.B.VOLTAGE_LEVEL; // a 5 bit bitfield of the register

       	   wRemainder = wDesiredVoltageCode - wCurrentVoltageCode; // V code bits remaining of total increase

           if( wRemainder > wStepSize )
           {    HW_DCDC_VDDD.B.VOLTAGE_LEVEL += wStepSize;  // set the new voltage
                
                //while( HW_DCDC_VDDD.B.VDD_OK != 0 ) _nop(); // must be 0 or fall to 0. Designer required. 
                //When TBR func was separate funclet, DSP slower than OK bit setup time so delay count for worst case. 
                //          Designer specifies 3T at 1.5Mhz so 72cycles@36MHz; 144cycles@72MHz. For safety use 160.
                //          as seen on scope.  
                iDelayCounter = VDD_OK_SETUP_CYCLES_AT_70MHZ;
                while( iDelayCounter-- > 0 ) _nop();
                WaitForVDD_OKOrTimeOut();

                // SysDelay(1); // spin for 1 ms to let the VDDD and VDDIO voltages settle. Not a reliable ms. 
                iDelayCounter = POST_OK_SETTLING_DELAY_STEPUP_VALUE;  
                while(iDelayCounter-- > 0) _nop();
                 
			    SetDCDCTestBitRegister(); 	
		   }
		   else
		   {
           		if(wRemainder > 0) 
           		{	HW_DCDC_VDDD.B.VOLTAGE_LEVEL = wDesiredVoltageCode;  // set final voltage  
           			SetDCDCTestBitRegister();
				}
				//Voltage Step-Up has completed.  Time to leave.
				break;
		   }
		}
    }
    else if(iVoltageType == VOLTAGE_TYPE_VDDIO)
    {   
        HW_DCDC_VDDIO.B.VOLTAGE_LEVEL = wDesiredVoltageCode;  // set final voltage  
    }
    //iVoltageType is always one of above cases since input is prequalified    

    return SUCCESS;
}


//------------------------------------------------------------------------------
// Private Function: SetDCDCTestBitRegister() 
// Inputs: No parameters. reads HW_REVR, HW_DCDC_VDDD
// Outputs: void return. Writes HW_DCDDC_TBR
// Notes:   This version is originally written for STMP35xx registers. 
//          Moved here from SysSetSpeed per SOC designer requirement to set DCDCTBR
//          at time of VDDD change. Returns immediately if in a buck mode. 
//          The first full C version of this function sets the DCDCTBR to one of
//          4 values depending on VddD voltage value. 
//------------------------------------------------------------------------------
void SetDCDCTestBitRegister(void) 
{
    WORD wDCDCMode = HW_REVR.B.DCDCMODE; // low 3 bits are the mode. 7 total codes
    int iDelayCounter;
    WORD wVdddVoltageLevelCode = (WORD)(HW_DCDC_VDDD.B.VOLTAGE_LEVEL);
    //WORD wVdddVoltageLevelCode = (WORD)HW_DCDC_VDDD.I;  
    //wVdddVoltageLevelCode &= 0x00001f; 

    // if pins & therefore read-only-reg specify DCDC converter #1 buck mode, exit.

    //if( (HW_REVR.B.DCDCMODE != DCDCMODE_7_CONVERTER1_2CH_BOOST_CONVERTER2_OFF) && 
    //    (HW_REVR.B.DCDCMODE != DCDCMODE_5_CONVERTER1_3CH_BOOST_CONVERTER2_OFF) && 
    //    (HW_REVR.B.DCDCMODE != DCDCMODE_6_RESERVED) &&
    //    (HW_REVR.B.DCDCMODE != DCDCMODE_4_RESERVED) ) 
    //    {   DebugBuildAssert(0);
    //        return;
    //    }  
    //The clear logic above simplifies to this:  
    if( (wDCDCMode & DCDCMODE_X_BUCK_MASKED_SETMASK) != DCDCMODE_X_BUCK_MASKED_SETMASK ) 
    {   //DebugBuildAssert(0);
        return; // If buck not masked, exit. bit high means buck mode not present for DCDC converter 1
    }  

    #if 0
    // Alternate working (verified) technique to ensure a non buck DCDC converter mode. 
    // With just 3 instructions, saves the most memory but less portable since assembly.
#pragma asm
    include "hwequ.inc"
    btst    #>HW_REVR_DCDCMODE_BUCK_MASK_POS,x:HW_REVR
                                ; What mode is dcdc1 in? Is buck masked bit set?
    jcs     _continueSegment               
    rts                         ; Dont need to change the DCDC TBR
_continueSegment
#pragma endasm
    #endif

    // comment this line when testpoint unneeded
    //if(VStepHalt) {DebugBuildAssert(0); _asm( " nop"); _asm( " nop"); _asm( " nop");} 

    HW_DCDC_TBR.B.DCDC_ANA_BGR_BIAS = 1; // for all cases. Required by IC designer.
	 
    // Set the DCDC test bit reg fields for duty cycle & bat adjust 
    if(     (wVdddVoltageLevelCode) >= 0x12) // 0x12 is 1.60V. Range [1.60, 2.02]
    {  
        HW_DCDC_TBR.B.DCDC1_ADJ_TN = 0;
        ClearHwDcdcTbrBatteryADJ();     
    }
    else if((wVdddVoltageLevelCode) >= 0x0F) // 0x0F is 1.50V. Range [1.50, 1.57V]
    { //Scope optimized step down sequence achieved with ver that just sets 2 bits and sets R&C values. 
        //if(!(WORD)(HW_DCDC_TBR.B.DCDC1_BAT_ADJ))
        //{
            //#define DCDC_BATT_ADJ_COUNTDOWN_VALUE 3200
            //iDelayCounter = DCDC_BATT_ADJ_COUNTDOWN_VALUE; 
            // 3200 here showed on scope that this bitset is not cause of vddio drop, 
            // since vddio drop occurs before this, near time when vddd is reduced.
            //while(iDelayCounter-- > 0) _nop();
        HW_DCDC_TBR.B.DCDC1_BAT_ADJ = 1;
            //iDelayCounter = POST_DCDC_BATT_ADJ_COUNTDOWN_VALUE; // 3200 initialliy tried.
            //while(iDelayCounter-- > 0) _nop();      
        //}        
        HW_DCDC_TBR.B.DCDC1_ADJ_TN = 0;
        // SystemHalt();
    }
    else if((wVdddVoltageLevelCode) >= 0x0C) // 0x0C is 1.40V. Range [1.40, 1.47]
    {
        HW_DCDC_TBR.B.DCDC1_ADJ_TN = 1;  
        HW_DCDC_TBR.B.DCDC1_BAT_ADJ = 1;     
    }
    else           // VOLTAGE_LEVEL <= 0x0B    V range [1.34, 1.37].         
    {
        HW_DCDC_TBR.B.DCDC1_ADJ_TN = 2;
        HW_DCDC_TBR.B.DCDC1_BAT_ADJ = 1;     
    }

}// end function

//------------------------------------------------------------------------------
// Private Function: ClearHwDcdcTbrBatteryADJ() 
// Inputs: No parameters. Clear	HW_DCDC_TBR[DCDC1_BAT_ADJ] Register
// Outputs: None
// Notes:   This version is originally written for STMP35xx registers. 
//          Clearing the DCDC1_BAT_ADJ bit may cause false VDDIO Brownouts.
//			This function will disable VDDIO brownout before clearing the bit.
//------------------------------------------------------------------------------
void ClearHwDcdcTbrBatteryADJ(void)
{
	WORD iDelayCounter;

	//check to see if bit is set.  No need to delay if the bit is already cleared. 
	if(HW_DCDC_TBR.B.DCDC1_BAT_ADJ)
	{ 
		//Before Changing TBR Register, clear VDDIO Brownout.  Clearing the DCDC1_BAT_ADJ may cause 
		//false Brownouts.
		HW_DCDC_VDDIO.B.BROWNOUT_ENABLE	= 0;

		HW_DCDC_TBR.B.DCDC1_BAT_ADJ = 0;

    	// SysDelay(1); // spin for 1 ms to let the VDDD and VDDIO voltages settle. Not a reliable ms. 
    	iDelayCounter = DCDCTBR_SETTLING_DELAY_VALUE;  
    	while(iDelayCounter-- > 0) _nop();

		// Now it should be safe to re-enable VDDIO Brownout
		HW_DCDC_VDDIO.B.BROWNOUT_ENABLE	= 1;
	}
} 



#ifdef FUNCLET
#pragma asm
  extern    SysCallFunclet
  global FSteppingVoltageSet
  org p,".ptextsteppingvoltagesetprologue":
FSteppingVoltageSet
  nolist
  include "sysmacro.asm"
  include "resource.inc"
  list
  CallFunclet2 RSRC_FUNCLET_STEPPINGVOLTAGESET
#pragma endasm


#pragma asm
	org p,".ptextskiponewordforsteppingvoltage":
	dc RSRC_FUNCLET_STEPPINGVOLTAGESET    
#pragma endasm
#endif
