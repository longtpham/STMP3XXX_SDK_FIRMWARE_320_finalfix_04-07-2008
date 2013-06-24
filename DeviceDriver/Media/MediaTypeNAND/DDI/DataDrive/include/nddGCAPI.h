/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: nddGCAPI.h
// Description: Garbage Collector API and defines
/////////////////////////////////////////////////////////////////////////////////


// The following are called by application code
RETCODE NANDForegroundGarbageCollector(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);

RETCODE NANDBackgroundGarbageCollector(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);
RETCODE NANDBackgroundGarbageCollectorEnable(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);
RETCODE NANDBackgroundGarbageCollectorDisable(P_LOGICAL_DRIVE_DESCRIPTOR pLogicalDataDrive);


// The following is called by other NAND Data Drive routines
RETCODE _reentrant NANDDataDriveFreshenUpdate(P_NAND_REGION_INFO pRegion, WORD wUBlkAddr);

// Physical block 0 will never be a true UBlock.  This is used for "dummy" update freshening,
// to push other updates down in the FreshList - i.e. to make them get a little "stale"
#define FRESHEN_DUMMY_UBLOCK (0)



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
