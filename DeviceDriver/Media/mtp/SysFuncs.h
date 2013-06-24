/**
***
*** Filename: SysFuncs.h
*** Description: PTP string and array function prototypes
***
***
*** Copyright (c) SigmaTel, Inc. Unpublished
***
*** SigmaTel, Inc.
*** Proprietary & Confidential
***
*** This source code and the algorithms implemented therein constitute
*** confidential information and may comprise trade secrets of SigmaTel, Inc.
*** or its associates, and any use thereof is subject to the terms and
*** conditions of the Confidential Disclosure Agreement pursuant to which this
*** source code was originally received.
**/
//////////////////////////////////////////////////////////////////
// Copyright © Microsoft Corporation, 2003
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//   Includes
//////////////////////////////////////////////////////////////////
#ifndef __SYSFUNCS_H_INCLUDED
#define __SYSFUNCS_H_INCLUDED

/*****************************************************************************
 * Defines the PTP string structure
 *****************************************************************************/

void _reentrant MtpPutLittleEndianInX(void _X *pBuffer, int iStartIndex, long lValue, int iBytes);
#endif