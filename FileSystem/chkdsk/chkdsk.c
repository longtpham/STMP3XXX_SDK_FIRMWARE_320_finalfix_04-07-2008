////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: chkdsk.c
// Description: Check Disk Utility Main Program
// Limitations:
//  1 - Only works for 512 bytes sectors
//  2 - Because of the recursivity for CountSubDirs(), each sub directory
//      takes 2 words in the stack. Watch for stack overflows.
//
// To do:
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"
#include "platform.h"
#include "chkdsk.h"
#include "cwrapfunc.h"
#include "bitio.h"
#include "fatutils.h"
#include "project.h"
#include "mediadetection.h"
#include "filesystem.h"
#ifdef STMP_BUILD_MSC
    #include "misc.h"
#endif
#include "ddildl.h"
#include "ddildl_internal.h"
#include "filesystem.h"
#include "regsrtc.h"
#include "persist_bits.h"

#pragma optimize 3

//FAT_WarningMessage
#include "extern.h"


#define MAX_FAT_ENTRY_WORDS 2731            // Max Number of FAT entries (0xffff for FAT16)
                                            // divided by number of bits in a DSP word (24)

#define BYTESPERSECTOROFFSET	  11
#define MAX_NESTING_LEVEL         16
int nestinglevel=0;
extern DATA_DRIVE_PBS_LOC DataDriveStatus[];

extern RETCODE _reentrant DriveReadSector(WORD wLogDriveNumber, DWORD dwSectorNumber,
                                          P_SECTOR_BUFFER pSectorData);
RETCODE _reentrant DriveWriteSector(WORD wLogDriveNumber, DWORD dwSectorNumber,
                                    P_SECTOR_BUFFER pSectorData);

// All other global, custom configurable data is in Y memory.
// Two handles are reserved by the file system, so the number of handles must be more than 2

RETCODE _reentrant DataDriveInit(WORD wDriveNumber);

// TOVERIFY made this X instead of Y since buffer is defined as X. SECTOR_BUFFER is WORD _X (june15,05)
extern SECTOR_BUFFER * MediaBuffer;

extern _X WORD DirRecordBuffer[];                 // Holds a directory record sector
extern _X WORD FatBuffer[];                       // Buffer holds a sector of FAT for writing back the FAT
extern _X WORD XScratchSpace[];

WORD _X *FATEntryStatus;    // This table holds the status for each FAT entry
                            // Each FAT entry is one bit in this table
                            // If bit set to 1, the entry is used and good
                            // If bit set to 0, the entry is unknown

WORD _X *CacheMem_chkdsk[MAX_CACHES];        // This table will be used for FAT32

tCACHEDESCR_checkdisk CacheDesc_chkdsk[MAX_CACHES];

WORD FATBuffer[705*3];                         // Holds up to 3 sectors (of size 2KB or smaller)


WORD glb_wFileCorrupted = 0;        // Total number of corrupted files in the current device
BYTE glb_bFailReadSect = FALSE;                 // This flag indicates if a sector fail to read/write
WORD Flag = 0;                                  // on the current device. This is a critical error
                                                // and chkdsk activity should be suspended for the
                                                // current device

DWORD dwChkDskClkCycles = 0;        // For debug only
WORD _X cachedSectorSize;
WORD _X cachedSectorSizeInWords;
WORD _X cachedClusterEntryPerSectorShift;
WORD _X cachedDirRecordsPerSector;
WORD _X cachedWordsPerSector;

PARTITION_BOOT_SECTOR stPartitionBootSector;
FAT_STRUCT stFat = {0, 0, CLEAN, 1, 0, FATBuffer};
LONG (*GetNextCxFromFat) (DWORD wCurCx);     // pointer to function
extern LONG _reentrant GetNextCxFat12(DWORD wCurCx);
extern LONG _reentrant GetNextCxFat16(DWORD wCurCx);
extern LONG _reentrant GetNextCxFat32(DWORD wCurCx);
extern LONG _reentrant GetNextCx(DWORD wCurCx);

#if 0
void chkdskall(void)
{


    BYTE bLogicalDeviceCounter = MAX_LOGICAL_DEVICES;
    BYTE i = 0, j;
//    #pragma asm
//        debug
//    #pragma endasm


    for ( i=0;i<bLogicalDeviceCounter;i++ )
    {
        for ( j=0; j < MAX_CACHES;j++ )
        {
            CacheDesc_chkdsk[j].CacheValid = 0;
        }
        CheckDisk(i);
    }

    //dwChkDskClkCycles = GetTime() - dwChkDskClkCycles;

}
#endif
///////////////////////////////////////////////////////////////////////////////
//
//>  Name:          CheckDisk
//
//   Type:          Function
//
//   Description:
//
//   Inputs:        None
//
//   Outputs:
//
//   Notes:         None
//<
///////////////////////////////////////////////////////////////////////////////
void CheckDisk(BYTE bDiskNum)
{

	//FAT_WarningMessage
	extern INT		g_iCheckDiskError;


    DIR_CTRL_BLK stDirCtrlBlk;
    FILE_CTRL_BLK stFileCtrlBlk;
    // WORD wScanError;
    WORD j,Retval;
    DWORD sectorno,i,TotalFatsectors=0,sector_1st_FAT,SectorsToWrite,count;
    INT _X *buf;
    WORD RetVal;
    //   LONG (*GetNextCxFromFat) (DWORD wCurCx);     // pointer to function

    glb_wFileCorrupted = 0;
    glb_bFailReadSect = FALSE;

//    #pragma asm
//       debug
//    #pragma endasm

    FATEntryStatus = &XScratchSpace[0];


    //FAT_WarningMessage
    g_iCheckDiskError = 0;
		if (bDiskNum == 0)
		{
			if ((MediaTable[bDiskNum].RsvdSectors == 0) || (MediaTable[bDiskNum].FATSize == 0))
			{
					g_iCheckDiskError = 1;
					return;
			}
		}


    if ( SUCCESS != DataDriveInit(bDiskNum) )
    //FAT_WarningMessage
    //        SystemHalt();
	  {
			g_iCheckDiskError = 1;	
			return;
	  }

   /* 
    Initialize number of entry per cluster. However, due to the fact that we can 
    assume that a file is larger than 2 sectors in most cases, so the number of entries
    that we should examine can be less. 
    The case for 512 bytes per sector, the right value should be between 7 and 11. A reasonable 
    value can be set at 10. However, the smaller value we set, the slower the process. It is
    really a trade-off between speed and acuracy. but since it has been working with 512(12) 
    for a long time, I did not changed this value.
    For 2048 bytes per sector, the right value should be between 9 and 12. 
     */

    cachedSectorSize = g_Drive[bDiskNum].pDescriptor->wSectorSizeInBytes;
    cachedSectorSizeInWords = (cachedSectorSize+2)/3;
    switch ( cachedSectorSize )
    {
    case SEC_512_BYTES_PER_SECTOR:
        cachedClusterEntryPerSectorShift = SEC_512_BYTES_CLUSTER_ENTRY_PER_SECTOR_SHIFT;
        cachedDirRecordsPerSector = SEC_512_BYTES_DIR_REC_PER_SECTOR;
        cachedWordsPerSector = SEC_512_BYTES_WORDS_PER_SECTOR;
        break;

#ifndef LARGE_SECTOR_DEFS_AS_SMALL_SECTOR
    case SEC_2K_BYTES_PER_SECTOR:

        cachedClusterEntryPerSectorShift = SEC_2K_BYTES_CLUSTER_ENTRY_PER_SECTOR_SHIFT;
        cachedDirRecordsPerSector = SEC_2K_BYTES_DIR_REC_PER_SECTOR;
        cachedWordsPerSector = SEC_2K_BYTES_WORDS_PER_SECTOR;
        break;
#endif        
    default:

	   //FAT_WarningMessage
	   //        SystemHalt();
	   g_iCheckDiskError = 1;	
      
        return; // !!!!
    };

    j = 0;
    for ( i=0; i < MAX_CACHES; i++ )
    {
        CacheMem_chkdsk[i] = (WORD _X *) &XScratchSpace[j];
        j += cachedWordsPerSector;
    }

    for ( j=0; j < MAX_CACHES;j++ )
    {
        CacheDesc_chkdsk[j].CacheValid = 0;
        CacheDesc_chkdsk[j].WriteAttribute = 0;
    }

    // Initialize the Partition Boot Sector for current Logical Device
    if ( InitPartitionBootSectorStruct(bDiskNum) != NOERROR ) return;

    // QUit if FAT is not supported
    if ( stPartitionBootSector.TypeFileSystem == FATUNSUPPORTED ) return;

    // Initialize the Root Directory Control Block Structure
    stDirCtrlBlk.StartSectCurDir = stPartitionBootSector.wStartSectRootDir;
    stDirCtrlBlk.wStartCxCurDir = stPartitionBootSector.Rootdirstartcx;
    stDirCtrlBlk.CurSect = stDirCtrlBlk.NumberFiles =  0;
    stDirCtrlBlk.Device = bDiskNum;
    stDirCtrlBlk.pwBuffer = DirRecordBuffer;
    stDirCtrlBlk.Control = CLEAN;

    // Initializes part of the FAT structure
    stFat.FirstPrimaryFatSect =stPartitionBootSector.wStartSectPrimaryFat;
    stFat.FirstSecondaryFatSect = stPartitionBootSector.wStartSectSecondaryFat;
    stFat.Device = bDiskNum;
    stFat.Control = CLEAN;
    stFat.FatSectorCached = 0;
    stFat.pwBuffer = FATBuffer;

    if ( stPartitionBootSector.TypeFileSystem == FS_FAT32 )
    {
        sectorno= stFat.FirstSecondaryFatSect;
        TotalFatsectors = stPartitionBootSector.dwTotalclusters >> CLUSTER_ENTRY_PER_SECTOR_SHIFT;
        TotalFatsectors++;
        CacheMem_chkdsk[0][0] = 0x03;             // 2 first clusters are always reserved

        for ( i = 1; i < cachedWordsPerSector; i++ )
            CacheMem_chkdsk[0][i] = 0;

        if ( DriveWriteSector(bDiskNum, sectorno+DataDriveStatus[bDiskNum].iPbsSectorOffset, &CacheMem_chkdsk[0][0]) != NOERROR )
            return;
        sectorno++;


        CacheMem_chkdsk[0][0] = 0;               // Cleared for erasing rest of the sectors.
        // first sector is already written
        for ( i=0;i < (TotalFatsectors-1);i++ )
        {
            if ( DriveWriteSector(bDiskNum, sectorno+DataDriveStatus[bDiskNum].iPbsSectorOffset, &CacheMem_chkdsk[0][0]) != NOERROR )
                return;
            sectorno++;
        }
    }
    else
    {
        for ( j=0 ; j < MAX_FAT_ENTRY_WORDS ; j++ )
            FATEntryStatus[j] = 0x0;

        FATEntryStatus[0] = 0x03;              // 2 first clusters are always reserved
    }



    if ( stPartitionBootSector.TypeFileSystem == FS_FAT12 )
        GetNextCxFromFat = &GetNextCxFat12;

    else if ( stPartitionBootSector.TypeFileSystem == FS_FAT16 )
        GetNextCxFromFat = &GetNextCxFat16;

    else if ( stPartitionBootSector.TypeFileSystem == FS_FAT32 )
        GetNextCxFromFat = &GetNextCxFat32;

    else
        return;


    // Load the 1st 3 sectors of primary FAT
    LoadFatSector(stPartitionBootSector.wStartSectPrimaryFat, MediaBuffer);

    // Reserve the clusters occupied by Rootdirectory
    ReserveCluster(stPartitionBootSector.Rootdirstartcx,bDiskNum);


	//FAT_WarningMessage
	if (g_iCheckDiskError)
		return;


    // Scan Files and sub dirs in root directory
    if ( ScanFilesAndSubDirs(&stDirCtrlBlk) != NOERROR )
    {
        // Bad Root Directory ....
        HandleFailReadSector();
        return;
    }

    // Checking disk done
    // If a corrupted file has been found, we need to do the second pass
    // on scandisk. This second pass will find all used clusters for all
    // good files and sub directories and then delete all FAT entries
    // not used by the good files and subdirectories.
    if ( (glb_wFileCorrupted != 0) && (glb_bFailReadSect == FALSE) )
    {
        // Flush last sector if needed
        if ( stDirCtrlBlk.Control == DIRTY )
            if ( DriveWriteSector(stDirCtrlBlk.Device,stDirCtrlBlk.CurSect + DataDriveStatus[bDiskNum].iPbsSectorOffset,stDirCtrlBlk.pwBuffer) != NOERROR )
            {
                HandleFailReadSector();
                return;
            }


        ScanAndUpdateFat(bDiskNum,stPartitionBootSector.TypeFileSystem);
        if ( stFat.Control != CLEAN )
        {
            if ( WriteFatSector(stFat.FatSectorCached, FatBuffer) != NOERROR )
            {
                HandleFailReadSector();
                return;
            }
        }
    }

    /* copy primary FAT table to secondary FAT Table*/

#ifdef SECOND_FAT_OP
    sector_1st_FAT = stFat.FirstPrimaryFatSect + DataDriveStatus[bDiskNum].iPbsSectorOffset;;
    // sectorno = Starting sector of Secondary FAT
    sectorno= stFat.FirstSecondaryFatSect + DataDriveStatus[bDiskNum].iPbsSectorOffset;;

    if ( (glb_wFileCorrupted != 0) && (glb_bFailReadSect == FALSE) )
    {
        // When FAT is corrupted.
        count = stPartitionBootSector.wNumberFatSectors;
    }
    else
    {
        // When FAT is not corrupted, update only the sectors we used for bit buffering.
        count = TotalFatsectors;
    }

    while ( count > 0 )
    {
        if ( count > MAX_CACHES )
            SectorsToWrite = MAX_CACHES;
        else
            SectorsToWrite = count;
        count -= SectorsToWrite;
        // Read a group of sectors from Primary FAT
        for ( j = 0; j < SectorsToWrite; j++ )
        {
            if ( DriveReadSector(bDiskNum, (DWORD)sector_1st_FAT, &CacheMem_chkdsk[j][0]) != NOERROR )
                return;
            sector_1st_FAT++;
        }

        // Write a group of sectors to Secondary FAT
        for ( j=0;j< SectorsToWrite;j++ )
        {
            if ( DriveWriteSector(bDiskNum,sectorno,&CacheMem_chkdsk[j][0]) != NOERROR )
                return;
            sectorno++;
        }
    }
#endif

}



///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           ScanAndUpdateFat
//
//   Type:           Function
//
//   Description:    Scans FATEntryStatus array and frees up all FAT entries
//                      not used.
//
//   Inputs:        None
//
//   Outputs:        Result of operation
//                      NOERROR -> Successful
//                      !NOERROR -> Unsuccessful, at least a media sector
//                          could not be read/write. This is a major failure
//
//   Notes:         None
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant ScanAndUpdateFat(BYTE bDiskNum,FAT_TYPE TypeFileSystem)
{
    DWORD wNumberFatCx, wCurrentCluster;


    for ( wCurrentCluster = 2 ; wCurrentCluster < stPartitionBootSector.dwTotalclusters ; wCurrentCluster++ )
    {
        if ( UpdateBit(wCurrentCluster, FATEntryStatus,bDiskNum,stPartitionBootSector.TypeFileSystem,GET_BIT)==NOERROR )
            if ( FreeCxFat(wCurrentCluster) != NOERROR )
                return(!NOERROR);
    }

    return(NOERROR);

}


///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           CheckCrossLinkFile
//
//   Type:           Function
//
//   Description:    Checks if a file is cross linked with another previously
//                   checked file
//
//   Inputs:         wStartCluster      : Start Cluster for file to check
//
//   Outputs:        Result of operation
//                      FALSE -> No cross link
//                      TRUE -> Cross link
//
//   Notes:          When calling this function the caller should check
//                   if the file cluster chain terminates, otherwise we could
//                   be trapped in the loop
//<
///////////////////////////////////////////////////////////////////////////////
LONG _reentrant CheckCrossLinkFile(DWORD wStartCluster,BYTE bLogDevNumber)
{
    DWORD wCluster = wStartCluster, wClusterCount = 1,i;

    // This handles case of a 0 byte file
    if ( wCluster == 0 )
        return(FALSE);

    do
    {
        if ( UpdateBit(wCluster, FATEntryStatus,bLogDevNumber,stPartitionBootSector.TypeFileSystem,SET_BIT) )
        {
            wCluster = wStartCluster;
            for ( i = 0; i < (wClusterCount - 1); i++ )
            {
                UpdateBit(wCluster, FATEntryStatus,bLogDevNumber,stPartitionBootSector.TypeFileSystem,FREE_BIT);
                wCluster = GetNextCxFromFat(wCluster);
            }
            return(-1);
        }

        wCluster = GetNextCxFromFat(wCluster);

        if ( wCluster <= 1 )
        {
            wCluster = wStartCluster;
            for ( i = 0; i < wClusterCount ; i++ )
            {
                UpdateBit(wCluster, FATEntryStatus,bLogDevNumber,stPartitionBootSector.TypeFileSystem,FREE_BIT);
                wCluster = GetNextCxFromFat(wCluster);
            }
            return(-1);
        }

        if ( IsLastCx(wCluster) )
            return(wClusterCount);

        wClusterCount++;
    }while ( wClusterCount < 0x20000 );     // We should get out of this loop

    return(wClusterCount);
}






///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           ScanDirectory
//
//   Type:           Function
//
//   Description:    Scans a directory
//
//   Inputs:        *pstDirCtrlBlk      : Pointer to Directory Control Block
//
//   Outputs:        Result of operation
//                      NOERROR -> Successful
//                      !NOERROR -> The directory is in default
//
//   Notes:
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant ScanDirectory(DIR_CTRL_BLK *pstDirCtrlBlk)
{

    // Check if sub directory legitime. A legitime sub dir has:
    //  1st record = parent directory
    //  2nd record = this directory

    // Read 1st sector for the sub directory
    ReadDirSector(pstDirCtrlBlk->StartSectCurDir, pstDirCtrlBlk);

    // Check if 1st record is this directory
    if ( FSGetByte((void *)pstDirCtrlBlk->pwBuffer,0,X_MEMORY ) != 0x2e )
        return(!NOERROR);

    // Check if this directory has directory signature
    if ( !(FSGetByte((void *)pstDirCtrlBlk->pwBuffer,DIR_REC_ATT_POS,X_MEMORY) & 0x10) )
        return(!NOERROR);

    // Check if this directory size is 0
    if ( FSGetDWord((void *)pstDirCtrlBlk->pwBuffer,DIR_REC_SIZE_POS,X_MEMORY) != 0x0 )
        return(!NOERROR);

    // Check to make sure that start cluster is non-zero

    if ( (FSGetDWord((void *)pstDirCtrlBlk->pwBuffer,DIR_REC_FIRST_CX_HIGH_POS,X_MEMORY) == 0x0) &&
            (FSGetDWord((void *)pstDirCtrlBlk->pwBuffer,DIR_REC_FIRST_CX_POS,X_MEMORY) == 0x0) )
        return(!NOERROR);
   
    // Check if 2nd record is parent directory
    if ( FSGetWord((void *)pstDirCtrlBlk->pwBuffer,0 + BYTES_PER_DIR_RECORD, X_MEMORY) != 0x2e2e )
        return(!NOERROR);

    // Check if parent directory has directory signature
    if ( !(FSGetByte((void *)pstDirCtrlBlk->pwBuffer,DIR_REC_ATT_POS + BYTES_PER_DIR_RECORD,X_MEMORY) & 0x10) )
        return(!NOERROR);

    // Check if parent directory size is 0
    if ( FSGetDWord((void *)pstDirCtrlBlk->pwBuffer,DIR_REC_SIZE_POS + BYTES_PER_DIR_RECORD,X_MEMORY ) != 0x0 )
        return(!NOERROR);


    // Check for cross linked files
    if ( CheckCrossLinkFile(pstDirCtrlBlk->wStartCxCurDir,pstDirCtrlBlk->Device) < 0 )
    {
        return(!NOERROR);
    }


    return(NOERROR);
}




///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           ReserveCluster
//
//   Type:           Function
//
//   Description:    Mark clusters for the current file used in the FATEntryStatus
//                   array
//
//   Inputs:         wStartCluster      : Start Cluster for file to check
//
//   Outputs:        None
//
//   Notes:          When calling this function the caller should check
//                   if the file cluster chain terminates, otherwise we could
//                   be trapped in the loop
//<
///////////////////////////////////////////////////////////////////////////////

void _reentrant ReserveCluster(DWORD  wStartCluster,BYTE bLogDevNumber)
{
	
	//FAT_WarningMessage
	extern INT	g_iCheckDiskError;
	WORD		wClusterCount = 0;

    DWORD wCluster = wStartCluster;

    // This handles case of a 0 byte file
    if ( wCluster == 0 )
        return;

    do
    {
        UpdateBit(wCluster, FATEntryStatus,bLogDevNumber,stPartitionBootSector.TypeFileSystem,SET_BIT);
        wCluster = GetNextCxFromFat(wCluster);
        if ( IsLastCx(wCluster) )
        {
            return;
        }

				//FAT_WarningMessage
				wClusterCount++;
				if (wClusterCount >= stPartitionBootSector.dwTotalclusters)
				{
					g_iCheckDiskError = 1;
            return;
				}

    }while ( 1 );      // We should get out of this loop
}


///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           InitPartitionBootSectorStruct
//
//   Type:           Function
//
//   Description:    Initializes the global structure type PARTITION_BOOT_SECTOR
//                   for the device passed in.
//
//
//   Inputs:         bLogDevNumber      : Is the Logical Device Number
//
//   Outputs:        Result of operation
//                      NOERROR -> Successfull
//                      !NOERROR -> Unsuccessfull
//
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant InitPartitionBootSectorStruct(BYTE bLogDevNumber)
{
    BYTE bResult;
    DWORD wReadWord,TotalDatasectors,Totalclusters;
    WORD i,SecValue=1;
    /* Read the BootSector*/
    if ( DriveReadSector(bLogDevNumber, (DWORD)0 + DataDriveStatus[bLogDevNumber].iPbsSectorOffset, MediaBuffer ) != NOERROR )
    {
        return(!NOERROR);
    }


    wReadWord =  FSGetWord((void *)MediaBuffer,BYTE_POS_SIGNATURE, X_MEMORY);

    if ( wReadWord != SIGN_WORD_VALUE )
        return(!NOERROR);


    // Fill the rest of the PARTITION_BOOT_SECTOR structure
    wReadWord = FSGetByte((void *)MediaBuffer,BYTE_POS_NUM_FAT, X_MEMORY);
    stPartitionBootSector.bNumberFats = wReadWord;

    wReadWord =  FSGetWord((void *)MediaBuffer,BYTE_POS_NUM_ROOT_SECT, X_MEMORY);
    stPartitionBootSector.wNumberRootDirEntries = wReadWord;

    wReadWord =  FSGetWord((void *)MediaBuffer,BYTE_POS_NUM_FAT_SECT, X_MEMORY);
    if ( wReadWord == 0 )
        wReadWord =  FSGetDWord((void *)MediaBuffer,BYTE_POS_NUM_FAT_SECT_32, X_MEMORY);
    stPartitionBootSector.wNumberFatSectors = wReadWord;

    wReadWord = FSGetByte((void *)MediaBuffer,BYTE_POS_NUM_SECT_PER_CX, X_MEMORY);
 
   // check whether the sectors per cluster are valid
    for ( i = 0; i < 8; i++ )
    {
        if ( wReadWord == SecValue )
            break;
        SecValue <<= 1;
    }

    if ( SecValue == 256 )
        return(!NOERROR);
    stPartitionBootSector.bSectPerCx = wReadWord;

    wReadWord =  FSGetWord((void *)MediaBuffer,BYTE_POS_TOTAL_SECTS, X_MEMORY);
    if ( wReadWord == 0 )
        wReadWord =  FSGetDWord((void *)MediaBuffer,BYTE_POS_TOTAL_SECTS_32, X_MEMORY);
    stPartitionBootSector.dwTotalsectors  =  wReadWord;

    stPartitionBootSector.wStartSectPrimaryFat =  FSGetWord((void *)MediaBuffer,BYTE_POS_NUM_RES_SECT, X_MEMORY);
    stPartitionBootSector.wStartSectSecondaryFat = stPartitionBootSector.wStartSectPrimaryFat + stPartitionBootSector.wNumberFatSectors;
    stPartitionBootSector.wStartSectRootDir = stPartitionBootSector.wStartSectSecondaryFat + stPartitionBootSector.wNumberFatSectors;
    stPartitionBootSector.wStartSectData = stPartitionBootSector.wStartSectRootDir + (stPartitionBootSector.wNumberRootDirEntries/DIR_REC_PER_SECTOR);
    stPartitionBootSector.wNumberRootDirSectors = (stPartitionBootSector.wNumberRootDirEntries) /DIR_REC_PER_SECTOR;

    stPartitionBootSector.dwNumHiddenSectors =  FSGetByte((void *)MediaBuffer,BYTE_POS_NUM_HIDDEN_SECT, X_MEMORY);
    stPartitionBootSector.dwNumHiddenSectors |= ((DWORD)FSGetByte((void *)MediaBuffer,BYTE_POS_NUM_HIDDEN_SECT+1, X_MEMORY))<<8;
    stPartitionBootSector.dwNumHiddenSectors |= ((DWORD)FSGetByte((void *)MediaBuffer,BYTE_POS_NUM_HIDDEN_SECT+2, X_MEMORY))<<16;
    stPartitionBootSector.dwNumHiddenSectors |= ((DWORD)FSGetByte((void *)MediaBuffer,BYTE_POS_NUM_HIDDEN_SECT+3, X_MEMORY))<<24;

    TotalDatasectors = stPartitionBootSector.dwTotalsectors -(stPartitionBootSector.wStartSectPrimaryFat +
                                                              (stPartitionBootSector.bNumberFats * stPartitionBootSector.wNumberFatSectors) +
                                                              stPartitionBootSector.wNumberRootDirSectors);

    Totalclusters   =  (TotalDatasectors / stPartitionBootSector.bSectPerCx) + 1;
    stPartitionBootSector.dwTotalclusters = Totalclusters;
    if ( Totalclusters < 4085 )
        stPartitionBootSector.TypeFileSystem = FS_FAT12;
    else if ( Totalclusters < 65525 )
        stPartitionBootSector.TypeFileSystem = FS_FAT16;
    else
        stPartitionBootSector.TypeFileSystem = FS_FAT32;

    if ( stPartitionBootSector.TypeFileSystem == FS_FAT32 )
    {
        if ( stPartitionBootSector.bNumberFats < 2 )
            return(!NOERROR);
        wReadWord =  FSGetDWord((void *)MediaBuffer,BYTE_POS_ROOT_DIR_CX, X_MEMORY);
    }
    else
        wReadWord = 0;
    stPartitionBootSector.Rootdirstartcx = wReadWord;

    return(NOERROR);

}


///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           ScanFilesAndSubDirs
//
//   Type:           Function
//
//   Description:    Counts the number of files and sub directories
//                   for the directory specified in structure DIR_CTRL_BLK
//
//
//   Inputs:
//                  *pstDirCtrlBlk      : Structure defining the directory to process
//
//   Outputs:        Result of operation
//                      NOERROR -> Successfull
//                      !NOERROR -> Unsuccessfull
//
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant ScanFilesAndSubDirs(DIR_CTRL_BLK *pstDirCtrlBlk)
{
    WORD SectCounter = 0, RecordCounter,  n,  StartRecordByte;
    BOOL EndDirEntries = FALSE;
    BYTE bTempByte;
    WORD wTempWord;
    WORD wLoopCount,Retval;
    DWORD Sect,CurCx;
    FILE_CTRL_BLK stFileCtrlBlk;
    LONG wClusterCount,i;
    DWORD wCxSizeBytes = stPartitionBootSector.bSectPerCx * BYTES_PER_SECT; // Cx size in Bytes
    DWORD dwFileSizeDiskBytes,wCluster;
    DWORD Rootdircluster;
    // Initialize the sector to read to the start sector current dir
    Sect = pstDirCtrlBlk->StartSectCurDir;

    // Initialize Current Cx to start cluster current dir
    CurCx = pstDirCtrlBlk->wStartCxCurDir;
    Flag=1;
    nestinglevel++;
    //	Rootdircluster = CurCx;
    if ( nestinglevel > MAX_NESTING_LEVEL )
    {
        nestinglevel--;
        return(NOERROR);
    }
    // Scan all sectors allocated for this directory
    do
    {
        // If we're in the root directory then we just loop through a set number
        //  of sectors.  If not in the root then the directory may have multiple
        //  clusters and we process them one at a time.
        if ( CurCx == 0 )
        {
            wLoopCount = stPartitionBootSector.wNumberRootDirSectors;
        }
        else
        {
            wLoopCount = stPartitionBootSector.bSectPerCx;
        }

        for ( n = 0 ; n < wLoopCount ; Sect++, n++, SectCounter++ )
        {
            // Read new sector
            if ( ReadDirSector(Sect, pstDirCtrlBlk) != NOERROR )
                return(!NOERROR);

            // Scan all entries of sector loaded in Buffer
            for ( RecordCounter = 0; RecordCounter < DIR_REC_PER_SECTOR; RecordCounter++ )
            {
                if ( Flag == 1 )
                {
                    if ( ReadDirSector(Sect, pstDirCtrlBlk) != NOERROR )
                        return(!NOERROR);
                    Flag=0;
                }

                StartRecordByte = RecordCounter*BYTES_PER_DIR_RECORD;

                bTempByte = FSGetByte((void *)pstDirCtrlBlk->pwBuffer,StartRecordByte,X_MEMORY);

                // Check if last directory record
                if ( bTempByte == 0x0 )
                {
                    Flag=1;
                    nestinglevel--;
                    return(NOERROR);
                }

                // Check if record deleted or parent or this directory
                if ( (bTempByte == 0xe5) || (bTempByte == 0x2e) )
                    continue;

                // Check if record is part of long file name
                bTempByte = FSGetByte((void *)pstDirCtrlBlk->pwBuffer,StartRecordByte + DIR_REC_ATT_POS,X_MEMORY);
                wTempWord =  FSGetWord((void *)pstDirCtrlBlk->pwBuffer,StartRecordByte + DIR_REC_FIRST_CX_POS,X_MEMORY);

                if ( (bTempByte == 0x0f) && (wTempWord == 0x00) )
                    continue;

                // Check if file, sub dir or volume and increment specific counter
                if ( bTempByte & 0x10 )
                {
                    if ( GetFileCtrlBlk(RecordCounter, Sect, pstDirCtrlBlk, &stFileCtrlBlk) == !NOERROR )
                    {
                        HandleFailReadSector();
                        break;
                    }

                    // Flush last sector if needed
                    if ( pstDirCtrlBlk->Control == DIRTY )

                        if ( DriveWriteSector(pstDirCtrlBlk->Device,pstDirCtrlBlk->CurSect + DataDriveStatus[pstDirCtrlBlk->Device].iPbsSectorOffset,pstDirCtrlBlk->pwBuffer)!=NOERROR )
                        {
                            HandleFailReadSector();
                            break;
                        }


                        // Initialize the Directory Control Block Structure
                    pstDirCtrlBlk->wStartCxCurDir = stFileCtrlBlk.StartCluster;
                    pstDirCtrlBlk->StartSectCurDir = CxToSect(stFileCtrlBlk.StartCluster);
                    pstDirCtrlBlk->NumberFiles = 0;
                    pstDirCtrlBlk->Control = CLEAN;


                    // Scan the directory and if bad cluster chain delete it from the directory
                    // record
                    if ( ScanDirectory(pstDirCtrlBlk) != NOERROR )
                    {
                        if ( DeleteFileRecord(RecordCounter, Sect, pstDirCtrlBlk) != NOERROR )
                        {
                            HandleFailReadSector();
                            break;
                        }
                        else
                        {
                            Flag=1;
                            continue;       // go process the next sub directory
                        }
                    }


                    // Count Files in current directory
                    if ( ScanFilesAndSubDirs(pstDirCtrlBlk) != NOERROR )
                    {
                        HandleFailReadSector();
                        break;
                    }

                }

                else if ( !(bTempByte & 0x08) )
                {
                    if ( GetFileCtrlBlk(RecordCounter, Sect, pstDirCtrlBlk, &stFileCtrlBlk) != NOERROR )
                        return(!NOERROR);   // Can not read the sector
		        
                    /* To improve speed and make it better, we should check for the special case
                    where start cluster and/or size equals to 0 */

                    // Allow "placeholder" file entries to remain; these are empty files that exist, but are 0-length, with no cluster yet allocated.
                    if( (stFileCtrlBlk.StartCluster == 0) && (stFileCtrlBlk.Size == 0) )
                    {
                         // Do nothing. Skip over it
                         continue;
                    }

                    if( stFileCtrlBlk.StartCluster == 0 || stFileCtrlBlk.Size == 0 )
                    {
                        if ( DeleteFileRecord(RecordCounter, Sect, pstDirCtrlBlk) != NOERROR )
                            return(!NOERROR);   // Can not write to the sector
                        else
                        {
                            Flag=1;
                            continue;
                        }
                    }

                    // Count Length of file in clusters and check crosslink file and set bit for the file
                    else if ( (wClusterCount =  CheckCrossLinkFile(stFileCtrlBlk.StartCluster,pstDirCtrlBlk->Device)) < 0 )
                    {
                        if ( DeleteFileRecord(RecordCounter, Sect, pstDirCtrlBlk) != NOERROR )
                            return(!NOERROR);   // Can not write to the sector
                        else
                        {
                            Flag=1;
                            continue;
                        }

                    }

                    else
                    {
                        // First take care of the 0 file lenght special case
                        if ( wClusterCount != stFileCtrlBlk.Size )
                        {
                            // Calculates size of file on disk
                            dwFileSizeDiskBytes = wClusterCount * wCxSizeBytes;

                            // File size must be greater than size of (NumberCx - 1)
                            // and smaller than size of NumberCx
                            if ( (stFileCtrlBlk.Size > dwFileSizeDiskBytes) || (stFileCtrlBlk.Size < (dwFileSizeDiskBytes-wCxSizeBytes)) )
                            {

                                wCluster = stFileCtrlBlk.StartCluster;
                                for ( i = 0; i < wClusterCount ; i++ )
                                {
                                    UpdateBit(wCluster, FATEntryStatus,pstDirCtrlBlk->Device,stPartitionBootSector.TypeFileSystem,FREE_BIT);
                                    wCluster = GetNextCxFromFat(wCluster);
                                }
                                if ( DeleteFileRecord(RecordCounter,Sect, pstDirCtrlBlk) != NOERROR )
                                    return(!NOERROR);
                                else
                                {
                                    Flag=1;
                                    continue;
                                }
                            }
                        }
                    }

                } //else if ( !(bTempByte & 0x08) )

            } //for ( RecordCounter = 0; RecordCounter < DIR_REC_PER_SECTOR; RecordCounter++ )

        } //for ( n = 0 ; n < wLoopCount ; Sect++, n++, SectCounter++ )


        // For loop terminated because need another cluster
        // If no other cluster available or at the root => No more entries free
        // otherwise, keep searching directory records in the next cluster
        //	CurCx=0;
        if ( CurCx == 0 )
        {
            Flag=1;
            nestinglevel--;
            return(NOERROR); // CurCx = 0 => Current directory is the root
        }                 // Therefore no other clusters available
        // The root is full of directory entries
        n++;
        CurCx = GetNextCxFromFat(CurCx);
        //	Rootdircluster = CurCx;

        if ( CurCx <= 1 )          // Cx return is not valid. Error!
            return(!NOERROR);

        if ( IsLastCx(CurCx) )     // That's the end of it
        {
            Flag=1;
            nestinglevel--;
            return(NOERROR);
        }

        // stmp3686 solution: consider non-user-data 2 initial clx & non-user-data initial sectors. JN
        Sect = CxToSect(CurCx);
    }while ( 1 );
}


///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           GetFileCtrlBlk
//
//   Type:           Function
//
//   Description:    Fills the File Control Block structure for the specified record
//
//
//   Inputs:         bRecordNumber      : Record number in relation to the sector (0 to 31)
//                   wSectNumber        : Sector Number where record is located
//                   *pstDirCtrlBlk     : Pointer to Directory Ctrl Block Structure
//                   *pstFileCtrlBlk    : Pointer to the File Control Block structure to fill
//
//   Outputs:        Result of operation
//                     NOERROR -> Operation successful
//                     !NOERROR -> Impossible to read the sector
//
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant GetFileCtrlBlk(BYTE bRecordNumber, DWORD wSectNumber, DIR_CTRL_BLK *pstDirCtrlBlk, FILE_CTRL_BLK *pstFileCtrlBlk)
{
    WORD wStartRecByte;
    WORD clusterlo,clusterhi;
    if ( ReadDirSector(wSectNumber, pstDirCtrlBlk) != NOERROR )
        return(!NOERROR);

    // Calculates the position of record 1st byte
    wStartRecByte = bRecordNumber * BYTES_PER_DIR_RECORD;

    // Fills the File Contol Block Structure
    pstFileCtrlBlk->StartNameCharacter = FSGetByte((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte, X_MEMORY);
    pstFileCtrlBlk->Attribut = FSGetByte((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte + DIR_REC_ATT_POS,X_MEMORY);
    //   pstFileCtrlBlk->StartCluster =  FSGetWord((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte + DIR_REC_FIRST_CX_POS,X_MEMORY);
    pstFileCtrlBlk->Size = FSGetDWord((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte + DIR_REC_SIZE_POS,X_MEMORY);
    clusterlo =  FSGetWord((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte + DIR_REC_FIRST_CX_POS,X_MEMORY);
    clusterhi =  FSGetWord((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte + DIR_REC_FIRST_CX_HIGH_POS,X_MEMORY);
    pstFileCtrlBlk->StartCluster =  (DWORD)clusterlo +(((DWORD)clusterhi)<<16);
    return(NOERROR);
}




///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           DeleteFileRecord
//
//   Type:           Function
//
//   Description:    Deletes a file record and its associate long file name (if any)
//                   from the directory entries
//
//   Inputs:         bRecordNumber      : Record number in relation to the sector (0 to 31)
//                   wSectNumber        : Sector Number where record is located
//                   *pstDirCtrlBlk     : Pointer to Directory Ctrl Block Structure
//
//   Outputs:        Result of operation
//                     NOERROR -> Operation successful
//                     !NOERROR -> Impossible to read/write the sector
//
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant DeleteFileRecord(BYTE bRecordNumber, DWORD wSectNumber, DIR_CTRL_BLK *pstDirCtrlBlk)
{
    WORD wStartRecByte, wLoopCounter = 0;
    BYTE bCurRecord;
    DWORD  wCurSect;
    glb_wFileCorrupted++;

    // Read new sector
    if ( ReadDirSector(wSectNumber, pstDirCtrlBlk) != NOERROR )
        return(!NOERROR);

    // Calculates the position of record 1st byte
    wStartRecByte = bRecordNumber * BYTES_PER_DIR_RECORD;

    // Mark the file "deleted"
    //SetByteX(wStartRecByte, FILE_DELETED_CODE, pstDirCtrlBlk->pwBuffer);
    PutByte((void *)pstDirCtrlBlk->pwBuffer,FILE_DELETED_CODE,wStartRecByte,X_MEMORY);

    pstDirCtrlBlk->Control = DIRTY;

    // Chek if 1st record of the current directory
    if ( (bRecordNumber == 0) && (pstDirCtrlBlk->StartSectCurDir == wSectNumber) )
        return(NOERROR);            // we are done

    // Check if long file names and delete each long file name entry
    // specific to this file
    if ( bRecordNumber == 0 )
    {
        bCurRecord = DIR_REC_PER_SECTOR - 1;
        wCurSect = wSectNumber - 1;
    }
    else
    {
        bCurRecord = bRecordNumber - 1;
        wCurSect = wSectNumber;
    }

    do
    {
        BYTE bTempByte;
        WORD wTempWord;

        // Read new sector
        if ( ReadDirSector(wCurSect, pstDirCtrlBlk) != NOERROR )
            return(!NOERROR);

        // Calculates the position of record 1st byte
        wStartRecByte = bCurRecord * BYTES_PER_DIR_RECORD;

        // Check if record is part of long file name
        bTempByte = FSGetByte((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte + DIR_REC_ATT_POS,X_MEMORY);
        wTempWord =  FSGetWord((void *)pstDirCtrlBlk->pwBuffer,wStartRecByte + DIR_REC_FIRST_CX_POS,X_MEMORY);

        if ( (bTempByte == 0x0f) && (wTempWord == 0x00) )
        {
            // Mark the file "deleted"
            PutByte((void *)pstDirCtrlBlk->pwBuffer,FILE_DELETED_CODE,wStartRecByte, X_MEMORY);
            pstDirCtrlBlk->Control = DIRTY;
        }
        else
            return(NOERROR);

        // Decrement record number
        if ( bCurRecord == 0 )            // If last record in the sector, need a new sector
        {
            bCurRecord = DIR_REC_PER_SECTOR - 1;
            wCurSect--;
        }
        else
            bCurRecord--;

    }while ( wLoopCounter++ < MAX_ENTRIES_LONG_FILE_NAME );

    return(!NOERROR);       // If we arrive here this means that we fail to find
                            // the end of a long file name

}



///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           ReadDirSector
//
//   Type:           Function
//
//   Description:    Reads a sector in the directory data
//
//   Inputs:         wSectNumber        : Sector Number to read
//                   *pstDirCtrlBlk     : Pointer to Directory Ctrl Block Structure
//
//   Outputs:        Result of operation
//                     NOERROR -> Operation successful
//                     !NOERROR -> Impossible to read/write the sector
//
//   Notes:          none
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant ReadDirSector(DWORD wSectNumber, DIR_CTRL_BLK *pstDirCtrlBlk)
{

    // Reads directory sector in buffer
    if ( wSectNumber != pstDirCtrlBlk->CurSect )
    {
        // Check if need to save the current sector first
        if ( pstDirCtrlBlk->Control != CLEAN )
            if ( DriveWriteSector( pstDirCtrlBlk->Device,pstDirCtrlBlk->CurSect + DataDriveStatus[pstDirCtrlBlk->Device].iPbsSectorOffset,pstDirCtrlBlk->pwBuffer)!=NOERROR )
                return(!NOERROR);


            // Read new sector
        if ( DriveReadSector(pstDirCtrlBlk->Device, (DWORD)wSectNumber+DataDriveStatus[pstDirCtrlBlk->Device].iPbsSectorOffset, pstDirCtrlBlk->pwBuffer) != NOERROR )
        {
            return(!NOERROR);
        }


        // Update current sector loaded in Directory Control Structure
        pstDirCtrlBlk->CurSect = wSectNumber;
        pstDirCtrlBlk->Control = CLEAN;
    }

    return(NOERROR);
}




///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           CxToSect
//
//   Type:           Function
//
//   Description:    Calculates the 1st sector number for the Cluster passed
//
//
//   Inputs:         wCx                : Cluster number
//
//   Outputs:        1st sector number for wCx
//
//   Notes:
//          The logical sector 0 is the Partition boot sector for this media
//          The data area always starts at cluster #2
//          Therefore:
//          StartSect = ((wCx - 2)*SectPerCluster) + StartSectNumberForDataArea
//<
///////////////////////////////////////////////////////////////////////////////
DWORD _reentrant CxToSect(DWORD wCx)
{
    return(((wCx - 2) * stPartitionBootSector.bSectPerCx) + stPartitionBootSector.wStartSectData);
}





///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           HandleFailReadSector
//
//   Type:           Function
//
//   Description:    Records the fact that a sector could not be read/written
//                   on the media
//
//
//   Inputs:         None
//
//   Outputs:        None
//
//   Notes:
//          Failing to read/write a sector on the media is a critical error
//          that should terminate the chkdsk activity on the specific media
//<
///////////////////////////////////////////////////////////////////////////////
void _reentrant HandleFailReadSector(void)
{
    glb_bFailReadSect = TRUE;
}




//void InitTime(void)
//{
//    HW_DCLKCNTU.I = 0;
//    HW_DCLKCNTL.I = 0;
//}
//
//DWORD GetTime(void)
//{
//    return((((DWORD)(HW_DCLKCNTU.I))<<24) | ((DWORD)(HW_DCLKCNTL.I)));
//}
//
#pragma asm
    section Xscratch
    global    FXScratchSpace,FFatBuffer,FDirRecordBuffer
    org x,".XscratchSpace",scratch:

FXScratchSpace      ds      3540
FFatBuffer          ds      705
FDirRecordBuffer    ds      705

    endsec
#pragma endasm


