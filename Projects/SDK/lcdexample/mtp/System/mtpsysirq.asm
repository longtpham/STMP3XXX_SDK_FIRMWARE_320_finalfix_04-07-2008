;/////////////////////////////////////////////////////////////////////////////
;!
;! \file    mtpsysirq.asm
;! \brief   MTP System interrupt handlers.
;!
;!
;! Copyright (c) SigmaTel, Inc. Unpublished
;!
;! SigmaTel, Inc.
;! Proprietary  Confidential
;!
;! This source code and the algorithms implemented therein constitute
;! confidential information and may comprise trade secrets of SigmaTel, Inc.
;! or its associates, and any use thereof is subject to the terms and
;! conditions of the Confidential Disclosure Agreement pursuant to which this
;! source code was originally received.
;!
;/////////////////////////////////////////////////////////////////////////////
    section MtpSysIrq

;///////////////////////////////////////////////////////////////////////////////
;   Includes and external referencse
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "regsdcdc.inc"
    include "sysmacro.asm"
    list

    extern      FSystemShutdown
    extern      FSystemPowerOff

;///////////////////////////////////////////////////////////////////////////////
;   Symbol Exports 
;///////////////////////////////////////////////////////////////////////////////

    global      SysBrownout_ISR
                
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////
     org p,"ptextmtpsysirq":

;;//////////////////////////////////////////////////////////////////////////////
;;
;; Name:          SysBrownout_ISR
;;
;; Type:          Function
;;
;; Description:   Handle brown out.  While this function too should be 
;;    defined as an _interrupt function in the case of an actual brown out it
;;    is never returned and in the case it is not a brown out we just 
;;    save restore only the registers that are modified.
;;
;; Inputs:        none
;;
;; Outputs:       none
;;
;; Notes:         none
;;
;;//////////////////////////////////////////////////////////////////////////////
SysBrownout_ISR

    push    y1

    ; Clear the DM bit (double-precision multiply mode, bit 6) of the Mode Register
    ; This will be returned to its previous state when the RTI occurs (since the SR
    ; will be popped back off System Stack Low (SSL).
    ; Accumulator behavior is unpredictable when the DM bit is set.
    ; Next cycle after clearing it should NOT be an ALU instruction.
    andi    #$BF,mr
    nop

    ;
    ; Make sure this was actually a brown out ISR if not just return.
    ;
    move    x:HW_DCDC_VDDIO,y1
    jclr    #HW_DCDC_VDDIO_BROWNOUT_STATUS_BITPOS,y1,done
  
    ;
    ; This was a brownout so shutdown and power off the system, the call to 
    ; SystemPowerOff should never return.
    ;
    jsr     FSystemShutdown;
    jsr     FSystemPowerOff;
    

done:    
    pop     y1
    rti

    endsec
