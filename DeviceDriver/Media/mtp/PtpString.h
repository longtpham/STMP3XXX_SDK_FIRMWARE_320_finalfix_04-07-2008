/**
***
*** Filename: ptpstring.h
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
#ifndef __PTPSTRING_H_INCLUDED
#define __PTPSTRING_H_INCLUDED

#include "types.h"
/*****************************************************************************
 * Defines the PTP string structure
 *****************************************************************************/

#define MAX_PTP_STRING_CHARS        256
#define MAX_PTP_SHORT_STRING_CHARS   32 

typedef enum _STRING_TYPE {
    SHORT_STRING,
    LONG_STRING
} STRING_TYPE;

typedef struct SPtpString
{
  INT  NumChars; /* Number of chars in the string (including the NULL) */
  WCHAR StringChars[MAX_PTP_STRING_CHARS];  /* Holds actual Unicode string with 2 bytes characters, NULL-terminated */
} SPtpString;

//
// This is needed becuase you cannot do sizeof(SPtpString) to check the size of
// this structure during preprocessor run.
//
#define SPTPSTRING_SIZE (MAX_PTP_STRING_CHARS + 1)

typedef struct SPtpShortString
{
  INT  NumChars; /* Number of chars in the string (including the NULL) */
  WCHAR StringChars[MAX_PTP_SHORT_STRING_CHARS];  /* Holds actual Unicode string with 2 bytes characters, NULL-terminated */
} SPtpShortString;

// from time.h
struct tm
{
  int   tm_sec;         /* seconds after the minute - [0, 59]   */
  int   tm_min;         /* minutes after the hour - [0, 59]     */
  int   tm_hour;        /* hours since midnight - [0, 23]       */
  int   tm_mday;        /* day of the month - [1, 31]           */
  int   tm_mon;         /* months since January - [0, 11]       */
  int   tm_year;        /* year since 1900                      */
  int   tm_wday;        /* days since Sunday - [0, 6]           */
  int   tm_yday;        /* days since January 1 - [0, 365]      */
  int   tm_isdst;       /* Daylight Saving Time flag            */
};
/**
 * Initializes the PTP string by zeroing out the string.
 */
void initPtpString(SPtpString * pString);


/**
 * Deallocates the memory for the PtpString structure
 */
void destroyPtpString(SPtpString *pString);

/**
 * Copies the pStr into the PtpString structure.  The size parameter indicates
 * if the pStr is composed of 8-bit characters or 16-bit characters.
 */
_reentrant void initChar2PtpString(SPtpString *pString, void * pStr, UINT16 word_size, STRING_TYPE _type);

/**
 * Concatenates the pStr into the PtpString structure.  The size parameter indicates
 * if the pStr is composed of 8-bit characters or 16-bit characters.
 */
_reentrant void catChar2PtpString(SPtpString *pString, void * pStr, UINT16 word_size, STRING_TYPE _type);

/**
 * Concatenates the two PtpString structures into the first.
 */
_reentrant void catPtpString(SPtpString *pDstString, SPtpString *pSrcString, STRING_TYPE _type);

/**
 * Copies the time set by lpTime and converts it to a PTP String.
 */
_reentrant void initTime2PtpString(SPtpShortString *pString, struct tm *lTime);

/**
 * extracts time from PtpString and stores it in time object.
 */
_reentrant void initPtpString2Time(SPtpShortString *pString, struct tm *lTime);

/**
 * extracts number from string.
 */
_reentrant void MtpExtractNumberFromPtpString( WCHAR *pUnicodeString, INT iDigits, INT iOffset, INT * iNumber);

/**
* Converts the number to PtpString
*/
_reentrant void MtpStringPrintNumber(WCHAR *pUnicodeString,INT iNumber, INT iDigits, INT iFiller);

/**
 * Copies one string into another.
 */
_reentrant void copyPtpString(SPtpString *pDst, SPtpString *pSrc);

/* copy Ptpstring in _X memory; usually in raw data from host */
_reentrant void copy_XPtpString(SPtpString *pDst, void _X *pSrc, UINT16 *byteOffset);

/**
 * Copies the void * pSrc string to the PTP string pDst.
 */
_reentrant void copyChar2PtpString(SPtpString *pDst, void *pSrc);

/**
 * Checks to see if two different PTP strings are equal.
 */
_reentrant BOOL isEqualPtpString(SPtpString *pDst, SPtpString *pSrc);

/**
 * Returns the size in bytes of the PTP string.
 */
UINT32 sizeInBytesPtpString(SPtpString *pString);
    

/**
 * packs the PTP string into a packed char string
 */
_reentrant UINT16 copyPtpToPackedString (_packed char *packedStr, SPtpString *pPtpStr);

/**
 * copies packed string into PTP string
 */
_reentrant UINT16 copyPackedToPtpString (_packed char *packedStr, SPtpString *pPtpStr);


/*****************************************************************************
 * Defines the PTP array structure
 *****************************************************************************/
    
#define ELEMENT_NOT_FOUND   -1
#define INITIAL_ARRAY_SIZE  20

typedef enum DATATYPE
{
  UINT8_TYPE,
  UINT16_TYPE,
  UINT32_TYPE
} DATATYPE;

typedef struct SPtpArray
{
  DATATYPE type;
  UINT32 ArraySize;
  UINT32 NumElements;
  UINT32 *ArrayEntry;
} SPtpArray;


/**
 * Initializes the PTP array with the type and the default
 * array size.
 */
_reentrant void initPtpArray(SPtpArray *pArray, DATATYPE type, UINT32 arraySize, UINT32 *pActualArray);

/**
 * Returns the size in bytes of the PTP array structure and data.
 */
_reentrant UINT32 sizeInBytesPtpArray(SPtpArray *pArray);


/**
 * Finds and returns the index of the desired element.
 */
_reentrant UINT32 findElementPtpArray(SPtpArray *pArray, UINT32 element);

/**
 * Appends the desired element to the end of the array.
 */
_reentrant BOOL appendElementPtpArray(SPtpArray *pArray, UINT32 element);

/**
 * Removes the specified element from the array.
 */
_reentrant BOOL removeElementPtpArray(SPtpArray *pArray, UINT32 element);

/**
 * Copies the PTP array from one structure to the other.
 * Note that the destination array's allocated data buffer
 * may be smaller than the source's available data buffer.
 */
_reentrant BOOL copyPtpArray(SPtpArray *pDst, SPtpArray *pSrc);

/**
 * Packs the data within the PTP array into a byte array preceded
 * by the number of bytes.
 */
_reentrant UINT32 packPtpArray(UINT16 _X *pBuf, UINT32 *dwBytesWritten, SPtpArray *pArray);


#endif 
