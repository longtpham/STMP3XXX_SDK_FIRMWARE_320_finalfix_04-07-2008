////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2005
//
// Filename: main.c
// Description: Main file for USB MTP firmware (LCDexample)
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "sysirq.h"
#include "icoll.h"
#include "kernel.h"
#include "filesystem.h"
#include "mtp.h"
#include "scsi.h"
#include "bootmanager.h"
#include "misc.h"
#include "mediadetection.h"
#include "usb_api.h"
#include "ddildl.h"
#include "sysresourceapi.h"
#include "instrumentation.h"
#include "battery_config.h"
#include "battery.h"
#include "lowresolutionadc.h"
#include "sysserialnumber.h"
#include "instrument.h"
#include "resource.h"
#include "usbmsc.h"
#include "persist_bits.h"
#include "fsapi.h"
#include "fsproj.h"
#ifdef k_profiel_mtp
#include "Profile_Tools.h"
#endif

#include "regsgpio.h"
#include "regsrtc.h"
#include "regsusb20.h"
#include "regscodec.h"
#include "ddildl\ddildl_defs.h"
#include "restoredrivefrombackupMTP.h"

    #pragma asm
    nolist
    include "sysmacro.asm"
    list 
    #pragma endasm

    
extern WORD g_wNumDrives;
extern BOOL g_bLimitedVDDD;
_reentrant WORD SysGetDesiredResourceTagMTP(void);
//$$$
// this constant s/b in memorysetup.h, but that file doesn't exist yet...
// it should have the correct value as well...
#define kix_media_region 0
// these prototypes s/b in memorysetup.h, but that file doesn't exist yet...
_reentrant void region_discard_current_resource(int);
_reentrant WORD region_get_current_resource(int);
//$$$

////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////
int RebuildRsrcNum = RSRC_STORE_REBUILD;
int AddMetadataRsrcNum = RSRC_STORE_ADDMETADATA;
int SetObjectProtectionRsrcNum = RSRC_MTP_SETOBJECTPROTECTION;

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

extern USBMTP_DEVICE UsbMtpDevice;
extern void SysLoadMtpBootInit(void);
extern RETCODE UsbProjectInit(void);
extern RETCODE _reentrant UsbMscProjectInitDevice0(void);
#if ((NUM_REMOVABLE_MEDIA == 1) && (MULTI_LUN_BUILD==0))
    extern RETCODE _reentrant MtpProjectInitDevice1(void);
#endif
extern RETCODE _reentrant MiscProjectInit(void);
extern RETCODE CompactFlashSetPortTiming(WORD wTiming);
extern BOOL bReconfigureAsDcc;

#if NUM_REMOVABLE_MEDIA == 1
    #ifdef MMC
        extern RETCODE _reentrant InitMmcSense(WORD wNum);
        extern BOOL _reentrant HalMmcIsDeviceInserted(WORD wNum);
    #else
        extern _asmfunc void InitSmartMediaSense(void);
        extern _asmfunc BOOL SmIsDeviceInserted(WORD wNum);
    #endif
#endif
extern WORD wMediaState;
extern WORD wMediaStateOld;
    extern USBMSC_DEVICE UsbMscDevice[];
        
BOOL g_bPowerOff = FALSE;
WORD wTaskMediaInsertionCallBackDelay = 250;


extern void ChipUpdateCapability(void);

//
// This is located in overlaymanager.c.
//
extern void _reentrant LoadCode(WORD wResource, MemoryRegion* pRegion);

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

//RETCODE DebugInit(void);

////////////////////////////////////////////////////////////////////////////////
// Debug Variables
////////////////////////////////////////////////////////////////////////////////
/*
    #ifdef SCSI_DEBUG_WRITE_TIME_HISTORY
#define SIZE_OF_WRITE_TIME_HISTORY_BUFFER   1024
#define WRITE_TIME_HISTORY_BUFFER_SHIFT     10
    DWORD dwWriteTimeCurrent;
    DWORD dwWriteTimeAverage;
    DWORD dwWriteTimeLow;
    DWORD dwWriteTimeHigh;        
    WORD wWriteTimeHistoryBufferSize;
    DWORD dwWriteTimeHistory[SIZE_OF_WRITE_TIME_HISTORY_BUFFER];
    WORD wWriteTimeHistoryPointer;
    WORD wWriteTimeHistoryDivideShift;
    
    WORD dwWriteSectorCount;
    WORD dwWriteRandomCount;
    WORD dwWriteSeqFirstCount;
    WORD dwWriteSeqNextCount;
    #endif
    
    #ifdef SCSI_DEBUG_READ_TIME_HISTORY
#define SIZE_OF_READ_TIME_HISTORY_BUFFER   128
#define READ_TIME_HISTORY_BUFFER_SHIFT     7
    DWORD dwReadTimeCurrent;
    DWORD dwReadTimeAverage;
    DWORD dwReadTimeLow;
    DWORD dwReadTimeHigh;        
    WORD wReadTimeHistoryBufferSize;
    DWORD dwReadTimeHistory[SIZE_OF_READ_TIME_HISTORY_BUFFER];
    WORD wReadTimeHistoryPointer;
    WORD wReadTimeHistoryDivideShift;
    #endif

    #ifdef SCSI_DEBUG_READ_COMMAND_HISTORY
#define SIZE_OF_READ_HISTORY_BUFFER 800
    WORD wReadHistorySize =  SIZE_OF_READ_HISTORY_BUFFER;
    WORD _Y ReadHistory[SIZE_OF_READ_HISTORY_BUFFER]; 
    WORD wReadHistoryPointer = 0;
    WORD wReadHistoryEnd = SIZE_OF_READ_HISTORY_BUFFER;
    #endif
    
    #ifdef SCSI_DEBUG_WRITE_COMMAND_HISTORY
#define SIZE_OF_WRITE_HISTORY_BUFFER 600
    WORD wWriteHistorySize =  SIZE_OF_WRITE_HISTORY_BUFFER;
    WORD _Y WriteHistory[SIZE_OF_WRITE_HISTORY_BUFFER]; 
    WORD wWriteHistoryPointer = 0;
    WORD wWriteHistoryEnd = SIZE_OF_WRITE_HISTORY_BUFFER;
    #endif
*/
////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

// These must be the last variables before code
#pragma asm
    org     y,".ytextmain":
;    GLOBAL SysBrownout
SysBrownout     dc      0

; restore the p mem counter
    org     p,".ptextmain":
    ; These two are needed for the dccui.asm to build correctly simply because
    ;  they are included in dcc.xref
#pragma endasm

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          Project_Startup          
//
//   Type:          Function
//
//   Description:   A main entry point called by the kernel
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void Project_Startup(void)
{
    //BOOL bDone;
    WORD i;
    //WORD j;
    //DWORD wTime;
    //WORD wCount = 5000;
    //RETCODE retCode;
    USHORT  usStatus;
    BOOL bLowPowerUsbStickyBitReadResult = FALSE;

    //bDone = FALSE;

    // Let's start with a clean system
    SystemShutdown();

    #pragma asm

        extern  FTimeInit
        jsr     FTimeInit
    
    #pragma endasm
    
    if(HW_USBCSR.B.ARCCONNECT == 1)
        SysDelayMs(500);

    EventSetup00();    
    EventSetup01();    
    EventSetup02();    
    EventSetup03();    
    EventSetup04();    
    EventSetup05();    
    EventSetup06();    
    EventSetup07();    

    //------------------------------------------
    // Historic note: When running in boxview, setting all pins to 8mA here helped when done
    // before call to MediaInit on FM's engr board. (BB said). EVK & cust layouts were ok.

    // This pin drive strength init to 8mA for flash input pins is now in nandhalinit (mediainit)
    //------------------------------------------

    // flash timing is now done in nandmediainit, not here.
    //change the hold switch to be an input. Reduces current draw in usb mode.
    HW_GP1ENR.B.B11 = TRUE;

    StickyBitInit();

    #pragma asm
  
    include "regsdcdc.inc"
    include "regsrtc.inc"
    ; These next four lines were necessary for the MTP db sticky bit to transfer across applications
    bclr    #3,X:HW_RTC_PERSIST0
    ; For the 35xx...
    ; Enable XTAL on power down, to preserve RTC value and persistent bits
    bset    #5,X:HW_DCDC_PERSIST             ; Set the SLEEP_XTAL_ENABLE bit in the DCDC persistent shadow register
    bset    #10,X:HW_DCDC_PERSIST            ; Update the master DCDC persistent register in the clock domain
    bclr    #10,X:HW_DCDC_PERSIST
            
    #pragma endasm
     #ifdef k_profile_mtp
    //Init the Trace
    ClearTrace(); 
    #endif
    // Set the working resource tag, and clear resource drive number
	SysSetResourceTag(SysGetDesiredResourceTagMTP());   

    // Initialize boot media (NAND)	
    if(MediaInit(0) != SUCCESS)
    {
        SystemHalt();
    }
	
    // Discover the media and its drives
    if(MediaDiscoverAllocation(0) != SUCCESS)
    {
        SystemHalt();
    }

 
    // Skip the data drive initializations until resource drive is initialized
#ifdef MMC
    for(i=3;i<g_wNumDrives;i++)
#else
    for(i=2;i<g_wNumDrives;i++)
#endif    
    {
        DriveInit(i);
    }
    //
    // Initially load the code necessary to use the file system.  This cannot 
    // be loaded a boot time since the bootload resides in this memory area.
    //
    LoadCode(RSRC_FATWRITE_P_CODE, g_RegionTable[4]);
    SysLoadMtpBootInit();
    
#ifdef MMC
    // Full MMC init comes after USB state configured, not here. 
    g_iHighPwrPeriphInitMode = STRUCTURE_INIT_ONLY; // only allowed to do an MMC struct init here.

    // This call only sets structure members and always returns unsuccessful 
    // status since full MMC init is intentionally blocked, so it can be completed 
    // after USB STATE CONFIGURED when the allowed USB current is known. 
    #if 1
    MediaInit(1);
    #else
    if((retCode = MediaInit(1)) != SUCCESS) 
    {
        // debugging code
        #if 0 
        if((retCode != MMC_MEDIA_ERROR_DEVICE_NOT_INSERTED) && (retCode != LMEDIA_ERROR_HARDWARE_FAILURE))
        {   SystemHalt();
        } 
        #endif
    }
    #endif

    // Initialize all data drives
    DriveInit(0);
    DriveInit(1);
#else
    // for non-mmc (player with no external drive) we don't initialize drive 1 (b: is reserved for external media only).
    DriveInit(0);
//    DriveInit(1);
#endif
   
    // Initialize the chip serial number.  This must occur before seting up the
    // USB because USB uses the chip serial number for its serial number string
    ChipSerialNumberInit();

    ChipUpdateCapability(); // Initialize the hardware capabilities.

    UsbMscDevice[0].InitializeFunction = UsbMscProjectInitDevice0;

    assert(MiscProjectInit()==SUCCESS);



    // For High Speed Device we must enable USB as fast as possible to get 
    // within (or at least close to) the 100mS USB Spec from +5V connect and D+
    // high.

    // We noticed that ARC will receive on EP2 even so the endpoint is not primed
    // to avoid it, we will hold the usb by NAKing the data phase on the first
    // get descriptor.
    // The usb is released after initializing EP2.
    usb_device_hold();

    // power | USB current draw reduction via unused peripheral power down.
    HW_ADCCSR.B.CLKGT = TRUE; // CLKGT bit set to gate off the ADC's clock. power savings
    HW_DACCSR.B.CLKGT = TRUE; // CLKGT bit set to gate off the DAC's clock. power savings
    //end pwr reduction via peripheral power down



    //--------- USB Current Config
    // When host controlled low power mode is off, this default is kept iff battery charge or MMC build
    //usb_set_current_limit(500); //This default is set in the StDescConfig descriptors_3500.c structure init rather than here.
    if( ReadStickyBit((volatile _X WORD *) &HW_RTC_PERSISTENT1, HW_RTC_PERSISTANT1_LOW_POWER_USB_BITPOS, 
                                 &bLowPowerUsbStickyBitReadResult) != SUCCESS )
    {
        DebugBuildAssert(FALSE); // halts only in DEBUG builds.
    }
    if( bLowPowerUsbStickyBitReadResult )
    {//if we're in low power mode (we couldn't enumerate at the higher current requested in the descriptor
        //clear the persistant low power USB mode bit. 
        if ( ClearStickyBit((volatile _X WORD *)&HW_RTC_PERSISTENT1, 
                            HW_RTC_PERSISTANT1_LOW_POWER_USB_BITPOS)  != SUCCESS)
        {    
            DebugBuildAssert(FALSE); // halts only in DEBUG build, vanishes in RETAIL build.   
        }
        //set the current limit request in the config descriptor for this time to 100 mA.
        //DebugBuildAssert(jCurrLimEntry==0); // emulator can enable
        usb_set_current_limit(100);
    }
    else
    {   // if (!battery_charge_build && !(MMC|SD)) 100 mA USB current is all that's needed.
        #ifndef BATTERY_CHARGE
        #if (NUM_REMOVABLE_MEDIA==0)  // and if   no removable media such as MMC
            //DebugBuildAssert(jCurrLimEntry==0); //emulator can enable
            usb_set_current_limit(100); // For NAND-only, non recharge build.
        #endif
        #endif
    }//--------- end USB Current Config


    // Initialize the USB port
    assert(usb_device_init()==SUCCESS);
    
    // Run the project-specific init
    UsbMscDevice[0].InitializeFunction();

    // Initialize the USB MSC driver
    assert(UsbMscDriverInitialize()==SUCCESS);
    
    // Unmask all interrupts
    SysUnMaskAllInterrupts(INTERRUPTS_UNMASK_ALL);

    // Enable the USB port
    usb_device_enable(); 

    //change the hold switch to be an input
    HW_GP1ENR.B.B11= TRUE;

    // Run power initialization
    assert(PowerInit()==SUCCESS);

    // Delay for a bit so the GPIOs & pad power-downs settle out
    SysDelayMs(5);    
    

    
    InstrumentInitialize();

}


//------------------------------------------------------------------------------
// Description: Changes player resource tag global if demanded by refresh resource.bin persistant bit. 
// Inputs: none
// Outputs: Returns value of resource tag that is commanded by sticky bit

_reentrant WORD SysGetDesiredResourceTagMTP(void)
{
    if(IsSystemDriveDirty() == TRUE)
    {
        g_wHostResourceTag = DRIVE_TAG_HOSTRSC2_BIN;
    }
    else
    {
        g_wHostResourceTag = DRIVE_TAG_HOSTRSC_BIN;
    }

    return g_wHostResourceTag; 
}

