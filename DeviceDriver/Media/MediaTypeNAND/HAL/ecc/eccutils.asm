;///////////////////////////////////////////////////////////////////////////////
;  Copyright(C) SigmaTel, Inc. 2000-2003
;
;  File        : ECCUtils.asm
;  Description : Helper routines for managing the ECC data
;///////////////////////////////////////////////////////////////////////////////

   SECTION NANDHAL_ECCUTILS

;///////////////////////////////////////////////////////////////////////////////
;   Macros
;///////////////////////////////////////////////////////////////////////////////

    nolist
    include 'sysmacro.asm'
    include "decr.asm"
    include "incr.asm"
    list

;///////////////////////////////////////////////////////////////////////////////
;   Includes
;///////////////////////////////////////////////////////////////////////////////

    nolist
    INCLUDE "eccutils.inc"
    INCLUDE 'NANDHal.inc'
    INCLUDE "regsswizzle.inc"
	INCLUDE "encadpcmequ.inc"
	include  "regsicoll.inc"

    list

;///////////////////////////////////////////////////////////////////////////////
;   External Definitions
;///////////////////////////////////////////////////////////////////////////////

    GLOBAL  CalculateECC
    GLOBAL  VerifyAndFixECC
    GLOBAL  FCalculateECC
    GLOBAL  FVerifyAndFixECC

;///////////////////////////////////////////////////////////////////////////////
;   External References
;///////////////////////////////////////////////////////////////////////////////
    nolist
    include "const.xref"	; 
    list

    EXTERN  Get8Bit
    EXTERN  Rdnt_GetECC
    EXTERN  AccGetBitCnt
    EXTERN  Put8Bit
    EXTERN  RdntSetOddECC 
    EXTERN  RdntSetEvenECC
    EXTERN  pECCTable,EccByteTable
    EXTERN  GetWordByte
    EXTERN  WordPutByte
    EXTERN  HalDivideByThree

;///////////////////////////////////////////////////////////////////////////////
;   Equates
;///////////////////////////////////////////////////////////////////////////////

;///////////////////////////////////////////////////////////////////////////////
;  X memory
;///////////////////////////////////////////////////////////////////////////////

                
;///////////////////////////////////////////////////////////////////////////////
;  Y memory
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   P Memory
;///////////////////////////////////////////////////////////////////////////////

    org P,"NANDHAL_ECCUTILS_p":

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           CalculateECC
;
;   Type:           Function
;
;   Description:    Calculate the ECC for both the odd and even half of the page
;
;   Inputs:         
;           R1 = Buffer
;
;   Outputs:        
;           A0 = ECCValue1  EVEN page ECC
;           A1 = ECCValue2  ODD page ECC
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')
FCalculateECC     ; C Wrapper
    move    r0,r1

CalculateECC:
    push    r0
    push    n0
    push    r1
    push    r2
    push    r3
    push    n3

    push    b0
    push    b1
    push    b2
    push    x0
    push    x1
    push    y0
    push    y1
    push    r5
    push    n5
    push    r6
    push    n6
    push    r4
    push    n4
    
	IF @DEF('PD_MP3_ENCODE')	;add code only if mp3 encode is enabled in player.mk
		;next qualifier - only if mp3 encoder isr is active
		move     y:MP3EncoderIsrState,x0
	    jclr     #ENC_ISR_STATE_RECORDING,x0,_DoNotDisableMP3EncIsr
		move     x:HW_IPR,x0
		push     x0
		bclr     #HW_IPR_L2P_BITPOS,x0
		bclr     #HW_IPR_L2P_BITPOS+1,x0
		move     x0,x:HW_IPR
		nop
		nop
		nop
_DoNotDisableMP3EncIsr
	ENDIF


    push    r1   
     ; Enable Swizzle
     move    #0,x0
     bset    #HW_SWIZZLECSR1_EN_BITPOS,x0
     move    x0,x:HW_SWIZZLECS1R

     clr     A
     move    #>85,n5
     jsr     CalculateEccPage
     pop     r1
     push    a0   
     move    #>(PACKED_SMALL_PAGE_OFFSET-1),x0 ;
     move    r1,b
     add     x0,b       y:<Const_000001,A
     move    b,r1
     move    #>84,n5
     jsr     CalculateEccPage
     pop     x1
     move    a0,a            
     move    x1,a0           


	IF @DEF('PD_MP3_ENCODE')	;add code only if mp3 encode is enabled in player.mk
		;next qualifier - only if mp3 encoder isr is active
		move     y:MP3EncoderIsrState,x0
	    jclr    #ENC_ISR_STATE_RECORDING,x0,_DoNotReEnableMP3EncIsr	;because it was not disabled at the start of CalculateECC
		pop      x0
		move	 x0,x:HW_IPR
		nop
		nop
		nop
		nop
_DoNotReEnableMP3EncIsr
	ENDIF


    pop     n4
    pop     r4
    pop     n6
    pop     r6
    pop     n5
    pop     r5
    pop     y1
    pop     y0
    pop     x1
    pop     x0
    pop     b2
    pop     b1
    pop     b0
    
    pop     n3
    pop     r3
    pop     r2
    pop     r1
    pop     n0
    pop     r0

    rts                                 ;
   ELSE
      CalculateECC:
         move y:<Const_ffffff,a0
         move y:<Const_ffffff,a1
         move y:<Const_000000,a2
         rts
   ENDIF


   IF @DEF('HAL_ECC_ENABLE')
; r1 points to the buffer containing data.
; A contains the byte offset to start the computations with.
; Compute ECC and return the value in A
; A = 0, for even page, A = 1 for odd page
; n5 contains number of words to process inside the loop

    if @def('NO_SWIZZLE')

CalculateEccPage
    move    r1,r4
	move	A,n0
    clr     A	
    move    A,r1
    move    A,r2
    move    A,n3
    move    y:<Const_000002,m0
    move    A,r5
    move   (r0)+n0
    move    #>$ff,y1
    move    #pECCTable,r6
    move    #>MASK_CPS,n4
	do	#256,_LoopEnd
        move    y:(r0)+,y0      
        tfr     y0,B            x:(r4),x0
        mpy     x0,y0,A         r4,r3
;		a=table[data[i]]; /* Get CP0-CP5 code from table */
        tst     B               y:<Const_000080,y0
        teq     x0,A            
        and     y1,A            (r3)+    
        cmp     y0,B            A1,n6
        teq     A,B             r3,r4                   ; A->B is dummy

        move    n4,A
        move    y:(r6+n6),x0
;       reg1^=(a&MASK_CPS); /* XOR with a */        
        and     x0,A      r1,y0
        eor     y0,A      y:<Const_000040,B
        and     x0,B      A1,r1
        jeq     _LoopContinue
        move    r5,A
        tfr     A,B    n3,x0
        eor     x0,A   r2,x0 
        not     B      A1,n3
        and     y1,B
        eor     x0,B
        move    B1,r2
_LoopContinue
        move    (r5)+
_LoopEnd
        move    n3,r3
        move    y:<Const_ffffff,m0
        jsr     TransResult         ;//r2,r3 contains the params
        jsr     ProcessPostTransResult

        rts


    else

EccPreComputation
        move    x:(r4)+,x0    y:(r3)+,y0              
        move    (r3)+
EccPostComputation
        ; Read data in to Swizzle register
        move    x0,x:(r0)               
        ; Get byte1
        move    x:(r3)+,n6
        tfr    x1,A       (r5)+
        move    y:(r6+n6),x0
;       reg1^=(a&MASK_CPS); /* XOR with a */        
        and     x0,A      r1,y0
        eor     y0,A      y:<Const_000040,B
        and     x0,B      A1,r1
        jeq     _Continue1

        move    r5,A
        tfr     A,B    n3,x0
        eor     x0,A   r2,x0 
        not     B      A1,n3
        and     y1,B
        eor     x0,B
        move    B1,r2
_Continue1
        move    r5,x0
        move    #>255,A
        cmp     x0,A
        jeq     _Continue2

        move    x:(r3)+,n6
        tfr    x1,A       (r5)+
        move    y:(r6+n6),x0
;       reg1^=(a&MASK_CPS); /* XOR with a */        
        and     x0,A      r1,y0
        eor     y0,A      y:<Const_000040,B
        and     x0,B      A1,r1
        jeq     _Continue2

        move    r5,A
        tfr     A,B    n3,x0
        eor     x0,A   r2,x0 
        not     B      A1,n3
        and     y1,B
        eor     x0,B
        move    B1,r2
_Continue2
        rts




CalculateEccPage
    move    r1,r4
    move    #EccByteTable,r0
	move	A,n0
    clr     A	
    move    A,r1
    move    A,r2
    move    A,n3
    move    #HW_SWIZZLEBITREVR,r3
    move    y:<Const_000003,m3
    move    A,r5
    move    #>$ff,y1
    move    (r5)-                               ; PreDecrement
    move    #pECCTable,r6
    move    #>MASK_CPS,x1
    move    n0,A
    tst     A       #HW_SWIZZLEDATA1R,r0    
    jsne    EccPreComputation
    move    x:(r4)+,x0    y:(r3)+,y0              
	do	n5,_LoopEnd
        ; Read data in to Swizzle register
        move    x0,x:(r0)   
        ; Get byte1
        move    x:(r3)+,n6
        tfr    x1,A       (r5)+
        move    y:(r6+n6),x0
;       reg1^=(a&MASK_CPS); /* XOR with a */        
        and     x0,A      r1,y0
        eor     y0,A      y:<Const_000040,B
        and     x0,B      A1,r1
        jeq     _LoopContinue1

        move    r5,A
        tfr     A,B    n3,x0
        eor     x0,A   r2,x0 
        not     B      A1,n3
        and     y1,B
        eor     x0,B
        move    B1,r2
_LoopContinue1
        move    x:(r3)+,n6
        tfr    x1,A       (r5)+
        move    y:(r6+n6),x0
;       reg1^=(a&MASK_CPS); /* XOR with a */        
        and     x0,A      r1,y0
        eor     y0,A      y:<Const_000040,B
        and     x0,B      A1,r1
        jeq     _LoopContinue2

        move    r5,A
        tfr     A,B    n3,x0
        eor     x0,A   r2,x0 
        not     B      A1,n3
        and     y1,B
        eor     x0,B
        move    B1,r2
_LoopContinue2

        move    x:(r3)+,n6
        tfr    x1,A       (r5)+
        move    y:(r6+n6),x0
;       reg1^=(a&MASK_CPS); /* XOR with a */        
        and     x0,A      r1,y0
        eor     y0,A      y:<Const_000040,B
        and     x0,B      A1,r1
        jeq     _LoopContinue3

        move    r5,A
        tfr     A,B    n3,x0
        eor     x0,A   r2,x0 
        not     B      A1,n3
        and     y1,B
        eor     x0,B
        move    B1,r2
_LoopContinue3
        move    x:(r4)+,x0    y:(r3)+,y0              

_LoopEnd
        jsr     EccPostComputation
        move    n3,r3
        move    y:<Const_ffffff,m3
        jsr     TransResult         ;//r2,r3 contains the params
        jsr     ProcessPostTransResult

        rts

    endif

        ENDIF



;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           ProcessPostTransResult
;
;   Type:           Function
;
;   Description:    ?
;
;   Inputs:         ?
;
;   Outputs:        ?
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')
ProcessPostTransResult:
                push    x0
                push    x1
                push    b0
                push    b1
                push    b2

                move  r2,a
                not   a #>$0000ff,x1
                and   x1,a
                move  a1,r2

                move  r3,a
                not   a #>$0000ff,x1
                and   x1,a
                move  a1,r3

                move  r1,a
                not   a #>$00003F,x1
                and   x1,a
                asl   a #>BIT1BIT0,x1
                asl   a
                or    x1,a
                move  a1,r1

                move  r3,x0
                move  y:<Const_000100,x1
                mpy   x0,x1,a
                asr   a

                move  r2,b0
                add   b,a r2,x0
                move  a0,x0
                mpy   x0,x1,a
                asr   a

                move  r1,b0
                add   b,a
                move  y:<Const_000000,a1

                pop     b2
                pop     b1
                pop     b0
                pop     x1
                pop     x0
                rts
   ENDIF




;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           TransResult
;
;   Type:           Function
;
;   Description:    TransResult
;
;   Inputs:         
;               r2: reg 2
;               r3: reg 3
;
;   Outputs:        
;               r2: ECC2
;               r3: ECC3
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////
;ALGORITHM
;static void trans_result(reg2,reg3,ecc1,ecc2)
;unsigned char reg2; // LP14,LP12,LP10,... *
;unsigned char reg3; // LP15,LP13,LP11,... *
;unsigned char *ecc1; // LP15,LP14,LP13,... *
;unsigned char *ecc2; // LP07,LP06,LP05,... *
;{
;   unsigned char a; // Working for reg2,reg3 *
;   unsigned char b; // Working for ecc1,ecc2 *
;   unsigned char i; // For counting *
;   a=BIT7; b=BIT7; // 80h=10000000b *
;   *ecc1=*ecc2=0; // Clear ecc1,ecc2 *
;   for(i=0; i<4; ++i) 
;   {
;       if ((reg3&a)!=0) 
;           *ecc1|=b; // LP15,13,11,9 -> ecc1 *
;       b=b>>1; // Right shift *
;       if ((reg2&a)!=0) 
;           *ecc1|=b; // LP14,12,10,8 -> ecc1 *
;       b=b>>1; // Right shift *
;       a=a>>1; // Right shift *
;   }
;   b=BIT7; // 80h=10000000b *
;   for(i=0; i<4; ++i)
;   {
;       if ((reg3&a)!=0) 
;           *ecc2|=b; // LP7,5,3,1 -> ecc2 *
;       b=b>>1; // Right shift *
;       if ((reg2&a)!=0) 
;           *ecc2|=b; // LP6,4,2,0 -> ecc2 *
;       b=b>>1; // Right shift *
;       a=a>>1; // Right shift *
;   }
;}
;
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')
      TransResult:
                push    a0
                push    a1
                push    a2
                push    b0
                push    b1
                push    b2
                push    x0
                push    x1
                push    y0
                push    y1
                push    r5
                push    n5
                push    r6
                push    n6
                push    r4
                push    n4


                move        #>BIT7,r5               ;//a
                move        #>BIT7,r6               ;//b
                move        y:<Const_000000,x0                   ;//ecc2
                move        y:<Const_000000,x1                   ;//ecc3
                move        y:<Const_000000,r4                   ;<JFP>
                move        y:<Const_000004,n4                   ;<JFP>
                decr        r4                      ;<JFP>
                nop

         TR_FIRSTLOOP:
                incr  r4                ;<JFP>
                move  r4,y0                 ;<JFP>
                move  n4,a                  ;<JFP>
                cmp   y0,a                  ;//
                jeq   TR_FIRSTLOOP_END      ;//

                move        r5,a                    ;
                move        r3,y0                   ;
                and         y0,a                    ;
                jeq         SKIP_ODDLP0             ;
                move        r6,b                    ;
                or          x0,b                    ;
                move        b1,x0                   ;

         SKIP_ODDLP0:
                move        r6,b1                   ;
                asr         b                       ;
                move        b1,r6                   ;
                move        r5,a                    ;
                move        r2,y0                   ;
                and         y0,a                    ;
                jeq         SKIP_EVENLP0            ;
                move        r6,b                    ;
                or          x0,b                    ;
                move        b1,x0                   ;

         SKIP_EVENLP0:
                move        r6,b1                   ;
                asr         b                       ;
                move        b1,r6                   ;
                move        r5,b1                   ;
                asr         b                       ;
                move        b1,r5                   ;
                jmp         TR_FIRSTLOOP            ;

         TR_FIRSTLOOP_END:
                move        #>BIT7,r6               ;//b
                move        y:<Const_000000,r4                   ;<JFP>
                move        y:<Const_000004,n4                   ;<JFP>
                decr        r4                      ;<JFP>
                nop                                 ;

         TR_SECONDLOOP:
                incr        r4                      ;<JFP>
                move        r4,y0                   ;<JFP>
                move        n4,a                    ;<JFP>
                cmp         y0,a                    ;//
                jeq         TR_SECONDLOOP_END       ;//

                move        r5,a                    ;
                move        r3,y0                   ;
                and         y0,a                    ;
                jeq         SKIP_ODDLP1             ;
                move        r6,b                    ;
                or          x1,b                    ;
                move        b1,x1                   ;

         SKIP_ODDLP1:
                move        r6,b1                   ;
                asr         b                       ;
                move        b1,r6                   ;
                move        r5,a                    ;
                move        r2,y0                   ;
                and         y0,a                    ;
                jeq         SKIP_EVENLP1            ;
                move        r6,b                    ;
                or          x1,b                    ;
                move        b1,x1                   ;

         SKIP_EVENLP1:
                move        r6,b1                   ;
                asr         b                       ;
                move        b1,r6                   ;
                move        r5,b1                   ;
                asr         b                       ;
                move        b1,r5                   ;
                jmp         TR_SECONDLOOP           ;

         TR_SECONDLOOP_END:
                move        x0,r2                   ;
                move        x1,r3                   ;
         TR_COMPLETION:
                pop     n4
                pop     r4
                pop     n6
                pop     r6
                pop     n5
                pop     r5
                pop     y1
                pop     y0
                pop     x1
                pop     x0
                pop     b2
                pop     b1
                pop     b0
                pop     a2
                pop     a1
                pop     a0
                rts                                 ;
                ENDIF
                

;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           VerifyAndFixECC
;
;   Type:           Function
;
;   Description:    
;       Verify if the ECC is correctable, uncorrectable, or 
;       no need for correction. If it's correctable it also fixes
;       the error
;       This function is split into various smaller functions
;       The part of the algorithm they implement are also given
;       alongside
;
;   Inputs:         
;           r1 = pBuffer (r0 for C call)
;           r2 = pRdntAreaBuffer (r4 for C call)
;           a0 = Calculated ECC of the even page
;           a1 = Calculated ECC of the odd page
;
;   Outputs:        
;           Carry denotes an error
;           A1  = - NANDHAL_ECC_NOERROR
;                 - NANDHAL_ECC_ERROR_FIXED 
;                 - NANDHAL_ECC_FIX_FAILED
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////
;ALGORITHM
;static unsigned char correct_data(data,eccdata,ecc1,ecc2,ecc3)
;unsigned char *data; // DATA
;unsigned char *eccdata; // ECC DATA
;unsigned char ecc1; // LP15,LP14,LP13,...
;unsigned char ecc2; // LP07,LP06,LP05,...
;unsigned char ecc3; // CP5,CP4,CP3,...,"1","1"
;{
;   unsigned long l; // Working to check d
;   unsigned long d; // Result of comparison
;   unsigned int i; // For counting
;   unsigned char d1,d2,d3; // Result of comparison
;   unsigned char a; // Working for add
;   unsigned char add; // Byte address of cor. DATA
;   unsigned char b; // Working for bit
;   unsigned char bit; // Bit address of cor. DATA
;   d1=ecc1^eccdata[1]; d2=ecc2^eccdata[0]; // Compare LP's
;   d3=ecc3^eccdata[2]; // Comapre CP's
;   d=((unsigned long)d1<<16) // Result of comparison
;       +((unsigned long)d2<<8)
;       +(unsigned long)d3;
;   if (d==0) return(0); // If No error, return 
;   if (((d^(d>>1))&CORRECTABLE)==CORRECTABLE) 
;   { // If correctable 
;       l=BIT23;
;       add=0; // Clear parameter 
;       a=BIT7;
;       for(i=0; i<8; ++i) 
;       { // Checking 8 bit 
;           if ((d&l)!=0) 
;               add|=a; // Make byte address from LP's 
;           l>>=2; a>>=1; // Right Shift 
;       }
;       bit=0; // Clear parameter 
;       b=BIT2;
;       for(i=0; i<3; ++i) 
;       { // Checking 3 bit 
;           if ((d&l)!=0) 
;               bit|=b; // Make bit address from CP's 
;           l>>=2; b>>=1; // Right shift 
;       }
;       b=BIT0;
;       data[add]^=(b<<bit); // Put corrected data 
;       return(1);
;   }
;   i=0; // Clear count 
;   d&=0x00ffffffL; // Masking 
;   while(d) { // If d=0 finish counting 
;       if (d&BIT0) ++i; // Count number of 1 bit 
;       d>>=1; // Right shift 
;   }
;   if (i==1) { // If ECC error *
;       eccdata[1]=ecc1; eccdata[0]=ecc2; // Put right ECC code *
;       eccdata[2]=ecc3;
;       return(2);
;   }
;   return(3); // Uncorrectable error *
;}
;
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')&&@DEF('HAL_ECC_CHECK_ON_READ')

FVerifyAndFixECC  ; C Wrapper
    move r0,r1
    move r4,r2

      VerifyAndFixECC:
                push    b0
                push    b1
                push    b2
                push    x0
                push    x1
                push    y0
                push    y1
                push    r3
                push    n3
                push    r6
                push    n6

                move    y:<Const_000000,r3               ;
                move    a0,x0               ;//Calculated ECC1
                move    a1,y0               ;//Calculated ECC2
                jsr     Rdnt_GetECC         ;//
                move    a0,x1               ;//Actual ECC1
                move    a1,y1               ;//Actual ECC2
         VAFECC_CHECK_EVEN_PAGE:
                
                move    x1,b                ;
                move    y:<Const_000000,b2               ;
                eor     x0,b                ;
                jeq     VAFECC_CHECK_ODD_PAGE   ;
                jsr     Swapd               ;
                incr    r3                  ;
                move    #>EVEN_HALF,r6      ;
                jsr     IsDataCorrectable   ;
                jscs    FixData             ;
                jcs     VAFECC_CHECK_ODD_PAGE   ;
                jsr     IsECCCorrectable    ;
                jscs    FixECC              ;
                jcs     VAFECC_CHECK_ODD_PAGE   ;
                jmp     VAFECC_ECCFAIL      ;

         VAFECC_CHECK_ODD_PAGE:
                move    y0,x0               ;
                move    y1,x1               ;
                move    x1,b                ;
                move    y:<Const_000000,b2               ;
                eor     x0,b                ;
                jeq     VAFECC_CHECK_DONE   ;
                jsr     Swapd               ;
                incr    r3                  ;
                move    #>ODD_HALF,r6       ;
                jsr     IsDataCorrectable   ;
                jscs    FixData         ;
                jcs     VAFECC_CHECK_DONE   ;

                jsr     IsECCCorrectable    ;
                jscs    FixECC          ;
                jcs     VAFECC_CHECK_DONE   ;

                jmp     VAFECC_ECCFAIL  ;

         VAFECC_CHECK_DONE:
                move    r3,a                ;
                move    y:<Const_000000,x0              ;
                cmp     x0,a                ;
                jgt     VAFECC_ERROR_CORRECTABLE    ;

                move    #NANDHAL_ECC_NOERROR,a    ;//Set success and return
                and     #$fe,ccr            ;
                jmp     VAFECC_ONCOMPLETION

         VAFECC_ECCFAIL:
                move    #NANDHAL_ECC_FIX_FAILED,a ;//Set FAIL and return
                or      #01,ccr             ;
                jmp     VAFECC_ONCOMPLETION ;//

         VAFECC_ERROR_CORRECTABLE:
                move    #NANDHAL_ECC_ERROR_FIXED,a ;//Set success and return
                or      #01,ccr             ;

         VAFECC_ONCOMPLETION:
                pop     n6
                pop     r6
                pop     n3
                pop     r3
                pop     y1
                pop     y0
                pop     x1
                pop     x0
                pop     b2
                pop     b1
                pop     b0
                rts

   ELSE
      VerifyAndFixECC:
                and      #$fe,ccr
                rts
   ENDIF


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           Swapd
;
;   Type:           Function
;
;   Description:    
;       Helper routine for Verify
;       Takes the value in b and swaps it as follows
;           swapd($112233) = $221133
;       The d in the algorithm is not a simple eor of ecc's it's a 
;       swapped eor
;
;   Inputs:         
;           b1 = value to be swapped
;
;   Outputs:        
;           b1 = swapped value
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')&&@DEF('HAL_ECC_CHECK_ON_READ')
      Swapd:
                push    x0
                push    a0
                push    a1
                push    a2

                move    b1,a1           ;
                move    #>LOW8,a0       ;
                jsr     GetWordByte  ;
                move    a1,x0           ;
                
                move    b1,a1           ;
                move    #>MID8,a0       ;
                jsr     GetWordByte  ;

                move    a1,a2           ;
                move    b1,a1           ;
                move    #>LOW8,a0       ;
                jsr     WordPutByte     ;

                move    x0,a2           ;
                move    #>MID8,a0       ;
                jsr     WordPutByte     ;

                move    a1,b1           ;
                move    y:<Const_000000,b2           ;
                pop     a2
                pop     a1
                pop     a0
                pop     x0
                rts
   ENDIF


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           IsDataCorrectable
;
;   Type:           Function
;
;   Description:    Checks whether the data can be fixed based on the ECC
;
;   Inputs:         
;           b1: The EXORed value of ECCCalculated and ECCActual
;
;   Outputs:        
;           Carry denotes a TRUE or FALSE
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////
;ALGORITHM
;   if (((d^(d>>1))&CORRECTABLE)==CORRECTABLE) 
;   { // If correctable 
;
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')&&@DEF('HAL_ECC_CHECK_ON_READ')
      IsDataCorrectable:
                push    a0
                push    a1
                push    a2
                push    b0
                push    b1
                push    b2
                push    x0
                push    x1

                clr     a                   ;
                move    b1,a0               ;
                asr     a b1,x0             ;
                move    a0,b                ;
                eor     x0,b                ;
                move    y:<Const_000000,b2               ;
                move    #>CORRECTABLE,x0    ;
                and     x0,b                ;
                cmp     x0,b                ;
                jeq     IDC_CORRECTABLE ;
                and     #$fe,ccr            ;
                jmp     IDC_ONCOMPLETION    ;

         IDC_CORRECTABLE:
                or      #01,ccr             ;   
         IDC_ONCOMPLETION:
                pop     x1
                pop     x0
                pop     b2
                pop     b1
                pop     b0
                pop     a2
                pop     a1
                pop     a0
                rts
   ENDIF


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           IsECCCorrectable
;
;   Type:           Function
;
;   Description:    Checks whether the ECC can be fixed
;
;   Inputs:         
;           b1: The EXORed value of ECCCalculated and ECCActual
;
;   Outputs:        
;           Carry denotes a TRUE or FALSE
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////
;ALGORITHM
;i=0; // Clear count    
;d&=0x00ffffffL; // Masking     
;   if (d&BIT0) ++i; // Count number of 1 bit 
;   d>>=1; // Right shift 
;}
;if (i==1) { // If ECC error *
;
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')&&@DEF('HAL_ECC_CHECK_ON_READ')
      IsECCCorrectable:
                push    a0
                push    a1
                push    a2
                push    x0

                move    b1,a0               ;
                jsr     AccGetBitCnt        ;
                move    y:<Const_000001,x0              ;
                move    a0,a                ;
                cmp     x0,a                ;
                jeq     IEC_CORRECTABLE     ;
                and     #$fe,ccr            ;
                jmp     IEC_ONCOMPLETION    ;
         IEC_CORRECTABLE:
                or      #01,ccr             ;   
         IEC_ONCOMPLETION:
                pop     x0
                pop     a2
                pop     a1
                pop     a0
                rts
   ENDIF


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FixData
;
;   Type:           Function
;
;   Description:    Fixes the error in data
;
;   Inputs:         
;           r1 = pBuffer
;           r2 = pRdntAreaBuffer
;           a0 = Calculated ECC of the even page
;           a1 = Calculated ECC of the odd page
;
;   Outputs:        
;           Carry denotes an error
;           A0  = - NANDHAL_ECC_NOERROR
;                 - NANDHAL_ECC_ERROR_FIXED 
;                 - NANDHAL_ECC_FIX_FAILED
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////
;ALGORITHM
;   { // If correctable         
;       l=BIT23;
;       add=0; // Clear parameter 
;       a=BIT7;
;       for(i=0; i<8; ++i) 
;       { // Checking 8 bit 
;           if ((d&l)!=0) 
;               add|=a; // Make byte address from LP's 
;           l>>=2; a>>=1; // Right Shift 
;       }
;       bit=0; // Clear parameter 
;       b=BIT2;
;       for(i=0; i<3; ++i) 
;       { // Checking 3 bit 
;           if ((d&l)!=0) 
;               bit|=b; // Make bit address from CP's 
;           l>>=2; b>>=1; // Right shift 
;       }
;       b=BIT0;
;       data[add]^=(b<<bit); // Put corrected data 
;       return(1);
;
;
;Reference registers
;               ;//l    = x1
;               ;//add  = x0                
;               ;//a    = r2    
;               ;//b    = n2
;               ;//bit  = r3
;               ;//d    = y0
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')&&@DEF('HAL_ECC_CHECK_ON_READ')
      FixData:
                push    a0
                push    a1
                push    a2
                push    b0
                push    b1
                push    b2
                push    x0
                push    x1
                push    y0
                push    y1
                push    r1
                push    n1
                push    r2
                push    n2
                push    r3
                push    n3
                push    r5
                push    n5

                move    y:<Const_000000,n5           ;//Save buffer
                move    r1,n1               ;//Storage for Data[]
                move    b1,y0               ;//Store d
                move    #>BIT7,a0           ;
                move    a0,r2               ;
                move    #>BIT2,a0               ;
                move    a0,n2               ;   
                move    y:<Const_000000,a0               ;
                move    a0,r3               ;   
                move    #>BIT23,x1          ;// 
                move    y:<Const_000000,x0               ;//add=0

                do      #8,FD_LPENDLOOP         ;//
                move    y0,b                ;
                and     x1,b                ;
                jeq     FD_SKIP_ADD         ;
                move    r2,a                ;
                or      x0,a                ;
                move    a1,x0               ;
         FD_SKIP_ADD:
                clr     a                   ;
                move    x1,a0               ;
                asr     a                   ;
                asr     a                   ;
                move    a0,x1               ;
                clr     a                   ;
                move    r2,a                ;
                asr     a
                move    a1,r2               ;
         FD_LPENDLOOP:
                do      #3,FD_CPENDLOOP     
                move    y0,b                ;
                and     x1,b                ;
                jeq     FD_SKIP_BIT         ;
                move    n2,a                ;
                move    r3,y1               ;
                or      y1,a                ;
                move    a1,r3               ;
         FD_SKIP_BIT:
                clr     a                   ;
                move    x1,a0               ;
                asr     a                   ;
                asr     a                   ;
                move    a0,x1               ;
                clr     a                   ;
                move    n2,a                ;
                asr     a
                move    a1,n2               ;
         FD_CPENDLOOP:
                clr     a                   ;
                move    #>BIT0,a0           ;//b
                move    r3,x1               ;
                clr     b                   ;
                cmp     x1,b                ;

                jeq     FD_SKIP_BIT_SHIFT
                rep     x1                  ;
                asl     a                   ;   
         FD_SKIP_BIT_SHIFT:
                move    a0,r3               ;//Save the value to be ORed
                move    r6,a                ;
                move    #>EVEN_HALF,y1      ;
                cmp     y1,a                ;
                jeq     FD_EVENPAGE         ;
         FD_ODDPAGE:
                move    #>PGSIZE_256,y1     ;
                move    x0,a                ;
                add     y1,a                ;
                move    a1,x0               ;
         FD_EVENPAGE:
;                move    y:<Const_000003,a               ;
;                jsr     modulo
                ; Call HalDivideByThree
                ; Numerator should be in x0
                ; Output will be: x0 = Quotient , x1 = Remainder
                jsr     HalDivideByThree
;                move    a1,x1               ;
;                move    a0,x0               ;
                move    n1,a                ;
                add     x0,a                ;
                move    x1,x0               ;
                move    a1,r5               ;
                jsr     Get8Bit             ;
                move    x1,a                ;
                move    r3,x1               ;
                eor     x1,a                ;
                move    x0,x1               ;
                move    a1,x0               ;
                jsr     Put8Bit             ;
                jmp     FD_COMPLETION       ;

         FD_COMPLETION:
                or      #01,ccr 

                pop     n5
                pop     r5
                pop     n3
                pop     r3
                pop     n2
                pop     r2
                pop     n1
                pop     r1
                pop     y1
                pop     y0
                pop     x1
                pop     x0
                pop     b2
                pop     b1
                pop     b0
                pop     a2
                pop     a1
                pop     a0
                rts
   ENDIF


;///////////////////////////////////////////////////////////////////////////////
;
;>  Name:           FixECC
;
;   Type:           Function
;
;   Description:    If the error is in the ECC it fills the ECC field correctly 
;
;   Inputs:         
;           r7: EVEN_HALF or ODD_HALF
;           x0:Calculated ECCDATA
;
;   Outputs:        
;           A0: return
;                   
;   Notes:          ?
;<
;///////////////////////////////////////////////////////////////////////////////

   IF @DEF('HAL_ECC_ENABLE')&&@DEF('HAL_ECC_CHECK_ON_READ')
      FixECC:
            push    a0
            push    a1
            push    a2
            push    y0
                
            move    r6,y0                       ;
            move    y:<Const_000000,a                       ;
            cmp     y0,a                        ;
            jeq     FE_EVEN_HALF                ;
            jsr     RdntSetOddECC               ;
            jmp     FE_ONCOMPLETION             ;

         FE_EVEN_HALF:
            jsr     RdntSetEvenECC

         FE_ONCOMPLETION:
            or      #01,ccr 

            pop     y0
            pop     a2
            pop     a1
            pop     a0
            rts

   ENDIF


   ENDSEC ;//ECCUTILS



