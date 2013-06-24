    if !(@def(__REGS_CLK_CONTROL_INC))
__REGS_CLK_CONTROL_INC equ 1

;ASM Only
HW_CCR                                  equ     $FA00      ; Clock control register
HW_RCR                                  equ     $FA01      ; Reset control register
HW_DCLKCNTL                             equ     $FFEA      ; DCLK counter LOW
HW_DCLKCNTU                             equ     $FFEB      ; DCLK counter HIGH
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  Clock Control Register (HW_CCR) Bit Positions
HW_CCR_CKRST_BITPOS                     equ     0
HW_CCR_LTC_BITPOS                       equ     1
HW_CCR_PLLEN_BITPOS                     equ     2
HW_CCR_XTLEN_BITPOS                     equ     3
HW_CCR_PLL_SOURCE_SEL_BITPOS            equ     4
HW_CCR_ADIV_B0_BITPOS                   equ     5
HW_CCR_ADIV_B1_BITPOS                   equ     6
HW_CCR_ADIV_B2_BITPOS                   equ     7
HW_CCR_CKSRC_BITPOS                     equ     8
HW_CCR_DDIV_BITPOS                      equ     9
HW_CCR_DDIV_B0_BITPOS                   equ     9
HW_CCR_DDIV_B1_BITPOS                   equ     10
HW_CCR_DDIV_B2_BITPOS                   equ     11
HW_CCR_PDIV_BITPOS                      equ     12
HW_CCR_PDIV_B0_BITPOS                   equ     12
HW_CCR_PDIV_B1_BITPOS                   equ     13
HW_CCR_PDIV_B2_BITPOS                   equ     14
HW_CCR_PDIV_B3_BITPOS                   equ     15
HW_CCR_PDIV_B4_BITPOS                   equ     16
HW_CCR_PWDN_BITPOS                      equ     17
HW_CCR_ACKEN_BITPOS                     equ     18
HW_CCR_LOCK_BITPOS                      equ     19
HW_CCR_DACDIV_BITPOS                    equ     20
HW_CCR_ADIV1_B0_BITPOS                  equ     20
HW_CCR_ADIV1_B1_BITPOS                  equ     21
HW_CCR_ADIV1_B2_BITPOS                  equ     22
HW_CCR_DDIV_MSB_BITPOS			equ	23

HW_CCR_CKRST_SETMASK                    equ     1<<HW_CCR_CKRST_BITPOS
HW_CCR_LTC_SETMASK                      equ     1<<HW_CCR_LTC_BITPOS
HW_CCR_PLLEN_SETMASK                    equ     1<<HW_CCR_PLLEN_BITPOS
HW_CCR_XTLEN_SETMASK                    equ     1<<HW_CCR_XTLEN_BITPOS
HW_CCR_ADCDIV_SETMASK                   equ     $7<<HW_CCR_ADIV_B0_BITPOS
HW_CCR_CKSRC_SETMASK                    equ     1<<HW_CCR_CKSRC_BITPOS
HW_CCR_DDIV_SETMASK                     equ     $7<<HW_CCR_DDIV_BITPOS
HW_CCR_PDIV_SETMASK                     equ     $1F<<HW_CCR_PDIV_BITPOS
HW_CCR_PWDN_SETMASK                     equ     1<<HW_CCR_PWDN_BITPOS
HW_CCR_ACKEN_SETMASK                    equ     1<<HW_CCR_ACKEN_BITPOS
HW_CCR_LOCK_SETMASK                     equ     1<<HW_CCR_LOCK_BITPOS
HW_CCR_DACDIV_SETMASK                   equ     $7<<HW_CCR_DACDIV_BITPOS

HW_CCR_CKRST_CLRMASK                    equ     ~HW_CCR_CKRST_SETMASK
HW_CCR_LTC_CLRMASK                      equ     ~HW_CCR_LTC_SETMASK
HW_CCR_PLLEN_CLRMASK                    equ     ~HW_CCR_PLLEN_SETMASK
HW_CCR_XTLEN_CLRMASK                    equ     ~HW_CCR_XTLEN_SETMASK
HW_CCR_ADCDIV_CLRMASK                   equ     ~HW_CCR_ADCDIV_SETMASK
HW_CCR_CKSRC_CLRMASK                    equ     ~HW_CCR_CKSRC_SETMASK
HW_CCR_DDIV_CLRMASK                     equ     ~HW_CCR_DDIV_SETMASK
HW_CCR_PDIV_CLRMASK                     equ     ~HW_CCR_PDIV_SETMASK
HW_CCR_PWDN_CLRMASK                     equ     ~HW_CCR_PWDN_SETMASK
HW_CCR_ACKEN_CLRMASK                    equ     ~HW_CCR_ACKEN_SETMASK
HW_CCR_LOCK_CLRMASK                     equ     ~HW_CCR_LOCK_SETMASK
HW_CCR_DACDIV_CLRMASK                   equ     ~HW_CCR_DACDIV_SETMASK

;C insert
;typedef union               
;{
;    struct
;    {
;        int CKRST       :1; // Clock Reset
;        int LTC         :1;
;        int PLLEN       :1;
;        int XTLEN       :1;
;        int FLB         :1;
;        unsigned ADIV   :3;
;        int CKSRC       :1;
;        unsigned DDIV   :3;
;        unsigned PDIV   :5;
;        int PWDN        :1;
;        int ACKEN       :1;
;        int LOCK        :1;
;        unsigned ADIV1  :3;
;        unsigned DDIV_MSB:1;
;    } B;
;
;    int I;
;    unsigned int U;
;
;} ccr_type;
;#define HW_CCR     (*(volatile ccr_type _X*) (0xFA00))
;endc

;///////////////////////////////////////////////////////////////////////////////
;  Reset Control Register (HW_RCR) Bit Positions
HW_RCR_STKLVL_BITPOS                    equ     0
HW_RCR_SRST_BITPOS                      equ     4
HW_RCR_IRQA_BITPOS                      equ     8
HW_RCR_IRQB_BITPOS                      equ     9
HW_RCR_NMI_BITPOS                       equ     10
HW_RCR_SUNFLLVL_BITPOS                  equ     11
HW_RCR_SUNFLEN_BITPOS                   equ     15
HW_RCR_SOVFLLVL_BITPOS                  equ     16
HW_RCR_SOVFLEN_BITPOS                   equ     20
HW_RCR_IRQB2NMI_BITPOS                  equ     21
HW_RCR_SUNFL_BITPOS                     equ     22
HW_RCR_SOVFL_BITPOS                     equ     23

HW_RCR_STKLVL_WIDTH                     equ     (4)
HW_RCR_SRST_WIDTH                       equ     (4)
HW_RCR_IRQA_WIDTH                       equ     (1)
HW_RCR_IRQB_WIDTH                       equ     (1)
HW_RCR_NMI_WIDTH                        equ     (1)
HW_RCR_SUNFLLVL_WIDTH                   equ     (4)
HW_RCR_SUNFLEN_WIDTH                    equ     (1)
HW_RCR_SOVFLLVL_WIDTH                   equ     (4)
HW_RCR_SOVFLEN_WIDTH                    equ     (1)
HW_RCR_IRQB2NMI_WIDTH                   equ     (1)
HW_RCR_SUNFL_WIDTH                      equ     (1)
HW_RCR_SOVFL_WIDTH                      equ     (1)

HW_RCR_STKLVL_SETMASK                   equ     (((1<<HW_RCR_STKLVL_WIDTH)-1)<<HW_RCR_STKLVL_BITPOS)
HW_RCR_SRST_SETMASK                     equ     (((1<<HW_RCR_SRST_WIDTH)-1)<<HW_RCR_SRST_BITPOS)
HW_RCR_IRQA_SETMASK                     equ     (((1<<HW_RCR_IRQA_WIDTH)-1)<<HW_RCR_IRQA_BITPOS)
HW_RCR_IRQB_SETMASK                     equ     (((1<<HW_RCR_IRQB_WIDTH)-1)<<HW_RCR_IRQB_BITPOS)
HW_RCR_NMI_SETMASK                      equ     (((1<<HW_RCR_NMI_WIDTH)-1)<<HW_RCR_NMI_BITPOS)
HW_RCR_SUNFLLVL_SETMASK                 equ     (((1<<HW_RCR_SUNFLLVL_WIDTH)-1)<<HW_RCR_SUNFLLVL_BITPOS)
HW_RCR_SUNFLEN_SETMASK                  equ     (((1<<HW_RCR_SUNFLEN_WIDTH)-1)<<HW_RCR_SUNFLEN_BITPOS)
HW_RCR_SOVFLLVL_SETMASK                 equ     (((1<<HW_RCR_SOVFLLVL_WIDTH)-1)<<HW_RCR_SOVFLLVL_BITPOS)
HW_RCR_SOVFLEN_SETMASK                  equ     (((1<<HW_RCR_SOVFLEN_WIDTH)-1)<<HW_RCR_SOVFLEN_BITPOS)
HW_RCR_IRQB2NMI_SETMASK                 equ     (((1<<HW_RCR_IRQB2NMI_WIDTH)-1)<<HW_RCR_IRQB2NMI_BITPOS)
HW_RCR_SUNFL_SETMASK                    equ     (((1<<HW_RCR_SUNFL_WIDTH)-1)<<HW_RCR_SUNFL_BITPOS)
HW_RCR_SOVFL_SETMASK                    equ     (((1<<HW_RCR_SOVFL_WIDTH)-1)<<HW_RCR_SOVFL_BITPOS)

HW_RCR_STKLVL_CLRMASK                   equ     ~HW_RCR_STKLVL_SETMASK
HW_RCR_SRST_CLRMASK                     equ     ~HW_RCR_SRST_SETMASK
HW_RCR_IRQA_CLRMASK                     equ     ~HW_RCR_IRQA_SETMASK
HW_RCR_IRQB_CLRMASK                     equ     ~HW_RCR_IRQB_SETMASK
HW_RCR_NMI_CLRMASK                      equ     ~HW_RCR_NMI_SETMASK
HW_RCR_SUNFLLVL_CLRMASK                 equ     ~HW_RCR_SUNFLLVL_SETMASK
HW_RCR_SUNFLEN_CLRMASK                  equ     ~HW_RCR_SUNFLEN_SETMASK
HW_RCR_SOVFLLVL_CLRMASK                 equ     ~HW_RCR_SOVFLLVL_SETMASK
HW_RCR_SOVFLEN_CLRMASK                   equ    ~HW_RCR_SOVFLEN_SETMASK
HW_RCR_IRQB2NMI_CLRMASK                 equ     ~HW_RCR_IRQB2NMI_SETMASK
HW_RCR_SUNFL_CLRMASK                    equ     ~HW_RCR_SUNFL_SETMASK
HW_RCR_SOVFL_CLRMASK                    equ     ~HW_RCR_SOVFL_SETMASK

;C insert
;typedef union               
;{
;    struct
;   {
;        int STKLVL   : HW_RCR_STKLVL_WIDTH;
;        int SRST     : HW_RCR_SRST_WIDTH;
;        int IRQA     : HW_RCR_IRQA_WIDTH;
;        int IRQB     : HW_RCR_IRQB_WIDTH;
;        int NMI      : HW_RCR_NMI_WIDTH;
;        int SUNFLLVL : HW_RCR_SUNFLLVL_WIDTH;
;        int SUNFLEN  : HW_RCR_SUNFLEN_WIDTH;
;        int SOVFLLVL : HW_RCR_SOVFLLVL_WIDTH;
;        int SOVFLEN  : HW_RCR_SOVFLEN_WIDTH;
;        int IRQB2NMI : HW_RCR_IRQB2NMI_WIDTH;
;        int SUNFL    : HW_RCR_SUNFL_WIDTH;
;        int SOVFL    : HW_RCR_SOVFL_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;
;
;} rcr_type;
;#define HW_RCR     (*(volatile rcr_type _X*) (0xFA01))
;endc


;///////////////////////////////////////////////////////////////////////////////
;  DCLK Count Lower register (HW_DCLKCNTL) Bit Positions
HW_DCLKCNTL_LOW_BITPOS                    equ     0

HW_DCLKCNTL_LOW_WIDTH                     equ     (24)        

HW_DCLKCNTL_LOW_SETMASK              equ     (((1<<HW_DCLKCNTL_LOW_WIDTH)-1)<<HW_DCLKCNTL_LOW_BITPOS) 
HW_DCLKCNTL_LOW_CLRMASK              equ     ~HW_DCLKCNTL_LOW_SETMASK
;C insert
;typedef union               
;{
;    struct
;   {
;        int LOW;
;    } B;
;
;    int I;
;    unsigned int U;
;
;} dclkcntl_type;
;#define HW_DCLKCNTL (*(volatile dclkcntl_type _X*) (0xFFEA))
;endc

;///////////////////////////////////////////////////////////////////////////////
;  DCLK Count UPPER register (HW_DCLKCNTU) Bit Positions
HW_DCLKCNTU_HIGH_BITPOS                    equ     0

HW_DCLKCNTU_HIGH_WIDTH                     equ     (24)        

HW_DCLKCNTU_HIGH_SETMASK              equ     (((1<<HW_DCLKCNTU_HIGH_WIDTH)-1)<<HW_DCLKCNTU_HIGH_BITPOS) 
HW_DCLKCNTU_HIGH_CLRMASK              equ     ~HW_DCLKCNTU_HIGH_SETMASK
;C insert
;typedef union               
;{
;    struct
;   {
;        int HIGH;
;    } B;
;
;    int I;
;    unsigned int U;
;
;} dclkcntu_type;
;#define HW_DCLKCNTU (*(volatile dclkcntu_type _X*) (0xFFEB))
;endc

 ;*********************  REGISTER ALIAS DEFINES TO MATCH LEGACY CODE *******************************
 ;
 ; The following defines were added to match regs3410.inc definition to build SDK2XXX code without needing 
 ; to update the actual files. Only the defines needed to build SDK2.400 were added. 

HW_CLK_CNT_L                            equ     $00FFEA         ; Clock count register (lower)
HW_CLK_CNT_U                            equ     $00FFEB         ; Clock count register (upper)
HW_CYC_STEAL                            equ     $00FFEC         ; Cycle steal count register

    endif
    
