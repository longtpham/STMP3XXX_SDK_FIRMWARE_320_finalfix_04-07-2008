////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2002
//
// Filename: buttontest.c
// Description: Test for the Button API
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "messages.h"
#include "hwequ.h"
#include "lcdsupport.h"
#include "fmtunersupport.h"
#include "fmtunertest.h"
#include "ddildl.h"
#include "sysresourceapi.h"
//#define MAXLCD_CONT 100

#define DRIVE_TAG_RESOURCE_BIN_RAM 0x10

extern WORD g_wNumDrives;
extern WORD g_wNumMedia;

extern BYTE s_btHandle_PDM;
extern BYTE s_bthandle_SDM;
extern BYTE g_btHandlePGM;
extern BYTE g_btHandleSGMs;
extern BYTE g_btHandleScript00;

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
WORD g_JanusDrive;

////////////////////////////////////////////////////////////////////////////////
//   Globals
////////////////////////////////////////////////////////////////////////////////
#define MAXLCD_CONT 100
INT SysLowResolutionADCInit;
INT SysLowResolutionAdcReadVolt;
 
//WORD wGlobalDelay = 500000;       //Delay between tests.
WORD wGlobalDelay = 200000;       //Delay between tests.

extern WORD g_wCurrentFrequency;
extern BOOL _X g_bSearchEndAtBandLimit;
extern WORD * MenuHeadPtr;
extern WORD * MenuTailPtr ;

WORD wStations[] = {88700, 89500, 90500, 91700, 93700, 94700, 95500, 96700, 98100, 100700, 101500, 102300, 105900, 107100, 87500};
#define ALL_STATIONS sizeof(wStations)

_packed BYTE bStartTest[] = "FMTuner Testing";
_packed BYTE bWriting[] = "Writing...";
_packed BYTE bTest1[] = "Tuning test";
_packed BYTE bTest2[] = "Searching up";
_packed BYTE bTest3[] = "Searching Down";
_packed BYTE bTest4[] = "Manual up";
_packed BYTE bTest5[] = "Manual down";
_packed BYTE bTest6[] = "Stereo Pilot";
_packed BYTE bTest7[] = "Scanning...";
_packed BYTE bTest8[] = "Presets...";
_packed BYTE bTest9[] = "Buttons...";
INT i;
//BOOL g_bWriteBmngr512Bytes=TRUE; // unused stub 

 
void _reentrant RetrieveFmtMsg(void);
WORD _reentrant GetKey(void);


void _reentrant RetrieveMenuMsg(void);
extern _reentrant INT TunerGetStatus(void);

INT g_pCurrentTask = 0;
WORD *Kernel_pTempStack;

void main(void)
{
	WORD keypress = 0;
	
	INT i = 0;	//index counter for for loop
    // Init the media
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
	
	SysResetAllInterrupts();
	SysResetAllTimers();
	SysTimeInit();

    DisplayModuleInit(ModuleTableDisplay);
	ButtonInit();
	ButtonsOn();

	FMTAnalogInit();
	TunerModuleInit(TunerModuleEntry);

    SysPostMessage(2,TUNER_POWER_ON);
	RetrieveFmtMsg();

	SwizzleText(bStartTest);
	SwizzleText(bWriting);
	SwizzleText(bTest1);
	SwizzleText(bTest2);
	SwizzleText(bTest3);
	SwizzleText(bTest4);
	SwizzleText(bTest5);
	SwizzleText(bTest6);
	SwizzleText(bTest7);
	SwizzleText(bTest8);
	SwizzleText(bTest9);
  
	// Clear Display
	LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

    PrintStringAddr(10, 10, bStartTest);
	
	// tune to the set station
	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest1);
	SysPostMessage(3, TUNER_TUNE_FREQUENCY, 95500); 
	RetrieveFmtMsg();
	Delay(wGlobalDelay);

	for (i=0; i< ALL_STATIONS; i++)
	{
		SysPostMessage(3, TUNER_TUNE_FREQUENCY, wStations[i]); 
		RetrieveFmtMsg();
		Delay(wGlobalDelay);
	}

	SysPostMessage(3, TUNER_SET_SENSITIVITY, 30); 
	RetrieveFmtMsg();

	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest2);
	i=0;
	while(i++ < 5){
		SysPostMessage(2, TUNER_TUNE_SEARCH_UP);
		RetrieveFmtMsg();
		Delay(wGlobalDelay);
	}

	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest3);
	i=0;
	while(i++ < 5){
		SysPostMessage(2, TUNER_TUNE_SEARCH_DOWN);
		RetrieveFmtMsg();
		Delay(wGlobalDelay);
	}

	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest6);
	// tuner mono
	SysPostMessage(3, TUNER_FORCED_MONO, 1);
	RetrieveFmtMsg();

	// tuner stereo
	SysPostMessage(3, TUNER_FORCED_MONO, 0);
	RetrieveFmtMsg();

	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest4);
	i=0;
	while(i++<10){
		SysPostMessage(2, TUNER_TUNE_MANUAL_UP);
		RetrieveFmtMsg();
		Delay(1000);
	}
	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest5);
	i=0;
	while(i++<10){
		SysPostMessage(2, TUNER_TUNE_MANUAL_DOWN);
		RetrieveFmtMsg();
		Delay(1000);
	}
	
	SysPostMessage(2, TUNER_RESET);
	RetrieveFmtMsg();

	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest7);
// Test the station preset
	SysPostMessage(2, TUNER_PRESET_STATION);
	RetrieveFmtMsg();
	Delay(wGlobalDelay);

	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest8);
	SysPostMessage(3, TUNER_TUNE_TO_PRESET, 1);
	RetrieveFmtMsg();
	Delay(wGlobalDelay);

	SysPostMessage(3, TUNER_TUNE_TO_PRESET, 2);
	RetrieveFmtMsg();
	Delay(wGlobalDelay);

	SysPostMessage(3, TUNER_TUNE_TO_PRESET, 3);
	RetrieveFmtMsg();
	Delay(wGlobalDelay);

	SysPostMessage(3, TUNER_TUNE_TO_PRESET, 4);
	RetrieveFmtMsg();
	Delay(wGlobalDelay);

// Buttons control with wraparound searching
	g_bSearchEndAtBandLimit = FALSE;
	LcdClear(0, 20, LCD_X_PIXELS, 30);
    PrintStringAddr(10, 20, bTest9);
	SysPostMessage(3, TUNER_TUNE_FREQUENCY, 102300); 
	RetrieveFmtMsg();

	while(1){
		do{		//wait for a keypress
			keypress = GetKey();
		}while(keypress == 0);

		if(keypress == 0xC){
			// Search channel up
			SysPostMessage(2, TUNER_TUNE_SEARCH_UP);
			RetrieveFmtMsg();
		}
		else if(keypress == 0x6){
			// Search channel down
			SysPostMessage(2, TUNER_TUNE_SEARCH_DOWN);
			RetrieveFmtMsg();
		}
		else if(keypress == 0xA){
			// Tune Manual up
			SysPostMessage(2, TUNER_TUNE_MANUAL_UP);
			RetrieveFmtMsg();
		}
		else if(keypress == 0x4){
			// Tune Manual down
			SysPostMessage(2, TUNER_TUNE_MANUAL_DOWN);
			RetrieveFmtMsg();
		} 
		// Rest of the buttons are unused
		// More button functionality can be added

	}
}

void _reentrant RetrieveFmtMsg(void)
{
    INT error= 0;
	_packed BYTE bDecimal[] = ".";
    #pragma asm
        nolist
        extern  Fglb_TunerQueueDescriptor
        extern  FTunerModuleProcessQueue
        list
        
        move    #Fglb_TunerQueueDescriptor,r0
        move    #-1,m0
        jsr     FTunerModuleProcessQueue
    #pragma endasm

	while (g_wFMTunerError.m_bTUNER_BUSY || g_wFMTunerError.m_bSCANNING_STATION)
	{
		#pragma asm
			nolist
			extern  Fglb_TunerQueueDescriptor
			extern  FTunerModuleProcessQueue
			list
        
			move    #Fglb_TunerQueueDescriptor,r0
			move    #-1,m0
			jsr     FTunerModuleProcessQueue
		#pragma endasm
	}
	
	RetrieveMenuMsg();
	Delay(5000);
	error =	TunerGetStatus();

	LcdClear(0, 30, LCD_X_PIXELS, LCD_Y_PIXELS);
	PrintNumber(20, 35, (g_wCurrentFrequency+50)/1000,3,' ');
    PrintStringAddr(35, 35, bDecimal);
	PrintNumber(40, 35, ((g_wCurrentFrequency+50)%1000)/100,1,' '); 
} 


void _reentrant RetrieveMenuMsg(void)
{
	// We are not interested in processing menu messages.
	// The menu queue is included because it is used by 5767. We just prevent the menu
	// queue from overflowing in this way.
	MenuHeadPtr = MenuTailPtr;

}

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
