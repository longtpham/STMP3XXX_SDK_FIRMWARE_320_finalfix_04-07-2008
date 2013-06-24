////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2006
//
// File : SysResource.c
// Description : Functions for System Resource
////////////////////////////////////////////////////////////////////////////////
#include "SysResourceApi.h"
#include "SysResource.h"
#include "ddildl_internal.h"
#include "stringlib.h"
#include "persist_bits.h" 
#ifndef   USBMSC_BUILD //do restore for both player and MTP
#include "RestoreDriveFromBackup.h"
#endif
#ifdef PLAYER_STRESS
extern _asmfunc void SysSaveSettings(void);
extern INT _Y g_iSysSaveSettingsDone;
#endif

extern void SysDelay(WORD);
extern WORD g_wNumDrives;
extern WORD g_wResourceTag;
extern WORD wResourceTag;

// customizible array size MAX_LOGICAL_DRIVES can stay since this file is not in a plib
extern LOGICAL_DRIVE g_Drive[MAX_LOGICAL_DRIVES];
SECTOR_BUFFER pSectorBuffer[SECTOR_BUFFER_SIZE];

Struct_Buffer_Sector_Desc SectorDesc = {0x7fffffffffff, pSectorBuffer};

WORD    wResourceDrive = 0;         // Initialize to 0. A resource drive can not be zero
                                    // (0 must be a data drive), thus a zero value
                                    // will indicate a non-initialized drive number


WORD g_ResourceRead = 0;
short g_ResourceByteIndex;
short g_ResourceByteLength;
WORD g_wDirtyDrive = 0;

RETCODE _reentrant SysSetIrqLevel(WORD wIrqLevel);
WORD _reentrant SysGetIrqLevel(void);
WORD  DivideByThree(WORD wVal, WORD _Y *pRemainder);
void SystemReboot(void);

// Function: ReadResource
// Inputs: 
// Returns:
// Statics or other dependancies:
// Description: Use DriveReadSector to Read a resource from the resource drive. 
// May 2006 enhancement JLN: If read fails (as with read disturbances), switch 
// to read any future resources from extra copy of resource drive. 
// Future ver should also erase and rewrite original resource drive so we can use it later too.
//
#if DEBUG
int g_ForceError = 0; // during debug force it on, or when refresh requested by err handler. Usually set this to 0 at init. 
int g_NumErrors = 0;
DWORD g_dwResourceReadCounter=0;
#endif

static RETCODE _reentrant ReadResource(DWORD dwSectorNum, void * pwDestBuffer, 
                    WORD wNumWordsToCopy, WORD wStartByteSector, WORD wMemSpace )
{
    RETCODE ret = SUCCESS;
    DWORD physSectorNum;
    short tmp;
    WORD _X *pSB = pSectorBuffer;
    short numLogSectInPhys = (g_Drive[wResourceDrive].pDescriptor->wSectorSizeInBytes>>9); // WAS: / SECTOR_DATA_SIZE_IN_BYTES);

    physSectorNum = dwSectorNum;

    // possible values are 1 and 4
    if ( 1 < numLogSectInPhys )
        physSectorNum >>= 2;

    tmp = (dwSectorNum&(numLogSectInPhys-1));

#if DEBUG
    if (g_ForceError > 0) g_ForceError++;
#endif

#ifdef READ_ONLY_THE_RESOURCE

    g_ResourceRead = tmp+1;
    
    g_ResourceByteIndex = wStartByteSector;
    g_ResourceByteLength = wNumWordsToCopy * 3;

    ret = DriveReadSector(wResourceDrive,physSectorNum,pSectorBuffer);
    g_ResourceRead = 0;

    CopyWords( wNumWordsToCopy, pwDestBuffer, pSectorBuffer, wMemSpace );

#else   //  Read a partial sector for large sectors(512 bytes out of 2048)

    if ( dwSectorNum != SectorDesc.dwSectNumber )
    {
        // Update cached sector
        SectorDesc.dwSectNumber = dwSectorNum;

        g_ResourceRead = tmp+1;

        g_ResourceByteIndex = 0;
        g_ResourceByteLength = 512;

        ret = DriveReadSector(wResourceDrive,physSectorNum,pSectorBuffer);
//        if(ret != SUCCESS) SystemHalt();
        //g_ResourceRead = 0;   // Move to after Ping pong complete

#ifndef USBMSC_BUILD
        if((ret != SUCCESS) 
#if DEBUG
            || (g_ForceError > 10000)
#endif
            )
        {   
            g_bEraseComplete = FALSE;   // Clear the g_bEraseComplete flag so that encoder ISR know to call 
                                        // NAND erase function
            // Only like to swap the drive if the sticky bit is not set
            if(!g_bResourceGetDirty)    // We check the global instead of the sticky bit in case sticky bit
                                        // hasn't gone through
            {
               
#if DEBUG
    g_ForceError=0;
    g_NumErrors++;
#endif
               // Only toggle system drive if the drive dirty, so it's the place to log the dirty drive number
                g_wDirtyDrive = ToggleSystemDrive();
                MarkSystemDriveDirty();
                // Retry the read using the new system drive.
                numLogSectInPhys = (g_Drive[wResourceDrive].pDescriptor->wSectorSizeInBytes>>9); 
                physSectorNum = dwSectorNum;

                // possible values are 1 and 4
                if ( 1 < numLogSectInPhys )
                    physSectorNum >>= 2;

                tmp = (dwSectorNum&(numLogSectInPhys-1));

                // Update cached sector
                SectorDesc.dwSectNumber = dwSectorNum;

                g_ResourceRead = tmp+1;

                g_ResourceByteIndex = 0;
                g_ResourceByteLength = 512;
                
                // Switch resource and read resource again   
                ret = DriveReadSector(wResourceDrive,physSectorNum,pSectorBuffer);        
                // Both resource.bin corrupted, really bad we have to reboot the system
                if(ret != SUCCESS)  // Both resource.bin go bad, then no choice besides reboot
                {
                    DebugBuildAssert(0);                      
                    SystemReboot();    
                }
            }
            else
            {
                DebugBuildAssert(0);
                SystemReboot();     
            }
            
        }
        // ************** Original resource drive is refreshed. Continue using 2nd version of resource drive for this session
#endif  // #ifdef USBMSC_BUILD
       g_ResourceRead = 0;
       
    }

    CopyWords( wNumWordsToCopy, pwDestBuffer, (WORD SECTOR_BUFFER_MEM *)(pSectorBuffer + (wStartByteSector/3)), wMemSpace );

#endif     // #ifdef READ_ONLY_THE_RESOURCE 

    return ret;
}

void SystemReboot(void)
{
    int iCount;
    int iTemp;
    iCount=50000;
    //Ensures persistant bit is cleared before we exit so we won't have chance of doing restore on each boot. 
    // WARNING: SysDelay make sure this does not rely on funclets to init
    //SysDelay(500);  // some delay here is required so sticky bit get the new value before rebooting. 500 msec works here. 
    while((iCount--) >0) 
    {    
        iTemp = iCount*5;   // delay before reboot
    }

#pragma asm
	move    #985040,b1
	move    b1,x:$FA01 ;HW_RCR
#pragma endasm
	while(1) ;
}
///////////////////////////////////////////////////////////////////////////////
//> Name: ResourceFileRead
//
//  Type: Function
//
//  Description: Reads number of bytes from the resource drive to the specified
//               buffer
//
//  Inputs:     dwStartPos      First Byte Position
//              wNumBytesToRead Number of Bytes to Read
//              pwBuffer        Pointer to Destination Buffer
//              wMemSpace       Memory Space of Destination Buffer
//
//  Outputs:     SUCCESS or error code.
//
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant ResourceFileRead(DWORD dwStartPos, WORD wNumBytesToRead, void * pwBuffer, WORD wMemSpace)
{
    WORD    wNumberBytesCopied = 0, wStartByteSector, wCachedBytes, wNumBytesToCopy;
    WORD    wTag, i, wNumWordsToCopy = 0;
    DWORD   dwStartSector;
    void * pwDestBuffer = pwBuffer;


    // Discover the drive number for the resources
    // The media number must be 0 (boot media)
    if(wResourceDrive == 0)
    {   for(i=0;i<g_wNumDrives;i++)
        {   if(g_Drive[i].pDescriptor->wTag == wResourceTag)
            {   wResourceDrive = i;
                break;
            }
        }

        // If Drive Resource number is 0, halt the system
        if(!wResourceDrive)
        {   // continues in retail build, halts in debug build.
            DebugBuildAssert(0);
        }

    }

    // Calculates the Starting Sector
    dwStartSector = dwStartPos / SECTOR_DATA_SIZE_IN_BYTES;
    
    // Calculates the starting byte on the sector
    wStartByteSector = dwStartPos - (dwStartSector*SECTOR_DATA_SIZE_IN_BYTES);
    
    // Calculates total bytes pre-cached
    wCachedBytes = SECTOR_DATA_SIZE_IN_BYTES - wStartByteSector;

    // Process new sector
    do
    {
        int iError ;
        WORD wOldInterrupt; 

        if(wCachedBytes >= wNumBytesToRead)
            wNumBytesToCopy = wNumBytesToRead;
        else
            wNumBytesToCopy = wCachedBytes;


        //wNumWordsToCopy = (wNumBytesToCopy+2) / 3;
        wNumWordsToCopy = DivideByThree((wNumBytesToCopy+2), (WORD*)&iError);

        wOldInterrupt = SysGetIrqLevel();
        SysSetIrqLevel(0x0100);

        iError = ReadResource(dwStartSector, pwDestBuffer, wNumWordsToCopy, wStartByteSector, wMemSpace);
                    
        SysSetIrqLevel(wOldInterrupt );

        if(iError != SUCCESS)
            return((int)RSC_ERROR_READING_RSC);

        // Update number bytes to read
        wNumBytesToRead -= wNumBytesToCopy;
        (WORD *)pwDestBuffer += wNumWordsToCopy;

        // Update the starting byte sector to start of sector
        wStartByteSector = 0;
            
        // Update Number of cached bytes to the full sector
        wCachedBytes = SECTOR_DATA_SIZE_IN_BYTES;
    
        dwStartSector++;

    }while(wNumBytesToRead > 0);        
     
    return((int)SUCCESS);

}

///////////////////////////////////////////////////////////////////////////////
//> Name: CopyWords
//
//  Type: Function
//
//  Description: This function copies the number of words specified
//
//  Inputs:     wNumWordsToCopy Number of words to copy
//              pDest           Pointer to Destination Buffer
//              pSrc            Pointer to Source Buffer
//              wMemSpaceDest   Memory Space of the Destination Buffer
//
//  Outputs:     SUCCESS or error code.
//
//  Notes:      This function handles spaces X, Y or P for the destination buffer
//              The source buffer space is known at compile time
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant CopyWords(WORD wNumWordsToCopy, void * pDest, WORD SECTOR_BUFFER_MEM * pSrc, 
                        WORD wMemSpaceDest)
{
    RETCODE Status = SUCCESS;

    if(wMemSpaceDest == TARGET_MEM_P)
    {   CopyWordsFromSectorBufToP(wNumWordsToCopy, (WORD _P *)pDest, pSrc);
    }
    else if(wMemSpaceDest == TARGET_MEM_Y)
    {   CopyWordsFromSectorBufToY(wNumWordsToCopy, (WORD _Y *)pDest, pSrc);
    }
    else if(wMemSpaceDest == TARGET_MEM_X)
    {   CopyWordsFromSectorBufToX(wNumWordsToCopy, (WORD _X *)pDest, pSrc);
    }
    else
    {   Status = (int) RSC_INVALID_MEM_SPACE;
    }
    return(Status);
}    




///////////////////////////////////////////////////////////////////////////////
//> Name: CloseResourceDrive
//
//  Type: Function
//  Description:    Closes the current resource drive. The next call to read 
//                  resource will force an initialization.
//  Inputs:         None
//
//  Outputs:        None
//
///////////////////////////////////////////////////////////////////////////////
void _reentrant CloseResourceDrive(void)
{

    wResourceDrive = 0;         // Initialize to 0. A resource drive can not be zero
                                // (0 must be a data drive), thus a zero value
                                // will indicate a non-initialized drive number
                                
}                                



#define COPYBUFF_USE_ASSEMBLY 1
#ifdef COPYBUFF_USE_ASSEMBLY
#pragma asm

    include "sysmacro.asm"
        org p,".ptextsysresource":
FCopyWordsFromSectorBufToY:
    tst a
    jeq _exit
    do a,_exit
        move x:(r4)+,a
        move a1,y:(r0)+
_exit
    rts



FCopyWordsFromSectorBufToX:
    tst a
    jeq _exit
    do a,_exit
        move x:(r4)+,a
        move a1,x:(r0)+
_exit
    rts

FCopyWordsFromSectorBufToP:
    tst a
    jeq _exit
    do a,_exit
        move x:(r4)+,a
        move a1,p:(r0)+
_exit
    rts

#pragma endasm
#else

///////////////////////////////////////////////////////////////////////////////
//> Name: CopyWordsFromSectorBufToY
//
//  Type: Function
//
//  Description: This function copies the number of words specified
//               from a buffer declared in Y to a buffer declared in Y
//
//  Inputs:     wNumWordsToCopy Number of words to copy
//              pDest           Pointer to Destination Buffer
//              pSrc            Pointer to Source Buffer
//
//  Outputs:    None
//
//  Notes:      
//              
///////////////////////////////////////////////////////////////////////////////
void _reentrant CopyWordsFromSectorBufToY(WORD wNumWordsToCopy, WORD _Y * pDest,
                    WORD SECTOR_BUFFER_MEM * pSrc)
{
    INT i;

    for(i = 0 ; i < wNumWordsToCopy ; i++)
    {
        pDest[i] = pSrc[i]; 
    }
}



///////////////////////////////////////////////////////////////////////////////
//> Name: CopyWordsFromSectorBufToX
//
//  Type: Function
//
//  Description: This function copies the number of words specified
//               from a buffer declared in Y to a buffer declared in X
//
//  Inputs:     wNumWordsToCopy Number of words to copy
//              pDest           Pointer to Destination Buffer
//              pSrc            Pointer to Source Buffer
//
//  Outputs:    None
//
//  Notes:      
//              
///////////////////////////////////////////////////////////////////////////////
void _reentrant CopyWordsFromSectorBufToX(WORD wNumWordsToCopy, WORD _X * pDest,
                    WORD SECTOR_BUFFER_MEM * pSrc)
{
    INT i;

    for(i = 0 ; i < wNumWordsToCopy ; i++)
    {
        pDest[i] = pSrc[i]; 
    }
}



///////////////////////////////////////////////////////////////////////////////
//> Name: CopyWordsFromSectorBufToP
//
//  Type: Function
//
//  Description: This function copies the number of words specified
//               from a buffer declared in Y to a buffer declared in P
//
//  Inputs:     wNumWordsToCopy Number of words to copy
//              pDest           Pointer to Destination Buffer
//              pSrc            Pointer to Source Buffer
//
//  Outputs:    None
//
//  Notes:      
//              
///////////////////////////////////////////////////////////////////////////////
void _reentrant CopyWordsFromSectorBufToP(WORD wNumWordsToCopy, WORD _P * pDest,
                    WORD SECTOR_BUFFER_MEM * pSrc)
{
    INT i;

    for(i = 0 ; i < wNumWordsToCopy ; i++)
    {
        pDest[i] = pSrc[i]; 
    }
}

#endif













