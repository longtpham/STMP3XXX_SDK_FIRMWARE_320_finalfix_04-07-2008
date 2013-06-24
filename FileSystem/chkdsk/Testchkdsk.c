////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002-2004
//
// Filename: TestChkdsk.c
// Description: Check Disk Persistent Bits utilities.
//
// To do:
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "chkdsk.h"

#include "regsrtc.h"
#include "persist_bits.h"

#ifdef USE_PLAYLIST3
extern _reentrant int write_detection_access(int option);	//added by samuel on 1/3/05 for flash write detection
#endif

extern _asmfunc void CheckDiskLoad(void);

static void do_nothing(void) {      // This function is here to make the dang FUNCLET, below, work right.
                                    // Without this here, the compiler switches back to ".ptext..." as soon
                                    // as it hits some code.  With this here, the "org p,"SYSFUNCLET_..._P"
                                    // stays in effect.
                                    // The function is declared "static" so that it's invisible outside this file.
  return;
}

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:          TestSkipCheckDisk
//
//   Type:          Function
//
//   Description:   Determine whether checkdisk needs to be run by looking
//                  at the sticky bits.
//
//   Inputs:        WORD wDiskNum - data drive number.
//
//   Outputs:       None
//
//   Notes:         We only want to skip the NAND portion of checkdisk.  MMC
//                  should be tested each time.
//                  SysLoadFATWrite and SysUnLoadFATWrite must be called 
//                  before and after this routine because Checkdisk must
//                  write sectors if an error is found.
//<
///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifdef FUNCLET
    #pragma asm
        org p,".ptexttestchkdsk":
        FTestSkipCheckDisk:

        nolist
        include "sysmacro.asm"
        include "resource.inc"        
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_TESTSKIPCHECKDISK
        CallFunclet RSRC_FUNCLET_TESTSKIPCHECKDISK

        org p,"SYSFUNCLET_TESTSKIPCHECKDISK_P":
        dc      RSRC_FUNCLET_TESTSKIPCHECKDISK
    #pragma endasm
    
    //Use the exact same prototype of the static function to convert
void TestSkipCheckDisk_Funclet(WORD wDiskNum)
#else
void TestSkipCheckDisk(WORD wDiskNum)
#endif
{
#ifdef CHKDSK
// We don't want to run this test for the D3501
#ifndef D3501
   
   RETCODE returnCode;
   BOOL bBitResult;
   //WORD i;

 #ifdef DEVICE_3500
   // if SkipCheckDisk is false, we need to run Checkdisk
   if( SUCCESS == ReadStickyBit((volatile _X WORD *)&HW_RTC_SKIP_CHECKDISK_REG,
                              HW_RTC_PERSISTENT1_SKIP_CHECKDISK_BITPOS,
                              &bBitResult)  )
    {      
        if (!bBitResult) // 0 means don't skip checkdisk so load and run it now. 
        {        
          #ifdef USE_PLAYLIST3
	        write_detection_access(1);
          #endif
            CheckDiskLoad();  
            // if we want to scan all logical medias (initial startup)              
            CheckDisk(wDiskNum);
         
            // clear MTP database clean flag when checkdisk runs, so database is rebuilt next MTP startup.
            ClearStickyBit((volatile _X WORD *)&HW_RTC_MTP_DB_CLEAN_FLAG_REG, 
                                    HW_RTC_PERSISTANT1_MTP_DB_CLEAN_FLAG_BITPOS);
        }
    }
    else 
    {   DebugBuildAssert(0); // halt in DEBUG build if failed to read persist bit. 
        //Continues in RETAIL build (avoid a RETAIL reboot loop that SystemHalt can give).
    }

   // Always want to clear SkipCheckDisk in case we don't power down gracefully.
   ClearStickyBit((volatile _X WORD *)&HW_RTC_SKIP_CHECKDISK_REG, 
                  HW_RTC_PERSISTENT1_SKIP_CHECKDISK_BITPOS);

 #else // non DEVICE_3500 case
   CheckDiskLoad();  
   // if we want to scan all logical medias (initial startup)              
   CheckDisk(wDiskNum);
 #endif 

#endif //D3501
#endif //CHKDSK
}

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:          SetSkipCheckDisk
//
//   Type:          Function
//
//   Description:   Upon a graceful power-down, we want to set the
//                  SkipCheckDisk bit.
//                   
//   Inputs:        none
//
//   Outputs:       None
//                   
//   Notes:         Upon a graceful power-down, we want to set the
//                  SkipCheckDisk bit so we don't run CheckDisk the next time.
//<
///////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#ifdef FUNCLET
    #pragma asm
        org p,".ptexttestchkdsk":
        FSetSkipCheckDisk:

        nolist
        include "sysmacro.asm"
        include "resource.inc"        
        list

        ;If using funclet, function will be executed via funclet kernel
        extern RSRC_FUNCLET_SETSKIPCHECKDISK
        CallFunclet RSRC_FUNCLET_SETSKIPCHECKDISK

        org p,"SYSFUNCLET_SETSKIPCHECKDISK_P":
        dc      RSRC_FUNCLET_SETSKIPCHECKDISK
    #pragma endasm
    
    //Use the exact same prototype of the static function to convert
void SetSkipCheckDisk_Funclet(void)
#else
void SetSkipCheckDisk(void)
#endif
{
#ifdef CHKDSK
// We don't want to run this test for the D3501
   #ifndef D3501

   #ifdef DEVICE_3500
   //RETCODE returnCode;
   // We want to set SkipCheckDisk if we power down gracefully
   SetStickyBit((volatile _X WORD *)&HW_RTC_SKIP_CHECKDISK_REG, 
                  HW_RTC_PERSISTENT1_SKIP_CHECKDISK_BITPOS);
   #endif // ifdef DEVICE_3500
   #endif
#endif
}
    
