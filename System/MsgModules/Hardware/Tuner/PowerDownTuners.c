//-----------------------------------------------------------------------------
// Copyright (C) Sigmatel, Inc. 2003
//-----------------------------------------------------------------------------
//	File: PowerDownTuners.c
//	Purpose: Self contain routine for power down FM Tuners in USBMSC and DCC mode
//	Input:	none
//	Output:	none
//	Comments:	default to TEA5767 Tuner unless TEA5757 is defined
//
//-----------------------------------------------------------------------------
 
#include "hwequ.h"
#include "resource.h"
#include "project.h"
#ifdef TEA5757
#include "Philips5757/Philips5757.h"
#else
#include "Philips5767/Philips5767.h"
extern _reentrant void Philips3WireClockDelay(void);
extern _reentrant void Philips3WireWriteByte(INT);
#endif
_reentrant void PowerDownFMTuners(void)
{
// if TEA5757 tuner
#ifdef 	 TEA5757
//  enable the Power control GPIO and turn off the FET
	HW_3WIRE_FM_POWER_DOWN_PWR = TRUE;
	HW_3WIRE_FM_POWER_DOWN_ENR = TRUE;
    HW_3WIRE_FM_POWER_DOWN_DOER = TRUE;
    HW_3WIRE_FM_POWER_DOWN_DOR = TRUE;	

	HW_3WIRE_CLOCK_PWR = TRUE;
	HW_3WIRE_DATA_PWR = TRUE;
	HW_3WIRE_WRITE_ENABLE_PWR = TRUE;
    HW_3WIRE_MONO_UNTUNED_PWR = TRUE;

	HW_3WIRE_CLOCK_ENR = TRUE;
	HW_3WIRE_DATA_ENR = TRUE;
	HW_3WIRE_WRITE_ENABLE_ENR = TRUE;
    HW_3WIRE_MONO_UNTUNED_ENR = TRUE;

	HW_3WIRE_CLOCK_DOER = TRUE;
	HW_3WIRE_DATA_DOER = FALSE;
	HW_3WIRE_WRITE_ENABLE_DOER = TRUE;
    HW_3WIRE_MONO_UNTUNED_DOER = FALSE;

	HW_3WIRE_CLOCK_DOR = FALSE;
	HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
#else
// if TEA5767 tuner
// power up the GPIO and send the serial command to the tuner to shutdown
	HW_3WIRE_CLOCK_PWR = TRUE;
	HW_3WIRE_DATA_PWR = TRUE;
	HW_3WIRE_WRITE_ENABLE_PWR = TRUE;
    HW_3WIRE_BUS_ENABLE_PWR = TRUE;

	HW_3WIRE_CLOCK_ENR = TRUE;
	HW_3WIRE_DATA_ENR = TRUE;
	HW_3WIRE_WRITE_ENABLE_ENR = TRUE;
    HW_3WIRE_BUS_ENABLE_ENR = TRUE;

	HW_3WIRE_CLOCK_DOER = TRUE;
	HW_3WIRE_DATA_DOER = TRUE;
	HW_3WIRE_WRITE_ENABLE_DOER = TRUE;
    HW_3WIRE_BUS_ENABLE_DOER = TRUE;

	HW_3WIRE_CLOCK_DOR = FALSE;
	HW_3WIRE_DATA_DOR = TRUE;
	HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
    HW_3WIRE_BUS_ENABLE_DOR = TRUE;

    //drive the 5767 to input mode
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = FALSE;
        Philips3WireClockDelay();
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;
        Philips3WireClockDelay();

// write serial data to TEA5767 to put it in the standby mode
	Philips3WireWriteByte(0x80);	// Mute, search mode, the tuner and PLL high byte
	Philips3WireWriteByte(0x00);	// PLL low byte
	Philips3WireWriteByte(0x00);	// SUD, SSLx, HLSI, MS, ML, MR, SWP1
	Philips3WireWriteByte(0x51);	// SWP2, STANDBY, BL, XTAL, SMUTE, HCC, SNC, SI
	Philips3WireWriteByte(0x00);	// PLLREF, DTC, 0, 0, 0, 0, 0, 0
    HW_3WIRE_WRITE_ENABLE_DOR = TRUE;

#endif
}

#ifndef TEA5757
_reentrant void Philips3WireClockDelay(void)
{
    int i;
   for(i=0;i<PHILIPS_3WIRE_DELAY;i++)
   {
       _asm(" nop");
   };
}

_reentrant void Philips3WireWriteByte(INT data)
{
    int i;

    for(i=0;i<8;i++)
    {
        if(data&0x80)
            HW_3WIRE_DATA_DOR = TRUE;
        else
            HW_3WIRE_DATA_DOR = FALSE;
        data = (data<<1)&0xfe;
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = TRUE;
        Philips3WireClockDelay();
        HW_3WIRE_CLOCK_DOR = FALSE;
    }
        Philips3WireClockDelay();
        Philips3WireClockDelay();
}

#endif
