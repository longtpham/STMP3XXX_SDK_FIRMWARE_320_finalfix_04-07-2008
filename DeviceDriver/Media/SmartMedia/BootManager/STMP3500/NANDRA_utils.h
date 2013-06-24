/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: NANDRAUtils.h
// Description: Defs and function prototypes for Redundant Area Access Functions
/////////////////////////////////////////////////////////////////////////////////


#ifndef NANDRA_UTILS_H
#define NANDRA_UTILS_H 1


/////////////////////////////////////////////////////////////////////////////////
//  Includes
/////////////////////////////////////////////////////////////////////////////////
//#include "../COMMON/nandstructs.h"
#include "types.h"
//#include "nandddi.h"


// Declare Memory Spaces To Use When Coding
// A. Sector Buffers
#define SECTOR_BUFFER_MEM_X _X
#define SECTOR_BUFFER_MEM_Y _Y
#define SECTOR_BUFFER_MEM SECTOR_BUFFER_MEM_X
//typedef WORD SECTOR_BUFFER_MEM SECTOR_BUFFER;
//typedef SECTOR_BUFFER * P_SECTOR_BUFFER;
#define SECTOR_BUFFER   (WORD SECTOR_BUFFER_MEM)
// B. Media DDI Memory
#define MEDIA_DDI_MEM _Y



/////////////////////////////////////////////////////////////////////////////////
//  Definitions
/////////////////////////////////////////////////////////////////////////////////

#define RA_ECC_POS                  (SectorSizeInWordsPlusOne + 2)
#define WHOLE_SECTOR TRUE
#define RA_ONLY FALSE

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


////////////////////ECC Error Defines///////////////////////////////////////////
#define   COMPUTE_ECC_SUCCESS               0
#define   COMPUTE_ECC_NOT_DONE              0x516

#define   NANDHAL_ECC_NOERROR               0
#define   NANDHAL_ECC_ERROR_FIXED           0x519
#define   NANDHAL_ECC_FIX_FAILED            0x51A




/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////
WORD _reentrant NANDRA_ToStandardForm(WORD _X * pRA, BOOL bWholeSector);
WORD _reentrant NANDRA_ToNANDForm(WORD _X * pRA, BOOL bWholeSector);
WORD _reentrant NANDRAGetByte(WORD _X * pRA, BOOL bWholeSector, WORD wByteOffsetInRA,
                         WORD *pwValue);
WORD _reentrant NANDRASetByte(WORD _X * pRA, BOOL bWholeSector, WORD wByteOffsetInRA,
                         WORD wValue);
WORD _reentrant NANDRAGetCRC(WORD _X * pRA, BOOL bWholeSector,
                         WORD *pwCRC);
WORD _reentrant NANDRA_CRCCorrect(struct NANDDescriptorStruct _P *pNANDDesc, WORD _X * pRA,
                          WORD iDeviceNum, WORD iSectorNum);

WORD _reentrant NANDRAGetLBA(WORD _X * pRA, BOOL bWholeSector,
                 WORD *pwLBA, WORD *pwLSA);

WORD _reentrant NANDRAGetBlockStatus(WORD _X * pRA, BOOL bWholeSector,
                         WORD *pwBlockStatus);

WORD _reentrant NANDRAGetStmpCode(WORD _X * pRA, BOOL bWholeSector,
                          WORD *pwStmpCode);

#endif


/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////  EOF  //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
