////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2004-2005
//
// Filename: main.c
// Description: Main file for USB MSC firmware (LCDexample)
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
#include "usbmsc.h"
#include "scsi.h"
#include "bootmanager.h"
#include "misc.h"
#include "mediadetection.h"
//#include "chkdsk.h"
#include "usb_api.h"
#include "ddildl.h"
#include "sysresourceapi.h"
#include "instrumentation.h"
#include "battery_config.h"
#include "battery.h"
#include "lowresolutionadc.h"
#include "sysserialnumber.h"
//#define INSTRUMENT_ENABLE 1
#include "instrument.h"

#if defined(DEVICE_3500)
    #include "regsgpio.h"
    #include "regsrtc.h"
    #include "persist_bits.h"
    #include "regsusb20.h"
    #include "regscodec.h"
#endif

    #pragma asm
    nolist
    include "sysmacro.asm"
    list 
    #pragma endasm

#ifdef MFG_TOOL
extern Struct_Standard_Dev_Desc _USB_MEM StDescDevice;
#endif



INT g_JanusDrive;

extern WORD g_wNumDrives;
extern BOOL g_bLimitedVDDD;
////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

    #ifdef SCSI_COMMAND_HISTORY
        extern WORD _Y CommandHistory[];
        extern WORD _Y CdbHistory[][SCSI_CDB_SIZE_IN_WORDS];
    #endif
    
    extern USBMSC_DEVICE UsbMscDevice[];

//    extern USBPROJECTSPECIFICINITFUNCTION UsbProjectSpecificInit;
    extern RETCODE UsbProjectInit(void);
    extern RETCODE UsbMscProjectInitDevice0(void);
    #if ((NUM_REMOVABLE_MEDIA == 1) && (MULTI_LUN_BUILD==0))
        extern RETCODE _reentrant UsbMscProjectInitDevice1(void);
    #endif
    extern RETCODE MiscProjectInit(void);
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
        
BOOL g_bPowerOff = FALSE;
WORD wTaskMediaInsertionCallBackDelay = 250;

extern void ChipUpdateCapability(void);

extern _Y WORD * _Y _lc_u_y_MMC_copy_end;
extern _Y WORD * _Y _lc_u_y_MMC_copy_start;
extern _Y WORD * _Y _lc_u_y_MMC_bssclear_start;
extern _Y WORD * _Y _lc_u_y_MMC_bssclear_end;
extern _P WORD * _P _lc_u_boot_mmc;  

extern _P WORD * _P _lc_u_p_OVLY_start;
extern _Y WORD * _Y _lc_u_y_OVLY_copy_start;
extern _Y WORD * _Y _lc_u_y_OVLY_copy_end;

void _reentrant CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize);

void ClearMemory(_Y WORD * _Y pYCopyStart, WORD wYClearSize);

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

RETCODE DebugInit(void);
void doSwitch();     // dummy func halts/reboots
void LoadMtp(void); // "

////////////////////////////////////////////////////////////////////////////////
// Debug Variables
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

//
// Needed to make library happy with USB/MTP mode.
//
WORD g_MtpArbitrationDone=FALSE;

#ifdef UPDATER
int g_iBuildType = BUILD_TYPE_UPDATER;
#else
 #ifdef MFG_TOOL
int g_iBuildType = BUILD_TYPE_UPDATER;
 #else
int g_iBuildType = BUILD_TYPE_HOSTLINK;
 #endif
#endif


// If a project such as a test uses MMC but not USB, it needs to bypass the USB 
// current check in the MMC driver, it must include this global define & init value.
// Two lines Needed for MMC driver to link and fully enumerate. Simulates granting of high current by USB host. 
//#define USB_HIGH_CURRENT_GRANTED 3
//int g_iHighPwrPeriphInitMode = USB_HIGH_CURRENT_GRANTED;
//


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
//int jCurrLimEntry=1;
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
    
    // Startup
    // Run debug initialization
    (DebugInit()== SUCCESS);

    // Let's start with a clean system
    SystemShutdown();

    #pragma asm

        extern  FTimeInit
        jsr     FTimeInit
    
    #pragma endasm
    
// In order to free up P memory space, we're doing the same trick as above for
// some other initialization functions.
    {
        unsigned int ClearSize = (unsigned int)(&_lc_u_y_MMC_bssclear_end-&_lc_u_y_MMC_bssclear_start);

        unsigned int CopySize = (unsigned int)(&_lc_u_y_OVLY_copy_end-&_lc_u_y_OVLY_copy_start);
        
        CopyCodeIntoBootmanagerArea((_Y WORD *)&_lc_u_y_OVLY_copy_start, 
                                    (_P WORD * _P)&_lc_u_p_OVLY_start, CopySize);
        
        // In either case(MMC or NAND only), clear the bss section.
        ClearMemory((_Y WORD *)&_lc_u_y_MMC_bssclear_start, ClearSize);
    }
    
#if defined(DEVICE_3500)
    if(HW_USBCSR.B.ARCCONNECT == 1)
        SysDelayMs(500);
#endif

    EventSetup00();    
    EventSetup01();    
    EventSetup02();    
    EventSetup03();    
    EventSetup04();    
    EventSetup05();    
    EventSetup06();    
    EventSetup07();    

    // Initialize the chip serial number.  This must occur before seting up the
    //  USB because USB uses the chip serial number for its serial number string
    ChipSerialNumberInit();

	ChipUpdateCapability();		// Initialize the hardware capabilities.

    //------------------------------------------
    // Historic note: When running in boxview, setting all pins to 8mA here helped when done
    // before call to MediaInit on FM's engr board. (BB said). EVK & cust layouts were ok.

    // This pin drive strength init to 8mA for flash input pins is now in nandhalinit (mediainit)
    //------------------------------------------


#if defined(DEVICE_3500)
    //change the hold switch to be an input
    HW_GP1ENR.B.B11= TRUE;
#endif

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



    if(MediaInit(0) != SUCCESS) // RAMDRIVE
    {
        SystemHalt();
    }

    MediaInit(1); // NAND Flash

    UsbMscDevice[0].InitializeFunction = UsbMscProjectInitDevice0;

#if defined(DEVICE_3500)

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
    }
    //--------- end USB Current Config
    
    //if MFG tool build, change serial number index
    #ifdef MFG_TOOL
        StDescDevice.btIndexSerialNumber=0;
    #endif


    // Initialize the USB port
    assert(usb_device_init()==((BYTE)SUCCESS));
    
    // Run the project-specific init
    UsbMscDevice[0].InitializeFunction();
    
    // Unmask all interrupts
    SysUnMaskAllInterrupts(INTERRUPTS_UNMASK_ALL);

    // Enable the USB port
    usb_device_enable(); 
#endif

    // Release if held.
    if (usb_device_is_hold())
    {
      usb_device_release();
    }

    // Discover the Media and its drives
    if(MediaDiscoverAllocation(0) != SUCCESS)
    {
        SystemHalt();
    }

    MediaDiscoverAllocation(1);

#ifdef MMC
    // Full MMC init comes after USB state configured, not here. 
    g_iHighPwrPeriphInitMode = STRUCTURE_INIT_ONLY; // only allowed to do an MMC struct init here.
    // This call only sets structure members and always returns unsuccessful 
    // status since full MMC init is intentionally blocked, so it can be completed 
    // after USB STATE CONFIGURED when the allowed USB current is known. 
    #if 1
    MediaInit(2);
    #else
    if((retCode = MediaInit(2)) != SUCCESS) 
    {   // debugging code
        #if 0 
        if(  (retCode != MMC_MEDIA_ERROR_DEVICE_NOT_INSERTED)
            && (retCode != LMEDIA_ERROR_HARDWARE_FAILURE)       )
        {   SystemHalt();
        } 
        #endif
    }
    #endif
#endif


// Don't need MMC Discover Allocation because it may not be inserted at this time.

    assert(MiscProjectInit()==SUCCESS);

    // Init All Drives
    for(i=2;i<g_wNumDrives;i++)
    {
        DriveInit(i);
    }

#if defined(DEVICE_3500)
    // Initialize the USB MSC driver
    assert(UsbMscDriverInitialize()==SUCCESS);
#endif    

    SysSetResourceTag(DRIVE_TAG_RESOURCE_BIN);

    // Run power initialization
    assert(PowerInit()==SUCCESS);

    //On the 35xx, the speed of the part is determined by the high speed PHY PLL, so no
    //need to call PLLInit here.

    // Delay for a bit so the GPIOs & pad power-downs settle out
    SysDelayMs(5);    
    
#if defined(DEVICE_3410) || defined(DEVICE_3310)
    // **********   note: 3410 not supported in this file now. 
    // Init the Low-resolution ADC BATTERY channel.  Its necessary for all 34xx series
    // devices.  It is not currently used for the STMP35XX.  3410 no longer supported in this main.c!
    SysBatteryLRADCInit();
    // Battery Monitoring must be done only during battery charging
    // The rest of the time the system operates from USB +5V
    // and battery monitoring does not make any sense.
    // Also, battery brownout will be always triggered if system operates
    // with no battery.

    // Init the brownout function of the LRADC battery channel
    SysLRADCBrownoutInit(BATT_BROWNOUT);

    // Initialize the USB port
    assert(usb_device_init()==SUCCESS);

    // Initialize the USB MSC driver
    assert(UsbMscDriverInitialize()==SUCCESS);

    // Install the IRQB vector, enable the IRQB interrupt
    // IPRSetIVLPriority(), called in usb_device_init() clears out the IRQB and IRQA fields
    // So IRQB has to be set here.
    SysLRADCBrownoutIsrInit();

    // 3500 is debounced by hardware during startup
    // Wait a little bit to avoid USB insertion bounce
    SysDelayMs(20);

    // Unmask all interrupts
    SysUnMaskAllInterrupts(INTERRUPTS_UNMASK_ALL);

    // Enable the USB port
    assert(usb_device_enable()== SUCCESS);

#endif

    
    InstrumentInitialize();
    
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          DebugInit
//
//   Type:          Function
//
//   Description:   Debug variable initialization
//
//   Inputs:        none
//
//   Outputs:       RETCODDE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE DebugInit(void)
{
    WORD i;
    WORD j;

    // Init the DCLK count register for debug stuff
    SetDclkCount(0);

    #ifdef SCSI_COMMAND_HISTORY
        // Clear out the command history buffer
        for(i=0;i<SCSI_COMMAND_HISTORY_SIZE;i++)
        {
            CommandHistory[i] = 0xFFFF00;
            for(j=0;j<6;j++)
                CdbHistory[i][j] = 0xAAAAAA;           
        }
    #endif
    
    #ifdef SCSI_DEBUG_READ_COMMAND_HISTORY
        // Clear out the read history buffer
        for(i=0;i<SIZE_OF_READ_HISTORY_BUFFER;i++)
        {
            ReadHistory[i] = 0xFFFFFF;
        }
    #endif
    
    #ifdef SCSI_DEBUG_WRITE_COMMAND_HISTORY
        // Clear out the read history buffer
        for(i=0;i<SIZE_OF_WRITE_HISTORY_BUFFER;i++)
        {
            WriteHistory[i] = 0xFFFFFF;
        }
    #endif
    
    #ifdef SCSI_DEBUG_WRITE_TIME_HISTORY
        // setup the write time history stuff
        dwWriteTimeCurrent = 0;
        dwWriteTimeAverage = 0;
        dwWriteTimeLow = (DWORD)0x100000000000;
        dwWriteTimeHigh = 0;
        wWriteTimeHistoryPointer = 0;
        wWriteTimeHistoryDivideShift = WRITE_TIME_HISTORY_BUFFER_SHIFT;
        wWriteTimeHistoryBufferSize = SIZE_OF_WRITE_TIME_HISTORY_BUFFER;
        for(i=0;i<SIZE_OF_WRITE_TIME_HISTORY_BUFFER;i++)
        {
            dwWriteTimeHistory[i] = 0;
        }

        dwWriteSectorCount = 0;
        dwWriteRandomCount = 0;
        dwWriteSeqFirstCount = 0;
        dwWriteSeqNextCount = 0;
    #endif

    #ifdef SCSI_DEBUG_READ_TIME_HISTORY
        // setup the read time history stuff
        dwReadTimeCurrent = 0;
        dwReadTimeAverage = 0;
        dwReadTimeLow = (DWORD)0x100000000000;
        dwReadTimeHigh = 0;
        wReadTimeHistoryPointer = 0;
        wReadTimeHistoryDivideShift = READ_TIME_HISTORY_BUFFER_SHIFT;
        wReadTimeHistoryBufferSize = SIZE_OF_READ_TIME_HISTORY_BUFFER;
        for(i=0;i<SIZE_OF_READ_TIME_HISTORY_BUFFER;i++)
        {
            dwReadTimeHistory[i] = 0;
        }
    #endif

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          CopyCodeIntoBootmanagerArea
//
//   Type:          Function
//
//   Description:   This function copies functions from Y memory
//                  into the appropriate P memory area to save P 
//                  memory space.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant CopyCodeIntoBootmanagerArea(_Y WORD * _Y pYCopyStart, 
                _P WORD * _P pPCopyStart, WORD wYCopySize)
{
  INT i;

  for (i=0;i<wYCopySize;i++)
  {
    pPCopyStart[i] = pYCopyStart[i];
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ClearMemory
//
//   Type:          Function
//
//   Description:   This function clears Y memory that has been initialized
//                  with P memory functions that are copied to Y memory.  Many 
//                  functions declare this memory as .bss memory and expect it
//                  to be cleared prior to operation.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void ClearMemory(_Y WORD * _Y pYCopyStart, WORD wYClearSize)
{
  INT i;

  // Now clear out the memory that the overlay code was
  for (i=0;i<wYClearSize;i++)
  {
    pYCopyStart[i] = 0;
  }
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          LoadMtp
//
//   Type:          Function
//
//   Description:   This is here as a placeholder and should never be called.
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void LoadMtp(void)
{
    SystemHalt();
}

void doSwitch()
{
    SystemHalt();
}


