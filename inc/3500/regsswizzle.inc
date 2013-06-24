;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright(C) SigmaTel, Inc. 2002-2003
;;; Filename    : regsswizzle.inc
;;; Description : Register definitions for Swizzle interface
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;///////////////////////////////////////////////////////////////////////////////
; The following naming conventions are followed in this file.
; All registers are named using the format...
;     HW_<module>_<regname>
; where <module> is the module name which can be any of the following...
;     USB20
; (Note that when there is more than one copy of a particular module, the
; module name includes a number starting from 0 for the first instance of
; that module)
;
; <regname> is the specific register within that module
;
; We also define the following...
;
;     HW_<module>_<regname>_BITPOS
; which defines the starting bit (i.e. LSB) of a multi bit field
;
;     HW_<module>_<regname>_SETMASK
; which does something else, and
;
;     HW_<module>_<regname>_CLRMASK
; which does something else.
;
; Other rules
;     All caps
;     Numeric identifiers start at 0
    if !(@def(regsswizzleinc))
regsswizzleinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   SWIZZLE STMP3500 Registers 
;   Last Updated 7.11.2003 Dave Dyches
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;





HW_SWIZZLE_BASEADDR			equ		$F380


;ASM Only 
HW_SWIZZLECS1R 				equ 	(HW_SWIZZLE_BASEADDR)       ;// Swizzle Control & Status Register 1 
HW_SWIZZLECS2R 				equ 	(HW_SWIZZLE_BASEADDR+1)     ;// Swizzle Control & Status Register 2 
HW_SWIZZLESIZER 			equ		(HW_SWIZZLE_BASEADDR+2)     ;// Swizzle Transfer Size Register 
HW_SWIZZLESOURCER 			equ		(HW_SWIZZLE_BASEADDR+3)     ;// Swizzle Source Address Register 
HW_SWIZZLEDATA1R 			equ		(HW_SWIZZLE_BASEADDR+4)     ;// Swizzle Data1 Register     
HW_SWIZZLEDATA2R 			equ		(HW_SWIZZLE_BASEADDR+5)     ;// Swizzle Data2 Register 
HW_SWIZZLEDESTADDRR 		equ     (HW_SWIZZLE_BASEADDR+6)     ;// Swizzle Destination Address Register 
HW_SWIZZLEBIGENDIANR 		equ		(HW_SWIZZLE_BASEADDR+7)   	;// Swizzle Big Endian Register 
HW_SWIZZLEBITREVR 			equ		(HW_SWIZZLE_BASEADDR+8)     ;// Swizzle BITREV Register 
HW_SWIZZLEPASSLSBR 			equ		(HW_SWIZZLE_BASEADDR+9)     ;// Swizzle Pass Least Significant Byte Register 
HW_SWIZZLEPASSISBR 			equ		(HW_SWIZZLE_BASEADDR+10)    ;// Swizzle Pass Intermediate Byte Register 
HW_SWIZZLEPASSMSBR 			equ		(HW_SWIZZLE_BASEADDR+11)    ;// Swizzle Pass Most Significant Byte Register 
HW_SWIZZLEPASSLSWR 			equ		(HW_SWIZZLE_BASEADDR+12)    ;// Swizzle Pass Least Significant Word Register 
HW_SWIZZLEPASSISWR 			equ		(HW_SWIZZLE_BASEADDR+13)    ;// Swizzle Pass Intermediate Significant Word Register 
HW_SWIZZLEPASSMSWR 			equ		(HW_SWIZZLE_BASEADDR+14)    ;// Swizzle Pass Most Significant Word Register 
HW_SWIZZLEBARRELR 			equ		(HW_SWIZZLE_BASEADDR+15)    ;// Swizzle Barrel Shift Register 
HW_SWIZZLEDIV3LR 			equ		(HW_SWIZZLE_BASEADDR+16)    ;// Swizzle Divide By 3 Lower Register 
HW_SWIZZLEDIV3UR 			equ		(HW_SWIZZLE_BASEADDR+17)    ;// Swizzle Divide By 3 upper Register 
;endasm

;//  Swizzle CSR1 (HW_SWIZZLECSR1) Bit Definitions
HW_SWIZZLECSR1_EN_BITPOS	    equ 	(0)
HW_SWIZZLECSR1_LA_BITPOS   	    equ     (1)
HW_SWIZZLECSR1_LNR_BITPOS       equ     (2)
HW_SWIZZLECSR1_SIGN_BITPOS      equ     (3)
HW_SWIZZLECSR1_SHIFT_BITPOS     equ     (4)
HW_SWIZZLECSR1_MEM_BITPOS       equ     (8)
HW_SWIZZLECSR1_CLK_OFF_BITPOS   equ     (9)
HW_SWIZZLECSR1_NEWADD_BITPOS    equ     (10)
HW_SWIZZLECSR1_RSVD_BITPOS      equ     (11)

HW_SWIZZLECSR1_EN_WIDTH 	    equ 	(1)
HW_SWIZZLECSR1_LA_WIDTH         equ     (1)
HW_SWIZZLECSR1_LNR_WIDTH        equ     (1)
HW_SWIZZLECSR1_SIGN_WIDTH       equ     (1)
HW_SWIZZLECSR1_SHIFT_WIDTH      equ     (4)
HW_SWIZZLECSR1_MEM_WIDTH        equ     (1)
HW_SWIZZLECSR1_CLK_OFF_WIDTH    equ     (1)
HW_SWIZZLECSR1_NEWADD_WIDTH     equ     (1)
HW_SWIZZLECSR1_RSVD_WIDTH       equ     (13)

HW_SWIZZLECSR1_EN_SETMASK      	equ     (((1<HW_SWIZZLECSR1_EN_WIDTH)-1)<<HW_SWIZZLECSR1_EN_BITPOS)
HW_SWIZZLECSR1_LA_SETMASK      	equ     (((1<HW_SWIZZLECSR1_LA_WIDTH)-1)<<HW_SWIZZLECSR1_LA_BITPOS)
HW_SWIZZLECSR1_LNR_SETMASK      equ     (((1<HW_SWIZZLECSR1_LNR_WIDTH)-1)<<HW_SWIZZLECSR1_LNR_BITPOS)
HW_SWIZZLECSR1_SIGN_SETMASK     equ     (((1<HW_SWIZZLECSR1_SIGN_WIDTH)-1)<<HW_SWIZZLECSR1_SIGN_BITPOS)
HW_SWIZZLECSR1_SHIFT_SETMASK    equ     (((1<HW_SWIZZLECSR1_SHIFT_WIDTH)-1)<<HW_SWIZZLECSR1_SHIFT_BITPOS)
HW_SWIZZLECSR1_MEM_SETMASK      equ     (((1<HW_SWIZZLECSR1_MEM_WIDTH)-1)<<HW_SWIZZLECSR1_MEM_BITPOS)
HW_SWIZZLECSR1_CLK_OFF_SETMASK  equ     (((1<HW_SWIZZLECSR1_CLK_OFF_WIDTH)-1)<<HW_SWIZZLECSR1_CLK_OFF_BITPOS)
HW_SWIZZLECSR1_NEWADD_SETMASK   equ     (((1<HW_SWIZZLECSR1_NEWADD_WIDTH)-1)<<HW_SWIZZLECSR1_NEWADD_BITPOS)
HW_SWIZZLECSR1_RSVD_SETMASK     equ     (((1<HW_SWIZZLECSR1_RSVD_WIDTH)-1)<<HW_SWIZZLECSR1_RSVD_BITPOS)

HW_SWIZZLECSR1_EN_CLRMASK      	equ     (~HW_SWIZZLECSR1_EN_SETMASK)
HW_SWIZZLECSR1_LA_CLRMASK      	equ     (~HW_SWIZZLECSR1_LA_SETMASK)
HW_SWIZZLECSR1_LNR_CLRMASK      equ     (~HW_SWIZZLECSR1_LNR_SETMASK)
HW_SWIZZLECSR1_SIGN_CLRMASK     equ     (~HW_SWIZZLECSR1_SIGN_SETMASK)
HW_SWIZZLECSR1_SHIFT_CLRMASK    equ     (~HW_SWIZZLECSR1_SHIFT_SETMASK)
HW_SWIZZLECSR1_MEM_CLRMASK      equ     (~HW_SWIZZLECSR1_MEM_SETMASK)
HW_SWIZZLECSR1_CLK_OFF_CLRMASK  equ     (~HW_SWIZZLECSR1_CLK_OFF_SETMASK)
HW_SWIZZLECSR1_NEWADD_CLRMASK   equ     (~HW_SWIZZLECSR1_NEWADD_SETMASK)
HW_SWIZZLECSR1_RSVD_CLRMASK     equ     (~HW_SWIZZLECSR1_RSVD_SETMASK)

;C insert
;////////////////////////////////////////////////////////////////////////////////
;//  Bit Manipulation Unit Registers
;////////////////////////////////////////////////////////////////////////////////
;typedef union
;{
;    struct
;    {
;    int EN      :1;     /* Swizzle Enable                           */
;    int LA      :1;     /* Left Align Data                          */
;    int LNR     :1;     /* Left Barrel Shift                        */
;    int SIGN    :1;     /* Sign Extend Data                         */
;    unsigned SHIFT :4;          /* Number of positions to shift (0 to 23)   */
;    int MEM     :1;     /* Manipulate in Memory (not in registers)  */
;    int CLK_OFF :1;     /* Gate the Clock, Power Off                */
;    int NEWADD  :1;     /* Place the data into a new location       */     
;    } B;
;    int I;
;    unsigned U;
;} swizzlecsr1_type;
;////////////////////////////////////////////////////////////////////////////////

;#define HW_SWIZZLECS1R (*(volatile swizzlecsr1_type _X*) (HW_SWIZZLE_BASEADDR))       /* Swizzle Control & Status Register 1 */
;endc

;//  Swizzle CSR2 (HW_SWIZZLECSR2) Bit Definitions
HW_SWIZZLECSR2_KICK_BITPOS	    equ 	(0)
HW_SWIZZLECSR2_SASEL_BITPOS   	equ     (1)
HW_SWIZZLECSR2_DESASEL_BITPOS   equ     (3)
HW_SWIZZLECSR2_BIGE_BITPOS      equ     (5)
HW_SWIZZLECSR2_BITREV_BITPOS    equ     (6)
HW_SWIZZLECSR2_PLSB_BITPOS      equ     (7)
HW_SWIZZLECSR2_PISB_OFF_BITPOS  equ     (8)
HW_SWIZZLECSR2_PMSB_BITPOS      equ     (9)
HW_SWIZZLECSR2_P16L_BITPOS      equ     (10)
HW_SWIZZLECSR2_P16I_BITPOS      equ     (11)
HW_SWIZZLECSR2_BS_EN_BITPOS     equ     (12)
HW_SWIZZLECSR2_SBYTEDEST_BITPOS equ     (13)
HW_SWIZZLECSR2_UNKICK_BITPOS    equ     (15)
HW_SWIZZLECSR2_RSVD_BITPOS      equ     (16)

HW_SWIZZLECSR2_KICK_WIDTH 	    equ 	(1)
HW_SWIZZLECSR2_SASEL_WIDTH      equ     (2)
HW_SWIZZLECSR2_DESASEL_WIDTH    equ     (2)
HW_SWIZZLECSR2_BIGE_WIDTH       equ     (1)
HW_SWIZZLECSR2_BITREV_WIDTH     equ     (1)
HW_SWIZZLECSR2_PLSB_WIDTH       equ     (1)
HW_SWIZZLECSR2_PMSB_WIDTH       equ     (1)
HW_SWIZZLECSR2_P16L_WIDTH       equ     (1)
HW_SWIZZLECSR2_P16I_WIDTH       equ     (1)
HW_SWIZZLECSR2_BS_EN_WIDTH      equ     (1)
HW_SWIZZLECSR2_SBYTEDEST_WIDTH  equ     (2)
HW_SWIZZLECSR2_UNKICK_WIDTH     equ     (1)
HW_SWIZZLECSR2_RSVD_WIDTH       equ     (8)

HW_SWIZZLECSR2_KICK_SETMASK     equ     (((1<HW_SWIZZLECSR2_KICK_WIDTH)-1)<<HW_SWIZZLECSR2_KICK_BITPOS)
HW_SWIZZLECSR2_SASEL_SETMASK    equ     (((1<HW_SWIZZLECSR2_SASEL_WIDTH)-1)<<HW_SWIZZLECSR2_SASEL_BITPOS)
HW_SWIZZLECSR2_DESASEL_SETMASK  equ     (((1<HW_SWIZZLECSR2_DESASEL_WIDTH)-1)<<HW_SWIZZLECSR2_DESASEL_BITPOS)
HW_SWIZZLECSR2_BIGE_SETMASK     equ     (((1<HW_SWIZZLECSR2_BIGE_WIDTH)-1)<<HW_SWIZZLECSR2_BIGE_BITPOS)
HW_SWIZZLECSR2_BITREV_SETMASK   equ     (((1<HW_SWIZZLECSR2_BITREV_WIDTH)-1)<<HW_SWIZZLECSR2_BITREV_BITPOS)
HW_SWIZZLECSR2_PLSB_SETMASK     equ     (((1<HW_SWIZZLECSR2_PLSB_WIDTH)-1)<<HW_SWIZZLECSR2_PLSB_BITPOS)
HW_SWIZZLECSR2_PMSB_SETMASK     equ     (((1<HW_SWIZZLECSR2_PMSB_WIDTH)-1)<<HW_SWIZZLECSR2_PMSB_BITPOS)
HW_SWIZZLECSR2_P16L_SETMASK     equ     (((1<HW_SWIZZLECSR2_P16L_WIDTH)-1)<<HW_SWIZZLECSR2_P16L_BITPOS)
HW_SWIZZLECSR2_P16I_SETMASK     equ     (((1<HW_SWIZZLECSR2_P16I_WIDTH)-1)<<HW_SWIZZLECSR2_P16I_BITPOS)
HW_SWIZZLECSR2_BS_EN_SETMASK    equ     (((1<HW_SWIZZLECSR2_BS_EN_WIDTH)-1)<<HW_SWIZZLECSR2_BS_EN_BITPOS)
HW_SWIZZLECSR2_SBYTEDEST_SETMASK equ     (((1<HW_SWIZZLECSR2_SBYTEDEST_WIDTH)-1)<<HW_SWIZZLECSR2_SBYTEDEST_BITPOS)
HW_SWIZZLECSR2_UNKICK_SETMASK   equ     (((1<HW_SWIZZLECSR2_UNKICK_WIDTH)-1)<<HW_SWIZZLECSR2_UNKICK_BITPOS)
HW_SWIZZLECSR2_RSVD_SETMASK     equ     (((1<HW_SWIZZLECSR2_RSVD_WIDTH)-1)<<HW_SWIZZLECSR2_RSVD_BITPOS)

HW_SWIZZLECSR2_KICK_CLRMASK     equ     (~HW_SWIZZLECSR2_KICK_SETMASK)
HW_SWIZZLECSR2_SASEL_CLRMASK    equ     (~HW_SWIZZLECSR2_SASEL_SETMASK)
HW_SWIZZLECSR2_DESASEL_CLRMASK  equ     (~HW_SWIZZLECSR2_DESASEL_SETMASK)
HW_SWIZZLECSR2_BIGE_CLRMASK     equ     (~HW_SWIZZLECSR2_BIGE_SETMASK)
HW_SWIZZLECSR2_BITREV_CLRMASK   equ     (~HW_SWIZZLECSR2_BITREV_SETMASK)
HW_SWIZZLECSR2_PLSB_CLRMASK     equ     (~HW_SWIZZLECSR2_PLSB_SETMASK)
HW_SWIZZLECSR2_PMSB_CLRMASK     equ     (~HW_SWIZZLECSR2_PMSB_SETMASK)
HW_SWIZZLECSR2_P16L_CLRMASK     equ     (~HW_SWIZZLECSR2_P16L_SETMASK)
HW_SWIZZLECSR2_P16I_CLRMASK     equ     (~HW_SWIZZLECSR2_P16I_SETMASK)
HW_SWIZZLECSR2_BS_EN_CLRMASK    equ     (~HW_SWIZZLECSR2_BS_EN_SETMASK)
HW_SWIZZLECSR2_SBYTEDEST_CLRMASK equ     (~HW_SWIZZLECSR2_SBYTEDEST_SETMASK)
HW_SWIZZLECSR2_UNKICK_CLRMASK   equ     (~HW_SWIZZLECSR2_UNKICK_SETMASK)
HW_SWIZZLECSR2_RSVD_CLRMASK     equ     (~HW_SWIZZLECSR2_RSVD_SETMASK)

;C insert
;///////////////////////////////////////////////////////////////////////////////
;typedef union
;{
;    struct
;    {
;    int KICK    :1;         /* Start transfer                      */
;    unsigned SASEL :2;              /* Source memory Select                */
;    unsigned DESASEL :2;                    /* Destination memory Select           */
;    int BIGE        :1;             /* Big Endian Enable                   */
;    int BITREV      :1;     /* Bit reverse the data                */
;    int PLSB        :1;             /* Pass Least Significant Byte         */
;    int PISB        :1;     /* Pass Intermediate Byte              */
;    int PMSB        :1;     /* Pass Most Significant Byte          */
;    int P16L        :1;     /* Pass Least Significant word         */
;    int P16I        :1;     /* Pass Intermediate significant word  */
;    int BS_EN       :1;     /* Barrel Shift Enable                 */
;    unsigned SBYTE  :2;                     /* Start byte                          */
;    int UNKICK      :1;     /* Halt transfer                       */  
;    } B;
;    unsigned int I;
;    unsigned int U;
;} swizzlecsr2_type;
;///////////////////////////////////////////////////////////////////////////////

;#define HW_SWIZZLECS2R (*(volatile swizzlecsr2_type _X*) (HW_SWIZZLE_BASEADDR+1))     /* Swizzle Control & Status Register 2 */
;endc

;//  Swizzle SIZER (HW_SWIZZLESIZER) Bit Definitions
HW_SWIZZLESIZER_SIZE_BITPOS	        equ 	(0)
HW_SWIZZLESIZER_NEW_SHIFT_BITPOS    equ     (16)
HW_SWIZZLESIZER_RSVD_BITPOS         equ     (21)

HW_SWIZZLESIZER_SIZE_WIDTH	        equ 	(16)
HW_SWIZZLESIZER_NEW_SHIFT_WIDTH     equ     (5)
HW_SWIZZLESIZER_RSVD_WIDTH          equ     (3)

HW_SWIZZLESIZER_SIZE_SETMASK        equ     (((1<HW_SWIZZLESIZER_SIZE_WIDTH)-1)<<HW_SWIZZLESIZER_SIZE_BITPOS)
HW_SWIZZLESIZER_NEW_SHIFT_SETMASK   equ     (((1<HW_SWIZZLESIZER_NEW_SHIFT_WIDTH)-1)<<HW_SWIZZLESIZER_NEW_SHIFT_BITPOS)
HW_SWIZZLESIZER_RSVD_SETMASK        equ     (((1<HW_SWIZZLESIZER_RSVD_WIDTH)-1)<<HW_SWIZZLESIZER_RSVD_BITPOS)

HW_SWIZZLESIZER_SIZE_CLRMASK        equ     (~HW_SWIZZLESIZER_SIZE_SETMASK)
HW_SWIZZLESIZER_NEW_SHIFT_CLRMASK   equ     (~HW_SWIZZLESIZER_NEW_SHIFT_SETMASK)
HW_SWIZZLESIZER_RSVD_CLRMASK        equ     (~HW_SWIZZLESIZER_RSVD_SETMASK)

;C insert
;///////////////////////////////////////////////////////////////////////////////
;typedef union
;{
;    struct
;    {
;    unsigned SIZE      :16;        /* Number of memory words to manipulate */
;    unsigned NEW_SHIFT :5;         /* Source memory Select                 */
;    } B;
;    int I;
;    unsigned U;
;} swizzlesizer_type;
;///////////////////////////////////////////////////////////////////////////////

;#define HW_SWIZZLESIZER (*(volatile swizzlesizer_type _X*) (HW_SWIZZLE_BASEADDR+2))        /* Swizzle Transfer Size Register */

;#define HW_SWIZZLESOURCER (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+3))      /* Swizzle Source Address Register */
;#define HW_SWIZZLEDATA1R (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+4))       /* Swizzle Data1 Register */
;#define HW_SWIZZLEDATA2R (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+5))       /* Swizzle Data2 Register */
;#define HW_SWIZZLEDESTADDRR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+6))    /* Swizzle Destination Address Register */
;#define HW_SWIZZLEBIGENDIANR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+7))   /* Swizzle Big Endian Register */
;#define HW_SWIZZLEBITREVR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+8))      /* Swizzle BITREV Register */
;#define HW_SWIZZLEPASSLSBR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+9))     /* Swizzle Pass Least Significant Byte Register */
;#define HW_SWIZZLEPASSISBR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+10))    /* Swizzle Pass Intermediate Byte Register */
;#define HW_SWIZZLEPASSMSBR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+11))    /* Swizzle Pass Most Significant Byte Register */
;#define HW_SWIZZLEPASSLSWR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+12))    /* Swizzle Pass Least Significant Word Register */
;#define HW_SWIZZLEPASSISWR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+13))    /* Swizzle Pass Intermediate Significant Word Register */
;#define HW_SWIZZLEPASSMSWR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+14))    /* Swizzle Pass Most Significant Word Register */
;#define HW_SWIZZLEBARRELR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+15))    /* Swizzle Barrel Shift Register */
;#define HW_SWIZZLEDIV3LR (*(volatile unsigned int _X*) (HW_SWIZZLE_BASEADDR+16))    /* Swizzle Divide By 3 Lower Register */
;endc


;//  Swizzle DIV3UR (HW_SWIZZLEDIV3UR) Bit Definitions
HW_SWIZZLEDIV3UR_DIV3_UPPER_BITPOS  equ 	(0)
HW_SWIZZLEDIV3UR_RSVD0_BITPOS       equ     (8)
HW_SWIZZLEDIV3UR_REMAINDER_BITPOS   equ     (20)
HW_SWIZZLEDIV3UR_RSVD1_BITPOS       equ     (22)

HW_SWIZZLEDIV3UR_DIV3_UPPER_WIDTH	equ 	(8)
HW_SWIZZLEDIV3UR_RSVD0_WIDTH        equ     (12)
HW_SWIZZLEDIV3UR_REMAINDER_WIDTH    equ     (2)
HW_SWIZZLEDIV3UR_RSVD1_WIDTH        equ     (2)

HW_SWIZZLEDIV3UR_DIV3_UPPER_SETMASK equ     (((1<HW_SWIZZLEDIV3UR_DIV3_UPPER_WIDTH)-1)<<HW_SWIZZLEDIV3UR_DIV3_UPPER_BITPOS)
HW_SWIZZLEDIV3UR_RSVD0_SETMASK      equ     (((1<HW_SWIZZLEDIV3UR_RSVD0_WIDTH)-1)<<HW_SWIZZLEDIV3UR_RSVD0_BITPOS)
HW_SWIZZLEDIV3UR_REMAINDER_SETMASK  equ     (((1<HW_SWIZZLEDIV3UR_REMAINDER_WIDTH)-1)<<HW_SWIZZLEDIV3UR_REMAINDER_BITPOS)
HW_SWIZZLEDIV3UR_RSVD1_SETMASK      equ     (((1<HW_SWIZZLEDIV3UR_RSVD1_WIDTH)-1)<<HW_SWIZZLEDIV3UR_RSVD1_BITPOS)

HW_SWIZZLEDIV3UR_DIV3_UPPER_CLRMASK equ     (~HW_SWIZZLEDIV3UR_DIV3_UPPER_SETMASK)
HW_SWIZZLEDIV3UR_RSVD0_CLRMASK      equ     (~HW_SWIZZLEDIV3UR_RSVD0_SETMASK)
HW_SWIZZLEDIV3UR_REMAINDER_CLRMASK  equ     (~HW_SWIZZLEDIV3UR_REMAINDER_SETMASK)
HW_SWIZZLEDIV3UR_RSVD1_CLRMASK      equ     (~HW_SWIZZLEDIV3UR_RSVD1_SETMASK)

;C insert
;///////////////////////////////////////////////////////////////////////////////
;typedef union
;{
;    struct
;    {
;    unsigned DIV3_UPPER    :8;     /* Number of memory words to manipulate */
;    unsigned RSVD0         :12;         /* Source memory Select                 */
;    unsigned REMAINDER     :2;         /* Source memory Select                 */
;    } B;
;    int I;
;    unsigned U;
;} swizzlediv3ur_type;
;///////////////////////////////////////////////////////////////////////////////

;#define HW_SWIZZLEDIV3UR (*(volatile swizzlediv3ur_type _X*) (HW_SWIZZLE_BASEADDR+17))    /* Swizzle Divide By 3 Upper Register */
;endc

    endif
