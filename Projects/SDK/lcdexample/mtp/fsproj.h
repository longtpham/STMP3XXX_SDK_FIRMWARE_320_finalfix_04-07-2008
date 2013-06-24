//------------------------------------------------------------------------------
// Copyright(C) SigmaTel, Inc. 2005
//------------------------------------------------------------------------------
#ifndef __fsproj_h__
#define __fsproj_h__
#define NUMHANDLES 20  
#ifdef MMC                   
#define NUMDEVICES 3 
#else                      
#define NUMDEVICES 2 
#endif                       

// TOVERIFY: large block project has this at 2, but MTP 3.05 project has it as 4. CRITICAL
#define NUMCACHES 2
#define DRIVELETTERS {'a','b','c'}
#include "cachedef.h"
#endif // __fsproj_h__
//---eof------------------------------------------------------------------------

