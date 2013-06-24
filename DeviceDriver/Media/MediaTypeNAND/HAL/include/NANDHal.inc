;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2003
;
; Filename:    NANDHal.inc
; Description: Contains equates to mirror NANDHal.h file.
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(_NANDHAL_INC))
_NANDHAL_INC   equ     1
    
  include "errors.inc"

  define NANDHALmem 'y'   ; note, this must match NANDHAL_MEM in NANDHal.h


    ENDIF  ; IF (!@def(_NANDHAL_INC))

