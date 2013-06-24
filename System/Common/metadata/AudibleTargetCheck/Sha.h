/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/Sha.h 1.2 2005/01/13 00:58:03Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 SigmaTel_Beta3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



//----------- Include base definitions if not included
#ifndef __SHA_H
#define __SHA_H

#ifdef __cplusplus
extern "C" {
#endif


#define SHA_HASH_OUTPUT_SIZE 20

void shsBlock(unsigned char *dataIn, int len, unsigned char dataOut[SHA_HASH_OUTPUT_SIZE]);


#ifdef __cplusplus
}      // Closing of external "C"
#endif

#endif
