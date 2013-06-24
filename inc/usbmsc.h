////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: usbmsc.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _USBMSC_H
#define _USBMSC_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "usb_api.h"
#include "scsi.h"

////////////////////////////////////////////////////////////////////////////////
// Defs
////////////////////////////////////////////////////////////////////////////////

#define USB_INTERFACE_CLASS_MASS_STORAGE    0x08

#define USB_INTERFACE_SUB_CLASS_RBC                             0x01
#define USB_INTERFACE_SUB_CLASS_SFF_8020I                       0x02
#define USB_INTERFACE_SUB_CLASS_MMC_2                           0x02
#define USB_INTERFACE_SUB_CLASS_QIC_157                         0x03
#define USB_INTERFACE_SUB_CLASS_UFI                             0x04
#define USB_INTERFACE_SUB_CLASS_SFF_8070I                       0x05
#define USB_INTERFACE_SUB_CLASS_SCSI_TRANSPARENT_COMMAND_SET    0x06

#define USB_INTERFACE_PROTOCOL_CBI_CCI                          0x01
#define USB_INTERFACE_PROTOCOL_CBI_NO_CCI                       0x02
#define USB_INTERFACE_PROTOCOL_BULK_ONLY                        0x50

#define USBMSC_CBW_SIGNATURE            (DWORD)0x43425355
#define USBMSC_CBW_BULK_OUT_SIZE        (WORD)0x00001F

#define USBMSC_BULK_OUT_SCSI_CDB_START  5

// This is the number of sector buffers if the data area is interpreted as the smallest sector buffer size
#define USBMSC_MAX_NUM_SECTOR_BUFFERS       (16)
// We add one to pad at the beginning of the buffer and end of each entry to avoid the USB DMA defect
#define USBMSC_TOTAL_BUFFER_SIZE            ((SECTOR_BUFFER_ALLOC_SIZE(SSFDC_SECTOR_TOTAL_SIZE)*USBMSC_MAX_NUM_SECTOR_BUFFERS)+1)

#define USBMSC_CSW_SIGNATURE_LOW        (WORD)0x425355
#define USBMSC_CSW_SIGNATURE_HIGH       (WORD)0x000053
#define USBMSC_CSW_COMMAND_PASSED       (WORD)0x000000
#define USBMSC_CSW_COMMAND_FAILED       (WORD)0x000001
#define USBMSC_CSW_PHASE_ERROR          (WORD)0x000002

#define GET_MAX_LUN_STAGE_DATA_0        (WORD)0x000000
#define GET_MAX_LUN_STAGE_DATA_1        (WORD)0x000001

#define USBMSC_MAX_LUNS                 4

#define USBMSC_DEVICE_0                 0
#define USBMSC_DEVICE_1                 1
#define USBMSC_DEVICE_2                 2
#define USBMSC_DEVICE_3                 3

////////////////////////////////////////////////////////////////////////////////
// Errors

#define USBMSC_ERROR_INVALID_DEVICE                             (WORD)0xFFFFFF
#define USBMSC_ERROR_INVALID_LUN                                (WORD)0xFFFFFE
#define USBMSC_DEVICE_INIT_ALL_FAILED                           (WORD)0x000001
#define USBMSC_DEVICE_INIT_FAILED                               (WORD)0x000002

////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////

typedef RETCODE (*USBMSC_DEVICE_INIT_FUNCTION)(void);
typedef RETCODE (*USBMSC_DEVICE_RESET_FUNCTION)(void);

typedef union
{
    struct
    {
        int Reserved    : 6;
        int Obsolete    : 1;
        int Direction   : 1;
    } B;
    int I;
} USBMSC_CBW_FLAGS;

typedef struct
{
    DWORD dwCBWSignature;
    DWORD dwCBWTag;
    DWORD dwCBWDataTransferLength;
    USBMSC_CBW_FLAGS CBWFlags;
    WORD wCBWLUN;
    WORD wReserved0;
    WORD wCBWCBLength;
    WORD wReserved1;
} USBMSC_CBW;




typedef struct
{
    SyncObject  TransferObject;
    WORD wDeviceNumber;
    WORD wBulkInEndpointNum;
    WORD _X *CommandOutBuffer;
    WORD _X *CommandInBuffer;
    WORD wTransferSize;
    WORD wBulkOutEndpointNum;
    WORD wInterface;
    WORD wNumLunsSupported;
    USBMSC_CBW CBW;
    SCSI_LUN Lun[USBMSC_MAX_LUNS];
    BOOL bCommandActive;
    BOOL bMSCReset;
    USBMSC_DEVICE_INIT_FUNCTION InitializeFunction;
    USBMSC_DEVICE_RESET_FUNCTION ResetFunction;
    SECTOR_ARRAY  SectorArray;
} USBMSC_DEVICE;


extern long int g_lUSBMSCBulkInBytes;
extern long int g_lUSBMSCLastBulkInTime;
extern long int g_lUSBMSCFirstBulkInTime;
extern long int g_lUSBMSCBulkOutBytes;
extern long int g_lUSBMSCLastBulkOutTime;
extern long int g_lUSBMSCFirstBulkOutTime;

extern USBMSC_DEVICE UsbMscDevice[];
extern WORD g_wUsbMscNumDevices;


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

void _reentrant UsbMscHandler0(void);
void _reentrant UsbMscHandler1(void);
RETCODE _reentrant UsbMscCbwProcessor(WORD wNum);
RETCODE _reentrant UsbMscSendCsw(WORD wNum);
RETCODE _reentrant UsbMscDriverInitialize(void);
RETCODE _reentrant UsbMscDeviceInitializeAll(void);
RETCODE _reentrant UsbMscDeviceInitialize(WORD wNum);
RETCODE _reentrant UsbMscDeviceReset(WORD wNum);


#define SECTOR_HANDLER  0x800000
#define COMMAND_HANDLER 0x400000

void _reentrant UsbMscBulkOutService(BOOL bSetup, BYTE btDirection, _packed BYTE _USB_MEM *pBuffer, WORD wLength,WORD wPrivateData);
void _reentrant UsbMscBulkInService(BOOL bSetup, BYTE btDirection, _packed BYTE _USB_MEM *pBuffer, WORD wLength,WORD wPrivateData);
void _reentrant UsbMscResetService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData);
void _reentrant UsbMscResumeService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData);
void _reentrant UsbMscSuspendService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData);

RETCODE _reentrant UsbMscCheckDiskAll(void);

#define DISPLAY_USB_ACTIVE  0       // Currently reading or writing
#define USB_BLINK_RATE_ON       50      // 50msec
#define USB_BLINK_RATE_OFF      300     // 450mse

#define PERSISTANT_BIT_FOR_LOW_POWER_MODE (HW_RTC_PERSISTANT1.B.b0)


#endif // #ifndef _USBMSC_H
