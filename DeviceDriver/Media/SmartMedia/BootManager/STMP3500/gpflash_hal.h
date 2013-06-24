/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: cf_hal.h
// Description: Function prototypes for Compact Flash version of NAND Hardware API
/////////////////////////////////////////////////////////////////////////////////


_reentrant WORD InitGPFLASHPort(WORD iDeviceNum);
_reentrant WORD WriteCmdGPFLASH(WORD iDeviceNum, WORD iCommandCode, WORD bDeassertCEn);
_reentrant WORD WriteAddrGPFLASH(WORD iDeviceNum, WORD iNumColBytes, WORD iColumnAddress, 
                                  WORD iNumRowBytes, WORD iRowAddress, WORD bDeassertCEn);
_reentrant WORD ReadDataGPFLASH(WORD iDeviceNum, WORD iNumBytesToRead, WORD *pBuffer, WORD b16BitMode);

_reentrant WORD WaitForReadyGPFLASH(WORD iDeviceNum, WORD iTimeoutCount);
_reentrant WORD PowerUpFlashPads(void);
_reentrant WORD PowerUpFlashPads_16bit(void);
_reentrant WORD TerminateGPFLASHPort(WORD iDeviceNum);


// WaitForREADY() timeout counts
// These control how long we will wait for the NAND to finish some internal operation
// (e.g. Read Page (to cache), Program Page (from cache), Erase Block, Reset).
// NOTE: For timeouts, each "count" is on the order of 50 nsec.
// (<10 instructions, at a few 10s of nanosecs each)
// The settings below are probably very conservative, but still short in human time
// (e.g. 2000000 counts is approx. 100 msec)
#define NAND_READ_PAGE_TIMEOUT  (2000000)
#define NAND_READ_REDUNDANT_TIMEOUT  (2000000)
#define NAND_RESET_TIMEOUT  (2000000)

#define DEASSERT_CEB_AT_END        TRUE
#define LEAVE_CEB_ASSERTED_AT_END  FALSE

