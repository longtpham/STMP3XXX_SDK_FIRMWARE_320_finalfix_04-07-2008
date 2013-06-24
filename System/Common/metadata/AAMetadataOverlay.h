/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/rcs/AAMetadataOverlay.h 1.6 2005/03/03 04:30:56Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



#ifndef	AAMETADATAOVERLAY_H
#define	AAMETADATAOVERLAY_H

//--------- Defines of flags for opening of aa file.
// Main header with version of "aa" file. Also magic number for aa file
#define AUD_FILE_COMMON_HEADER_FLAG 		  ( 1 << AUD_FILE_COMMON_HEADER         )
//    Keeps set of value-name pairs. Basically all metadata. 
#define AUD_FILE_OPTIONAL_HEADER_FLAG 		( 1 << AUD_FILE_OPTIONAL_HEADER       )
//    Audible specific header. Keeps digital signature in particular.
#define AUD_FILE_AUDIBLE_SPEC_HEADER_FLAG ( 1 << AUD_FILE_AUDIBLE_SPEC_HEADER   )
//    List of player IDs associated with this "aa" file. We never put any ID.
#define AUD_FILE_PLAYERID_SEC_FLAG 		    ( 1 << AUD_FILE_PLAYERID_SEC          )
//   List of group IDs associated with this file. Usually 1. 
#define AUD_FILE_GROUPID_SEC_FLAG 		    ( 1 << AUD_FILE_GROUPID_SEC           )
//   Table of context. Keeps chapters, blocks and time stamp information.       
#define AUD_FILE_TOC_SEC_FLAG 			      ( 1 << AUD_FILE_TOC_SEC               )
//    Hashes for audio. Used to validate that audio was not changed in file.    
#define AUD_FILE_HASH_SEC_FLAG 			      ( 1 << AUD_FILE_HASH_SEC              )
//    Audio prompts ( usually audio title ).                                    
#define AUD_FILE_PROMPT_TABLE_SEC_FLAG 	  ( 1 << AUD_FILE_PROMPT_TABLE_SEC      )
#define AUD_FILE_PROMPT_SEC_FLAG 			    ( 1 << AUD_FILE_PROMPT_SEC            )
//     Main audio of "aa" file.
#define AUD_FILE_AUDIO_SEC_FLAG 			    ( 1 << AUD_FILE_AUDIO_SEC             )
//     JPEG image in file.
#define AUD_FILE_IMAGE_SEC_FLAG 			    ( 1 << AUD_FILE_IMAGE_SEC             )

//------------ This flag causes call to decrypt header. 
#define AUD_FILE_DECRYPT_HEADER_FLAG      ( 1L << 31 )


/*
//
// For ReadAAFileSectionsHeader.c
//
#define	ValidateAAFileHeader		MetaValidateAAFileHeader
#define	ReadAAFileSections			MetaReadAAFileSections
#define	ReadAudibleCommonHeader		MetaReadAudibleCommonHeader
#define	ReadAudibleOptionalHeader	MetaReadAudibleOptionalHeader
#define	ReadAudibleSpecificHeader	MetaReadAudibleSpecificHeader
#define	ReadAudibleIDList			MetaReadAudibleIDList
#define	ReadAudibleTableOfContent	MetaReadAudibleTableOfContent
#define	ReadAudibleAudioOffsets		MetaReadAudibleAudioOffsets
#define	ReadAudibleImage			MetaReadAudibleImage
#define	CAudibleAAInternal_FreeAudibleAAInternalMemory	CMetaAudibleAAInternal_FreeAudibleAAInternalMemory
*/


//
// For AAFileMiscHelpers.c
//
#define	ReadULong							MetaReadULong
//#define	ReadLong							MetaReadLong
#define	ReadUShort							MetaReadUShort
//#define	ReadShort							MetaReadShort
#define	WriteLong							MetaWriteLong
#define	WriteShort							MetaWriteShort
#define	ReadTextLine						MetaReadTextLine
#define	CAudInputBuffer_AddRef				CMetaAudInputBuffer_AddRef
#define	CAudInputBuffer_Release				CMetaAudInputBuffer_Release
#define	CAudInputBuffer_Read				CMetaAudInputBuffer_Read
#define	CAudInputBuffer_SetCurrentPos		CMetaAudInputBuffer_SetCurrentPos
#define	CAudInputBuffer_GetCurrentPos		CMetaAudInputBuffer_GetCurrentPos
#define	CAudInputBuffer_GetFileSize			CMetaAudInputBuffer_GetFileSize
#define	CAudInputBuffer_CAudInputBuffer		CMetaAudInputBuffer_CAudInputBuffer
#define	CAudInputBuffer_SeekFromCurPos		CMetaAudInputBuffer_SeekFromCurPos
#define	CAudInputBuffer_GetBufferFromCurPos	CMetaAudInputBuffer_GetBufferFromCurPos
#define	CAudOutputBuffer_Write				CMetaAudOutputBuffer_Write
#define	CAudOutputBuffer_SetCurrentPos		CMetaAudOutputBuffer_SetCurrentPos
#define	CAudOutputBuffer_GetCurrentPos		CMetaAudOutputBuffer_GetCurrentPos
#define	CAudOutputBuffer_CAudOutputBuffer	CMetaAudOutputBuffer_CAudOutputBuffer

//
// For InputStreamImplCStand.c
//
#define	InitInputStreamCStand		MetaInitInputStreamCStand
#define	CFileStand_SetCurrentPos	MetaCFileStand_SetCurrentPos

//
// For AudCStandAllocation.c
//
#define	stm_MemStack						Metastm_MemStack
#define	Audible_malloc						MetaAudible_malloc
#define	Audible_free						MetaAudible_free
#define	Audible_FreeAllMemory				MetaAudible_FreeAllMemory
#define	AudibleSetHeapMemory				MetaAudibleSetHeapMemory
#define	AudibleGetHeapMemory				MetaAudibleGetHeapMemory

//
// For AudCStandImpGeneric.c
//
//#define	Audible_memset						MetaAudible_memset
//#define	Audible_memcpy						MetaAudible_memcpy
#define	Audible_memcmp						MetaAudible_memcmp
//#define	Audible_strcpy						MetaAudible_strcpy
//#define	Audible_strncpy						MetaAudible_strncpy
//#define	Audible_strlen						MetaAudible_strlen
//#define	Audible_strcmp						MetaAudible_strcmp
//#define	Audible_strstr						MetaAudible_strstr
//#define	Audible_strncmp						MetaAudible_strncmp
//#define	Audible_strcat						MetaAudible_strcat
#define	Audible_IsDigit						MetaAudible_IsDigit
#define	Audible_atol						MetaAudible_atol

//
// For CircularBuffer.c
//
#define	CircularBuffer_CircularBuffer			MetaCircularBuffer_CircularBuffer
#define	CircularBuffer_ChangeAllocationBuffer	MetaCircularBuffer_ChangeAllocationBuffer
#define	CircularBuffer_Destructor				MetaCircularBuffer_Destructor
#define	CircularBuffer_NormalisePositions		MetaCircularBuffer_NormalisePositions
#define	CircularBuffer_Read						MetaCircularBuffer_Read
#define	CircularBuffer_GetDataLeft				MetaCircularBuffer_GetDataLeft
#define	CircularBuffer_Write					MetaCircularBuffer_Write
#define	CircularBuffer_Unread					MetaCircularBuffer_Unread
#define	CircularBuffer_FlushData				MetaCircularBuffer_FlushData
#define	CircularBuffer_SetWritePosition			MetaCircularBuffer_SetWritePosition
#define	CircularBuffer_GetBufferSize			MetaCircularBuffer_GetBufferSize
#define	CircularBuffer_GetBufferPointer			MetaCircularBuffer_GetBufferPointer

//
// For FillAudibleFileInfo.c
//
#define	FillAudibleFileInfoFromInternalData		MetaFillAudibleFileInfoFromInternalData
#define	FillAudibleEncStringsFromInternalData	MetaFillAudibleEncStringsFromInternalData
//#define	FillAudioInfoFromInternalData			MetaFillAudioInfo
#define	FillCodecInfoFromInternalData			MetaFillCodecInfoFromInternalData
#define	ProcessValueNamePairForAudibleInfo		MetaProcessValueNamePairForAudibleInfo
#define	ProcessValueNamePairForAudibleInfoEmbedded		MetaProcessValueNamePairForAudibleInfoEmbedded


#include "AAFileAccess.h"
#include "CircularBuffer.h"
#include "CoreEncryption.h"
#include "IAACodec.h"
#include "AAFileInternalCommon.h"


typedef struct GetMetadataOverlayArgs_t {
	CAudibleFileInfoEmbedded *	pAudibleFileInfoEmb;
	long 						flags;
} GetMetadataOverlayArgs_t;

//AudErrorCode _reentrant AudibleAA_GetMetadataOverlay( CAudibleAAInternal *pAudFileData, IAudibleInputStream * pFile, CAudibleFileInfoEmbedded * pAudibleFileInfoEmb, long flags );
AudErrorCode _reentrant AudibleAA_GetMetadataOverlay( CAudibleAAInternal *pAudFileData, IAudibleInputStream * pFile );

#endif	// AAMETADATAOVERLAY_H
