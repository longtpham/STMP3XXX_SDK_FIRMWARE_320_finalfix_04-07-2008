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
        include "regsclkctrl.inc"
        include "regscore.inc"

        list
        
        section PLL_SPEED
    global FProgramPll, ProgramPll;

PLL_LOCK_DELAY_LOW equ $001388

    org     p,"PLL_SPEED_P":
;///////////////////////////////////////////////////////////////////////////////
;   ProgramPll
;   Entry:
;       x0 = Frequency Mask
;           48MHz = $007000
;           
;///////////////////////////////////////////////////////////////////////////////
FProgramPll
    move a,x0
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
    bset    #HW_CCR_CKSRC_BITPOS,x1
    move    x1,x:HW_CCR
    bset    #HW_SR_C_BITPOS,sr
    rts        

    endsec