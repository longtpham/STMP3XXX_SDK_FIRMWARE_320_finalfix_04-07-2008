////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : Descriptor.h
// Description : 
////////////////////////////////////////////////////////////////////////////////

#ifndef __descriptor_h__
#define __descriptor_h__

void _reentrant PrepareDesc(void);
BYTE _X * _reentrant GetDeviceDesc(USHORT * usLength);
BYTE _X * _reentrant GetConfigDesc(USHORT * usLength);
BYTE _X * _reentrant GetStringDesc(USHORT * usLength, BYTE btString);
BYTE _X * _reentrant GetDeviceQualifierDesc(USHORT * usLength);
BYTE _X * _reentrant GetOtherSpeedConfigDesc(USHORT * usLength);

#endif
