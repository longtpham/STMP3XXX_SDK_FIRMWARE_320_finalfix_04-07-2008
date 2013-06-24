////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: spi.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _SPI_H
#define _SPI_H

#include "regsspi.h"
#include "regsgpio.h"
///////////////////////////////////////////////////////////////////////////////
// SPI definitions
///////////////////////////////////////////////////////////////////////////////
#define SPI_MODE_MASTER 0
#define SPI_MODE_SLAVE  1

#define SPI_CS_SELECT   0
#define SPI_CS_DESELECT 1

#define SPI_CS_POLARITY_POSITIVE 0
#define SPI_CS_POLARITY_NEGATIVE 1

///////////////////////////////////////////////////////////////////////////////
// SPI Error codes
///////////////////////////////////////////////////////////////////////////////

#define ERR_SPI_LOGICAL_DEVICE_BLOCKED      1
#define ERR_SPI_LOGICAL_DEVICE_NOT_BLOCKED  2
#define ERR_SPI_BUS_BLOCKED                 3
#define ERR_SPI_BUS_NOT_BLOCKED             4
#define ERR_SPI_XMIT_TIMEOUT                5
///////////////////////////////////////////////////////////////////////////////
// SPI structure definitions
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    BOOL    bGrabbed;
}SPI_BUS_FLAGS;
//typedef SpiBuFlagsStruct SPI_BUS_FLAGS;

typedef struct {
    SPI_BUS_FLAGS   Flags;
    WORD wSpCsrShadow;
}SPI_PHYSICAL_BUS;
//typedef SpiBusStruct SPI_PHYSICAL_BUS;

typedef struct {
    BOOL    bGrabbed;
}SPI_LOGICAL_DEVICE_FLAGS;
//typedef SpiLogicalDeviceFlags SPI_LOGICAL_DEVICE_FLAGS;

typedef struct {
    SPI_LOGICAL_DEVICE_FLAGS Flags;
    WORD wSpCsrShadow;
    _X WORD *wGpioBase;
    WORD wGpioSetMask;
    WORD wGpioClearMask;
}SPI_LOGICAL_DEVICE;
//typedef SpiLogicalDeviceStruct SPI_LOGICAL_DEVICE;

///////////////////////////////////////////////////////////////////////////////
// SPI prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant HalSpiDriverInit(WORD wMode);
#ifdef MMC_SEMAPHORE
RETCODE _reentrant HalSpiLogicalGrab(WORD wNum);
RETCODE _reentrant HalSpiLogicalRelease(WORD wNum);
#endif
RETCODE _reentrant HalSpiDeviceInit(WORD wNum);
RETCODE _reentrant HalSpiChipSelect(WORD wNum, WORD wCsState);
RETCODE _reentrant HalSpiByteXmitNoCsSync(WORD wByteWrite, WORD * pwByteRead);
RETCODE _reentrant HalSpiByteXmitSync(WORD wNum, WORD wByteWrite,
    WORD * pwByteRead);
RETCODE _reentrant HalSpiBufferXmitNoCsSync(DWORD dwBytes,
    WORD _X pwWriteBuffer[], WORD _X pwReadBuffer[]);
RETCODE _reentrant HalSpiBufferReadNoCsSync(DWORD dwBytes, WORD wDummyByte,
    WORD _X pwBuffer[]);
RETCODE _reentrant HalSpiBufferWriteNoCsSync(DWORD dwBytes, WORD _X pwBuffer[]);

#endif // #ifndef _SPI_H
