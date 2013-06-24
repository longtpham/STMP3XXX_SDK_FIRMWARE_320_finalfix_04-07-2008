/*;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : usbbattery.c
;  Description : LCD based reference design
;///////////////////////////////////////////////////////////////////////////////
;  3500-specific battery functions
;///////////////////////////////////////////////////////////////////////////////
*/

// This is the 3500 battery functionality for the 3050 USBMSC projects, 
// until LRADC reads are taken out of buttons.asm in the 2450 branch, and the two are merged.
// 3050 projects don't use buttons.asm, so reads are done directly in this code.

#include "types.h"
//#include "hwequ.h"	// obsolete
#include "regsicoll.h"
#include "regslradc.h"
#include "project.h"
#include "lowresolutionadc.h"
#include "battery_config.h"
#include "battery.h"
#include "icoll.h"


#if     ((VBATT_REF)==(REF_2700MV))
#define VBATT_REF_MV 2700
#define BATT_OFFSET 0x51
#elif   ((VBATT_REF)==(REF_2600MV))
#define VBATT_REF_MV 2600
#define BATT_OFFSET 0x51
#elif   ((VBATT_REF)==(REF_2560MV))
#define VBATT_REF_MV 2560
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_VDDIO))
#define VBATT_REF_MV 3300
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_5400MV))
#define VBATT_REF_MV 5400
#define BATT_OFFSET 0x51
#elif   ((VBATT_REF)==(REF_5200MV))
#define VBATT_REF_MV 5200
#define BATT_OFFSET 0x51
#elif   ((VBATT_REF)==(REF_5120MV))
#define VBATT_REF_MV 5120
#define BATT_OFFSET 0
#elif   ((VBATT_REF)==(REF_VDDIOx2))
#define VBATT_REF_MV 6600
#define BATT_OFFSET 0
#else
#pragma asm
    warn 'No battery reference chosen'
#pragma endasm 
#endif

// ADC result is [(RangeMax - RangeMin)/256] + programmed offset
// Conversion result is 8-bit, but DATA_OUT is 9-bits after adding pre-programmed offset.
// Offset accounts for RangeMin > Ground
#define VBATT_N_LEVELS   (((VBATT_MAX_MV - VBATT_MIN_MV)*(255 + BATT_OFFSET))/VBATT_REF_MV) //# counts in range
#define VBATT_BASE_LEVEL ((VBATT_MIN_MV*(255 + BATT_OFFSET))/VBATT_REF_MV)		            //# counts at 0%
#define VBATT_MAX_LEVEL  ((VBATT_MAX_MV*(255 + BATT_OFFSET))/VBATT_REF_MV)		     		// min / (mv/bit) = # cnts at 0%

//Low battery detection bit
#define LOW_BATTERY_DETECTED_BITPOS	  0
#define LOW_BATTERY_DETECTED 1<<LOW_BATTERY_DETECTED_BITPOS  //set low battery bit


_asmfunc void SysBrownOutIsr(void);

// Variables - not used, but allows us to include the lowresolutionADC initialization routines.
WORD LowResAdcStatus;
WORD LowResAdcBattResult, LowResAdcBattAvg;
WORD LowResAdcAuxResult, LowResAdcAuxAvg;

WORD g_bLowBattery;
WORD g_AvgBattLevel = VBATT_MAX_LEVEL;  // Max out to begin with then drift down.
WORD g_BattLevelSum = 0;                // Running sum.

WORD battBrownoutCount;


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant int  SysBatteryGetLevel (void)
//
//   Type:          Function 
//
//   Description:   Grabs the current battery level for USB display
//
//   Inputs:        none
//
//  Notes:          
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant WORD SysBatteryGetLevel(void)
{
	static WORD wAvgCount;
	WORD battlevel;		

	//grab LRADC resolution and calculate the battery %
 	battlevel = HW_BATT_RESULT.B.DATA_OUT;

    //For use by SysLowResolutionAdcReadVolt()
    LowResAdcBattResult = battlevel;




	/*
	// Exponential Average -> 87.5/12.5
    g_AvgBattLevel = ((14*g_AvgBattLevel) + (2*battlevel)) >> 4;
	*/

	// Mimic the averaging done in buttons.asm for the player.
	g_BattLevelSum += battlevel;
	if (++wAvgCount >= 16)
	{
		g_AvgBattLevel = g_BattLevelSum >> 4;
		wAvgCount = 0;
		g_BattLevelSum = 0;

        //For use by SysLowResolutionAdcReadVolt()
        LowResAdcBattAvg = g_AvgBattLevel;


	}

	// Convert to levels (10% increments)
	if(g_AvgBattLevel >= VBATT_BASE_LEVEL)
 		battlevel = ((g_AvgBattLevel - VBATT_BASE_LEVEL)*100)/(VBATT_N_LEVELS);
	else
		battlevel = 0;

	//if average is less than safety margin, set low battery detected
	if(battlevel < BATT_SAFETY_MARGIN)
		g_bLowBattery = LOW_BATTERY_DETECTED;
	else
		g_bLowBattery &= (WORD)(~LOW_BATTERY_DETECTED);
       
    // no rounding is currently supported
    // don't use resource outside of 10 available bitmaps
    if (battlevel < 10)
       battlevel = 0;
    else if (battlevel > 100)
       battlevel = 10;
    else 
       battlevel /= 10;               

    return battlevel;   //return level 
}
