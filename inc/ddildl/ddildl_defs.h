////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename: ddildl_defs.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _DDILDL_DEFS_H
#define _DDILDL_DEFS_H

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////

typedef enum {
  MediaStateUnknown,
  MediaStateErased,
  MediaStateAllocated
} MEDIA_STATE, * P_MEDIA_STATE;

typedef enum {
    MediaTypeNand = 0,
    MediaTypeMMC = 1,
    MediaTypeHDD = 2,
    MediaTypeRAM = 3
} PHYSICAL_MEDIA_TYPE, * P_PHYSICAL_MEDIA_TYPE;

typedef enum {
    MediaInfoNumberOfDrives = 0,
    MediaInfoSizeInBytes = 1,
    MediaInfoAllocationUnitSizeInBytes = 2,
    MediaInfoIsInitialized = 3,
    MediaInfoMediaState = 4,
    MediaInfoIsWriteProtected = 5,
    MediaInfoPhysicalMediaType = 6,
    MediaInfoSizeOfSerialNumberInBytes = 7,
    MediaInfoSerialNumber = 8,
    MediaInfoIsSystemMedia = 9,
    MediaInfoIsMediaPresent = 10
} LOGICAL_MEDIA_INFO, * P_LOGICAL_MEDIA_INFO;

#ifdef MFG_TOOL
  
typedef enum {
    SerialNoInfoSizeOfSerialNumberInBytes = 0,
    SerialNoInfoSerialNumber = 1
} SERIAL_NO_INFO, * P_SERIAL_NO_INFO;
#endif

typedef enum {
    DriveInfoSectorSizeInBytes = 0,
    DriveInfoEraseSizeInBytes = 1,
    DriveInfoSizeInBytes = 2,
    DriveInfoSizeInMegaBytes = 3,
    DriveInfoSizeInSectors = 4,
    DriveInfoType = 5,
    DriveInfoTag = 6,
    DriveInfoComponentVersion = 7,
    DriveInfoProjectVersion = 8,
    DriveInfoIsWriteProtected = 9,
    DriveInfoSizeOfSerialNumberInBytes = 10,
    DriveInfoSerialNumber = 11,
    DriveInfoMediaPresent = 12,
    DriveInfoMediaChange = 13,
    DriveInfoSectorAllocation = 14
} LOGICAL_DRIVE_INFO, * P_LOGICAL_DRIVE_INFO;

#define DRIVE_TAG_STMPSYS_S         0x00        //!< player drive tag.
#define DRIVE_TAG_HOSTLINK_S        0x01        //!< UsbMscMtp drive tag, old name was DRIVE_TAG_USBMSC_S. 
#define DRIVE_TAG_RESOURCE_BIN      0x02        //!< player resource drive tag.
#define DRIVE_TAG_EXTRA_S           0x03        //!< the host has 0x03 reserved for an extra system drive.
// if you change line below, also change this equ: asm_DRIVE_TAG_RESOURCE1_BIN in player main.asm!! Values must match. 
#define DRIVE_TAG_RESOURCE1_BIN (DRIVE_TAG_RESOURCE_BIN+0x10)//<! was just 0x04. old comment"the host has 0x04 reserved for an extra system drive." May17'06 JLN: correction: ChrisW says 0x10+resource.bin tag is tag for 2nd ver of resource.bin in new 2.61x post sdkrelease updater. 
#define DRIVE_TAG_RESOURCE2_BIN (DRIVE_TAG_RESOURCE_BIN+0x20)// master has tag 0x22 (3rd copy)
#define DRIVE_TAG_OTGHOST_S         0x05        //!< the host has 0x05 reserved for OTG drive.
#define DRIVE_TAG_HOSTRSC_BIN       0x06        //!< UsbMscMtp resource drive tag, old name was DRIVE_TAG_MTP_BIN.
#define DRIVE_TAG_DATA              0x0A        //!< data drive tag.
#define DRIVE_TAG_DATA_HIDDEN       0x0B        //!< hidden data drive tag, old name was DRIVE_TAG_HIDDEN
#define DRIVE_TAG_BOOTMANAGER_S     0x50        //!< boot manager drive tag
#define DRIVE_TAG_UPDATER_S         0xFF        //!< the host has 0xFF reserved for usbmsc.sb file used in recovery mode operation only.
#define DRIVE_TAG_HOSTRSC1_BIN      0x16        //!< back up resource for hostrsrc.bin.
#define DRIVE_TAG_HOSTRSC2_BIN      0x26        //!< MASTER COPY resource for hostrsrc.bin.
// Do not use this enum... use the defs above.  We need to use defs so customers
//  may extend the system drives without DDI source code.
/*
typedef enum {
    ResourceBinDriveTag = 0x00,
    BootManagerDriveTag = 0x50,
    StmpSysDriveTag = 0x01,
    UsbMscDriveTag = 0x02,
    DataDriveTag = 0x0A
} LOGICAL_DRIVE_TAG, * P_LOGICAL_DRIVE_TAG;
*/

typedef enum {
    DriveTypeData = 0,
    DriveTypeSystem = 1,
    DriveTypeHidden = 2,
    DriveTypeUnknown = 3
} LOGICAL_DRIVE_TYPE, * P_LOGICAL_DRIVE_TYPE;

typedef struct {
    WORD wDriveNumber;          // In reference to the entire system
    LOGICAL_DRIVE_TYPE Type;
    WORD wTag;
    DWORD dwSizeInBytes;
    BOOL bRequired;
} MEDIA_ALLOCATION_TABLE_ENTRY, * P_MEDIA_ALLOCATION_TABLE_ENTRY;

typedef struct {
    WORD wNumEntries;
    MEDIA_ALLOCATION_TABLE_ENTRY Entry[MAX_MEDIA_TABLE_ENTRIES];
} MEDIA_ALLOCATION_TABLE, * P_MEDIA_ALLOCATION_TABLE;


#define TA6_ROM_REV_ID  6
#define ROM_REV_ID  (*((WORD _X *)0xFA02) & 0x0F)

 
typedef union {

    struct {
        WORD MinorL      : 8;
        WORD MinorH      : 8;
        WORD MiddleL     : 8;
        WORD MiddleH     : 8;
        WORD MajorL      : 8;
        WORD MajorH      : 8;
        } PARTIAL_VERSION;
        
    DWORD   Version;        

} SYSTEM_VERSION;


#endif // #ifndef _DDILDL_DEFS_H
