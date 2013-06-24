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


//
//	Entry point APIs for use with SysCallFunction().
//
extern	INT	_reentrant AAGetActivationRecords( INT pNumRecords, INT unused2, INT * pActivationBuf );
extern	INT	_reentrant AASetActivationRecords( INT iNumRecords, INT unused2, INT * pActivationBuf );


#ifdef __cplusplus
}      // Closing of external "C"
#endif

#endif	// AAACTIVATIONRECORDS_H
