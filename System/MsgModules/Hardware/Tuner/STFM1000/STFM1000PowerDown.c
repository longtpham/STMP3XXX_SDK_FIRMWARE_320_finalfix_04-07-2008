////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2005
//
// Filename: STFM1000PowerDown.c
// Description: 
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

#ifdef FUNCLET
 #pragma asm
    extern SysCallFunclet
 #pragma endasm
#endif

_reentrant void STFM1000I2CConfigureRegister(WORD I2C_Sub_Addr, DWORD DataOut);
_reentrant INT STFM1000I2CReset(WORD mode, WORD ClockDiv);
_reentrant INT STFM1000I2CWriteTunerRegister(WORD I2C_Sub_Addr, DWORD DataOut);
extern _reentrant INT SysGetSpeed(void);

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}


#ifdef FUNCLET
    #pragma asm
        org p,".stfm1000_SYS":
        global FSTFM1000TunerSafePowerDown        
        FSTFM1000TunerSafePowerDown:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet2 RSRC_FUNCLET_TUNER_SAFE_POWER_DOWN

        org p,"SYSFUNCLET_TUNER_SAFE_POWER_DOWN_P":
        dc      RSRC_FUNCLET_TUNER_SAFE_POWER_DOWN
    #pragma endasm
_reentrant INT STFM1000TunerSafePowerDown_Funclet(void)
#else
_reentrant INT STFM1000TunerSafePowerDown(void)
#endif
{
    /***************************************************************************************************
     *  We have to disable the Steely Dan to pull down the I2S pins in order to reboot the system
     *  when detected the USB connected. (The defect stmp9124)
     *  The following should be the values of registers which should ensure that the I2S pads are in hi-z.
     *  pad_pd10ktst_oeb[0] = 1 (in initialization6 reg)
     *  pad_pd10ktst_oeb[1] = 1(in initialization6 reg)
     *  tst_clk_out = 0(in FM datapath reg)
     *  clk1_reg[15] = 0(in clk1 reg)
     *  clk1_reg[19] = 0(in clk1 reg)
     *  Then we set Steely Dan as low power mode 
     ***************************************************************************************************/

    STFM1000I2CConfigureRegister(INITIALIZATION6_REG_ADDR,  //0x24,          // Init6,
                                 0xcbfe0000);   // 0x0000fecb);
    
    STFM1000I2CConfigureRegister(DATAPATH_REG_ADDR, //0x5c,          // datapath,
                                 0x10020100);   // 0x00010210
    
    STFM1000I2CConfigureRegister(REF_REG_ADDR,  //0x28,          // REF_REG_ADDR,
                                 0x00000000);          
    
    STFM1000I2CConfigureRegister(LNA_REG_ADDR,  //0x2C,          // LNA_REG_ADDR,
                                 0x00000000);   
        
    STFM1000I2CConfigureRegister(MIXFILT_REG_ADDR,  //0x30,          // MIXFILT_REG_ADDR,
                                 0x00000000); 
  
    STFM1000I2CConfigureRegister(CLK1_REG_ADDR, //0x34,          // CLK1_REG_ADDR,
                                 0x00000003); //0x00004003);   // Set bit 22, 24,25 to reduce off state current by 200uA. sdk 2.610 release val was 0x0 but this is enhanced.
  
    STFM1000I2CConfigureRegister(CLK2_REG_ADDR, //0x38,          // CLK2_REG_ADDR,
                                 0x00000000);   
        
    STFM1000I2CConfigureRegister(ADC_REG_ADDR,  //0x3C,          // ADC_REG_ADDR,
                                 0x00000000);   
    return (0);                                           
}

_reentrant void STFM1000I2CConfigureRegister(WORD I2C_Sub_Addr, DWORD DataOut)
{
    INT RetCode; 
    INT wTriesCount = 0;
    do{
     
        RetCode = STFM1000I2CWriteTunerRegister(I2C_Sub_Addr,// Register addr
                                                DataOut      // Data to set the register
                                                );
       if(0 != RetCode)
       {
           RetCode = STFM1000I2CReset(0,0);
       }
       else
       { 
            break;
       }
    }while(++wTriesCount < MAX_I2C_WRITE_TRIES);
    if(RetCode != 0)
    {
       SystemHalt();
    }
    
}    

_reentrant INT STFM1000I2CWriteTunerRegister(WORD I2C_Sub_Addr, DWORD DataOut)
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

_reentrant INT STFM1000I2CReset(WORD mode, WORD ClockDiv)
{
    int Speed;
    int cnt;
    int iI2CWord3;
    int RtnCode = 0;
    
    HW_GP0ENR.B.B16 = 0;        // enable output
    HW_GP0ENR.B.B17 = 0;        // enable output

    HW_GP0PWR.B.B16 = 1;        // power on
    HW_GP0PWR.B.B17 = 1;        // power on
     
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
        if (TIMEOUT_COUNT <= cnt) RtnCode = -3;	//return(TIMEOUT_ERROR);
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
        Speed = SysGetSpeed();
        switch(Speed)
        {
            case SPEED_IDLE: ClockDiv = I2C_IDLE_SPEED;          	break;  
            case SPEED_FM:   ClockDiv = I2C_FM_SPEED;               break; 
            case SPEED_FM_EQ:   ClockDiv = I2C_FM_EQ_SPEED;         break;         
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


