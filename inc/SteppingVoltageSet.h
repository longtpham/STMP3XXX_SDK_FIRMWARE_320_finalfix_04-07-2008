////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004
//
// Filename:      SteppingVoltageSet.h
// Description :  Functions to step the voltage up and down. 
// History & Notes: Supports STMP35xx VDDD voltage stepping initially. 
//                Others such as VDDIO stepping can be added to this API which
//                would make convergent V steps for voltage type specified
//                in order to have reduced undershoot in the step down case and 
//                reduced overshoot in the step up case. 
////////////////////////////////////////////////////////////////////////////////

#include "types.h"

// Error codes
#define ERR_STEPDOWN_NOT_NEEDED         1
#define ERR_STEPUP_NOT_NEEDED           2

#define ERR_INVALID_VOLTAGE_CODE        3
#define ERR_INVALID_VOLTAGE_CODE_LOW    4
#define ERR_INVALID_POLARITY_PARAM      5
#define ERR_INVALID_VOLTAGE_TYPE        6
#define ERR_INVALID_STEP_DIRECTION      7


// Public Input parameter macros
#define VOLTAGE_TYPE_VDDD       1
#define VOLTAGE_TYPE_VDDIO      2

//------------------------------------------------------------------------------
// Public Function Name: SteppingVoltageSet
// Description: Performs intermediate V step(s) if needed. Steps iStepSize. 
//              For VDDD, polls VDD_OK register bit after each intermediate step if used. 
//              This possible polling adds inline delay that may be on the order 
//              of 1 ms per intermediate step. 
// Inputs:  WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize, /* 3 parms are V bit codes */
//          int iVoltageType      /* VOLTAGE_TYPE_VDDD or VOLTAGE_TYPE_VDDIO (vddio untested)   */
//
// Returns: RETCODE: passes back the status codes from VoltageStepUp or VoltageStepDown. 
// Notes:   When called from asm, the registers with these params are: a,b,x0,y0
//------------------------------------------------------------------------------
RETCODE SteppingVoltageSet(WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize, 
                               int iVoltageType);

   



// PRIVATE functions. Call the function above, not these. 
RETCODE VoltageStepUp(WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize,
                         int iVoltageType);
RETCODE VoltageStepDown(WORD wCurrentVoltageCode, WORD wDesiredVoltageCode, WORD wStepSize,
                             int iVoltageType);

void ClearHwDcdcTbrBatteryADJ(void);

void SetDCDCTestBitRegister(void);

 


