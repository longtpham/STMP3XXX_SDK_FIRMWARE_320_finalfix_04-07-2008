/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/

/*---------------------------------------------------------------------------*\
	File:	AAMetaStrings.h
	
	Description:
		Header file for mini-metadata string retrieval module.
\*---------------------------------------------------------------------------*/

#ifndef	AAMETASTRINGS_H
#define	AAMETASTRINGS_H

#include "AAFileAccess.h"
#include "CircularBuffer.h"
#include "CoreEncryption.h"
#include "IAACodec.h"
#include "AAFileInternalCommon.h"

#ifndef	_SIZE_T_DEF
#define	_SIZE_T_DEF
typedef unsigned int	size_t;
#endif


#ifdef __cplusplus
extern "C" {
#endif


//
//	Generic API
//
extern	AudErrorCode _reentrant AAMetaGetString( int pInFile, char * pMetaStrBuf, size_t iMetaStrBufSize, char * pMetaName );


//
//	For SigmaTel, these entry point APIs are for use with code banks to be
//	called using SysCallFunction().
//
extern	AudErrorCode _reentrant AAGetMetaTitle( int pInFile, int iTitleBufSize, char * pTitleBuf );
extern	AudErrorCode _reentrant AAGetMetaAuthor( int pInFile, int iAuthorBufSize, char * pAuthorBuf );
extern	AudErrorCode _reentrant AAGetMetaProductID( int pInFile, int iProdIDBufSize, char * pProdIDBuf );

extern	AudErrorCode _reentrant AAGetMetaHeaderKey( int pInFile, int iHeaderKeyBufSize, char * pHeaderKeyBuf );
extern	AudErrorCode _reentrant AAGetMetaHeaderSeed( int pInFile, int iHeaderSeedBufSize, char * pHeaderSeedBuf );
extern	AudErrorCode _reentrant AAGetMetaEncBlocks( int pInFile, int iEncBlocksBufSize, char * pEncBlocksBuf );

extern	AudErrorCode _reentrant AAGetMetaCodecID( int pFile, int iUnused, unsigned short * pCodecID );
extern	AudErrorCode _reentrant AAGetMetaCodec( int pFile, int iUnused, CAudibleFileCodecData * pCodecInfo );
extern	AudErrorCode _reentrant AAGetMetaAudio( int pFile, int iUnused, CAudibleFileAudioData * pAudioInfo );
extern	AudErrorCode _reentrant AAGetMetaAudibleSpecificHeader( int pFile, int iUnused, CAudibleSpecificHeader * pSpecificHead );
extern	AudErrorCode _reentrant AAGetMetaPlayerIDList( int pFile, int iUnused, CAudibleGroupOrPlayer_IDList * pIDList );
extern	AudErrorCode _reentrant AAGetMetaGroupIDList( int pFile, int iUnused, CAudibleGroupOrPlayer_IDList * pIDList );

extern	AudErrorCode _reentrant AAGetMetaCommon( int pFile, int iUnused, CAudibleCommonHeader * pCommonHead );
extern	AudErrorCode _reentrant AAGetMetaTOC( int pFile, int iUnused, CAudibleFileAudioData * pAudioInfo );

#ifdef __cplusplus
}      // Closing of external "C"
#endif

#endif	// AAMETASTRINGS_H
