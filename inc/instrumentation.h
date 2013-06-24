#ifndef __INSTRUMENTATION
#define __INSTRUMENTATION

#ifdef USE_INSTRUMENTATION

#include "instrument_consts.h"

_reentrant _callee_save void InstrumentPushContext(BYTE btContext, BYTE btValue);
_reentrant _callee_save void InstrumentPopContext(void);
_reentrant _callee_save void InstrumentSetValue(BYTE btValue);
_reentrant _callee_save void InstrumentSetEvent(BYTE btEvent);
_reentrant _callee_save void InstrumentClearEvent(BYTE btEvent);
_reentrant _callee_save void InstrumentToggleEvent(BYTE btEvent);
_reentrant _callee_save void InstrumentInitialize(void);
#else
#define InstrumentPushContext(x,y) ;
#define InstrumentPopContext()     ;
#define InstrumentSetValue(x)      ;
#define InstrumentSetEvent(x)      ;
#define InstrumentClearEvent(x)    ;
#define InstrumentToggleEvent(x)   ;
#define InstrumentInitialize()     ;
#endif

#endif