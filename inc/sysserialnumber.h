////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2003
//
// Filename: sysserialnumber.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSSERIALNUMBER_H
#define _SYSSERIALNUMBER_H

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
#define CHIP_ID_BYTES       (INT)8     // Currently 64 bits.
#define CHIP_ID_DIGITS      (INT)(CHIP_ID_BYTES*2)
#define CHIP_ID_LOOPS1      (INT)((CHIP_ID_DIGITS+5)/6)
#define CHIP_ID_LOOPS2      (INT)(CHIP_ID_DIGITS/3)

#define MMC_CARD_TYPE_SD_PSN_SIZE_BYTES 4
#define MMC_CARD_TYPE_NONSD_SPEC14_SN_SIZE_BYTES 12
#define MMC_CARD_TYPE_NONSD_SPEC20AND31_SN_SIZE_BYTES 4

#define NUM_RAW_SERIAL_NUMBER_BYTES         8
#define NUM_RAW_SERIAL_NUMBER_BYTES_NULL    10
#define MAX_NUM_RAW_SERIAL_NUMBER_WORDS     7
#define MAX_NUM_ASCII_SERIAL_NUMBER_WORDS (MMC_CARD_TYPE_NONSD_SPEC14_SN_SIZE_BYTES*2)


///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Structure definitions
///////////////////////////////////////////////////////////////////////////////
typedef struct {
    WORD wRawSizeInBytes;
    WORD wAsciiSizeInBytes;
    WORD wRaw[MAX_NUM_RAW_SERIAL_NUMBER_WORDS];
    BYTE btAscii[MAX_NUM_ASCII_SERIAL_NUMBER_WORDS];
} SERIAL_NUMBER, * P_SERIAL_NUMBER;

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant ChipSerialNumberInit(void);
RETCODE _reentrant InternalMediaSerialNumberInit(void);

// The following two functions are project-specific and should be defined in
//  the project/system/miscproject.c file
RETCODE _reentrant SerialNumberEnableInit(void);
BOOL _reentrant SerialNumbersEnabled(void);

// The two helper functions below are driver-private and not part of the API. Shared with MMC.
RETCODE _reentrant ChipGetSerialNumber(WORD * pwBuffer);
RETCODE _reentrant CreateNullSerialNumber(P_SERIAL_NUMBER pSerialNumber);
WORD _reentrant ConvertHexToASCII(WORD wInHex);

extern _Y SERIAL_NUMBER g_ChipSerialNumber;
extern _Y SERIAL_NUMBER g_InternalMediaSerialNumber;
extern SERIAL_NUMBER g_MmcSerialNumber;

#endif // #ifndef _SYSSERIALNUMBER_H
