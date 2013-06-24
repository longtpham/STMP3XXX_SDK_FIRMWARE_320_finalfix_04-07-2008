////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2003
//
// Filename: lradctest.c
// Description: STMP3 Low Resolution Analog to Digital Converter Test Program
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "hwequ.h"
#include "lradctest.h"
#include "messages.h"
#include "lcdsupport.h"
#include "lowresolutionadc.h"
#include "mediadetection.h"
#include "ddildl.h"
#include "sysresourceapi.h"
#include "regslradc.h"

//
// These are required as stubs by the libraries.
//
void EnterNonReentrantSection(void)
{
}
void LeaveNonReentrantSection(void)
{
}
//
// TODO why is this needed?
//
int g_JanusDrive;

extern WORD g_wNumDrives;
extern WORD g_wNumMedia;

extern BYTE s_btHandle_PDM;
extern BYTE s_bthandle_SDM;
extern BYTE g_btHandlePGM;
extern BYTE g_btHandleSGMs;
extern BYTE g_btHandleScript00;

#define DRIVE_TAG_RESOURCE_BIN_RAM 0x10

_packed BYTE bTitle[]           = "LRADC TEST";
_packed BYTE bAdcAuxText[]      = "Aux (mV):";
_packed BYTE bAdcBattText[]     = "Batt (mV):";
_packed BYTE bAdcAuxAvgText[]   = "Aux Avg(mV):";
_packed BYTE bAdcBattAvgText[]  = "Batt Avg(mV):";
_packed BYTE bMessageLine1[]    = "Connect Voltage";
_packed BYTE bMessageLine2[]    = "to AUX line";
_packed BYTE bMessagePressKey[] = "* Press key *";
_packed BYTE bMessageRefTitle[] = "Ref:";

#ifdef DEVICE_3500
_packed BYTE bRef0[]         = "2700 MV";
_packed BYTE bRef1[]         = "2600 MV";
_packed BYTE bRef2[]         = "2560 MV";
_packed BYTE bRef3[]         = "VDDIO";
_packed BYTE bRef4[]         = "5400 MV";
_packed BYTE bRef5[]         = "5200 MV";
_packed BYTE bRef6[]         = "5120 MV";
_packed BYTE bRef7[]         = "VDDIOx2";

#else
_packed BYTE bRef0[]         = "2800 MV";
_packed BYTE bRef1[]         = "2900 MV";
_packed BYTE bRef2[]         = "2700 MV";
_packed BYTE bRef3[]         = "2780 MV";
_packed BYTE bRef4[]         = "VDDIO";
_packed BYTE bRef5[]         = "";
_packed BYTE bRef6[]         = "";
_packed BYTE bRef7[]         = "";

#endif

// Need this variable for syshwinit.asm which uses a variable 
// from WMA library which fattest clearly does not need


void *g_pCurrentTask = NULL;
WORD *Kernel_pTempStack;

// sp init during startup.asm (Exec)
void main(void)
{
    INT i;
    WORD wKey = 0;
    INT iAdcBattReading, iAdcAuxReading;
    INT iAdcBattAvgReading, iAdcAuxAvgReading;
    enum LowResADCRef eReference=REF_0; 
    enum LowResADCHalfPwr ePower=OFF;    
      
    for(i = 0 ; i < g_wNumMedia ; i++)
    {
        if(MediaInit(i) != SUCCESS)
        {
            SystemHalt();
        }
    }        

    // Discover the Media and its drives
    for(i = 0 ; i < g_wNumMedia ; i++)
    {    
        if(MediaDiscoverAllocation(i) != SUCCESS)
        {
            SystemHalt();
        }
    }        


    // Init All Drives
    for(i=0;i<g_wNumDrives;i++)
    {
        if(DriveInit(i) != SUCCESS)
        {
        }
    }

    SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN_RAM);


    #pragma asm
        include "sysspeed.inc"
        extern  SysResetAllTimers
        extern  SysResetAllInterrupts
        extern  SysTimeInit
        extern  ButtonInit
        extern  ButtonsOn
        extern  PowerInit
        extern  AnalogInit
        extern  SysSpeedForce
        extern  SysAllLinear
               
        ; dummy for SysHwInit.asm
        global  MixerMasterVol


MixerMasterVol dc 0

        jsr     PowerInit
        jsr     SysResetAllTimers
        jsr     SysResetAllInterrupts
        jsr     SysTimeInit

        ; Init the buttons
        jsr     ButtonInit
    
        ; turn on the buttons
        jsr     ButtonsOn

        jsr     AnalogInit

        move    #>SPEED_MP3,x1
        jsr     SysSpeedForce
    
        jsr     SysAllLinear

    #pragma endasm

    DisplayModuleInit(ModuleTableDisplay);
    Delay(200);

    SwizzleText(bTitle);
    SwizzleText(bAdcAuxText);
    SwizzleText(bAdcBattText);
    SwizzleText(bAdcAuxAvgText);
    SwizzleText(bAdcBattAvgText);
    SwizzleText(bMessageRefTitle);
	
	SwizzleText(bRef0);
	SwizzleText(bRef1);
	SwizzleText(bRef2);
	SwizzleText(bRef3);
	SwizzleText(bRef4);
	SwizzleText(bRef5);
	SwizzleText(bRef6);
	SwizzleText(bRef7);    

    SwizzleText(bMessageLine1);
    SwizzleText(bMessageLine2);
    SwizzleText(bMessagePressKey);
    
    // Display Test Title
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(20, 0, bTitle);
    PrintStringAddr(2,16,bMessageLine1);
    PrintStringAddr(2,24,bMessageLine2);
    PrintStringAddr(2,40,bMessagePressKey);

    do
    {
        wKey = GetKey();
    }while(wKey == 0);                     

    LcdClear(0, 8, LCD_X_PIXELS, LCD_Y_PIXELS-8);
    PrintStringAddr(2,40,bMessagePressKey);

    // Display Aux Entry
    PrintStringAddr(2,8,bAdcAuxText);
             
    // Display Aux Avg Entry
    PrintStringAddr(2,16,bAdcAuxAvgText);
             
    // Display Batt Entry
    PrintStringAddr(2,24,bAdcBattText);

    // Display Batt Avg Entry
    PrintStringAddr(2,32,bAdcBattAvgText);

    LcdClear(0, 5*8, LCD_X_PIXELS, 8);

    SysLowResolutionADCInit(BATT, OFF, eReference);     // Init LRADC Channel Battery 
    SysLowResolutionADCInit(AUX, OFF, eReference);      // Init LRADC Channel Auxiliary
    
    // Display Press Key
    PrintStringAddr(2,56,bMessagePressKey);

    // Display Ref Voltage
    PrintStringAddr(2,48,bMessageRefTitle);
    PrintStringAddr(30,48,bRef0);

    do
    {
        iAdcBattReading = SysLowResolutionAdcReadVolt(BATT,INSTANT_RESULT);         
        iAdcAuxReading = SysLowResolutionAdcReadVolt(AUX,INSTANT_RESULT);
        iAdcBattAvgReading = SysLowResolutionAdcReadVolt(BATT,AVERAGE_RESULT);         
        iAdcAuxAvgReading = SysLowResolutionAdcReadVolt(AUX,AVERAGE_RESULT);

        // clear out the values
        LcdClear(ADC_VALUE_X_COORD, 1*8, LCD_X_PIXELS-ADC_VALUE_X_COORD, 4*8);

        // Display Aux Value
        PrintNumber(ADC_VALUE_X_COORD, 8, (WORD)iAdcAuxReading, 4, '0');
            
        // Display Aux Avg Value
        PrintNumber(ADC_VALUE_X_COORD, 16, (WORD)iAdcAuxAvgReading, 4, '0');
                 
        // Display Batt Value
        PrintNumber(ADC_VALUE_X_COORD, 24, (WORD)iAdcBattReading, 4, '0');
        
        // Display Batt Avg Value
        PrintNumber(ADC_VALUE_X_COORD, 32, (WORD)iAdcBattAvgReading, 4, '0');
        
        wKey = GetKey();
        
        if(wKey != 0)
        {

            if(wKey != PLAY)
            {
                switch(eReference)
                {
                    case REF_0:
                        eReference = REF_1;
                        PrintStringAddr(30,48,bRef1);
                        break;
                         
                    case REF_1:
                        eReference = REF_2;
                        PrintStringAddr(30,48,bRef2);
                        break;

                    case REF_2:
                        eReference = REF_3;
                        PrintStringAddr(30,48,bRef3);
                        break;

                    case REF_3:
                        eReference = REF_4;
                        PrintStringAddr(30,48,bRef4);
                        break;

					#ifdef DEVICE_3500
                    case REF_4:
                        eReference = REF_5;
                        PrintStringAddr(30,48,bRef5);
                        break;

						case REF_5:
						eReference = REF_6;
                        PrintStringAddr(30,48,bRef6);
                        break;

						case REF_6:
						eReference = REF_7;
                        PrintStringAddr(30,48,bRef7);
                        break;

						case REF_7:
						eReference = REF_0;
                        PrintStringAddr(30,48,bRef0);
                        break;

					#else
                    case REF_4:
                        eReference = REF_0;
                        PrintStringAddr(30,48,bRef0);
                        break;
					
					#endif
                    default:
						eReference = REF_0;
						PrintStringAddr(30,48,bRef0);
                        break;                    
                }
            }

            else
            {
                if(ePower == ON)
                    ePower = OFF;
                else
                    ePower = ON;
            }                    
                            
            SysLowResolutionADCInit(BATT, ePower, eReference);     // Init LRADC Channel Battery 
            SysLowResolutionADCInit(AUX, ePower, eReference);      // Init LRADC Channel Auxiliary

        }

        Delay(5000);

    }while(1);
    
}


// dummy routine for syshwinit functions
void _reentrant PowerUpHeadPhones(void){}


///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           GetKey
//
//   Type:           Function
//
//   Description:    
//                   
//
//   Inputs:         none
//
//   Outputs:        none
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
WORD _reentrant GetKey(void)
{
    #pragma asm
        extern SysGetButton
        extern SysAllLinear

        jsr     SysGetButton
        
        jcc     _NoButton

        clr     a
        move    x0,a
        jmp     _Done
        
_NoButton
        clr     a

_Done
        jsr     SysAllLinear
    #pragma endasm
}    

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysMaskAllInterrupts
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       WORD            Old interrupt level bits
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
WORD _reentrant SysMaskAllInterrupts(void)
{

    #pragma asm
        clr     a
        clr     b   #>$000300,x0

        move    sr,a1               ; get the old value

        move    sr,b1
        or      x0,b
        move    b1,sr               ; mask interrupts

        and     x0,a                ; mask off for the return
        nop
    #pragma endasm

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SysUnMaskAllInterrupts
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        WORD            Restore interrupt level bits
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant SysUnMaskAllInterrupts(WORD wLevel)
{

    #pragma asm
        clr     b   #>$FFFCFF,x0

        move    sr,b1               ; get the current sr
        and     x0,b                ; mask off current bits

        move    b1,x0
        or      x0,a                ; mask in the restore bits
        move    a1,sr               ; make it so

        nop
        nop
    #pragma endasm

}
