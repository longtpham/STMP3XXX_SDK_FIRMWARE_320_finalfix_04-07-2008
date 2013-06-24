;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : sysirq.asm
;  Description : STMP IRQ Handling Functions
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_SysIrq
    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmacro.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "hwequ.inc"
    include "sysresources.inc"
    include "msgequ.inc"
    include "buttons.inc"
 if (@DEF('FUNCLET'))
    include "resource.inc"
 endif
    include "irqequ.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions
    global  IcollClear
    global  SysResetAllInterrupts
    global  SysSetupEncVectors
    global  SysSetupDecVectors
    global  FGetIrqL
	global  FSysGetIrqLevel
    global  FSetIrqL
	global	FSysSetIrqLevel
    global  GetIrqL
    global  SetIrqL

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysmem.xref"
    include "sysmisc.xref"
    include "stack.xref"
    include "stackmem.xref"
    include "sysresource.xref"
    include "const.xref"
    include "systime.xref"
    list

    extern  ButtonIsr                   ;easier to have the extern than include button.xref (which would require button.asm)
    extern  SysBrownOutIsr
    extern  SysHeadPhoneShortIsr
    extern  FECCISR

 if (@DEF('FUNCLET'))
    extern  SysCallFunclet
 endif

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

    org     x,"SYSFUN_SysIrq_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_SysIrq_Y":

; data for SysMaskAllInterrupts
Si_SaveReg_Ipr  dc  0
; data for SysUnmaskAllInterrupts
; data for SysResetAllInterrupts
; data for SysEnableIrq

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysIrq_P":


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           IcollClear
;
;   Type:           Function
;
;   Description:    Clears all ICOLL registers
;
;   Inputs:         none
;
;   Outputs:        none
;
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
IcollClear
    ;
    ; First, clear all ICOLL registers
    ;
    move    #HW_ICOLL_BASEADDR,r0
    move    y:<Const_000000,x0
    move    y:<Const_ffffff,m0
    do      #(HW_ICLOBSVZ1R-HW_ICOLL_BASEADDR),_reset_icoll
    move    x0,x:(r0)+
    nop
_reset_icoll
    rts


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysResetAllInterrupts
;
;   Type:           Function
;
;   Description:    Resets all system interrupts.
;
;   Inputs:         none
;
;   Outputs:        none
;
;   Notes:          This function completely reconfigures the interrupt vectors
;                   and interrupt hardware as required for the audio player
;                   software architecture.
;<
;///////////////////////////////////////////////////////////////////////////////
SysResetAllInterrupts

 if (@DEF('FUNCLET'))

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_SYSRESETALLINT

    org     p,"SYSFUNCLET_SysResetAllInt_P":

    dc      RSRC_FUNCLET_SYSRESETALLINT           ; Funclet Resource number

 endif


    ;
    ; Set the SR bits
    ;
    move    sr,x0
    bset    #HW_SR_IM0_BITPOS,x0
    bset    #HW_SR_IM1_BITPOS,x0
    move    x0,sr

    ;
    ; Clear out the ICOLL
    ;
    clr     a
    move    a,x:HW_ICLENABLE0R
    move    a,x:HW_ICLENABLE1R
    move    a,x:HW_ICLSTEER0R
    move    a,x:HW_ICLSTEER1R
    move    a,x:HW_ICLSTEER2R
    move    y:<Const_ffffff,x0
    move    x0,x:HW_ICLPRIOR0R
    move    x0,x:HW_ICLPRIOR1R
    move    x0,x:HW_ICLPRIOR2R
    move    x0,x:HW_ICLPRIOR3R

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        ; do nothing
    else
        move    x0,x:HW_ICLPRIOR4R
    endif

    ; init the vector addresses
    move    #>$bf080,x0             ; opcode for JSR
    ; Sys Time Timer
    move    #HW_IVECTIMER2,r1
    move    #>SysTimeIsr,n1
    move    x0,p:(r1)+
    move    n1,p:(r1)
    ; Button  Timer
    move    #HW_IVECTIMER1,r1
    move    #>ButtonIsr,n1
    move    x0,p:(r1)+
    move    n1,p:(r1)

    ;Init Brownout interrupt
    move    #HW_IVECIRQB,r1
    move    #>SysBrownOutIsr,n1
    move    x0,p:(r1)+
    move    n1,p:(r1)

    ;Init Headphone short interrupt
    move    #HW_IVECIRQA,r1
    move    #>SysHeadPhoneShortIsr,n1
    move    x0,p:(r1)+
    move    n1,p:(r1)

    ; Init ECC interrupt
    move    #HW_IVECHWECC,r1
    move    #>FECCISR,n1
    move    x0,p:(r1)+
    move    n1,p:(r1)

    ; init the ICOLL registers
    move    #>ICOLL_EN0_INIT,x0
    move    x0,x:HW_ICLENABLE0R
    move    #>ICOLL_EN1_INIT,x0
    move    x0,x:HW_ICLENABLE1R
    move    #>ICOLL_STEER0_INIT,x0
    move    x0,x:HW_ICLSTEER0R
    move    #>ICOLL_STEER1_INIT,x0
    move    x0,x:HW_ICLSTEER1R
    move    #>ICOLL_STEER2_INIT,x0
    move    x0,x:HW_ICLSTEER2R
    move    #>ICOLL_PR0_INIT,x0
    move    x0,x:HW_ICLPRIOR0R
    move    #>ICOLL_PR1_INIT,x0
    move    x0,x:HW_ICLPRIOR1R
    move    #>ICOLL_PR2_INIT,x0
    move    x0,x:HW_ICLPRIOR2R
    move    #>ICOLL_PR3_INIT,x0
    move    x0,x:HW_ICLPRIOR3R
    move    #>ICOLL_PR4_INIT,x0
    move    x0,x:HW_ICLPRIOR4R

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        ; Initialize HW_IPR register depending on chip version
        move    x:HW_REVR,a
        move    #>HW_3400_REV_UA2_SETMASK,x1
        sub     x1,a    #>IPR_INIT_IRQA_IRQB_ENABLE,x0 ;By default higher version

        jgt     _KeepDefaultValue

        move    #>IPR_INIT,x0

    else

        ; Initialize HW_IPR register depending on chip version
        move    #>IPR_INIT_IRQA_IRQB_ENABLE,x0

    endif

_KeepDefaultValue
        move    x0,x:HW_IPR

    ; Init the SR bits
    move    sr,x0
    bclr    #HW_SR_IM0_BITPOS,x0
    bclr    #HW_SR_IM1_BITPOS,x0
    move    x0,sr

    rts

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,"SYSFUN_SysIrq_P":

 endif

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysSetupEncVectors
;
;   Type:           Function
;
;   Description:    Setup Interrupt Vectors for the ADC ISR
;
;   Inputs:         x0 = ADC Full vector routine address
;                   x1 = ADC Overflow vector routine address
;                   y0 = Encoder Module Timer vector routine address
;
;   Outputs:        none
;
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
SysSetupEncVectors

 if (@DEF('FUNCLET'))

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_SYSSETUPENCVEC

    org     p,"SYSFUNCLET_SysSetupEncVec_P":

    dc      RSRC_FUNCLET_SYSSETUPENCVEC           ; Funclet Resource number

 endif

    move    #>$bf080,y1                 ; opcode for JSR
    ; ADC Full
    move    #HW_IVECADCF,r1
    nop
    move    y1,p:(r1)+
    move    x0,p:(r1)
    ; ADC Overflow
    move    #HW_IVECADCOF,r1
    nop
    move    y1,p:(r1)+
    move    x1,p:(r1)
    ; Hardware workaround
    ; Force the ADC isr to vector $46 also (hardware bug)
    move    #$46,r1
    nop
    move    y1,p:(r1)+
    move    x1,p:(r1)
    ; Encoder Module Timer
    move    #HW_IVECTIMER0,r1
    nop
    move    y1,p:(r1)+
    move    y0,p:(r1)
    rts

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,"SYSFUN_SysIrq_P":

 endif

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SysSetupDecVectors
;
;   Type:           Function
;
;   Description:    Setup Interrupt Vectors for the DAC ISR
;
;   Inputs:         x0 = DAC Refill vector routine address
;                   x1 = DAC Underflow vector routine address
;                   y0 = Decoder Module Timer vector routine address
;
;   Outputs:        none
;
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
SysSetupDecVectors

 if (@DEF('FUNCLET'))

    ;If using funclet, function will be executed via funclet kernel
    CallFunclet RSRC_FUNCLET_SYSSETUPDECVEC

    org     p,"SYSFUNCLET_SysSetupDecVec_P":

    dc      RSRC_FUNCLET_SYSSETUPDECVEC           ; Funclet Resource number

 endif

    move    #>$bf080,y1                 ; opcode for JSR
    ; DAC Refill
    move    #HW_IVECDACE,r1
    nop
    move    y1,p:(r1)+
    move    x0,p:(r1)
    ; DAC Underflow
    move    #HW_IVECDACUF,r1
    nop
    move    y1,p:(r1)+
    move    x1,p:(r1)
    ; Decoder Module Timer
    move    #HW_IVECTIMER0,r1
    nop
    move    y1,p:(r1)+
    move    y0,p:(r1)
    rts

 if (@DEF('FUNCLET'))

    ;If using funclet, switch counter back to original here
    org    p,"SYSFUN_SysIrq_P":

 endif

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           GetIrqL
;
;   Type:           Function
;
;   Description:    Get the SR IRQ Level
;
;   Inputs:         none
;
;   Outputs:        x0 = sr & IPL_MASK
;
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
FGetIrqL
FSysGetIrqLevel
GetIrqL
    move    sr,x0                       ;Reads Status Register
    move    #>HW_SR_IM_SETMASK,a1           ;
    and     x0,a                        ;Sets all bits to zero but Interrupt Mask Bits (I1,I0)
    move    a1,x0
    rts





;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           SetIrqL
;
;   Type:           Function
;
;   Description:    Sets the SR IRQ Level
;
;   Inputs:         x0 = Desired IRQ Level
;
;   Outputs:        none
;
;   Notes:          none
;<
;///////////////////////////////////////////////////////////////////////////////
FSetIrqL
FSysSetIrqLevel
    move    a1,x0
SetIrqL
    move    sr,y0                       ;Reads Status Register
    move    #>HW_SR_IM_CLRMASK,a1
    and     y0,a                        ;Clears Interrupt Masks (Bits I1 and I0)

    or      x0,a                        ;Updates Interrupt Masks with desired level
    move    a1,sr                       ;Updates Status Register
    rts

    endsec

