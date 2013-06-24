////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: scsi.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _SCSI_H
#define _SCSI_H

#include "types.h"
#include "filesystem.h"
#include "sectorarrayhandler.h"
////////////////////////////////////////////////////////////////////////////////
// Debug stuff
////////////////////////////////////////////////////////////////////////////////

    #ifdef SCSI_COMMAND_HISTORY
#define SCSI_COMMAND_HISTORY_SIZE       500
#define SCSI_CDB_SIZE_IN_WORDS          6
    #endif
    
////////////////////////////////////////////////////////////////////////////////
// Defs
////////////////////////////////////////////////////////////////////////////////

#define SCSI_CDB_BUFFER_OP_CODE_MASK    (WORD)0x0000FF

////////////////////////////////////////////////////////////////////////////////
// Supported SCSI commands
#define SCSI_OPCODE_INQUIRY                         0x12
#define SCSI_OPCODE_READ_CAPACITY                   0x25
#define SCSI_OPCODE_READ_FORMAT_CAPACITIES          0x23
#define SCSI_OPCODE_READ_6                          0x08
#define SCSI_OPCODE_READ_10                         0x28
#define SCSI_OPCODE_READ_12                         0xA8
#define SCSI_OPCODE_READ_16                         0x88
#define SCSI_OPCODE_WRITE_6                         0x0A
#define SCSI_OPCODE_WRITE_10                        0x2A
#define SCSI_OPCODE_WRITE_12                        0xAA
#define SCSI_OPCODE_WRITE_16                        0x8A
#define SCSI_OPCODE_REQUEST_SENSE                   0x03
#define SCSI_OPCODE_TEST_UNIT_READY                 0x00
#define SCSI_OPCODE_START_STOP_UNIT                 0x1B
#define SCSI_OPCODE_FORMAT_UNIT                     0x04
#define SCSI_OPCODE_RESERVE_10                      0x56
#define SCSI_OPCODE_RELEASE_10                      0x57
#define SCSI_OPCODE_SEND_DIAGNOSTIC                 0x1D
#define SCSI_OPCODE_MODE_SENSE_6                    0x1A
#define SCSI_OPCODE_MODE_SENSE_10                   0x5A
#define SCSI_OPCODE_PREVENT_ALLOW_MEDIUM_REMOVAL    0x1E
#define SCSI_OPCODE_START_STOP_UNIT                 0x1B // Start Stop Unit from the SBC-2 spec
#define SCSI_OPCODE_READ_BUFFER                     0x3C

#define SCSI_CDB_SIZE_INQUIRY                       6
#define SCSI_CDB_SIZE_READ_CAPACITY                 10
#define SCSI_CDB_SIZE_READ_6                        6
#define SCSI_CDB_SIZE_READ_10                       10
#define SCSI_CDB_SIZE_READ_12                       12
#define SCSI_CDB_SIZE_READ_16                       16
#define SCSI_CDB_SIZE_WRITE_6                       6
#define SCSI_CDB_SIZE_WRITE_10                      10
#define SCSI_CDB_SIZE_WRITE_12                      12
#define SCSI_CDB_SIZE_WRITE_16                      16
#define SCSI_CDB_SIZE_REQUEST_SENSE                 6
#define SCSI_CDB_SIZE_TEST_UNIT_READY               6
#define SCSI_CDB_SIZE_START_STOP_UNIT               6
#define SCSI_CDB_SIZE_FORMAT_UNIT                   6
#define SCSI_CDB_SIZE_RESERVE_10                    10
#define SCSI_CDB_SIZE_RELEASE_10                    10
#define SCSI_CDB_SIZE_SEND_DIAGNOSTIC               6
#define SCSI_CDB_SIZE_MODE_SENSE_6                  6
#define SCSI_CDB_SIZE_MODE_SENSE_10                 10
#define SCSI_CDB_SIZE_PREVENT_ALLOW_MEDIUM_REMOVAL  6
#define SCSI_CDB_SIZE_START_STOP_UNIT               6

////////////////////////////////////////////////////////////////////////////////
// Unsupported SCSI commands known to have been received.  Unknown SCSI commands
//  not in this list are only trapped in development builds.  Release builds
//  simply fail all commands not in this list.  Use the
//  SCSI_TRAP_UNKNOWN_OPCODES build option in the make file to trap.
#define SCSI_OPCODE_READ_FORMAT_CAPACITIES          0x23 // Read Format Capacities from the
                                                         //  SFF-8070i spec
#define SCSI_OPCODE_VERIFY_10                       0x2F // Verify(10) from the SBC-2 spec

////////////////////////////////////////////////////////////////////////////////
// Data direction defs
#define SCSI_DATA_TO_HOST               0
#define SCSI_DATA_FROM_HOST             1

////////////////////////////////////////////////////////////////////////////////
// Sense Data defs
#define SENSE_DATA_RESPONSE_CODE_CURRENT_ERRORS     0x70
#define SENSE_DATA_RESPONSE_CODE_DEFERRED_ERRORS    0x71
#define SENSE_DATA_INFORMATION_VALID                1<<7
#define SENSE_DATA_INFORMATION_INVALID              0
#define SENSE_DATA_ILI_CORRECT                      0<<4
#define SENSE_DATA_ILI_INCORRECT                    1<<4

////////////////////////////////////////////////////////////////////////////////
// Sense Key defs
#define SCSI_SENSE_KEY_NO_SENSE             0<<16
#define SCSI_SENSE_KEY_RECOVERED_ERROR      1<<16
#define SCSI_SENSE_KEY_NOT_READY            2<<16
#define SCSI_SENSE_KEY_MEDIUM_ERROR         3<<16
#define SCSI_SENSE_KEY_HARDWARE_ERROR       4<<16
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST      5<<16
#define SCSI_SENSE_KEY_UNIT_ATTENTION       6<<16
#define SCSI_SENSE_KEY_DATA_PROTECT         7<<16
#define SCSI_SENSE_KEY_BLANK_CHECK          8<<16
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC      9<<16
#define SCSI_SENSE_KEY_COPY_ABORTED         10<<16
#define SCSI_SENSE_KEY_ABORTED_COMMAND      11<<16
#define SCSI_SENSE_KEY_OBSOLETE             12<<16
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW      13<<16
#define SCSI_SENSE_KEY_MISCOMPARE           14<<16
#define SCSI_SENSE_KEY_RESERVED             15<<16

////////////////////////////////////////////////////////////////////////////////
// Additional Sense Code & Qualifier defs.  Asc is the LSB.
#define SCSI_ASC_NO_ADDITIONAL_SENSE_INFO                               0x0000

#define SCSI_ASC_LOGICAL_UNIT_NOT_READY_MANUAL_INTERVENTION_REQUIRED    0x0304

#define SCSI_ASC_LOGICAL_UNIT_DOES_NOT_RESPOND_TO_SELECTION             0x0005

#define SCSI_ASC_INVALID_COMMAND_OPERATION_CODE                         0x0020

#define SCSI_ASC_LOGICAL_UNIT_NOT_SUPPORTED                             0x0025

#define SCSI_ASC_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE                     0x0021 

#define SCSI_ASC_WRITE_PROTECTED                                        0x0027

#define SCSI_ASC_NOT_READY_TO_READY_CHANGE_MEDIUM_MAY_HAVE_CHANGED      0x0028

#define SCSI_ASC_DEVICE_INTERNAL_RESET                                  0x0429

#define SCSI_ASC_INCOMPATIBLE_MEDIUM_INSTALLED                          0x0030
#define SCSI_ASC_CANNOT_READ_MEDIUM_UNKNOWN_FORMAT                      0x0130
#define SCSI_ASC_CANNOT_READ_MEDIUM_INCOMPATIBLE_FORMAT                 0x0230
#define SCSI_ASC_CANNOT_WRITE_MEDIUM_UNKNOWN_FORMAT                     0x0430
#define SCSI_ASC_CANNOT_WRITE_MEDIUM_INCOMPATIBLE_FORMAT                0x0530

#define SCSI_ASC_MEDIUM_FORMAT_CORRUPTED                                0x0031

#define SCSI_ASC_UNRECOVERED_READ_ERROR                                 0x0011

#define SCSI_ASC_MEDIUM_NOT_PRESENT                                     0x003A
#define SCSI_ASC_MEDIUM_DESTINATION_ELEMENT_FULL                        0x003B

#define SCSI_ASC_INQUIRY_DATA_HAS_CHANGED                               0x033F
#define SCSI_ASC_REPORTED_LUNS_DATA_HAS_CHANGED                         0x0E3F

#define SCSI_ASC_LOGICIAL_UNIT_FAILED_SELF_CONFIGURATION                0x004C

#define SCSI_ASC_INVALID_FIELD_IN_CDB                                   0x0024

#define SCSI_ASC_LOGICAL_UNIT_FAILURE                                   0x013E
#define SCSI_ASC_LOGICAL_UNIT_NOT_READY_INIT_CMD_REQUIRED               0x0204

////////////////////////////////////////////////////////////////////////////////
// Inquiry Peripheral Qualifier defs
#define SCSI_INQUIRY_PERIPHERAL_QUALIFIER_LUN_CONNECTED         0
#define SCSI_INQUIRY_PERIPHERAL_QUALIFIER_LUN_NOT_CONNECTED     1
#define SCSI_INQUIRY_PERIPHERAL_QUALIFIER_RESERVED              2
#define SCSI_INQUIRY_PERIPHERAL_QUALIFIER_NO_SUPPORT            3

////////////////////////////////////////////////////////////////////////////////
// Inquiry Peripheral Qualifier defs
#define SCSI_INQUIRY_PERIPHERAL_DEVICE_TYPE_DIRECT_ACCESS       0
#define SCSI_INQUIRY_PERIPHERAL_DEVICE_TYPE_NO_SUPPORT          0x1F

////////////////////////////////////////////////////////////////////////////////
// Inquiry RMB defs
#define SCSI_INQUIRY_RMB_DEVICE_IS_NOT_REMOVABLE                0
#define SCSI_INQUIRY_RMB_DEVICE_IS_REMOVABLE                    1

////////////////////////////////////////////////////////////////////////////////
// Inquiry Vendor Specific SigmaTel struct defs
#define SCSI_INQUIRY_VENDOR_SPECIFIC_STRUCT_MDS                 1

#define SCSI_MDS_FLAGS_SYSTEM_MEDIA_FLAG_NON_SYSTEM             (WORD)0x00
#define SCSI_MDS_FLAGS_SYSTEM_MEDIA_FLAG_SYSTEM                 (WORD)0x01
                                                                
#define SCSI_MDS_MEDIA_TYPE_NAND                                (WORD)0x00
#define SCSI_MDS_MEDIA_TYPE_SMARTMEDIA                          (WORD)0x01
#define SCSI_MDS_MEDIA_TYPE_MMC_1_4                             (WORD)0x10
#define SCSI_MDS_MEDIA_TYPE_MMC_2_1_1                           (WORD)0x11
#define SCSI_MDS_MEDIA_TYPE_MMC_3_1                             (WORD)0x12
#define SCSI_MDS_MEDIA_TYPE_MMC_UNKNOWN_VERSION                 (WORD)0x13
#define SCSI_MDS_MEDIA_TYPE_SD_1_0                              (WORD)0x20
#define SCSI_MDS_MEDIA_TYPE_SD_UNKNOWN_VERSION                  (WORD)0x21

////////////////////////////////////////////////////////////////////////////////
// Error codes
////////////////////////////////////////////////////////////////////////////////

#define SCSI_PROJECT_SPECIFIC_INIT_FAILED                       (WORD)0x000001
#define SCSI_DEVICE_INIT_ALL_FAILED                             (WORD)0x000002
#define SCSI_ERROR_LUN_NOT_READY                                (WORD)0x000003
#define SCSI_ERROR_INVALID_COMMAND                              (WORD)0x000004
#define SCSI_ERROR_USB_MSC_RESET_DURING_SCSI_COMMAND            (WORD)0x000005
#define SCSI_ERROR_READ_SECTOR_FAILURE                          (WORD)0x000006
#define SCSI_ERROR_WRITE_SECTOR_FAILURE                         (WORD)0x000007
#define SCSI_ERROR_WAIT_FOR_USB_TIMEOUT                         (WORD)0x000008
#define SCSI_READ_FAILURE_TIMEOUT_USB_WILL_NOT_STOP             (WORD)0x000009
#define SCSI_WRITE_FAILURE_TIMEOUT_USB_WILL_NOT_STOP            (WORD)0x00000A
#define SCSI_ERROR_USB_DISCONNECT                               (WORD)0x00000B
#define SCSI_ERROR_DDI_CALL_FAILED                              (WORD)0x00000C
#define SCSI_ERROR_LBA_OUT_OF_RANGE                         (RETCODE)(0x000100)
#define SCSI_WRITE_FAILURE_ZONE_FULL                        (RETCODE)(0x9AAAAA)

////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////

typedef RETCODE (*SCSI_CH)(WORD wDeviceNum, WORD _X* pCdb);
typedef RETCODE (*SCSI_PROJECT_INIT)(void);
typedef RETCODE (*SCSI_LUN_FLAG_MEDIA_CHANGE_FUNCTION)(void);

typedef struct
{
    WORD wValid;
    WORD wSenseKey;
    WORD wIli;
    DWORD dwInformation;
    WORD wAscqAsc; 
    WORD wAddlData;
} SCSI_SENSE_DATA;

typedef enum
{
    ScsiMediaStateUnknown = 0,
    ScsiMediaStateReady = 1,
    ScsiMediaStateFormatRequired = 2,
    ScsiMediaStateHardwareFailed = 3
} SCSI_MEDIA_STATE;

typedef struct
{
    DWORD dwNumBytesXferred;
    WORD wCompletionStatus;
    SCSI_SENSE_DATA SenseData;
    BOOL bStopped;
    BOOL bWriteProtected;
    BOOL bMediaConnected;
    BOOL bMediaIsRemovable;
    WORD wRemovableType;
    WORD wRemovableEntry;
    WORD wRemovableMask;
    BOOL bMediaEnumerated;
    BOOL bMediaChanged;
    BOOL bMediaInsertionDetected;
    BOOL bMediaValid;
    SCSI_MEDIA_STATE eMediaState;
    WORD wFileSystemDriveNumber;
    WORD wFileSystemMediaNumber;
    BOOL bStartUnitAttention;
    MEDIA_SIZE MediaSize;
    WORD wInquiryVendorData[10];
    WORD wInquiryVendorSpecific[7];
    SCSI_LUN_FLAG_MEDIA_CHANGE_FUNCTION FlagMediaChange;
    DWORD dwSequentialBoundaryMask;
    DWORD dwSequentialBoundary;
} SCSI_LUN;


typedef struct SCSICommandEntryStruct
{

    WORD     wCommand;
    SCSI_CH  pFunc;
} SCSI_COMMAND_ENTRY;


////////////////////////////////////////////////////////////////////////////////
// Prototypes
////////////////////////////////////////////////////////////////////////////////

RETCODE _reentrant ScsiDeviceTaskRouter(WORD wDeviceNum, WORD _X * pCdb);
RETCODE _reentrant ScsiDeviceTaskManager(WORD wDeviceNum, WORD wOpCode, WORD _X * pCdb, SCSI_CH pFunc);
RETCODE _reentrant ScsiDeviceServer(WORD wDeviceNum, WORD _X* pCdb,    SCSI_CH pFunc);

SCSI_CH _reentrant ScsiValidOperationCode(WORD wOpCode);
SCSI_CH _reentrant SearchForSCSICommand(SCSI_COMMAND_ENTRY *pList, WORD wOpCode);

RETCODE _reentrant ScsiInit(void);
RETCODE _reentrant ScsiDeviceInitializeAll(void);
RETCODE _reentrant ScsiDeviceInitialize(WORD wNum);
RETCODE _reentrant ScsiLunInitialize(WORD wDevNum, WORD wLun);
RETCODE ScsiLunSetupSize(WORD wDevNum, WORD wLun);
RETCODE ScsiLunSetupInquiryVendorSpecificData(WORD wDevNum, WORD wLun);
RETCODE _reentrant ScsiLunSetupDefaultSenseData(SCSI_LUN * pLun);
RETCODE _reentrant ScsiDummyFlagMediaChange(void);

#define SCSI_IDLE      0       // Not reading or writing
#define SCSI_READING   1       // A read operation is in progress
#define SCSI_WRITING   2       // A write operation is in progress

extern WORD g_wSCSIActivityState;

RETCODE _reentrant SCSIWriteSectorHelper(WORD wDevice, WORD wDrive, DWORD dwStartSector, WORD wSectorCount, BOOL bUseMultiwrite);
RETCODE _reentrant SCSIReadSectorHelper(WORD wDevice, WORD wDrive, DWORD dwStartSector, WORD wSectorCount);

SCSI_CH _reentrant GetVendorSpecificSCSIHandler(WORD wOpCode);
SCSI_CH _reentrant GetCustomerExtentionSCSIHandler(WORD wOpCode);

#endif // #ifndef _SCSI_H
