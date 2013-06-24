;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2005
;;  File        : regsusbphy.inc
;;  Description : USB20 PHY Register definition
;;  Updated 2.23.2003 By M. Henson
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

    if !(@def(regsusbphyinc))
regsusbphyinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   USB2.0 PHY STMP Registers 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HW_USBPHY_BASEADDR             equ     ($F210)

;ASM Only
HW_USBPHYPWD                   equ     (HW_USBPHY_BASEADDR)
HW_USBPHYTX                    equ     (HW_USBPHY_BASEADDR+1)
HW_USBPHYPLL                   equ     (HW_USBPHY_BASEADDR+2)
HW_USBPHYRX                    equ     (HW_USBPHY_BASEADDR+3)
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  USB PHY Powerdown Register (HW_USBPHYPWD) Bit Definitions

HW_USBPHYPWD_TXDISCON1500_BITPOS           equ     (5)
HW_USBPHYPWD_PLLVCOPWD_BITPOS              equ     (6)
HW_USBPHYPWD_PLLVCPPWD_BITPOS              equ     (7)
HW_USBPHYPWD_TXPWDFS_BITPOS                equ     (10)
HW_USBPHYPWD_TXPWDIBIAS_BITPOS             equ     (11)
HW_USBPHYPWD_TXPWDV2I_BITPOS               equ     (12)
HW_USBPHYPWD_TXPWDVBG_BITPOS               equ     (13)
HW_USBPHYPWD_TXPWDCOMP_BITPOS              equ     (14)
HW_USBPHYPWD_RXPWDDISCONDET_BITPOS         equ     (16)
HW_USBPHYPWD_RXPWDENV_BITPOS               equ     (17)
HW_USBPHYPWD_RXPWD1PT1_BITPOS              equ     (18)
HW_USBPHYPWD_RXPWDDIFF_BITPOS              equ     (19)
HW_USBPHYPWD_RXPWDRX_BITPOS                equ     (20)
HW_USBPHYPWD_PWDIBIAS_BITPOS               equ     (22)
HW_USBPHYPWD_REGRESET_BITPOS               equ     (23)

HW_USBPHYPWD_TXDISCON1500_SETMASK          equ     (1<<HW_USBPHYPWD_TXDISCON1500_BITPOS)
HW_USBPHYPWD_PLLVCOPWD_SETMASK             equ     (1<<HW_USBPHYPWD_PLLVCOPWD_BITPOS)
HW_USBPHYPWD_PLLVCPPWD_SETMASK             equ     (1<<HW_USBPHYPWD_PLLVCPPWD_BITPOS)
HW_USBPHYPWD_TXPWDFS_SETMASK               equ     (1<<HW_USBPHYPWD_TXPWDFS_BITPOS)
HW_USBPHYPWD_TXPWDIBIAS_SETMASK            equ     (1<<HW_USBPHYPWD_TXPWDIBIAS_BITPOS)
HW_USBPHYPWD_TXPWDV2I_SETMASK              equ     (1<<HW_USBPHYPWD_TXPWDV2I_BITPOS)
HW_USBPHYPWD_TXPWDVBG_SETMASK              equ     (1<<HW_USBPHYPWD_TXPWDVBG_BITPOS)
HW_USBPHYPWD_TXPWDCOMP_SETMASK             equ     (1<<HW_USBPHYPWD_TXPWDCOMP_BITPOS)
HW_USBPHYPWD_RXPWDDISCONDET_SETMASK        equ     (1<<HW_USBPHYPWD_RXPWDDISCONDET_BITPOS)
HW_USBPHYPWD_RXPWDENV_SETMASK              equ     (1<<HW_USBPHYPWD_RXPWDENV_BITPOS)
HW_USBPHYPWD_RXPWD1PT1_SETMASK             equ     (1<<HW_USBPHYPWD_RXPWD1PT1_BITPOS)
HW_USBPHYPWD_RXPWDDIFF_SETMASK             equ     (1<<HW_USBPHYPWD_RXPWDDIFF_BITPOS)
HW_USBPHYPWD_RXPWDRX_SETMASK               equ     (1<<HW_USBPHYPWD_RXPWDRX_BITPOS)
HW_USBPHYPWD_PWDIBIAS_SETMASK              equ     (1<<HW_USBPHYPWD_PWDIBIAS_BITPOS)
HW_USBPHYPWD_REGRESET_SETMASK              equ     (1<<HW_USBPHYPWD_REGRESET_BITPOS)

HW_USBPHYPWD_TXDISCON1500_CLRMASK          equ     (~HW_USBPHYPWD_TXDISCON1500_SETMASK)
HW_USBPHYPWD_PLLVCOPWD_CLRMASK             equ     (~HW_USBPHYPWD_PLLVCOPWD_SETMASK)
HW_USBPHYPWD_PLLVCPPWD_CLRMASK             equ     (~HW_USBPHYPWD_PLLVCPPWD_SETMASK)
HW_USBPHYPWD_TXPWDFS_CLRMASK               equ     (~HW_USBPHYPWD_TXPWDFS_SETMASK)
HW_USBPHYPWD_TXPWDIBIAS_CLRMASK            equ     (~HW_USBPHYPWD_TXPWDIBIAS_SETMASK)
HW_USBPHYPWD_TXPWDV2I_CLRMASK              equ     (~HW_USBPHYPWD_TXPWDV2I_SETMASK)
HW_USBPHYPWD_TXPWDVBG_CLRMASK              equ     (~HW_USBPHYPWD_TXPWDVBG_SETMASK)
HW_USBPHYPWD_TXPWDCOMP_CLRMASK             equ     (~HW_USBPHYPWD_TXPWDCOMP_SETMASK)
HW_USBPHYPWD_RXPWDDISCONDET_CLRMASK        equ     (~HW_USBPHYPWD_RXPWDDISCONDET_SETMASK)
HW_USBPHYPWD_RXPWDENV_CLRMASK              equ     (~HW_USBPHYPWD_RXPWDENV_SETMASK)
HW_USBPHYPWD_RXPWD1PT1_CLRMASK             equ     (~HW_USBPHYPWD_RXPWD1PT1_SETMASK)
HW_USBPHYPWD_RXPWDDIFF_CLRMASK             equ     (~HW_USBPHYPWD_RXPWDDIFF_SETMASK)
HW_USBPHYPWD_RXPWDRX_CLRMASK               equ     (~HW_USBPHYPWD_RXPWDRX_SETMASK)
HW_USBPHYPWD_PWDIBIAS_CLRMASK              equ     (~HW_USBPHYPWD_PWDIBIAS_SETMASK)
HW_USBPHYPWD_REGRESET_CLRMASK              equ     (~HW_USBPHYPWD_REGRESET_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int RSVD0          :5;
;        int TXDISCON1500   :1;
;        int PLLVCOPWD      :1;
;        int PLLVCPPWD      :1;
;        int RSVD1          :2;
;        int TXPWDFS        :1;
;        int TXPWDIBIAS     :1;
;        int TXPWDV2I       :1;
;        int TXPWDVBG       :1;
;        int TXPWDCOMP      :1;
;        int RSVD2          :1;
;        int RXPWDDISCONDET :1;
;        int RXPWDENV       :1;
;        int RXPWD1PT1      :1;
;        int RXPWDDIFF      :1;
;        int RXPWDRX        :1;
;        int RSVD3          :1;
;        int PWDIBIAS       :1;
;        int REGRESET       :1;
;    } B;
;    int I;
;} usbphypwd_type;

;#define HW_USBPHYPWD      (*(volatile usbphypwd_type _X*) (HW_USBPHY_BASEADDR))    /* USB PHY Powerdown Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB PHY Transmit register (HW_USBPHYTX) Bit Definitions
HW_USBPHYTX_TXCAL1500_BITPOS               equ     (0)
HW_USBPHYTX_TXENCAL1500_BITPOS             equ     (5)
HW_USBPHYTX_TXHSXCVR_BITPOS                equ     (6)
HW_USBPHYTX_TXCALIBRATE_BITPOS             equ     (7)
HW_USBPHYTX_TXCAL45DN_BITPOS               equ     (8)
HW_USBPHYTX_TXENCAL45DN_BITPOS             equ     (13)
HW_USBPHYTX_TXHSTERM_BITPOS                equ     (14)
HW_USBPHYTX_TXSKEW_BITPOS                  equ     (15)
HW_USBPHYTX_TXCAL45DP_BITPOS               equ     (16)
HW_USBPHYTX_TXENCAL45DP_BITPOS             equ     (21)
HW_USBPHYTX_TXFSHIZ_BITPOS                 equ     (22)
HW_USBPHYTX_TXCOMPOUT_BITPOS               equ     (23)

HW_USBPHYTX_TXCAL1500_WIDTH                equ     (4)
HW_USBPHYTX_TXENCAL1500_WIDTH              equ     (1)
HW_USBPHYTX_TXHSXCVR_WIDTH                 equ     (1)
HW_USBPHYTX_TXCALIBRATE_WIDTH              equ     (1)
HW_USBPHYTX_TXCAL45DN_WIDTH                equ     (4)
HW_USBPHYTX_TXENCAL45DN_WIDTH              equ     (1)
HW_USBPHYTX_TXHSTERM_WIDTH                 equ     (1)
HW_USBPHYTX_TXSKEW_WIDTH                   equ     (1)
HW_USBPHYTX_TXCAL45DP_WIDTH                equ     (4)
HW_USBPHYTX_TXENCAL45DP_WIDTH              equ     (1)
HW_USBPHYTX_TXFSHIZ_WIDTH                  equ     (1)
HW_USBPHYTX_TXCOMPOUT_WIDTH                equ     (1)

HW_USBPHYTX_TXCAL1500_SETMASK              equ     (((1<<HW_USBPHYTX_TXCAL1500_WIDTH)-1)<<HW_USBPHYTX_TXCAL1500_BITPOS)
HW_USBPHYTX_TXENCAL1500_SETMASK            equ     (((1<<HW_USBPHYTX_TXENCAL1500_WIDTH)-1)<<HW_USBPHYTX_TXENCAL1500_BITPOS)
HW_USBPHYTX_TXHSXCVR_SETMASK               equ     (((1<<HW_USBPHYTX_TXHSXCVR_WIDTH)-1)<<HW_USBPHYTX_TXHSXCVR_BITPOS)
HW_USBPHYTX_TXCALIBRATE_SETMASK            equ     (((1<<HW_USBPHYTX_TXCALIBRATE_WIDTH)-1)<<HW_USBPHYTX_TXCALIBRATE_BITPOS)
HW_USBPHYTX_TXCAL45DN_SETMASK              equ     (((1<<HW_USBPHYTX_TXCAL45DN_WIDTH)-1)<<HW_USBPHYTX_TXCAL45DN_BITPOS)
HW_USBPHYTX_TXENCAL45DN_SETMASK            equ     (((1<<HW_USBPHYTX_TXENCAL45DN_WIDTH)-1)<<HW_USBPHYTX_TXENCAL45DN_BITPOS)
HW_USBPHYTX_TXHSTERM_SETMASK               equ     (((1<<HW_USBPHYTX_TXHSTERM_WIDTH)-1)<<HW_USBPHYTX_TXHSTERM_BITPOS)
HW_USBPHYTX_TXSKEW_SETMASK                 equ     (((1<<HW_USBPHYTX_TXSKEW_WIDTH)-1)<<HW_USBPHYTX_TXSKEW_BITPOS)
HW_USBPHYTX_TXCAL45DP_SETMASK              equ     (((1<<HW_USBPHYTX_TXCAL45DP_WIDTH)-1)<<HW_USBPHYTX_TXCAL45DP_BITPOS)
HW_USBPHYTX_TXENCAL45DP_SETMASK            equ     (((1<<HW_USBPHYTX_TXENCAL45DP_WIDTH)-1)<<HW_USBPHYTX_TXENCAL45DP_BITPOS)
HW_USBPHYTX_TXFSHIZ_SETMASK                equ     (((1<<HW_USBPHYTX_TXFSHIZ_WIDTH)-1)<<HW_USBPHYTX_TXFSHIZ_BITPOS)
HW_USBPHYTX_TXCOMPOUT_SETMASK              equ     (((1<<HW_USBPHYTX_TXCOMPOUT_WIDTH)-1)<<HW_USBPHYTX_TXCOMPOUT_BITPOS)

HW_USBPHYTX_TXCAL1500_CLRMASK              equ     (~HW_USBPHYTX_TXCAL1500_SETMASK)
HW_USBPHYTX_TXENCAL1500_CLRMASK            equ     (~HW_USBPHYTX_TXENCAL1500_SETMASK)
HW_USBPHYTX_TXHSXCVR_CLRMASK               equ     (~HW_USBPHYTX_TXHSXCVR_SETMASK)
HW_USBPHYTX_TXCALIBRATE_CLRMASK            equ     (~HW_USBPHYTX_TXCALIBRATE_SETMASK)
HW_USBPHYTX_TXCAL45DN_CLRMASK              equ     (~HW_USBPHYTX_TXCAL45DN_SETMASK)
HW_USBPHYTX_TXENCAL45DN_CLRMASK            equ     (~HW_USBPHYTX_TXENCAL45DN_SETMASK)
HW_USBPHYTX_TXHSTERM_CLRMASK               equ     (~HW_USBPHYTX_TXHSTERM_SETMASK)
HW_USBPHYTX_TXSKEW_CLRMASK                 equ     (~HW_USBPHYTX_TXSKEW_SETMASK)
HW_USBPHYTX_TXCAL45DP_CLRMASK              equ     (~HW_USBPHYTX_TXCAL45DP_SETMASK)
HW_USBPHYTX_TXENCAL45DP_CLRMASK            equ     (~HW_USBPHYTX_TXENCAL45DP_SETMASK)
HW_USBPHYTX_TXFSHIZ_CLRMASK                equ     (~HW_USBPHYTX_TXFSHIZ_SETMASK)
HW_USBPHYTX_TXCOMPOUT_CLRMASK              equ     (~HW_USBPHYTX_TXCOMPOUT_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int TXCAL1500          :4;
;        int RSVD0              :1;
;        int TXENCAL1500        :1;
;        int TXHSXCVR           :1;
;        int TXCALIBRATE        :1;
;        int TXCAL45DN          :4;
;        int RSVD1              :1;
;        int TXENCAL45DN        :1;
;        int TXHSTERM           :1;
;        int TXSKEW             :1;
;        int TXCAL45DP          :4;
;        int RSVD2              :1;
;        int TXENCAL45DP        :1;
;        int TXFSHIZ            :1;
;        int TXCOMPOUT          :1;
;    } B;
;    int I;
;} usbphytx_type;

;#define HW_USBPHYTX      (*(volatile usbphytx_type _X*) (HW_USBPHY_BASEADDR+1))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB PHY PLL register (HW_USBPHYPLL) Bit Definitions
HW_USBPHYPLL_PLLV2ISEL_BITPOS              equ     (0)
HW_USBPHYPLL_PLLCPDBLIP_BITPOS             equ     (5)
HW_USBPHYPLL_PLLVCOCLK2_BITPOS             equ     (6)
HW_USBPHYPLL_PLLVCOCLK24_BITPOS            equ     (7)
HW_USBPHYPLL_PLLCPNSEL_BITPOS              equ     (8)
HW_USBPHYPLL_PLLCLKDIVSEL_BITPOS           equ     (12)
HW_USBPHYPLL_PLLPFDRST_BITPOS              equ     (20)
HW_USBPHYPLL_PLLCPSHORTLFR_BITPOS          equ     (21)
HW_USBPHYPLL_PLLVCOKSTART_BITPOS           equ     (22)
HW_USBPHYPLL_PLLCLKDIVRSTZ_BITPOS          equ     (23)

HW_USBPHYPLL_PLLV2ISEL_WIDTH               equ     (4)
HW_USBPHYPLL_PLLCPDBLIP_WIDTH              equ     (1)
HW_USBPHYPLL_PLLVCOCLK2_WIDTH              equ     (1)
HW_USBPHYPLL_PLLVCOCLK24_WIDTH             equ     (1)
HW_USBPHYPLL_PLLCPNSEL_WIDTH               equ     (4)
HW_USBPHYPLL_PLLCLKDIVSEL_WIDTH            equ     (4)
HW_USBPHYPLL_PLLPFDRST_WIDTH               equ     (1)
HW_USBPHYPLL_PLLCPSHORTLFR_WIDTH           equ     (1)
HW_USBPHYPLL_PLLVCOKSTART_WIDTH            equ     (1)
HW_USBPHYPLL_PLLCLKDIVRSTZ_WIDTH           equ     (1)

HW_USBPHYPLL_PLLV2ISEL_SETMASK             equ     (((1<<HW_USBPHYPLL_PLLV2ISEL_WIDTH)-1)<<HW_USBPHYPLL_PLLV2ISEL_BITPOS)
HW_USBPHYPLL_PLLCPDBLIP_SETMASK            equ     (((1<<HW_USBPHYPLL_PLLCPDBLIP_WIDTH)-1)<<HW_USBPHYPLL_PLLCPDBLIP_BITPOS)
HW_USBPHYPLL_PLLVCOCLK2_SETMASK            equ     (((1<<HW_USBPHYPLL_PLLVCOCLK2_WIDTH)-1)<<HW_USBPHYPLL_PLLVCOCLK2_BITPOS)
HW_USBPHYPLL_PLLVCOCLK24_SETMASK           equ     (((1<<HW_USBPHYPLL_PLLVCOCLK24_WIDTH)-1)<<HW_USBPHYPLL_PLLVCOCLK24_BITPOS)
HW_USBPHYPLL_PLLCPNSEL_SETMASK             equ     (((1<<HW_USBPHYPLL_PLLCPNSEL_WIDTH)-1)<<HW_USBPHYPLL_PLLCPNSEL_BITPOS)
HW_USBPHYPLL_PLLCLKDIVSEL_SETMASK          equ     (((1<<HW_USBPHYPLL_PLLCLKDIVSEL_WIDTH)-1)<<HW_USBPHYPLL_PLLCLKDIVSEL_BITPOS)
HW_USBPHYPLL_PLLPFDRST_SETMASK             equ     (((1<<HW_USBPHYPLL_PLLPFDRST_WIDTH)-1)<<HW_USBPHYPLL_PLLPFDRST_BITPOS)
HW_USBPHYPLL_PLLCPSHORTLFR_SETMASK         equ     (((1<<HW_USBPHYPLL_PLLCPSHORTLFR_WIDTH)-1)<<HW_USBPHYPLL_PLLCPSHORTLFR_BITPOS)
HW_USBPHYPLL_PLLVCOKSTART_SETMASK          equ     (((1<<HW_USBPHYPLL_PLLVCOKSTART_WIDTH)-1)<<HW_USBPHYPLL_PLLVCOKSTART_BITPOS)
HW_USBPHYPLL_PLLCLKDIVRSTZ_SETMASK         equ     (((1<<HW_USBPHYPLL_PLLCLKDIVRSTZ_WIDTH)-1)<<HW_USBPHYPLL_PLLCLKDIVRSTZ_BITPOS)

HW_USBPHYPLL_PLLV2ISEL_CLRMASK             equ     (~HW_USBPHYPLL_PLLV2ISEL_SETMASK)
HW_USBPHYPLL_PLLCPDBLIP_CLRMASK            equ     (~HW_USBPHYPLL_PLLCPDBLIP_SETMASK)
HW_USBPHYPLL_PLLVCOCLK2_CLRMASK            equ     (~HW_USBPHYPLL_PLLVCOCLK2_SETMASK)
HW_USBPHYPLL_PLLVCOCLK24_CLRMASK           equ     (~HW_USBPHYPLL_PLLVCOCLK24_SETMASK)
HW_USBPHYPLL_PLLCPNSEL_CLRMASK             equ     (~HW_USBPHYPLL_PLLCPNSEL_SETMASK)
HW_USBPHYPLL_PLLCLKDIVSEL_CLRMASK          equ     (~HW_USBPHYPLL_PLLCLKDIVSEL_SETMASK)
HW_USBPHYPLL_PLLPFDRST_CLRMASK             equ     (~HW_USBPHYPLL_PLLPFDRST_SETMASK)
HW_USBPHYPLL_PLLCPSHORTLFR_CLRMASK         equ     (~HW_USBPHYPLL_PLLCPSHORTLFR_SETMASK)
HW_USBPHYPLL_PLLVCOKSTART_CLRMASK          equ     (~HW_USBPHYPLL_PLLVCOKSTART_SETMASK)
HW_USBPHYPLL_PLLCLKDIVRSTZ_CLRMASK         equ     (~HW_USBPHYPLL_PLLCLKDIVRSTZ_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int PLLV2ISEL        :4;
;        int RSVD0            :1;
;        int PLLCPDBLIP       :1;
;        int PLLVCOCLK2       :1;
;        int PLLVCOCLK24      :1;
;        int PLLCPNSEL        :4;
;        int PLLCLKDIVSEL     :4;
;        int RSVD1            :4;
;        int PLLPFDRST        :1;
;        int PLLCPSHORTLFR    :1;
;        int PLLVCOKSTART     :1;
;        int PLLCLKDIVRSTZ    :1;
;    } B;
;    int I;
;} usbphypll_type;

;#define HW_USBPHYPLL      (*(volatile usbphypll_type _X*) (HW_USBPHY_BASEADDR+2))    
;endc

;///////////////////////////////////////////////////////////////////////////////
;  USB PHY PLL register (HW_USBPHYRX) Bit Definitions
HW_USBRX_ENVADJ_BITPOS                      equ     (0)
HW_USBRX_DISCONADJ_BITPOS                   equ     (4)
HW_USBRX_DEBUGMODE_BITPOS                   equ     (8)
HW_USBRX_PLLLKTIMECTL_BITPOS                equ     (12)
HW_USBRX_PLLCKDIVCTL_BITPOS                 equ     (16)
HW_USBRX_HOSTMODETEST_BITPOS                equ     (20)
HW_USBRX_FSCKSOURCESEL_BITPOS               equ     (21)
HW_USBRX_REGRXDBYPASS_BITPOS                equ     (22)
HW_USBRX_PLLLOCKED_BITPOS                   equ     (23)

HW_USBRX_ENVADJ_WIDTH                       equ     (4)
HW_USBRX_DISCONADJ_WIDTH                    equ     (4)
HW_USBRX_DEBUGMODE_WIDTH                    equ     (4)
HW_USBRX_PLLLKTIMECTL_WIDTH                 equ     (4)
HW_USBRX_PLLCKDIVCTL_WIDTH                  equ     (4)
HW_USBRX_HOSTMODETEST_WIDTH                 equ     (1)
HW_USBRX_FSCKSOURCESEL_WIDTH                equ     (1)
HW_USBRX_REGRXDBYPASS_WIDTH                 equ     (1)
HW_USBRX_PLLLOCKED_WIDTH                    equ     (1)

HW_USBRX_ENVADJ_SETMASK                     equ     (((1<<HW_USBRX_ENVADJ_WIDTH)-1)<<HW_USBRX_ENVADJ_BITPOS)
HW_USBRX_DISCONADJ_SETMASK                  equ     (((1<<HW_USBRX_DISCONADJ_WIDTH)-1)<<HW_USBRX_DISCONADJ_BITPOS)
HW_USBRX_DEBUGMODE_SETMASK                  equ     (((1<<HW_USBRX_DEBUGMODE_WIDTH)-1)<<HW_USBRX_DEBUGMODE_BITPOS)
HW_USBRX_PLLLKTIMECTL_SETMASK               equ     (((1<<HW_USBRX_PLLLKTIMECTL_WIDTH)-1)<<HW_USBRX_PLLLKTIMECTL_BITPOS)
HW_USBRX_PLLCKDIVCTL_SETMASK                equ     (((1<<HW_USBRX_PLLCKDIVCTL_WIDTH)-1)<<HW_USBRX_PLLCKDIVCTL_BITPOS)
; 480 MHz PLL is divided by named number here. Setmask divider field nibble of 7 gives actual divider of 8 and so on. (8 gives 9, 9 gives 10)
;              480Mhz/7 =68.57Mhz
HW_USBPHYRX_PLLDIV_BY_7                     equ     $060000

;              480Mhz/8 ~60Mhz
HW_USBPHYRX_PLLDIV_BY_8                     equ     $070000

;              480Mhz/9 =53.3Mhz
HW_USBPHYRX_PLLDIV_BY_9                     equ     $080000

;              480Mhz/10 =48Mhz
HW_USBPHYRX_PLLDIV_BY_10                    equ     $090000


HW_USBRX_HOSTMODETEST_SETMASK               equ     (((1<<HW_USBRX_HOSTMODETEST_WIDTH)-1)<<HW_USBRX_HOSTMODETEST_BITPOS)
HW_USBRX_FSCKSOURCESEL_SETMASK              equ     (((1<<HW_USBRX_FSCKSOURCESEL_WIDTH)-1)<<HW_USBRX_FSCKSOURCESEL_BITPOS)
HW_USBRX_REGRXDBYPASS_SETMASK               equ     (((1<<HW_USBRX_REGRXDBYPASS_WIDTH)-1)<<HW_USBRX_REGRXDBYPASS_BITPOS)
HW_USBRX_PLLLOCKED_SETMASK                  equ     (((1<<HW_USBRX_PLLLOCKED_WIDTH)-1)<<HW_USBRX_PLLLOCKED_BITPOS)

HW_USBRX_ENVADJ_CLRMASK                     equ     (~HW_USBRX_ENVADJ_SETMASK)
HW_USBRX_DISCONADJ_CLRMASK                  equ     (~HW_USBRX_DISCONADJ_SETMASK)
HW_USBRX_DEBUGMODE_CLRMASK                  equ     (~HW_USBRX_DEBUGMODE_SETMASK)
HW_USBRX_PLLLKTIMECTL_CLRMASK               equ     (~HW_USBRX_PLLLKTIMECTL_SETMASK)
HW_USBRX_PLLCKDIVCTL_CLRMASK                equ     (~HW_USBRX_PLLCKDIVCTL_SETMASK)
HW_USBRX_HOSTMODETEST_CLRMASK               equ     (~HW_USBRX_HOSTMODETEST_SETMASK)
HW_USBRX_FSCKSOURCESEL_CLRMASK              equ     (~HW_USBRX_FSCKSOURCESEL_SETMASK)
HW_USBRX_REGRXDBYPASS_CLRMASK               equ     (~HW_USBRX_REGRXDBYPASS_SETMASK)
HW_USBRX_PLLLOCKED_CLRMASK                  equ     (~HW_USBRX_PLLLOCKED_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;     int ENVADJ               :4;
;     int DISCONADJ            :4;
;     int DEBUGMODE            :4;
;     int PLLLKTIMECTL         :4;
;     int PLLCKDIVCTL          :4;
;     int HOSTMODETEST         :1;
;     int FSCKSOURCESEL        :1;
;     int REGRXDBYPASS         :1;
;     int PLLLOCKED            :1;
;    } B;
;    int I;
;} usbphyrx_type;

;#define HW_USBPHYRX      (*(volatile usbphyrx_type _X*) (HW_USBPHY_BASEADDR+3))    
;endc

    endif
