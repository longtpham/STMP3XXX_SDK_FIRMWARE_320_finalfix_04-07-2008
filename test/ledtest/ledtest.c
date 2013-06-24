/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename:     ledtest.c
// Description:  STMP3 test module for the LED API.
/////////////////////////////////////////////////////////////////////////////////

#include "ledtest.h"

extern _asmfunc void LedInit(void);	//should be in led.asm
extern _asmfunc void ExecuteModules (void);
void Wait(unsigned int);

extern int ShortestTime;	//in led.xref
extern unsigned long SysTime;
extern _asmfunc void SysTimeIsr(void);

///////////////////////////////////////////////////////////////////////////////
//   Program Entry Point
///////////////////////////////////////////////////////////////////////////////
void main(void)
{
	//clear interrupts just in case DCC has messed with them 
	HW_ICLENABLE0R.I = 0;
	HW_ICLENABLE1R.I = 0;
	HW_ICLSTEER0R.I = 0;
	HW_ICLSTEER1R.I = 0;
	HW_ICLSTEER2R.I = 0;

#pragma asm
    include "regsicoll.inc"
    include "regscore.inc"    
	include "systimers.inc"
	include "regstimer.inc"	;stmp6550
	extern Const_ffffff

    move    y:<Const_ffffff,x0
    move    x0,x:HW_ICLPRIOR0R
    move    x0,x:HW_ICLPRIOR2R
    move    x0,x:HW_ICLPRIOR3R
	
	move    #>$1FFFFF,a0	;set the priority for the MS Timer to highest
	move    a0,x:HW_ICLPRIOR1R

   	move     #$003000,a0	;setup the IPR register for interrupt enabling
   	move    a0,x:HW_IPR 

#pragma endasm

//setup the vector table for Timer2
	VECTOR(HW_IVECTIMER2,(int)SysTimeIsr);
	
	SysResetAllTimers();
	SysTimeInit();

#pragma asm    
    ; Set the SR bits
    move    sr,x0
    bclr    #HW_SR_IM0_BITPOS,x0
    bclr    #HW_SR_IM1_BITPOS,x0
    move    x0,sr
    
    ;enable the timer2 interrupt
	bset    #>15,X:HW_ICLENABLE0R

#pragma endasm

	SysTime = 0;
	ShortestTime = -1;
	LedInit(); 
 
	while(1){	  //loop forever
		
		SysPostMessage(5,LED_CONTROL,0,1,0);	   //converted messages from .asm
		SysPostMessage(5,LED_CONTROL,1,1,0);	   //turn on both LEDs
		Wait(60);
		SysPostMessage(5,LED_CONTROL,0,0,0);	   //turn off LED 1
		Wait(60);
		SysPostMessage(5,LED_CONTROL,1,0,0);	   //turn off LED 2
		Wait(60);
 		SysPostMessage(5,LED_CONTROL,0,500,500);	//LED blink.
		SysPostMessage(5,LED_CONTROL,1,250,250);
		Wait(7000);
	}	
}

#pragma asm
SysLoadResource:
	rts
#pragma endasm

void Wait(unsigned int wDelay){
WORD wTemp = 10000;	//how many clock cycles???
    do
    {
        do
        {      
            wTemp--;        
    
        }while(wTemp);
        ExecuteModules();//calls led module to blink leds if the moduletimer has expired
        wTemp = 10000;     
        wDelay--;
    }while(wDelay);    
}
