;///////////////////////////////////////////////////////////////////////////////
;// Copyright(C) SigmaTel, Inc. 2003
;//
;// Filename: errors.inc
;// Description: 
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(ERRORS_INC))
ERRORS_INC equ     1

LDRIVE_ERROR_INVALID_DRIVE_NUMBER                               equ     $0101
LDRIVE_ERROR_NOT_INITIALIZED                                    equ     $0102
LDRIVE_ERROR_HARDWARE_FAILURE                                   equ     $0103
LDRIVE_ERROR_INVALID_DRIVE_TYPE                                 equ     $0104
LDRIVE_ERROR_INVALID_INFO_TYPE                                  equ     $0105
LDRIVE_ERROR_SECTOR_OUT_OF_BOUNDS                               equ     $0106
LDRIVE_ERROR_WRITE_FAILURE                                      equ     $0107
LDRIVE_ERROR_WRITE_PROTECTED                                    equ     $0108
LDRIVE_ERROR_MULTI_WRITE_IN_PROGRESS                            equ     $0109
LDRIVE_ERROR_MUST_HAVE_MORE_THAN_ONE_SECTOR                     equ     $010A
LDRIVE_ERROR_MULTI_WRITE_NOT_SETUP                              equ     $010B
; The WRITE_ABORT Error is returned when the system drive detects a bad
; block in the device. The block was marked bad and tables updated.
; The application must start over.
LDRIVE_ERROR_WRITE_ABORT                                        equ     $010C
LDRIVE_ERROR_READ_FAILURE                                       equ     $010D
LDRIVE_ERROR_MEDIA_NOT_ALLOCATED                                equ     $010E
LDRIVE_ERROR_LOW_LEVEL_MEDIA_FORMAT_REQUIRED                    equ     $010F

LMEDIA_ERROR_HARDWARE_FAILURE                                   equ     $0200
LMEDIA_ERROR_INVALID_MEDIA_NUMBER                               equ     $0201
LMEDIA_ERROR_MEDIA_NOT_INITIALIZED                              equ     $0202
LMEDIA_ERROR_MEDIA_NOT_DISCOVERED                               equ     $0203
LMEDIA_ERROR_INVALID_MEDIA_INFO_TYPE                            equ     $0204
LMEDIA_ERROR_ALLOCATION_TO_LARGE                                equ     $0205
LMEDIA_ERROR_MEDIA_NOT_ERASED                                   equ     $0206
LMEDIA_ERROR_MEDIA_ERASED                                       equ     $0207
;Specific to NAND
LMEDIA_ERROR_NO_REGIONS_IN_MEDIA                                equ     $0208     
LMEDIA_ERROR_DRIVES_MAX_OUT                                     equ     $0209 
LMEDIA_ERROR_MEDIA_WRITE_PROTECTED                              equ     $020A
LMEDIA_ERROR_INVALID_INFO_TYPE                                  equ     $020B
LMEDIA_ERROR_BAD_BLOCKS_MAX_OUT                                 equ     $020C
LMEDIA_ERROR_NOT_ALLOCATED                                      equ     $020D
                                                                           
LMEDIA_ERROR_DRIVE_TYPE_NOT_SUPPORTED                           equ     $0210
                                                                           
NAND_ERROR_WRITE_PHYS_SECTOR_PROGRAM_FAILED                     equ     $0301
ERROR_MULTI_WRITE_SECTOR_FIFO_OVERFLOW                          equ     $0302
ERROR_MULTI_WRITE_SECTOR_FIFO_UNDERFLOW                         equ     $0303

NAND_DATA_DRIVE_ERROR_LOGICAL_SECTOR_OUT_OF_BOUNDS              equ     $0401
NAND_DATA_DRIVE_ERROR_RBLOCK_NOT_FOUND                          equ     $0402
NAND_DATA_DRIVE_ERROR_NO_FREE_RBLOCKS                           equ     $0403
NAND_DATA_DRIVE_ERROR_USECTOR_LOOKUP_INTERNAL_1                 equ     $0404
NAND_DATA_DRIVE_SECTOR_NOT_UPDATED                              equ     $0405
NAND_DATA_DRIVE_ERROR_CANT_RECYCLE_USECTOR_MAP                  equ     $0406
NAND_DATA_DRIVE_ERROR_UBLOCK_LBAS_INCONSISTENT                  equ     $0407
NAND_DATA_DRIVE_ERROR_HSECTORIDX_IN_UBLOCK_OUT_OF_RANGE         equ     $0408
NAND_DATA_DRIVE_ERROR_CANT_RECYCLE_UBLOCK                       equ     $0409
NAND_DATA_DRIVE_ERROR_BBTABLE_FULL                              equ     $040A
NAND_DATA_DRIVE_ERROR_UPDATE_NOT_OPEN                           equ     $040B
NAND_DATA_DRIVE_ERROR_ADD_USECTOR_INTERNAL_1                    equ     $040C
NAND_DATA_DRIVE_ERROR_CANT_GET_ERASED_UBLOCK                    equ     $040D
NAND_DATA_DRIVE_ERROR_NO_ERASED_UBLOCKS                         equ     $040E
NAND_DATA_DRIVE_ERROR_CANT_ERASE_FREE_UBLOCK                    equ     $040F
NAND_DATA_DRIVE_ERROR_KILLUSECTOR_INTERNAL_1                    equ     $0410
NAND_DATA_DRIVE_ERROR_KILLUSECTOR_INTERNAL_2                    equ     $0411
NAND_DATA_DRIVE_RETURN_BLOCK_NOT_UPDATED                        equ     $0412
NAND_DATA_DRIVE_ERROR_UBLOCK_PROTECT_TABLE_FULL                 equ     $0413
NAND_DATA_DRIVE_ERROR_UBLOCK_ALREADY_PROTECTED                  equ     $0414
NAND_DATA_DRIVE_ERROR_UBLOCK_NOT_PROTECTED                      equ     $0415
NAND_DATA_DRIVE_ERROR_UBLOCKPROTECT_INTERNAL_1                  equ     $0416
NAND_DATA_DRIVE_ERROR_RECOVERUBLOCK_INTERNAL_1                  equ     $0417
NAND_DATA_DRIVE_ERROR_RECOVERUBLOCK_INTERNAL_2                  equ     $0418
NAND_DATA_DRIVE_ERROR_UBLOCK_NOT_IN_UTABLE                      equ     $0419
NAND_DATA_DRIVE_ERROR_CANT_ALLOCATE_USECTORS_MAPS               equ     $041A
NAND_DATA_DRIVE_ERROR_CANT_INIT_DATA_REGIONS_LIST               equ     $041B
NAND_DATA_DRIVE_ERROR_TOO_MANY_UBLOCKS_IN_CONFIG                equ     $041C
NAND_DATA_DRIVE_ERROR_USECTOR_INDEX_IS_NOT_NEXT                 equ     $041D
NAND_DATA_DRIVE_ERROR_USECTOR_ALREADY_UPDATED                   equ     $041E
NAND_DATA_DRIVE_ERROR_BgGC_USECTOR_ERASE_TIMEDOUT               equ     $041F
NAND_DATA_DRIVE_ERROR_BgGC_HSECTOR_COPY_TIMEDOUT                equ     $0420
NAND_DATA_DRIVE_ERROR_BgGC_ALREADY_ENABLED                      equ     $0421
NAND_DATA_DRIVE_ERROR_BgGC_HSECTOR_ERASE_TIMEDOUT               equ     $0422
NAND_DATA_DRIVE_ERROR_BgGC_SECTOR_NOT_UPDATED                   equ     $0423
NAND_DATA_DRIVE_ERROR_NO_STALE_UBLOCKS                          equ     $0424
NAND_DATA_DRIVE_ERROR_NAND_IS_READY_TIMEOUT                     equ     $0425
NAND_DATA_DRIVE_ERROR_CANT_CLOSE_UPDATE                         equ     $0426

NAND_DATA_DRIVE_ERROR_CMW_S0_FETCH_FAILED                       equ     $042C
NAND_DATA_DRIVE_ERROR_CMW_S1_FETCH_FAILED                       equ     $042D
NAND_DATA_DRIVE_ERROR_CMW_S2_1_FETCH_FAILED                     equ     $042E
NAND_DATA_DRIVE_ERROR_CMW_S2_2_FETCH_FAILED                     equ     $042F
NAND_DATA_DRIVE_ERROR_CMW_S3_1_FETCH_FAILED                     equ     $0430
NAND_DATA_DRIVE_ERROR_CMW_S3_2_FETCH_FAILED                     equ     $0431
NAND_DATA_DRIVE_ERROR_CMW_S4_1_FETCH_FAILED                     equ     $0432
NAND_DATA_DRIVE_ERROR_CMW_S4_2_FETCH_FAILED                     equ     $0433
NAND_DATA_DRIVE_ERROR_CMW_S5_FETCH_FAILED                       equ     $0434
NAND_DATA_DRIVE_ERROR_CMW_S0_ECC_TIMEOUT                        equ     $0435
NAND_DATA_DRIVE_ERROR_CMW_S2_ECC_TIMEOUT                        equ     $0436
NAND_DATA_DRIVE_ERROR_CMW_S3_ECC_TIMEOUT                        equ     $0437
NAND_DATA_DRIVE_ERROR_CMW_S4_ECC_TIMEOUT                        equ     $0438
NAND_DATA_DRIVE_ERROR_CMW_S5_ECC_TIMEOUT                        equ     $0439
NAND_DATA_DRIVE_ERROR_CMW_WRITE_FAILED                          equ     $043A
NAND_DATA_DRIVE_ERROR_CMW_RELEASE_FAILED                        equ     $043B

NAND_DATA_DRIVE_ERROR_INVALID_LOGICAL_SECTOR                    equ     $0427
NAND_DATA_DRIVE_ERROR_INVALID_RELATIVE_SECTOR                   equ     $0428
NAND_DATA_DRIVE_ERROR_UBLOCK_IS_EMPTY                           equ     $0429
NAND_DATA_DRIVE_ERROR_USECTOR_MAP_IS_BAD                        equ     $042A
NAND_DATA_DRIVE_ERROR_TOO_MANY_BLOCK_RECOVERIES                 equ     $042B

; indicates that during the NandHalInit, the NandType did not
; match the previously initialized Nand chip type.
NANDHAL_ERROR_NANDTYPE_MISMATCH                                 equ     $0500 
NANDHAL_ERROR_LOOKUP_ID_FAILED                                  equ     $0501
; Errors from calling HIL functions
NANDHAL_ERROR_INIT_PORT                                         equ     $0502
NANDHAL_ERROR_WRITE_PORT_CMD                                    equ     $0503
NANDHAL_ERROR_WRITE_PORT_ADDR                                   equ     $0504
NANDHAL_ERROR_READ_PORT_DATA                                    equ     $0505
NANDHAL_ERROR_WAIT_FOR_READY_PORT                               equ     $0506
NANDHAL_ERROR_POWER_UP_FLASH_PADS                               equ     $0507
NANDHAL_ERROR_TERMINATE_PORT                                    equ     $0508
NANDHAL_ERROR_LOCKPORT_TIMEOUT                                  equ     $0509
NANDHAL_ERROR_LOCKNAND_TIMEOUT                                  equ     $050A
; for 3410 build
NANDHAL_ERROR_LOCKPORT_LOCKED                                   equ     $050B
  ; for 3410 build
NANDHAL_ERROR_LOCKNAND_LOCKED                                   equ     $050C
NANDHAL_ERROR_WRITE_DATA_PORT                                   equ     $050D

; indicates that the GetStatus returned a FAIL
NANDHAL_ERROR_GETSTATUS_FAILED                                  equ     $050E
NANDHAL_ERROR_WRITE_FAILED                                      equ     $050F
NANDHAL_ERROR_READ_FAILED                                       equ     $0510
NANDHAL_ERROR_READID1_FAILED                                    equ     $0511
NANDHAL_ERROR_READID2_FAILED                                    equ     $0512
NANDHAL_ERROR_READIDEXT_FAILED                                  equ     $0513
; indicates the NAND was already BUSY
NANDHAL_ERROR_SETNANDBUSY_FAILED                                equ     $0514
NANDHAL_ERROR_ASYNCWAIT_CALLBACK_ERR                            equ     $0515
; ECC Error Codes
; ECC Computation has successfully completed
COMPUTE_ECC_SUCCESS                                             equ     0         
; ECC computation still busy
COMPUTE_ECC_NOT_DONE                                            equ     $0516
; timed out while waiting on object
NANDHAL_ERROR_LOCKECC_TIMEOUT                                   equ     $0517
; flag already set for SDK2.4 lock
NANDHAL_ERROR_LOCKECC_LOCKED                                    equ     $0518

NANDHAL_ECC_NOERROR                                             equ     0
; SKIP 0519-051B for space optimization in ECC calculate functions
NANDHAL_ECC_ERROR_FIXED                                         equ     $051C
NANDHAL_ECC_ERROR_FIXED_REWRITE_SECTOR                          equ     $051D
NANDHAL_ECC_FIX_FAILED                                          equ     $051E
NANDHAL_ECC_ERROR_FIXED_REWRITE_FAILED                          equ     $051F

; MMC Errors
MMC_MEDIA_ERROR_DEVICE_NOT_INSERTED                             equ     $0600
MMC_MEDIA_ERROR_RESET_FAILED                                    equ     $0601
MMC_MEDIA_APP_COMMAND_FAILED                                    equ     $0602
MMC_MEDIA_ERROR_INIT_FAILED                                     equ     $0603
MMC_MEDIA_ERROR_SEND_OP_TIMEOUT                                 equ     $0604
MMC_MEDIA_READ_OCR_FAILED                                       equ     $0605
MMC_MEDIA_UNSUPPORTED_OCR_VOLTAGES                              equ     $0606
MMC_MEDIA_READ_CSD_FAILED                                       equ     $0607
MMC_MEDIA_INVALID_CSD_VERSION                                   equ     $0608
MMC_MEDIA_READ_CID_FAILED                                       equ     $0609
MMC_MEDIA_INVALID_CID                                           equ     $060A
MMC_MEDIA_SPEC_VERSION_NOT_SUPPORTED                            equ     $060B
MMC_MEDIA_ERROR_NOT_FORMATTED                                   equ     $060C
MMC_MEDIA_ERROR_NOT_ENUMERATED                                  equ     $060D

MMC_DATA_DRIVE_ERROR_WRITE_SECTOR_FAIL                          equ     $0700
MMC_DATA_DRIVE_ERROR_INVALID_SECTOR                             equ     $0701
MMC_DATA_DRIVE_ERROR_READ_SECTOR_FAIL                           equ     $0702
MMC_DATA_DRIVE_ERROR_WRITE_PROTECTED                            equ     $0703
MMC_DATA_DRIVE_ERROR_ERASE_FAILED                               equ     $0704
MMC_DATA_DRIVE_ERROR_SEQUENTIAL_FIRST_FAIL                      equ     $0705
MMC_DATA_DRIVE_ERROR_SEQUENTIAL_NEXT_FAIL                       equ     $0706
MMC_DATA_DRIVE_ERROR_SEQUENTIAL_END_FAIL						equ		$0707
MMC_DATA_DRIVE_ERROR_BLOCK_ALIGN_FAIL                        	equ		$0708

;///////////////////////////////////////////////////////////////////////////////
;// MMC HAL Error codes
;///////////////////////////////////////////////////////////////////////////////
MMC_HAL_ERROR_PHYSICAL_DEVICE_BLOCKED                           equ     $0800
MMC_HAL_ERROR_PHYSICAL_DEVICE_NOT_BLOCKED                       equ     $0801
MMC_HAL_ERROR_SPI_BUS_BLOCKED                                   equ     $0802
MMC_HAL_ERROR_SPI_BUS_NOT_BLOCKED                               equ     $0803
MMC_HAL_ERROR_SPI_DRIVER_INIT_FAILED                            equ     $0804
MMC_HAL_ERROR_SPI_BUS_INIT_FAILED                               equ     $0805

MMC_HAL_ERROR_NO_COMMAND_RESPONSE                               equ     $0810
MMC_HAL_ERROR_BAD_START_BYTE                                    equ     $0811
MMC_HAL_ERROR_BAD_WRITE_STATUS                                  equ     $0812
MMC_HAL_ERROR_BAD_CSD_WRITE_STATUS                              equ     $0813

MMC_HAL_ERROR_START_BYTE_TIMEOUT                                equ     $0820
MMC_HAL_ERROR_WRITE_BUSY_TIMEOUT                                equ     $0821
MMC_HAL_ERROR_CSD_WRITE_BUSY_TIMEOUT                            equ     $0822
MMC_HAL_ERROR_ERASE_BUSY_TIMEOUT                                equ     $0823
MMC_HAL_ERROR_REGISTER_READ_TIMEOUT                             equ     $0824

MMC_HAL_ERROR_CMD_FAIL_CMD0                                     equ     $0830
MMC_HAL_ERROR_CMD_FAIL_CMD1                                     equ     $0831
MMC_HAL_ERROR_CMD_FAIL_CMD9                                     equ     $0832
MMC_HAL_ERROR_CMD_FAIL_CMD10                                    equ     $0833
MMC_HAL_ERROR_CMD_FAIL_CMD12                                    equ     $0834
MMC_HAL_ERROR_CMD_FAIL_CMD13                                    equ     $0835
MMC_HAL_ERROR_CMD_FAIL_CMD16                                    equ     $0836
MMC_HAL_ERROR_CMD_FAIL_CMD17                                    equ     $0837
MMC_HAL_ERROR_CMD_FAIL_CMD18                                    equ     $0838
MMC_HAL_ERROR_CMD_FAIL_CMD23                                    equ     $0839
MMC_HAL_ERROR_CMD_FAIL_CMD24                                    equ     $083A
MMC_HAL_ERROR_CMD_FAIL_CMD25                                    equ     $083B
MMC_HAL_ERROR_CMD_FAIL_CMD27                                    equ     $083C
MMC_HAL_ERROR_CMD_FAIL_CMD28                                    equ     $083D
MMC_HAL_ERROR_CMD_FAIL_CMD29                                    equ     $083E
MMC_HAL_ERROR_CMD_FAIL_CMD30                                    equ     $083F
MMC_HAL_ERROR_CMD_FAIL_CMD32                                    equ     $0840
MMC_HAL_ERROR_CMD_FAIL_CMD33                                    equ     $0841
MMC_HAL_ERROR_CMD_FAIL_CMD35                                    equ     $0842
MMC_HAL_ERROR_CMD_FAIL_CMD36                                    equ     $0843
MMC_HAL_ERROR_CMD_FAIL_CMD38                                    equ     $0844
MMC_HAL_ERROR_CMD_FAIL_CMD42                                    equ     $0845
MMC_HAL_ERROR_CMD_FAIL_CMD55                                    equ     $0846
MMC_HAL_ERROR_CMD_FAIL_CMD56                                    equ     $0847
MMC_HAL_ERROR_CMD_FAIL_CMD58                                    equ     $0848
MMC_HAL_ERROR_CMD_FAIL_CMD59                                    equ     $0849

MMC_HAL_ERROR_SD_CMD_FAIL_ACMD13                                equ     $0850
MMC_HAL_ERROR_SD_CMD_FAIL_ACMD22                                equ     $0851
MMC_HAL_ERROR_SD_CMD_FAIL_ACMD23                                equ     $0852
MMC_HAL_ERROR_SD_CMD_FAIL_ACMD41                                equ     $0853
MMC_HAL_ERROR_SD_CMD_FAIL_ACMD42                                equ     $0854
MMC_HAL_ERROR_SD_CMD_FAIL_ACMD51                                equ     $0855

MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD0                             equ     $0860
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD1                             equ     $0861
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD9                             equ     $0862
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD10                            equ     $0863
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD12                            equ     $0864
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD13                            equ     $0865
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD16                            equ     $0866
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD17                            equ     $0867
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD18                            equ     $0868
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD23                            equ     $0869
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD24                            equ     $086A
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD25                            equ     $086B
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD27                            equ     $086C
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD28                            equ     $086D
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD29                            equ     $086E
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD30                            equ     $086F
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD32                            equ     $0870
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD33                            equ     $0871
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD35                            equ     $0872
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD36                            equ     $0873
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD38                            equ     $0874
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD42                            equ     $0875
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD55                            equ     $0876
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD56                            equ     $0877
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD58                            equ     $0878
MMC_HAL_ERROR_BAD_CMD_RESPONSE_CMD59                            equ     $0879

MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD13                        equ     $0880
MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD22                        equ     $0881
MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD23                        equ     $0882
MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD41                        equ     $0883
MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD42                        equ     $0884
MMC_HAL_ERROR_SD_BAD_CMD_RESPONSE_ACMD51                        equ     $0885

;;;;;;;; < Library Error Codes > ;;;;;;;;
;MATH errors...
ERRZeroDivide                                                   equ    $7FFFFF

;Memory Pointer errors...
INVALID_MEMORY_SPACE                                            equ    $900000
;****************************************************************/
;
    IF (!@def(NOERROR))
NOERROR                                                         equ     0
    ENDIF

;//Added by RAKS

ERRProgram                                                      equ    $911111
ERRNoMedia                                                      equ    $922222
ERRHwFail                                                       equ    $933333   
ERRInvMedia                                                     equ    $944444
ERRBlkFail                                                      equ    $955555
ERRInvDataStatus                                                equ    $966666
ERRWriteFail                                                    equ    $977777
ERRECCCorr                                                      equ    $988888
ERRECCFail                                                      equ    $999999
ERRZoneFull                                                     equ    $9aaaaa
ERR_PAGE_UNASSIGNED                                             equ    $9aaaaB   
;
; Partition Table Error Codes.                 Note 9AAAAA used above and below.
ERRPartTableSignature                                           equ    $9AAAAA   ;invalid partition table signature. 
ERRBootID                                                       equ    $9BBBBB   ;boot ID bad
ERRBootTableSignature                                           equ    $9CCCCC   
   ;//Nothing was here : RAKS
ERRBootJump                                                     equ    $9DDDDD  ;JUMP instruction bad
ERRIDByte                                                       equ    $9EEEEE  ;ID Byte bad


;****************************************************************
;        < < <  EMC5600 FAT12 FileSystem  > > >
;****************************************************************
; Master ERROR File
;****************************************************************/
;
;
; FAT File System Error Codes
FATCorrupt                                                      equ     -1        ; corrupt FAT
DIRInvlaidSector                                                equ     -2        ; invalid FAT sector
                                
FATwarningFreeCx                                                equ    $0AFCFC
FATwarningEndOfChain                                            equ    $0AECEC
FATwarningReservedCx                                            equ    $0A1111
FATwarningBadCx                                                 equ    $0ABCBC       
FATwarningNoFreeCxs                                             equ    $0A0F0F   ;FATcxsFree==0 
FATwarningBoundaryCx                                            equ    $0A2222
FATwarningWriteBackAborted                                      equ    $0A3333
                                
FATfailedReadPrimaryFAT                                         equ    $7F0000
FATfailedWritePrimaryFAT                                        equ    $7FFFFF   
FATfailedWriteSecondaryFAT                                      equ    $7F8888
                                
FATerrorNoFreeBlocks                                            equ    $80A0A0
FATerrorTableOverFlow                                           equ    $80CCCC
FATerrorClusterUnderFlow                                        equ    $80DDDD
FATerrorClusterOverFlow                                         equ    $80EEEE
FATerrorNoFreeCxs                                               equ    $80FFFF   ;No Free Cxs on FAT Scan!
                                
MATHerrorNotPowerOf2                                            equ    $802222
                                
MEDIAerrorFSNotSupported                                        equ    $810000
MEDIAerrorMediaNotUsable                                        equ    $811111
                                
CXerrorValueOverFlow                                            equ    $82AAAA
CXerrorValueUnderFlow                                           equ    $82BBBB
                                
HNDLerrorNoHandles                                              equ    $83AAAA
HNDLerrorInvalidHandle                                          equ    $83BBBB
HNDLerrorInvalidDevice                                          equ    $830D0D
HNDLerrorIndexSect                                              equ    $83CCCC
                                
FCBwarningEndOfFile                                             equ    $84EFEF
FCBerrorCXtoSect                                                equ    $84CCCC
FCBerrorDiskFull                                                equ    $84DFDF

SEEKprefix                                                      equ    $85
SEEKerrorAccessDenied                                           equ    $85ADAD
SEEKerrorPassedBeginingOfFile                                   equ    $85BFBF
SEEKerrorCurruptFCB                                             equ    $85CCCC
SEEKerrorPassedEndOfFile                                        equ    $85EFEF

READprefix                                                      equ    $86
READerrorMemoryOverFlow                                         equ    $860F0F
READerrorInvalidBufferPntr                                      equ    $86BFBF
READerrorInvalidByteCount                                       equ    $86BCBC
READerrorAccessDenied                                           equ    $86ADAD
READerrorFreeCx                                                 equ    $860000
READerrorEndOfChain                                             equ    $861111
READerrorReservedCx                                             equ    $862222
READerrorBadCx                                                  equ    $863333

WRITEprefix                                                     equ    $87
WRITEerrorMemoryOverFlow                                        equ    $870F0F
WRITEerrorInvalidBufferPntr                                     equ    $87BFBF
WRITEerrorInvalidByteCount                                      equ    $87BCBC
WRITEerrorAccessDenied                                          equ    $87ADAD
WRITEerrorFreeCx                                                equ    $870000
WRITEerrorEndOfChain                                            equ    $871111
WRITEerrorReservedCx                                            equ    $872222
WRITEerrorBadCx                                                 equ    $873333

UTILprefix                                                      equ    $88
UTILerrorDeviceInvalid                                          equ    $88BDBD
                                                        
;
; Directory Error Codes

ERR_DIRInvalidSector                                            equ    $A00000  ; invalid DIR sector specified
ERR_DIRInvalidPath                                              equ    $A00001  ; PATH search string has invlaid directory
ERR_DIR_ALREADY_EXISTS                                          equ    $A00002
ERR_DIR_WRITE_FAILED                                            equ    $A00003
ERR_DIR_NOT_EMPTY                                               equ    $A00003
ERR_CLUSTER_OUT_OF_RANGE                                        equ    $A11111  ; Bad cluster number
ERR_OPEN_ROOT                                                   equ    $A22222  ; Can't open ROOT
ERR_CLUSTER_FLAGGED_BAD                                         equ    $A33333 
ERR_CLUSTER_UNREADABLE                                          equ    $A44444 
ERR_SECTOR_UNREADABLE                                           equ    $A44441 
ERR_INVLAID_DIR_REQUEST                                         equ    $A55555  ; bad record number to read
ERR_ReadRecordRangeError                                        equ    $A66666  ; record requeste not in range
ERR_ReadPastEndOfROOT                                           equ    $A66660  ; record requeste past end of ROOT
ERR_BadDeviceNumber                                             equ    $A77777  ; device number invalid
ERR_FCB_Not_SubDir                                              equ    $A88888  ; FCB Does not popint to a sub-Directory entry
ERR_INVLAID_PATH                                                equ    $A99999  ; Path specified is invalid
ERR_FILE_NOT_FOUND                                              equ    $AAAAAA
ERR_FILE_EXISTS                                                 equ    $ABBBBB
ERR_DISK_FULL                                                   equ    $ACCCCC
ERR_DIR_DEVICE_NOT_READY                                        equ    $ACCCC0
ERR_CX_ALLOCATION                                               equ    $ADDDDD
ERR_INVALID_MODE                                                equ    $AEEEEE

ERR_DUPLICATE_FILE_NAME                                         equ    $AFFFFC
ERR_FAT_WRITE_FAILED                                            equ    $AFFFFD
ERR_CANT_DELETE_CHAIN                                           equ    $AFFFFE
ERR_FAT_CHAIN_CORRUPT                                           equ    $AFFFFF
;
; String Manipulation Error Codes
ERR_END_OF_STRING                                               equ    $ABBBB0

    ENDIF ;@DEF(ERRORS_INC)
