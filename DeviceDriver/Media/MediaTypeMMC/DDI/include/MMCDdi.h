////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003
//
// Filename: MMCDdi.h
// Description: 
////////////////////////////////////////////////////////////////////////////////

#ifndef _MMCDDI_H
#define _MMCDDI_H

///////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////

#include "ddildl_internal.h"
#include "mmchal.h"

///////////////////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////////////////

#define MMC_SECTOR_SIZE_IN_BYTES            512

typedef enum {
  eMmcLockStateFree   = 0,
  eMmcLockStateLocked = 1
} MMC_LOCK_STATE;

typedef BOOL    (*P_MMCBUSYDONE_CALLBACK_FUNCTION)(int, void*);

/////////////////////////////////////////////////////////////////////////////////
// MMC Physical Device Struct
/////////////////////////////////////////////////////////////////////////////////
typedef struct {
    BOOL  bMMCCardPresent;
  	BOOL	bEnumerated;
    BOOL	bWriteProtected;
    BOOL	bEraseTested;
    BOOL	bEraseIsFunctional;
//    BOOL	bMmcMultiWriteSupported;
//    BOOL	bMmcMultiWriteInProgress;
} MMC_PHYSICAL_DEVICE_FLAGS, * P_MMC_PHYSICAL_DEVICE_FLAGS;


/////////////////////////////////////////////////////////////////////////////////
// MMC CID Struct
/////////////////////////////////////////////////////////////////////////////////
typedef struct {

  	WORD 	wMID;	    //Manufacturer ID
    WORD 	wCIN[4];	// Spec 1.4 only
    WORD 	wOID;	    // OEM/App ID
    DWORD 	dwPNM;	// Product Name
    WORD 	wPRV;	    // Product Revision
    DWORD 	dwPSN;	// Product Serial Number
    WORD 	wMDT;	    // Manufacturing Date
    WORD 	wCRC;	    // CRC checksum
} MMC_CID, *P_MMC_CID;

/////////////////////////////////////////////////////////////////////////////////
// MMC CSD Struct
/////////////////////////////////////////////////////////////////////////////////
typedef struct {

    WORD 	wCSD_STRUCTURE;		// *
    WORD 	wSPEC_VERS;		    // * System Specification Version
    WORD 	wTAAC;			      // Data Read Access Time 1
    WORD 	wNSAC;			      // Data Read Access Time 2
    WORD 	wTRANS_SPEED;		  // Max Data Transfer Rate
    WORD 	wCCC;			        // Card Command Classes
    WORD 	wREAD_BL_LEN;		  // * Max Read Data block Length
    WORD 	wREAD_BL_PARTIAL;		// Partial Blocks for read allowed
    WORD 	wWRITE_BLK_MISALIGN;	// Write Block misaligned
    WORD 	wREAD_BLK_MISALIGN;	// Read Block misaligned
    WORD 	wDSR_INP;		      // DSR Implemented
    WORD 	wC_SIZE;			    // * Device Size
    WORD 	wVDD_R_CURR_MIN;	// Max Read Current at Vdd Min
    WORD 	wVDD_R_CURR_MAX;	// Max Read Current at Vdd Max
    WORD 	wVDD_W_CURR_MIN;  // Max Write Current at Vdd Min
    WORD 	wVDD_W_CURR_MAX;	// Max Write Current at Vdd Max
    WORD 	wC_SIZE_MULT;		  // * Device Size Mulitplier
    WORD 	wERASE_BLK_EN;		// Erase Block Enable (SD 1.0)
    WORD 	wSECTOR_SIZE;		  // Sector Size (SD 1.0)
    WORD 	wERASE_GRP_SIZE;	// * Erase Group Size
    WORD 	wERASE_GRP_MULT;	// * Erase Group Multiplier
    WORD 	wWP_GRP_SIZE;		  // Write Protect Group Size
    WORD 	wWP_GROUP_ENABLE;	// Write Protect Group Enable
    WORD 	wDEFAULT_ECC;		  // Manufacturer Default ECC
    WORD 	wR2W_FACTOR;		  // Write Speed Factor
    WORD 	wWRITE_BL_LEN;	  // Max Write data block length
    WORD 	wWRITE_BL_PARTIAL;	// Partial blocks for  Write allowed
    WORD 	wFILE_FORMAT_GRP;	// File format group
    WORD 	wCOPY;			      // Copy Flag (OTP)
    WORD 	wPERM_WRITE_PROTECT;	// Permanent Write Protection
    WORD 	wTMP_WRITE_PROTECT;	// Temporary Write Protection
    WORD 	wFILE_FORMAT;		  // File Format
    WORD 	wECC;			        // ECC
    WORD 	wCRC;			        // CRC
                            //      *  =  Spec 3.1

} MMC_CSD, *P_MMC_CSD;

typedef struct _MMCMultiWriteStruct
{
  BOOL  bMultiWriteSupported;
  BOOL  bMultiWriteStarted;
  BOOL  bMultiWriteInProgress;
  DWORD dwMultiWriteStartSectorNumber;
  DWORD dwMultiWriteCurrentSectorNumber;
  WORD  wMultiWriteSectorsRemaining;
  DWORD dwMMCPreviousWriteSector;
  MULTI_WRITE_CALLBACK_FUNCTION pCallBackFunction;
} MMCMultiWriteStruct;

/////////////////////////////////////////////////////////////////////////////////
// MMC Media Info Struct
/////////////////////////////////////////////////////////////////////////////////
typedef struct {
  MMC_LOCK_STATE  Lock;
  BOOL            bMMCIsBusy;       // Used for SetMMCBusy and MMCIsReady
  P_MMCBUSYDONE_CALLBACK_FUNCTION pMMCBusyDoneCallbackFunction;
  void *          pMMCBusyArg;              // Used for SetMMCBusy and MMCIsReady
  DWORD           dwTotalSectors;           //
  MMC_PHYSICAL_DEVICE_FLAGS	Flags;	// operation flags
  MMC_CARD_TYPE	  Type;		// MMC or SD?
  MMC_CID		      Cid;		// Card ID structure
  MMC_CSD		      Csd;		// Card Specific Data Register
  WORD		        wEraseSize;	// Erase Size
  DWORD		        dwDeviceSize;
  MMC_PHYSICAL_BUS * pMMCPhysicalBus;     //SPI Bus Pin definitions for Media.
  MMCMultiWriteStruct MultWrite;
} MMC_MEDIA_INFO, * P_MMC_MEDIA_INFO;


///////////////////////////////////////////////////////////////////////////////
// Error codes
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Typedefs
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// Global variables
///////////////////////////////////////////////////////////////////////////////
extern LOGICAL_MEDIA_API MMCMediaApi;
extern LOGICAL_DRIVE_API MMCDataDriveApi;
extern MMC_MEDIA_INFO MEDIA_DDI_MEM MmcMediaInfo;

#endif // #ifndef _MMCDDI_H
