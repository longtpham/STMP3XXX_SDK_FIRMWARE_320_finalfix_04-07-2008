/**
***
*** Filename: mtp_ids.c
*** Description: MTP ID variables
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
#include "usb_api.h"
//#include "support.h"  //TOVEROFY was present on lbmlc branch but not on mtp sdk3.0 project. Verify not needed then delete line.
#include "mtp.h"

// Customization - only works with USB strings.  SCSI strings are required to 
// be ASCII (see SPC-2, page 86 - Inquiry command).
// If customization in Unicode is desired, uncomment the CUSTOMIZE_UNICODE
// line in usbmtp.mk
#ifndef CUSTOMIZE_UNICODE
// If using ASCII characters, replace the following string with your own.

const BYTE g_Manufacturer[]="<USB MF>";             // currently sz 9 words including NULL
const BYTE g_ProductName[] ="MTP Player      ";     // sz 17 "
const BYTE g_ConfigString[]="USB/MTP LCD Player";   // sz 19 "
const BYTE g_Interface0String[]="Internal Device";  // sz 16 "
const BYTE g_Interface1String[]="External Device";  // sz 16 "
const BYTE g_MSOSDescriptorString[]="MSFT100";      // sz  8 " 
// The above strings at 1 word per char plus NULLs total 76 words on mar 25 2005. 
// call these const so moves to low y_const hole 
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
// Unicode string for "USB/MTP LCD Player"
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
// Unicode string for "MSFT100"	 
const WORD g_MSOSDescriptorString[] = { 0x004D, 0x0053, 0x0046, 0x0054, 0x0031, 0x0030, 0x0030 };
// mar 25 '05: 84 words for these CUSTOMIZE_UNICODE strings. const keyword added so now in low y_const mem hole.
#endif

// End of USB Strings
// SCSI requires the strings to be in ASCII format.                         
// Still need SCSI strings in ASCII (SCSI Inquiry - SPC-2 page 86).
const BYTE g_SCSIVendorID[]= "SigmaTel";			  //Must be 8 characters (leave spaces if necessary)
#ifdef MMC
const BYTE g_SCSIProductID[]= "MSCNMMC         ";	//for MMC players
#else
const BYTE g_SCSIProductID[]= "MTPMSCN         ";	  //Must be 16 characters (leave spaces if necessary). 
// Changed above line from MSCN to MTPMSCN July14 2006 as requested by host developer SP in stmp00011735
#endif

WORD g_wUSBVendorID = 0x066F;

#ifdef MMC
WORD g_wUSBProductID = 0xA011;	//for MMC/SD players
#else
WORD g_wUSBProductID = 0xA010;	//for NAND only players
#endif

#if (MULTI_LUN_BUILD==0)
WORD g_wUSBReleaseID = 0x0100;
#else
WORD g_wUSBReleaseID = 0x0100;
#endif



#if 0

//
// This variable will be sucked up as a resource.  It will be loaded in GetDeviceInfo.  Be sure not to define
// other variables in _X memory or they will also be sucked up.
//
StaticRscMtpDeviceInfo _X g_sMtpDeviceInfo = {

/* StandardVersion */         0x000064,
/* VendorExtensionID */       0x000000000006,
/* VendorExtensionVersion */  0x000064,
#ifndef AUDIBLE
/* The following is a PtpString structures (256 UINT16 with a byte length)
/* VendorExtensionDesc */      0x000030, // length, "microsoft.com: 1.0; microsoft.com/WMDRMPD: 10.1"
                0x00006d, 0x000069, 0x000063, 0x000072, 0x00006f, 0x000073, 0x00006f, 0x000066,
                0x000074, 0x00002e, 0x000063, 0x00006f, 0x00006d, 0x00003a, 0x000020, 0x000031,
                0x00002e, 0x000030, 0x00003b, 0x000020, 0x00006d, 0x000069, 0x000063, 0x000072,
                0x00006f, 0x000073, 0x00006f, 0x000066, 0x000074, 0x00002e, 0x000063, 0x00006f,
                0x00006d, 0x00002f, 0x000057, 0x00004d, 0x000044, 0x000052, 0x00004d, 0x000050,
                0x000044, 0x00003a, 0x000020, 0x000031, 0x000030, 0x00002e, 0x000031, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
#else
/* The following is a PtpString structures (256 UINT16 with a byte length)
/* VendorExtensionDesc */      0x000041, // length, "microsoft.com: 1.0; microsoft.com/WMDRMPD: 10.1; audible.com: 1.0"
                0x00006d, 0x000069, 0x000063, 0x000072, 0x00006f, 0x000073, 0x00006f, 0x000066,
                0x000074, 0x00002e, 0x000063, 0x00006f, 0x00006d, 0x00003a, 0x000020, 0x000031,
                0x00002e, 0x000030, 0x00003b, 0x000020, 0x00006d, 0x000069, 0x000063, 0x000072,
                0x00006f, 0x000073, 0x00006f, 0x000066, 0x000074, 0x00002e, 0x000063, 0x00006f,
                0x00006d, 0x00002f, 0x000057, 0x00004d, 0x000044, 0x000052, 0x00004d, 0x000050,
                0x000044, 0x00003a, 0x000020, 0x000031, 0x000030, 0x00002e, 0x000031, 0x00003b,
                0x000020, 0x000061, 0x000075, 0x000064, 0x000069, 0x000062, 0x00006c, 0x000065,
                0x00002e, 0x000063, 0x00006f, 0x00006d, 0x00003a, 0x000020, 0x000031, 0x00002e,
                0x000030, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
                0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
#endif
/* FunctionalMode */        PTP_FUNCTIONMODE_STDANDARD,

/* OpsSupported */          NUM_OPS_SUPPORTED,
/* OP_SUPPORTED_LIST */      
                            PTP_OPCODE_GETDEVICEINFO,
                            PTP_OPCODE_OPENSESSION,
                            PTP_OPCODE_CLOSESESSION,
                            PTP_OPCODE_GETSTORAGEIDS,
                            PTP_OPCODE_GETSTORAGEINFO,
                            PTP_OPCODE_GETNUMOBJECTS,
                            PTP_OPCODE_GETOBJECTHANDLES,
                            PTP_OPCODE_GETOBJECTINFO,
                            PTP_OPCODE_GETOBJECT,
                            PTP_OPCODE_DELETEOBJECT,
                            PTP_OPCODE_SENDOBJECTINFO,
                            PTP_OPCODE_SENDOBJECT,
                            PTP_OPCODE_GETDEVICEPROPDESC,
                            PTP_OPCODE_GETDEVICEPROPVALUE,
                            PTP_OPCODE_SETDEVICEPROPVALUE,
                            MTP_OPCODE_GETOBJECTPROPDESC,
                            MTP_OPCODE_GETOBJECTPROPSUPPORTED,
                            MTP_OPCODE_GETOBJECTPROPVALUE,
                            MTP_OPCODE_SETOBJECTPROPVALUE,
                            MTP_OPCODE_GETOBJECTPROPLIST,
                            MTP_OPCODE_SETOBJECTPROPLIST,
                            MTP_OPCODE_SENDOBJECTPROPLIST,
                            MTP_OPCODE_JANUS_GETSECURETIMECHALLENGE,
                            MTP_OPCODE_JANUS_SETSECURETIMERESPONSE,   
                            MTP_OPCODE_JANUS_SETLICENSERESPONSE,    
                            MTP_OPCODE_JANUS_GETSYNCLIST,       
                            MTP_OPCODE_JANUS_SEND_METER_CHALLENGE_QUERY,
                            MTP_OPCODE_JANUS_GET_METER_CHALLENGE,      
                            MTP_OPCODE_JANUS_SET_METER_RESPONSE,            
                            MTP_OPCODE_JANUS_CLEANDATASTORE,
                            MTP_OPCODE_JANUS_GET_LICENSE_STATE,
                            MTP_OPCODE_JANUS_SEND_WMDRMPD_COMMAND,
                            MTP_OPCODE_JANUS_SEND_WMDRMPD_REQUEST,  
                            PTP_OPCODE_FORMATSTORE,
                            MTP_OPCODE_SIGMATEL_RESET,
                            MTP_OPCODE_SIGMATEL_ERASEBOOT,           

/* EventsSupported */       NUM_EVENTS_SUPPORTED,

/* EVENT_SUPPORTED_LIST[] */ 
                            PTP_EVENTCODE_OBJECTADDED,
                            PTP_EVENTCODE_OBJECTREMOVED,

// None for these two
/* DevicePropsSupported */  NUM_DEVICE_PROPERTIES,
#ifdef AUDIBLE
                            MTP_PROPERTYCODE_AUDIBLE_ACTIVATION_DATA,
#endif
                            MTP_PROPERTYCODE_JANUS_SECURETIME,
                            MTP_PROPERTYCODE_JANUS_DEVICECERTIFICATE,

/* CaptureFormatsSupported */   0,

/* ObjectFormatsSupported */NUM_OBJECT_FORMATS,
/* OBJECT_FORMAT_LIST[] */  
                            PTP_FORMATCODE_MP3,
                            PTP_FORMATCODE_WAVE,
                            MTP_FORMATCODE_WMA,
#ifdef AUDIBLE
                            MTP_FORMATCODE_AUDIBLE,
#endif
                            PTP_FORMATCODE_ASSOCIATION,
                            PTP_FORMATCODE_UNDEFINED,
/* The following are short PtpString structures (32 UINT16 each with a byte length)
/* Manufacturer[] */     0x0009,     // length, "Sigmatel"
                0x0053, 0x0069, 0x0067, 0x006D, 0x0061, 0x0054, 0x0065, 0x006C,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
/* Model[] */            0x0011,     // length, "MTP Player "
                0x004D, 0x0054, 0x0050, 0x0020, 0x0050, 0x006C, 0x0061, 0x0079,
                0x0065, 0x0072, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

/* DeviceVersion[] */   0x000E,     // length, "%OUT_PAD:128%"
                0x0025, 0x004F, 0x0055, 0x0054, 0x005F, 0x0050, 0x0041, 0x0044,
                0x003A, 0x0031, 0x0032, 0x0038, 0x0025, 0x0000, 0x0000, 0x0000,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

/* SerialNumber[] */     0x000E,     // length, "STMPMTP000001"
                0x0053, 0x0054, 0x004D, 0x0050, 0x004D, 0x0054, 0x0050, 0x0030,
                0x0030, 0x0030, 0x0030, 0x0030, 0x0031, 0x0000, 0x0000, 0x0000,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
                0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
}; // end of g_sMtpDeviceInfo initialization

#endif