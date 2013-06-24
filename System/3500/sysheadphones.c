/*;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2004
;
;  File        : sysheadphones.asm
;  Description : routines used to setup/shutdown headphones
;///////////////////////////////////////////////////////////////////////////////
*/

#include "hwequ.h"
#include "resource.h"
#include "project.h"

//#define UP                  1
//#define DOWN                2
#define INITIAL_INCR        100     // step size during DAC ISR RAMP // Change 300 to 100 to low down the 'POP' sound when startup --17/01-07
#define INITIAL_DAC_VALUE   900000  // initial dac value when powering up
#define FINAL_DAC_VALUE     900000  // while powering down

/******************* Global Defines owned by the DAC *******************************/
#define DAC_BUFF_MOD  0x7                   // Size of the DAC buffer (add 1 to get size) (128 Words)
#define DAC_BUFF_SIZE  DAC_BUFF_MOD+1       // Size of the DAC buffer (add 1 to get size)
#define DAC_BUFF_WORDCOUNT 4                // Defines how many words to send before asserting an interrupt to be filled

#ifndef HP_SHORT_TRIP_POINT
// Previous version was always defining HP_SHORT_TRIP_POINT as HW_HPCTRL_SHORT_LVLADJ_1_00X_SETMASK
 #if defined(CAPLESS_HP)
#define HP_SHORT_TRIP_POINT  	HW_HPCTRL_SHORT_LVLADJ_0_75X_SETMASK
 #else
#define HP_SHORT_TRIP_POINT  	HW_HPCTRL_SHORT_LVLADJ_1_25X_SETMASK
 #endif
#endif

extern unsigned _X MixerMasterVol;
extern void SysDelay (unsigned int wait_time);

#if !defined(CAPLESS_HP)
_reentrant void _long_interrupt(-1) DAC_Refill_ISR1(void);
_reentrant void _long_interrupt(-1) DAC_Refill_ISR2(void);
_reentrant void _long_interrupt(-1) DAC_Underflow_ISR1(void);
_reentrant void _long_interrupt(-1) DAC_Underflow_ISR2(void);
#endif

#ifdef FUNCLET
  #pragma asm
    extern SysCallFunclet
  #pragma endasm
#endif    

//int PowerUpDown;
BOOL TransmitZeros = FALSE; 
BOOL TransmitMax = FALSE;

// Define the DAC source buffer 
int _circ _X DAC_Buffer[DAC_BUFF_SIZE];   
int _circ _X *pDAC_Buffer_Head = DAC_Buffer;

// DAC Underflow ISR (Vector 31 is at P:$60)
void _long_interrupt(31) DAC_Underflow_ISR(void){
}

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}

/*;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           PowerUpHeadPhones
;
;   Type:           Function
;
;   Description:    Executes the head phones power up sequence
;
;   Inputs:         none
;
;   Outputs:        none 
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
*/
#ifdef FUNCLET
    #pragma asm
        global FPowerUpHeadPhones
        FPowerUpHeadPhones:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_POWERUPHEADPHONES

        org p,"SYSFUNCLET_PowerUpHeadPhones_P":
        dc      RSRC_FUNCLET_POWERUPHEADPHONES
    #pragma endasm
_reentrant void PowerUpHeadPhones_Funclet(void)
{
#else
_reentrant void PowerUpHeadPhones(void)
{
#endif
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////Build proper HP Start-Up Routine//////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#if defined(CAPLESS_HP)
    HW_MIXMASTERVR.B.MUTE = 1;		//mute master before HP power-up

    SysDelay(1);                    // Wait about 1ms
    HW_HPCTRL.B.CAPLESS=1;          // Put Headphone in Capless Mode

    SysDelay(1);                    // Wait about 1ms
    HW_HPCTRL.B.HPPWD=0;           // Turn Headphone amp on as Class A

    SysDelay(1);                    // Wait about 1ms
	HW_MIXPWRDNR.I = 0xA00;			// power up the DAC Mixer

    SysDelay(1);                    // Wait about 1ms
    HW_HPCTRL.B.HPCLASSAB=1;          // Turn Headphone to Class AB

	// In Capless Mode, Short Init() must be performed after power-up to prevent false short
    // Headphone Short Init() -- define short detect mode, enable edge triggered IRQA
    // auto mode -- hw will power down headphone amp before IRQA interrupt
    // The Common Amplifier (SHORTMODE_CM) has internal short protection/recovery and should be OK in mode 00.  
    HW_HPCTRL.B.SHORTMODE_LR = HW_AUTO_HEADAMP_SHUTDOWN_EN;     // Arm the HW auto hphone amp shutoff mechanism.
    HW_HPCTRL.I = HW_HPCTRL.I | HP_SHORT_TRIP_POINT;	//Set sensitivity
    HW_IPR.B.IRQAT = 1;                                     // falling edge
    HW_IPR.B.IRQAP = HW_IPR_IRQA_SETMASK;                   // Enable IRQA Priority headphone short circuit interrupt.
    ///////////// end headphone short init
#else
//////////////// HP Start-up to reduce popping  ///////////////////////
    int i;
//    PowerUpDown = UP;    
 
//    HW_GP0DOR.B.B16 = 0;     // temp timing gpio
    // Turn Off ZCD mute, volume, etc
    HW_MIXTBR.B.DZCMA = 0;          // Turn off Master Volume ZCD    
    HW_MIXTBR.B.DZCDA = 0;          // Turn off DAC Volume ZCD    
    HW_MIXTBR.B.EZD = 0;            // Turn off Mixer Volume ZCD   (datasheet v2.0 is wrong)

    // prefill DAC w/ level corresponding to ~0V
    for (i=0; i<DAC_BUFF_SIZE; i++){
        *pDAC_Buffer_Head++ = INITIAL_DAC_VALUE;
    }

    HW_MIXMASTERVR.I = 0x0; 
    HW_MIXDACINVR.I = 0x0;          // DACIn unmuted, vol = 0

//  DAC_IRQ_init();
    // setup dac vectors because main.asm clears the vector table
    VECTOR(HW_IVECDACE,(int)DAC_Refill_ISR1);
    VECTOR(HW_IVECDACUF,(int)DAC_Underflow_ISR1);

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
    HW_DACSRR.I=0x0780df;           // Value set for 44.1KHz
    HW_DACCPR.I=0;

    HW_DACCSR.B.TXIEN=1;            // Enable interrupts
    HW_DACCSR.B.TXEN=1;             // DAC will start transmitting preloaded values

    // Headphone Short Init() -- define short detect mode, enable edge triggered IRQA
    // auto mode -- hw will power down headphone amp before IRQA interrupt 
    // HW_HPCTRL.B.SHORTMODE_LR = HW_AUTO_HEADAMP_SHUTDOWN_EN;     // Arm the HW auto hphone amp shutoff mechanism.
    HW_HPCTRL.I = HW_HPCTRL.I | HP_SHORT_TRIP_POINT;			//Set sensitivity
    HW_IPR.B.IRQAT = 1;                                     // falling edge
    HW_IPR.B.IRQAP = HW_IPR_IRQA_SETMASK;                   // Enable IRQA Priority headphone short circuit interrupt.
    ///////////// end headphone short init

    SysDelay(1);                    // Wait about 1ms
    HW_HPCTRL.B.HPPWD=0;           // Turn Headphone amp on as Class A

    SysDelay(1);                    // Wait about 1ms
    HW_HPCTRL.B.SHORTMODE_LR = HW_AUTO_HEADAMP_SHUTDOWN_EN;     // Arm the HW auto hphone amp shutoff mechanism.
    HW_MIXPWRDNR.I = 0xA00;			// power up the DAC Mixer

    SysDelay(1);                    // Wait about 1ms
    HW_HPCTRL.B.HPCLASSAB=1;          // Turn Headphone to Class AB


    // ramp DAC buffer value down to 0 slowly to ramp the headphone output  
    // to mid-point (Vag)   
    while(*pDAC_Buffer_Head>0) ;    // One line loop


    for (i=0;i<=8;i++){
        HW_MIXMASTERVR.B.MR = i;        
        HW_MIXMASTERVR.B.ML = i;
        SysDelay(1);
    } 
    HW_MIXMASTERVR.B.MUTE = 1;		//mute master before HP power-up
    HW_MIXMASTERVR.I = MixerMasterVol;  // return to default volume
    
    HW_MIXDACINVR.I = 0x0606;           // DAC Volume back to default

    HW_DACCSR.B.TXEN=0;                 // Stop the DAC Transmit     

	//ZCD on creates pops between songs.  Disable for 3500.
#endif //IF/ELSE CAPLESS_HP
}

#if !defined(CAPLESS_HP)
// these will be overwritten when DacDrv is inited/enabled
// DAC Refill ISR (Vector number 30 is located at at P:$60)
_reentrant void _long_interrupt(-1) DAC_Refill_ISR1(void){
    int i;

    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;			// reset DAC word count to prevent underflows

    for (i=0;i<DAC_BUFF_WORDCOUNT;i++)  {
        if(TransmitMax == TRUE)
            *pDAC_Buffer_Head++ = INITIAL_DAC_VALUE; // value corresponding to 0 o/p Volts.
        else{
            if(*pDAC_Buffer_Head > 0)
                (*pDAC_Buffer_Head++) -= INITIAL_INCR;
        }
    }
}

// DAC Underflow ISR (Vector 31 is at P:$60)
_reentrant void _long_interrupt(-1) DAC_Underflow_ISR1(void){
    // Clear the Underflow bit
    HW_DACCSR.B.TXEN=0;
    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;
    HW_DACCSR.B.TXEN=1;
}
#endif

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextsysheadphones":
    #pragma endasm
#endif

/*;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           PowerDownHeadPhones
;
;   Type:           Function
;
;   Description:    Executes the head phones power down sequence
;
;   Inputs:         none
;
;   Outputs:        none 
;                   
;   Notes:          
;<
;///////////////////////////////////////////////////////////////////////////////
*/
#ifdef FUNCLET
    #pragma asm
        FPowerDownHeadPhones:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_POWERDOWNHEADPHONES

        org p,"SYSFUNCLET_PowerDownHeadPhones_P":
        dc      RSRC_FUNCLET_POWERDOWNHEADPHONES
    #pragma endasm
_reentrant void PowerDownHeadPhones_Funclet(void)
#else
_reentrant void PowerDownHeadPhones(void)
#endif    
{
/////////////////////////////////////////////////////////////////////////////////////
///////////////////////Build proper HP Shutdown Routine//////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#if defined(CAPLESS_HP)
//No HP shut-down needed for Capless.  Add code if needed.
#else
//////////////////////// AC-C SHUTDOWN /////////////////////////////////
//////////////// HP Shutdown to reduce popping  ///////////////////////
    int i; 
    pDAC_Buffer_Head = DAC_Buffer; //point to start of DAC_Buffer
//    PowerUpDown = DOWN;
    HW_DACCSR.B.TXEN=0;             // stop transmission
    HW_DACCSR.B.TXIEN=0;            //stop interrupt


	// Mute LINE1 and LINE2, to avoid volume increase if shutting down
	// while listening to FM tuner
	HW_MIXLINE1INVR.B.MUTE = 1;
	HW_MIXLINE2INVR.B.MUTE = 1;

    // prefill DAC w/ level corresponding to Vag = Vdd/2 Volts,
    //because its gonna be at this avg level when the player is playing

    for (i=0; i<DAC_BUFF_SIZE; i++){
        *pDAC_Buffer_Head++ = 0; 
    }
    
    // NOTE: this does not happen immediately -- must wait until completed -- see below
    HW_MIXTBR.B.DZCMA = 0;          // Turn off Master Volume ZCD    
    HW_MIXTBR.B.DZCDA = 0;          // Turn off DAC Volume ZCD    
    HW_MIXTBR.B.EZD = 0;            // Turn off Mixer Volume ZCD    

    VECTOR(HW_IVECDACE,(int)DAC_Refill_ISR2); //change dac refill isr to the local one
    VECTOR(HW_IVECDACUF,(int)DAC_Underflow_ISR2);


    HW_ICLENABLE0R.B.SEN0 = 1;      // Enable source 0 (DAC Refill)
    HW_ICLENABLE0R.B.SEN1 = 1;      // Enable source 1 (DAC Overflow)
    HW_ICLPRIOR0R.B.S0P = 0;        // Set DAC Fill IRQ ICOLL priority to 0 (highest)
    HW_ICLPRIOR0R.B.S1P = 0;        // Set DAC Underflow IRQ ICOLL priority to 0 (highest)
    HW_ICLSTEER0R.B.S0S = 0;        // Steer the DAC Fill IRQ to IVL1 
    HW_ICLSTEER0R.B.S1S = 0;        // Steer the DAC Underflow IRQ to IVL1 
    HW_IPR.B.L1P = 3;               // Set Priority of IVL1 to 3 (high) 
    
///////////DAC INIT////////////////////
    // Set up DAC Base Address Reg
    HW_DACBAR.I = (int)DAC_Buffer;
    // Set up DAC Modulo Reg
    HW_DACMR.I=DAC_BUFF_MOD;
    // Set up DAC Word Count Reg
    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;
    // Set up the DAC Sample Rate Reg to 44.1kHz
    HW_DACSRR.I=0x0780df;       
    HW_DACCPR.I=0;
    
    // Set up the DAC Control Register (but don't start sending data yet)
    TransmitZeros = TRUE ;      // transmit zeros until we unmute master vol. 
    HW_DACCSR.B.TXIEN=1;        // Enable interrupts
    HW_DACCSR.B.TXEN=1;         // DAC will start transmitting preloaded values, 
    
    SysDelay(5); //allow filling the DAC buffer with zeros (= Vag). 

    HW_MIXMASTERVR.B.MUTE = 0; //unmute MASTERVR , keep MR and ML at 1F (lowest volume after fade out)
    
    // fade to full scale volume while unmuted
    i=HW_MIXMASTERVR.B.MR;
    while(i-- >0){
      HW_MIXMASTERVR.B.MR=i;
      HW_MIXMASTERVR.B.ML=i;
      SysDelay(1);
    }

    HW_MIXDACINVR.I = 0x0;      // DACIn unmuted, vol = 0

// NOTE: delay required to allow zero crossing to disable inside the chip
    SysDelay(100);              

///////  changing parameter values used inside the DAC Refill ISR //////////////
    HW_DACCSR.B.TXIEN=0;        // Stop the DAC interrupt to change locked globals
    TransmitZeros = FALSE;      
//    HW_GP0DOR.B.B16 = 1;    // temp timing GPIO
    HW_DACCSR.B.TXIEN=1;        // Start again
/////////////////////////////////////////////////////////////////////////////////

    // ramp DAC buffer value up to final value -- ramps headphone output to 0 V
    while(*pDAC_Buffer_Head < FINAL_DAC_VALUE){};    // One line loop
    SysDelay(1);
    
    HW_HPCTRL.I = HW_HPCTRL.I | HW_HPCTRL_POP_SETMASK;     //set anti-pop bits to reduce the residual charge after headphones are powered down.

    HW_ICLENABLE0R.B.SEN0 = 0;  // disable source 0 (DAC Refill)
    HW_ICLENABLE0R.B.SEN1 = 0;  // disnable source 1 (DAC Overflow)
#endif  //End IF/ELSE Capless    
}
 
#if !defined(CAPLESS_HP)
// these will be overwritten when DacDrv is inited/enabled
// DAC Refill ISR (Vector number 30 is located at at P:$60)
_reentrant void _long_interrupt(-1) DAC_Refill_ISR2(void){
    int i;

    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;			// reset DAC word count to prevent underflows

    for (i=0;i<DAC_BUFF_WORDCOUNT;i++)  {
        if(TransmitZeros == TRUE)
            *pDAC_Buffer_Head++ = 0; //Vag = Vdd/2
        else{
            if(*pDAC_Buffer_Head < FINAL_DAC_VALUE)
                (*pDAC_Buffer_Head++) += INITIAL_INCR;
        }
    }
}

// DAC Underflow ISR (Vector 31 is at P:$60)
_reentrant void _long_interrupt(-1) DAC_Underflow_ISR2(void){
    // Clear the Underflow bit
    HW_DACCSR.B.TXEN=0;
    HW_DACWCR.I=DAC_BUFF_WORDCOUNT;
    HW_DACCSR.B.TXEN=1;
}
#endif

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextsysheadphones":
   #pragma endasm
#endif
