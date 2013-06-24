;//////////////////////////////////////////////////////////////////////////////
;/                                               
;/ Filename: kernel_bootstrap.asm
;/                                               
;/ Description:   
;/
;/ Copyright (c) SigmaTel, Inc. Unpublished
;/
;/ SigmaTel, Inc.
;/ Proprietary  Confidential
;/
;/ This source code and the algorithms implemented therein constitute
;/ confidential information and may comprise trade secrets of SigmaTel, Inc.
;/ or its associates, and any use thereof is subject to the terms and
;/ conditions of the Confidential Disclosure Agreement pursuant to which this
;/ source code was originally received.
;/                                               
;//////////////////////////////////////////////////////////////////////////////

    include 'const.xref'    
	include 'regsgpio.inc'

    extern  Fg_KernelStack
    extern  FKernel_Startup
    extern  FProject_Startup
    extern  FKernel_MainLoop

    global  Exec

    org p,".ptextkernel_startup":

	if (!@DEF('MTP_BUILD'))
 global SysAllLinear
SysAllLinear
    move    y:<Const_ffffff,m0
    move    y:<Const_ffffff,m1
    move    y:<Const_ffffff,m2
    move    y:<Const_ffffff,m3
    move    y:<Const_ffffff,m4
    move    y:<Const_ffffff,m5
    move    y:<Const_ffffff,m6
    move    y:<Const_ffffff,m7
    rts
    endif
; Note: In mtp and usbmsc, Exec resides in kernel_startup section    
; 
Exec
    ; 3410 and 3500 use this to allow repeated reset and go in boxview. 
    ; Added to 35xx debug builds Apr 21, 2005.  Player already had this.
   if (@def('DEBUG')) 
    move    #>$ff8fff,x0                    
    move    x0,x:$fffe
   endif

    ;set up kernel stack
    move    #Fg_KernelStack,r7
    move    #>0,sp
    move    #>0,sr
    jsr     SysAllLinear
    ;do kernel specific setup stuff
    jsr     FKernel_Startup
    ;Do project specific setup stuff
    jsr     FProject_Startup
    
;; GP32 or MMC_DETECT. STMP9220 Let pin float high to reduce USB suspend current
;;   	bset #HW_GP_B8_BITPOS,x:HW_GP1ENR
    	bclr #HW_GP_B8_BITPOS,x:HW_GP1DOER	;; make sure pin is used as input
;;   	bset #HW_GP_B8_BITPOS,x:HW_GP1DOR
   	
;; GP39  STMP9222 and STMP9223 Reduce current to STFM1000 (Turn off VDDD, VDDIO, and I2S_WCLK)
   	bset #HW_GP_B15_BITPOS,x:HW_GP1ENR
   	bset #HW_GP_B15_BITPOS,x:HW_GP1DOER
   	bset #HW_GP_B15_BITPOS,x:HW_GP1DOR
 
;; GP42 or SCAN_C4. STMP9221 Let pin float high to reduce USB suspend current
		bclr #HW_GP_B18_BITPOS,x:HW_GP1PWR		;; tri-state the pin
;;    	bset #HW_GP_B18_BITPOS,x:HW_GP1ENR
;;    	bset #HW_GP_B18_BITPOS,x:HW_GP1DOER  
;;    	bset #HW_GP_B18_BITPOS,x:HW_GP1DOR
      	
    jsr     FKernel_MainLoop
    jmp     *

;--------------------------------------------------------------
; Vector Note: DSP hardware reset vector at P:0 has "jmp Exec"
    org p,".ptextResetVector":$0
    jmp Exec
;--------------------------------------------------------------
    
