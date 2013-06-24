///////////////////////////////////////////////////////////////////////////////
//!
//! \file system.c
//! \brief
//!  This file holds the code for the UI task.
//!
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////
#include "kernel.h"
#include "project.h"
#include "messages.h"
#include "mtp.h"
#include "scsi.h"
#include "misc.h"
#include "batterycharge.h"
#include "resource.h"

#if (NUM_REMOVABLE_MEDIA == 1)
#include "fsproj.h"
#include "fstypes.h"
#include "ddildl.h"
#include "fsapi.h"
#include "usbmsc.h"
#endif


#ifdef MMC
  #include "MMCddi.h"
  #include "MMCCommon.h"
#endif


#include "regsdcdc.h"
#include "regslradc.h"
#include "regsrevision.h"
#include "sectordef.h"
#include "usb_api.h" // Added oct 20 2005 for


extern TaskEntry g_DisplayTaskEntry;
extern CircularBufferDescriptor g_DisplayTaskQueueDescriptor;

extern TaskEntry g_USBMSCHandler0;
extern TaskEntry g_OverlayManagerTaskEntry;

extern WORD MTPDetected;

extern _X  BOOL g_bServiceDCDC;

extern volatile INT g_DB_Dirty;
extern volatile INT g_stor_dirty;

//
// Global which reflects external media presence
//
BOOL g_bExternalMediaConnected = FALSE;

#if (NUM_REMOVABLE_MEDIA == 1)
int const g_NumStores=2;
//
// This needs to be true to make the enumeration wait for the MMC device to be
// initialized.
//
WORD bMMCInit = TRUE;
#else
int const g_NumStores=1;
//
// This needs to be false if there is no MMC device or the enumeration will
// not complete.
//
WORD bMMCInit = FALSE;
#endif // #if (NUM_REMOVABLE_MEDIA == 1)

#if (NUM_REMOVABLE_MEDIA == 1)
void _reentrant ScsiInsertionRemovalCode(void);
//extern TaskEntry g_MtpDeviceInsertion;
void _reentrant MtpCheckExternalMedia(void);
// If a project such as a test uses MMC but not USB, it needs to bypass the USB 
// current check in the MMC driver, it must include this global define & init value.
// Two lines Needed for MMC driver to link and fully enumerate. Simulates granting of high current by USB host. 
// #define USB_HIGH_CURRENT_GRANTED 3
// int g_iHighPwrPeriphInitMode = USB_HIGH_CURRENT_GRANTED;
BOOL bFirstFullMmcInitSinceReset = TRUE;
#define  MEDIA_INSERTION_DELAY  50                      // MMC insertion debounce time    
#define  MMC_MEDIA_NUM          1                       // Temporary. find the right way to do this
#define  MMC_DRIVE_NUM          1                       // Temporary. find the right way to do this
extern _reentrant INT Computefreeluster(INT DeviceNumber);
extern WORD g_MtpArbitrationDone;
#endif

volatile WORD g_wActivityState = SCSI_IDLE;

// This variable was introduced in SDK3.0 to solve refreshing display when using multi writes
// in a system using overlays. Overlays will disturb multi write sequence.
volatile WORD g_wActivityStateMultiWrite = SCSI_IDLE;
BOOL gMscMtpEnable = TRUE;

//The task entry points can be declared here, or from an include file.
_reentrant void UserInterfaceTask(void);

//!
//! The size of the user interface task's stack.
//!
#define USER_INTERFACE_STACK_SIZE 200

//!
//! This is the user interface task's stack.
//!
WORD g_UserInterfaceStack[USER_INTERFACE_STACK_SIZE];

//!
//! This is the user interfaces task message queues.
//!
_circ WORD g_UserInterfaceMessageQueue[32];

CircularBufferDescriptor g_UserInterfaceQueueDescriptor={ g_UserInterfaceMessageQueue,g_UserInterfaceMessageQueue};

//!
//! This is the Task Entry for the User interface task.
//!
TaskEntry g_UserInterfaceTaskEntry = 
{
    &g_UserInterfaceQueueDescriptor, //!< Message queue associated with the task
    0,                               //!< Pointer to wait on (meaning depends on state)
    0,                               //!< Word to wait on (meaning depends on state)
    EVENT_TIMEOUT,                   //!< Task's current State
    g_UserInterfaceStack,            //!< Current stack pointer
    0,                               //!< Task ID
    TIMEOUT_IMMEDIATE,               //!< initial timeout
    {
        RSRC_TASK_UI_CODEBANK,       //!< Region resources
        0,                           // System Region Resource
        RSRC_NANDDATADRIVEWRITESECTOR, // MediaDD Region Resource
        0,                          // Funclet Region Resource
        RSRC_FATWRITE_P_CODE        // FileSystemResource  
    },
  
    0,                               //!< Priority
    g_UserInterfaceStack,            //!< stack pointer
    USER_INTERFACE_STACK_SIZE,       //!< stack size
    UserInterfaceTask                //!< Entry point
};              


TaskEntry *g_TaskList[]={
                            &g_UserInterfaceTaskEntry,
                            &g_DisplayTaskEntry,
                            &g_USBMSCHandler0,
                            &g_OverlayManagerTaskEntry,
                            NULL
                        };

//the routing table describes which messages need to go to which queues.
RoutingEntry g_RoutingTable[] = 
{
    {MSG_TYPE_LCD, &g_DisplayTaskQueueDescriptor},
    NULL
};

#if MTP_BUILD
// MTP libs and app need a 683 word = 2048byte buffer for sdk3.1. TOVERIFY4
#undef SCRATCH_USER_Y_SIZE
#define SCRATCH_USER_Y_SIZE SEC_2K_BYTES_WORDS_PER_SECTOR_NO_RA
#endif

SyncObject g_Sync_UserScratchY;
_Y WORD g_wUserScratchY[SCRATCH_USER_Y_SIZE]; // sdk3.100.017 had this as 512 words in mtp says map file. A later ver will be 683.


// Size Note: sdk3.100.017: SCRATCH_USER_X_SIZE is 256 words in player, 512 words in mtp says map file.
SyncObject g_Sync_UserScratchY;
_X WORD g_wUserScratchX[SCRATCH_USER_X_SIZE]; 

BOOL g_bShutdown = FALSE;

extern int g_iCommonCommands;
extern int g_iUpdaterCommands;
extern int g_iUnknownCommands;

extern void USBLCDDisplayInit(void);
extern void USBLCDCheckForTransfers(void);
extern void USBLCDIdle(void);
extern void USBLCDReading(void);
extern void USBLCDWriting(void);
extern void USBLCDCheckBrownout(void);
extern void USBLCDDisplayBatteryLevel(void);
extern void USBLCDCheckBatteryLevel(void);
extern void USBLCDLowBattery(void);
extern void USBLCDDoNotDisconnect(void);
extern void USBLCDCheckDBStoreDirty(void);

////////////////////////////////////////////////////////////////////////////////
// Defines
// #define USB_THROUGHPUT          1
#define BOOST_NLEV_BASE_STEP  46


#ifdef  USB_THROUGHPUT
  #define UPDATE_RATE           1000
#else
  #define UPDATE_RATE           100
#endif
#define HALF_SEC_UPDATE_RATE    (500/UPDATE_RATE)

#define WRITE_STATE_PERSISTENT_TIME (2*HALF_SEC_UPDATE_RATE)

//! Right now this is set to 2 seconds of no activity before the background
//! system task closes the store files and flushes the the filesystem.
const INT g_StoreWatchDogTimeout = 2000/UPDATE_RATE;

//! This is the variable that will count the number of times the UserInterface
//! task runs before closing the store files and flushing the filesystem.
volatile INT g_StoreWatchDogCount=0;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////
#ifndef USB_THROUGHPUT
// Function Description: 
// Inputs:  no parameters
// Returns: no register returns
// Notes:   
void _reentrant UserInterfaceTask(void)
{
    USHORT      usStatus;
    //int iTransferRate;
    WORD State=SCSI_IDLE;
    //Message Msg;
    int iDelay = HALF_SEC_UPDATE_RATE;
    BOOL bNeedToReEnable = FALSE;
    int Write_State_Persisten_Time = WRITE_STATE_PERSISTENT_TIME; 
    int i;
    
    for (i=10;i< USER_INTERFACE_STACK_SIZE;i++ )
    {   g_UserInterfaceStack[i]=0xc0ffee;
    }

    Init5VSense();

    USBLCDDisplayInit();

    SysWait(250);

    while(1)
    {
    #if (NUM_REMOVABLE_MEDIA == 1)
        // Wait until we have decided whether device is in MTP vs. MSC mode
        // Media detection uses different code for the two modes
        if(g_MtpArbitrationDone)
        {   if(MTPDetected)
            {   MtpCheckExternalMedia();
            }
            else
            {   ScsiInsertionRemovalCode();                
            }
        }
    #endif

        usb_device_get_status(USB_STATUS, &usStatus);
        if(usStatus == USB_STATE_SUSPENDED)
        {   SysWait(UPDATE_RATE);
        }
        else
        {   SysPostMessage(2,LCD_END_FRAME);
            SysWait(UPDATE_RATE);
            SysPostMessage(2,LCD_BEGIN_FRAME);
            USBLCDCheckForTransfers();
            
            if(g_bServiceDCDC)
            {   ServiceDCDC();  //Service DCDC converter
            }
        #ifdef BATTERY_CHARGE
            // For 3500 Battery Monitoring must be done only during battery charging
            // The rest of the time the system operates from USB +5V
            // and battery monitoring does not make any sense.
            // Also, battery brownout always triggered if sys operates w/ no battery.

            //Only charge if we're in high usb current mode. March 11 2005 addition
            if(usb_get_current_limit() <= 100) //mA
            {    BatteryChargeDisableCharging(TRUE); // until next stmp bootup
            }
            else
            {   if(usStatus !=  USB_STATE_CONFIGURED)
                {
                    if(bNeedToReEnable == FALSE)
                    {   BatteryChargeDisableCharging(FALSE);
                        bNeedToReEnable = TRUE;
                    }
                }
                else 
                {   if(bNeedToReEnable == TRUE)
                    {   BatteryChargeEnableCharging();
                        bNeedToReEnable = FALSE;
                    }                
                    BatteryChargeStateMachine();
                }
            }
        #endif

#if defined(BATTERY_CHARGE)            
            // I only want to update the Battery icon once per 1/2 second
            // but always check and average battery level.
            USBLCDCheckBatteryLevel();
#endif            
            if (iDelay-- <= 0)
            {
#if defined(BATTERY_CHARGE)               
                USBLCDDisplayBatteryLevel();
#endif                                
                USBLCDCheckDBStoreDirty();
                iDelay = HALF_SEC_UPDATE_RATE;
            }


            // g_wActivityState - This global is set by other code to indicate the state of the
            // system.
            // Can be set by SCSI code, or MTP code (depending on the connection)

            // Modification added for MTP/MSC system
            // In MTP/MSC, MSC uses overlays thus we need to be careful yielding to the kernel for display updates.
            // This can not be done in the middle of a multi write sequence.
            // The code was modified and uses the same technique as a monostable device. Each time scsi write is entered
            // the global g_wActivityStateMultiWrite is set to SCSI_WRITING. This will trigger the monostable for  
            // WRITE_STATE_PERSISTENT_TIME miliseconds. If a new scsi write command arrives prior WRITE_STATE_PERSISTENT_TIME
            // expires, the new scsi write command re-starts the monostable for a new full WRITE_STATE_PERSISTENT_TIME ms.
            // Once the monostable time expires without no new SCSI write commands, the state will switch to IDLE and display 
            // will return to the READY state.
            // The display task has a chance to run everytime a new SCSI write command arrives. There is no display refresh
            // during mutltiwrites. 
            if((g_wActivityStateMultiWrite == SCSI_WRITING) || (Write_State_Persisten_Time < WRITE_STATE_PERSISTENT_TIME))
            {
                if(g_wActivityStateMultiWrite == SCSI_WRITING)
                {
                    g_wActivityStateMultiWrite = SCSI_IDLE;
                    Write_State_Persisten_Time = 0;
                    
                    if(State != SCSI_WRITING)
                    {   State = SCSI_WRITING;
                        USBLCDWriting();
                    }
                }
                else
                {   Write_State_Persisten_Time += UPDATE_RATE;
                }
            }   
            else
            {
                if(State!= g_wActivityState )
                {   State = g_wActivityState;
                    switch(State)
                    {
                    case SCSI_IDLE:
                        USBLCDIdle();
                        break;
                    case SCSI_READING:
                        USBLCDReading();
                        break;
                    case SCSI_WRITING:
                        USBLCDWriting();
                        break;
                    default :
                        USBLCDIdle();
                    }
                }
            }
        } // end else !suspended

        {   USHORT usStatus;  // fixes 8777 (0 or 2 usb unplug tries out of 20 would not 
                              // boot player & mtp was still running due to rare irq loss)
            usb_device_get_status(USB_STATUS_CONNECTION,&usStatus); //we could just check the usb 5v bit instead of calling this.
            if( usStatus == USB_DISCONNECTED )
            {   // Restart the MTP device and re-enumerate
                //DebugBuildAssert(0); //During JLN verification, this hit & the reset started player. 
                StorFlush();   // flush the database first
                SystemShutdown();// we could use pragma asm to jump to the missed isr instead which resets also.
                SystemReset();
            }
        }
        
        // Update the count if it is not disabled( negative means disabled).
        if (g_StoreWatchDogCount >= 0)
        {   g_StoreWatchDogCount++;
        }
        
    } // while(1)
} // void _reentrant UserInterfaceTask(void)
#endif // ifndef USB_THROUGHPUT


#if (NUM_REMOVABLE_MEDIA == 1)
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ScsiInsertionRemovalCode
//
//   Type:          Function
//
//   Description:   Called by system task
//
//   Inputs:        none
//
//   Outputs:       none 
//
//   Notes:         If we are in MSC mode (connected to non-MTP system) use this code taken from the SCSI driver
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant ScsiInsertionRemovalCode(void)
{
    SCSI_LUN * pLun;
    BOOL bChanged;
    WORD i;
    WORD j;
    //WORD k;
    WORD wFsLun;
      //   Loop through all MSC devices    
      for(i=0;i<g_wUsbMscNumDevices ;i++)
      {
        // Loop through each LUN
        for(j=0;j<UsbMscDevice[i].wNumLunsSupported;j++)
        {
          // Only check if it is removable
          if(UsbMscDevice[i].Lun[j].bMediaIsRemovable)
          {
              pLun = &UsbMscDevice[i].Lun[j];
              wFsLun = pLun->wFileSystemDriveNumber; 

              // Has the media changed?
              DriveGetInfo(wFsLun,DriveInfoMediaChange,&bChanged);

              if (  bChanged
                             /* checks for USB STATE CONFIGURED */
                    || ((g_iHighPwrPeriphInitMode==USB_HIGH_CURRENT_GRANTED) && (bFirstFullMmcInitSinceReset==TRUE))
                 )
              {
                // if arrival is due to USB STATE CONFIGURED, this ScsiLunInitialize completes MMC init
                if( (g_iHighPwrPeriphInitMode==USB_HIGH_CURRENT_GRANTED) && (bFirstFullMmcInitSinceReset==TRUE) ) 
                {   bFirstFullMmcInitSinceReset = FALSE;
                    //DebugBuildAssert(jCallingScsiLunInit==0); //emulator can enable
                } 
                                                    
                // This one changed.
                pLun->bMediaChanged = TRUE;
                            
                // Debounce
                SysDelayMs(MEDIA_INSERTION_DELAY);    

                pLun->bMediaInsertionDetected = TRUE;
                          
                //pLun->FlagMediaChange();
                // Function set in usbmscproject.c, above call is fine in USBMSC project,
                // but in MTP project, this code is in a resource, so use SysCallFunction.
                SysCallFunction(RSRC_MMCMEDIAGETINFO, MmcFlagMediaChange, 0, 0, 0);
                

                // Let's initialize it.  If removed then the init will
                //  take care of reseting some key variables.
                if( ScsiLunInitialize(i, j) != SUCCESS) 
                {   //DebugBuildAssert(FALSE); // MMC init errors deemed non fatal so commented out unless debugging/profiling. 
                }
            }
          }
        }            
      }
}

///////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MtpCheckExternalMedia
//
//   Type:          Function
//
//   Description:   Called by system task
//
//   Inputs:        none
//
//   Outputs:       none 
//
//   Notes:         MTP gets one chance to properly initialize an inserted external media card
//                  After usb has been initialized, if media is present, initialize it. Then StorInit() can run
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant MtpCheckExternalMedia(void)
{
    BOOL bExternalMediaChanged = FALSE;
    BOOL bMMCPresent = FALSE;
    WORD retCode = !SUCCESS;
    WORD i;

    // Find out if media presence has changed
    DriveGetInfo(MMC_DRIVE_NUM,DriveInfoMediaChange,&bExternalMediaChanged);
    
    // bExternalMediaChanged should be yes if it's started up inserted

    if(bMMCInit || bExternalMediaChanged)
    {
       // Debounce media insertion/removal
        SysDelayMs(MEDIA_INSERTION_DELAY);    

        // Is the media connected or disconnected?
        retCode = DriveGetInfo(MMC_DRIVE_NUM,DriveInfoMediaPresent,&bMMCPresent);
    }

    if(bMMCInit)
    {
        if((retCode == SUCCESS) && g_MtpArbitrationDone)
        {
            if((g_iHighPwrPeriphInitMode == USB_HIGH_CURRENT_GRANTED) && (bMMCPresent))
            {
                // Conditions are okay for MMC initialization to complete.
                // Initialize the MMC media, drive, and filesystem

                // Complete the final phase of MMC media init
                MediaInit(MMC_MEDIA_NUM);

                // Init filesystem for the MMC data drive
                FSDriveInit(MMC_DRIVE_NUM);

                // Load freecluster code resource
                SysLoadMtpBootInit();
                
                // Needed to complete MMC filesystem initialization
                Computefreeluster(MMC_DRIVE_NUM);

                // Reload the FS write resource
                SysLoadWrite();
            
                // Firmware can poll this global to check external media presence
                g_bExternalMediaConnected = TRUE;                        
            }
          
            else
            {
                g_bExternalMediaConnected = FALSE;
            }
        
            // StorInit() watches bMMCInit, and waits for it to become FALSE before carrying out its initialization
            bMMCInit = FALSE;
       }
    }
    else if(bExternalMediaChanged)
    {
        if((retCode == SUCCESS) && (bMMCPresent))
        {   
            // If, after the system has started and been initialized in MTP mode, external media is inserted, 
            // we can't notify the host, so do nothing.
            ; // Do nothing...
        }
        else
        {   
            // Signal firmware that media has been removed
            g_bExternalMediaConnected = FALSE;
        }
    }
}
#endif  // #if (NUM_REMOVABLE_MEDIA == 1)









// Alternative code for measuring and displaying USB speed
/////////////////////////////////////////////////////////////////////////////////////
// Can be used to measure and display USB transfer rates
// Uncomment #define USB_THROUGHPUT to enable
#ifdef USB_THROUGHPUT
// Function Description: 
// Inputs:  no parameters
// Returns: no register returns
// Notes:   
void _reentrant UserInterfaceTask(void)
{
    int iTransferRate;
    WORD State=SCSI_IDLE;
    //Message Msg;
    int iDelay = HALF_SEC_UPDATE_RATE;
    int Image2Display = 0;
    BOOL bNeedToReEnable = FALSE;
    USHORT usStatus;

    SysPostMessage(5,LCD_CLEAR_RANGE,0,0,98,64);
    SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,ReadyImage);
    SysWait(250);

    while(1)
    {
    #ifdef BATTERY_CHARGE
        //Only charge if we're in high usb current mode. March 11 2005 addition
        if(usb_get_current_limit() <= 100) //mA
        {    BatteryChargeDisableCharging(TRUE); // until next stmp bootup
        }
        else
        {   usb_device_get_status(USB_STATUS, &usStatus);

            if(usStatus !=  USB_STATE_CONFIGURED)
            {
                if(bNeedToReEnable == FALSE)
                {   BatteryChargeDisableCharging(FALSE);
                    bNeedToReEnable = TRUE;
                }
            }
            else
            {
                if(bNeedToReEnable == TRUE)
                {   BatteryChargeEnableCharging();
                    bNeedToReEnable = FALSE;
                }    
        
                BatteryChargeStateMachine();
            }
        }
    #endif
        SysWait(UPDATE_RATE);
        if(State!= g_wActivityState )
        {
            State = g_wActivityState;
            switch(State)
            {
            case SCSI_IDLE:
                Image2Display = ReadyImage;   
                break;
            case SCSI_READING:
                Image2Display = ReadingImage;  
                break;
            case SCSI_WRITING:
                Image2Display = WritingImage; 
                break;
            }
            if (0 != Image2Display) {
                SysPostMessage(5,LCD_PRINT_RANGE_RSRC,0,0,Image2Display);
            }
        }
        iTransferRate = g_lBulkInBytes / (1+g_lLastBulkInTime-g_lFirstBulkInTime);
        SysPostMessage(7,LCD_PRINT_NUMBER,0,16,iTransferRate,5,' ');
        iTransferRate = g_lBulkOutBytes / (1+g_lLastBulkOutTime-g_lFirstBulkOutTime);
        SysPostMessage(7,LCD_PRINT_NUMBER,0,24,iTransferRate,5,' ');

        SysWait(0);
        SysPostMessage(7,LCD_PRINT_NUMBER, 0,40,g_iCommonCommands,5,' ');
        SysPostMessage(7,LCD_PRINT_NUMBER,32,40,g_iUpdaterCommands,5,' ');
        SysPostMessage(7,LCD_PRINT_NUMBER,64,40,g_iUnknownCommands,5,' ');
    }// end while(1)
}
#endif

//#undef FUNCLET
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ServiceDCDC
//
//   Type:          Function
//
//   Description:   Service the DCDC converter to prepare for disconnect
//
//   Inputs:        none
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
///#ifdef FUNCLET
///    #pragma asm
///        FServiceDCDC:
///
///        nolist
///        include "sysmacro.asm"
///        include "resource.inc"
///        list
///
///        ;If using funclet, function will be executed via funclet kernel
///        CallFunclet RSRC_FUNCLET_SERVICEDCDC
///
///        org p,"RSRC_FUNCLET_SERVICEDCDC_P":
///        dc      RSRC_FUNCLET_SERVICEDCDC
///    #pragma endasm
///void _reentrant ServiceDCDC_Funclet(void)
///#else
void _reentrant ServiceDCDC(void)
///#endif
{     
//Only add code if using 5V to DCDC Power Transfer
#ifdef DCDC_POWER_TRANSFER

    INT iBattLevel=0;
    INT iNLevel=0;

    if(g_bServiceDCDC)    //service DCDC converter if flag is set
    {         
        if((HW_VDD5V_PWR_CHARGE.B.VDD5V_PRESENT) || (HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT))
        {
//Build option to service DCDC based on DCDC Mode (boost or buck)
#ifdef BATTERY_TYPE_LI_ION
            // Open DCDC control loop and set it up to be close to desired target for disconnect.
            HW_DCDC1_CTRL1.B.R = 9;
            HW_DCDC1_CTRL1.B.C = 0;
            HW_DCDC2_CTRL1.B.R = 9;
            HW_DCDC2_CTRL1.B.C = 0;

            //Check for Single Channel Buck.  DCDCMODE=1 for 100-pin package.  DCDCMODE=3 if 144-pin package
            //is forced to single channel buck
            if ((HW_REVR.B.DCDCMODE == 0x1)||(HW_REVR.B.DCDCMODE == 0x3))
            {   
                // Control loop is in a bad state
                HW_DCDC1_CTRL0.B.NLEV = 0x1A;
                SysWait(1);
                HW_DCDC1_CTRL0.B.NLEV = 0x12;                
                // Ring OSC1 should be around 0x78 at this point
                g_bServiceDCDC = FALSE;    //control loop in a good state; stop service
                HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;  //Clear PWDN_ON_IOBRNOUT bit
            }
            else
            {   // 2 Channel buck mode
                // Ring OSC2 should be between 0x20 and 0x30 after the loop runs a few times
                if((HW_SPEED.B.RINGOSC2 < 0x27) || (HW_SPEED.B.RINGOSC2 > 0x30))
                {  
                    // Control loop is in a bad state
                    HW_DCDC2_CTRL1.B.FFOR = 1;
                    SysWait(1);
                    HW_DCDC2_CTRL1.B.FFOR = 0;
                }
                else
                {
                    g_bServiceDCDC = FALSE;    //control loop in a good state; stop service     
                    HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;  //Clear PWDN_ON_IOBRNOUT bit
                    HW_DCDC2_CTRL0.B.NLEV = 0x1F;  //Now change NLEV to Buck State
                }

                // Control loop is in a bad state
                HW_DCDC1_CTRL0.B.NLEV = 0x1A;
                SysWait(1);
                HW_DCDC1_CTRL0.B.NLEV = 0x12;                
                // Ring OSC1 should be around 0x78 at this point
            }
#else //Boost Player
            // Open DCDC control loop and set it up to be close to desired target for disconnect.
            HW_DCDC1_CTRL1.B.R = 9;
            HW_DCDC1_CTRL1.B.C = 0;
            HW_DCDC2_CTRL1.B.R = 9;
            HW_DCDC2_CTRL1.B.C = 0;

            // boost mode
            //Check the battery level and updated if it has changed.  Required for Boost Mode.  
            iBattLevel=HW_BATT_RESULT.B.DATA_OUT;

            // alter NLEV and return back to step control loop
            //scale NLEV based on battery voltage.  if >1.44 NLEV=06, 1.44V-0.9V NLEV=06 to NLEV=31 
            iNLevel = 31-(iBattLevel-BOOST_NLEV_BASE_STEP);   //assumes HW_BATT_CTRL.I = 0x020200 (steps 46-72)
            if (iNLevel < 6)
                iNLevel = 6;
            else if (iNLevel > 31)
                iNLevel = 31;

            HW_DCDC1_CTRL0.B.NLEV = iNLevel; 
            
            //Control Loop in an open state, clear PWDN_ON_IOBRNOUT bit
            HW_VDD5V_PWR_CHARGE.B.PWDN_ON_IOBRNOUT=0;                       
            
            // disable battery brownout for case with 5V present and battery removed
            // ** currently disabled in usbmsc in SDK ** add code if change SDK
            
#endif
        }
        else  //5V is not connected,  close DCDC control loop
        {
//Build option to service DCDC based on DCDC Mode (boost or buck)
#ifdef BATTERY_TYPE_LI_ION
            //close DCDC Control loop
            HW_DCDC1_CTRL1.B.R = 5;
            HW_DCDC1_CTRL1.B.C = 4;
            HW_DCDC2_CTRL1.B.R = 5;
            HW_DCDC2_CTRL1.B.C = 4;
#else
            //close DCDC Control loop
            HW_DCDC1_CTRL1.B.R = 3;
            HW_DCDC1_CTRL1.B.C = 4;
            HW_DCDC2_CTRL1.B.R = 3;
            HW_DCDC2_CTRL1.B.C = 4;
#endif
        }
  } //end g_bServiceDCDC
#endif  //DCDC_POWER_TRANSFER


}  //end Check5V
