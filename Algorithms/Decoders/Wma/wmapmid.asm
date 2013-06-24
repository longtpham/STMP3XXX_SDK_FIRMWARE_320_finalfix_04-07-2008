;;***********************************************************************
;SigmaTel, Inc
;$Archive: $
;$Revision:  $
;$Date: $
;Description: Initialize Portable Media Id, for DRM		
;***********************************************************************/
	section DRMPMId

;*********************************************************************************
;	Include Files
;
;*********************************************************************************
	include "wmaequ.inc"
	include "dspconst.inc"

;*********************************************************************************
;	Global Definitions
;
;*********************************************************************************

	global	Fg_pmid,Fpmid_length,PmidInit,FPmidInit

;*********************************************************************************
;	External Definitions
;
;*********************************************************************************
	
	if @def('WMATEST')
	else
    extern FDeviceGetId
    extern FDeviceGetIdSize
	endif
;*********************************************************************************
;		X memory 
;
;*********************************************************************************


; Integration Note:
; It is essential that we have a single function
; int sdmi_get_pmid(unsigned char *pmid), for all devices
; The serial number should be returned in the same format
; as returned to the host.
; Both the WMA Encoder and Decoder treat as characters.
; While char will be aligned on 8 bit boundaries on host.
; This will be aligned on 24 bit boundaries on the core.


; Default values (Used for test vectors).
; This are overwritten by PmidInit

        if @def('WMAAPI_NO_DRM')
        else
        
        org x,".XlicData":
Fg_pmid
                dc              $87
                dc              $c5
                dc              $09
                dc              $79
                dc              $02
                dc              $01
                dc              $42
                dc              $53
                dc              $00
                dc              $d1
                dc              $f1
                dc              $1b
                dc              $2f
                dc              $36
                dc              $13
                dc              $14
               ; dc              $33
               ; dc              $30
               ; dc              $31
               ; dc              $31
				ds				22					; Allocate buffer of size 42 to take care
													; of very long serial numbers
													

Fpmid_length    dc               16

;*********************************************************************************
;       Y memory 
;
;*********************************************************************************
        org y,".YlicData":
; Maintain large enough buffer to take care of max SN size from media functions
pmid_buf		ds				 24					; To pass to Media functions

;*********************************************************************************
;		P memory 
;
;*********************************************************************************
        org p,".pInit":

	if @def('WMATEST')
; Inputs:       none.
; Outputs:      0 in A
FPmidInit
PmidInit
    clr A
    rts
	
	else
; Function:     PmidInit
; Description:  Uses API calls to get the Portable Media ID and its size in
;               number of ascii bytes.  
;
; Inputs:       Current Dir Dev Id in A (aka media device number)
;
; Outputs:      Writes global x:Fpmid_length (number of ascii bytes)
;               Writes global x:Fg_pmid with retrieved storage media ID string. 
;                              (as one ASCII low byte per word)
; Returns status code in A: zero on success, else an error code. 
;
; Notes:        Writes y:pmid_buf with result from API. Then formatted copies it
;               to output x:Fg_pmid
;///////////////////////////////////////////////////////////////////////////////
FPmidInit
PmidInit
    move    A1,x:Fpmid_length  ; temporarily save media device num here.

    move    #pmid_buf,r0
    jsr     FDeviceGetIdSize  ; Get the media device's ID size in bytes. to y:(r0)

    move    x:Fpmid_length,y1  ; restore temp saved media device num input param.

    tst     A                       ; Did size function return err or success?
    jne     _exit                   ; returns any error code
    move    y:pmid_buf,A            ; Save media device's ID size (num ascii bytes)
    move    A1,x:Fpmid_length       ; 
    tst     A                       ; size == 0 check
    move    A1,x0                   ; num ascii SN bytes in x0
    jeq     _NoIdFound              ; if 0 length media ID, exit

    ; Get the media ID unpacked string. SN length (num ascii bytes) is in x0
    clr     a
    move    y1,A1                   ; media device number
    ; inputs: y:ro = &pmid_buf, a1= media device num
    move    #pmid_buf,r0
    jsr     FDeviceGetId ;into FS. Calls DataDriveGetSerialNumber() which uses DriveGetInfo()
    ; Return on error without overwriting any error code in A
    tst     A
    jne     _exit
    move    #pmid_buf,r0
    move    #Fg_pmid,r2
        
    ; Now copy the pmid_buf to the g_pmid as 1 ascii byte per word. 
    move    x:Fpmid_length,A     ; number of words to copy. Each contains 1 ascii byte of SN.
    
    ; This copies data as multiples of 3, but it does not matter, as rest of the buffer
    ; is anyway ignored.
    do      A1,_endCopyasBytes ; A1 is num words in SN. Each word contains one ascii byte.
        move    y:(r0)+,A
        move    A,x:(r2)+   ; Fg_pmid gets a copy of the ASCII string (stays unpacked)
_endCopyasBytes
    ; testpoint: verified correct x:Fpmid_length and format of x:Fg_pmid 
    ;    which are one SN byte per LS byte of word (one SN ascii digit).
    nop
_NoIdFound
	clr		A						; Return clean
_exit
    rts

	endif

    endif
    
    endsec
