;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2002-2004
;
;  File        : nanddevicedriver.asm
;  Description : Support functions for NANDs Device Driver
;///////////////////////////////////////////////////////////////////////////////

    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

        nolist
        include "sysequ.inc"
        include "stack.inc"
        include "smequ.inc"
    if (@def('DEVICE_3500'))
        include "regsusb20.inc"
        include "regsgpio.inc"
        include "regsspare.inc"
		include "regscore.inc"
	else
		include "hwequ.inc"
    endif
        list

        section BOOTROM_FLASH


;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////


    if ("FLH"=="SMFLH")

    global  FlashSimCheck

    endif        

    global   Sm001MbPtr
    global   Sm002MbPtr
    global   Sm004MbPtr
    global   Sm008MbPtr
    global   Sm016MbPtr
    global   Sm032MbPtr
    global   Sm064MbPtr
    global   Sm128MbPtr
    global   Sm000MbPtr
    
    global  FLH_PAGESTATE_A
    global  NandFindNextDevice

    if (!@def('BOOT_SEQUENCE_ORG'))
    global  NandFindPrevDevice
    endif

    global  NandSaveBootInfo
    global  NandFillSinkBuffer

    
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "bootnand.xref"
    list
    
    extern  FlashError1
    extern  SM_INFO_LARGE_MEDIA_FLAG
    extern  SM_INFO_16BIT_BUS_FLAG  
    extern  SM_INFO_LARGE_ADD_FLAG  
    extern  SM_INFO_SSFDC_CMD_FLAG  

    extern  FSectorSizeInWords
    extern  FSectorSizeInWordsPlusOne
    extern  FNandLookupID
    extern  pCurrentNANDDescriptor
    extern  FNandSearchForSTMPBlock
    extern  FNandReadSectorDataWrapper
    extern  FNandIncBlockNumber
    extern  FNandIsBlockBoundary
    extern  NandSectorNum
    extern  FNandHalInit
    
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

; !!!!!!!!!!!!! We're assuming SmartMedia >= 4MB !!!!!!!!!!!!!!!!!!!!!!!!
; !!!!! This FLH_PAGESIZE is different than that found in smequ.inc !!!!!!!!!!!
; !!!!!  it's okay because they are used differently.
FLH_PAGESIZE        equ     $0100
FLH_PAGESPERBLOCK   equ     32
FLH_PHYSBLOCKSIZE   equ     FLH_PAGESIZE*FLH_PAGESPERBLOCK
FLH_PAGESTATE_A     equ     1
FLH_PAGESTATE_B     equ     2
FLH_PAGESTATE_C     equ     3

; Flash progress stuff
FLH_PROG_INIT       equ     1<<0

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

    org     p,"BOOTROM_Flash_P":

;///////////////////////////////////////////////////////////////////////////////
;   NandSaveBootInfo
; Exit:
;   y0 = CardNum
;///////////////////////////////////////////////////////////////////////////////
NandSaveBootInfo
    ; get the flags for this card
    move    x:BootInfo,x1

    ; clear out bootinfo (preserve BootType)
    clr     a
    move    x1,a1
    move    #>$800000,x0
    and     x0,a
    move    a1,x:BootInfo

    ; get the offset to the SmInfoCardx word
    clr     a
    move    x:NandCurDevNum,y0              ; get the card number
    move    y0,a1

    move    #>SmCard0Info,x0
    add     x0,a
    move    a,r0
    nop
    move    x1,x:(r0)
    rts
    
;///////////////////////////////////////////////////////////////////////////////
;   NandFindNextDevice
;
;   Exit :   
;           Success      Fail
;   -----------------------------
;   Carry      1             0
;
;///////////////////////////////////////////////////////////////////////////////
NandFindNextDevice

_find_a_card
    move    x:NandCurDevNum,a
	jsr		FNandHalInit				; Detects the NAND and returns SUCCESS (0)
										; if found. Otherwise returns !SUCCESS
    tst     a                           ; Non-zero ID means card found
    jeq     _card_found

    ; card not found so inc card number and look again
    clr     a                           
    move    a,x:FlashPageBuf            ; clear the old ID read

    move    x:NandCurDevNum,a1
    move    #>$000001,x0                ; increment the card number
    add     x0,a
    move    a1,x:NandCurDevNum          ; save the card number

    move    #>4,x0
    cmp     x0,a                        ; end of cards?
    jne     _find_a_card

    ; checked all 4 chip-selects and no cards found
    bclr    #0,sr    
    rts

_card_found
    bset    #0,sr    
    rts    


    if (!@def('BOOT_SEQUENCE_ORG'))
;///////////////////////////////////////////////////////////////////////////////
;   NandFindPrevDevice
;
;   Exit :   
;           Success      Fail
;   -----------------------------
;   Carry      1             0
;
;///////////////////////////////////////////////////////////////////////////////
NandFindPrevDevice

_find_a_card_Boot
    move    x:NandCurDevNum,a
    jsr     FNandHalInit				; Detects the NAND and returns SUCCESS (0)
							; if found. Otherwise returns !SUCCESS
    tst     a                           ; Non-zero ID means card found
    jeq     _card_found_Boot

    ; card not found so inc card number and look again
    move    x:NandCurDevNum,a
    tst     a                        ; end of cards?
    jeq     _find_no_card_Boot

    dec     a                           ; decrement the card number
    move    a1,x:NandCurDevNum          ; save the card number

    clr     a                           
    move    a,x:FlashPageBuf            ; clear the old ID read

    jmp     _find_a_card_Boot

_find_no_card_Boot
    ; checked all 4 chip-selects and no cards found
    bclr    #0,sr    
    rts

_card_found_Boot
    bset    #0,sr    
    rts    

    endif


;///////////////////////////////////////////////////////////////////////////////
;   NandFillSinkBuffer
; Reads a new sector datta from the NAND and copies it into the sink buffer.
; This function handles tthe page boundaries andd searches for a new system
; block.
;
; Entry
;	none
; Exit
;	none
;
; In case of an error (i.e. no more blocks available, ... the function jumps
; to FlasError()
;///////////////////////////////////////////////////////////////////////////////
NandFillSinkBuffer
    ; do we need to look for a new STMPBlock?
	; After reading the last page of a bblock, x:SmLookForBlock is set to 1
	; therefore the next page to be read is in a different block. Before reading
	;this page, we need to find the neext system block.
    move    x:SmLookForBlock,a
    tst     a
    jeq     _sr_read_page

    ; need to find another STMPBlock
    clr     a
    move    a,x:SmLookForBlock
    jsr     FNandSearchForSTMPBlock
    tst     a
    jne     _sr_block_search_error

_sr_read_page
    ; reset the head and tail pointers to the sink buffer
    move    #>FlashSinkBuf,a0
    move    a0,x:FlashSinkHeadPtr
    move    a0,x:FlashSinkTailPtr
    
    ; read a page
    move    x:pCurrentNANDDescriptor,r0
    move    x:NandCurDevNum,a
    move    x:NandSectorNum,b
    move    #>FlashPageBuf,r4
    jsr     FNandReadSectorDataWrapper
    tst     a
    jne     _sr_page_read_error

    ; copy data from the NAND page buffer to the sink buffer
    jsr     NandCopyData

    ; increment the page 
    move    x:NandSectorNum,a
    move    #>$000001,x0
    add     x0,a
    move    a1,x:NandSectorNum
    jsr     FNandIsBlockBoundary
    tst     a
    jne     _sr_block_boundary_found
    rts

_sr_block_boundary_found
    ; we're at a boundary so we need to flag for an STMPBlock search next time
    jsr     FNandIncBlockNumber
    move    #>$000001,x0
    move    x0,x:SmLookForBlock
    rts

_sr_block_search_error
_sr_page_read_error
    ; catastrophic error
    jmp     FlashError1






;///////////////////////////////////////////////////////////////////////////////
;   NandCopyData
;	Copies the sector data pointed by #FlashPageBuf into buuffer pointed by
;	x:FlashSinkHeadPtr.
;
;	Due to byte alignment problems, the amount of data copied is variable.
;	There are 3 cases:
;	Case 1 FLH_PAGESTATE_A - The 1st byte destination byte is at byte offset 0:
;		170 (510 bytes) words are copied. The 2 extra bbytes are saved in
;		FlashLagniappe variable. After copy, the state is set to FLH_PAGESTATE_B
;	Case 2 FLH_PAGESTATE_B - The 1st byte destination byte is at byte offset 2:
;		171 (513 bytes) words are copied. The 2 first bytes in the destination buffer 
;		are the 2 bytes saved previously in the FlashLagniappe variable.
;		Only 511 bytes from the source buffer are copied into the ddestination
;		buffer. The extra byte is are saved in FlashLagniappe variable.
;		After copy, the state is set to FLH_PAGESTATE_C.
;	Case 3 FLH_PAGESTATE_C - The 1st byte destination byte is at byte offset 1:
;		171 (513 bytes) words are copied. The first byte in the destination buffer 
;		is the byte saved previously in the FlashLagniappe variable.
;		All 512 bytes from the source buffer are copied into the destination
;		buffer. This case leaves the destination buffer word aligned.
;		After copy, the state is set to FLH_PAGESTATE_A.
;///////////////////////////////////////////////////////////////////////////////
NandCopyData
    ; setup the buffer pointers
    move    #>FlashPageBuf,r0
    move    #>$ffff,m0
    move    x:FlashSinkHeadPtr,r1
    
    ; go to the current page state
    move    x:>FlashPageState,a
    move    #>FLH_PAGESTATE_A,x0
    cmp     x0,a                    
    jeq     pagestatea                
    move    #>FLH_PAGESTATE_B,x0
    cmp     x0,a                    
    jeq     pagestateb                
    move    #>FLH_PAGESTATE_C,x0
    cmp     x0,a                    
    jeq     pagestatec                

pagestatea
    ; in Page State A, flash byte 0 is aligned with FlashPageBuf byte 0
    move    x:FSectorSizeInWords,x0
    do      x0,_done_a
    move    x:(r0)+,x0                          ; get data    
    move    x0,x:(r1)+                          ; write to sink buf
_done_a
    move    x:(r0),x0                           ; get lagniappe
    move    x0,a
    move    #>$00FFFF,x1                        ; mask off upper byte
    and     x1,a
    move    a1,x:FlashLagniappe                 ; save lagniappe
    move    r1,x:FlashSinkHeadPtr               ; update the FlashSinkBufPtr
    move    #>FLH_PAGESTATE_B,x1                ; next state
    move    x1,x:FlashPageState

    rts                                         ; done
    
pagestateb
    ; in Page State B, flash byte 2 is aligned with FlashPageBuf byte 0
    move    #>$008000,x1                        ; setup the shift up factor
    move    x:FlashLagniappe,y0                 ; get lagniappe
                                                ; y0=__1100
    move    x:FSectorSizeInWordsPlusOne,x0
    do      x0,_done_b
    move    x:(r0)+,x0                          ; get next
    mpy     x0,x1,a         #>$00FFFF,x0        ; mask lower byte into upper lagniappe byte
                                                ; a1=__1100, a0=22____
    and     x0,a                                ; mask of artificats of signed multiply
    move    a1,y1                               ; temp save lagniappe
    move    a0,a1
    or      y0,a                                ; perform the mask
    move    a1,x:(r1)+                          ; write to sink buf
    move    y1,y0                               ; setup lagniappe
_done_b
    move    #>$0000FF,x1                        ; mask off upper two bytes of new lagniappe
    move    y0,a
    and     x1,a
    move    a1,x:FlashLagniappe                 ; save lagniappe
    move    r1,x:FlashSinkHeadPtr               ; update the FlashSinkBufPtr
    move    #>FLH_PAGESTATE_C,x1                ; next state
    move    x1,x:FlashPageState

    rts
    
pagestatec
    ; in Page State B, flash byte 1 is aligned with FlashPageBuf byte 0
    move    #>$000080,x1                        ; setup the shift up factor
    move    x:FlashLagniappe,y0                 ; get lagniappe
                                                ; y0=____00
    move    x:FSectorSizeInWordsPlusOne,x0
    do      x0,_done_c
    move    x:(r0)+,x0                          ; get next
    mpy     x0,x1,a         #>$0000FF,x0        ; mask lower 2 bytes into upper lagniappe 2 bytes
                                                ; a1=____00, a0=2211__
    and     x0,a                                ; mask of artificats of signed multiply
    move    a1,y1                               ; temp save lagniappe
    move    a0,a1
    or      y0,a                                ; perform the mask
    move    a1,x:(r1)+                          ; write to sink buf
    move    y1,y0                               ; setup lagniappe
_done_c
                                                ; no lagniappe to save!
    move    r1,x:FlashSinkHeadPtr               ; update the FlashSinkBufPtr
    move    #>FLH_PAGESTATE_A,x1                ; next state
    move    x1,x:FlashPageState

    rts

    
    endsec
