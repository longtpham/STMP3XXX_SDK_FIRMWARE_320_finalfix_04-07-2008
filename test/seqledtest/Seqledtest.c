/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename:     ledtest.c
// Description:  STMP3 test module for the LED API.
/////////////////////////////////////////////////////////////////////////////////

#include "seqledtest.h"
// LED_NUM can come from ledtable.asm
#define LED_NUMBER	5

// select the sequential on time
#define LED_SEQ_ON_TIME	100
// the LED off time depending on the number of LED in sequencing and the on time
#define LED_SEQ_OFF_TIME LED_SEQ_ON_TIME * (LED_NUMBER-1)
// create the delay on time for the LEDs
#define LED_DLY_TIME1	LED_SEQ_ON_TIME
#define LED_DLY_TIME2	LED_SEQ_ON_TIME+LED_DLY_TIME1
#define LED_DLY_TIME3	LED_SEQ_ON_TIME+LED_DLY_TIME2
#define LED_DLY_TIME4	LED_SEQ_ON_TIME+LED_DLY_TIME3
// blink the LED at equal on/off time
#define LED_BLINK_ON_TIME	150
#define LED_BLINK_OFF_TIME	LED_BLINK_ON_TIME

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
		
		SysPostMessage(5,LED_CONTROL,0,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   //blink all LEDs
		SysPostMessage(5,LED_CONTROL,1,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,2,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,3,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,4,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		Wait(2000);
		SysPostMessage(6,LED_DLY_ON_CONTROL,0,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,0);	   //Set the firing sequence
		SysPostMessage(6,LED_DLY_ON_CONTROL,1,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME1);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,2,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME2);	   
		SysPostMessage(6,LED_DLY_ON_CONTROL,3,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME3);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,4,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME4);	   
		Wait(6000);
		SysPostMessage(5,LED_CONTROL,0,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   //blink all LEDs
		SysPostMessage(5,LED_CONTROL,1,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,2,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,3,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,4,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		Wait(2000);
		SysPostMessage(6,LED_DLY_ON_CONTROL,4,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,0);	   //Alternate the firing sequence
		SysPostMessage(6,LED_DLY_ON_CONTROL,3,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME1);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,2,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME2);	   
		SysPostMessage(6,LED_DLY_ON_CONTROL,1,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME3);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,0,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,LED_DLY_TIME4);	   
		Wait(6000);
		SysPostMessage(5,LED_CONTROL,0,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   //blink all LEDs
		SysPostMessage(5,LED_CONTROL,1,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,2,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,3,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,4,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		Wait(2000);
		SysPostMessage(6,LED_DLY_ON_CONTROL,4,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,200);	   //exploding
		SysPostMessage(6,LED_DLY_ON_CONTROL,3,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,100);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,2,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,0);	   
		SysPostMessage(6,LED_DLY_ON_CONTROL,1,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,100);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,0,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,200);	   
		Wait(6000);
		SysPostMessage(5,LED_CONTROL,0,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   //blink all LEDs
		SysPostMessage(5,LED_CONTROL,1,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,2,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,3,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		SysPostMessage(5,LED_CONTROL,4,LED_BLINK_ON_TIME,LED_BLINK_OFF_TIME);	   
		Wait(2000);
		SysPostMessage(6,LED_DLY_ON_CONTROL,4,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,0);	   //imploding
		SysPostMessage(6,LED_DLY_ON_CONTROL,3,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,100);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,2,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,200);	   
		SysPostMessage(6,LED_DLY_ON_CONTROL,1,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,100);	  
		SysPostMessage(6,LED_DLY_ON_CONTROL,0,LED_SEQ_ON_TIME,LED_SEQ_OFF_TIME,0);	   
		Wait(6000);
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
