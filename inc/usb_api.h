////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : USB_API.H
// Description : USB_API.H defines and structures definition for USB device
//               driver API.
//               This 
////////////////////////////////////////////////////////////////////////////////

#ifndef __usb_api_h__
#define __usb_api_h__ 1

#include "types.h"
#include "project.h"

#ifndef _USB_MEM
//if not overwridden, default to the compiler default memory space
#define _USB_MEM
#endif

#define USB_LANGUAGE_ENGLISH 0x0409
#define USB_LANGUAGE_CHINESE_PRC 0x0804
#define USB_LANGUAGE_CHINESE_TAIWAN 0x0404
#define USB_LANGUAGE_JAPANESE 0x0411

#define MAX_STRING 30

#define  USB_SPEED_FULL                      (0)
#define  USB_SPEED_LOW                       (1)
#define  USB_SPEED_HIGH                      (2)

#define EP_FS_MAX_PACKET_SIZE               (64)    

#define EP_HS_MAX_PACKET_SIZE               (512)   

// Data transferred to devices based on Sigmatel 3500 chip may sometimes be 
// corrupted. The corruption occurs when device is connected in USB High Speed 
// mode, and size of the transfer in bytes is divisible by 128, but not 
// divisible by the packet size. The workaround is implemented in Microsoft MTP 
// driver to allow support for those devices.

// the bug can corrupt the end of data buffer. It occurs when host sends out a 
// short packet that is multiple of 128 bytes. During this scenario the USB core
// does writes that are voided (byte enable == all zero), which was an undefined 
// capability and thus the gasket did not apply a proper response. This should 
// not affect Mass Storage applications because data transfers are 512 bytes and 
// scsi commands are < 16 bytes. Other applications with their own driver will
// need to pad the packets if they fall into the error case. "What this basically 
// boils down to is that any time the host sends a short packet of 128, 256 or
// 384 bytes to a high-speed connected player a number of bytes will be corrupted.
// This is easiest to reproduce in SendObject commands but can strike any of the 
// commands that take an arbitrary amount of data.
#define ARC_HS_128_MODULO_BUG               (128)


/* Error codes */
#define USB_OK                              (0x00)
#define USBERR_REGISTER_SERVICE             (0x01)  // Failed to register a service
#define USBERR_NO_SERVICE                   (0x02)  // No registered service found 
#define USBERR_TX_FAILED                    (0x03)  // Fail to prepare XD descriptor
#define USBERR_SIZE_TOO_LARGE               (0x04)  // Number of bytes to transfer is too large
#define USBERR_EP_INIT_FAILED               (0x05)  // Endpoint init fail
#define USBERR_CONFIG_FAILED                (0x06)  // Error reported by the application
                                                    // if the new config failed or not supported
#define USBERR_DEVICE_NOT_CONFIGURED        (0x07)  // Error reported by the transfer function
                                                    // if attempting to transfer while the device
                                                    // is not configured
#define USBERR_INIT_FAILED                  (0x08)  // Fail to initialize the USB API


/* Endpoint types */
#define  USB_CONTROL_ENDPOINT             (0)
#define  USB_ISOCHRONOUS_ENDPOINT         (1)
#define  USB_BULK_ENDPOINT                (2)
#define  USB_INTERRUPT_ENDPOINT           (3)

/* End point direction */
#define  USB_OUT                         (0)
#define  USB_IN                          (1)

/* End point packed size */
#define  USB_FS_MAX_PACKET_SIZE           (64)
#define  USB_HS_BULK_OUT_MAX_PACKET_SIZE  (256)
#define  USB_HS_BULK_IN_MAX_PACKET_SIZE   (128)


#define  USB_TEST_MODE_TEST_J             (0x01)
#define  USB_TEST_MODE_TEST_K             (0x02)
#define  USB_TEST_MODE_TEST_SE0_NAK       (0x03)
#define  USB_TEST_MODE_TEST_PACKET        (0x04)

/* Available service types */
/* Services 0 through 15 are reserved for endpoints */
#define  USB_SERVICE_EP0                  (0x00)
#define  USB_SERVICE_EP1                  (0x01)
#define  USB_SERVICE_EP2                  (0x02)
#define  USB_SERVICE_EP3                  (0x03)
#define  USB_SERVICE_EP4                  (0x04)
#define  USB_SERVICE_EP5                  (0x05)
#define  USB_SERVICE_EP6                  (0x06)
#define  USB_SERVICE_EP7                  (0x07)
#define  USB_SERVICE_EP8                  (0x08)
#define  USB_SERVICE_EP9                  (0x09)
#define  USB_SERVICE_EP10                 (0x0a)
#define  USB_SERVICE_EP11                 (0x0b)
#define  USB_SERVICE_EP12                 (0x0c)
#define  USB_SERVICE_EP13                 (0x0d)
#define  USB_SERVICE_EP14                 (0x0e)
#define  USB_SERVICE_EP15                 (0x0f)
#define  USB_SERVICE_BUS_RESET            (0x10)
#define  USB_SERVICE_SUSPEND              (0x11)
#define  USB_SERVICE_SOF                  (0x12)
#define  USB_SERVICE_RESUME               (0x13)
#define  USB_SERVICE_SLEEP                (0x14)
#define  USB_SERVICE_SPEED_DETECTION      (0x15)
#define  USB_SERVICE_TEST                 (0x16)
#define  USB_SERVICE_CHANGE_CONFIG        (0x17)
#define  USB_SERVICE_CHANGE_IFACE         (0x18)   
#define  NON_EP_SERVICES                  (USB_SERVICE_SPEED_DETECTION - USB_SERVICE_EP15)

// This is the maximum service structures allocated 
// One for each End point + the common services (non endpoint services) 
#define MAX_NUMBER_SERVICES               (MAX_NUM_EP + NON_EP_SERVICES)  

#define NUMBER_BYTES_IN_SETUP               (8)

// possible values of btStatus on the xd_struct
#define  USB_STATUS_TRANSFER_IDLE           (0)
#define  USB_STATUS_TRANSFER_SUCCESS        (1)            
#define  USB_STATUS_TRANSFER_ACCEPTED       (2)
#define  USB_STATUS_TRANSFER_IN_PROGRESS    (3)
#define  USB_STATUS_TRANSFER_ERROR          (4)

// types of status set/returned by set_status() and get_status()
#define  USB_STATUS_DEVICE_STATE          (0x01)
#define  USB_STATUS_INTERFACE             (0x02)
#define  USB_STATUS_ADDRESS               (0x03)
#define  USB_STATUS_CURRENT_CONFIG        (0x04)
#define  USB_STATUS_SOF_COUNT             (0x05)
#define  USB_STATUS                       (0x06)
#define  USB_STATUS_TEST_MODE             (0x07)
#define  USB_STATUS_DEVICE_SPEED          (0x08)  
#define  USB_STATUS_DEVICE_TEST           (0x09)  
#define  USB_STATUS_CURRENT_IFACE         (0x0a)
#define  USB_STATUS_CONNECTION            (0x0b)  
// For status endpoint, the endpoint number is coded in the less significant nibble
#define  USB_STATUS_ENDPOINT              (0x10)
#define  USB_STATUS_ENDPOINT_NUMBER_MASK  (0x0F)

#define USB_MAX_SIZE_PER_TRANSFER       (0x4000)    // This is the maximum size that
                                                    // we allow per transfer

//Possible states the device can be in
#define  USB_STATE_UNKNOWN                   (0xff)
#define  USB_STATE_UNPLUGGED                 (0x05) // USB Cable unplugged
#define  USB_STATE_PLUGGED                   (0x04) // USB Cable inserted
#define  USB_STATE_POWERED                   (0x03)
#define  USB_STATE_DEFAULT                   (0x02)
#define  USB_STATE_ADDRESSED                 (0x01)
#define  USB_STATE_CONFIGURED                (0x00)
#define  USB_STATE_SUSPENDED                 (0x80)

// Possible states the connection can be in
//#define USB_STATE_UNKNOWN                 (0xff)
#define USB_CONNECTED                       (0x00)
#define USB_DISCONNECTED                    (0x01)



typedef union
{
    struct
    {
        int Recipient               : 5;
        int Type                    : 2;
        int DataTransferDirection   : 1;
        int Request                 : 8;
        int                         : 8;
    } B;
    
    int I;
} USBRequestType ;

typedef struct {
   BYTE     bValid;
   USBRequestType RequestType;
   BYTE     btRequest;
   WORD     wValue;
   WORD     wIndex;
   WORD     wLength;
    } USB_SetupData;

// Descriptor sizes in bytes
#define SIZE_DEVICE_DESCRIPTOR_IN_BYTES     (18)
#define SIZE_DEVICE_DESCRIPTOR_IN_WORDS     (6)
#define SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_BYTES   (10)
#define SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_WORDS   (4)
#define SIZE_CONFIG_DESCRIPTOR_IN_BYTES     (9)
#define SIZE_CONFIG_DESCRIPTOR_IN_WORDS     (3)
#define SIZE_INTERFACE_DESCRIPTOR_IN_BYTES  (9)
#define SIZE_INTERFACE_DESCRIPTOR_IN_WORDS  (3)
#define SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES   (7)


// Descriptor types
#define DESCRIPTOR_TYPE_DEVICE              (0x01)
#define DESCRIPTOR_TYPE_CONFIG              (0x02)
#define DESCRIPTOR_TYPE_STRING              (0x03)
#define DESCRIPTOR_TYPE_INTERFACE           (0x04)
#define DESCRIPTOR_TYPE_ENDPOINT            (0x05)
#define DESCRIPTOR_TYPE_DEVICE_QUALIFIER    (0x06)
#define DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG  (0x07)
#define DESCRIPTOR_TYPE_INTERFACE_POWER     (0x08)


#define USB_REQUEST_DIRECTION_HOST_TO_DEVICE    ((WORD)0x000000)
#define USB_REQUEST_DIRECTION_DEVICE_TO_HOST    ((WORD)0x000001)

#define USB_REQUEST_TYPE_STANDARD               ((WORD)0x000000)
#define USB_REQUEST_TYPE_CLASS                  ((WORD)0x000001)
#define USB_REQUEST_TYPE_VENDOR                 ((WORD)0x000002)
#define USB_REQUEST_TYPE_RESERVED               ((WORD)0x000003)

#define USB_REQUEST_RECIPIENT_DEVICE            ((WORD)0x000000)
#define USB_REQUEST_RECIPIENT_INTERFACE         ((WORD)0x000001)
#define USB_REQUEST_RECIPIENT_ENDPOINT          ((WORD)0x000002)
#define USB_REQUEST_RECIPIENT_OTHER             ((WORD)0x000003)

#define USB_REQUEST_STANDARD_GET_STATUS         ((WORD)0x000000)
#define USB_REQUEST_STANDARD_CLEAR_FEATURE      ((WORD)0x000001)
#define USB_REQUEST_STANDARD_RESERVED_1         ((WORD)0x000002)
#define USB_REQUEST_STANDARD_SET_FEATURE        ((WORD)0x000003)
#define USB_REQUEST_STANDARD_RESERVED_2         ((WORD)0x000004)
#define USB_REQUEST_STANDARD_SET_ADDRESS        ((WORD)0x000005)
#define USB_REQUEST_STANDARD_GET_DESCRIPTOR     ((WORD)0x000006)
#define USB_REQUEST_STANDARD_SET_DESCRIPTOR     ((WORD)0x000007)
#define USB_REQUEST_STANDARD_GET_CONFIGURATION  ((WORD)0x000008)
#define USB_REQUEST_STANDARD_SET_CONFIGURATION  ((WORD)0x000009)
#define USB_REQUEST_STANDARD_GET_INTERFACE      ((WORD)0x00000A)
#define USB_REQUEST_STANDARD_SET_INTERFACE      ((WORD)0x00000B)
#define USB_REQUEST_STANDARD_SYNCH_FRAME        ((WORD)0x00000C)

#define USB_CLEAR_FEATURE_ENDPOINT_HALT         (0)
#define USB_CLEAR_FEATURE_REMOTE_WAKEUP         (1)


// Struct Device Descriptor
typedef struct {
    BYTE    bLength;                // (0) Size of this descriptor in bytes (18)
    BYTE    btDescriptorType;       // (1) Device descriptor type (1)
    BYTE    btBcdUsbLsb;            // (2) USB specification release number (for version 2.00 0x00)
    BYTE    btBcdUsbMsb;            // (3)                                  (for version 2.00 0x02)     
    BYTE    btDeviceClass;          // (4) Class code
    BYTE    btDeviceSubclass;       // (5) Subclass code
    BYTE    btDeviceProtocol;       // (6) Protocol code
    BYTE    btMaxPacketSize0;       // (7) Maximum packet size for endpoint 0 (8, 16, 32 or 64)
    BYTE    btIdVendorLsb;          // (8) Vendor Id
    BYTE    btIdVendorMsb;          // (9)
    BYTE    btIdProductLsb;         // (10) Product Id
    BYTE    btIdProductMsb;         // (11)
    BYTE    btBcdReleaseLsb;        // (12) Device release number in BCD
    BYTE    btBcdReleaseMsb;        // (13)
    BYTE    btIndexManufacturer;    // (14) Index of string descriptor describing Manufacturer
    BYTE    btIndexProduct;         // (15) Index of string descriptor describing Product
    BYTE    btIndexSerialNumber;    // (16) Index of string descriptor describing Device Serial Number
    BYTE    btNumConfigurations;    // (17) Number of possible configurations
} Struct_Standard_Dev_Desc;

// Struct Device Qualifier Descriptor
typedef struct {
    BYTE    bLength;                // (0) Size of this descriptor in bytes (10)
    BYTE    btDescriptorType;       // (1) Device descriptor type (6)
    BYTE    btBcdUsbLsb;            // (2) USB specification release number (for version 2.00 0x00)
    BYTE    btBcdUsbMsb;            // (3)                                  (for version 2.00 0x02)     
    BYTE    btDeviceClass;          // (4) Class code
    BYTE    btDeviceSubclass;       // (5) Subclass code
    BYTE    btDeviceProtocol;       // (6) Protocol code
    BYTE    btMaxPacketSize0;       // (7) Maximum packet size for endpoint 0 other speed (8, 16, 32 or 64)
    BYTE    btNumConfigurations;    // (8) Number of other-speed possible configurations
    BYTE    btReserved;             // (9) Reserved for future use, must be 0
} Struct_Dev_Qualifier_Desc;

// Struct Configuration Descriptor
typedef struct {
    BYTE    bLength;                // (0) Size of this descriptor in bytes (9)
    BYTE    btDescriptorType;       // (1) Device descriptor type (2 or 7)
    BYTE    btTotalLengthLsb;       // (2) Total length of data returned for this config.
    BYTE    btTotalLengthMsb;       // (3)                                  
    BYTE    btNumInterfaces;        // (4) Number of Interfaces supported by this configuration
    BYTE    btConfigurationValue;   // (5) Value to use as an argument to the SetConfiguration() request
    BYTE    btIndexConfiguration;   // (6) Index of string descriptor describing this configuration
    BYTE    btBitMapAttributes;     // (7) Configuration Characteristics
                                    //     D7:      Reserved (set to one)
                                    //     D6:      Self-powered
                                    //     D5:      Remote Wakeup
                                    //     D4..0:   Reserved (reset to zero)        
    BYTE    btMaxPower;             // (8) Maximum power consumption expressed in 2mA units.
} Struct_Standard_Config_Desc, Struct_Other_Speed_Config_Desc;

// Struct Interface Descriptor
typedef struct {
    BYTE    bLength;                // (0) Size of this descriptor in bytes (9)
    BYTE    btDescriptorType;       // (1) Device descriptor type (4)
    BYTE    btInterfaceNum;         // (2) Number of this interface. Zero-based value.
    BYTE    btAlternateSetting;     // (3)                                  
    BYTE    btNumEndpoints;         // (4) Number of endpointsused by this interface (excluding endpoint 0).
    BYTE    btInterfaceClass;       // (5) Class code
    BYTE    btInterfaceSubClass;    // (6) Sub Class code
    BYTE    btInterfaceProtocol;    // (7) Protocol Code
    BYTE    btIndexInterface;       // (8) Index of string descriptor describing this interface
} Struct_Standard_Interface_Desc;

// Struct Endpoint Descriptor
typedef struct {
    BYTE    bLength;                // (0) Size of this descriptor in bytes (7)
    BYTE    btDescriptorType;       // (1) Device descriptor type (5)
    BYTE    btEndpointAddress;      // (2) Address of the endpoint on the USB device described by this descriptor
                                    //      Bit 3..0: Endpoint number
                                    //      Bit 6..4: Reserved, reset to zero
                                    //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
    BYTE    btBitMapAttributes;     // (3) Endpoint attributes
                                    //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                    //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                    //      bits 7..6: Reserved bits, must be reset to zero.                                    
    BYTE    btMaxPacketSizeLsb;     // (4) Maximum packet size. Refer to standard for bits specification
    BYTE    btMaxPacketSizeMsb;     // (5)
    BYTE    btInterval;             // (6) Interval for polling endpoint for data transfers (refer to standard)
} Struct_Standard_Endpoint_Desc;

typedef struct {
    BYTE    btLength;                   // (0) Size of this descriptor in bytes (2*MAX_USB_LANG + 2)
    BYTE    btDescriptorType;           // (1) String descriptor type (3)
    BYTE    btString[MAX_STRING*2];       
} Struct_String_Desc;

//
// Externals
//
extern void _reentrant usb_dci_chip_enable (void);
extern void _reentrant usb_dci_shutdown(void);

BYTE _reentrant usb_device_init(void);
void _inline usb_device_shutdown(void)
{
    usb_dci_shutdown();
}

BYTE _reentrant usb_device_register_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength,WORD wPrivateData)); 
//Remove unused function usb_device_get_service() to reclain _P memroy.
//BYTE _reentrant usb_device_get_service(BYTE btType, void (* Service)(BOOL bSetup, BYTE btDirection, BYTE _USB_MEM * pbtBuffer, WORD wLength)); 

BYTE _reentrant usb_device_init_endpoint(BYTE btEndpoint, USHORT usMaxPacketSize, BYTE btDirection, BYTE btType, BOOL bFlag);

extern USB_SetupData *_reentrant usb_dci_get_setup_data(BYTE btEndpoint);
#define usb_device_read_setup_data(btEndpoint) usb_dci_get_setup_data(btEndpoint)

void _reentrant usb_device_get_status(BYTE btType, USHORT * pusStatus);
void _reentrant usb_device_set_status(BYTE btType, USHORT usStatus);
BYTE _reentrant usb_device_get_transfer_status(BYTE btEndpoint, BYTE btDirection);

extern void _reentrant usb_dci_stall_endpoint(BYTE btEndpoint, BYTE btDirection);
#define usb_device_stall_endpoint(btEndpoint, btDirection) usb_dci_stall_endpoint((btEndpoint), (btDirection))

extern void _reentrant usb_dci_unstall_endpoint(BYTE btEndpoint, BYTE btDirection);
#define usb_device_unstall_endpoint(btEndpoint, btDirection) usb_dci_unstall_endpoint((btEndpoint), (btDirection))

extern void _reentrant usb_dci_finish_transaction(BYTE btEndpoint);
#define usb_device_finish_transaction(btEndpoint) usb_dci_finish_transaction(btEndpoint);

////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          usb_device_enable
//
//   Type:          Function
//
//   Description:   Enables the USB device for transfers
//
//   Inputs:        none
//
//   Outputs:       USB_OK or error code
//
//   Notes:         none
//<
////////////////////////////////////////////////////////////////////////////////
void _inline usb_device_enable(void)
{
    usb_dci_chip_enable();
}

void _reentrant usb_device_release(void);
void _reentrant usb_device_hold(void);
BYTE _reentrant usb_device_is_hold(void);
void _reentrant usb_device_data_phase_required(void);

BOOL _reentrant usb_device_is_first_init(void);
void _reentrant usb_device_set_first_init(void);
void _reentrant usb_device_clear_first_init(void);

WORD _reentrant usb_get_current_limit(void);
RETCODE _reentrant usb_set_current_limit(WORD);

////////////////////////////////////////////////////////////////////
extern BYTE _reentrant usb_dci_get_endpoint_status(BYTE btEndpoint);
#define usb_device_get_endpoint_status(btEndpoint) usb_dci_get_endpoint_status(btEndpoint)

#define USB_ENDPOINT_STATUS_OK                          0
#define USB_ENDPOINT_STATUS_STALLED                     1
#define USB_ENDPOINT_STATUS_RECIEVED_UNSTALL_REQUEST    2
// WHQL
#define USB_ENDPOINT_STATUS_HOST_STALLED                3
////////////////////////////////////////////////////////////////////


BYTE _reentrant usb_device_send_data(BYTE btEndpoint, WORD _X * pbtStmpWordAdd, BYTE btByteOffset, USHORT usSize, WORD wPrivateData, BOOL bIsDataPhase);
BYTE _reentrant usb_device_recv_data(BYTE btEndpoint, WORD _X * pbtStmpWordAdd, BYTE btByteOffset, USHORT usSize, WORD wPrivateData, BOOL bIsDataPhase);
void _reentrant ch9ServiceEp0(BOOL bSetup, BYTE btDirection, WORD _X *pbtBuffer, WORD wLength,WORD wPrivateData);
BYTE _reentrant usb_device_transfer_data(BYTE btEndpoint, WORD _X* pbtStmpWordAdd, 
                                         BYTE btByteOffset, USHORT usSize, WORD wPrivateData, 
                                         BOOL bIsDataPhase, WORD wDirection);

#ifndef CUSTOMIZE_UNICODE
extern const BYTE g_Manufacturer[];
extern const BYTE g_ProductName[];
extern const BYTE g_ConfigString[];
extern const BYTE g_Interface0String[];
extern const BYTE g_Interface1String[];
extern const BYTE g_MSOSDescriptorString[];
#else   // CUSTOMIZE_UNICODE
extern const WORD g_Manufacturer[];
extern const WORD g_ProductName[];
extern const WORD g_ConfigString[];
extern const WORD g_Interface0String[];
extern const WORD g_Interface1String[];
extern const WORD g_MSOSDescriptorString[];
#endif
extern WORD g_LanguageList[];

extern const BYTE g_SCSIVendorID[];
extern const BYTE g_SCSIProductID[];
extern void *g_pStrings[];

extern WORD g_wUSBVendorID;
extern WORD g_wUSBProductID;
extern WORD g_wUSBReleaseID;

#endif
/* EOF */

