; Filename: icoll_resetvectortable.asm
; Copyright (C) Sigmatel 2005

    include 'const.xref'

    global  IcollResetVectorTable
    extern  FIcollInvalidVector
    extern  FatalError

    org p,".ptexticoll_resetvectortable":

; Inputs:  No register params. Uses function start address IcollInvalidVector
; Outputs: void  writes the entire vector table. 
; Description: Default sets the entire 35xx vector table (64 2 word entries)  
; to 'jsr IcollInvalidVector' (DEBUG build halts) or 'jsr FatalError' (RETAIL build)
;
IcollResetVectorTable
    move    #>$0BF080,a                 ; jsr opcode
 if (@DEF('RETAIL'))
    move    #>FatalError,b
 else
    move    #>FIcollInvalidVector,b
 endif
    move    y:<Const_000000,r0
    do  #64,_enddo
        move    a1,p:(r0)+
        move    b1,p:(r0)+
_enddo
    rts

