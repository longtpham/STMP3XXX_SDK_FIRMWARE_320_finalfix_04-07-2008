/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: NANDRAUtils.h
// Description: Defs and function prototypes for Redundant Area Access Functions
/////////////////////////////////////////////////////////////////////////////////


#ifndef NANDRA_UTILS_H
#define NANDRA_UTILS_H 1


/////////////////////////////////////////////////////////////////////////////////
//  Includes
/////////////////////////////////////////////////////////////////////////////////
#include "nandddi.h"



/////////////////////////////////////////////////////////////////////////////////
//  Definitions
/////////////////////////////////////////////////////////////////////////////////

#define BLOCK_STATUS_MAJORITY    1


// Defines of offsets, relative to the beginning of the Redundant Area
// Layout 2


// The RA CRC covers bytes 1-6
#define RA_CRC_DATA_OFFSET      1
#define RA_CRC_SIZE             6

/////////////////////////////////////////////////////////////////////////////////
//  IMPORTANT: The SWAP definitions below assume Layout 2S and 2N, as described
//  above.  If the layouts are changed, these values, or even the whole NANDRaSwap()
//  routine may need to be changed.
/////////////////////////////////////////////////////////////////////////////////

#define RA_SWAP_BYTE_OFFSET_1   1
#define RA_SWAP_BYTE_OFFSET_2   7

#define RA_ECC_POS                  (g_NandAuxData.SectorDataSizeInWords + 2)
#define WHOLE_SECTOR                (g_NandAuxData.SectorDataSizeInBytes)
#define RA_ONLY                     (0)

#define SEC_512_BYTES_RA_ECC_POS    173

    // "ST" code
#define ST_TAG (0x005453)

// When the number of ECC corrections during a sector read meets or exceeds
// this threshold, we'll switch to a 2nd copy of resource.bin and load from it and refresh 1st copy of it.
#define ECC_REWRITE_THRESHOLD   4

#define RA_CRC_OFFSET			0
#define RA_LBA_1_OFFSET			1
#define RA_STMP_TAG_1_OFFSET		1
#define RA_RSI_0_OFFSET			2
#define RA_RSI_1_OFFSET			3
#define RA_CUSTOMER_BYTE_OFFSET 	4
#define RA_BLOCK_STATUS_OFFSET		5
#define RA_LBA_0_OFFSET			6
#define RA_STMP_TAG_0_OFFSET		6
#define RA_RS_ECC_OFFSET		7

/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant NANDRASwap(P_SECTOR_BUFFER pRA, WORD wSectorOffset);

RETCODE NANDRAInit(P_SECTOR_BUFFER pRA, WORD wSectorOffset);

#define NANDRA_ToStandardForm(pRA, wSectorOffset) NANDRASwap(pRA, wSectorOffset)
#define NANDRA_ToNANDForm(pRA, wSectorOffset) NANDRASwap(pRA, wSectorOffset)

RETCODE _reentrant NANDRAGetByte(P_SECTOR_BUFFER pRA, WORD wSectorOffset, WORD wByteOffsetInRA,
                         WORD *pwValue);
RETCODE NANDRASetByte(P_SECTOR_BUFFER pRA, WORD wSectorOffset, WORD wByteOffsetInRA,
                         WORD wValue);
#define NANDRAGetCRC(pRA, wSectorOffset, pwCRC) NANDRAGetByte(pRA, wSectorOffset, RA_CRC_OFFSET, pwCRC)

RETCODE NANDRAFillCRC(P_SECTOR_BUFFER pRA, WORD wSectorOffset);
RETCODE _reentrant NANDRA_CRCCorrect(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDesc, P_SECTOR_BUFFER pRA, 
                          int iSectorNum);
RETCODE  _reentrant NANDRAGetLBA(P_SECTOR_BUFFER pRA, WORD wSectorOffset,
                 WORD *pwLBA, WORD *pwLSA);
RETCODE NANDRASetLBA(P_SECTOR_BUFFER pRA, WORD wSectorOffset,
                 WORD wLBA, WORD wLSA);
#define NANDRAGetBlockStatus(pRA, wSectorOffset, pwBlockStatus) NANDRAGetByte(pRA, wSectorOffset, RA_BLOCK_STATUS_OFFSET, pwBlockStatus)
#define NANDRASetBlockStatus(pRA, wSectorOffset, wBlockStatus) NANDRASetByte(pRA, wSectorOffset, RA_BLOCK_STATUS_OFFSET, wBlockStatus)
RETCODE _reentrant NANDRAGetStmpCode(P_SECTOR_BUFFER pRA, WORD wSectorOffset,
                          WORD *pwStmpCode);
RETCODE NANDRASetStmpCode(P_SECTOR_BUFFER pRA, WORD wSectorOffset,
                          WORD wStmpCode);

#define NANDRAGetCustomerByte(pRA, wSectorOffset, pwCustomerByte) NANDRAGetByte(pRA, wSectorOffset, RA_CUSTOMER_BYTE_OFFSET, pwCustomerByte)

#define NANDRASetCustomerByte(pRA, wSectorOffset, wCustomerByte) NANDRASetByte(pRA, wSectorOffset, RA_CUSTOMER_BYTE_OFFSET, wCustomerByte)

BOOL _reentrant bBlockMarkedBadInRA(P_SECTOR_BUFFER pRA, WORD wSectorOffset);


#endif


/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  EOF  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
