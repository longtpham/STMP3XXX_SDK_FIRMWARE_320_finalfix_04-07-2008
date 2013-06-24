////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2006
//
// File : SysResourceApi.c
// Description : API functions for System Resource
//               SysOpenResource
//               SysCloseResource
//               SysLoadResource
//               SysResourceFileRead
//               SysResourceFileSeek
//               SysResourceFileTell
//               SysSetResourceTag
//               SysGetResourceTag not implemented
////////////////////////////////////////////////////////////////////////////////

#include "SysResourceApi.h"
#include "SysResource.h"
#include "types.h" // SystemHalt, DebugBuildAssert, Assert

// Run if wait time elapsed
#define EVENT_TIMER 0x000004

// Added Dec25 '05 in 2.610 pre rel since only player's PLIBS build had this defined from player.mk.
// Having this defined in buildlibs.bat's generic_libs.mk file case prevented STFM record failure on record press.
//#define FULL_PLAYER_KERNEL TRUE


#ifdef FULL_PLAYER_KERNEL
int _asmfunc SysWaitOnEvent(unsigned int uEvent,struct CMessage *,int uLength);
int GetSemaphore0(void);
int ReleaseSemaphore0(void);
#endif

_asmfunc SysAllLinear(void);

Struct_Handle_Nested_Rsc NestedRscHandle[MAX_NESTED_RSC] = {
            {0x00, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00},
            {FREE_RSC_HANDLE, 0x00, 0x00}
        };


WORD    wResourceTag = DRIVE_TAG_RESOURCE_BIN;

extern void EnterNonReentrantSection(void);
extern void LeaveNonReentrantSection(void);

///////////////////////////////////////////////////////////////////////////////
//> Name: SysOpenResource
//
//  Type: Function
//
//  Description: Open the specified "nested" resource file.
// (Move the resource file
//  Inputs:      wRscNum                    Nested Resource number
//               btHandleNestedParentRsc    Nested Parent Resource Handle
//                                          (0 if Parent is the root)
//
//  Outputs:     Handle or error code.
//
//  Notes:
//  The origin of the resource is set to the begin of the nested resource
//  header (Resource header includes 3 words which are the resource number,
//  the resource size in bytes and the resource header)
//
//      This function is not reentrant. It uses global variables
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysOpenResource(WORD wRscNum, BYTE btHandleNestedParentRsc)
{

    BYTE btRscHandle = 0;
    WORD wSeekPosition = 0;
    INT i;
    WORD pLocalBuffer[3];

    EnterNonReentrantSection();

    // Find Free Handle
    for(i=0 ; i<MAX_NESTED_RSC ; i++)
    {
        if(NestedRscHandle[i].wRscNumber == FREE_RSC_HANDLE)
        {
            btRscHandle = i;
            break;
        }
    }

    if(btRscHandle)
    {   // Parent Resource is nested
        if(btHandleNestedParentRsc)
        {
            if(btHandleNestedParentRsc < MAX_NESTED_RSC)
            {
                if(NestedRscHandle[btHandleNestedParentRsc].wRscNumber != FREE_RSC_HANDLE)
                {
                    // Seek to begin of Parent Nested Resource
                    wSeekPosition = NestedRscHandle[btHandleNestedParentRsc].wStartPosition;
                    wSeekPosition += 9;         // Bypass the 9 resource header bytes
                }
                else    {
                    LeaveNonReentrantSection();
                     return((int)RSC_INVALID_NESTED_NOT_ITIALIZED);
                }
            }
            else    {
                LeaveNonReentrantSection();
                return((int)RSC_INVALID_NESTED_HANDLE);
            }
        }
        else
        {   // Parent resource is root
        }

        // We need to seek to the location where the resource offset is defined
        // in the parent resource header.
        wSeekPosition += wRscNum * 6;  // index entry length in bytes is 6
        NestedRscHandle[btRscHandle].wCurrentPosition = wSeekPosition;

        // Reads the absolute start position for the nested resource we want to open
        ResourceFileRead(wSeekPosition, 3, pLocalBuffer, TARGET_MEM_Y); // 3 bytes/word

        // Seeks to the nested resource we want to open

        if(btHandleNestedParentRsc)
        {
            // If the parent resource is not the root,
            // The begin of the resource is :
            //    Origin of parent resource + 9 bytes header + Offset Address of the resource
            //    we are currently opening
            //    (the addrees of the resource is always an offset from the
            //      parent resource start address)
            wSeekPosition = (pLocalBuffer[0] * 3)
                    + NestedRscHandle[btHandleNestedParentRsc].wStartPosition
                    + 9;
        }
        else
        {
            // Parent is root, the address is absolute
            wSeekPosition = (pLocalBuffer[0] * 3);
        }

        NestedRscHandle[btRscHandle].wRscNumber = wRscNum;
        NestedRscHandle[btRscHandle].wStartPosition = wSeekPosition;
        NestedRscHandle[btRscHandle].wCurrentPosition = wSeekPosition;

        LeaveNonReentrantSection();
        return(btRscHandle);

    }
    else    {
       LeaveNonReentrantSection();
       return((int)RSC_MAX_HANDLE_REACHED);
    }
}

///////////////////////////////////////////////////////////////////////////////
//> Name: SysCloseResource
//
//  Type: Function
//  Description: Close the specified "nested" resource file by freeing its handle
//  Inputs:      btHandleNestedRsc  Nested Resource Handle to close
//
//  Outputs:     RETCODE
//
//      This function is not reentrant. It uses global variables
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysCloseResource(BYTE btHandleNestedRsc)
{
    EnterNonReentrantSection();

    if(btHandleNestedRsc < MAX_NESTED_RSC)
    {
        NestedRscHandle[btHandleNestedRsc].wRscNumber = FREE_RSC_HANDLE;
        NestedRscHandle[btHandleNestedRsc].wStartPosition = 0;
        NestedRscHandle[btHandleNestedRsc].wCurrentPosition = 0;

        LeaveNonReentrantSection();
         return(SUCCESS);
    }
    else    {

        LeaveNonReentrantSection();
         return((int)RSC_INVALID_NESTED_HANDLE);
    }
}

///////////////////////////////////////////////////////////////////////////////
//> Name: SysLoadResource
//
//  Type: Function
//  Description: Load a resource from resource.bin
//  Inputs:      wRscNum            Resource number
//               btHandleNestedRsc  Nested Resource Handle
//               wRscType           Resource type
//               *pwTargetBuffer    Pointer to target buffer
//               wTargetSpace       Space target << 16
//               wMaxSize           Max number of bytes in targetBuffer
//
//  Outputs:     RETCODE
//
//      This function is not reentrant. It uses global variables
///////////////////////////////////////////////////////////////////////////////
// Note: You can disable the resource index cache by setting RESOURCE_INDEX_CACHE_SIZE to 0
// for mtp or player in SysResource.h  Enabling it caused mtp
// performance to rise a few percent in testing. The table only holds offfsets for the first n resources.
// Prelim tests show player can save 400msec on intersong delay by enabling this
// 128 resource index table and ordering resource.inc so high frequency ones are in the index cache.
//------------------------------------------------
//_X WORD CIRC ResourceLog[64]; // DEBUG ONLY last 32 resources loaded by this func will be logged here for debug.
//WORD _X wMarkerWord = 0xdeafee;  // DEBUG ONLY
//WORD CIRC _X * ptrToXResourceLog = &ResourceLog[0]; // DEBUG ONLY *(++ptrToXResourceLog) = wRscNum;
//-------------------------------------------------
RETCODE _reentrant SysLoadResource(WORD wRscNum, BYTE btHandleNestedRsc, WORD wRscType,
                        WORD *pwTargetBuffer, WORD wTargetSpace, WORD wMaxSize)
{
   #if ENABLE_RESOURCE_INDEX_CACHE
    // Each word here stores a word offset to header for a resource in resource.bin.
    // (offsets only count data area, skips padding and maybe ecc data)
    static WORD _X IndexCache[RESOURCE_INDEX_CACHE_SIZE] = {0};
   #endif

    WORD wSeekPosition;
    WORD pLocalBuffer[3];

    EnterNonReentrantSection(); // In MTP build, this disables L1 irq's.
	SysAllLinear();

    if(btHandleNestedRsc < MAX_NESTED_RSC)
    {
        wSeekPosition = NestedRscHandle[btHandleNestedRsc].wStartPosition;

        // We need to seek to the location where the resource offset is defined
        // in the parent resource header.
        wSeekPosition += wRscNum * 6;

        if(btHandleNestedRsc)
        {
            // A nested resource has a resource header (extra 9 bytes)
            // whereas the root resource does not.
            wSeekPosition += 9;
        }

        NestedRscHandle[btHandleNestedRsc].wCurrentPosition = wSeekPosition;

    #if ENABLE_RESOURCE_INDEX_CACHE
        if ( !btHandleNestedRsc && (wRscNum < RESOURCE_INDEX_CACHE_SIZE) && (0 != IndexCache[wRscNum]) )
        {
            pLocalBuffer[0] = IndexCache[wRscNum]; // this is intented to save a media read.
        }
        else
        {
            // possible optimization is to cache all indexes in the sector we read
        // Reads the absolute start position for the resource we want to load
        ResourceFileRead(wSeekPosition, 3, pLocalBuffer, TARGET_MEM_Y);
        if ( !btHandleNestedRsc && (RESOURCE_INDEX_CACHE_SIZE > wRscNum) )
                    IndexCache[wRscNum] = pLocalBuffer[0];
        }
    #else
        // Reads the absolute start position for the resource we want to load
        ResourceFileRead(wSeekPosition, 3, pLocalBuffer, TARGET_MEM_Y);
    #endif

        wSeekPosition = pLocalBuffer[0] * 3;

        if(btHandleNestedRsc)
        {
            // For a nested resource we need to add the Stat address
            // whereas the root resource has the start address 0
            wSeekPosition += NestedRscHandle[btHandleNestedRsc].wStartPosition;
            wSeekPosition += 9;
        }

        // Reads the specified resource header (9 bytes)
        ResourceFileRead(wSeekPosition, 9, pLocalBuffer, TARGET_MEM_Y);
        wSeekPosition += 9;

        // Update current position
        NestedRscHandle[btHandleNestedRsc].wCurrentPosition = wSeekPosition;

        // Check resource number
        if(pLocalBuffer[0] != wRscNum)  {
            LeaveNonReentrantSection();
            return((int)RSC_INVALID_RSC);
        }

        // Check resource size
        if(pLocalBuffer[1] > wMaxSize)  {
            LeaveNonReentrantSection();
              return((int)RSC_INVALID_RSC);
        }
        wMaxSize = pLocalBuffer[1];

        // Check resource type
        if(pLocalBuffer[2] != wRscType) {
          LeaveNonReentrantSection();
          return((int)RSC_INVALID_RSC);
        }

        // Read the resource
//        LeaveNonReentrantSection();  // Must leave before calling SysResourceFile Read
        SysResourceFileRead(btHandleNestedRsc, wMaxSize, wTargetSpace, pwTargetBuffer);
        LeaveNonReentrantSection();  // Must leave before calling SysResourceFile Read
        return(SUCCESS);
    }
    else
    {
        LeaveNonReentrantSection();
        return((int)RSC_INVALID_NESTED_HANDLE);
    }
}

///////////////////////////////////////////////////////////////////////////////
//> Name: SysResourceFileRead
//
//  Type: Function
//  Description:    Reads number of bytes from the current resource position
//  Inputs:         wNumBytes       Number bytes to read
//                  btHandleNestedRsc   Nested Resource Handle
//                  wTargetSpace        Space target << 16
//                  *pwTargetBuffer     Pointer to target buffer
//
//  Outputs:     RETCODE
//
//      This function is not reentrant. It uses global variables
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysResourceFileRead(BYTE btHandleNestedRsc, WORD wNumBytes,
                            WORD wTargetSpace, WORD *pwTargetBuffer)
{
    WORD wSeekPosition;

    EnterNonReentrantSection();

    if(btHandleNestedRsc < MAX_NESTED_RSC)
    {
        // Seek to the current position
        wSeekPosition = NestedRscHandle[btHandleNestedRsc].wCurrentPosition;

        // Read the resource
        ResourceFileRead(wSeekPosition, wNumBytes, pwTargetBuffer, wTargetSpace);
        wSeekPosition += wNumBytes;

        // Update current position
        NestedRscHandle[btHandleNestedRsc].wCurrentPosition = wSeekPosition;

        LeaveNonReentrantSection();
         return(SUCCESS);
    }
    else
    {
        LeaveNonReentrantSection();
        return((int)RSC_INVALID_NESTED_HANDLE);
    }

}

///////////////////////////////////////////////////////////////////////////////
//> Name: SysResourceFileSeek
//
//  Type: Function
//  Description:    Adjusts the seek pointer.
//  Inputs:         wNumWords       Number words to seek
//                      if 0, seek to begin of the resource.
//                      If the btHandle is 0, seeks to beginning
//                      of the resource file.
//                  btHandleNestedRsc   Nested Resource Handle
//
//  Outputs:     RETCODE
//
//      This function is not reentrant. It uses global variables
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant SysResourceFileSeek(BYTE btHandleNestedRsc, WORD wNumWords)
{
    WORD wSeekPosition;

    if(btHandleNestedRsc < MAX_NESTED_RSC)
    {
        if(wNumWords == 0)
        {   // Check if request is "rewind to begin of nested resource"
            wSeekPosition = NestedRscHandle[btHandleNestedRsc].wStartPosition;
        }
        else
        {   // Seek number of bytes from current position
            wSeekPosition = NestedRscHandle[btHandleNestedRsc].wCurrentPosition;
            wSeekPosition += (wNumWords * 3);
        }

        // Update current position
        NestedRscHandle[btHandleNestedRsc].wCurrentPosition = wSeekPosition;

        return(SUCCESS);
    }
    else
        return((int)RSC_INVALID_NESTED_HANDLE);

}

///////////////////////////////////////////////////////////////////////////////
//> Name: SysResourceFileTell
//
//  Type: Function
//  Description:    Returns the current resource position
//  Inputs:         btHandleNestedRsc   Nested Resource Handle
//
//  Outputs:        Current position (in Words) for the specified resource
//
///////////////////////////////////////////////////////////////////////////////
WORD _reentrant SysResourceFileTell(BYTE btHandleNestedRsc)
{
    return((NestedRscHandle[btHandleNestedRsc].wCurrentPosition / 3));
}

///////////////////////////////////////////////////////////////////////////////
//> Name: SysSetResourceTag
//
//  Type: Function
//  Description:    Sets the tag for the active resource drive
//  Inputs:         wTag:   New Resource Drive Tag
//
//  Outputs:        None
//
//  This function will close the current resource drive and set
//  the save locally the tag for the new resource drive.
//  The next call to SysLoadResource/SysOpenResource will discover
//  the resource drive and load resources from it.
//
///////////////////////////////////////////////////////////////////////////////
void _reentrant SysSetResourceTag(WORD wTag)
{
    INT i;

    wResourceTag = wTag;

    CloseResourceDrive();

    for(i=1 ; i<MAX_NESTED_RSC ; i++)
    {
        NestedRscHandle[i].wRscNumber = FREE_RSC_HANDLE;
    }

}

#ifdef FULL_PLAYER_KERNEL
///////////////////////////////////////////////////////////////////////////////
// Name: GetSemaphore0
//
//  Type: Function
//  Description:    Gets the Semaphore0
//  Inputs:         None
//
//  Outputs:        1 if Semaphore NOT acquired as it was alread set by another
//                  Caller shoulg wait until semaphore is acquired
//                  And be nice and let other processes run
//                  0 if Semaphore acquired - OK to continue
//  Notes:          Uses atomic instruction bset to test and set the semaphore
//                  TBD - make more general to specify other bits as semaphores
//
///////////////////////////////////////////////////////////////////////////////
#pragma asm
    extern  Semaphore
FGetSemaphore0              ; a0 set/cleared for C-callers
GetSemaphore0               ; Carry bit set/cleared in Status Register for asm callers
    clr     a               ; a = 0 if Semaphore acquired
    bset    #$0,Y:Semaphore ; C = 1 if Semaphore NOT acquired
                            ; C = 0 if  Semaphore acquired
    jcc     GotIt
    move    #>1,a           ; a = 1 if Semaphore NOT acquired as it was alread set by another
GotIt:
    rts
#pragma endasm
///////////////////////////////////////////////////////////////////////////////
// Name: Release Semaphore0
//
//  Type: Function
//  Description:    Releases the Semaphore0
//  Inputs:         None
//
//  Outputs:        1 if Semaphore released properly
//                  0 if Semaphore already released - an error
//  Notes:          uses atomic instruction blcr to test and clear semaphore
//
///////////////////////////////////////////////////////////////////////////////
#pragma asm
FReleaseSemaphore0          ; a0 set/cleared for C-callers
ReleaseSemaphore0           ; Carry bit set/cleared in Status Register for asm callers
    clr    a
    bclr    #$0,Y:Semaphore ; C = 0 if  Semaphore already released - an ERROR
                            ; C = 1 if Semaphore released properly
    jcc     Error           ; Error if bit was already cleared. It should have been set prior to releasing
    move    #>1,a           ; a = 0 if  Semaphore already released - an ERROR
Error:
    rts                 ; a = 1 if Semaphore released properly
 #pragma endasm


#pragma asm
                    org     y,"EXEC_semaphore_Y":
Semaphore           dc 0
#pragma endasm

#endif
///////////////////////////////////////////////////////////////////////////////
// Name: SysGetResourceTag
//
//  Type: Function
//  Description:    Gets the tag for the current resource drive
//  Inputs:         None
//
//  Outputs:        None
//  Notes:          SDK3.0 had nothing here. 2.6 deleted this header.
//
///////////////////////////////////////////////////////////////////////////////
