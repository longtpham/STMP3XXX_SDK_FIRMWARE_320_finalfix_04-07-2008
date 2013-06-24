/*;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : dac.c
;  Description : routines used to startup the DAC
;///////////////////////////////////////////////////////////////////////////////
*/
//#define  DEVICE_3410

#include "hwequ.h"

extern _X WORD CIRC source_data[0x400];

/******************* Global Defines owned by the DAC *******************************/
#define DAC_BUFF_MOD  0x7             // Size of the DAC buffer (add 1 to get size) (128 Words)
#define DAC_BUFF_SIZE  DAC_BUFF_MOD+1 // Size of the DAC buffer (add 1 to get size)
#define DAC_BUFF_WORDCOUNT 4          // Defines how many words to send before asserting 
									  // an interrupt to be filled

// Define the DAC source buffer 
_X CIRC WORD* _X DAC_Buffer  = source_data;

// these will be overwritten when DacDrv is inited/enabled
// DAC Refill ISR (Vector number 30 is located at at P:$60)
void _long_interrupt(30) DAC_Refill_ISR(void){
    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;
}

// DAC Underflow ISR (Vector 31 is at P:$60)
void _long_interrupt(31) DAC_Underflow_ISR(void){
	// Clear the Underflow bit
    HW_DACCSR.B.TXEN=0;
    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;
    HW_DACCSR.B.TXEN=1;
}
void _reentrant  mix_init(void){
	HW_CCR.I=0x04000D;		// Dclk = 40MHz, analog gets XTAL_clk for DAC
	//HW_CMTR.B.GBC=0x11;			// Set up the Codec/Mixer Test register per Mark
	HW_MIXPWRDNR.B.PR1=0;			// Power up Mixer and DAC, leave ADC off
	HW_MIXPWRDNR.B.PR2=0;
	//HW_HPCR.I=0x14;				// Turn Headphone on, BIGSIG=1, Class AB (will pop, %improve this) 	

}

void _reentrant start_dac(void) {

	mix_init();
    HW_MIXMASTERVR.I = 0x0;
    // DAC volume at 0 and unmuted
    HW_MIXDACINVR.I = 0x0;          // DACIn unmuted, vol = 0

	//  DAC_IRQ_init();
    // setup dac vectors because main.asm clears the vector table
    VECTOR(HW_IVECDACE,(int)DAC_Refill_ISR);
    VECTOR(HW_IVECDACUF,(int)DAC_Underflow_ISR);
#pragma asm
	bclr	#9,sr                  
	bclr	#8,sr                  
#pragma endasm 

    HW_ICLENABLE0R.B.SEN0 = 1;      // Enable source 0 (DAC Refill)
    HW_ICLENABLE0R.B.SEN1 = 1;      // Enable source 1 (DAC Overflow)
    HW_ICLPRIOR0R.B.S0P = 0;        // Set DAC Fill IRQ ICOLL priority to 0 (highest)
    HW_ICLPRIOR0R.B.S1P = 0;        // Set DAC Underflow IRQ ICOLL priority to 0 (highest)
    HW_ICLSTEER0R.B.S0S = 0;        // Steer the DAC Fill IRQ to IVL1 
    HW_ICLSTEER0R.B.S1S = 0;        // Steer the DAC Underflow IRQ to IVL1 
    HW_IPR.B.L1P = 3;               // Set Priority of IVL1 to 3 (high) 

    
//  DAC_init();
    // Set up DAC Base Address Reg
    HW_DACBAR.I = (int)DAC_Buffer;
    // Set up DAC Modulo Reg
    HW_DACMR.I=DAC_BUFF_MOD;
    // Set up DAC Word Count Reg
    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;
    // Set up the DAC Sample Rate Reg
    HW_DACSRR.I=0x04CE14;           // Value set for 44.1KHz
    // Set up the DAC Control Register (but don't start sending data yet)
    HW_DACCSR.B.TXIEN=1;            // Enable interrupts
    HW_DACCSR.B.TXEN=1;             // DAC will start transmitting preloaded values


}

