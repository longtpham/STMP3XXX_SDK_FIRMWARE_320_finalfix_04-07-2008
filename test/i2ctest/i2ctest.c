////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: i2ctest.c
// Description: File System Test Program
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "hwequ.h"
#include "messages.h"
#include "i2ctest.h"
#include "lcdsupport.h"
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
WORD g_JanusDrive;

//BOOL g_bWriteBmngr512Bytes=TRUE; // unused stub 

/*_circ*/ int DatBuff[56];       //temp buffer for data
/*_circ*/ int * pHead = DatBuff; //quasi-queue...head & tail point to same place for now
/*_circ*/ int * pTail = DatBuff;
	
_packed BYTE bTitle[] = "I2C TEST";    
_packed BYTE bMessageLine1[] = "24LC128 EEPROM";
_packed BYTE bMessagePressKey[] = "* Press key *";
_packed BYTE bReading[] = "Reading...";
_packed BYTE bWriting[] = "Writing...";
_packed BYTE bPass[] = "Test Passed";
_packed BYTE bFail[] = "Test Failed";

INT SysLowResolutionADCInit;
INT SysLowResolutionAdcReadVolt;
	
/* Data to be written to EEPROM. This is 48 bytes of data packed into 15 locations on STMP. 
   The 24LC128 EEPROM has a 64 byte paged write limitation. If you send more than that 
   in a I2C session then the page wraps around and data is lost.*/

int Data[16] = {258,197637,395016,592395,789774,987153,1184532,1381911,
				1579290,1776669,1974048,2171427,2368806,2566185,2763564,0x2D2E2F};
	
INT g_pCurrentTask = 0;
WORD *Kernel_pTempStack;

void main(void)
{
	
	unsigned int FAIL = 0;
	unsigned int i=0;

    WORD wKey = 0;    
	
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
	
	
	
	
	
	SysResetAllTimers();
	SysResetAllInterrupts();
    SysTimeInit();
	ButtonInit();
	ButtonsOn();
	SysAllLinear();

	DisplayModuleInit(ModuleTableDisplay);
    Delay(200);
	
    SwizzleText(bTitle);
    SwizzleText(bMessageLine1);			  //prepare all strings for printing
	SwizzleText(bMessagePressKey);
	SwizzleText(bWriting);
	SwizzleText(bReading);
	SwizzleText(bFail);
	SwizzleText(bPass);
    // Display Test Title

LABEL:    
    //Code to recover control of the I2C Port
	/*
	HW_I2CCSR.B.TREQ = 1;		   
	HW_I2CCSR.B.TREQ = 0;
	HW_I2CCSR.B.I2C_EN = 1;
	*/
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    PrintStringAddr(20, 0*8, bTitle);
	
    PrintStringAddr(2, 2*8, bMessageLine1);
	PrintStringAddr(2, 5*8, bMessagePressKey);
    
    do
    {
        wKey = GetKey();
    }while(wKey == 0);                     

   
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
	PrintStringAddr(0, 10, bWriting);

	//Now run the test!
    // Display Press Key
	pHead = Data;	//point phead to the write array
	I2C_Write_Master_Sync(0x0000A0,0x000000,16,pHead);
	
	Delay(80000);
	PrintStringAddr(0, 20, bReading);

	pHead = DatBuff;
	I2C_Read_Master_Sync(0x0000A0,0x000000,57,pHead);
	Delay(80000);
	
	i = 0;
	while(i < (0x2D / 3)){
		if(DatBuff[i] != (i*3) * 65536 + (i*3+1) * 256 + (i*3+2)){  
			//data is stored packed into 24-bit word, so we need to massage the data
			FAIL = 1;
		}
		//Display the values in the buffer. This data is packed 3-bytes/WORD.
		//Uncomment to write data to LCD
		/*
		PrintNumber(65, 0, DatBuff[i], 10, '0');
		wKey = 0;
		do
    	{
       	 	wKey = GetKey();
    	}while(wKey == 0);
		*/
		i+=1;
	}
	
	if(FAIL == 1){
		PrintStringAddr(0, 30, bFail);
	} else{
		PrintStringAddr(0, 30, bPass);
	}

	wKey = 0;
	do
    {
        wKey = GetKey();
    }while(wKey == 0);
	goto LABEL;
}
    

void I2C_Read_Master_Sync(int I2C_Device_Addr, int I2C_Sub_Addr, int num_words, /*_circ*/ int * ptr)
{
	int I2C_Addr = 0;		//init to zero
	int dummy = 0;			//ditto
	HW_I2CDIV.I = 0x78;
// Make LSB of the Slave address a 0 for write, right shift by 16 to put it into bits 23:16
	I2C_Addr = (I2C_Device_Addr & 0xFFFFFE) * 65536; 
	I2C_Addr += (I2C_Sub_Addr & 0x00FFFF);	// Add in the 16-bits of the Sub address
	//I2C_Addr = 0xA00000;
	HW_I2CDAT.I = I2C_Addr;	// Place the Slave Device Write address in the data register

// Init the I2C Peripheral: Standard Mode, 8-bits/word, no IRQs...
	HW_I2CCSR.I = 0x201;		
	
	HW_I2CCSR.B.TREQ = 1;		// Set the Transmit Request bit to 1
	HW_I2CCSR.B.TREQ = 0;		// Set the Transmit Request bit to 0
	
	while(HW_I2CCSR.B.SUBA != 1);//spin on SUBA
	
	HW_I2CCSR.B.TREQ = 1;		// Generate the Repeated Start
	HW_I2CCSR.B.TREQ = 0;
	
	//probably happens automatically
	while (HW_I2CCSR.B.TDE != 1);	// Wait for the transmit data empty bit to be set
									// Indicating that the transmit is done

	// Place the I2C Slave Device Read address in the data register...setting R/W to a 1.  Also mask off the 	
	HW_I2CDAT.I=((I2C_Addr | 0x010000) & 0xFF0000);	
		
	while (num_words > 0)
	{
		while (HW_I2CCSR.B.RDR != 1)	// Wait for Receive Data Register bit to be set
			;							// Indicating that the word is ready
		*ptr = HW_I2CDAT.I;				//store the recieved data in the data buffer
		HW_I2CCSR.B.ROFLCL = 1;			//clear the Reciever Overflow bit
		HW_I2CCSR.B.ROFLCL = 0;
		ptr += 1;						//increment the pointer
		num_words -= 3;					//we have recieved 3 bytes
	}							
   HW_I2CCSR.B.LWORD = 1;			// Send the stop command (will receive 1 more byte)
   while (HW_I2CCSR.B.RDR != 1)	// Wait for Receive Data Register bit to be set
			;							// Indicating that the receive is done
	dummy = HW_I2CDAT.I;
	
	//HW_I2CCSR.I = 0;	  //uncomment to break the part.
/*	
When running the test with the above line uncommented it will be necessary to 
run the test once before any failure is noticed just press any key after the test 
has run to run it multiple times.  Also connect an oscilloscope to the Clock line
of the I2C port to see the failure in action.  After running with this uncommented, 
uncomment the lines after the LABEL label to see the fix for this problem.  
This problem exists in ROM for I2C boot mode.  The code after the LABEL label 
should be used in players the boot from I2C to remedy this failure. 
*/

}

//be sure that the amount of data being sent and the sub address all fit within the page boundary of the EEPROM.
void I2C_Write_Master_Sync(int I2C_Device_Addr, int I2C_Sub_Addr, int num_words, /*_circ*/ int * ptr)
{
	int I2C_Addr = 0;		//init to zero

// Make LSB of the Slave address a 0 for write, right shift by 16 to put it into bits 23:16
	I2C_Addr = (I2C_Device_Addr & 0xFFFFFE) * 65536; 
	I2C_Addr += (I2C_Sub_Addr & 0x00FFFF);	// Add in the 16-bits of the Sub address

	HW_I2CDAT.I = I2C_Addr;	// Place the Slave Device Write address in the data register

// Init the I2C Peripheral: Standard Mode, 24-bits/word, no IRQs...
	HW_I2CCSR.I = 0x201;		
	
	HW_I2CCSR.B.TREQ = 1;		// Set the Transmit Request bit to 1
	HW_I2CCSR.B.TREQ = 0;		// Set the Transmit Request bit to 0
	
	while(HW_I2CCSR.B.SUBA != 1)//spin on SUBA
		;

		
	while (num_words > 1)
	{
		while (HW_I2CCSR.B.TUFL != 1)	//Wait for Truansmit Underflow
			;							//Indicating that the word is ready
		HW_I2CDAT.I = *ptr;				//store the to be sent data in the I2C Data Register
		HW_I2CCSR.B.TUFLCL = 1;			//clear the Reciever Overflow bit
		HW_I2CCSR.B.TUFLCL = 0;
		ptr += 1;						//increment the pointer
		num_words -= 1;					//we have sent 3 bytes
	}							
   while (HW_I2CCSR.B.TUFL != 1)	// Wait for Receive Data Register bit to be set
			;							// Indicating that the word is ready   
   
   HW_I2CCSR.B.LWORD = 1;			// Send the stop command (will send 3 more bytes)
   HW_I2CDAT.I = *ptr;		//store the to be sent data in the I2C Data Register
   HW_I2CCSR.B.TUFLCL = 1;			//clear the Reciever Overflow bit
   HW_I2CCSR.B.TUFLCL = 0;

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
