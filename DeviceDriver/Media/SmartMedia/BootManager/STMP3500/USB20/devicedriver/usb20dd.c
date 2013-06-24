////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
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
#include "usb20devapi.h"
#include "usb20ddprv.h"
#include "regs3500usb20.h"

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////
volatile BOOL  bInIsr;

////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

extern dQH_Struct_Cap _X dQH_Scratch_Cap;
extern dQH_Struct_Setup _X dQH_Scratch_Setup_Buf;
extern dTD_Struct _X * _X dTD_Base_Scratch_Address[];
extern Dev_State_Struct _X stDeviceState;
extern xd_struct _X * _X dXD_Base_Address[];
extern BYTE _X * _X dQH_Base_Add;
extern BYTE _X * _X dTD_Base_Address[];

 
////////////////////////////////////////////////////////////////////////////////
//  Equates
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////
#ifndef BOOT_ROM
#ifndef USE_EEPROM
#define  USB_TEST_MODE_TEST_PACKET_LENGTH   (53)

/* Test packet for Test Mode : TEST_PACKET. USB 2.0 Specification section 7.1.20 */
_packed BYTE _X TestPacket[USB_TEST_MODE_TEST_PACKET_LENGTH] = 
{
   /* Synch */
   /* DATA 0 PID */
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
   0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 
   0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 
   0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF, 
   0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF, 
   0xEF, 0xF7, 0xFB, 0xFD, 0x7E
};
#endif
#endif

#ifdef USE_EEPROM
#define TT_OPTIMIZE_SIZE
#endif

USHORT _X usDevDriverSendData;
WORD _X wUsbIsrSaveSp;

union 
{
   WORD _X w;
   DWORD _X dw;
} TempRegister1;

//WORD  _X wTempRegister1;

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////
#ifdef  SIM_USB20

// Structure defining all ARC registers for simulation only
arc_reg_struct  stArcSoftRegs = {
    (DWORD)0x01100001,      // SoftArcHcsParams
    (DWORD)0x00080b00,      // SoftArcUsbCmd
    (DWORD)0x00000000,      // SoftArcUsbSts
    (DWORD)0x00000000,      // SoftArcUsbIntr
    (DWORD)0x00000000,      // SoftArcDevAddr
    (DWORD)0x00000000,      // SoftArcEndptListAddr
    (DWORD)0x00001c04,      // SoftArcPortsc1
    (DWORD)0x00000002,      // SoftArcUsbMode
    (DWORD)0x00000000,      // SoftArcEndptSetupStat
    (DWORD)0x00000000,      // SoftArcEndptPrime
    (DWORD)0x00000000,      // SoftArcEndptFlush
    (DWORD)0x00000000,      // SoftArcEndptStatus
    (DWORD)0x00000000      // SoftArcEndptComplete
};

#endif

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////

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
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _reentrant usb_dci_init(Dev_State_Struct _X * pStDeviceState)
{
   
   /* Install the interrupt service routine */
//   USB_install_isr(usb_dev_ptr->DEV_VEC, _usb_dci_vusb20_isr, NULL);
   
   pStDeviceState->usUsbState = USB_STATE_UNKNOWN;

   /* Initialize the VUSB_HS controller */   
   usb_dci_chip_init(pStDeviceState);
   
   return USB_OK;   
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_dci_chip_init
//
//   Type:          Function
//
//   Description:   Initializes the USB device controller
//
//   Inputs:        pStDeviceState = Pointer to the device state structure
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant usb_dci_chip_init(Dev_State_Struct _X * pStDeviceState)
{ /* Body */
    USHORT      usEndpointCounter;

//    DWORD       dwTempRegister;
    // Enable the ARC Core
    HW_USBCSR.B.ARCCONNECT = 1;

      /* Stop the controller */
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcUsbCmd), &TempRegister1.dw);
   #else
       read_usb_reg(HW_ARC_USBCMD, &TempRegister1.dw);
   #endif
   
#ifndef TT_OPTIMIZE_SIZE
     dwTempRegister &= HW_ARC_USBCMD_RS_CLRMASK; 
#else
     TempRegister1.w &= HW_ARC_USBCMD_RS_CLRMASK; 
#endif

   
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcUsbCmd), TempRegister1.dw);
   #else
       write_usb_reg(HW_ARC_USBCMD, TempRegister1.dw);
   #endif
         
   /* Reset the controller to get default values */
#ifndef TT_OPTIMIZE_SIZE
   dwTempRegister |= HW_ARC_USBCMD_RST_SETMASK;
#else
   TempRegister1.w |= HW_ARC_USBCMD_RST_SETMASK; 
#endif
   
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcUsbCmd), TempRegister1.dw);
   #else
      write_usb_reg(HW_ARC_USBCMD, TempRegister1.dw);
   #endif
   
   #ifdef SIM_USB20
   
       // Do nothing
   
   #else   
           
      do
      {
           // Wait for the controller reset to complete

           read_usb_reg(HW_ARC_USBCMD, &TempRegister1.dw);
#ifndef TT_OPTIMIZE_SIZE
      }while(TempRegister1.dw & HW_ARC_USBCMD_RST_SETMASK);
#else
      }while(TempRegister1.w & HW_ARC_USBCMD_RST_SETMASK);
#endif
   
   #endif   
   
   #ifdef USE_EEPROM  // BOOT EEPROM forces connection at full speed 
   read_usb_reg(HW_ARC_PORTSC1, &TempRegister1.dw);
   // Forces a full speed connect only for the boot rom
   TempRegister1.dw |= (DWORD)HW_ARC_PORTSC1_PFSC_SETMASK;
   #else
   
      #ifdef BOOT_ROM
          // BOOT ROM forces connection at full speed    
         #ifdef SIM_USB20
             read_usb_reg(&(stArcSoftRegs.SoftArcPortsc1), &TempRegister1.dw);
         #else
             read_usb_reg(HW_ARC_PORTSC1, &TempRegister1.dw);
         #endif
      #endif
      // Forces a full speed connect only for the boot rom
      TempRegister1.dw |= (DWORD)HW_ARC_PORTSC1_PFSC_SETMASK;
   #endif      // USE_EEPROM

   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcPortsc1), TempRegister1.dw);
   #else
       write_usb_reg(HW_ARC_PORTSC1, TempRegister1.dw);
   #endif
   
   
      // Program the endpoint list address
      // dQHs structures are always the 1st in our memory map,
      // therefore the endpoint list address is set to 0x0000. 
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptListAddr), (DWORD)0x00);
   #else
       write_usb_reg(HW_ARC_ENDPTLISTADDR, (DWORD)0x00); 
   #endif
   
      /* Program the controller to be the USB device controller */
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcUsbMode), (DWORD)0x02);
   #else
       write_usb_reg(HW_ARC_USBMODE, (DWORD)0x02); 
   #endif
      
       // Zero out the USB structures (dQHs, dTDs and dXDs)  
       FInitUsbStructures();
      
      // Make sure the setup endpoint status are cleared
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptSetupStat), (DWORD)0x00);
   #else
       write_usb_reg(HW_ARC_ENDPTSETUPSTAT, (DWORD)0x00);
   #endif  
      
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
           FdQHPackCap(usEndpointCounter, (USHORT)USB_RECV, (BYTE)0x01);
           FdQHPackCap(usEndpointCounter, (USHORT)USB_SEND, (BYTE)0x01);
       } // Endfor      
   
      /* Initialize the endpoint 0 properties */
      // Reset the Toggle bit
      // Make sure that the enpoint is not stall
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[0]), &TempRegister1.dw);
   #else
      read_usb_reg(HW_ARC_ENDPTCTRL(0), &TempRegister1.dw);
   #endif
   
#ifndef TT_OPTIMIZE_SIZE
      TempRegister1.dw = TempRegister1.dw 
                       | (DWORD)HW_ARC_ENDPTCTRL_TXR_SETMASK
                       | (DWORD)HW_ARC_ENDPTCTRL_RXR_SETMASK;
   
      TempRegister1.dw = TempRegister1.dw 
                       & (DWORD)HW_ARC_ENDPTCTRL_TXS_CLRMASK 
                       & (DWORD)HW_ARC_ENDPTCTRL_RXS_CLRMASK;
#else
      TempRegister1.w |= (HW_ARC_ENDPTCTRL_TXR_SETMASK | HW_ARC_ENDPTCTRL_RXR_SETMASK);
      TempRegister1.w &= (HW_ARC_ENDPTCTRL_TXS_CLRMASK | HW_ARC_ENDPTCTRL_RXS_CLRMASK);
#endif
   
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[0]), TempRegister1.dw);
   #else
       write_usb_reg(HW_ARC_ENDPTCTRL(0), TempRegister1.dw);                                                     
   #endif
      
      /* Enable interrupts */
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcUsbIntr), &TempRegister1.dw);
   #else
      read_usb_reg(HW_ARC_USBINTR, &TempRegister1.dw);
   #endif
   
#ifndef TT_OPTIMIZE_SIZE
      TempRegister1.dw = TempRegister1.dw
                       | (DWORD)HW_ARC_USBINTR_UE_SETMASK
                       | (DWORD)HW_ARC_USBINTR_UEE_SETMASK
                       | (DWORD)HW_ARC_USBINTR_PCE_SETMASK
                       | (DWORD)HW_ARC_USBINTR_URE_SETMASK
                       | (DWORD)HW_ARC_USBINTR_SLE_SETMASK;
#else
      TempRegister1.w |= (HW_ARC_USBINTR_UE_SETMASK | HW_ARC_USBINTR_UEE_SETMASK |
                        HW_ARC_USBINTR_PCE_SETMASK | HW_ARC_USBINTR_URE_SETMASK |
                        HW_ARC_USBINTR_SLE_SETMASK);
#endif
                               
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcUsbIntr), TempRegister1.dw);
   #else
       write_usb_reg(HW_ARC_USBINTR, TempRegister1.dw);
   #endif
   
       pStDeviceState->usUsbState = (USHORT)USB_STATE_UNKNOWN;
      
      /* Set the Run bit in the command register */
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcUsbCmd), (DWORD)HW_ARC_USBCMD_RS_SETMASK);
   #else
       write_usb_reg(HW_ARC_USBCMD, (DWORD)HW_ARC_USBCMD_RS_SETMASK); 
   #endif
   
   
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
/*
    #pragma asm
    extern  FwUsbIsrSaveSp
    extern  SaveSystemStack

    move    #_ReturnFromHardwareStackSave,r6 ; return address
    move    #>FwUsbIsrSaveSp,r5
    jmp     SaveSystemStack
_ReturnFromHardwareStackSave
    #pragma endasm

*/

    #pragma asm
        ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
        ; This will be returned to its previous state when the RTI occurs (since the SR
        ; will be popped back off System Stack Low (SSL).
        ; Accumulator behavior is unpredictable when the DM bit is set.
        ; Next cycle after clearing it should NOT be an ALU instruction.
        andi #$BF,mr
    #pragma endasm  
      
//   DWORD    dwStatus;
   DWORD    dwMask;

    bInIsr = TRUE;

      for (;;) 
      {
   #ifdef SIM_USB20
           read_usb_reg(&(stArcSoftRegs.SoftArcUsbSts), &TempRegister1.dw);
   #else
           read_usb_reg(HW_ARC_USBSTS, &TempRegister1.dw);
   #endif
         
         // Check for any pending interrupt
   #ifdef SIM_USB20
           read_usb_reg(&(stArcSoftRegs.SoftArcUsbIntr), &dwMask);
   #else
           read_usb_reg(HW_ARC_USBINTR, &dwMask);
   #endif
   
         if (!(TempRegister1.dw & dwMask))
         {
            break;             // No more interruption to process
         } /* Endif */
      
         /* Clear all the interrupts occured */
   #ifdef SIM_USB20
           write_usb_reg(&(stArcSoftRegs.SoftArcUsbSts), (DWORD)(0x00));
   #else
           write_usb_reg(HW_ARC_USBSTS, TempRegister1.dw);
   #endif
   
#ifndef TT_OPTIMIZE_SIZE      
         if (TempRegister1.dw & (DWORD)HW_ARC_USBSTS_URI_SETMASK) 
         {
            usb_dci_process_reset(&stDeviceState);
         } /* Endif */
      
         if (TempRegister1.dw & (DWORD)HW_ARC_USBSTS_PCI_SETMASK) 
         {
            usb_dci_process_port_change(&stDeviceState);
         } /* Endif */
      
         if (TempRegister1.dw & (DWORD)HW_ARC_USBSTS_UEI_SETMASK) 
         {
            (stDeviceState.wError)++;
         } /* Endif */
      
         if (TempRegister1.dw & (DWORD)HW_ARC_USBSTS_UI_SETMASK)
         {
            usb_dci_process_tr_complete(&stDeviceState);
         } /* Endif */
         
         if (TempRegister1.dw & (DWORD)HW_ARC_USBSTS_SLI_SETMASK)
         {
            usb_dci_process_suspend(&stDeviceState);
            break;
         } /* Endif */
         
         if (TempRegister1.dw & (DWORD)HW_ARC_USBSTS_SRI_SETMASK)
         {
            // This should not occur. Interruption is disabled during initialization
   
         } /* Endif */
#else

         if (TempRegister1.w & HW_ARC_USBSTS_URI_SETMASK) 
         {
            usb_dci_process_reset(&stDeviceState);
         } /* Endif */
      
         if (TempRegister1.w & HW_ARC_USBSTS_PCI_SETMASK) 
         {
            usb_dci_process_port_change(&stDeviceState);
         } /* Endif */
      
         if (TempRegister1.w & HW_ARC_USBSTS_UEI_SETMASK) 
         {
            (stDeviceState.wError)++;
         } /* Endif */
      
         if (TempRegister1.w & HW_ARC_USBSTS_UI_SETMASK)
         {
            usb_dci_process_tr_complete(&stDeviceState);
         } /* Endif */
         
         if (TempRegister1.w & HW_ARC_USBSTS_SLI_SETMASK)
         {
            usb_dci_process_suspend(&stDeviceState);
            break;
         } /* Endif */
         
         if (TempRegister1.w & HW_ARC_USBSTS_SRI_SETMASK)
         {
            // This should not occur. Interruption is disabled during initialization
   
         } /* Endif */
            
#endif
       } /* Endfor */
    bInIsr = FALSE;


    // Restores the hardware stack
/*
    #pragma asm
    extern  RestoreSystemStack
    
    move    #_ReturnFromHardwareStackRestore,r6
    move    #>FwUsbIsrSaveSp,r5
    jmp     RestoreSystemStack
_ReturnFromHardwareStackRestore
    #pragma endasm
*/
    
} /* EndBody */




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
void _reentrant usb_dci_process_suspend(Dev_State_Struct _X * pStDeviceState)
{ /* Body */

#ifndef BOOT_ROM
#ifndef USE_EEPROM
    if(pStDeviceState->usUsbState != (USHORT)USB_STATE_SUSPEND)
    {    
        // Save the USB state prior the suspend, so we can restore it
        // after a resume
        pStDeviceState->usUsbStatePriorSusp = pStDeviceState->usUsbState;
        pStDeviceState->usUsbState = (USHORT)USB_STATE_SUSPEND;
   
        usb_device_call_service(USB_SERVICE_SLEEP, FALSE, 0, 0, 0);
    }  
#endif
#endif

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
void _reentrant usb_dci_process_port_change(Dev_State_Struct _X * pStDeviceState)
{ /* Body */

//    DWORD   dwStatus;
//    DWORD   dwReg;
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
   #ifdef SIM_USB20
               read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), &TempRegister1.dw);
   #else
               read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &TempRegister1.dw);   
   #endif
#ifndef TT_OPTIMIZE_SIZE         
               TempRegister1.dw = TempRegister1.dw | ((DWORD)HW_ARC_ENDPTCTRL_RXE_SETMASK
                             | (DWORD)HW_ARC_ENDPTCTRL_TXE_SETMASK);
#else
               TempRegister1.w |=  (HW_ARC_ENDPTCTRL_RXE_SETMASK | 
                                     HW_ARC_ENDPTCTRL_TXE_SETMASK);
#endif
         
   #ifdef SIM_USB20
           write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), TempRegister1.dw);
   #else
           write_usb_reg((HW_ARC_ENDPTCTRL(btEndpoint)), TempRegister1.dw);
   #endif
        } // Endfor
        
    } /* Endif */

    // Check if reset is still present on the bus
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcPortsc1), &TempRegister1.dw);
   #else
       read_usb_reg(HW_ARC_PORTSC1, &TempRegister1.dw);
   #endif
#ifndef TT_OPTIMIZE_SIZE   
       if (!(TempRegister1.dw & HW_ARC_PORTSC1_PR_SETMASK)) 
       {   // Reset completed, check which speed we will operate at
           if (TempRegister1.dw & HW_ARC_PORTSC1_HSP_SETMASK) 
               pStDeviceState->btSpeed = (BYTE)USB_SPEED_HIGH;
           else    
               pStDeviceState->btSpeed = (BYTE)USB_SPEED_FULL;
         
           usb_device_call_service(USB_SERVICE_SPEED_DETECTION, FALSE, 0, 0, (WORD)pStDeviceState->btSpeed);
   
       } /* Endif */
#else  
       if (!(TempRegister1.w & HW_ARC_PORTSC1_PR_SETMASK)) 
       {   // Reset completed, check which speed we will operate at
           if (TempRegister1.w & HW_ARC_PORTSC1_HSP_SETMASK) 
               pStDeviceState->btSpeed = (BYTE)USB_SPEED_HIGH;
           else    
               pStDeviceState->btSpeed = (BYTE)USB_SPEED_FULL;
         
           usb_device_call_service(USB_SERVICE_SPEED_DETECTION, FALSE, 0, 0, (WORD)pStDeviceState->btSpeed);
   
       } // Endif
#endif            
   

#ifndef BOOT_ROM
#ifndef USE_EEPROM

    // Port change detect could have been generated by a SUSPEND state
    if (TempRegister1.dw & (DWORD)HW_ARC_PORTSC1_SUSP_SETMASK) 
    {   // If so, suspend the system

        // Save the USB state prior the SUSPEND, to restore after a resume
        // Set the state of USB to SUSPEND
        if(pStDeviceState->usUsbState != USB_STATE_SUSPEND)
        { 
            pStDeviceState->usUsbStatePriorSusp = pStDeviceState->usUsbState;
            pStDeviceState->usUsbState = USB_STATE_SUSPEND;
        }        
      
      usb_device_call_service(USB_SERVICE_SUSPEND, FALSE, 0, 0, 0);
    } /* Endif */

    // Port change detect could have been generated by leaving a SUSPEND state
    if ((!(TempRegister1.dw & (DWORD)HW_ARC_PORTSC1_SUSP_SETMASK)) && (pStDeviceState->usUsbState == USB_STATE_SUSPEND)) 
    {
        // Resume the service
        pStDeviceState->usUsbState = pStDeviceState->usUsbStatePriorSusp;
        usb_device_call_service(USB_SERVICE_RESUME, FALSE, 0, 0, 0);
    } /* Endif */

#endif
#endif

} /* EndBody */



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
void _reentrant usb_dci_process_reset(Dev_State_Struct _X * pStDeviceState)
{ /* Body */
   DWORD    dwRegister;
   BYTE     btEndpoint;
   
   /* Inform the application so that it can cancel all previously queued transfers */
   usb_device_call_service(USB_SERVICE_BUS_RESET, FALSE, 0, 0, 0);
   
   // Clear the device address register
   // The address bits are past bit 25-31. Set the address
#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcDevAddr), &dwRegister);
#else
   read_usb_reg(HW_ARC_DEVADDR, &dwRegister);
#endif

   dwRegister = dwRegister & 0x01ffffff;

#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcDevAddr), dwRegister);
#else
    write_usb_reg(HW_ARC_DEVADDR, dwRegister);
#endif

    // Update the device state structure
    pStDeviceState->btAddress = (BYTE)0x00;
    
#ifndef TT_OPTIMIZE_SIZE   
      /* Clear all the setup token semaphores */
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptSetupStat), &dwRegister);
   #else
       read_usb_reg(HW_ARC_ENDPTSETUPSTAT, &dwRegister);   
   #endif
   
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptSetupStat), dwRegister);
   #else
       write_usb_reg(HW_ARC_ENDPTSETUPSTAT, dwRegister);   
   #endif
#else
      /* Clear all the setup token semaphores */   
       read_usb_reg(HW_ARC_ENDPTSETUPSTAT, &TempRegister1.dw);
      
       write_usb_reg(HW_ARC_ENDPTSETUPSTAT, TempRegister1.dw);
#endif      

   /* Clear all the endpoint complete status bits */   
#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcEndptComplete), &dwRegister);
#else
    read_usb_reg(HW_ARC_ENDPTCOMPLETE, &dwRegister);
#endif

#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcEndptComplete), dwRegister);
#else
    write_usb_reg(HW_ARC_ENDPTCOMPLETE, dwRegister);
#endif

   // Cancel all primed status by waiting untill all bits in the ENDPTPRIME are 0
   // then writing 0xffffffff to ENDPTFLUSH
    do
    {
        // Wait until all ENDPTPRIME bits cleared
#ifdef SIM_USB20
        read_usb_reg(&(stArcSoftRegs.SoftArcEndptPrime), &dwRegister);
#else
        read_usb_reg(HW_ARC_ENDPTPRIME, &dwRegister);
#endif
    }while(dwRegister & (DWORD)0xffffffffffff);


   /* Write 1s to the Flush register */
#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcEndptFlush), (DWORD)0xffffffffffff);
#else
    write_usb_reg(HW_ARC_ENDPTFLUSH, (DWORD)0xffffffffffff);
#endif
   
    // Read the reset bit in the PORTSCx register and make sure that it is still active
    // A USB reset will occur for a minimum of 3ms and the device driver software
    // must reach this point in the reset cleanup before the end of the reset occurs
    // Otherwise a hardware reset of the device controller is recommended (rare)
#ifdef TT_OPTIMIZE_SIZE
   #ifdef SIM_USB20
   
         pStDeviceState->btBusResetting = TRUE;
         pStDeviceState->usUsbState = USB_STATE_POWERED;
   
   #else
       read_usb_reg(HW_ARC_PORTSC1, &dwRegister);
       
      if (dwRegister & (DWORD)HW_ARC_PORTSC1_PR_SETMASK) 
      {
         pStDeviceState->btBusResetting = TRUE;
         pStDeviceState->usUsbState = USB_STATE_POWERED;
      } 
      else 
      { 
         /* re-initialize */      
         usb_dci_chip_init(pStDeviceState);
         return;
      } /* Endif */
      
   #endif
#else   
       read_usb_reg(HW_ARC_PORTSC1, &TempRegister1.dw);
       
      if (TempRegister1.w & HW_ARC_PORTSC1_PR_SETMASK) 
      {
         pStDeviceState->btBusResetting = TRUE;
         pStDeviceState->usUsbState = USB_STATE_POWERED;
      } 
      else 
      { 
         /* re-initialize */      
         usb_dci_chip_init(pStDeviceState);
         return;
      } /* Endif */      
#endif      

    // Set all transfer structures to idle
    for(btEndpoint = 0 ; btEndpoint < MAX_NUM_EP ; btEndpoint++)
    {    
        dXD_Base_Address[btEndpoint*2]->btStatus = USB_STATUS_TRANSFER_IDLE;
        dXD_Base_Address[(btEndpoint*2) + 1]->btStatus = USB_STATUS_TRANSFER_IDLE;
    }
    
// I am really not sure about this here ....
// Waiting answer from ARC
//   usb_device_call_service(USB_SERVICE_BUS_RESET, FALSE, 0, 0, 0);

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
void _reentrant usb_dci_process_tr_complete(Dev_State_Struct _X * pStDeviceState)
{ /* Body */

   DWORD        dwRegSetup;
   DWORD        dwRegComplete;
   void (*ProcessFirst)(DWORD, DWORD);
   void (*ProcessLast)(DWORD, DWORD);
   
    // We use separate loops for SETUP packets and other packet types.
    // The setup packet is processed first. It must be done as soon as possible
    
    // Processing the setup transaction .......
#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcEndptSetupStat), &dwRegSetup);
#else
    read_usb_reg(HW_ARC_ENDPTSETUPSTAT, &dwRegSetup); // Reads the setup status register
#endif
    
    // Processing the non-setup transaction .......
#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcEndptComplete), &dwRegComplete);
#else
    read_usb_reg(HW_ARC_ENDPTCOMPLETE, &dwRegComplete);    // Reads the end point complete register
#endif
    
    /* Clear the bits in the register */
#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcEndptComplete), (DWORD)0x00);
#else
    write_usb_reg(HW_ARC_ENDPTCOMPLETE, dwRegComplete);     // Clears the end point complete register
#endif  

    if((dwRegComplete & dwRegSetup) || ((dwRegComplete >> 16) & dwRegSetup))
    {
        ProcessFirst = &ProcessEndptComplete;
        ProcessLast = &ProcessEndptSetup;
    }
    else
    {
        ProcessFirst = &ProcessEndptSetup;
        ProcessLast = &ProcessEndptComplete;
    }
    
    (*ProcessFirst)(dwRegComplete, dwRegSetup);
    (*ProcessLast)(dwRegComplete, dwRegSetup);
    
}    
     
#ifndef TT_OPTIMIZE_SIZE
    
void _reentrant ProcessEndptSetup(DWORD dwRegComplete, DWORD dwRegSetup)
{
    BYTE         btEndpointNumber;

    // a setup packet pending will have its bit set to '1'
    if(dwRegSetup)
    { // Setup packets to process
        // for each endpoint define
        for(btEndpointNumber = 0 ; btEndpointNumber < MAX_NUM_EP ; btEndpointNumber++)
        {
            // Do we have a setup packet pending
            // If we have a setup packet pending do not clear the endpoint setup status
            // bit in the register. We need to keep the setup lockout enable until we
            // read the setup bytes. Clearing the bit is done there.
            if((WORD)dwRegSetup & ((WORD)(1 << btEndpointNumber)))
                usb_device_call_service(btEndpointNumber, TRUE, 0, 0, NUMBER_BYTES_IN_SETUP);  // calls the service routine
        }    
    }
}
    
   
void _reentrant ProcessEndptComplete(DWORD dwRegComplete, DWORD dwRegSetup)
{   
    BYTE         btEndpointNumber;
    BYTE         btEndpoint, btDirection;
    dTD_Struct _X * pStdTD;
    xd_struct  _X * pStXd;
    USHORT       usActualBytesTransfer;

    if (dwRegComplete)
    { // non setup packets to process
        // For each endpoint define
        for (btEndpointNumber = 0 ; btEndpointNumber < 32 ; btEndpointNumber++)
        {
            if (dwRegComplete & ((DWORD)(1 << btEndpointNumber)))
            {   // We have something , found which direction and endpoint
                if (btEndpointNumber > 15) 
                {
                    btEndpoint = btEndpointNumber - 16;
                    btDirection = USB_SEND;
                }
                else
                {
                    btEndpoint = btEndpointNumber;
                    btDirection = USB_RECV;
                } /* Endif */

                // Extract the specified dTD
                FdTDUnpack((USHORT)btEndpoint, btDirection);
                pStdTD = dTD_Base_Scratch_Address[(2*btEndpoint) + btDirection];
                pStXd = dXD_Base_Address[(2*btEndpoint) + btDirection]; 

                // Check if the dTD is still active
                if(pStdTD->usStatusIoc & (USHORT)USB_EP_dTD_ACTIVE)
                {
                    pStXd->btStatus = USB_STATUS_TRANSFER_IN_PROGRESS;
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
                        pStXd->btStatus = USB_STATUS_TRANSFER_ERROR;
                    }
                    else
                    {   // Transfer successful
                        pStXd->btStatus = USB_STATUS_TRANSFER_SUCCESS;
                    }                    
                
                    // Calculate the actual number of bytes transfered
                    usActualBytesTransfer = pStXd->usTotalLength - pStdTD->usTotalBytes;
                
                    // Retire the dTD
                    pStdTD->pbtNextdTDPointer = 0x00;        // Set the next dTD pointer to 0
                    pStdTD->usStatusIoc = (USHORT)0x00;      // Clear status and Interrupt on Completion bit
                }
            
                // Call the function registered to service the specified endpoint
                usb_device_call_service(btEndpoint, FALSE, btDirection, (BYTE _X *)pStXd, usActualBytesTransfer);  
            }
        } /* Endfor */
    } /* Endif */

} /* EndBody */

#else

BOOL _reentrant EndptTestSubTask(DWORD dwRegCompare, BYTE btEndPointNumber)
{       
   
   if((WORD)dwRegCompare & ((WORD)(1 << btEndPointNumber)))
      return TRUE;
   else
      return FALSE;
} /* EndBody */


void _reentrant ProcessEndptSetup(DWORD dwRegComplete, DWORD dwRegSetup)
{
    BYTE         btEndpointNumber;

    // a setup packet pending will have its bit set to '1'
    if(dwRegSetup)
    { // Setup packets to process
        // for each endpoint define
        for(btEndpointNumber = 0 ; btEndpointNumber < MAX_NUM_EP ; btEndpointNumber++)
        {
            // Do we have a setup packet pending
            // If we have a setup packet pending do not clear the endpoint setup status
            // bit in the register. We need to keep the setup lockout enable until we
            // read the setup bytes. Clearing the bit is done there.
//            if((WORD)dwRegSetup & ((WORD)(1 << btEndpointNumber)))
           if (EndptTestSubTask(dwRegSetup, btEndpointNumber))
           {
              usb_device_call_service(btEndpointNumber, TRUE, 0, 0, NUMBER_BYTES_IN_SETUP);  // calls the service routine
           }
        }    
    }
}

void _reentrant ProcessEndptComplete(DWORD dwRegComplete, DWORD dwRegSetup)
{   
    BYTE         btEndpointNumber;
    BYTE         btEndpoint, btDirection;
    dTD_Struct _X * pStdTD;
    xd_struct  _X * pStXd;
    USHORT       usActualBytesTransfer;

    if (dwRegComplete)
    { // non setup packets to process
        // For each endpoint define
        for (btEndpointNumber = 0 ; btEndpointNumber < 32 ; btEndpointNumber++)
        {
//            if (dwRegComplete & ((DWORD)(1 << btEndpointNumber)))
            if (EndptTestSubTask(dwRegComplete, btEndpointNumber))           
            {   // We have something , found which direction and endpoint
                if (btEndpointNumber > 15) 
                {
                    btEndpoint = btEndpointNumber - 16;
                    btDirection = USB_SEND;
                }
                else
                {
                    btEndpoint = btEndpointNumber;
                    btDirection = USB_RECV;
                } /* Endif */

                // Extract the specified dTD
                FdTDUnpack((USHORT)btEndpoint, btDirection);
                pStdTD = dTD_Base_Scratch_Address[(2*btEndpoint) + btDirection];
                pStXd = dXD_Base_Address[(2*btEndpoint) + btDirection]; 

                // Check if the dTD is still active
                if(pStdTD->usStatusIoc & (USHORT)USB_EP_dTD_ACTIVE)
                {
                    pStXd->btStatus = USB_STATUS_TRANSFER_IN_PROGRESS;
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
                        pStXd->btStatus = USB_STATUS_TRANSFER_ERROR;
                    }
                    else
                    {   // Transfer successful
                        pStXd->btStatus = USB_STATUS_TRANSFER_SUCCESS;
                    }                    
                
                    // Calculate the actual number of bytes transfered
                    usActualBytesTransfer = pStXd->usTotalLength - pStdTD->usTotalBytes;
                
                    // Retire the dTD
                    pStdTD->pbtNextdTDPointer = 0x00;        // Set the next dTD pointer to 0
                    pStdTD->usStatusIoc = (USHORT)0x00;      // Clear status and Interrupt on Completion bit
                }
            
                // Call the function registered to service the specified endpoint
                usb_device_call_service(btEndpoint, FALSE, btDirection, (BYTE _X *)pStXd, usActualBytesTransfer);  
            }
        } /* Endfor */
    } /* Endif */

} /* EndBody */


#endif




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
BYTE _reentrant usb_dci_init_endpoint(xd_struct _X * pStXD)
{ /* Body */
    DWORD   dwRegEndpointPrime;   
    DWORD   dwRegEndpointStatus;
   
       // Check if the Endpoint is Primed.
       // If the enpoint is primed, then return an error because the endpoint is 
       // in used already
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptPrime), &dwRegEndpointPrime);
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptStatus), &dwRegEndpointStatus);
   #else
       read_usb_reg(HW_ARC_ENDPTPRIME, &dwRegEndpointPrime);
       read_usb_reg(HW_ARC_ENDPTSTATUS, &dwRegEndpointStatus);
   #endif
       
       if(!(dwRegEndpointPrime & (1 << ((16 * pStXD->btDirection) + pStXD->btEndpointNum)))  // Not requested for primed
           && (!(dwRegEndpointStatus &((16 * pStXD->btDirection) + pStXD->btEndpointNum))))  // Endpoint buffer not ready
       { 
           USB_lock();
           
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
            
            USB_unlock();
         
           /* Enable the endpoint for Rx and Tx and set the endpoint type */
   #ifdef SIM_USB20
           read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[pStXD->btEndpointNum]), &TempRegister1.dw);
   #else
           read_usb_reg(HW_ARC_ENDPTCTRL(pStXD->btEndpointNum), &TempRegister1.dw);           
   #endif
         
           if(pStXD->btDirection == USB_SEND)           
#ifndef TT_OPTIMIZE_SIZE
           {
               TempRegister1.dw = TempRegister1.dw 
                                       | ((DWORD)HW_ARC_ENDPTCTRL_TXE_SETMASK    //(DWORD)(1 << HW_ARC_ENDPTCTRL_TXE_BITPOS)     // (DWORD)HW_ARC_ENDPTCTRL_TXE_SETMASK
                                       | (DWORD)HW_ARC_ENDPTCTRL_TXR_SETMASK)
                                       | (DWORD)((DWORD)pStXD->btEndpointType << HW_ARC_ENDPTCTRL_TXT_BITPOS) ;
           }
           else
           {
               TempRegister1.dw = TempRegister1.dw 
                                   | ((DWORD)HW_ARC_ENDPTCTRL_RXE_SETMASK
                                   | (DWORD)HW_ARC_ENDPTCTRL_RXR_SETMASK)
                                   | ((DWORD)pStXD->btEndpointType << HW_ARC_ENDPTCTRL_RXT_BITPOS) ;       
         
           }
#else
           {
               TempRegister1.w |= ((HW_ARC_ENDPTCTRL_TXE_SETMASK | HW_ARC_ENDPTCTRL_TXR_SETMASK) |
                                  (pStXD->btEndpointType << HW_ARC_ENDPTCTRL_TXT_BITPOS));
           }
           else
           {                   
              TempRegister1.w |= ((HW_ARC_ENDPTCTRL_RXE_SETMASK | HW_ARC_ENDPTCTRL_RXR_SETMASK) |
                                 (pStXD->btEndpointType << HW_ARC_ENDPTCTRL_RXT_BITPOS));
           }
#endif
           
   #ifdef SIM_USB20
           write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[pStXD->btEndpointNum]), TempRegister1.dw);
   #else
           write_usb_reg((HW_ARC_ENDPTCTRL(pStXD->btEndpointNum)), TempRegister1.dw);
   #endif
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
dQH_Struct_Setup _X * _reentrant usb_dci_get_setup_data(BYTE btEndpoint)
{ /* Body */
   
#ifdef SIM_USB20
    DWORD   dwReg;
#endif
        
    // Copies the 8 setup bytes into the dQH_Scratch_Setup_Buf 
    FdQHUnpackSetup((USHORT)btEndpoint, USB_RECV);
   
    // Clear the bit in the endpoint setup status register
    // to disable the setup lockout mechanism
#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcEndptSetupStat), &dwReg);
    dwReg &= (WORD)~(1<<btEndpoint);
    write_usb_reg(&(stArcSoftRegs.SoftArcEndptSetupStat), dwReg);
#else
    write_usb_reg(HW_ARC_ENDPTSETUPSTAT, (DWORD)(1 << btEndpoint));
#endif

    return(&dQH_Scratch_Setup_Buf);   
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
    
#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), &dwRegister);
#else
    read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &dwRegister);
#endif
    return((dwRegister & (DWORD)((DWORD)(HW_ARC_ENDPTCTRL_TXS_SETMASK | (DWORD)HW_ARC_ENDPTCTRL_RXS_SETMASK))) ? TRUE : FALSE);
    
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
//    DWORD dwRegister;


   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), &TempRegister1.dw);
   #else
       read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &TempRegister1.dw); 
   #endif
   
#ifndef TT_OPTIMIZE_SIZE
       if (usStatus == TRUE)
           dwRegister |=(DWORD)((DWORD)HW_ARC_ENDPTCTRL_TXS_SETMASK 
                                 | (DWORD)HW_ARC_ENDPTCTRL_RXS_SETMASK);
       else
           dwRegister &=(DWORD)((DWORD)HW_ARC_ENDPTCTRL_TXS_CLRMASK 
                                 & (DWORD)HW_ARC_ENDPTCTRL_RXS_CLRMASK);
#else
       if (usStatus == TRUE)
          TempRegister1.w |= (HW_ARC_ENDPTCTRL_TXS_SETMASK | HW_ARC_ENDPTCTRL_RXS_SETMASK);
       else
          TempRegister1.w &= (HW_ARC_ENDPTCTRL_TXS_CLRMASK & HW_ARC_ENDPTCTRL_RXS_CLRMASK);
#endif
       
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), TempRegister1.dw);
   #else
       write_usb_reg((HW_ARC_ENDPTCTRL(btEndpoint)), TempRegister1.dw);   
   #endif
   
   
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
#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcDevAddr), (DWORD)((DWORD)btAddress << HW_ARC_DEVADDR_ADD_BITPOS));
#else
    write_usb_reg(HW_ARC_DEVADDR, (DWORD)((DWORD)btAddress << HW_ARC_DEVADDR_ADD_BITPOS));
#endif

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

    xd_struct  _X * pStXd;
   
    pStXd = dXD_Base_Address[(2*btEndpoint) + btDirection]; 

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

//    DWORD   dwRegister;

   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), &TempRegister1.dw);
   #else
       read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &TempRegister1.dw);
   #endif
#ifndef TT_OPTIMIZE_SIZE       
       TempRegister1.dw |= (btDirection ? (DWORD)HW_ARC_ENDPTCTRL_TXS_SETMASK 
                                  : (DWORD)HW_ARC_ENDPTCTRL_RXS_SETMASK); 
#else
       TempRegister1.w |= (btDirection ? HW_ARC_ENDPTCTRL_TXS_SETMASK 
                                      : HW_ARC_ENDPTCTRL_RXS_SETMASK);
#endif
   
       /* Stall the endpoint for Rx or Tx and set the endpoint type */
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), TempRegister1.dw);
   #else
       write_usb_reg((HW_ARC_ENDPTCTRL(btEndpoint)), TempRegister1.dw);
   #endif
           
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
    
       /* Stall the endpoint for Rx or Tx and set the endpoint type */
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), &TempRegister1.dw);
   #else
       read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &TempRegister1.dw);
   #endif
#ifndef TT_OPTIMIZE_SIZE   
       dwRegister &= (btDirection ? (DWORD)HW_ARC_ENDPTCTRL_TXS_CLRMASK 
                                  : (DWORD)HW_ARC_ENDPTCTRL_RXS_CLRMASK); 
#else
       TempRegister1.w &= (btDirection ? HW_ARC_ENDPTCTRL_TXS_CLRMASK 
                                      : HW_ARC_ENDPTCTRL_RXS_CLRMASK); 
#endif
       /* Stall the endpoint for Rx or Tx and set the endpoint type */
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), TempRegister1.dw);
   #else
       write_usb_reg((HW_ARC_ENDPTCTRL(btEndpoint)), TempRegister1.dw);
   #endif
           
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
   #ifdef SIM_USB20
       read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), &TempRegister1.dw);
   #else
      read_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), &TempRegister1.dw);
   #endif
#ifndef TT_OPTIMIZE_SIZE   
      dwTempRegister = dwTempRegister 
                       | (DWORD)HW_ARC_ENDPTCTRL_TXR_SETMASK
                       | (DWORD)HW_ARC_ENDPTCTRL_RXR_SETMASK;
#else
      TempRegister1.w |= HW_ARC_ENDPTCTRL_TXR_SETMASK
                      | HW_ARC_ENDPTCTRL_RXR_SETMASK;
#endif
   
   #ifdef SIM_USB20
       write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[btEndpoint]), TempRegister1.dw);
   #else
       write_usb_reg(HW_ARC_ENDPTCTRL(btEndpoint), TempRegister1.dw);                                                     
   #endif 
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
BYTE _reentrant usb_dci_transfer_data(xd_struct _X * pStXd)
{ /* Body */
    dTD_Struct _X * pStdTD;
    BYTE _X * pdTD;
    WORD    wStmpByteAddress;
    USHORT  usArcPage, usArcByteOffset, usArcdTD;

#ifdef SIM_USB20
    DWORD   dwReg;
#endif  
    
    
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
        usArcByteOffset = (USHORT)(wStmpByteAddress - (WORD)(usArcPage << 12)); 
    }

    // Get the address for the specific dTD
    pStdTD = dTD_Base_Scratch_Address[(pStXd->btEndpointNum*2) + pStXd->btDirection];

    // Get the address for the specific dTD
    pdTD = dTD_Base_Address[(pStXd->btEndpointNum*2) + pStXd->btDirection];

    // Initialize the dTD scratch 
    pStdTD->pbtNextdTDPointer = (BYTE *)0x00;     // Always one dTD per transfer
    pStdTD->usTotalBytes = pStXd->usTotalLength;  // Set the number of bytes to transfer
    pStdTD->usCurrentOffset = usArcByteOffset;     // Byte Offset
    pStdTD->pbtBufPage0 = (BYTE *)usArcPage;      // Set the page address
    pStdTD->pbtBufPage1 = (BYTE *)(usArcPage + 1);
    pStdTD->pbtBufPage2 = (BYTE *)(usArcPage + 2);
    pStdTD->pbtBufPage3 = (BYTE *)(usArcPage + 3);
    pStdTD->pbtBufPage4 = (BYTE *)(usArcPage + 4);
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
#ifdef SIM_USB20
    // In the simulation mode you will fake a complete transfer here instead
    // of priming the endpoint
    // To fake a complete transfer we have to:
    //  1. Set the status byte to 0x00 in the specified dTD    
    //  2. Set the number of bytes transmitted to 0 in the specified dTD
    //  3. Set the USB Interrupt in the USB Status register
    //  4. Set the specified bit in the endpoint complete register
    // Initialize the dTD scratch 

    //  Set the status byte to 0x00 in the specified dTD    
    //  Set the number of bytes transmitted to 0 in the specified dTD
    pStdTD->usStatusIoc = (USHORT)0x00;
    pStdTD->usTotalBytes = (USHORT)0x00;

    // Pack the dTD
    FdTDPack(pStXd->btEndpointNum, pStXd->btDirection);
    
    //  3. Set the USB Interrupt in the USB Status register
    read_usb_reg(&(stArcSoftRegs.SoftArcUsbSts),&dwReg);
    write_usb_reg(&(stArcSoftRegs.SoftArcUsbSts), (DWORD)(dwReg |0x01));
    
    //  4. Set the specified bit in the endpoint complete register
    read_usb_reg(&(stArcSoftRegs.SoftArcEndptComplete),&dwReg);
    dwReg |= (1<<(pStXd->btEndpointNum + (16*pStXd->btDirection))); 
    write_usb_reg(&(stArcSoftRegs.SoftArcEndptComplete), dwReg);

#else
    write_usb_reg(HW_ARC_ENDPTPRIME,(DWORD)(1 << ((16 * pStXd->btDirection) + pStXd->btEndpointNum)));

#ifndef BOOT_ROM
#ifndef USE_EEPROM
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
            dwReg &= (DWORD)(1 << ((16 * pStXd->btDirection) + pStXd->btEndpointNum));
        }while(dwReg);
        
        // Check if new setup packet arrived
        read_usb_reg(HW_ARC_ENDPTSETUPSTAT,&dwReg);
        dwReg &= (DWORD)(1 << pStXd->btEndpointNum);
        if(dwReg)
        {
            // New setup packet arrived, free the dXD        
            pStXd->btStatus = USB_STATUS_TRANSFER_IDLE;
        }            
    }    
#endif
#endif    
        
#endif

    return USB_OK;

} /* EndBody */



#ifndef BOOT_ROM
#ifndef USE_EEPROM

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
    
#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[0]), &dwRegister);
#else
    read_usb_reg(HW_ARC_ENDPTCTRL(0), &dwRegister);
#endif
    
    // Reset the toggle bit
    dwRegister |= HW_ARC_ENDPTCTRL_TXR_SETMASK; 
    
#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcEndptCtrl[0]), dwRegister);
#else
    write_usb_reg(HW_ARC_ENDPTCTRL(0), dwRegister);
#endif

    // Check if Test required is send test packet 
    if(usTest == (USHORT)USB_TEST_MODE_TEST_PACKET)
    {
        usb_device_send_data(0, (BYTE _X *)TestPacket, 0, USB_TEST_MODE_TEST_PACKET_LENGTH, FALSE);
    }        

#ifdef SIM_USB20
    read_usb_reg(&(stArcSoftRegs.SoftArcPortsc1), &dwRegister);
#else
    read_usb_reg(HW_ARC_PORTSC1, &dwRegister);
#endif
    
    dwRegister &= (DWORD)HW_ARC_PORTSC1_PE_CLRMASK
                & (DWORD)HW_ARC_PORTSC1_PEC_CLRMASK
                & (DWORD)HW_ARC_PORTSC1_OCC_CLRMASK; 
    
#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcPortsc1), dwRegister);
#else
    write_usb_reg(HW_ARC_PORTSC1, dwRegister);
#endif
    
    // Set test to execute in Arc Register
    dwRegister |= (DWORD)(usTest << HW_ARC_PORTSC1_PTC_BITPOS);
     
#ifdef SIM_USB20
    write_usb_reg(&(stArcSoftRegs.SoftArcPortsc1), dwRegister);
#else
    write_usb_reg(HW_ARC_PORTSC1, dwRegister);
#endif

} /* EndBody */

#endif // USE_EEPROM
#endif // BOOT_ROM

#ifdef  SIM_USB20

void _reentrant write_usb_reg(DWORD *dwSoftReg, DWORD dwData)         
{
    *dwSoftReg = dwData;
}

void _reentrant read_usb_reg(DWORD *dwSoftReg, DWORD * dwData)
{
    *dwData = *dwSoftReg;
}

#else

// This function unpacks the 48-bit double word into 2 16-bit words and writes 
// it back to the ARC register
//void _reentrant write_usb_reg(USHORT usRegAdd, DWORD dwData)         
//{
//    USB_lock();
//    HW_USBARCDATALOW.B.DATA = (WORD)(dwData & (WORD)(HW_USBARCDATALOW_DATA_SETMASK));
//    HW_USBARCDATAHIGH.B.DATA = (WORD)((dwData >> 16) & (DWORD)(HW_USBARCDATALOW_DATA_SETMASK));
//    usRegAdd &= (USHORT)HW_USBARCACCESS_ADD_SETMASK;
//    HW_USBARCACCESS.B.ADD = usRegAdd;
//    HW_USBARCACCESS.B.RWB = 0;
//    HW_USBARCACCESS.B.KICK = 1;         
//    USB_unlock();
//}    


// This macro reads the ARC 32-bit register and packs it in a 48-bit double word
// Use this macro in combination with WRITE_USB_REG macro to read-modify-write
// a ARC register
//void _reentrant read_usb_reg(USHORT usRegAdd, DWORD * dwData)
//{
//    USB_lock();
//    usRegAdd &= HW_USBARCACCESS_ADD_SETMASK;
//    HW_USBARCACCESS.B.ADD = usRegAdd;
//    HW_USBARCACCESS.B.RWB = 1;
//    HW_USBARCACCESS.B.KICK = 1;
//    while(HW_USBARCACCESS.B.KICK);
//    *dwData = (DWORD)(HW_USBARCDATALOW.B.DATA & HW_USBARCDATALOW_DATA_SETMASK);
//    *dwData |= (DWORD)(HW_USBARCDATAHIGH.B.DATA << 16);
//    USB_unlock();
//}

// This function unpacks the 48-bit double word into 2 16-bit words and writes 
// it back to the ARC register
void _reentrant write_usb_reg(USHORT usRegAdd, DWORD dwData)         
{

    #pragma asm

    nolist
;    include "sysmacro.inc"
    include "..\..\..\common\stack.inc"
    include "regs3500usb20.inc"
    list
    
    push    a0
    push    a1
    push    b1
    
    jsr     FUSB_lock
    
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

    jsr     FUSB_unlock

    #pragma endasm  

}    





// This function reads the ARC 32-bit register and packs it in a 48-bit double word
// Use this macro in combination with WRITE_USB_REG macro to read-modify-write
// a ARC register
void _reentrant read_usb_reg(USHORT usRegAdd, DWORD * dwData)
{
    #pragma asm

    nolist
;    include "sysmacro.inc"
;    include "sysmacro.asm"
    include "regs3500usb20.inc"
    list
    
    push    a1
    push    r0
    
    jsr     FUSB_lock
    
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

    move    b1,x:(r0)+
    move    a1,x:(r0)    

    jsr     FUSB_unlock

    #pragma endasm  

}
#endif
