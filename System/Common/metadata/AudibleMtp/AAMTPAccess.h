/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/

/*---------------------------------------------------------------------------*\
	File:	AAMTPAccess.h
	
	Description:
		Header file for MTP access of AA file properties.
\*---------------------------------------------------------------------------*/

#ifndef	AAMTPACCESS_H
#define	AAMTPACCESS_H

#include "AAFileAccess.h"
//#include "CircularBuffer.h"
//#include "CoreEncryption.h"
//#include "IAACodec.h"
//#include "AAFileInternalCommon.h"

#ifndef	_SIZE_T_DEF
#define	_SIZE_T_DEF
typedef unsigned int	size_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef struct TitleSegmentData_t {
	unsigned long	IsPartialFile;
	unsigned long	origFileAudioSizeByte; 
	unsigned long	segmentStartByte;
	unsigned long	segmentEndByte;
} TitleSegmentData_t;


//
//	These entry point APIs are for use with code banks to be called using SysCallFunction().
//
extern	AudErrorCode _reentrant AAMTPGetTitle( int PackedPathNameAddress, int btCurrentDevice, char * pTitleBuf );
extern	AudErrorCode _reentrant AAMTPGetAuthor( int pInFile, int iAuthorBufSize, char * pAuthorBuf );
extern	AudErrorCode _reentrant AAMTPGetProductID( int pInFile, int iProdIDBufSize, char * pProdIDBuf );

extern	AudErrorCode _reentrant AAMTPGetDuration( int PackedPathNameAddress, int btCurrentDevice, int * pDuration );
extern	AudErrorCode _reentrant AAMTPGetCodecID( int PackedPathNameAddress, int btCurrentDevice, unsigned short * pCodecID );
extern	AudErrorCode _reentrant AAMTPGetTitleSegment( int PackedPathNameAddress, int btCurrentDevice, UINT128 * pTitleSegment );

extern	AudErrorCode _reentrant AAMTPGetPlaybackPosition( int PackedPathNameAddress, int btCurrentDevice, unsigned long * pPlaybackPos );
extern	AudErrorCode _reentrant AAMTPGetPlayedThrough( int PackedPathNameAddress, int btCurrentDevice, unsigned int * pPlayedThrough );

#ifdef __cplusplus
}      // Closing of external "C"
#endif

#endif	// AAMTPACCESS_H
