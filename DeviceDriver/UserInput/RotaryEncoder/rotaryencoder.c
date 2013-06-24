////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2001
//
// File : rotaryencoder.c
// Description : This file contains the necessary routine to setup and decode the
//	quadrature encoder or endless rotary encoder.  The mechanical or hall effect
//	device output two signals with 90 degree out-of-phase of each other.
//	It is assume that the rest state of the encoder is PHASE_A where both switches
//	of the encoder are off. If the encoder traverse from PHASE_A to PHASE_B, PHASE_B
//	to PHASE_C, and PHASE_C to PHASE_A then the shaft or the wheel of the encoder is
//	rotated clockwise.  If the phases are reversed, then the encoder is rotated counter
//	clockwise.  Most of the encoder has detent that give the clicking feedback to the
//	user and each click yeild one CW or CCW pulse.  Each pulse has 4 states or phases but the encoder
//	may not have 4 states per click.  The variable PHASE_PER_DETENT	allow the encoder
//	to adjust the output count to the phase per detent.
//	This driver uses interupts on both GPIO to keep up with the state changes. It uses
//	the combination of the current state and previous state to determine the rotation
//	direction.  If the encoder could not keep up with the pulses, the illegal states are
//	ignored and no change in the pulse counting.
//	The application defines the uses for the encoder. It can redefine the maximum and minimum value
//	of the counting.  The menu system can change the application of the rotary encoder
//	to fit its need such as volume control in most application and music track selector
//	when user in track selecting mode or FM station frequency tuning in FM tuner mode.
//	This device driver is not directly integrated into the CORE system.  It is designed as
//	an add-on driver.  It is initialized after the completion of the StartProject.  The counter starts
//	to change as soon as the shaft or the wheel is turned after the completion of the initialization.
//	In the menu system that requies an event when the encoder changes its count, the menu should setup the 
//	driver with the message and its count.  If the count provided by the menu is different from the encoder's count,
//	g_wRotaryEncoderValue, the notification is immediately posted other wise the notification message is saved
//	until the encoder's value is changed.
//	To minimize the time spent in the ISR, the bEncoderChanged flag is set when the g_wRotaryEncoder is changed. A call
//	from the exec loop to handle the rotary encoder, HandleRotaryEncoder, check this flag and if the notification message
//	is saved then it post that message to the menu to notify the change and changes the notification message to -1 to indicate
//	that no notify message is saved. The bEnocderChanged flag is also reset.
//	The menu system should resend the Notify message to the driver after it has received the notification. It should
//	read the g_wRotaryEncoderValue and equate this value to the applicabble variable.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "hwequ.h"
#include "exec.h"

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

// define hardware
// experiment using either endge sensitive or level sensitive interrupts to get the best performance
#define EDGE_INTR	FALSE

#ifndef STMP34xx_144TQFP   // default to 100-pin STMP34xx
// filtered swith A of the encoder on GP33
#define HW_ROTARY_A_ENR  HW_GP1ENR.B.B9
#define HW_ROTARY_A_DOR  HW_GP1DOR.B.B9
#define HW_ROTARY_A_DIR  HW_GP1DIR.B.B9
#define HW_ROTARY_A_DOER  HW_GP1DOER.B.B9
#define HW_ROTARY_A_PWR  HW_GP1PWR.B.B9
#define HW_ROTARY_A_IPENR	HW_GP1IPENR.B.B9
#define HW_ROTARY_A_IENR	HW_GP1IENR.B.B9
#define HW_ROTARY_A_ILVLR	HW_GP1ILVLR.B.B9
#define HW_ROTARY_A_IPOLR	HW_GP1IPOLR.B.B9
#define HW_ROTARY_A_ISTATR	HW_GP1ISTATR.B.B9

// filtered swith B of the encoder on GP37
#define HW_ROTARY_B_ENR  HW_GP1ENR.B.B13
#define HW_ROTARY_B_DOR  HW_GP1DOR.B.B13
#define HW_ROTARY_B_DIR  HW_GP1DIR.B.B13
#define HW_ROTARY_B_DOER  HW_GP1DOER.B.B13
#define HW_ROTARY_B_PWR	 HW_GP1PWR.B.B13
#define HW_ROTARY_B_IPENR	HW_GP1IPENR.B.B13
#define HW_ROTARY_B_IENR	HW_GP1IENR.B.B13
#define HW_ROTARY_B_ILVLR	HW_GP1ILVLR.B.B13
#define HW_ROTARY_B_IPOLR	HW_GP1IPOLR.B.B13
#define HW_ROTARY_B_ISTATR	HW_GP1ISTATR.B.B13
// Interupt control
#define HW_ROTARY_ICOLENB  HW_ICLENABLE0R.B.SEN11
#define HW_ROTARY_ICOLSTAT	HW_ICLSTATUS0R.B.SST11
#define HW_ROTARY_ICOLPRIOR	HW_ICLPRIOR1R.B.S11P
#define HW_ROTARY_ICOLSTEER	HW_ICLSTEER0R.B.S11S
#else // 144-pin STMP34xx
// filtered swith A of the encoder on GP69
#define HW_ROTARY_A_ENR  HW_GP2ENR.B.B21
#define HW_ROTARY_A_DOR  HW_GP2DOR.B.B21
#define HW_ROTARY_A_DIR  HW_GP2DIR.B.B21
#define HW_ROTARY_A_DOER  HW_GP2DOER.B.B21
#define HW_ROTARY_A_PWR  HW_GP2PWR.B.B21
#define HW_ROTARY_A_IPENR	HW_GP2IPENR.B.B21
#define HW_ROTARY_A_IENR	HW_GP2IENR.B.B21
#define HW_ROTARY_A_ILVLR	HW_GP2ILVLR.B.B21
#define HW_ROTARY_A_IPOLR	HW_GP2IPOLR.B.B21
#define HW_ROTARY_A_ISTATR	HW_GP2ISTATR.B.B21

// filtered swith B of the encoder on GP68
#define HW_ROTARY_B_ENR  HW_GP2ENR.B.B20
#define HW_ROTARY_B_DOR  HW_GP2DOR.B.B20
#define HW_ROTARY_B_DIR  HW_GP2DIR.B.B20
#define HW_ROTARY_B_DOER  HW_GP2DOER.B.B20
#define HW_ROTARY_B_PWR	 HW_GP2PWR.B.B20
#define HW_ROTARY_B_IPENR	HW_GP2IPENR.B.B20
#define HW_ROTARY_B_IENR	HW_GP2IENR.B.B20
#define HW_ROTARY_B_ILVLR	HW_GP2ILVLR.B.B20
#define HW_ROTARY_B_IPOLR	HW_GP2IPOLR.B.B20
#define HW_ROTARY_B_ISTATR	HW_GP2ISTATR.B.B20
// Interupt control
#define HW_ROTARY_ICOLENB  HW_ICLENABLE0R.B.SEN12
#define HW_ROTARY_ICOLSTAT	HW_ICLSTATUS0R.B.SST12
#define HW_ROTARY_ICOLPRIOR	HW_ICLPRIOR1R.B.S12P
#define HW_ROTARY_ICOLSTEER	HW_ICLSTEER1R.B.S12S
#endif
	
#define PHASE_PER_DETENT	4	  // Phases per click used to convert the phase count to clicks

// the value of the encoder output is between 0 and 100
#define MAX_ENCODER_VALUE	100
#define MIN_ENCODER_VALUE	0
#define INIT_ENCODER_VALUE	MAX_ENCODER_VALUE/2

#ifndef STMP34xx_144TQFP   // default to 100-pin STMP34xx
	#define USE_VECTOR  HW_IVECGPIO1	/* setup vector */
#else
	#define USE_VECTOR  HW_IVECGPIO2	/* setup vector */
#endif


////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
// Globals
int g_wRotaryEncoderValue;		// This variable holds the absolute position of the encoder
int g_wMaxEncoderValue;		    //  The maximum and minimum position allowed are defined here
int g_wMinEncoderValue;
int g_iDebugErrorCount;			// it is used for debugging purpose only, counts number of error

// Statics
int PhaseCounter = 0;
static int NotifyMessage = -1;
INT LastState=3;
INT ThisState;
BOOL bEncoderRotated = FALSE;

void _long_interrupt(USE_VECTOR) RotaryEncoder_ISR(void);
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RotaryEncoderInit
//
//   Type:          Function
//
//   Description:   Initialize the HW registers and variables.
//
//   Inputs:        none
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

// Initialize the Encoder
_reentrant void RotaryEncoderInit( void )
{
	HW_ROTARY_B_ENR = TRUE;
	HW_ROTARY_B_DOER = FALSE;
	HW_ROTARY_B_PWR = TRUE;
	
	HW_ROTARY_A_ENR = TRUE;
	HW_ROTARY_A_DOER = FALSE;
	HW_ROTARY_A_PWR = TRUE;
		
	NotifyMessage = -1;	// no call back message set

	g_iDebugErrorCount = 0;	// clear the error count

	PhaseCounter = 0;
	LastState = 0;
	bEncoderRotated = FALSE;

	g_wMaxEncoderValue = MAX_ENCODER_VALUE;
	g_wMinEncoderValue = MIN_ENCODER_VALUE;

// wait here until the GPIOs are steady
	while ((HW_ROTARY_B_DIR==FALSE)||(HW_ROTARY_A_DIR==FALSE));

	if ( HW_ROTARY_A_DIR == TRUE)
	{
		LastState |= 1;
	}

	if ( HW_ROTARY_B_DIR == TRUE)
	{
		LastState |= 2;
	}

	if ((g_wRotaryEncoderValue > MAX_ENCODER_VALUE) || (g_wRotaryEncoderValue < 0))
	{
		g_wRotaryEncoderValue = INIT_ENCODER_VALUE;
	}

	ThisState = LastState;
// Setup interupt collector
	HW_ROTARY_ICOLENB  = TRUE;
	HW_ROTARY_ICOLPRIOR	= 4;
	HW_ROTARY_ICOLSTEER	= 2;

// setup interrupt on both pins
	VECTOR(USE_VECTOR, (INT) RotaryEncoder_ISR);	/* setup vector */

#if EDGE_INTR
	HW_ROTARY_B_ILVLR = HW_ROTARY_A_ILVLR = FALSE;
#else
	HW_ROTARY_B_ILVLR = HW_ROTARY_A_ILVLR = TRUE;
#endif
	HW_ROTARY_B_IPOLR = HW_ROTARY_A_IPOLR = FALSE;
	HW_ROTARY_B_IPENR = HW_ROTARY_A_IPENR = TRUE;
	HW_ROTARY_B_IENR = HW_ROTARY_A_IENR = TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          HandleRotaryEncoder
//
//   Type:          Function
//
//   Description:   Test if the shaft/wheel has been rotated and post the message if
//					the rotation is true.
//
//   Inputs:        none
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

_reentrant void HandleRotaryEncoder( void )
{
	if ((NotifyMessage!=-1)&& bEncoderRotated)
	{
		bEncoderRotated = FALSE;
		SysPostMessage(2,NotifyMessage);
		NotifyMessage = -1;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          vRotaryEncoderNotifySetup
//
//   Type:          Function
//
//   Description:   setup the notify message when the encoder value changed. This is done
//					so the encoder can notify the menu system about the change. If the value
//					passed to this routine is different from the value of the current encoder
//					value, the message is immediately posted.
//
//   Inputs:        INT NotifyMsg,  INT iCallBackIfNotThisCount
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

_reentrant int iRotaryEncoderNotifySetup(INT ThisNotifyMsg, INT iNotifyIfNotThisCount)
{
	if ((iNotifyIfNotThisCount>=g_wMinEncoderValue) && (iNotifyIfNotThisCount<=g_wMaxEncoderValue))
	{
		NotifyMessage = ThisNotifyMsg;	// call back message set
		if (iNotifyIfNotThisCount!=g_wRotaryEncoderValue)
		{
			SysPostMessage(2,NotifyMessage);
			NotifyMessage = -1;	  // one call back per setup
			bEncoderRotated = FALSE;
		}
		return (0);
	}
	return (-1);
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          RotaryEncoder_ISR
//
//   Type:          Interrupt service routine
//
//   Description:   capture the encoder rotation and translate it into the direction and value.
//					The value does not change above or below the maximum and minimum values set
//					set in g_wMaxEncoderValue and g_wMinEncoderValue.
//					Select either the edge sensitive or level sensitive interrupt for the best
//					performance.
//
//   Inputs:        INT NotifyMsg,  INT iCallBackIfNotThisCount
//
//   Outputs:       None
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

void _long_interrupt(USE_VECTOR) RotaryEncoder_ISR(void)
{

    #pragma asm
    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr
    nop
    #pragma endasm

	// since only the GP68 and GP69 are on this interrupt, we don't have to check if the interrupts belong to them
	// otherwise only direct the rotaryencoder interrupts here.
	// get the new phase information
	ThisState=(HW_ROTARY_B_DIR<<1) | HW_ROTARY_A_DIR;
	// find out which GPIO is interrupting
	if (HW_ROTARY_B_ISTATR==TRUE)
	{ /* Phase B interrupting */
#if EDGE_INTR
//		HW_ROTARY_B_IENR = FALSE;
		HW_ROTARY_B_ISTATR = TRUE; /* clear the status */
		HW_ROTARY_B_IPOLR ^= 1; /* flip the interrupt polarity */
//		HW_ROTARY_B_IENR = TRUE;
        HW_ROTARY_B_ISTATR = TRUE;
#else
//		HW_ROTARY_B_IENR = FALSE;
		HW_ROTARY_B_IPENR = FALSE;
        HW_ROTARY_B_ISTATR = FALSE;
		HW_ROTARY_B_IPOLR ^= 1; /* flip the interrupt polarity */
		HW_ROTARY_B_IPENR = TRUE;
//		HW_ROTARY_B_IENR = TRUE;
#if defined(DEVICE_3500)        // Hack needed for clearing the Status register.
        HW_ROTARY_A_ISTATR = FALSE;
#endif
#endif
	}
	else
	{ /* Phase A is interrupting */
#if EDGE_INTR
//		HW_ROTARY_A_IENR = FALSE;
		HW_ROTARY_A_ISTATR = TRUE; /* clear the status */
		HW_ROTARY_A_IPOLR ^= 1; /* flip the interrupt polarity */
//		HW_ROTARY_A_IENR = TRUE;
        HW_ROTARY_A_ISTATR = TRUE;
#else
//		HW_ROTARY_A_IENR = FALSE;
		HW_ROTARY_A_IPENR = FALSE;
        HW_ROTARY_A_ISTATR = FALSE;
		HW_ROTARY_A_IPOLR ^= 1; /* flip the interrupt polarity */
		HW_ROTARY_A_IPENR = TRUE;
//		HW_ROTARY_A_IENR = TRUE;
#if defined(DEVICE_3500)        // Hack needed for clearing the Status register.
        HW_ROTARY_B_ISTATR = FALSE;
#endif
#endif
	}
	switch(ThisState*4+LastState)
	{			 // self transitions
			case 0 :
			case 5 :
			case 10 :
			case 15 :	// should not be here
				break;
				// Clockwise states
			case 11 : // 1011 -> Phase A to Phase B
			case 2 : // 0010 -> Phase B to Phase C
			case 4 : // 0100 -> Phase C to Phase D
			case 13 : // 1101 -> Phase D to Phase A
				PhaseCounter++;
				break;
				// Counter clockwise states
			case 7 : // 0111 -> Phase A to Phase D
			case 1 : // 0001 -> Phase D to Phase C
			case 8 : // 1000 -> Phase C to Phase B
			case 14 : // 1110 -> Phase B to Phase A
				PhaseCounter--;
				break;
				// Illegal transitions
			case 3 : // 0011 -> Phase A to Phase C
			case 6 : // 0110 -> Phase B to Phase D
			case 12 : // 1100 -> Phase C to Phase A
			case 9 : // 1001 -> Phase D to Phase B
				break;
	}
	LastState=ThisState;
	// if the g_wRotaryEncoderValue is out-of-range then put it at the limit
	if (g_wRotaryEncoderValue > g_wMaxEncoderValue)
		g_wRotaryEncoderValue = g_wMaxEncoderValue;
	else if (g_wRotaryEncoderValue < g_wMinEncoderValue)
		g_wRotaryEncoderValue = g_wMinEncoderValue;
	// translate the phases into actual count or clicks
	if ((PhaseCounter >= PHASE_PER_DETENT)||(PhaseCounter <= - PHASE_PER_DETENT))
	{
		if (PhaseCounter >= PHASE_PER_DETENT)
		{
			PhaseCounter -= PHASE_PER_DETENT;
			if (++g_wRotaryEncoderValue > g_wMaxEncoderValue)
				{ g_wRotaryEncoderValue = g_wMaxEncoderValue;}
		}
		else
		{
			PhaseCounter += PHASE_PER_DETENT;
			if (--g_wRotaryEncoderValue < g_wMinEncoderValue)
				{ g_wRotaryEncoderValue = g_wMinEncoderValue;}
		}

		bEncoderRotated = TRUE;
	}
}
