////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : usb20.h
// Description : USB2.0 defines and structures definition
////////////////////////////////////////////////////////////////////////////////

#ifndef __usb_h__
#define __usb_h__ 1

#include "usb20project.h"

/* Error codes */
#define  USB_OK                              (0x00)
#define USBERR_REGISTER_SERVICE             (0x01)  // Failed to register a service
#define USBERR_NO_SERVICE                   (0x02)  // No registered service found 
#define  USBERR_INIT_FAILED                  (0x2F) // Fail to initialize the USB20 API
#define USBERR_TX_FAILED                    (0x03)  // Fail to prepare XD descriptor
#define USBERR_SIZE_TOO_LARGE               (0x04)  // Number of bytes to transfer is too large
#define USBERR_EP_INIT_FAILED               (0x05)  // Endpoint init fail

#define  USB_RECV                         (0)
#define  USB_SEND                         (1)

#define  USB_STATE_UNKNOWN                   (0xff)
#define  USB_STATE_POWERED                   (0x03)
#define  USB_STATE_DEFAULT                   (0x02)
#define  USB_STATE_ADDRESS                   (0x01)
#define  USB_STATE_CONFIG                    (0x00)
#define  USB_STATE_SUSPEND                   (0x80)

#define  USB_SELF_POWERED                    (0x01)
#define  USB_REMOTE_WAKEUP                   (0x02)


#define  USB_SPEED_FULL                      (0)
#define  USB_SPEED_LOW                       (1)
#define  USB_SPEED_HIGH                      (2)

#define EPCTRL_TX_ENABLE           (0x00800000)
#define EPCTRL_RX_ENABLE           (0x00000080)
#define EPCTRL_TX_DATA_TOGGLE_RST  (0x00400000)
#define EPCTRL_RX_DATA_TOGGLE_RST  (0x00000040)
#define EPCTRL_TX_EP_STALL         (0x00010000)
#define EPCTRL_RX_EP_STALL         (0x00000001)
#define EPCTRL_TX_EP_TYPE_POS       (18)
#define EPCTRL_RX_EP_TYPE_POS       (2)

// Setup Request Types
#define SETUP_REQUEST_TYPE_MASK     (0x60)
#define SETUP_REQUEST_TYPE_STANDARD (0x00)
#define SETUP_REQUEST_TYPE_CLASS    (0x20)
#define SETUP_REQUEST_TYPE_VENDOR   (0x40)
#define SETUP_REQUEST_TYPE_RES      (0x60)

// Standard request codes
#define STANDARD_REQUEST_GET_STATUS     (0x00)
#define STANDARD_REQUEST_CLEAR_FEATURE  (0x01)
#define STANDARD_REQUEST_SET_FEATURE    (0x03)
#define STANDARD_REQUEST_SET_ADDRESS    (0x05)    
#define STANDARD_REQUEST_GET_DESCRIPTOR (0x06)
#define STANDARD_REQUEST_SET_DESCRIPTOR (0x07)
#define STANDARD_REQUEST_GET_CONFIG     (0x08)
#define STANDARD_REQUEST_SET_CONFIG     (0x09)
#define STANDARD_REQUEST_GET_INTERFACE  (0x0a)
#define STANDARD_REQUEST_SET_INTERFACE  (0x0b)
#define STANDARD_REQUEST_SYNCH_FRAME    (0x0c)

// Get Status Request types 
#define GET_STATUS_DEVICE               (0x80)
#define GET_STATUS_INTERFACE            (0x81)
#define GET_STATUS_ENDPOINT             (0x82)

// Clear and Set Feature Request types
#define FEATURE_DEVICE                  (0x00)
#define FEATURE_INTERFACE               (0x01)
#define FEATURE_ENDPOINT                (0x02)

// Descriptor types
#define DESCRIPTOR_TYPE_DEVICE              (0x01)
#define DESCRIPTOR_TYPE_CONFIG              (0x02)
#define DESCRIPTOR_TYPE_STRING              (0x03)
#define DESCRIPTOR_TYPE_INTERFACE           (0x04)
#define DESCRIPTOR_TYPE_ENDPOINT            (0x05)
#define DESCRIPTOR_TYPE_DEVICE_QUALIFIER    (0x06)
#define DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG  (0x07)
#define DESCRIPTOR_TYPE_INTERFACE_POWER     (0x08)

#define VUSBHS_ADDRESS_BIT_SHIFT            (25)

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
    BYTE    btBcdDeviceLsb;         // (12) Device release number in BCD
    BYTE    btBcdDeviceMsb;         // (13)
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

// Struct String descriptor zero
typedef struct {
    BYTE    btLength;                   // (0) Size of this descriptor in bytes (2*MAX_USB_LANG + 2)
    BYTE    btDescriptorType;           // (1) String descriptor type (3)
    BYTE    btLangCode[2*MAX_USB_LANG];   // LangId code array
} Struct_String_Desc_Zero;

#endif

/* EOF */