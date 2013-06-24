///////////////////////////////////////////////////////////////////////////////
//  Copyright(C) SigmaTel, Inc. 2000-2001
//  File        : LowResoulutionAdcInit.c
// Description : Low Resolution ADC Init Routine
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "regslradc.h"
#include "lowresolutionadc.h"

#ifndef BATT_OFFSET
#define BATT_OFFSET 0x56
#endif

extern WORD wAdcBattResolution;
extern WORD wAdcAuxResolution;
extern WORD wAdcAux2Resolution;

#pragma optimize 1
/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysLowResolutionADCInit
//
//   Type:          Function 
//
//   Description:   Initializes specified low resolution ADC
//
//   Inputs:        ADC Channel, the 2 channels supported are BATT and AUX
//                  Half Power mode, 2 values supported are OFF and ON
//                  ADC Channel Reference (see LowResADCRef enum for the list
//                    of supported values
//
//   Outputs:       none
//
//   Notes:         Half Power Mode, reduces the ADC power consumption at the expense 
//                  of precision. In this mode the LSBit is insignificant.
//<
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
void _reentrant SysLowResolutionADCInitFunclet(enum LowResADCChannel Adc, enum LowResADCHalfPwr Power, enum LowResADCRef Ref)
#else
void _reentrant SysLowResolutionADCInit(enum LowResADCChannel Adc, enum LowResADCHalfPwr Power, enum LowResADCRef Ref)
#endif
{
    WORD    wRegister = 0, wResolution, wOffset;
    
    wRegister = wRegister + Power + Ref;          // This is the new control value 

    // Determine ADC resolution 
    switch(Ref)
        {
            case REF_2700MV:
                wResolution = RES_REF_2700MV;
                wOffset = BATT_OFFSET;
                break;
            case REF_2600MV:
                wResolution = RES_REF_2600MV;
                wOffset = BATT_OFFSET;
                break;
            case REF_2560MV:
                wResolution = RES_REF_2560MV;
                wOffset = 0;
                break;
            case REF_VDDIO:
                wResolution = RES_REF_VDDIO;
                wOffset = 0;
                break;
            case REF_5400MV:
                wResolution = RES_REF_5400MV;
                wOffset = BATT_OFFSET;
                break;
            case REF_5200MV:
                wResolution = RES_REF_5200MV;
                wOffset = BATT_OFFSET;
                break;
            case REF_5120MV:
                wResolution = RES_REF_5120MV;
                wOffset = 0;
                break;
            case REF_VDDIOx2:
                wResolution = RES_REF_VDDIOx2;
                wOffset = 0;
                break;
                
            default:
                wResolution = 0;
                wOffset = 0;
                break;                
        }
        
    // Initialize ADC parameters
    switch(Adc)
    {
        case BATT:
            wAdcBattResolution = wResolution;                               // Resolution
            LowResAdcStatus = LowResAdcStatus | LOW_RES_ADC_BATT_EN_MASK;   // Enable for button ISR
            HW_BATT_CTRL.I = HW_BATT_CTRL.I & 0xF0307F;
            HW_BATT_CTRL.I = HW_BATT_CTRL.I | wRegister;                    // Setup control register
            HW_BATT_CTRL.B.INPUT_OFFSET = wOffset;
            break;

        case AUX:
            wAdcAuxResolution = wResolution;                                // Resolution
            LowResAdcStatus = LowResAdcStatus | LOW_RES_ADC_AUX_EN_MASK;    // Enable for button ISR
            HW_LRADC1_CTRL.I = HW_LRADC1_CTRL.I & 0xF0307F;
            HW_LRADC1_CTRL.I = HW_LRADC1_CTRL.I | wRegister;                // Setup control register
            HW_LRADC1_CTRL.B.INPUT_OFFSET = wOffset;
            break;

        case AUX2:
            wAdcAux2Resolution = wResolution;                               // Resolution
            LowResAdcStatus = LowResAdcStatus | LOW_RES_ADC_AUX2_EN_MASK;   // Enable for button ISR
            HW_LRADC2_CTRL.I = HW_LRADC2_CTRL.I & 0xF0307F;
            HW_LRADC2_CTRL.I = HW_LRADC2_CTRL.I | wRegister;                // Setup control register
            HW_LRADC2_CTRL.B.INPUT_OFFSET = wOffset;
            break;
               
        default:
            break;
    }                                

}

#ifdef FUNCLET
#pragma asm
  extern    SysCallFunclet

  global FSysLowResolutionADCInit
  org p,".ptextlowresolutionadcinitprologue":
FSysLowResolutionADCInit
  nolist
  include "sysmacro.asm"
  include "resource.inc"
  list
  CallFunclet RSRC_FUNCLET_LOWRESOLUTIONADCINIT
#pragma endasm
#endif
