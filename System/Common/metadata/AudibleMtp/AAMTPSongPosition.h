/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/inc/rcs/audiblesongposition.h 1.2 2005/01/13 00:58:00Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 SigmaTel_Beta3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



#ifndef	AUDIBLESONGPOSITION_H
#define	AUDIBLESONGPOSITION_H

typedef struct tFilePosition {
    unsigned long   version;
    unsigned long   checksum;
    unsigned long   filePos;
    unsigned long   playThroughFlag;
} tFilePosition;

extern	AudErrorCode _reentrant AAMTPSetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice);
extern	AudErrorCode _reentrant AAMTPGetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice, unsigned long * pPlaybackPos, int * pPlayedThrough);


#endif	// AUDIBLESONGPOSITION_H
