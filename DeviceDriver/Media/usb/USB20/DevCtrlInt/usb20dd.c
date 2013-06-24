////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2005
//
// File : usb20dd.c
// Description : USB2.0 device Controller Interface functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "usb_api.h"
#include "..\usb_internal.h"
#include "..\..\usb_dci.h"
#include "..\usb20ddprv.h"
#include "regsusb20.h"
#include "regsusb20phy.h"
#include "regsdcdc.h"
#include "regsclkctrl.h"
#include "sysirq.h"
#include "icoll.h"
#include "ipr.h"
#include "regscore.h"
//#define INSTRUMENT_ENABLE 1
#include "instrument.h"
#include "..\phy.h"
#include "regsgpio.h"
#include "regsrevision.h"

#pragma optimize 1   

#ifdef MTP_BUILD
#ifdef CLCD
#define MTP_LCD_SUSPEND 
#endif
#endif

#ifdef MTP_LCD_SUSPEND
#include "s6b33b0a.h"
#endif

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////
WORD wUsbIsrSaveSp;

extern WORD CapGetChipPartID(void);

#ifdef CLCD
#include "displaydriver_color.h"
#endif
////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

extern dQH_Struct_Cap _X dQH_Scratch_Cap;
extern dQH_Struct_Setup _X dQH_Scratch_Setup_Buf;
extern dTD_Struct _X * _X dTD_Base_Scratch_Address[];
extern Dev_State_Struct _USB_MEM stDeviceState;
extern xd_struct _X * _X dXD_Base_Address[];
extern BYTE _X * _X dQH_Base_Add;
extern BYTE _X * _X dTD_Base_Address[];

extern xd_struct _USB_MEM * g_dXD_Queues[MAX_NUM_EP][2];

////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////
void _long_interrupt(-1) usb_dci_resume_wrapper(void);
WORD _reentrant read_and_clear_arc_reg_usbsts(void);
WORD _reentrant read_usb_reg_16(USHORT usRegAdd);
void _reentrant read_and_clear_usb_reg(USHORT usRegAdd, DWORD * dwData);

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////
#define  USB_TEST_MODE_TEST_PACKET_LENGTH   (53)

// Chip capability register locations
#define CHP_CAP_0		0xFA90
#define CHP_CAP_1		0xFA91
#define CHP_CAP_2		0xFA92
#define CHP_CAP_3		0xFA93
#define CHP_CAP_4		0xFA94
#define CHP_CAP_5       0xFA95
#define ChipCapReg0		(*((volatile WORD _X *) (CHP_CAP_0)))  
#define ChipCapReg1		(*((volatile WORD _X *) (CHP_CAP_1)))
#define ChipCapReg2		(*((volatile WORD _X *) (CHP_CAP_2)))
#define ChipCapReg3		(*((volatile WORD _X *) (CHP_CAP_3)))
#define ChipCapReg4		(*((volatile WORD _X *) (CHP_CAP_4)))
#define ChipCapReg5		(*((volatile WORD _X *) (CHP_CAP_5)))

/* Test packet for Test Mode : TEST_PACKET. USB 2.0 Specification section 7.1.20 */
_packed BYTE _X TestPacket[USB_TEST_MODE_TEST_PACKET_LENGTH + 1] = 
{
    // This is three byte little endian, little bit endian.
    // We only need the data portion of the test packet. The ARC 
    // core will generate the synch, data PID parts of the packet
    // before the data, followed by the CRC and end of packet
    // after the data.
    
    // Synch
    // PID = 0xC3
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0xAA, 0xAA, 0xAA, 
    0xAA, 0xAA, 0xAA, 
    0xEE, 0xAA, 0xAA,
    0xEE, 0xEE, 0xEE, 
    0xEE, 0xEE, 0xEE, 
    0xFF, 0xFE, 0xEE,
    0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 
    0xBF, 0x7F, 0xFF,
    0xF7, 0xEF, 0xDF,
    0xFC, 0xFD, 0xFB,
    0xDF, 0xBF, 0x7E,
    0xFB, 0xF7, 0xEF,
    0x00, 0x7E, 0xFD
    // CRC = 0xCEB6
    // End
};

USHORT _X usDevDriverSendData;
////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
#ifdef MTP_BUILD
#define N_SETUP_ENDPT	1
#else
#define N_SETUP_ENDPT	8
#endif

USB_SetupData g_SetupData[N_SETUP_ENDPT];

static WORD s_wArcUsbIntRegShadow;

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ChipUpdateIdFlag
//
//   Type:          Function
//
//   Description:   Gets the Device ID from the registers.
//
//   Inputs:        None
//
//   Outputs:       The Device ID.
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
WORD ChipUpdateIdFlag(void)
{
    WORD wDeviceIdExt = (ChipCapReg2 >>14) & (0x0001FF); // 9-bit field
    WORD wAlternateDeviceIdExt = ( (ChipCapReg2 >> 15) & 0x000100) | (ChipCapReg5 & 0x0000FF); // 9bit alt id
    WORD wDeviceId;
    
    // Note: 16 bit field RMJ is revision major field == 0x3500 if 35xx family
    wDeviceId = HW_REVR.I;
        
    wDeviceId >>= 8;
    
    wDeviceId ^= (wDeviceIdExt | wAlternateDeviceIdExt); // 0x35xx
    
    return wDeviceId;
}  

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_init
//
//   Type:          Function
//
//   Description:   Initializes the USB device controller
//
//   Inputs:        pStDeviceState = Pointer to the device state structure
//
//   Outputs:       USB_OK or error code
//
//   Notes:         When this function is compiled as a funclet is is really
//      part of another funclet SYSFUNCLET_USB_DEVICE_INIT.   This funclet has
//      the entry point usb_device_init() which is the only function that should
//      ever call the usb_dci_init function.
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        org p,"SYSFUNCLET_USB_DCI_INIT_P":
    #pragma endasm
#endif
BYTE _reentrant usb_dci_init(Dev_State_Struct _USB_MEM * pStDeviceState)
{
   
    INT i;

    // parse_configurationtree();

    PHY_Startup();

    pStDeviceState->usUsbState = USB_STATE_UNKNOWN;
   
    // Initialize DMA ARC Offset
    HW_USBDMAOFF.B.ADD = (INT) &dQH_Base_Add;

    // Turn ON USB port clock        
    
    HW_USBCSR.B.USBEN = 0;
    HW_USBCSR.B.CLKOFF = 0;
    HW_USBCSR.B.USBEN = 1;
   
   //install all the interrupt vectors, etc.
   //5V Disconnect/Connect IRQ's should be at PRIORITY 000.
   IcollInstallIsrVector(usb_dci_isr,ICOLL_SOURCE_USB_CONTROLLER);
   	
   IcollSetPriority(ICOLL_SOURCE_USB_CONTROLLER,ICOLL_PRIORITY_001);
   IcollSetSteering(ICOLL_SOURCE_USB_CONTROLLER,ICOLL_STEERING_IVL1);    
   IcollEnable(ICOLL_SOURCE_USB_CONTROLLER);

   IPRSetIVLPriority(IVL_LINE_1,IVL_PRIORITY_1);

   for(i=0;i<N_SETUP_ENDPT;i++)
   {
       g_SetupData[i].bValid = FALSE;
   }
   
   if (0x3502==CapGetChipPartID())
   {
     g_pStrings[MS_OS_STRING_DESCRIPTOR_INDEX_IN_ARRAY] = NULL;
   }
   
   parse_strings();
   
   // Update USB Connection status
   usb_dci_update_connection_status();
        
   return USB_OK;   
}

#ifdef FUNCLET
    #pragma asm
        org p,".ptextusb20dd":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_chip_enable
//
//   Type:          Function
//
//   Description:   Initializes the USB device controller
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_chip_enable()
{ /* Body */
    USHORT      usEndpointCounter;

    DWORD       dwTempRegister;

   // Enable the ARC Core
   HW_USBCSR.B.ARCCONNECT = 1;

   /* Stop the controller */
    read_usb_reg(HW_ARC_USBCMD, &dwTempRegister);

   dwTempRegister &= HW_ARC_USBCMD_RS_CLRMASK; 

    write_usb_reg(HW_ARC_USBCMD, dwTempRegister);
      
   /* Reset the controller to get default values */
   dwTempRegister |= HW_ARC_USBCMD_RST_SETMASK; 

   write_usb_reg(HW_ARC_USBCMD, dwTempRegister);

   do
   {
        // Wait for the controller reset to complete
        read_usb_reg(HW_ARC_USBCMD, &dwTempRegister);
   }while(dwTempRegister & HW_ARC_USBCMD_RST_SETMASK);


   // Program the endpoint list address
   // dQHs structures are always the 1st in our memory map,
   // therefore the endpoint list address is set to 0x0000. 
    write_usb_reg(HW_ARC_ENDPTLISTADDR, (DWORD)0x00); 

   /* Program the controller to be the USB device controller */
    write_usb_reg(HW_ARC_USBMODE, (DWORD)0x02); 
   
    // Zero out the USB structures (dQHs, dTDs and dXDs)  
    FInitUsbStructures();
   
   // Make sure the setup endpoint status are cleared
    write_usb_reg(HW_ARC_ENDPTSETUPSTAT, (DWORD)0x00);
   
    // Initialize dQH Scratch Cap structure
    // MaxPacketSize to Max_Control_Payload (64)
    // Generate interrupt on setup packet
    dQH_Scratch_Cap.usMaxPktLengthZltMult = (USHORT)USB_MAX_CTRL_PAYLOAD;
    dQH_Scratch_Cap.usIos = (USHORT)USB_EP_QUEUE_HEAD_IOS;
    dQH_Scratch_Cap.pbtNextdTDPointer = (BYTE *) 0x0;
    dQH_Scratch_Cap.usStatusIoc = (USHORT) 0x0; 

   /* Initialize all device queue heads */
   for( usEndpointCounter = 0 ; usEndpointCounter < MAX_NUM_EP  ; usEndpointCounter++)
   {
        FdQHPackCap(usEndpointCounter, (USHORT)USB_OUT, (BYTE)0x01);
        FdQHPackCap(usEndpointCounter, (USHORT)USB_IN, (BYTE)0x01);
    } // Endfor      

   /* Initialize the endpoint 0 properties */
   // Reset the Toggle bit
   // Make sure that the enpoint is not stall
   read_usb_reg(HW_ARC_ENDPTCTRL(0), &dwTempRegister);

   dwTempRegister = dwTempRegister 
                    | (DWORD)HW_ARC_ENDPTCTRL_TXR_SETMASK
                    | (DWORD)HW_ARC_ENDPTCTRL_RXR_SETMASK;

    dwTempRegister = dwTempRegister 
                    & (DWORD)HW_ARC_ENDPTCTRL_TXS_CLRMASK 
                    & (DWORD)HW_ARC_ENDPTCTRL_RXS_CLRMASK;

    write_usb_reg(HW_ARC_ENDPTCTRL(0), dwTempRegister);                                                     
   
   /* Enable interrupts */
   read_usb_reg(HW_ARC_USBINTR, &dwTempRegister);

   dwTempRegister = dwTempRegister 
                    | (DWORD)HW_ARC_USBINTR_UE_SETMASK
                    | (DWORD)HW_ARC_USBINTR_UEE_SETMASK
                    | (DWORD)HW_ARC_USBINTR_PCE_SETMASK
                    | (DWORD)HW_ARC_USBINTR_URE_SETMASK;
                            
    write_usb_reg(HW_ARC_USBINTR, dwTempRegister);

    s_wArcUsbIntRegShadow = (WORD)dwTempRegister;

    // Note - need to enable the device separately.  See usb_dci_chip_enable below
   /* Set the Run bit in the command register */
    write_usb_reg(HW_ARC_USBCMD, (DWORD)HW_ARC_USBCMD_RS_SETMASK); 
   
} /* EndBody */




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_isr
//
//   Type:          Function
//
//   Description:   USB2.0 Interrupt Handler
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _long_interrupt(-1) usb_dci_isr(void)
{ /* Body */

    // Saves the Hardware Stack 
    #pragma asm
    extern  FwUsbIsrSaveSp
    extern  SaveSystemStack

    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi #$BF,mr

    move    #_ReturnFromHardwareStackSave,r6 ; return address
    move    #>FwUsbIsrSaveSp,r5
    jmp     SaveSystemStack
_ReturnFromHardwareStackSave
    #pragma endasm
    WORD     wStatus;
    WORD     wOldIVL;
   
   EventSet04();
   
   wOldIVL = SysMaskAllInterrupts();
   
   for (;;) 
   {
      //get the status
      wStatus = read_and_clear_arc_reg_usbsts();
      wStatus &= s_wArcUsbIntRegShadow; //only care about the events that we've got enabled in the interrupt enable mask

      if (!wStatus)
      {
         break;             // No more interruption to process
      } /* Endif */
   
      if (wStatus & HW_ARC_USBSTS_URI_SETMASK) 
      {
         usb_dci_process_reset(&stDeviceState);
      } /* Endif */
   
      if (wStatus & HW_ARC_USBSTS_PCI_SETMASK) 
      {
         usb_dci_process_port_change(&stDeviceState);
      } /* Endif */
   
      if (wStatus & HW_ARC_USBSTS_UEI_SETMASK) 
      {
         (stDeviceState.wError)++;

      } /* Endif */
   
      if (wStatus & HW_ARC_USBSTS_UI_SETMASK)
      {
         usb_dci_process_tr_complete(&stDeviceState);
      } /* Endif */
      
      if (wStatus & HW_ARC_USBSTS_SLI_SETMASK)
      {
		 //IRQ's are blocked.  Must ensure 5V Disconnect IRQ is active before
		 //entering suspend to detect a disconnect event.
		 SysSetIrqLevel(HW_SR_IM_L2_SETMASK);
         usb_dci_process_suspend(&stDeviceState);
         break;
      } /* Endif */
      
      if (wStatus & HW_ARC_USBSTS_SRI_SETMASK)
      {
         // This should not occur. Interruption is disabled during initialization

      } /* Endif */
      
   } /* Endfor */

   SysUnMaskAllInterrupts(wOldIVL);

    // Restores the hardware stack
    #pragma asm
    extern  RestoreSystemStack
    
    move    #_ReturnFromHardwareStackRestore,r6
    move    #>FwUsbIsrSaveSp,r5
    jmp     RestoreSystemStack
_ReturnFromHardwareStackRestore
    #pragma endasm

    EventClear04();

    
} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_wait_until_suspend_ends
//
//   Type:          Function
//
//   Description:   Waits until an event happens on the bus to break out of suspend.
//
//   Inputs:        pStDeviceState = Pointer to the device state structure
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
WORD usb_dci_wait_until_suspend_ends(Dev_State_Struct _USB_MEM * pStDeviceState)
{

    // Prepare for reading PORTSC1 ARC Register
    #pragma asm
        nolist
        include "sysmacro.asm"
        include "regsusb20.inc"
        include "regsusb20phy.inc"
        list

        ;Set Address, R/W/ Bit and kick transaction
        move    #>HW_USBARCACCESS_ADD_SETMASK,x0
        move    #>HW_ARC_PORTSC1,a
        and     x0,a
        move    a1,x:HW_USBARCACCESS
        bset    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS

        ;kick transaction
_Test_Again
        bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

        ;Wait end of transaction
_Loop
        move    x:HW_USBARCACCESS,a0
        jset    #HW_USBARCACCESS_KICK_BITPOS,a0,_Loop

        clr     a
        move    x:HW_USBARCDATALOW,a1        ;a1=XLL
        move    #>$000c00,x0
        and     x0,a    #>$000000,x0
        
        cmp     x0,a
        jeq     _Reset_True 

        move    #>$000400,x0
        cmp     x0,a
        jeq     _Test_Again
        
        clr     a
        
        jmp     _Done
        
_Reset_True
        move    #>1,a
        
_Done        

#pragma endasm  

}

#ifdef MTP_LCD_SUSPEND
extern INT gTUNER_STFM1000;
extern INT gi16BitOn;
_reentrant void MtpDisplaySendCommand(WORD wCommand)
{
	//LCD_RD_GPDO = TRUE;
	LCD_DC_GPDO = FALSE;
	LCD_CS_GPDO = FALSE;
	//_MultiNop();	//slight pause
	LCD_WR_GPDO = FALSE;
	//_MultiNop();	//slight pause
#ifdef CLCD_8BIT
	if (gTUNER_STFM1000) 
#else
	if (gTUNER_STFM1000 && !gi16BitOn)
#endif
	{
        LCD_DATA_D00 = wCommand & 0x01;
        LCD_DATA_D03 = (wCommand & 0x08) ? 1 : 0;
        LCD_DATA_D04 = (wCommand & 0x10) ? 1 : 0;
    }
	LCD_DATA_DO	= wCommand;	
	//_MultiNop();	//10ns pause
#ifdef CLCD_16BIT
	_MultiNop();_MultiNop();_MultiNop();_MultiNop();
#endif //#ifdef CLCD_16BIT
	LCD_WR_GPDO = TRUE;   
	LCD_CS_GPDO = TRUE;
}
_reentrant void MtpDisplaySuspendIn(void)
{
    HW_PWM_CSR.I = 0; //MASTER ENABLE to Start clocks

    LCD_BLON_GPDOE  = FALSE;
    LCD_BLON_GPDO   = FALSE;
    LCD_BLON_GPDI   = TRUE;
    LCD_BLON_GPEN   = TRUE;
    LCD_BLON_GPPWR  = FALSE;
    
    MtpDisplaySendCommand(0x50);    //Display off
#ifdef CLCD_8BIT
    MtpDisplaySendCommand(0x2D);    //Standby mode on   note: s6b33b0a could not wake up from standby mode if 16bit by experiment
#endif
    LCD_DC_GPDOE    = FALSE;
    LCD_DC_GPDO     = FALSE;
    LCD_DC_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DC_GPPWR    = FALSE;
#endif

    LCD_CS_GPDOE    = FALSE;
    LCD_CS_GPDO     = FALSE;
    LCD_CS_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_CS_GPPWR    = FALSE;
#endif

    LCD_RD_GPDOE    = FALSE;
    LCD_RD_GPDO     = FALSE;
    LCD_RD_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_RD_GPPWR    = FALSE;
#endif

    LCD_WR_GPDOE    = FALSE;
    LCD_WR_GPDO     = FALSE;
    LCD_WR_GPDI     = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_WR_GPPWR    = FALSE;
#endif

#ifdef CLCD_8BIT
	if (gTUNER_STFM1000)
	{   LCD_DATA_D00_GPDOE  =  FALSE; 
	    LCD_DATA_D00        =  FALSE;       
	    LCD_DATA_D00_DI     =  TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D00_GPPWR  =  FALSE;    
#endif
	
	    LCD_DATA_D03_GPDOE  =  FALSE; 
	    LCD_DATA_D03        =  FALSE;       
	    LCD_DATA_D03_DI     =  TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D03_GPPWR  =  FALSE;    
#endif
	
	    LCD_DATA_D04_GPDOE  =  FALSE; 
	    LCD_DATA_D04        =  FALSE;       
	    LCD_DATA_D04_DI     =  TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D04_GPPWR  =  FALSE;    
#endif
	
	    LCD_DATA_DOE   &= ~(0xe6);  // 1110 0110 except bits 0, 3, 4
	  	LCD_DATA_DO    &= ~(0xe6);  // 1110 0110 except bits 0, 3, 4  
	  	LCD_DATA_DI    |=   0xe6;   // 1110 0110 except bits 0, 3, 4  
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_PWR   &= ~(0xe6);
#endif
	}	 
	else // non STFM1000 layout
    {   LCD_DATA_DOE   &= ~(0xff);
	    LCD_DATA_DO    &= ~(0xff);
	    LCD_DATA_DI    |=   0xff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    	LCD_DATA_PWR   &= ~(0xff);
#endif
     }
#else //#ifdef CLCD_8BIT
    LCD_DATA_DOE   &= ~(0xffff);
    LCD_DATA_DO    &= ~(0xffff);
    LCD_DATA_DI    |=   0xffff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_PWR   &= ~(0xffff);
#endif
    CLCD_16BIT_SWITCH_GPDOE = FALSE;
    CLCD_16BIT_SWITCH       = FALSE;
    CLCD_16BIT_SWITCH_GPDI  = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    CLCD_16BIT_SWITCH_GPPWR = FALSE;
#endif
#endif //#ifdef CLCD_8BIT
}
_reentrant void MtpDisplaySuspendOut(void)
{
    LCD_DC_GPDI     = FALSE;
    LCD_DC_GPDO     = TRUE;
    LCD_DC_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DC_GPPWR    = TRUE;
#endif

    LCD_CS_GPDI     = FALSE;
    LCD_CS_GPDO     = FALSE;
    LCD_CS_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_CS_GPPWR    = TRUE;
#endif

    LCD_RD_GPDI     = FALSE;
    LCD_RD_GPDO     = TRUE;
    LCD_RD_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_RD_GPPWR    = TRUE;
#endif

    LCD_WR_GPDI     = FALSE;
    LCD_WR_GPDO     = TRUE;
    LCD_WR_GPDOE    = TRUE;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_WR_GPPWR    = TRUE;
#endif

#ifdef CLCD_8BIT
	if (gTUNER_STFM1000)
	{   LCD_DATA_D00_DI     =  FALSE;
	    LCD_DATA_D00        =  TRUE;       
	    LCD_DATA_D00_GPDOE  =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D00_GPPWR  =  TRUE;    
#endif
	
	    LCD_DATA_D03_DI     =  FALSE;
	    LCD_DATA_D03        =  TRUE;       
	    LCD_DATA_D03_GPDOE  =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D03_GPPWR  =  TRUE;    
#endif
	
	    LCD_DATA_D04_DI     =  FALSE;
	    LCD_DATA_D04        =  TRUE;       
	    LCD_DATA_D04_GPDOE  =  TRUE; 
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_D04_GPPWR  =  TRUE;    
#endif
	
	  	LCD_DATA_DI    &= ~(0xe6);  // 1110 0110 except bits 0, 3, 4  
	  	LCD_DATA_DO    |=   0xe6;   // 1110 0110 except bits 0, 3, 4  
	    LCD_DATA_DOE   |=   0xe6;   // 1110 0110 except bits 0, 3, 4
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	    LCD_DATA_PWR   |=   0xe6;
#endif
	}	 
	else // non STFM1000 layout
    {   LCD_DATA_DI    &= ~(0xff);
	    LCD_DATA_DO    |=   0xff;
	    LCD_DATA_DOE   |=   0xff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    	LCD_DATA_PWR   |=   0xff;
#endif
     }
#else //#ifdef CLCD_8BIT
    //******* 16-bit interface is already turned on before.
    LCD_DATA_DI    &= ~(0xffff);
    LCD_DATA_DO    |=   0xffff;
    LCD_DATA_DOE   |=   0xffff;
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
    LCD_DATA_PWR   |=   0xffff;
#endif

    CLCD_16BIT_SWITCH_GPDI  =  FALSE;
    CLCD_16BIT_SWITCH       =  CLCD_16BIT_SWITCH_16BIT_ON;
    CLCD_16BIT_SWITCH_GPDOE =  TRUE; 	    
#if defined(DEVICE_3410) || defined(DEVICE_3310) || defined(DEVICE_3500)
	CLCD_16BIT_SWITCH_GPPWR =  TRUE; 	
#endif
#endif //#ifdef CLCD_8BIT

#ifdef CLCD_8BIT
    MtpDisplaySendCommand(0x2C);    //Standby mode off  note: s6b33b0a could not wake up from standby mode if 16bit by experiment
#endif
    MtpDisplaySendCommand(0x51);    //Display on

    LCD_BLON_GPDOE  = FALSE;
    LCD_BLON_GPDO   = FALSE;
    LCD_BLON_GPDI   = FALSE;
    LCD_BLON_GPEN   = FALSE;
    LCD_BLON_GPPWR  = TRUE;

    HW_PWM_CSR.I = PWM_ENABLE_MASK; //MASTER ENABLE to Start clocks
}
#endif
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_process_suspend
//
//   Type:          Function
//
//   Description:   Services DC Suspend interrupt
//
//   Inputs:        pStDeviceState = Pointer to the device state structure
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_process_suspend(Dev_State_Struct _USB_MEM * pStDeviceState)
{ /* Body */
    WORD wReset = FALSE;
    
    if(pStDeviceState->usUsbState != (USHORT)USB_STATE_SUSPENDED)
    {    
#ifdef CLCD
#ifdef MTP_LCD_SUSPEND   
        MtpDisplaySuspendIn();
#else
        HALDisplaySuspendIn();
#endif
#endif
        //shut down the phy
        PHY_Shutdown();

        //call the suspend service
        usb_device_call_service(USB_SERVICE_SUSPEND, FALSE, 0, 0, 0,0);

        //put the clock as low as possible
        HW_CCR.B.DDIV_MSB = 1;
        HW_CCR.B.DDIV = 3;

        wReset=usb_dci_wait_until_suspend_ends(pStDeviceState);

        HW_CCR.B.DDIV_MSB = 0;
        HW_CCR.B.DDIV = 0;
        HW_GP0DOR.B.B14 = TRUE;

        usb_device_call_service(USB_SERVICE_RESUME, FALSE, 0, 0, 0,0);

        PHY_Resume();

#ifdef CLCD
#ifdef MTP_LCD_SUSPEND
        MtpDisplaySuspendOut(); //CQ
#else
        HALDisplaySuspendOut();
#endif
#endif
        if(wReset)
        {
            usb_dci_process_reset(pStDeviceState);
            
            // JCP - If we have problems to wake up from suspend, we may want to 
			// comment the next usb_device_hold() call ...
            usb_device_hold();
            
            usb_device_set_first_init();
            
        }

    }        

} /* EndBody */




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_process_port_change
//
//   Type:          Function
//
//   Description:   Services Port Change Detect Interrupt
//
//   Inputs:        pStDeviceState = Pointer to the device state structure
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_process_port_change(Dev_State_Struct _USB_MEM * pStDeviceState)
{ /* Body */

    DWORD   dwStatus, dwReg;
    BYTE    btEndpoint;
    
    if (pStDeviceState->btBusResetting == TRUE)
    {   // Port change Detect issue due probably to the end of USB bus reset

        // Clear the bus resetting flag
        pStDeviceState->btBusResetting = FALSE;

        // Set the state of USB to DEFAULT
        pStDeviceState->usUsbState = USB_STATE_DEFAULT;
        // At this point we need to enable all endpoints so to NAK in response
        // to a host transfer. If the enpoint is not enable and the host initiates
        // a transfer, ARC will ignore it and who knows what the host will do.
        for(btEndpoint = 1 ; btEndpoint < MAX_NUM_EP ; btEndpoint++)
        {    
            read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &dwReg);
      
            dwReg = dwReg | (DWORD)HW_ARC_ENDPTCTRL_RXE_SETMASK
                          | (DWORD)HW_ARC_ENDPTCTRL_TXE_SETMASK;
      
            write_usb_reg((HW_ARC_ENDPTCTRL(btEndpoint)), dwReg);
        } // Endfor

    } /* Endif */

    // Check if reset is still present on the bus
    read_usb_reg(HW_ARC_PORTSC1, &dwStatus);

    if (!(dwStatus & HW_ARC_PORTSC1_PR_SETMASK)) 
    {   // Reset completed, check which speed we will operate at
        if (dwStatus & HW_ARC_PORTSC1_HSP_SETMASK) 
            pStDeviceState->btSpeed = (BYTE)USB_SPEED_HIGH;
        else    
            pStDeviceState->btSpeed = (BYTE)USB_SPEED_FULL;
      
        usb_device_call_service(USB_SERVICE_SPEED_DETECTION, FALSE, 0, 0, (WORD)pStDeviceState->btSpeed,0);

    } // Endif
      
      
    


    // Port change detect could have been generated by a SUSPEND state
    if (dwStatus & (DWORD)HW_ARC_PORTSC1_SUSP_SETMASK) 
    {   // If so, suspend the system
        //I don't believe this branch is ever called. RS

        // Save the USB state prior the SUSPEND, to restore after a resume
        // Set the state of USB to SUSPEND
        if(pStDeviceState->usUsbState != USB_STATE_SUSPENDED)
        { 
            pStDeviceState->usUsbStatePriorSusp = pStDeviceState->usUsbState;
            pStDeviceState->usUsbState = USB_STATE_SUSPENDED;
        }        
      
      usb_device_call_service(USB_SERVICE_SUSPEND, FALSE, 0, 0, 0,0);
    } // Endif

    // Port change detect could have been generated by leaving a SUSPEND state
    if ((!(dwStatus & (DWORD)HW_ARC_PORTSC1_SUSP_SETMASK)) && (pStDeviceState->usUsbState == USB_STATE_SUSPENDED)) 
    {
        //I don't believe this branch is ever called. RS
        pStDeviceState->usUsbState = pStDeviceState->usUsbStatePriorSusp;
    } // Endif 



} /* EndBody */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_clear_dxd_queue
//
//   Type:          Function
//
//   Description:   Clears a particular dxd queue
//
//   Inputs:        wEndpoint
//                  wDirection
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_clear_dxd_queue(WORD wEndpoint, WORD wDirection)
{
    xd_struct _USB_MEM * pCurrentXd = g_dXD_Queues[wEndpoint][wDirection];
    xd_struct _USB_MEM * pNextXd;
    while(pCurrentXd)
    {
        pNextXd = pCurrentXd->pNext;
        FreeXd(pCurrentXd);
        pCurrentXd = pNextXd;
    }
    g_dXD_Queues[wEndpoint][wDirection] = NULL;

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_process_reset
//
//   Type:          Function
//
//   Description:   Services reset interrupt
//
//   Inputs:        pStDeviceState = Pointer to the device state structure
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_process_reset(Dev_State_Struct _USB_MEM * pStDeviceState)
{ /* Body */
   DWORD    dwRegister;
   BYTE     btEndpoint;
   int i;
   
      // Initializes the endpoint 0. If application needs different init params 
    // for endpoint 0 it must do it in the USB_SERVICE_BUS_RESET service
    usb_device_init_endpoint(0, 64, USB_OUT, USB_CONTROL_ENDPOINT, TRUE);
    usb_device_init_endpoint(0, 64, USB_IN, USB_CONTROL_ENDPOINT, TRUE);
    
    //clear out the dxds
    for(i=0;i<MAX_NUM_EP;i++)
    {
        usb_dci_clear_dxd_queue(i,0);
        usb_dci_clear_dxd_queue(i,1);
    }


   /* Inform the application so that it can cancel all previously queued transfers */
   usb_device_call_service(USB_SERVICE_BUS_RESET, FALSE, 0, 0, 0,0);
   
   // Clear the device address register
   // The address bits are past bit 25-31. Set the address
   read_usb_reg(HW_ARC_DEVADDR, &dwRegister);

   dwRegister = dwRegister & 0x01ffffff;

    write_usb_reg(HW_ARC_DEVADDR, dwRegister);


   //Enable the suspend interrupt.............................
   read_usb_reg(HW_ARC_USBINTR, &dwRegister);               //
   dwRegister = dwRegister                                  // Suspend is initially not enabled to avoid the
                    | (DWORD)HW_ARC_USBINTR_SLE_SETMASK;    // suspend at the beginning of the enumeration process
    write_usb_reg(HW_ARC_USBINTR, dwRegister);              //
    s_wArcUsbIntRegShadow = (WORD) dwRegister;
   //.........................................................
   dwRegister = HW_ARC_USBSTS_SLI_SETMASK;
   write_usb_reg(HW_ARC_USBSTS, dwRegister);

    // Update the device state structure
    pStDeviceState->btAddress = (BYTE)0x00;
    
   
   /* Clear all the setup token semaphores */
    read_usb_reg(HW_ARC_ENDPTSETUPSTAT, &dwRegister);

    write_usb_reg(HW_ARC_ENDPTSETUPSTAT, dwRegister);

   /* Clear all the endpoint complete status bits */   
    read_usb_reg(HW_ARC_ENDPTCOMPLETE, &dwRegister);

    write_usb_reg(HW_ARC_ENDPTCOMPLETE, dwRegister);

   // Cancel all primed status by waiting untill all bits in the ENDPTPRIME are 0
   // then writing 0xffffffff to ENDPTFLUSH
    do
    {
        // Wait until all ENDPTPRIME bits cleared
        read_usb_reg(HW_ARC_ENDPTPRIME, &dwRegister);
    }while(dwRegister & (DWORD)0xffffffffffff);


   /* Write 1s to the Flush register */
    write_usb_reg(HW_ARC_ENDPTFLUSH, (DWORD)0xffffffffffff);
   
    // Read the reset bit in the PORTSCx register and make sure that it is still active
    // A USB reset will occur for a minimum of 3ms and the device driver software
    // must reach this point in the reset cleanup before the end of the reset occurs
    // Otherwise a hardware reset of the device controller is recommended (rare)
    read_usb_reg(HW_ARC_PORTSC1, &dwRegister);
    
   if (dwRegister & (DWORD)HW_ARC_PORTSC1_PR_SETMASK) 
   {
      pStDeviceState->btBusResetting = TRUE;
      pStDeviceState->usUsbState = USB_STATE_POWERED;
   } 
   else 
   { 
      /* re-initialize */      
      usb_dci_chip_enable();
      return;
   } /* Endif */
   

    // Set all transfer structures to idle
    for(btEndpoint = 0 ; btEndpoint < MAX_NUM_EP ; btEndpoint++)
    {    
        dXD_Base_Address[btEndpoint*2]->btStatus = USB_STATUS_TRANSFER_IDLE;
        dXD_Base_Address[(btEndpoint*2) + 1]->btStatus = USB_STATUS_TRANSFER_IDLE;
    }        
    
} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_process_tr_complete
//
//   Type:          Function
//
//   Description:   Services transaction complete interrupt
//
//   Inputs:        pStDeviceState = Pointer to the device state structure
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_process_tr_complete(Dev_State_Struct _USB_MEM * pStDeviceState)
{ /* Body */

   WORD         wRegSetup;
   DWORD        dwRegComplete;
   void (*ProcessFirst)(DWORD, WORD) = &ProcessEndptSetup;
   void (*ProcessLast)(DWORD, WORD) = &ProcessEndptComplete;
   
    // We use separate loops for SETUP packets and other packet types.
    // The setup packet is processed first. It must be done as soon as possible
    
    // Processing the setup transaction .......
    wRegSetup = read_usb_reg_16(HW_ARC_ENDPTSETUPSTAT);     // Reads the setup status register
    
    // Processing the non-setup transaction .......
    read_and_clear_usb_reg(HW_ARC_ENDPTCOMPLETE, &dwRegComplete);    // Reads the end point complete register
    
    // The order to process setup packets and non setup packets depends on
    // the previous transactions.
    // If a control endpoint has a setup packet and a non setup transfer complete
    // pending, then we should handle the non setup transfer first because it is
    // probably a result of the last setup transaction.
    if(((WORD)(dwRegComplete) & wRegSetup) || ((WORD)(dwRegComplete >> 24) & wRegSetup))
    {
        ProcessFirst = &ProcessEndptComplete;
        ProcessLast = &ProcessEndptSetup;
    }

    (*ProcessFirst)(dwRegComplete, wRegSetup);
    (*ProcessLast)(dwRegComplete, wRegSetup);
    
}    
     
    
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ProcessEndptSetup
//
//   Type:          Function
//
//   Description:   Services transaction complete for setup phase 
//
//   Inputs:        dwRegComplete = Endpoints with Transaction complete pending
//                  wRegSetup = Endpoints with Setup pendings
//
//   Outputs:       none
//
//   Notes:        dwRegComplete is not used
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ProcessEndptSetup(DWORD dwRegComplete, WORD wRegSetup)
{
    BYTE         btEndpointNumber;

    // a setup packet pending will have its bit set to '1'
    if(wRegSetup)
    { // Setup packets to process
        // for each endpoint define
        for(btEndpointNumber = 0 ; btEndpointNumber < MAX_NUM_EP ; btEndpointNumber++)
        {
            // Do we have a setup packet pending
            // If we have a setup packet pending do not clear the endpoint setup status
            // bit in the register. We need to keep the setup lockout enable until we
            // read the setup bytes. Clearing the bit is done there.
            if(wRegSetup & ((WORD)(1 << btEndpointNumber)))
                usb_device_call_service(btEndpointNumber, TRUE, 0, 0, NUMBER_BYTES_IN_SETUP,0);  // calls the service routine
        }    
    }
}
    
   
WORD    wRegCompleteTx; 
WORD    wRegCompleteRx; 
WORD    wMask;
WORD    wRegComplete; 



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ProcessEndptComplete
//
//   Type:          Function
//
//   Description:   Services transaction complete for complete transaction others
//                  than setup packet
//
//   Inputs:        dwRegComplete = Endpoints with Transaction complete pending
//                  wRegSetup = Endpoints with Setup pendings
//
//   Outputs:       none
//
//   Notes:        dwRegSetup is not used
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ProcessEndptComplete(DWORD dwRegComplete, WORD wRegSetup)
{   
    BYTE         btEndpointNumber;
    BYTE         btEndpoint, btDirection;
    dTD_Struct _X * pStdTD;
    xd_struct  _USB_MEM * pStXd;
    USHORT       usActualBytesTransfer;
    
    wRegCompleteTx = (dwRegComplete >> 24);
    wRegCompleteRx = dwRegComplete & 0x00ffff;
    wMask = 1;
    wRegComplete = wRegCompleteRx;
    

    if (dwRegComplete)
    { // non setup packets to process
        // For each endpoint define
        for (btEndpointNumber = 0 ; btEndpointNumber < 32 ; )
        {
            if(!wRegCompleteTx && !wRegCompleteRx)
                break;

            if (wRegComplete & wMask)
            {   // We have something , found which direction and endpoint
                
                wRegComplete &= ~wMask;

                if (btEndpointNumber > 15) 
                {
                    btEndpoint = btEndpointNumber - 16;
                    btDirection = USB_IN;
                }
                else
                {
                    btEndpoint = btEndpointNumber;
                    btDirection = USB_OUT;
                } /* Endif */
                


                // Extract the specified dTD
                FdTDUnpack((USHORT)btEndpoint, btDirection);
                pStdTD = dTD_Base_Scratch_Address[(2*btEndpoint) + btDirection];
                pStXd = g_dXD_Queues[btEndpoint][btDirection];


                // Check if the dTD is still active
                if(pStdTD->usStatusIoc & (USHORT)USB_EP_dTD_ACTIVE)
                {
		    if (pStXd)
		    {
                        pStXd->btStatus = USB_STATUS_TRANSFER_IN_PROGRESS;
		    }

                    usActualBytesTransfer = 0;
                }
                else
                {
                    if(pStdTD->usStatusIoc & (USHORT)USB_EP_dTD_NO_ERROR)
                    {   // Error detected
                        // For IN BULK no errors should ever be encountered
                        // For OUT BULK the only error encountered is when:
                        // A long packet was received (number of bytes greater
                        // than maximum packet size) or (total bytes received
                        // greater than total bytes specified).
                        // The ARC core discards the remaining packet and set
                        // the error bit in the dTD. In addition, the endpoint
                        // will be flushed and the USBERR interrupt will
                        // become active. 
		        if (pStXd)
			{
                            pStXd->btStatus = USB_STATUS_TRANSFER_ERROR;
			}
                    }
                    else
                    {   // Transfer successful
		        if (pStXd)
			{
                            pStXd->btStatus = USB_STATUS_TRANSFER_SUCCESS;
			}
                    }                    
                   
                    // Calculate the actual number of bytes transfered
                    usActualBytesTransfer = pStXd->usTotalLength - pStdTD->usTotalBytes;

                    // Retire the dTD
                    pStdTD->pbtNextdTDPointer = 0x00;        // Set the next dTD pointer to 0
                    pStdTD->usStatusIoc = (USHORT)0x00;      // Clear status and Interrupt on Completion bit

                }

                // Call the function registered to service the specified endpoint
                usb_device_call_service(btEndpoint, FALSE, btDirection, (WORD _X *)pStXd->pbtStartAddressWord, usActualBytesTransfer, pStXd->wPrivateData);  

                //move the next DXD up.
                if (pStXd)
                {
                    g_dXD_Queues[btEndpoint][btDirection] = pStXd->pNext;

                    /* MPAYNE: 5/31/05
                    * The FreeXd call was outside of this IF statement and that
                    * was causing defect 7098.  The problem was that for some
                    * reason pStXd was getting set to NULL in some Plays For
                    * Sure content transfer tests.
                    */
                    //free current Xd
                    FreeXd(pStXd);
                }


                // start new transfer if needed
                if(g_dXD_Queues[btEndpoint][btDirection] != NULL)
                    usb_dci_transfer_data(g_dXD_Queues[btEndpoint][btDirection]);
            }

            if(btEndpointNumber < MAX_NUM_EP)
            {
                btEndpointNumber += 16;
                wRegCompleteRx = wRegComplete;
                wRegComplete = wRegCompleteTx;
            }
            else
            {
                btEndpointNumber -= 15;
                wRegCompleteTx = wRegComplete;
                wRegComplete = wRegCompleteRx;
                wMask *= 2;
            }

        } /* Endfor */
    } /* Endif */

} /* EndBody */







////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_init_endpoint
//
//   Type:          Function
//
//   Description:   Initializes the specified endpoint and the endpoint queue
//                  head
//
//   Inputs:        pStXD = Pointer to a XD structure
//
//   Outputs:       USB_OK or error code
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_dci_init_endpoint(xd_struct _USB_MEM * pStXD)
{ /* Body */
    DWORD   dwRegEndpointPrime;   
    DWORD   dwRegEndpointStatus;
    WORD wOldIVL;
   
    // Check if the Endpoint is Primed.
    // If the enpoint is primed, then return an error because the endpoint is 
    // in used already
    read_usb_reg(HW_ARC_ENDPTPRIME, &dwRegEndpointPrime);
    read_usb_reg(HW_ARC_ENDPTSTATUS, &dwRegEndpointStatus);
    
    if(    (!(dwRegEndpointPrime  & (1 << ((16 * pStXD->btDirection) + pStXD->btEndpointNum))))     // Not requested for primed
        && (!(dwRegEndpointStatus & (1 << ((16 * pStXD->btDirection) + pStXD->btEndpointNum))))  )  // Endpoint buffer not ready
    { 
        wOldIVL = SysMaskAllInterrupts();
        
        // Fill the dQH cap scratch structure

        // First check for type of endpoint
        if(pStXD->btEndpointType == USB_ISOCHRONOUS_ENDPOINT)
        {
            // Isochronous endpoint type
            /* Mult bit should be set for isochronous endpoints */
            dQH_Scratch_Cap.usMaxPktLengthZltMult = (USHORT)(1 << USB_EP_QUEUE_HEAD_MULT_POS |
                                                              pStXD->usMaxPacketSize);
        }
        else
        {
            if (pStXD->btEndpointType != USB_CONTROL_ENDPOINT)
            {
                // Bulk or interrupt endpoint type            
                dQH_Scratch_Cap.usMaxPktLengthZltMult = (USHORT) (pStXD->btDontZeroTerminate << USB_EP_QUEUE_HEAD_DONT_ZERO_TERMINATE_POS
                                                                    | pStXD->usMaxPacketSize);
            }                                                                    
            else
            {            
                // Control endpoint type
                dQH_Scratch_Cap.usMaxPktLengthZltMult = (USHORT) (pStXD->btDontZeroTerminate << USB_EP_QUEUE_HEAD_DONT_ZERO_TERMINATE_POS
                                                                    | pStXD->usMaxPacketSize);
                dQH_Scratch_Cap.usIos = (USHORT)USB_EP_QUEUE_HEAD_IOS;
            }                
         }
         
         // Write the scratch structure to the dQH field
         FdQHPackCap(pStXD->btEndpointNum, pStXD->btDirection, (BYTE)0x01);
         
         SysUnMaskAllInterrupts(wOldIVL);
      
        /* Enable the endpoint for Rx and Tx and set the endpoint type */
        read_usb_reg(HW_ARC_ENDPTCTRL(pStXD->btEndpointNum), &dwRegEndpointStatus);
      
        if(pStXD->btDirection == USB_IN)
        {
            dwRegEndpointStatus = dwRegEndpointStatus 
                                    | (DWORD)HW_ARC_ENDPTCTRL_TXE_SETMASK    //(DWORD)(1 << HW_ARC_ENDPTCTRL_TXE_BITPOS)     // (DWORD)HW_ARC_ENDPTCTRL_TXE_SETMASK
                                    | (DWORD)HW_ARC_ENDPTCTRL_TXR_SETMASK
                                    | (DWORD)((DWORD)pStXD->btEndpointType << HW_ARC_ENDPTCTRL_TXT_BITPOS) ;       
        }
        else
        {
            dwRegEndpointStatus = dwRegEndpointStatus 
                                | (DWORD)HW_ARC_ENDPTCTRL_RXE_SETMASK
                                | (DWORD)HW_ARC_ENDPTCTRL_RXR_SETMASK
                                | ((DWORD)pStXD->btEndpointType << HW_ARC_ENDPTCTRL_RXT_BITPOS) ;       
      
        }
        
        write_usb_reg((HW_ARC_ENDPTCTRL(pStXD->btEndpointNum)), dwRegEndpointStatus);
   }
   else
   { 
      return USBERR_EP_INIT_FAILED;
   } /* Endif */
      
   return USB_OK;
   
} /* EndBody */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_get_setup_data
//
//   Type:          Function
//
//   Description:   Reads the setup data from the dQH
//
//   Inputs:        
//                  btEndpoint  : Endpoint number
//
//   Outputs:       
//                  a pointer to the setup structure
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
USB_SetupData *_reentrant usb_dci_get_setup_data(BYTE btEndpoint)
{ /* Body */
    USB_SetupData * pSetupData =  &g_SetupData[btEndpoint];

   
    if(!pSetupData->bValid)        
    {
        // Copies the 8 setup bytes into the dQH_Scratch_Setup_Buf 
        FdQHUnpackSetup((USHORT)btEndpoint, USB_OUT);
        pSetupData->RequestType.I 
            =  dQH_Scratch_Setup_Buf.btRequestType;
        pSetupData->btRequest = dQH_Scratch_Setup_Buf.btRequest;
        pSetupData->wValue = dQH_Scratch_Setup_Buf.btValueLsb + (dQH_Scratch_Setup_Buf.btValueMsb<<8);
        pSetupData->wIndex = dQH_Scratch_Setup_Buf.btIndexLsb + (dQH_Scratch_Setup_Buf.btIndexMsb<<8);
        pSetupData->wLength = dQH_Scratch_Setup_Buf.btLengthLsb + (dQH_Scratch_Setup_Buf.btLengthMsb<<8);
        
        // Clear the bit in the endpoint setup status register
        // to disable the setup lockout mechanism
        write_usb_reg(HW_ARC_ENDPTSETUPSTAT, (DWORD)(1 << btEndpoint));

        pSetupData->bValid = TRUE;
    
    }

    return(pSetupData);   
} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_get_endpoint_status
//
//   Type:          Function
//
//   Description:   Gets the endpoint stall status
//
//   Inputs:        
//                  btEndpoint  : Endpoint number
//
//   Outputs:       
//                  TRUE if endpoint stall
//                  FALSE otherwise
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_dci_get_endpoint_status(BYTE btEndpoint)
{ /* Body */
    DWORD dwRegister;
    
    read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &dwRegister);
    return((dwRegister & (DWORD)((DWORD)HW_ARC_ENDPTCTRL_TXS_SETMASK | (DWORD)HW_ARC_ENDPTCTRL_RXS_SETMASK)) ? TRUE : FALSE);
    
} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_set_endpoint_status
//
//   Type:          Function
//
//   Description:   STALLS the endpoint
//
//   Inputs:        
//                  btEndpoint  : Endpoint number
//                  usStatus    : New status (TRUE or FALSE)
//
//   Outputs:  none
//                  
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_set_endpoint_status(BYTE btEndpoint, USHORT usStatus)
{ /* Body */

    if (usStatus == TRUE)
    {
        usb_dci_stall_endpoint(btEndpoint, USB_OUT);
        usb_dci_stall_endpoint(btEndpoint, USB_IN);
    }    
    else
    {
        usb_dci_unstall_endpoint(btEndpoint, USB_OUT);
        usb_dci_unstall_endpoint(btEndpoint, USB_IN);
    }        
    
} /* EndBody */



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_set_address
//
//   Type:          Function
//
//   Description:   Sets the newly assigned device address
//
//   Inputs:        
//                  btAddress  : Address
//
//   Outputs:  none
//                  
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_set_address(BYTE btAddress)
{ /* Body */
    
   // The address bits are past bit 25-31. Set the address */
    write_usb_reg(HW_ARC_DEVADDR, (DWORD)((DWORD)btAddress << HW_ARC_DEVADDR_ADD_BITPOS));

} /* EndBody */


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_get_transfer_status
//
//   Type:          Functiorn
//
//   Description:   Gets the status of the last transfer on the specified
//                  couple endpoint/direction
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  btDirection      : Direction
//
//   Outputs:       
//                  Transfer status
//
//   Notes:
//                  The status returned is the btStatus entry from the XD
//                  associated with the couple endpoint/direction         
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_dci_get_transfer_status(BYTE btEndpoint, BYTE btDirection)
{ /* Body */

    xd_struct  _USB_MEM * pStXd;
   
    pStXd = g_dXD_Queues[btEndpoint][btDirection];

    return(pStXd->btStatus);
   
} /* EndBody */

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_stall_endpoint
//
//   Type:          Functiorn
//
//   Description:   Stalls the specified endpoint for the specified direction
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  btDirection      : Direction
//
//   Outputs:       
//                  None
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_stall_endpoint(BYTE btEndpoint, BYTE btDirection)
{ /* Body */

    DWORD   dwRegister;

    read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &dwRegister);
    
    dwRegister |= (btDirection ? (DWORD)HW_ARC_ENDPTCTRL_TXS_SETMASK 
                               : (DWORD)HW_ARC_ENDPTCTRL_RXS_SETMASK); 

    // Reset the DATA0 to synchronize the DATA PID's between the Host and Device
    dwRegister |= (btDirection ? (DWORD)HW_ARC_ENDPTCTRL_TXR_SETMASK 
                               : (DWORD)HW_ARC_ENDPTCTRL_RXR_SETMASK); 

    /* Stall the endpoint for Rx or Tx and set the endpoint type */
    write_usb_reg((HW_ARC_ENDPTCTRL(btEndpoint)), dwRegister);
           
} /* EndBody */

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_unstall_endpoint
//
//   Type:          Functiorn
//
//   Description:   Unstalls the specified endpoint for the specified direction
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  btDirection      : Direction
//
//   Outputs:       
//                  None
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_unstall_endpoint(BYTE btEndpoint, BYTE btDirection)
{ /* Body */

    DWORD   dwRegister;
    

    /* UnStall the endpoint for Rx or Tx and set the endpoint type */
    read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &dwRegister);

    dwRegister &= (btDirection ? (DWORD)HW_ARC_ENDPTCTRL_TXS_CLRMASK 
                               : (DWORD)HW_ARC_ENDPTCTRL_RXS_CLRMASK); 

    // Reset the DATA0 to synchronize the DATA PID's between the Host and Device
    dwRegister &= (btDirection ? (DWORD)HW_ARC_ENDPTCTRL_TXR_CLRMASK 
                               : (DWORD)HW_ARC_ENDPTCTRL_RXR_CLRMASK); 

    /* Stall the endpoint for Rx or Tx and set the endpoint type */
    write_usb_reg((HW_ARC_ENDPTCTRL(btEndpoint)), dwRegister);
           
} /* EndBody */





////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_init_data_toggle_endpoint
//
//   Type:          Functiorn
//
//   Description:   Unstalls the specified endpoint for the specified direction
//
//   Inputs:        
//                  btEndpoint      : Endpoint
//                  btDirection      : Direction
//
//   Outputs:       
//                  None
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_init_data_toggle_endpoint(BYTE btEndpoint)
{
    DWORD   dwTempRegister;
    
   // Reset the Toggle bit
   read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &dwTempRegister);

   dwTempRegister = dwTempRegister 
                    | (DWORD)HW_ARC_ENDPTCTRL_TXR_SETMASK
                    | (DWORD)HW_ARC_ENDPTCTRL_RXR_SETMASK;

    write_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), dwTempRegister);                                                     
}




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_transfer_data
//
//   Type:          Functiorn
//
//   Description:   Prepares the arc core to transfer the data described by 
//                  the XD descriptor
//
//   Inputs:        
//                  pStXd  : Pointer to the transfer descriptor
//
//   Outputs:       
//                  USB_OK
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_dci_transfer_data(xd_struct _USB_MEM * pStXd)
{ /* Body */
    dTD_Struct _X * pStdTD;
    BYTE _X * pdTD;
    WORD    wStmpByteAddress, wReg;
    USHORT  usArcPage, usArcByteOffset, usArcdTD;
    DWORD   dwReg;
    
    
    int iEndPoint = pStXd->btEndpointNum;
    int iDirection = pStXd->btDirection;
    g_dXD_Queues[iEndPoint][iDirection] = pStXd ; 

    if(pStXd->usTotalLength == 0)
    {   // This is a zero length transfer. Initialize the buffer address to 0
        usArcPage = 0;
        usArcByteOffset = 0;
    }
    else
    {

        // Calculates the address of the data buffer in the arc space
        // Arc space is byte oriented with 4-byte words. The address on the dTDs
        // specifies a 4Kbyte page address and a byte offset in the page address
        // To convert the STMP address into a ARC address we must:
        //      1. Subtract STMP base address
        //          STMP_word_add = STMP_word_add - BASE_ADD
        //      2. Convert STMP 3-byte word in bytes
        //          ARC_byte_add = (STMP_word_add * 3) + STMP_byte_offset
        //      3. Calculate the 4k-page number
        //          ARC_page_number = ARC_byte_add >> 12  (Divide by 4K)
        //      4. Calculate ARC byte offset in page
        //          ARC_byte_offset = ARC_byte_add - (ARC_page_number * 0x1000) 
        //          
        
        // Phase 1 and 2
        //  STMP address is limited to 16 bits, therefore multiplying by 3 
        //  limits to 18 bits max. We will use a WORD for this math.
        wStmpByteAddress = (WORD) pStXd->pbtStartAddressWord;
        wStmpByteAddress -= (WORD)&dQH_Base_Add;
        wStmpByteAddress = (WORD)(wStmpByteAddress * 3) + pStXd->btStartOffsetByte;

        // Phase 3
        // The max size of the wStmpByteAddress is 18 bits, shifting right 12 bits
        // yields in a max size for usArcPage of 6 bits. We will use USHORT cast
        // because usArcPage will be casted later into a byte pointer.
        usArcPage = (USHORT)(wStmpByteAddress >> 12);

        // Phase 4
        usArcByteOffset = (USHORT)(wStmpByteAddress & 0xFFF);
    }

    // Get the address for the specific dTD
    pStdTD = dTD_Base_Scratch_Address[(pStXd->btEndpointNum<<1) + pStXd->btDirection];

    // Get the address for the specific dTD
    pdTD = dTD_Base_Address[(pStXd->btEndpointNum<<1) + pStXd->btDirection];

    // Initialize the dTD scratch 
    pStdTD->pbtNextdTDPointer = (BYTE *)0x00;     // Always one dTD per transfer
    pStdTD->usTotalBytes = pStXd->usTotalLength;  // Set the number of bytes to transfer
    pStdTD->usCurrentOffset = usArcByteOffset;     // Byte Offset
    pStdTD->pbtBufPage0 = (BYTE *)usArcPage;      // Set the page address
    pStdTD->pbtBufPage1 = (BYTE *)(usArcPage + 1);

    // We never use more than 12KBytes buffer so setting those pages are just a waste
    // of MIPS
//    pStdTD->pbtBufPage2 = (BYTE *)(usArcPage + 2);
//    pStdTD->pbtBufPage3 = (BYTE *)(usArcPage + 3);
//    pStdTD->pbtBufPage4 = (BYTE *)(usArcPage + 4);
                                                    // Activate the dTD and set the IOC bit
    pStdTD->usStatusIoc = (USB_EP_dTD_IOC | USB_EP_dTD_ACTIVE);

    // Pack the dTD
    FdTDPack(pStXd->btEndpointNum, pStXd->btDirection);

    // Convert dTD address to ARC address
    usArcdTD = (USHORT)(pdTD) - (USHORT)&dQH_Base_Add;
    usArcdTD *= 3;

    // Initialize the dQH for the specific endpoint
    dQH_Scratch_Cap.pbtNextdTDPointer = (BYTE *)usArcdTD;  

    // Pack the dQH 
    FdQHPackCap(pStXd->btEndpointNum, pStXd->btDirection, (BYTE)0x00);

    // Prime the endpoint
    write_usb_reg(HW_ARC_ENDPTPRIME,(DWORD)(1 << ((pStXd->btDirection<<4) + pStXd->btEndpointNum)));

    // This is risky. I did not have enough information from ARC about
    // delay between ENDPTPRIME to '0' and ENDPTSTAT to '1'.
    // Therefore do not include this code in the ROM
    
    if(pStXd->bIsDataPhase == TRUE)
    {
        // Add code here to handle the case where a setup packet arrives 
        // while priming the endpoint for the data phase on the previous
        // setup packet. Arc documentation requires:
        // " After priming the data phase packet, the device driver must verify a new
        //   setup packet has not been received by reading the ENPTSETUPSTAT register
        //   immediately verifying that the prime had completed. If a new packetis indicated
        //   after the ENDPTPRIME bit is cleared, then the transfer descriptor
        //   can be freed and the device driver must reinterpret the setup packet.
        
        do
        {
            // Wait for end of prime
            read_usb_reg(HW_ARC_ENDPTPRIME, &dwReg);
//            dwReg &= (DWORD)(1 << ((16 * pStXd->btDirection) + pStXd->btEndpointNum));
        }while(dwReg);
        
        // Check if new setup packet arrived
        wReg = read_usb_reg_16(HW_ARC_ENDPTSETUPSTAT);
        wReg &= (1 << pStXd->btEndpointNum);
        if(wReg)
        {
            // New setup packet arrived, free the dXD        
            pStXd->btStatus = USB_STATUS_TRANSFER_IDLE;
        }            
    }    

    return USB_OK;

} /* EndBody */




////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_set_test_mode
//
//   Type:          Functiorn
//
//   Description:   Prepares the arc core to execute USB test vector
//
//   Inputs:        
//                  usTest  : USB Vector test number 
//
//   Outputs:       none
//
//   Notes:
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_set_test_mode(USHORT usTest) 
{ /* Body */

    DWORD   dwRegister;
    
    // Reset the toggle bit
    read_usb_reg(HW_ARC_ENDPTCTRL(0), &dwRegister);
    dwRegister |= HW_ARC_ENDPTCTRL_TXR_SETMASK; 
    write_usb_reg(HW_ARC_ENDPTCTRL(0), dwRegister);

    if((usTest == (USHORT)USB_TEST_MODE_TEST_J) || (usTest == (USHORT)USB_TEST_MODE_TEST_K))
    {
        // This code was added for USB 2.0 compliance issues Test J and Test K
        // Levels must be less than 12 mV

        // Power Down Rx and Tx components that are not used in Test J and Test K modes
        // RXPWDRX to 1.
        // RXPWDDIFF to 1.
        // RXPWD1PT1 to 1.
        // RXPWDENV to 1.
        // RXPWDDISCONDET to 1.
        // TXPWDCOMP to 1.
        HW_USBPHYPWD.I |= 0x1f4000;
    
        // Selects crystal for DCLK instead of PLL
        HW_CCR.B.CKSRC = 0;
    
        // Runs UTMI and FS off DCLK instead of USB PLL
        HW_USBPHYRX.B.FSCKSOURCESEL = 0;
    
        // Slow Down USB PLL to its minimum frequency
        HW_USBPHYPLL.B.PLLCLKDIVSEL = 0x07;
        HW_USBPHYPLL.B.PLLCPNSEL = 0x07;
    }

    // Check if Test required is send test packet 
    if(usTest == (USHORT)USB_TEST_MODE_TEST_PACKET)
    {
        usb_device_send_data(0, (void _X *)TestPacket, 0, USB_TEST_MODE_TEST_PACKET_LENGTH, FALSE,000000);
    }        

    // Reset PORTSC[0] bits CSC, PEC, and OCC
    read_usb_reg(HW_ARC_PORTSC1, &dwRegister);
    dwRegister &= (DWORD)HW_ARC_PORTSC1_CSC_CLRMASK     // was HW_ARC_PORTSC1_PE_CLRMASK
                & (DWORD)HW_ARC_PORTSC1_PEC_CLRMASK
                & (DWORD)HW_ARC_PORTSC1_OCC_CLRMASK; 
    write_usb_reg(HW_ARC_PORTSC1, dwRegister);
    
    // Set test to execute in Arc Register
    dwRegister |= (DWORD)(usTest << HW_ARC_PORTSC1_PTC_BITPOS);
    write_usb_reg(HW_ARC_PORTSC1, dwRegister);
    
} /* EndBody */



// This function unpacks the 48-bit double word into 2 16-bit words and writes 
// it back to the ARC register
//void _reentrant write_usb_reg(USHORT usRegAdd, DWORD dwData)         
//{
//    WORD wOldIVL;
//    wOldIVL = SysMaskAllInterrupts();
//    HW_USBARCDATALOW.B.DATA = (WORD)(dwData & (WORD)(HW_USBARCDATALOW_DATA_SETMASK));
//    HW_USBARCDATAHIGH.B.DATA = (WORD)((dwData >> 16) & (DWORD)(HW_USBARCDATALOW_DATA_SETMASK));
//    usRegAdd &= (USHORT)HW_USBARCACCESS_ADD_SETMASK;
//    HW_USBARCACCESS.B.ADD = usRegAdd;
//    HW_USBARCACCESS.B.RWB = 0;
//    HW_USBARCACCESS.B.KICK = 1;         
//    SysUnMaskAllInterrupts(wOldIVL);
//}    


// This macro reads the ARC 32-bit register and packs it in a 48-bit double word
// Use this macro in combination with WRITE_USB_REG macro to read-modify-write
// a ARC register
//void _reentrant read_usb_reg(USHORT usRegAdd, DWORD * dwData)
//{
//    WORD wOldIVL;
//    wOldIVL = SysMaskAllInterrupts();
//    usRegAdd &= HW_USBARCACCESS_ADD_SETMASK;
//    HW_USBARCACCESS.B.ADD = usRegAdd;
//    HW_USBARCACCESS.B.RWB = 1;
//    HW_USBARCACCESS.B.KICK = 1;
//    while(HW_USBARCACCESS.B.KICK);
//    *dwData = (DWORD)(HW_USBARCDATALOW.B.DATA & HW_USBARCDATALOW_DATA_SETMASK);
//    *dwData |= (DWORD)(HW_USBARCDATAHIGH.B.DATA << 16);
//    SysUnMaskAllInterrupts(wOldIVL);
//}

// This function unpacks the 48-bit double word into 2 16-bit words and writes 
// it back to the ARC register
// Interrupts must be disabled prior to calling this function!
void _reentrant write_usb_reg(USHORT usRegAdd, DWORD dwData)         
{

    #pragma asm

    nolist
    include "sysmacro.asm"
    include "regsusb20.inc"
    include "regsusb20phy.inc"
    list
    
    push    a0
    push    a1
    push    b1
    
;    jsr     FSysMaskAllInterrupts
    move    a1,y0                   ; Saves old isr level
    
    pop     b1
    pop     a1
    pop     a0
    
    move    #>HW_USBARCACCESS_ADD_SETMASK,x0
    and     x0,b    #>$00ffff,x1
    move    b1,x:HW_USBARCACCESS
    
    move    a0,b1
    and     x1,b
    move    b,x:HW_USBARCDATALOW
    
    rep     #8
        asl     a

    and     x1,a
    move    a1,x:HW_USBARCDATAHIGH
    
    bclr    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

    move    y0,a1                   ; Restores old isr level
;    jsr     FSysUnMaskAllInterrupts

    #pragma endasm  

}    





// This function reads the ARC 32-bit register and packs it in a 48-bit double word
// Use this macro in combination with WRITE_USB_REG macro to read-modify-write
// a ARC register
// Interrupts must be disabled prior to calling this function!
void _reentrant read_usb_reg(USHORT usRegAdd, DWORD * dwData)
{
    #pragma asm

    nolist
    include "sysmacro.asm"
    include "regsusb20.inc"
    include "regsusb20phy.inc"
    list
    
    push    a1
    push    r0
    
;    jsr     FSysMaskAllInterrupts
    move    a1,y0                   ; Saves old isr level
    
    pop     r0
    pop     a1
    
    ;Set Address, R/W/ Bit and kick transaction
    move    #>HW_USBARCACCESS_ADD_SETMASK,x0
    and     x0,a
    move    a1,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

    ;Wait end of transaction
_Loop
    move    x:HW_USBARCACCESS,a0
    jset    #HW_USBARCACCESS_KICK_BITPOS,a0,_Loop

    ;32-bit word ARC Register is split into 2 16-bit words
    ;Unify those 2 16-bit words in a DWORD result 
    clr     a   #>$00ffff,x0       
    move    x:HW_USBARCDATAHIGH,a1      ;a1=XHH
    rep     #8
        asr     a                       ;a1a0=XXHH00

    move    x:HW_USBARCDATALOW,b1       ;b1=XLL
    and     x0,b    a0,x1               ;b1=0LL    x1=H00
    or      x1,b    #>$0000ff,x0        ;b1=HLL
    and     x0,a                        ;a1=00H

    move    b1,y:(r0)+
    move    a1,y:(r0)    

    move    y0,a1                   ; Restores old isr level
;    jsr     FSysUnMaskAllInterrupts

    #pragma endasm  

}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          read_and_clear_usb_reg
//
//   Type:          Functiorn
//
//   Description:   Reads and clears the ARC register
//
//   Inputs:        
//                  usRegAdd = RAC Register Address to read and clear
//                  *dwData  = Pointer to variable to store ARC Register Value
//
//   Outputs:       
//                  none
//
//   Notes:
//                  This function was designed to speed up the USB ISR
//                  Do not call this function outside the ISR.
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant read_and_clear_usb_reg(USHORT usRegAdd, DWORD * dwData)
{
    #pragma asm

    nolist
    include "sysmacro.asm"
    include "regsusb20.inc"
    include "regsusb20phy.inc"
    list
    
    ;Set Address, R/W/ Bit and kick transaction
    move    #>HW_USBARCACCESS_ADD_SETMASK,x0
    and     x0,a
    move    a1,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

    ;Wait end of transaction
_Loop
    move    x:HW_USBARCACCESS,a0
    jset    #HW_USBARCACCESS_KICK_BITPOS,a0,_Loop

    clr     a   #>$00ffff,x0       
    move    x:HW_USBARCDATAHIGH,a1      ;a1=XHH
    and     x0,a
    
    move    x:HW_USBARCDATALOW,b1       ;b1=XLL
    and     x0,b    

    move    b1,y:(r0)+
    move    a1,y:(r0)    

    ; Clear the arc register
    bclr    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

    #pragma endasm  

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          read_and_clear_arc_reg_usbsts
//
//   Type:          Functiorn
//
//   Description:   Reads and clears the ARC register USBSTATUS
//
//   Inputs:        
//                  none
//
//   Outputs:       
//                  Returns the ARC register USBSTATUS prior clearing it
//
//   Notes:
//                  This function was designed to speed up the USB ISR
//                  Do not call this function outside the ISR.
//<
////////////////////////////////////////////////////////////////////////////////
WORD _reentrant read_and_clear_arc_reg_usbsts(void)
{
    #pragma asm

    nolist
    include "sysmacro.asm"
    include "regsusb20.inc"
    include "regsusb20phy.inc"
    list

    ;Set Address, R/W/ Bit and kick transaction
    move    #>HW_USBARCACCESS_ADD_SETMASK,x0
    move    #>HW_ARC_USBSTS,a
    and     x0,a
    move    a1,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

    ;Wait end of transaction
_Loop
    move    x:HW_USBARCACCESS,a0
    jset    #HW_USBARCACCESS_KICK_BITPOS,a0,_Loop

    clr     a  #$00ffff,x0
    move    x:HW_USBARCDATALOW,a1
    and     x0,a
        
    ; Clear the arc register
    bclr    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

    #pragma endasm  

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          read_usb_reg_16
//
//   Type:          Functiorn
//
//   Description:   Returns the 16 LSBits from the specific ARC Register
//
//   Inputs:        
//                  usRegAdd    Address ARC Register to read
//
//   Outputs:       
//                  16 LSBits from the specific ARC Register
//
//   Notes:
//                  This function was designed to speed up the USB ISR
//                  Do not call this function outside the ISR.
//<
////////////////////////////////////////////////////////////////////////////////
WORD _reentrant read_usb_reg_16(USHORT usRegAdd)
{
    #pragma asm

    nolist
    include "sysmacro.asm"
    include "regsusb20.inc"
    include "regsusb20phy.inc"
    list
    
    ;Set Address, R/W/ Bit and kick transaction
    move    #>HW_USBARCACCESS_ADD_SETMASK,x0
    and     x0,a
    move    a1,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_RWB_BITPOS,x:HW_USBARCACCESS
    bset    #HW_USBARCACCESS_KICK_BITPOS,x:HW_USBARCACCESS

    ;Wait end of transaction
_Loop
    move    x:HW_USBARCACCESS,a0
    jset    #HW_USBARCACCESS_KICK_BITPOS,a0,_Loop

    clr     a   #$00ffff,x0
    move    x:HW_USBARCDATALOW,a1     
    and     x0,a

    #pragma endasm  

}





void _reentrant usb_dci_finish_transaction(BYTE btEndpoint)
{
    g_SetupData[btEndpoint].bValid=FALSE;
}


void _reentrant usb_dci_update_connection_status(void)
{

    BOOL bSetBrownOut = FALSE;
    
    // If VDDIO Auto Brownout is set, disable it to sense 5V 
    if(HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT)
    {
        bSetBrownOut = TRUE;
        HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT = 0;
    }     

   // Check if power source is +5V
   // If power is not from +5V => USB is not connected.
   // If power is from +5V => USB connected or Charger Connected
    if(!HW_USBCSR.B.VBUSSENSE)
    {
        stDeviceState.usConnectionState = USB_DISCONNECTED;
    }
    else
    {
        // Enable USB plug in circuit
        HW_USBCSR.B.PLUGGEDIN_EN = 1;
        
        #pragma asm
            ; The circuitry takes up to 1Usec to stabilize, which is 24 dclks
            ; at 24Mhz
            rep #24
                nop
        #pragma endasm
        
        if(HW_USBCSR.B.PLUGGEDIN)
        {
            stDeviceState.usConnectionState = USB_CONNECTED;
        }
        else
        {
            stDeviceState.usConnectionState = USB_DISCONNECTED;
        }
    }

    // Restore VDDIO Auto Brown Out.
    if(bSetBrownOut)
    {
        HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT = 1;
    }     

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_shutdown
//
//   Type:          Function
//
//   Description:   Shut down the USB port
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_shutdown(void)
{

  HW_USBCSR.B.USBEN = 0;
  HW_USBCSR.B.CLKOFF = 0;
  HW_USBCSR.B.USBEN = 1;
  
  // Turns OFF ARC Core
  HW_USBCSR.B.ARCCONNECT = 0;
  
  // Disables the USB Clock
  HW_USBCSR.B.CLKOFF = 1;
  
  // Disable USB STMP
  HW_USBCSR.B.USBEN = 0;
  

}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_chip_init
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
//#define USBTEST_FOR_USBCV_1_3
#ifdef USBTEST_FOR_USBCV_1_3
void usb_dci_Reset_Bulk_Endpoint(void)
{
    DWORD    dwRegister;
    usb_dci_clear_dxd_queue(1,USB_IN);
    usb_dci_clear_dxd_queue(2,USB_OUT);
    usb_dci_clear_dxd_queue(1,USB_OUT);
    usb_dci_clear_dxd_queue(2,USB_IN);

    /* Clear all the endpoint complete status bits */
    write_usb_reg(HW_ARC_ENDPTCOMPLETE,(DWORD)( (1 <<((1<<4) + 1)) + (1 << 2) ) );
    // Cancel all primed status by waiting untill all bits in the ENDPTPRIME are 0
    // then writing 0xffffffff to ENDPTFLUSH
    do
    {
        // Wait until all ENDPTPRIME bits cleared
        read_usb_reg(HW_ARC_ENDPTPRIME, &dwRegister);
    }while(dwRegister & ((DWORD)( (1 <<((1<<4) + 1)) + (1 << 2) )));


    /* Write 1s to the Flush register */
    write_usb_reg(HW_ARC_ENDPTFLUSH, (DWORD)( (1 <<((1<<4) + 1)) + (1 << 2) ) );
    // Set all transfer structures to idle
    dXD_Base_Address[1*2]->btStatus = USB_STATUS_TRANSFER_IDLE;
    dXD_Base_Address[(1*2) + 1]->btStatus = USB_STATUS_TRANSFER_IDLE;
    dXD_Base_Address[2*2]->btStatus = USB_STATUS_TRANSFER_IDLE;
    dXD_Base_Address[(2*2) + 1]->btStatus = USB_STATUS_TRANSFER_IDLE;

    usb_dci_unstall_endpoint(1,USB_OUT);
    usb_dci_unstall_endpoint(1,USB_IN);
    usb_dci_unstall_endpoint(2,USB_OUT);
    usb_dci_unstall_endpoint(2,USB_IN);
}
#endif

