////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2005
//
// File : SysResource.h
// Description : Structures and Functions Definitions for System Resource
////////////////////////////////////////////////////////////////////////////////

#ifndef __SysResource_h__
#define __SysResource_h__ 1

#include "types.h"
#include "ddildl.h"
#include "sectordef.h"

typedef struct {
    DWORD    dwSectNumber;          // Current Sector Loaded in Buffer
    P_SECTOR_BUFFER pSectBuffer;    // Pointer to the sector buffer
} Struct_Buffer_Sector_Desc, * pStruct_Buffer_Sector_Desc;

#define SECTOR_DATA_SIZE_IN_BYTES           (510)  // Number of valid data bytes before padding

#ifdef READ_ONLY_THE_RESOURCE
    #define SECTOR_BUFFER_SIZE              (SECTOR_BUFFER_ALLOC_SIZE(SSFDC_SECTOR_TOTAL_SIZE))
#else
    #define SECTOR_BUFFER_SIZE              (SECTOR_BUFFER_ALLOC_SIZE(SSFDC_SECTOR_TOTAL_SIZE)+9)      // The extra 27 bytes here is for extra ECC data needed
#endif
// Player index cache enabled saves ~400 mseconds of intersong delay. 
#ifdef STMP_BUILD_PLAYER
	// This number selected based on profiling (8/17/2005)
	#define RESOURCE_INDEX_CACHE_SIZE           (128)   
#else //MTP						
         // This number selected based on profiling (8/17/2005)						
	#define RESOURCE_INDEX_CACHE_SIZE           (75)
#endif


#if RESOURCE_INDEX_CACHE_SIZE > 0
	#define ENABLE_RESOURCE_INDEX_CACHE (1)
#else
	#define ENABLE_RESOURCE_INDEX_CACHE (0)
#endif

extern RETCODE _reentrant DriveReadSector(WORD wLogDriveNumber, DWORD dwSectorNumber,
        P_SECTOR_BUFFER pSectorData);
extern RETCODE _reentrant DriveGetInfo(WORD wLogDriveNumber, LOGICAL_DRIVE_INFO Type,
        void * pInfo);

RETCODE _reentrant ResourceFileRead(DWORD dwStartPos, WORD wNumBytesToRead, void * pwBuffer, WORD MemSpaceDest);

void _reentrant CloseResourceDrive(void);

RETCODE _reentrant CopyWords(WORD wNumWordsToCopy, void * pDest, WORD SECTOR_BUFFER_MEM * pSrc,
                        WORD wMemSpaceDest);

void _reentrant CopyWordsFromSectorBufToY(WORD wNumWordsToCopy, WORD _Y * pDest,
                WORD SECTOR_BUFFER_MEM * pSrc);
void _reentrant CopyWordsFromSectorBufToX(WORD wNumWordsToCopy, WORD _X * pDest,
                WORD SECTOR_BUFFER_MEM * pSrc);
void _reentrant CopyWordsFromSectorBufToP(WORD wNumWordsToCopy, WORD _P * pDest,
                WORD SECTOR_BUFFER_MEM * pSrc);

#endif
/* EOF */

