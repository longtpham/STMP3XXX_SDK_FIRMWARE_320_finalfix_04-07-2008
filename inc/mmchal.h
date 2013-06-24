////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: mmchal.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _MMCHAL_H
#define _MMCHAL_H

///////////////////////////////////////////////////////////////////////////////
// MMC HAL definitions
///////////////////////////////////////////////////////////////////////////////

#define MMC_NULL_ARG    0

// MMC commands
#define MMC_CMD0        0x40|0
#define MMC_CMD1        0x40|1
#define MMC_CMD8		0x40|8
#define MMC_CMD9        0x40|9
#define MMC_CMD10       0x40|10
#define MMC_CMD12       0x40|12
#define MMC_CMD13       0x40|13
#define MMC_CMD16       0x40|16
#define MMC_CMD17       0x40|17
#define MMC_CMD18       0x40|18
#define MMC_CMD23       0x40|23
#define MMC_CMD24       0x40|24
#define MMC_CMD25       0x40|25
#define MMC_CMD27       0x40|27
#define MMC_CMD28       0x40|28
#define MMC_CMD29       0x40|29
#define MMC_CMD30       0x40|30
#define MMC_CMD32       0x40|32
#define MMC_CMD33       0x40|33
#define MMC_CMD35       0x40|35
#define MMC_CMD36       0x40|36
#define MMC_CMD38       0x40|38
#define MMC_CMD42       0x40|42
#define MMC_CMD55       0x40|55
#define MMC_CMD56       0x40|56
#define MMC_CMD58       0x40|58
#define MMC_CMD59       0x40|59

// SD-specific commands
#define SD_ACMD13       0x40|13
#define SD_ACMD22       0x40|22
#define SD_ACMD23       0x40|23
#define SD_ACMD41       0x40|41
#define SD_ACMD42       0x40|42
#define SD_ACMD51       0x40|51

// Responses
#define MMC_RESPONSE_TYPE       WORD
#define MMC_RESPONSE_TYPE_R1    0
#define MMC_RESPONSE_TYPE_R2    1
#define MMC_RESPONSE_TYPE_R3    2

#define MMC_R1_NO_ERROR                     0x00
#define MMC_R1_IDLE_NO_ERROR                0x01
#define MMC_R1_IN_IDLE_STATE_BITPOS         0
#define MMC_R1_ERASE_RESET_BITPOS           1
#define MMC_R1_ILLEGAL_COMMAND_BITPOS       2
#define MMC_R1_COM_CRC_ERROR_BITPOS         3
#define MMC_R1_ERASE_SEQUENCE_ERROR_BITPOS  4
#define MMC_R1_ADDRESS_ERROR                5
#define MMC_R1_PARAMETER_ERROR              6

#define MMC_START_BYTE          0xFE

#define MMC_CSD_VERSION_10      0
#define MMC_CSD_VERSION_11      1
#define MMC_CSD_VERSION_12      2
#define MMC_CSD_VERSION_FROM_EXT_CSD	3

#define MMC_SPEC_VERSION_10     0
#define MMC_SPEC_VERSION_14     1
#define MMC_SPEC_VERSION_20     2
#define MMC_SPEC_VERSION_31     3
#define MMC_SPEC_VERSION_40     4

#define SD_CSD_VERSION_10       0

#define MMC_CARD_TYPE           WORD
#define MMC_CARD_TYPE_MMC       (MMC_CARD_TYPE)0
#define MMC_CARD_TYPE_SD        (MMC_CARD_TYPE)1

#define MMC_SUPPORTED_SECTOR_SIZE   512

#define MMC_OCR_SUPPORT (DWORD)0x000000020000

//   The STMP3400 max SCK speed is DCLK/6, STMP3410 is DCLK/2:
//   Assume a 100MHz DCLK & SCK=DCLK/2=50Mhz. It takes a minimum of 15 DCLKs
//   between each SPI byte transfer, so one byte takes (8*2)+15=31 DCLKs.  This
//   is a byte rate of 1.612MBytes/s.  Assume all timeouts are 1s.
#define MMC_PORT_TIMEOUT            (WORD)(100000000/31)
#define MMC_START_BYTE_TIMEOUT      MMC_PORT_TIMEOUT
#define MMC_WRITE_BUSY_TIMEOUT      MMC_PORT_TIMEOUT
#define MMC_CSD_WRITE_BUSY_TIMEOUT  MMC_PORT_TIMEOUT
#define MMC_ERASE_BUSY_TIMEOUT      MMC_PORT_TIMEOUT
#define MMC_REGISTER_READ_TIMEOUT   MMC_PORT_TIMEOUT

///////////////////////////////////////////////////////////////////////////////
// MMC HAL structure definitions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// MMC definitions

// ifdef this struct for SPI or MMCBUS (currently only support SPI)
typedef struct {
    _X WORD *wGpioBase;
    WORD wGpioSetMask;
    WORD wGpioClearMask;
    _X WORD *wWriteProtectGpioBase;
    WORD wWriteProtectGpioSetMask;
    WORD wWriteProtectGpioClearMask;
    _X WORD *wMediaDetectGpioBase;
    WORD wMediaDetectGpioSetMask;
    WORD wMediaDetectGpioClearMask;
    WORD wMediaDetectPolarity;
    WORD wSpiDeviceNumber;
    BOOL bSPIBusInitialized;
} MMC_PHYSICAL_BUS;

typedef struct {
    WORD wCommand;
    WORD wArg3;
    WORD wArg2;
    WORD wArg1;
    WORD wArg0;
    WORD wCRC7;
} MMC_CMD;

typedef union {
    struct {
        WORD Idle            : 1;
        WORD EraseReset      : 1;
        WORD IllCommand      : 1;
        WORD ComCrcError     : 1;
        WORD EraseSeqError   : 1;
        WORD AddressError    : 1;
        WORD ParamError      : 1;
        WORD ZeroBit         : 1;
    } B;
    WORD I;
} MMC_RESPONSE_R1;

typedef union {
    struct {
        WORD CardLocked      : 1;
        WORD WpeLucf         : 1;
        WORD Error           : 1;
        WORD CCError         : 1;
        WORD CardEccFail     : 1;
        WORD WpViolation     : 1;
        WORD EraseParam      : 1;
        WORD OorCsdovw       : 1;
        WORD Idle            : 1;
        WORD EraseReset      : 1;
        WORD IllCommand      : 1;
        WORD ComCrcError     : 1;
        WORD EraseSeqError   : 1;
        WORD AddressError    : 1;
        WORD ParamError      : 1;
    } B;
    WORD I;
} MMC_RESPONSE_R2;

typedef struct {
    DWORD dwOcr;
    MMC_RESPONSE_R1 R1;
} MMC_RESPONSE_R3;

typedef union {
    MMC_RESPONSE_R1 R1;
    MMC_RESPONSE_R2 R2;
    MMC_RESPONSE_R3 R3;
} MMC_RESPONSE;
    

///////////////////////////////////////////////////////////////////////////////
// MMC prototypes
///////////////////////////////////////////////////////////////////////////////

// functions that map directly to MMC commands
RETCODE _reentrant HalMmcGoIdleStateSync(MMC_PHYSICAL_BUS * pBusPointer, MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcSendOpCondSync(MMC_PHYSICAL_BUS * pBusPointer, MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcSendCidSync(MMC_PHYSICAL_BUS * pBusPointer, WORD _X pwRegBuffer[],
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcSendCsdSync(MMC_PHYSICAL_BUS * pBusPointer, WORD _X pwRegBuffer[],
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcAppCmdSync(MMC_PHYSICAL_BUS * pBusPointer, MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcReadOcrSync(MMC_PHYSICAL_BUS * pBusPointer, MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcSendStatusSync(MMC_PHYSICAL_BUS * pBusPointer, MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcReadSingleBlockSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    WORD _X pwReadBuffer[], MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcProgramCsdSync(MMC_PHYSICAL_BUS * pBusPointer, WORD _X pwRegBuffer[],
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcSetBlocklenSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwLength, 
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcBusInit(MMC_PHYSICAL_BUS * pBusPointer);

RETCODE _reentrant HalMmcWriteBlockSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    WORD _X pwWriteBuffer[], MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcWriteMultipleBlockFirstSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    WORD _X pwWriteBuffer[], MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcWriteMultipleBlockNextSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    WORD _X pwWriteBuffer[], MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcWriteMultipleBlockEndSync(MMC_PHYSICAL_BUS * pBusPointer);
RETCODE _reentrant HalMmcTagSectorStartSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcTagSectorEndSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcTagEraseGroupStartSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcTagEraseGroupEndSync(MMC_PHYSICAL_BUS * pBusPointer, DWORD dwAddress,
    MMC_RESPONSE * Response);
RETCODE _reentrant HalMmcErase(MMC_PHYSICAL_BUS * pBusPointer, MMC_RESPONSE * Response);

// functions that map directly to SD commands
RETCODE _reentrant HalSdSendOpCondSync(MMC_PHYSICAL_BUS * pBusPointer, MMC_RESPONSE * Response);

// other HAL functions
#ifdef MMC_SEMAPHORE
RETCODE _reentrant HalMmcPhysicalGrab(WORD wNum);
RETCODE _reentrant HalMmcPhysicalRelease(WORD wNum);
#endif
BOOL _reentrant HalMmcIsDeviceInserted(MMC_PHYSICAL_BUS * pBusPointer);

// ancillary functions
RETCODE _reentrant MmcSendCommandSync(MMC_PHYSICAL_BUS * pBusPointer, WORD wCommand, 
    DWORD dwArgument, MMC_RESPONSE * Response, MMC_RESPONSE_TYPE ResponseType);
RETCODE _reentrant MmcWaitForRegisterSync(MMC_PHYSICAL_BUS * pBusPointer);
RETCODE _reentrant MmcTransactionCompleteSync(MMC_PHYSICAL_BUS * pBusPointer);
RETCODE _reentrant MmcCalcCommandCrc(MMC_CMD * pCmd);

#endif // #ifndef _MMCHAL_H
