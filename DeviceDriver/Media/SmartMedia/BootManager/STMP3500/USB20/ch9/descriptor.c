////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002 - 2003
//
// File : Descriptor.c
// Description : Descriptor handler
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "usb20.h"
#include "usb20project.h"
#include "descriptor.h"
#include "regsrevision.h"


BYTE _packed _P btDevDescInP[SIZE_DEVICE_DESCRIPTOR_IN_BYTES] = {
    SIZE_DEVICE_DESCRIPTOR_IN_BYTES,    // (0) Size of this descriptor in bytes (18)
    0x01,                               // (1) Device descriptor type (1)
    0x00,                               // (2) USB specification release number (for version 2.00 0x00)
    0x02,                               // (3)                                  (for version 2.00 0x02)     
    0x00,                               // (4) Class code
    0x00,                               // (5) Subclass code
    0x00,                               // (6) Protocol code
    APP_CONTROL_MAX_PKT_SIZE,           // (7) Maximum packet size for endpoint 0 (8, 16, 32 or 64)
    0x6f,                               // (8) Vendor Id 
    0x06,                               // (9)
    0x00,                               // (10) Product Id - Auto fill by reading Revision Register
    0x35,                               // (11)
    0,                                  // (12) Device release number in BCD - Auto fill in
    0,                                  // (13) by reading the REVID Register
    0x01,                               // (14) Index of string descriptor describing Manufacturer
    0x02,                               // (15) Index of string descriptor describing Product
    0,                                  // (16) Index of string descriptor describing Device Serial Number
    NUMBER_OF_CONFIG                    // (17) Number of possible configurations
};


//================================================================================
// Describes information about a high-speed capable device that would change if
// the device were operating at the other speed.
//
// In this application, the device operates in the same manner at both speeds.
//================================================================================
BYTE _packed _P btDevQualifierDescInP[SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_BYTES] = {
    SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (10)
    0x06,                               // (1) Device descriptor type (6)
    0x00,                               // (2) USB specification release number (for version 2.00 0x00)
    0x02,                               // (3)                                  (for version 2.00 0x02)     
    0x00,                               // (4) Class code
    0x00,                               // (5) Subclass code
    0x00,                               // (6) Protocol code
    APP_CONTROL_MAX_PKT_SIZE,           // (7) Maximum packet size for endpoint 0 (8, 16, 32 or 64)
    NUMBER_OF_CONFIG,                   // (8) Number of possible configurations
    0x00                                // (9) Reserved must be zero
};



// In this application there are:
//      1 Configuration (config 0)
//          The config 0 has:
//              1 Interface (Interface 0)
//                  The interface 0 has:
//                      1 Endpoint (excluding endpoint 0)      
#define CONFIG1_DESC_SIZE    ((SIZE_CONFIG_DESCRIPTOR_IN_BYTES) + (SIZE_INTERFACE_DESCRIPTOR_IN_BYTES) \
                                + ((MAX_NUM_EP-1)*SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES))
#define CONFIG1_DESC_SIZE_LSB   ((CONFIG1_DESC_SIZE) & (0x00ff))
#define CONFIG1_DESC_SIZE_MSB   ((CONFIG1_DESC_SIZE >> 8) & (0x00ff))   
#define CONFIG1_DESC_SIZE_WORDS     9

//================================================================================
// Declare and define the Complete Configuration Descriptor for each Configuration
//================================================================================

//================================================================================
// In this application, the device operates in the same manner at both speeds.
// Therefore we will use the same descriptor for Configuration Descriptor
// and Other Speed Configuration Descriptor. The only thing changing is the
// descriptor type (0x07 for other speed config). It will be changed dynamically
// within the function returning such descriptor.
//================================================================================
BYTE _packed _P btConfig1DescInP[CONFIG1_DESC_SIZE] = {
        // Config Descriptor
        SIZE_CONFIG_DESCRIPTOR_IN_BYTES,    // (0) Size of this descriptor in bytes (9)
        0x02,                               // (1) Device descriptor type (2 or 7)
        CONFIG1_DESC_SIZE_LSB,              // (2) Total length of data returned for this config.
        CONFIG1_DESC_SIZE_MSB,              // (3)                                  
        CONFIG_DESC_NUM_INTERFACES,         // (4) Number of Interfaces supported by this configuration
        0x01,                               // (5) Value to use as an argument to the SetConfiguration() request
        0x00,                               // (6) Index of string descriptor describing this configuration
        0xC0,                               // (7) Configuration Characteristics
                                            //     D7:      Reserved (set to one)
                                            //     D6:      Self-powered
                                            //     D5:      Remote Wakeup
                                            //     D4..0:   Reserved (reset to zero)        
        0x32,                               // (8) Maximum power consumption expressed in 2mA units.
        // Interface Descriptor (One unique interface)
        SIZE_INTERFACE_DESCRIPTOR_IN_BYTES, // (0) Size of this descriptor in bytes (9)
        0x04,                               // (1) Device descriptor type (4)
        0x00,                               // (2) Number of this interface. Zero-based value.
        0x00,                               // (3)                                  
        MAX_NUM_EP-1,                       // (4) Number of endpointsused by this interface (excluding endpoint 0).
        0xff,                               // (5) Class code
        0x00,                               // (6) Sub Class code
        0xff,                               // (7) Protocol Code
        0x00,                               // (8) Index of string descriptor describing this interface
        // Endpoint Descriptor (one unique endpoint)
        SIZE_ENDPOINT_DESCRIPTOR_IN_BYTES,  // (0) Size of this descriptor in bytes (7)
        0x05,                               // (1) Device descriptor type (5)
        0x01,                               // (2) Address of the endpoint on the USB device described by this descriptor
                                            //      Bit 3..0: Endpoint number
                                            //      Bit 6..4: Reserved, reset to zero
                                            //      Bit 7:    Direction. Ignored for control endpoints (0 OUT, 1 IN)
        0x02,                               // (3) Endpoint attributes
                                            //      bits 1..0: Transfer Type (00 Control, 01 Iso, 10 Bulk, 11 Int)
                                            //      bits 5..2: If not an iso endpoints, they are reserved and must be reset to zero.
                                            //      bits 7..6: Reserved bits, must be reset to zero.                                    
        APP_CONTROL_MAX_PKT_SIZE,           // (4) Maximum packet size. Refer to standard for bits specification
        0x00,                               // (5)
        0x00                                // (6) Interval for polling endpoint for data transfers (refer to standard)
};


// Reserve memory for packed descriptor
BYTE _packed _X btDevDescInX[SIZE_DEVICE_DESCRIPTOR_IN_BYTES];                      // Only One of those
BYTE _packed _X btDevQualifierDescInX[SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_BYTES];   // Only one of those
BYTE _packed _X btConfig1DescInX[CONFIG1_DESC_SIZE];                                // One of those per Configuration


//================================================================================
// String Descriptor 0 Definition
// This application support one language only (English).
//================================================================================
// Struct String descriptor zero
#define SIZE_STR_DESC_ZERO_IN_BYTES     (2+(2*MAX_USB_LANG))
#define SIZE_STR_DESC_ZERO_IN_WORDS     (2)

BYTE _packed _P btStringZeroDescInP[SIZE_STR_DESC_ZERO_IN_BYTES] = {
    SIZE_STR_DESC_ZERO_IN_BYTES,        // (0) Size of this descriptor in bytes (MAX_USB_LANG + 2)
    0x03,                               // (1) String descriptor type (3)
    0x09,
    0x04                                // LangId code array
};

// Reserve memory for packed descriptor
BYTE _packed _X btStringZeroDescInX[SIZE_STR_DESC_ZERO_IN_BYTES];       // Only one of those


//================================================================================
// Define all the strings Here ......
// This application support one language only (English).
// There are 2 strings not included last string ....
//================================================================================

#define SIZE_USB_STRING_1           (30)
#define SIZE_USB_STRING_1_IN_WORDS  (10)
BYTE _packed _P btUsbString1InP[SIZE_USB_STRING_1] = {
                                            SIZE_USB_STRING_1, 
                                            DESCRIPTOR_TYPE_STRING,
                                            'S', 0x00,
                                            'i', 0x00,
                                            'g', 0x00,
                                            'm', 0x00,
                                            'a', 0x00,
                                            't', 0x00,
                                            'e', 0x00,
                                            'l', 0x00,
                                            ',', 0x00,
                                            ' ', 0x00,
                                            'I', 0x00,
                                            'n', 0x00,
                                            'c', 0x00,
                                            '.', 0x00
                                            };

#define SIZE_USB_STRING_2           (18)
#define SIZE_USB_STRING_2_IN_WORDS  (6)
BYTE _packed _P btUsbString2InP[SIZE_USB_STRING_2] = {
                                            SIZE_USB_STRING_2, 
                                            DESCRIPTOR_TYPE_STRING,
                                            'S', 0x00,
                                            'T', 0x00,
                                            'M', 0x00,
                                            'P', 0x00,
                                            '3', 0x00,
                                            '5', 0x00,
                                            '0', 0x00,
                                            '0', 0x00
                                            };

#define SIZE_USB_STRING_LAST            (34)
#define SIZE_USB_STRING_LAST_IN_WORDS   (12)
BYTE _packed _P btUsbStringLastInP[SIZE_USB_STRING_LAST] = {
                                            SIZE_USB_STRING_LAST, 
                                            DESCRIPTOR_TYPE_STRING,
                                            'B', 0x00,
                                            'A', 0x00,
                                            'D', 0x00,
                                            ' ', 0x00,
                                            'S', 0x00,
                                            'T', 0x00,
                                            'R', 0x00,
                                            'I', 0x00,
                                            'N', 0x00,
                                            'G', 0x00,
                                            ' ', 0x00,
                                            'I', 0x00,
                                            'N', 0x00,
                                            'D', 0x00,
                                            'E', 0x00,
                                            'X', 0x00
                                            };


// Reserve memory for packed string descriptors
BYTE _packed _X btUsbString1InX[SIZE_USB_STRING_1];
BYTE _packed _X btUsbString2InX[SIZE_USB_STRING_2];
BYTE _packed _X btUsbStringLastInX[SIZE_USB_STRING_LAST];

// Reserve memory for string descriptors Index
BYTE _packed _X * _X pbtStringDescIndex[NUM_USB_STRINGS + 2];



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          PrepareDesc          
//
//   Type:          Function
//
//   Description:   Prepare descriptors by copying them from the P memory
//                  to X memory   
//
//   Inputs:        None
//                  
//   Outputs:       None
//
//   Notes: This function is needed only for the boot ROM because we cannot
//          declare and initialize the descriptors in X memory.
//          The functio also swaps lower byte and Upper byte        
//<
////////////////////////////////////////////////////////////////////////////////
void _reentrant PrepareDesc(void)
{
    INT i;
    WORD    wUpperByte;
    WORD    wLowerByte;
    WORD    wMidByte;

    // Copy Device descriptor from P to X
    for(i = 0 ; i < SIZE_DEVICE_DESCRIPTOR_IN_WORDS ; i++)
    {    
        wLowerByte = (*(btDevDescInP + i) & 0xff0000) >> 16;
        wUpperByte = (*(btDevDescInP + i) & 0x0000ff) << 16;
        wMidByte = *(btDevDescInP + i) & 0x00ff00;
        *(btDevDescInX + i) = wLowerByte | wMidByte | wUpperByte; 
    }        

    // Copy Device Qualifier descriptor from P to X
    for(i = 0 ; i < SIZE_DEVICE_QUALIFIER_DESCRIPTOR_IN_WORDS ; i++)
    {    
        wLowerByte = (*(btDevQualifierDescInP + i) & 0xff0000) >> 16;
        wUpperByte = (*(btDevQualifierDescInP + i) & 0x0000ff) << 16;
        wMidByte = *(btDevQualifierDescInP + i) & 0x00ff00;
        *(btDevQualifierDescInX + i) = wLowerByte | wMidByte | wUpperByte; 
    }        

    // Copy Configuration descriptor from P to X
    for(i = 0 ; i < CONFIG1_DESC_SIZE_WORDS ; i++)
    {    
        wLowerByte = (*(btConfig1DescInP + i) & 0xff0000) >> 16;
        wUpperByte = (*(btConfig1DescInP + i) & 0x0000ff) << 16;
        wMidByte = *(btConfig1DescInP + i) & 0x00ff00;
        *(btConfig1DescInX + i) = wLowerByte | wMidByte | wUpperByte; 
    }        

    // Copy String Zero descriptor from P to X
    for(i = 0 ; i < SIZE_STR_DESC_ZERO_IN_WORDS ; i++)
    {    
        wLowerByte = (*(btStringZeroDescInP + i) & 0xff0000) >> 16;
        wUpperByte = (*(btStringZeroDescInP + i) & 0x0000ff) << 16;
        wMidByte = *(btStringZeroDescInP + i) & 0x00ff00;
        *(btStringZeroDescInX + i) = wLowerByte | wMidByte | wUpperByte; 
    }        

    // Copy String 1 descriptor from P to X
    for(i = 0 ; i < SIZE_USB_STRING_1_IN_WORDS ; i++)
    {    
        wLowerByte = (*(btUsbString1InP + i) & 0xff0000) >> 16;
        wUpperByte = (*(btUsbString1InP + i) & 0x0000ff) << 16;
        wMidByte = *(btUsbString1InP + i) & 0x00ff00;
        *(btUsbString1InX + i) = wLowerByte | wMidByte | wUpperByte; 
    }        

    // Copy String 2 descriptor from P to X
    for(i = 0 ; i < SIZE_USB_STRING_2_IN_WORDS ; i++)
    {    
        wLowerByte = (*(btUsbString2InP + i) & 0xff0000) >> 16;
        wUpperByte = (*(btUsbString2InP + i) & 0x0000ff) << 16;
        wMidByte = *(btUsbString2InP + i) & 0x00ff00;
        *(btUsbString2InX + i) = wLowerByte | wMidByte | wUpperByte; 
    }        

    // Copy String Last descriptor from P to X
    for(i = 0 ; i < SIZE_USB_STRING_LAST_IN_WORDS ; i++)
    {    
        wLowerByte = (*(btUsbStringLastInP + i) & 0xff0000) >> 16;
        wUpperByte = (*(btUsbStringLastInP + i) & 0x0000ff) << 16;
        wMidByte = *(btUsbStringLastInP + i) & 0x00ff00;
        *(btUsbStringLastInX + i) = wLowerByte | wMidByte | wUpperByte; 
    }        

    // Initialize the string descriptor index array
    pbtStringDescIndex[0] = btStringZeroDescInX;
    pbtStringDescIndex[1] = btUsbString1InX;
    pbtStringDescIndex[2] = btUsbString2InX;
    pbtStringDescIndex[3] = btUsbStringLastInX;

}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetDeviceDesc          
//
//   Type:          Function
//
//   Description:   Get the pointer to the device descriptor
//                  
//
//   Inputs:        
//                  
//   Outputs:       pbtDesc = pointer to the descriptor
//                  usLength = Descriptor size in bytes
//
//   Notes: 
//          
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _X * _reentrant GetDeviceDesc(USHORT * usLength)
{
    WORD wReg;
    BYTE * pbtDesc;

    pbtDesc = (BYTE _X *)btDevDescInX;

    // Fill in the device ID
    wReg = HW_REVR.I & 0xffff00;
    *(pbtDesc+3) &= 0x0000ff;
    *(pbtDesc+3) |= wReg;

    // Fill in the revision ID
    wReg = HW_REVR.I & 0x00007f;
    *(pbtDesc+4) &= 0xff0000;
    *(pbtDesc+4) |= wReg;

    *usLength = (BYTE)(*pbtDesc & 0x0000ff);
    
    return(pbtDesc); 
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetConfigDesc          
//
//   Type:          Function
//
//   Description:   Get the pointer to the config descriptor
//                  
//
//   Inputs:        
//                  
//   Outputs:       pbtDesc = pointer to the descriptor
//                  usLength = Descriptor size in bytes
//
//   Notes: In this application config and other speed config are the same
//          but the descriptor type. This function always sets the type 
//          to the Config type. 
//          
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _X * _reentrant GetConfigDesc(USHORT * usLength)
{
    *(btConfig1DescInX) &= 0xff00ff;        // Mask the descriptor type
    *(btConfig1DescInX) |= 0x000200;        // Set descriptor type to device
    *usLength = (BYTE)CONFIG1_DESC_SIZE;
    
    return((BYTE _X *)btConfig1DescInX); 
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetStringDesc          
//
//   Type:          Function
//
//   Description:   Get the pointer to the specified string descriptor
//                  
//
//   Inputs:        
//                  btString : String Descriptor Number
//                  
//   Outputs:       pbtDesc = pointer to the descriptor
//                  usLength = Descriptor size in bytes
//
//   Notes: 
//          
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _X * _reentrant GetStringDesc(USHORT * usLength, BYTE btString)
{
    BYTE _X * pbtDesc;
    
    // Check for valid srting
    if(btString > NUM_USB_STRINGS)
        btString = NUM_USB_STRINGS + 1;     // Set string number to last string

    pbtDesc = (BYTE _X *)pbtStringDescIndex[btString];
    *usLength = (BYTE)(*pbtDesc & 0x0000ff);
    
    return(pbtDesc); 
}


////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetDeviceQualifierDesc          
//
//   Type:          Function
//
//   Description:   Get the pointer to the device qualifier descriptor
//                  
//
//   Inputs:        
//                  
//   Outputs:       pbtDesc = pointer to the descriptor
//                  usLength = Descriptor size in bytes
//
//   Notes: 
//          
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _X * _reentrant GetDeviceQualifierDesc(USHORT * usLength)
{
    *usLength = (BYTE)(*btDevQualifierDescInX & 0x0000ff); 
    return((BYTE _X *)btDevQualifierDescInX);
}



////////////////////////////////////////////////////////////////////////////////
//
//>  Name:          GetOtherSpeedConfigDesc          
//
//   Type:          Function
//
//   Description:   Get the pointer to the other speed config descriptor
//                  
//   Inputs:        
//                  
//   Outputs:       pbtDesc = pointer to the descriptor
//                  usLength = Descriptor size in bytes
//
//   Notes: In this application config and other speed config are the same
//          but the descriptor type. This function always sets the type 
//          to the other speed config type. 
//          
//<
////////////////////////////////////////////////////////////////////////////////
BYTE _X * _reentrant GetOtherSpeedConfigDesc(USHORT * usLength)
{
    *(btConfig1DescInX) &= 0xff00ff;        // Mask the descriptor type
    *(btConfig1DescInX) |= 0x000700;        // Set descriptor type to device
    *usLength = (BYTE)(*btConfig1DescInX & 0x0000ff); 
    return((BYTE _X *)btConfig1DescInX);
}
