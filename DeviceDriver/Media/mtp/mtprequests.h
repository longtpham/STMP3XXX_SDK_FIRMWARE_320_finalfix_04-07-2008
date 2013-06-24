////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2002
//
// Filename: usbmscrequests.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _MTPREQUESTS_H
#define _MTPREQUESTS_H

////////////////////////////////////////////////////////////////////////////////
// Defs
////////////////////////////////////////////////////////////////////////////////
#define MSOS_VENDORCODE_TO_GET_MS_DESCRIPTOR    0x01
#define MSOS_FEATURE_INDEX_EXTENDED_CONFIG_DESC 0x0004
#define MSOS_FEATURE_INDEX_EXTENDED_PROP_DESC   0x0005

#define MS_OS_NUMBER_OF_FUNCTIONS_OF_DEVICE   0x01

#define HOST_TO_DEVICE          (0<<7)  // xfer direction OUT
#define DEVICE_TO_HOST          (1<<7)  // xfer direction IN
//
#define STANDARD                (0<<5)  // Type
#define CLASS                   (1<<5)  // Type
#define VENDOR                  (2<<5)  // Type
#define RESERVED                (3<<5)  // Type
// 
#define RECIPIENT_DEVICE        0       // Recipient
#define RECIPIENT_INTERFACE     1       // Recipient
#define RECIPIENT_ENDPOINT      2       // Recipient
#define RECIPIENT_OTHER         3       // Recipient
////////////////////////////////////////////////////////////////////////////////
// Errors
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////
typedef struct _MSOS_EXT_CONFIG_DESCRIPTOR_HEADER
{
    UINT32 dwLength;         // Length of the entire Extended Configuration Descriptor
    UINT16 bcdVersion;       // Version info in BCD: 0x0100 ~ 1.00
    UINT16 wIndex;           // must be MSOS_FEATURE_INDEX_EXTENDED_CONFIG_DESC, or 0x0004
    BYTE  bCount;           // total number of Function Sections that follow the header section
    BYTE  bReserved[7];     // RESERVED, should be 0x00
}   MSOS_EXT_CONFIG_DESCRIPTOR_HEADER , *PMSOS_EXT_CONFIG_DESCRIPTOR_HEADER ;

typedef struct _MSOS_EXT_CONFIG_DESCRIPTOR_FUNCTION
{
    BYTE  bFirstInterfaceNumber; // Starting Interface Number for this function
    BYTE  bInterfaceCount;       // Total number of interfaces 
    BYTE  compatibleID[8];       // Compatible ID as defined by Microsoft
    BYTE  subCompatibleID[8];    // The Sub Compatible ID as defined by Microsoft
    BYTE  bReserved[6];          // RESERVED, should be 0x00
}   MSOS_EXT_CONFIG_DESCRIPTOR_FUNCTION , *PMSOS_EXT_CONFIG_DESCRIPTOR_FUNCTION ;

typedef struct _MSOS_EXT_PROP_DESCRIPTOR_HEADER
{
    UINT32 dwLength;         // Length of the entire Extended Property Descriptor
    UINT16 bcdVersion;       // Version info in BCD: 0x0100 ~ 1.00
    UINT16 wIndex;           // must be MSOS_FEATURE_INDEX_EXTENDED_PROP_DESC, or 0x0005
    UINT16 wCount;           // total number of Custom Proeprty Sections that follow the header section
}   MSOS_EXT_PROP_DESCRIPTOR_HEADER , *PMSOS_EXT_PROP_DESCRIPTOR_HEADER ;

typedef struct _MS_OS_EXTENDED_CONFIGURATION_DESCRIPTOR
{   // A USB configuration contains all interfaces and all endpoint descriptors
    MSOS_EXT_CONFIG_DESCRIPTOR_HEADER   HeaderSection;
    MSOS_EXT_CONFIG_DESCRIPTOR_FUNCTION FunctionSection[MS_OS_NUMBER_OF_FUNCTIONS_OF_DEVICE];
} MS_OS_EXTENDED_CONFIGURATION_DESCRIPTOR, * PMS_OS_EXTENDED_CONFIGURATION_DESCRIPTOR;

////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////
void _reentrant MTPRequestHandler(BOOL bSetup, BYTE btDirection, WORD  _X *pbtBuffer, WORD wLength,WORD wPrivateData);



#endif // #ifndef _MTPREQUESTS_H
