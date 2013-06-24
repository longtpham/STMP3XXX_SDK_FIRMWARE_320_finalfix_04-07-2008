///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2003
// File        : Battery.c 
// Description : LRADC Battery API
///////////////////////////////////////////////////////////////////////////////
// Customized for the 3500 device.
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "hwequ.h"
#include "battery.h"
#include "project.h"
#include "battery_config.h"
#include "lowresolutionadc.h"
            
#if     ((VBATT_REF)==(REF_2700MV))
#define VBATT_REF_MV 2700
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_2600MV))
#define VBATT_REF_MV 2600
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_2560MV))
#define VBATT_REF_MV 2560
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_VDDIO))
#define VBATT_REF_MV 3300
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_5400MV))
#define VBATT_REF_MV 5400
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_5200MV))
#define VBATT_REF_MV 5200
#define BATT_OFFSET 0x56
#elif   ((VBATT_REF)==(REF_5120MV))
#define VBATT_REF_MV 5120
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_VDDIOx2))
#define VBATT_REF_MV 6600
#define BATT_OFFSET 0
#else
    //Need to define VBATT_REF in battery_config.h, or it won't work.
#endif

// ADC result is [(RangeMax - RangeMin)/256] + programmed offset
// Conversion result is 8-bit, but DATA_OUT is 9-bits after adding pre-programmed offset.
// Offset accounts for RangeMin > Ground
#define VBATT_N_LEVELS   (((VBATT_MAX_MV - VBATT_MIN_MV)*(255 + BATT_OFFSET))/VBATT_REF_MV) //# counts in range
#define VBATT_BASE_LEVEL ((VBATT_MIN_MV*(255 + BATT_OFFSET))/VBATT_REF_MV)		            //# counts at 0%

WORD battBrownoutCount;
extern WORD wAdcBattResolution;

void BatteryDummy (void)
{
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysLRADCBrownoutInit
//
//   Type:          Function 
//
//   Description:   Initializes specified low resolution ADC brownout trip point and enables the battery 
//                  brownout circuit.
//
//   Inputs:        Brownout trip point in mV
//
//   Outputs:       none
//
//   Notes:         
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".ptextbattery":
        global FSysLRADCBrownoutInit
        FSysLRADCBrownoutInit:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_SYSLRADCBROWNOUTINIT
                            
        org p,"RSRC_FUNCLET_SYSLRADCBROWNOUTINIT_P":
        dc      RSRC_FUNCLET_SYSLRADCBROWNOUTINIT
    #pragma endasm

void _reentrant SysLRADCBrownoutInit_funclet(WORD wLowResADCBrownoutLevel)
#else
void _reentrant SysLRADCBrownoutInit(WORD wLowResADCBrownoutLevel)
#endif
{

	// The LowResADCBrownoutLevel represents a mV value below which to trigger an event.
	// threshValue represents the appropriate LRADC conversion value that represents the brownout level
//	battBrownoutCount = (wLowResADCBrownoutLevel*(255 + BATT_OFFSET));
//	battBrownoutCount = (battBrownoutCount/VBATT_REF_MV);

	battBrownoutCount = (wLowResADCBrownoutLevel * 10) / wAdcBattResolution;
	
	// For ranges not referenced to zero
	HW_BATT_CTRL.B.INPUT_OFFSET = BATT_OFFSET;

	// Set the trip point
	HW_BATT_THRSH.B.THRESHOLD_EVENT0 = battBrownoutCount;

	// Trigger exception when lower than threshold
	HW_BATT_CTRL.B.POLARITY_EVENT0 = 0;

	// Clear the flag
	HW_BATT_RESULT.B.IRQ_EVENT0 = 1;
	
	// Enable the interrupt
	HW_BATT_CTRL.B.IRQ_EN_EVENT0 = 1;


}
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextbattery":
    #pragma endasm
#endif
   
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysBatteryGetLevel
//
//   Type:          Function 
//
//   Description:   Returns the percentage of battery remaining. 
//                  If battery is at max value, a 100 is returned.
//                  If battery is at min defined value, a 0 is defined.
//                  Below VBATT_MIN, the return value will be negative.
//
//   Inputs:        None
//
//   Outputs:       Battery Level Percentage is returned.
//
//   Notes:         VBATT_BASE_LEVEL = Minimum valid battery level / resolution 
//                                     represents ADC count value at minimum voltage level
//                  VBATT_N_LEVELS = (max batt level - min batt level) / resolution  
//                                     represents # of ADC counts in the supported battery range
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,".ptextbattery":
        global FSysBatteryGetLevel
        FSysBatteryGetLevel:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_SYSBATTERYGETLEVEL
                            
        org p,"RSRC_FUNCLET_SYSBATTERYGETLEVEL_P":
        dc      RSRC_FUNCLET_SYSBATTERYGETLEVEL
    #pragma endasm

_reentrant WORD SysBatteryGetLevel_funclet(void)
#else
_reentrant WORD SysBatteryGetLevel(void)
#endif
{
    INT iVBattLevel;
    INT iBatteryCurrentAvg;

	// Get the current averages value
    iBatteryCurrentAvg = SysLowResolutionAdcReadAvg(BATT);

    // At startup, the averaged value may not be available yet. If not, get instantaneous value.
    if (iBatteryCurrentAvg == 0)
      iBatteryCurrentAvg = SysLowResolutionAdcReadBin(BATT);

    // Calculate the battery level as a percentage of total battery power.
    iVBattLevel = ((iBatteryCurrentAvg - VBATT_BASE_LEVEL)*100)/(VBATT_N_LEVELS);
    
	return(iVBattLevel);
}                            
#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
            org p,".ptextbattery":
    #pragma endasm
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MvToLradcResult
//
//   Type:          1 line public API Function 
//
//   Description:   Returns the LRADC result for a given millivolt number. 
//
//   Inputs:        WORD mVolts
//
//   Outputs:       WORD LRADC read result expected for a given mV value.
//
//   Notes:         Use this result to compare to LRADC battery read results.
//<
//////////////////////////////////////////////////////////////////////////////////////////

_reentrant WORD MvToLradcResult(WORD mVolts)
{
    return(((mVolts)*((WORD)255 + (WORD)BATT_OFFSET))/(WORD)VBATT_REF_MV);
}





