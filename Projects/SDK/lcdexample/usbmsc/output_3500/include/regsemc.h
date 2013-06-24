#if !(defined(__REGS_EMC_INC))
#define __REGS_EMC_INC 1

/////////////////////////////////////////////////////////////////////////////////
//   Module base addresses
/////////////////////////////////////////////////////////////////////////////////
#define HW_EMC_BASEADDR 0xF000

/////////////////////////////////////////////////////////////////////////////////
//  EMC Registers
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//  Flash Control Register (HW_FLCR) Bit Definitions

typedef union               /*Flash Control Register*/
{
    struct
    {
    int KICK        :1;
    int RW          :1;
    int TCIE        :1;
    int IRQP        :1;
    unsigned MMD    :2;
    unsigned NB     :11;
    unsigned RSVD   :4;
    int SRST        :1;
    } B;
    int I;
} flcr_type;

#define HW_FLCR_KICK_BITPOS 0
#define HW_FLCR_RW_BITPOS 1
#define HW_FLCR_TCIE_BITPOS 2
#define HW_FLCR_IRQP_BITPOS 3
#define HW_FLCR_MMD_BITPOS 4
#define HW_FLCR_NB_BITPOS 6
#define HW_FLCR_SRST_BITPOS 21

#define HW_FLCR_KICK_SETMASK 1<<HW_FLCR_KICK_BITPOS
#define HW_FLCR_RW_SETMASK 1<<HW_FLCR_RW_BITPOS
#define HW_FLCR_READ_KICK_SETMASK HW_FLCR_KICK_SETMASK|HW_FLCR_RW_SETMASK
#define HW_FLCR_WRITE_KICK_SETMASK HW_FLCR_KICK_SETMASK
#define HW_FLCR_TCIE_SETMASK 1<<HW_FLCR_TCIE_BITPOS
#define HW_FLCR_IRQP_SETMASK 1<<HW_FLCR_IRQP_BITPOS
#define HW_FLCR_MMD_SETMASK 3<<HW_FLCR_MMD_BITPOS
#define HW_FLCR_NB_SETMASK 0x7FF<<HW_FLCR_NB_BITPOS
#define HW_FLCR_SRST_SETMASK 1<<HW_FLCR_SRST_BITPOS

#define HW_FLCR_KICK_CLRMASK ~(WORD)HW_FLCR_KICK_SETMASK
#define HW_FLCR_RW_CLRMASK ~(WORD)HW_FLCR_RW_SETMASK
#define HW_FLCR_TCIE_CLRMASK ~(WORD)HW_FLCR_TCIE_SETMASK
#define HW_FLCR_IRQP_CLRMASK ~(WORD)HW_FLCR_IRQP_SETMASK
#define HW_FLCR_MMD_CLRMASK ~(WORD)HW_FLCR_MMD_SETMASK
#define HW_FLCR_NB_CLRMASK ~(WORD)HW_FLCR_NB_SETMASK
#define HW_FLCR_SRST_CLRMASK ~(WORD)HW_FLCR_SRST_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  Flash Start Address Low Register (HW_FLSALR) Bit Definitions

typedef union           /* Flash Start Address Low*/
{
    struct
    {
    unsigned XA     : 24;
    } B;
    int I;
} flsalr_type;

#define HW_FLSALR_XA_BITPOS 0

#define HW_FLSALR_XA_SETMASK 0xFFFFFF<<HW_FLSALR_XA_BITPOS

#define HW_FLSALR_XA_CLRMASK ~(WORD)HW_FLSALR_XA_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  Flash Start Address High Register (HW_FLSAHR) Bit Definitions

typedef union           /* Flash Start Address High*/
{
    struct
    {
    unsigned XA     :8;
    unsigned DA     :16;
    } B;
    int I;
} flsahr_type;

#define HW_FLSAHR_XA_BITPOS 0

#define HW_FLSAHR_XA_SETMASK 0xFFFFFF<<HW_FLSAHR_XA_BITPOS

#define HW_FLSAHR_XA_CLRMASK ~(WORD)HW_FLSAHR_XA_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  EMC Flash CompactFlash Control Register (HW_FLCFCR) Bit Definitions

typedef union           /* Flash CompactFlash Control Register*/
{
    struct
    {
        int WP          :1;
        int CDP         :1;
        unsigned SM     :2;
        int XATTR       :1;
        int CRST        :1;
        int XWT         :1;
        int RI          :1;
        int IFCE        :1;
        int ISCE        :1;
        int INCE        :1;
        int IFCS        :1;
        int ISCS        :1;
        int INCS        :1;
        unsigned CFAI   :2;
        int XDDI        :1;
        unsigned CS     :2;
        int CRE         :1;
        unsigned VS     :2;
        int DASP        :1;
        int MODE16      :1; 
    } B;
    int I;
} flcfcr_type;

#define HW_FLCFCR_WP_BITPOS 0
#define HW_FLCFCR_CDP_BITPOS 1
#define HW_FLCFCR_SM_BITPOS 2
#define HW_FLCFCR_XATTR_BITPOS 4
#define HW_FLCFCR_CRST_BITPOS 5
#define HW_FLCFCR_XWT_BITPOS 6
#define HW_FLCFCR_RI_BITPOS 7
#define HW_FLCFCR_IFCE_BITPOS 8
#define HW_FLCFCR_ISCE_BITPOS 9
#define HW_FLCFCR_INCE_BITPOS 10
#define HW_FLCFCR_IFCS_BITPOS 11
#define HW_FLCFCR_ISCS_BITPOS 12
#define HW_FLCFCR_INCS_BITPOS 13
#define HW_FLCFCR_CFAI_BITPOS 14
#define HW_FLCFCR_XDDI_BITPOS 16
#define HW_FLCFCR_CS_BITPOS 17
#define HW_FLCFCR_CRE_BITPOS 19
#define HW_FLCFCR_VS_BITPOS 20
#define HW_FLCFCR_DASP_BITPOS 22

#define HW_FLCFCR_WP_SETMASK 1<<HW_FLCFCR_WP_BITPOS
#define HW_FLCFCR_CDP_SETMASK 1<<HW_FLCFCR_CDP_BITPOS
#define HW_FLCFCR_SM_SETMASK 3<<HW_FLCFCR_SM_BITPOS
#define HW_FLCFCR_XATTR_SETMASK 1<<HW_FLCFCR_XATTR_BITPOS
#define HW_FLCFCR_CRST_SETMASK 1<<HW_FLCFCR_CRST_BITPOS
#define HW_FLCFCR_XWT_SETMASK 1<<HW_FLCFCR_XWT_BITPOS
#define HW_FLCFCR_RI_SETMASK 1<<HW_FLCFCR_RI_BITPOS
#define HW_FLCFCR_IFCE_SETMASK 1<<HW_FLCFCR_IFCE_BITPOS
#define HW_FLCFCR_ISCE_SETMASK 1<<HW_FLCFCR_ISCE_BITPOS
#define HW_FLCFCR_INCE_SETMASK 1<<HW_FLCFCR_INCE_BITPOS
#define HW_FLCFCR_IFCS_SETMASK 1<<HW_FLCFCR_IFCS_BITPOS
#define HW_FLCFCR_ISCS_SETMASK 1<<HW_FLCFCR_ISCS_BITPOS
#define HW_FLCFCR_INCS_SETMASK 1<<HW_FLCFCR_INCS_BITPOS
#define HW_FLCFCR_CFAI_SETMASK 3<<HW_FLCFCR_CFAI_BITPOS
#define HW_FLCFCR_XDDI_SETMASK 1<<HW_FLCFCR_XDDI_BITPOS
#define HW_FLCFCR_CS_SETMASK 3<<HW_FLCFCR_CS_BITPOS
#define HW_FLCFCR_CRE_SETMASK 1<<HW_FLCFCR_CRE_BITPOS
#define HW_FLCFCR_VS_SETMASK 3<<HW_FLCFCR_VS_BITPOS
#define HW_FLCFCR_DASP_SETMASK 1<<HW_FLCFCR_DASP_BITPOS

#define HW_FLCFCR_WP_CLRMASK ~(WORD)HW_FLCFCR_WP_SETMASK
#define HW_FLCFCR_CDP_CLRMASK ~(WORD)HW_FLCFCR_CDP_SETMASK
#define HW_FLCFCR_SM_CLRMASK ~(WORD)HW_FLCFCR_SM_SETMASK
#define HW_FLCFCR_XATTR_CLRMASK ~(WORD)HW_FLCFCR_XATTR_SETMASK
#define HW_FLCFCR_CRST_CLRMASK ~(WORD)HW_FLCFCR_CRST_SETMASK
#define HW_FLCFCR_XWT_CLRMASK ~(WORD)HW_FLCFCR_XWT_SETMASK
#define HW_FLCFCR_RI_CLRMASK ~(WORD)HW_FLCFCR_RI_SETMASK
#define HW_FLCFCR_IFCE_CLRMASK ~(WORD)HW_FLCFCR_IFCE_SETMASK
#define HW_FLCFCR_ISCE_CLRMASK ~(WORD)HW_FLCFCR_ISCE_SETMASK
#define HW_FLCFCR_INCE_CLRMASK ~(WORD)HW_FLCFCR_INCE_SETMASK
#define HW_FLCFCR_IFCS_CLRMASK ~(WORD)HW_FLCFCR_IFCS_SETMASK
#define HW_FLCFCR_ISCS_CLRMASK ~(WORD)HW_FLCFCR_ISCS_SETMASK
#define HW_FLCFCR_INCS_CLRMASK ~(WORD)HW_FLCFCR_INCS_SETMASK
#define HW_FLCFCR_CFAI_CLRMASK ~(WORD)HW_FLCFCR_CFAI_SETMASK
#define HW_FLCFCR_XDDI_CLRMASK ~(WORD)HW_FLCFCR_XDDI_SETMASK
#define HW_FLCFCR_CS_CLRMASK ~(WORD)HW_FLCFCR_CS_SETMASK
#define HW_FLCFCR_CRE_CLRMASK ~(WORD)HW_FLCFCR_CRE_SETMASK
#define HW_FLCFCR_VS_CLRMASK ~(WORD)HW_FLCFCR_VS_SETMASK
#define HW_FLCFCR_DASP_CLRMASK ~(WORD)HW_FLCFCR_DASP_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  EMC Flash Compact Flash Timer1 Register (HW_FLCFTMR1R) Bit Definitions

typedef union           /* Flash CompactFlash Timer1 Register*/
{
    struct
    {
        unsigned TRWSU  :5;
        unsigned TRPW   :7;
        unsigned TWPW   :7;
        unsigned TRWH   :5;
    } B;
    int I;
} flcftmr1r_type;

#define HW_FLCFTMR1R_TRWSU_BITPOS 0
#define HW_FLCFTMR1R_TRPW_BITPOS 5
#define HW_FLCFTMR1R_TWPW_BITPOS 12
#define HW_FLCFTMR1R_TRWH_BITPOS 19

#define HW_FLCFTMR1R_TRWSU_SETMASK 0x1F<<HW_FLCFTMR1R_TRWSU_BITPOS
#define HW_FLCFTMR1R_TRPW_SETMASK 0x7F<<HW_FLCFTMR1R_TRPW_BITPOS
#define HW_FLCFTMR1R_TWPW_SETMASK 0x7F<<HW_FLCFTMR1R_TWPW_BITPOS
#define HW_FLCFTMR1R_TRWH_SETMASK 0x1F<<HW_FLCFTMR1R_TRWH_BITPOS

#define HW_FLCFTMR1R_TRWSU_CLRMASK ~(WORD)HW_FLCFTMR1R_TRWSU_SETMASK
#define HW_FLCFTMR1R_TRPW_CLRMASK ~(WORD)HW_FLCFTMR1R_TRPW_SETMASK
#define HW_FLCFTMR1R_TWPW_CLRMASK ~(WORD)HW_FLCFTMR1R_TWPW_SETMASK
#define HW_FLCFTMR1R_TRWH_CLRMASK ~(WORD)HW_FLCFTMR1R_TRWH_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  EMC Flash Compact Flash Timer1 Register (HW_FLCFTMR2R) Bit Definitions

typedef union           /* Flash CompactFlash Timer2 Register*/
{
    struct
    {
        unsigned TWW    :4;
        unsigned TWTO   :10;
        unsigned THW    :5; 
        unsigned TRAQ   :5;
    } B;
    int I;
} flcftmr2r_type;

#define HW_FLCFTMR2R_TWW_BITPOS 0
#define HW_FLCFTMR2R_TWTO_BITPOS 4
#define HW_FLCFTMR2R_THW_BITPOS 14
#define HW_FLCFTMR2R_TRAQ_BITPOS 19

#define HW_FLCFTMR2R_TWW_SETMASK 0xF<<HW_FLCFTMR2R_TWW_BITPOS
#define HW_FLCFTMR2R_TWTO_SETMASK 0x3FF<<HW_FLCFTMR2R_TWTO_BITPOS
#define HW_FLCFTMR2R_THW_SETMASK 0x1F<<HW_FLCFTMR2R_THW_BITPOS
#define HW_FLCFTMR2R_TRAQ_SETMASK 0x1F<<HW_FLCFTMR2R_TRAQ_BITPOS

#define HW_FLCFTMR2R_TWW_CLRMASK ~(WORD)HW_FLCFTMR2R_TWW_SETMASK
#define HW_FLCFTMR2R_TWTO_CLRMASK ~(WORD)HW_FLCFTMR2R_TWTO_SETMASK
#define HW_FLCFTMR2R_THW_CLRMASK ~(WORD)HW_FLCFTMR2R_THW_SETMASK
#define HW_FLCFTMR2R_TRAQ_CLRMASK ~(WORD)HW_FLCFTMR2R_TRAQ_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  EMC Flash SmartMedia Control Register (HW_FLSMCR) Bit Definitions

typedef union           /* Flash SmartMedia Control Register*/
{
    struct
    {
        unsigned CS     :2;
        int SE          :1;
        int WP          :1;
        int SIZE        :1;
        int ICMD        :8;
        int TOIE        :1;
        int BPIE        :1;
        int TOIRQ       :1;
        int BPIRQ       :1;
    } B;
    int I;
} flsmcr_type;

#define HW_FLSMCR_CS_BITPOS 0
#define HW_FLSMCR_SE_BITPOS 2
#define HW_FLSMCR_WP_BITPOS 3
#define HW_FLSMCR_SIZE_BITPOS 4
#define HW_FLSMCR_ICMD_BITPOS 5
#define HW_FLSMCR_TOIE_BITPOS 13
#define HW_FLSMCR_BPIE_BITPOS 14
#define HW_FLSMCR_TOIRQ_BITPOS 15
#define HW_FLSMCR_BPIRQ_BITPOS 16

#define HW_FLSMCR_CS_SETMASK 1<<HW_FLSMCR_CS_BITPOS
#define HW_FLSMCR_SE_SETMASK 1<<HW_FLSMCR_SE_BITPOS
#define HW_FLSMCR_WP_SETMASK 1<<HW_FLSMCR_WP_BITPOS
#define HW_FLSMCR_SIZE_SETMASK 1<<HW_FLSMCR_SIZE_BITPOS
#define HW_FLSMCR_ICMD_SETMASK (0xFF)<<HW_FLSMCR_ICMD_BITPOS
#define HW_FLSMCR_TOIE_SETMASK 1<<HW_FLSMCR_TOIE_BITPOS
#define HW_FLSMCR_BPIE_SETMASK 1<<HW_FLSMCR_BPIE_BITPOS
#define HW_FLSMCR_TOIRQ_SETMASK 1<<HW_FLSMCR_TOIRQ_BITPOS
#define HW_FLSMCR_BPIRQ_SETMASK 1<<HW_FLSMCR_BPIRQ_BITPOS

#define HW_FLSMCR_SE_ASSERT_SETMASK 0x000000
#define HW_FLSMCR_SE_DEASSERT_SETMASK 0x000004
#define HW_FLSMCR_WP_ASSERT_SETMASK 0x000000
#define HW_FLSMCR_WP_DEASSERT_SETMASK 0x000008
#define HW_FLSMCR_SIZE_SMALL_SETMASK 0x000000
#define HW_FLSMCR_SIZE_LARGE_SETMASK 0x000010
#define HW_FLSMCR_ICMD_RESET_SETMASK 0x001FE0
#define HW_FLSMCR_ICMD_READ_STATUS_SETMASK 0x000E00
#define HW_FLSMCR_ICMD_BLOCK_ERASE_SETMASK 0x000C00
#define HW_FLSMCR_ICMD_ERASE_SETMASK 0x001A00
#define HW_FLSMCR_ICMD_RP_FIRST_SETMASK 0x000000
#define HW_FLSMCR_ICMD_RP_SECOND_SETMASK 0x000020
#define HW_FLSMCR_ICMD_RP_SPARE_SETMASK 0x000A00
#define HW_FLSMCR_ICMD_READ_ID_SETMASK 0x001200

#define HW_FLSMCR_CS_CLRMASK ~(WORD)HW_FLSMCR_CS_SETMASK
#define HW_FLSMCR_SE_CLRMASK ~(WORD)HW_FLSMCR_SE_SETMASK
#define HW_FLSMCR_WP_CLRMASK ~(WORD)HW_FLSMCR_WP_SETMASK
#define HW_FLSMCR_SIZE_CLRMASK ~(WORD)HW_FLSMCR_SIZE_SETMASK
#define HW_FLSMCR_ICMD_CLRMASK ~(WORD)HW_FLSMCR_ICMD_SETMASK
#define HW_FLSMCR_TOIE_CLRMASK ~(WORD)HW_FLSMCR_TOIE_SETMASK
#define HW_FLSMCR_BPIE_CLRMASK ~(WORD)HW_FLSMCR_BPIE_SETMASK
#define HW_FLSMCR_TOIRQ_CLRMASK ~(WORD)HW_FLSMCR_TOIRQ_SETMASK
#define HW_FLSMCR_BPIRQ_CLRMASK ~(WORD)HW_FLSMCR_BPIRQ_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  EMC Flash SmartMedia Timer1 Register (HW_FLSMTMR1R) Bit Definitions

typedef union           /* Flash SmartMedia Timer1 Register*/
{
    struct
    {
        unsigned TRWSU  :5;
        unsigned TRPW   :6;
        unsigned TWPW   :6;
        unsigned TRWH   :5;
    } B;
    int I;
} flsmtmr1r_type;

#define HW_FLSMTMR1R_TRWSU_BITPOS 0
#define HW_FLSMTMR1R_TRPW_BITPOS 5
#define HW_FLSMTMR1R_TWPW_BITPOS 11
#define HW_FLSMTMR1R_TRWH_BITPOS 17

#define HW_FLSMTMR1R_TRWSU_SETMASK 0x1F<<HW_FLSMTMR1R_TRWSU_BITPOS
#define HW_FLSMTMR1R_TRPW_SETMASK 0x3F<<HW_FLSMTMR1R_TRPW_BITPOS
#define HW_FLSMTMR1R_TWPW_SETMASK 0x3F<<HW_FLSMTMR1R_TWPW_BITPOS
#define HW_FLSMTMR1R_TRWH_SETMASK 0x1F<<HW_FLSMTMR1R_TRWH_BITPOS

#define HW_FLSMTMR1R_TRWSU_CLRMASK ~(WORD)HW_FLSMTMR1R_TRWSU_SETMASK
#define HW_FLSMTMR1R_TRPW_CLRMASK ~(WORD)HW_FLSMTMR1R_TRPW_SETMASK
#define HW_FLSMTMR1R_TWPW_CLRMASK ~(WORD)HW_FLSMTMR1R_TWPW_SETMASK
#define HW_FLSMTMR1R_TRWH_CLRMASK ~(WORD)HW_FLSMTMR1R_TRWH_SETMASK


/////////////////////////////////////////////////////////////////////////////////
//  EMC Flash SmartMedia Timer2 Register (HW_FLSMTMR2R) Bit Definitions

typedef union           /* Flash SmartMedia Timer2 Register*/
{
    struct
    {
        unsigned TWT    :6;
        unsigned TWTO   :18;
    } B;
    int I;
} flsmtmr2r_type;

#define HW_FLSMTMR2R_TWT_BITPOS 0
#define HW_FLSMTMR2R_TWTO_BITPOS 6

#define HW_FLSMTMR2R_TWT_SETMASK 0x3F<<HW_FLSMTMR2R_TWT_BITPOS
#define HW_FLSMTMR2R_TWTO_SETMASK 0x3FF<<HW_FLSMTMR2R_TWTO_BITPOS

#define HW_FLSMTMR2R_TWT_CLRMASK ~(WORD)HW_FLSMTMR2R_TWT_SETMASK
#define HW_FLSMTMR2R_TWTO_CLRMASK ~(WORD)HW_FLSMTMR2R_TWTO_SETMASK

/*//////////////////////////////////////////////////////////////////////////////
  //  EMC Flash Control Status Register2 (HW_FLCR2) Bit Definitions     */
typedef union 
{
  struct
  {
    unsigned ASEL     :2;        /* Memory Select */
    unsigned RA       :1;        /* Right Align word into 24bit memory for True IDE  xfers */
    unsigned LA       :1;        /* Left  Align word into 24bit memory for True IDE  xfers */
    unsigned NEGDMA   :1;        /* Inverts data from Flash to memory */
    unsigned NEGFL    :1;        /* Inverts data from memory to Flash */
    unsigned CLKOFF   :1;        /* Power down - turns clk off */
    int PAD0          :17;    
  } B;
  int I;
} flcr2_type;

/////////////////////////////////////////////////////////////////////////////////
//  EMC Flash SmartMedia Status Register (HW_FLSMSR) Bit Definitions
#define HW_FLSMSR_RDY_BITPOS 0
#define HW_FLSMSR_BUSY_BITPOS 7

#define HW_FLCR      (*(volatile flcr_type      _X*) (HW_EMC_BASEADDR))    /* EMC Flash Control Register */
#define HW_FLSALR    (*(volatile flsalr_type    _X*) (HW_EMC_BASEADDR+1))  /* EMC Flash Start Address Low Register */
#define HW_FLSAHR    (*(volatile flsahr_type    _X*) (HW_EMC_BASEADDR+2))  /* EMC Flash Start Address High Register */
#define HW_FLSSMPR   (*(volatile flssmpr_type   _X*) (HW_EMC_BASEADDR+3))  /*  */
#define HW_FLCR2     (*(volatile flcr2_type     _X*) (HW_EMC_BASEADDR+4))  /* EMC Flash Control Register2 */
#define HW_FLCFCR    (*(volatile flcfcr_type    _X*) (HW_EMC_BASEADDR+8))  /* EMC Flash CompactFlash Control Register*/
#define HW_FLCFTMR1R (*(volatile flcftmr1r_type _X*) (HW_EMC_BASEADDR+9))  /* EMC Flash Compact Flash Timer1 Register*/
#define HW_FLCFTMR2R (*(volatile flcftmr2r_type _X*) (HW_EMC_BASEADDR+10)) /* EMC Flash Compact Flash Timer2 Register*/
#define HW_FLSMCR    (*(volatile flsmcr_type    _X*) (HW_EMC_BASEADDR+16)) /* EMC Flash SmartMedia Control Register*/
#define HW_FLSMTMR1R (*(volatile flsmtmr1r_type _X*) (HW_EMC_BASEADDR+17)) /* EMC Flash SmartMedia Timer1 Register*/
#define HW_FLSMTMR2R (*(volatile flsmtmr2r_type _X*) (HW_EMC_BASEADDR+18)) /* EMC Flash SmartMedia Timer2 Register*/
#define HW_FLSMSR    (*(volatile flssmsr_type   _X*) (HW_EMC_BASEADDR+19)) /*  */

//*********************  REGISTER ALIAS DEFINES TO MATCH LEGACY CODE *******************************
// The following defines were added to match regs3410.inc definition to build SDK2XXX code without needing 
// to update the actual files. Only the defines needed to build SDK2.400 were added.

#define HW_FLC2R HW_EMC_BASEADDR+4

#endif

