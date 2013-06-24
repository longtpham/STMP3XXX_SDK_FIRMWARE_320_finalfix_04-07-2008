/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/dsaverify.h 1.2 2005/01/13 00:58:02Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 SigmaTel_Beta3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



//dsaverify.h verifies with AASmallVerify 
#ifndef __DSAVERIFY_H
#define __DSAVERIFY_H

#ifdef __cplusplus
extern "C" {
#endif


typedef enum kt 
{ datasign=0, 
	commandsign, 
	metacommandsign
} KeyType;

int   AASmallVerify( void *hash, void *rawSig, KeyType ktype );
//----------- Audible public keys.
extern const unsigned short metacommandsign_public_key[];
//----------- Changes signature to light signature
int AudibleValidAndGenerateLightSign( void *hash, void *rawsig );

#ifdef __cplusplus
}      // Closing of external "C"
#endif

#endif // __DSAVERIFY_H

