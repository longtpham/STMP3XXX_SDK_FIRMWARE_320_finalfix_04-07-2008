/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: nddUpdateInternals.h
// Description: Definition for Data Drive Update Blocks Internal routines.
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
//  Handy macros
/////////////////////////////////////////////////////////////////////////////////



#define UBlkAddrToUBlkIdx(pRegion, wUBlkAddr) \
    ((wUBlkAddr) - (pRegion)->iUBlksPhysAddr)

#define UBlkIdxToUBlkAddr(pRegion, wUBlkIdx) \
    ((wUBlkIdx) + (pRegion)->iUBlksPhysAddr)


//#define bNANDMultiPlanesMatch(Block1, Block2, Mask) \
//    (((Block1) & (Mask)) == ((Block2) & (Mask)))
#define bNANDMultiPlanesMatch(Block1, Block2, Mask)     (TRUE)




#define LBA_VALUE_ERASED (0xffff)




/////////////////////////////////////////////////////////////////////////////////
//  Function Prototypes
/////////////////////////////////////////////////////////////////////////////////


_reentrant RETCODE  NANDDataDriveGetFreeErasedUBlock(P_NAND_REGION_INFO pRegion, 
                                          WORD wOldHUBlockAddr, 
                                          WORD *pwNewUBlockAddr);

_reentrant RETCODE NANDDataDriveGetRecycledUBlock(P_NAND_REGION_INFO pRegion, 
                                       WORD wBlockAddrForPlane, 
                                       WORD *pwUBlkAddr);


RETCODE NANDDataDriveSortEntryInUTable(P_NAND_REGION_INFO pRegion, 
                                       WORD wUTblIdx);

RETCODE NANDDataDriveUBlkIdxToUTblIdx(P_NAND_REGION_INFO pRegion, 
                                      WORD wUBlkIdx, 
                                      WORD *pwUTblIdx);

RETCODE NANDDataDriveGetUSectorsMap(P_NAND_REGION_INFO pRegion, 
                                    WORD wUBlkIdx, 
                                    P_UPDATE_SECTORS_MAP_DESCRIPTOR *ppUSectorsMap);

RETCODE NANDDataDriveBuildUSectorsMap(P_NAND_REGION_INFO pRegion, 
                                      WORD wUBlkIdx, 
                                      WORD *pwUSectorsMapIdx);

RETCODE NANDDataDriveGetBlankUSectorsMap(
    P_NAND_REGION_INFO pRegion,
    WORD wUBlkIdx,
    WORD *pwUSectorsMapIdx);

_reentrant RETCODE NANDDataDriveProtectUBlock(P_NAND_REGION_INFO pRegion, 
                      WORD wUBlkIdx);

RETCODE NANDDataDriveUnprotectUBlock(P_NAND_REGION_INFO pRegion,
                        WORD wUBlkIdx);

BOOL bNANDDataDriveUBlockIsProtected(P_NAND_REGION_INFO pRegion, 
                        WORD wUBlkIdx);

//WORD wNANDDataDriveGetMultiPlaneMask(P_NAND_REGION_INFO pRegion);
#define wNANDDataDriveGetMultiPlaneMask(a)          (0)


WORD iNANDDataDriveGetLBAFromRA(WORD _X pRABuffer[]);

WORD iNANDDataDriveGetRSIFromRA(WORD _X pRABuffer[]);


BOOL _reentrant bFindFreeUBlock(P_NAND_REGION_INFO pRegion, int *piUTblIdx);

RETCODE NANDDataDriveChangeUBlockMapping(P_NAND_REGION_INFO pRegion, WORD wUBlkAddr, WORD wUBlkMapping);

RETCODE _reentrant NANDDataDriveKillUSectorsMap(P_NAND_REGION_INFO pRegion, WORD wUBlkAddr);

RETCODE NANDDataDriveAddUSectorWithoutFreshen(P_NAND_REGION_INFO pRegion, WORD wUBlkAddr, WORD wUSectorIdx, WORD wHSectorIdx);


// Garbage Collector routines
BOOL  _reentrant bNANDDataDriveUpdateIsFresh(P_NAND_REGION_INFO pRegion, WORD wUBlkAddr);
RETCODE  _reentrant NANDDataDriveFindStaleUpdate(P_NAND_REGION_INFO pRegion, WORD *pwUBlkAddr);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
