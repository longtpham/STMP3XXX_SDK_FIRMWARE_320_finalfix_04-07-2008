/**
***
*** Filename: mtp.h
*** Description: Main MTP constant definitions and function prototypes
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
#ifndef _MTP_H
#define _MTP_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////

#include "kernel.h"
#include "usb_api.h"
#include "ptp.h"
#include "ptpstring.h"
#include "mtpProperty.h"

////////////////////////////////////////////////////////////////////////////////
// Defs
////////////////////////////////////////////////////////////////////////////////
#define BufByte(a,b,c) b[a/3] |= c<<(8*(a%3))		//a specifies Byte in Array, b specifies buffer, c specifies actual data
#define BufWord(a,b,c) BufByte(a,b,(c&0xff)); BufByte(a+1,b,((c>>8) & 0xff))  
#define BufDWord(a,b,c) BufWord(a, b,(c&0xffff)); BufWord(a+2,b,((c>>16) & 0xffff)) 

#define BufDDWord(a,b,c) BufWord(a, b,(c&0xffff)); BufWord(a+2,b,((c>>16) & 0xffff)) 

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


#define TRANSFER_IN_BUFFER_SIZE 2052 
#define TRANSFER_BUFFER_SIZE    512

#define MIN_AVAILABLE_THRESHOLD         2048 // 2048KB = 2MB

#define SIZEOF_UINT8            1
#define SIZEOF_UINT16           2
#define SIZEOF_UINT32           4
#define SIZEOF_UINT64           8


// For static arrays.  These must be updated by manufacturer to reflect capabilities of product
#define		NUM_OPS_SUPPORTED		    36
#define		NUM_EVENTS_SUPPORTED		2//fam:16
#define		NUM_CAPTURE_FORMATS			0
#ifdef AUDIBLE
#define		NUM_OBJECT_FORMATS			6  //Audible adds 1 format from base SDK
#define		NUM_DEVICE_PROPERTIES		3  //Audible adds 1 device property
#else
#define		NUM_OBJECT_FORMATS			5//fam:11
#define		NUM_DEVICE_PROPERTIES		2//fam:5
#endif
#define     MAX_STORAGE_IDS             1 // 2 for MMC

#define     MAX_CHILD_HANDLES           20
#define     MAX_OBJECT_REFERENCES       2

#define     INVALID_OBJECTHANDLE        0xFFFFFFFF

////////////////////////////////////////////////////////////////////////////////
// Errors

//#define USBMSC_ERROR_INVALID_DEVICE                             (WORD)0xFFFFFF
//#define USBMSC_ERROR_INVALID_LUN                                (WORD)0xFFFFFE
#define MTP_DEVICE_INIT_ALL_FAILED                           (WORD)0x000001
#define MTP_DEVICE_INIT_FAILED                               (WORD)0x000002

////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////

typedef RETCODE (*MTP_DEVICE_INIT_FUNCTION)(void);
typedef RETCODE (*MTP_DEVICE_RESET_FUNCTION)(void);

/**
 * \enum eDevicePhase
 *
 * Defines which phase the device is currently in.
 */
typedef enum DevicePhase 
{
    DEVICE_PHASE_NOTREADY = 0,  /**< Device is in a busy state. */
    DEVICE_PHASE_IDLE = 1,      /**< Device is in an idle state. */
    DEVICE_PHASE_COMMAND = 2,   /**< Device is in the command phase. */
    DEVICE_PHASE_DATAIN = 3,    /**< Device is in the data in phase. */
    DEVICE_PHASE_DATAOUT = 4,   /**< Device is in the data out phase. */
    DEVICE_PHASE_RESP0NSE = 5,  /**< Device is in the response phase. */
    DEVICE_PHASE_STALLED = 6    /**< Device endpoints were stalled. */
} eDevicePhase;

typedef enum DeviceStatus
{
        DEVICE_STATUSOK = 0,      /**< Device OK. */
        DEVICE_LOWBATTERY = 1,    /**< Fatal error, can't continue operation. */
        DEVICE_OUTOFMEMORY = 2,   /**< Out of memory, Can't continue. */
        DEVICE_STOREREMOVED = 3,  /**< Fatal error, especially if during a data transfer. */
        DEVICE_DEVICEERROR = 4,    /**< Unidentified fatal device error, can't continue. */
        DEVICE_HOST_CANCELLED_OP = 5,   /**< Operation cancelled; cleanup required */ 
        DEVICE_CANCELLED_OP = 6,  /**< Device initiated operation cancellation */
        DEVICE_FS_CRITICAL = 7,   /**< Critical file system error; unrecoverable */
        DEVICE_STORE_CRITICAL = 8 /**< Critcal STMP store error; unrecoverable */
} eDeviceStatus;



/**
 * SStorageInfo
 *
 * SStorageInfo is the structure for storing all the information for all storage
 * on the device.
 */

typedef struct StorageInfo
{
    /* PTP Storage Info Members */
    WORD        StorageType;           // storage type
    WORD        FileSystemType;        // file system type
    WORD        AccessCapability;      // access capability (e.g. read/write)
    UINT64      MaxCapacity;           // maximum capacity in bytes
    UINT64      FreeSpaceInBytes;      // free space in bytes
    DWORD       FreeSpaceInObjects;    // free space in objects

    SPtpShortString  StorageDescription;    // optional storage description: variable length
    SPtpShortString  VolumeLabel;           // optional volume label: variable length
} SStorageInfo;
typedef struct MtpStore
{
    /* Root path to the store */
    SPtpShortString szRootPath;
    DWORD           StorageID;
    SStorageInfo    StorageInfo;
//    SLinkList       ObjectList;
}SMtpStore;



//
// This structure defines the static device info data returned on MtpGetDeviceInfo calls.
// It is initialized in mtp_ids.c and extracted as a data resource loaded in MtpGetDeviceInfo().
//
typedef struct 
{
    UINT16          StandardVersion;          /**< Version of the MTP standard supported. */
    UINT32          VendorExtensionID;        /**< Vendor extension ID. */
    UINT16          VendorExtensionVersion;   /**< Vendor extension version. */
    SPtpString      VendorExtensionDesc;/**< Vendor extension description string. */
    UINT16          FunctionalMode;           /**< Funtional mode. */
    UINT32          OpsSupported;
    UINT16          OP_SUPPORTED_LIST[NUM_OPS_SUPPORTED];
    UINT32          EventsSupported;
    UINT16          EVENT_SUPPORTED_LIST[NUM_EVENTS_SUPPORTED];
    UINT32          DevicePropsSupported;
    UINT16          DEVICE_PROPS_SUPPORTED_LIST[NUM_DEVICE_PROPERTIES];
    UINT32          CaptureFormatsSupported;
    // There would be a list of Capture Formats here that we support, but
    // we don't support any.  
    UINT32          ObjectFormatsSupported;
    UINT16          OBJECT_FORMAT_LIST[NUM_OBJECT_FORMATS];
    SPtpShortString Manufacturer;       /**< The manufacturer's name. */
    SPtpShortString Model;             /**< A string containing the model name. */
    SPtpShortString DeviceVersion;    /**< A string contaiing the device version. */
    SPtpShortString SerialNumber;   /**< The serial number string. */
} StaticRscMtpDeviceInfo;

typedef struct
{
    SyncObject  TransferObject;
    SyncObject  InterruptObject;
    WORD wDeviceNumber;
    WORD wBulkInEndpointNum;
    WORD _X *CommandOutBuffer;
    WORD _X *CommandInBuffer;
    eDevicePhase        Phase;              /**< Current device phase. */
    eDeviceStatus	Status;
    WORD wTransferSize;
    WORD wBulkOutEndpointNum;
    WORD wInterruptEndpointNum;
    WORD wInterface;
    BYTE                numStores;      /**< Number of valid MtpStores in the array */
    DWORD               DefaultStorageID;           /**< Used when SendObjectInfo doesn't specify StorageID. */
    DWORD               DefaultParentHandle;        /**< Used when SendObjectInfo doesn't specify Parent Object Handle. */
    SUsbCmdBlock PtpCmd;
 //   MTP_DEVICE_INIT_FUNCTION InitializeFunction;
//    MTP_DEVICE_RESET_FUNCTION ResetFunction;
} USBMTP_DEVICE, *PUSBMTP_DEVICE;


extern long int g_lBulkInBytes;
extern long int g_lLastBulkInTime;
extern long int g_lFirstBulkInTime;
extern long int g_lBulkOutBytes;
extern long int g_lLastBulkOutTime;
extern long int g_lFirstBulkOutTime;


extern USBMTP_DEVICE UsbMtpDevice;


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

void _reentrant MtpHandler0(void);
void _reentrant MtpHandler1(void);
RETCODE _reentrant MtpCbwProcessor(WORD wNum);
//RETCODE _reentrant UsbMscSendCsw(WORD wNum);
RETCODE _reentrant MtpDriverInitialize(void);
//RETCODE _reentrant UsbMscDeviceInitializeAll(void);
//RETCODE _reentrant MtpDeviceInitialize(WORD wNum);
RETCODE _reentrant MtpDeviceReset(WORD wNum);
void MtpCheckStore(void);

#define SECTOR_HANDLER  0x800000
#define COMMAND_HANDLER 0x400000

void MtpBulkOutService(BOOL bSetup, BYTE btDirection, _packed BYTE _USB_MEM *pBuffer, WORD wLength,WORD wPrivateData);
void MtpBulkInService(BOOL bSetup, BYTE btDirection, _packed BYTE _USB_MEM *pBuffer, WORD wLength,WORD wPrivateData);
void MtpInterruptService(BOOL bSetup, BYTE btDirection, _packed BYTE _USB_MEM *pBuffer, WORD wLength,WORD wPrivateData);
void MtpResetService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData);
void MtpResumeService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData);
void MtpSuspendService(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData);
void _reentrant MtpLastPacketHandler(BYTE btEndPoint, WORD wDirection, DWORD dwSizeOfLastPacket, WORD wPrivateData, SyncObject * pSyncObject);

//
// Stall the BULKIN/OUT endpoints
//
void Stall(void);

//
// Unstall the BULKIN/OUT endpoints
//
void Unstall(void);

//RETCODE _reentrant UsbMscCheckDiskAll(void);

#define DISPLAY_USB_ACTIVE  0       // Currently reading or writing
#define USB_BLINK_RATE_ON       50      // 50msec
#define USB_BLINK_RATE_OFF      300     // 450mse

#define PERSISTANT_BIT_FOR_LOW_POWER_MODE (HW_RTC_PERSISTANT1.B.b0)

// These can be used by MTP code to signal a change in the animations used in the user interface task
// They match with SCSI_x defines in scsi.h
#define MTP_IDLE      0       // Not reading or writing
#define MTP_READING   1       // A read operation is in progress
#define MTP_WRITING   2       // A write operation is in progress

/**
 * Initializes the Mtp Device Info structure.
 *
 * @param pInfo Pointer to the device information structure.
 * @param pDevice Pointer to the device structure to initialize.
 */
// void initMtpDeviceInfo(SMtpDeviceInfo *pInfo, USBMTP_DEVICE *pDevice);
/**
 * Initializes the MTP device structure.
 *
 * @param pMtpDevice Pointer to the MTP device structure.
 * @return TRUE or FALSE depending on the initialization results.
 */
BOOL  InitializeMtpDevice(PUSBMTP_DEVICE pMtpDevice);
/**
 * Initializes the MTP device.  This function should only be called once
 * during the initialization of the device.
 * 
 * @param pMtpDevice Pointer to the MTP device structure to initialize.
 * @return TRUE or FALSE depending on the initialization results.
 */
int _reentrant OneTimeInitDeviceMtpDevice(int unused1, int unused2, int * piHandle);

/**
 * Cleans up the MTP device structure.
 *
 * @param pMtpDevice Pointer to the MTP device to be cleaned up.
 */
void  CleanupMtpDevice(USBMTP_DEVICE * pMtpDevice);
/**
 * Resets the MTP device.
 *
 * @param pMtpDevice Pointer to the MTP device to be reset.
 */
void  ResetDeviceMtpDevice(PUSBMTP_DEVICE pMtpDevice);
/**
 * Gets the specified device property of the MTP device.
 *
 * @param pMtpDevice Pointer to the MTP device structure.
 * @param PropCode The property code to be retreived.
 * @return A pointer to the MTP device property structure.
 */SMtpDevicePropDesc*  GetDevicePropertyMtpDevice(PUSBMTP_DEVICE pMtpDevice, UINT16 PropCode);

#endif // #ifndef _MTP_H
