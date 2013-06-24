////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2002
//
// Filename: FMTunerAnalog.c
// Description: Initialization module for FM Tuner Analog section.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "messages.h"
#include "hwequ.h"
#include "sysresourceapi.h"

////////////////////////////////////////////////////////////////////////////////
//   Globals
////////////////////////////////////////////////////////////////////////////////

void _reentrant RetrieveMixerMsg(void);
_reentrant void ForceFMTInit (void);

void _reentrant SysPostMessage(int iLength,...);


/////////////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          _reentrant void FMTAnalogInit(void)
//
//   Type:          Function 
//
//   Description:   Initializes the analog sections of the FM tuner
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
//////////////////////////////////////////////////////////////////////////////////////////
_reentrant void FMTAnalogInit(void)
{
	// power up analog mixer for LINE IN
    HW_MIXPWRDNR.B.PR2 = 0;  // 3410 only

#if defined(DEVICE_3410)
    HW_HPCTRL.B.HPHPWD = 0;
    HW_MIXMASTERVR.I = 0;
    HW_MIXLINE1INVR.I = 0x404;
    HW_MIXLINE2INVR.I = 0x404;
    HW_HPCTRL.B.CLASSAB = 1;
    SysPostMessage(3,MIXER_LINE_SETLVL,0x000a0a);
#else
    HW_HPCTRL.B.HPPWD = 0;
    HW_MIXMASTERVR.I = 0;
    HW_MIXLINE1INVR.I = 0x404;

#ifdef FMTUNER_ON_LINE2_IN 
    HW_MIXLINE2INVR.I = 0x404;  // only on 144 pin package.
#endif

    HW_HPCTRL.B.HPCLASSAB = 1;
    SysPostMessage(3,MIXER_LINE_SETLVL,0x000F0F);
#endif

    HW_MIXRECSELR.I = 0x505;
    
    SysPostMessage(2,MIXER_LINE_UNMUTE);
    SysPostMessage(2,MIXER_DAC_MUTE);
	//SysPostMessage(3,MIXER_MASTER_FADE_IN,10);
	RetrieveMixerMsg();
	
	ForceFMTInit();	
}

void _reentrant RetrieveMixerMsg(void)
{
    #pragma asm
        nolist
        extern  MixerQueueDescriptor
        extern  MixerProcessQueue
        list
        
        move    #MixerQueueDescriptor,r0
        move    #-1,m0
        jsr     MixerProcessQueue
    #pragma endasm
}

_reentrant void ForceFMTInit (void)
{
#pragma asm
    include "msgequ.inc"
    extern  MODULE_NUM_FMTUNER
    extern  SignalModule
    move    #>MODULE_NUM_FMTUNER,x1
    move    #>EVENT_INIT,x0
    jsr     SignalModule
#pragma endasm
}
