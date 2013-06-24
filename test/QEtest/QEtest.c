////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2002
//
// Filename: QEtest.c
// Description: Test for the Quadrature encoder or rotary encoder API
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "messages.h"
#include "hwequ.h"
#include "lcdsupport.h"
#include "QEtest.h"
#include "rotaryencoder.h"
#include "ddildl.h"
#include "sysresourceapi.h"

extern WORD g_wNumDrives;
extern WORD g_wNumMedia;

extern BYTE s_btHandle_PDM;
extern BYTE s_bthandle_SDM;
extern BYTE g_btHandlePGM;
extern BYTE g_btHandleSGMs;
extern BYTE g_btHandleScript00;

#define DRIVE_TAG_RESOURCE_BIN_RAM 0x10

#define MAXLCD_CONT 100

////////////////////////////////////////////////////////////////////////////////
//   Globals
////////////////////////////////////////////////////////////////////////////////
_packed BYTE bText[] = "Event:";
_packed BYTE bTwoKeys[] = "Multiple Keys";
_packed BYTE bQEData[] = "Rotary Encoder: ";

INT Button_Bitmap[] = {0x00008,0x00008,0x7e7e00,0x7e7e7e,0x00007e};	//the bitmap of the box to be drawn
INT xcoords[] = {1,9,17,25,1,9,17,25,1,9,17,25};	//x coordinates for boxes
INT ycoords[] = {1,1,1,1,9,9,9,9,17,17,17,17};		//y coordinates for boxes
INT state[] = {	LCD_PRINT_RANGE_ADDR,	//default to regular printing...no inverting
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR,
				LCD_PRINT_RANGE_ADDR};
INT keypress = 0;
INT g_pCurrentTask = 0;
WORD *Kernel_pTempStack;

#pragma asm
FEnterNonReentrantSection
FLeaveNonReentrantSection
    rts
#pragma endasm


INT g_JanusDrive = -1;

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
	WORD wQE_data;

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
	RotaryEncoderInit();
	g_wRotaryEncoderValue = 50;
	//prepare all text for printing
	SwizzleText(bTwoKeys);
	SwizzleText(bText);
	SwizzleText(bQEData);

//TBD - Manot, fix the lcd display,	SetContrast(MAXLCD_CONT);
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);

	for(i = 0; i<12;i +=1){		 //post messages to draw the boxes
		SysPostMessage(5,LCD_PRINT_RANGE_ADDR,xcoords[i],ycoords[i],Button_Bitmap);
	}
    RetrieveLcdMessage();

	PrintEventString();	//Print "Event: " to the screen
	PrintEncoderString();	// print "Rotary Encoder:" to lcd
	wQE_data = g_wRotaryEncoderValue;
	//ExecuteModules();	//should empty the LCD Queue???
	
	//main program loop
	while(1){	 //run forever
// read and update the rotary encoder register		
		wQE_data = g_wRotaryEncoderValue;
		do{		//wait for a keypress
			keypress = GetKey();
			HandleRotaryEncoder();
		}while((keypress < 0) && (wQE_data == g_wRotaryEncoderValue));
// show the encoder value	
		PrintNumber(70,50,g_wRotaryEncoderValue,3,'0'); 	// display the encoder data

		if (keypress >= 0)
		{
		//print the event number
			PrintNumber(35,40,keypress,3,'0');
			if(keypress == 24)
				MultipleKeyPress();

			else
			{	
				if(keypress >= 12) 
					keypress -= 12; 
			
				if(state[keypress] == LCD_PRINT_RANGE_ADDR) //Toggle the state of the button bitmap
					state[keypress] = LCD_PRINT_RANGE_INV_ADDR;
				else
					state[keypress] = LCD_PRINT_RANGE_ADDR;
		
				SysPostMessage(5,state[keypress],xcoords[keypress],ycoords[keypress],Button_Bitmap);	   //update the
				RetrieveLcdMessage();
			}
	    } /* if (keypress >= 0) */	
	}	/* while (1) */
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
		dec		a
_Done
        jsr     SysAllLinear
    #pragma endasm
}



void MultipleKeyPress(){
	SysPostMessage(5,LCD_PRINT_STRING_ADDR,18,50,bTwoKeys);
	//bTwoKeys is a string of chars...defined above
	RetrieveLcdMessage();
}

void PrintEventString(){
	SysPostMessage(5,LCD_PRINT_STRING_ADDR,0,40,bText);	//print text to LCD
	RetrieveLcdMessage();
}

void PrintEncoderString(){
	SysPostMessage(5,LCD_PRINT_STRING_ADDR,0,50,bQEData);	//print text to LCD
	RetrieveLcdMessage();
}
