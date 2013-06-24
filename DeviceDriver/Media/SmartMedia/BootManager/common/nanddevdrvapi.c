///////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: nanddevdrvapi.c
// Description: NAND Device Driver API functions
///////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "nandstructs.h"
#include "smequ.h"


#include "gpflash_hal.h"
#include "nanddevdrvapi.h"
#include "NANDRA_utils.h"

#ifndef SUCCESS
#define SUCCESS	0
#endif

#define MakerIDMask 0x0000ff
#define SamsungMakerID 0x0000ec
#define SandiskMakerID 0x000045
#define ToshibaMakerID 0x000098
#define ReadID2MLCMask 0x00000c
#define ReadID2MLCValue 0x000004

#define TOSHIBA_SAMSUNG_TYPE1_TYPE3_MASK	0x00c000
#define READ_ID2_DATA_BUS_WIDTH_MASK		0x00C000
#define READ_ID2_DATA_BUS_8BIT				0x004000
#define READ_EXT_ID_DATA_BUS_WIDTH_MASK		0x004000
#define READ_EXT_ID_DATA_BUS_8BIT  			0x000000
	
//#define NO_SELF_CORRECTING_NAND_SECTOR_READS  1
#define CRC_ECC_CORRECT 1
//////////////////////////////////////////////////////////////////////////////
//  Extern functions
//////////////////////////
extern RETCODE _reentrant NANDHalCorrectECC_4Bit(WORD _X * pCompleteSectorBuf);
WORD  DivideByThree(WORD wVal);

WORD _X SectorSize;
WORD _X SectorSizeInWords;
WORD _X SectorSizeInWordsPlusOne;

///////////////////////////////////////////////////////////////////////////////
// Private functions
#ifdef INCLUDE_TYPE2_NAND
_reentrant WORD Type2_ComputeAddress(struct NANDDescriptorStruct _P *pNANDDesc,
                                     WORD iSectorNum, WORD iByteOffset, 
                                     WORD *piColAddr, WORD *piRowAddr);
#endif

_reentrant RETCODE DetermineNandType(WORD wNandDeviceNumber);

void _reentrant NANDHALCommon_ReadId( WORD wChipNum ) ;
//_reentrant RETCODE NANDHALCommon_ReadId2(WORD wChipNum, WORD _X * pID);
//_reentrant RETCODE NANDHALCommon_ReadIdExtended(WORD wChipNum, WORD _X * pID);
_reentrant struct NANDDescriptorStruct _P * NandLookupID(NAND_DEVICECODE_MAP _P * _P pNANDDevicesMap);




///////////////////////////////////////////////////////////////////////////////
// Hardware API Function Descriptors


// For the STMP3500, we always use the GPFLASH interface.  For STMP3410, we always use the Compact Flash 
// interface.  THIS IS TRUE FOR BOOT ROM ONLY.  For non-BOOTROM code, the 3410 may use Compact Flash for 
// Type2 NANDs and EMC for Type1 (SmartMedia). So the determination of which interface functions to call 
// will be dynamic, and may vary with the chip/card number.  The code below, and any code that uses these 
// pointers may need some rewrites for non-BOOTROM use.


                                          // 3500 build - use GPFLASH Port

_P struct NANDHardwareAPIFunctionsStruct		GPFLASHNandPortFunctions = {     // Use GPFLASH Interface

  InitGPFLASHPort,   			// *pPortInit(int iDeviceNum);
  WriteCmdGPFLASH,   		    // *pWritePortCmd(int iDeviceNum, int iCommandCode);
  WriteAddrGPFLASH,     		// *pWritePortAddr(iDeviceNum, iNumColBytes, 
                                //                 iColumnAddress, iNumRowBytes, iRowAddress);
  ReadDataGPFLASH,   			// *pReadPortData(int iDeviceNum, int iNumBytesToRead, 
                                // int *pBuffer, int b16BitMode);
  WaitForReadyGPFLASH,          // WORD *pWaitForReadyPort(WORD iDeviceNum, WORD iTimeoutCount);

  // TBD: This needs to have a Compact Flash version and an EMC version, with appropriate names.
  PowerUpFlashPads,       		// WORD *pPowerUpPads(void);
  PowerUpFlashPads_16bit,	    // void *pPowerUpPads_16bit(void);
  TerminateGPFLASHPort          // WORD *pTerminatePort(int iDeviceNum);
};


_P struct NANDHardwareAPIFunctionsStruct		* _P g_pCurrentNandPort = &GPFLASHNandPortFunctions;


///////////////////////////////////////////////////////////////////////////////
// Private (static) variables

                                       // Tracking for "cached" pages in Type2 NANDs
                                       // If s_bCachedPageAvailableInNAND[iDev] is TRUE, 
                                       // a Page is already available in the NAND and
                                       // can be read with Random Output commands.  The
                                       // Page Number of the cached page is given in 
                                       // s_iCachedPageNumInNAND[iDev].
//_P static WORD s_bCachedPageAvailableInNAND[4] = {0, 0, 0, 0};
//#ifdef INCLUDE_TYPE2_NAND
//_P static WORD s_iCachedPageNumInNAND[4];
//#endif

struct
{
    // Read ID Byte 1
    WORD MakerCode              : 8;
    // Read ID Byte 2
    WORD DeviceCode             : 8;
    // Read ID Byte 3
    WORD InternalChipNumber     : 2;        // Number of die = (1 << n)
    WORD CellType               : 2;        // Number of bits per memory cell = ( 1 << (n+1) )
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
} SECTOR_BUFFER_MEM ReadIDDecode;
// Note on # of planes for Toshiba Large Block MLC
//  In case of multi-stacked device, the "plane number" will show the total number of planes of the packaged
//  device and not the number of planes per die. (e.g. If 2 pieces of device that has 2 planes were stacked
//  in a package, the plane number will show "4")

_P struct NANDDescriptorStruct * _P pNandInitStruct;

extern _P NAND_DEVICECODE_MAP UniqueInitDescriptorIdList[];
extern _P NAND_DEVICECODE_MAP Type1DescriptorIdList[];
#ifdef INCLUDE_TYPE2_NAND
extern _P NAND_DEVICECODE_MAP Type2DescriptorIdList[];
#endif
extern _P NAND_DEVICECODE_MAP LargeMLCDescriptorIdList[];

extern _X WORD CurrentNANDBusWidth;
extern struct NANDDescriptorStruct _P * _X pCurrentNANDDescriptor;


#pragma asm
;///////////////////////////////////////////////////////////////////////////////
;> Name: DivideByThree
;
;  Type: Function
;  Description:     WORD  DivideByThree(WORD wVal, WORD _Y *pRemainder);
;  Inputs:          wVal (in A)          - The value to divde by 3
;                   pRemainder (in R0)   - Pointer for returning the remainder
;  Outputs:         Return (in A) the quotient: wVal/3
;  Notes:
;<
;///////////////////////////////////////////////////////////////////////////////
FDivideByThree

OneThird equ 0.333333333333
Three    equ 3


    move    a1,x0                           ;put the operand into x0
    clr     a               #>OneThird,y0
    mpy     x0,y0,b         x0,a0           ;b1 = wVal/3,  a0 = wVal

    move    b1,x0                        	;x0 = wVal/3
    asl     a               #>Three,y0      ;right shift it, so the MAC will be correct, y0 = 3
    mac     -x0,y0,a               	        ;a = 3 * (wVal/3) - wVal   (the remainder)

    asr     a                               ; get the right shift, now

;    move    a0,y:(r0)                       ;return remainder through y:(r0)
    move    x0,a                            ;and quotient in A
    rts


#pragma endasm

///////////////////////////////////////////////////////////////////////////////
//> Name:        NandHalInit
//
//  Type:        Function
//  Description: Finds and reports the NAND specifications.
//  Inputs:		 wNandDeviceNumber : Physical NAND number to initialize.      
//  Outputs:     Returns ERROR CODE for Error, ELSE SUCCESS plus pCurrentNANDDescriptor is filled in
//  Notes:       Error Checking: Expects DDI to clear the pNandParams->NandType to zero before first call.
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant RETCODE NandHalInit(WORD wNandDeviceNumber) {

  	RETCODE Status;

    if(g_pCurrentNandPort->pWritePortCmd(wNandDeviceNumber, 0xFF, FALSE))
        return(1);


    // Wait for the NAND to return to READY
    if(g_pCurrentNandPort->pWaitForReadyPort(wNandDeviceNumber, NAND_RESET_TIMEOUT))
        return(1);

  	Status = DetermineNandType(wNandDeviceNumber);
   
  	if(Status != SUCCESS)
		return(Status);

  	pCurrentNANDDescriptor = pNandInitStruct;
    SectorSize = pCurrentNANDDescriptor->pNandDescriptorSubStruct->pPageDescriptor->iSectorDataSize;
    SectorSizeInWords = DivideByThree(SectorSize);
    SectorSizeInWordsPlusOne = SectorSizeInWords + 1;
	return(SUCCESS);
}



///////////////////////////////////////////////////////////////////////////////
//> Name:        DetermineNandType
//
//  Type:        Function
//  Description: Ping the NAND specified in the input struct and selects the appropriate
//				 NAND_INIT_DESCRIPTOR
//  Inputs:
//  Outputs:     Error Code or SUCCESS
//  Notes:       
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant RETCODE DetermineNandType(WORD wNandDeviceNumber)
{

    _P NAND_DEVICECODE_MAP  *  pNANDDeviceMap;

	// By default always set the NAND to 8-bit bus
	CurrentNANDBusWidth = 8;

  	// Call ReadID1 to determine NAND Maker (also gets the Device number)
    NANDHALCommon_ReadId(wNandDeviceNumber);

    pNandInitStruct = NandLookupID(UniqueInitDescriptorIdList);
    // If not found in the unique list, check the specializations
    if ( NULL == pNandInitStruct )
    {
        // Test for MLC
        if ( 0 != ReadIDDecode.CellType )
        {
            // Media is LBMLC
            pNANDDeviceMap = LargeMLCDescriptorIdList;
        }
		else
        {
            // Media is Type 2
            pNANDDeviceMap = Type2DescriptorIdList;
        }

        pNandInitStruct = NandLookupID(pNANDDeviceMap);

    }

	return(SUCCESS);	

}


///////////////////////////////////////////////////////////////////////////////
//> Name:        NANDHALCommon_ReadId
//
//  Type:        Function
//  Description: Read the 6-byte ID from the chip.
//  Inputs:      
//  Outputs:     Returns ERROR CODE for Error, ELSE SUCCESS
//<
/////////////////////////////////////////////////////////////////////////////////
void _reentrant NANDHALCommon_ReadId( WORD wChipNum )
{
  g_pCurrentNandPort->pWritePortCmd( wChipNum, 0x90, 0 );
  g_pCurrentNandPort->pWritePortAddr(wChipNum, 0, 0, 0, 0, 0);
  g_pCurrentNandPort->pReadPortData(wChipNum, 6, (WORD _X *)&ReadIDDecode,0);
}


/////////////////////////////////////////////////////////////////////////////////
//> Name:        NAND_INIT_DESCRIPTOR _P * NandLookupID(WORD iDeviceCode)
//
//  Type:        Function
//  Description: Search through the NAND Types table for a match to wDeviceCode
//               and return a pointer to the matching NAND Descriptor.
//               If the Table entry has an "FF" in the high byte, ignore the
//               Manufacturer ID (match any manufacturer);  otherwise, the
//               whole ID must match.
//  Inputs:      WORD wDeviceCode - 0x00ddmm
//                   "dd" is the Device ID
//                   "mm" is the Manufacturer ID
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant struct NANDDescriptorStruct _P * NandLookupID(NAND_DEVICECODE_MAP _P * _P pNANDDevicesMap) {

    WORD wDeviceCode = ((WORD _X *)&ReadIDDecode)[0] & 0x00FFFF;
	WORD wNandIdx;

	for( wNandIdx = 0 ; pNANDDevicesMap[wNandIdx].DeviceManufacturerCode != 0x000001 ; wNandIdx++ )
	{
      	if (wDeviceCode == pNANDDevicesMap[wNandIdx].DeviceManufacturerCode)
        	return (pNANDDevicesMap[wNandIdx].pNANDDescriptor);
	}

  return(NULL);
}




/////////////////////////////////////////////////////////////////////////////////
//> Name: NANDInitInterfacePort
//
//  Type: Function
//  Description: 
//  Inputs: 
//  Outputs: 1 or 0
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NANDInitInterfacePort(WORD iDeviceNum) {

     g_pCurrentNandPort->pPowerUpPads();

     if (g_pCurrentNandPort->pPortInit(iDeviceNum))    // Init the NAND Port
       return(1);

     return(0);
}

     
/////////////////////////////////////////////////////////////////////////////////
//> Name: NANDTerminateInterfacePort(WORD iDeviceNum)
//
//  Type: Function
//  Description: 
//  Inputs: 
//  Outputs: 1 or 0
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NANDTerminateInterfacePort(WORD iDeviceNum) {

     if (g_pCurrentNandPort->pTerminatePort(iDeviceNum))    // Terminate the NAND Port
       return(1);

     // NOTE: Leave the pads powered up for the next guy.

     return(0);
}




/////////////////////////////////////////////////////////////////////////////////
//> Name: NandReadSectorDataWrapper
//
//  Type: Function
//  Description: 
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NandReadSectorDataWrapper(struct NANDDescriptorStruct _P *pNANDDevice, 
                                          WORD iDeviceNum, 
                                          WORD iSectorNum, 
                                          WORD _X *pBuf) 
{
    WORD wStatus;

    wStatus = pNANDDevice->pNandDescriptorSubStruct->pDeviceDriverFunctions->pReadSectorData(pNANDDevice, 
                                                              iDeviceNum, 
                                                              iSectorNum, 
                                                              pBuf,
                                                              READ_TYPE_DATA_AREA);
    // Check wStatus ReadSectorData return code. 
    //    Type 2 func (also used by type 4) returned SUCCESS or 1 if Type2_ReadFromPage errored.
    if(wStatus != SUCCESS) 
    {    return 1; // 1 means *pReadSectorData function failed with error return. 
    }   // Previous ver ignored pReadSectorData return codes. Shall we continue to do that?

    // May26 2004: Insert CRC/ECC correction:   For all nand types. 
    // Convert ram version of Redundant Area data to std RAM form. 
    NANDRA_ToStandardForm(pBuf, TRUE); // bWholeSector is TRUE. This always returns SUCCESS.

    //  Check & correct CRC if needed.
    //  NANDRA_CRCCorrect() returns SUCCESS, if no correction needed.
    //                  something else ??, if successfully corrected
    //                  something else ??, if correction failed
    if( (wStatus = NANDHalCorrectECC_4Bit(pBuf)) != SUCCESS)
    {   if(wStatus == NANDHAL_ECC_FIX_FAILED) // If correction was needed but failed
           return(2); // announce "correction was needed but failed"
    }

    // Data is good so convert RA back to NAND form as expected by bootmanager.
    // Check & correct ECC. No need to convert RA back to nand form
//    NANDRA_ToNANDForm(pBuf, TRUE); // bWholeSector is TRUE. This always returns SUCCESS.

    
    return SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////
//> Name: NandReadSectorRedundantWrapper
//
//  Type: Function
//  Description: 
//  Inputs: 
//  Outputs: 
//  Notes: 
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD NandReadSectorRedundantWrapper(struct NANDDescriptorStruct _P *pNANDDevice, 
                                       WORD iDeviceNum, WORD iSectorNum, WORD _X *pBuf) 
{
    return SUCCESS; // No uncorrectable errors occurred during the RA read
    
}


/////////////////////////////////////////////////////////////////////////////////
//> Name: CommonWaitThenRead
//
//  Type: Function
//  Description: 
//  Inputs: 
//  Outputs: 1 or SUCCESS
//  Notes:
//		Both types of reads require a wait for ready then read.
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD CommonWaitThenRead(struct NANDDescriptorStruct _P *pNANDDevice, 
                                  WORD iDeviceNum, WORD iSize, WORD _X *pBuf)
{
  // Wait for READY line to come HIGH
  // Read could be whole page or just 
  // Redundant Area.  Use the longer timeout.

  if(g_pCurrentNandPort->pWaitForReadyPort(iDeviceNum, NAND_READ_PAGE_TIMEOUT))
    return(1);

   // Read in the bytes
  if(g_pCurrentNandPort->pReadPortData(iDeviceNum, iSize, pBuf, (CurrentNANDBusWidth==16)))
    return(1);

  return SUCCESS;
}



#ifdef INCLUDE_TYPE2_NAND
/////////////////////////////////////////////////////////////////////////////////
//> Name: Type2_ReadSectorData
//
//  Type: Function
//  Description: 
//  Inputs: 
//  Outputs: 1 or SUCCESS
//  Notes: 
//		To reduce code size for EEPROM build we combined Type2_ReadSectorData()
//		and Type2_ReadSectorRedundant(). INCLUDE_TYPE2_NAND must be defined.
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD Type2_ReadSectorData(struct NANDDescriptorStruct _P *pNANDDesc, 
                                            WORD iDeviceNum, 
                                            WORD iSectorNum,
                                     WORD _X *pBuf, BYTE bType)
{

	WORD iCol;
	WORD iPageNum;
	INT iByteOffset;
	INT iSize;
    BOOL sendSecondReadCmd;


		iByteOffset = 0;
		iSize = pNANDDesc->pNandDescriptorSubStruct->pPageDescriptor->iSectorTotalSize; 

    switch ( pNANDDesc->pNandDescriptorSubStruct->NandType )
    {
    case TYPE1_NAND:
        iPageNum = iSectorNum;
        iCol = 0;
        sendSecondReadCmd = 0;
        break;

    default:
    case TYPE2_NAND:
    case TYPE4_NAND:
    case TYPE5_NAND:
        if ( pNANDDesc->pNandDescriptorSubStruct->pPageDescriptor->iSectorToPageShift == 0 )
            iPageNum = iSectorNum;
        else
    iPageNum = iSectorNum >> pNANDDesc->pNandDescriptorSubStruct->pPageDescriptor->iSectorToPageShift;

  iCol = (iSectorNum & pNANDDesc->pNandDescriptorSubStruct->pPageDescriptor->iSectorInPageMask) *
    pNANDDesc->pNandDescriptorSubStruct->pPageDescriptor->iSectorTotalSize + iByteOffset;

        if ( CurrentNANDBusWidth == 16 )        // For 16-bit NANDs, the Column Address is in Words
    iCol = iCol>>1;                   // so divide by 2.

        sendSecondReadCmd = 1;
        break;
    };

    // Do FULL READ

    if ( g_pCurrentNandPort->pWritePortCmd(iDeviceNum,
                                           pNANDDesc->pNandDescriptorSubStruct->pDeviceCommandCodes->iRead1Code, TRUE) )
      return(1);

    if ( g_pCurrentNandPort->pWritePortAddr(iDeviceNum,
                                      pNANDDesc->pNandDescriptorSubStruct->pDeviceAddressing->iNumColumnBytes, iCol,
                                            pNANDDesc->pNandDescriptorSubStruct->pDeviceAddressing->iNumRowBytes, iPageNum, (0 != sendSecondReadCmd)) )
      return(1);

    if ( 0 != sendSecondReadCmd )
    {
        if ( g_pCurrentNandPort->pWritePortCmd(iDeviceNum, pNANDDesc->pNandDescriptorSubStruct->pDeviceCommandCodes->iRead1_2ndCycleCode, TRUE) )
            return(1);
    }

    // Wait for READY line to come HIGH
    if ( CommonWaitThenRead(pNANDDesc, iDeviceNum, iSize, pBuf) )
        return(1);

  return(0);  // Success
}

#endif // #ifdef INCLUDE_TYPE2_NAND




/////////////////////////////////////////////////////////////////////////////////
//> Name: ResetNand
//
//  Type: Function
//  Description: 	Send a RESET command to the specified NAND.
//  Inputs: 
//  Outputs: 
//  Notes:			This covers all NAND types, so far.
//<
/////////////////////////////////////////////////////////////////////////////////
_reentrant WORD ResetNand(struct NANDDescriptorStruct _P *pNANDDevice, WORD iDeviceNum) {

		  // Send the RESET
  if(g_pCurrentNandPort->pWritePortCmd(iDeviceNum, pNANDDevice->pNandDescriptorSubStruct->pDeviceCommandCodes->iResetCode, FALSE))
    return(1);


		  // Wait for the NAND to return to READY
          // TBD: This is OK for BootROM, but may need to be asynchronous in the SDK
  if(g_pCurrentNandPort->pWaitForReadyPort(iDeviceNum, NAND_RESET_TIMEOUT))
    return(1);

  return(0);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////




