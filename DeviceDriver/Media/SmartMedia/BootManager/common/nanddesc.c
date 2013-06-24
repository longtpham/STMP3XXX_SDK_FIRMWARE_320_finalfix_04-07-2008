/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2006
//
// Filename: nanddesc.c
// Description: C Support functions for NAND flash Device Driver
/////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "smequ.h"

#include "nandstructs.h"
#include "nanddevdrvapi.h"

#ifdef NAND_WRITES_ALLOWED
#include "nanddevdrvapi_w.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Exports
_reentrant struct NANDDescriptorStruct _P * matchDevice(WORD iDeviceCode);




///////////////////////////////////////////////////////////////////////////////
//  Sector Descriptors
_P struct NANDSectorDescriptorStruct				Sm_SectorDescriptor = {
  528,				// iSectorSize
  512,				// iDataPageSize
  16,				// iRedundantAreaSize
  0,                // iSectorToPageShift
  0xffffff          // iSectorInPageMask
};

#ifdef INCLUDE_TYPE2_NAND
_P struct NANDSectorDescriptorStruct				Type2_SectorDescriptor = {
  2112,				// iSectorSize
  2048,                // iDataPageSize
  64,				// iRedundantAreaSize
  0,                // iSectorToPageShift
  0x000000          // iSectorInPageMask
};
#endif

///////////////////////////////////////////////////////////////////////////////
// Block Descriptors
_P struct NANDBlockDescriptorStruct 			Sm_Small_BlockDescriptor = {
  16,				// iNumSectorsPerBlock
  16				// iPagesPerBlock
};
_P struct NANDBlockDescriptorStruct 			Sm_Large_BlockDescriptor = {
  32,				// iNumSectorsPerBlock
  32				// iPagesPerBlock
};

#ifdef INCLUDE_TYPE2_NAND
_P struct NANDBlockDescriptorStruct 			Type2_BlockDescriptor = {
  64,				// iNumSectorsPerBlock
  64				// iPagesPerBlock
};
#endif

#ifdef INCLUDE_RENESAS_DESCRIPTORS
// Note that we're fooling the code here.  We're combining 4 actual blocks
// into 1 Virtual block.
_P struct NANDBlockDescriptorStruct 			Type4_BlockDescriptor = {
  32,				// iNumSectorsPerBlock
  8 				// iPagesPerBlock
};
#endif

_P struct NANDBlockDescriptorStruct 			Type5_BlockDescriptor = {
  128,				// iNumSectorsPerBlock
  128				// iPagesPerBlock
};


///////////////////////////////////////////////////////////////////////////////
// Device Driver API Functions
_P struct NANDDeviceDriverFunctionsStruct		Sm_DeviceDriverAPI  = {

  Type2_ReadSectorData,		// *pReadSectorData(struct NANDDescriptorStruct *pNANDDevice, WORD iChip,
                            //                  WORD iSectorNum, WORD *pDataBuf, BYTE bType);
//  Sm_ReadSectorRedundant,	// *pReadRedundantArea(struct NANDDescriptorStruct *pNANDDevice, WORD iChip,
                            //                     WORD iSectorNum, WORD *pDataBuf);
  ResetNand,                // *pResetNand(WORD iDeviceNum)
#ifdef NAND_WRITES_ALLOWED
  Sm_WriteSectorData,       // *pWriteSectorData(struct NANDDescriptorStruct _P *pNANDDevice, WORD iChip,
                            //                   WORD iSectorNum, WORD *pDataBuf);
  Sm_EraseBlock             // *pEraseBlock(struct NANDDescriptorStruct _P *pNANDDevice, WORD iBlockNum);
#endif
};


#ifdef INCLUDE_TYPE2_NAND
_P struct NANDDeviceDriverFunctionsStruct		Type2_DeviceDriverAPI  = {

  Type2_ReadSectorData,		    // *pReadSectorData(struct NANDDescriptorStruct *pNANDDevice, WORD iChip,
                                //                  WORD iSectorNum, WORD *pDataBuf, BYTE bType);
//  Type2_ReadSectorRedundant,	// *pReadRedundantArea(struct NANDDescriptorStruct *pNANDDevice, WORD iChip,
                                //                     WORD iSectorNum, WORD *pDataBuf);
  ResetNand,                    // *pResetNand(WORD iDeviceNum)

#ifdef NAND_WRITES_ALLOWED
  Type2_WriteSectorData,        // *pWriteSectorData(struct NANDDescriptorStruct _P *pNANDDevice, WORD iChip,
                                //                   WORD iSectorNum, WORD *pDataBuf);
  Sm_EraseBlock                 // *pEraseBlock(struct NANDDescriptorStruct _P *pNANDDevice, WORD iBlockNum);
#endif

};
#endif


///////////////////////////////////////////////////////////////////////////////
// NAND Command Codes Descriptors
_P struct NANDCommandCodesStruct 				Sm_CommandCodes = {

  0x000090, 						// iReadIDCode
  0x000070,	                        // iReadStatusCode
  0x0000ff,                         // iResetCode

  0x000080, 	                    // iSerial Data Input
  0x000000,                         // iRead1Code
  0xffffff,                         // Second Cycle for Read (Type 2 NANDs)
  0x000001,                         // iRead2Code
  0x000050,                         // iRead3Code

#ifdef NAND_WRITES_ALLOWED
  0x000010,                         // iPageProgramCode
  0xffffff,                         // iCacheProgramCode

  // TBD: Add ReadForCopy Back 0x00 & 0x35
  0xffffff,                         // iCopyBackProgramCode
  // TBD: Add Copy Back Program 2nd Cycle 0x10

  0x000060,                         // iBlockEraseCode
  0x0000d0,                         // iBlockErase2Code

  0xffffff,                         // iRandomDataInCode

  0xffffff,                         // iRandomDataOutCode
  0xffffff,                         // iRandomDataOut_2ndCycleCode

  0xffffff,                         // iMultiPlaneBlockEraseCode
  0xffffff                          // iReadMultiPlaneStatusCode
#endif
};

#ifdef INCLUDE_TYPE2_NAND
_P struct NANDCommandCodesStruct 				Type2_CommandCodes = {

  0x000090, 						// iReadIDCode
  0x000070,	                        // iReadStatusCode
  0x0000ff,                         // iResetCode

  0x000080, 	                    // iSerial Data Input
  0x000000,                         // iRead1Code
  0x000030,                         // iRead1_2ndCycle
  0xffffff,                         // iRead2Code
  0xffffff,                         // iRead3Code

#ifdef NAND_WRITES_ALLOWED
  0x000010,                         // iPageProgramCode
  0x000015,                         // iCacheProgramCode

		
  // TBD: Add ReadForCopy Back 0x00 & 0x35
  0x000085,                         // iCopyBackProgramCode
  // TBD: Add Copy Back Program 2nd Cycle 0x10

  0x000060,                         // iBlockEraseCode
  0x0000D0,                         // iBlockErase2Code

  0x000085,                         // iRandomDataInCode

  0x000005,                         // iRandomDataOutCode
  0x0000e0,                         // iRandonDataOut_2ndCycleCode

  0xffffff,                         // iMultiPlaneBlockEraseCode
  0xffffff                          // iReadMultiPlaneStatusCode
#endif
};
#endif



///////////////////////////////////////////////////////////////////////////////
// Device Addressing Descriptors
_P struct NANDDeviceAddressingDescriptorStruct 		Sm_Small_Addressing = {

  1,						// Number of Column Address bytes required
  2							// Number of Row Address bytes required
};
_P struct NANDDeviceAddressingDescriptorStruct 		Sm_Large_Addressing = {

  1,						// Number of Column Address bytes required
  3							// Number of Row Address bytes required
};

#ifdef INCLUDE_TYPE2_NAND
_P struct NANDDeviceAddressingDescriptorStruct 		Type2_Small_Addressing = {

  2,						// Number of Column Address bytes required
  2							// Number of Row Address bytes required
};
_P struct NANDDeviceAddressingDescriptorStruct 		Type2_Large_Addressing = {
  2,						// Number of Column Address bytes required
  3							// Number of Row Address bytes required
};
#endif

///////////////////////////////////////////////////////////////////////////////
// NAND Descriptors
//   Fill in the following fields for each entry:
/***************************************************************************
_P struct NANDDescriptorStruct 		TBD = {
    &NANDDescriptorSubStructType1/2/3/4Small/LargeAddr,  // NandDescriptorSubStruct with common member values for this nand type.
    TBD,                                    // DataBusWidth = 8 or 16 bits
//  TBD,                                    // TotalSectors Unused in bootmanager
//  TBD,                                    // TotalPages   Unused in bootmanager
    TBD                                     // TotalBlocks
//  TBD                                     // TotalZones.  Unused in bootmanager
};
****************************************************************************/

// This SubStruct is used by all the type 1 descriptor structs below. mem savings.
_P struct NANDDescriptorSubStruct NANDDescriptorSubStructType1SmallAddr = {
    &Sm_SectorDescriptor,	                // *pSectorDescriptor
    &Sm_Small_BlockDescriptor,              // *pBlockDescriptor
    &Sm_DeviceDriverAPI,                    // *pDeviceDriverFunctions
    &Sm_CommandCodes,                       // *pDeviceCommandCodes
    &Sm_Small_Addressing,                   // *pDeviceAddressing
    TYPE1_NAND                              // NandType
};

// Similar but for Large Addressing
_P struct NANDDescriptorSubStruct NANDDescriptorSubStructType1LargeAddr = {
    &Sm_SectorDescriptor,	                // *pSectorDescriptor
    &Sm_Large_BlockDescriptor,              // *pBlockDescriptor
    &Sm_DeviceDriverAPI,                    // *pDeviceDriverFunctions
    &Sm_CommandCodes,                       // *pDeviceCommandCodes
    &Sm_Large_Addressing,                   // *pDeviceAddressing
    TYPE1_NAND                              // NandType
};

_P struct NANDDescriptorStruct 		Type1_8MB_8bit_NAND = {		// SmartMedia/SSFDC/Type 1, 8 MB, 8-bit, 3.3V

    &NANDDescriptorSubStructType1SmallAddr, // See members and values at define above.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,			                            // DataBusWidth = 8 or 16 bits
//  16384,                                  // TotalSectors Unused in bootmanager
//  16384,                                  // TotalPages   Unused in bootmanager
    1024                                    // TotalBlocks
//  1                                       // TotalZones   Unused in bootmanager
};


_P struct NANDDescriptorStruct 		Type1_16MB_8bit_NAND = {		// SmartMedia/SSFDC/Type 1, 16 MB, 8-bit, 3.3V

    &NANDDescriptorSubStructType1LargeAddr,          // See members and values at define above.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,			                            // DataBusWidth = 8 or 16 bits
//  32768,                                  // TotalSectors Unused in bootmanager
//  32768,                                  // TotalPages   Unused in bootmanager
    1024                                    // TotalBlocks
//  1                                       // TotalZones   Unused in bootmanager
};

_P struct NANDDescriptorStruct 		Type1_32MB_8bit_NAND = {		// SmartMedia/SSFDC/Type 1, 32 MB, 8-bit, 3.3V

    &NANDDescriptorSubStructType1LargeAddr, // See members and values at define above.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,			                            // DataBusWidth = 8 or 16 bits
//  65536,                                  // TotalSectors Unused in bootmanager
//  65536,                                  // TotalPages   Unused in bootmanager
    2048                                    // TotalBlocks
//  2                                       // TotalZones   Unused in bootmanager
};

_P struct NANDDescriptorStruct 		Type1_64MB_8bit_NAND = {		// SmartMedia/SSFDC/Type 1, 64 MB, 8-bit, 3.3V

    &NANDDescriptorSubStructType1LargeAddr, // See members and values at define above.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,			                            // DataBusWidth = 8 or 16 bits
//  131072,                                 // TotalSectors Unused in bootmanager
//  131072,                                 // TotalPages   Unused in bootmanager
    4096                                   // TotalBlocks
//  4                                       // TotalZones   Unused in bootmanager
};

_P struct NANDDescriptorStruct 		Type1_128MB_8bit_NAND = {		// SmartMedia/SSFDC/Type 1, 128 MB, 8-bit, 3.3V

    &NANDDescriptorSubStructType1LargeAddr, // See members and values at define above.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,			                            // DataBusWidth = 8 or 16 bits
//  262144,                                 // TotalSectors Unused in bootmanager
//  262144,                                 // TotalPages   Unused in bootmanager
    8192                                    // TotalBlocks
//  8                                       // TotalZones   Unused in bootmanager
};

_P struct NANDDescriptorStruct      Type1_16MB_16bit_NAND = {

    &NANDDescriptorSubStructType1LargeAddr, // See members and values at define above.

											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  32768,                                  // TotalSectors Unused in bootmanager
//  32768,                                  // TotalPages   Unused in bootmanager
    1024                                    // TotalBlocks
//  1                                       // TotalZones   Unused in bootmanager
};



_P struct NANDDescriptorStruct      Type1_32MB_16bit_NAND = {

    &NANDDescriptorSubStructType1LargeAddr, // See members and values at define above. 											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  65536,                                  // TotalSectors Unused in bootmanager
//  65536,                                  // TotalPages   Unused in bootmanager
    2048                                    // TotalBlocks
//  2                                       // TotalZones   Unused in bootmanager
};




_P struct NANDDescriptorStruct      Type1_64MB_16bit_NAND = {
    &NANDDescriptorSubStructType1LargeAddr, // See members and values at define above.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  131072,                                 // TotalSectors Unused in bootmanager
//  131072,                                 // TotalPages   Unused in bootmanager
    4096                                    // TotalBlocks
//  4                                       // TotalZones   Unused in bootmanager
};

_P struct NANDDescriptorStruct      Type1_128MB_16bit_NAND = {

    &NANDDescriptorSubStructType1LargeAddr, // See members and values at define above.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                      // DataBusWidth = 8 or 16 bits
//  262144,                                 // TotalSectors Unused in bootmanager
//  262144,                                 // TotalPages   Unused in bootmanager
    8192                                    // TotalBlocks
//  8                                       // TotalZones   Unused in bootmanager
};





/////////////////////////////////////////////////////////////////////////////////
//  Type 2 NANDs
/////////////////////////////////////////////////////////////////////////////////
#ifdef INCLUDE_TYPE2_NAND

// This SubStruct is used by all the type 2 descriptor structs below. mem savings.
_P struct NANDDescriptorSubStruct NANDDescriptorSubStructType2SmallAddr = {
    &Type2_SectorDescriptor,                // *pSectorDescriptor
    &Type2_BlockDescriptor,                 // *pBlockDescriptor
    &Type2_DeviceDriverAPI,                 // *pDeviceDriverFunctions
    &Type2_CommandCodes,                    // *pDeviceCommandCodes
    &Type2_Small_Addressing,                // *pDeviceAddressing
    TYPE2_NAND                              // NandType
};

_P struct NANDDescriptorSubStruct NANDDescriptorSubStructType2LargeAddr = {
    &Type2_SectorDescriptor,                // *pSectorDescriptor
    &Type2_BlockDescriptor,                 // *pBlockDescriptor
    &Type2_DeviceDriverAPI,                 // *pDeviceDriverFunctions
    &Type2_CommandCodes,                    // *pDeviceCommandCodes
    &Type2_Large_Addressing,                // *pDeviceAddressing
    TYPE2_NAND                              // NandType
};

//------------- Type 2 nands below use one of the SubStructs above.

_P struct NANDDescriptorStruct 		Type2_128MB_8bit_NAND = {
    &NANDDescriptorSubStructType2SmallAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,                                      // DataBusWidth = 8 or 16 bits
//  262144,                                 // TotalSectors Unused in bootmanager
//  65536,                                  // TotalPages   Unused in bootmanager
    1024                                    // TotalBlocks
//  1                                       // TotalZones   Unused in bootmanager
};


_P struct NANDDescriptorStruct 		Type2_128MB_16bit_NAND = {
    &NANDDescriptorSubStructType2SmallAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  262144,                                 // TotalSectors Unused in bootmanager
//  65536,                                  // TotalPages   Unused in bootmanager
    1024                                    // TotalBlocks
//  1                                       // TotalZones   Unused in bootmanager
};




_P struct NANDDescriptorStruct 		Type2_256MB_8bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr,  // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,                                      // DataBusWidth = 8 or 16 bits
//  524288,                                 // TotalSectors Unused in bootmanager
//  131072,                                 // TotalPages   Unused in bootmanager
    2048                                    // TotalBlocks
//  2                                       // TotalZones   Unused in bootmanager
};



_P struct NANDDescriptorStruct 		Type2_256MB_16bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  524288,                                 // TotalSectors Unused in bootmanager
//  131072,                                 // TotalPages   Unused in bootmanager
    2048                                    // TotalBlocks
//  2                                       // TotalZones   Unused in bootmanager
};




_P struct NANDDescriptorStruct      Type2_512MB_8bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,                                      // DataBusWidth = 8 or 16 bits
//  1048576,                                // TotalSectors Unused in bootmanager
//  262144,                                 // TotalPages   Unused in bootmanager
    4096                                   // TotalBlocks
//  4                                       // TotalZones   Unused in bootmanager
};



_P struct NANDDescriptorStruct      Type2_512MB_16bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  1048576,                                // TotalSectors Unused in bootmanager
//  262144,                                 // TotalPages   Unused in bootmanager
    4096                                    // TotalBlocks
//  4                                       // TotalZones   Unused in bootmanager
};



    // Those are guesses because datasheets not yet released
_P struct NANDDescriptorStruct      Type2_1024MB_8bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,                                      // DataBusWidth = 8 or 16 bits
//  2*1048576,                              // TotalSectors Unused in bootmanager
//  2*262144,                               // TotalPages   Unused in bootmanager
    2*4096                                  // TotalBlocks
//  8                                       // TotalZones   Unused in bootmanager
};



    // Those are guesses because datasheets not yet released
_P struct NANDDescriptorStruct      Type2_1024MB_16bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  2*1048576,                              // TotalSectors Unused in bootmanager
//  2*262144,                               // TotalPages   Unused in bootmanager
    2*4096                                  // TotalBlocks
//  2*4                                     // TotalZones   Unused in bootmanager
};



    // Those are guesses because datasheets not yet released
_P struct NANDDescriptorStruct      Type2_2048MB_8bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    8,                                      // DataBusWidth = 8 or 16 bits
//  4*1048576,                              // TotalSectors Unused in bootmanager
//  4*262144,                               // TotalPages   Unused in bootmanager
    4*4096                                  // TotalBlocks
//  4*2                                     // TotalZones   Unused in bootmanager
};



    // Those are guesses because datasheets not yet released
_P struct NANDDescriptorStruct      Type2_2048MB_16bit_NAND = {
    &NANDDescriptorSubStructType2LargeAddr, // consolidation for mem savings.
											// Use this parameter only to initialize the global CurrentNANDBusWidth
											// Some NANDs required more real time process to determine  their bus
											// width. (see CurrentNANDBusWidth declaration Note for further information)
    16,                                     // DataBusWidth = 8 or 16 bits
//  4*1048576,                              // TotalSectors Unused in bootmanager
//  4*262144,                               // TotalPages   Unused in bootmanager
    4*4096                                  // TotalBlocks
//  4*2                                     // TotalZones   Unused in bootmanager
};

#endif //#ifdef INCLUDE_TYPE2_NAND





/////////////////////////////////////////////////////////////////////////////////
//  Type 4 Renesas AG-AND
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////// WARNING !!!!!!!!! /////////////////////////
/// Check with Gray to fill these structures properly - update: filled.
////////////////////////////////////////////////////////////////////////////////
#ifdef INCLUDE_RENESAS_DESCRIPTORS
_P struct NANDDescriptorSubStruct NANDDescriptorSubStructType4 = {
    &Type2_SectorDescriptor,        // *pSectorDescriptor
    &Type4_BlockDescriptor,   	    // *pBlockDescriptor
    &Type2_DeviceDriverAPI,         // *pDeviceDriverFunctions
    &Type2_CommandCodes,            // *pDeviceCommandCodes
    &Type2_Small_Addressing,        // *pDeviceAddressing
    TYPE4_NAND,                     // NandType
};
//----------- Type 4 nands below use the SubStruct above. mem savings.

_P struct NANDDescriptorStruct  Type4_128MB_8bit_AND = {
    &NANDDescriptorSubStructType4,
									// Use this parameter only to initialize the global CurrentNANDBusWidth
									// Some NANDs required more real time process to determine  their bus
									// width. (see CurrentNANDBusWidth declaration Note for further information)
	8,
    8192                        	// TotalBlocks
};

_P struct NANDDescriptorStruct  Type4_512MB_8bit_AND = {
    &NANDDescriptorSubStructType4,
									// Use this parameter only to initialize the global CurrentNANDBusWidth
									// Some NANDs required more real time process to determine  their bus
									// width. (see CurrentNANDBusWidth declaration Note for further information)
	8,
    8192                        	// TotalBlocks
};

#endif   //#ifdef INCLUDE_RENESAS_DESCRIPTORS

_P struct NANDDescriptorSubStruct NANDDescriptorSubStructType5 = {
    &Type2_SectorDescriptor,        // *pSectorDescriptor
    &Type5_BlockDescriptor,   	    // *pBlockDescriptor
    &Type2_DeviceDriverAPI,         // *pDeviceDriverFunctions
    &Type2_CommandCodes,            // *pDeviceCommandCodes
    &Type2_Large_Addressing,        // *pDeviceAddressing
    TYPE5_NAND,                     // NandType
};

/////////////////////////////////////////////////////////////////////////////////
//  Type 5 - Toshiba/Sandisk Large Page MLC
/////////////////////////////////////////////////////////////////////////////////
_P struct NANDDescriptorStruct  Type5_256MB_MLC_NAND = {
    &NANDDescriptorSubStructType5,                 // SubDescriptor
    8,
    1024,
};

_P struct NANDDescriptorStruct  Type5_512MB_MLC_NAND = {
    &NANDDescriptorSubStructType5,                 // SubDescriptor
    8,
    2048,
};

_P struct NANDDescriptorStruct  Type5_1024MB_MLC_NAND = {
    &NANDDescriptorSubStructType5,                 // SubDescriptor
    8,
    4096,
};

_P struct NANDDescriptorStruct  Type5_2048MB_MLC_NAND = {
    &NANDDescriptorSubStructType5,                 // SubDescriptor
    8,
    8192,
};

///////////////////////////////////////////////////////////////////////////////
// Device Code - NAND Descriptor Map
// Associates Device/Manufacturer IDs with NAND Descriptors (above), to give their
// parameters.
///////////////////////////////////////////////////////////////////////////////

_P NAND_DEVICECODE_MAP UniqueInitDescriptorIdList[] =
{
  // Note: Devices with pages smaller than 512 bytes are not supported.

  {0x00e6ec, &Type1_8MB_8bit_NAND},           // SSFDC, 8 MB NANDs
  {0x00e698, &Type1_8MB_8bit_NAND},           // SSFDC, 8 MB NANDs

  {0x0073ec, &Type1_16MB_8bit_NAND},          // SSFDC, 16 MB NANDs
  {0x007398, &Type1_16MB_8bit_NAND},          // SSFDC, 16 MB NANDs
  {0x007320, &Type1_16MB_8bit_NAND},          // SSFDC, 16 MB NANDs
  {0x007345, &Type1_16MB_8bit_NAND},          // SSFDC, 16 MB NANDs
  {0x0073ad, &Type1_16MB_8bit_NAND},          // SSFDC, 16 MB NANDs    (Hynix 128Mbit)

  {0x0075ec, &Type1_32MB_8bit_NAND},          // SSFDC, 32 MB NANDs
  {0x007598, &Type1_32MB_8bit_NAND},          // SSFDC, 32 MB NANDs
  {0x007520, &Type1_32MB_8bit_NAND},          // SSFDC, 32 MB NANDs
  {0x007545, &Type1_32MB_8bit_NAND},          // SSFDC, 32 MB NANDs
  {0x0075ad, &Type1_32MB_8bit_NAND},          // SSFDC, 32 MB NANDs    (Hynix 256Mbit)

  {0x0076ec, &Type1_64MB_8bit_NAND},          // SSFDC, 64 MB NANDs (Samsung, 512Mbit, K9F1208UOM, multiplane)
  {0x007698, &Type1_64MB_8bit_NAND},          // SSFDC, 64 MB NANDs (Toshiba, 512Mbit, TC58512)
  {0x007620, &Type1_64MB_8bit_NAND},          // SSFDC, 64 MB NANDs (STMicro, 512Mbit, NAND512W3A)
  {0x007645, &Type1_64MB_8bit_NAND},          // SSFDC, 64 MB NANDs (SanDisk, 512Mbit, SDTNGAHE0)
  {0x0076ad, &Type1_64MB_8bit_NAND},          // SSFDC, 64 MB NANDs, (Hynix,   512Mbit, HY27US08121M)

  {0x0079ec, &Type1_128MB_8bit_NAND},         // SSFDC, 128 MB NANDs
  {0x007998, &Type1_128MB_8bit_NAND},         // Toshiba, 128 MB NANDs
  {0x007920, &Type1_128MB_8bit_NAND},         // SSFDC, 128 MB NANDs (STMicro, 1Gbit, NAND01GW3A)
  {0x007945, &Type1_128MB_8bit_NAND},         // Sandisk, 128 MB NANDs
  {0x0079ad, &Type1_128MB_8bit_NAND},         // SSFDC, 128 MB NANDs (Hynix, 1Gbit, HY27UA081G1M)

#ifdef INCLUDE_RENESAS_DESCRIPTORS
  {0x000107, &Type4_128MB_8bit_AND},       // Renesas AG-AND, 128 MB
  {0x002907, &Type4_128MB_8bit_AND},       // Renesas AG-AND, 4Gbit
#endif

  {0x000001, NULL}       					// Last entry
};

#ifdef INCLUDE_TYPE2_NAND
_P NAND_DEVICECODE_MAP Type2DescriptorIdList[] =
{
  // Note: An FF in the high byte means to ignore the
  // Manufacturer Code (low byte) in matches (wildcard match)

  // Note: Matches are checked in order, so you can test for a specific manufacturer
  // before allowing a wildcard match to any manufacturer for that device.

  // Note: Devices with pages smaller than 512 bytes are not supported.

  {0x00f120, &Type2_128MB_8bit_NAND},      // 128 MB NANDs    ST Micro
  {0x00f1ad, &Type2_128MB_8bit_NAND},      // 128 MB NANDs    Hynix
  {0x00f12c, &Type2_128MB_8bit_NAND},      // 128 MB NANDs    Micron
  {0x00f1ec, &Type2_128MB_8bit_NAND},      // 128 MB NANDs    Samsung
  {0x00f198, &Type2_128MB_8bit_NAND},      // 128 MB NANDs    Toshiba
  {0x00f145, &Type2_128MB_8bit_NAND},      // 128 MB NANDs    SanDisk

  {0x00da20, &Type2_256MB_8bit_NAND},      // 256 MB NANDs    ST Micro
  {0x00daad, &Type2_256MB_8bit_NAND},      // 256 MB NANDs    Hynix
  {0x00da2c, &Type2_256MB_8bit_NAND},      // 256 MB NANDs    Micron
  {0x00daec, &Type2_256MB_8bit_NAND},      // 256 MB NANDs    Samsung
  {0x00da98, &Type2_256MB_8bit_NAND},      // 256 MB NANDs    Toshiba
  {0x00da45, &Type2_256MB_8bit_NAND},      // 256 MB NANDs    SanDisk

  {0x00dc20, &Type2_512MB_8bit_NAND},      // 512 MB NANDs    ST Micro
  {0x00dcad, &Type2_512MB_8bit_NAND},      // 512 MB NANDs    Hynix
  {0x00dc2c, &Type2_512MB_8bit_NAND},      // 512 MB NANDs    Micron
  {0x00dc45, &Type2_512MB_8bit_NAND},      // 512 MB NANDs    SanDisk
  {0x00dc98, &Type2_512MB_8bit_NAND},      // 512 MB NANDs    Toshiba
  {0x00dcec, &Type2_512MB_8bit_NAND},      // 512 MB NANDs    Samsung

  {0x00d398, &Type2_1024MB_8bit_NAND},	   // 1024 MB, 8bit bus, 3.3V
  {0x00d3ec, &Type2_1024MB_8bit_NAND},	   // 1024 MB, 8bit bus, 3.3V
  {0x00d345, &Type2_1024MB_8bit_NAND},   //Prelim
  {0x00d320, &Type2_1024MB_8bit_NAND},   //Prelim
  {0x00d3ad, &Type2_1024MB_8bit_NAND},   //Prelim
  {0x00d32c, &Type2_1024MB_8bit_NAND},   //Prelim

  {0x00d520, &Type2_2048MB_8bit_NAND},   //Prelim
  {0x00d5ad, &Type2_2048MB_8bit_NAND},   //Prelim
  {0x00d52c, &Type2_2048MB_8bit_NAND},   //Prelim
  {0x00d5ec, &Type2_2048MB_8bit_NAND},   //Prelim
  {0x00d598, &Type2_2048MB_8bit_NAND},   //Prelim
  {0x00d545, &Type2_2048MB_8bit_NAND},   //Prelim

  {0x000001, NULL}       					// Last entry
};
#endif // #ifdef INCLUDE_TYPE2_NAND

_P NAND_DEVICECODE_MAP LargeMLCDescriptorIdList[] =
{
  {0x00da98, &Type5_256MB_MLC_NAND},          // 2  GBit Large MLC, Toshiba TC58NVG1D4BFT00
  {0x00da45, &Type5_256MB_MLC_NAND},          // 2  GBit Large MLC, Sandisk

  {0x00dc98, &Type5_512MB_MLC_NAND},          // 4  GBit Large MLC, Toshiba TC58NVG2D4BFT00
  {0x00dc45, &Type5_512MB_MLC_NAND},          // 4  GBit Large MLC, Sandisk
  {0x00dcec, &Type5_512MB_MLC_NAND},          // 4  GBit Large MLC, Samsung K9G4G08U0M
  {0x00dcad, &Type5_512MB_MLC_NAND},          // 4  GBit Large MLC, Hynix HY27UT084G2M 512MByte / HY27UU088G5M 1GByte 2CEs.
  {0x00dc20, &Type5_512MB_MLC_NAND},          // 4  GBit Large MLC, ST Micro NAND04GW3C2A


  {0x00d398, &Type5_1024MB_MLC_NAND},         // 8  GBit Large MLC, Toshiba TH58NVG3D4xFT00
  {0x00d345, &Type5_1024MB_MLC_NAND},         // 8  GBit Large MLC, Sandisk
  {0x00d3ec, &Type5_1024MB_MLC_NAND},         // 8  GBit Large MLC, Samsung                   //Prelim
  {0x00d3ad, &Type5_1024MB_MLC_NAND},         // 8  GBit Large MLC, Hynix HY27UV08AG5M (2*8Gb). 2 CE. 2GByte.

  {0x00d598, &Type5_2048MB_MLC_NAND},         // 16 GBit Large MLC, Toshiba TH58NVG4D4xFT00   //Prelim
  {0x00d545, &Type5_2048MB_MLC_NAND},         // 16 GBit Large MLC, Sandisk                   //Prelim
  {0x00d5ec, &Type5_2048MB_MLC_NAND},         // 16 GBit Large MLC, Samsung                   //Prelim

  {0x000001, NULL}       					// Last entry
};

extern _X WORD CurrentNANDBusWidth;




/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
