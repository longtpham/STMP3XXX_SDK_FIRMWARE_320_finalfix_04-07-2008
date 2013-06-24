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

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
;    include "sysequ.inc"
    include "hwequ.inc"
;    include "sysresources.inc"
;    include "msgequ.inc"
;    include "buttons.inc"
;    include "irqequ.inc"
    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    ;Functions
    global  FGetIrqL
    global  FSetIrqL
    global  GetIrqL
    global  SetIrqL
    
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

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

;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysIrq_P":


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
