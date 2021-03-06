////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: mmcerase.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _MMCERASE_H
#define _MMCERASE_H

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Structure definitions
///////////////////////////////////////////////////////////////////////////////
   
///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

BOOL _reentrant MmcEraseIsTested(WORD wNum);
BOOL _reentrant MmcEraseIsFunctional(WORD wNum);
BOOL _reentrant MmcIsEnumerated(WORD wNum);
RETCODE _reentrant MmcTestErase(WORD wDeviceNumber, DWORD dwStartBlockAddress,
    DWORD dwEndBlockAddress);
RETCODE _reentrant HalMmcEraseBlocks(WORD wDeviceNumber,
    DWORD dwStartBlockAddress, DWORD dwEndBlockAddress);

#endif // #ifndef _MMCERASE_H
