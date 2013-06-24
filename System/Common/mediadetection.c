////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// File : mediadetection.c
// Description : Media Detection functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Macros
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  External Definitions
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "mediadetection.h"

#pragma optimize 1
////////////////////////////////////////////////////////////////////////////////
//  External References
////////////////////////////////////////////////////////////////////////////////

    extern MEDIA_INSERTION RemovableMediaTable[];
#ifdef FUNCLET
#pragma asm
    extern SysCallFunclet
#pragma endasm    
#endif    

////////////////////////////////////////////////////////////////////////////////
//  Defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Prototypes
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  X Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Y Memory
////////////////////////////////////////////////////////////////////////////////

WORD wMediaState = (WORD)0;
WORD wMediaStatePrevious = (WORD)0;

MEDIA_DETECT_FLAGS wMediaDetectFlags;

////////////////////////////////////////////////////////////////////////////////
//  P Memory
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           MediaDetctionInitHW
//
//   Type:           Function
//
//   Description:    Initializes media hardware
//
//   Inputs:         MEDIA_INSERTION * MediaDescriptors
//
//   Outputs:        RETCODE NOERROR 
//
//   Notes:          This must be run before FSInit()  
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MediaDetectionInitHW(MEDIA_INSERTION * MediaDescriptors)
{
    WORD i;
    WORD j;
    
    // init all
    for(i=0;i<NUM_REMOVABLE_MEDIA;i++)
    {
        // call the appropriate InitRemovableDetection function
        (MediaDescriptors[i].InitRemovableDetection)(MediaDescriptors[i].wPhysicalDeviceNumber);
    }
        
    return(NOERROR);
}



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           MediaDetectionInitStateVars
//
//   Type:           Function
//
//   Description:    Run first detection and initialize state variables used 
//                   during media detection
//
//   Inputs:         MEDIA_INSERTION * MediaDescriptors
//
//   Outputs:        RETCODE NOERROR
//
//   Notes:          This one calls a funclet so must be run after FSInit()
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MediaDetectionInitStateVars(MEDIA_INSERTION * MediaDescriptors)
{

    // Run the first detection
    MediaDetection(MediaDescriptors);
    
    // No previous state so clear the changed flag
    wMediaDetectFlags.B.MediaChanged = 0;
    wMediaStatePrevious = wMediaState;

    return(NOERROR);
}



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           MediaDetection
//
//   Type:           Function
//
//   Description:    
//
//   Inputs:         
//
//   Outputs:        
//
//   Notes:          Funclet
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef FUNCLET
    #pragma asm
        FMediaDetection:

        nolist
        include "sysmacro.asm"
        include "resource.inc"
        list

        ;If using funclet, function will be executed via funclet kernel
        CallFunclet RSRC_FUNCLET_MEDIADETECTION

        org p,"SYSFUNCLET_MediaDetection_P":
        dc      RSRC_FUNCLET_MEDIADETECTION
    #pragma endasm
    
        //Use the exact same prototype of the static function to convert
BOOL _reentrant MediaDetection_Funclet(MEDIA_INSERTION * MediaDescriptors)
#else    
BOOL _reentrant MediaDetection(MEDIA_INSERTION * MediaDescriptors)
#endif    
{
    WORD i;
    WORD j;
    WORD wTemp = 0;
    WORD wBitPos = 0;
    
    // check all
    for(i=0;i<NUM_REMOVABLE_MEDIA;i++)
    {
        // call the appropriate IsDeviceInserted function
        if((MediaDescriptors[i].IsDeviceInserted)(MediaDescriptors[i].wPhysicalDeviceNumber))
        {
            wTemp |= 1<<wBitPos;
        }
        wBitPos++;
    }
    
    // exit if no change
    if(wTemp == wMediaState)
        return(FALSE);
    
    // media has been inserted/removed, update the MediaState
    wMediaStatePrevious = wMediaState;
    wMediaState = wTemp;
    
    // set the MediaChanged flag in MediaDetectFlags
    wMediaDetectFlags.B.MediaChanged = 1;
    
    return(TRUE);
}

#ifdef FUNCLET
    #pragma asm
        ;The name of the following counter is the concatenation of the string ;".ptext" and the name of the C source file.
        org p,".ptextmediadetection":
    #pragma endasm
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           IsPhysicalMediaRemovable
//
//   Type:           Function
//
//   Description:    
//
//   Inputs:         
//
//   Outputs:        
//
//   Notes:          
//<
////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant PhysicalIsMediaRemovable(WORD wType, WORD wNum)
{
    WORD i;
    
    for(i=0;i<NUM_REMOVABLE_MEDIA;i++)
    {
        if(RemovableMediaTable[i].wMediaType == wType)
        {
            if(RemovableMediaTable[i].wPhysicalDeviceNumber == wNum)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           PhysicalHasMediaChanged
//
//   Type:           Function
//
//   Description:    
//
//   Inputs:         
//
//   Outputs:        
//
//   Notes:          
//<
////////////////////////////////////////////////////////////////////////////////
BOOL _reentrant PhysicalHasMediaChanged(WORD wType, WORD wNum)
{
    WORD i;
    
    for(i=0;i<NUM_REMOVABLE_MEDIA;i++)
    {
        if(RemovableMediaTable[i].wMediaType == wType)
        {
            if(RemovableMediaTable[i].wPhysicalDeviceNumber == wNum)
            {
                if((wMediaState & (1<<i)) == (wMediaStatePrevious & (1<<i)))
                {
                    return FALSE;
                }
                else
                {
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           MediaDetectionReset
//
//   Type:           Function
//
//   Description:    
//
//   Inputs:         
//
//   Outputs:        
//
//   Notes:          
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MediaDetectionReset(void)
{
    wMediaDetectFlags.B.MediaChanged = FALSE;    
    wMediaDetectFlags.B.MediaChanged = FALSE;    

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:           PhysicalMediaChangedReset
//
//   Type:           Function
//
//   Description:    
//
//   Inputs:         
//
//   Outputs:        
//
//   Notes:          
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant PhysicalMediaChangedReset(WORD wType, WORD wNum)
{
    WORD i;
    WORD wTemp;
    
    for(i=0;i<NUM_REMOVABLE_MEDIA;i++)
    {
        if(RemovableMediaTable[i].wMediaType == wType)
        {
            if(RemovableMediaTable[i].wPhysicalDeviceNumber == wNum)
            {
                wTemp = wMediaState & (1<<i);
                if(wTemp)
                {
                    wMediaStatePrevious |= wTemp;
                }
                else
                {
                    wMediaStatePrevious &= ~wTemp;
                }
                
                return SUCCESS;
            }
        }
    }

    return MD_ERROR_INVALID_MEDIA_NUMBER;
}
