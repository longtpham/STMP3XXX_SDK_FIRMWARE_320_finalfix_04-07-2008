/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
//
// Filename: nanddevdrvapi.h
// Description: Function prototypes for NAND Device Driver API
/////////////////////////////////////////////////////////////////////////////////


// TBD: Decide where the next prototype really belongs...
// _reentrant struct NANDDescriptorStruct _P * NandLookupID(WORD iDeviceCode);

// Used in ReadSectorData functions to determine which area is to be read (data or redundant area)
#define	READ_TYPE_REDUNDANT_AREA 	0	
#define READ_TYPE_DATA_AREA 		1
 
_reentrant WORD NANDInitInterfacePort(WORD iDeviceNum);
_reentrant WORD NANDTerminateInterfacePort(WORD iDeviceNum);


_reentrant WORD Sm_ReadId(struct NANDDescriptorStruct _P *pNANDDevice, WORD iDeviceNum);


_reentrant WORD NandReadSectorDataWrapper(struct NANDDescriptorStruct _P *pNANDDevice, 
                                          WORD iDeviceNum, WORD iPageNum, WORD _X *pPageBuf);

_reentrant WORD NandReadSectorRedundantWrapper(struct NANDDescriptorStruct _P *pNANDDevice, 
                                               WORD iDeviceNum, WORD iSectorNum, WORD _X *pBuf);

_reentrant WORD ResetNand(struct NANDDescriptorStruct _P *pNANDDevice, WORD iDeviceNum);

/////////////////////////////////////////////////////////////////////////////////
// SmartMedia (SSFDC) Device Driver Routines
_reentrant WORD Sm_ReadSectorData(struct NANDDescriptorStruct _P *pNANDDevice, WORD iDeviceNum, 
                                  WORD iPageNum, WORD _X *pDataBuf, BYTE bType);

//_reentrant WORD Sm_ReadSectorRedundant(struct NANDDescriptorStruct _P *pNANDDevice, WORD iDeviceNum, 
//                                       WORD iPageNum, WORD *pDataBuf);


#ifdef INCLUDE_TYPE2_NAND
/////////////////////////////////////////////////////////////////////////////////
// Type2 NANDs (e.g. Samsung 1Gbit and 2Gbit NANDs)
_reentrant WORD Type2_ReadSectorData(struct NANDDescriptorStruct _P *pNANDDesc, 
                                            WORD iDeviceNum, 
                                            WORD iSectorNum,
                                            WORD _X *pBuf,
                                            BYTE bType);

//_reentrant WORD Type2_ReadSectorRedundant(struct NANDDescriptorStruct _P *pNANDDesc, 
//                                            WORD iDeviceNum, 
//                                            WORD iSectorNum,
//                                                 WORD _X *pBuf);
#endif

#ifdef INCLUDE_MLC_DESCRIPTORS
_reentrant WORD Type3ReadSectorData(struct NANDDescriptorStruct _P *pNANDDevice, 
                                    WORD iDeviceNum, WORD iSectorNum, 
                                    WORD _X *pBuf, BYTE bType);
#endif

