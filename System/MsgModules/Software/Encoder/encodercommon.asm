;;***********************************************************************
;SigmaTel, Inc
;$Archive:  $
;$Revision: 2 $
;$Date: 1/24/04 12:14p $
;Description:  
;Notes:	
;***********************************************************************/
    Section	EncoderCommon
;************************************************************************
;  Include files
;************************************************************************
; Use relative paths only
    include "sysmacro.asm"
	include "const.xref"
    include "resource.inc"
    include "sysresources.inc"
    include "sysequ.inc"

;************************************************************************
;	Extern Variables
;************************************************************************
    EXTERN   F_lc_u_e_EncoderCommon_P
    EXTERN   F_lc_u_b_EncoderCommon_P
    EXTERN   F_lc_e_AdcmemX
    EXTERN   F_lc_b_AdcmemX
    EXTERN   F_lc_e_AdcmemY
    EXTERN   F_lc_b_AdcmemY
    EXTERN   SysLoadResource

;************************************************************************
;	Global Variables
;************************************************************************
    global LoadEncoderCommon 

    org p,"LoadEncoderCommon_P":

;************************************************************************
;  Code start
;************************************************************************
    if (@DEF('FUNCLET'))           
   
       extern    SysCallFunclet2
       ;If using funclet, function will be executed via funclet kernel
       CallFunclet2 RSRC_FUNCLET_LOADENCODERCOMMON
   
       org p,"SYSFUNCLET_LoadEncoderCommon_P":
       
       dc      RSRC_FUNCLET_LOADENCODERCOMMON           ; Funclet Resource number

    endif   
;////////////////////////////////////////////////////////////////////////
;   Name:           LoadEncoderCommon
;   Type:           Funclet
;   Description:    
;   Inputs:         
;   Output:         
;   Return Value:   
;   Registers Trashed : 
;   Notes:          
;////////////////////////////////////////////////////////////////////////

LoadEncoderCommon
;;;    error ;;; debug only
    ; Load the Adc Driver into P memory
    move    #>RSRC_ENCODERCOMMON_P_CODE,x0                     ; Resource number
    move    #>RSRC_TYPE_DATA,x1                         ; Resource type is data
    move    #>(F_lc_u_e_EncoderCommon_P-F_lc_u_b_EncoderCommon_P)*3,y1      ; Size of the overlay area
    move    #>F_lc_u_b_EncoderCommon_P,y0                         ; Address of the overlay area
    move    #TARGET_MEM_P,a1                            ; Overlay area is in P memory
    jsr     SysLoadResource

    ; Load the Adc Driver into X memory
    move    #>RSRC_ADC_ADCX_CODE,x0                     ; Resource number
    move    #>RSRC_TYPE_DATA,x1                         ; Resource type is data
    move    #>(F_lc_e_AdcmemX-F_lc_b_AdcmemX)*3,y1      ; Size of the overlay area
    move    #>F_lc_b_AdcmemX,y0                         ; Address of the overlay area
    move    #TARGET_MEM_X,a1                            ; Overlay area is in X memory
    jsr     SysLoadResource

    ; Load the Adc Driver into Y memory
    move    #>RSRC_ADC_ADCY_CODE,x0                     ; Resource number
    move    #>RSRC_TYPE_DATA,x1                         ; Resource type is data
    move    #>(F_lc_e_AdcmemY-F_lc_b_AdcmemY)*3,y1      ; Size of the overlay area
    move    #>F_lc_b_AdcmemY,y0                         ; Address of the overlay area
    move    #TARGET_MEM_Y,a1                            ; Overlay area is in Y memory
    jsr     SysLoadResource

    rts
    
    if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,"LoadEncoderCommon_P":

 endif
    
    endsec
    
