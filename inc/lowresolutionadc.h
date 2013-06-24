//;///////////////////////////////////////////////////////////////////////////////
//; Copyright(C) SigmaTel, Inc. 2000-2003
//;
//; Filename:    lowresolutionadc.h
//; Description: General purpose interface to LRADCs
//;              Generic symbols are mapped to more specific symbols in inc\34xx\regslradc.inc files
//;///////////////////////////////////////////////////////////////////////////////

#ifndef _LOWRESOLUTIONADC_H
#define _LOWRESOLUTIONADC_H

#include "regslradc.h"

// Variables
extern WORD LowResAdcStatus;
extern WORD LowResAdcBattResult, LowResAdcBattAvg;					// Results and Averages come from buttons.asm
extern WORD LowResAdcAuxResult,  LowResAdcAuxAvg;
extern WORD LowResAdcAux2Result, LowResAdcAux2Avg;					// Aux2 result and average valid for 3500


// Types
enum LowResADCChannel {BATT, AUX, AUX2};						    // AUX2 valid for 3500
enum LowResADCHalfPwr {OFF = 0, ON = LRADC_HALF_POWER_SETMASK};
enum LowResADCRef     {REF_0 = HW_LRADC_REF_0_SETMASK,
                       REF_1 = HW_LRADC_REF_1_SETMASK,
					   REF_2 = HW_LRADC_REF_2_SETMASK,
					   REF_3 = HW_LRADC_REF_3_SETMASK,
					   REF_4 = HW_LRADC_REF_4_SETMASK,
					   REF_5 = HW_LRADC_REF_5_SETMASK,
					   REF_6 = HW_LRADC_REF_6_SETMASK,
					   REF_7 = HW_LRADC_REF_7_SETMASK
                      };


// ADC Reference resolutions in mV*10
// 1 ADC count = Vref(mV)/([2^N] - 1) where N = # bits in result, and Vref based on selected LRADC reference.
#define RES_REF_0      HW_LRADC_RES_REF_0
#define RES_REF_1      HW_LRADC_RES_REF_1
#define RES_REF_2      HW_LRADC_RES_REF_2
#define RES_REF_3      HW_LRADC_RES_REF_3
#define RES_REF_4      HW_LRADC_RES_REF_4
#define RES_REF_5      HW_LRADC_RES_REF_5
#define RES_REF_6      HW_LRADC_RES_REF_6
#define RES_REF_7      HW_LRADC_RES_REF_7


// These bits are used against LowResAdcStatus, to signal which channels to read in buttons routine
#define LOW_RES_ADC_BATT_EN_MASK    1 
#define LOW_RES_ADC_AUX_EN_MASK		2 
#define LOW_RES_ADC_AUX2_EN_MASK	4

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysLowResolutionAdcReadBin
//
//   Type:          Function 
//
//   Description:   Returns the instantaneous Result of the Low Resolution ADC Channel specified
//
//   Inputs:        Constant specifying the Low Resolution ADC Channel
//                  BATT for Battery channel
//                  AUX for Auxiliary channel 1
//                  AUX2 for Auxiliary channel 2
//
//   Outputs:       Average in binary format (0->127 for 3410, 0->255 for 3500)
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEVICE_3500
#define SysLowResolutionAdcReadBin(n)    (n==BATT ? HW_BATT_RESULT.B.DATA_OUT : (n == AUX ? HW_LRADC1_RESULT.B.DATA_OUT: HW_LRADC2_RESULT.B.DATA_OUT))
#else
//3410 does not support AUX2
#define SysLowResolutionAdcReadBin(n)    (n==BATT ? HW_LRADC_RES.B.BATLRR : (n == AUX ? HW_LRADC_RES.B.AUXLRR: 0))
#endif

/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysLowResolutionAdcReadAvg
//
//   Type:          Function 
//
//   Description:   Reads the Averaged Result of the Low Resolution ADC Channel specified
//
//   Inputs:        Constant specifying the Low Resolution ADC Channel
//                  BATT for Battery channel
//                  AUX for Auxiliary channel 1
//                  AUX2 for Auxiliary channel 2
//
//   Outputs:       Average in binary format (0->127 for 3410, 0->255 for 3500)
//
//   Notes:         The average is done using 256 samples
//<
//////////////////////////////////////////////////////////////////////////////////////////
#define SysLowResolutionAdcReadAvg(n)    (n==BATT ? LowResAdcBattAvg : (n == AUX ? LowResAdcAuxAvg : LowResAdcAux2Avg))

// Functions  
void _reentrant SysLowResolutionADCInit(enum LowResADCChannel Adc, enum LowResADCHalfPwr Power, enum LowResADCRef Ref);
INT SysLowResolutionAdcReadVolt(BYTE bAdcChannel,BOOL bAverage);

// Equates
#define AVERAGE_RESULT	TRUE
#define INSTANT_RESULT  FALSE

#endif // _LOWRESOLUTIONADC_H