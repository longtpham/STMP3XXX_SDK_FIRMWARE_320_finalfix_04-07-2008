    if !(@def(__HW_REVR))
__HW_REVR equ 1

;ASM Only
HW_REVR									equ     $FA02
;endasm

HW_GLUE_BASEADDR                        equ     $FA00

HW_REVR_RMN_BITPOS                      equ     (0)
HW_REVR_DCDCMODE_BITPOS                 equ     (5)
HW_REVR_RMJ_BITPOS                      equ     (8)

HW_REVR_RMN_WIDTH                       equ     (5)
HW_REVR_DCDCMODE_WIDTH                  equ     (3)

HW_REVR_DCDCMODE_SETMASK                equ     (((1<<HW_REVR_DCDCMODE_WIDTH)-1)<<HW_REVR_DCDCMODE_BITPOS)

HW_REVR_DCDCMODE_CLRMASK                equ     (~HW_REVR_DCDCMODE_SETMASK)


;///////////////////////////////////////////////////////////////////////////////
;  Revision Register (HW_REVR) bitfields and values. (read only)
;  June15 2004: C struct updated to be correct: 
;   Added DCDCMODE bitfield. Removed RMP bitfield. Reduced size of RMN bitfield to 5 bits.
;C Insert
;typedef union               
;{
;    struct {
;        unsigned RMN    :5;     //Minor Revision
;        unsigned DCDCMODE : 3;  //DCDC mode field function depends on DCDC mode pin strapping
;           #define DCDCMODE_7_CONVERTER1_2CH_BOOST_CONVERTER2_OFF      7
;           #define DCDCMODE_6_RESERVED                                 6
;           #define DCDCMODE_5_CONVERTER1_3CH_BOOST_CONVERTER2_OFF      5
;           #define DCDCMODE_4_RESERVED                                 4
;           #define DCDCMODE_3_CONVERTER1_1CH_BUCK_CONVERTER2_OFF       3
;           #define DCDCMODE_2_CONVERTER1_OFF_CONVERTER2_OFF            2
;           #define DCDCMODE_1_CONVERTER1_1CH_BUCK_CONVERTER2_1CH_BOOST 1
;           #define DCDCMODE_0_CONVERTER1_1CH_BUCK_CONVERTER2_1CH_BUCK  0
;           #define DCDCMODE_X_BUCK_MASKED_SETMASK 0x4
;        unsigned RMJ    :16;    //Major Revision
;    } B;
;
;    int I;
;
;} revr_type;
;#define HW_REVR (*(volatile revr_type _X*) (HW_GLUE_BASEADDR+2))
;endC

HW_REVR_DCDCMODE_B0_BITPOS				equ     5
HW_REVR_DCDCMODE_B1_BITPOS				equ     6
HW_REVR_DCDCMODE_B2_BITPOS				equ     7

HW_REVR_DCDCMODE_BUCK_MASK_POS          equ     HW_REVR_DCDCMODE_B2_BITPOS
; 3 bit bitfield: (HW_REVR_DCDCMODE_B2_BITPOS|HW_REVR_DCDCMODE_B1_BITPOS|HW_REVR_DCDCMODE_B0_BITPOS)
HW_REVR_DCDC_MODE_SETMASK               equ     $0000E0
HW_REVR_RMN_SETMASK                     equ     $FF<<HW_REVR_RMN_BITPOS
HW_REVR_RMJ_SETMASK                     equ     $FFFF<<HW_REVR_RMJ_BITPOS

HW_REVR_RMN_CLRMASK                     equ     ~HW_REVR_RMN_SETMASK
HW_REVR_RMJ_CLRMASK                     equ     ~HW_REVR_RMJ_SETMASK

    endif ;!@def(__HW_REVR)
