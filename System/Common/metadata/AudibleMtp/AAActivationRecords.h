/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/

/*---------------------------------------------------------------------------*\
	File:	AAActivationRecords.h
	
	Description:
		Header file for activation record access module.
\*---------------------------------------------------------------------------*/

#ifndef	AAACTIVATIONRECORDS_H
#define	AAACTIVATIONRECORDS_H

#include "types.h"


#ifdef __cplusplus
extern "C" {
#endif

//Activation record size. UINT8, 560 records, 560 bytes
#define  AA_ACTIVATION_RECORD_SIZE_BYTES 560
//Per MTP spec, the 1st 4 bytes of the Data buffer will contain the array size.
//Total transfer size = 4 + size of array  
#define  MTP_AA_ACTIVATION_RECORD_TRANSFER_SIZE_BYTES 	4 + AA_ACTIVATION_RECORD_SIZE_BYTES  
//
//	Entry point APIs for use with SysCallFunction().
//
extern	INT	_reentrant AAGetActivationRecords( INT pNumRecords, INT unused2, INT * pActivationBuf );
extern	INT	_reentrant AASetActivationRecords( INT iNumRecords, INT unused2, INT * pActivationBuf );


#ifdef __cplusplus
}      // Closing of external "C"
#endif

#endif	// AAACTIVATIONRECORDS_H
