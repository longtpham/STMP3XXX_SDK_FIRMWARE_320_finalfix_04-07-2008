////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2005
//
// Filename: FatUtils.c
// Description: Utilities to work on the FAT
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "platform.h"
#include "chkdsk.h"
#include "regsswizzle.h"
#include "fatutils.h"
#include "cwrapfunc.h"
#include "extern.h"
#include "filesystem.h"

extern PARTITION_BOOT_SECTOR stPartitionBootSector;
extern FAT_STRUCT stFat;

// TOVERIFY made this ptr to SECTOR_BUFFER like the define was (instead of just extern WORD _X MediaBuffer). 
extern SECTOR_BUFFER * MediaBuffer;

extern WORD _X FatBuffer[];
extern tCACHEDESCR_checkdisk CacheDesc_chkdsk[MAX_CACHES];
extern WORD _X *CacheMem_chkdsk[MAX_CACHES];        // This table will be used for FAT32
extern WORD MaskArray[24];
void _reentrant IncrementCacheCounters_chkdsk(void);

extern RETCODE _reentrant DriveReadSector(WORD wLogDriveNumber, DWORD dwSectorNumber,
    P_SECTOR_BUFFER pSectorData);
extern RETCODE _reentrant DriveWriteSector(WORD wLogDriveNumber, DWORD dwSectorNumber,
    P_SECTOR_BUFFER pSectorData);

extern LONG (*GetNextCxFromFat) (DWORD wCurCx);     // pointer to function
extern LONG GetDWordY(void *buffer, INT Offset,INT MemoryType);
extern DATA_DRIVE_PBS_LOC DataDriveStatus[];



///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           LoadFatSector
//
//   Type:           Function
//
//   Description:    Loads 3 sectors of FAT into the FAT buffer specified by stFat structure
//                   
//
//   Inputs:         Sect               : Starting Sector
//                   *pwBuffer          : Pointer to buffer in X memory. This buffer 
//                                          is a temporarely buffer.
//
//   Outputs:        
//                     NOERROR -> Operation successful
//                     !NOERROR -> Impossible to read the sector 
//                   
//   Notes:          
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant LoadFatSector(DWORD Sect, WORD _X *pwBuffer)
{
   // if(Sect <= 0 || Sect > stPartitionBootSector.wNumberFatSectors)
     //   return(!NOERROR);
    
    if(Sect < stPartitionBootSector.wStartSectPrimaryFat || Sect > (stPartitionBootSector.wStartSectSecondaryFat-1))
	     return (!NOERROR);

    // Needs to check if already loaded
    if(Sect == stFat.FatSectorCached)
        return(NOERROR);
    
    // Needs to check if current group dirty and save fat if so
    if(stFat.Control != CLEAN)
        if(WriteFatSector(stFat.FatSectorCached, FatBuffer) != NOERROR)
            return(!NOERROR);
        
 
    /* Read the 1st Sector*/ 

   if(DriveReadSector(stFat.Device, (DWORD)Sect+DataDriveStatus[stFat.Device].iPbsSectorOffset, pwBuffer) != NOERROR)
        return(!NOERROR);
   
    
   // Get 1st Sector Fat Buffer using swizzle block
    HW_SWIZZLECS1R.I = 0x501;         // Manipulates data in memory and places it in memory
    HW_SWIZZLESIZER.U = cachedSectorSizeInWords;        // Data Size
    SetupSwizzleAddrXY(MediaBuffer, stFat.pwBuffer);

    HW_SWIZZLECS2R.I = 9;         // Kick it, Source in Xmem, Dest in Ymem
    
    // Wait for swizzle completed
    while(HW_SWIZZLECS2R.I & 0x1);
    

    
    if ( DriveReadSector(stFat.Device, (DWORD)(Sect+1)+DataDriveStatus[stFat.Device].iPbsSectorOffset, pwBuffer) != NOERROR )
         return(!NOERROR);
    

    // Get 2nd Sector Fat Buffer using swizzle block
    HW_SWIZZLECS1R.I = 0x501;         // Manipulates data in memory and places it in memory
    HW_SWIZZLESIZER.U = cachedSectorSizeInWords;        // Data Size

    SetupSwizzleAddrXY(pwBuffer, stFat.pwBuffer+cachedSectorSizeInWords-1);

    HW_SWIZZLECS2R.I = 0x4009;         // Kick it, Source in Xmem, Dest in Ymem

    // Wait for swizzle completed
    while ( HW_SWIZZLECS2R.I & 0x1 );

	
    if ( DriveReadSector(stFat.Device, (DWORD)(Sect+2)+DataDriveStatus[stFat.Device].iPbsSectorOffset, pwBuffer) != NOERROR )
       return(!NOERROR);
	
    // Get 3rd Sector Fat Buffer using swizzle block
    HW_SWIZZLECS1R.I = 0x501;         // Manipulates data in memory and places it in memory
    HW_SWIZZLESIZER.U = cachedSectorSizeInWords;        // Data Size

    SetupSwizzleAddrXY(pwBuffer, stFat.pwBuffer+cachedSectorSizeInWords+cachedSectorSizeInWords-1);

    HW_SWIZZLECS2R.I = 0x2009;         // Kick it, Source in Xmem, Dest in Ymem

    // Wait for swizzle completed
    while ( HW_SWIZZLECS2R.I & 0x1 );
    
    stFat.FatSectorCached = Sect;

    return(NOERROR);

}




///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           WriteFatSector
//
//   Type:           Function
//
//   Description:    Writes 3 sectors of FAT. Data to write is specified by stFat structure
//                   
//
//   Inputs:         Sect               : Starting Sector
//                   *pwBuffer          : Pointer to buffer in X memory. This buffer 
//                                          is a temporarely buffer.
//
//   Outputs:        
//                     NOERROR -> Operation successful
//                     !NOERROR -> Impossible to write the sector 
//                   
//   Notes:          
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant WriteFatSector(DWORD Sect, WORD _X *pwBuffer)
{
    WORD wTemp;
    DWORD wSectSecFat;

    // Get 1st Sector Fat Buffer using swizzle block
    HW_SWIZZLECS1R.I = 0x501;     // Manipulates data in memory and places it in memory
    HW_SWIZZLESIZER.U = cachedSectorSizeInWords;        // Data Size
    
    
    SetupSwizzleAddrYX(stFat.pwBuffer, pwBuffer);

    HW_SWIZZLECS2R.I = 3;         // Kick it, Source in Ymem, Dest in Xmem
    
    // Wait for swizzle completed
    while ( HW_SWIZZLECS2R.I & 0x1 );
    
    // Write 1st sector in Primary FAT
    if ( Sect < stPartitionBootSector.wStartSectSecondaryFat )
    {    
        if ( DriveWriteSector(stFat.Device, (DWORD)Sect+DataDriveStatus[stFat.Device].iPbsSectorOffset, pwBuffer) != NOERROR )
        	return(!NOERROR);
    }
    else
    {
        stFat.Control = CLEAN;            
        return(NOERROR);
    }        
                
    
    wSectSecFat = (Sect - stFat.FirstPrimaryFatSect) + stFat.FirstSecondaryFatSect;
     
    // Get 2nd Sector Fat Buffer using swizzle block
    HW_SWIZZLECS1R.I = 0x501;       // Manipulates data in memory and places it in memory
    HW_SWIZZLESIZER.U = cachedSectorSizeInWords+1;        // Data Size


    wTemp = *(pwBuffer-1);
    SetupSwizzleAddrYX(stFat.pwBuffer+cachedSectorSizeInWords-1, pwBuffer-1);

    HW_SWIZZLECS2R.I = 0x2003;         // Kick it, Source in Ymem, Dest in Xmem

    // Wait for swizzle completed
    while ( HW_SWIZZLECS2R.I & 0x1 );

    *(pwBuffer-1) = wTemp;

    if ( (Sect + 1) < stPartitionBootSector.wStartSectSecondaryFat )
    {    
       // Write 2nd sector
        if ( DriveWriteSector(stFat.Device, Sect+1+DataDriveStatus[stFat.Device].iPbsSectorOffset, pwBuffer) != NOERROR )
        	return(!NOERROR);
       
    }
    else
    {
        stFat.Control = CLEAN;            
        return(NOERROR);
    }        
    

    // Get 3rd Sector Fat Buffer using swizzle block
    HW_SWIZZLECS1R.I = 0x501;         // Manipulates data in memory and places it in memory
    HW_SWIZZLESIZER.U = cachedSectorSizeInWords+1;        // Data Size

    wTemp = *(pwBuffer-1);
    SetupSwizzleAddrYX(stFat.pwBuffer+cachedSectorSizeInWords+cachedSectorSizeInWords-1, pwBuffer-1);

    HW_SWIZZLECS2R.I = 0x4003;         // Kick it, Source in Xmem, Dest in Ymem

    // Wait for swizzle completed
    while ( HW_SWIZZLECS2R.I & 0x1 );
    
    *(pwBuffer-1) = wTemp;

    // Write 3rd sector
    if ( (Sect + 2) < stPartitionBootSector.wStartSectSecondaryFat )
    {    
        if ( DriveWriteSector(stFat.Device, Sect+2+DataDriveStatus[stFat.Device].iPbsSectorOffset, pwBuffer) != NOERROR )
        	return(!NOERROR);
     }
    else
    {
        stFat.Control = CLEAN;            
        return(NOERROR);
    }        

        stFat.Control = CLEAN;            

    return(NOERROR);

}



LONG _reentrant GetNextCxFat12(DWORD wCurCx)
{
    DWORD wStartSect;
    WORD   wOffCx;
    // Calculates start sector of group of 3 buffer sectors for FAT
    wStartSect = wCurCx / FAT12_ENTRIES_PER_SECT_GROUP;

    // Calculates the cluster offset in the Fat buffer
    wOffCx = wCurCx - (wStartSect*FAT12_ENTRIES_PER_SECT_GROUP);
    
    // Loads the fat sector group into fat buffer if needed
    // Each Fat buffer group is 3 sectors
    if ( ((wStartSect * 3)  + stFat.FirstPrimaryFatSect) != stFat.FatSectorCached )
        if ( LoadFatSector(((wStartSect * 3) + stFat.FirstPrimaryFatSect), MediaBuffer) != NOERROR )
            return(-1);
            
    return(FetchCxFat12(wOffCx));
}


LONG _reentrant GetNextCxFat16(DWORD wCurCx)
{
    DWORD wStartSect;
    WORD   wOffCx;
    // Calculates start sector of group of 3 buffer sectors for FAT
    wStartSect = wCurCx / FAT16_ENTRIES_PER_SECT_GROUP;

    // Calculates the cluster offset in the Fat buffer
    wOffCx = wCurCx - (wStartSect*FAT16_ENTRIES_PER_SECT_GROUP);
    
    // Loads the fat sector group into fat buffer if needed
    // Each Fat buffer group is 3 sectors
    if ( ((wStartSect * 3) + stFat.FirstPrimaryFatSect) != stFat.FatSectorCached )
        if ( LoadFatSector(((wStartSect * 3) + stFat.FirstPrimaryFatSect), MediaBuffer) != NOERROR )
            return(-1);
            
    return(FSGetWord(stFat.pwBuffer,(wOffCx*2), Y_MEMORY));

}        


LONG _reentrant GetNextCxFat32(DWORD wCurCx)
{
   WORD  wOffCx;
   DWORD  wStartSect;
    
    // Calculates start sector of group of 3 buffer sectors for FAT
//    wStartSect = Getsectorno(wCurCx);
    wStartSect = wCurCx / FAT32_ENTRIES_PER_SECT_GROUP;
    // Calculates the cluster offset in the Fat buffer
    wOffCx = wCurCx - (wStartSect*FAT32_ENTRIES_PER_SECT_GROUP);
    
    // Loads the fat sector group into fat buffer if needed
    // Each Fat buffer group is 3 sectors
    if ( ((wStartSect * 3) + stFat.FirstPrimaryFatSect) != stFat.FatSectorCached )
        if ( LoadFatSector(((wStartSect * 3) + stFat.FirstPrimaryFatSect), MediaBuffer) != NOERROR )
            return(-1);
            
    return(GetDWordY(stFat.pwBuffer,(wOffCx*4), Y_MEMORY));

}        



///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           FetchCxFat12
//
//   Type:           Function
//
//   Description:    Fetches a FAT entry from the current FAT buffer
//                   
//
//   Inputs:         wCurCx             : Fat Entry to fetch
//
//   Outputs:        
//                     contents of FAT entry
//                   
//   Notes:          
//                  The FAT entry wCurCx is referenced to the FAT sector 
//                  buffered. Use GetNextCxFat12() if FAT entry is an
//                  absolute number. 
//<
///////////////////////////////////////////////////////////////////////////////
LONG _reentrant FetchCxFat12(DWORD wCurCx)
{
    DWORD    wTemp = wCurCx/2;
    WORD    wResult=0;
    
    if ( wTemp*2 != wCurCx )
        // Fecth MS 12-bit word from entry
        return((LONG)(*(stFat.pwBuffer + wTemp) >> 12));
    else
        // Fecth LS 12-bit word from entry
        return((LONG)(*(stFat.pwBuffer +  wTemp) & 0x000fff));
}                
        




///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           FreeCxFat
//
//   Type:           Function
//
//   Description:    Frees the specified FAT entry.
//                   
//
//   Inputs:         wFatEntry  : Fat Entry to free up
//
//   Outputs:        
//                     NOERROR -> Operation successful
//                     !NOERROR -> File System FAT not supported
//                              or FAT outside of boundaries
//                              or impossible to read a sector
//                   
//   Notes:          
//                  Depending of the file system FAT, this function calls 
//                      FreeCxFat12 or FreeCxFat16
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant FreeCxFat(DWORD wFatEntry)
{
    BOOL (*FreeCxFromFat) (DWORD wCurCx);     // pointer to function
    LONG wFatEntryValue; 
    
    // Read content of Fat Entry
    wFatEntryValue = GetNextCxFromFat(wFatEntry);

    // If -1, the FAT is not supported => catastrophic
    if ( wFatEntryValue == -1 )
        return(!NOERROR);
    
    // If 0, the FAT entry is already free
    if ( wFatEntryValue == 0x0 )
        return(NOERROR);        
    
    if ( stPartitionBootSector.TypeFileSystem == FS_FAT12 )
        FreeCxFromFat = &FreeCxFat12;
        
    else if(stPartitionBootSector.TypeFileSystem == FS_FAT16)
        FreeCxFromFat = &FreeCxFat16;
 
    else if ( stPartitionBootSector.TypeFileSystem == FS_FAT32 )
        FreeCxFromFat = &FreeCxFat32;
 
    else
        return(!NOERROR);
        
    return((*FreeCxFromFat)(wFatEntry));            

}


BOOL _reentrant FreeCxFat12(DWORD wCluster)
{
    DWORD wStartSect, wOffCx, wTemp;
    
    // Calculates start sector of group of 3 buffer sectors for FAT
    wStartSect = wCluster / FAT12_ENTRIES_PER_SECT_GROUP;

    // Calculates the cluster offset in the Fat buffer
    wOffCx = wCluster - (wStartSect*FAT12_ENTRIES_PER_SECT_GROUP);

    // Loads the fat sector group into fat buffer if needed
    // Each Fat buffer group is 3 sectors
    if ( ((wStartSect * 3)  + stFat.FirstPrimaryFatSect) != stFat.FatSectorCached )
        if ( LoadFatSector(((wStartSect * 3) + stFat.FirstPrimaryFatSect), MediaBuffer) != NOERROR )
            return(!NOERROR);

    wTemp = wOffCx/2;
    
    if ( wTemp*2 != wOffCx )
        // Cluster to free is in MS 12-bit word from entry
        *(stFat.pwBuffer + wTemp) = *(stFat.pwBuffer + wTemp) & 0x000fff;
    else
        // Cluster to free is in LS 12-bit word from entry
        *(stFat.pwBuffer + wTemp) = *(stFat.pwBuffer + wTemp) & 0xfff000;
        
    // Mark the FAT sector group dirty
    stFat.Control = DIRTY;
    
    return(NOERROR);        
}



BOOL _reentrant FreeCxFat16(DWORD wCluster)
{
    DWORD wStartSect, wOffCx;
    
    // Calculates start sector of group of 3 buffer sectors for FAT
    wStartSect = wCluster / FAT16_ENTRIES_PER_SECT_GROUP;

    // Calculates the cluster offset in the Fat buffer
    wOffCx = wCluster - (wStartSect*FAT16_ENTRIES_PER_SECT_GROUP);
    
    // Loads the fat sector group into fat buffer if needed
    // Each Fat buffer group is 3 sectors
    if ( ((wStartSect * 3) + stFat.FirstPrimaryFatSect) != stFat.FatSectorCached )
        if ( LoadFatSector(((wStartSect * 3) + stFat.FirstPrimaryFatSect), MediaBuffer) != NOERROR )
            return(!NOERROR);

    PutWord(stFat.pwBuffer,0x0000,(wOffCx*2),DEFAULT_MEMORY);
    
    // Mark the FAT sector group dirty
    stFat.Control = DIRTY;

    return(NOERROR);

}

BOOL _reentrant FreeCxFat32(DWORD wCluster)
{
    DWORD wStartSect ;
    INT wOffCx;
    // Calculates start sector of group of 3 buffer sectors for FAT
//    wStartSect = Getsectorno(wCluster);
    wStartSect = wCluster / FAT32_ENTRIES_PER_SECT_GROUP;

    // Calculates the cluster offset in the Fat buffer
    wOffCx = wCluster - (wStartSect*FAT32_ENTRIES_PER_SECT_GROUP);
    
    // Loads the fat sector group into fat buffer if needed
    // Each Fat buffer group is 3 sectors
    if ( ((wStartSect * 3) + stFat.FirstPrimaryFatSect) != stFat.FatSectorCached )
        if ( LoadFatSector(((wStartSect * 3) + stFat.FirstPrimaryFatSect), MediaBuffer) != NOERROR )
            return(!NOERROR);

            PutDword(stFat.pwBuffer,0x0000,(wOffCx*4),DEFAULT_MEMORY);
    
    // Mark the FAT sector group dirty
    stFat.Control = DIRTY;

    return(NOERROR);

}





///////////////////////////////////////////////////////////////////////////////
//
//>  Name:           IsLastCx
//
//   Type:           Function
//
//   Description:    Checks if cluster is last of the chain
//                      The chain ends with a cluster value of 0xfff (FAT12) 
//                      or 0xffff (FAT16)
//
//   Inputs:         wCluster             : Cluster 
//
//   Outputs:        
//                      TRUE, if last cluster
//                      FALSE, otherwise.
//                   
//   Notes:          
//                  none.
//<
///////////////////////////////////////////////////////////////////////////////
BOOL _reentrant IsLastCx(DWORD wCluster)
{
    if ( stPartitionBootSector.TypeFileSystem == FS_FAT12 )
        return((wCluster==0x000fff ? TRUE : FALSE));
   
    else if ( stPartitionBootSector.TypeFileSystem == FS_FAT16 )
        return((wCluster==0x00ffff ? TRUE : FALSE));
    
   
    // Must be FAT32
    return((wCluster==0x0fffffff ? TRUE : FALSE));
}

 





INT _reentrant FAT32_UpdateBit(INT DeviceNum, LONG sectorNumber, WORD wOffsetToWord, WORD wOffsetBit,WORD Bittype)
{
	INT i=0,selection,Counter,index;
	WORD temp;
    // First Search if the buffer is available in the cache

	index = SearchmatchingSector(sectorNumber,MAX_CACHES, &CacheDesc_chkdsk[0]);
    if ( index>=0 )
		{
        switch ( Bittype )
		{
			case GET_BIT:
	            CacheDesc_chkdsk[index].CacheCounter = READCOUNTER;
            return(CacheMem_chkdsk[index][wOffsetToWord] & MaskArray[wOffsetBit]);
			case SET_BIT:
	            CacheDesc_chkdsk[index].CacheCounter = WRITECOUNTER;
			    CacheDesc_chkdsk[index].WriteAttribute = 1;
				temp = (CacheMem_chkdsk[index][wOffsetToWord] & MaskArray[wOffsetBit]);
	            CacheMem_chkdsk[index][wOffsetToWord] = CacheMem_chkdsk[index][wOffsetToWord] | MaskArray[wOffsetBit];                
				return temp;
			case FREE_BIT:
	            CacheDesc_chkdsk[index].CacheCounter = WRITECOUNTER;
			    CacheDesc_chkdsk[index].WriteAttribute = 1;
				temp = 0xffffff ^ MaskArray[wOffsetBit];
				CacheMem_chkdsk[index][wOffsetToWord] = CacheMem_chkdsk[index][wOffsetToWord] & temp;
				return 0;						
		}
	}
					
	// Cache Miss, so must read.
	// Now find the Least recently used Buffer
	selection = 0;
	Counter = 0;
    for ( i = 0; i < MAX_CACHES; i++ )
	{
        if ( CacheDesc_chkdsk[i].CacheValid )
		{
            if ( CacheDesc_chkdsk[i].CacheCounter > Counter )
			{
				selection = i;
				Counter = CacheDesc_chkdsk[i].CacheCounter;
			}
		}
		else
		{
			selection = i;
			CacheDesc_chkdsk[selection].CacheValid = 1;
			break;
		}
	}
	// Flush the sector to the disk, if write attribute was set.
    if ( CacheDesc_chkdsk[selection].WriteAttribute == 1 )
	{
        if ( (MediaWrite(DeviceNum, CacheDesc_chkdsk[selection].SectorNumber,(int _X *)CacheMem_chkdsk[selection],0)) <0 )
			return -1;
	}
    if ( (MediaRead(DeviceNum, sectorNumber, (int _X *)CacheMem_chkdsk[selection],0)) <0 )
			return -1;
		
	CacheDesc_chkdsk[selection].SectorNumber = sectorNumber;
	IncrementCacheCounters_chkdsk();
    switch ( Bittype )
		{
			case GET_BIT:
	            CacheDesc_chkdsk[selection].CacheCounter = READCOUNTER;
        return(CacheMem_chkdsk[selection][wOffsetToWord] & MaskArray[wOffsetBit]);
			case SET_BIT:
	            CacheDesc_chkdsk[selection].CacheCounter = WRITECOUNTER;
			    CacheDesc_chkdsk[selection].WriteAttribute = 1;
				temp = (CacheMem_chkdsk[selection][wOffsetToWord] & MaskArray[wOffsetBit]);
	            CacheMem_chkdsk[selection][wOffsetToWord] = CacheMem_chkdsk[selection][wOffsetToWord] | MaskArray[wOffsetBit];                
				return temp;
			case FREE_BIT:
	            CacheDesc_chkdsk[selection].CacheCounter = WRITECOUNTER;
			    CacheDesc_chkdsk[selection].WriteAttribute = 1;
				temp = 0xffffff ^ MaskArray[wOffsetBit];
				CacheMem_chkdsk[selection][wOffsetToWord] = CacheMem_chkdsk[selection][wOffsetToWord] & temp;
				return 0;						

		}
	return 0;
}






void _reentrant IncrementCacheCounters_chkdsk(void)
{
    INT i;
    for ( i = 0; i< MAX_CACHES;i++ )
        (CacheDesc_chkdsk[i].CacheCounter)++;
}











#pragma asm

;************************************************************************
;  Include files
;************************************************************************
; Use relative paths only
    include "sysmacro.asm"
	include "const.xref" 
;************************************************************************
;	Global Variables
;************************************************************************

	
    org y,".yStmpSearchmatchingSector":

CACHEDECS_SECTORNO  equ   1

;************************************************************************
;  Code start
;************************************************************************
	org p,".pStmpSearchmatchingSector":

;////////////////////////////////////////////////////////////////////////
;   Name:           SearchmatchingSector
;   Type:           Function
;   Inputs:         A =  sector number
;					ro = Cache descriptor address
;   Output:         index 
;   Registers Trashed : Assume all except r7
;   Notes:          
;////////////////////////////////////////////////////////////////////////

FSearchmatchingSector
SearchmatchingSector
    push ssh
    move #>CACHEDECS_SECTORNO,n0
    move  A0,y0
	lua   (r0)+n0,r4
	clr   A		#>5,n4
	move n4,n0 
	nop
	move (r0)-n0
    do B,_END
        move y:(r4)+n4,B 
        cmp y0,B	(r0)+n0   
		jne _next
		move y:(r0),B
		tst B
		jeq _next
		Enddo

		jmp _EXIT

_next	 
		inc  A

_END
	move #>-1,A0

_EXIT
	pop ssh
	move A0,A
    rts

             
  
#pragma endasm   
          





//extern INT MediaRead(INT deviceNumber, LONG sectorNumber,int _X *readbuf,INT SectorType);


