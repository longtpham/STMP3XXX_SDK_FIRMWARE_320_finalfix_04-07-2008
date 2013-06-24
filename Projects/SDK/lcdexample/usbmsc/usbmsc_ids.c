#include "types.h"
#include "usb_api.h"


// Customization - only works with USB strings.  SCSI strings are required to 
// be ASCII (see SPC-2, page 86 - Inquiry command).
// If customization in Unicode is desired, uncomment the CUSTOMIZE_UNICODE
// line in usbmsc.mk
#ifndef CUSTOMIZE_UNICODE
// If using ASCII characters, replace the following string with your own.
const BYTE g_Manufacturer[]="<USB MF>";             // sz 9 words including NULL
const BYTE g_ProductName[] ="<USB PRODUCT>   ";     // sz 17 "
const BYTE g_ConfigString[]="USB/MSC LCD Player";   // sz 19 "
const BYTE g_Interface0String[]="Internal Device";  // sz 16 "
const BYTE g_Interface1String[]="External Device";  // sz 16 "    total sz= 77 words


#else   // CUSTOMIZE_UNICODE

// Insert the unicode to be used below terminated with a zero.
//const WORD g_Manufacturer[]= { 0x003C,0x0055,0x0053,0x0042,0x0020,0x004D,0x0046,0x003E, // English
// Put your company's name here
const WORD g_Manufacturer[]= { 0x60A8,0x516C,0x53F8,0x540D,0x79F0,0x5728,0x8FD9,0x91CC,   // Chinese
                               0x0000};
//const WORD g_ProductName[] = { 0x003C,0x0055,0x0053,0x0042,0x0020,0x0050,0x0052,0x004F, // English
//                               0x0044,0x0055,0x0043,0x0054,0x003e,0x0020,0x0020,0x0020, // English
// Put your company's product name here
const WORD g_ProductName[] = { 0x8BF7,0x5C06,0x60A8,0x516C,0x53F8,0x4EA7,0x54C1,0x540D,   // Chinese
                               0x79F0,0x653E,0x5728,0x8FD9,0x91CC,0x0020,0x0020,0x0020,   // Chinese
                               0x0000};
// Unicode string for "USB/MSC LCD Player"
const WORD g_ConfigString[] = { 0x0055,0x0053,0x0042,0x002F,0x004D,0x0053,0x0043,0x0020,  // English
                                0x004C,0x0043,0x0044,0x0020,0x0050,0x006C,0x0061,0x0079,  // English
                                0x0065,0x0072,0x0000};                                    // English
// Unicode string for "Internal Device"						 
const WORD g_Interface0String[] = { 0x0049,0x006E,0x0074,0x0065,0x0072,0x006E,0x0061,0x006C,  // English
                                    0x0020,0x0044,0x0065,0x0076,0x0069,0x0063,0x0065,0x0000}; // English
// Unicode string for "External Device"	 
// This string is only used in Non-Multi LUN build.						 
const WORD g_Interface1String[] = { 0x0045,0x0078,0x0074,0x0065,0x0072,0x006E,0x0061,0x006C,  // English
                                    0x0020,0x0044,0x0065,0x0076,0x0069,0x0063,0x0065,0x0000}; // English
#endif

// End of USB Strings

// SCSI requires the strings to be in ASCII format.                         
// Still need SCSI strings in ASCII (SCSI Inquiry - SPC-2 page 86).
#ifdef UPDATER
    const BYTE g_SCSIVendorID[]= "_Generic";    //Must be 8 characters (leave spaces if necessary)
#else
    const BYTE g_SCSIVendorID[]= "SigmaTel";    //Must be 8 characters (leave spaces if necessary)
#endif

#ifdef UPDATER
    const BYTE g_SCSIProductID[]= "MSC Recovery    "; //Must be 16 characters (leave spaces if necessary)
#else
    #ifndef MFG_TOOL
        #ifdef MMC
        const BYTE g_SCSIProductID[]= "MSCNMMC         "; //for MMC players
        #else
        const BYTE g_SCSIProductID[]= "MSCN            ";   //Must be 16 characters (leave spaces if necessary)
        #endif
    #else
    const BYTE g_SCSIProductID[]= "StMfgTool       ";   //Must be 16 characters (leave spaces if necessary)
    #endif
#endif

WORD g_wUSBVendorID = 0x066f;

#ifdef UPDATER
    WORD g_wUSBProductID = 0xA000;  //for updater.sb
#else
    #ifndef MFG_TOOL
        #ifdef MMC
        WORD g_wUSBProductID = 0x8004;  //for MMC/SD players
        #else
        WORD g_wUSBProductID = 0x8000;  //for NAND only players
        #endif
    #else
    WORD g_wUSBProductID = 0x0000;  //for MFG Tool
    #endif
#endif


#if (MULTI_LUN_BUILD==0)
WORD g_wUSBReleaseID = 0x1000;
#else
WORD g_wUSBReleaseID = 0x1001;
#endif
