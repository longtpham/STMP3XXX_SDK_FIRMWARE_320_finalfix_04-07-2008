/**
***
*** Filename: usbmscMtpSwitch.c
*** Description: Routines required to switch from USBMSC personality
***              to MTP personality.
***
***
*** Copyright (c) SigmaTel, Inc. Unpublished
***
*** SigmaTel, Inc.
*** Proprietary & Confidential
***
*** This source code and the algorithms implemented therein constitute
*** confidential information and may comprise trade secrets of SigmaTel, Inc.
*** or its associates, and any use thereof is subject to the terms and
*** conditions of the Confidential Disclosure Agreement pursuant to which this
*** source code was originally received.
**/

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////
#include "types.h"

#ifdef MTP_BUILD
#include "mtp.h"
#include "usbmscMtpSwitch.h"
#include "MtpHandler.h"
#include "fsproj.h"
#include "extern.h"
#include "mtpInitDevice.h"
#endif

#include "resource.h"
#include "SysResourceApi.h"
#include "usbmsc.h"
#include "ddildl.h"
extern INT g_JanusDrive;
extern void Janus_MTP_Init(int Drivenumber);
extern void _reentrant LoadCode(WORD wResource, MemoryRegion* pRegion);
extern INT  FSDriveInit(INT DeviceNumber);
extern void LoadJanusYConst(void);
extern void LoadAes_Enc_P(void);
extern int Janus_InitializeFlag;
extern int Janus_Wrapper(void);
extern void LoadJanusXConst(void);
extern void LoadJanusCommonConst(void);
extern void SysLoadWrite(void);
extern void LoadMTPDataBase_P(void);
extern void LoadHdsNonovl(void);
extern void LoadDrmHdsImpl(void);
extern WORD g_protectedMode;
////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////     
//
// This is located in overlaymanager.c.
//
extern void _reentrant LoadCode(WORD wResource, MemoryRegion* pRegion);

extern INT _reentrant StorFlush(void);

extern RETCODE _reentrant MtpProjectInitDevice0(void);
#ifdef MTP_BUILD
extern INT SetCWDHandle(INT DeviceNo);
extern void SysLoadJanusSupport(void);
extern void SysLoadDecTable(void);
extern void LoadJanusYData(void);
extern SMtpHandler MtpHandler;
#endif

extern TaskEntry *g_pCurrentTask;
extern MemoryRegion g_SysRegion;

////////////////////////////////////////////////////////////////////////////////
// defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

#ifdef MTP_BUILD
RETCODE _reentrant ExtractPch(SUsbCmdBlock* pCmd, WORD _X* pwBuffer);
BOOL isValidCmdUSBContainer(SUsbCmdBlock* pCmd);
extern _reentrant INT Computefreeluster(INT DeviceNumber);
_reentrant void ClearMemory(_X WORD * _X pXClearStart, WORD wXClearSize);
#endif

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

#ifdef MTP_BUILD
#ifndef USBMSC_HANDLER_STACK_SIZE 
#define USBMSC_HANDLER_STACK_SIZE 1000
#endif 
#ifndef MTP_BUILD
//For Mtp ,Device1CommandOutBuffer moved to usbmtp_cmdoutbuf_ovly_mem.c, to be overlayed with janus sections.
//Increased buffer size to 1.5K words to facilitate faster transfers on send_object calls.
WORD _X Device1CommandOutBuffer[TRANSFER_BUFFER_SIZE];
#endif
WORD _X Device1CommandInBuffer[TRANSFER_BUFFER_SIZE];
WORD g_USBMSCHandler0Stack[USBMSC_HANDLER_STACK_SIZE];
WORD MTPDetected = FALSE;
WORD g_MtpArbitrationDone = 0;
_circ WORD g_USBMSCHandler0MessageQueue[16];
CircularBufferDescriptor g_USBMSCHandler0MessageQueueDescriptor={ g_USBMSCHandler0MessageQueue,g_USBMSCHandler0MessageQueue};



TaskEntry g_USBMSCHandler0 = 
{
    &g_USBMSCHandler0MessageQueueDescriptor,  // Message Queue
    NULL,                           // Waiting on ptr (meaning depends on state)
    0,                              // Waiting on value (meaning depends on state)
    EVENT_TIMEOUT,                  // current state
    g_USBMSCHandler0Stack,          // current stack pointer
    0,                              // Task ID
    TIMEOUT_IMMEDIATE,              // Initial timeout value
    {   //
        // Region requirements
        // 
        RSRC_MTP_HANDLER,           // Task Region Resource
        0,                          // System Region Resource
        RSRC_NANDDATADRIVEWRITESECTOR, // MediaDD Region Resource
        0,                          // Funclet Region Resource
        RSRC_FATWRITE_P_CODE        // FileSystemResource  
    },
    0,                              // Priority
    g_USBMSCHandler0Stack,          // Initial stack ptr (only used in init)
    USBMSC_HANDLER_STACK_SIZE,      // Stack size
    UsbMscHandler0                  // Initial entry point
};
#endif // ifdef MTP_BUILD

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          LoadMtp
//
//   Type:          Function
//
//   Description:   This routine effects switch from USBMSC to MTP.  This
//                  function does not exit.
//
//   Inputs:        none
//
//   Outputs:       none 
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

#ifdef MTP_BUILD

void LoadMtp(void)
{
    extern INT _P _lc_u_bP_FileSystem_cluster;

    //TaskEntry **taskListPtr; // march 2005 commented out unused locals
    //SyncObject *pObject;
    WORD i,Drivenum;
    SUsbCmdBlock* pPCH;
    USBMTP_DEVICE * pDev;
    USHORT usStatus;

    // STMP35xx register to allocate y RAM bank as portion of p and all of y
    WORD _X *pHW_PYCFG = (WORD _X *)0xffe9; 


    // Get back that 8KWords from P and move it to Y via 35xx pycfg register
    // 0x18 assigns 24kWords total Y and the other 0x18 contributes 24kW to P.
    *pHW_PYCFG = 0x1818;

    LoadCode(RSRC_FATFILESYSTEM_MTP, &g_SysRegion);

	LoadMTPDataBase_P();
    
    // 
    // Initialize MTP
    //
    MtpProjectInitDevice0();

    // Now we are ready to process packet which is outstanding.
    
    // Get a local pointer to the device
    pDev = &UsbMtpDevice;

    usb_device_get_status(USB_STATUS, &usStatus);
    if(usStatus == USB_STATE_POWERED)
    {   //We get this state (USB_STATE_POWERED) if we're reset.
        usb_device_set_first_init();

        UsbMscHandler0();
    }

    // Get a local pointer to the PTP Container Header
    pPCH = &(pDev->PtpCmd);

    // Extract the PCH information from the Bulk Out buffer
    ExtractPch(pPCH, pDev->CommandOutBuffer);

    // Verify that this is a valid Command block (Length and Type)
    if (isValidCmdUSBContainer(pPCH) == FALSE)
    {
        //
        //   Invalid command block received: STALL Bulk-in/out endpoints
        //
        Stall();       

        UsbMscHandler0();

    }

    // If control flow reaches this point, MTP has been successfully detected
    MTPDetected = TRUE;
    

    FSInit( bufx, bufy, maxdevices, maxhandles, maxcaches);
      
    FSDriveInit(0);

    Computefreeluster(0);  // Compute free space for NAND only.


    SetCWDHandle(0);
    
//** If MMC is present, Janus Drive is allocated drive number 2 else 1. 
// to initalize g_janusdrive ,pass drivenum to Janus_MTP_Init function.
//it initalize the Janus drive according to passed drive num.
    #ifdef MMC
        Drivenum=2;
    #else 
        Drivenum=1;
    #endif

    Janus_MTP_Init(Drivenum);


    Janus_Wrapper();
    g_protectedMode=0;

    SysLoadWrite();
	LoadHdsNonovl();

    SysCallFunction(RSRC_MTP_INITDEVICE, OneTimeInitDeviceMtpDevice, 0, 0, (int *)pDev);

    // Tell the mtp external media insertion task that we've arbitrated mtp vs.
    // msc This will be unused in usbmsc.
    g_MtpArbitrationDone = TRUE;

    //
    // Parse and process this command. 
    //
    processCommandMtpHandle(&MtpHandler, pPCH);

    UsbMscHandler0();

} /* LoadMtp */
#endif

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UsbMscHandler0
//
//   Type:          Function
//
//   Description:   This routine is entry point for USBMSC task.  If MTP
//                  is detected, this routine becomes MTP task.
//
//   Inputs:        none
//
//   Outputs:       none 
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
#ifdef MTP_BUILD
void _reentrant UsbMscHandler0(void)
{
    int i;
    //Message  Msg;
    for (i=10;i<USBMSC_HANDLER_STACK_SIZE;i++)
    {
        g_USBMSCHandler0Stack[i] = 0xeac0ed;
    } 
    
    while(1)
    {
        if (MTPDetected)
        {
            break;
        }
        else
        {
            UsbMscCbwProcessor(0);
        }
    }

    //
    // Now that we are in MTP we will never go back to USBMSC mode.
    //
    while(1)
    {
        MtpCbwProcessor(0);
    
        //
        // See if the store needs to be flushed.
        //
        MtpCheckStore();
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MtpCheckStore
//
//   Type:          Function
//
//   Description:   This just checks if we need to flush the store/database.
//
//   Inputs:        none
//
//   Outputs:       none 
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
long g_FlushCount=0;
void MtpCheckStore(void)
{
    if (g_StoreWatchDogCount >= g_StoreWatchDogTimeout)
    {
        if ((g_DB_Dirty != 0) || (g_stor_dirty != 0) )
        {
            g_FlushCount++;
            
            if( StorFlush() )
                SystemHalt();
        }
        //
        // Reset the watchdog counter.
        //
        g_StoreWatchDogCount = 0;
    }
}

#else
void _reentrant UsbMscHandler0(void)
{
    assert(UsbMscDriverInitialize()==SUCCESS);

    while(1)
    {
        UsbMscCbwProcessor(0);
    }
}
#endif // MTP_BUILD

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          ClearMemory
//
//   Type:          Function
//
//   Description:   This function clears X memory only.
//
//   Inputs:        _X WORD * _X pXClearStart (ptr in x that points to x)
//                  WORD wXClearSizeInWords
//
//   Outputs:       none
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////

#ifdef MTP_BUILD

_reentrant void ClearMemory(_X WORD * _X pXClearStart, WORD wXClearSizeInWords)
{
  INT i;

  for (i=0;i<wXClearSizeInWords;i++)
  {
    pXClearStart[i] = 0;
  }
}

#endif /* #ifdef MTP_BUILD */
