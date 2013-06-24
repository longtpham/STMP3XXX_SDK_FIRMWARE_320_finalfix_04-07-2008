////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2005
//
// Filename: STFM1000TestPresence.c
// Description: Test for STFM tuner chip present
//
// Note: I2c Read function not present. See Player code I2c.c for Read function.
////////////////////////////////////////////////////////////////////////////////
#include "hwequ.h"
#include "messages.h"
#include "exec.h"
#include "resource.h"
#include "project.h"
#include "gettime.h"
#include "types.h"
#include "..\tunerdriver.h"
#include "stfm1000.h"
#include "hwequ.h"

#ifdef PLAYER
#include "sysspeed.h"
#endif
void _reentrant SysDelayMs(WORD wDelay); // def in usbmscmisc.c

#ifdef FUNCLET
 #pragma asm
    extern SysCallFunclet
 #pragma endasm
#endif

extern BOOL g_bLimitedVDDD;

//_reentrant INT I2CReset(WORD mode, WORD ClockDiv);  //delete line later.  prototypes are moved to an h file now
//INT I2CWriteTunerRegister(WORD I2C_Sub_Addr, DWORD DataOut); //delete line later.  prototypes are moved to an h file now

extern _reentrant INT SysGetSpeed(void);
//void STFM1000ReduceCurrentXTALAmp(void); //delete line later.  prototypes are moved to an h file now

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}

// Inputs:  none
// Outputs: 
// Notes:   
//
#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        global FSTFM1000TestPresence        
               FSTFM1000TestPresence:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_STFM1000_TEST_PRESENCE

        org p,"SYSFUNCLET_STFM1000_TEST_PRESENCE_P":
        dc      RSRC_FUNCLET_STFM1000_TEST_PRESENCE
    #pragma endasm
_reentrant int STFM1000TestPresence_Funclet(void)
#else
_reentrant int STFM1000TestPresence(void)
#endif
{
extern 	INT  gTUNER_STFM1000;
	INT    RetCode;
   #if defined(USBMSC_BUILD) || defined(MTP_BUILD)   
    // player worked without this since i2s already sets this pin to logic 0 so I only do it in usbmsc.
    HW_GP1PWR.B.B15 = 1; // enable power for gpio pin to fet which provides stfm1000 rail
    HW_GP1ENR.B.B15 = 1; // set to assigned function (gpio) 
    HW_GP1DOER.B.B15 = 1; // set to output pin direction
    HW_GP1DOR.B.B15 = 0; // data out reg. set it low to power on FET & stfm1000 so stfm i2c responds to probe (required for revH evk). 
    SysDelayMs(20); // wait for STFM1000 newly powered up rail to stabilize.
   #endif 

    // Set to 100kHz for I2C
    RetCode =I2CReset(0, 0);
   
   #if defined(USBMSC_BUILD) || defined(MTP_BUILD)   
    // Reduce the current to crystal amp (CQ STMP9780)
    //STFM1000ReduceCurrentXTALAmp();
    // Moved this here so it can be part of the funclet.  Could have been inlined.
    // Dummy write to test for STFM1000 being present  
//3200Merge+: remove the 2 lines
//    I2CWriteTunerRegister(CLK1_REG_ADDR,         // 0x34 CLK1_REG_ADDR,
//                      0x00000003);  // 0x03000000; XTALAMP_SAVEPOWER_BIT0 = 1, XTALAMP_SAVEPOWER_BIT1 = 1
//3200Merge-
    #endif

        RetCode = I2CWriteTunerRegister(INITIALIZATION6_REG_ADDR,          // 0x20 Init5, 0x24 Init6
                                 		0xcbfe0000);   // 0x0000fecb);
   #if defined(USBMSC_BUILD) || defined(MTP_BUILD)   
    HW_GP1DOR.B.B15 = 1; // data out reg. set it high to POWER OFF FET & stfm1000 so stfm i2c to save power.
   #endif

	if(0 == RetCode)			// STFM1000 present
	{   gTUNER_STFM1000 = 1;
	}
	else					// STFM1000 not present
	{   gTUNER_STFM1000 = 0;
	}
    
	return(0);
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000powerdown":
    #pragma endasm
#endif

// Inputs: 
// Outputs:
// Notes:  
//
#ifdef FUNCLET
    #pragma asm
         org p,".stfm1000_SYS":
;        global FI2CWriteTunerRegister        
               FI2CWriteTunerRegister:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_I2C_WRITE_TUNER_REGISTER

        org p,"SYSFUNCLET_I2C_WRITE_TUNER_REGISTER_P":
        dc      RSRC_FUNCLET_I2C_WRITE_TUNER_REGISTER
    #pragma endasm
_reentrant INT I2CWriteTunerRegister_Funclet(WORD I2C_Sub_Addr, DWORD DataOut) 
#else
_reentrant INT I2CWriteTunerRegister(WORD I2C_Sub_Addr, DWORD DataOut)
#endif
{        
    DWORD_type MyData;
    i2cdata_type i2cdata;       
    INT cnt;
    MyData.D = DataOut;
    for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++) 
    {    
        //SysWaitOnEvent(0,0,0);       // let kernel spin once
        if (TIMEOUT_COUNT <= cnt) return(-1);
    } 

    i2cdata.B.BYTE0 = 0xC0;     
    i2cdata.B.BYTE1 = I2C_Sub_Addr;  
    i2cdata.B.BYTE2 = MyData.U.W0;  // First byte to write (in LSB of word) 
    HW_I2CDAT.U = i2cdata.U; 
       
    HW_I2CCSR.B.TREQ = 1;       // Master transaction request - generate start condition    
    HW_I2CCSR.B.TREQ = 0;       // and transmit Slave Device Write addres
    
    for(cnt = 0; (1 != HW_I2CCSR.B.TUFL); cnt++)                                  
    {    
        //SysWaitOnEvent(0,0,0);       // let kernel spin once
        if (TIMEOUT_COUNT <= cnt) return(-1);
    }
    
    if(HW_I2CCSR.B.ACKF) return(-2); 
    
    HW_I2CCSR.B.WL = 1;   // Word Length 1, 2, or 3 bytes
    HW_I2CCSR.B.LWORD = 1;          // Send the stop command 

    HW_I2CDAT.U = MyData.U.W1;         // Send last 3 bytes
      
    HW_I2CCSR.B.TUFLCL = 1;         // Clear the Receiver Overflow bit   
    HW_I2CCSR.B.TUFLCL = 0;      
     
    if(HW_I2CCSR.B.ACKF)  return(-2); 
 
    return(0);
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000powerdown":
    #pragma endasm
#endif


//
// Inputs:  
// Outputs: 
// Notes:
#ifdef FUNCLET
    #pragma asm
         org p,".stfm1000_SYS":
;        global FI2CReset        
               FI2CReset:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2  RSRC_FUNCLET_I2C_RESET

        org p,"SYSFUNCLET_I2C_RESET_P":
        dc      RSRC_FUNCLET_I2C_RESET
    #pragma endasm
_reentrant I2CReset_Funclet(WORD mode, WORD ClockDiv)
#else
_reentrant I2CReset(WORD mode, WORD ClockDiv)
#endif   
{
    int Speed;
    int cnt;
    int iI2CWord3;
    int RtnCode=0;
    
    HW_GP0ENR.B.B16 = 0;        // enable output    i2c serial clock enabled  (0 configs as assigned function)
    HW_GP0ENR.B.B17 = 0;        // enable output    i2c serial data line enabled

    HW_GP0PWR.B.B16 = 1;        // power on         i2c serial clock
    HW_GP0PWR.B.B17 = 1;        // power on         i2c serial data line

#if 1
    for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++)    // Delay time 1 cnt
    {    
        if (TIMEOUT_COUNT <= cnt)
        {
            HW_I2CCSR.I = 0;	// Reset I2C device
            HW_I2CCSR.B.TREQ = 1;
            HW_I2CCSR.B.TREQ = 0;
            HW_I2CCSR.B.I2C_EN = 1;
            HW_I2CCSR.B.ACKF = 0;               
            break;
        }
    } 
#else
    for(cnt = 0; (0 != HW_I2CCSR.B.BUSY); cnt++)    // Delay time 1 cnt
    {    
        if (TIMEOUT_COUNT <= cnt) RtnCode = -3;   //return(TIMEOUT_ERROR);
    } 
#endif
    
    for(cnt = 0; cnt < 100; cnt++) { _nop(); }  // Short delay  
 
    HW_I2CCSR.I = 0;	// Reset I2C device
  	
    iI2CWord3 = HW_I2CCSR.I;	// verify that reset worked
    
    if ( 0 != (RESET_HW_I2CCSR_MASK & iI2CWord3) )
    {
      	 RtnCode = -1;
    }       
 
    HW_I2CCSR.B.I2C_EN = 1;	 	// Enable I2C device
    
    if (0 == ClockDiv)         // lookup Clock Div based on System Clock Speed
    {
    
#ifdef PLAYER
        Speed = SysGetSpeed();
        switch(Speed)
        {
            case SPEED_IDLE: ClockDiv = I2C_IDLE_SPEED;          	break;            
            case SPEED_MP3: ClockDiv = I2C_MP3_SPEED;           	break;
            case SPEED_ADPCM: ClockDiv = I2C_ADPCM_SPEED;         	break;
            case SPEED_MIXER: ClockDiv = I2C_MIXER_SPEED;         	break;
            case SPEED_ADCBASE: ClockDiv = I2C_ADCBASE_SPEED;       break;
            case SPEED_MAX: ClockDiv = I2C_MAX_SPEED;           	break;
            case SPEED_WMA: ClockDiv = I2C_WMA_SPEED;           	break;
            case SPEED_MP3ENCODE: ClockDiv = I2C_MP3ENCODE_SPEED;   break;
            case SPEED_PEAK: 
            default: ClockDiv = I2C_PEAK_SPEED;         			break;  
        } 
#else  // USBMSC mode
		if ( TRUE == g_bLimitedVDDD )
		{
			ClockDiv = I2C_MAX_SPEED; 		// STMP3502 0r STMP3503 Clock is 48MHz
		}
		else
		{
			ClockDiv = I2C_USBMSC_SPEED; 	// For USBMSC: fDCLK is fixed at 68.57 Mhz
		}
#endif        
    }  
    
    //HW_I2CDIV.I = (ClockDiv<<1) & 0x0001FE;   // Clock Divider Register (e.g. 0x78) Write Only 
    HW_I2CDIV.B.FACT = ClockDiv;                // Cannot be read, we need to set bit[8:1]
                                                
    HW_I2CCSR.B.MODE = mode;    // Operating Mode Bit  1=Fast

    if (1 == HW_I2CCSR.B.ROFL) 	// Clear Receiver Overflow
    {
    	cnt = HW_I2CDAT.U;
    	HW_I2CCSR.B.ROFLCL = 1;
    	HW_I2CCSR.B.ROFLCL = 0;
    }
    
    if (1 == HW_I2CCSR.B.TUFL) 	// Clear Transmitter Undererflow
    {
    	HW_I2CDAT.U = 0;
    	HW_I2CCSR.B.TUFLCL = 1;
    	HW_I2CCSR.B.TUFLCL = 0;
    }
  
    return(RtnCode);
}
#ifdef FUNCLET
    #pragma asm
        org p,".ptextstfm1000powerdown":
    #pragma endasm
#endif

/////////////////////////////////////////////////////////////////////////////
// Inputs       : None
// Outputs      : None
// Description  : Writes to the STFM1000 CLK1_REG_ADDR register.  Sets
//                bits XTALAMP_SAVEPOWER_BIT0 and XTALAMP_SAVEPOWER_BIT1
//                to reduce USB jitter (CQ STMP9780).  
/////////////////////////////////////////////////////////////////////////////
// body of func moved to single caller. Could have been inline instead
#if 0
void STFM1000ReduceCurrentXTALAmp(void)
{
    I2CWriteTunerRegister(0x34,         // CLK1_REG_ADDR,
                          0x00000003);  // 0x03000000; XTALAMP_SAVEPOWER_BIT0 = 1, XTALAMP_SAVEPOWER_BIT1 = 1 
}
#endif




