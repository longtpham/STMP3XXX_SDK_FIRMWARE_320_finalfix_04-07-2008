////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename: ddildl.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _DDILDL_H
#define _DDILDL_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
// version before march 11 2005 
//#ifdef PLAYER_BUILD
//#define MAX_MEDIA_TABLE_ENTRIES                12
//#else
//#define MAX_MEDIA_TABLE_ENTRIES                14
//#endif
// Mar 14 2007: Only requires the maximum number of drives in one media
// NAND:(2 data drives + 9 system drives + 1 extra system drive in future) = 12
#define MAX_MEDIA_TABLE_ENTRIES                12

#define MAX_LOGICAL_MEDIA                       2

//
// WARNING
// The maximum number of drives needed for player, mtp and usbmsc 
// builds is currently the player build which has the largest number of drives = 12(NAND)+1(MMC).
// Private lib references to this change to use a ddiproject global that's init per project 
// instead so those plib references are no longer fixed at lib compile time. March 1 2005
#ifdef PLAYER_BUILD
#define MAX_LOGICAL_DRIVES                      13
#else // MTP_BUILD & USBMSC/updater need support for the ram drive used in msc
/********************************************************************************
 * We have to define 2 more than the actually size for usbmsc case
 * We have totally 9 system drives, 3 data drives, 1 extra system drive in future, 
 * so the total number is 13. The max will be 13 + 2 = 15
 ********************************************************************************/
#define MAX_LOGICAL_DRIVES                      15
#endif

#define DDI_MAGIC_NUMBER                        0xADEADE

///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

#include "errors.h"

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef void (*MULTI_WRITE_CALLBACK_FUNCTION)(LONG lValue);

#include "ddildl\ddildl_defs.h"

///////////////////////////////////////////////////////////////////////////////
// Prototypes
// Note that these are prototypes for the highest level access to the DDI.
//  The LDL steering mechanism converts the wLogMediaNumber & wLogDriveNumber
//  parameters & passes pointers to the media/drive desctriptrs to the DDIs.
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MediaInit(WORD wLogMediaNumber);
RETCODE _reentrant MediaAllocate(WORD wLogMediaNumber,
    P_MEDIA_ALLOCATION_TABLE pMediaTable);
RETCODE _reentrant MediaDiscoverAllocation(WORD wLogMediaNumber);
RETCODE _reentrant MediaGetMediaTable(WORD wLogMediaNumber,
    P_MEDIA_ALLOCATION_TABLE *pMediaTable);
RETCODE _reentrant MediaGetInfo(WORD wLogMediaNumber, LOGICAL_MEDIA_INFO Type,
    void * _Y pInfo);
RETCODE _reentrant MediaErase(WORD wLogMediaNumber, WORD wMagicNumber);

RETCODE _reentrant DriveInit(WORD wLogDriveNumber);
RETCODE _reentrant DriveGetInfo(WORD wLogDriveNumber, LOGICAL_DRIVE_INFO Type,
    void * pInfo);
RETCODE _reentrant DriveSetInfo(WORD wLogDriveNumber, LOGICAL_DRIVE_INFO Type,
    void * pInfo);
RETCODE _reentrant DriveReadSector(WORD wLogDriveNumber, DWORD dwSectorNumber,
    P_SECTOR_BUFFER pSectorData);
RETCODE _reentrant DriveWriteSector(WORD wLogDriveNumber, DWORD dwSectorNumber,
    P_SECTOR_BUFFER pSectorData);
RETCODE _reentrant DriveMultiWriteSetup(WORD wLogDriveNumber,
    DWORD dwStartSectorNumber, WORD wSectorCount,
    MULTI_WRITE_CALLBACK_FUNCTION pCallBackFunction);
RETCODE _reentrant DriveMultiWriteSector(WORD wLogDriveNumber,
    P_SECTOR_BUFFER pSectorData, LONG lCallBackPassThru);
RETCODE _reentrant DriveErase(WORD wLogDriveNumber, WORD wMagicNumber);
RETCODE _reentrant DriveFlush(WORD wDriveNumber);

RETCODE DdiInitAll(void);


#endif // #ifndef _DDILDL_H
