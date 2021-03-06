/////////////////////////////////////////////////////////////////////////////////
//// Copyright(C) SigmaTel, Inc. 2003
////
//// Filename: errors.inc
//// Description: 
/////////////////////////////////////////////////////////////////////////////////

#if (!defined(ERRORS_INC))
#define ERRORS_INC 1

#define LDRIVE_ERROR_INVALID_DRIVE_NUMBER 0x0101
#define LDRIVE_ERROR_NOT_INITIALIZED 0x0102
#define LDRIVE_ERROR_HARDWARE_FAILURE 0x0103
#define LDRIVE_ERROR_INVALID_DRIVE_TYPE 0x0104
#define LDRIVE_ERROR_INVALID_INFO_TYPE 0x0105
#define LDRIVE_ERROR_SECTOR_OUT_OF_BOUNDS 0x0106
#define LDRIVE_ERROR_WRITE_FAILURE 0x0107
#define LDRIVE_ERROR_WRITE_PROTECTED 0x0108
#define LDRIVE_ERROR_MULTI_WRITE_IN_PROGRESS 0x0109
#define LDRIVE_ERROR_MUST_HAVE_MORE_THAN_ONE_SECTOR 0x010A
#define LDRIVE_ERROR_MULTI_WRITE_NOT_SETUP 0x010B
// The WRITE_ABORT Error is returned when the system drive detects a bad
// block in the device. The block was marked bad and tables updated.
// The application must start over.
#define LDRIVE_ERROR_WRITE_ABORT 0x010C
#define LDRIVE_ERROR_READ_FAILURE 0x010D
#define LDRIVE_ERROR_MEDIA_NOT_ALLOCATED 0x010E
#define LDRIVE_ERROR_LOW_LEVEL_MEDIA_FORMAT_REQUIRED 0x010F

#define LMEDIA_ERROR_HARDWARE_FAILURE 0x0200
#define LMEDIA_ERROR_INVALID_MEDIA_NUMBER 0x0201
#define LMEDIA_ERROR_MEDIA_NOT_INITIALIZED 0x0202
#define LMEDIA_ERROR_MEDIA_NOT_DISCOVERED 0x0203
#define LMEDIA_ERROR_INVALID_MEDIA_INFO_TYPE 0x0204
#define LMEDIA_ERROR_ALLOCATION_TO_LARGE 0x0205
#define LMEDIA_ERROR_MEDIA_NOT_ERASED 0x0206
#define LMEDIA_ERROR_MEDIA_ERASED 0x0207
//Specific to NAND
#define LMEDIA_ERROR_NO_REGIONS_IN_MEDIA 0x0208     
#define LMEDIA_ERROR_DRIVES_MAX_OUT 0x0209 
#define LMEDIA_ERROR_MEDIA_WRITE_PROTECTED 0x020A
#define LMEDIA_ERROR_INVALID_INFO_TYPE 0x020B
#define LMEDIA_ERROR_BAD_BLOCKS_MAX_OUT 0x020C
#define LMEDIA_ERROR_NOT_ALLOCATED 0x020D

#define LMEDIA_ERROR_DRIVE_TYPE_NOT_SUPPORTED 0x0210

#define NAND_ERROR_WRITE_PHYS_SECTOR_PROGRAM_FAILED 0x0301
#define ERROR_MULTI_WRITE_SECTOR_FIFO_OVERFLOW 0x0302
#define ERROR_MULTI_WRITE_SECTOR_FIFO_UNDERFLOW 0x0303

#define NAND_DATA_DRIVE_ERROR_LOGICAL_SECTOR_OUT_OF_BOUNDS 0x0401
#define NAND_DATA_DRIVE_ERROR_RBLOCK_NOT_FOUND 0x0402
#define NAND_DATA_DRIVE_ERROR_NO_FREE_RBLOCKS 0x0403
#define NAND_DATA_DRIVE_ERROR_USECTOR_LOOKUP_INTERNAL_1 0x0404
#define NAND_DATA_DRIVE_SECTOR_NOT_UPDATED 0x0405
#define NAND_DATA_DRIVE_ERROR_CANT_RECYCLE_USECTOR_MAP 0x0406
#define NAND_DATA_DRIVE_ERROR_UBLOCK_LBAS_INCONSISTENT 0x0407
#define NAND_DATA_DRIVE_ERROR_HSECTORIDX_IN_UBLOCK_OUT_OF_RANGE 0x0408
#define NAND_DATA_DRIVE_ERROR_CANT_RECYCLE_UBLOCK 0x0409
#define NAND_DATA_DRIVE_ERROR_BBTABLE_FULL 0x040A
#define NAND_DATA_DRIVE_ERROR_UPDATE_NOT_OPEN 0x040B
#define NAND_DATA_DRIVE_ERROR_ADD_USECTOR_INTERNAL_1 0x040C
#define NAND_DATA_DRIVE_ERROR_CANT_GET_ERASED_UBLOCK 0x040D
#define NAND_DATA_DRIVE_ERROR_NO_ERASED_UBLOCKS 0x040E
#define NAND_DATA_DRIVE_ERROR_CANT_ERASE_FREE_UBLOCK 0x040F
#define NAND_DATA_DRIVE_ERROR_KILLUSECTOR_INTERNAL_1 0x0410
#define NAND_DATA_DRIVE_ERROR_KILLUSECTOR_INTERNAL_2 0x0411
#define NAND_DATA_DRIVE_RETURN_BLOCK_NOT_UPDATED 0x0412
#define NAND_DATA_DRIVE_ERROR_UBLOCK_PROTECT_TABLE_FULL 0x0413
#define NAND_DATA_DRIVE_ERROR_UBLOCK_ALREADY_PROTECTED 0x0414
#define NAND_DATA_DRIVE_ERROR_UBLOCK_NOT_PROTECTED 0x0415
#define NAND_DATA_DRIVE_ERROR_UBLOCKPROTECT_INTERNAL_1 0x0416
#define NAND_DATA_DRIVE_ERROR_RECOVERUBLOCK_INTERNAL_1 0x0417
#define NAND_DATA_DRIVE_ERROR_RECOVERUBLOCK_INTERNAL_2 0x0418
#define NAND_DATA_DRIVE_ERROR_UBLOCK_NOT_IN_UTABLE 0x0419
#define NAND_DATA_DRIVE_ERROR_CANT_ALLOCATE_USECTORS_MAPS 0x041A
#define NAND_DATA_DRIVE_ERROR_CANT_INIT_DATA_REGIONS_LIST 0x041B
#define NAND_DATA_DRIVE_ERROR_TOO_MANY_UBLOCKS_IN_CONFIG 0x041C
#define NAND_DATA_DRIVE_ERROR_USECTOR_INDEX_IS_NOT_NEXT 0x041D
#define NAND_DATA_DRIVE_ERROR_USECTOR_ALREADY_UPDATED 0x041E
#define NAND_DATA_DRIVE_ERROR_BgGC_USECTOR_ERASE_TIMEDOUT 0x041F
#define NAND_DATA_DRIVE_ERROR_BgGC_HSECTOR_COPY_TIMEDOUT 0x0420
#define NAND_DATA_DRIVE_ERROR_BgGC_ALREADY_ENABLED 0x0421
#define NAND_DATA_DRIVE_ERROR_BgGC_HSECTOR_ERASE_TIMEDOUT 0x0422
#define NAND_DATA_DRIVE_ERROR_BgGC_SECTOR_NOT_UPDATED 0x0423
#define NAND_DATA_DRIVE_ERROR_NO_STALE_UBLOCKS 0x0424
#define NAND_DATA_DRIVE_ERROR_NAND_IS_READY_TIMEOUT 0x0425
#define NAND_DATA_DRIVE_ERROR_CANT_CLOSE_UPDATE 0x0426

#define NAND_DATA_DRIVE_ERROR_CMW_S0_FETCH_FAILED 0x042C
#define NAND_DATA_DRIVE_ERROR_CMW_S1_FETCH_FAILED 0x042D
#define NAND_DATA_DRIVE_ERROR_CMW_S2_1_FETCH_FAILED 0x042E
#define NAND_DATA_DRIVE_ERROR_CMW_S2_2_FETCH_FAILED 0x042F
#define NAND_DATA_DRIVE_ERROR_CMW_S3_1_FETCH_FAILED 0x0430
#define NAND_DATA_DRIVE_ERROR_CMW_S3_2_FETCH_FAILED 0x0431
#define NAND_DATA_DRIVE_ERROR_CMW_S4_1_FETCH_FAILED 0x0432
#define NAND_DATA_DRIVE_ERROR_CMW_S4_2_FETCH_FAILED 0x0433
#define NAND_DATA_DRIVE_ERROR_CMW_S5_FETCH_FAILED 0x0434
#define NAND_DATA_DRIVE_ERROR_CMW_S0_ECC_TIMEOUT 0x0435
#define NAND_DATA_DRIVE_ERROR_CMW_S2_ECC_TIMEOUT 0x0436
#define NAND_DATA_DRIVE_ERROR_CMW_S3_ECC_TIMEOUT 0x0437
#define NAND_DATA_DRIVE_ERROR_CMW_S4_ECC_TIMEOUT 0x0438
#define NAND_DATA_DRIVE_ERROR_CMW_S5_ECC_TIMEOUT 0x0439
#define NAND_DATA_DRIVE_ERROR_CMW_WRITE_FAILED 0x043A
#define NAND_DATA_DRIVE_ERROR_CMW_RELEASE_FAILED 0x043B

#define NAND_DATA_DRIVE_ERROR_INVALID_LOGICAL_SECTOR 0x0427
#define NAND_DATA_DRIVE_ERROR_INVALID_RELATIVE_SECTOR 0x0428
#define NAND_DATA_DRIVE_ERROR_UBLOCK_IS_EMPTY 0x0429
#define NAND_DATA_DRIVE_ERROR_USECTOR_MAP_IS_BAD 0x042A
#define NAND_DATA_DRIVE_ERROR_TOO_MANY_BLOCK_RECOVERIES 0x042B

// indicates that during the NandHalInit, the NandType did not
// match the previously initialized Nand chip type.
#define NANDHAL_ERROR_NANDTYPE_MISMATCH 0x0500 
#define NANDHAL_ERROR_LOOKUP_ID_FAILED 0x0501
// Errors from calling HIL functions
#define NANDHAL_ERROR_INIT_PORT 0x0502
#define NANDHAL_ERROR_WRITE_PORT_CMD 0x0503
#define NANDHAL_ERROR_WRITE_PORT_ADDR 0x0504
#define NANDHAL_ERROR_READ_PORT_DATA 0x0505
#define NANDHAL_ERROR_WAIT_FOR_READY_PORT 0x0506
#define NANDHAL_ERROR_POWER_UP_FLASH_PADS 0x0507
#define NANDHAL_ERROR_TERMINATE_PORT 0x0508
#define NANDHAL_ERROR_LOCKPORT_TIMEOUT 0x0509
#define NANDHAL_ERROR_LOCKNAND_TIMEOUT 0x050A
// for 3410 build
#define NANDHAL_ERROR_LOCKPORT_LOCKED 0x050B
// for 3410 build
#define NANDHAL_ERROR_LOCKNAND_LOCKED 0x050C
#define NANDHAL_ERROR_WRITE_DATA_PORT 0x050D

// indicates that the GetStatus returned a FAIL
#define NANDHAL_ERROR_GETSTATUS_FAILED 0x050E
#define NANDHAL_ERROR_WRITE_FAILED 0x050F
#define NANDHAL_ERROR_READ_FAILED 0x0510
#define NANDHAL_ERROR_READID1_FAILED 0x0511
#define NANDHAL_ERROR_READID2_FAILED 0x0512
#define NANDHAL_ERROR_READIDEXT_FAILED 0x0513
// indicates the NAND was already BUSY
#define NANDHAL_ERROR_SETNANDBUSY_FAILED 0x0514
#define NANDHAL_ERROR_ASYNCWAIT_CALLBACK_ERR 0x0515
// ECC Error Codes
// ECC Computation has successfully completed
#define COMPUTE_ECC_SUCCESS 0         
// ECC computation still busy
#define COMPUTE_ECC_NOT_DONE 0x0516
// timed out while waiting on object
#define NANDHAL_ERROR_LOCKECC_TIMEOUT 0x0517
// flag already set for SDK2.4 lock
#define NANDHAL_ERROR_LOCKECC_LOCKED 0x0518

#define NANDHAL_ECC_NOERROR 0
// SKIP 0519-051B for space optimization in ECC calculate functions
#define NANDHAL_ECC_ERROR_FIXED 0x051C
#define NANDHAL_ECC_ERROR_FIXED_REWRITE_SECTOR 0x051D
#define NANDHAL_ECC_FIX_FAILED 0x051E
#define NANDHAL_ECC_ERROR_FIXED_REWRITE_FAILED 0x051F

// MMC Errors
#define MMC_MEDIA_ERROR_DEVICE_NOT_INSERTED 0x0600
#define MMC_MEDIA_ERROR_RESET_FAILED 0x0601
#define MMC_MEDIA_APP_COMMAND_FAILED 0x0602
#define MMC_MEDIA_ERROR_INIT_FAILED 0x0603
#define MMC_MEDIA_ERROR_SEND_OP_TIMEOUT 0x0604
#define MMC_MEDIA_READ_OCR_FAILED 0x0605
#define MMC_MEDIA_UNSUPPORTED_OCR_VOLTAGES 0x0606
#define MMC_MEDIA_READ_CSD_FAILED 0x0607
#define MMC_MEDIA_INVALID_CSD_VERSION 0x0608
#define MMC_MEDIA_READ_CID_FAILED 0x0609
#define MMC_MEDIA_INVALID_CID 0x060A
#define MMC_MEDIA_SPEC_VERSION_NOT_SUPPORTED 0x060B
#define MMC_MEDIA_ERROR_NOT_FORMATTED 0x060C
#define MMC_MEDIA_ERROR_NOT_ENUMERATED 0x060D

#define MMC_DATA_DRIVE_ERROR_WRITE_SECTOR_FAIL 0x0700
#define MMC_DATA_DRIVE_ERROR_INVALID_SECTOR 0x0701
#define MMC_DATA_DRIVE_ERROR_READ_SECTOR_FAIL 0x0702
#define MMC_DATA_DRIVE_ERROR_WRITE_PROTECTED 0x0703
#define MMC_DATA_DRIVE_ERROR_ERASE_FAILED 0x0704
#define MMC_DATA_DRIVE_ERROR_SEQUENTIAL_FIRST_FAIL 0x0705
#define MMC_DATA_DRIVE_ERROR_SEQUENTIAL_NEXT_FAIL 0x0706
#define MMC_DATA_DRIVE_ERROR_SEQUENTIAL_END_FAIL 0x0707
#define MMC_DATA_DRIVE_ERROR_BLOCK_ALIGN_FAIL 0x0708

/////////////////////////////////////////////////////////////////////////////////
//// MMC HAL Error codes
/////////////////////////////////////////////////////////////////////////////////
#define MMC_HAL_ERROR_PHYSICAL_DEVICE_BLOCKED 0x0800
#define MMC_HAL_ERROR_PHYSICAL_DEVICE_NOT_BLOCKED 0x0801
#define MMC_HAL_ERROR_SPI_BUS_BLOCKED 0x0802
#define MMC_HAL_ERROR_SPI_BUS_NOT_BLOCKED 0x0803
#define MMC_HAL_ERROR_SPI_DRIVER_INIT_FAILED 0x0804
#define MMC_HAL_ERROR_SPI_BUS_INIT_FAILED 0x0805

#define MMC_HAL_ERROR_NO_COMMAND_RESPONSE 0x0810
#define MMC_HAL_ERROR_BAD_START_BYTE 0x0811
#define MMC_HAL_ERROR_BAD_WRITE_STATUS 0x0812
#define MMC_HAL_ERROR_BAD_CSD_WRITE_STATUS 0x0813

#define MMC_HAL_ERROR_START_BYTE_TIMEOUT 0x0820
#define MMC_HAL_ERROR_WRITE_BUSY_TIMEOUT 0x0821
#define MMC_HAL_ERROR_CSD_WRITE_BUSY_TIMEOUT 0x0822
#define MMC_HAL_ERROR_ERASE_BUSY_TIMEOUT 0x0823
#define MMC_HAL_ERROR_REGISTER_READ_TIMEOUT 0x0824

#define MMC_HAL_ERROR_CMD_FAIL_CMD0 0x0830
#define MMC_HAL_ERROR_CMD_FAIL_CMD1 0x0831
#define MMC_HAL_ERROR_CMD_FAIL_CMD9 0x0832
#define MMC_HAL_ERROR_CMD_FAIL_CMD10 0x0833
#define MMC_HAL_ERROR_CMD_FAIL_CMD12 0x0834
#define MMC_HAL_ERROR_CMD_FAIL_CMD13 0x0835
#define MMC_HAL_ERROR_CMD_FAIL_CMD16 0x0836
#define MMC_HAL_ERROR_CMD_FAIL_CMD17 0x0837
#define MMC_HAL_ERROR_CMD_FAIL_CMD18 0x0838
#define MMC_HAL_ERROR_CMD_FAIL_CMD23 0x0839
#define MMC_HAL_ERROR_CMD_FAIL_CMD24 0x083A
#define MMC_HAL_ERROR_CMD_FAIL_CMD25 0x083B
#define MMC_HAL_ERROR_CMD_FAIL_CMD27 0x083C
#define MMC_HAL_ERROR_CMD_FAIL_CMD28 0x083D
#define MMC_HAL_ERROR_CMD_FAIL_CMD29 0x083E
#define MMC_HAL_ERROR_CMD_FAIL_CMD30 0x083F
#define MMC_HAL_ERROR_CMD_FAIL_CMD32 0x0840
#define MMC_HAL_ERROR_CMD_FAIL_CMD33 0x0841
#define MMC_HAL_ERROR_CMD_FAIL_CMD35 0x0842
#define MMC_HAL_ERROR_CMD_FAIL_CMD36 0x0843
#define MMC_HAL_ERROR_CMD_FAIL_CMD38 0x0844
#define MMC_HAL_ERROR_CMD_FAIL_CMD42 0x0845
#define MMC_HAL_ERROR_CMD_FAIL_CMD55 0x0846
#define MMC_HAL_ERROR_CMD_FAIL_CMD56 0x0847
#define MMC_HAL_ERROR_CMD_FAIL_CMD58 0x0848
#define MMC_HAL_ERROR_CMD_FAIL_CMD59 0x0849

#define MMC_HAL_ERROR_SD_CMD_FAIL_ACMD13 0x0850
#define MMC_HAL_ERROR_SD_CMD_FAIL_ACMD22 0x0851
#define MMC_HAL_ERROR_SD_CMD_FAIL_ACMD23 0x0852
#define MMC_HAL_ERROR_SD_CMD_FAIL_ACMD41 0x0853
#define MMC_HAL_ERROR_SD_CMD_FAIL_ACMD42 0x0854
#define MMC_HAL_ERROR_SD_CMD_FAIL_ACMD51 0x0855

#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD0 0x0860
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD1 0x0861
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD9 0x0862
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD10 0x0863
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD12 0x0864
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD13 0x0865
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD16 0x0866
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD17 0x0867
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD18 0x0868
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD23 0x0869
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD24 0x086A
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD25 0x086B
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD27 0x086C
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD28 0x086D
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD29 0x086E
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD30 0x086F
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD32 0x0870
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD33 0x0871
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD35 0x0872
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD36 0x0873
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD38 0x0874
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD42 0x0875
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD55 0x0876
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD56 0x0877
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD58 0x0878
#define MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD59 0x0879

#define MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD13 0x0880
#define MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD22 0x0881
#define MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD23 0x0882
#define MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD41 0x0883
#define MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD42 0x0884
#define MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD51 0x0885

//;;;;;;; < Library Error Codes > ;;;;;;;;
//MATH errors...
#define ERRZeroDivide 0x7FFFFF

//Memory Pointer errors...
#define INVALID_MEMORY_SPACE 0x900000
//****************************************************************/
#if (!defined(NOERROR))
#define NOERROR 0
#endif

////Added by RAKS

#define ERRProgram 0x911111
#define ERRNoMedia 0x922222
#define ERRHwFail 0x933333   
#define ERRInvMedia 0x944444
#define ERRBlkFail 0x955555
#define ERRInvDataStatus 0x966666
#define ERRWriteFail 0x977777
#define ERRECCCorr 0x988888
#define ERRECCFail 0x999999
#define ERRZoneFull 0x9aaaaa
#define ERR_PAGE_UNASSIGNED 0x9aaaaB   
// Partition Table Error Codes.                 Note 9AAAAA used above and below.
//invalid partition table signature. 
#define ERRPartTableSignature 0x9AAAAA   
//boot ID bad
#define ERRBootID 0x9BBBBB   
#define ERRBootTableSignature 0x9CCCCC   
////Nothing was here : RAKS
//JUMP instruction bad
#define ERRBootJump 0x9DDDDD  
//ID Byte bad
#define ERRIDByte 0x9EEEEE  


//****************************************************************
//        < < <  EMC5600 FAT12 FileSystem  > > >
//****************************************************************
// Master ERROR File
//****************************************************************/
// FAT File System Error Codes
// corrupt FAT
#define FATCorrupt -1        
// invalid FAT sector
#define DIRInvlaidSector -2        

#define FATwarningFreeCx 0x0AFCFC
#define FATwarningEndOfChain 0x0AECEC
#define FATwarningReservedCx 0x0A1111
#define FATwarningBadCx 0x0ABCBC       
//FATcxsFree==0 
#define FATwarningNoFreeCxs 0x0A0F0F   
#define FATwarningBoundaryCx 0x0A2222
#define FATwarningWriteBackAborted 0x0A3333

#define FATfailedReadPrimaryFAT 0x7F0000
#define FATfailedWritePrimaryFAT 0x7FFFFF   
#define FATfailedWriteSecondaryFAT 0x7F8888

#define FATerrorNoFreeBlocks 0x80A0A0
#define FATerrorTableOverFlow 0x80CCCC
#define FATerrorClusterUnderFlow 0x80DDDD
#define FATerrorClusterOverFlow 0x80EEEE
//No Free Cxs on FAT Scan!
#define FATerrorNoFreeCxs 0x80FFFF   

#define MATHerrorNotPowerOf2 0x802222

#define MEDIAerrorFSNotSupported 0x810000
#define MEDIAerrorMediaNotUsable 0x811111

#define CXerrorValueOverFlow 0x82AAAA
#define CXerrorValueUnderFlow 0x82BBBB

#define HNDLerrorNoHandles 0x83AAAA
#define HNDLerrorInvalidHandle 0x83BBBB
#define HNDLerrorInvalidDevice 0x830D0D
#define HNDLerrorIndexSect 0x83CCCC

#define FCBwarningEndOfFile 0x84EFEF
#define FCBerrorCXtoSect 0x84CCCC
#define FCBerrorDiskFull 0x84DFDF

#define SEEKprefix 0x85
#define SEEKerrorAccessDenied 0x85ADAD
#define SEEKerrorPassedBeginingOfFile 0x85BFBF
#define SEEKerrorCurruptFCB 0x85CCCC
#define SEEKerrorPassedEndOfFile 0x85EFEF

#define READprefix 0x86
#define READerrorMemoryOverFlow 0x860F0F
#define READerrorInvalidBufferPntr 0x86BFBF
#define READerrorInvalidByteCount 0x86BCBC
#define READerrorAccessDenied 0x86ADAD
#define READerrorFreeCx 0x860000
#define READerrorEndOfChain 0x861111
#define READerrorReservedCx 0x862222
#define READerrorBadCx 0x863333

#define WRITEprefix 0x87
#define WRITEerrorMemoryOverFlow 0x870F0F
#define WRITEerrorInvalidBufferPntr 0x87BFBF
#define WRITEerrorInvalidByteCount 0x87BCBC
#define WRITEerrorAccessDenied 0x87ADAD
#define WRITEerrorFreeCx 0x870000
#define WRITEerrorEndOfChain 0x871111
#define WRITEerrorReservedCx 0x872222
#define WRITEerrorBadCx 0x873333

#define UTILprefix 0x88
#define UTILerrorDeviceInvalid 0x88BDBD

// Directory Error Codes

// invalid DIR sector specified
#define ERR_DIRInvalidSector 0xA00000  
// PATH search string has invlaid directory
#define ERR_DIRInvalidPath 0xA00001  
#define ERR_DIR_ALREADY_EXISTS 0xA00002
#define ERR_DIR_WRITE_FAILED 0xA00003
#define ERR_DIR_NOT_EMPTY 0xA00003
// Bad cluster number
#define ERR_CLUSTER_OUT_OF_RANGE 0xA11111  
// Can't open ROOT
#define ERR_OPEN_ROOT 0xA22222  
#define ERR_CLUSTER_FLAGGED_BAD 0xA33333 
#define ERR_CLUSTER_UNREADABLE 0xA44444 
#define ERR_SECTOR_UNREADABLE 0xA44441 
// bad record number to read
#define ERR_INVLAID_DIR_REQUEST 0xA55555  
// record requeste not in range
#define ERR_ReadRecordRangeError 0xA66666  
// record requeste past end of ROOT
#define ERR_ReadPastEndOfROOT 0xA66660  
// device number invalid
#define ERR_BadDeviceNumber 0xA77777  
// FCB Does not popint to a sub-Directory entry
#define ERR_FCB_Not_SubDir 0xA88888  
// Path specified is invalid
#define ERR_INVLAID_PATH 0xA99999  
#define ERR_FILE_NOT_FOUND 0xAAAAAA
#define ERR_FILE_EXISTS 0xABBBBB
#define ERR_DISK_FULL 0xACCCCC
#define ERR_DIR_DEVICE_NOT_READY 0xACCCC0
#define ERR_CX_ALLOCATION 0xADDDDD
#define ERR_INVALID_MODE 0xAEEEEE

#define ERR_DUPLICATE_FILE_NAME 0xAFFFFC
#define ERR_FAT_WRITE_FAILED 0xAFFFFD
#define ERR_CANT_DELETE_CHAIN 0xAFFFFE
#define ERR_FAT_CHAIN_CORRUPT 0xAFFFFF
// String Manipulation Error Codes
#define ERR_END_OF_STRING 0xABBBB0

#endif //@DEF(ERRORS_INC)

