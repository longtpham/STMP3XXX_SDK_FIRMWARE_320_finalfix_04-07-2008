;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2001
;
;  File        : syspll.asm
;  Description : PLL System Routines
;///////////////////////////////////////////////////////////////////////////////

    page    255,255,0

    section SYSFUN_SysPll
        
;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include "sysequ.inc"
    include "hwequ.inc"
    list
        
;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    global  ProgramPll

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

PLLOut equ  40   ; PLL output freq (MHz)  Range: 39.3216 MHz - 77.4144 Mhz
;PLLOut equ  60   ; PLL output freq (MHz)  Range: 39.3216 MHz - 77.4144 Mhz 
pdiv   set  (@CVI(@CVI(@MIN(@MAX(@FLR((PLLOut*20.000/24.576)-32.000),0),31)))*4096)

; PLL Frequency Ranges (controlled by pdiv[4:0] bits)
;
; pdiv[4:0]   pllout (MHz)
;  0.0         39.3216
;  1.0         40.5504
;  2.0         41.7792
;  3.0         43.0080
;  4.0         44.2368
;  5.0         45.4656
;  6.0         46.6944
;  7.0         47.9232
;  8.0         49.1520
;  9.0         50.3808
; 10.0         51.6096
; 11.0         52.8384
; 12.0         54.0672
; 13.0         55.2960
; 14.0         56.5248
; 15.0         57.7536
; 16.0         58.9824
; 17.0         60.2112
; 18.0         61.4400
; 19.0         62.6688
; 20.0         63.8976
; 21.0         65.1264
; 22.0         66.3552
; 23.0         67.5840
; 24.0         68.8128
; 25.0         70.0416
; 26.0         71.2704
; 27.0         72.4992
; 28.0         73.7279
; 29.0         74.9568
; 30.0         76.1855
; 31.0         77.4144

;///////////////////////////////////////////////////////////////////////////////
;   X Memory
;///////////////////////////////////////////////////////////////////////////////
        
    org     x,"SYSFUN_SysPll_X":

;///////////////////////////////////////////////////////////////////////////////
;   Y Memory
;///////////////////////////////////////////////////////////////////////////////

    org     y,"SYSFUN_SysPll_Y":
    
;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org     p,"SYSFUN_SysPll_P":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           ProgrammPll
;
;   Type:           Function
;
;   Description:    Old - should remove
;
;   Inputs:         ???
;
;   Outputs:        ???
;                   
;   Notes:          ???
;<
;///////////////////////////////////////////////////////////////////////////////
ProgramPll

    if (@def('DEVICE_3400')||@def('DEVICE_3300'))
        ; Set the bypass_bffs bit
        ;  No documentation yet from Design... word of mouth from Danny.
        bset    #HW_CMTR_USBFFB_BITPOS,x:HW_CMTR
    endif        
    
    ;
    ; Setup the PLL.  USB requires that we run at 48Mhz.
    ;
    ; TODO - abstract this.  In general, we won't know what speed we'll be
    ;   using.  However, for quite a while we'll only be using USB input.
    ;
    ; Setup the PLL.
    ;
    ; pdiv = (20*PLLOut/24.576MHz)-32
    ; PLLOut is frequency in MHz (specified in equates)
    ;
    ; The bits in clock control register are:
    ;   (X:EC00 or X:HW_CCR)
    ; Bit       Value
    ; -----------------------------
    ; Rsrv      0000        
    ; lock      0           
    ; acken     1           
    ; pwdn      0           
    ; pdiv      derived from PLLOut     
    ; ddiv      000         
    ; cksrc     0           
    ; adiv      000         
    ; flb       0           
    ; xtlen     1     ;**** this bit must be set to get clocks to analog           
    ; pllen     1           
    ; ltc       1           
    ; ckrst     1           
    ; 0000 0 1 0 xxxxx 000 0 000 0 1 1 1 1
    ; 0000 010x xxxx 0000 0000 1111 

    move    #>$04000f,x0
    move    #>pdiv,a
    or      x0,a
    move    a1,x:HW_CCR

    ; delay for 3*10k clocks.  Only need to wait for 12.288k but what the heck.
    clr     a
    move    #>10000,a0
_wait_loop
    dec     a
    jne     _wait_loop
    
    ; we're locked, so change the digital clock source to PLL
    move    x:HW_CCR,a1
    bset    #HW_CCR_CKSRC_BITPOS,a
    move    a1,x:HW_CCR

EndPLLInit
    rts


    endsec
        
