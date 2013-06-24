

    include "regsrtc.inc"
    include "regsclkctrl.inc"
    include "sysmacro.asm"

    global FGetTime
    global FTimeInit

    extern HW_RTCUPR
    extern HW_RTCLOWR

    ;org l,".ldataclockTick":
;Fstc_ClockTick
;    ds  1

    org p,".ptextgettime":

FGetTime:
    push    sr
    bset    #9,sr
    bset    #8,sr
    bset    #0,x:HW_CCR ;pipeline to make sure we don't get interrupted    
    nop                 ;pipeline to make sure we don't get interrupted    
    move    x:HW_RTCUPR,a
    move    x:HW_RTCLOWR,a0
    pop     sr
;    move    a10,l:Fstc_ClockTick
    rts



FTimeInit

    if (@def('DEVICE_3500'))
        ; Needs to be set if not we never get the divider value loaded
        ; from shadows register to real register
        
        
        bset    #HW_CCR_CKRST_BITPOS,x:HW_CCR

        bset    #HW_CCR_ACKEN_BITPOS,x:HW_CCR

        bset    #HW_CCR_XTLEN_BITPOS,x:HW_CCR

        move #>$2000,x0
        rep x0
            nop

        ; Remove Soft Reset
        bclr    #HW_RTC_CSR_SFTRST_BITPOS,x:HW_RTC_CSR

        ; Wait a little ...
        move #>$2000,x0
        rep x0
            nop

        ; The 3500 has a 64 pre-divider. To count 1 ms intervals
        ; with a 24Mhz Crystal, the divider must be set to 375
        ; Total Divider is 375*64 = 24000  
        move    #>375,x0

        nop
        nop
        nop
        
        ; Before writing to the RTC clock divider we must make sure that the 
        ; NEW_REGS crystal divide bit is clear
_Loop
        btst    #12,x:HW_RTC_CSR    
        jcs     _Loop
        
        move    x0,x:HW_RTC_DIVIDE
    endif

    rts
