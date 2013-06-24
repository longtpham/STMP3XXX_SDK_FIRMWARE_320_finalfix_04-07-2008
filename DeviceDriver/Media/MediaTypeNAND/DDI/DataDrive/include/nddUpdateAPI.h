/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: nddUpdateAPI.h
// Description: API definitions for Data Drive Update Blocks module
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//  Handy macros
/////////////////////////////////////////////////////////////////////////////////



#define bIsHBlock(pRegion, wBlkAddr) \
    (((pRegion)->iHBlksPhysAddr <= (wBlkAddr)) && ((wBlkAddr) < ((pRegion)->iHBlksPhysAddr + (pRegion)->iNumHBlks)))

#define bIsUBlock(pRegion, wBlkAddr) \
    (((pRegion)->iUBlksPhysAddr <= (wBlkAddr)) && ((wBlkAddr) < ((pRegion)->iUBlksPhysAddr + (pRegion)->iNumUBlks)))





/////////////////////////////////////////////////////////////////////////////////
//  API Funciton Prototypes
/////////////////////////////////////////////////////////////////////////////////



RETCODE NANDDataDriveKillUSectors(P_NAND_REGION_INFO pRegion,
                                 WORD wHBlkAddr,
                                 WORD wUBlkAddr,
                                 WORD wUSectorIdx, 
                                 WORD wSectorCount,
                                 WORD *pwNewUBlkAddr);

RETCODE NANDDataDriveKillUpdate(P_NAND_REGION_INFO pRegion, 
                                WORD wUBlkAddr);


RETCODE NANDDataDriveAddUSector(P_NAND_REGION_INFO pRegion, 
                                WORD wUBlkAddr, 
                                WORD wUSectorIdx, 
                                WORD wHSectorIdx);

_reentrant RETCODE NANDDataDriveCloseUpdate(P_NAND_REGION_INFO pRegion, 
                                 WORD wHRBlockAddr, 
                                 WORD wUBlkAddr,
                                 BOOL bPreserveHSectors);

RETCODE NANDDataDriveStartNewUpdate(P_NAND_REGION_INFO pRegion, 
                                    WORD wHBlockAddr, 
                                    WORD *pwUBlkAddr);

RETCODE NANDDataDriveLookupUBlock(P_NAND_REGION_INFO pRegion, 
                                  WORD wHBlkAddr, 
                                  WORD *pwUBlkAddr);

RETCODE NANDDataDriveLookupUSector(P_NAND_REGION_INFO pRegion, WORD wUBlkAddr, 
                                   WORD wHSectorIdx, WORD *pwUSectorIdx);

_reentrant RETCODE NANDDataDriveRecoverBadUBlock(P_NAND_REGION_INFO pRegion, 
                                      WORD wOldUBlkAddr, 
                                      WORD wNumSectors, 
                                      WORD *pwNewUBlkAddr);


RETCODE NANDDataDriveMarkAndMapBadBlock(P_NAND_REGION_INFO pRegion, WORD wBlockAddr, int iMapping);

WORD  DivideByThree(WORD wVal, WORD _Y *pRemainder);


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
