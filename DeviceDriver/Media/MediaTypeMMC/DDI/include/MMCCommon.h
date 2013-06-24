////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: MMCCommon.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _MMC_COMMON_H
#define _MMC_COMMON_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
//#define SECTOR_SIZE                     (528/3)
#define MMC_BUFFER_SECTOR_SIZE            (528/3) // 528 so buffers can be shared 
                                                  // with NAND.
#define MMC_SECTOR_SIZE                   (512/3)
///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MmcSerialNumberInit(P_MMC_MEDIA_INFO pMediaInfo);
RETCODE _reentrant MmcEnumeratePhysicalDevice(P_MMC_MEDIA_INFO pMediaInfo, WORD wNum);
RETCODE _reentrant ResetDevice(P_MMC_MEDIA_INFO pMediaInfo);
RETCODE _reentrant ProcessOcr(P_MMC_MEDIA_INFO pMediaInfo);
RETCODE _reentrant ProcessCsd(P_MMC_MEDIA_INFO pMediaInfo);
RETCODE _reentrant ProcessCid(P_MMC_MEDIA_INFO pMediaInfo);
RETCODE _reentrant MmcTestErase(P_MMC_MEDIA_INFO pMediaInfo, DWORD dwStartBlockAddress,
                                DWORD dwEndBlockAddress);
RETCODE _reentrant HalMmcEraseBlocks(P_MMC_MEDIA_INFO pMediaInfo,
                                     DWORD dwStartBlockAddress, DWORD dwEndBlockAddress);

BOOL _reentrant MmcIsHardwareWriteProtected(P_MMC_MEDIA_INFO pMediaInfo);

BOOL _reentrant MmcMultiBlockWriteIsSupported(P_MMC_MEDIA_INFO pMediaInfo);

RETCODE _reentrant MmcFlagMediaChange(void);

RETCODE _reentrant MMCDetectMedia(P_MMC_MEDIA_INFO pMmcMediaInfo);
BOOL _reentrant MMCDetectMediaChange (P_MMC_MEDIA_INFO pMmcMediaInfo);

#endif // #ifndef _MMC_COMMON_H
