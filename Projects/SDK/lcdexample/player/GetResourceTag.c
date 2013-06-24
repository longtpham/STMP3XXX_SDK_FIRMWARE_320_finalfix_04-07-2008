/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2006
//
// Filename: GetResourceTag.c
// 

#include "filesystem.h"
#include "ddildl.h" // in  \inc  DriveErase call here probably needs this. 
#include "types.h"        // in \inc
#include "persist_bits.h" // in \inc
#include "RestoreDriveFromBackup.h"

extern WORD g_wPlayerResourceTag;

//------------------------------------------------------------------------------
// Inputs:      int destination drive to erase and restore. int source drive read (logical drive nums)
//
//------------------------------------------------------------------------------
// Description: Changes player resource tag global if demanded by refresh resource.bin persistant bit. 
// Inputs: none
// Outputs: Returns value of resource tag that is commanded by sticky bit. 
// Notes:   This also sets value of static bRefreshResourceDriveStickyBitReadResult
//          So this function must be called before calling RestoreDriveFromBackup()
//
_reentrant WORD SysGetDesiredResourceTag(void)
{
    if (IsSystemDriveDirty() == TRUE)
    {
        g_wPlayerResourceTag = MASTER_SYSTEM_DRIVE_TAG;
    }
    else
    {
        g_wPlayerResourceTag = FIRST_SYSTEM_DRIVE_TAG;
    }

    return g_wPlayerResourceTag; 
}

WORD GetFirstSystemDriveNumber()
{
    // Get drive number of first copy of resource.bin
    return FIRST_SYSTEM_DRIVE_NUMBER;
}

WORD GetSecondSystemDriveNumber()
{
    // Get drive number of second copy of resource.bin
    return SECOND_SYSTEM_DRIVE_NUMBER;
}

WORD GetMasterSystemDriveNumber()
{
    // Get drive number of third copy of resource.bin
    return MASTER_SYSTEM_DRIVE_NUMBER;
}
