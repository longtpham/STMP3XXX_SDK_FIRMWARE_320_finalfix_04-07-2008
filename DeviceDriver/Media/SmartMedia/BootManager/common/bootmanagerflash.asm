;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2006
;
;  File        : bootmanagerflash.asm
;  Description : BootManager flash routines
;  Notes       : Updated July 2006 with port of segments surrounded by if !BOOT_SEQUENCE_ORG
;                to search for firmware from end of flash to reduce boot time.
;///////////////////////////////////////////////////////////////////////////////

        
    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

        nolist
        include "sysequ.inc"
        include "hwequ.inc"
        include "smequ.inc"
        include "stack.inc"

    if (@def('DEVICE_3500'))
	else
		include "hwequ.inc"
    endif

        list

        section BOOTMANAGER_FLASH


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global    FlashReceive1, FlashInitPort1, FlashExit1, FlashError1

    if (!@def('BOOT_SEQUENCE_ORG'))
    global    FlashInitPort1_Boot, FlashError1_Boot
    endif

    if (!@def('DEVICE_3410'))
    global  FlashInit_1p8V, b1p8Volts
    endif

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "bootnand.xref"
    list
    
    extern  FLH_PAGESTATE_A
    extern  PowerUpFlashPads
    extern  NandFindNextDevice
    extern  FNandSearchForSTMPBlock

    if (!@def('BOOT_SEQUENCE_ORG'))
    extern  NandFindPrevDevice
    extern  FNandSearchForSTMPBlockReverse
    endif

    extern  NandSaveBootInfo
    extern  NandFillSinkBuffer
    extern  NandSectorNum
    extern  FcheckPadPower
    extern  FNANDInitInterfacePort
    extern  FNANDTerminateInterfacePort
    extern  FSectorSizeInWordsPlusOne
    
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

    org     p,"BOOTMANAGER_Flash_P":




;///////////////////////////////////////////////////////////////////////////////
;   Flash Init Port 
;
;   Must preserve:
;       y1
;///////////////////////////////////////////////////////////////////////////////
FlashInitPort1
    push    y1
    clr     a                           ; Any device in a storm, er... port
    jsr     FNANDInitInterfacePort
    pop     y1

    ; init some variables
    move    #>$000000,x0
    move    x0,x:NandSectorNum
    move    x0,x:NandCurDevNum

SmNextCardInit
;    move    y1,y:FlashInitSaveY1        ; save y1
    move    y1,X:FlashInitSaveY1        ; save y1

_fip_find_card_loop
    ; clear out some variables
    move    #>$000000,x0
    move    x0,x:NandBlockNum     
    move    x0,x:SmLookForBlock

    ; clear out the bootinfo & retain the BootType
    move    x:BootInfo,x0
    clr     a
    move    #>$800000,a1
    and     x0,a
    move    a1,x:BootInfo

    ; Find the next NAND device
    jsr     NandFindNextDevice
    jcs     _fip_card_detected
    jmp     ChangeToBootUsb           ; go boot from USB

_fip_card_detected
    ; card was found so flag it as detected
    bset    #SM_INFO_CARD_DETECTED,x:BootInfo

    jsr     FcheckPadPower              ; If this is a 16-bit NAND, make sure the extra pads are powered up

    ; try to find an STMPBlock 
    jsr     FNandSearchForSTMPBlock
    tst     a
    jeq     _fip_good_boot_card

    ; no STMPBlocks found... try the next card
    jsr     NandSaveBootInfo          ; Returns the current card number in y0

    ; increment the card number
    move    #>$000001,a
    add     y0,a
    move    a1,x:NandCurDevNum
    move    #>$000004,x0
    cmp     x0,a
    jne     _fip_find_card_loop

    ; end of cards
    jmp     ChangeToBootUsb

_fip_good_boot_card
    ; found a good boot card so try to boot from it
    bset    #BOOT_INFO_ATTEMPT,x:BootInfo
    ; Set initial page state
    move    #>FLH_PAGESTATE_A,x1        ; initial state is A
    move    x1,x:FlashPageState
    ; Setup the sink buf pointers
    move    #FlashSinkBuf,x1
    move    x1,x:FlashSinkHeadPtr
    move    x1,x:FlashSinkTailPtr

_fip_exit
;    move    y:FlashInitSaveY1,y1
    move    X:FlashInitSaveY1,y1
    rts

    if (!@def('BOOT_SEQUENCE_ORG'))
;///////////////////////////////////////////////////////////////////////////////
;   Flash Init Port Boot
;
;   Must preserve:
;       y1
;///////////////////////////////////////////////////////////////////////////////
FlashInitPort1_Boot
    push    y1
    clr     a                           ; Any device in a storm, er... port
    jsr     FNANDInitInterfacePort
    pop     y1

    ; init some variables
    move    #>$000000,x0
    move    x0,x:NandSectorNum

    ; from nand3 to nand0
    move    #>$000003,x0
    move    x0,x:NandCurDevNum

    move    y1,X:FlashInitSaveY1        ; save y1

_fip_find_card_loop_Boot
    ; clear out some variables
    move    #>$000000,x0
    move    x0,x:NandBlockNum     
    move    x0,x:SmLookForBlock

    ; clear out the bootinfo & retain the BootType
    move    x:BootInfo,x0
    clr     a
    move    #>$800000,a1
    and     x0,a
    move    a1,x:BootInfo

    ; Find the next NAND device
    jsr     NandFindPrevDevice
    jcs     _fip_card_detected_Boot
    jmp     ChangeToBootUsb           ; go boot from USB

_fip_card_detected_Boot
    ; card was found so flag it as detected
    bset    #SM_INFO_CARD_DETECTED,x:BootInfo

    jsr     FcheckPadPower              ; If this is a 16-bit NAND, make sure the extra pads are powered up

    ; try to find an STMPBlock 
    jsr     FNandSearchForSTMPBlockReverse
    tst     a
    jeq     _fip_good_boot_card_Boot

    ; no STMPBlocks found... try the next card
    jsr     NandSaveBootInfo          ; Returns the current card number in y0

    move    y0,a
    tst     a
    ; end of cards
    jeq     ChangeToBootUsb

    ; decrement the card number
    dec     a
    move    a1,x:NandCurDevNum
    jmp     _fip_find_card_loop_Boot

_fip_good_boot_card_Boot
    ; found a good boot card so try to boot from it
    bset    #BOOT_INFO_ATTEMPT,x:BootInfo
    ; Set initial page state
    move    #>FLH_PAGESTATE_A,x1        ; initial state is A
    move    x1,x:FlashPageState
    ; Setup the sink buf pointers
    move    #FlashSinkBuf,x1
    move    x1,x:FlashSinkHeadPtr
    move    x1,x:FlashSinkTailPtr

    move    X:FlashInitSaveY1,y1
    rts

    endif

    if (!@def('DEVICE_3410'))
;///////////////////////////////////////////////////////////////////////////////
;> Name: FlashInit_1p8V
;
;  Type: Function
;  Description: 	Init routine for 1.8V NAND mode
;  Inputs: 
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
FlashInit_1p8V


    move    #>1,a
    move    a,p:b1p8Volts           ; Change the flag to TRUE (default is FALSE)

    jsr FlashInitPort1               ; Let the 3.3V version handle the rest
    rts




b1p8Volts   dc  0                   ; 1 if using 1.8V NAND interface, else 0
                                    ; NOTE: All NANDs must be either 1.8V or 3.3V

    endif


;///////////////////////////////////////////////////////////////////////////////
;   Flash Receive
;
;   Must preserve:
;       y1
;   On entry:
;   On exit:
;       x0 = data
;///////////////////////////////////////////////////////////////////////////////
FlashReceive1
    ;
    ; Save some registers
    ;
    move    y1,x:FlashSaveY1
    
    ;
    ; Data in FlashSinkBuf?
    ;
    move    x:FSectorSizeInWordsPlusOne,y0    ; y0=buffer size
    move    x:FlashSinkHeadPtr,x0   ; a=head ptr
    move    x:FlashSinkTailPtr,a    ; x0=tail ptr
    sub     x0,a                    ; a=tail-head
    jgt     _NoModuloCorrect        
    add     y0,a                    ; perform modulo correction
_NoModuloCorrect    
    move    a1,x1                   ; save available space
    
    ; find out if we have any data
    move    x:FSectorSizeInWordsPlusOne,a
    cmp     x1,a
    jne     _buf_not_empty
    
    ;
    ; No data in FlashSinkBuf, so initiate a page read
    ;     
    jsr     NandFillSinkBuffer
    
    ;
    ; There's data in FlashSinkBuf, get a word
    ;
_buf_not_empty
    move    x:FlashSinkTailPtr,r1   ; x1=tail ptr
    nop
    move    x:(r1)+,x0              ; get data
    move    r1,x:FlashSinkTailPtr   ; update the tail ptr
    

    ;
    ; Restore some registers
    ;
    move    x:FlashSaveY1,y1
    rts






;///////////////////////////////////////////////////////////////////////////////
;   FlashExit
;///////////////////////////////////////////////////////////////////////////////
FlashExit1
    bset    #BOOT_INFO_SUCCESS,x:BootInfo
    jsr     NandSaveBootInfo
    

    clr     a                       ; Put all EMC pins back in Peripheral Mode

    jsr     FNANDTerminateInterfacePort

    rts


;///////////////////////////////////////////////////////////////////////////////
;   FlashError
;///////////////////////////////////////////////////////////////////////////////
FlashError1
    jsr     NandSaveBootInfo          ; Returns Current card number in y0

    ; inc the card number
    move    #>$000001,a
    add     y0,a
    move    #>$000004,x0
    cmp     x0,a
    jeq     _no_more_cards

    move    a1,x:NandCurDevNum
    jsr     SmNextCardInit

    move    #0,sp
    jmp     Getkey

_no_more_cards
    jmp     ChangeToBootUsb


    if (!@def('BOOT_SEQUENCE_ORG'))
FlashError1_Boot
    jsr     NandSaveBootInfo          ; Returns Current card number in y0

    move    y0,a
    tst     a
    ; end of cards
    jeq     ChangeToBootUsb

    ; dec the card number
    dec     a
    move    a1,x:NandCurDevNum
    jsr     SmNextCardInit

    move    #0,sp
    jmp     Getkey

    endif

    endsec
