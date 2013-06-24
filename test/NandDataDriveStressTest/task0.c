////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename:    task0.c
// Description: main() Program for Nand Data Drive Stress Test
////////////////////////////////////////////////////////////////////////////////
                       
////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "kernel.h"
#include "resource.h"
#include "messages.h"
#include "ddildl.h"
#include "NANDDdi.h"
#include "NANDDataDrive.h"
#include "NANDMedia.h"
#include "regsclkctrl.h"
#include "regsrtc.h"
#include "sysresourceapi.h"
#include "displaydrv.h"
#include "gettime.h"
#include "nandra_utils.h"
#include "regsgpio.h"

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////
extern SECTOR_BUFFER   NANDRedundantArea[]; 
extern NANDHAL_MEM NAND_PHYSICAL_MEDIA_DESCRIPTOR g_NandDescriptor[];
extern void ProgramPll(WORD wFreqMask);
extern WORD g_wNumNANDChips;
SECTOR_BUFFER SectorBuffer[SECTOR_BUFFER_ALLOC_SIZE(LARGE_SECTOR_TOTAL_SIZE)];

#define MAX_BAD_BUFFER  1000
typedef struct _sFailedSector
{
    int Sector;
    int NumFailures;   
} sFailedSector;

sFailedSector g_BadSectors[MAX_BAD_BUFFER]=
{
    0
};
int g_TotalBS=0;
int g_LastBadSector=-1;
int g_LastErrorCode=0;

#define NO_BUTTON           0
#define PLAY_BUTTON         1
#define STOP_BUTTON         2
#define PREVIOUS_BUTTON     3
#define NEXT_BUTTON         4

#define STATE_RUNNING            0
#define STATE_STOPPED            1
int g_State = STATE_RUNNING;

#define ERROR_NONE                       0
#define ERROR_MISC                      -1
#define ERROR_DRIVEWRITESECTOR          -2
#define ERROR_DRIVEREADSECTOR           -3
#define ERROR_DATA_COMPARE              -4
#define ERROR_DRIVEGETINFO              -5
#define ERROR_DRIVEMULTIWRITESETUP      -6
#define ERROR_DRIVEMULTIWRITESECTOR     -7

//g_NumIterations = 0;

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////
// LCD defines
#define LCD_X_PIXELS    98
#define LCD_Y_PIXELS    64

#define MAX_BAD_BLOCKS 200
#define MAX_NANDS 4
#define BB_DISPLAY_LINES 3

typedef enum {
    BlockTypeBad = 0,
    BlockTypeGood = 1,
    BlockTypeSystem = 2,
    BlockTypeUnknown = 3
} BLOCK_TYPE;

typedef struct {
    WORD  wChip;
    WORD  wBadBlockAddr;
    DWORD dwElapsedTime;
    INT   iBlockType;
} BAD_BLOCK_TIME, * P_BAD_BLOCK_TIME;
  
typedef struct {
    DWORD dwElapsedTime;        // SECONDS
    DWORD dwNumSectorsWritten;  // MBytes
    WORD  wNumBadBlocks;
} STRESS_TEST_STRUCT, * P_STRESS_TEST_STRUCT;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
RETCODE WriteAndVerifyDataDrive(void);
void TestCallBackFunction(LONG lValue);
WORD _reentrant rand( void );
void _reentrant srand( WORD seed );
DWORD SysGetCurrentTime(void);
RETCODE GetBlockType(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD wBlockAddr, BLOCK_TYPE * eBlockType);
void SysTimeReset(void);
void ChipUpdateCapability(void);

// LCD Prototypes
void _reentrant SwizzleText(_packed BYTE *pText);
void _reentrant LcdClear(WORD wPosX, WORD wPosY, WORD wLength, WORD wHeight);
void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString);
void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString);
void _reentrant PrintNumber(WORD wMsgXPos, WORD wMsgYPos, WORD wNum, WORD wWidth, WORD wLeadChar);
void StringPrintNumber(WORD *pUnicodeString,INT iNumber, INT iDigits, INT iFiller);
void StringPrintDwordNumber(WORD *pUnicodeString,DWORD dwNumber, INT iDigits, INT iFiller);
_reentrant void DisplayPrintLongerTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert);
_reentrant void DisplayPrintBadBlockAddr(INT x,INT y,WORD wChip,WORD wBBAddr,INT bInvert);
_reentrant void PrintStressTestTopLine(INT iHours, INT iMinutes, INT iSeconds);
_reentrant void PrintStressTest2ndLine(WORD wNumBadBlocks);
_reentrant void PrintStressTest3rdLine(DWORD dwNumSectorsWritten);
_reentrant void PrintStressTestAddrLine(INT iYPos, DWORD dwTime, WORD wBlockNum, INT iBlockType, WORD wChip);
DWORD SysGetCurrentTestTime(void);
_reentrant void PrintFailAddrLine(INT iYPos, INT wChip, INT wBlockNum, INT wCount);
_reentrant void GetFormattedTime(INT *iHours, INT *iMinutes, INT *iSeconds);
_reentrant void UpdateDisplay(INT *iHours, INT *iMinutes, INT *iSeconds);
_reentrant void UpdateBadBlocks(void);

//_reentrant void EnableButtons(void);
_reentrant int CheckButtons(void);

_packed BYTE bStartTest[] = "NDD STRESS TEST";
_packed BYTE bEndTest[]   = "NDD STRESS END TEST";
_packed BYTE bBBCount[] = "NumBadBlocks=";
_packed BYTE bFailedCount[] = "FailedBlocks=";
_packed BYTE bSectorCount[] = "SectsWrote=";
_packed BYTE bStopTest[] = "OK To Unplug Now";
_packed BYTE bTestFailed[] = "Failed:";
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
#define NUM_MULTI_WRITE_BUFFERS 10
SECTOR_BUFFER   pMultiWriteBuffer[NUM_MULTI_WRITE_BUFFERS][SECTOR_BUFFER_ALLOC_SIZE(LARGE_SECTOR_TOTAL_SIZE)+16];
BOOL bMultiWriteBufferFree[NUM_MULTI_WRITE_BUFFERS];
WORD wMultiWriteBufferIdx;
SECTOR_BUFFER   pWriteBuffer[SECTOR_BUFFER_ALLOC_SIZE(LARGE_SECTOR_TOTAL_SIZE)+16];
SECTOR_BUFFER   pReadBuffer[SECTOR_BUFFER_ALLOC_SIZE(LARGE_SECTOR_TOTAL_SIZE)+16];
WORD g_wTotalBadBlocks=0;
DWORD dwNumSectors;
WORD g_wSector;
WORD g_rtCode;
WORD g_wStartSector;
WORD g_wNumSectors;
WORD g_wRandomCount;
#define NUM_RANDOM_LOGS 5

STRESS_TEST_STRUCT    NandStressTestStruct;
BAD_BLOCK_TIME        NandBadBlockTime[MAX_BAD_BLOCKS];
WORD  g_wBadBlockIndex;
STRESS_TEST_UPDATE_BB g_NandStressTestUpdateBB;

INT  iHours = 0;
INT  iMinutes = 0;
INT  iSeconds = 0;

INT  iTempHours = 0;
INT  iTempMinutes = 0;
INT  iTempSeconds = 0;
WORD g_wDisplayIdx = 0;
DWORD g_dwBBTimeUpdate = 0;
DWORD g_dwUpdateDisplayTime = 0;

INT g_JanusDrive = -1;
//BOOL g_bWriteBmngr512Bytes=TRUE; // unused stub 


 // filename restoredrivefrombackup.h
#define FIRST_SYSTEM_DRIVE_TAG      (0x02)
#define SECOND_SYSTEM_DRIVE_TAG     (0x12)
#define MASTER_SYSTEM_DRIVE_TAG     (0x22) //????

#ifdef MMC
#define FIRST_SYSTEM_DRIVE_NUMBER   (6)
#define SECOND_SYSTEM_DRIVE_NUMBER  (7)
#define MASTER_SYSTEM_DRIVE_NUMBER  (8) //????
#else
#define FIRST_SYSTEM_DRIVE_NUMBER   (5)
#define SECOND_SYSTEM_DRIVE_NUMBER  (6)
#define MASTER_SYSTEM_DRIVE_NUMBER  (7) //????
#endif
extern WORD    wResourceDrive;      
WORD g_wPlayerResourceTag = DRIVE_TAG_RESOURCE_BIN;                     // Use this for initialization

BOOL g_bResourceGetDirty = FALSE;
BOOL g_bEraseComplete = TRUE;

_reentrant WORD ToggleSystemDrive(void)
{
    WORD OldDriveNumber;

    if (g_wPlayerResourceTag == FIRST_SYSTEM_DRIVE_TAG)
    {
        g_wPlayerResourceTag = SECOND_SYSTEM_DRIVE_TAG;

        DebugBuildAssert(wResourceDrive == FIRST_SYSTEM_DRIVE_NUMBER);

        OldDriveNumber = wResourceDrive;
        wResourceDrive = SECOND_SYSTEM_DRIVE_NUMBER;
    }
    else
    {
        g_wPlayerResourceTag = FIRST_SYSTEM_DRIVE_TAG;

        DebugBuildAssert(wResourceDrive == SECOND_SYSTEM_DRIVE_NUMBER);

        OldDriveNumber = wResourceDrive;
        wResourceDrive = FIRST_SYSTEM_DRIVE_NUMBER;
    }
    
    wResourceTag = g_wPlayerResourceTag;

    return OldDriveNumber;
}

#include "Persist_bits.h"
#define PERSIST_WAIT_TIME                            2000
RETCODE _reentrant SetStickyBit(volatile _X WORD * pSBPointer, WORD BitPosition)
{
    WORD wStickyBitMask = 1 << BitPosition;
    WORD wRetValue = !SUCCESS;
    WORD i;

    for (i=0;i<PERSIST_WAIT_TIME;i++)
    {
      if (!HW_RTC_CSR.B.NEWREGS)
      {
          // Set the new value of the Sticky Bit.
          *pSBPointer |= wStickyBitMask;
          wRetValue = SUCCESS;
      }
    }
	return wRetValue;
}
void _reentrant StickyBitForceUpdate(void)
{    
    HW_RTC_CSR.B.FORCE = 1; // Force an update
}
_reentrant WORD MarkSystemDriveDirty(void)
{
    if(g_bResourceGetDirty)
    {
        SystemHalt(); 
    } 
    g_bResourceGetDirty = TRUE; // Set the global
    SetStickyBit((volatile _X WORD *) &HW_RTC_RESOURCE_BIN_REFRESH_FLAG_REG, 
                         HW_RTC_PERSISTANT1_RESOURCE_BIN_REFRESH_FLAG_BITPOS); 

    StickyBitForceUpdate(); 
    
    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
// stub functions
void EnterNonReentrantSection(void)
{
}
void LeaveNonReentrantSection(void)
{
}
//----------------------------------

long int  FSGetDWord(void *buffer, int Offset,int MemoryType)
{
    SystemHalt();
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Task0EntryPoint
//
//   Type:          Function
//
//   Description:   This is the main task. This task is responsible for writing to the
//                  media for as long as the test is run. This task posts messages
//                  to the display task for updating the LCD.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
extern NAND_REGION_INFO g_NandRegion[];
void _reentrant Task0EntryPoint(void)
{
    INT i, iBadBlockCounter = 0;
    BLOCK_TYPE eBlockType;
    WORD wChip = 0;    // this is not being used yet.
    DWORD dwTime = 0;  // the time in ms is updated here
    INT RetCode;

    //==============================================================
    //==============================================================
    // POST SOME MESSAGES TO THE LCD TO START IT UP.
    SwizzleText(bStartTest);
    SwizzleText(bBBCount);
    SwizzleText(bFailedCount);
    SwizzleText(bSectorCount);
    //  StopButtonFound  
    SwizzleText(bStopTest);
    SwizzleText(bTestFailed);
    // Clear Display
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    SysWait(10);
    // Print Test Name
    PrintStringAddr(11, 30, bStartTest);
    SysWait(2000);
    LcdClear(0, 0, LCD_X_PIXELS, LCD_Y_PIXELS);
    SysWait(10);
    UpdateDisplay(&iHours, &iMinutes, &iSeconds);
    SysWait(10);
    //==============================================================
    //==============================================================

    //==============================================================
    //==============================================================
    // Initialize The Variables:
    NandStressTestStruct.dwElapsedTime = 0;
    NandStressTestStruct.dwNumSectorsWritten = 0;
    NandStressTestStruct.wNumBadBlocks = 0;

    for (i = 0; i < MAX_BAD_BLOCKS; i++)
    {
      NandBadBlockTime[i].wBadBlockAddr = 0;
      NandBadBlockTime[i].dwElapsedTime = 0;
    }

    g_wBadBlockIndex = 0;       // use this to index to new bad blocks

    // Init the update struct
    g_NandStressTestUpdateBB.wNumBadBlocks = 0;
    for (i = 0; i < MAX_BAD_BLOCKS; i++)
    {
      g_NandStressTestUpdateBB.wBadBlockAddr[i] = 0;
    }
    //==============================================================
    //==============================================================
  
    UpdateDisplay(&iHours, &iMinutes, &iSeconds);

    //==============================================================
    // Now fill in the bad blocks that are found at startup.
    // Loop through all the blocks on this chip
    // This takes a little while (about 20 seconds per Gigbit.)
    for (wChip=0;wChip<g_wNumNANDChips;wChip++)
    {
        for(i=0;i<g_NandDescriptor[wChip].wTotalBlocks;i++)
        {
            if(GetBlockType(&g_NandDescriptor[wChip],i,&eBlockType) != SUCCESS)
                SystemHalt();
            UpdateDisplay(&iHours, &iMinutes, &iSeconds);
            // Record it if it is a Bad Block
            if (eBlockType == BlockTypeBad)
            {
                 if (g_wBadBlockIndex < MAX_BAD_BLOCKS)
                 {
                    NandBadBlockTime[g_wBadBlockIndex].wBadBlockAddr = i;
                    NandBadBlockTime[g_wBadBlockIndex].wChip = wChip;

                    g_wBadBlockIndex ++;
                 }
                 NandStressTestStruct.wNumBadBlocks++;
                 // DO I NEED MULTINAND. I THINK SO! g_Nand[wChip].wBadBlockCount++;
            }
        }
        g_wTotalBadBlocks = NandStressTestStruct.wNumBadBlocks;
        UpdateDisplay(&iHours, &iMinutes, &iSeconds);
    }
    //==============================================================

    //==============================================================
    //==============================================================
    // Loop infinitely while writing and reading the NAND
    while (1)
    {
      // Update the Display.
      UpdateDisplay(&iHours, &iMinutes, &iSeconds);

      // Write and Read the NAND
        RetCode = WriteAndVerifyDataDrive();
        
        if (ERROR_DRIVEREADSECTOR == RetCode)
        {
            for (i=0; i<g_TotalBS; i++)
            {
                if (g_BadSectors[i].Sector == g_LastBadSector)
                {
                    g_BadSectors[i].NumFailures++;
                }
            }
            if (i > g_TotalBS)
            {
                g_BadSectors[g_TotalBS].Sector = g_LastBadSector;
                g_BadSectors[g_TotalBS].NumFailures = 1;
                g_TotalBS++;
            }
        }
        else if (ERROR_NONE != RetCode)
        {
            //
            // No reason to go on.
            //
            PrintStringAddr(50, 1, bTestFailed);
            PrintNumber(80, 1, -RetCode, 3, ' ');
            SysWait(0);
            SystemHalt();
        }
      // At this point, update the number of sectors written on the LCD
      dwTime = GetTime();
      NandStressTestStruct.dwElapsedTime = dwTime;
      UpdateDisplay(&iHours, &iMinutes, &iSeconds);
    } // while(1)
    //==============================================================
    //==============================================================
}

//
//
// Notes: 
//
RETCODE WriteAndVerifyDataDrive(void)
{
    INT RetCode;
    INT i,j, iCount=0;
    WORD wDataDriveNum;
    BOOL bDataDriveFound = FALSE;
    LOGICAL_DRIVE_TYPE DriveType;
    
    // Find the NAND Data Drive
    for(wDataDriveNum=0;wDataDriveNum<g_wNumDrives;wDataDriveNum++)
    {
        RetCode = DriveGetInfo(
            wDataDriveNum,
            DriveInfoType,
            &DriveType);
        if (RetCode != SUCCESS)
        {
            g_LastErrorCode = RetCode;
            return(ERROR_DRIVEGETINFO);
        }
        
        if(DriveType == DriveTypeData)
        {   iCount++;
            if (iCount > 1)
            {
              bDataDriveFound = TRUE;
              break;
            }
        }
        UpdateDisplay(&iHours, &iMinutes, &iSeconds);
    }
    UpdateDisplay(&iHours, &iMinutes, &iSeconds);
    if(!bDataDriveFound)
    {
        g_LastErrorCode = -1;
        return(ERROR_MISC);
    }
    
    // Get the total number of sectors for the drive
    RetCode = DriveGetInfo(
        wDataDriveNum,
        DriveInfoSizeInSectors,
        &dwNumSectors);
    if (RetCode != SUCCESS)
    {
        g_LastErrorCode = RetCode;
        return(ERROR_DRIVEGETINFO);
    }

    // Write & re-write sectors
    for(g_wSector=0;g_wSector<(dwNumSectors/64);g_wSector++) // TOVERIFY2 see 64 comment below
    {
        // Init the sector
        for(j=0;j<g_NandAuxData.SectorDataSizeInWords;j++)
        {
            pWriteBuffer[j] = (WORD)(g_wSector); 
        }

        // Write the sector
        RetCode = DriveWriteSector(
            wDataDriveNum,
            (DWORD)(g_wSector*3),
            pWriteBuffer);
        if (RetCode != SUCCESS)
        {
            g_LastErrorCode = RetCode;
            g_LastBadSector = g_wSector*3;
            return(ERROR_DRIVEWRITESECTOR);
        }
        NandStressTestStruct.dwNumSectorsWritten ++;
        UpdateDisplay(&iHours, &iMinutes, &iSeconds);
        // Init the sector
        for(j=0;j<g_NandAuxData.SectorDataSizeInWords;j++)
        {
            pWriteBuffer[j] = (WORD)(~g_wSector); 
        }

        // Write the sector
        RetCode = DriveWriteSector(
            wDataDriveNum,
            (DWORD)(g_wSector*3),
            pWriteBuffer);
        if (RetCode != SUCCESS)
        {
            g_LastErrorCode = RetCode;
            g_LastBadSector = g_wSector*3;
            return(ERROR_DRIVEWRITESECTOR);
        }
        NandStressTestStruct.dwNumSectorsWritten ++;
        UpdateDisplay(&iHours, &iMinutes, &iSeconds);
    }

    // Verify all sectors written
    // TOVERIFY2: dwNumSectors is the total number of sectors in a flash
    // and when a bad sector is found, the entire block is marked as bad. Thus
    // for most of the type of flash that we support:
    //      Type 1: Block size = 32K/512bytes per sector = 64 sectors
    //      Type 2: Block size = 128K/2Kbytes per sector = 64 sectors
    // However for type 5 and 6, this is not true. Each block in these types
    // is 256K ==> Number of sectors in a block is 256K/2K = 128 sectors.
    // In the following code, the number of iteration should be half the size!!!!!
    // IN certain cases, this might count the number of bad block twice !!!!!!!Please verify

    for(i=0;i<(dwNumSectors/64);i++)
    {
        RetCode = DriveReadSector(
            wDataDriveNum,
            (DWORD)(i*3),
            pReadBuffer);
        if (RetCode != SUCCESS)
        {
            g_LastErrorCode = RetCode;
            g_LastBadSector = i*3;
            return(ERROR_DRIVEREADSECTOR);
        }

        // Init the compare buffer
        for(j=0;j<g_NandAuxData.SectorDataSizeInWords;j++)
        {
            pWriteBuffer[j] = (WORD)(~i); 
        }

        // Verify the sector
        for(j=0;j<g_NandAuxData.SectorDataSizeInWords-1;j++)
        {
            if(pReadBuffer[j] != pWriteBuffer[j])
            {
                g_LastErrorCode = -1;
                g_LastBadSector = i*3;
                return(ERROR_DATA_COMPARE);
            }
        }
        UpdateDisplay(&iHours, &iMinutes, &iSeconds);
    }
    // Write the sectors -- 
    //   use multiwrites with random number of sectors and random start points
    srand((WORD)HW_DCLKCNTL.I);          // generate a seed for rand()
    
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
        
        for(j=0;j<NUM_MULTI_WRITE_BUFFERS;j++)
        {
            bMultiWriteBufferFree[j] = TRUE;
        }
        wMultiWriteBufferIdx = 0;
        
        // MutliWrite Setup
        RetCode = DriveMultiWriteSetup(
            wDataDriveNum,
            (DWORD)g_wStartSector,
            g_wNumSectors,
            TestCallBackFunction);
        if (RetCode != SUCCESS)
        {
            SystemHalt();
            g_LastErrorCode = RetCode;
            return(ERROR_DRIVEMULTIWRITESETUP);
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
            for(j=0;j<g_NandAuxData.SectorDataSizeInWords;j++)
            {
                pMultiWriteBuffer[wMultiWriteBufferIdx][j] =
                    (WORD)(g_wSector); 
            }

            if((RetCode = DriveMultiWriteSector(
                wDataDriveNum,
                pMultiWriteBuffer[wMultiWriteBufferIdx],
                (LONG)wMultiWriteBufferIdx)) != SUCCESS)
            {
                SystemHalt();
                g_LastErrorCode = RetCode;
                return(ERROR_DRIVEMULTIWRITESECTOR);
            }
            NandStressTestStruct.dwNumSectorsWritten ++;
            UpdateDisplay(&iHours, &iMinutes, &iSeconds);
        
            wMultiWriteBufferIdx++;
            if(wMultiWriteBufferIdx == NUM_MULTI_WRITE_BUFFERS)
            {
                wMultiWriteBufferIdx = 0;
            }
            UpdateDisplay(&iHours, &iMinutes, &iSeconds);
        }

        // Verify the sectors
        for(g_wSector=g_wStartSector;
            g_wSector<(g_wStartSector+g_wNumSectors);
            g_wSector++)
        {
            RetCode = DriveReadSector(
                wDataDriveNum,
                (DWORD)(g_wSector),
                pReadBuffer);
            if (RetCode != SUCCESS)
            {
                g_LastErrorCode = RetCode;
                g_LastBadSector = g_wSector;
                return(ERROR_DRIVEREADSECTOR);
            }
    
            // Init the compare buffer
            for(j=0;j<g_NandAuxData.SectorDataSizeInWords;j++)
            {
                pWriteBuffer[j] = (WORD)(g_wSector); 
            }

            // Verify the sector
            for(j=0;j<g_NandAuxData.SectorDataSizeInWords-1;j++)
            {
                if(pReadBuffer[j] != pWriteBuffer[j])
                {
                    g_LastErrorCode = -1;
                    g_LastBadSector = g_wSector;
                    return(ERROR_DATA_COMPARE);
                }
            }
            UpdateDisplay(&iHours, &iMinutes, &iSeconds);
        }
    }
    return(ERROR_NONE);
}

void TestCallBackFunction(LONG lValue)
{
    bMultiWriteBufferFree[lValue] = TRUE;
    return;
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


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetBlockType
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
RETCODE GetBlockType(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
    WORD wBlockAddr,
    BLOCK_TYPE * eBlockType)
{
    INT i;
    P_NANDHAL_API_DESCRIPTOR pNandHalApi; 
    WORD wBaseSectorAddr;

    pNandHalApi = pNANDDescriptor->pNANDParams->pNandHalApi;
    wBaseSectorAddr = wBlockAddr * 
        pNANDDescriptor->pNANDParams->wNumberSectorsPerBlock;
    
    // Assume it's a good block
    *eBlockType = BlockTypeGood;
    
    for(i=0;i<pNANDDescriptor->pNANDParams->wNumberSectorsPerBlock;i++)
    {
        // read page
        if(pNandHalApi->pReadSectorRedundant(
            pNANDDescriptor,
            wBaseSectorAddr + i,
            SectorBuffer, FALSE) != SUCCESS)
        {
            SystemHalt();
        }

        NANDRA_ToNANDForm(SectorBuffer, RA_ONLY);
        
        if((SectorBuffer[1] & 0xFF0000) == 0xFF0000)
        {
            continue;
        }
        
        // It may be a bad block or a system block
        if((SectorBuffer[2] & 0x00FFFF) == 0x005453)
        {
            *eBlockType = BlockTypeSystem;
            return SUCCESS;
        }
        *eBlockType = BlockTypeBad;
        return SUCCESS;
    }
    return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           SwizzleText
//
//   Type:           Function
//
//   Description:    Swap byte 2 and byte 0 of string words pointed by arg pText  
//                   
//
//   Inputs:         pText = Pointer to the string to swizzle
//
//   Outputs:        none
//                   
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////

void _reentrant SwizzleText(_packed BYTE *pText)
{
    int bDone=FALSE;
    unsigned int *pTmp=(unsigned int*)pText;
    unsigned int tmp;
    while(!bDone)
    {
        tmp = *pTmp;
        tmp = ((tmp & 0x0000ff)<<16) | ((tmp & 0x00ff00)) | ((tmp & 0xff0000)>>16);
        *pTmp++=tmp;
        if(!(tmp&0x0000ff) || !(tmp&0x00ff00)  || !(tmp&0xff0000))
            bDone = TRUE;
    }
}

void _reentrant LcdClear(WORD wPosX, WORD wPosY, WORD wLength, WORD wHeight)
{
    SysPostMessage(6,LCD_CLEAR_RANGE,wPosX,wPosY,wLength,wHeight);
}

void _reentrant PrintStringAddr(WORD wPosX, WORD wPosY, _packed BYTE *pMsgString)
{
    SysPostMessage(5,LCD_PRINT_STRING_ADDR,wPosX,wPosY,pMsgString);
}

void _reentrant PrintNumber(WORD wMsgXPos, WORD wMsgYPos, WORD wNum, WORD wWidth, WORD wLeadChar)
{
    SysPostMessage(7,LCD_PRINT_NUMBER,wMsgXPos,wMsgYPos,wNum,wWidth,wLeadChar);
}

_reentrant void DisplayPrintLongerTime(INT x,INT y,INT iHours     ,INT iMinutes   ,INT iSeconds   ,INT bInvert)
{
    WORD pUnicodeString[3+1+2+1+2+1+2];//3 for hours, 1 for colon, 2 for minutes, 1 for colon, 2 for seconds, 1 for terminator, and 2 for padding
    StringPrintNumber(pUnicodeString+0, iHours  , 3,'0');
    pUnicodeString[3]=':';
    StringPrintNumber(pUnicodeString+4, iMinutes, 2,'0');
    pUnicodeString[6]=':';
    StringPrintNumber(pUnicodeString+7, iSeconds, 2,'0');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);
}

_reentrant void DisplayPrintBadBlockAddr(INT x,INT y,WORD wChip,WORD wBBAddr,INT bInvert)
{
  // Prints the Bad Block Address in the format of:    ChipNumber:BadBlockPhysAddr
  // For example, a bad block at address $2ee on chip 0 prints as:  0:02EE
    WORD pUnicodeString[2+1+4];//1 for the chip number, 1 for colon, 4 for the address
    StringPrintNumber(pUnicodeString+0, wChip, 2,' ');
    pUnicodeString[2]=':';
    StringPrintNumber(pUnicodeString+3, wBBAddr, 4,'0');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);
}

_reentrant void PrintStressTestTopLine(INT iHours, INT iMinutes, INT iSeconds)
{
  INT iXPos = 1;
  INT iYPos = 1;

    if (g_State == STATE_RUNNING)
    {
  DisplayPrintLongerTime(iXPos, iYPos, iHours, iMinutes, iSeconds,0);
}
    else
    {
        // Print Test Name
        PrintStringAddr(0, 0, bStopTest);
    }
}

_reentrant void PrintStressTest2ndLine(WORD wNumBadBlocks)
{
    if (NandStressTestStruct.dwElapsedTime & 0x40)
    {
        PrintStringAddr(1, 11, bBBCount);
  PrintNumber(70, 11, wNumBadBlocks, 4, ' ');
}
    else
    {
        PrintStringAddr(1, 11, bFailedCount);
        PrintNumber(70, 11, g_TotalBS, 4, ' ');
    }
}

void StringPrintDwordNumber(WORD *pUnicodeString,DWORD dwNumber, INT iDigits, INT iFiller)
{
    int i;
    for(i=0;i<iDigits;i++)
    {
        if(dwNumber||!i)
        {
            pUnicodeString[iDigits-i-1] = 0x30+ dwNumber%10;
            dwNumber /= 10;
        }
        else
        {
            pUnicodeString[iDigits-i-1] = iFiller;
        }

    }
    pUnicodeString[iDigits]=0;

}

_reentrant void DisplayPrintDword(INT x,INT y,DWORD dwNum,WORD wWidth, INT bInvert)
{
    WORD pUnicodeString[16];
    StringPrintDwordNumber(pUnicodeString+0, dwNum, wWidth,' ');
    DisplayPrintStringAddressUnicode(x,y,pUnicodeString,bInvert);
}

_reentrant void PrintStressTest3rdLine(DWORD dwNumSectorsWritten)
{
  INT iXPos = 1;
  INT iYPos = 22;

  DisplayPrintDword(1, 22, dwNumSectorsWritten, 16, 0);
}



_reentrant void GetFormattedTime(INT *iHours, INT *iMinutes, INT *iSeconds)
{
   DWORD dwTime, dwSeconds;
   dwTime = GetTime();
    
   dwSeconds = dwTime/1000;
   *iHours = dwSeconds/(3600);
   dwSeconds = dwSeconds -(DWORD)(*iHours*3600);
   *iMinutes = dwSeconds/60;
   dwSeconds = dwSeconds - (DWORD)(*iMinutes*60);
   *iSeconds = dwSeconds;
}

_reentrant void HandleStopped(void)
{
    int wIndex=0;
    int wDispIndex;
    int DirtyScreen=1;
    int Button = NO_BUTTON;
    int i;
    
    LcdClear(0, 0, LCD_X_PIXELS, 20);
    PrintStringAddr(0, 0, bStopTest);
    PrintStringAddr(1, 11, bFailedCount);
    PrintNumber(70, 11, g_TotalBS, 4, ' ');
    LcdClear(0, 33, LCD_X_PIXELS, LCD_X_PIXELS);
    SysWait(0);

    do
    {
        //
        // If we get a previous button then scroll up the screen.
        //
        if (Button == PREVIOUS_BUTTON)
        {
            wIndex-=BB_DISPLAY_LINES;
            if (wIndex < 0)
            {
                wIndex = 0;
            }
            DirtyScreen = 1;
        }
        //
        // If we get a next button then scroll down the screen.
        //
        else if (Button == NEXT_BUTTON)
        {
            if (g_TotalBS > BB_DISPLAY_LINES)
            {
                wIndex+=BB_DISPLAY_LINES;
                if (wIndex > g_TotalBS - BB_DISPLAY_LINES)
                {
                    wIndex = (g_TotalBS-BB_DISPLAY_LINES);
                }
            }
            DirtyScreen = 1;
        }
        //
        // Don't bother printing if nothing changed or there is nothing
        // to print.
        //
        if (DirtyScreen && (g_TotalBS > 0))
        {
            for (i=0; i < BB_DISPLAY_LINES; i++)
            {
                wDispIndex = i + wIndex;
                
                PrintFailAddrLine(i*11+33,
                    0,
                    g_BadSectors[wDispIndex].Sector,
                    g_BadSectors[wDispIndex].NumFailures);
            }
            DirtyScreen = 0;
            DisplayUpdateDisplay();
        }
        Button = CheckButtons();
    } while(g_State == STATE_STOPPED);
    LcdClear(0, 33, LCD_X_PIXELS, LCD_X_PIXELS);
    SysWait(0);
}
_reentrant void UpdateDisplay(INT *iHours, INT *iMinutes, INT *iSeconds)
{
      DWORD dwTime;
    int Button;
    int i;
      dwTime = GetTime();
      NandStressTestStruct.dwElapsedTime = dwTime;

      if ((dwTime-1000) > g_dwUpdateDisplayTime)
      {
        g_dwUpdateDisplayTime = dwTime;
        LcdClear(0, 0, LCD_X_PIXELS, 20);
        GetFormattedTime(iHours, iMinutes, iSeconds);
        SysWait(0);
        PrintStressTestTopLine(*iHours, *iMinutes, *iSeconds);
        SysWait(0);
        PrintStressTest2ndLine(g_wTotalBadBlocks);
        SysWait(0);
        PrintStressTest3rdLine(NandStressTestStruct.dwNumSectorsWritten);
        SysWait(0);
        UpdateBadBlocks();
        SysWait(0);
    }
    
    Button = CheckButtons();
    
    //
    // Handle the stopped state. 
    //
    if (g_State == STATE_STOPPED)
    {
        HandleStopped();
    }
}

///////////////////////////////////////////////////////////////////////////////
//!
//! PrintFailAddrLine
//!
//! Prints out the failing information for a given block.
//!
//! \param iYPos 
//!     The offset in Y to print this string.
//! \param 
//!     wChip The chip that failed.
//! \param wBlockNum 
//!     The actual block number that failed.
//! \param wCount
//!     The number of times that this block has failed.
//!
//! \retval
//!
//! Notes: none
//!
///////////////////////////////////////////////////////////////////////////////
_reentrant void PrintFailAddrLine(INT iYPos, INT wChip, INT wBlockNum, INT wCount)
{
    WORD pUnicodeString[15];

    StringPrintNumber(pUnicodeString+0, wChip, 2,' ');
    pUnicodeString[2]=':';
    StringPrintNumber(pUnicodeString+3, wBlockNum, 4,'0');
    pUnicodeString[7]=':';
    StringPrintNumber(pUnicodeString+8, wCount, 4,'0');
    pUnicodeString[12]=0;
    DisplayPrintStringAddressUnicode(1, iYPos,pUnicodeString,0);
}

_reentrant void PrintStressTestAddrLine(INT iYPos, DWORD dwTime, WORD wBlockNum, INT iBlockType, WORD wChip)
{
  INT iXPos = 1;
  DWORD dwSeconds;
  // Print the block address
  DisplayPrintBadBlockAddr(iXPos, iYPos, wChip,wBlockNum,0);

  //Print the Time the Block Went Bad
  dwSeconds = dwTime/1000;
  iTempHours = dwSeconds/(3600);
  dwSeconds = dwSeconds -(DWORD)(iTempHours*3600);
  iTempMinutes = dwSeconds/60;
  dwSeconds = dwSeconds - (DWORD)(iTempMinutes*60);
  iTempSeconds = dwSeconds;
  SysWait(0);
  DisplayPrintLongerTime(iXPos+40, iYPos, iTempHours, iTempMinutes, iTempSeconds,0);
  // Add support here for BLOCK TYPE!!! (Print H,U,R,S)

}

_reentrant void UpdateBadBlocks(void)
{
    INT i, iBBIdx;
    WORD  wNewBadBlockCount;
    WORD  wNumBB = 0;
    WORD  wNumDisplays, wIndex;
    DWORD dwTime = GetTime();

      // Fill in any new bad blocks that were discovered
      wNewBadBlockCount = g_NandStressTestUpdateBB.wNumBadBlocks;
      if (wNewBadBlockCount)
      {
        for (i = 0; i < wNewBadBlockCount; i++)
        {
          NandBadBlockTime[g_wBadBlockIndex].wChip = g_NandStressTestUpdateBB.wChip[i];
          NandBadBlockTime[g_wBadBlockIndex].wBadBlockAddr = g_NandStressTestUpdateBB.wBadBlockAddr[i];
          NandBadBlockTime[g_wBadBlockIndex].dwElapsedTime = dwTime;
          NandStressTestStruct.wNumBadBlocks++;
          g_wTotalBadBlocks ++;
          g_wBadBlockIndex ++;
        }
      }
      wNumBB = g_wTotalBadBlocks;
      g_NandStressTestUpdateBB.wNumBadBlocks = 0;   // reset the value for next time

      // IF there are bad blocks, display them.
      if (wNumBB)
      {
        if(wNumBB<=BB_DISPLAY_LINES)
        {
          for (iBBIdx=0; iBBIdx<wNumBB; iBBIdx++)
          {
            PrintStressTestAddrLine(iBBIdx*11+33, NandBadBlockTime[iBBIdx].dwElapsedTime, NandBadBlockTime[iBBIdx].wBadBlockAddr, NandBadBlockTime[iBBIdx].iBlockType, NandBadBlockTime[iBBIdx].wChip);
            SysWait(0);
          }
        }
        else
        {
          wNumDisplays = wNumBB/BB_DISPLAY_LINES;  // this is the number of displays needed to show all bad blocks
          if (wNumBB%BB_DISPLAY_LINES)
            wNumDisplays++;                        // add one if there is a remainder
          for (iBBIdx=0; iBBIdx < BB_DISPLAY_LINES; iBBIdx++)
          {
            wIndex = iBBIdx+(BB_DISPLAY_LINES*g_wDisplayIdx);
            PrintStressTestAddrLine(iBBIdx*11+33, NandBadBlockTime[wIndex].dwElapsedTime, NandBadBlockTime[wIndex].wBadBlockAddr, NandBadBlockTime[wIndex].iBlockType, NandBadBlockTime[wIndex].wChip);
            SysWait(1);
          }

          if ((dwTime-g_dwBBTimeUpdate) > 3500)
          {// update display after 1 second
              g_wDisplayIdx ++;
              if (g_wDisplayIdx >= wNumDisplays)
                  g_wDisplayIdx = 0;
              g_dwBBTimeUpdate = dwTime;
          }
        } // else
      } // if (NandStressTestStruct.wNumBadBlocks)
}

#define GPIO_PLAY_BUTTON        0x400
#define GPIO_STOP_BUTTON        0x2
#define GPIO_PREVIOUS_BUTTON    0x080000
#define GPIO_NEXT_BUTTON        0x080000
///////////////////////////////////////////////////////////////////////////////
//!
//!   ReadButtons
//!
//!   This function reads the actual GPIOs to determine if a button is currently
//!   pressed.
//!
//!   \param
//!
//!   \retval
//!
//!   Notes:         none
//!
///////////////////////////////////////////////////////////////////////////////
int ReadButtons(void)
{
    unsigned int GPIOPins[3];
    int Button=NO_BUTTON;
    int i;

    //
    // Just read the play button.
    //
    if (HW_GP1DIR.U & GPIO_PLAY_BUTTON)
    {
        Button=PLAY_BUTTON;
    }
    else
    {
        //
        // Drive the column high.
        //
        HW_GP2DOER.B.B0 = 1;
    
        //
        // Wait for settling on the GPIOs
        //
        for (i=0;i<10;i++)
        {
            GPIOPins[2] = HW_GP2DIR.U;
        }

        // 
        // Read GPIO 1 values.
        //
        GPIOPins[1] = HW_GP1DIR.U;

        //
        // Drive the column low.
        //
        HW_GP2DOER.B.B0 = 0;
    
        //
        // Check the rows to see if STOP or PREVIOUS have been pressed.
        //
        if (GPIOPins[2] & GPIO_STOP_BUTTON)
        {
            Button = STOP_BUTTON;
        }
        else if (GPIOPins[1] & GPIO_PREVIOUS_BUTTON)
        {
            Button = PREVIOUS_BUTTON;
        }
        else
        {
            //
            // Drive the column high.
            //
            HW_GP1DOER.B.B23 = 1;

            for (i=0;i<10;i++)
            {
                GPIOPins[1] = HW_GP1DIR.U;
            }

            //
            // Drive the column low.
            //
            HW_GP1DOER.B.B23 = 0;
            
            //
            // See if the row for NEXT button is high.
            //
            if (GPIOPins[1] & GPIO_NEXT_BUTTON)
            {
                Button = NEXT_BUTTON;
            }
        }
    }
    return(Button);
}

///////////////////////////////////////////////////////////////////////////////
//!
//!   CheckButtons
//!
//!   This function returns if any buttons are pressed.
//!
//!   \param
//!
//!   \retval Returns a button value or NO_BUTTON.
//!
//!   Notes:         none
//!
///////////////////////////////////////////////////////////////////////////////
_reentrant int CheckButtons(void)
{
    int Button;
    
    switch (Button = ReadButtons())
    {
        case PLAY_BUTTON:
        {
            g_State = STATE_RUNNING;
            break;
        }
        case STOP_BUTTON:
        {
            g_State = STATE_STOPPED;
            break;
        }
        case PREVIOUS_BUTTON:
        {
            break;
        }
        case NEXT_BUTTON:
        {
            break;
        }
    }
    if (Button != NO_BUTTON)
    {
        while (NO_BUTTON != ReadButtons());
    }
    
    return(Button);
}
