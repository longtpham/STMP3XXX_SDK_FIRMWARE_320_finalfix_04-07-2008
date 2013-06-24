;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2002-2003
;
;  File        : 3500bootusb.asm
;  Description : USB Boot module
;///////////////////////////////////////////////////////////////////////////////

    page    132,60,0,0,0
        
            
;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

        nolist
        include "sysequ.inc"
        include "regsclkctrl.inc"
        include "regsusb20.inc"
		include "regsusb20phy.inc"
		include "regsicoll.inc"
		include "regscore.inc"
        list
        

        section BOOTROM_USB

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////
        global  UsbReceive,UsbInitPort,UsbExit
        global  ProcessUsbEp1Data
        global  UsbError
        global  DetectUsb
        global  DelayLongB
        
;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

        extern  ROM_SIZE
        extern  BootInfo
        extern  Fusb_device_recv_data,FInitUsb20,Fusb_device_wait_configuration
        extern  Fusb_init_ep1
        extern  SetupTimeOut,CheckTimeOut

        extern  USB_SINKBUFPTR
        extern  USB_SINKHEADPTR,USB_SINKTAILPTR
        extern  UsbPageState,UsbLagniappe,UsbEp1IrqPending,UsbProgress

        extern  UsbInfo    

        extern  BOOT_INFO_ATTEMPT         
        extern  BOOT_INFO_SUCCESS         
        extern  BOOT_INFO_TIMEOUT
    
        extern  USB_INFO_USB_DETECTED    
        extern  USB_INFO_USB_DISCONNECTED
        extern  USB_INFO_PLL_TIMEOUT      
        extern  USB_INFO_BOOT_TIMEOUT

        extern  TimeOutInitHigh
        extern  TimeOutInitLow      
        extern  TIMEOUT_USB_HIGH
        extern  TIMEOUT_USB_LOW
        
        extern  FdQH_Base_Add
        
        extern  USB_SINKBUFSIZE
        extern  USB_SINKMODULO
        
        extern  gUsbConnected

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

USB20_BUFSIZE       equ     $0016
USB20_BUFMODULO     equ     $FFFF

USB_PAGESTATE_A     equ     1
USB_PAGESTATE_B     equ     2
USB_PAGESTATE_C     equ     3

USB_EP1IRQPEND      equ     0

USB_PROG_ANALOG     equ     0
USB_PROG_ICOLL      equ     1
USB_PROG_USBENABLED equ     3
USB_PROG_IRQENABLED equ     4

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
    org x,"USB20_Buffer":
Usb20Buffer     ds  USB20_BUFSIZE
; EP1Initialized  ds  1
        
;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////
        
;///////////////////////////////////////////////////////////////////////////////
;   Code
;///////////////////////////////////////////////////////////////////////////////

;        org p(MC_RT_USB_BOOT):,p(MC_LT_USB_BOOT):
;        org p:$3e7,p:$13e7
;        org p:$3e7
    org     p,"BOOTROM_Usb_P":
    
;///////////////////////////////////////////////////////////////////////////////
;   UsbInitPort
;///////////////////////////////////////////////////////////////////////////////
UsbInitPort

    ;Init EP1 Initialized Status
;    clr     a
;    move    a1,x:EP1Initialized

    ; set the boot attempt flag
    bset    #BOOT_INFO_ATTEMPT,x:BootInfo
    
    ; init the timeout
    move    #>TIMEOUT_USB_HIGH,x0
    move    x0,x:TimeOutInitHigh
    move    #>TIMEOUT_USB_LOW,x0
    move    x0,x:TimeOutInitLow

    ; Detect USB
    ; We do not need to debounce USB connect signal, this is done
    ; at startup
    jsr     DetectUsb
    jcs     _usb_detected

    jmp     UsbError

_usb_detected
    bset    #USB_INFO_USB_DETECTED,x:BootInfo
    
    ; clear progress
    move    #>$000000,x0
    move    x0,x:UsbProgress

    WARN '*************** See Danny for comments on this ***************** '

    ; Set the bypass_bffs bit
    ;  No documentation yet from Design... word of mouth from Danny.
;    bset    #ATBR_BYPBFFS,x:ATBR
    
    WARN '*************** Add code to setup power for the USB2.0 '

    ; Add code to program power on the USB modules
    ; Run the PLL at 60MHz
    jsr     ProgramUsbClock

    ; set progress
    bset    #USB_PROG_ANALOG,x:UsbProgress
    
    ; Steer USB interrupts onto IVL1.  Actually, steer everything by just
    ;   writing 0's to all the steering registers
    move    #>$000000,x0
    move    x0,x:HW_ICLSTEER0R
    move    x0,x:HW_ICLSTEER1R
    move    x0,x:HW_ICLSTEER2R
    
    ; Set the USB interrupt priorities:
    ;  for now we'll go with equal priorities
    move    #>$000000,x0
    move    x0,x:HW_ICLPRIOR0R
    move    x0,x:HW_ICLPRIOR1R
    move    x0,x:HW_ICLPRIOR2R
    move    x0,x:HW_ICLPRIOR3R
    
    ; Enable ICOLL interrupts
    bset    #HW_ICLENABLE0R_SEN20_BITPOS,x:HW_ICLENABLE0R    
    bset    #HW_ICLENABLE0R_SEN21_BITPOS,x:HW_ICLENABLE0R    
    bset    #HW_ICLENABLE0R_SEN22_BITPOS,x:HW_ICLENABLE0R    
    
    ; set progress
    bset    #USB_PROG_ICOLL,x:UsbProgress


    ;
    ; Init the sink head & tail ptrs
    ;
    move    #USB_SINKBUFPTR,x0
    move    x0,x:USB_SINKHEADPTR
    move    x0,x:USB_SINKTAILPTR

    ; Set initial page state
    move    #>USB_PAGESTATE_A,x0                    ; initial state is A
    move    x0,x:UsbPageState

    ;
    ; Enable the USB port
    ;
    ; What about USB detect interruption. I do not wish to enable it
    ; I just want to poll it. Is this OK ????
    
    move    #>FdQH_Base_Add,x0                      ; Initialize DMA ARC Offset
    move    x0,x:HW_USBDMAOFF

    bclr    #HW_USBCSR_USBEN_BITPOS,x:HW_USBCSR     ; Make sure the port is disabled
    bclr    #HW_USBCSR_CLKOFF_BITPOS,x:HW_USBCSR    ; Turn ON the USB port clock
    bset    #HW_USBCSR_USBEN_BITPOS,x:HW_USBCSR     ; Enable STMP USB state machines
    
    ; set progress
    bset    #USB_PROG_USBENABLED,x:UsbProgress

    ;
    ; Enable interrupts
    ;    
    bclr    #9,sr
    bclr    #8,sr

    ; Enable USB arc and setup the services
    jsr     FInitUsb20

    ;; Enable IV1 interrupt.
    move    #>$0,x0
    bset    #13,x0
    bset    #12,x0
    movep   x0,x:HW_IPR    

    ; set progress
    bset    #USB_PROG_IRQENABLED,x:UsbProgress

    rts


;///////////////////////////////////////////////////////////////////////////////
;   USB Receive
;
;   Must be preserved: y1
;///////////////////////////////////////////////////////////////////////////////
UsbReceive
    ;   Wait for the USB Device to be configured before starting reception
    jsr     Fusb_device_wait_configuration
    
;    clr     a
;    move    x:EP1Initialized,a1
;    tst     a
;    jne     _EP1IsInitialized

;    jsr     Fusb_init_ep1
;    move    #>1,a1
;    move    a1,x:EP1Initialized

;_EP1IsInitialized
    ; setup the timeout counter
    jsr     SetupTimeOut
    
    ;
    ; If there's enough space in the sink buf, enable Bulk In endpoint
    ;
_FindData
    move    x:UsbEp1IrqPending,x0
    ; If we get interrupted here then the ISR will clear the semaphore, and when
    ;   we get back from the ISR the following jset will be testing an old
    ;   value.  The old value will be 1, so we'll take the branch.  This is
    ;   benign, though, because we'll fall through and return some data and
    ;   when we get back we'll fall through the jset because the move
    ;   instruction above will now get a 0 for the semaphore. We'll fall through
    ;   the jset and check to see if there's enough room.  If enough, we'll
    ;   set the semaphore and enable the EP.  If not, we'll leave the EP
    ;   disabled.
    jset    #USB_EP1IRQPEND,x0,_LessThan64BytesInSinkBuf
    ; See if there's enough room in the sink buffer to transfer the 
    ;   entire Ep1Buf
    move    #>USB_SINKBUFSIZE,y0    ; y0=buffer size
    move    x:USB_SINKHEADPTR,x0    ; a=head ptr
    move    x:USB_SINKTAILPTR,a     ; x0=tail ptr
    move    a,r1                    ; save tail ptr
    sub     x0,a                    ; a=tail-head
    jgt     _NoModuloCorrect        
    add     y0,a                    ; perform modulo correction
_NoModuloCorrect    
    move    a1,x1                   ; save available space
    move    #>USB20_BUFSIZE,x0
    cmp     x0,a
    jle     _LessThan64BytesInSinkBuf

    ; There's enough space - enable EP1 so we can get more data from
    ;  the USB.
    
    ; set the irq pending semaphore
    bset    #USB_EP1IRQPEND,x:UsbEp1IrqPending
    
    ; Prepare the reception on endpoint 1
    move    #>1,a1                  ; Endpoint 1
    move    #>Usb20Buffer,r0        ; Data buffer pointer
    move    #>0,b1                  ; Starting at byte offset 0
    move    #>64,x0                 ; We like to receive 64 bytes
    move    #>0,y0                  ; It is not a data phase on a setup transfer
    jsr     Fusb_device_recv_data   ; Function returns 0 if success
    move    #0,x0
    cmp     x0,a
    jne     UsbError



_LessThan64BytesInSinkBuf    
    ; see if there's any data in the sink buffer
    move    #>USB_SINKBUFSIZE,y0    ; y0=buffer size
    move    x:USB_SINKHEADPTR,x0    ; a=head ptr
    move    x:USB_SINKTAILPTR,a     ; x0=tail ptr
    move    a,r1                    ; save tail ptr
    sub     x0,a                    ; a=tail-head
    jgt     _NoModuloCorrect2       
    add     y0,a                    ; perform modulo correction
_NoModuloCorrect2    
    ; find out if we have any data
    move    #>USB_SINKBUFSIZE,x1
    cmp     x1,a
    jne     _data_found
    jsr     CheckTimeOut
    jcs     _usb_timeout

    jmp     _FindData        

_data_found
    ; we have data so get a word & update the tail ptr
    move    #>USB_SINKMODULO,m1     ; get the modulo
    nop
    move    x:(r1)+,x0              ; get data, return it in x0
    move    r1,x:USB_SINKTAILPTR    ; update the tail ptr

    move    #-1,m1                  ; Restore r1 register to linear mode
                                    ; C functions expect it to be this way.

    rts

_usb_timeout
    bset    #BOOT_INFO_TIMEOUT,x:BootInfo
    jmp     UsbError
_usb_disconnected
    bset    #USB_INFO_USB_DISCONNECTED,x:BootInfo
    jmp     UsbError
    


;///////////////////////////////////////////////////////////////////////////////
;   USBExit
;///////////////////////////////////////////////////////////////////////////////
UsbExit

    WARN '*** Add code here to switch back to crystal ***'
    WARN '*** Add code here to power down the PHY ***'

    move    x:BootInfo,x0
    bset    #BOOT_INFO_SUCCESS,x0
    move    x0,x:UsbInfo
    rts




;///////////////////////////////////////////////////////////////////////////////
;   UsbError
;///////////////////////////////////////////////////////////////////////////////
UsbError
    move    x:BootInfo,x0
    move    x0,x:UsbInfo
    if ("SHTDN"=="FALSE")
        debug
    else
        nop
    endif ; if ("SHTDN"=="FALSE")

    WARN '**** See with Hardware folks for shutdown part sequence ********'
    ; shut down the part
    debug
    


;///////////////////////////////////////////////////////////////////////////////
;   USB EP1 ISR
;
;   a1 = Number of bytes received
;///////////////////////////////////////////////////////////////////////////////
ProcessUsbEp1Data    
    
    ; clear the irq pending semaphore
    bclr    #USB_EP1IRQPEND,x:UsbEp1IrqPending

    move    a1,x0
    clr     a
    move    x0,a0
    
    ;
    ; Copy Usb20Buffer to UsbSinkBuf & return
    ;    

    ; setup the buffer pointers
    move    #Usb20Buffer,r0
    move    #USB20_BUFMODULO,m0
    move    x:USB_SINKHEADPTR,r1
    move    #USB_SINKMODULO,m1
    
    ; go to the current page state
    move    x:>UsbPageState,a
    move    #>USB_PAGESTATE_A,x0
    cmp     x0,a                    
    jeq     pagestatea                
    move    #>USB_PAGESTATE_B,x0
    cmp     x0,a                    
    jeq     pagestateb                
    move    #>USB_PAGESTATE_C,x0
    cmp     x0,a                    
    jeq     pagestatec                

pagestatea
    ; in Page State A, Ep byte 0 is aligned with EpBuf byte 0
    do      #21,_done_a
    move    x:(r0)+,x0                          ; get data    
    move    x0,x:(r1)+                          ; write to sink buf
_done_a
    move    x:(r0),x0                           ; get lagniappe
    move    x0,a
    move    #>$0000FF,x1                        ; mask off upper bytes
    and     x1,a
    move    a1,x:UsbLagniappe                   ; save lagniappe
    move    r1,x:USB_SINKHEADPTR                ; update the UsbSinkHeadPtr
    move    #>USB_PAGESTATE_B,x1                ; next state
    move    x1,x:UsbPageState
    jmp     usb_rx_done                         ; done
    
pagestateb
    ; in Page State B, Ep byte 1 is aligned with EpBuf byte 0
    move    #>$000080,x1                        ; setup the shift up factor
    move    x:UsbLagniappe,y0                   ; get lagniappe
                                                ; y0=__1100
    do      #21,_done_b
    move    x:(r0)+,x0                          ; get next
    mpy     x0,x1,a         #>$0000FF,x0        ; mask lower 2 bytes into upper lagniappe 2 bytes
                                                ; a1=____00, a0=2211__
    and     x0,a                                ; mask of artificats of signed multiply
    move    a1,y1                               ; temp save lagniappe
    move    a0,a1
    or      y0,a                                ; perform the mask
    move    a1,x:(r1)+                          ; write to sink buf
    move    y1,y0                               ; setup lagniappe
_done_b
    move    x:(r0),x0                           ; get last partial word
    move    #>$008000,y1                        ; setup the shift factor
    mpy     x0,y1,a         #>$00FF00,x0
    move    a0,x0
    mpy     x0,y1,a         #>$00FF00,x0
    and     x0,a            
    or      y0,a
    move    a1,x:UsbLagniappe                   ; save lagniappe
    move    r1,x:USB_SINKHEADPTR                ; update the UsbSinkBufPtr
    move    #>USB_PAGESTATE_C,x1                ; next state
    move    x1,x:UsbPageState
    jmp     usb_rx_done
    
pagestatec
    ; in Page State B, Ep byte 2 is aligned with EpBuf byte 0
    move    #>$008000,x1                        ; setup the shift up factor
    move    x:UsbLagniappe,y0                   ; get lagniappe
                                                ; y0=____00
    do      #22,_done_c
    move    x:(r0)+,x0                          ; get next
    mpy     x0,x1,a         #>$00FFFF,x0        ; mask lower byte into upper lagniappe byte
                                                ; a1=__1100, a0=22____
    and     x0,a                                ; mask of artificats of signed multiply
    move    a1,y1                               ; temp save lagniappe
    move    a0,a1
    or      y0,a                                ; perform the mask
    move    a1,x:(r1)+                          ; write to sink buf
    move    y1,y0                               ; setup lagniappe
_done_c
                                                ; no lagniappe to save!
    move    r1,x:USB_SINKHEADPTR                ; update the UsbSinkBufPtr
    move    #>USB_PAGESTATE_A,x1                ; next state
    move    x1,x:UsbPageState
    
    
usb_rx_done

    move    #-1,m1                  ; Restore r1 register to linear mode
                                    ; C functions expect it to be this way.

    rts

;///////////////////////////////////////////////////////////////////////////////
;   DetectUsb
;
;   3500 is powered by the USB +5V. At startup, the USB is tested for presence
;   and the bit 0 of global variable gUsbConnected is set if USB connected
;   and clear otherwise. This is the bit that is tested within this function
;   to report USB detection. The global variable gUsbConnected is not updated
;   anywhere in the code after startup. If UsB gets disconnected, we loose power
;   and the part automatically shuts down.
;///////////////////////////////////////////////////////////////////////////////
DetectUsb

    move    x:gUsbConnected,x0
    btst    #0,x0

    rts    

;///////////////////////////////////////////////////////////////////////////////
;   DelayLongB
;
;///////////////////////////////////////////////////////////////////////////////
DelayLongB
    dec b
    jne DelayLongB
    rts

;///////////////////////////////////////////////////////////////////////////////
;   ProgramUsbClock
;   
;       USB Pll is programmed to its default value of 60Mhz        
;///////////////////////////////////////////////////////////////////////////////
ProgramUsbClock
    ; From Steve Vu information
    bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR      ; Enables writes to Clock Control Register
    bset    #HW_CCR_PLLEN_BITPOS,x:HW_CCR      ; Enable (old 3410) PLL because the MUX 
                                        ; switching from old Pll to new (USB) PLL
                                        ; requires a clock during this phase (prevents glitches)

    move    #>$800000,x0
    move    x0,x:HW_USBPHYPWD           ; Force all PLL regs to default state
    move    #>$000000,x0
    move    x0,x:HW_USBPHYPWD           ; Power ON the PHY

    if (!@DEF('SIM'))
        bset    #HW_USBRX_PLLLOCKED_BITPOS,x:HW_USBPHYRX ;Start PLL lock Sequence    
    endif
    
    clr     a
    move    #>PLL_LOCK_DELAY_LOW,a0

_wait_loop
    ; We will wait a maximum of 200Us
    move    x:HW_USBPHYRX,x0
    jset    #HW_USBRX_PLLLOCKED_BITPOS,x0,_locked   ; Wait for PLL to lock    
    dec     a
    jne     _wait_loop
    
_locked
    bset    #HW_CCR_PLL_SRC_SEL_BITPOS,x:HW_CCR                         ; Select the new PLL (USB)
    bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR                               ; Route clock from Crystal to PLL
    
    bclr    #HW_CCR_PLLEN_BITPOS,x:HW_CCR       ; Disable (old 3410) PLL now that we have
                                        ; the new one locked

    bclr    #HW_USBCSR_CLKOFF_BITPOS,x:HW_USBCSR    ; Turns ON clock for USB Block
                                                    ; ARC + STMP USB INterface
    rts    

    if (!@DEF('SIM'))

;///////////////////////////////////////////////////////////////////////////////
;   ProgramPll
;   Entry:
;       x0 = Frequency Mask
;           48MHz = $007000
;           
;///////////////////////////////////////////////////////////////////////////////
ProgramPll
    ;
    ; Setup the PLL.  USB requires that we run at 48Mhz.
    ;
    ; pdiv = (20*F/24.576MHz)-32
    ; F = 48MHz, so
    ; pdiv = 7.0625 = 7, so Factual = 47.9232MHz
    ;
    ; Bit       Value
    ; -----------------------------
    ; Rsrv      0000        
    ; lock      0           
    ; acken     1           
    ; pwdn      0           
    ; pdiv      00111       
    ; ddiv      000         
    ; cksrc     0           
    ; adiv      000         
    ; flb       0           
    ; xtlen     0           
    ; pllen     1           
    ; ltc       0           
    ; ckrst     1           
    ; 0000 0 1 0 00111 000 0 000 0 0 1 0 1
    ; 0000 0100 0111 0000 0000 0101 
    move    #>$040005,a
    or      x0,a
    move    a1,x1
    move    x1,x:HW_CCR 
    
    ; Start the lock timer counter (ltc)
    ; Bit       Value
    ; -----------------------------
    ; Rsrv      0000        
    ; lock      0           
    ; acken     1           
    ; pwdn      0           
    ; pdiv      00111       
    ; ddiv      000         
    ; cksrc     0           
    ; adiv      000         
    ; flb       0           
    ; xtlen     0           
    ; pllen     1           
    ; ltc       1           
    ; ckrst     1           
    ; 0000 0 1 0 00111 000 0 000 0 0 1 1 1
    ; 0000 0100 0111 0000 0000 0111 
    bset    #HW_CCR_LTC_BITPOS,x1
    move    x1,x:HW_CCR

    ; delay for 3*10k clocks.  Only need to wait for 12.288k but what the heck.
    clr     a
    move    #>PLL_LOCK_DELAY_LOW,a0
_wait_loop
    dec     a
    jne     _wait_loop

    ; we're locked, so change the digital clock source to PLL
    bset    #HW_CCR_CKRST_BITPOS,x1
    move    x1,x:HW_CCR
    bset    #HW_SR_C_BITPOS,sr
    rts        

    endif
    
    endsec
