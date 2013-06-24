////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: mediaroutines.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _MEDIAROUTINES_H
#define _MEDIAROUTINES_H

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

RETCODE ReadSector(WORD wDeviceNum, DWORD dwSectorNum, WORD wType, 
    WORD _X * pBuffer);
RETCODE WriteSector(WORD wDeviceNum, DWORD dwSectorNum, WORD wType, 
    WORD _X * pBuffer);
extern _asmfunc DWORD MediaGetSize(WORD wDeviceNum, WORD wParameter);
extern _asmfunc RETCODE MediaCleanUp(WORD wDeviceNum);

extern _asmfunc BOOL MediaGetWriteProtectStatus(WORD wDeviceNum);
#endif // #ifndef _MEDIAROUTINES_H