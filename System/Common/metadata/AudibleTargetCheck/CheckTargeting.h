/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/CheckTargeting.h 1.2 2005/01/13 00:58:01Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 SigmaTel_Beta3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



#ifndef	CHECKTARGETING_H
#define	CHECKTARGETING_H

#include "types.h"
#include "AAFileAccess.h"
#include "CoreEncryption.h"
#include "CircularBuffer.h"
#include "IAACodec.h"
#include "AAFileInternalCommon.h"


typedef struct AudibleDRMArgs_t {
	CAudibleDRMRecordData	*pRecords;
	short					RecordCount;
} AudibleDRMArgs_t;


/*
 * Entry point for SysCallFunction()
 */
extern _reentrant AudErrorCode
AACheckTargeting( INT pInternalAA, INT pDRMArgs, INT *playerID );


/*
 * Actual Check Targeting work function
 */
extern _reentrant AudErrorCode
InternalAACheckTargeting( CAudibleAAInternal *pInternalAA, CAudibleDRMRecordData *pRecords,
                          short RecordCount, unsigned char playerID[AUDIBLE_PLAYER_ID_SIZE] );

#endif	// CHECKTARGETING_H
