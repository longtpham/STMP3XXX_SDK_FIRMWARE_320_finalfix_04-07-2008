;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2002-2003
;
;  File        : usb20ddmem.asm
;  Description : Memory allocation for usb 2.0 device driver
;                and functions to manipulate DQHs and dTDs 
;///////////////////////////////////////////////////////////////////////////////


    page    132,60,0,0,0

;*******************************************************************************
;  Include files
;*******************************************************************************

        nolist
        include "sysmacro.asm"
        list
        
;*******************************************************************************
;  Macros
;*******************************************************************************

;*******************************************************************************
;  Extern definitions
;*******************************************************************************

    global  FdQH_Scratch_Cap
    global  FdQH_Scratch_Setup_Buf

    global  FdTD_Base_Scratch_Address
    global  FdTD_Base_Address
    global  FdXD_Base_Address       

    global  FInitUsbStructures
    global  FdTDUnpack
    global  FdTDPack
    global  FdQHPackCap
    global  FdQHUnpackSetup
    
    global  FdQH_Base_Add

;*******************************************************************************
;  Extern references
;*******************************************************************************
    extern FSysMaskAllInterrupts
    extern FSysUnMaskAllInterrupts
;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
    ; *** NOTE: if you change this equate, change the define for MAX_NUM_EP in usb_internal.h as well.
    ; The two definitions must be equivalent for proper operation.
    MAX_NUM_EP: equ 4

    ; Calculates the STMP Space Address for each dQH entry. The address is coded
    ; in STMP Word Address, Byte Offset in STMP word

    DUPF    OUTNUM,0,(MAX_NUM_EP-1)
        DUPF INNUM,0,1
BASE\OUTNUM\\INNUM                   equ     (((OUTNUM*2)+INNUM)*64)       
dQH_IOS_W\OUTNUM\\INNUM              equ     (((BASE\OUTNUM\\INNUM)/3)+FdQH_Base_Add)
dQH_IOS_BO\OUTNUM\\INNUM             equ     (BASE\OUTNUM\\INNUM)-((dQH_IOS_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_MAX_PKT_LENGTH_W\OUTNUM\\INNUM   equ     ((BASE\OUTNUM\\INNUM+2)/3)+FdQH_Base_Add
dQH_MAX_PKT_LENGTH_BO\OUTNUM\\INNUM  equ     (BASE\OUTNUM\\INNUM+2)-((dQH_MAX_PKT_LENGTH_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_NEXT_dTD_PTR_LOWER_BYTES_W\OUTNUM\\INNUM     equ     ((BASE\OUTNUM\\INNUM+8)/3)+FdQH_Base_Add
dQH_NEXT_dTD_PTR_LOWER_BYTES_BO\OUTNUM\\INNUM    equ     (BASE\OUTNUM\\INNUM+8)-((dQH_NEXT_dTD_PTR_LOWER_BYTES_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_NEXT_dTD_PTR_UPPER_BYTES_W\OUTNUM\\INNUM     equ     ((BASE\OUTNUM\\INNUM+10)/3)+FdQH_Base_Add
dQH_NEXT_dTD_PTR_UPPER_BYTES_BO\OUTNUM\\INNUM    equ     (BASE\OUTNUM\\INNUM+10)-((dQH_NEXT_dTD_PTR_UPPER_BYTES_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_STATUS_W\OUTNUM\\INNUM           equ     ((BASE\OUTNUM\\INNUM+12)/3)+FdQH_Base_Add
dQH_STATUS_BO\OUTNUM\\INNUM          equ     (BASE\OUTNUM\\INNUM+12)-((dQH_STATUS_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_TOTAL_BYTES_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+14)/3)+FdQH_Base_Add
dQH_TOTAL_BYTES_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+14)-((dQH_TOTAL_BYTES_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE0_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+40)/3)+FdQH_Base_Add
dQH_SETUP_BYTE0_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+40)-((dQH_SETUP_BYTE0_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE1_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+41)/3)+FdQH_Base_Add
dQH_SETUP_BYTE1_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+41)-((dQH_SETUP_BYTE1_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE2_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+42)/3)+FdQH_Base_Add
dQH_SETUP_BYTE2_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+42)-((dQH_SETUP_BYTE2_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE3_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+43)/3)+FdQH_Base_Add
dQH_SETUP_BYTE3_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+43)-((dQH_SETUP_BYTE3_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE4_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+44)/3)+FdQH_Base_Add
dQH_SETUP_BYTE4_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+44)-((dQH_SETUP_BYTE4_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE5_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+45)/3)+FdQH_Base_Add
dQH_SETUP_BYTE5_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+45)-((dQH_SETUP_BYTE5_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE6_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+46)/3)+FdQH_Base_Add
dQH_SETUP_BYTE6_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+46)-((dQH_SETUP_BYTE6_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
dQH_SETUP_BYTE7_W\OUTNUM\\INNUM      equ     ((BASE\OUTNUM\\INNUM+47)/3)+FdQH_Base_Add
dQH_SETUP_BYTE7_BO\OUTNUM\\INNUM     equ     (BASE\OUTNUM\\INNUM+47)-((dQH_SETUP_BYTE7_W\OUTNUM\\INNUM-FdQH_Base_Add)*3)
        ENDM
    ENDM

MASK_TOTAL_BYTES        equ     $3fff


;///////////////////////////////////////////////////////////////////////////////
;   Memory Reservation
; 
;   The equates below are declared here for a better comprehension of the code
;///////////////////////////////////////////////////////////////////////////////

    org x,"USB20_Device_Driver":

        ; Reserve memory space for dQHs
START_MEM_dQH       equ     *

FdQH_Base_Add
    ds      ((MAX_NUM_EP*2*64)/3)+1   ; Each End point has 2 DQHs. Each dQH is 64 bytes long

USB20_dQH_ALLOCATED_SIZE_BYTES  equ ((*-FdQH_Base_Add)*3) 


        ; Reserve dQH scratch endpoint capabilities space
        ; This structure is also defined in C. All dQH writes go thru this structure.
        ; To update the actual dQH, use the assembly function dQHPackCap()  
FdQH_Scratch_Cap            ds  5

dQH_SCRATCH_CAP_IOS_POS                 equ     0   ; 16-bit value. All bits set to 0 except bit 15 (ios)
dQH_SCRATCH_CAP_MAX_PKT_LENGTH_POS      equ     1   ; 16 bit value
                                                    ; bits 0-10 = Maximum Packet Length
                                                    ; bits 11-12 - always 0
                                                    ; bit 13 - zlt
                                                    ; bit 14-15 - Mult
dQH_SCRATCH_CAP_dTD_PTR_POS             equ     2   ; 16 bit pointer. We limit the ARC address space to 64Kbytes
dQH_SCRATCH_CAP_STATUS_POS              equ     3   ; 16 bit value
                                                    ; bits 0-7 Status
                                                    ; bits 8-14 - always 0
                                                    ; bit 15 - ioc
dQH_SCRATCH_TOTAL_BYTES_POS             equ     4   ; 16 bit value
                                                    ; bits 0-13 Total Bytes transfered
                                                    ; bits 14-15 - always 0 


        ; Reserve dQH scratch setup buffer
        ; This structure is also defined in C. All dQH setup reads go thru this structure.
        ; To update with the actual dQH information, use the assembly function dQHUnpackSetup()
FdQH_Scratch_Setup_Buf  ds  8

    DUPF    NUM,0,7
dQH_SCRATCH_SETUP_BUF_POS_\NUM          equ     NUM
    ENDM
                                                                
    ; The sum of those sizes must be less or equal 32 words (96 bytes)
dTD_SIZE            equ 10
dTD_SCRATCH_SIZE    equ 9
dXD_SIZE            equ 10
dTD_RESERVED_SIZE   equ 32-(dTD_SIZE+dTD_SCRATCH_SIZE+dXD_SIZE)


        ; Force alignment for the next data section on a 96 bytes boundary
dQH_N_96BYTE_BOUNDARY       equ     ((*-FdQH_Base_Add)/32)+1
dQH_RESERVED_AREA_SIZE      equ     (dQH_N_96BYTE_BOUNDARY*32)-(*-FdQH_Base_Add)     

dQH_Reserved            ds  dQH_RESERVED_AREA_SIZE          ; section on a 96 byte boundary

END_MEM_dQH         equ     *

        ; Reserve memory space for dTDs
        ; Each dTD must be exactly 32 words apart for aligment purposes

START_dTD_MEM       equ     *
    DUPF    NUM,0,((2*MAX_NUM_EP)-1)
dTD_\NUM                ds  dTD_SIZE                ; The actual dTD used by ARC
FdTD_SCRATCH_\NUM       ds  dTD_SCRATCH_SIZE        ; Scratch structure to pass and read parameters
                                                    ; to and from the actual dTD
                                                    ; This structure is also defined in C. All dTD reads/writes
                                                    ; used this structure. To update this structure with the actual
                                                    ; dTD values use dTDUnpack(). To update the actual dTD with
                                                    ; the values of this structure, use dTDUnpack().
FdXD_\NUM               ds  dXD_SIZE                ; Structure to define the transfer in progress on this dTD
                                                    ; This structure is also defined in C.
dTD_Reserved_\NUM       ds  dTD_RESERVED_SIZE       ; Force alignment on 32 words (96 bytes) boundary
    ENDM

END_dTD_MEM         equ     *

USB20_STRUCT_ALLOCATED_SIZE_BYTES   equ ((*-FdQH_Base_Add)*3) 

FdTD_Base_Address           ds  (2*MAX_NUM_EP)
FdTD_Base_Scratch_Address   ds  (2*MAX_NUM_EP)
FdXD_Base_Address           ds  (2*MAX_NUM_EP)

dTD_NEXT_LINK_PTR_POS   equ 0       ; 16 bit pointer. We limit the ARC address space to 64Kbytes
dTD_STATUS_IOC_POS      equ 1       ; 16 bit value
                                    ; bits 0-7 Status
                                    ; bits 8-14 - always 0
                                    ; bit 15 - ioc
dTD_TOTAL_BYTES_POS     equ 2       ; 16 bit value
                                    ; bits 0-13 Total Bytes to transfer
                                    ; bits 14-15 - always 0 
dTD_CUR_OFF_POS         equ 3       ; 16 bit value
                                    ; Offset into the 4KB buffer where the packet is to begin
                                    ; (limited to 12 bits)
dTD_BP0_POS             equ 4       ; 16 bit pointer - Page 0 offset for the packet buffer
dTD_BP1_POS             equ 5       ; 16 bit pointer - Page 1 offset for the packet buffer
dTD_BP2_POS             equ 6       ; 16 bit pointer - Page 2 offset for the packet buffer
dTD_BP3_POS             equ 7       ; 16 bit pointer - Page 3 offset for the packet buffer
dTD_BP4_POS             equ 8       ; 16 bit pointer - Page 4 offset for the packet buffer

;*******************************************************************************
;  P memory
;*******************************************************************************

    org     p,"BOOTROM_CmdHandler_P":

;///////////////////////////////////////////////////////////////////////////////
;       FInitUsbStructures
;       
; On Entry: 
;   none
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0
;   Accumulators:  
;   Data Regs:     x0
;
; This functions:
;       . zeroes out dQH and associated structures
;       . zeroes out dTD and associated structures
;       . Initializes dTDs table pointers
;       . Initializes dXDs table pointers
;       . Initializes dTDs Scratch table pointers
;///////////////////////////////////////////////////////////////////////////////
FInitUsbStructures
    move    #>$ffff,m0

    ; Zero out dTD memory allocation and associated structures
    move    #>0,x0
    move    #>START_dTD_MEM,r0
    rep     #(END_dTD_MEM-START_dTD_MEM)
        move    x0,x:(r0)+

    ; Zero out dQH memory allocation and associated structures
    move    #>START_MEM_dQH,r0
    move    #>0,x0
    rep     #(END_MEM_dQH-START_MEM_dQH)
        move    x0,x:(r0)+

    move    #>FdTD_Base_Address,r0
    DUPF    NUM,0,((2*MAX_NUM_EP)-1)
        move    #>dTD_\NUM,x0
        move    x0,x:(r0)+
    ENDM

    move    #>FdTD_Base_Scratch_Address,r0
    DUPF    NUM,0,((2*MAX_NUM_EP)-1)
        move    #>FdTD_SCRATCH_\NUM,x0
        move    x0,x:(r0)+
    ENDM

    move    #>FdXD_Base_Address,r0
    DUPF    NUM,0,((2*MAX_NUM_EP)-1)
        move    #>FdXD_\NUM,x0
        move    x0,x:(r0)+
    ENDM
    rts

;///////////////////////////////////////////////////////////////////////////////
;       FdTDPack
;       
; On Entry: 
;   a1 : Endpoint Number
;   b1 : Direction
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0, m1, r1, n1
;   Accumulators:  a, b
;   Data Regs:     x0, x1, y0
;
; This functions copies the info from the dTD_SCRATCH structure back to the dTD
; for the specific endpoint.
; The function is quite messy due to the ARC IP and STMP having different
; word length.
;///////////////////////////////////////////////////////////////////////////////
FdTDPack
   ; ** This function can be called from any context, and it needs mutual
   ; ** exclusion with itself.
    push    a1
    push    b1

    jsr     FSysMaskAllInterrupts
    move    a1,y0                   ; Save old ISR level

    pop     b1
    pop     a1

    ; Insure linear addressing
    move    #>$ffff,m0
    move    m0,m1

    ; Get the specific dTD and scratch structure base address
    lsl     a1      b1,x0
    add     x0,a
    move    a1,n0                       ; a1 is the endpoint number
    move    a1,n1
    move    #>FdTD_Base_Address,r0
    move    #>FdTD_Base_Scratch_Address,r1 
    move    x:(r0+n0),r0                ; r0 = Specific dTD Base Address
    move    x:(r1+n1),r1                ; r1 = Specific Scratch Structure Base Address

    ; Copy and Pack the Next Link Pointer and IOC_Status
    move    #>dTD_NEXT_LINK_PTR_POS,n1
    clr     a
    move    x:(r1+n1),a0
    move    #>dTD_STATUS_IOC_POS,n1
    bset    #0,a0                       ; Set TER bit. We always use one dTD per transfer
    move    x:(r1+n1),b1                ;Load the status + IOC 
    rep     #8
        lsl     b                       ;Shift left 8 times
    move    b1,x1        
    or      x1,a
    move    a0,x:(r0)+                  ; Save results to the dTD (word 0 and word 1)
    move    a1,x:(r0)+

    ; Copy and pack Total Bytes and Current Offset
    move    #>dTD_CUR_OFF_POS,n1
    move    #>(1<<(16-1)),x1                ; Current Offset starts at bit 16 0f dTD word 2 (STMP Space)
    move    x:(r1+n1),x0
    mpy     x0,x1,a
    move    #>dTD_TOTAL_BYTES_POS,n1
    move    a0,b1
    move    x:(r1+n1),x1
    or      x1,b                    ; b1 = TotalBytes + 1st byte Current Offset
    move    b1,x:(r0)+              ; Save result to the dTD (word 2)

    ; Copy and Pack Buffer Pointer Page 0
    move    #>dTD_BP0_POS,n1
    move    #>(1<<(4-1)),x1             ; BP0 starts at bit 4 of dTD word 3 (STMP Space)
    move    x:(r1+n1),x0
    mpy     x0,x1,b
    move    b0,x1
    or      x1,a                    ; a1 had the 4 upper bits of Current Offset
    move    a1,x:(r0)+              ; Save result to the dTD (word 3)

    ; Copy and pack Buffer Pointer Page 1
    move    #>dTD_BP1_POS,n1
    move    #>(1<<(12-1)),x1            ; BP1 starts at bit 12 of dTD word 4 (STMP Space)
    move    x:(r1+n1),x0
    mpy     x0,x1,a
    move    a0,x:(r0)+              ; Save Result to the dTD (word 4)
                                    ; We only save the 1st 12 bits of BP1. The rest most be 0
                                    ; because we only have (96K*3)bytes disponible.

    ; Copy and Pack Buffer Pointer Page 2
    move    #>dTD_BP2_POS,n1
    move    #>(1<<(20-1)),x1            ; BP2 starts at bit 20 of dTD word 5 (STMP Space)
    move    x:(r1+n1),x0
    mpy     x1,x0,a
    move    a0,x:(r0)+              ; Save Results to dTD (word 5 and word 6)
    move    a1,x:(r0)+

    ; Copy and Pack Buffer Pointer Page 3
    move    #>dTD_BP3_POS,n1
    move    #>(1<<(4-1)),x1             ; BP3 starts at bit 4 of dTD word 7 (STMP Space)
    move    x:(r1+n1),x0
    mpy     x1,x0,a
    move    a0,x:(r0)+              ; Save Result to dTD (word 7)

    ; Copy and Pack Buffer Pointer Page 4
    move    #>dTD_BP4_POS,n1
    move    #>(1<<(12-1)),x1            ; BP3 starts at bit 12 of dTD word 8 (STMP Space)
    move    x:(r1+n1),x0
    mpy     x1,x0,a
    move    a0,x:(r0)+              ; Save Result to dTD (word 8 and word 9)
    move    a1,x:(r0)


    move    y0,a1                   ; Restore old ISR level
    jsr     FSysUnMaskAllInterrupts

   rts



;///////////////////////////////////////////////////////////////////////////////
;       FdTDUnpack
;       
; On Entry: 
;   a1 : Endpoint Number
;   b1 : Direction
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0, m1, r1, n1
;   Accumulators:  a
;   Data Regs:     x0, y0
;
; This functions copies the info from the dTD for the specific endpoint
; to the dTD_SCRATCH structure for the specific endpoint.
;
; The only entries updated by this function are:
;   dTD_STATUS_IOC_POS  
;   dTD_TOTAL_BYTES_POS 
;   dTD_CUR_OFF_POS
; The other entries are irrelevant and not touch by the USB20 device
;
; The function is quite messy due to the ARC IP and STMP having different
; word length.
;///////////////////////////////////////////////////////////////////////////////
FdTDUnpack
    ; ** This function can be called from any context, and it needs mutual
    ; ** exclusion with itself.
    push    a1
    push    b1

    jsr     FSysMaskAllInterrupts
    move    a1,y0                   ; Save old ISR level

    pop     b1
    pop     a1

    ; Insure linear addressing
    move    #>$ffff,m0
    move    m0,m1

    ; Get the specific dTD and scratch structure base address
    lsl     a1      b1,x0
    add     x0,a
    move    a1,n0                   ; a1 is the endpoint number
    move    a1,n1
    move    #>FdTD_Base_Address,r0
    move    #>FdTD_Base_Scratch_Address,r1 
    move    x:(r0+n0),r0            ; r0 = Specific dTD Base Address
    move    x:(r1+n1),r1            ; r1 = Specific Scratch Structure Base Address

    ; Get the STATUS_IOC field
    move    #>1,n0                  ; STATUS_IOC field is word 1 of dTD (STMP Space)                    
    move    #>dTD_STATUS_IOC_POS,n1
    move    x:(r0+n0),a1
    move    a1,x:(r1+n1)

    ; Get the TOTAL_BYTES field
    move    #>2,n0                  ; TOTAL_BYTES field is word 2 of dTD (STMP Space)
    move    #>MASK_TOTAL_BYTES,x0
    move    x:(r0+n0),a1
    move    #>dTD_TOTAL_BYTES_POS,n1    
    and     x0,a                    ; Remove unrelevant bits
    move    a1,x:(r1+n1)


    move    y0,a1                   ; Restore old ISR level
    jsr     FSysUnMaskAllInterrupts

    rts

        

    
;///////////////////////////////////////////////////////////////////////////////
;       GetNext2Bytes
;       
; On Entry: 
;   r0 : Word Address
;   x0 : Byte Offset
; On Exit:
;   a1 : Results
; Modified:
;   Address Regs:  r0
;   Accumulators:  a
;   Data Regs:     x0, x1
;
; Fetches the next 2 bytes starting at word address and byte offset.
; The bytes are copied into the less significant bytes of a1. The most
; significant byte of a1 is cleared.
;
;///////////////////////////////////////////////////////////////////////////////
GetNext2Bytes
    push    y0
    
    clr     a
    move    x0,a
    tst     a       #>1,x0
    move    x:(r0)+,x1          
    jne     _CheckByteOffset1

    ; Byte offset is 0
    move    x1,a1
_MaskUnused
    move    #>$00ffff,x0
    and     x0,a1
    pop     y0
    rts

    ; Byte Offset is 1
_CheckByteOffset1
    cmp     x0,a    #>(1<<(16-1)),x0
    jne     _ByteOffset2
    mpy     x0,x1,a
    jmp     _MaskUnused

    ; Most difficult - Byte Offset is 2
    ; The 2 bytes to get are splitted across 2 words
_ByteOffset2
    move    #>(1<<(8-1)),x0
    mpy     x0,x1,a     #>$0000ff,x0
    and     x0,a        #>(1<<(8-1)),x1
    move    a1,y0                           ; y0 = Partial result
    move    x:(r0),x0
    mpy     x0,x1,a     #>$00ff00,x0
    move    a0,a1
    and     x0,a
    or      y0,a
    
    pop     y0
    rts
                


;///////////////////////////////////////////////////////////////////////////////
;       PutNext2Bytes
;       
; On Entry: 
;   r0 : Word Address
;   x0 : Byte Offset
;   x1 : Data to put (most significant byte is 00) 00ABCD
; On Exit:
;   none
; Modified:
;   Address Regs:  r0
;   Accumulators:  a, b
;   Data Regs:     x0, x1
;
; Fetches the next 2 bytes starting at word address and byte offset and
; replaces them by the 2 bytes passed in x1
;
;///////////////////////////////////////////////////////////////////////////////
PutNext2Bytes
    clr     b
    move    x0,b1                       ; b1 = Byte Offset
    move    x:(r0),a1                   ; a1 = Destination word
    tst     b   #>1,x0                  
    jne     _CheckByteOffset1

    ; Byte Offset 0 - a1=DDXXXX where XXXX is bytes to replace
    move    #>$ff0000,x0
_MaskAndSave
    and     x0,a                        ; zeroed out the XX in a1
    or      x1,a                        ; put in new bytes
    move    a1,x:(r0)                   ; save value
    rts

_CheckByteOffset1
    cmp     x0,b    #>(1<<(8-1)),x0
    jne     _ByteOffset2

    ; Byte Offset 1 - a1=XXXXDD where XXXX is bytes to replace
    mpy     x0,x1,b #>$0000ff,x0        ; b0 = ABCD00
    move    b0,x1                       ; x1 = ABCD00
    jmp     _MaskAndSave

_ByteOffset2
    ; Byte Offset 2 - a1=XXDDDD and x:(r0)+=DDDDXX
    ; The bytes to replace span across 2 words
    move    #>(1<<(16-1)),x0
    mpy     x0,x1,b     #>$00ffff,x0    ; b1b0 = 0000AB CD0000
    and     x0,a        b0,x0           ; a1=00DDDD x0=CD0000
    or      x0,a        #>$ffff00,x0    ; a1=CDDDDD
    move    a1,x:(r0)+                   ; Save 1st byte to replace
    move    x:(r0),a1                   ; Get next word a1=DDDDXX
    move    b1,x1                       ; x1=0000AB
    jmp     _MaskAndSave



;///////////////////////////////////////////////////////////////////////////////
;       GetNextByte
;       
; On Entry: 
;   r0 : Word Address
;   x0 : Byte Offset
; On Exit:
;   a1 : Result
; Modified:
;   Address Regs:  r0
;   Accumulators:  a
;   Data Regs:     x0, x1
;
; Fetches the next byte starting at word address and byte offset.
; The byte is copied into the less significant byte of a1. The most
; significant bytes of a1 are cleared.
;
;///////////////////////////////////////////////////////////////////////////////
GetNextByte
    clr     a
    move    x0,a
    tst     a       #>1,x0
    move    x:(r0),x1           
    jne     _CheckByteOffset1

    ; Byte offset is 0
    move    x1,a1
_MaskUnused
    move    #>$0000ff,x0
    and     x0,a1
    rts

    ; Byte Offset is 1
_CheckByteOffset1
    cmp     x0,a    #>(1<<(16-1)),x0
    jne     _ByteOffset2
    mpy     x0,x1,a
    jmp     _MaskUnused

    ; Byte Offset is 2
_ByteOffset2
    move    #>(1<<(8-1)),x0
    mpy     x0,x1,a
    jmp     _MaskUnused




;///////////////////////////////////////////////////////////////////////////////
;       FdQHUnpackCap
;       
; On Entry: 
;   a1 : Endpoint Number
;   b1 : Direction
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0 
;   Accumulators:  
;   Data Regs: y0     
;
; This functions copies the info from the dQH for the specific endpoint/direction
; to the dQH_SCRATCH_CAP structure.
;
; The only entries updated by this function are:
;   dQH_SCRATCH_CAP_IOS_POS 
;   dQH_SCRATCH_CAP_STATUS_POS  
;   dQH_SCRATCH_TOTAL_BYTES_POS
; The other entries are irrelevant.
;
; The function is quite messy due to the ARC IP and STMP having different
; word length.
; dQHs must be organized per ARC IP recomendations. All dQHs are in memory
; located back to back. The problem is that they are 64 bytes long, therefore
; accessing each entry in the STMP address space is quite difficult. To avoid
; lengthy divisions each endpoint has its own unpack function. This function
; finds the appropriate unpack function to call and calls it.
;///////////////////////////////////////////////////////////////////////////////
FdQHUnpackCap

    ; ** This function can be called from any context, and it needs mutual
    ; ** exclusion with itself.
    push    a1
    push    b1

    jsr     FSysMaskAllInterrupts
    move    a1,y0                   ; Save old ISR level

    pop     b1
    pop     a1
    

    ; Insure linear addressing
    move    #>$ffff,m0
    move    #>BaseAdddQHUnpackCapEndPoint,r0
    move    a1,n0
    nop
    move    p:(r0+n0),r0
    move    b1,a0
    jsr     (r0)
    nop


    move    y0,a1                   ; Restore old ISR level
    jsr     FSysUnMaskAllInterrupts

    rts



;///////////////////////////////////////////////////////////////////////////////
;       dQHUnpackCapEndPoint_\NUM
;       
; On Entry: 
;   a0 : EndPoint Direction
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0, m1, r1, n1 
;   Accumulators:  a
;   Data Regs: x0           
;
; This functions is called by the dQHUnpack() (see comments for more details)
;
; One function is generated per endpoint.
;///////////////////////////////////////////////////////////////////////////////
    DUPF    NUM,0,(MAX_NUM_EP-1)
dQHUnpackCapEndPoint\NUM
    move    #>$ffff,m1
    move    m1,m0
    move    #>FdQH_Scratch_Cap,r1
    move    a0,a1
    move    #>USB_OUT,x0
    cmp     x0,a
    jeq     _EP_DirOut\NUM

_EP_DirIn\NUM
    ; Get IOS entry
    move    #>dQH_IOS_W\NUM\1,r0
    move    #>dQH_IOS_BO\NUM\1,x0
    jsr     GetNext2Bytes
    move    #>dQH_SCRATCH_CAP_IOS_POS,n1
    nop
    move    a1,x:(r1+n1)    

    ; Get Status
    move    #>dQH_STATUS_W\NUM\1,r0
    move    #>dQH_STATUS_BO\NUM\1,x0
    jsr     GetNext2Bytes
    move    #>dQH_SCRATCH_CAP_STATUS_POS,n1
    nop
    move    a1,x:(r1+n1)    

    ; Get Total Bytes Transfered
    move    #>dQH_TOTAL_BYTES_W\NUM\1,r0
    move    #>dQH_TOTAL_BYTES_BO\NUM\1,x0
    jsr     GetNext2Bytes
    move    #>dQH_SCRATCH_TOTAL_BYTES_POS,n1
    nop
    move    a1,x:(r1+n1)    
    rts

_EP_DirOut\NUM
    ; Get IOS entry
    move    #>dQH_IOS_W\NUM\0,r0
    move    #>dQH_IOS_BO\NUM\0,x0
    jsr     GetNext2Bytes
    move    #>dQH_SCRATCH_CAP_IOS_POS,n1
    nop
    move    a1,x:(r1+n1)    

    ; Get Status
    move    #>dQH_STATUS_W\NUM\0,r0
    move    #>dQH_STATUS_BO\NUM\0,x0
    jsr     GetNext2Bytes
    move    #>dQH_SCRATCH_CAP_STATUS_POS,n1
    nop
    move    a1,x:(r1+n1)    

    ; Get Total Bytes Transfered
    move    #>dQH_TOTAL_BYTES_W\NUM\0,r0
    move    #>dQH_TOTAL_BYTES_BO\NUM\0,x0
    jsr     GetNext2Bytes
    move    #>dQH_SCRATCH_TOTAL_BYTES_POS,n1
    nop
    move    a1,x:(r1+n1)    
    rts

    ENDM


;///////////////////////////////////////////////////////////////////////////////
;       FdQHPackCap
;       
; On Entry: 
;   a1 : Endpoint Number
;   b1 : Direction
;   x0 : Init
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0 
;   Accumulators:  
;   Data Regs: y0     
;
; This functions copies the info from the dQH for the specific endpoint/direction
; to the dQH_SCRATCH_CAP structure.
;
; If Init is clear (0) :
;   The only entries updated by this function are:
;       Status_IOC field
;       dTD pointer field
; Else
;       Status_IOC field
;       dTD pointer field
;       MaxPacketLength field
;       IOS field
;
; The other entries are irrelevant.
;
; The function is quite messy due to the ARC IP and STMP having different
; word length.
; dQHs must be organized per ARC IP recomendations. All dQHs are in memory
; located back to back. The problem is that they are 64 bytes long, therefore
; accessing each entry in the STMP address space is quite difficult. To avoid
; lengthy divisions each endpoint has its own pack function. This function
; finds the appropriate pack function to call and calls it.
;///////////////////////////////////////////////////////////////////////////////
FdQHPackCap

    ; ** This function can be called from any context, and it needs mutual
    ; ** exclusion with itself.
    push    a1
    push    b1
    push    x0

    jsr     FSysMaskAllInterrupts
    move    a1,y0                   ; Save old ISR level

    pop     x0
    pop     b1
    pop     a1

    ; Insure linear addressing
    move    #>$ffff,m0
    move    #>BaseAdddQHPackCapEndPoint,r0
    move    a1,n0
    nop
    move    p:(r0+n0),r0
    move    b1,a0
    jsr     (r0)
    nop


    move    y0,a1                   ; Restore old ISR level
    jsr     FSysUnMaskAllInterrupts

    rts




;///////////////////////////////////////////////////////////////////////////////
;       dQHPackCapEndPoint_\NUM
;       
; On Entry: 
;   a0 : EndPoint Direction
;   x0 : init
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0, m1, r1, n1 
;   Accumulators:  a
;   Data Regs: x0, x1           
;
; This functions is called by the dQHPack() (see comments for more details)
;
; One function is generated per endpoint.
;///////////////////////////////////////////////////////////////////////////////
    DUPF    NUM,0,(MAX_NUM_EP-1)
dQHPackCapEndPoint\NUM
    move    #>$ffff,m1
    move    m1,m0
    move    #>FdQH_Scratch_Cap,r1
    move    a0,a1
    move    #>USB_OUT,x1
    cmp     x1,a
    jeq     _EP_\NUM\_DirOut

_EP_\NUM\_DirIn
    ; Check if init fields also required
    move    x0,a
    tst     a
    jeq     _Bypass_Init_\NUM\_DirIn

    ; Put IOS entry
    move    #>dQH_IOS_W\NUM\1,r0
    move    #>dQH_IOS_BO\NUM\1,x0
    move    #>dQH_SCRATCH_CAP_IOS_POS,n1
    nop
    move    x:(r1+n1),x1    
    jsr     PutNext2Bytes

    ; Put Max Packet Length
    move    #>dQH_MAX_PKT_LENGTH_W\NUM\1,r0
    move    #>dQH_MAX_PKT_LENGTH_BO\NUM\1,x0
    move    #>dQH_SCRATCH_CAP_MAX_PKT_LENGTH_POS,n1
    nop
    move    x:(r1+n1),x1    
    jsr     PutNext2Bytes

_Bypass_Init_\NUM\_DirIn
    ; Put Status
    move    #>dQH_STATUS_W\NUM\1,r0
    move    #>dQH_STATUS_BO\NUM\1,x0
    move    #>dQH_SCRATCH_CAP_STATUS_POS,n1
    nop
    move    x:(r1+n1),x1
    jsr     PutNext2Bytes

    ; Put Next dTD Pointer (2 LS Bytes)
    move    #>dQH_NEXT_dTD_PTR_LOWER_BYTES_W\NUM\1,r0
    move    #>dQH_SCRATCH_CAP_dTD_PTR_POS,n1
    move    #>dQH_NEXT_dTD_PTR_LOWER_BYTES_BO\NUM\1,x0
    move    x:(r1+n1),x1
    jsr     PutNext2Bytes

    rts

_EP_\NUM\_DirOut
    ; Check if init fields also required
    move    x0,a
    tst     a
    jeq     _Bypass_Init_\NUM\_DirOut

    ; Put IOS entry
    move    #>dQH_IOS_W\NUM\0,r0
    move    #>dQH_IOS_BO\NUM\0,x0
    move    #>dQH_SCRATCH_CAP_IOS_POS,n1
    nop
    move    x:(r1+n1),x1    
    jsr     PutNext2Bytes

    ; Put Max Packet Length
    move    #>dQH_MAX_PKT_LENGTH_W\NUM\0,r0
    move    #>dQH_MAX_PKT_LENGTH_BO\NUM\0,x0
    move    #>dQH_SCRATCH_CAP_MAX_PKT_LENGTH_POS,n1
    nop
    move    x:(r1+n1),x1    
    jsr     PutNext2Bytes

_Bypass_Init_\NUM\_DirOut
    ; Put Status
    move    #>dQH_STATUS_W\NUM\0,r0
    move    #>dQH_STATUS_BO\NUM\0,x0
    move    #>dQH_SCRATCH_CAP_STATUS_POS,n1
    nop
    move    x:(r1+n1),x1
    jsr     PutNext2Bytes

    ; Put Next dTD Pointer (2 LS Bytes)
    move    #>dQH_NEXT_dTD_PTR_LOWER_BYTES_W\NUM\0,r0
    move    #>dQH_SCRATCH_CAP_dTD_PTR_POS,n1
    move    #>dQH_NEXT_dTD_PTR_LOWER_BYTES_BO\NUM\0,x0
    move    x:(r1+n1),x1
    jsr     PutNext2Bytes

    rts

    ENDM



;///////////////////////////////////////////////////////////////////////////////
;       FdQHUnpackSetup
;       
; On Entry: 
;   a1 : Endpoint Number
;   b1 : Direction
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0 
;   Accumulators:  
;   Data Regs: y0     
;
; This functions copies the info from the dQH for the specific endpoint/direction
; to the dQH_SCRATCH_SETUP structure.
;
; The only entries updated by this function are:
;   dQH_SCRATCH_SETUP_BUF_0_POS
;   dQH_SCRATCH_SETUP_BUF_1_POS
;   dQH_SCRATCH_SETUP_BUF_2_POS
;   dQH_SCRATCH_SETUP_BUF_3_POS
;   dQH_SCRATCH_SETUP_BUF_4_POS
;   dQH_SCRATCH_SETUP_BUF_5_POS
;   dQH_SCRATCH_SETUP_BUF_6_POS
;   dQH_SCRATCH_SETUP_BUF_7_POS
; The other entries are irrelevant.
;
; The function is quite messy due to the ARC IP and STMP having different
; word length.
; dQHs must be organized per ARC IP recomendations. All dQHs are in memory
; located back to back. The problem is that they are 64 bytes long, therefore
; accessing each entry in the STMP address space is quite difficult. To avoid
; lengthy divisions each endpoint has its own unpack function. This function
; finds the appropriate unpack function to call and calls it.
;///////////////////////////////////////////////////////////////////////////////
FdQHUnpackSetup

    ; ** This function can be called from any context, and it needs mutual
    ; ** exclusion with itself.
    push    a1
    push    b1

    jsr     FSysMaskAllInterrupts
    move    a1,y0                   ; Save old ISR level

    pop     b1
    pop     a1

    ; Insure linear addressing
    move    #>$ffff,m0
    move    #>BaseAdddQHUnpackSetupEndPoint,r0
    move    a1,n0
    nop
    move    p:(r0+n0),r0
    move    b1,a0
    jsr     (r0)
    nop


    move    y0,a1                   ; Restore old ISR level
    jsr     FSysUnMaskAllInterrupts

    rts


;///////////////////////////////////////////////////////////////////////////////
;       dQHUnpackSetupEndPoint_\NUM
;       
; On Entry: 
;   a0 : EndPoint Direction
; On Exit:
;   none
; Modified:
;   Address Regs:  m0, r0, n0, m1, r1, n1 
;   Accumulators:  a
;   Data Regs: x0           
;
; This functions is called by the dQHUnpackSetup() (see comments for more details)
;
; One function is generated per endpoint.
;///////////////////////////////////////////////////////////////////////////////
    DUPF    NUM,0,(MAX_NUM_EP-1)
dQHUnpackSetupEndPoint\NUM
    move    #>$ffff,m1
    move    m1,m0
    move    #>FdQH_Scratch_Setup_Buf,r1
    move    a0,a1
    move    #>USB_OUT,x0
    cmp     x0,a
    jeq     _EP_\NUM\_DirOut

    ; Get Setup Byte  
    move    #>dQH_SETUP_BYTE0_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE0_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE1_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE1_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE2_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE2_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE3_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE3_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE4_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE4_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE5_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE5_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE6_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE6_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE7_W\NUM\1,r0
    move    #>dQH_SETUP_BYTE7_BO\NUM\1,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    rts

_EP_\NUM\_DirOut
    move    #>dQH_SETUP_BYTE0_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE0_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE1_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE1_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE2_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE2_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE3_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE3_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE4_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE4_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE5_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE5_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE6_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE6_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    move    #>dQH_SETUP_BYTE7_W\NUM\0,r0
    move    #>dQH_SETUP_BYTE7_BO\NUM\0,x0
    jsr     GetNextByte
    move    a1,x:(r1)+

    rts

    ENDM


    ; Address table for dQHUnpackCapEndPoint_\NUM functions 
BaseAdddQHUnpackCapEndPoint
    DUPF    NUM,0,(MAX_NUM_EP-1)
        dc      dQHUnpackCapEndPoint\NUM
    ENDM

    ; Address table for dQHUnpackSetupEndPoint_\NUM functions 
BaseAdddQHUnpackSetupEndPoint
    DUPF    NUM,0,(MAX_NUM_EP-1)
        dc      dQHUnpackSetupEndPoint\NUM
    ENDM

    ; Address table for dQHPackCapEndPoint_\NUM functions 
BaseAdddQHPackCapEndPoint
    DUPF    NUM,0,(MAX_NUM_EP-1)
        dc      dQHPackCapEndPoint\NUM
    ENDM


; Delete after test
USB_OUT    equ     0

