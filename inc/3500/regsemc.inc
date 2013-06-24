    if !(@def(__REGS_EMC_INC))
__REGS_EMC_INC equ 1

;///////////////////////////////////////////////////////////////////////////////
;   Module base addresses
;///////////////////////////////////////////////////////////////////////////////
HW_EMC_BASEADDR                         equ     $F000

;///////////////////////////////////////////////////////////////////////////////
;  EMC Registers
;///////////////////////////////////////////////////////////////////////////////

;ASM Only
HW_FLCR                                 equ     HW_EMC_BASEADDR
HW_FLSALR                               equ     HW_EMC_BASEADDR+1
HW_FLSAHR                               equ     HW_EMC_BASEADDR+2
HW_FLSSMPR                              equ     HW_EMC_BASEADDR+3
HW_FLCR2                                equ     HW_EMC_BASEADDR+4
HW_FLCFCR                               equ     HW_EMC_BASEADDR+8
HW_FLCFTMR1R                            equ     HW_EMC_BASEADDR+9
HW_FLCFTMR2R                            equ     HW_EMC_BASEADDR+10
HW_FLSMCR                               equ     HW_EMC_BASEADDR+16
HW_FLSMTMR1R                            equ     HW_EMC_BASEADDR+17
HW_FLSMTMR2R                            equ     HW_EMC_BASEADDR+18
HW_FLSMSR                               equ     HW_EMC_BASEADDR+19
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  Flash Control Register (HW_FLCR) Bit Definitions

;C Insert
;typedef union               /*Flash Control Register*/
;{
;    struct
;    {
;    int KICK        :1;
;    int RW          :1;
;    int TCIE        :1;
;    int IRQP        :1;
;    unsigned MMD    :2;
;    unsigned NB     :11;
;    unsigned RSVD   :4;
;    int SRST        :1;
;    } B;
;    int I;
;} flcr_type;
;endc

HW_FLCR_KICK_BITPOS                     equ     0
HW_FLCR_RW_BITPOS                       equ     1
HW_FLCR_TCIE_BITPOS                     equ     2
HW_FLCR_IRQP_BITPOS                     equ     3
HW_FLCR_MMD_BITPOS                      equ     4
HW_FLCR_NB_BITPOS                       equ     6
HW_FLCR_SRST_BITPOS                     equ     21

HW_FLCR_KICK_SETMASK                    equ     1<<HW_FLCR_KICK_BITPOS
HW_FLCR_RW_SETMASK                      equ     1<<HW_FLCR_RW_BITPOS
HW_FLCR_READ_KICK_SETMASK               equ     HW_FLCR_KICK_SETMASK|HW_FLCR_RW_SETMASK
HW_FLCR_WRITE_KICK_SETMASK              equ     HW_FLCR_KICK_SETMASK
HW_FLCR_TCIE_SETMASK                    equ     1<<HW_FLCR_TCIE_BITPOS
HW_FLCR_IRQP_SETMASK                    equ     1<<HW_FLCR_IRQP_BITPOS
HW_FLCR_MMD_SETMASK                     equ     3<<HW_FLCR_MMD_BITPOS
HW_FLCR_NB_SETMASK                      equ     $7FF<<HW_FLCR_NB_BITPOS
HW_FLCR_SRST_SETMASK                    equ     1<<HW_FLCR_SRST_BITPOS

HW_FLCR_KICK_CLRMASK                    equ     ~HW_FLCR_KICK_SETMASK
HW_FLCR_RW_CLRMASK                      equ     ~HW_FLCR_RW_SETMASK
HW_FLCR_TCIE_CLRMASK                    equ     ~HW_FLCR_TCIE_SETMASK
HW_FLCR_IRQP_CLRMASK                    equ     ~HW_FLCR_IRQP_SETMASK
HW_FLCR_MMD_CLRMASK                     equ     ~HW_FLCR_MMD_SETMASK
HW_FLCR_NB_CLRMASK                      equ     ~HW_FLCR_NB_SETMASK
HW_FLCR_SRST_CLRMASK                    equ     ~HW_FLCR_SRST_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Flash Start Address Low Register (HW_FLSALR) Bit Definitions

;C Insert
;typedef union           /* Flash Start Address Low*/
;{
;    struct
;    {
;    unsigned XA     : 24;
;    } B;
;    int I;
;} flsalr_type;
;endc

HW_FLSALR_XA_BITPOS                     equ     0

HW_FLSALR_XA_SETMASK                    equ     $FFFFFF<<HW_FLSALR_XA_BITPOS

HW_FLSALR_XA_CLRMASK                    equ     ~HW_FLSALR_XA_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  Flash Start Address High Register (HW_FLSAHR) Bit Definitions

;C Insert
;typedef union           /* Flash Start Address High*/
;{
;    struct
;    {
;    unsigned XA     :8;
;    unsigned DA     :16;
;    } B;
;    int I;
;} flsahr_type;
;endc

HW_FLSAHR_XA_BITPOS                     equ     0

HW_FLSAHR_XA_SETMASK                    equ     $FFFFFF<<HW_FLSAHR_XA_BITPOS

HW_FLSAHR_XA_CLRMASK                    equ     ~HW_FLSAHR_XA_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  EMC Flash CompactFlash Control Register (HW_FLCFCR) Bit Definitions

;C Insert
;typedef union           /* Flash CompactFlash Control Register*/
;{
;    struct
;    {
;        int WP          :1;
;        int CDP         :1;
;        unsigned SM     :2;
;        int XATTR       :1;
;        int CRST        :1;
;        int XWT         :1;
;        int RI          :1;
;        int IFCE        :1;
;        int ISCE        :1;
;        int INCE        :1;
;        int IFCS        :1;
;        int ISCS        :1;
;        int INCS        :1;
;        unsigned CFAI   :2;
;        int XDDI        :1;
;        unsigned CS     :2;
;        int CRE         :1;
;        unsigned VS     :2;
;        int DASP        :1;
;        int MODE16      :1; 
;    } B;
;    int I;
;} flcfcr_type;
;endc

HW_FLCFCR_WP_BITPOS                     equ     0
HW_FLCFCR_CDP_BITPOS                    equ     1
HW_FLCFCR_SM_BITPOS                     equ     2
HW_FLCFCR_XATTR_BITPOS                  equ     4
HW_FLCFCR_CRST_BITPOS                   equ     5
HW_FLCFCR_XWT_BITPOS                    equ     6
HW_FLCFCR_RI_BITPOS                     equ     7
HW_FLCFCR_IFCE_BITPOS                   equ     8
HW_FLCFCR_ISCE_BITPOS                   equ     9
HW_FLCFCR_INCE_BITPOS                   equ     10
HW_FLCFCR_IFCS_BITPOS                   equ     11
HW_FLCFCR_ISCS_BITPOS                   equ     12
HW_FLCFCR_INCS_BITPOS                   equ     13
HW_FLCFCR_CFAI_BITPOS                   equ     14
HW_FLCFCR_XDDI_BITPOS                   equ     16
HW_FLCFCR_CS_BITPOS                     equ     17
HW_FLCFCR_CRE_BITPOS                    equ     19
HW_FLCFCR_VS_BITPOS                     equ     20
HW_FLCFCR_DASP_BITPOS                   equ     22

HW_FLCFCR_WP_SETMASK                    equ     1<<HW_FLCFCR_WP_BITPOS
HW_FLCFCR_CDP_SETMASK                   equ     1<<HW_FLCFCR_CDP_BITPOS
HW_FLCFCR_SM_SETMASK                    equ     3<<HW_FLCFCR_SM_BITPOS
HW_FLCFCR_XATTR_SETMASK                 equ     1<<HW_FLCFCR_XATTR_BITPOS
HW_FLCFCR_CRST_SETMASK                  equ     1<<HW_FLCFCR_CRST_BITPOS
HW_FLCFCR_XWT_SETMASK                   equ     1<<HW_FLCFCR_XWT_BITPOS
HW_FLCFCR_RI_SETMASK                    equ     1<<HW_FLCFCR_RI_BITPOS
HW_FLCFCR_IFCE_SETMASK                  equ     1<<HW_FLCFCR_IFCE_BITPOS
HW_FLCFCR_ISCE_SETMASK                  equ     1<<HW_FLCFCR_ISCE_BITPOS
HW_FLCFCR_INCE_SETMASK                  equ     1<<HW_FLCFCR_INCE_BITPOS
HW_FLCFCR_IFCS_SETMASK                  equ     1<<HW_FLCFCR_IFCS_BITPOS
HW_FLCFCR_ISCS_SETMASK                  equ     1<<HW_FLCFCR_ISCS_BITPOS
HW_FLCFCR_INCS_SETMASK                  equ     1<<HW_FLCFCR_INCS_BITPOS
HW_FLCFCR_CFAI_SETMASK                  equ     3<<HW_FLCFCR_CFAI_BITPOS
HW_FLCFCR_XDDI_SETMASK                  equ     1<<HW_FLCFCR_XDDI_BITPOS
HW_FLCFCR_CS_SETMASK                    equ     3<<HW_FLCFCR_CS_BITPOS
HW_FLCFCR_CRE_SETMASK                   equ     1<<HW_FLCFCR_CRE_BITPOS
HW_FLCFCR_VS_SETMASK                    equ     3<<HW_FLCFCR_VS_BITPOS
HW_FLCFCR_DASP_SETMASK                  equ     1<<HW_FLCFCR_DASP_BITPOS

HW_FLCFCR_WP_CLRMASK                    equ     ~HW_FLCFCR_WP_SETMASK
HW_FLCFCR_CDP_CLRMASK                   equ     ~HW_FLCFCR_CDP_SETMASK
HW_FLCFCR_SM_CLRMASK                    equ     ~HW_FLCFCR_SM_SETMASK
HW_FLCFCR_XATTR_CLRMASK                 equ     ~HW_FLCFCR_XATTR_SETMASK
HW_FLCFCR_CRST_CLRMASK                  equ     ~HW_FLCFCR_CRST_SETMASK
HW_FLCFCR_XWT_CLRMASK                   equ     ~HW_FLCFCR_XWT_SETMASK
HW_FLCFCR_RI_CLRMASK                    equ     ~HW_FLCFCR_RI_SETMASK
HW_FLCFCR_IFCE_CLRMASK                  equ     ~HW_FLCFCR_IFCE_SETMASK
HW_FLCFCR_ISCE_CLRMASK                  equ     ~HW_FLCFCR_ISCE_SETMASK
HW_FLCFCR_INCE_CLRMASK                  equ     ~HW_FLCFCR_INCE_SETMASK
HW_FLCFCR_IFCS_CLRMASK                  equ     ~HW_FLCFCR_IFCS_SETMASK
HW_FLCFCR_ISCS_CLRMASK                  equ     ~HW_FLCFCR_ISCS_SETMASK
HW_FLCFCR_INCS_CLRMASK                  equ     ~HW_FLCFCR_INCS_SETMASK
HW_FLCFCR_CFAI_CLRMASK                  equ     ~HW_FLCFCR_CFAI_SETMASK
HW_FLCFCR_XDDI_CLRMASK                  equ     ~HW_FLCFCR_XDDI_SETMASK
HW_FLCFCR_CS_CLRMASK                    equ     ~HW_FLCFCR_CS_SETMASK
HW_FLCFCR_CRE_CLRMASK                   equ     ~HW_FLCFCR_CRE_SETMASK
HW_FLCFCR_VS_CLRMASK                    equ     ~HW_FLCFCR_VS_SETMASK
HW_FLCFCR_DASP_CLRMASK                  equ     ~HW_FLCFCR_DASP_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  EMC Flash Compact Flash Timer1 Register (HW_FLCFTMR1R) Bit Definitions

;C Insert
;typedef union           /* Flash CompactFlash Timer1 Register*/
;{
;    struct
;    {
;        unsigned TRWSU  :5;
;        unsigned TRPW   :7;
;        unsigned TWPW   :7;
;        unsigned TRWH   :5;
;    } B;
;    int I;
;} flcftmr1r_type;
;endc

HW_FLCFTMR1R_TRWSU_BITPOS               equ     0
HW_FLCFTMR1R_TRPW_BITPOS                equ     5
HW_FLCFTMR1R_TWPW_BITPOS                equ     12
HW_FLCFTMR1R_TRWH_BITPOS                equ     19

HW_FLCFTMR1R_TRWSU_SETMASK              equ     $1F<<HW_FLCFTMR1R_TRWSU_BITPOS
HW_FLCFTMR1R_TRPW_SETMASK               equ     $7F<<HW_FLCFTMR1R_TRPW_BITPOS
HW_FLCFTMR1R_TWPW_SETMASK               equ     $7F<<HW_FLCFTMR1R_TWPW_BITPOS
HW_FLCFTMR1R_TRWH_SETMASK               equ     $1F<<HW_FLCFTMR1R_TRWH_BITPOS

HW_FLCFTMR1R_TRWSU_CLRMASK              equ     ~HW_FLCFTMR1R_TRWSU_SETMASK
HW_FLCFTMR1R_TRPW_CLRMASK               equ     ~HW_FLCFTMR1R_TRPW_SETMASK
HW_FLCFTMR1R_TWPW_CLRMASK               equ     ~HW_FLCFTMR1R_TWPW_SETMASK
HW_FLCFTMR1R_TRWH_CLRMASK               equ     ~HW_FLCFTMR1R_TRWH_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  EMC Flash Compact Flash Timer1 Register (HW_FLCFTMR2R) Bit Definitions

;C Insert
;typedef union           /* Flash CompactFlash Timer2 Register*/
;{
;    struct
;    {
;        unsigned TWW    :4;
;        unsigned TWTO   :10;
;        unsigned THW    :5; 
;        unsigned TRAQ   :5;
;    } B;
;    int I;
;} flcftmr2r_type;
;endc

HW_FLCFTMR2R_TWW_BITPOS                 equ      0
HW_FLCFTMR2R_TWTO_BITPOS                equ      4
HW_FLCFTMR2R_THW_BITPOS                 equ      14
HW_FLCFTMR2R_TRAQ_BITPOS                equ      19

HW_FLCFTMR2R_TWW_SETMASK                equ      $F<<HW_FLCFTMR2R_TWW_BITPOS
HW_FLCFTMR2R_TWTO_SETMASK               equ      $3FF<<HW_FLCFTMR2R_TWTO_BITPOS
HW_FLCFTMR2R_THW_SETMASK                equ      $1F<<HW_FLCFTMR2R_THW_BITPOS
HW_FLCFTMR2R_TRAQ_SETMASK               equ      $1F<<HW_FLCFTMR2R_TRAQ_BITPOS

HW_FLCFTMR2R_TWW_CLRMASK                equ      ~HW_FLCFTMR2R_TWW_SETMASK
HW_FLCFTMR2R_TWTO_CLRMASK               equ      ~HW_FLCFTMR2R_TWTO_SETMASK
HW_FLCFTMR2R_THW_CLRMASK                equ      ~HW_FLCFTMR2R_THW_SETMASK
HW_FLCFTMR2R_TRAQ_CLRMASK               equ      ~HW_FLCFTMR2R_TRAQ_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  EMC Flash SmartMedia Control Register (HW_FLSMCR) Bit Definitions

;C Insert
;typedef union           /* Flash SmartMedia Control Register*/
;{
;    struct
;    {
;        unsigned CS     :2;
;        int SE          :1;
;        int WP          :1;
;        int SIZE        :1;
;        int ICMD        :8;
;        int TOIE        :1;
;        int BPIE        :1;
;        int TOIRQ       :1;
;        int BPIRQ       :1;
;    } B;
;    int I;
;} flsmcr_type;
;endc

HW_FLSMCR_CS_BITPOS                     equ      0
HW_FLSMCR_SE_BITPOS                     equ      2
HW_FLSMCR_WP_BITPOS                     equ      3
HW_FLSMCR_SIZE_BITPOS                   equ      4
HW_FLSMCR_ICMD_BITPOS                   equ      5
HW_FLSMCR_TOIE_BITPOS                   equ      13
HW_FLSMCR_BPIE_BITPOS                   equ      14
HW_FLSMCR_TOIRQ_BITPOS                  equ      15
HW_FLSMCR_BPIRQ_BITPOS                  equ      16

HW_FLSMCR_CS_SETMASK                    equ      1<<HW_FLSMCR_CS_BITPOS
HW_FLSMCR_SE_SETMASK                    equ      1<<HW_FLSMCR_SE_BITPOS
HW_FLSMCR_WP_SETMASK                    equ      1<<HW_FLSMCR_WP_BITPOS
HW_FLSMCR_SIZE_SETMASK                  equ      1<<HW_FLSMCR_SIZE_BITPOS
HW_FLSMCR_ICMD_SETMASK                  equ      ($FF)<<HW_FLSMCR_ICMD_BITPOS
HW_FLSMCR_TOIE_SETMASK                  equ      1<<HW_FLSMCR_TOIE_BITPOS
HW_FLSMCR_BPIE_SETMASK                  equ      1<<HW_FLSMCR_BPIE_BITPOS
HW_FLSMCR_TOIRQ_SETMASK                 equ      1<<HW_FLSMCR_TOIRQ_BITPOS
HW_FLSMCR_BPIRQ_SETMASK                 equ      1<<HW_FLSMCR_BPIRQ_BITPOS

HW_FLSMCR_SE_ASSERT_SETMASK             equ      $000000
HW_FLSMCR_SE_DEASSERT_SETMASK           equ      $000004
HW_FLSMCR_WP_ASSERT_SETMASK             equ      $000000
HW_FLSMCR_WP_DEASSERT_SETMASK           equ      $000008
HW_FLSMCR_SIZE_SMALL_SETMASK            equ      $000000
HW_FLSMCR_SIZE_LARGE_SETMASK            equ      $000010
HW_FLSMCR_ICMD_RESET_SETMASK            equ      $001FE0
HW_FLSMCR_ICMD_READ_STATUS_SETMASK      equ      $000E00
HW_FLSMCR_ICMD_BLOCK_ERASE_SETMASK      equ      $000C00
HW_FLSMCR_ICMD_ERASE_SETMASK            equ      $001A00
HW_FLSMCR_ICMD_RP_FIRST_SETMASK         equ      $000000
HW_FLSMCR_ICMD_RP_SECOND_SETMASK        equ      $000020
HW_FLSMCR_ICMD_RP_SPARE_SETMASK         equ      $000A00
HW_FLSMCR_ICMD_READ_ID_SETMASK          equ      $001200

HW_FLSMCR_CS_CLRMASK                    equ      ~HW_FLSMCR_CS_SETMASK
HW_FLSMCR_SE_CLRMASK                    equ      ~HW_FLSMCR_SE_SETMASK
HW_FLSMCR_WP_CLRMASK                    equ      ~HW_FLSMCR_WP_SETMASK
HW_FLSMCR_SIZE_CLRMASK                  equ      ~HW_FLSMCR_SIZE_SETMASK
HW_FLSMCR_ICMD_CLRMASK                  equ      ~HW_FLSMCR_ICMD_SETMASK
HW_FLSMCR_TOIE_CLRMASK                  equ      ~HW_FLSMCR_TOIE_SETMASK
HW_FLSMCR_BPIE_CLRMASK                  equ      ~HW_FLSMCR_BPIE_SETMASK
HW_FLSMCR_TOIRQ_CLRMASK                 equ      ~HW_FLSMCR_TOIRQ_SETMASK
HW_FLSMCR_BPIRQ_CLRMASK                 equ      ~HW_FLSMCR_BPIRQ_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  EMC Flash SmartMedia Timer1 Register (HW_FLSMTMR1R) Bit Definitions

;C Insert
;typedef union           /* Flash SmartMedia Timer1 Register*/
;{
;    struct
;    {
;        unsigned TRWSU  :5;
;        unsigned TRPW   :6;
;        unsigned TWPW   :6;
;        unsigned TRWH   :5;
;    } B;
;    int I;
;} flsmtmr1r_type;
;endc

HW_FLSMTMR1R_TRWSU_BITPOS               equ      0
HW_FLSMTMR1R_TRPW_BITPOS                equ      5
HW_FLSMTMR1R_TWPW_BITPOS                equ      11
HW_FLSMTMR1R_TRWH_BITPOS                equ      17

HW_FLSMTMR1R_TRWSU_SETMASK              equ      $1F<<HW_FLSMTMR1R_TRWSU_BITPOS
HW_FLSMTMR1R_TRPW_SETMASK               equ      $3F<<HW_FLSMTMR1R_TRPW_BITPOS
HW_FLSMTMR1R_TWPW_SETMASK               equ      $3F<<HW_FLSMTMR1R_TWPW_BITPOS
HW_FLSMTMR1R_TRWH_SETMASK               equ      $1F<<HW_FLSMTMR1R_TRWH_BITPOS

HW_FLSMTMR1R_TRWSU_CLRMASK              equ      ~HW_FLSMTMR1R_TRWSU_SETMASK
HW_FLSMTMR1R_TRPW_CLRMASK               equ      ~HW_FLSMTMR1R_TRPW_SETMASK
HW_FLSMTMR1R_TWPW_CLRMASK               equ      ~HW_FLSMTMR1R_TWPW_SETMASK
HW_FLSMTMR1R_TRWH_CLRMASK               equ      ~HW_FLSMTMR1R_TRWH_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  EMC Flash SmartMedia Timer2 Register (HW_FLSMTMR2R) Bit Definitions

;C Insert
;typedef union           /* Flash SmartMedia Timer2 Register*/
;{
;    struct
;    {
;        unsigned TWT    :6;
;        unsigned TWTO   :18;
;    } B;
;    int I;
;} flsmtmr2r_type;
;endc

HW_FLSMTMR2R_TWT_BITPOS                 equ     0
HW_FLSMTMR2R_TWTO_BITPOS                equ     6

HW_FLSMTMR2R_TWT_SETMASK                equ     $3F<<HW_FLSMTMR2R_TWT_BITPOS
HW_FLSMTMR2R_TWTO_SETMASK               equ     $3FF<<HW_FLSMTMR2R_TWTO_BITPOS

HW_FLSMTMR2R_TWT_CLRMASK                equ     ~HW_FLSMTMR2R_TWT_SETMASK
HW_FLSMTMR2R_TWTO_CLRMASK               equ     ~HW_FLSMTMR2R_TWTO_SETMASK

;C Insert
;/*//////////////////////////////////////////////////////////////////////////////
;  //  EMC Flash Control Status Register2 (HW_FLCR2) Bit Definitions     */
;typedef union 
;{
;  struct
;  {
;    unsigned ASEL     :2;        /* Memory Select */
;    unsigned RA       :1;        /* Right Align word into 24bit memory for True IDE  xfers */
;    unsigned LA       :1;        /* Left  Align word into 24bit memory for True IDE  xfers */
;    unsigned NEGDMA   :1;        /* Inverts data from Flash to memory */
;    unsigned NEGFL    :1;        /* Inverts data from memory to Flash */
;    unsigned CLKOFF   :1;        /* Power down - turns clk off */
;    int PAD0          :17;    
;  } B;
;  int I;
;} flcr2_type;
;endc

;///////////////////////////////////////////////////////////////////////////////
;  EMC Flash SmartMedia Status Register (HW_FLSMSR) Bit Definitions
HW_FLSMSR_RDY_BITPOS                    equ     0
HW_FLSMSR_BUSY_BITPOS                   equ     7

;C Insert
;#define HW_FLCR      (*(volatile flcr_type      _X*) (HW_EMC_BASEADDR))    /* EMC Flash Control Register */
;#define HW_FLSALR    (*(volatile flsalr_type    _X*) (HW_EMC_BASEADDR+1))  /* EMC Flash Start Address Low Register */
;#define HW_FLSAHR    (*(volatile flsahr_type    _X*) (HW_EMC_BASEADDR+2))  /* EMC Flash Start Address High Register */
;#define HW_FLSSMPR   (*(volatile flssmpr_type   _X*) (HW_EMC_BASEADDR+3))  /*  */
;#define HW_FLCR2     (*(volatile flcr2_type     _X*) (HW_EMC_BASEADDR+4))  /* EMC Flash Control Register2 */
;#define HW_FLCFCR    (*(volatile flcfcr_type    _X*) (HW_EMC_BASEADDR+8))  /* EMC Flash CompactFlash Control Register*/
;#define HW_FLCFTMR1R (*(volatile flcftmr1r_type _X*) (HW_EMC_BASEADDR+9))  /* EMC Flash Compact Flash Timer1 Register*/
;#define HW_FLCFTMR2R (*(volatile flcftmr2r_type _X*) (HW_EMC_BASEADDR+10)) /* EMC Flash Compact Flash Timer2 Register*/
;#define HW_FLSMCR    (*(volatile flsmcr_type    _X*) (HW_EMC_BASEADDR+16)) /* EMC Flash SmartMedia Control Register*/
;#define HW_FLSMTMR1R (*(volatile flsmtmr1r_type _X*) (HW_EMC_BASEADDR+17)) /* EMC Flash SmartMedia Timer1 Register*/
;#define HW_FLSMTMR2R (*(volatile flsmtmr2r_type _X*) (HW_EMC_BASEADDR+18)) /* EMC Flash SmartMedia Timer2 Register*/
;#define HW_FLSMSR    (*(volatile flssmsr_type   _X*) (HW_EMC_BASEADDR+19)) /*  */
;endc

 ;*********************  REGISTER ALIAS DEFINES TO MATCH LEGACY CODE *******************************
 ;
 ; The following defines were added to match regs3410.inc definition to build SDK2XXX code without needing 
 ; to update the actual files. Only the defines needed to build SDK2.400 were added.

HW_FLC2R                                equ     HW_EMC_BASEADDR+4

    endif
