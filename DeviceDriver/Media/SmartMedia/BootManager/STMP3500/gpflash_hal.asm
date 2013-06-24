;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: gpflash_hal.asm
; Description: GPFLASH Interface Hardware Abstration Layer
;///////////////////////////////////////////////////////////////////////////////

    page    132,60,0,0,0

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

        nolist
        include "sysequ.inc"
        include 'regsgpflash.inc'
        include 'regsgpio.inc'
        include '..\common\stack.inc'
        list

        section BOOTROM_FLASH

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global  InitGPFLASHPort
    global  FInitGPFLASHPort
    global  TerminateGPFLASHPort
    global  FTerminateGPFLASHPort
    global  WriteCmdGPFLASH
    global  FWriteCmdGPFLASH
    global  WriteAddrGPFLASH
    global  FWriteAddrGPFLASH
    global  ReadDataGPFLASH
    global  FReadDataGPFLASH
    global  WaitForReadyGPFLASH
    global  FWaitForReadyGPFLASH


;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    EXTERN    DelayLongBReg
    nolist

    list
    

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////
                            ; A DMA transfer should never really time out, unless the GPFLASH
                            ; hardware is broken or it gets programmed incorrectly.  Otherwise,
                            ; the appropriate number of strobes will happen, whether or not the
                            ; flash chip is ready.  So set this timeout for as long as possible
                            ; and don't expect it to ever happen.
NAND_XFER_TIMEOUT       equ     $7fffff

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
;   Notes
;
;   FOO - This code should be optimized when adapted to SDK device drivers.
;         There are many quick-and-dirty bits in here, such as doing "REP-SHIFT"
;         loops instead of multiplies.  And so on...
;///////////////////////////////////////////////////////////////////////////////


; Set GPFlash port for worst-case 4 nand multinand for now.
;    ifdef @('NAND1')
;GPFLASH_PERIOD      equ     $302
;    else
;    ifdef @('NAND2')
;GPFLASH_PERIOD      equ     $304
;    else
;    ifdef @('NAND4')
;GPFLASH_PERIOD      equ     $305
;    else
;GPFLASH_PERIOD       equ     $302
;    endif   
;    endif   
;    endif   
GPFLASH_PERIOD      equ     $305

;///////////////////////////////////////////////////////////////////////////////
;> Name: InitGPFLASHPort
;
;  Type: Function
;  Description: 
;  Inputs: 
;           A   -   Card number
;  Outputs: 
;  Notes:   C callable:  void  InitGPFLASHPort(int iCardNumber)
;<
;///////////////////////////////////////////////////////////////////////////////
FInitGPFLASHPort
InitGPFLASHPort
    
    ; set the GPFlash pads to 8mA
    move    #>7,a
    move    a,x:HW_GP08MA
    move    a,x:HW_GP18MA
    move    a,x:HW_GP28MA
    move    a,x:HW_GP38MA
    
    ; set the GPFlash timing to something safe enough for four nands
    bclr    #HW_GPFLASH_CSR0_CLK_DISABLE_BITPOS,x:HW_GPFLASH_CSR0R
    move    #>GPFLASH_PERIOD,a
    move    a,x:HW_GPFLASH_TIMING2R    
    
    clr     b
    move    #>WAIT_10_MS,b0
    asl     b                       ; delay for ~20ms
    jsr     DelayLongBReg
    
    ; Disable Write Protect# (High) - Renesas has RES# tied to WP#
    bset    #HW_GPFLASH_CSR1_WP0_BITPOS,x:HW_GPFLASH_CSR1R
    bset    #HW_GPFLASH_CSR1_MUX_BITPOS,x:HW_GPFLASH_CSR1R
    
    clr     b
    move    #>WAIT_10_MS,b0
    asl     b                       ; delay for ~20ms
    jsr     DelayLongBReg
    
    clr     a                       ; Status - SUCCESS

    rts



;///////////////////////////////////////////////////////////////////////////////
;> Name: TerminateGPFLASHPort
;
;  Type: Function
;  Description: 
;  Inputs: 
;           A   -   Card number
;  Outputs: 
;  Notes:   C callable:  void  InitGPFLASHPort(int iCardNumber)
;<
;///////////////////////////////////////////////////////////////////////////////
FTerminateGPFLASHPort
TerminateGPFLASHPort

    ; No pins to return to default state

    clr     a                       ; Status - SUCCESS

    rts





;///////////////////////////////////////////////////////////////////////////////
;> Name: WriteCmdGPFLASH
;
;  Type: Function
;  Description: 
;  Inputs: 
;           A   -   Current Device Number
;           B   -   NAND Command Code   (e.g. $70 for Read Status)
;           X0  -   Deassert CEn at end, if this flag is non-zero
;  Outputs: 
;  Notes: 	C-callable:	int WriteCmdGPFLASH(int iDeviceNum, int iCommandCode)
;<
;///////////////////////////////////////////////////////////////////////////////
FWriteCmdGPFLASH
WriteCmdGPFLASH

CSR0_CMD_SETUP  equ (HW_GPFLASH_CSR0_WRITE+HW_GPFLASH_CSR0_CLEAR_INT+HW_GPFLASH_CSR0_CMD_XFER+HW_GPFLASH_CLOCK_ENABLE)


CSR1_CMD_SETUP  equ (HW_GPFLASH_USE_X_MEMORY+HW_GPFLASH_START_BYTE_LSB+HW_GPFLASH_MUX_GPFLASH_ACCESS+HW_GPFLASH_8BIT_ACCESS)




    move    r7,r0                   ; Save stack pointer;  will point to Command buffer
    push    b1                      ; Store the command in the command buffer

    push    a                       ; Save Device Number




    ;///////////  Setup CSR0    ///////////////

    move    #>CSR0_CMD_SETUP,b           ; Set up CSR0

    move    x0,a                        ; if (DeassrtCEn)
    tst     a
    jeq     _set_CSR0                   ;   Set CEB CTRL to "go HIGH at end"
        bset    #HW_GPFLASH_CSR0_CEB_CTRL_BITPOS,b1
_set_CSR0 
    move    b,x:HW_GPFLASH_CSR0R





    ;///////////  Setup CSR1 ///////////////
    pop     a                               ; Restore DeviceNum
    move    #>3,x1                           ; Mask DeviceNum, just for safety
    and     x1,a
    rep     #HW_GPFLASH_CSR1_CEB_BITPOS     ; Then shift it to the right position (CEB)
        asl a

    move    #>CSR1_CMD_SETUP,x1             ; Then add the rest of the CSR1 setup
    or      x1,a

    push    a
    move    x:HW_GPFLASH_CSR1R,a            ; Read current CSR1
    move    #>HW_GPFLASH_WPn_MASK,x1        ; Keep just the WPn bits
    and     x1,a
    pop     x1                              ; Combine with the new settings
    or      x1,a                            

    move    a1,x:HW_GPFLASH_CSR1R           ; And stuff the register




    ;///////////  Setup DMA     ///////////////
    move    r0,x:HW_GPFLASH_DMA_ADDR        ; Point to Command on stack



    ;///////////  Setup DMA     ///////////////
    move    #>1,y0
    move    y0,x:HW_GPFLASH_XFER_SIZER



    ;///////////  Setup Timing  ///////////////
    ; ??
    ;   LEAVE TIMING SETUP AT DEFAULT SETTINGS, FOR NOW




    ;///////////  Setup Busy    ///////////////
    ; ??
    ;   NO BUSY INTERRUPTS, FOR NOW

    clr     a
    move    a,x:HW_GPFLASH_TIMINGBUSYR



    ;///////////  Kick the operation    ///////////////
    bset    #HW_GPFLASH_CSR0_KICK_BITPOS,b1     ; CSR0 setup still in B, Add "KICK" bit
    move    b,x:HW_GPFLASH_CSR0R





    ;///////////  Wait for COMPLETE    ///////////////
    jsr     WaitForComplete


    pop     b                       ; Pop the Command buffer off the stack

    clr     a                       ; Return SUCCESS

    rts







;///////////////////////////////////////////////////////////////////////////////
;> Name: WriteAddrGPFLASH
;
;  Type: Function
;  Description: 
;  Inputs: 
;           A   -   Current Device Number
;           B   -   Number of Column Address bytes
;           X0  -   Column Address (byte offset into Page)
;           Y0  -   Number of Row Address bytes
;           X1  -   Row Address (Page number)
;           Y1  -   Deassert CEn at end, if this flag is non-zero
;  Outputs: 
;  Notes:   C-callable: int WriteAddrGPFLASH(iDeviceNum, iNumColBytes, iColumnAddress,
;                                                   iNumRowBytes, iRowAddress)
;<
;///////////////////////////////////////////////////////////////////////////////
FWriteAddrGPFLASH
WriteAddrGPFLASH

CSR0_ADDR_SETUP  equ    (HW_GPFLASH_CSR0_WRITE+HW_GPFLASH_CSR0_CLEAR_INT+HW_GPFLASH_CSR0_ADDR_XFER+HW_GPFLASH_CLOCK_ENABLE)
CSR1_ADDR_SETUP  equ    (HW_GPFLASH_USE_X_MEMORY+HW_GPFLASH_START_BYTE_LSB+HW_GPFLASH_MUX_GPFLASH_ACCESS+HW_GPFLASH_8BIT_ACCESS)


                        ; The Column Address and Row Address are passed in separately.
                        ; For use with the GPFLASH, these need to be concatenated into
                        ; one continuous DMA transfer;  otherwise, the ALE line may cause
                        ; trouble.

                        ; The size each address (Column and Row) can be from 0-3 bytes.
                        ; So the complete address can be up to two 24-bit words.  The
                        ; ordering is that the LSB of the Column Address (if present)
                        ; goes in the LSB of the first (lower address) word.  Next come
                        ; the rest of the Column address bytes (if present), then the
                        ; Row address bytes.  So, depending on the sizes of each address,
                        ; the second word may not be used.  (The DMA setup will take care 
                        ; of this.

                        ; To deal with this, the code below loads the Row address into the
                        ; low word of a 56-bit accumulator.  Then it shifts this up 8 bits
                        ; for each byte in the Column Address (if any).  Then it adds in the
                        ; Column address in the low word.

                        ; This assumes that the Column address has no extraneous data in
                        ; any unused higher bytes, which should be true when called from C.


    move    r7,r0                   ; Save ptr to Addr DMA buffer (to be)
    move    (r7)+
    move    (r7)+

    push    a1                      ; Save Device Number

    clr     a
    move    x1,a0                   ; A = Row Addr (in LSW)

    move    b1,x1                   ; Save nColBytes in x1, so we can use B

    tst     b
    jeq     ColsDone                ; if (nColBytes>0) {
    do      b1,ColLoop              ;   for (each Col addr byte) {
        rep #8                      ;       shift row addr left one byte
            asl a
    nop                             
ColLoop                             ;   }

    clr     b
    move    x0,b0                   ;   B0 = Col Addr
    add     b,a                     ;   A = A + B  // Full Address (Rows and Cols)
ColsDone                            ; }

    move    r0,r1                   ; save ptr to DMA buffer
    move    a0,x:(r0)+              ; put Addr into DMA buffer
    move    a1,x:(r0)+

    move    y0,a
    add     x1,a
    move    a,x0                    ; x0 = NRowBytes + NColBytes






    ;///////////  Setup CSR0    ///////////////
    move    #>CSR0_ADDR_SETUP,a

    move    y1,b                            ; if (DeassrtCEn)
    tst     b
    jeq     _set_CSR0                       ;   Set CEB CTRL to "go HIGH at end"
        bset    #HW_GPFLASH_CSR0_CEB_CTRL_BITPOS,a1
_set_CSR0 
    move    a1,x:HW_GPFLASH_CSR0R           ; and stuff the register

    pop     x1                              ; x1 = Device Number

    push    a1                              ; Save CSR0settings for later



    ;///////////  Setup CSR1 ///////////////
    move    x1,a                            ; A = Device Number

    move    #>3,x1                           ; Mask DeviceNum, just for safety
    and     x1,a
    rep     #HW_GPFLASH_CSR1_CEB_BITPOS     ; Then shift it to the right position (CEB)
        asl a

    move    #>CSR1_ADDR_SETUP,x1            ; Then add the rest of the CSR1 setup
    or      x1,a

    push    a
    move    x:HW_GPFLASH_CSR1R,a             ; Read current CSR1
    move    #>HW_GPFLASH_WPn_MASK,x1         ; Keep just the WPn bits
    and     x1,a
    pop     x1                               ; Combine with the new settings
    or      x1,a                            

    move    a1,x:HW_GPFLASH_CSR1R            ; And stuff the register





    ;///////////  Setup DMA     ///////////////
    move    r1,x:HW_GPFLASH_DMA_ADDR        ; Point to Command on stack




    ;///////////  Setup XFER SIZE     ///////////////
    move    x0,a                            ; Put xfer size (NColBytes+NRowBytes)
    move    a,x:HW_GPFLASH_XFER_SIZER



    ;///////////  Setup Timing  ///////////////
    ; ??
    ;   LEAVE TIMING SETUP AT DEFAULT SETTINGS, FOR NOW





    ;///////////  Setup Busy    ///////////////
    ; ??
    ;   NO BUSY INTERRUPTS, FOR NOW

    clr     b
    move    b,x:HW_GPFLASH_TIMINGBUSYR





    ;///////////  Kick the operation    ///////////////
    pop     a                                   ; Restore CSR0 settings
    bset    #HW_GPFLASH_CSR0_KICK_BITPOS,a1     ; Add "KICK" bit
    move    a,x:HW_GPFLASH_CSR0R






    ;///////////  Wait for COMPLETE    ///////////////
_wait
    jsr     WaitForComplete


    pop     a                               ; Release the DMA buffer
    pop     a

    clr     a                               ; return SUCCESS
    rts






;///////////////////////////////////////////////////////////////////////////////
;> Name: ReadDataGPFLASH
;
;  Type: Function
;  Description: 
;  Inputs: 
;           A   -   Device Number
;           B   -   Number of bytes to read
;           R0  -   Destination address (in X: memory)
;           X0  -   8/16 bit mode (0=8 bits, 1=16 bits)
;  Outputs: 
;  Notes:   C-callable:	int ReadDataGPFLASH(int iDeviceNum, int iNumBytesToRead, int *pBuffer)
;<
;///////////////////////////////////////////////////////////////////////////////
FReadDataGPFLASH
ReadDataGPFLASH

CSR0_READ_SETUP  equ  (HW_GPFLASH_CSR0_READ+HW_GPFLASH_CSR0_CLEAR_INT+HW_GPFLASH_CSR0_DATA_XFER+HW_GPFLASH_CSR0_CEB_CTRL_RETURN_TO_HIGH+HW_GPFLASH_CLOCK_ENABLE)


CSR1_READ_SETUP  equ  (HW_GPFLASH_USE_X_MEMORY+HW_GPFLASH_START_BYTE_LSB+HW_GPFLASH_MUX_GPFLASH_ACCESS)



    push    b1                                      ; Save byte count


    ;///////////  Setup CSR0    ///////////////
    move    #>CSR0_READ_SETUP,b

    move    b,x:HW_GPFLASH_CSR0R
    move    b,y1                                    ; Save CSR0 settings, for later


    ;///////////  Setup CSR1 ///////////////
    move    #>3,x1                                   ; Mask DeviceNum, just for safety
    and     x1,a
    rep     #HW_GPFLASH_CSR1_CEB_BITPOS             ; Then shift it to the right position (CEB)
        asl a

    move    x0,b                                    ; if (16 bit mode)
    tst     b
    jeq     _mode_set
        bset    #HW_GPFLASH_CSR1_MODE16_BITPOS,a1   ;   set Mode16
_mode_set

    move    #>CSR1_READ_SETUP,x1                    ; Then add the rest of the CSR1 setup
    or      x1,a

    push    a
    move    x:HW_GPFLASH_CSR1R,a                    ; Read current CSR1
    move    #>HW_GPFLASH_WPn_MASK,x1                ; Keep just the WPn bits
    and     x1,a
    pop     x1                                      ; Combine with the new settings
    or      x1,a                                    

    move    a1,x:HW_GPFLASH_CSR1R                   ; And stuff the register





    ;///////////  Setup DMA     ///////////////
    move    r0,x:HW_GPFLASH_DMA_ADDR        ; Point to Command on stack




    ;///////////  Setup XFER Size     ///////////////
    pop     b
    move    b,x:HW_GPFLASH_XFER_SIZER





    ;///////////  Setup Busy    ///////////////
    ; ??
    ;   NO BUSY INTERRUPTS, FOR NOW

    clr     b
    move    b,x:HW_GPFLASH_TIMINGBUSYR





    ;///////////  Kick the operation    ///////////////
    move    y1,b                                ; B = saved CSR0 settings
    bset    #HW_GPFLASH_CSR0_KICK_BITPOS,b1     ; Add "KICK" bit
    move    b,x:HW_GPFLASH_CSR0R                ; Stuff the register





    ;///////////  Wait for COMPLETE    ///////////////
_wait
    jsr     WaitForComplete


    clr     a                       ; Return SUCCESS

    rts


;///////////////////////////////////////////////////////////////////////////////
;> Name: WaitForReadyGPFLASH
;
;  Type: Function
;  Description:     Spin while the READY line is LOW.
;  Inputs:          iDeviceNum - This is ignored in this version, but could be
;                                used to test for a specific NAND to be be READY.
;                   iTimeoutCount - if <= 0, wait forever
;                                   else count down until READY - fail if you reach zero
;  Outputs:         0 - SUCCESS.  READY asserted
;                   1 - FAIL.  iTimeoutCount expired
;  Notes:           Timeout is simple - just spin until count expires.  So it's
;                   counting some number of INSTRUCTION CYCLE TIMES.
;                   Later verison may use a real timer, allowing asynchronous use.
;<
;///////////////////////////////////////////////////////////////////////////////
WaitForReadyGPFLASH
FWaitForReadyGPFLASH


    ;  iDeviceNum in A is ignored, for this version

    move    #>1,x0

_wait_for_ready                         ; repeat {

    move    x:HW_GPFLASH_CSR0R,y1       ;   if (NAND is READY)
    jclr    #HW_GPFLASH_CSR0_EXT_BUSY_VAL_BITPOS,y1,_not_READY
    clr     a                           ;       return SUCCESS
    rts


_not_READY
    tst     b
    jle     _wait_for_ready     ;   if (iTimeoutCount <= 0)
                                ;       wait forever

    sub     x0,b                ;   iTimeoutCount--
    jgt     _wait_for_ready     ;   if (iTimeoutCount > 0)
                                ;       keep waiting
                                ;   else
    move    #>1,a               ;       return FAIL
    rts
                                ; }




;///////////////////////////////////////////////////////////////////////////////
;> Name: WaitForComplete
;
;  Type: Function
;  Description:      For local (internal) use, only.  Not for export.
;  Inputs: 
;  Outputs: 
;  Notes: 
;<
;///////////////////////////////////////////////////////////////////////////////
WaitForComplete

    move    #>NAND_XFER_TIMEOUT,a
    move    #>1,x0

_wait_for_done                              ; repeat {

    move    x:HW_GPFLASH_CSR0R,x1           ;   if (done)
    jclr    #HW_GPFLASH_CSR0_INT_STATUS_BITPOS,x1,_not_done

    clr     a                               ;       return SUCCESS
    rts

_not_done
    sub     x0,a                            ;   decrement timeout count
    jgt     _wait_for_done                  ;   if (count <= 0)


                                            ;       TIMED OUT
    move    #>1,a                           ;       return FAIL
    rts
                                            ; }

    endsec


;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
