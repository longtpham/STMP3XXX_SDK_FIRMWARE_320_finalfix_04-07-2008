    if (!@def(_MEDIADETECTION_INC))
_MEDIADETECTION_H               equ     1

MDF_MEDIA_CHANGED_BITPOS        equ     0
MDF_INIT_FILE_SYSTEM_BITPOS     equ     1

MDF_IMEDIA_CHANGED_SETMASK      equ     1<<MDF_MEDIA_CHANGED_BITPOS
MDF_INIT_FILE_SYSTEM_SETMASK    equ     1<<MDF_INIT_FILE_SYSTEM_BITPOS

MDF_MEDIA_CHANGED_CLRMASK       equ     ~MDF_IMEDIA_CHANGED_SETMASK
MDF_INIT_FILE_SYSTEM_CLRMASK    equ     ~MDF_INIT_FILE_SYSTEM_BITPOS

MEDIA_DETECT_POSITIVE_POLARITY  equ     0
MEDIA_DETECT_NEGATIVE_POLARITY  equ     1

MEDIA_TYPE_SMARTMEDIA           equ     0
MEDIA_TYPE_MMC                  equ     1

MD_ERROR_INVALID_MEDIA_NUMBER   equ     1

   if (!@def(NUM_REMOVABLE_MEDIA))
NUM_REMOVABLE_MEDIA             equ     1
   endif

   endif ; if (!@def(_MEDIADETECTION_INC))