////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: usbmscproject.c
// Description: Project-specific USB MSC
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//   Includes and external references
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "project.h"
#include "sysirq.h"
#include "icoll.h"

#include "filesystem.h"
#include "usb_api.h"
#include "usbmsc.h"
#include "usbmscrequests.h"
#include "scsi.h"
#include "bootmanager.h"
#include "misc.h"
#include "mediadetection.h"
#include "nandhal.h"
#include "sectordef.h"

#ifdef MTP_BUILD
#include "mtp.h"
#include "usbmscMtpSwitch.h"
#endif

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

RETCODE UsbMscProjectInitDevice0(void);
RETCODE UsbMscProjectResetDevice0(void);
#if ((NUM_REMOVABLE_MEDIA == 1) && (MULTI_LUN_BUILD==0))
RETCODE UsbMscProjectInitDevice1(void);
RETCODE UsbMscProjectResetDevice1(void);
#endif

////////////////////////////////////////////////////////////////////////////////
// Debug Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

WORD g_wUsbMscNumDevices = USBMSC_NUM_DEVICES;

#ifndef MTP_BUILD

WORD _X Device1CommandInBuffer[64];
WORD _X Device1CommandOutBuffer[64];

USBMSC_DEVICE UsbMscDevice[SCSI_NUM_TARGETS];

#ifndef USBMSC_HANDLER_STACK_SIZE 
#define USBMSC_HANDLER_STACK_SIZE 500
#endif 

WORD g_USBMSCHandler0Stack[USBMSC_HANDLER_STACK_SIZE];

_circ WORD g_USBMSCHandler0MessageQueue[16];
CircularBufferDescriptor g_USBMSCHandler0MessageQueueDescriptor={ g_USBMSCHandler0MessageQueue,g_USBMSCHandler0MessageQueue};
TaskEntry g_USBMSCHandler0 = {&g_USBMSCHandler0MessageQueueDescriptor,  //Message queue
                                NULL,                                   //Waiting on Ptr (meaning depends on state)
                                0,                                      //waiting on value (meaning depends on state)
                                EVENT_TIMEOUT,                          //current state
                                g_USBMSCHandler0Stack,                  //current stack pointer
                                0,                                      //Task ID
                                TIMEOUT_IMMEDIATE,                      //Initial timeout value
                                {0},                                    //Region requirements
                                0,                                      //Priority
                                g_USBMSCHandler0Stack,                  //Initial stack ptr (only used in init)
                                USBMSC_HANDLER_STACK_SIZE,              //Stack size
                                UsbMscHandler0};                        //Initial entry point


SECTOR_TYPE Target0SectorArray[USBMSC_MAX_NUM_SECTOR_BUFFERS];
WORD _X Target0BufferArray[USBMSC_TOTAL_BUFFER_SIZE];

#else    			// ifndef MTP_BUILD


extern SECTOR_TYPE Target0SectorArray[USBMSC_MAX_NUM_SECTOR_BUFFERS];
extern WORD _X Target0BufferArray[USBMSC_TOTAL_BUFFER_SIZE];

extern USBMSC_DEVICE UsbMscDevice[SCSI_NUM_TARGETS];
#endif    			// ifndef MTP_BUILD
        
////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UsbMscProjectInitDevice0
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
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE UsbMscProjectInitDevice0(void)
{
    WORD sectorBufferSizeInWords;
    USBMSC_DEVICE * pDev;
    WORD i;
    
    // Device 1 setup
    pDev = &(UsbMscDevice[USBMSC_DEVICE_0]);

    // Run the reset routine
    UsbMscProjectResetDevice0();

    #if defined(DEVICE_3410) || defined(DEVICE_3310)
    
        usb_device_init_endpoint(pDev->wBulkInEndpointNum ,64,USB_IN,USB_BULK_ENDPOINT,FALSE);
        usb_device_init_endpoint(pDev->wBulkOutEndpointNum,64,USB_OUT,USB_BULK_ENDPOINT,FALSE);
        
    #else

        // Do nothing.
        // The endpoints will be initialized in the UsbMscCbwProcessor()
    
    #endif
    
    usb_device_register_service(pDev->wBulkInEndpointNum,UsbMscBulkInService);
    usb_device_register_service(pDev->wBulkOutEndpointNum,UsbMscBulkOutService);
    usb_device_register_service(0,USBMSCRequestHandler);

    usb_device_register_service(USB_SERVICE_BUS_RESET,UsbMscResetService);

#ifdef DEVICE_3500
    usb_device_register_service(USB_SERVICE_SUSPEND,UsbMscSuspendService);
    usb_device_register_service(USB_SERVICE_RESUME,UsbMscResumeService);
#endif

    for(i=0;i<SCSI_DEVICE_NUM_LUNS_DEV_0;i++)
    {
        ScsiLunSetupDefaultSenseData(&(pDev->Lun[i]));
        pDev->Lun[i].wCompletionStatus = USBMSC_CSW_COMMAND_PASSED;
        pDev->Lun[i].dwNumBytesXferred = 0;
    }

    // Lun 0 stuff
    pDev->Lun[0].wInquiryVendorData[0] = (g_SCSIVendorID[0]<<16);
    pDev->Lun[0].wInquiryVendorData[1] = g_SCSIVendorID[1] | 
        (g_SCSIVendorID[2]<<8) |
        (g_SCSIVendorID[3]<<16);
    pDev->Lun[0].wInquiryVendorData[2] = g_SCSIVendorID[4] | 
        (g_SCSIVendorID[5]<<8) |
        (g_SCSIVendorID[6]<<16);
    pDev->Lun[0].wInquiryVendorData[3] = g_SCSIVendorID[7] | 
        (g_SCSIProductID[0]<<8) |
        (g_SCSIProductID[1]<<16);
    pDev->Lun[0].wInquiryVendorData[4] = g_SCSIProductID[2] | 
        (g_SCSIProductID[3]<<8) |
        (g_SCSIProductID[4]<<16);
    pDev->Lun[0].wInquiryVendorData[5] = g_SCSIProductID[5] | 
        (g_SCSIProductID[6]<<8) |
        (g_SCSIProductID[7]<<16);
    pDev->Lun[0].wInquiryVendorData[6] = g_SCSIProductID[8] | 
        (g_SCSIProductID[9]<<8) |
        (g_SCSIProductID[10]<<16);
    pDev->Lun[0].wInquiryVendorData[7] = g_SCSIProductID[11] | 
        (g_SCSIProductID[12]<<8) |
        (g_SCSIProductID[13]<<16);
    pDev->Lun[0].wInquiryVendorData[8] = g_SCSIProductID[14] | 
        (g_SCSIProductID[15]<<8) |
        ('0'<<16);
    pDev->Lun[0].wInquiryVendorData[9] = '1' | 
        ('0'<<8) |
        ('0'<<16);

    pDev->Lun[0].wInquiryVendorSpecific[0] = 
        (0<<20) | (pDev->wDeviceNumber<<16) |             // Lun 0, Device num
        (SCSI_INQUIRY_VENDOR_SPECIFIC_STRUCT_MDS<<8) | 1; // MDS, 1 struct       
    pDev->Lun[0].wInquiryVendorSpecific[1] = 
        (SCSI_MDS_MEDIA_TYPE_NAND<<8) |                   // Nand
        SCSI_MDS_FLAGS_SYSTEM_MEDIA_FLAG_SYSTEM;          // System media 
    pDev->Lun[0].wInquiryVendorSpecific[2] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[3] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[4] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[5] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[6] = 0;
    pDev->Lun[0].FlagMediaChange = ScsiDummyFlagMediaChange;

#if (MULTI_LUN_BUILD==1)
    // Lun 1 stuff
    pDev->Lun[1].wInquiryVendorData[0] = (g_SCSIVendorID[0]<<16);
    pDev->Lun[1].wInquiryVendorData[1] = g_SCSIVendorID[1] | 
        (g_SCSIVendorID[2]<<8) |
        (g_SCSIVendorID[3]<<16);
    pDev->Lun[1].wInquiryVendorData[2] = g_SCSIVendorID[4] | 
        (g_SCSIVendorID[5]<<8) |
        (g_SCSIVendorID[6]<<16);
    pDev->Lun[1].wInquiryVendorData[3] = g_SCSIVendorID[7] | 
        (g_SCSIProductID[0]<<8) |
        (g_SCSIProductID[1]<<16);
    pDev->Lun[1].wInquiryVendorData[4] = g_SCSIProductID[2] | 
        (g_SCSIProductID[3]<<8) |
        (g_SCSIProductID[4]<<16);
    pDev->Lun[1].wInquiryVendorData[5] = g_SCSIProductID[5] | 
        (g_SCSIProductID[6]<<8) |
        (g_SCSIProductID[7]<<16);
    pDev->Lun[1].wInquiryVendorData[6] = g_SCSIProductID[8] | 
        (g_SCSIProductID[9]<<8) |
        (g_SCSIProductID[10]<<16);
    pDev->Lun[1].wInquiryVendorData[7] = g_SCSIProductID[11] | 
        (g_SCSIProductID[12]<<8) |
        (g_SCSIProductID[13]<<16);
    pDev->Lun[1].wInquiryVendorData[8] = g_SCSIProductID[14] | 
        (g_SCSIProductID[15]<<8) |
        ('0'<<16);
    pDev->Lun[1].wInquiryVendorData[9] = '1' | 
        ('0'<<8) |
        ('0'<<16);

    pDev->Lun[1].wInquiryVendorSpecific[0] = 
        (0<<20) | (pDev->wDeviceNumber<<16) |             // Lun 0, Device num
        (SCSI_INQUIRY_VENDOR_SPECIFIC_STRUCT_MDS<<8) | 1; // MDS, 1 struct       
    pDev->Lun[1].wInquiryVendorSpecific[1] = 
        #ifdef MMC
            (SCSI_MDS_MEDIA_TYPE_MMC_UNKNOWN_VERSION<<8) |    // MMC
        #else
            (SCSI_MDS_MEDIA_TYPE_SMARTMEDIA<<8) |             // SmartMedia
        #endif
        SCSI_MDS_FLAGS_SYSTEM_MEDIA_FLAG_NON_SYSTEM;      // Non-system media 
    pDev->Lun[1].wInquiryVendorSpecific[2] = 0;
    pDev->Lun[1].wInquiryVendorSpecific[3] = 0;
    pDev->Lun[1].wInquiryVendorSpecific[4] = 0;
    pDev->Lun[1].wInquiryVendorSpecific[5] = 0;
    pDev->Lun[1].wInquiryVendorSpecific[6] = 0;
    #ifdef MMC
        pDev->Lun[1].FlagMediaChange = MmcFlagMediaChange;
    #else
        pDev->Lun[1].FlagMediaChange = ScsiDummyFlagMediaChange;
    #endif
    
#endif   //MultiLUN build.
    // Add one to the length of the sector buffer to account for the USB DMA defect
    sectorBufferSizeInWords = g_NandAuxData.SectorTotalSizeInWords + 1;
    // Pad the start address of the buffer array to avoid the USB DMA defect
    SectorArrayInitialize(&pDev->SectorArray,USBMSC_TOTAL_BUFFER_SIZE/sectorBufferSizeInWords,sectorBufferSizeInWords, Target0SectorArray,(Target0BufferArray+1));

    // Device1CommandInBuffer is shared with Mtp
    pDev->CommandInBuffer  = Device1CommandInBuffer;
    pDev->CommandOutBuffer = Device1CommandOutBuffer;

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UsbMscProjectResetDevice0
//
//   Type:          Function
//
//   Description:   Project-specific USB MSC device reset for device 0
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE UsbMscProjectResetDevice0(void)
{
    USBMSC_DEVICE * pDev;
    WORD i;
    
    // Device 0 setup
    pDev = &(UsbMscDevice[USBMSC_DEVICE_0]);

    pDev->wDeviceNumber = 0;

    // Setup the BulkIn Endpoint
    pDev->wBulkInEndpointNum=1;

#ifdef DEVICE_3500
    // Setup the BulkOut Endpoint
    pDev->wBulkOutEndpointNum=2;
#else
    // Setup the BulkOut Endpoint
    pDev->wBulkOutEndpointNum=4;
#endif

    // Setup the Usb Interface
    pDev->wInterface = 0;

    // prevent MMC enum & USB current overage if we're in low power mode.
    // This reports max lun dev num as having no additional removable media other 
    // than NAND flash drive, so mmc drive does not appear on USBMSC peer.  
    if ( usb_get_current_limit() <= 100)
        pDev->wNumLunsSupported = 1; // JLN: emulates case: NUM_REMOVABLE_MEDIA==0. (non MMC case). Reviewed. 
    else
        pDev->wNumLunsSupported = SCSI_DEVICE_NUM_LUNS_DEV_0;

    pDev->bCommandActive = FALSE;
    pDev->bMSCReset = FALSE;
    pDev->InitializeFunction = UsbMscProjectInitDevice0;
    pDev->ResetFunction = UsbMscProjectResetDevice0;
    
    //TODO:  Set up sectors
    
    for(i=0;i<SCSI_DEVICE_NUM_LUNS_DEV_0;i++)
    {
        pDev->Lun[i].bStopped = FALSE;
        pDev->Lun[i].bWriteProtected = FALSE;
        pDev->Lun[i].bMediaConnected = TRUE;      // This is the internal media
        pDev->Lun[i].bMediaIsRemovable = FALSE;
        pDev->Lun[i].wRemovableType = 0;          // don't care
        pDev->Lun[i].wRemovableEntry = 0;         // don't care
        pDev->Lun[i].wRemovableMask = 0       ;   // don't care
        pDev->Lun[i].bMediaEnumerated = FALSE;
        pDev->Lun[i].bStartUnitAttention = TRUE;
        pDev->Lun[i].bMediaChanged = FALSE;
        pDev->Lun[i].bMediaValid = FALSE;
        pDev->Lun[i].eMediaState = ScsiMediaStateUnknown;
        pDev->Lun[i].MediaSize.dwTotalSizeInMegaBytes = 0;
        pDev->Lun[i].MediaSize.dwTotalNumberOfSectors = 0;
        pDev->Lun[i].MediaSize.dwTotalNumberOfBytes = 0;
        pDev->Lun[i].MediaSize.wSectorSizeInBytes = 0;
        pDev->Lun[i].bMediaInsertionDetected = FALSE;
    }
    
#ifdef MMC
    pDev->Lun[0].wFileSystemDriveNumber = NUM_LOGICAL_MEDIA-2;
    pDev->Lun[0].wFileSystemMediaNumber = NUM_LOGICAL_MEDIA-2;
#else
    pDev->Lun[0].wFileSystemDriveNumber = NUM_LOGICAL_MEDIA-1;
    pDev->Lun[0].wFileSystemMediaNumber = NUM_LOGICAL_MEDIA-1;
#endif

#if (MULTI_LUN_BUILD==1)
    pDev->Lun[1].bMediaIsRemovable = TRUE;
    #ifdef MMC
        pDev->Lun[1].wRemovableType = MEDIA_TYPE_MMC;            
    #else
        pDev->Lun[1].wRemovableType = MEDIA_TYPE_SMARTMEDIA;            
    #endif
    pDev->Lun[1].wRemovableEntry = 0;           
    pDev->Lun[1].wRemovableMask = 0x000001;     

#ifdef MTP_BUILD
    pDev->Lun[1].wFileSystemDriveNumber = 1;
    pDev->Lun[1].wFileSystemMediaNumber = 1;
#else
    pDev->Lun[1].wFileSystemDriveNumber = 2;
    pDev->Lun[1].wFileSystemMediaNumber = 2;
#endif
#endif

    return SUCCESS;
}

#if ((NUM_REMOVABLE_MEDIA == 1) && (MULTI_LUN_BUILD==0))
////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UsbMscProjectInitDevice1
//
//   Type:          Function
//
//   Description:   Project-specific USB MSC device init for device 1
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE UsbMscProjectInitDevice1(void)
{
    
    USBMSC_DEVICE * pDev;
    WORD i;
    
    // Device 1 setup
    pDev = &(UsbMscDevice[USBMSC_DEVICE_1]);

    // Run the reset routine
    UsbMscProjectResetDevice1();

    UsbEndpointSetup(pDev->pBulkIn);
    UsbEndpointSetup(pDev->pBulkOut);

    for(i=0;i<SCSI_DEVICE_NUM_LUNS_DEV_1;i++)
    {
        ScsiLunSetupDefaultSenseData(&(pDev->Lun[i]));
        pDev->Lun[i].wCompletionStatus = USBMSC_CSW_COMMAND_PASSED;
        pDev->Lun[i].dwNumBytesXferred = 0;
    }

    // Lun 0 stuff
    pDev->Lun[0].wInquiryVendorData[0] = (g_SCSIVendorID[0]<<16);
    pDev->Lun[0].wInquiryVendorData[1] = g_SCSIVendorID[1] | 
        (g_SCSIVendorID[2]<<8) |
        (g_SCSIVendorID[3]<<16);
    pDev->Lun[0].wInquiryVendorData[2] = g_SCSIVendorID[4] | 
        (g_SCSIVendorID[5]<<8) |
        (g_SCSIVendorID[6]<<16);
    pDev->Lun[0].wInquiryVendorData[3] = g_SCSIVendorID[7] | 
        (g_SCSIProductID[0]<<8) |
        (g_SCSIProductID[1]<<16);
    pDev->Lun[0].wInquiryVendorData[4] = g_SCSIProductID[2] | 
        (g_SCSIProductID[3]<<8) |
        (g_SCSIProductID[4]<<16);
    pDev->Lun[0].wInquiryVendorData[5] = g_SCSIProductID[5] | 
        (g_SCSIProductID[6]<<8) |
        (g_SCSIProductID[7]<<16);
    pDev->Lun[0].wInquiryVendorData[6] = g_SCSIProductID[8] | 
        (g_SCSIProductID[9]<<8) |
        (g_SCSIProductID[10]<<16);
    pDev->Lun[0].wInquiryVendorData[7] = g_SCSIProductID[11] | 
        (g_SCSIProductID[12]<<8) |
        (g_SCSIProductID[13]<<16);
    pDev->Lun[0].wInquiryVendorData[8] = g_SCSIProductID[14] | 
        (g_SCSIProductID[15]<<8) |
        (HARDWARE_VERSION_MAJOR_CHAR<<16);
    pDev->Lun[0].wInquiryVendorData[9] = HARDWARE_VERSION_MINOR_CHAR_2 | 
        (HARDWARE_VERSION_MINOR_CHAR_1<<8) |
        (HARDWARE_VERSION_MINOR_CHAR_0<<16);

    pDev->Lun[0].wInquiryVendorSpecific[0] = 
        (0<<20) | (pDev->wDeviceNumber<<16) |             // Lun 0, Device num
        (SCSI_INQUIRY_VENDOR_SPECIFIC_STRUCT_MDS<<8) | 1; // MDS, 1 struct       
    pDev->Lun[0].wInquiryVendorSpecific[1] = 
        #ifdef MMC
            (SCSI_MDS_MEDIA_TYPE_MMC_UNKNOWN_VERSION<<8) |    // MMC
        #else
            (SCSI_MDS_MEDIA_TYPE_SMARTMEDIA<<8) |             // SmartMedia
        #endif
        SCSI_MDS_FLAGS_SYSTEM_MEDIA_FLAG_NON_SYSTEM;      // Non-system media 
    pDev->Lun[0].wInquiryVendorSpecific[2] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[3] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[4] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[5] = 0;
    pDev->Lun[0].wInquiryVendorSpecific[6] = 0;
    #ifdef MMC
        pDev->Lun[0].FlagMediaChange = MmcFlagMediaChange;
    #else
        pDev->Lun[0].FlagMediaChange = ScsiDummyFlagMediaChange;
    #endif

    return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          UsbMscProjectResetDevice1
//
//   Type:          Function
//
//   Description:   Project-specific USB MSC device reset for device 1
//
//   Inputs:        none
//
//   Outputs:       RETCODE
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
RETCODE  UsbMscProjectResetDevice1(void)
{
    USBMSC_DEVICE * pDev;
    WORD i;
    
    // Device 0 setup
    pDev = &(UsbMscDevice[USBMSC_DEVICE_1]);

    pDev->wDeviceNumber = USBMSC_DEVICE_1;

    UsbEp2IsrFunction = ScsiBulkInIsrFunction;
    wUsbEp2IsrFunctionParam = USBMSC_DEVICE_1; // Target 1 Bulk In
    UsbEp5IsrFunction = UsbMscCbwProcessorIsrFunction;
    wUsbEp5IsrFunctionParam = USBMSC_DEVICE_1; // Target 1 Bulk Out

    // Setup the Usb Interface
    pDev->pInterface = &(UsbInterface[1]);
    pDev->pInterface->wInterface = 1;
    pDev->pInterface->bClearFeature = FALSE;

    // Setup the BulkIn Endpoint
    pDev->pBulkIn = &(UsbEndpoint[USB_ENDPOINT_2]);
    pDev->pBulkIn->pwBuffer = wUsbEp2Buffer;
    pDev->pBulkIn->pFunction = &UsbEp2IsrFunction;
    pDev->bBulkInDataComplete = FALSE;

    // Setup the BulkOut Endpoint
    pDev->pBulkOut = &(UsbEndpoint[USB_ENDPOINT_5]);
    pDev->pBulkOut->pwBuffer = wUsbEp5Buffer;
    pDev->pBulkOut->pFunction = &UsbEp5IsrFunction;
    pDev->bBulkOutDataReady = FALSE;

    pDev->wNumLunsSupported = SCSI_DEVICE_NUM_LUNS_DEV_1;
    pDev->bCommandActive = FALSE;
    pDev->bResetDevice = FALSE;
    pDev->InitializeFunction = UsbMscProjectInitDevice1;
    pDev->ResetFunction = UsbMscProjectResetDevice1;
    
    for(i=0;i<USBMSC_NUM_SECTOR_BUFFERS;i++)
    {
        pDev->Sector[i].bValid = FALSE;
        pDev->Sector[i].pwData = &(Target1BufferArray[i]);
    }
    
    for(i=0;i<SCSI_DEVICE_NUM_LUNS_DEV_1;i++)
    {
        pDev->Lun[i].bStopped = FALSE;
        pDev->Lun[i].bWriteProtected = FALSE;
        pDev->Lun[i].bMediaConnected = FALSE;       // external media
        pDev->Lun[i].bMediaIsRemovable = TRUE;
        #ifdef MMC
            pDev->Lun[i].wRemovableType = MEDIA_TYPE_MMC;            
        #else
            pDev->Lun[i].wRemovableType = MEDIA_TYPE_SMARTMEDIA;            
        #endif
        pDev->Lun[i].wRemovableEntry = 0;           
        pDev->Lun[i].wRemovableMask = 0x000001;     
        pDev->Lun[i].bMediaEnumerated = FALSE;
        pDev->Lun[i].bStartUnitAttention = TRUE;
        pDev->Lun[i].bMediaChanged = FALSE;
        pDev->Lun[i].bMediaValid = FALSE;
        pDev->Lun[i].eMediaState = ScsiMediaStateUnknown;
        pDev->Lun[i].MediaSize.dwTotalSizeInMegaBytes = 0;
        pDev->Lun[i].MediaSize.dwTotalNumberOfSectors = 0;
        pDev->Lun[i].MediaSize.dwTotalNumberOfBytes = 0;
        pDev->Lun[i].MediaSize.wSectorSizeInBytes = 0;
        pDev->Lun[i].bMediaInsertionDetected = FALSE;
    }
    
    pDev->Lun[0].wFileSystemDriveNumber = 2;
    pDev->Lun[0].wFileSystemMediaNumber= 2;

    return SUCCESS;
}
#endif
