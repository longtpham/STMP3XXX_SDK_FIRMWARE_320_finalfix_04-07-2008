////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: mmcsal.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _MMCSAL_H
#define _MMCSAL_H

///////////////////////////////////////////////////////////////////////////////
// MMC SAL definitions
///////////////////////////////////////////////////////////////////////////////

// TODO:  these need to be global file system defs
#define DEVICE_INSTALLED                    (WORD)(0)
#define DEVICE_NOT_FOUND                    (WORD)(2)
#define DEVICE_INVALID                      (WORD)(-1)
#define DEVICE_ERROR_WRITE_PROTECTED        (WORD)(-2)

#define MMC_SEND_OP_TIMEOUT                 MMC_PORT_TIMEOUT

#define CSD_PARAM                           WORD
#define CSD_CSD_STRUCTURE                   (WORD)(0x000000)
#define CSD_SPEC_VERS                       (WORD)(0x000001)
#define CSD_TAAC                            (WORD)(0x000002)
#define CSD_NSAC                            (WORD)(0x000003)
#define CSD_TRAN_SPEED                      (WORD)(0x000004)
#define CSD_CCC                             (WORD)(0x000005)
#define CSD_READ_BL_LEN                     (WORD)(0x000006)
#define CSD_READ_BL_PARTIAL                 (WORD)(0x000007)
#define CSD_WRITE_BLK_MISALIGN              (WORD)(0x000008)
#define CSD_READ_BLK_MISALIGN               (WORD)(0x000009)
#define CSD_DSR_IMP                         (WORD)(0x00000A)
#define CSD_C_SIZE                          (WORD)(0x00000B)
#define CSD_VDD_R_CURR_MIN                  (WORD)(0x00000C)
#define CSD_VDD_R_CURR_MAX                  (WORD)(0x00000D)
#define CSD_VDD_W_CURR_MIN                  (WORD)(0x00000E)
#define CSD_VDD_W_CURR_MAX                  (WORD)(0x00000F)
#define CSD_C_SIZE_MULT                     (WORD)(0x000010)
#define CSD_ERASE_BLK_EN                    (WORD)(0x000011)
#define CSD_SECTOR_SIZE                     (WORD)(0x000012)
#define CSD_ERASE_GRP_SIZE                  (WORD)(0x000013)
#define CSD_ERASE_GRP_MULT                  (WORD)(0x000014)
#define CSD_WP_GRP_SIZE                     (WORD)(0x000015)
#define CSD_WP_GRP_ENABLE                   (WORD)(0x000016)
#define CSD_DEFAULT_ECC                     (WORD)(0x000017)
#define CSD_R2W_FACTOR                      (WORD)(0x000018)
#define CSD_WRITE_BL_LEN                    (WORD)(0x000019)
#define CSD_WRITE_BL_PARTIAL                (WORD)(0x00001A)
#define CSD_FILE_FORMAT_GRP                 (WORD)(0x00001B)
#define CSD_COPY                            (WORD)(0x00001C)
#define CSD_PERM_WRITE_PROTECT              (WORD)(0x00001D)
#define CSD_TMP_WRITE_PROTECT               (WORD)(0x00001E)
#define CSD_FILE_FORMAT                     (WORD)(0x00001F)
#define CSD_ECC                             (WORD)(0x000020)
#define CSD_CRC                             (WORD)(0x000021)

///////////////////////////////////////////////////////////////////////////////
// MMC SAL Error codes
///////////////////////////////////////////////////////////////////////////////

#define ERR_MMC_INVALID_SECTOR              (WORD)(0x000100)
#define ERR_MMC_READ_SECTOR_FAIL            (WORD)(0x000101)
#define ERR_MMC_WRITE_SECTOR_FAIL           (WORD)(0x000102)
#define ERR_MMC_HARDWARE_WRITE_PROTECTION   (WORD)(0x000103)
#define ERR_MMC_INVALID_WRITE_TYPE          (WORD)(0x000104)
#define ERR_MMC_INVALID_READ_TYPE           (WORD)(0x000105)

#define ERR_MMC_PHYSICAL_DEVICE_ENUMERATED  (WORD)(0x000200)
#define ERR_MMC_RESET_FAILED                (WORD)(0x000201)
#define ERR_MMC_INIT_FAILED                 (WORD)(0x000202)
#define ERR_MMC_READ_OCR_FAILED             (WORD)(0x000203)
#define ERR_MMC_READ_CSD_FAILED             (WORD)(0x000204)
#define ERR_MMC_READ_CID_FAILED             (WORD)(0x000205)
#define ERR_MMC_INVALID_CID                 (WORD)(0x000206)
#define ERR_MMC_NOT_MMC_CARD_TYPE           (WORD)(0x000207)
#define ERR_MMC_SPEC_VERSION_NOT_SUPPORTED  (WORD)(0x000208)
#define ERR_MMC_UNSUPPORTED_OCR_VOLTAGES    (WORD)(0x000209)
#define ERR_MMC_APP_COMMAND_FAILED          (WORD)(0x00020A)
#define ERR_MMC_PHYSICAL_DEVICE_NOT_INSERTED (WORD)(0x00020B)
#define ERR_MMC_PHYSICAL_DEVICE_NOT_ENUMERATED (WORD)(0x00020B)

#define ERR_MMC_INVALID_CSD_VERSION         (WORD)(0x000300)
#define ERR_MMC_INVALID_SECTOR_SIZE         (WORD)(0x000301)
#define ERR_MMC_PARITION_SECTOR_READ_FAIL   (WORD)(0x000302)
#define ERR_MMC_NO_PARTITION_TABLE_FOUND    (WORD)(0x000303)
#define ERR_MMC_LOGICAL_DEVICE_NOT_FORMATTED (WORD)(0x000304)
#define ERR_MMC_LOGICAL_DEVICE_NOT_ENUMERATED (WORD)(0x000305)

#define ERR_MMC_SEND_OP_TIMEOUT             (WORD)(0x000400)

#define ERR_MMC_INVALID_CSD_PARAMETER       (WORD)(0x000500)

///////////////////////////////////////////////////////////////////////////////
// MMC SAL structure definitions
///////////////////////////////////////////////////////////////////////////////

typedef struct {
    BOOL    bFormatted;
    BOOL    bGrabbed;
} MMC_LOGICAL_DEVICE_FLAGS;

typedef struct {
    MMC_LOGICAL_DEVICE_FLAGS Flags;
    WORD wPhysicalDevice;
    WORD wPartition;
} MMC_LOGICAL_DEVICE;

typedef struct {
    WORD wStatus;
    WORD wStartHead;
    WORD wStartSectorCylinder;
    WORD wSystemId;
    WORD wEndHead;
    WORD wEndSectorCylinder;
    DWORD dwRelativeSector;
    DWORD dwTotalSectors;
} PARTITION_TABLE_ENTRY;

typedef struct {
    WORD wNumPartitionsFound;
    WORD wLastPartitionAssigned;
    PARTITION_TABLE_ENTRY Partition[MMC_MAX_PARTITIONS];
} PARTITION_TABLE;

typedef DWORD MMC_OCR;

typedef struct {
    WORD wMID;
    WORD wCIN[4];                       // Spec 1.4 only
    WORD wOID;
    DWORD dwPNM;
    WORD wPRV;
    DWORD dwPSN;
    WORD wMDT;
    WORD wCRC;
} MMC_CID;
// for the following struct, params marked with * are initialized to date
typedef struct {
    WORD wCSD_STRUCTURE;            // *
    WORD wSPEC_VERS;                // *
    WORD wTAAC;
    WORD wNSAC;
    WORD wTRAN_SPEED;
    WORD wCCC;
    WORD wREAD_BL_LEN;              // *
    WORD wREAD_BL_PARTIAL;
    WORD wWRITE_BLK_MISALIGN;
    WORD wREAD_BLK_MISALIGN;
    WORD wDSR_IMP;
    WORD wC_SIZE;                   // *
    WORD wVDD_R_CURR_MIN;
    WORD wVDD_R_CURR_MAX;
    WORD wVDD_W_CURR_MIN;
    WORD wVDD_W_CURR_MAX;
    WORD wC_SIZE_MULT;              // *
    WORD wERASE_BLK_EN;             // SD 1.0
    WORD wSECTOR_SIZE;              // SD 1.0
    WORD wERASE_GRP_SIZE;           // *
    WORD wERASE_GRP_MULT;           // * MMC 3.1
    WORD wWP_GRP_SIZE;
    WORD wWP_GRP_ENABLE;
    WORD wDEFAULT_ECC;
    WORD wR2W_FACTOR;
    WORD wWRITE_BL_LEN;
    WORD wWRITE_BL_PARTIAL;
    WORD wFILE_FORMAT_GRP;
    WORD wCOPY;
    WORD wPERM_WRITE_PROTECT;
    WORD wTMP_WRITE_PROTECT;
    WORD wFILE_FORMAT;
    WORD wECC;
    WORD wCRC;
} MMC_CSD;

typedef struct {
    BOOL    bEnumerated;
    BOOL    bGrabbed;
    BOOL    bWriteProtected;
    BOOL    bEraseTested;
    BOOL    bEraseIsFunctional;
    BOOL    bMmcMultiWriteSupported;
    BOOL    bMmcMultiWriteInProgress;
} MMC_PHYSICAL_DEVICE_FLAGS;

typedef struct {
    MMC_PHYSICAL_DEVICE_FLAGS Flags;
    MMC_CARD_TYPE Type;
    MMC_CID Cid;
    MMC_CSD Csd;
    WORD wEraseSize;
    DWORD dwDeviceSize;
    DWORD dwMmcPreviousWriteSector;
} MMC_PHYSICAL_DEVICE;

    
///////////////////////////////////////////////////////////////////////////////
// MMC prototypes
///////////////////////////////////////////////////////////////////////////////

// functions that map directly to MMC commands
RETCODE _reentrant MDDInit( WORD wDeviceNumber );
RETCODE _reentrant MDDCleanUp(WORD wDeviceNumber);
RETCODE _reentrant MDDReadSector( WORD wDeviceNumber, WORD wSectorNumber, WORD _X *pwBuffer, WORD wReadType );
RETCODE _reentrant MDDWriteSector( WORD wDeviceNumber, WORD wSectorNumber, WORD _X *pwBuffer, WORD wWriteType );
RETCODE _reentrant MDDWriteSectorNull( WORD wDeviceNumber, WORD wSectorNumber, WORD _X *pwBuffer, WORD wWriteType );
DWORD _reentrant MDDGetSize(WORD wDeviceNumber, WORD wParameter);
RETCODE _reentrant MDDGetSerialNumber(WORD wDeviceNumber, WORD * pwBuffer);
BOOL _reentrant MmcIsHardwareWriteProtected(WORD wNum);
BOOL _reentrant MmcIsWriteProtected(WORD wNum);
BOOL _reentrant MmcMultiBlockWriteIsSupported(WORD wNum);
WORD _reentrant MmcGetCsdParameter(WORD wDeviceNumber, CSD_PARAM Param);
DWORD _reentrant MmcCalcNumPhysicalSectors(WORD wDeviceNumber);
MMC_CARD_TYPE _reentrant MmcGetMediaType(WORD wDeviceNumber);
BOOL _reentrant MmcIsSecureDigital(WORD wDeviceNumber);
WORD _reentrant MmcGetEraseSize(WORD wDeviceNumber);
RETCODE _reentrant MmcFlagMediaChange(void);

extern BOOL bMmcBusInitialized;
extern BOOL bMmcMultiWriteInProgress;
extern DWORD dwMmcPreviousWriteSector;

#endif // #ifndef _MMCSAL_H
