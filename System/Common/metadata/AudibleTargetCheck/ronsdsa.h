/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/ronsdsa.h 1.3 2005/02/19 00:43:06Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



/*  %A% %Q%  */
/*---------------------------------------------------------------------------*\
	File:	dsa.h

	Description:
		Header file for DSA routines.

	Revision History:
		Author		Date		Description
		------		----		-----------
		R Avisa		1 Feb 97	Initial version
		
		Cosmetic changes by Igor Grebnev in 2004
		SimpleDSAVerify commented as it was not implemented. 
		I guess RonsSimpleDSAVerify replaced it.
\*---------------------------------------------------------------------------*/

#ifndef	__RONDSA_H
#define	__RONDSA_H

#include <types.h>


#ifdef __cplusplus
extern "C" {
#endif


#define KEY_BITS               512			            /* Either 512 or 1024 */
#define RAW_PUBKEY_BYTES       (20+3*KEY_BITS/8)

int _reentrant RonsSimpleDSAVerifyEntry
( INT rawHash,
  INT rawSig,
  INT *rawPubKey
);

int RonsSimpleDSAVerify
( unsigned short rawHash[10], 
  unsigned short rawSig[20],
	const unsigned short rawPubKey[RAW_PUBKEY_BYTES/2]
);

#ifdef __cplusplus
}      // Closing of external "C"
#endif

#endif // __RONDSA_H
