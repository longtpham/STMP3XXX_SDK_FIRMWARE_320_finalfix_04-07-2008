// filename: \test\RscMgrTest\SysInittask.c
#include "kernel.h"
#include "resource.h"
#include "messages.h"
#include "ddildl.h"
#include "sysresourceapi.h"

extern WORD g_wNumDrives;

//
// Stubs needed by the library code, these are not used.
//
void EnterNonReentrantSection(void)
{
}
void LeaveNonReentrantSection(void)
{
}

// use this for tests that must halt instead of ver that resets the DSP in RETAIL build. 
#pragma asm
 global FatalError
FatalError:
    debug
    nop
#pragma endasm

//
//
void Project_Startup(void)
{

    Message msg;

    #pragma asm

        extern  FTimeInit
        jsr     FTimeInit
    
    #pragma endasm

    SysPostMessage(3,LCD_SET_CONTRAST,30);

}

//
//
_reentrant void TaskSysInitEntryPoint(void)
{
    int a = 123456, i;
    
    // Init the media
    if(MediaInit(0) != SUCCESS)
    {
        SystemHalt();
    }

    // Discover the Media and its drives
    if(MediaDiscoverAllocation(0) != SUCCESS)
    {
        SystemHalt();
    }

    // Init All Drives
    for(i=0;i<g_wNumDrives;i++)
    {
        if(DriveInit(i) != SUCCESS)
        {
        }
    }

    SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN);
    
    while(1)
    {

        SysWait(-1);

    }
}

