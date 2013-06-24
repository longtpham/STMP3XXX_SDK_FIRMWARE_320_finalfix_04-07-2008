#if !(defined(__REGS_CLK_CONTROL_INC))
#define __REGS_CLK_CONTROL_INC 1


/////////////////////////////////////////////////////////////////////////////////
//  Clock Control Register (HW_CCR) Bit Positions
#define HW_CCR_CKRST_BITPOS 0
#define HW_CCR_LTC_BITPOS 1
#define HW_CCR_PLLEN_BITPOS 2
#define HW_CCR_XTLEN_BITPOS 3
#define HW_CCR_PLL_SOURCE_SEL_BITPOS 4
#define HW_CCR_ADIV_B0_BITPOS 5
#define HW_CCR_ADIV_B1_BITPOS 6
#define HW_CCR_ADIV_B2_BITPOS 7
#define HW_CCR_CKSRC_BITPOS 8
#define HW_CCR_DDIV_BITPOS 9
#define HW_CCR_DDIV_B0_BITPOS 9
#define HW_CCR_DDIV_B1_BITPOS 10
#define HW_CCR_DDIV_B2_BITPOS 11
#define HW_CCR_PDIV_BITPOS 12
#define HW_CCR_PDIV_B0_BITPOS 12
#define HW_CCR_PDIV_B1_BITPOS 13
#define HW_CCR_PDIV_B2_BITPOS 14
#define HW_CCR_PDIV_B3_BITPOS 15
#define HW_CCR_PDIV_B4_BITPOS 16
#define HW_CCR_PWDN_BITPOS 17
#define HW_CCR_ACKEN_BITPOS 18
#define HW_CCR_LOCK_BITPOS 19
#define HW_CCR_DACDIV_BITPOS 20
#define HW_CCR_ADIV1_B0_BITPOS 20
#define HW_CCR_ADIV1_B1_BITPOS 21
#define HW_CCR_ADIV1_B2_BITPOS 22
#define HW_CCR_DDIV_MSB_BITPOS 23

#define HW_CCR_CKRST_SETMASK 1<<HW_CCR_CKRST_BITPOS
#define HW_CCR_LTC_SETMASK 1<<HW_CCR_LTC_BITPOS
#define HW_CCR_PLLEN_SETMASK 1<<HW_CCR_PLLEN_BITPOS
#define HW_CCR_XTLEN_SETMASK 1<<HW_CCR_XTLEN_BITPOS
#define HW_CCR_ADCDIV_SETMASK 0x7<<HW_CCR_ADIV_B0_BITPOS
#define HW_CCR_CKSRC_SETMASK 1<<HW_CCR_CKSRC_BITPOS
#define HW_CCR_DDIV_SETMASK 0x7<<HW_CCR_DDIV_BITPOS
#define HW_CCR_PDIV_SETMASK 0x1F<<HW_CCR_PDIV_BITPOS
#define HW_CCR_PWDN_SETMASK 1<<HW_CCR_PWDN_BITPOS
#define HW_CCR_ACKEN_SETMASK 1<<HW_CCR_ACKEN_BITPOS
#define HW_CCR_LOCK_SETMASK 1<<HW_CCR_LOCK_BITPOS
#define HW_CCR_DACDIV_SETMASK 0x7<<HW_CCR_DACDIV_BITPOS

#define HW_CCR_CKRST_CLRMASK ~(WORD)HW_CCR_CKRST_SETMASK
#define HW_CCR_LTC_CLRMASK ~(WORD)HW_CCR_LTC_SETMASK
#define HW_CCR_PLLEN_CLRMASK ~(WORD)HW_CCR_PLLEN_SETMASK
#define HW_CCR_XTLEN_CLRMASK ~(WORD)HW_CCR_XTLEN_SETMASK
#define HW_CCR_ADCDIV_CLRMASK ~(WORD)HW_CCR_ADCDIV_SETMASK
#define HW_CCR_CKSRC_CLRMASK ~(WORD)HW_CCR_CKSRC_SETMASK
#define HW_CCR_DDIV_CLRMASK ~(WORD)HW_CCR_DDIV_SETMASK
#define HW_CCR_PDIV_CLRMASK ~(WORD)HW_CCR_PDIV_SETMASK
#define HW_CCR_PWDN_CLRMASK ~(WORD)HW_CCR_PWDN_SETMASK
#define HW_CCR_ACKEN_CLRMASK ~(WORD)HW_CCR_ACKEN_SETMASK
#define HW_CCR_LOCK_CLRMASK ~(WORD)HW_CCR_LOCK_SETMASK
#define HW_CCR_DACDIV_CLRMASK ~(WORD)HW_CCR_DACDIV_SETMASK

typedef union               
{
    struct
    {
        int CKRST       :1; // Clock Reset
        int LTC         :1;
        int PLLEN       :1;
        int XTLEN       :1;
        int FLB         :1;
        unsigned ADIV   :3;
        int CKSRC       :1;
        unsigned DDIV   :3;
        unsigned PDIV   :5;
        int PWDN        :1;
        int ACKEN       :1;
        int LOCK        :1;
        unsigned ADIV1  :3;
        unsigned DDIV_MSB:1;
    } B;

    int I;
    unsigned int U;

} ccr_type;
#define HW_CCR     (*(volatile ccr_type _X*) (0xFA00))

/////////////////////////////////////////////////////////////////////////////////
//  Reset Control Register (HW_RCR) Bit Positions
#define HW_RCR_STKLVL_BITPOS 0
#define HW_RCR_SRST_BITPOS 4
#define HW_RCR_IRQA_BITPOS 8
#define HW_RCR_IRQB_BITPOS 9
#define HW_RCR_NMI_BITPOS 10
#define HW_RCR_SUNFLLVL_BITPOS 11
#define HW_RCR_SUNFLEN_BITPOS 15
#define HW_RCR_SOVFLLVL_BITPOS 16
#define HW_RCR_SOVFLEN_BITPOS 20
#define HW_RCR_IRQB2NMI_BITPOS 21
#define HW_RCR_SUNFL_BITPOS 22
#define HW_RCR_SOVFL_BITPOS 23

#define HW_RCR_STKLVL_WIDTH (4)
#define HW_RCR_SRST_WIDTH (4)
#define HW_RCR_IRQA_WIDTH (1)
#define HW_RCR_IRQB_WIDTH (1)
#define HW_RCR_NMI_WIDTH (1)
#define HW_RCR_SUNFLLVL_WIDTH (4)
#define HW_RCR_SUNFLEN_WIDTH (1)
#define HW_RCR_SOVFLLVL_WIDTH (4)
#define HW_RCR_SOVFLEN_WIDTH (1)
#define HW_RCR_IRQB2NMI_WIDTH (1)
#define HW_RCR_SUNFL_WIDTH (1)
#define HW_RCR_SOVFL_WIDTH (1)

#define HW_RCR_STKLVL_SETMASK (((1<<HW_RCR_STKLVL_WIDTH)-1)<<HW_RCR_STKLVL_BITPOS)
#define HW_RCR_SRST_SETMASK (((1<<HW_RCR_SRST_WIDTH)-1)<<HW_RCR_SRST_BITPOS)
#define HW_RCR_IRQA_SETMASK (((1<<HW_RCR_IRQA_WIDTH)-1)<<HW_RCR_IRQA_BITPOS)
#define HW_RCR_IRQB_SETMASK (((1<<HW_RCR_IRQB_WIDTH)-1)<<HW_RCR_IRQB_BITPOS)
#define HW_RCR_NMI_SETMASK (((1<<HW_RCR_NMI_WIDTH)-1)<<HW_RCR_NMI_BITPOS)
#define HW_RCR_SUNFLLVL_SETMASK (((1<<HW_RCR_SUNFLLVL_WIDTH)-1)<<HW_RCR_SUNFLLVL_BITPOS)
#define HW_RCR_SUNFLEN_SETMASK (((1<<HW_RCR_SUNFLEN_WIDTH)-1)<<HW_RCR_SUNFLEN_BITPOS)
#define HW_RCR_SOVFLLVL_SETMASK (((1<<HW_RCR_SOVFLLVL_WIDTH)-1)<<HW_RCR_SOVFLLVL_BITPOS)
#define HW_RCR_SOVFLEN_SETMASK (((1<<HW_RCR_SOVFLEN_WIDTH)-1)<<HW_RCR_SOVFLEN_BITPOS)
#define HW_RCR_IRQB2NMI_SETMASK (((1<<HW_RCR_IRQB2NMI_WIDTH)-1)<<HW_RCR_IRQB2NMI_BITPOS)
#define HW_RCR_SUNFL_SETMASK (((1<<HW_RCR_SUNFL_WIDTH)-1)<<HW_RCR_SUNFL_BITPOS)
#define HW_RCR_SOVFL_SETMASK (((1<<HW_RCR_SOVFL_WIDTH)-1)<<HW_RCR_SOVFL_BITPOS)

#define HW_RCR_STKLVL_CLRMASK ~(WORD)HW_RCR_STKLVL_SETMASK
#define HW_RCR_SRST_CLRMASK ~(WORD)HW_RCR_SRST_SETMASK
#define HW_RCR_IRQA_CLRMASK ~(WORD)HW_RCR_IRQA_SETMASK
#define HW_RCR_IRQB_CLRMASK ~(WORD)HW_RCR_IRQB_SETMASK
#define HW_RCR_NMI_CLRMASK ~(WORD)HW_RCR_NMI_SETMASK
#define HW_RCR_SUNFLLVL_CLRMASK ~(WORD)HW_RCR_SUNFLLVL_SETMASK
#define HW_RCR_SUNFLEN_CLRMASK ~(WORD)HW_RCR_SUNFLEN_SETMASK
#define HW_RCR_SOVFLLVL_CLRMASK ~(WORD)HW_RCR_SOVFLLVL_SETMASK
#define HW_RCR_SOVFLEN_CLRMASK ~(WORD)HW_RCR_SOVFLEN_SETMASK
#define HW_RCR_IRQB2NMI_CLRMASK ~(WORD)HW_RCR_IRQB2NMI_SETMASK
#define HW_RCR_SUNFL_CLRMASK ~(WORD)HW_RCR_SUNFL_SETMASK
#define HW_RCR_SOVFL_CLRMASK ~(WORD)HW_RCR_SOVFL_SETMASK

typedef union               
{
    struct
   {
        int STKLVL   : HW_RCR_STKLVL_WIDTH;
        int SRST     : HW_RCR_SRST_WIDTH;
        int IRQA     : HW_RCR_IRQA_WIDTH;
        int IRQB     : HW_RCR_IRQB_WIDTH;
        int NMI      : HW_RCR_NMI_WIDTH;
        int SUNFLLVL : HW_RCR_SUNFLLVL_WIDTH;
        int SUNFLEN  : HW_RCR_SUNFLEN_WIDTH;
        int SOVFLLVL : HW_RCR_SOVFLLVL_WIDTH;
        int SOVFLEN  : HW_RCR_SOVFLEN_WIDTH;
        int IRQB2NMI : HW_RCR_IRQB2NMI_WIDTH;
        int SUNFL    : HW_RCR_SUNFL_WIDTH;
        int SOVFL    : HW_RCR_SOVFL_WIDTH;
    } B;

    int I;
    unsigned int U;

} rcr_type;
#define HW_RCR     (*(volatile rcr_type _X*) (0xFA01))


/////////////////////////////////////////////////////////////////////////////////
//  DCLK Count Lower register (HW_DCLKCNTL) Bit Positions
#define HW_DCLKCNTL_LOW_BITPOS 0

#define HW_DCLKCNTL_LOW_WIDTH (24)        

#define HW_DCLKCNTL_LOW_SETMASK (((1<<HW_DCLKCNTL_LOW_WIDTH)-1)<<HW_DCLKCNTL_LOW_BITPOS) 
#define HW_DCLKCNTL_LOW_CLRMASK ~(WORD)HW_DCLKCNTL_LOW_SETMASK
typedef union               
{
    struct
   {
        int LOW;
    } B;

    int I;
    unsigned int U;

} dclkcntl_type;
#define HW_DCLKCNTL (*(volatile dclkcntl_type _X*) (0xFFEA))

/////////////////////////////////////////////////////////////////////////////////
//  DCLK Count UPPER register (HW_DCLKCNTU) Bit Positions
#define HW_DCLKCNTU_HIGH_BITPOS 0

#define HW_DCLKCNTU_HIGH_WIDTH (24)        

#define HW_DCLKCNTU_HIGH_SETMASK (((1<<HW_DCLKCNTU_HIGH_WIDTH)-1)<<HW_DCLKCNTU_HIGH_BITPOS) 
#define HW_DCLKCNTU_HIGH_CLRMASK ~(WORD)HW_DCLKCNTU_HIGH_SETMASK
typedef union               
{
    struct
   {
        int HIGH;
    } B;

    int I;
    unsigned int U;

} dclkcntu_type;
#define HW_DCLKCNTU (*(volatile dclkcntu_type _X*) (0xFFEB))

//*********************  REGISTER ALIAS DEFINES TO MATCH LEGACY CODE *******************************
// The following defines were added to match regs3410.inc definition to build SDK2XXX code without needing 
// to update the actual files. Only the defines needed to build SDK2.400 were added. 

// Clock count register (lower)
#define HW_CLK_CNT_L 0x00FFEA         
// Clock count register (upper)
#define HW_CLK_CNT_U 0x00FFEB         
// Cycle steal count register
#define HW_CYC_STEAL 0x00FFEC         

#endif


