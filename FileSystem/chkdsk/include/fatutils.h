////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: Fattest.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _CHKDSK_FATUTILS_H
#define _CHKDSK_FATUTILS_H

BOOL _reentrant LoadFatSector(DWORD wSect, WORD _X *pwBuffer);
BOOL _reentrant WriteFatSector(DWORD wSect, WORD _X *pwBuffer);
BOOL _reentrant IsLastCx(DWORD wCluster);
LONG _reentrant GetNextCx(DWORD wCurCx);
LONG _reentrant GetNextCxFat12(DWORD wCurCx);
LONG _reentrant GetNextCxFat16(DWORD wCurCx);
LONG _reentrant GetNextCxFat32(DWORD wCurCx);

LONG _reentrant FetchCxFat12(DWORD wCurCx);
LONG _reentrant GetLengthCxChain(DWORD wCluster);
BOOL _reentrant FreeCxFat(DWORD wCluster);
extern BOOL _reentrant FreeCxFat12(DWORD wCluster);
BOOL _reentrant FreeCxFat16(DWORD wCluster);
extern BOOL _reentrant FreeCxFat32(DWORD wCluster);
DWORD _reentrant Getsectorno(DWORD wCluster);
WORD SearchmatchingSector(LONG sectorNumber,INT MAXCACHES, tCACHEDESCR_checkdisk *CacheDesc_chkdsk);
   
#endif 


