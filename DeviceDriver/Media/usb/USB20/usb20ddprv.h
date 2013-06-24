////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// File        : usb20ddprv.h
// Description : This file contains the private defines, externs and
//           data structure definitions required by the USB2.0 Device Driver
////////////////////////////////////////////////////////////////////////////////

#ifndef __usb20ddprv_h__
#define __usb20ddprv_h__ 1

#include "types.h"
#include "regsusb20.h"
#include "regsusb20phy.h"

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////


     
// This macro reads the ARC 32-bit register and returns in a 24-bit word
// the LSW (16 bit)
// This macro is usefull to test endpoint bits. Do not use this macro to
// read-modify-write sequence since the upper 16 bits of the ARC register
// are not saved 
#define READ_USB_REG_LOW(Reg, Data)     
/*
    Reg &= HW_USBARCACCESS_ADD_SETMASK; \
    HW_USBARCACCESS.B.ADD = Reg;        \
    HW_USBARCACCESS.B.RWB = 1;          \
    HW_USBARCACCESS.B.KICK = 1;         \
    while(HW_USBARCACCESS.B.KICK);      \
    Data = (WORD)(HW_USBARCDATALOW.B.DATA & HW_USBARCDATALOW_DATA_SETMASK); 
*/

// This macro reads the ARC 32-bit register and returns in a 24-bit word
// the HSW (16 bit)
// This macro is usefull to test endpoint bits. Do not use this macro to
// read-modify-write sequence since the lower 16 bits of the ARC register
// are not saved 
#define READ_USB_REG_HIGH(Reg, Data)    
/*
    Reg &= HW_USBARCACCESS_ADD_SETMASK; \
    HW_USBARCACCESS.B.ADD = Reg;        \
    HW_USBARCACCESS.B.RWB = 1;          \
    HW_USBARCACCESS.B.KICK = 1;         \
    while(HW_USBARCACCESS.B.KICK);      \
    Data = (WORD)(HW_USBARCDATAHIGH.B.DATA & HW_USBARCDATALOW_DATA_SETMASK); 
*/


#define  MAX_EP_TR_DESCRS                    MAX_NUM_EP
#define  MAX_XDS_FOR_TR_CALLS                MAX_EP_TR_DESCRS
#define  MAX_USB_DEVICES                     (1)

#define  USB_MAX_CTRL_PAYLOAD                (64)

/* Endpoint Queue Head Bit Masks */
#define  USB_EP_QUEUE_HEAD_IOS              (0x8000)

/* Endpoint Queue Head Bit Position */
#define USB_EP_QUEUE_HEAD_MULT_POS                  (14)
#define USB_EP_QUEUE_HEAD_DONT_ZERO_TERMINATE_POS   (13)

// dTD Masks
#define USB_EP_dTD_ACTIVE                   (0x0080)
#define USB_EP_dTD_HALTED                   (0x0040)
#define USB_EP_dTD_DATA_BUF_ERR             (0x0020)
#define USB_EP_dTD_DATA_TRANSACT_ERR        (0x0008)
#define USB_EP_dTD_NO_ERROR                 (0x00e8)
#define USB_EP_dTD_IOC                      (0x8000)

    // This structure is also defined in assembly. All dQH writes go thru this structure.
    // To update the actual dQH, use the assembly function dQHPackCap()  
    // The structure is allocated in assembly file address dQH_Scratch_Cap
typedef struct {
    USHORT  usIos;                  // Bit 15 is the Interrupt On Setup
                                    // Bits 0 to 14 always set to 0
    USHORT  usMaxPktLengthZltMult;  // Bits 0-10 = Maximum Packet Length
                                    // Bits 11-12 - Always 0
                                    // Bit 13 - Zero Length Termination Select Bit
                                    // bits 14-15 - Number packets executed per transaction
                                    //              always 00 for control and bulk endpoints
    BYTE *  pbtNextdTDPointer;      // Pointer to the next dTD. We limit the address to 16 bits
                                    // 64Kbytes in ARC space.
    USHORT  usStatusIoc;            // Bits 0-7 - Status of transaction
                                    // Bits 8-14 - Always 0
                                    // Bit 15 - Interrupt On Completion (IOC)
    USHORT  usTotalBytes;            // Total bytes transfered
    } dQH_Struct_Cap;


    // There is one structure of this type per endpoint. The structures are allocated in assembly
    // at addresses dTD_SCRATCH_0 to dTD_SCRATCH_n where n is number of endpoints - 1
typedef struct {
    BYTE *  pbtNextdTDPointer;      // Pointer to the next dTD. We limit the address to 16 bits
                                    // 64Kbytes in ARC space.
    USHORT  usStatusIoc;            // Bits 0-7 - Status of transaction
                                    // Bits 8-14 - Always 0
                                    // Bit 15 - Interrupt On Completion (IOC)
    USHORT  usTotalBytes;           // bits 0-13 total bytes to transfer
                                    // bits 14-15 Always set to 0
    USHORT  usCurrentOffset;        // Offset into the 4KBytes buffer where the packet is to begin
                                    // limited to 12 bits
    BYTE *  pbtBufPage0;            // Page 0 offset for the packet buffer
    BYTE *  pbtBufPage1;            // Page 1 offset for the packet buffer
    BYTE *  pbtBufPage2;            // Page 2 offset for the packet buffer
    BYTE *  pbtBufPage3;            // Page 3 offset for the packet buffer
    BYTE *  pbtBufPage4;            // Page 4 offset for the packet buffer
    } dTD_Struct;


typedef struct {
   BYTE     btRequestType;
   BYTE     btRequest;
   BYTE     btValueLsb;
   BYTE     btValueMsb;
   BYTE     btIndexLsb;
   BYTE     btIndexMsb;
   BYTE     btLengthLsb;
   BYTE     btLengthMsb;
    } dQH_Struct_Setup;


    // Use only for ARC simulation
#ifdef  SIM_USB20

typedef struct {
    DWORD       SoftArcHcsParams;      
    DWORD       SoftArcUsbCmd;         
    DWORD       SoftArcUsbSts;         
    DWORD       SoftArcUsbIntr;        
    DWORD       SoftArcDevAddr;        
    DWORD       SoftArcEndptListAddr;  
    DWORD       SoftArcPortsc1;        
    DWORD       SoftArcUsbMode;        
    DWORD       SoftArcEndptSetupStat; 
    DWORD       SoftArcEndptPrime;     
    DWORD       SoftArcEndptFlush;     
    DWORD       SoftArcEndptStatus;    
    DWORD       SoftArcEndptComplete;  
    DWORD       SoftArcEndptCtrl[MAX_NUM_EP];     
} arc_reg_struct;

#endif

extern _asmfunc void FInitUsbStructures(void);
extern _asmfunc void FdQHPackCap(USHORT usEndpointNumber, BYTE btDirection, BYTE btFlag);
extern _asmfunc void FdQHUnpackSetup(USHORT usEndpointNumber, BYTE btDirection);
extern _asmfunc void FdTDUnpack(USHORT usEndpointNumber, BYTE btDirection);
extern _asmfunc void FdTDPack(USHORT usEndpointNumber, BYTE btDirection);
void _reentrant ProcessEndptSetup(DWORD dwRegComplete, WORD wRegSetup);
void _reentrant ProcessEndptComplete(DWORD dwRegComplete, WORD wRegSetup);

#ifdef  SIM_USB20
void _reentrant write_usb_reg(DWORD *dwSoftReg, DWORD dwData);         
void _reentrant read_usb_reg(DWORD *dwSoftReg, DWORD * dwData);
#else
void _reentrant write_usb_reg(USHORT usRegAdd, DWORD dwData);         
void _reentrant read_usb_reg(USHORT usRegAdd, DWORD * dwData);
#endif

void _long_interrupt(-1) usb_dci_isr(void);

#ifndef BOOT_ROM
void _reentrant usb_dci_set_test_mode(USHORT usTest);
#endif
 
#endif
