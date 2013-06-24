 IF !@DEF('smdddefs_h')
   DEFINE   smdddefs_h	'1'
;/****************************************************************
;  08/01/00                                        By:  HHL, RAKS
;****************************************************************
;        < < <  EMC5600 Device Driver Project  > > >
;****************************************************************
; Main INCLUDE file for the EMC5600 Device Driver Project
;****************************************************************/
;

    include "regsemc.inc"

;///////////////////////////////////////////////////////////////////////////
;//////////////////ENABLE DISABLE VARIOUS FUNCTIONLITY HERE/////////////////
;///////////////////////////////////////////////////////////////////////////

;OFF  DEFINE   SUP_1MB              '1' ;//If 1MB is supported
;OFF  DEFINE   SUP_2MB              '1' ;//If 2MB is supported
;OFF  DEFINE   SUP_4MB              '1' ;//If 4MB is supported
      DEFINE   SUP_8MB              '1' ;//If 8MB is supported
      DEFINE   SUP_16MB             '1' ;//If 16MB is supported
      DEFINE   SUP_32MB             '1' ;//If 32MB is supported
      DEFINE   SUP_64MB             '1' ;//If 64MB is supported

 if (@DEF('FAT16'))           
      DEFINE   SUP_128MB            '1' ;//If 128MB is supported
 endif
 
;OFF  DEFINE   SUP_SMALL_MEDIA      '1'
                                        ;//Relocates a block in case
                                        ;//an ECC error is found

        ;If you plan to use the write functions, you must have the following defines
 if (!@DEF('SMNOECCW'))
        DEFINE  ENABLE_ECC  '1'         ;Enables ECC Functionality by default
 endif
 if (!@DEF('SMNOECCR'))
        DEFINE  CHECK_ECC_ON_READ   '1'
 endif

;///////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////

;/****************************************************************
; Definition of various EMC5600(DMA Register) offsets
;***************************************************************/
    IF !@DEF('EMC5600Defs')
      DEFINE   EMC5600Defs       '1'

      DEFINE   EMC5600MMAP       'x'         ;//Assuming that the DMA is
                                             ;//memory mapped to x memory
                
EMC5600_BASE      equ     HW_EMC_BASEADDR      ;//The base address of the
                                                            ;//EMC5600_BASE memory map
                                             ;//offsets to various registers
                                             ;//in this area are given below
      DEFINE   EMC5600_PIO_CTRL     "EMC5600_BASE+$0"       ;//Control
      DEFINE   EMC5600_PIO_SA_LOW   "EMC5600_BASE+$1"       ;//Start Address Low
      DEFINE   EMC5600_PIO_SA_HIGH  "EMC5600_BASE+$2"       ;//Start Address High
      DEFINE   EMC5600_PIO_DBG_FORCE "EMC5600_BASE+$3"      ;//Debug force
      DEFINE   EMC5600_PIO_DBG_OBS1 "EMC5600_BASE+$4"       ;//Debug Observe
      DEFINE   EMC5600_PIO_SEM      "EMC5600_BASE+$5"       ;//Semaphore
      DEFINE   EMC5600_PIO_DBG_OBS2 "EMC5600_BASE+$6"       ;//Debug Observe2
                                                            ;//Note: $7 is reserved
                                                            ;//Note: $8 to $f are used by
                                                            ;//compact flash registers
                                                            ;//The offset of the SmartMedia
                                                            ;//registers are given below
      DEFINE   EMC5600_SM_PIO_CTRL  "EMC5600_BASE+$10"      ;//Control
      DEFINE   EMC5600_SM_PIO_T1    "EMC5600_BASE+$11"      ;//Timer 1
      DEFINE   EMC5600_SM_PIO_T2    "EMC5600_BASE+$12"      ;//Timer 2
      DEFINE   EMC5600_SM_PIO_STAT  "EMC5600_BASE+$13"      ;//Status


    ENDIF

    IF !@DEF('GEN_CONSTANTS')
      DEFINE   GEN_CONSTANTS        '1'


;Modify on 12/20/2000 by JCP
;For code size reduction and speed improvement
;     DEFINE   TRANSFER_MODE_READ   '1'
;     DEFINE   TRANSFER_MODE_WRITE  '0'

      DEFINE   TRANSFER_MODE_READ   '2'
      DEFINE   TRANSFER_MODE_WRITE  '0'

;Modify on 12/20/2000 by JCP
;For code size reduction and speed improvement
;     DEFINE   ICMD_READID          '$90'
;     DEFINE   ICMD_READWRITE1      '$00'
;     DEFINE   ICMD_READWRITE3      '$50'
;     DEFINE   ICMD_READSTATUS      '$70'
;     DEFINE   ICMD_RESET           '$ff'
;     DEFINE   ICMD_ERASE           '$60'

      DEFINE   ICMD_READID          '$1200'
      DEFINE   ICMD_READWRITE1      '$00'
      DEFINE   ICMD_READWRITE3      '$0a00'
      DEFINE   ICMD_READSTATUS      '$0e00'
      DEFINE   ICMD_RESET           '$1fe0'
      DEFINE   ICMD_ERASE           '$0c00'





      DEFINE   MAX_DEVICES          "MaxDevices"                        ;//Supports four mem cards
      DEFINE   MAX_PARTITION        '4'
      DEFINE   PART_SIGN_OFFSET_FROM_END_OF_PART0   '$10'
      DEFINE   BOOT_COMMAND_SIZE    '149'       ;//(148*3 = 444 +2 = 446 bytes)



      DEFINE   MEDIA_INVALID        '0'     ;//
      DEFINE   MEDIA_NOT_FOUND      '0'     ;//
      DEFINE   MEDIA_PRESENT        '1'     ;//When the media is detected
      DEFINE   MEDIA_VALID          '3'     ;//Set when CIS_IDI is valid
      DEFINE   MEDIA_FORMATTED      '7'     ;//Set after verifying the partition
                                            ;//signature in MBR

      DEFINE   MAX_RETRY_CNT        '3'

      DEFINE   LOG2PHY_TABLE_SIZE   '512'
      DEFINE   FREE_TABLE_SIZE      '43'
      DEFINE   PGSIZE_256           '256'
      DEFINE   PGSIZE_512           '512'
      DEFINE   HALF_BIG_PAGE        "PGSIZE_256"

      DEFINE   RDNTAREASIZE_8       '8'
      DEFINE   RDNTAREASIZE_16      '16'



      ;//PACK DEFINES
      DEFINE   PACK_SMALL_PAGE_OFFSET       '86'
      DEFINE   PACK_SMALL_PAGE_RDNT_AREA_OFFSET '3'

      DEFINE   DATA_STAT_OFFSET_PGSIZE_264  '87'
      DEFINE   BLK_STAT_OFFSET_PGSIZE_264   '87'

      DEFINE   DATA_STAT_OFFSET_PGSIZE_528  '172'
      DEFINE   BLK_STAT_OFFSET_PGSIZE_528   '172'

      DEFINE   PGCNT_16         '16'
      DEFINE   PGCNT_32         '32'

      DEFINE   BKCNT_256        '256'
      DEFINE   BKCNT_512        '512'
      DEFINE   BKCNT_1024       '1024'
      DEFINE   MAX_BLK_CNT      '1024'
      DEFINE   MAX_LOGBLK_CNT   '1000'

      DEFINE    USED_FLAG      '0'
      DEFINE    FREE_FLAG      '1'


      DEFINE    ZONECNT_1       '1'
      DEFINE    ZONECNT_2       '2'
      DEFINE    ZONECNT_4       '4'
      DEFINE    ZONECNT_8       '8'

      DEFINE   S_1MB                '1'
      DEFINE   S_2MB                '2'
      DEFINE   S_4MB                '4'
      DEFINE   S_8MB                '8'
      DEFINE   S_16MB               '16'
      DEFINE   S_32MB               '32'
      DEFINE   S_64MB               '64'
      DEFINE   S_128MB              '128'

      ;//Byte position constants
      DEFINE    HIGH8               '0'
      DEFINE    MID8                '1'
      DEFINE    LOW8                '2'

      ;//Half Word position constants
      DEFINE    HIGH12              '0'
      DEFINE    LOW12               '1'

      DEFINE    EVEN_HALF           '0'
      DEFINE    ODD_HALF            '1'


;//REDUNDANT AREA MACROS
      DEFINE    REDT_BLK_STATUS_OFFSET      '1'
      DEFINE    REDT_DATA_STATUS_OFFSET     '1'
      DEFINE    REDT_ECC_FIELD1_OFFSET      '4'
      DEFINE    REDT_ECC_FIELD2_OFFSET      '2'
      DEFINE    REDT_LOG_BLK_ADDR1_OFFSET   '2'
      DEFINE    REDT_LOG_BLK_ADDR2_OFFSET   '3'


      ;//The number of cycles that must ellapse before
      ;//an operation should time out with a failure
      DEFINE    ERASE_BUSY_TIME             '$D3820A'
      DEFINE    READ_PAGE_BUSY_TIME         '$19C8A0'
      DEFINE    READ_PRA_BUSY_TIME          '$00CE45'
      DEFINE    READ_SMID_BUSY_TIME         '$0019C8'
      DEFINE    WRITE_PRA_BUSY_TIME         '$486AAA'
      DEFINE    WRITE_PAGE_BUSY_TIME        '$7FFFFF'
      DEFINE    RESET_MEDIA_BUSY_TIME       '$030583'

    ENDIF


    IF @DEF('ENABLE_ECC')
      DEFINE   BIT7                 '$80'
      DEFINE   BIT6                 '$40'
      DEFINE   BIT5                 '$20'
      DEFINE   BIT4                 '$10'
      DEFINE   BIT3                 '$08'
      DEFINE   BIT2                 '$04'
      DEFINE   BIT1                 '$02'
      DEFINE   BIT0                 '$01'
      DEFINE   BIT1BIT0             '$03'
      DEFINE   BIT23                '$00800000'
      DEFINE   MASK_CPS             '$3f'
      DEFINE   CORRECTABLE          '$00555554'
    ENDIF


  ENDIF