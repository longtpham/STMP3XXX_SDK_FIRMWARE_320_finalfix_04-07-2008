////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : usb20ch9.h
// Description : USB2.0 Chapter Application skeleton header file
////////////////////////////////////////////////////////////////////////////////

#ifndef __ch9_h__
#define __ch9_h__

extern _asmfunc void UsbError(void);
extern _asmfunc void ProcessUsbEp1Data(WORD wLength);


void _reentrant reset_ep0(BOOL bSetup, BYTE btDirection, BYTE _X *pbtBuffer, WORD wLength);
void _reentrant service_ep0(BOOL bSetup, BYTE btDirection, BYTE _X *pbtBuffer, WORD wLength);
void _reentrant service_speed(BOOL bSetup, BYTE btDirection, BYTE _X *pbtBuffer, WORD wLength);

void _reentrant ch9GetStatus(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant ch9ClearFeature(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant ch9SetFeature(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup); 
void _reentrant ch9SetAddress(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup); 
void _reentrant ch9GetDescription(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant ch9SetDescription(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant ch9GetConfig(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup); 
void _reentrant ch9SetConfig(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant ch9GetInterface(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant ch9SetInterface(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant ch9SynchFrame(BYTE bSetup, dQH_Struct_Setup _X * pStLocalSetup);
void _reentrant InitUsb20(void);
void _reentrant usb_init_ep1(void);

#endif

