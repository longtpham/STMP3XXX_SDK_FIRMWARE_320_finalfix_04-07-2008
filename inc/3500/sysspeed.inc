;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2004
;
; Speed defs
; NOTE: sysspeed.H is generated from sysspeed.INC. Only modify inc version.
; Speed clients: See SPEED_CLIENT_ defines and non 3400 Speed Indeces
;///////////////////////////////////////////////////////////////////////////////

    IF (!@def(SYSSPEED_INC))
SYSSPEED_INC    equ     1

; PLL Frequency Divider (Assume DDIV=0)
; PLL Output Frequency = (33+PDIV)*(XTAL/20)   XTAL=24 MHz
; PDIV = (Speed * 20 / 24) - 33  (if DDIV=0)
;
; Digital Clock Post Divider 
; Digital Clock = PLL Output Fequency / 2^DDIV   (Speed is in MHz)
;
;
; GP Flash Timing Registers
; x:HW_GPFLASH_TIMING1R
;      TSU = 0ns 
;      TH = 10ns Min
; Use Default Settings since max speed is 75.6 MHz or 13.2275 ns
; Add Register if needed
;
; x:HW_GPFLASH_TIMING2R 
;      TDS = 25ns Min  data setup time
;      TDH = 15ns Min  data hold  time
;      TDS + TDH must be > 50 nS
;
;
; Speed Indexes (into the speed table)
;!!!!! ** MUST BE ORDERED FROM LOWEST TO HIGHEST FREQ ** !!!!!
SPEED_IDLE					equ     0 ; NOTE: Executive speed client must only use speed index 1 or 0 (IDLE).
SPEED_AUDIBLE_MP3			equ		1
SPEED_AUDIBLE_ACELP_SR8KHz	equ		2
SPEED_FM        			equ    	3
SPEED_FM_EQ                 equ     4
SPEED_AUDIBLE_ACELP_SR16KHz	equ		5
SPEED_MP3					equ		6
SPEED_ADPCM					equ		7
SPEED_MIXER					equ		8
SPEED_ADCBASE				equ		9
SPEED_MAX					equ		10
SPEED_WMA					equ		11
SPEED_MP3ENCODE				equ		12
SPEED_PEAK					equ		13
SPEED_LAST					equ		SPEED_PEAK
SPEED_UNINITIALIZED			equ		SPEED_LAST+1
; SPEED_UNINITIALIZED must be the highest index value

IDLE_SPEED      equ     10              ; Idle speed is 10 MHz  (98.04nS)
IDLE_PDIV       equ     1               ; (1+33)*(24/20) = 40.8
IDLE_DDIV       equ     2               ; 40.5504 / (2^2) = 10.2 MHz
IDLE_VOLTAGE    equ     $00000B         ; 1.37v
IDLE_BOVOLTAGE  equ     $000800         ; 1.28v 


; tdh and tds: hold and setup times in n dclks are 6-bit fields that are right justified byte aligned
; 60MHz MSC uses the following clock setup: 
; NAND1     $000203 -> (2+3)/60MHz =  83.3ns   2 dclk hold and 3 dclk setup
; NAND2     $000304 -> (3+4)/60MHz = 116.7ns
; NAND3     $000305 -> (3+5)/60MHz = 133.3ns
; NAND4     $000305 -> (3+5)/60MHz = 133.3ns
;
; So, for 10MHz idle speed:
; NAND1,2,4 may use $000101 
    if (@def('DEBUG'))
    if (@def('NAND1'))
IDLE_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    else
    if (@def('NAND2'))
IDLE_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    else
    if (@def('NAND3'))
IDLE_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    else
    if (@def('NAND4'))
IDLE_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    else
IDLE_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
    endif
    endif
    endif
    else
IDLE_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
;---------    

AUDIBLE_MP3_SPEED      equ     15              ; speed is 15 MHz(66.6nS)
AUDIBLE_MP3_PDIV       equ     17              ; PLL O/P freq = (33 + PDIV)*(24/20) = (33 + 17)*(24/20) = 60MHz
AUDIBLE_MP3_DDIV       equ     2               ; DCLK = 40.8MHz/(2^DDIV) = 15MHz
AUDIBLE_MP3_VOLTAGE    equ     $00000B         ; 1.37V
AUDIBLE_MP3_BOVOLTAGE  equ     $000800         ; 1.28V 

; NAND1     15MHz *  83.3ns = 1.249 -> $000101
; NAND2     15MHz * 116.7ns = 1.750 -> $000101
; NAND3|4   15MHz * 133.3ns = 1.999 -> $000101

    if (@def('DEBUG'))
    if (@def('NAND1'))
AUDIBLE_MP3_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    else
    if (@def('NAND2'))
AUDIBLE_MP3_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND3'))
AUDIBLE_MP3_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND4'))
AUDIBLE_MP3_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
AUDIBLE_MP3_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
    endif
    endif
    endif
    else
AUDIBLE_MP3_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
;---------   

AUDIBLE_ACELP_SR8KHz_SPEED      equ     15              ; speed is 15 MHz(66.6nS)
AUDIBLE_ACELP_SR8KHz_PDIV       equ     17              ; PLL O/P freq = (33 + PDIV)*(24/20) = (33 + 17)*(24/20) = 60MHz
AUDIBLE_ACELP_SR8KHz_DDIV       equ     2               ; DCLK = 40.8MHz/(2^DDIV) = 15MHz
AUDIBLE_ACELP_SR8KHz_VOLTAGE    equ     $00000B         ; 1.37V
AUDIBLE_ACELP_SR8KHz_BOVOLTAGE  equ     $000800         ; 1.28V 

; NAND1     15MHz *  83.3ns = 1.249 -> $000101
; NAND2     15MHz * 116.7ns = 1.750 -> $000101
; NAND3|4   15MHz * 133.3ns = 1.999 -> $000101

    if (@def('DEBUG'))
    if (@def('NAND1'))
AUDIBLE_ACELP_SR8KHz_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    else
    if (@def('NAND2'))
AUDIBLE_ACELP_SR8KHz_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND3'))
AUDIBLE_ACELP_SR8KHz_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND4'))
AUDIBLE_ACELP_SR8KHz_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
AUDIBLE_ACELP_SR8KHz_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
    endif
    endif
    endif
    else
AUDIBLE_ACELP_SR8KHz_GPSPEED    equ     $000101         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
;--------- 
; Changed speed from 18Mhz to 24Mhz stmp10415
FM_SPEED        equ     24 ; stmp10415   was 18              ; FM speed is 18 MHz, which is as low as possible for Steely Dan Tuner in sdk 2.610. In 3.110 we need >22Mhz so we use 24Mhz. 
FM_PDIV         equ     7  ; stmp10415   was 27              ; (27+33)*(24/20) = 72 in sdk2.610.  In sdk 3.110 we use (7+33)*(24/20) = 48.
FM_DDIV         equ     1  ; stmp10415   was  2              ; 72 / (2^2) = 18 MHz in sdk 2.610.    In 3.110 we use 48/2^1 = 24Mhz. 

FM_VOLTAGE      equ     $00000d         ; 1.44V	- Steely Dan spec at 1.35V
FM_BOVOLTAGE    equ     $000800         ; 1.28v
    if (@def('DEBUG'))
    if (@def('NAND1'))
FM_GPSPEED     equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
    if (@def('NAND2'))
FM_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
    if (@def('NAND3'))
FM_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
    if (@def('NAND4'))
FM_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
FM_GPSPEED     equ      $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    endif
    endif
    endif
    endif
    else
FM_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    endif
;---------
FM_EQ_SPEED     equ     30  ; Increase to 30 MHz to support equalizer
FM_EQ_PDIV      equ     17  ; (17+33)*(24/20) = 60
FM_EQ_DDIV      equ     1   ;  60 / (2^1) = 30 MHz  

; Higher voltages when using equalizer and TA2 STFM1000 to prevent lockup when changing EQ settings rapidly
FM_EQ_VOLTAGE   equ    $00000D   ;1.44v  - Steely Dan spec at 1.35V
FM_EQ_BOVOLTAGE equ    $000900   ;1.31v     

    if (@def('DEBUG'))
    if (@def('NAND1'))
FM_EQ_GPSPEED     equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
    if (@def('NAND2'))
FM_EQ_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
    if (@def('NAND3'))
FM_EQ_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
    if (@def('NAND4'))
FM_EQ_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    else
FM_EQ_GPSPEED     equ      $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    endif
    endif
    endif
    endif
    else
FM_EQ_GPSPEED     equ      $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 1 for TDS
    endif
;---------
  

AUDIBLE_ACELP_SR16KHz_SPEED      equ     36            ; Audible Acelp speed is 36 MHz(27.78nS)
AUDIBLE_ACELP_SR16KHz_PDIV       equ     27	           ; PLL O/P freq = (33 + PDIV)*(24/20)	= (33+27)*(24/20) = 72MHz
AUDIBLE_ACELP_SR16KHz_DDIV       equ     1             ; DCLK = 60MHz/(2^DDIV) = 36MHz
AUDIBLE_ACELP_SR16KHz_VOLTAGE    equ     $00000B       ; 1.37V
AUDIBLE_ACELP_SR16KHz_BOVOLTAGE  equ     $000800       ; 1.28V 

; NAND1     36MHz *  83.3ns = 2.999 -> $000102
; NAND2     36MHz * 116.7ns = 4.201 -> $000203
; NAND4     36MHz * 133.3ns = 4.788 -> $000203
    if (@def('DEBUG'))
    if (@def('NAND1'))
AUDIBLE_ACELP_SR16KHz_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    else
    if (@def('NAND2'))
AUDIBLE_ACELP_SR16KHz_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND3'))
AUDIBLE_ACELP_SR16KHz_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND4'))
AUDIBLE_ACELP_SR16KHz_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
AUDIBLE_ACELP_SR16KHz_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
    endif
    endif
    endif
    else
AUDIBLE_ACELP_SR16KHz_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
    endif
;---------   


 

MP3_SPEED       equ     36              ; MP3 speed is 36 MHz  (27.78nS)
MP3_PDIV        equ     27              ; (27+33)*(24/20) = 72
MP3_DDIV        equ     1               ; 72 / (2^1) = 36 MHz
MP3_VOLTAGE     equ     $00000B         ; 1.37v
MP3_BOVOLTAGE   equ     $000800         ; 1.28v

; NAND1     36MHz *  83.3ns = 2.999 -> $000102
; NAND2     36MHz * 116.7ns = 4.201 -> $000203
; NAND4     36MHz * 133.3ns = 4.788 -> $000203
    if (@def('DEBUG'))
    if (@def('NAND1'))
MP3_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND2'))
MP3_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND3'))
MP3_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND4'))
MP3_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
MP3_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    endif
    endif
    endif
    endif
    else
MP3_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    endif
;---------
ADPCM_SPEED       equ     36              ; ADPCM speed is 36 MHz  (27.78nS)
ADPCM_PDIV        equ     27              ; (27+33)*(24/20) = 72
ADPCM_DDIV        equ     1               ; 72 / (2^1) = 36 MHz
ADPCM_VOLTAGE     equ     $00000B         ; 1.37v
ADPCM_BOVOLTAGE   equ     $000800         ; 1.28v

; NAND1     36MHz *  83.3ns = 2.999 -> $000102
; NAND2     36MHz * 116.7ns = 4.201 -> $000203
; NAND3|4   36MHz * 133.3ns = 4.788 -> $000203
    if (@def('DEBUG'))
    if (@def('NAND1'))
ADPCM_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND2'))
ADPCM_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND3'))
ADPCM_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND4'))
ADPCM_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
ADPCM_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    endif
    endif
    endif
    endif
    else
ADPCM_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    endif
;---------
; Mixer use requires a min vddd. Reason for this: 
; ************
; Stmp00004930: Field failure: Distortion from the mixer. Very worst case 
;           conditions (at cold) will cause the ADC to not provide any samples.
;           All mixer/adc functions must run VDDD>=1.43V. This includes encoding
;           and Fmtuner  operation (uses line-in via mixer) in the SDK. 
; ************
MIXER_SPEED       equ     IDLE_SPEED      ; same as Idle speed defined above
MIXER_PDIV        equ     IDLE_PDIV       ; same as idle case defined above
MIXER_DDIV        equ     IDLE_DDIV       ; same as idle case defined above
MIXER_VOLTAGE     equ     $00000D         ; 1.44v
MIXER_BOVOLTAGE   equ     $000A00         ; 1.34v as in the MAX_BOVOLTAGE case below which has same Vddd.

MIXER_GPSPEED     equ     IDLE_GPSPEED    ; HW_GPFLASH_TIMING2R - 1dclk/access time for both TDH and TDS
;---------
ADCBASE_SPEED       equ     36              ; ADCBASE speed is 36 MHz  (27.78nS)
ADCBASE_PDIV        equ     27              ; (27+33)*(24/20) = 72
ADCBASE_DDIV        equ     1               ; 72 / (2^1) = 36 MHz
ADCBASE_VOLTAGE     equ     $00000D         ; 1.44v Min vddd voltage to operate high res ADC @ low temp.
ADCBASE_BOVOLTAGE   equ     $000A00         ; 1.34v Voltages same as max case. Rest matches ADPCM case.

; NAND1     36MHz *  83.3ns = 2.999 -> $000102
; NAND2     36MHz * 116.7ns = 4.201 -> $000203
; NAND4     36MHz * 133.3ns = 4.788 -> $000203
    if (@def('DEBUG'))
    if (@def('NAND1'))
ADCBASE_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    else
    if (@def('NAND2'))
ADCBASE_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND3'))
ADCBASE_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND4'))
ADCBASE_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
ADCBASE_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    endif
    endif
    endif
    endif
    else
ADCBASE_GPSPEED    equ     $000102         ; HW_GPFLASH_TIMING2R - 1dclk/access time for TDH and 2 for TDS
    endif
;---------
MAX_SPEED       equ     48              ; MAX speed is 48 MHz (20.83ns)
MAX_PDIV        equ     7               ; (7+33)*(24/20) = 48
MAX_DDIV        equ     0               ; 47.9232 / (2^0) = 48 MHz
MAX_VOLTAGE     equ     $00000D         ; 1.44v
MAX_BOVOLTAGE   equ     $000A00         ; 1.34v

; NAND1     48MHz *  83.3ns = 3.998 -> $000202 -> $000203 required (setup needs 3dclk)
; NAND2     48MHz * 116.7ns = 5.602 -> $000303
; NAND3|4   48MHz * 133.3ns = 6.398 -> $000304
    if (@def('DEBUG'))
    if (@def('NAND1'))
MAX_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND2'))
MAX_GPSPEED    equ     $000303         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND3'))
MAX_GPSPEED    equ     $000304         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
    else
    if (@def('NAND4'))
MAX_GPSPEED    equ     $000304         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
    else
MAX_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    endif
    endif
    endif
    endif
    else
MAX_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    endif
;---------
WMA_SPEED       equ     60              ; WMA speed is 60MHz   (16.67nS)
WMA_PDIV        equ     17              ; (17+33)*(24/20) = 60
WMA_DDIV        equ     0               ; 60 / (2^0) = 60 MHz
WMA_VOLTAGE     equ     $000013         ; 1.63V
WMA_BOVOLTAGE   equ     $001000         ; 1.54V

; Matching values for MSC (see note above for IDLE_GPSPEED)
    if (@def('DEBUG'))
    if (@def('NAND1'))
WMA_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND2'))
WMA_GPSPEED    equ     $000304         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
    else
    if (@def('NAND3'))
WMA_GPSPEED    equ     $000305         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 5 for TDS
    else
    if (@def('NAND4'))
WMA_GPSPEED    equ     $000305         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 5 for TDS
    else
WMA_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    endif
    endif
    endif
    endif
    else
WMA_GPSPEED    equ     $000203         ; HW_GPFLASH_TIMING2R - 2dclk/access time for TDH and 3 for TDS
    endif
;---------
MP3ENCODE_SPEED       equ     65              ; MP3ENCODE speed is 65MHz   (15.35nS) but see next line
MP3ENCODE_PDIV        equ     20              ; (22+33)*(24/20) = 65; but   (20+33)*(24/20)=63.6 (T=15.723 ns)
MP3ENCODE_DDIV        equ     0               ; 65.1264 / (2^0) = 65 MHz;   63.6 / 2^0 = 63.6 MHz
MP3ENCODE_VOLTAGE     equ     $000019         ; 1.82V
MP3ENCODE_BOVOLTAGE   equ     $001700         ; 1.76 v 

; NAND1     65MHz *  83.3ns = 5.415 -> $000303
; NAND2     65MHz * 116.7ns = 7.586 -> $000404
; NAND3|4   65MHz * 133.3ns = 8.665 -> $000405
    if (@def('DEBUG'))
    if (@def('NAND1'))
MP3ENCODE_GPSPEED    equ     $000303         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
    else
    if (@def('NAND2'))
MP3ENCODE_GPSPEED    equ     $000404         ; HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 4 for TDS
    else
    if (@def('NAND3'))
MP3ENCODE_GPSPEED    equ     $000405         ; HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 5 for TDS
    else
    if (@def('NAND4'))
MP3ENCODE_GPSPEED    equ     $000405         ; HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 5 for TDS
    else
MP3ENCODE_GPSPEED    equ     $000303         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
    endif
    endif
    endif
    endif
    else
MP3ENCODE_GPSPEED    equ     $000303         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 3 for TDS
    endif
;---------
    ; x:HW_GPFLASH_TIMING2R 
    ;      TDS = 25ns Min data setup time. Note usbmsc says 45 for min. 20ns is a typical nand flash.
    ;      TDH = 15ns Min data hold  time. Note usbmsc says 30 for min. But has min (tds+tdh)>50ns.
    ;      TDS + TDH must be > 50 nS
PEAK_SPEED       equ     75              ; PEAK speed is 75.6MHz T=13.2275 ns  
PEAK_PDIV        equ     30              ; (29+33)*(24/20) = 75.6          
PEAK_DDIV        equ     0               ; 75.6 / (2^0) = 75.6 MHz     
PEAK_VOLTAGE     equ     $00001c         ; 1.92V 
PEAK_BOVOLTAGE   equ     $001900         ; 1.82V                        

; DEBUG builds have different setup&hold due to engineering board's capacitance. RETAIL BUILDS USE ONE VALUE. 
; ns times below are from usbmsc's total (tds + tdh) = setup+hold. USBMSC runs at 60 MHz.
; NAND1     75.6 MHz *  83.3ns = 6.247 DCLKs so 7 -> $000304      
; NAND2     75.6 MHz * 116.7ns = 8.823 DCLKs so 9 -> $000405      
; NAND3|4   75.6 MHz * 133.3ns = 10.078 DCLKs     -> $000505 If issues try 000506 here (TDS=6).     
    if (@def('DEBUG'))
    if (@def('NAND1'))
PEAK_GPSPEED    equ     $000304         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
    else
    if (@def('NAND2'))
PEAK_GPSPEED    equ     $000405         ; HW_GPFLASH_TIMING2R - 4dclk/access time for TDH and 5 for TDS; nand1 settings work on multinand if good layout.
    else
    if (@def('NAND3'))
PEAK_GPSPEED    equ     $000505         ; HW_GPFLASH_TIMING2R - 5dclk/access time for TDH and 5 for TDS
    else
    if (@def('NAND4'))
PEAK_GPSPEED    equ     $000505         ; HW_GPFLASH_TIMING2R - 5dclk/access time for TDH and 5 for TDS
    else
PEAK_GPSPEED    equ     $000304         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
    endif
    endif
    endif
    endif
    else
PEAK_GPSPEED    equ     $000304         ; HW_GPFLASH_TIMING2R - 3dclk/access time for TDH and 4 for TDS
    endif

;------------------------------------------
;> 
; Multi-Client Clock Speed Adjustment API:  
;
; Prototypes here are exported from sysspeed.inc to sysspeed.h by incToC util
;
;C insert
;_reentrant INT SysGetSpeed(void);
;endc
;             or asm input params: none
;
;             Returns the CurrentSpeedIndex
;
;C insert
;_reentrant INT SysSpeedIncrease(int SpeedIndex, int clockUserId); 
;endc
;             or asm input parms:            x1,              y0
;
;       Returns resulting SpeedIndex from SysGetSpeed
;
;C insert
;_reentrant INT SysSpeedClockFree(int clockUserId);  
;endc
;                   or asm input:              y0
;
;       Returns resulting SpeedIndex from SysGetSpeed
;
; Generally, new clock adjusting modules must insert a bit equate below 
; (speed ordered) and use the API calls:
; SysSpeedIncrease(SpeedIndex, MY_BIT) and SysSpeedClockFree(MY_BIT) 
; passing in the client's clockUserId below for these calls
; which manipulate a corresponding speed client bit in clockUserFlag.
; SysSpeedClockFree(MY_BIT) reduces clock speed to that of highest active client
; or IDLE. Each bit here is a client active indicator managed via this speed API.
;
; clockUserFlag  dc      0      ; Module private var defined in sysspeed.asm
; ORDER MUST BE LOWEST TO HIGHEST FREQ so insert your new speed client ID and renumber.
SPEED_CLIENT_EXECUTIVE      equ     0 ; Exec turns up the clock on button events. 
SPEED_CLIENT_AUDIBLE_MP3    equ	    1
SPEED_CLIENT_STFM_DEC       equ     2
SPEED_CLIENT_AUDIBLE_ACELP  equ	    3
SPEED_CLIENT_STFM_EQ        equ     4
SPEED_CLIENT_MP3_DEC        equ     5
SPEED_CLIENT_ADPCM_DEC      equ     6
SPEED_CLIENT_ADPCM_ENC      equ     7
SPEED_CLIENT_MIXER          equ     8
SPEED_CLIENT_MP3_DEC_SYNC   equ     9
SPEED_CLIENT_PARSER         equ     10 ; Music and voice parser share this client
SPEED_CLIENT_MENU_WOW       equ     11 ; Same speed as parser.asm or higher. C client. Rename if using.
SPEED_CLIENT_MVIDEO         equ     12
SPEED_CLIENT_METADATA       equ     13
SPEED_CLIENT_PLAYERLIB      equ     14
SPEED_CLIENT_PLAYLIST1      equ     15 
SPEED_CLIENT_WMA_DEC        equ     16
SPEED_CLIENT_MP3_ENC        equ     17 ; Move line when used to maintain speed order
SPEED_CLIENT_JPEG_DEC       equ	    18 ;
SPEED_CLIENT_HIGHEST_USER   equ     18 ; change value to match high bit pos above

; Above equates are exported to incToC utility generated sysspeed.h file.
; <
;-----------------------------------------


    ENDIF   ; IF (!@def(SYSSPEED_INC))


