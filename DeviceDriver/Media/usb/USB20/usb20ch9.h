////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : usb20ch9.h
// Description : USB2.0 Chapter Application skeleton header file
////////////////////////////////////////////////////////////////////////////////

#ifndef __ch9_h__
#define __ch9_h__

void _reentrant ch9ServiceEp0(BOOL bSetup, BYTE btDirection, WORD _X *pbtBuffer, WORD wLength,WORD wPrivateData);
/*
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
*/

extern BYTE _X * _reentrant GetDescriptor(BYTE btDescType, BYTE btDevSpeed, BYTE btIndex);
#endif

