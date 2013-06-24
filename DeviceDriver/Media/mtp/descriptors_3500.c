////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : Descriptors.c
// Description : This file only supports
//                  1. 1 Language (English)
//                  2. Descriptor up to 255 bytes in size 
////////////////////////////////////////////////////////////////////////////////
#include "usb_api.h"
#include "mtp.h"
#include "mtprequests.h"
#include "sysserialnumber.h"

// Defined, allocated and initialized in project specific file usbstrings.asm

Struct_Standard_Dev_Desc _USB_MEM StDescDevice = {
    SIZE_DEVICE_DESCRIPTOR_IN_BYTES,    // (0) Size of this descriptor in bytes (18)
    DESCRIPTOR_TYPE_DEVICE,             // (1) Device descriptor type (1)
    0x00,                               // (2) USB specification release number (for version 2.00 0x00)
    0x02,                               // (3)                                  (for version 2.00 0x02)     
    0x00,                               // (4) Class code
    0x00,                               // (5) Subclass code
    0x00,                               // (6) Protocol code
    64,                                 // (7) Maximum packet size for endpoint 0 (8, 16, 32 or 64)
    0x6f,                               // (8) Vendor Id 
    0x06,                               // (9)
    0x10,                               // (10) Product Id - Auto fill by reading Revision Register
    0x90,                               // (11)
    0x01,                                  // (12) Device release number in BCD - Auto fill in
    0x00,                                  // (13) by reading the REVID Register
    0x01,                               // (14) Index of string descriptor describing Manufacturer
    0x02,                               // (15) Index of string descriptor describing Product
    0x03,                               // (16) Index of string descriptor describing Device Serial Number
    1                                   // (17) Number of possible configurations
};


//================================================================================
// Describes information about a high-speed capable device that would change if
// the device were operating at the other speed.
//
// In this application, the device operates in the same manner at both speeds.
//================================================================================
Struct_Dev_Qualifier_Desc _USB_MEM StDescDeviceQualFs = {
    SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (10)
    DESCRIPTOR_TYPE_DEVICE_QUALIFIER,   // (1) Device descriptor type (6)
    0x00,                               // (2) USB specification release number (for version 2.00 0x00)
    0x02,                               // (3)                                  (for version 2.00 0x02)     
    0x00,                               // (4) Class code
    0x00,                               // (5) Subclass code
    0x00,                               // (6) Protocol code
    64,                                 // (7) Maximum packet size for endpoint 0 (8, 16, 32 or 64)
    0x01,                                  // (8) Number of possible configurations
    0x00                                // (9) Reserved must be zero
};

Struct_Dev_Qualifier_Desc _USB_MEM StDescDeviceQualHs = {
    SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (10)
    DESCRIPTOR_TYPE_DEVICE_QUALIFIER,   // (1) Device descriptor type (6)
    0x00,                               // (2) USB specification release number (for version 2.00 0x00)
    0x02,                               // (3)                                  (for version 2.00 0x02)     
    0x00,                               // (4) Class code
    0x00,                               // (5) Subclass code
    0x00,                               // (6) Protocol code
    64,                                 // (7) Maximum packet size for endpoint 0 (8, 16, 32 or 64)
    1,                                  // (8) Number of possible configurations
    0x00                                // (9) Reserved must be zero
};




// Full/High Speed Device Descriptor
// Parameter (2) and (3) are filled automatically
Struct_Standard_Config_Desc _USB_MEM StDescConfig = {
        // Config Descriptor
        SIZE_CONFIG_DESCRIPTOR_IN_BYTES,    // (0) Size of this descriptor in bytes (9)
        DESCRIPTOR_TYPE_CONFIG,             // (1) Device descriptor type (2 or 7)
        0x00,                               // (2) Total length of data returned for this config.
        0x00,                               // (3)                                  
        1,
        0x01,                               // (5) Value to use as an argument to the SetConfiguration() request
        0x04,                               // (6) Index of string descriptor describing this configuration
        0x80,                               // (7) Configuration Characteristics
                                            //     D7:      Reserved (set to one)
                                            //     D6:      Self-powered
                                            //     D5:      Remote Wakeup
                                            //     D4..0:   Reserved (reset to zero)        
                                            // (8) Maximum power consumption expressed in 2mA units.
       0xfa
    };

    
// This interface is used for both Configs (Full and High Speed)
Struct_Standard_Interface_Desc _USB_MEM StDescInterface0 = {
        // Interface Descriptor (One unique interface)
        SIZE_INTERFACE_DESCRIPTOR_IN_BYTES, // (0) Size of this descriptor in bytes (9)
        DESCRIPTOR_TYPE_INTERFACE,          // (1) Device descriptor type (4)
        0x00,                               // (2) Number of this interface. Zero-based value.
        0x00,                               // (3)                                  
        3,                                  // (4) Number of endpointsused by this interface (excluding endpoint 0).
        USB_INTERFACE_CLASS_MASS_STORAGE,   // (5) Class code
        USB_INTERFACE_SUB_CLASS_SCSI_TRANSPARENT_COMMAND_SET,// (6) Sub Class code
        USB_INTERFACE_PROTOCOL_BULK_ONLY,   // (7) Protocol Code
        0x05                                // (8) Index of string descriptor describing this interface
    };            

// Endpoint #1 for Full Speed Configuration
Struct_Standard_Endpoint_Desc _USB_MEM StDescEndPoint1FS = {
        // Endpoint Descriptor 
        SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (7)
        DESCRIPTOR_TYPE_ENDPOINT,           // (1) Device descriptor type (5)
        0x81,                               // (2) Address of the endpoint on the USB device described by this descriptor
                                            //      Bit 3..0: Endpoint number
                                            //      Bit 6..4: Reserved, reset to zero
                                            //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
        0x02,                               // (3) Endpoint attributes
                                            //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                            //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                            //      bits 7..6: Reserved bits, must be reset to zero.                                    
        EP_FS_MAX_PACKET_SIZE,              // (4) Maximum packet size. Refer to standard for bits specification
        0x00,                               // (5)
        0x00                                // (6) Interval for polling endpoint for data transfers (refer to standard)
    };
Struct_Standard_Endpoint_Desc _USB_MEM StDescEndPoint2FS = {
        // Endpoint Descriptor 
        SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (7)
        DESCRIPTOR_TYPE_ENDPOINT,           // (1) Device descriptor type (5)
        0x02,                               // (2) Address of the endpoint on the USB device described by this descriptor
                                            //      Bit 3..0: Endpoint number
                                            //      Bit 6..4: Reserved, reset to zero
                                            //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
        0x02,                               // (3) Endpoint attributes
                                            //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                            //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                            //      bits 7..6: Reserved bits, must be reset to zero.                                    
        EP_FS_MAX_PACKET_SIZE,              // (4) Maximum packet size. Refer to standard for bits specification
        0x00,                               // (5)
        0x00                                // (6) Interval for polling endpoint for data transfers (refer to standard)
    };

Struct_Standard_Endpoint_Desc _USB_MEM StDescEndPoint3FS = {
        // Endpoint Descriptor 
        SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (7)
        DESCRIPTOR_TYPE_ENDPOINT,           // (1) Device descriptor type (5)
        0x83,                               // (2) Address of the endpoint on the USB device described by this descriptor
                                            //      Bit 3..0: Endpoint number
                                            //      Bit 6..4: Reserved, reset to zero
                                            //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
        0x03,                               // (3) Endpoint attributes
                                            //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                            //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                            //      bits 7..6: Reserved bits, must be reset to zero.                                    
        EP_FS_MAX_PACKET_SIZE,              // (4) Maximum packet size. Refer to standard for bits specification
        0x00,                               // (5)
        0x10                                // (6) Interval for polling endpoint for data transfers (refer to standard)
    };

Struct_Standard_Endpoint_Desc _USB_MEM StDescEndPoint1HS = {
        // Endpoint Descriptor 
        SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (7)
        DESCRIPTOR_TYPE_ENDPOINT,           // (1) Device descriptor type (5)
        0x81,                               // (2) Address of the endpoint on the USB device described by this descriptor
                                            //      Bit 3..0: Endpoint number
                                            //      Bit 6..4: Reserved, reset to zero
                                            //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
        0x02,                               // (3) Endpoint attributes
                                            //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                            //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                            //      bits 7..6: Reserved bits, must be reset to zero.                                    
        (EP_HS_MAX_PACKET_SIZE & 0xff),     // (4) Maximum packet size. Refer to standard for bits specification
        ((EP_HS_MAX_PACKET_SIZE >> 8) & 0xff), // (5)
        0x00                                // (6) Interval for polling endpoint for data transfers (refer to standard)
    };
Struct_Standard_Endpoint_Desc _USB_MEM StDescEndPoint2HS = {
        // Endpoint Descriptor 
        SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (7)
        DESCRIPTOR_TYPE_ENDPOINT,           // (1) Device descriptor type (5)
        0x02,                               // (2) Address of the endpoint on the USB device described by this descriptor
                                            //      Bit 3..0: Endpoint number
                                            //      Bit 6..4: Reserved, reset to zero
                                            //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
        0x02,                               // (3) Endpoint attributes
                                            //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                            //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                            //      bits 7..6: Reserved bits, must be reset to zero.                                    
        (EP_HS_MAX_PACKET_SIZE & 0xff),     // (4) Maximum packet size. Refer to standard for bits specification
        ((EP_HS_MAX_PACKET_SIZE >> 8) & 0xff), // (5)
        0x00                                // (6) Interval for polling endpoint for data transfers (refer to standard)
    };
Struct_Standard_Endpoint_Desc _USB_MEM StDescEndPoint3HS = {
        // Endpoint Descriptor 
        SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (7)
        DESCRIPTOR_TYPE_ENDPOINT,           // (1) Device descriptor type (5)
        0x83,                               // (2) Address of the endpoint on the USB device described by this descriptor
                                            //      Bit 3..0: Endpoint number
                                            //      Bit 6..4: Reserved, reset to zero
                                            //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
        0x03,                               // (3) Endpoint attributes
                                            //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                            //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                            //      bits 7..6: Reserved bits, must be reset to zero.                                    
        EP_FS_MAX_PACKET_SIZE,     // (4) Maximum packet size. Refer to standard for bits specification
        0x00, 				// (5)
        0x10                                // (6) Interval for polling endpoint for data transfers (refer to standard)
    };

MS_OS_EXTENDED_CONFIGURATION_DESCRIPTOR Transfer_MSOS_Extended_Configuration = 
{
	{
		0x00000028,  										// Length 
		0x0100,												// Version info
		MSOS_FEATURE_INDEX_EXTENDED_CONFIG_DESC,			// MS OS Feature index LSB
		MS_OS_NUMBER_OF_FUNCTIONS_OF_DEVICE,        			// Number of function sections
		{0}                                                 			// RESERVED
	},
	{
        0x00,        // starting at interface                           
        0x01,	// number of interfaces
		"MTP",     // compatible ID
		"",         // sub compatible ID
		{0}	// RESERVED - filled with zeros at request time
 	}	
};


/////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT !!!!!!!!!!!! !!!!!!!!!!!!!!!!!
// Terminate the configuration rule with a NULL pointer
/////////////////////////////////////////////////////////////////////////////
BYTE _USB_MEM * Config1FSRule[] = 
            { // Full Speed Configuration
                (BYTE _USB_MEM *) &StDescConfig,
                (BYTE _USB_MEM *) &StDescInterface0,
                (BYTE _USB_MEM *) &StDescEndPoint1FS,
                (BYTE _USB_MEM *) &StDescEndPoint2FS,
                (BYTE _USB_MEM *) &StDescEndPoint3FS,
                NULL
            };
BYTE _USB_MEM * Config1HSRule[] = 
            { // High Speed Configuration
                (BYTE _USB_MEM *) &StDescConfig,
                (BYTE _USB_MEM *) &StDescInterface0,
                (BYTE _USB_MEM *) &StDescEndPoint1HS,
                (BYTE _USB_MEM *) &StDescEndPoint2HS,
                (BYTE _USB_MEM *) &StDescEndPoint3HS,
                NULL
            };

// The 1st configuration pointer is always the Full Speed
// The 2nd configuration pointer is always the High Speed
// If Full and High Speed configurations are the same set
// both entries to point the same configuration
BYTE _USB_MEM * ConfigStruct[] = 
            {
                (BYTE _USB_MEM *) Config1FSRule, 
                (BYTE _USB_MEM *) Config1HSRule,
                NULL
            };


////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//
//              USB Strings
//
//
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////



//Languages is a special case, and must be the first in the list of strings.
WORD g_LanguageList[] = {USB_LANGUAGE_ENGLISH,0};


//The serial number is 16 characters long, it will be filled in later by the USB startup.
//BYTE g_SerialNumber[]="0123456789012345";


//Languages is a special case, and must be the first in the list of strings.
void *g_pStrings[]= 
{
    g_LanguageList,
    (BYTE *)g_Manufacturer,
    (BYTE *)g_ProductName,
    g_ChipSerialNumber.btAscii,
    (BYTE *)g_ConfigString,
    (BYTE *)g_Interface0String,
    (BYTE *)g_Interface1String,
    (BYTE *)g_MSOSDescriptorString,
    NULL
};

