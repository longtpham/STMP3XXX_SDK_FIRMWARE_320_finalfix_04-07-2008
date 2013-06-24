////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: main.c
// Description: DDI Test
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel.h"
#include "ddildl.h"
#include "RAMDdi.h"

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

RETCODE MyMultiWriteCallBack(LONG lDunno);

//Required by kernel library
void ChipUpdateCapability(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

// These sector buffers are ok at 176 because this uses the RAM HAL and not NAND code
SECTOR_BUFFER pReadBuffer[176];
SECTOR_BUFFER pWriteBuffer[176];

WORD g_ResourceRead = 0;
short g_ResourceByteIndex = 0;
short g_ResourceByteLength = 0;

extern WORD g_wCapCheckCounter;

////////////////////////////////////////////////////////
// The following are known to a project at compile time:
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Main
//
//   Type:          Function
//
//   Description:   Main entry point
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant Task0EntryPoint(void)
{
    WORD i,j, k;
    BOOL bComplete;
    RETCODE rtCode;
    WORD wTag;
    WORD wResourceDriveNumber;
    BOOL bFound;
    DWORD dwNumSectors;
    DWORD dwSectorCount;
    
    // Init the media
    if(MediaInit(0) != SUCCESS)
    {
        SystemHalt();
    }
    
    // Allocate the test drives
    if(MediaDiscoverAllocation(0) != SUCCESS)
    {
        SystemHalt();
    }
    
    // Init the drives
    for(i=0;i<g_wNumDrives;i++)
    {
        if(DriveInit(i) != SUCCESS)
        {
        }
    }
    
    // Find the resource.bin drive
    bFound = FALSE;
    wResourceDriveNumber = 0;
    for(i=0;i<g_wNumDrives;i++)
    {
        if(DriveGetInfo(i, DriveInfoTag, &wTag) != SUCCESS)
        {
            SystemHalt();
        }
        
        if(wTag == DRIVE_TAG_RESOURCE_BIN)
        {
            wResourceDriveNumber = (WORD)i;
            bFound = TRUE;
            break;
        }
    }
    
    if(!bFound)
        SystemHalt();
    
    //
    // Write & verify all the sectors of all drives
    //
    
    // First, loop through all drives & write patterns to all sectors
    dwSectorCount = 1;
    for(i=0;i<g_wNumDrives;i++)
    {
        // First get the total number of sectors for the drive
        if(DriveGetInfo(i, DriveInfoSizeInSectors, &dwNumSectors) != SUCCESS)
        {
            SystemHalt();
        }
    
        // Now loop through all sectors
        for(j=0;j<dwNumSectors;j++)
        {
            // Init the sector
//            for(k=0;k<171;k++)    //tt RAM_SECTOR_SIZE is actually 170
            for(k=0;k<RAM_SECTOR_SIZE_IN_WORDS;k++)
            {
                pWriteBuffer[k] = (WORD)(dwSectorCount*0x111111); 
            }
        
            // Write the sector
            if(DriveWriteSector(i, j, pWriteBuffer) != SUCCESS)
            {
                SystemHalt();
            }
            
            // Bump the sector count
            dwSectorCount++;
        }
    }
    
    // Now loop through all drives & verify each sector
    dwSectorCount = 1;
    for(i=0;i<g_wNumDrives;i++)
    {
        // First get the total number of sectors for the drive
        if(DriveGetInfo(i, DriveInfoSizeInSectors, &dwNumSectors) != SUCCESS)
        {
            SystemHalt();
        }
        
        // Now loop through all sectors
        for(j=0;j<dwNumSectors;j++)
        {
            // Init the read sector
//            for(k=0;k<171;k++)    //tt RAM_SECTOR_SIZE is actually 170
            for(k=0;k<RAM_SECTOR_SIZE_IN_WORDS;k++)
            {
                pReadBuffer[k] = 0; 
            }
    
            // Read the sector
            if(DriveReadSector(i, j, pReadBuffer) != SUCCESS)
            {
                SystemHalt();
            }
            
            // Verify the sector
//            for(k=0;k<171;k++)    //tt RAM_SECTOR_SIZE is actually 170
            for(k=0;k<RAM_SECTOR_SIZE_IN_WORDS;k++)
            {
                pWriteBuffer[k] = (WORD)(dwSectorCount*0x111111); 
            }

//            for(k=0;k<171;k++)    //tt RAM_SECTOR_SIZE is actually 170
            for(k=0;k<RAM_SECTOR_SIZE_IN_WORDS;k++)
            {
                if(pReadBuffer[k] != pWriteBuffer[k])
                {                  
                    SystemHalt();
                }
            }
            
            // Bump the sector count
            dwSectorCount++;
        }
    }    
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // SUCCESS
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    SystemHalt();
}
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    // SUCCESS
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MyMultiWriteCallBack
//
//   Type:          Function
//
//   Description:   
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE MyMultiWriteCallBack(LONG lDunno)
{
    return SUCCESS;
}

// Stubs
void ChipUpdateCapability(void)
{
	;
}

// use this for tests that must halt instead of ver that resets the DSP in RETAIL build. 
#pragma asm
 global FatalError
FatalError:
    debug
    nop
#pragma endasm

    


