///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
//
// Filename: nandstructs.h
// Description: Structure definitions for NAND descriptors
///////////////////////////////////////////////////////////////////////////////

#ifndef NAND_STRUCTS_H
#define NAND_STRUCTS_H 1

// By design:



///////////////////////////////////////////////////////////////////////////////
// Sector/Page Descriptor
struct NANDSectorDescriptorStruct {
  WORD  iSectorTotalSize;		// Total Sector size (Data + Redundant Area)
  WORD	iSectorDataSize;	    // Number Data bytes per Page
  WORD	iRedundantAreaSize;		// Number of bytes per Redundant/Spare Area

  // A physical Page may contain multiple Sectors.  
  // It is assumed that the number or Sectors per Page is always 
  // a power of 2 (i.e. 1,2,4...)
  // The following elements are used to help address the Sectors within a Page
  WORD	iSectorToPageShift;		// Number of right shifts to compute Page Num from Sector Num
  WORD  iSectorInPageMask;		// Mask to extract "relative sector in Page" from absolute Sector Num
};


///////////////////////////////////////////////////////////////////////////////
// Block Descriptor
struct NANDBlockDescriptorStruct {
  WORD	NumSectorsPerBlock;
  WORD	PagesPerBlock;
  WORD	MaxBlocksPerZone;
};





///////////////////////////////////////////////////////////////////////////////
// RedundantArea Descriptor
struct NANDRedundantAreaDescriptorStruct {
                                // Describes locations of fields in the Redundant Area *when it
                                // is read by itself* (i.e. not as part of a FullSector read).

  WORD	BadBlockFlag_Offset;	// 24-bit word offset to Bad Block Flag
  WORD  BadBlockFlag_ByteNum;	// byte number (0-2) of Bad Block Flag
  WORD  Tag0_Offset;			// 24-bit word offset to Tag0
  WORD  Tag0_ByteNum;			// byte number of Tag0
  WORD  Tag1_Offset;			// 24-bit word offset to Tag1
  WORD  Tag1_ByteNum;			// byte number of Tag1
};

#define NAND_GOOD_BLOCK_VALUE	(0x0000ff)

///////////////////////////////////////////////////////////////////////////////
// Device Addressing Descriptor
struct NANDDeviceAddressingDescriptorStruct {

  WORD	iNumColumnBytes;	// Number of Column Address bytes required
  WORD	iNumRowBytes;		// Number of Row Address bytes required
};


///////////////////////////////////////////////////////////////////////////////
// NAND Command Codes Descriptor
struct NANDCommandCodesStruct {

  // NOTE: Command are a single byte, in the LSByte.  A value of -1 (0xffffff) indicates
  //        a code is not available for the current device.

  WORD iReadIDCode;			        // ReadID
  WORD iReadStatusCode;	            // Read Status
  WORD iResetCode;                  // Reset

  WORD iSerialDataInputCode;		// Serial Data Input
  WORD iRead1Code;			        // Read (Mode 1)
  WORD iRead1_2ndCycleCode;         // Second Cycle for Read (Type 2 NANDs)
  WORD iRead2Code;			        // Read (Mode 2)
  WORD iRead3Code;			        // Read (Mode 3)

#ifdef NAND_WRITES_ALLOWED
  WORD iPageProgramCode;	        // Page Program
  WORD iCacheProgramCode;           // Cache Program

  // TBD: Add ReadForCopy Back 0x00 & 0x35
  WORD iCopyBackProgramCode;        // Copy Back Program
  // TBD: Add Copy Back Program 2nd Cycle 0x10

  WORD iBlockEraseCode;             // Block Erase
  WORD iBlockErase2Code;            // Block Erase 2

  WORD iRandomDataInCode;           // Random Data Input

  WORD iRandomDataOutCode;          // Random Data Output
  WORD iRandomDataOut_2ndCycleCode; // Random Data Output 2nd Cycle

  WORD iMultiPlaneBlockEraseCode;	// Multiplane Block Erase

  WORD iReadMultiPlaneStatusCode;   // Read Multiplane Status
#endif
};



///////////////////////////////////////////////////////////////////////////////
// Device Driver API Functions
struct NANDDeviceDriverFunctionsStruct {

  _reentrant WORD (*pReadSectorData)(struct NANDDescriptorStruct _P *pNANDDevice, WORD iChip,
                                     WORD iSectorNum, WORD *pDataBuf, BYTE bType);
//  _reentrant WORD (*pReadSectorRedundant)(struct NANDDescriptorStruct _P *pNANDDevice, WORD iChip, 
//                                          WORD iSectorNum, WORD *pDataBuf);
  _reentrant WORD (*pResetNand)(WORD iDeviceNum);

#ifdef NAND_WRITES_ALLOWED
  // WRITE Functions

  _reentrant WORD (*pWriteSectorData)(struct NANDDescriptorStruct _P *pNANDDevice, 
                                      WORD iChip, 
                                      WORD iSectorNum, WORD *pDataBuf);
  _reentrant WORD (*pEraseBlock)(struct NANDDescriptorStruct _P *pNANDDevice, 
                                 WORD iDeviceNum, WORD iBlockNum);
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Hardware API Functions
struct NANDHardwareAPIFunctionsStruct {
  _reentrant WORD (*pPortInit)(WORD iDeviceNum);
  _reentrant WORD (*pWritePortCmd)(WORD iDeviceNum, WORD iCommandCode, WORD bDeassertCEn);
  _reentrant WORD (*pWritePortAddr)(WORD iDeviceNum, WORD iNumColBytes, 
                         WORD iColumnAddress, WORD iNumRowBytes, 
                         WORD iRowAddress, WORD bDeassertCEn);
  _reentrant WORD (*pReadPortData)(WORD iDeviceNum, WORD iNumBytesToRead, 
                                   WORD *pBuffer, WORD b16BitMode);
  _reentrant WORD (*pWaitForReadyPort)(WORD iDeviceNum, WORD iTimeoutCount);
  _reentrant WORD (*pPowerUpPads)(void);
  _reentrant WORD (*pPowerUpPads_16bit)(void);
  _reentrant WORD (*pTerminatePort)(WORD iDeviceNum);
};


#ifdef NAND_WRITES_ALLOWED
struct NANDHardwareAPI_w_FunctionsStruct {
  // WRITE Functions

  _reentrant void (*pEnableWritesPort)(WORD WPnum);
  _reentrant void (*pDisableWritesPort)(WORD WPnum);
  _reentrant WORD (*pWriteDataPort)(WORD iDeviceNum, WORD iNumBytesToWrite, 
                                            WORD *pBuffer, WORD b16BitMode);
};
#endif




///////////////////////////////////////////////////////////////////////////////
// This SubStruct is a consolidation for memory savings. 
typedef struct NANDDescriptorSubStruct{
    struct NANDSectorDescriptorStruct _P *pPageDescriptor;
    struct NANDBlockDescriptorStruct _P *pBlockDescriptor;
//    struct NANDRedundantAreaDescriptorStruct _P *pRedundantAreaDescriptor;
    struct NANDDeviceDriverFunctionsStruct _P *pDeviceDriverFunctions;
    struct NANDCommandCodesStruct _P *pDeviceCommandCodes;
    struct NANDDeviceAddressingDescriptorStruct _P *pDeviceAddressing;    
    WORD  NandType;       // = 1 (SSFDC), 2 (e.g. Samsung 1Gbit), or 3 (e.g. MLC)
};

// The NANDDescriptorStruct is whole NAND descriptor - includes pointers to some of the above structs
typedef struct NANDDescriptorStruct {
  struct NANDDescriptorSubStruct _P *pNandDescriptorSubStruct; // consolidated
  WORD	DataBusWidth;	// BusWidth = 8 or 16 bits
						// Use this parameter only to initialize the global CurrentNANDBusWidth
						// Some NANDs required more real time process to determine  their bus
						// width. (see CurrentNANDBusWidth declaration Note for further information)
//WORD  TotalSectors;   // Unused in bootmanager
//WORD  TotalPages;     // Unused in bootmanager
  WORD  TotalBlocks;
//WORD  TotalZones;     // Unused in bootmanager
} NAND_DESCRIPTOR, *P_NAND_DESCRIPTOR;



typedef struct {
  WORD    DeviceManufacturerCode;                 // 00:Device:Manufacturer
  struct NANDDescriptorStruct _P *pNANDDescriptor;
} NAND_DEVICECODE_MAP;

#define TYPE1_NAND  1
#define TYPE2_NAND  2
#define TYPE3_NAND  3
#define TYPE4_NAND  4
#define TYPE5_NAND  5
#define TYPE6_NAND  6
#define TYPE7_NAND  7


#endif
