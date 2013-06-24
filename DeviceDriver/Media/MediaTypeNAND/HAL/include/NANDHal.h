////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename:     NANDHal.h
// Description:  Contains structs, prototypes, equates for the NAND Hal routines.
////////////////////////////////////////////////////////////////////////////////

#ifndef _NANDHAL_H
#define _NANDHAL_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "errors.h"
#include "sectordef.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////
// Can disable ECC interrupt for testing
#if 0 
#define ECC_POLLING
#endif

#define RSECC_DELAY_COUNTER_VALUE 1000

//-------------------------------
// these byte values are also seen in device ID in nandtables.h and nanddesc.c for bootmgr.
#define STMakerID    0x20
#define HynixMakerID 0xAD

#define SAMSUNG_MAKER_ID 0xEC
#define MICRON_MAKER_ID   0x2c

#define ToshibaMakerID        0x98
#define RENESAS_MAKER_ID  0x07
//#define SandiskMakerID 0x000045
//-------------------------------
// place the tables in X space.
#define NAND_TABLES_MEMSPACE _X

#define NANDHAL_MEM_X _X
#define NANDHAL_MEM_Y _Y
#define NANDHAL_MEM NANDHAL_MEM_Y    // ***** IF THIS CHANGES, YOU MUST UPDATE NANDHal.inc to MATCH!!!!

#define LEAVE_CE_PIN_ASSERTED 0  // formerly FALSE
#define DEASSERT_CE_PIN       1  // formerly TRUE

// These are for readfunctions -- to ease readability
#define wColumnByteZero 0
#define wColumnAddrZero 0
#define wRowByteZero    0
#define wRowByteOne     1
#define wRowAddrZero    0
#define wReadZeroBytes  0
#define wReadOneByte    1
#define wReadTwoBytes   2
#define wReadThreeBytes 3
#define wReadSixBytes   6
#define wByteOffsetZero 0

// Timeouts -- TBD -- These either get replaced altogether, or go somewhere else.
// WaitForREADY() timeout counts
// These control how long we will wait for the NAND to finish some internal operation
// (e.g. Read Page (to cache), Program Page (from cache), Erase Block, Reset).
// NOTE: For timeouts, each "count" is on the order of 50 nsec.
// (<10 instructions, at a few 10s of nanosecs each)
// The settings below are probably very conservative, but still short in human time
// (e.g. 2000000 counts is approx. 100 msec)
#define NAND_READ_PAGE_TIMEOUT          (2000000)
#define NAND_READ_REDUNDANT_TIMEOUT     (2000000)
#define NAND_RESET_TIMEOUT              (2000000)
#define NAND_WRITE_PAGE_TIMEOUT         (2000000)
#define NAND_ERASE_BLOCK_TIMEOUT        (2000000)

// Read Status Register Masks
// commented out with definition of new API
// #define NAND_SR_PASS_MASK           0x000001
// #define NAND_SR_CACHEPASS_MASK      0x000002
// #define NAND_SR_READY_MASK          0x000060
// //#define NAND_SR_READY_MASK          0x000040
// #define NAND_SR_WRITEPROTECT_MASK   0x000080

// new definitions for standardized API
#define NAND_STATUS_PASS_MASK                   0x000001
#define NAND_STATUS_DISTRICT_0_PASS_MASK        0x000002
#define NAND_STATUS_DISTRICT_1_PASS_MASK        0x000004
#define NAND_STATUS_DISTRICT_2_PASS_MASK        0x000008
#define NAND_STATUS_DISTRICT_3_PASS_MASK        0x000010
#define NAND_STATUS_TRUE_READY_MASK             0x000020
#define NAND_STATUS_CACHE_READY_MASK            0x000040
#define NAND_STATUS_WRITE_PROTECT_MASK          0x000080
#define NAND_STATUS_CACHE_PREVIOUS_PASS_MASK    0x000100
// The next 3 are used for Renesas part.
#define NAND_STATUS_PROGRAM_CHECK_PASS_MASK     0x000008
#define NAND_STATUS_ERASE_CHECK_PASS_MASK       0x000010
#define NAND_STATUS_ECC_CORRECTABLE_MASK        0x000020
#define NAND_STATUS_INVALID_MASK                0x800000

#define NAND_STATUS_PASS_VALUE                  0x000001
#define NAND_STATUS_DISTRICT_0_PASS_VALUE       0x000002
#define NAND_STATUS_DISTRICT_1_PASS_VALUE       0x000004
#define NAND_STATUS_DISTRICT_2_PASS_VALUE       0x000008
#define NAND_STATUS_DISTRICT_3_PASS_VALUE       0x000010
#define NAND_STATUS_TRUE_READY_VALUE            0x000020
#define NAND_STATUS_CACHE_READY_VALUE           0x000040
#define NAND_STATUS_WRITE_PROTECT_VALUE         0x000080
#define NAND_STATUS_CACHE_PREVIOUS_PASS_VALUE   0x000100

#define NAND_STATUS_PROGRAM_CHECK_PASS_VALUE    0x000008
#define NAND_STATUS_ERASE_CHECK_PASS_VALUE      0x000010
#define NAND_STATUS_ECC_CORRECTABLE             0x000020

#define WP_NAND_INTERNAL    0   // write protect line for internal
#define WP_EXTERNAL         1   // write protect line for external

#define MakerIDMask 0x0000ff
#define INVALID_SECTOR_ADDRESS     (0xFFFFF0) // FFFFF0 is a dummy value intended to be an invalid sector address

/////////////////////////////////////////////////////////////////////////////////
//  Macros
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////
// NAND-Specific Physical Media structures
//////////////////////////////////////////
typedef enum {
  // NOTE: NandHalInit checks for zero to see if initialization needed.
  // Therefore, do not use a ZERO in the typedef.
  eNandTypeUnknown = 0, // use for init
  eNandType1 = 1,
  eNandType2 = 2,
  eNandType4 = 4,
  eNandType5 = 5,
  eNandType6 = 6,
  eNandType7 = 7
} NAND_TYPE;

typedef enum {
  eNandLockStateFree   = 0,
  eNandLockStateLocked = 1
} NAND_LOCK_STATE;

typedef enum {
    eNandProgCmdReadID                    = 0x000090,
    eNandProgCmdReadID2                   = 0x000091,
    eNandProgCmdReadStatus                = 0x000070,
    eNandProgCmdReset                     = 0x0000ff,
    eNandProgCmdSerialDataInput           = 0x000080,   // Page Program/Cache Program
    eNandProgCmdRead1                     = 0x000000,   // Read or Read for CopyBack
    eNandProgCmdRead1_2ndCycle            = 0x000030,   // Second Cycle for Read (Type 2 NANDs)
    eNandProgCmdReadForCopyBack_2ndCycle  = 0x000035,   // Second Cycle for Read for Copy Back
    eNandProgCmdReadForCacheCopyback_2nd  = 0x00003A,
    eNandProgCmdRead2                     = 0x000001,
    eNandProgCmdRead3                     = 0x000050,
    eNandProgCmdPageProgram               = 0x000010,   // Second cycle for wSerialDataInput for Page Program
    eNandProgCmdCacheProgram              = 0x000015,   // Second cycle for wSerialDataInput for Cache Program
    eNandProgCmdCopyBackProgram           = 0x000085,
    eNandProgCmdCopyBack2Program          = 0x00008C,
    eNandProgCmdCopyBackProgram_2ndCycle  = 0x000010,   // Second cycle for Copy Back Program
    eNandProgCmdBlockErase                = 0x000060,
    eNandProgCmdBlockErase_2ndCycle       = 0x0000d0,
    eNandProgCmdRandomDataIn              = 0x000085,
    eNandProgCmdRandomDataOut             = 0x000005,
    eNandProgCmdRandomDataOut_2ndCycle    = 0x0000E0,
    eNandProgCmdReadMultiPlaneStatus      = 0x000071,   // MLC MultiPlane
    eNandProgCmdReadErrorStatus           = 0x000072,   // MLC Single Plane Error Status
    eNandProgCmdReadMultiPlaneErrorStatus = 0x000073,   // MLC MultiPlane Error Status.
    eNandProgCmdMultiPlaneWrite           = 0x000011,
    eNandProgCmdStatusModeReset           = 0x00007F,
    eNandProgCmdMultiPlaneRead_2ndCycle   = 0x000031,
    eNandProgCmdPageDataOutput            = 0x000006,
    eNandProgCmdMultiPlaneVerifyErased    = 0x0000D3,
    eNandProgCmdMultiPlaneBlockErase      = 0x00ffff,   // TBD !!! Need code for this.
    eNandProgCmdNone                      = 0x7FFFFF,   // invalid entry
    eNandProgCmdDummyProgram              = 0x6FFFFF    // invalid entry
} NAND_PROGRAM_CODES;



// NAND Physical Media API functions
typedef RETCODE (*P_HAL_READ_SECTOR_FUNCTION)(struct NandPhysicalMediaTag *, WORD, P_SECTOR_BUFFER);
typedef RETCODE (*P_HAL_READ_SECTOR_REDUNDANT_FUNCTION)(struct NandPhysicalMediaTag *, WORD, P_SECTOR_BUFFER, BOOL);
typedef RETCODE (*P_HAL_ENABLE_WRITES_FUNCTION)(WORD);
typedef RETCODE (*P_HAL_DISABLE_WRITES_FUNCTION)(WORD);
typedef RETCODE (*P_HAL_WRITE_SECTOR_FUNCTION)(struct NandPhysicalMediaTag *, WORD, P_SECTOR_BUFFER);
typedef RETCODE (*P_HAL_WRITE_SECTOR_REDUNDANT_FUNCTION)(struct NandPhysicalMediaTag *, WORD, P_SECTOR_BUFFER);
typedef RETCODE (*P_HAL_ERASE_BLOCK_FUNCTION)(struct NandPhysicalMediaTag *, WORD);
typedef RETCODE (*P_HAL_ERASE_BLOCK_ASYNC_FUNCTION)(struct NandPhysicalMediaTag *, WORD);
typedef RETCODE (*P_HAL_COPY_SECTORS_FUNCTION)(struct NandPhysicalMediaTag *, WORD, WORD, WORD);
typedef RETCODE (*P_HAL_GET_STATUS_FUNCTION)(struct NandPhysicalMediaTag *, WORD *, WORD);
typedef RETCODE (*P_HAL_SETUP_WRITE_SECTOR_FUNCTION)(struct NandPhysicalMediaTag *, WORD);
typedef RETCODE (*P_HAL_SETUP_WRITE_SECTOR_REDUNDANT_FUNCTION)(struct NandPhysicalMediaTag *, WORD);
typedef RETCODE (*P_HAL_SEND_SECTOR_DATA_FUNCTION)(struct NandPhysicalMediaTag *, P_SECTOR_BUFFER);
typedef RETCODE (*P_HAL_SEND_PROGRAM_COMMAND_FUNCTION)(struct NandPhysicalMediaTag *, NAND_PROGRAM_CODES);
typedef RETCODE (*P_HAL_GET_DMA_STATUS_FUNCTION)(struct NandPhysicalMediaTag *);
typedef RETCODE (*P_HAL_LOCK_INTERFACE_PORT_FUNCTION)(void);
typedef RETCODE (*P_HAL_RELEASE_INTERFACE_PORT_FUNCTION)(void);
typedef RETCODE (*P_HAL_LOCK_NAND_FUNCTION)(struct NandPhysicalMediaTag *);
typedef RETCODE (*P_HAL_RELEASE_NAND_FUNCTION)(struct NandPhysicalMediaTag *);
typedef RETCODE (*P_HAL_START_COMPUTE_ECC_FUNCTION)(P_SECTOR_BUFFER,WORD);
typedef RETCODE (*P_HAL_COMPUTE_ECC_GET_STATUS_FUNCTION)(void);
typedef RETCODE (*P_HAL_CORRECT_ECC_FUNCTION)(P_SECTOR_BUFFER);
typedef BOOL    (*P_BUSYDONE_CALLBACK_FUNCTION)(int, void*);
typedef RETCODE (*P_HAL_MSW_SECTOR_COMMIT)(struct NandPhysicalMediaTag *);
typedef RETCODE (*P_HAL_MSW_PAGE_COMMIT)(struct NandPhysicalMediaTag *);


typedef struct {
    WORD wReadSectorCodeBank;
    P_HAL_READ_SECTOR_FUNCTION pReadSector;
    WORD wReadSectorRedundantCodeBank;
    P_HAL_READ_SECTOR_REDUNDANT_FUNCTION pReadSectorRedundant;
    WORD wGetStatusCodeBank;
    P_HAL_GET_STATUS_FUNCTION pGetStatus;
    WORD wEnableWritesCodeBank;
    P_HAL_ENABLE_WRITES_FUNCTION pEnableWrites;
    WORD wDisableWritesCodeBank;
    P_HAL_DISABLE_WRITES_FUNCTION pDisableWrites;
    WORD wWriteSectorCodeBank;
    P_HAL_WRITE_SECTOR_FUNCTION pWriteSector;
    WORD wWriteSectorRedundantCodeBank;
    P_HAL_WRITE_SECTOR_REDUNDANT_FUNCTION pWriteSectorRedundant;
    WORD wEraseBlockCodeBank;
    P_HAL_ERASE_BLOCK_FUNCTION pEraseBlock;
    WORD wEraseBlockAsyncCodeBank;
    P_HAL_ERASE_BLOCK_ASYNC_FUNCTION pEraseBlockAsync;
    WORD wCopySectorsCodeBank;
    P_HAL_COPY_SECTORS_FUNCTION pCopySectors;
    WORD wSetupWriteSectorCodeBank;
    P_HAL_SETUP_WRITE_SECTOR_FUNCTION pSetupWriteSector;
    WORD wSetupWriteSectorRedundantCodeBank;
    P_HAL_SETUP_WRITE_SECTOR_REDUNDANT_FUNCTION pSetupWriteSectorRedundant;
    WORD wSendSectorDataCodeBank;
    P_HAL_SEND_SECTOR_DATA_FUNCTION pSendSectorData;
    WORD wSendProgramCommandCodeBank;
    P_HAL_SEND_PROGRAM_COMMAND_FUNCTION pSendProgramCommand;
    WORD wStartComputeECCCodeBank;
    P_HAL_START_COMPUTE_ECC_FUNCTION pStartComputeECC;
    WORD wComputeECCGetStatusCodeBank;
    P_HAL_COMPUTE_ECC_GET_STATUS_FUNCTION pComputeECCGetStatus;
    WORD wCorrectECCCodeBank;
    P_HAL_CORRECT_ECC_FUNCTION pCorrectECC;
    WORD wSectorCommitBank;
    P_HAL_MSW_SECTOR_COMMIT pCommitSector;
    WORD wPageCommitBank;
    P_HAL_MSW_PAGE_COMMIT pCommitPage;

} NANDHAL_API_DESCRIPTOR, * P_NANDHAL_API_DESCRIPTOR;

typedef struct {
    P_NANDHAL_API_DESCRIPTOR pNandHalApi;
    NAND_TYPE                NandType;

    // SECTOR PARAMETERS
    WORD wSectorTotalSize;              // Total sector size (Data + Redundant Area)
    WORD wSectorDataSize;               // Number of Data Bytes per Page
    WORD wRedundantAreaSize;            // Number of Bytes per Redundant/Spare Area
    // A physical Page may contain multiple Sectors.
    // It is assumed that the number or Sectors per Page is always
    // a power of 2 (i.e. 1,2,4...)
    // The following elements are used to help address the Sectors within a Page
    WORD wSectorToPageShift;            // Number of right shifts to compute Page Num from Sector Num
    WORD wSectorInPageMask;             // Mask to extract "relative sector in Page" from absolute Sector Num

    // BLOCK PARAMETERS
    WORD wSectorsToBlockShift;
    WORD wSectorsPerBlockMask;
    WORD wNumberSectorsPerBlock;
    WORD wPagesPerBlock;

    // DEVICE ADDRESSING PARAMETERS
    WORD wNumColumnBytes;
    WORD wNumRowBytes;
    BOOL bSendAddressForEachSector;
} NAND_PARAMETERS_DESCRIPTOR, * P_NAND_PARAMETERS_DESCRIPTOR;

typedef struct NandPhysicalMediaTag{
    P_NAND_PARAMETERS_DESCRIPTOR pNANDParams;
    WORD                         wChipNumber;
    NAND_LOCK_STATE              Lock;
    BOOL                         bNANDIsBusy;              // Used for SetNANDBusy and NANDIsReady
    P_BUSYDONE_CALLBACK_FUNCTION pBusyDoneCallbackFunction;// Used for SetNANDBusy and NANDIsReady
    void                      *  pBusyArg;                 // Used for SetNANDBusy and NANDIsReady
    WORD                         wTotalSectors;
    WORD                         wTotalBlocks;
    WORD                         wTotalInternalDice;       // (1/2/4/...) - number of chips pretending to be a single chip
    WORD                         wBlocksPerDie;            // (wTotalBlocks / wTotalInternalDice )
} NAND_PHYSICAL_MEDIA_DESCRIPTOR, * P_NAND_PHYSICAL_MEDIA_DESCRIPTOR;


// These are physical parameters that can be overruled in NAND_INIT_DESCRIPTOR
// by analyzing the data read during read IDs command.
typedef struct {
    WORD                     wTotalInternalDice;       // (1/2/4/...) - number of chips pretending to be a single chip
    WORD                     wBlocksPerDie;            // (wTotalBlocks / wTotalInternalDice)

} NAND_OVER_RULED_PARAMTERS;

// These are used by the TypeNDescriptor files -- use for copying.
typedef struct {
    WORD wSectorTotalSize;       // Total sector size (Data + Redundant Area)
    // A physical Page may contain multiple Sectors.
    // It is assumed that the number or Sectors per Page is always
    // a power of 2 (i.e. 1,2,4...)
    // The following elements are used to help address the Sectors within a Page
    BOOL bSendAddressForEachSector; // FALSE if sectors in Page can be DMA'd without new setup
} NAND_SECTOR_DESCRIPTOR, * P_NAND_SECTOR_DESCRIPTOR;

typedef struct {
    WORD wNumberSectorsPerBlock;
    WORD wPagesPerBlock;
} NAND_BLOCK_DESCRIPTOR, * P_NAND_BLOCK_DESCRIPTOR;

typedef struct {
    WORD wNumColumnBytes;
    WORD wNumRowBytes;
} NAND_DEVICE_ADDR_DESCRIPTOR, * P_NAND_DEVICE_ADDR_DESCRIPTOR;

typedef struct {
    // at init -- these are copied to NAND_PARAMETERS_DESCRIPTOR
    NAND_SECTOR_DESCRIPTOR NAND_TABLES_MEMSPACE *         pSectorDescriptor;
    NAND_BLOCK_DESCRIPTOR  NAND_TABLES_MEMSPACE *         pBlockDescriptor;
    NAND_DEVICE_ADDR_DESCRIPTOR NAND_TABLES_MEMSPACE *    pDeviceAddressingDescriptor;
    P_NANDHAL_API_DESCRIPTOR pNandHalApi;
    NAND_TYPE                NandType;
} NAND_DESCRIPTOR_SUB_STRUCT, * P_NAND_DESCRIPTOR_SUB_STRUCT;

typedef struct {
    // at init -- these are copied to NAND_PARAMETERS_DESCRIPTOR
    NAND_DESCRIPTOR_SUB_STRUCT NAND_TABLES_MEMSPACE * pNandDescriptorSubStruct;
    // at init -- these are copied to NAND_PHYSICAL_MEDIA_DESCRIPTOR
    WORD                     wTotalBlocks;
    WORD                     wTotalInternalDice;       // (1/2/4/...) - number of chips pretending to be a single chip
} NAND_INIT_DESCRIPTOR, * P_NAND_INIT_DESCRIPTOR;

typedef union {	    		    // All fields in nanoseconds

    // By placing this word before the bitfield it allows structure copies to be done
    //  safely by assignment rather than by memcpy.

    WORD initializer;

    // These field lengths are arbitrary... they only need to be large enough to hold
    //  the values.  Possible optimization by using enumerations rather than the actual
    //  values here and teaching NANDHalSetGPFlashTiming how to interpret them.

    struct {
        WORD TDH    :7;				// tWH, tREH, tDH
        WORD TDS    :7;				// tWP, tDS
        WORD THOLD  :5;				// tCLH, tCH, tALH, tDH
        WORD TSU    :5;				// tCLS, tCS, tALS
    } decode;
} NAND_TIMINGS;

// Special types to aid in absolute vs relative block address confusion  
typedef WORD REGION_RELATIVE_BLK_ADDR;  
typedef WORD ABSOLUTE_BLK_ADDR;  

#define MK_NAND_TIMINGS_REAL( tsu, thold, tds, tdh )             ( ((tsu   & 0x1F) << 19)   \
                                                                 | ((thold & 0x1F) << 14)   \
                                                                 | ((tds   & 0x7F) << 7)    \
                                                                 |  (tdh   & 0x7F) )

//!! The max value for an unsigned 7 bit number is 2^7 - 1 = 127 so don't use any value >127 for tdh & tds (macro param 3&4), else overflow & crash occurs. 
//!! The max value for an unsigned 5 bit number is 2^5 - 1 = 31  so don't use any value >31  for           (macro param 1&2), else overflow & crash can occur. 

#if DEBUG
    //  NAND1 and NAND2 get optimized timings.  NAND3 and NAND4 get failsafe timings.
    #ifdef NAND1
        #define NAND_FAILSAFE_TIMINGS 			MK_NAND_TIMINGS_REAL( 0, 10, 45, 30 )
        #define MK_NAND_TIMINGS(a,b,c,d)        MK_NAND_TIMINGS_REAL(a,b,c,d)
    #else
        #ifdef NAND2
            #define NAND_FAILSAFE_TIMINGS 			MK_NAND_TIMINGS_REAL( 0, 10, 61, 48 )
            #define MK_NAND_TIMINGS(a,b,c,d)        MK_NAND_TIMINGS_REAL(a,b,c,d)
        #else
            #ifdef NAND3
                #define NAND_FAILSAFE_TIMINGS 			MK_NAND_TIMINGS_REAL( 0, 10, 82, 48 )
                #define MK_NAND_TIMINGS(a,b,c,d)        NAND_FAILSAFE_TIMINGS
            #else
                #ifdef NAND4
                    #define NAND_FAILSAFE_TIMINGS 			MK_NAND_TIMINGS_REAL( 0, 10, 82, 48 )
                    #define MK_NAND_TIMINGS(a,b,c,d)        NAND_FAILSAFE_TIMINGS
                #else
                    // DEBUG defined but not (NAND1 NAND2 NAND3 or NAND4)
                    #define NAND_FAILSAFE_TIMINGS 			MK_NAND_TIMINGS_REAL( 0, 10, 45, 30 )
                    #define MK_NAND_TIMINGS(a,b,c,d)        MK_NAND_TIMINGS_REAL(a,b,c,d)
                #endif
            #endif
        #endif
    #endif
#else
    // Retail builds get full speed timings
    // non DEBUG (RETAIL) builds (non engr board layouts) get one best case timing set.
    // You may be able to reduce these values per your flash datasheet to increase performance.
    #define MK_NAND_TIMINGS(a,b,c,d)        MK_NAND_TIMINGS_REAL((a),(b),(c),(d))
    #define NAND_FAILSAFE_TIMINGS 			MK_NAND_TIMINGS_REAL( 0, 10, 45, 30 )
#endif

typedef struct {
  WORD                                        DeviceManufacturerCode;   // 00:Device:Manufacturer
  NAND_TIMINGS                                NandTimings;
  NAND_INIT_DESCRIPTOR NAND_TABLES_MEMSPACE * pNANDInitDescriptor;
} NAND_DEVICECODE_MAP, * P_NAND_DEVICECODE_MAP;

typedef union
{
   SECTOR_BUFFER singleLargeSector[SECTOR_BUFFER_ALLOC_SIZE(LARGE_SECTOR_TOTAL_SIZE)];
   SECTOR_BUFFER singleSmallSector[SECTOR_BUFFER_ALLOC_SIZE(SSFDC_SECTOR_TOTAL_SIZE)];
} _X NAND_COPY_BUFFER;

typedef struct
{
    WORD SectorDataSizeInBytes;
    WORD SectorDataSizeInWords;
    WORD SectorRedundantSizeInBytes;
    WORD SectorRedundantSizeInWords;
    WORD SectorTotalSizeInBytes;
    WORD SectorTotalSizeInWords;
} NAND_OUT_OF_CONTEXT_DATA;


typedef enum
{
    WRITE_MODE_NONE,
    WRITE_MODE_SINGLE,
    WRITE_MODE_MULTI
} NAND_WRITE_MODE;

typedef struct _id_decode
{
    // Read ID Byte 1
    WORD MakerCode              : 8;
    // Read ID Byte 2
    WORD DeviceCode             : 8;
    // Read ID Byte 3
    WORD InternalChipNumber     : 2;        // Number of die = (1 << n)
    WORD CellType               : 2;        // Number of bits per memory cell = ( 1 << (n+1) ) // may 2005: 2 bits per cell for today's mlc.
    WORD VendorSpecific0        : 3;
    WORD CacheProgram           : 1;        // 0 == Not supported
    // Read ID Byte 4
    WORD PageSize               : 2;        // Page size in bytes = (1 << n) * 1024
    WORD RedundantAreaSize      : 1;        // Redundant area bytes per 512 data bytes = 8 * (1 << n)
    WORD Reserved0              : 1;
    WORD BlockSize              : 2;        // Block size in bytes = 64 * 1024 * (1 << n)
    WORD Organization           : 1;        // 0 == x8, 1 == x16
    WORD SamsungHSSerialAccess  : 1;        // 0 == 50/30ns, 1 == 25ns
    // Read ID Byte 5
    WORD VendorSpecific1        : 2;
    WORD PlaneNumber            : 2;        // # of planes total (see note below) = (1 << n)
    WORD PlaneSize              : 3;        // # of bytes per plane = 64 * 1024 * 1024 * (1 << n)
    WORD Reserved4              : 1;
    // Read ID Byte 6
    WORD Reserved5              : 3;
    WORD ToshibaHighSpeedMode   : 1;        // 0 == Not supported
    WORD Reserved6              : 4;
}t_id_decode;

// Note on # of planes for all Large Page Devices (SLC and MLC)
//  This field will contain the number of planes per die * the number of die on a per chip enable basis.
//    Examples;  1 plane, 1 die; value = 1
//      2 plane, 1 die; value = 2
//      2 plane, 2 die; value = 4
//      2 plane, 2 die, 2 chip enable; value = 2

//  In case of multi-stacked device, the "plane number" will show the total number of planes of the packaged
//  device and not the number of planes per die. (e.g. If 2 pieces of device that has 2 planes were stacked
//  in a package, the plane number will show "4")

///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////
RETCODE _reentrant Type1_GetStatus(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD * pStatus, WORD Mask);
RETCODE _reentrant Type1_CopySectors(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                                     WORD wSourceStartSectorNum, WORD wTargetStartSectorNum, WORD wNumSectors);
RETCODE _reentrant Type1_CommitSector(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant Type1_CommitPage(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);


RETCODE _reentrant Type2_CopySectors(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                                     WORD wSourceStartSectorNum, WORD wTargetStartSectorNum, WORD wNumSectors);
RETCODE _reentrant Type2_GetStatus(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD * pStatus, WORD Mask);
RETCODE _reentrant Type2_CommitSector(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant Type2_CommitPage(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);


RETCODE _reentrant Type4_GetStatus(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD * pStatus, WORD Mask);
RETCODE _reentrant Type4_EraseBlockAsync(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,  WORD wBlockNum);
RETCODE _reentrant Type4_CopySectors(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                                     WORD wSourceStartSectorNum, WORD wTargetStartSectorNum, WORD wNumSectors);
RETCODE _reentrant Type4_CommitSector(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);


RETCODE _reentrant Type5_CopySectors(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                                     WORD wSourceStartSectorNum, WORD wTargetStartSectorNum, WORD wNumSectors);


void _reentrant TypeX_FlushReadCache(short);
void _reentrant TypeX_ReadForCopyback( P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD wSourceStartSectorNum, WORD readCommand );
void _reentrant TypeX_CopybackProgram( P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD wTargetStartSectorNum, WORD firstProgCmd, WORD secondProgCmd, P_SECTOR_BUFFER pBuf );
RETCODE _reentrant TypeX_SetupWriteSector(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,  WORD wSectorNum);
RETCODE _reentrant TypeX_SetupWriteSectorRedundant(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,  WORD wSectorNum);
WORD _reentrant TypeX_ComputeCol(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD wByteOffset);
RETCODE _reentrant TypeX_ReadSectorData(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                        WORD iSectorNum, P_SECTOR_BUFFER pBuf);
RETCODE _reentrant TypeX_ReadSectorRedundant(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                         WORD iSectorNum, P_SECTOR_BUFFER pSectorBuf, BOOL crcCorrect);
RETCODE _reentrant TypeX_WaitForProgramDone(
                  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant TypeX_SendSectorData(
                  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                  P_SECTOR_BUFFER pBuf);
RETCODE _reentrant TypeX_SendSectorRedundantArea(
                  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                  P_SECTOR_BUFFER pBuf);
RETCODE _reentrant TypeX_SendProgramCommand(
                  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                  NAND_PROGRAM_CODES eCommandCode);
RETCODE _reentrant TypeX_WriteSector(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                                     WORD wSectorNum, P_SECTOR_BUFFER pBuf);
RETCODE _reentrant TypeX_WriteSectorRedundant(
                  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                  WORD wSectorNum, P_SECTOR_BUFFER pBuf);
RETCODE _reentrant TypeX_EraseBlock(
                  P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD wBlockNum);
RETCODE _reentrant TypeX_EraseBlockAsync(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                                         WORD wBlockNum);

RETCODE _reentrant NANDHALCommon_EnableWrites(WORD wWPn);
RETCODE _reentrant NANDHALCommon_DisableWrites(WORD wWPn);

RETCODE _reentrant NANDHalStartComputeECC_1Bit(P_SECTOR_BUFFER pCompleteSectorBuf, WORD wWhichHalf);
RETCODE _reentrant NANDHalStartComputeECC_4Bit(P_SECTOR_BUFFER pCompleteSectorBuf, WORD wWhichHalf);
RETCODE _reentrant NANDHalCorrectECC_1Bit(P_SECTOR_BUFFER pCompleteSectorBuf);
RETCODE _reentrant NANDHalCorrectECC_4Bit(P_SECTOR_BUFFER pCompleteSectorBuf);

// These functions are all direct-callable. Do not need to go through struct de-referencing.
RETCODE _reentrant NANDHalLockInterfacePort(void);
RETCODE _reentrant NANDHalReleaseInterfacePort(void);
RETCODE _reentrant NANDHalLockNand(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant NANDHalReleaseNand(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant NANDHalComputeECCGetStatus(void);
WORD _reentrant CRC8(WORD _X * wData, WORD wOffset, WORD wBcount);
void _reentrant NandReset(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant NandHalInit(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, NAND_TIMINGS * timings );
RETCODE _reentrant NANDHalSetNandBusy(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
                                      BOOL (* pCallbackFun)(int iStatus, void * pObj), void * pArg);
BOOL _reentrant bNANDHalNandIsReady(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant NANDHalWaitForAsync(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor);
RETCODE _reentrant HALMarkBlockBadInRA(
    P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor,
    ABSOLUTE_BLK_ADDR wBlockAddr);
RETCODE _reentrant NANDHilInit(WORD wDeviceNum);
RETCODE _reentrant NANDHalWaitForTrueReady(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD * pStatus);
RETCODE _reentrant NANDHalWaitForCacheReady(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD * pStatus);
void _reentrant NANDHalDblChkStatus(P_NAND_PHYSICAL_MEDIA_DESCRIPTOR pNANDDescriptor, WORD wMask, WORD *pwStatus);
RETCODE _reentrant NANDHalSetGPFlashTiming( NAND_TIMINGS const * pNT );
///////////////////////////////////////////////////////////////////////////////
// Externs
///////////////////////////////////////////////////////////////////////////////
extern NANDHAL_MEM NANDHAL_API_DESCRIPTOR Type1_NANDHALApiDescriptor;
extern NANDHAL_MEM NANDHAL_API_DESCRIPTOR Type2_NANDHALApiDescriptor;
extern NANDHAL_MEM NANDHAL_API_DESCRIPTOR Type4_NANDHALApiDescriptor;
extern NANDHAL_MEM NANDHAL_API_DESCRIPTOR Type5_NANDHALApiDescriptor;
extern NANDHAL_MEM WORD g_iCachedPageNumInNAND[];
#define INVALID_SECTOR_ADDRESS     (0xFFFFF0) // FFFFF0 is a dummy value intended to be an invalid sector address

extern NAND_COPY_BUFFER wCopyBuffer;

extern NAND_OUT_OF_CONTEXT_DATA g_NandAuxData;
//extern NAND_WRITE_MODE g_WriteMode;  // can enable this line later. sdk 2.610 has it. 
extern WORD g_maker_code;
extern WORD g_CellType;
extern WORD g_VendorSpecific1;
extern WORD g_device_code;

extern t_id_decode SECTOR_BUFFER_MEM * pReadIDDecode;
#if 1 // TOVERIFY2: 
///////////////////////////////////////////////////////////////////////////////
// Macros
///////////////////////////////////////////////////////////////////////////////
//On NAND4 builds, if less than 4 Nands are used, then the ID structure gets overwritten with garbage
//( zeros?),whenever the empty slots are probed. But we need to preserve the Maker code
//for Hynix and ST Micro Nands.
#define _nand_hal_preserve_data();\
{                                          \
  g_maker_code = pReadIDDecode->MakerCode; \
  g_CellType = pReadIDDecode->CellType; \
  g_device_code = pReadIDDecode->DeviceCode;\
  g_VendorSpecific1 = pReadIDDecode->VendorSpecific1; \
} // cell type backed up to glob so it's not lost during 2nd call to DetermineNandType in a multi CE present case. We call that > once only when >1 CE present.
#else // old ver
extern WORD NANDHAL_MEM wID1;
//On NAND4 builds, if less than 4 Nands are used, then the ID structure gets overwritten with garbage 
//( zeros?),whenever the empty slots are probed. But we need to preserve the Maker code and cell type
//for Hynix and ST Micro and other Nands. 
#define _nand_hal_preserve_data();\
{                                          \
  g_maker_code = (wID1 & MakerIDMask); \
  //g_CellType = pReadIDDecode->CellType; \  /* should save this and uncomment this line too if you ever revert to this old way */
  //g_VendorSpecific1 = pReadIDDecode->VendorSpecific1; \  /* should save this and uncomment this line too if you ever revert to this old way */
}
#endif

#endif // #ifndef _NANDHAL_H


