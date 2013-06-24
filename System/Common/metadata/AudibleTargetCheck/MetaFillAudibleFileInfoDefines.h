/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/MetaFillAudibleFileInfoDefines.h 1.2 2005/01/13 00:58:02Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 SigmaTel_Beta3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



#ifndef	METAFILLAUDIBLEFILEINFODEFINES_H
#define	METAFILLAUDIBLEFILEINFODEFINES_H

//
// For AAFileMiscHelpers.c
//
#define	ReadULong							FillReadULong
//#define	ReadLong							FillReadLong
#define	ReadUShort							FillReadUShort
//#define	ReadShort							FillReadShort
#define	WriteLong							FillWriteLong
#define	WriteShort							FillWriteShort
#define	ReadTextLine						FillReadTextLine
#define	CAudInputBuffer_AddRef				CFillAudInputBuffer_AddRef
#define	CAudInputBuffer_Release				CFillAudInputBuffer_Release
#define	CAudInputBuffer_Read				CFillAudInputBuffer_Read
#define	CAudInputBuffer_SetCurrentPos		CFillAudInputBuffer_SetCurrentPos
#define	CAudInputBuffer_GetCurrentPos		CFillAudInputBuffer_GetCurrentPos
#define	CAudInputBuffer_GetFileSize			CFillAudInputBuffer_GetFileSize
#define	CAudInputBuffer_CAudInputBuffer		CFillAudInputBuffer_CAudInputBuffer
#define	CAudInputBuffer_SeekFromCurPos		CFillAudInputBuffer_SeekFromCurPos
#define	CAudInputBuffer_GetBufferFromCurPos	CFillAudInputBuffer_GetBufferFromCurPos
#define	CAudOutputBuffer_Write				CFillAudOutputBuffer_Write
#define	CAudOutputBuffer_SetCurrentPos		CFillAudOutputBuffer_SetCurrentPos
#define	CAudOutputBuffer_GetCurrentPos		CFillAudOutputBuffer_GetCurrentPos
#define	CAudOutputBuffer_CAudOutputBuffer	CFillAudOutputBuffer_CAudOutputBuffer


//
// For CoreEncryption.c
//
#define	InitEncryptionKeyFromBuffer			FillInitEncryptionKeyFromBuffer
#define	Encrypt64Bits						FillEncrypt64Bits
#define	Decrypt64Bits						FillDecrypt64Bits
#define	Decrypt64BitsBlocks					FillDecrypt64BitsBlocks
#define	EncryptInChainMode					FillEncryptInChainMode

#endif	// METAFILLAUDIBLEFILEINFODEFINES_H

