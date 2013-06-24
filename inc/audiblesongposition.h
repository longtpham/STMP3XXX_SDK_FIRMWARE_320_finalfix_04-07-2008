/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM_SDK3000/stmp3xxx_sdk_firmware/inc/rcs/audiblesongposition.h 1.2 2005/06/17 09:16:02Z mmiu Exp $ */
/* $Name: $ */
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


typedef	union AudibleFlags_t {
	struct {
		int		IsAudibleFile	: 1;	// 0
		int		ChapterNavMode	: 1;	// 1
		int		AllowSave		: 1;	// 2
		int		Reserved		: 21;
	} bits;
	WORD	I;
} AudibleFlags_t;


#define	AA_IS_AUDIBLE_FILE		0x000001
#define	AA_CHAPTER_NAV_MODE		0x000002
extern	_X AudibleFlags_t g_AudibleFlags;


typedef struct tFilePosition {
    unsigned long   version;
    unsigned long   checksum;
    unsigned long   filePos;
    unsigned long   playThroughFlag;
} tFilePosition;

RETCODE _reentrant AudibleSetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice);
RETCODE _reentrant AudibleGetFilePosition(INT PackedPathNameAddress, INT btCurrentDevice, FILE_META_DATA *MetaData);


#endif	// AUDIBLESONGPOSITION_H
