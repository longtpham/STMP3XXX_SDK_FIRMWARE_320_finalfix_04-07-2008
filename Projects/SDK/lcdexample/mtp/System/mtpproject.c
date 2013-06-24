/**
***
*** Filename: mtpproject.c
*** Description: MTP project initialization functions
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
//////////////////////////////////////////////////////////////////
// Copyright © Microsoft Corporation, 2003
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//   Includes
//////////////////////////////////////////////////////////////////
#include "types.h"
#include "project.h"
#include "sysirq.h"
#include "icoll.h"

#include "filesystem.h"
#include "filespec.h"
#include "usb_api.h"
#include "mtp.h"
#include "ptp.h"
#include "mtphandler.h"
#include "mtprequests.h"
#include "scsi.h"
#include "bootmanager.h"
#include "misc.h"
#include "mediadetection.h"

#include "usbmscMtpSwitch.h"

#ifdef MMC
  #include "MMCddi.h"
  #include "MMCCommon.h"
#endif

    #pragma asm
    nolist
    include "sysmacro.asm"
    list 
    #pragma endasm
    
////////////////////////////////////////////////////////////////////////////////
// Symbol Exports
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Defs
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant MtpProjectInitDevice0(void);
#if ((NUM_REMOVABLE_MEDIA == 1) && (MULTI_LUN_BUILD==0))
RETCODE _reentrant MtpProjectInitDevice1(void);
RETCODE _reentrant MtpProjectResetDevice1(void);
#endif

////////////////////////////////////////////////////////////////////////////////
// Debug Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

USBMTP_DEVICE UsbMtpDevice;

////////////////////////////////////////////////////////////////////////////////
// Extern Variables
////////////////////////////////////////////////////////////////////////////////

extern SMtpHandler MtpHandler;
        
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          MtpProjectInitDevice0
//
//   Type:          Function
//
//   Description:   Project-specific USB MSC device init for device 0
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         none
//
////////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant MtpProjectInitDevice0(void)
{
    
    USBMTP_DEVICE * pDev;
    WORD i;
    
    // Device 1 setup
    pDev = &UsbMtpDevice;

    pDev->wDeviceNumber = 0;
    pDev->wBulkInEndpointNum=1;

    // Setup the BulkOut Endpoint
    pDev->wBulkOutEndpointNum=2;

    pDev->wInterruptEndpointNum=3;
	
    pDev->wInterface = 0;

    initMtpHandle(&MtpHandler, &UsbMtpDevice);

    usb_device_register_service(pDev->wBulkInEndpointNum,MtpBulkInService);
    usb_device_register_service(pDev->wBulkOutEndpointNum,MtpBulkOutService);
    usb_device_register_service(pDev->wInterruptEndpointNum,MtpInterruptService);
    usb_device_register_service(0,MTPRequestHandler);

    usb_device_register_service(USB_SERVICE_BUS_RESET,MtpResetService);

    usb_device_register_service(USB_SERVICE_SUSPEND,MtpSuspendService);
    usb_device_register_service(USB_SERVICE_RESUME,MtpResumeService);
    pDev->CommandInBuffer  = Device1CommandInBuffer;
    pDev->CommandOutBuffer = Device1CommandOutBuffer;
//    pDev->InterruptBuffer  = Device1InterruptBuffer;

    return SUCCESS;
}

static void initMtpHandle(SMtpHandler *pHandle, USBMTP_DEVICE*pDevice)
{
    int ii;

    pHandle->pDevice = pDevice;
    pHandle->SessionId = 0;
    pHandle->TransactionId = 0;
    
    pHandle->Data4SendObject.StoreID = 0;
    pHandle->Data4SendObject.hParentHandle = 0;
    pHandle->Data4SendObject.hObjectHandle = 0;
    pHandle->Data4SendObject.bValid = 0;
    pHandle->bMTPReset = FALSE;

}
