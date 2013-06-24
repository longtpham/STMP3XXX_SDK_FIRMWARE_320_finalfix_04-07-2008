/****************************************************************************
 *
 * @module      support.h
 *
 * @created     09-Februrary-2004
 *
 * @abstract    Support functions for the MTP driver stack
 *
 * @copyright   (C) COPYRIGHT POWER Q INC., 2004
 *
 ****************************************************************************/
#ifndef _SUPPORT_H_
#define _SUPPORT_H_
#include "types.h"

#define MAX_PATH 255

void _reentrant MtpPutLittleEndianInX(void _X *pBuffer, int iStartIndex, long lValue, int iBytes);
void _reentrant MtpPutLittleEndianInY(void _Y *pBuffer, int iStartIndex, long lValue, int iBytes);

#define MtpPutLittleEndianINT64InX(pBuffer, iStartIndex, lValue) MtpPutLittleEndianInX((pBuffer), (iStartIndex), (lValue), 8)
#define MtpPutLittleEndianINT32InX(pBuffer, iStartIndex, lValue) MtpPutLittleEndianInX((pBuffer), (iStartIndex), (lValue), 4)
#define MtpPutLittleEndianINT16InX(pBuffer, iStartIndex, lValue) MtpPutLittleEndianInX((pBuffer), (iStartIndex), (lValue), 2)

long _reentrant MtpGetLittleEndianINT128InX(void _X *pBuffer, int iStartIndex);
long _reentrant MtpGetLittleEndianINT64InX(void _X *pBuffer, int iStartIndex);
long _reentrant MtpGetLittleEndianINT32InX(void _X *pBuffer, int iStartIndex);
int  _reentrant MtpGetLittleEndianINT16InX(void _X *pBuffer, int iStartIndex);
void _reentrant MtpPutValueInX(WORD _X *pBuf, UINT32 *dwBytesToWrite, void *value, UINT32 size);

#define MtpPutLittleEndianINT16InY(pBuffer, iStartIndex, lValue) MtpPutLittleEndianInY((pBuffer), (iStartIndex), (lValue), 2)
/**
* copies a buffer from _X memory to default _Y
*/
void _reentrant memCpyFrom_X (void *pDst, void _X *pSrc, UINT32 size);

/**
* copies a buffer to _X memory to default _Y
*/
void _reentrant memCpyTo_X (void _X *pDst, void *pSrc, UINT32 size);

/**
 * \brief Allocates memory in the defined size.
 *
 * Allocates memory in the size passed.
 */
void *memAlloc(UINT32 size);

/**
 * \brief Frees up the memory previously allocated.
 *
 * Frees the memory previously allocated.
 */
void memFree(void *pMem);


/**
 * \brief Changes the size of the array pointed to by ptr.
 *
 * Changes the size of the memory previously allocated.
 */
void *memRealloc(void *ptr, UINT32 size);

/**
 * \brief Copies the data from pSrc to pDst
 *
 * Copies the data from pSrc to pDst
 */
void *memCpy(void *pDst, void *pSrc, UINT32 size);

#endif
