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
#include "buttontest.h"
#include "ddildl.h"
#include "sysresourceapi.h"
#include "regsgpio.h"
#include "regsgpflash.h"


//#define MAXLCD_CONT 100

#define DRIVE_TAG_RESOURCE_BIN_RAM 0x10

extern WORD g_wNumDrives;
extern WORD g_wNumMedia;

extern BYTE s_btHandle_PDM;
extern BYTE s_bthandle_SDM;
extern BYTE g_btHandlePGM;
extern BYTE g_btHandleSGMs;
extern BYTE g_btHandleScript00;


////////////////////////////////////////////////////////////////////////////////
//   Globals
////////////////////////////////////////////////////////////////////////////////
_packed BYTE bTEvent[] = "Event:";
_packed BYTE bTwoKeys[] = "Multiple Keys";
_packed BYTE bTRemoteCtl[] = "Remote Keys";
_packed BYTE bTClear[] = "                ";
INT Button_Bitmap[] = {0x00008,0x00008,0x7e7e00,0x7e7e7e,0x00007e};	//the bitmap of the box to be drawn
INT xcoords[] = {0,8,16,24,0,8,16,24,0,8,16,24};	//x coordinates for boxes
INT ycoords[] = {0,0,0,0,8,8,8,8,16,16,16,16};		//y coordinates for boxes
INT xcoordsr[] = {48,56,64,72,80};	//x coordinates for remote control boxes
INT state[] = {	LCD_PRINT_RANGE_ADDR,//0 MENU
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,//5 NEXT
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,//10 EQ
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,//12 play key
				LCD_PRINT_RANGE_ADDR,//13 hold key
				//for remote keys
				LCD_PRINT_RANGE_ADDR,//14 play key 
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR};//18 Vol-

INT g_iEventNumber = 0;
INT g_pCurrentTask = 0;
WORD *Kernel_pTempStack;

INT g_JanusDrive = -1;

/*
Do not enable these stubs since sysloadresource needs the real thing to link properly.
#pragma asm
FEnterNonReentrantSection
FLeaveNonReentrantSection
    rts
#pragma endasm
*/

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

long int  FSGetDWord(void *buffer, int Offset,int MemoryType)
{
    return -1;
}

/////////////////////////////////////////////////////////////////////////////////
//> Name: main
//  Type: Function
//  Description: Button test main -- init and main loop
//  Inputs: None 
//  Outputs: None
//  Notes: This program will run continuously, scanning the button matrix and
//         (scan and non-scan) for keypresses including a multiple keypress 
//<
/////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	INT i = 0;	//index counter for for loop
    
#if defined(DEVICE_3500)

    //; Prefill the entire vector table with defaults per build type. 
    SysDefaultAllVectors();

    // not needed since nandhalinit does this already. Should delete this 8mA block.
#ifdef USE_8MA_GPFLAH_PADS
    // Set 8mA drive for the GPFlash pins
    HW_GP08MA.I = 7;
    HW_GP18MA.I = 7;
    HW_GP28MA.I = 7;
    HW_GP38MA.I = 7;
#endif // USE_8MA_GPFLAH_PADS
#endif // DEVICE_3500


#pragma asm
   include "regsgpio.inc"
   extern Const_ffffff
   move    y:<Const_ffffff,x0      ;Power Up GPIO pads
   move    x0,x:HW_GP0PWR
   move    x0,x:HW_GP1PWR
   move    x0,x:HW_GP2PWR
#pragma endasm


    // Init the media
    for(i = 0 ; i < g_wNumMedia ; i++)
    {
        if(MediaInit(i) != SUCCESS)
        {   SystemHalt();
        }
    }        

    // Discover the Media and its drives
    for(i = 0 ; i < g_wNumMedia ; i++)
    {    
        if(MediaDiscoverAllocation(i) != SUCCESS)
        {   SystemHalt();
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
	DisplayModuleInit(ModuleTableDisplay); // note: crashed in sdk3.100.20 on sysloadresource (display init sequence) not returning. 
	ButtonInit();
	ButtonsOn();

	//prepare all text for printing
	SwizzleText(bTwoKeys);
	SwizzleText(bTEvent);
	SwizzleText(bTRemoteCtl);

//	SetContrast(MAXLCD_CONT);
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

	for(i = 0; i<NUM_KEYS_ON_KEY_PAD;i +=1)
	{		 //post messages to draw the keys on key pad
		SysPostMessage(5,LCD_PRINT_RANGE_ADDR,xcoords[i],ycoords[i],Button_Bitmap);
	}
	SysPostMessage(5,LCD_PRINT_RANGE_ADDR,72,30,Button_Bitmap);// Draw play key
	SysPostMessage(5,LCD_PRINT_RANGE_ADDR,80,30,Button_Bitmap);// Draw hold key

	#if defined(REMOTE2)
		for(i = 0; i<NUM_REMOTE_KEYS;i +=1)
		{		 //post messages to draw the remote keys
			SysPostMessage(5,LCD_PRINT_RANGE_ADDR,xcoordsr[i],8,Button_Bitmap);
		}
	#endif

    RetrieveLcdMessage();

	PrintString(bTEvent, 0, 40);	//Print "Event: " to the screen

	#if defined(REMOTE2)
		PrintString(bTRemoteCtl, 35, 20); // Print "Remote Keys"
	#endif

	
	//main program loop
	while(1){	 //run forever
		
		do{		//wait for a key press
			g_iEventNumber = GetKey();
		}while(g_iEventNumber == NO_BUTTON);

		//print the event number
		PrintNumber(35,40,g_iEventNumber,3,'0');
		PrintString(bTClear, TWO_KEY_X, TWO_KEY_Y);

		if(g_iEventNumber > 13 && g_iEventNumber < 28 ) // Event 14-27 = press and hold 
		{	
			g_iEventNumber -= NUM_KEYS_ON_KEY_PAD+2;//2=play and hold keys
		}

		#if defined(REMOTE2)
		else if( g_iEventNumber > 27 && g_iEventNumber < 33 )// Event 28-32 = remote keys press and release
		{
			g_iEventNumber -= NUM_KEYS_ON_KEY_PAD+2;
		}	
		else if( g_iEventNumber > 32 && g_iEventNumber < 38)// Event 33-37= press and hold keys from remote control
		{
			g_iEventNumber -= NUM_KEYS_ON_KEY_PAD+2+NUM_REMOTE_KEYS;
		}
		else if ( g_iEventNumber == 38)// Two key event number when with remote control
		{
			PrintString(bTwoKeys, TWO_KEY_X, TWO_KEY_Y);//
		}

		if ( g_iEventNumber != 38)// Two key event number when with remote control
		#else
		else if ( g_iEventNumber == 28)// Two key event number when without remote control
		{
			PrintString(bTwoKeys, TWO_KEY_X, TWO_KEY_Y);//
		}
		if ( g_iEventNumber != 28)
		#endif
		
		{
		if(state[g_iEventNumber] == LCD_PRINT_RANGE_ADDR) //Toggle the state of the button bitmap
			state[g_iEventNumber] = LCD_PRINT_RANGE_INV_ADDR;
		else
			state[g_iEventNumber] = LCD_PRINT_RANGE_ADDR;


		if ( g_iEventNumber == 12 ) //play key event
			SysPostMessage(5,state[g_iEventNumber],72,30,Button_Bitmap);
		else if ( g_iEventNumber == 13 ) //hold key event
			SysPostMessage(5,state[g_iEventNumber],80,30,Button_Bitmap);		
		else if ( g_iEventNumber < NUM_KEYS_ON_KEY_PAD )	//keys from key pad	
			SysPostMessage(5,state[g_iEventNumber],xcoords[g_iEventNumber],ycoords[g_iEventNumber],Button_Bitmap); 
		else	// Keys from remote control
			SysPostMessage(5,state[g_iEventNumber],xcoordsr[g_iEventNumber-14],8,Button_Bitmap);
		
			
		RetrieveLcdMessage();
		}
	}
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
		move #>$FFFFFF,a
        ;clr     a

_Done
        jsr     SysAllLinear
    #pragma endasm
}


void PrintString(_packed BYTE* pString, int x, int y)
{
	SysPostMessage(5,LCD_PRINT_STRING_ADDR,x,y,pString);	//print text to LCD
	RetrieveLcdMessage();
}



