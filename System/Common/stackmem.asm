;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2006
;
;  File        : stackmem.asm
;  Description : Data Stack memory
;///////////////////////////////////////////////////////////////////////////////

    section SYSFUN_StackMem

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////
    include "project.inc"
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////



    ;Y Variables
    global  IntL1_SaveR6
    global  IntL2_SaveM7
    global  IntL2_SaveR6
    global  IntL2_SaveR7
    global  IntL3_SaveM7
    global  IntL3_SaveR6
    global  IntL3_SaveR7
    global  L1DataStack
    global  L1Sp
    global  L2DataStack
    global  L2Sp
    global  L3DataStack
    global  L3Sp
    
    global  DecoderReadSaveR6
    global  Mp3DecSavePC
    global  Mp3DecSaveSR
    global  Mp3DecFileReadIsrSp
    
    global  L3DataStackEnd
    



;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
        

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////

    org     x,"SYSFUN_StackMem_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_StackMem_Y":

IntL1_SaveR6    ds      1
DecoderReadSaveR6   ds  1

Mp3DecSavePC    ds  1
Mp3DecSaveSR    ds  1
Mp3DecFileReadIsrSp ds  1

L1Sp            ds      1

IntL2_SaveR7    ds      1
IntL2_SaveM7    ds      1
IntL2_SaveR6    ds      1
L2Sp            ds      1

IntL3_SaveR7    ds      1
IntL3_SaveM7    ds      1
IntL3_SaveR6    ds      1
L3Sp            ds      1

    if !@def(STACK_L1_SIZE)
;        if (@def('DEVICE_3400')||@def('DEVICE_3300'))
;STACK_L1_SIZE equ 200
;        else ; need more stack for wma
            if (@def('FAT32TEST'))
STACK_L1_SIZE equ 700
            else 
STACK_L1_SIZE equ 350
            endif
;        endif ;(@def('DEVICE_3400')||@def('DEVICE_3300'))
    endif ;!!@def(STACK_L1_SIZE)

    if !@def(STACK_L2_SIZE)
STACK_L2_SIZE equ 65
    endif ;!!@def(STACK_L2_SIZE)

    if !@def(STACK_L3_SIZE)
;        if (@def('DEVICE_3400')||@def('DEVICE_3300'))
;STACK_L3_SIZE equ 140
;        else ; need more stack for wma
STACK_L3_SIZE equ 160
;        endif ;(@def('DEVICE_3400')||@def('DEVICE_3300'))
    endif ;!!@def(STACK_L3_SIZE)

L1DataStack
    bsc     STACK_L1_SIZE,$111111
L1DataStackEnd
    dc $DEADBE
L2DataStack
    bsc     STACK_L2_SIZE,$222222
L2DataStackEnd
    dc $DEADBE
L3DataStack
    bsc     STACK_L3_SIZE,$333333
L3DataStackEnd
    dc $DEADBE

    endsec

