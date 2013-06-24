;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2003
;
; Filename:    eccutils.inc
; Description: Contains equates for ecc utilities.
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(_ECCUTILS_INC))
_ECCUTILS_INC   equ     1


        ;If you plan to use the write functions, you must have the following defines
 if (!@DEF('NOECCW'))
        DEFINE  HAL_ECC_ENABLE  '1'         ;Enables ECC Functionality by default
 endif
 if (!@DEF('NOECCR'))
        DEFINE  HAL_ECC_CHECK_ON_READ   '1'
 endif

      ;//PACK DEFINES
      DEFINE   PACKED_SMALL_PAGE_OFFSET       '86'

      ;//Byte position constants
      DEFINE    HIGH8               '0'
      DEFINE    MID8                '1'
      DEFINE    LOW8                '2'

      ;//Half Word position constants
      DEFINE    HIGH12              '0'
      DEFINE    LOW12               '1'

      DEFINE    EVEN_HALF           '0'
      DEFINE    ODD_HALF            '1'

      DEFINE   PGSIZE_256           '256'

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

    ENDIF  ; IF (!@def(_ECCUTILS_INC))


