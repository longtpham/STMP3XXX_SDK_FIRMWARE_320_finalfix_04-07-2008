////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename:    task0.c
// Description: main() Program for Mmc Media DDI Test
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel.h"
#include "ddildl.h"
#include "MMCDdi.h"
#include "MMCMedia.h"
#include "regsclkctrl.h"
#include "regsrtc.h"
#ifdef D3500
#include "regsgpflash.h"
#include "regsgpio.h"
#endif


////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////
extern MMC_MEDIA_INFO MEDIA_DDI_MEM MmcMediaInfo;
extern SECTOR_BUFFER   MMCMediaSectorBuffer[];
////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

RETCODE Test1(void);
RETCODE TestDataDrive(void);
void TestCallBackFunction(LONG lValue);
DWORD GetDclkCount(void);
void SetDclkCount(DWORD dwCount);
DWORD SysGetCurrentTime(void);
RETCODE Hackage(void);
RETCODE DoDaMultiWrite(
    WORD wDriveNum,
    DWORD dwStartSector,
    WORD wNumSectors);

void ChipUpdateCapability(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#define NUM_MULTI_WRITE_BUFFERS 10
SECTOR_BUFFER   pMultiWriteBuffer[NUM_MULTI_WRITE_BUFFERS][178]; 
BOOL bMultiWriteBufferFree[NUM_MULTI_WRITE_BUFFERS];
WORD wMultiWriteBufferIdx;
SECTOR_BUFFER   pWriteBuffer[178]; 
SECTOR_BUFFER   pReadBuffer[178]; 

DWORD dwSector, dwNumSectors;
WORD g_wSector;
WORD g_rtCode;
WORD g_wStartSector;
WORD g_wNumSectors;
WORD g_wRandomCount;
#define NUM_RANDOM_LOGS 5
DWORD dwMultiWriteTimeStart;
DWORD dwMultiWriteTimeEnd;
DWORD dwMultiWriteTimeTotal;

// needed for MMC driver to link and fully enumerate. Simulates granting of high current by USB host.  
 #define USB_HIGH_CURRENT_GRANTED 3
int g_iHighPwrPeriphInitMode = USB_HIGH_CURRENT_GRANTED;

/////////////////////////////////////////////////////////////////////////////////
//  
// You can set g_bAllocate to FALSE in the debugger, to prevent calling MediaAllocate().
// That will allow you to analyze or test an existing configuration.
//
/////////////////////////////////////////////////////////////////////////////////
BOOL g_bAllocate = TRUE;

/////////////////////////////////////////////////////////////////////////////////
//
// You can set g_bAnalyze to TRUE in the debugger, to cause the program to stop at various points,
// allowing you to analyze various data structures.  
//
// It will stop after MMCMediaDiscoverAllocation(), allowing you to examine the MMCInfo table, 
// MMCMediaInfo contains all the information about the MMC card including the CSD, CID and
// size information needed for proper operation.
//
// It will stop after the Data Drive Single Sector Read/Write test.
// It will stop after the Data Drive Multiple Sector Read/Write test.
//
/////////////////////////////////////////////////////////////////////////////////
BOOL g_bAnalyze = FALSE;


extern WORD g_wCapCheckCounter;

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

#if defined(D3500)
#ifdef USE_8MA_GPFLAH_PADS
    // Set 8mA drive for the GPFlash pins
    HW_GP08MA.I = 7;
    HW_GP18MA.I = 7;
    HW_GP28MA.I = 7;
    HW_GP38MA.I = 7;
#endif // USE_8MA_GPFLAH_PADS
#endif // DEVICE_3500

    Test1();
    
    // SUCCESS!!!!!
    SystemHalt();
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Test1
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
RETCODE Test1(void)
{
    WORD wMagicNumber = 0;                // We don't have this implemented yet.
    INT i, iBadBlockCounter = 0;
    SYSTEM_VERSION SystemVersion;
    SYSTEM_VERSION SystemVersionCompare;

    // Call MMCMediaInit
    // This function will fill in parameters for:
    //     LOGICAL_MEDIA_DESCRIPTOR  (g_MediaDescriptor : declared in ddiproject.c)
    //     MMC_MEDIA_INFO   (MmcMediaInfo : declared in MMCMediaInit.c)
    //     MMC_PHYSICAL_MEDIA_DESCRIPTOR[] via MmcHalInit (g_MmcDescriptor[4] : 
    //            declared in ddiproject.c)
    if (MMCMediaInit(g_MediaDescriptor))
      // If fails here, check the A register for the error code in errors.inc)
      //   (ie. a value of 600h indicates the MMC card is not inserted.)
      SystemHalt();        // If MediaInit fails, HALT.


    if(g_bAllocate)
    {
      // If halts here, try ejecting the card then reset and restart debugger.
      if (MMCMediaAllocate(g_MediaDescriptor, g_MediaAllocationTable))
          SystemHalt();
    }    

    // If halts here, try ejecting the card then reset and restart debugger.
    if (MMCMediaDiscoverAllocation(g_MediaDescriptor))
        SystemHalt();

    /////////////////////////////////////////////////////////////////////////////////
    //  Stop for analysis
    /////////////////////////////////////////////////////////////////////////////////
    if (g_bAnalyze) {        
      SystemHalt();
      SystemHalt();
    }
    /////////////////////////////////////////////////////////////////////////////////
    //  Continue with tests
    /////////////////////////////////////////////////////////////////////////////////


    // Prepare drive system for writes
    if(DriveInit(g_MediaDescriptor->wMediaNumber) != SUCCESS)
    {
      SystemHalt();
    }

    if(g_bAllocate)
    {        
      if(DriveErase(g_MediaDescriptor->wMediaNumber, 0x00) != SUCCESS)
      {
        SystemHalt();
      }
    }


    if(TestDataDrive() != SUCCESS)
    {
        SystemHalt();
        SystemHalt();
    }

    /////////////////////////////////////////////////////////////////////////////////
    //  Stop for analysis
    /////////////////////////////////////////////////////////////////////////////////
    if (g_bAnalyze) {        
      SystemHalt();
      SystemHalt();
    }
    /////////////////////////////////////////////////////////////////////////////////
    //  Continue with tests
    /////////////////////////////////////////////////////////////////////////////////

/*
    if(Hackage() != SUCCESS)
    {
        SystemHalt();
        SystemHalt();
    }
 */

    return SUCCESS;
}



RETCODE TestDataDrive(void)
{
    INT i,j;
    WORD wDataDriveNum;
    BOOL bDataDriveFound = FALSE;
    LOGICAL_DRIVE_TYPE DriveType;
    
    // Find the Data Drive
    for(wDataDriveNum=0;wDataDriveNum<g_wNumDrives;wDataDriveNum++)
    {
        if(DriveGetInfo(
            wDataDriveNum,
            DriveInfoType,
            &DriveType) != SUCCESS)
        {
            return(!SUCCESS);
        }
        
        if(DriveType == DriveTypeData)
        {
            bDataDriveFound = TRUE;
            break;
        }
    }
    
    if(!bDataDriveFound)
    {
        return(!SUCCESS);
    }
    
    // Get the total number of sectors for the drive
    if(DriveGetInfo(
        wDataDriveNum,
        DriveInfoSizeInSectors,
        &dwNumSectors) != SUCCESS)
    {
        return(!SUCCESS);
    }

    /////////////////////////////////////////////////////////////////////////////////
    //  Single Sector Write Testing
    /////////////////////////////////////////////////////////////////////////////////

    // Write & re-write sectors
    for(g_wSector=0;g_wSector<(dwNumSectors/32);g_wSector++)
    {
        // Init the sector
        for(j=0;j<171;j++)
        {
            pWriteBuffer[j] = (WORD)(g_wSector); 
        }

        if(DriveWriteSector(
            wDataDriveNum,
            (DWORD)(g_wSector*32),
            pWriteBuffer) != SUCCESS)
        {
            return(!SUCCESS);
        }

        // Init the sector
        for(j=0;j<171;j++)
        {
            pWriteBuffer[j] = (WORD)(~g_wSector); 
        }

        if(DriveWriteSector(
            wDataDriveNum,
            (DWORD)(g_wSector*32),
            pWriteBuffer) != SUCCESS)
        {
            return(!SUCCESS);
        }
    }

    // Verify all sectors written
    for(i=0;i<(dwNumSectors/32);i++)
    {
        if(DriveReadSector(
            wDataDriveNum,
            (DWORD)(i*32),
            pReadBuffer) != SUCCESS)
        {
            return(!SUCCESS);
        }

        // Init the compare buffer
        for(j=0;j<171;j++)
        {
            pWriteBuffer[j] = (WORD)(~i); 
        }

        // Verify the sector
        for(j=0;j<170;j++)
        {
            if(pReadBuffer[j] != pWriteBuffer[j])
            {
                return(!SUCCESS);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////
    //  Stop for analysis
    /////////////////////////////////////////////////////////////////////////////////
    if (g_bAnalyze) {        
      SystemHalt();
      SystemHalt();
    }

    /////////////////////////////////////////////////////////////////////////////////
    //  MultiWrite Testing
    /////////////////////////////////////////////////////////////////////////////////

    for(i=0;i<NUM_MULTI_WRITE_BUFFERS;i++)
    {
        bMultiWriteBufferFree[i] = TRUE;
    }
    wMultiWriteBufferIdx = 0;

//    SetDclkCount(0);
//    dwMultiWriteTimeStart = SysGetCurrentTime();

    // Multi-write setup
    if(DriveMultiWriteSetup(
        wDataDriveNum,
        0,
        (WORD)(dwNumSectors),
        TestCallBackFunction) != SUCCESS)
    {
        SystemHalt();
        return(!SUCCESS);
    }

    // Write the sectors
    for(g_wSector=0;g_wSector<(WORD)dwNumSectors;g_wSector++)
    {
        if(bMultiWriteBufferFree[wMultiWriteBufferIdx] != TRUE)
        {
            SystemHalt();
        }
        
        bMultiWriteBufferFree[wMultiWriteBufferIdx] = FALSE;
        
        // Init the compare buffer
        for(j=0;j<171;j++)
        {
            pMultiWriteBuffer[wMultiWriteBufferIdx][j] =
                (WORD)(g_wSector); 
        }

        if((g_rtCode = DriveMultiWriteSector(
            wDataDriveNum,
            pMultiWriteBuffer[wMultiWriteBufferIdx],
            (LONG)wMultiWriteBufferIdx)) != SUCCESS)
        {
            SystemHalt();
            return(!SUCCESS);
        }
        
        wMultiWriteBufferIdx++;
        if(wMultiWriteBufferIdx == NUM_MULTI_WRITE_BUFFERS)
        {
            wMultiWriteBufferIdx = 0;
        }
    }

//    dwMultiWriteTimeEnd = SysGetCurrentTime();
//    dwMultiWriteTimeTotal = dwMultiWriteTimeEnd - dwMultiWriteTimeStart;

    // Verify the sectors
    for(i=0;i<dwNumSectors;i++)
    {
        if(DriveReadSector(
            wDataDriveNum,
            (DWORD)(i),
            pReadBuffer) != SUCCESS)
        {
            SystemHalt();
            return(!SUCCESS);
        }

        // Init the compare buffer
        for(j=0;j<171;j++)
        {
            pWriteBuffer[j] = (WORD)(i); 
        }

        // Verify the sector
        for(j=0;j<170;j++)
        {
            if(pReadBuffer[j] != pWriteBuffer[j])
            {
                SystemHalt();
                return(!SUCCESS);
            }
        }
    }

/*
    for(i=0;i<NUM_RANDOM_LOGS;i++)
    {
        RandomLog[i].wStartSector = 0xFFFFFF;
        RandomLog[i].wNumSectors = 0xFFFFFF;
    }
    
    srand((WORD)HW_DCLKCNTL.I);
    
    for(g_wRandomCount=0;g_wRandomCount<NUM_RANDOM_LOGS;g_wRandomCount++)
    {
        while(1)
        {
            if((g_wStartSector = rand()) + (g_wNumSectors = rand()) <
                dwNumSectors)
            {
                if(g_wNumSectors > 1)
                {
                    break;
                }
            }
        }

//        RandomLog[g_wRandomCount].wStartSector = g_wStartSector;
//        RandomLog[g_wRandomCount].wNumSectors = g_wNumSectors;
        
//        g_wStartSector = 0x46;
//        g_wNumSectors = 0x21;
//        g_wStartSector = 0x14;
//        g_wNumSectors = 12;
        
    for(j=0;j<NUM_MULTI_WRITE_BUFFERS;j++)
    {
        bMultiWriteBufferFree[j] = TRUE;
    }
    wMultiWriteBufferIdx = 0;
    
    if(DriveMultiWriteSetup(
        wDataDriveNum,
        (DWORD)g_wStartSector,
        g_wNumSectors,
        TestCallBackFunction) != SUCCESS)
    {
        SystemHalt();
        return(!SUCCESS);
    }

    // Write the sectors
    for(g_wSector=g_wStartSector;
        g_wSector<(g_wStartSector+g_wNumSectors);
        g_wSector++)
    {
        if(bMultiWriteBufferFree[wMultiWriteBufferIdx] != TRUE)
        {
            SystemHalt();
        }
    
        bMultiWriteBufferFree[wMultiWriteBufferIdx] = FALSE;
    
        // Init the compare buffer
        for(j=0;j<171;j++)
        {
            pMultiWriteBuffer[wMultiWriteBufferIdx][j] =
                (WORD)(g_wSector); 
        }

        if((g_rtCode = DriveMultiWriteSector(
            wDataDriveNum,
            pMultiWriteBuffer[wMultiWriteBufferIdx],
            (LONG)wMultiWriteBufferIdx)) != SUCCESS)
        {
            SystemHalt();
            return(!SUCCESS);
        }
    
        wMultiWriteBufferIdx++;
        if(wMultiWriteBufferIdx == NUM_MULTI_WRITE_BUFFERS)
        {
            wMultiWriteBufferIdx = 0;
        }
    }

    // Verify the sectors
    for(g_wSector=g_wStartSector;
        g_wSector<(g_wStartSector+g_wNumSectors);
        g_wSector++)
    {
        if(DriveReadSector(
            wDataDriveNum,
            (DWORD)(g_wSector),
            pReadBuffer) != SUCCESS)
        {
            SystemHalt();
            return(!SUCCESS);
        }

        // Init the compare buffer
        for(j=0;j<171;j++)
        {
            pWriteBuffer[j] = (WORD)(g_wSector); 
        }

        // Verify the sector
        for(j=0;j<170;j++)
        {
            if(pReadBuffer[j] != pWriteBuffer[j])
            {
                SystemHalt();
                return(!SUCCESS);
            }
        }
    }
*/    
    
    return SUCCESS;
}

/*
RETCODE Hackage(void)
{
    INT i,j;
    WORD wDataDriveNum;
    BOOL bDataDriveFound = FALSE;
    LOGICAL_DRIVE_TYPE DriveType;
    
    // Find the Data Drive
    for(wDataDriveNum=0;wDataDriveNum<g_wNumDrives;wDataDriveNum++)
    {
        if(DriveGetInfo(
            wDataDriveNum,
            DriveInfoType,
            &DriveType) != SUCCESS)
        {
            return(!SUCCESS);
        }
        
        if(DriveType == DriveTypeData)
        {
            bDataDriveFound = TRUE;
            break;
        }
    }
    
    if(!bDataDriveFound)
    {
        return(!SUCCESS);
    }
    
    // Get the total number of sectors for the drive
    if(DriveGetInfo(
        wDataDriveNum,
        DriveInfoSizeInSectors,
        &dwNumSectors) != SUCCESS)
    {
        return(!SUCCESS);
    }

    for(j=0;j<NUM_MULTI_WRITE_BUFFERS;j++)
    {
        bMultiWriteBufferFree[j] = TRUE;
    }
    wMultiWriteBufferIdx = 0;

    SystemHalt();
    
    if(DriveWriteSector(wDataDriveNum, 51, pWriteBuffer) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 112 , 8) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 112 , 8) != SUCCESS) SystemHalt();
    if(DriveWriteSector(wDataDriveNum, 52, pWriteBuffer) != SUCCESS) SystemHalt();
    if(DriveWriteSector(wDataDriveNum, 82, pWriteBuffer) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 112 , 8) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 128 , 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 256 , 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 384 , 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 512 , 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 640 , 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 768 , 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 896 , 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1024, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1152, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1280, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1408, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1536, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1664, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1792, 128) != SUCCESS) SystemHalt();
    if(DriveWriteSector(wDataDriveNum, 51, pWriteBuffer) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 1920, 128) != SUCCESS) SystemHalt();
    if(DriveWriteSector(wDataDriveNum, 52, pWriteBuffer) != SUCCESS) SystemHalt();
    if(DriveWriteSector(wDataDriveNum, 82, pWriteBuffer) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2048, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2176, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2304, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2432, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2560, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2688, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2816, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 2944, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3072, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3200, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3328, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3456, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3584, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3712, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3840, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 3968, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 4096, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 4224, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 4352, 128) != SUCCESS) SystemHalt();
    if(DoDaMultiWrite(wDataDriveNum, 4480, 128) != SUCCESS) SystemHalt();

    SystemHalt();
    
    return SUCCESS;
}
*/

/*
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
RETCODE DoDaMultiWrite(
    WORD wDriveNum,
    DWORD dwStartSector,
    WORD wNumSectors)
{
    // Multi-write setup
    if(DriveMultiWriteSetup(
        wDriveNum,
        dwStartSector,
        wNumSectors,
        TestCallBackFunction) != SUCCESS)
    {
        SystemHalt();
        return(!SUCCESS);
    }

    // Write the sectors
    for(g_wSector=0;g_wSector<wNumSectors;g_wSector++)
    {
        if(bMultiWriteBufferFree[wMultiWriteBufferIdx] != TRUE)
        {
            SystemHalt();
        }
        
        bMultiWriteBufferFree[wMultiWriteBufferIdx] = FALSE;
        
        if((g_rtCode = DriveMultiWriteSector(
            wDriveNum,
            pMultiWriteBuffer[wMultiWriteBufferIdx],
            (LONG)wMultiWriteBufferIdx)) != SUCCESS)
        {
            SystemHalt();
            return(!SUCCESS);
        }
        
        wMultiWriteBufferIdx++;
        if(wMultiWriteBufferIdx == NUM_MULTI_WRITE_BUFFERS)
        {
            wMultiWriteBufferIdx = 0;
        }
    }

    return SUCCESS;
}
*/

void TestCallBackFunction(LONG lValue)
{
    bMultiWriteBufferFree[lValue] = TRUE;
    return;
}

DWORD SysGetCurrentTime(void)
{
    return((HW_RTCUPR.I<<24) + HW_RTCLOWR.I);
/*9
    #pragma asm
        clr     a
        move    x:HW_RTCUP,a1               ; get the upper count
        move    x:HW_RTCLOW,a0              ; get the lower count
    #pragma endasm
*/
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetDclkCount
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        none          
//
//   Outputs:       DWORD               DCLK count
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
DWORD GetDclkCount(void)
{
    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SetDclkCount
//
//   Type:          Function
//
//   Description:   Duh
//
//   Inputs:        DWORD               DCLK count          
//
//   Outputs:       none               
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void SetDclkCount(DWORD dwCount)
{
    // TODO:  protect this from interrupts
    HW_DCLKCNTU.I = (WORD)(dwCount >> 24);
    HW_DCLKCNTL.I = (WORD)(dwCount);
}


//////////////////////////////////////////////////////
//
//>  Name:          WORD _reentrant rand(void)
//
//   Type:          Function
//
//   Description:   returns a pseudo random WORD
//
//   Inputs:        none
//
//   Outputs:       a pseudo random WORD
//
//   Notes:         right now, this only returns the number of 
//                  dclocks executed.   It makes an ok 8 bit 
//                  random number generator, but gets worse 
//                  as it gets wider.   This algorithm (or lack 
//                  thereof) can certainly be improved
//<
//////////////////////////////////////////////////////
static WORD next = 1;

WORD _reentrant rand( void )
{
    if(next == 1)
        next = (WORD)HW_DCLKCNTL.I;
    next = next * 1103515245 + 12345;
    return( (unsigned int)(next/65536) & (32768 - 1));
}

void _reentrant srand( WORD seed )
{
    next = seed;
}

void ChipUpdateCapability(void)
{
	return;
}

// use this for tests that must halt instead of ver that resets the DSP in RETAIL build. 
#pragma asm
 global FatalError
FatalError:
    debug
    nop
#pragma endasm



