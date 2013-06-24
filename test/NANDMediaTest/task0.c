////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename:    task0.c
// Description: main() Program for Nand Media DDI Test
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "kernel.h"
#include "ddildl.h"
#include "NANDDdi.h"
#include "NANDDataDrive.h"
#include "NANDMedia.h"
#include "regsclkctrl.h"
#include "regsrtc.h"
#if defined(DEVICE_3500)
#include "regsgpflash.h"
#include "regsgpio.h"
#endif
//#define INSTRUMENT_ENABLE 1
#include "instrument.h"
#include "NANDRA_utils.h"

#define EventSetBlockWrite(); EventSet07();
#define EventClearBlockWrite(); EventClear07();
//#define EventSetBlockWrite();
//#define EventClearBlockWrite();

#define EventSetDriveMultiWriteSector(); EventSet06();
#define EventClearDriveMultiWriteSector(); EventClear06();
//#define EventSetDriveMultiWriteSector();
//#define EventClearDriveMultiWriteSector();

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////
extern NAND_MEDIA_INFO MEDIA_DDI_MEM NandMediaInfo;
extern SECTOR_BUFFER * NANDMediaSectorBuffer;
extern SECTOR_BUFFER   NANDRedundantArea[]; 
////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

#define DATA_DRIVE_BASE_BAD_BLOCKS  11

#define MAX_BAD_BLOCKS 10
#define MAX_NANDS 4

//#define USE_ECC         1

// There are actually 170 2/3 data words per sector, but since we write that other byte
// as part of the Redundant Area, we can't verify all 170 2/3, or we'll get a verify error.
#define NUM_DATA_WORDS_PER_SECTOR (g_NandAuxData.SectorDataSizeInWords-1)

typedef struct {
      int    iNumBadBlocks;         // number of bad blocks to set
      WORD   wBadBlockAddr[MAX_BAD_BLOCKS];         // block address to mark bad
} BAD_BLOCK_MAKER_TABLE;
      

typedef struct {
  int iNumNands;
  BAD_BLOCK_MAKER_TABLE * NandBadBlockMakerTable[MAX_NANDS];
} NAND_BAD_BLOCK_LIST;

typedef struct {
    WORD wStartSector;
    WORD wNumSectors;
} RANDOM_LOG, * P_RANDOM_LOG;
  
////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

RETCODE Test1(void);
RETCODE TestSystemDriveGetInfo(void);
RETCODE TestSystemDriveReadWrite(void);
RETCODE TestDataDrive(void);
void TestCallBackFunction(LONG lValue);
RETCODE _reentrant NandMediaEraseALL(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor, WORD wMagicNumber);
RETCODE _reentrant NandMediaSetBadBlocks(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor, NAND_BAD_BLOCK_LIST * pNandBadBlockList);
WORD _reentrant rand( void );
void _reentrant srand( WORD seed );
DWORD GetDclkCount(void);
void SetDclkCount(DWORD dwCount);
DWORD SysGetCurrentTime(void);
RETCODE Hackage(void);
RETCODE DoDaMultiWrite(
    WORD wDriveNum,
    DWORD dwStartSector,
    WORD wNumSectors);
RETCODE _reentrant NANDDataDriveReadSectorEccTest(P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor, DWORD dwSectorNumber,
                                                  P_SECTOR_BUFFER pSectorData, INT iNandNumber);
RETCODE NANDDataDriveLookupUBlock(P_NAND_REGION_INFO pRegion, 
                                  WORD wHBlockAddr, 
                                  WORD *pwUBlockAddr);
RETCODE NANDDataDriveLookupUSector(P_NAND_REGION_INFO pRegion, WORD wUBlkAddr, 
                                   WORD wHSectorIdx, WORD *pwUSectorIdx);
RETCODE MyTest(void);
RETCODE MyTest2(void);
RETCODE MyTest3(void);
#if defined(DEVICE_3500)
RETCODE PllInit(void);
#endif
void FillSectorBuffer(P_SECTOR_BUFFER pBuffer, WORD wValue);
void ChipUpdateCapability(void);

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
extern WORD _X g_wHiddenDriveIdx;
BOOL g_bLimitedVDDD = FALSE;

// TOVERIFY LB proj had these but 3.0 did not.  These 3 may not be needed here.
WORD g_ResourceRead = 0;
short g_ResourceByteIndex = 0;
short g_ResourceByteLength = 0;


#define NUM_MULTI_WRITE_BUFFERS 10
SECTOR_BUFFER   pMultiWriteBuffer[NUM_MULTI_WRITE_BUFFERS][710];
BOOL bMultiWriteBufferFree[NUM_MULTI_WRITE_BUFFERS];
WORD wMultiWriteBufferIdx;
SECTOR_BUFFER   pWriteBuffer[710];
SECTOR_BUFFER   pReadBuffer[710];
SECTOR_BUFFER   pEccTestBuffer[710];
SECTOR_BUFFER   *pRedundBufTemp=NANDRedundantArea;

DWORD dwSector, dwNumSectors;
WORD g_wSector;
WORD g_rtCode;
WORD g_wStartSector;
WORD g_wNumSectors;
WORD g_wRandomCount;
#define NUM_RANDOM_LOGS 5
RANDOM_LOG RandomLog[NUM_RANDOM_LOGS];
DWORD dwMultiWriteTimeStart;
DWORD dwMultiWriteTimeEnd;
DWORD dwMultiWriteTimeTotal;

// EXAMPLE FOR SETTING BAD BLOCKS
//BAD_BLOCK_MAKER_TABLE Nand0BadBlocks = 
//{ 5, {3,4,16,200,900,0,0,0,0,0}};

// DEFAULT CASE -- NO BAD BLOCKS
BAD_BLOCK_MAKER_TABLE Nand0BadBlocks = 
{ 0, {0,0,0,0,0,0,0,0,0,0}};
BAD_BLOCK_MAKER_TABLE Nand1BadBlocks = 
{ 0, {0,0,0,0,0,0,0,0,0,0}};
BAD_BLOCK_MAKER_TABLE Nand2BadBlocks = 
{ 0, {0,0,0,0,0,0,0,0,0,0}};
BAD_BLOCK_MAKER_TABLE Nand3BadBlocks = 
{ 0, {0,0,0,0,0,0,0,0,0,0}};

NAND_BAD_BLOCK_LIST NandBadBlockList =
{ 1, {&Nand0BadBlocks, &Nand1BadBlocks, &Nand2BadBlocks, &Nand3BadBlocks}};

//#define DEBUG_128_SECTOR_MWRITES 1
#ifdef DEBUG_128_SECTOR_MWRITES
    #define NUM_128_SECTOR_MWRITES 100
    WORD g_wMwriteTime[NUM_128_SECTOR_MWRITES];
    WORD g_wMwriteTimeIdx = 0;
    DWORD g_dwMwriteTimeStart;
#endif

struct 

{
  WORD wCurrentSector;
  WORD wCurrentBlock;
  WORD wOffsetInSectorThatFails;
  WORD wStage;
  WORD wWriteFails;
  WORD wMultiWriteFails;
  WORD wEraseFails;
  WORD wReadFails;
  WORD wVerifyFails;
} g_ViewVars = { 0,0,0,0,0,0,0,0,0 };



/////////////////////////////////////////////////////////////////////////////////
//  
// You can set g_bAllocate to FALSE in the debugger, to prevent calling MediaErase() and MediaAllocate().
// That will allow you to analyze or test an existing configuration.
//
/////////////////////////////////////////////////////////////////////////////////
BOOL g_bAllocate = TRUE;

/////////////////////////////////////////////////////////////////////////////////
//  
// You can set g_bEccTest to FALSE in the debugger, to prevent calling ECC Tests.
//
/////////////////////////////////////////////////////////////////////////////////
BOOL g_bEccTest = FALSE;

/////////////////////////////////////////////////////////////////////////////////
//
// You can set g_bAnalyze to TRUE in the debugger, to cause the program to stop at various points,
// allowing you to analyze various data structures.  
//
// It will stop after NANDMediaDiscoverAllocation(), allowing you to examine the Regions table, 
// in g_aInternalNANDRegionInfoStructs[], which includes the start address and size of each Region
// and the starts and sizes of the UBlock and RBlock areas of the Data Drive Region(s).
//
// It will stop after all drives have been initialized (DriveInit()), so you can examine the BBTbls
// for all drives, plus the RBlock mapping for the Data Drive Region(s).
//
// It will stop after the System Drives Read/Write test.
// It will stop after the Data Drive Read/Write test.
//
/////////////////////////////////////////////////////////////////////////////////
BOOL g_bAnalyze = FALSE;


WORD g_wNumDrivesInit;

INT g_JanusDrive = -1;

    #ifdef SCSI_DEBUG_WRITE_COMMAND_HISTORY
#define SIZE_OF_WRITE_HISTORY_BUFFER 600
    WORD wWriteHistorySize =  SIZE_OF_WRITE_HISTORY_BUFFER;
    WORD _Y WriteHistory[SIZE_OF_WRITE_HISTORY_BUFFER]; 
    WORD wWriteHistoryPointer = 0;
    WORD wWriteHistoryEnd = SIZE_OF_WRITE_HISTORY_BUFFER;
    #endif


//BOOL g_bWriteBmngr512Bytes=TRUE; // unused stub


////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

// use this for tests that must halt instead of ver that resets the DSP in RETAIL build. 
#pragma asm
     global FatalError
    FatalError:
        debug
        nop
#pragma endasm


void ChipUpdateCapability(void)
{
	return;
}

long int  FSGetDWord(void *buffer, int Offset,int MemoryType)
{
    return -1;
}


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

  EventSetup00();    
  EventSetup01();    
  EventSetup02();    
  EventSetup03();    
  EventSetup04();    
  EventSetup05();    
  EventSetup06();    
  EventSetup07();    

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
extern NAND_REGION_INFO g_NandRegion[];
RETCODE Test1(void)
{
  WORD wMagicNumber = 0;                // We don't have this implemented yet.
  INT i, iBadBlockCounter = 0;
  SYSTEM_VERSION SystemVersion;
  SYSTEM_VERSION SystemVersionCompare;
  BAD_BLOCK_MAKER_TABLE * pBadBlockTable;

  g_wNumDrivesInit = g_wNumDrives;
    
#ifdef SCSI_DEBUG_WRITE_COMMAND_HISTORY
  // Clear out the read history buffer
  for(i=0;i<SIZE_OF_WRITE_HISTORY_BUFFER;i++)
    {
      WriteHistory[i] = 0xFFFFFF;
    }
#endif

  // Call NANDMediaInit
  // This function will fill in parameters for:
  //     LOGICAL_MEDIA_DESCRIPTOR  (g_MediaDescriptor : declared in ddiproject.c)
  //     NAND_MEDIA_INFO   (NandMediaInfo : declared in NANDMediaInit.c)
  //     NAND_PHYSICAL_MEDIA_DESCRIPTOR[] via NandHalInit (g_NandDescriptor[4] : declared in nandhalproject.c)
  //     NAND_PARAMETERS_DESCRIPTOR via NandHalInit (g_NandParams : declared in nandhalproject.c)
  if (NANDMediaInit(g_MediaDescriptor))
    SystemHalt();        // If MediaInit fails, HALT.


  if (!g_bAnalyze) { // Don't do this destructive case if you're trying to analyze an existing configuration   

    // Call NandMediaEraseALL
    // DANGER!!! This is a test program only. It will erase all blocks including bad blocks. It does not check them!
    NandBadBlockList.iNumNands = (int)NandMediaInfo.iNumNANDChips;
    for (i=0; i<NandBadBlockList.iNumNands; i++)
      { // Count up total bad blocks to mark
        pBadBlockTable = NandBadBlockList.NandBadBlockMakerTable[i];
        iBadBlockCounter = iBadBlockCounter + pBadBlockTable->iNumBadBlocks;
      }
    if (iBadBlockCounter)
      {
        if (NandMediaEraseALL(g_MediaDescriptor, wMagicNumber))
          SystemHalt();        // If MediaEraseALL fails, HALT.
        // Setup Bad Blocks.
        // This is a test program to specify blocks to mark bad.
        if (NandMediaSetBadBlocks(g_MediaDescriptor, &NandBadBlockList))
          SystemHalt();
      }
  }

  if(g_bAllocate)
    {
      if (NANDMediaErase(g_MediaDescriptor, wMagicNumber))
        SystemHalt();        // If MediaErase fails, HALT.

      if (NANDMediaAllocate(g_MediaDescriptor, g_MediaAllocationTable))
        SystemHalt();
    }    


  if (NANDMediaDiscoverAllocation(g_MediaDescriptor))
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
  for(i = 0 ; i < g_wNumDrives ; i++)
    {
      // skip the hidden data drive for now... pending unlock instructions
      if ( i == g_wHiddenDriveIdx )
        continue;
        
      if(DriveInit(i) != SUCCESS)
        SystemHalt();

      if(g_bAllocate)
        {        
          if(DriveErase(i, 0x00) != SUCCESS)
            {
              SystemHalt();
            }
        
          SystemVersion.Version = 0x000100020000 + i;
        
          DriveSetInfo(i, DriveInfoComponentVersion,
                       (void *)&(SystemVersion.Version));

          DriveGetInfo(i,DriveInfoComponentVersion,
                       (void *)&(SystemVersionCompare.Version));         

          if(SystemVersion.Version != SystemVersionCompare.Version)
            {
              SystemHalt();
            }             

          DriveSetInfo(i, DriveInfoProjectVersion,
                       (void *)&(SystemVersion.Version));
        
          DriveGetInfo(i,DriveInfoComponentVersion,
                       (void *)&(SystemVersionCompare.Version));         

          if(SystemVersion.Version != SystemVersionCompare.Version)
            {
              SystemHalt();
            }             
        }
    }

  //    Hackage();
  //    MyTest();
  //    MyTest2();
  //    MyTest3();
  //    SystemHalt();

  if(TestSystemDriveGetInfo() != SUCCESS)
    {
      SystemHalt();
    }            

  // ECC TEST
  // Now, Everything is initialized. Begin ECC Tests.
  if (g_bEccTest)
    {
      if (NANDDataDriveReadSectorEccTest(&g_DriveDescriptor[0],0,pEccTestBuffer,0))
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
  if(TestSystemDriveReadWrite() != SUCCESS)
    {
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


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MyTest
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
#define NUM_SECTORS 256
RETCODE MyTest(void)
{
  INT i,j;
  WORD wDataDriveNum;
  BOOL bDataDriveFound = FALSE;
  LOGICAL_DRIVE_TYPE DriveType;
    
#if defined(DEVICE_3500)
  if(PllInit() != SUCCESS)
    {
      SystemHalt();
    }
    
#endif

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
    
  for(g_wSector=0;g_wSector<dwNumSectors;g_wSector++)
    {
      if(DriveReadSector(
                         wDataDriveNum,
                         (DWORD)(g_wSector),
                         pReadBuffer) != SUCCESS)
        {
          SystemHalt();
        }
        
      if(((pReadBuffer[NUM_DATA_WORDS_PER_SECTOR+1] & 0xFFFF00) >> 8) != (g_wSector & 0x0000FF))
        {
          SystemHalt();
          SystemHalt();
        }
        
      if(((pReadBuffer[NUM_DATA_WORDS_PER_SECTOR+4] & 0xFFFF00) >> 8) != ((g_wSector >> 8) + 3))
        {
          SystemHalt();
          SystemHalt();
        }
        
    }
    
  SystemHalt();

  for(i=0;i<NUM_MULTI_WRITE_BUFFERS;i++)
    {
      bMultiWriteBufferFree[i] = TRUE;
    }
  wMultiWriteBufferIdx = 0;

  SetDclkCount(0);
  dwMultiWriteTimeStart = GetDclkCount();

  // Multi-write setup
  if(DriveMultiWriteSetup(
                          wDataDriveNum,
                          0,
                          NUM_SECTORS,
                          TestCallBackFunction) != SUCCESS)
    {
      SystemHalt();
      return(!SUCCESS);
    }

  // Write the sectors
  for(g_wSector=0;g_wSector<NUM_SECTORS;g_wSector++)
    {
      if(bMultiWriteBufferFree[wMultiWriteBufferIdx] != TRUE)
        {
          SystemHalt();
        }
        
      bMultiWriteBufferFree[wMultiWriteBufferIdx] = FALSE;
        
      // Init the compare buffer
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
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

  dwMultiWriteTimeEnd = GetDclkCount();
  dwMultiWriteTimeTotal = dwMultiWriteTimeEnd - dwMultiWriteTimeStart;

  // Verify the sectors
  for(g_wSector=0;g_wSector<NUM_SECTORS;g_wSector++)
    {
      if(DriveReadSector(
                         wDataDriveNum,
                         (DWORD)(g_wSector),
                         pReadBuffer) != SUCCESS)
        {
          SystemHalt();
          return(!SUCCESS);
        }

      // Verify the sector
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR;j++)
        {
          if(pReadBuffer[j] != g_wSector)
            {
              SystemHalt();
              return(!SUCCESS);
            }
        }
    }
    
  return SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MyTest2
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
#define MAX_START_SECTOR 512
#define MAX_NUM_SECTORS (3*256)
#define NUM_SECTORS 256
WORD g_dwStartSectorLoop;
WORD g_dwNumSectorsLoop;
WORD g_dwStartSector;
WORD g_wNumSectors;
WORD g_wTestCount;
RETCODE MyTest2(void)
{
  INT i,j,k;
  WORD wDataDriveNum;
  BOOL bDataDriveFound = FALSE;
  LOGICAL_DRIVE_TYPE DriveType;
    
#if defined(DEVICE_3500)
  if(PllInit() != SUCCESS)
    {
      SystemHalt();
    }
    
#endif

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

  g_wTestCount = 0;
    
  for(g_dwStartSectorLoop=0;
      g_dwStartSectorLoop<MAX_START_SECTOR;
      g_dwStartSectorLoop++)
    {
      for(g_dwNumSectorsLoop=2;
          g_dwNumSectorsLoop<MAX_NUM_SECTORS;
          g_dwNumSectorsLoop++)
        {
          g_wTestCount++;
            
          /*
            g_wNumDrives = g_wNumDrivesInit;
            
            if (NANDMediaErase(g_MediaDescriptor, 0x0))
            SystemHalt();        // If MediaErase fails, HALT.

            if (NANDMediaAllocate(g_MediaDescriptor, g_MediaAllocationTable))
            SystemHalt();

            if (NANDMediaDiscoverAllocation(g_MediaDescriptor))
            SystemHalt();

            for(j = 0 ; j < g_wNumDrives ; j++)
            {
            if(DriveInit(j) != SUCCESS)
            {
            SystemHalt();
            }
            }
          */            
          for(i=0;i<NUM_MULTI_WRITE_BUFFERS;i++)
            {
              bMultiWriteBufferFree[i] = TRUE;
            }
          wMultiWriteBufferIdx = 0;

          SetDclkCount(0);
          dwMultiWriteTimeStart = GetDclkCount();

          // Multi-write setup
          if(DriveMultiWriteSetup(
                                  wDataDriveNum,
                                  g_dwStartSectorLoop,
                                  g_dwNumSectorsLoop,
                                  TestCallBackFunction) != SUCCESS)
            {
              SystemHalt();
              return(!SUCCESS);
            }

          // Write the sectors
          for(g_wSector=g_dwStartSectorLoop;
              g_wSector<(g_dwStartSectorLoop + g_dwNumSectorsLoop);
              g_wSector++)
            {
              if(bMultiWriteBufferFree[wMultiWriteBufferIdx] != TRUE)
                {
                  SystemHalt();
                }
        
              bMultiWriteBufferFree[wMultiWriteBufferIdx] = FALSE;
        
              // Init the compare buffer
              for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
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

          dwMultiWriteTimeEnd = GetDclkCount();
          dwMultiWriteTimeTotal = dwMultiWriteTimeEnd - dwMultiWriteTimeStart;

          // Verify the sectors
          for(g_wSector=g_dwStartSectorLoop;
              g_wSector<(g_dwStartSectorLoop + g_dwNumSectorsLoop);
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

              // Verify the sector
              for(j=0;j<NUM_DATA_WORDS_PER_SECTOR;j++)
                {
                  if(pReadBuffer[j] != g_wSector)
                    {
                      SystemHalt();
                      return(!SUCCESS);
                    }
                }
            }
        }
    }    

  return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MyTest3
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
#define TEST_3_START_SECTOR     0
#define TEST_3_NUM_SECTORS      503
RETCODE MyTest3(void)
{
  INT i,j;
  WORD wDataDriveNum;
  BOOL bDataDriveFound = FALSE;
  LOGICAL_DRIVE_TYPE DriveType;
    
#if defined(DEVICE_3500)
  if(PllInit() != SUCCESS)
    {
      SystemHalt();
    }
    
#endif

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

  for(i=0;i<NUM_MULTI_WRITE_BUFFERS;i++)
    {
      bMultiWriteBufferFree[i] = TRUE;
    }
  wMultiWriteBufferIdx = 0;

  SetDclkCount(0);
  dwMultiWriteTimeStart = GetDclkCount();

  // Multi-write setup
  if(DriveMultiWriteSetup(
                          wDataDriveNum,
                          TEST_3_START_SECTOR,
                          TEST_3_NUM_SECTORS,
                          TestCallBackFunction) != SUCCESS)
    {
      SystemHalt();
      return(!SUCCESS);
    }

  // Write the sectors
  for(g_wSector=TEST_3_START_SECTOR;
      g_wSector<(TEST_3_START_SECTOR+TEST_3_NUM_SECTORS);
      g_wSector++)
    {
      if(bMultiWriteBufferFree[wMultiWriteBufferIdx] != TRUE)
        {
          SystemHalt();
        }
        
      bMultiWriteBufferFree[wMultiWriteBufferIdx] = FALSE;
        
      // Init the compare buffer
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
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

  dwMultiWriteTimeEnd = GetDclkCount();
  dwMultiWriteTimeTotal = dwMultiWriteTimeEnd - dwMultiWriteTimeStart;

  // Verify the sectors
  for(g_wSector=TEST_3_START_SECTOR;
      g_wSector<(TEST_3_START_SECTOR+TEST_3_NUM_SECTORS);
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

      // Verify the sector
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR;j++)
        {
          if(pReadBuffer[j] != g_wSector)
            {
              SystemHalt();
              return(!SUCCESS);
            }
        }
    }
    
  SystemHalt();
    
  return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TestSystemDriveGetInfo
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
RETCODE TestSystemDriveGetInfo(void)
{
  INT i;
  LOGICAL_DRIVE_TYPE eType;
  WORD wTag;
    
    
  for(i = g_wHiddenDriveIdx+1 ; i < g_wNumDrives ; i++)   // start with the index after the hidden data drive... it's the last data drive in the system
    {
      DriveGetInfo(i, DriveInfoType, (void *)&eType);
      if(eType != g_MediaAllocationTable[0].Entry[i].Type)
        {
          return(!SUCCESS);
        }
        
      DriveGetInfo(i, DriveInfoTag, (void *)&wTag);
      if(wTag != g_MediaAllocationTable[0].Entry[i].wTag)
        {
          return(!SUCCESS);
        }
        
    }

  return(SUCCESS);
    
}



BOOL  g_bAnalyzeSystemDrive = FALSE;
INT   g_iAnalyzeSystemDriveNumber = 0;
DWORD g_dwAnalyzeSystemDriveSector = 0;

RETCODE TestSystemDriveReadWrite(void)
{
  DWORD dwSectorCount;
  INT iDrive, i;


  //
  // Write & verify all the sectors of all drives
  //
    

  // But first... some diagnostic tools:
  // If g_bAnalyzeSystemDrive is TRUE, go into a loop to read specified System Drive Sectors
  // BEFORE WRITING ABYTHING, so they can be analyzed.  
  // Set g_iAnalyzeSystemDriveNumber and g_dwAnalyzeSystemDriveSector to tell which Sector you want.
  // Look in pReadBuffer to see the results of the read.
  // Clear g_bAnalyzeSystemDrive to break out of the loop.
  // This is probably only useful if g_bAllocate is FALSE.

  while (g_bAnalyzeSystemDrive) {
    if(DriveReadSector(g_iAnalyzeSystemDriveNumber, g_dwAnalyzeSystemDriveSector, pReadBuffer) != SUCCESS) {
      // ERROR!
      SystemHalt();
    }

    // Analyze results in pReadBuffer.
    SystemHalt();
  }


  // First, loop through all drives & write patterns to all sectors
  dwSectorCount = 1;
  for(iDrive = g_wHiddenDriveIdx+1 ; iDrive < g_wNumDrives ; iDrive++)
    {
      // First get the total number of sectors for the drive
      if(DriveGetInfo(iDrive, DriveInfoSizeInSectors, &dwNumSectors) != SUCCESS)
        {
          return(!SUCCESS);
        }
    
      // Now loop through all sectors
      for(dwSector = 0 ; dwSector < dwNumSectors ; dwSector++)
        {
          // Init the sector
          pWriteBuffer[0] = (WORD)(iDrive); 
          for(i=1;i<NUM_DATA_WORDS_PER_SECTOR+1;i++)
            {
              pWriteBuffer[i] = (WORD)(dwSectorCount); 
            }

          // Write the sector
          if(DriveWriteSector(iDrive, dwSector, pWriteBuffer) != SUCCESS)
            {
              return(!SUCCESS);
            }
            
          // Read the sector
          if(DriveReadSector(iDrive, dwSector, pReadBuffer) != SUCCESS)
            {
              return(!SUCCESS);
            }

          // Bump the sector count
          dwSectorCount++;
        }
    }
    
  // Now loop through all drives & verify each sector
  dwSectorCount = 1;
  for(iDrive = g_wHiddenDriveIdx+1 ; iDrive < g_wNumDrives ; iDrive++)
    {
      // First get the total number of sectors for the drive
      if(DriveGetInfo(iDrive, DriveInfoSizeInSectors, &dwNumSectors) != SUCCESS)
        {
          return(!SUCCESS);
        }
        
      // Now loop through all sectors
      for(dwSector = 0 ; dwSector < dwNumSectors ; dwSector++)
        {
          // Init the read sector
          for(i=0;i<NUM_DATA_WORDS_PER_SECTOR;i++)
            {
              pReadBuffer[i] = 0; 
            }
    
          // Read the sector
          if(DriveReadSector(iDrive, dwSector, pReadBuffer) != SUCCESS)
            {
              return(!SUCCESS);
            }
            
          // Verify the sector
          pWriteBuffer[0] = (WORD)(iDrive); 
          for(i=1;i<NUM_DATA_WORDS_PER_SECTOR;i++)
            {
              pWriteBuffer[i] = (WORD)(dwSectorCount); 
            }

          for(i=0;i<NUM_DATA_WORDS_PER_SECTOR;i++)
            {
              if(pReadBuffer[i] != pWriteBuffer[i])
                {
                  return(!SUCCESS);
                }
            }
            
          // Bump the sector count
          dwSectorCount++;
        }
    }    
    
  return(SUCCESS);
}


BOOL  g_bAnalyzeDataDrive = FALSE;
INT   g_iAnalyzeDataDriveNumber = 0;
DWORD g_dwAnalyzeDataDriveSector = 0;

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TestDataDrive
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
RETCODE TestDataDrive(void)
{
  INT i,j;
  WORD wDataDriveNum;
  BOOL bDataDriveFound = FALSE;
  LOGICAL_DRIVE_TYPE DriveType;

    /////////////////////////////////////////////////////////////////////////////////
    //  Normal test is every third sector, for 1/64 of drive.
    //  For thorough test, uncomment FULL_DATA_DRIVE_TEST, below.
    //  For fast test of first 50 sectors, uncomment FIRST_50_DATA_DRIVE_TEST, below.
    /////////////////////////////////////////////////////////////////////////////////

// #define FULL_DATA_DRIVE_TEST 1
// #define FIRST_50_DATA_DRIVE_TEST 1




  // But first... some diagnostic tools:
  // If g_bAnalyzeDataDrive is TRUE, go into a loop to read specified Data Drive Sectors
  // BEFORE WRITING ABYTHING, so they can be analyzed.  
  // Set g_iAnalyzeDataDriveNumber and g_dwAnalyzeDataDriveSector to tell which Sector you want.
  // Look in pReadBuffer to see the results of the read.
  // Clear g_bAnalyzeDataDrive to break out of the loop.
  // This is probably only useful if g_bAllocate is FALSE.

  while (g_bAnalyzeDataDrive) {
    if(DriveReadSector(g_iAnalyzeDataDriveNumber, g_dwAnalyzeDataDriveSector, pReadBuffer) != SUCCESS) {
      // ERROR!
      SystemHalt();
    }

    // Analyze results in pReadBuffer.
    SystemHalt();
  }

    
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

  // Write & re-write sectors
  for(g_wSector=0;g_wSector<(dwNumSectors/64);g_wSector++)
    //    for(g_wSector=0;g_wSector<5;g_wSector++)
    {
      // Init the sector
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
        {
          pWriteBuffer[j] = (WORD)(g_wSector); 
        }

      if(DriveWriteSector(
                          wDataDriveNum,
                          (DWORD)(g_wSector*3),
                          pWriteBuffer) != SUCCESS)
        {
          SystemHalt();
          return(!SUCCESS);
        }

      // Init the sector
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
        {
          pWriteBuffer[j] = (WORD)(~g_wSector); 
        }

      if(DriveWriteSector(
                          wDataDriveNum,
                          (DWORD)(g_wSector*3),
                          pWriteBuffer) != SUCCESS)
        {
          SystemHalt();
          return(!SUCCESS);
        }
    }

  // Verify all sectors written
  for(g_wSector=0;g_wSector<(dwNumSectors/64);g_wSector++)
    //    for(g_wSector=0;g_wSector<5;g_wSector++)
    {
      if(DriveReadSector(
                         wDataDriveNum,
                         (DWORD)(g_wSector*3),
                         pReadBuffer) != SUCCESS)
        {
          SystemHalt();
          return(!SUCCESS);
        }

      // Init the compare buffer
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
        {
          pWriteBuffer[j] = (WORD)(~g_wSector); 
        }

      // Verify the sector
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR;j++)
        {
          if(pReadBuffer[j] != pWriteBuffer[j])
            {
              SystemHalt();
              return(!SUCCESS);
            }
        }
    }

  if (g_bAnalyze)
    SystemHalt();


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
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
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
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
        {
          pWriteBuffer[j] = (WORD)(i); 
        }

      // Verify the sector
      for(j=0;j<NUM_DATA_WORDS_PER_SECTOR;j++)
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
  */    
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
          for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
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
          for(j=0;j<NUM_DATA_WORDS_PER_SECTOR+1;j++)
            {
              pWriteBuffer[j] = (WORD)(g_wSector); 
            }

          // Verify the sector
          for(j=0;j<NUM_DATA_WORDS_PER_SECTOR;j++)
            {
              if(pReadBuffer[j] != pWriteBuffer[j])
                {
                  SystemHalt();
                  return(!SUCCESS);
                }
            }
        }
    }
    
  return SUCCESS;
}

/*
  RETCODE Hackage(void)
  {
  INT i,j;
  WORD wDataDriveNum;
  BOOL bDataDriveFound = FALSE;
  LOGICAL_DRIVE_TYPE DriveType;
    
  #ifdef D_D3500
  if(PllInit() != SUCCESS)
  {
  SystemHalt();
  }
    
  #endif

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
    
  SetDclkCount(0);
  dwMultiWriteTimeStart = GetDclkCount();

  //    FillSectorBuffer(pWriteBuffer, 0x33);
  //    if(DriveWriteSector(wDataDriveNum, 0x33, pWriteBuffer) != SUCCESS) SystemHalt();
  //    if(DoDaMultiWrite(wDataDriveNum,  0x220 , 8) != SUCCESS) SystemHalt();
  //    if(DoDaMultiWrite(wDataDriveNum,  0x220 , 8) != SUCCESS) SystemHalt();
  //    if(DoDaMultiWrite(wDataDriveNum,   0x34, 4) != SUCCESS) SystemHalt();
  //    if(DoDaMultiWrite(wDataDriveNum,  0x12A , 4) != SUCCESS) SystemHalt();
  //    if(DoDaMultiWrite(wDataDriveNum,  0x220 , 8) != SUCCESS) SystemHalt();
  
  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector = 0x240), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  #ifdef DEBUG_128_SECTOR_MWRITES
  g_dwMwriteTimeStart = GetDclkCount();
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  #ifdef DEBUG_128_SECTOR_MWRITES
  if(g_wMwriteTimeIdx < NUM_128_SECTOR_MWRITES)
  {
  g_wMwriteTime[g_wMwriteTimeIdx] = 
  (WORD)(GetDclkCount() - g_dwMwriteTimeStart);
  g_wMwriteTimeIdx++;
  }
  #endif  
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x33);
  if(DriveWriteSector(wDataDriveNum, 0x33, pWriteBuffer) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  0x1440, 128) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x34);
  if(DriveWriteSector(wDataDriveNum, 0x34, pWriteBuffer) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x12A);
  if(DriveWriteSector(wDataDriveNum, 0x12A, pWriteBuffer) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum, (g_wStartSector=0x14C0), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  (g_wStartSector += 128), 128) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  0x1DC0 , 0x38) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x33);
  if(DriveWriteSector(wDataDriveNum, 0x33, pWriteBuffer) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  0x220 , 0x8) != SUCCESS) SystemHalt();
  if(DoDaMultiWrite(wDataDriveNum,  0x220 , 0x8) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x34);
  if(DriveWriteSector(wDataDriveNum, 0x34, pWriteBuffer) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x12A);
  if(DriveWriteSector(wDataDriveNum, 0x12A, pWriteBuffer) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x33);
  if(DriveWriteSector(wDataDriveNum, 0x33, pWriteBuffer) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x34);
  if(DriveWriteSector(wDataDriveNum, 0x34, pWriteBuffer) != SUCCESS) SystemHalt();
  //    FillSectorBuffer(pWriteBuffer, 0x12A);
  if(DriveWriteSector(wDataDriveNum, 0x12A, pWriteBuffer) != SUCCESS) SystemHalt();

  dwMultiWriteTimeEnd = GetDclkCount();
  dwMultiWriteTimeTotal = dwMultiWriteTimeEnd - dwMultiWriteTimeStart;

  for(g_wSector=0x240;g_wSector<dwNumSectors;g_wSector++)
  //    for(g_wSector=0x240;g_wSector<(4*128);g_wSector++)
  {
  if(DriveReadSector(
  wDataDriveNum,
  (DWORD)(g_wSector),
  pReadBuffer) != SUCCESS)
  {
  SystemHalt();
  }
        
  if(((pReadBuffer[171] & 0xFFFF00) >> 8) != (g_wSector & 0x0000FF))
  {
  SystemHalt();
  SystemHalt();
  }
        
  if(((pReadBuffer[174] & 0xFFFF00) >> 8) != ((g_wSector >> 8) + 3))
  {
  SystemHalt();
  SystemHalt();
  }
        
  }
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
INT i;
    
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

 if(dwStartSector >= 0x3C0)
   EventSetBlockWrite();

 // Write the sectors
 for(g_wSector=(WORD)dwStartSector;
     g_wSector<(WORD)(dwStartSector+(DWORD)wNumSectors);
     g_wSector++)
   {
     if(bMultiWriteBufferFree[wMultiWriteBufferIdx] != TRUE)
       {
         SystemHalt();
       }
        
     bMultiWriteBufferFree[wMultiWriteBufferIdx] = FALSE;
        
     //        FillSectorBuffer(pMultiWriteBuffer[wMultiWriteBufferIdx],
     //            g_wSector);
        
     EventSetDriveMultiWriteSector();

     if((g_rtCode = DriveMultiWriteSector(
                                          wDriveNum,
                                          pMultiWriteBuffer[wMultiWriteBufferIdx],
                                          (LONG)wMultiWriteBufferIdx)) != SUCCESS)
       {
         SystemHalt();
         return(!SUCCESS);
       }
        
     EventClearDriveMultiWriteSector();

     wMultiWriteBufferIdx++;
     if(wMultiWriteBufferIdx == NUM_MULTI_WRITE_BUFFERS)
       {
         wMultiWriteBufferIdx = 0;
       }
   }

 if(dwStartSector >= 0x3C0)
   EventClearBlockWrite();

 return SUCCESS;
}
*/
void FillSectorBuffer(P_SECTOR_BUFFER pBuffer, WORD wValue)
{ 
  INT i;
    
  for(i=0;i<NUM_DATA_WORDS_PER_SECTOR+1;i++)
    {
      pBuffer[i] = wValue;
    }
}

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

////////////////////////////////////////////////////////////////////////////////
// D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! !
// D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! ! D A N G E R ! ! !
//>  Name:          NandMediaEraseALL
//
//   Type:          Function
//
//   Description:   This function erases ALL blocks, good or bad. It doesn't care.
//
//   Inputs:        pDescriptor:    Pointer to the logical media descriptor
//                                  structure
//                  wMagicNumber:   Magic Number - used to prevent accidental call to function.

//
//   Outputs:       SUCCESS or error code if fail
//
//   Notes:         
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NandMediaEraseALL(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor,
                                     WORD wMagicNumber)
{
  INT iNandNumber, iBlockCounter, i, j;
  INT iBadBlockCounter=0;                     // counts number of bad blocks across all NAND media
  INT iBlockPhysAddr;
  INT iNumSectorsPerPage;
  RETCODE Status;
  WORD wNumberSectorsPerBlock;
  DWORD dwSectorNumber;

  NAND_MEDIA_INFO *pNandMediaInfoDesc;
  NAND_PHYSICAL_MEDIA_DESCRIPTOR *pNandPhysicalMediaDesc;
  NAND_PARAMETERS_DESCRIPTOR *pNandParamsDesc;
  NANDHAL_API_DESCRIPTOR *pNandHalApiDesc;

  // Setup some pointers to use later
  pNandMediaInfoDesc = pDescriptor->pMediaInfo;

  // For each NAND in the system
  for (iNandNumber = 0; iNandNumber < pNandMediaInfoDesc->iNumNANDChips; iNandNumber++)
    {
      // Setup some pointers to use later
      pNandPhysicalMediaDesc = &(pNandMediaInfoDesc->pNANDDesc[iNandNumber]);
      pNandParamsDesc = pNandPhysicalMediaDesc->pNANDParams;
      pNandHalApiDesc = pNandParamsDesc->pNandHalApi;
      // Get Sectors Per Block and Sectors Per Page (to use later)
      wNumberSectorsPerBlock = pNandParamsDesc->wNumberSectorsPerBlock;
      iNumSectorsPerPage = 1 << pNandParamsDesc->wSectorToPageShift; 

      // LOCK the NAND to prevent other routines from interfering
      NANDHalLockNand(pNandPhysicalMediaDesc);

      // Enable Writes on the write protect line
      (*pNandHalApiDesc->pEnableWrites)(WP_NAND_INTERNAL);

      // For every block of the individual NAND chip
      for(iBlockCounter = 0 ; iBlockCounter < pNandPhysicalMediaDesc->wTotalBlocks; iBlockCounter++)
        {
          // Calculate the absolute physical address of the block
          iBlockPhysAddr = iBlockCounter;      
            
          // Erase the block
          Status = (*pNandHalApiDesc->pEraseBlock)(pNandPhysicalMediaDesc, (WORD)iBlockCounter);
 
          if( Status != SUCCESS )
            {
              if(Status == NANDHAL_ERROR_WRITE_FAILED)
                {   // The block is BAD
                    // Calculates the 1st sector in the block
                  dwSectorNumber = iBlockCounter * wNumberSectorsPerBlock;

                  // Mark all pages bad within the block until one reads back bad
                  for(i = 0 ; i < pNandParamsDesc->wPagesPerBlock ; i++)
                    {
#ifdef USE_ECC                                        
                        // Mark the block as "bad"
                        NANDRASetBlockStatus(NANDMediaSectorBuffer, WHOLE_SECTOR, 0);
                        // Do a CRC over the RA
                        NANDRAFillCRC(NANDMediaSectorBuffer, WHOLE_SECTOR);
                        // Calculate the ECC
                        if (NANDCalculateCompleteEccSync(
                          pNandPhysicalMediaDesc, NANDMediaSectorBuffer) != SUCCESS)
                        {
                          SystemHalt();
                        }
                        // Change to "on the NAND" form.
                        NANDRA_ToNANDForm(NANDMediaSectorBuffer, WHOLE_SECTOR);
#endif
                      // Write zeros to the entire sector (including redundant area)
                      pNandHalApiDesc->pWriteSector(pNandPhysicalMediaDesc, (WORD)dwSectorNumber, NANDMediaSectorBuffer);
                      // Read back the redundant area (need to check the Bad Block Byte)
                      pNandHalApiDesc->pReadSectorRedundant(pNandPhysicalMediaDesc,(WORD)dwSectorNumber, pRedundBufTemp, FALSE);
                      NANDRA_ToNANDForm(pRedundBufTemp, RA_ONLY);
                      // IF Bad Block Byte != FF, the block has been successfully marked bad
                      if((NANDRedundantArea[RDNT_BAD_BLOCK_WORD_STATUS_POS] 
                          & RDNT_GOOD_BLOCK_MASK) !=  RDNT_GOOD_BLOCK_MASK)
                        break;       // as soon as a page is marked bad, we are done
                      dwSectorNumber += (WORD)iNumSectorsPerPage;
                    }
                } // endif(Status == NANDHAL_ERROR_WRITE_FAILED)
            } // endif ( Satus != SUCCESS ) 


        }   // End For Every Block on the NAND

      // Release the NAND
      NANDHalReleaseNand(pNandPhysicalMediaDesc);

      // Disable writes to chip
      (*pNandHalApiDesc->pDisableWrites)(WP_NAND_INTERNAL);
  
    } // endfor (i = 0; i < pDescriptor->pMediaInfo->iNumNANDChips; i++)

  return SUCCESS;
}

RETCODE _reentrant NandMediaSetBadBlocks(P_LOGICAL_MEDIA_DESCRIPTOR pDescriptor, NAND_BAD_BLOCK_LIST * pNandBadBlockList)
{
  int i, j, k;
  int iNumChips = pNandBadBlockList->iNumNands;
  INT iNumSectorsPerPage, iBadBlockNumber;
  BAD_BLOCK_MAKER_TABLE * pBadBlockTable;
  WORD wNumberSectorsPerBlock;
  DWORD dwSectorNumber;

  NAND_MEDIA_INFO *pNandMediaInfoDesc;
  NAND_PHYSICAL_MEDIA_DESCRIPTOR *pNandPhysicalMediaDesc;
  NAND_PARAMETERS_DESCRIPTOR *pNandParamsDesc;
  NANDHAL_API_DESCRIPTOR *pNandHalApiDesc;

  // Setup some pointers to use later
  pNandMediaInfoDesc = pDescriptor->pMediaInfo;

  for ( i = 0; i < iNumChips; i++)
    {
      // Setup some pointers to use later
      pNandPhysicalMediaDesc = &(pNandMediaInfoDesc->pNANDDesc[i]);
      pNandParamsDesc = pNandPhysicalMediaDesc->pNANDParams;
      pNandHalApiDesc = pNandParamsDesc->pNandHalApi;
      pBadBlockTable = pNandBadBlockList->NandBadBlockMakerTable[i];
      // Get Sectors Per Block and Sectors Per Page (to use later)
      wNumberSectorsPerBlock = pNandParamsDesc->wNumberSectorsPerBlock;
      iNumSectorsPerPage = 1 << pNandParamsDesc->wSectorToPageShift; 

      // LOCK the NAND to prevent other routines from interfering
      NANDHalLockNand(pNandPhysicalMediaDesc);

      // Enable Writes on the write protect line
      (*pNandHalApiDesc->pEnableWrites)(WP_NAND_INTERNAL);

      // FOR each of the blocks listed in the table to mark bad
      for (j = 0; j < pBadBlockTable->iNumBadBlocks; j++)
        {
          // Get Block Number to mark bad
          iBadBlockNumber = pBadBlockTable->wBadBlockAddr[j];

          // Calculate the 1st sector in the block
          dwSectorNumber = (DWORD)iBadBlockNumber * (DWORD)wNumberSectorsPerBlock;

          // Mark all pages bad within the block until one reads back bad
          for(k = 0 ; k < pNandParamsDesc->wPagesPerBlock ; k++)
            {
              // Write zeros to the entire sector (including redundant area)
              pNandHalApiDesc->pWriteSector(pNandPhysicalMediaDesc, (WORD)dwSectorNumber, NANDMediaSectorBuffer);
              // Read back the redundant area (need to check the Bad Block Byte)
              pNandHalApiDesc->pReadSectorRedundant(pNandPhysicalMediaDesc,(WORD)dwSectorNumber, pRedundBufTemp, FALSE);
              NANDRA_ToNANDForm(pRedundBufTemp, RA_ONLY);
              // IF Bad Block Byte != FF, the block has been successfully marked bad
              if((NANDRedundantArea[RDNT_BAD_BLOCK_WORD_STATUS_POS] 
                  & RDNT_GOOD_BLOCK_MASK) !=  RDNT_GOOD_BLOCK_MASK)
                break;       // as soon as a page is marked bad, we are done
              dwSectorNumber += (WORD)iNumSectorsPerPage;
            }

        } // for (j = 0; j < pBadBlockTable->iNumBadBlocks; j++)

      // Release the NAND
      NANDHalReleaseNand(pNandPhysicalMediaDesc);

      // Disable writes to chip
      (*pNandHalApiDesc->pDisableWrites)(WP_NAND_INTERNAL);

    } // for ( i = 0; i < iNumChips; i++)

  return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          NANDDataDriveReadSectorEccTest
//
//   Type:          Function
//
//   Description:   This function is the same as NANDDataDriveReadSector, except that 
//                  this one does tests on the ECC within it to be sure the ECC is working
//                  properly.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant NANDDataDriveReadSectorEccTest(
                                                  P_LOGICAL_DRIVE_DESCRIPTOR pDescriptor,
                                                  DWORD dwSectorNumber,
                                                  P_SECTOR_BUFFER pSectorData, INT iNandNumber)
{
  INT i;
  RETCODE rtCode;
  P_NAND_REGION_INFO g_pRegion;
  WORD wRegionRelativeSectorNumber;
  WORD wPhysicalBlockNumber;
  WORD wPhysicalHomeBlockNumber;
  WORD wPhysicalHomeSectorOffset;
  WORD wPhysicalReplacementBlockNumber;
  WORD wPhysicalReplacementSectorOffset;
  WORD wPhysicalUpdateBlockNumber;
  WORD wPhysicalUpdateSectorOffset;
  WORD wPhysicalSectorAddress;
  P_NAND_MEDIA_INFO pMediaInfo;
  WORD wSectorOffset;
  WORD wEccTemp,wEccTemp2,wEccTemp3;
  WORD wSectorNumber;

  NAND_MEDIA_INFO *pNandMediaInfoDesc;
  NAND_PHYSICAL_MEDIA_DESCRIPTOR *pNandPhysicalMediaDesc;
  NAND_PARAMETERS_DESCRIPTOR *pNandParamsDesc;
  NANDHAL_API_DESCRIPTOR *pNandHalApiDesc;

  // Setup some pointers to use later
  pNandMediaInfoDesc = pDescriptor->pMediaInfo;
  pNandPhysicalMediaDesc = &(pNandMediaInfoDesc->pNANDDesc[iNandNumber]);
  pNandParamsDesc = pNandPhysicalMediaDesc->pNANDParams;
  pNandHalApiDesc = pNandParamsDesc->pNandHalApi;
    
  // Make sure we're initialized
  if(!pDescriptor->bInitialized)
    {
      return LDRIVE_ERROR_NOT_INITIALIZED;
    }

  // Make sure we won't go out of bounds
  if(dwSectorNumber >= pDescriptor->dwNumberOfSectors)
    {
      return LDRIVE_ERROR_SECTOR_OUT_OF_BOUNDS;
    }

  // Get the media info
  pMediaInfo = (P_NAND_MEDIA_INFO)(pDescriptor->pMediaInfo);
    
  // Convert Logical to Relative
  if(NANDDataDriveConvertLogicalToRelative(
                                           pDescriptor->pDriveInfo,
                                           dwSectorNumber,
                                           &g_pRegion,
                                           &wRegionRelativeSectorNumber) != SUCCESS)
    {
      // TODO:  what to do?
    }
    
  // Convert Relative to Physical
  if(NANDDataDriveConvertRelativeToPhysical(
                                            g_pRegion,
                                            wRegionRelativeSectorNumber,
                                            &wPhysicalHomeBlockNumber,
                                            &wPhysicalHomeSectorOffset) != SUCCESS)
    {
      // TODO:  what to do?
    }
    
  // Start off assuming that we're reading from the HBlock
  wPhysicalBlockNumber = wPhysicalHomeBlockNumber;
  wSectorOffset = wPhysicalHomeSectorOffset;

  // If the sector has been updated then we read from the UBlock, otherwise we may
  //  need to read from an RBlock.
  if((NANDDataDriveLookupUBlock(
                                g_pRegion,
                                wPhysicalHomeBlockNumber,
                                &wPhysicalUpdateBlockNumber) == SUCCESS) &&

     // This block has been updated but has this sector been updated?
     (NANDDataDriveLookupUSector(
                                 g_pRegion,
                                 wPhysicalUpdateBlockNumber,
                                 wPhysicalHomeSectorOffset,
                                 &wPhysicalUpdateSectorOffset) == SUCCESS))
    {
      // The sector has been updated so read from the UBlock.
      wPhysicalBlockNumber = wPhysicalUpdateBlockNumber;
      wSectorOffset = wPhysicalUpdateSectorOffset;
    }
  else
    {
      // The sector has not been updated... is it in an RBlock?
      if(NANDDataDriveLookupRBlock(
                                   g_pRegion,
                                   wPhysicalHomeBlockNumber,
                                   &wPhysicalReplacementBlockNumber) == SUCCESS)
        {
          // Use the RBlock
          wPhysicalBlockNumber = wPhysicalReplacementBlockNumber;
        }
    }
    
  wSectorNumber = (wPhysicalBlockNumber*g_pRegion->pNANDParms->wNumberSectorsPerBlock) + wSectorOffset;

  // Read the sector
  if((rtCode = 
      g_pRegion->pNANDParms->pNandHalApi->pReadSector(g_pRegion->pNANDDesc, wSectorNumber, pSectorData)) != SUCCESS)
    {
      // TODO:  what to do?  This is a fatal error
    }

  // ECC TEST CODE IS HERE.
  // FIRST -- FLIP TWO BITS -- ONE IN THE FIRST HALF OF PAGE, ONE IN SECOND.
  // ERASE THE BLOCK. (Use HAL function)
  // WRITE THE PAGE BACK. (Use HAL function)
  // READ THE PAGE BACK -- ECC SHOULD HAVE FIXED IT. (Use DataDrive read)

  // THREE ECC TESTS ARE DONE:
  //     1. Use data read off NAND. Flip a bit in first 256-words, flip one in second. These should get fixed.
  //     2. Create a data sector. Compute ECC. Flip a bit in first 256-words, and in second. These should get fixed.
  //     3. Create a data sector. Flip 2 bits in first 256 words. This should fail the ECC correction.

  //=============================================================
  // FLIP THE FIRST BIT of a word in the first half of the page
  wEccTemp = pSectorData[0];    // store off original value
  if (pSectorData[0] & 0x01)
    pSectorData[0] = pSectorData[0] - 1;
  else
    pSectorData[0] = pSectorData[0] + 1;
  // FLIP THE FIRST BIT of a word in the second half of the page
  wEccTemp2 = pSectorData[88];    // store off original value
  if (pSectorData[88] & 0x01)
    pSectorData[88] = pSectorData[88] - 1;
  else
    pSectorData[88] = pSectorData[88] + 1;
  //=============================================================

  // Enable Writes on the write protect line
  (*pNandHalApiDesc->pEnableWrites)(WP_NAND_INTERNAL);

  if(pNandHalApiDesc->pEraseBlock(pNandPhysicalMediaDesc, wPhysicalBlockNumber))
    return ERROR;
  if (pNandHalApiDesc->pWriteSector(pNandPhysicalMediaDesc, wSectorNumber, pSectorData))
    return ERROR;
  if (NANDDataDriveReadSector(pDescriptor, dwSectorNumber, pSectorData))
    return ERROR;

  // Verify that the ECC corrected the bits
  // Test if both words were corrected
  if (!((wEccTemp == pSectorData[0]) && (wEccTemp2 == pSectorData[88])))
    return ERROR;

  // CONTINUE HERE!!!!
  // FIRST ECC TEST HAS PASSED!!!!



  // SECOND ECC SHOULD PASS TOO.
  // THIS ONE WRITES A PATTERN TO BE SURE THE CORRECT PAGE WAS READ LATER.

  // create some data
  for (i = 0; i < NUM_DATA_WORDS_PER_SECTOR; i++)
    pSectorData[i] = 0x987654 + i;
  // fill in the redundant area with ECC numbers
  pNandHalApiDesc->pStartComputeECC(pSectorData,0);

  //=============================================================
  // FLIP THE FOURTH BIT of a word in the first half of the page
  wEccTemp = pSectorData[6];    // store off original value
  if (pSectorData[6] & 0x04)
    pSectorData[6] = pSectorData[6] - 4;
  else
    pSectorData[6] = pSectorData[6] + 4;
  // FLIP THE SECOND BIT of a word in the second half of the page
  wEccTemp2 = pSectorData[150];    // store off original value
  if (pSectorData[150] & 0x02)
    pSectorData[150] = pSectorData[150] - 2;
  else
    pSectorData[150] = pSectorData[150] + 2;
  //=============================================================

  // Enable Writes on the write protect line
  (*pNandHalApiDesc->pEnableWrites)(WP_NAND_INTERNAL);

  if(pNandHalApiDesc->pEraseBlock(pNandPhysicalMediaDesc, wPhysicalBlockNumber))
    return ERROR;
  if (pNandHalApiDesc->pWriteSector(pNandPhysicalMediaDesc, wSectorNumber, pSectorData))
    return ERROR;
  if (NANDDataDriveReadSector(pDescriptor, dwSectorNumber, pSectorData))
    return ERROR;

  // Verify that the ECC corrected the bits
  // Test if both words were corrected
  if (!((wEccTemp == pSectorData[6]) && (wEccTemp2 == pSectorData[150])))
    return ERROR;

  // CONTINUE HERE!!!!
  // SECOND ECC TEST HAS PASSED!!!!


  // THIRD TEST SHOULD FAIL -- WE EXPECT IT TO FAIL. So, return success only if it fails.

  // create some data
  for (i = 0; i < NUM_DATA_WORDS_PER_SECTOR; i++)
    pSectorData[i] = 0x123456 + i;
  // fill in the redundant area with ECC numbers
  pNandHalApiDesc->pStartComputeECC(pSectorData,0);

  //=============================================================
  // FLIP THE TWO BITS IN ONE HALF -- our ECC can't fix them.
  wEccTemp = pSectorData[0];    // store off original value
  if (pSectorData[0] & 0x01)
    pSectorData[0] = pSectorData[0] - 1;
  else
    pSectorData[0] = pSectorData[0] + 1;
  // Now the second bit.
  wEccTemp2 = pSectorData[5];    // store off original value
  if (pSectorData[5] & 0x01)
    pSectorData[5] = pSectorData[5] - 1;
  else
    pSectorData[5] = pSectorData[5] + 1;
  //=============================================================

  if(pNandHalApiDesc->pEraseBlock(pNandPhysicalMediaDesc, wPhysicalBlockNumber))
    return ERROR;
  if (pNandHalApiDesc->pWriteSector(pNandPhysicalMediaDesc, wSectorNumber, pSectorData))
    return ERROR;
  if ((NANDDataDriveReadSector(pDescriptor, dwSectorNumber, pSectorData)) != NANDHAL_ECC_FIX_FAILED)
    return ERROR; // fix should have failed

  // Double-check : Verify that ECC DID NOT fix both bits (this would indicate something funky going on)
  if ((wEccTemp == pSectorData[0]) && (wEccTemp2 == pSectorData[5]))
    return ERROR;

  // Disable writes to chip
  (*pNandHalApiDesc->pDisableWrites)(WP_NAND_INTERNAL);

  return SUCCESS;
}

#if defined(DEVICE_3500)
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          PllInit
//
//   Type:          Function
//
//   Description:   PLL initialization
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE PllInit(void)
{

   #pragma asm
      include "regsdcdc.inc"
      include "regsusb20.inc"
      include "regsusb20phy.inc"
      include "regsclkctrl.inc"
      
      
      ; Turns OFF the +5V current limit. This limits the inrush current.
      ; When using power supply source from USB, the inrush current most
      ; be limited to a value (I think 100mA, see USB specs).
      ; By now, if plugged to the USB, the inrush current must be over.
      bset    #HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS,x:HW_VDD5V_PWR_CHARGE

      ; WARNING !!!!
      ; This bit actually shuts down also the DC-DC Converter, therefore use
      ; only if you are sure that the part is operating from the linear
      ; regulators by sensing presence of 5 Volts.

      move    x:HW_USBCSR,x0
      jclr    #HW_USBCSR_VBUSSENSE_BITPOS,x0,_DoNotSetBrownout

      bset    #HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS,x:HW_VDD5V_PWR_CHARGE

_DoNotSetBrownout

      ; USB Pll is programmed to its default value of 60Mhz        

      ; From Steve Vu information
      bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR      ; Enables writes to Clock Control Register
      bset    #HW_CCR_PLLEN_BITPOS,x:HW_CCR      ; Enable (old 3410) PLL because the MUX 
                                          ; switching from old Pll to new (USB) PLL
                                          ; requires a clock during this phase (prevents glitches)

      move    #>$800000,x0
      move    x0,x:HW_USBPHYPWD           ; Force all PLL regs to default state
      move    #>$000000,x0
      move    x0,x:HW_USBPHYPWD           ; Power ON the PHY

      bset    #HW_USBRX_PLLLOCKED_BITPOS,x:HW_USBPHYRX ;Start PLL lock Sequence    

      clr     a
      move    #>1600,a0

_wait_loop
      ; We will wait a maximum of 200Us
      move    x:HW_USBPHYRX,x0
      jset    #HW_USBRX_PLLLOCKED_BITPOS,x0,_locked   ; Wait for PLL to lock    
      dec     a
      jne     _wait_loop

_locked
      bset    #HW_CCR_PLL_SOURCE_SEL_BITPOS,x:HW_CCR                 ; Select the new PLL (USB)
      bset    #HW_CCR_CKSRC_BITPOS,x:HW_CCR                      ; Route clock from Crystal to PLL

      bclr    #HW_CCR_PLLEN_BITPOS,x:HW_CCR                      ; Disable (old 3410) PLL now that we have
                                                          ; the new one locked

      bclr    #HW_USBCSR_CLKOFF_BITPOS,x:HW_USBCSR        ; Turns ON clock for USB Block
                                                          ; ARC + STMP USB INterface
    #pragma endasm
  
    return SUCCESS;
    
}

#endif
