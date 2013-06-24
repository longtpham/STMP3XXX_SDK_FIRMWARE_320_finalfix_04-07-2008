////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename:     cachedef.h
// Description:  Constants and externs for the file system routines.
////////////////////////////////////////////////////////////////////////////////

// NOTE: this file is included by fsproj.h, which overrides DRIVELETTERS, NUMCACHES,
// NUMDEVICES, NUMHANDLES.  the values here for these are defaults for building as
// a library.

#ifndef __cachedef_h__
#define __cachedef_h__

#ifndef NUMHANDLES
#define NUMHANDLES 20  
#endif

#ifndef NUMDEVICES
#define NUMDEVICES 1 
#endif

#ifndef NUMCACHES
#define NUMCACHES 2 
#endif

#ifndef DRIVELETTERS
#define DRIVELETTERS {'a','b','c'}
#endif

// sector size variants.
#include "sectordef.h"

// in bytes?                    
#define HANDLEENTRYSIZE 19

// in bytes?                    
#define DEVICERECORDSIZE 28 //stmp6956

// in words.  todo: consider using a formula based on sizeof().
#define CACHEDESCRSIZE 8 

// in words, for large sectors.
#define CACHEBUFSIZE 705

// the number of small caches that fit into a large cache
#define SSFDC_SECTORS_PER_LARGE_SECTOR 4

// the amount of Y space required to cache sectors.  maximum space reserved
// based on large (2k) sectors.
#define XBUF_SIZE (NUMCACHES                                                   \
                   * SSFDC_SECTORS_PER_LARGE_SECTOR                            \
                   * SECTOR_BUFFER_ALLOC_SIZE(SSFDC_SECTOR_TOTAL_SIZE))

// X space allocated for cached sector buffers.
// it must be allocated outside of the file system, by the application, since
// the application can override the number of entries in fsproj.h, but the file
// system can be built as a library (in which case fsproj.h doesn't exist).
extern INT _X bufx[];

// the amount of Y space required to cache sectors. note that the maximim space
// for the cache descriptor entries is the size of each entry multiplied by
// the number of entries (specified for large sector entries) multiplied by
// the number of small sectors that can overlay a large sector.  i.e., when the 
// cache reconfigures from 2k sectors to 512 byte sectors there are 4 times as
// many useable cache dscr entries available.  this declaration must accommodate
// that many to ensure that sufficient space is reserved.
#ifdef k_opt_dynamic_cache
#define YBUF_SIZE ((NUMDEVICES * DEVICERECORDSIZE)                             \
                   + (NUMHANDLES * HANDLEENTRYSIZE)                            \
                   + (SSFDC_SECTORS_PER_LARGE_SECTOR * NUMCACHES * CACHEDESCRSIZE))
#else
#define YBUF_SIZE ((NUMDEVICES * DEVICERECORDSIZE)                             \
                   + (NUMHANDLES * HANDLEENTRYSIZE)                            \
                   + (NUMCACHES * CACHEDESCRSIZE))
#endif


// Y space allocated for file system handles and cache descriptors.
// it must be allocated outside of the file system, by the application, since
// the application can override the number of entries in fsproj.h, but the file
// system can be built as a library (in which case fsproj.h doesn't exist).
extern INT _Y bufy[];

// thse must be allocated outside of the file system, by the application, since
// the application can override these values in fsproj.h, but the file
// system can be built as a library (in which case fsproj.h doesn't exist).
extern INT DriveLetter[];
extern INT maxhandles;
extern INT maxdevices;
extern INT maxcaches; 

#endif // __cachedef_h__


