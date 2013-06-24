//------------------------------------------------------------------------------
// Copyright(C) SigmaTel, Inc. 2005
// Filename: fsproj.h (player lcdexample project)
//------------------------------------------------------------------------------
#ifndef __fsproj_h__
#define __fsproj_h__
// NOTE:  cachedef.h with its defaults is included here. Any defines in 
// this fsproj.h supercede and are used as actual NUMHANDLES, NUMDEVICES, NUMCACHES, DRIVELETTERS.  

// TOVERIFY  large block sdk 2.6 had 16 handles while sdk3.0 had only 8.
#define NUMHANDLES 8

// To help non MMC/SD builds, made this 2 not 3. jun28 2005
#ifdef MMC
#define NUMDEVICES 3
#else
#define NUMDEVICES 2
#endif

// TOVERIFY: SDK3.0 had 4 caches here but the large block sdk had only 2 larger caches. 
// Consider finding memory for 3 for performance.
#define NUMCACHES  2
#define DRIVELETTERS {'a','b','c'}

#include "cachedef.h"

// TOVERIFY  Don't think we need fstypes.h. Was added by HK patch of 2.521. See stmp00006965 description. 
//           Macro desired to be included by HK patch was moved to cachedef.h during 2.6 development.
//#include "fstypes.h"

// TOVERIFY:   TODO: DELETE LINES below until the #endif
//extern INT DriveLetter[];
//
//extern INT maxhandles;
//extern INT maxdevices;
//extern INT maxcaches; 

// The cache buffers are allocated in the X memory
//extern int _X bufx[NUMCACHES*CACHEBUFSIZE];

// All other global, custom configurable data is in Y memory.
// Two handles are reserved by the file system, so the number of handles must be more than 2
//extern int _Y bufy[NUMDEVICES*DEVICERECORDSIZE + NUMHANDLES*HANDLEENTRYSIZE + NUMCACHES*CACHEDESCRSIZE];

#endif // __fsproj_h__
//---eof------------------------------------------------------------------------

