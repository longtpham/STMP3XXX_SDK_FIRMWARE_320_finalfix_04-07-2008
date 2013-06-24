    if !(@def(regscodecinc))
regscodecinc            equ         1

 

;C insert

;#include "types.h"

;endc

 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   SYSTEM STMP Registers 
;	Last Edited 7.17.2003 M. Henson
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_CODEC_BASEADDR             equ     ($FA00)

 

;ASM Only

HW_MIXTBR                     equ     (HW_CODEC_BASEADDR+3)     ; Codec/Mixer test bit register
HW_MIXMASTERVR                equ     (HW_CODEC_BASEADDR+4)     ; Mixer master volume register
HW_MIXMICINVR                 equ     (HW_CODEC_BASEADDR+5)     ; Mixer mic-in volume register
HW_MIXLINE1INVR               equ     (HW_CODEC_BASEADDR+6)     ; Mixer line-in volume register
HW_MIXLINE2INVR               equ     (HW_CODEC_BASEADDR+7)     ; Mixer fm-in volume register
HW_MIXDACINVR                 equ     (HW_CODEC_BASEADDR+8)     ; Mixer DAC volume register
HW_MIXRECSELR                 equ     (HW_CODEC_BASEADDR+9)     ; Mixer ADC select register
HW_MIXADCGAINR                equ     (HW_CODEC_BASEADDR+10)    ; Mixer ADC gain register
HW_MIXPWRDNR                  equ     (HW_CODEC_BASEADDR+11)    ; Mixer power down register
HW_HPCTRL                     equ     (HW_CODEC_BASEADDR+21)
HW_REF_CTRL                   equ     (HW_CODEC_BASEADDR+25)    ; Reference Control Register
HW_PERSIST_CFG                equ     (HW_CODEC_BASEADDR+27)
HW_MIX_TEST                   equ     (HW_CODEC_BASEADDR+28)
;endasm

 

;///////////////////////////////////////////////////////////////////////////////
;   Headphone Control Register (HW_HPCTRL) Bit Definitions
HW_HPCTRL_TESTIALL_BITPOS           equ     0
HW_HPCTRL_TESTI1_BITPOS             equ     2
HW_HPCTRL_POP0_BITPOS               equ     4
HW_HPCTRL_POP1_BITPOS               equ     5
HW_HPCTRL_POP2_BITPOS               equ     6
HW_HPCTRL_RSVD0_BITPOS		    	equ	    7
HW_HPCTRL_HPPWD_BITPOS              equ     8
HW_HPCTRL_HPCLASSAB_BITPOS          equ     9
HW_HPCTRL_CAPLESS_BITPOS            equ     10
HW_HPCTRL_RSRVD1_BITPOS        	    equ     11
HW_HPCTRL_SHORTMODE_LR_BITPOS       equ     12
HW_HPCTRL_SHORTMODE_CM_BITPOS       equ     14
HW_HPCTRL_SHORT_LVLADJ_BITPOS       equ     16
HW_HPCTRL_RSRVD2_BITPOS		    	equ	    19
HW_HPCTRL_CHOP_CLK_BITPOS	    	equ	    20
HW_HPCTRL_SHORT_LR_BITPOS	    	equ	    22
HW_HPCTRL_SHORT_CM_BITPOS	    	equ	    23

HW_HPCTRL_TESTIALL_WIDTH            equ     2
HW_HPCTRL_TESTI1_WIDTH              equ     2
HW_HPCTRL_POP0_WIDTH                equ     1
HW_HPCTRL_POP1_WIDTH                equ     1
HW_HPCTRL_POP2_WIDTH                equ     1
HW_HPCTRL_RSVD0_WIDTH		    	equ	    1
HW_HPCTRL_HPPWD_WIDTH               equ     1
HW_HPCTRL_HPCLASSAB_WIDTH           equ     1
HW_HPCTRL_CAPLESS_WIDTH        	    equ     1
HW_HPCTRL_RSRVD1_WIDTH        	    equ     1
HW_HPCTRL_SHORTMODE_LR_WIDTH        equ     2
HW_HPCTRL_SHORTMODE_CM_WIDTH        equ     2
HW_HPCTRL_SHORT_LVLADJ_WIDTH        equ     3
HW_HPCTRL_RSRVD2_WIDTH		    equ	    1
HW_HPCTRL_CHOP_CLK_WIDTH	    equ	    2
HW_HPCTRL_SHORT_LR_WIDTH	    equ	    1
HW_HPCTRL_SHORT_CM_WIDTH	    equ	    1

HW_HPCTRL_POP_SETMASK              equ     $7<<HW_HPCTRL_POP0_BITPOS
HW_HPCTRL_POP_CLRMASK			   equ     ~HW_HPCTRL_POP_SETMASK

HW_HPCTRL_SHORT_LVLADJ_0_25X_SETMASK	  equ   $3<<HW_HPCTRL_SHORT_LVLADJ_BITPOS
HW_HPCTRL_SHORT_LVLADJ_0_50X_SETMASK	  equ   $2<<HW_HPCTRL_SHORT_LVLADJ_BITPOS
HW_HPCTRL_SHORT_LVLADJ_0_75X_SETMASK	  equ   $1<<HW_HPCTRL_SHORT_LVLADJ_BITPOS
HW_HPCTRL_SHORT_LVLADJ_1_00X_SETMASK	  equ   $0<<HW_HPCTRL_SHORT_LVLADJ_BITPOS
HW_HPCTRL_SHORT_LVLADJ_1_25X_SETMASK	  equ   $4<<HW_HPCTRL_SHORT_LVLADJ_BITPOS
HW_HPCTRL_SHORT_LVLADJ_1_50X_SETMASK	  equ   $5<<HW_HPCTRL_SHORT_LVLADJ_BITPOS
HW_HPCTRL_SHORT_LVLADJ_1_75X_SETMASK	  equ   $6<<HW_HPCTRL_SHORT_LVLADJ_BITPOS
HW_HPCTRL_SHORT_LVLADJ_2_00X_SETMASK	  equ   $7<<HW_HPCTRL_SHORT_LVLADJ_BITPOS

 if @def('CAPLESS_HP')
HP_SHORT_TRIP_POINT   equ	HW_HPCTRL_SHORT_LVLADJ_0_75X_SETMASK
 else
HP_SHORT_TRIP_POINT   equ	HW_HPCTRL_SHORT_LVLADJ_1_00X_SETMASK
 endif

;
;C insert
;// Headphone control register
;#define HW_HPCTRL (*(volatile hpctrl_type _X*)(HW_GLUE_BASEADDR+21))
;/////////////////////////////////////////////////////////////////////////////////
;//   Headphone Conrol Volume Register (HW_HPCTRL) Bit Definitions
;typedef union               
;{
;    struct {
;        unsigned TESTIALL :HW_HPCTRL_TESTIALL_WIDTH;
;        unsigned TESTI1 :HW_HPCTRL_TESTI1_WIDTH;
;        unsigned POP0 :HW_HPCTRL_POP0_WIDTH; 
;        unsigned POP1 :HW_HPCTRL_POP1_WIDTH; 
;        unsigned POP2 :HW_HPCTRL_POP2_WIDTH; 
;        unsigned RSVD0 :HW_HPCTRL_RSVD0_WIDTH;
;        unsigned HPPWD :HW_HPCTRL_HPPWD_WIDTH;
;        unsigned HPCLASSAB :HW_HPCTRL_HPCLASSAB_WIDTH;
;        unsigned CAPLESS :HW_HPCTRL_CAPLESS_WIDTH; 
;        unsigned RSRVD1 :HW_HPCTRL_RSRVD1_WIDTH;
;        unsigned SHORTMODE_LR :HW_HPCTRL_SHORTMODE_LR_WIDTH; 
;        unsigned SHORTMODE_CM :HW_HPCTRL_SHORTMODE_CM_WIDTH;
;        unsigned SHORT_LVLADJ :HW_HPCTRL_SHORT_LVLADJ_WIDTH; 
;        unsigned RSRVD2 :HW_HPCTRL_RSRVD2_WIDTH;
;        unsigned CHOP_CLK :HW_HPCTRL_CHOP_CLK_WIDTH; 
;        unsigned SHORT_LR :HW_HPCTRL_SHORT_LR_WIDTH;
;        unsigned SHORT_CM :HW_HPCTRL_SHORT_CM_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} hpctrl_type;
;// Headphone control short mode 2 bit values for above short bitfield.
;#define HW_AUTO_HEADAMP_SHUTDOWN_HOLD_RESET   0
;#define HW_AUTO_HEADAMP_SHUTDOWN_EN           1	
;#define HW_AUTO_HEADAMP_SHUTDOWN_DIS          2
;#define HW_AUTO_HEADAMP_SHUTDOWN_DIRECT       3
;endc

 
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;/////   MIXER REGISTERS ///////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////


;///////////////////////////////////////////////////////////////////////////////
;   Codec/Mixer Test Register (HW_MIXTBR) Bit Definitions 
HW_MIXTBR_INV_USB_CLK_BITPOS           equ     0
HW_MIXTBR_USB_DFF_BYPASS_BITPOS        equ     1
HW_MIXTBR_HOLD_GND_BITPOS              equ     2
HW_MIXTBR_ACKI_BITPOS                  equ     3
HW_MIXTBR_ASD2X_BITPOS                 equ     4
HW_MIXTBR_PCPCU_BITPOS                 equ     5
HW_MIXTBR_PCPCD_BITPOS                 equ     6
HW_MIXTBR_DCKI_BITPOS                  equ     7
HW_MIXTBR_MIC_BIAS_OUT_SEL_BITPOS      equ     8
HW_MIXTBR_PSRN_BITPOS                  equ     9
HW_MIXTBR_FX2_BITPOS                   equ     10
HW_MIXTBR_VCOS_BITPOS                  equ     11
HW_MIXTBR_XBCO_BITPOS                  equ     12
HW_MIXTBR_XBGC_BITPOS                  equ     13
HW_MIXTBR_ADTHD_BITPOS                 equ     14
HW_MIXTBR_MICBIAS_LSBITPOS             equ     15
HW_MIXTBR_PWDADC_BITPOS                equ     16
HW_MIXTBR_MICBIAS1_BITPOS              equ     17
HW_MIXTBR_EZD_BITPOS                   equ     18
HW_MIXTBR_DZCDA_BITPOS                 equ     19
HW_MIXTBR_DZCFM_BITPOS                 equ     20
HW_MIXTBR_DZCLI_BITPOS                 equ     21
HW_MIXTBR_DZCMI_BITPOS                 equ     22
HW_MIXTBR_DZCMA_BITPOS                 equ     23

HW_MIXTBR_INV_USB_CLK_WIDTH           equ     (1)
HW_MIXTBR_USB_DFF_BYPASS_WIDTH        equ     (1)
HW_MIXTBR_HOLD_GND_WIDTH              equ     (1)
HW_MIXTBR_ACKI_WIDTH                  equ     (1)
HW_MIXTBR_ASD2X_WIDTH                 equ     (1)
HW_MIXTBR_PCPCU_WIDTH                 equ     (1)
HW_MIXTBR_PCPCD_WIDTH                 equ     (1)
HW_MIXTBR_DCKI_WIDTH                  equ     (1)
HW_MIXTBR_MIC_BIAS_OUT_SEL_WIDTH      equ     (1)
HW_MIXTBR_PSRN_WIDTH                  equ     (1)
HW_MIXTBR_FX2_WIDTH                   equ     (1)
HW_MIXTBR_VCOS_WIDTH                  equ     (1)
HW_MIXTBR_XBCO_WIDTH                  equ     (1)
HW_MIXTBR_XBGC_WIDTH                  equ     (1)
HW_MIXTBR_ADTHD_WIDTH                 equ     (1)
HW_MIXTBR_MICBIAS_LSWIDTH             equ     (1)
HW_MIXTBR_PWDADC_WIDTH                equ     (1)
HW_MIXTBR_MICBIAS1_WIDTH              equ     (1)
HW_MIXTBR_EZD_WIDTH                   equ     (1)
HW_MIXTBR_DZCDA_WIDTH                 equ     (1)
HW_MIXTBR_DZCFM_WIDTH                 equ     (1)
HW_MIXTBR_DZCLI_WIDTH                 equ     (1)
HW_MIXTBR_DZCMI_WIDTH                 equ     (1)
HW_MIXTBR_DZCMA_WIDTH                 equ     (1)


HW_MIXTBR_INV_USB_CLK_SETMASK           equ     1<<HW_MIXTBR_INV_USB_CLK_BITPOS
HW_MIXTBR_USB_DFF_BYPASS_SETMASK        equ     1<<HW_MIXTBR_USB_DFF_BYPASS_BITPOS
HW_MIXTBR_HOLD_GND_SETMASK              equ     1<<HW_MIXTBR_HOLD_GND_BITPOS
HW_MIXTBR_ACKI_SETMASK                  equ     1<<HW_MIXTBR_ACKI_BITPOS
HW_MIXTBR_ASD2X_SETMASK                 equ     1<<HW_MIXTBR_ASD2X_BITPOS
HW_MIXTBR_PCPCU_SETMASK                 equ     1<<HW_MIXTBR_PCPCU_BITPOS
HW_MIXTBR_PCPCD_SETMASK                 equ     1<<HW_MIXTBR_PCPCD_BITPOS
HW_MIXTBR_DCKI_SETMASK                  equ     1<<HW_MIXTBR_DCKI_BITPOS
HW_MIXTBR_MIC_BIAS_OUT_SEL_SETMASK      equ     1<<HW_MIXTBR_MIC_BIAS_OUT_SEL_BITPOS
HW_MIXTBR_PSRN_SETMASK                  equ     1<<HW_MIXTBR_PSRN_BITPOS
HW_MIXTBR_FX2_SETMASK                   equ     1<<HW_MIXTBR_FX2_BITPOS
HW_MIXTBR_VCOS_SETMASK                  equ     1<<HW_MIXTBR_VCOS_BITPOS
HW_MIXTBR_XBCO_SETMASK                  equ     1<<HW_MIXTBR_XBCO_BITPOS
HW_MIXTBR_XBGC_SETMASK                  equ     1<<HW_MIXTBR_XBGC_BITPOS
HW_MIXTBR_ADTHD_SETMASK                 equ     1<<HW_MIXTBR_ADTHD_BITPOS
HW_MIXTBR_MICBIAS_LSSETMASK             equ     1<<HW_MIXTBR_MICBIAS_LSBITPOS
HW_MIXTBR_PWDADC_SETMASK                equ     1<<HW_MIXTBR_PWDADC_BITPOS
HW_MIXTBR_MICBIAS1_SETMASK              equ     1<<HW_MIXTBR_MICBIAS1_BITPOS
HW_MIXTBR_EZD_SETMASK                   equ     1<<HW_MIXTBR_EZD_BITPOS
HW_MIXTBR_DZCDA_SETMASK                 equ     1<<HW_MIXTBR_DZCDA_BITPOS
HW_MIXTBR_DZCFM_SETMASK                 equ     1<<HW_MIXTBR_DZCFM_BITPOS
HW_MIXTBR_DZCLI_SETMASK                 equ     1<<HW_MIXTBR_DZCLI_BITPOS
HW_MIXTBR_DZCMI_SETMASK                 equ     1<<HW_MIXTBR_DZCMI_BITPOS
HW_MIXTBR_DZCMA_SETMASK                 equ     1<<HW_MIXTBR_DZCMA_BITPOS

HW_MIXTBR_INV_USB_CLK_CLRMASK           equ     ~HW_MIXTBR_INV_USB_CLK_SETMASK
HW_MIXTBR_USB_DFF_BYPASS_CLRMASK        equ     ~HW_MIXTBR_USB_DFF_BYPASS_SETMASK
HW_MIXTBR_HOLD_GND_CLRMASK              equ     ~HW_MIXTBR_HOLD_GND_SETMASK
HW_MIXTBR_ACKI_CLRMASK                  equ     ~HW_MIXTBR_ACKI_SETMASK
HW_MIXTBR_ASD2X_CLRMASK                 equ     ~HW_MIXTBR_ASD2X_SETMASK
HW_MIXTBR_PCPCU_CLRMASK                 equ     ~HW_MIXTBR_PCPCU_SETMASK
HW_MIXTBR_PCPCD_CLRMASK                 equ     ~HW_MIXTBR_PCPCD_SETMASK
HW_MIXTBR_DCKI_CLRMASK                  equ     ~HW_MIXTBR_DCKI_SETMASK
HW_MIXTBR_MIC_BIAS_OUT_SEL_CLRMASK      equ     ~HW_MIXTBR_MIC_BIAS_OUT_SEL_SETMASK
HW_MIXTBR_PSRN_CLRMASK                  equ     ~HW_MIXTBR_PSRN_SETMASK
HW_MIXTBR_FX2_CLRMASK                   equ     ~HW_MIXTBR_FX2_SETMASK
HW_MIXTBR_VCOS_CLRMASK                  equ     ~HW_MIXTBR_VCOS_SETMASK
HW_MIXTBR_XBCO_CLRMASK                  equ     ~HW_MIXTBR_XBCO_SETMASK
HW_MIXTBR_XBGC_CLRMASK                  equ     ~HW_MIXTBR_XBGC_SETMASK
HW_MIXTBR_ADTHD_CLRMASK                 equ     ~HW_MIXTBR_ADTHD_SETMASK
HW_MIXTBR_MICBIAS_LSCLRMASK             equ     ~HW_MIXTBR_MICBIAS_LSSETMASK
HW_MIXTBR_PWDADC_CLRMASK                equ     ~HW_MIXTBR_PWDADC_SETMASK
HW_MIXTBR_MICBIAS1_CLRMASK              equ     ~HW_MIXTBR_MICBIAS1_SETMASK
HW_MIXTBR_EZD_CLRMASK                   equ     ~HW_MIXTBR_EZD_SETMASK
HW_MIXTBR_DZCDA_CLRMASK                 equ     ~HW_MIXTBR_DZCDA_SETMASK
HW_MIXTBR_DZCFM_CLRMASK                 equ     ~HW_MIXTBR_DZCFM_SETMASK
HW_MIXTBR_DZCLI_CLRMASK                 equ     ~HW_MIXTBR_DZCLI_SETMASK
HW_MIXTBR_DZCMI_CLRMASK                 equ     ~HW_MIXTBR_DZCMI_SETMASK
HW_MIXTBR_DZCMA_CLRMASK                 equ     ~HW_MIXTBR_DZCMA_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int INV_USB_CLK            : 1;
;        int USB_DFF_BYPASS         : 1;
;        int HOLD_GND               : 1;
;        int ACKI                   : 1;
;        int ASD2X                  : 1;
;        int PCPCU                  : 1;
;        int PCPCD                  : 1;
;        int DCKI                   : 1;
;        int MIC_BIAS_OUT_SEL       : 1;
;        int PSRN                   : 1;
;        int FX2                    : 1;
;        int VCOS                   : 1;
;        int XBCO                   : 1;
;        int XBGC                   : 1;
;        int ADTHD                  : 1;
;        int MICBIAS_LSBITPOS       : 1;
;        int PWDADC                 : 1;
;        int MICBIAS1               : 1;
;        int EZD                    : 1;
;        int DZCDA                  : 1;
;        int DZCFM                  : 1;
;        int DZCLI                  : 1;
;        int DZCMI                  : 1;
;        int DZCMA                  : 1;
;    } B;
;    int I;
;    unsigned int U;
;} mix_tbr_type;

;#define HW_MIXTBR      (*(volatile mix_tbr_type _X*) (HW_CODEC_BASEADDR+3))

;endc


;///////////////////////////////////////////////////////////////////////////////
;   Generic Volume Register (HW_MIXMASTERVR) Bit Definitions
HW_MIXVOLUMER_MR_BITPOS                 equ     0
HW_MIXVOLUMER_ML_BITPOS                 equ     8
HW_MIXVOLUMER_MUTE_BITPOS               equ     15

HW_MIXVOLUMER_MR_WIDTH                  equ     (5)
HW_MIXVOLUMER_ML_WIDTH                  equ     (5)
HW_MIXVOLUMER_MUTE_WIDTH                equ     1

HW_MIXVOLUMER_MR_SETMASK                equ     $1F<<HW_MIXVOLUMER_MR_BITPOS
HW_MIXVOLUMER_ML_SETMASK                equ     $1F<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_MUTE_SETMASK              equ     1<<HW_MIXVOLUMER_MUTE_BITPOS

HW_MIXVOLUMER_MR_CLRMASK                equ     ~HW_MIXVOLUMER_MR_SETMASK
HW_MIXVOLUMER_ML_CLRMASK                equ     ~HW_MIXVOLUMER_ML_SETMASK
HW_MIXVOLUMER_MUTE_CLRMASK              equ     ~HW_MIXVOLUMER_MUTE_SETMASK

HW_MIXVOLUMER_ML_PLUS_12P0_SETMASK      equ     0<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_PLUS_10P5_SETMASK      equ     1<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_PLUS_09P0_SETMASK      equ     2<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_PLUS_07P5_SETMASK      equ     3<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_PLUS_06P0_SETMASK      equ     4<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_PLUS_04P5_SETMASK      equ     5<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_PLUS_03P0_SETMASK      equ     6<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_PLUS_01P5_SETMASK      equ     7<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_ZERO_SETMASK           equ     8<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_01P5_SETMASK     equ     9<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_03P0_SETMASK     equ     10<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_04P5_SETMASK     equ     11<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_06P0_SETMASK     equ     12<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_07P5_SETMASK     equ     13<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_09P0_SETMASK     equ     14<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_10P5_SETMASK     equ     15<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_12P0_SETMASK     equ     16<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_13P5_SETMASK     equ     17<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_15P0_SETMASK     equ     18<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_16P5_SETMASK     equ     19<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_18P0_SETMASK     equ     20<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_19P5_SETMASK     equ     21<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_21P0_SETMASK     equ     22<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_22P5_SETMASK     equ     23<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_24P0_SETMASK     equ     24<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_25P5_SETMASK     equ     25<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_27P0_SETMASK     equ     26<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_28P5_SETMASK     equ     27<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_30P0_SETMASK     equ     28<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_31P5_SETMASK     equ     29<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_33P0_SETMASK     equ     30<<HW_MIXVOLUMER_ML_BITPOS
HW_MIXVOLUMER_ML_MINUS_34P5_SETMASK     equ     31<<HW_MIXVOLUMER_ML_BITPOS

HW_MIXVOLUMER_MR_PLUS_12P0_SETMASK      equ     0
HW_MIXVOLUMER_MR_PLUS_10P5_SETMASK      equ     1
HW_MIXVOLUMER_MR_PLUS_09P0_SETMASK      equ     2
HW_MIXVOLUMER_MR_PLUS_07P5_SETMASK      equ     3
HW_MIXVOLUMER_MR_PLUS_06P0_SETMASK      equ     4
HW_MIXVOLUMER_MR_PLUS_04P5_SETMASK      equ     5
HW_MIXVOLUMER_MR_PLUS_03P0_SETMASK      equ     6
HW_MIXVOLUMER_MR_PLUS_01P5_SETMASK      equ     7
HW_MIXVOLUMER_MR_ZERO_SETMASK           equ     8
HW_MIXVOLUMER_MR_MINUS_01P5_SETMASK     equ     9
HW_MIXVOLUMER_MR_MINUS_03P0_SETMASK     equ     10
HW_MIXVOLUMER_MR_MINUS_04P5_SETMASK     equ     11
HW_MIXVOLUMER_MR_MINUS_06P0_SETMASK     equ     12
HW_MIXVOLUMER_MR_MINUS_07P5_SETMASK     equ     13
HW_MIXVOLUMER_MR_MINUS_09P0_SETMASK     equ     14
HW_MIXVOLUMER_MR_MINUS_10P5_SETMASK     equ     15
HW_MIXVOLUMER_MR_MINUS_12P0_SETMASK     equ     16
HW_MIXVOLUMER_MR_MINUS_13P5_SETMASK     equ     17
HW_MIXVOLUMER_MR_MINUS_15P0_SETMASK     equ     18
HW_MIXVOLUMER_MR_MINUS_16P5_SETMASK     equ     19
HW_MIXVOLUMER_MR_MINUS_18P0_SETMASK     equ     20
HW_MIXVOLUMER_MR_MINUS_19P5_SETMASK     equ     21
HW_MIXVOLUMER_MR_MINUS_21P0_SETMASK     equ     22
HW_MIXVOLUMER_MR_MINUS_22P5_SETMASK     equ     23
HW_MIXVOLUMER_MR_MINUS_24P0_SETMASK     equ     24
HW_MIXVOLUMER_MR_MINUS_25P5_SETMASK     equ     25
HW_MIXVOLUMER_MR_MINUS_27P0_SETMASK     equ     26
HW_MIXVOLUMER_MR_MINUS_28P5_SETMASK     equ     27
HW_MIXVOLUMER_MR_MINUS_30P0_SETMASK     equ     28
HW_MIXVOLUMER_MR_MINUS_31P5_SETMASK     equ     29
HW_MIXVOLUMER_MR_MINUS_33P0_SETMASK     equ     30
HW_MIXVOLUMER_MR_MINUS_34P5_SETMASK     equ     31

;///////////////////////////////////////////////////////////////////////////////
;   Mixer Master Volume Register (HW_MIXMASTERVR) Bit Definitions
HW_MIXMASTERVR_MR_BITPOS                equ      0
HW_MIXMASTERVR_ML_BITPOS                equ      8
HW_MIXMASTERVR_MUTE_BITPOS              equ      15

HW_MIXMASTERVR_MR_WIDTH                 equ      (5)
HW_MIXMASTERVR_ML_WIDTH                 equ      (5)
HW_MIXMASTERVR_MUTE_WIDTH               equ      (1)

HW_MIXMASTERVR_MR_SETMASK               equ      $1F<<HW_MIXMASTERVR_MR_BITPOS
HW_MIXMASTERVR_ML_SETMASK               equ      $1F<<HW_MIXMASTERVR_ML_BITPOS
HW_MIXMASTERVR_MUTE_SETMASK             equ      1<<HW_MIXMASTERVR_MUTE_BITPOS

HW_MIXMASTERVR_MR_CLRMASK               equ      ~HW_MIXMASTERVR_MR_SETMASK
HW_MIXMASTERVR_ML_CLRMASK               equ      ~HW_MIXMASTERVR_ML_SETMASK
HW_MIXMASTERVR_MUTE_CLRMASK             equ      ~HW_MIXMASTERVR_MUTE_SETMASK


;C insert
;typedef union               
;{
;    struct
;    {
;        unsigned MR :5;
;        int         :3;
;        unsigned ML :5;
;        int         :2;
;        int MUTE    :1;
;    } B;
;    int I;
;    unsigned int U;
;} mix_mastervr_type;

;#define HW_MIXMASTERVR (*(volatile mix_mastervr_type _X*) (HW_CODEC_BASEADDR+4))

;endc


;///////////////////////////////////////////////////////////////////////////////
;   Mixer Mic In Volume Register (HW_MIXMICINVR) Bit Definitions
HW_MIXMICINVR_GN_BITPOS                 equ       0
HW_MIXMICINVR_P20DB_BITPOS              equ       6
HW_MIXMICINVR_MUTE_BITPOS               equ       15

HW_MIXMICINVR_GN_WIDTH                  equ       5
HW_MIXMICINVR_RSVD1_WIDTH               equ       1
HW_MIXMICINVR_P20DB_WIDTH               equ       1
HW_MIXMICINVR_RSVD2_WIDTH               equ       8
HW_MIXMICINVR_MUTE_WIDTH                equ       1
HW_MIXMICINVR_RSVD3_WIDTH               equ       8

HW_MIXMICINVR_GN_SETMASK                equ       $1F<<HW_MIXMICINVR_GN_BITPOS
HW_MIXMICINVR_P20DB_SETMASK             equ       1<<HW_MIXMICINVR_P20DB_BITPOS
HW_MIXMICINVR_MUTE_SETMASK              equ       1<<HW_MIXMICINVR_MUTE_BITPOS

HW_MIXMICINVR_GN_CLRMASK                equ       ~HW_MIXMICINVR_GN_SETMASK
HW_MIXMICINVR_P20DB_CLRMASK             equ       ~HW_MIXMICINVR_P20DB_SETMASK
HW_MIXMICINVR_MUTE_CLRMASK              equ       ~HW_MIXMICINVR_MUTE_SETMASK

HW_MIXMICINVR_GN_PLUS_12P0_SETMASK      equ     0
HW_MIXMICINVR_GN_PLUS_10P5_SETMASK      equ     1
HW_MIXMICINVR_GN_PLUS_09P0_SETMASK      equ     2
HW_MIXMICINVR_GN_PLUS_07P5_SETMASK      equ     3
HW_MIXMICINVR_GN_PLUS_06P0_SETMASK      equ     4
HW_MIXMICINVR_GN_PLUS_04P5_SETMASK      equ     5
HW_MIXMICINVR_GN_PLUS_03P0_SETMASK      equ     6
HW_MIXMICINVR_GN_PLUS_01P5_SETMASK      equ     7
HW_MIXMICINVR_GN_ZERO_SETMASK           equ     8
HW_MIXMICINVR_GN_MINUS_01P5_SETMASK     equ     9
HW_MIXMICINVR_GN_MINUS_03P0_SETMASK     equ     10
HW_MIXMICINVR_GN_MINUS_04P5_SETMASK     equ     11
HW_MIXMICINVR_GN_MINUS_06P0_SETMASK     equ     12
HW_MIXMICINVR_GN_MINUS_07P5_SETMASK     equ     13
HW_MIXMICINVR_GN_MINUS_09P0_SETMASK     equ     14
HW_MIXMICINVR_GN_MINUS_10P5_SETMASK     equ     15
HW_MIXMICINVR_GN_MINUS_12P0_SETMASK     equ     16
HW_MIXMICINVR_GN_MINUS_13P5_SETMASK     equ     17
HW_MIXMICINVR_GN_MINUS_15P0_SETMASK     equ     18
HW_MIXMICINVR_GN_MINUS_16P5_SETMASK     equ     19
HW_MIXMICINVR_GN_MINUS_18P0_SETMASK     equ     20
HW_MIXMICINVR_GN_MINUS_19P5_SETMASK     equ     21
HW_MIXMICINVR_GN_MINUS_21P0_SETMASK     equ     22
HW_MIXMICINVR_GN_MINUS_22P5_SETMASK     equ     23
HW_MIXMICINVR_GN_MINUS_24P0_SETMASK     equ     24
HW_MIXMICINVR_GN_MINUS_25P5_SETMASK     equ     25
HW_MIXMICINVR_GN_MINUS_27P0_SETMASK     equ     26
HW_MIXMICINVR_GN_MINUS_28P5_SETMASK     equ     27
HW_MIXMICINVR_GN_MINUS_30P0_SETMASK     equ     28
HW_MIXMICINVR_GN_MINUS_31P5_SETMASK     equ     29
HW_MIXMICINVR_GN_MINUS_33P0_SETMASK     equ     30
HW_MIXMICINVR_GN_MINUS_34P5_SETMASK     equ     31

;C insert
;typedef union               
;{
;    struct {
;        int GN          : HW_MIXMICINVR_GN_WIDTH;
;        int RSVD1       : HW_MIXMICINVR_RSVD1_WIDTH;
;        int P20DB       : HW_MIXMICINVR_P20DB_WIDTH;
;        int RSVD2       : HW_MIXMICINVR_RSVD2_WIDTH;
;        int MUTE        : HW_MIXMICINVR_MUTE_WIDTH;
;        int RSVD3       : HW_MIXMICINVR_RSVD3_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} mix_micinvr_type;

;#define HW_MIXMICINVR (*(volatile mix_micinvr_type _X*) (HW_CODEC_BASEADDR+5))

;endc




;///////////////////////////////////////////////////////////////////////////////
;   Mixer Line1 In Volume Register (HW_MIXLINE1INVR) Bit Definitions
HW_MIXLINE1INVR_GR_BITPOS               equ     0
HW_MIXLINE1INVR_GL_BITPOS               equ     8
HW_MIXLINE1INVR_MUTE_BITPOS             equ     15

HW_MIXLINE1INVR_GR_WIDTH                equ     5
HW_MIXLINE1INVR_RSVD1_WIDTH             equ     3
HW_MIXLINE1INVR_GL_WIDTH                equ     5
HW_MIXLINE1INVR_RSVD2_WIDTH             equ     2
HW_MIXLINE1INVR_MUTE_WIDTH              equ     1
HW_MIXLINE1INVR_RSVD3_WIDTH             equ     8

HW_MIXLINE1INVR_GR_SETMASK              equ     $1F<<HW_MIXLINE1INVR_GR_BITPOS
HW_MIXLINE1INVR_GL_SETMASK              equ     $1F<<HW_MIXLINE1INVR_GL_BITPOS
HW_MIXLINE1INVR_MUTE_SETMASK            equ     1<<HW_MIXLINE1INVR_MUTE_BITPOS

HW_MIXLINE1INVR_GR_CLRMASK              equ     ~HW_MIXLINE1INVR_GR_SETMASK
HW_MIXLINE1INVR_GL_CLRMASK              equ     ~HW_MIXLINE1INVR_GL_SETMASK
HW_MIXLINE1INVR_MUTE_CLRMASK            equ     ~HW_MIXLINE1INVR_MUTE_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int GR          : HW_MIXLINE1INVR_GR_WIDTH;
;        int RSVD1       : HW_MIXLINE1INVR_RSVD1_WIDTH;
;        int GL          : HW_MIXLINE1INVR_GL_WIDTH;
;        int RSVD2       : HW_MIXLINE1INVR_RSVD2_WIDTH;
;        int MUTE        : HW_MIXLINE1INVR_MUTE_WIDTH;
;        int RSVD3       : HW_MIXLINE1INVR_RSVD3_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} mix_line1invr_type;

;#define HW_MIXLINE1INVR (*(volatile mix_line1invr_type _X*) (HW_CODEC_BASEADDR+6))

;endc



;///////////////////////////////////////////////////////////////////////////////
;   Mixer Line2 In Volume Register (HW_MIXLINE2INVR) Bit Definitions
HW_MIXLINE2INVR_GR_BITPOS               equ     0
HW_MIXLINE2INVR_GL_BITPOS               equ     8
HW_MIXLINE2INVR_MUTE_BITPOS             equ     15

HW_MIXLINE2INVR_GR_WIDTH                equ     5
HW_MIXLINE2INVR_RSVD1_WIDTH             equ     3
HW_MIXLINE2INVR_GL_WIDTH                equ     5
HW_MIXLINE2INVR_RSVD2_WIDTH             equ     2
HW_MIXLINE2INVR_MUTE_WIDTH              equ     1
HW_MIXLINE2INVR_RSVD3_WIDTH             equ     8


HW_MIXLINE2INVR_GR_SETMASK              equ     $1F<<HW_MIXLINE2INVR_GR_BITPOS
HW_MIXLINE2INVR_GL_SETMASK              equ     $1F<<HW_MIXLINE2INVR_GL_BITPOS
HW_MIXLINE2INVR_MUTE_SETMASK            equ     1<<HW_MIXLINE2INVR_MUTE_BITPOS

HW_MIXLINE2INVR_GR_CLRMASK              equ     ~HW_MIXLINE2INVR_GR_SETMASK
HW_MIXLINE2INVR_GL_CLRMASK              equ     ~HW_MIXLINE2INVR_GL_SETMASK
HW_MIXLINE2INVR_MUTE_CLRMASK            equ     ~HW_MIXLINE2INVR_MUTE_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int GR          : HW_MIXLINE2INVR_GR_WIDTH;
;        int RSVD1       : HW_MIXLINE2INVR_RSVD1_WIDTH;
;        int GL          : HW_MIXLINE2INVR_GL_WIDTH;
;        int RSVD2       : HW_MIXLINE2INVR_RSVD2_WIDTH;
;        int MUTE        : HW_MIXLINE2INVR_MUTE_WIDTH;
;        int RSVD3       : HW_MIXLINE2INVR_RSVD3_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} mix_line2invr_type;

;#define HW_MIXLINE2INVR (*(volatile mix_line2invr_type _X*) (HW_CODEC_BASEADDR+7))

;endc



;///////////////////////////////////////////////////////////////////////////////
;   Mixer DAC In Volume Register (HW_MIXDACINVR) Bit Definitions
HW_MIXDACINVR_MR_BITPOS                 equ     0
HW_MIXDACINVR_ML_BITPOS                 equ     8
HW_MIXDACINVR_MUTE_BITPOS               equ     15

HW_MIXDACINVR_MR_WIDTH                  equ     (5)
HW_MIXDACINVR_ML_WIDTH                  equ     (5)
HW_MIXDACINVR_MUTE_WIDTH                equ     (1)

HW_MIXDACINVR_MR_SETMASK                equ     $1F<<HW_MIXDACINVR_MR_BITPOS
HW_MIXDACINVR_ML_SETMASK                equ     $1F<<HW_MIXDACINVR_ML_BITPOS
HW_MIXDACINVR_MUTE_SETMASK              equ     1<<HW_MIXDACINVR_MUTE_BITPOS

HW_MIXDACINVR_MR_CLRMASK                equ     ~HW_MIXDACINVR_MR_SETMASK
HW_MIXDACINVR_ML_CLRMASK                equ     ~HW_MIXDACINVR_ML_SETMASK
HW_MIXDACINVR_MUTE_CLRMASK              equ     ~HW_MIXDACINVR_MUTE_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int MR   : HW_MIXDACINVR_MR_WIDTH;
;        int ML   : HW_MIXDACINVR_ML_WIDTH;
;        int MUTE : HW_MIXDACINVR_MUTE_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} mix_dacinvr_type;

;#define HW_MIXDACINVR (*(volatile mix_dacinvr_type _X*) (HW_CODEC_BASEADDR+8))

;endc


;///////////////////////////////////////////////////////////////////////////////
;   Mixer Record Select Register (HW_MIXRECSELR) Bit Definitions
HW_MIXRECSELR_SR_BITPOS                 equ     0
HW_MIXRECSELR_SL_BITPOS                 equ     8
HW_MIXRECSELR_X_BITPOS                  equ     11

HW_MIXRECSELR_SR_WIDTH                  equ     3
HW_MIXRECSELR_RSVD1_WIDTH               equ     5
HW_MIXRECSELR_SL_WIDTH                  equ     3
HW_MIXRECSELR_X_WIDTH                   equ     1
HW_MIXRECSELR_RSVD2_WIDTH               equ     12

HW_MIXRECSELR_SR_SETMASK                equ     7<<HW_MIXRECSELR_SR_BITPOS
HW_MIXRECSELR_SL_SETMASK                equ     7<<HW_MIXRECSELR_SL_BITPOS
HW_MIXRECSELR_X_SETMASK                 equ     1<<HW_MIXRECSELR_X_BITPOS

HW_MIXRECSELR_SR_MIC_SETMASK            equ     0<<HW_MIXRECSELR_SR_BITPOS
HW_MIXRECSELR_SR_LINEIN2_SETMASK        equ     3<<HW_MIXRECSELR_SR_BITPOS
HW_MIXRECSELR_SR_LINEIN1_SETMASK        equ     4<<HW_MIXRECSELR_SR_BITPOS
HW_MIXRECSELR_SR_MIX_SETMASK            equ     5<<HW_MIXRECSELR_SR_BITPOS

HW_MIXRECSELR_SL_MIC_SETMASK            equ     0<<HW_MIXRECSELR_SL_BITPOS
HW_MIXRECSELR_SL_LINEIN2_SETMASK        equ     3<<HW_MIXRECSELR_SL_BITPOS
HW_MIXRECSELR_SL_LINEIN1_SETMASK        equ     4<<HW_MIXRECSELR_SL_BITPOS
HW_MIXRECSELR_SL_MIX_SETMASK            equ     5<<HW_MIXRECSELR_SL_BITPOS

HW_MIXRECSELR_SR_CLRMASK                equ     ~HW_MIXRECSELR_SR_SETMASK
HW_MIXRECSELR_SL_CLRMASK                equ     ~HW_MIXRECSELR_SL_SETMASK
HW_MIXRECSELR_SR_SL_CLRMASK             equ     ~(HW_MIXRECSELR_SR_SETMASK|HW_MIXRECSELR_SL_SETMASK)
HW_MIXRECSELR_X_CLRMASK                 equ     ~HW_MIXRECSELR_X_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int SR          : HW_MIXRECSELR_SR_WIDTH;
;        int RSVD1       : HW_MIXRECSELR_RSVD1_WIDTH;
;        int SL          : HW_MIXRECSELR_SL_WIDTH;
;        int X           : HW_MIXRECSELR_X_WIDTH;
;        int RSVD2       : HW_MIXRECSELR_RSVD2_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} mix_recselr_type;

;#define HW_MIXRECSELR (*(volatile mix_recselr_type _X*) (HW_CODEC_BASEADDR+9))

;endc



;///////////////////////////////////////////////////////////////////////////////
;   Mixer ADC In Gain Register (HW_MIXADCGAINR) Bit Definitions
HW_MIXADCGAINR_GR_BITPOS                equ     0
HW_MIXADCGAINR_GL_BITPOS                equ     8
HW_MIXADCGAINR_MUTE_BITPOS              equ     15

HW_MIXADCGAINR_GR_WIDTH                 equ     4
HW_MIXADCGAINR_RSVD1_WIDTH              equ     4
HW_MIXADCGAINR_GL_WIDTH                 equ     4
HW_MIXADCGAINR_RSVD2_WIDTH              equ     3
HW_MIXADCGAINR_MUTE_WIDTH               equ     1
HW_MIXADCGAINR_RSVD3_WIDTH              equ     8

HW_MIXADCGAINR_GR_SETMASK               equ     $1F<<HW_MIXADCGAINR_GR_BITPOS
HW_MIXADCGAINR_GL_SETMASK               equ     $1F<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_MUTE_SETMASK             equ     1<<HW_MIXADCGAINR_MUTE_BITPOS

HW_MIXADCGAINR_GR_CLRMASK               equ     ~HW_MIXADCGAINR_GR_SETMASK
HW_MIXADCGAINR_GL_CLRMASK               equ     ~HW_MIXADCGAINR_GL_SETMASK
HW_MIXADCGAINR_MUTE_CLRMASK             equ     ~HW_MIXADCGAINR_MUTE_SETMASK

HW_MIXADCGAINR_GL_00P0_SETMASK          equ     0<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_01P5_SETMASK          equ     1<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_03P0_SETMASK          equ     2<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_04P5_SETMASK          equ     3<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_06P0_SETMASK          equ     4<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_07P5_SETMASK          equ     5<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_09P0_SETMASK          equ     6<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_10P5_SETMASK          equ     7<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_12P0_SETMASK          equ     8<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_13P5_SETMASK          equ     9<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_15P0_SETMASK          equ     10<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_16P5_SETMASK          equ     11<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_18P0_SETMASK          equ     12<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_19P5_SETMASK          equ     13<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_21P0_SETMASK          equ     14<<HW_MIXADCGAINR_GL_BITPOS
HW_MIXADCGAINR_GL_22P5_SETMASK          equ     15<<HW_MIXADCGAINR_GL_BITPOS

HW_MIXADCGAINR_GR_00P0_SETMASK          equ     0
HW_MIXADCGAINR_GR_01P5_SETMASK          equ     1
HW_MIXADCGAINR_GR_03P0_SETMASK          equ     2
HW_MIXADCGAINR_GR_04P5_SETMASK          equ     3
HW_MIXADCGAINR_GR_06P0_SETMASK          equ     4
HW_MIXADCGAINR_GR_07P5_SETMASK          equ     5
HW_MIXADCGAINR_GR_09P0_SETMASK          equ     6
HW_MIXADCGAINR_GR_10P5_SETMASK          equ     7
HW_MIXADCGAINR_GR_12P0_SETMASK          equ     8
HW_MIXADCGAINR_GR_13P5_SETMASK          equ     9
HW_MIXADCGAINR_GR_15P0_SETMASK          equ     10
HW_MIXADCGAINR_GR_16P5_SETMASK          equ     11
HW_MIXADCGAINR_GR_18P0_SETMASK          equ     12
HW_MIXADCGAINR_GR_19P5_SETMASK          equ     13
HW_MIXADCGAINR_GR_21P0_SETMASK          equ     14
HW_MIXADCGAINR_GR_22P5_SETMASK          equ     15

;C insert
;typedef union               
;{
;    struct {
;        int GR          : HW_MIXADCGAINR_GR_WIDTH;
;        int RSVD1       : HW_MIXADCGAINR_RSVD1_WIDTH;
;        int GL          : HW_MIXADCGAINR_GL_WIDTH;
;        int RSVD2       : HW_MIXADCGAINR_RSVD2_WIDTH;
;        int MUTE        : HW_MIXADCGAINR_MUTE_WIDTH;
;        int RSVD3       : HW_MIXADCGAINR_RSVD3_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} mix_adcgainr_type;

;#define HW_MIXADCGAINR (*(volatile mix_adcgainr_type _X*) (HW_CODEC_BASEADDR+10))

;endc



;///////////////////////////////////////////////////////////////////////////////
;   Mixer Power Down Register (HW_MIXPWRDNR) Bit Definitions
HW_MIXPWRDNR_PR0_BITPOS                  equ     9
HW_MIXPWRDNR_PR1_BITPOS                 equ     10
HW_MIXPWRDNR_PR2_BITPOS                 equ     11

HW_MIXPWRDNR_PR0_WIDTH                   equ     (1)
HW_MIXPWRDNR_PR1_WIDTH                  equ     (1)
HW_MIXPWRDNR_PR2_WIDTH                  equ     (1)
HW_MIXPWRDNR_RSVD_WIDTH                 equ     (12)

HW_MIXPWRDNR_PR_SETMASK                 equ     7<<HW_MIXPWRDNR_PR0_BITPOS

HW_MIXPWRDNR_PR_CLRMASK                 equ     ~HW_MIXPWRDNR_PR_SETMASK

;C insert
;typedef union               
;{
;    struct {
;		int			: 9;
;       int PR0   	: HW_MIXPWRDNR_PR0_WIDTH;
;       int PR1  	: HW_MIXPWRDNR_PR1_WIDTH;
;       int PR2  	: HW_MIXPWRDNR_PR2_WIDTH;
;       int RSVD 	: HW_MIXPWRDNR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} mix_pwrdnr_type;

;#define HW_MIXPWRDNR (*(volatile mix_pwrdnr_type _X*) (HW_CODEC_BASEADDR+11))

;endc


;///////////////////////////////////////////////////////////////////////////////

;  Mixer Test Register (HW_MIX_TEST) Bit Definitions

HW_MIX_TEST_DAC_CHOP_CLK_WIDTH           equ     (2)

HW_MIX_TEST_RSVD0_WIDTH                  equ     (2)

HW_MIX_TEST_DAC_DISABLE_RTZ_WIDTH        equ     (1)

HW_MIX_TEST_DAC_MORE_AMP_I_WIDTH         equ     (1)

HW_MIX_TEST_RSVD1_WIDTH                  equ     (2)

HW_MIX_TEST_TMP_CFG_WIDTH                equ     (4)

HW_MIX_TEST_TMPPWD_WIDTH                 equ     (1)

HW_MIX_TEST_RSVD2_WIDTH                  equ     (11)



HW_MIX_TEST_DAC_CHOP_CLK_BITPOS          equ     (0)

HW_MIX_TEST_DAC_DISABLE_RTZ_BITPOS       equ     (4)

HW_MIX_TEST_DAC_MORE_AMP_I_BITPOS        equ     (5)

HW_MIX_TEST_TMP_CFG_BITPOS               equ     (8)

HW_MIX_TEST_TMPPWD_BITPOS                equ     (12)



HW_MIX_TEST_DAC_CHOP_CLK_SETMASK         equ     (((1<<HW_MIX_TEST_DAC_CHOP_CLK_WIDTH)-1)<<HW_MIX_TEST_DAC_CHOP_CLK_BITPOS)        

HW_MIX_TEST_DAC_DISABLE_RTZ_SETMASK      equ     (((1<<HW_MIX_TEST_DAC_DISABLE_RTZ_WIDTH)-1)<<HW_MIX_TEST_DAC_DISABLE_RTZ_BITPOS)        

HW_MIX_TEST_DAC_MORE_AMP_I_SETMASK       equ     (((1<<HW_MIX_TEST_DAC_MORE_AMP_I_WIDTH)-1)<<HW_MIX_TEST_DAC_MORE_AMP_I_BITPOS) 

HW_MIX_TEST_TMP_CFG_SETMASK              equ     (((1<<HW_MIX_TEST_TMP_CFG_WIDTH)-1)<<HW_MIX_TEST_TMP_CFG_BITPOS) 

HW_MIX_TEST_TMPPWD_SETMASK               equ     (((1<<HW_MIX_TEST_TMPPWD_WIDTH)-1)<<HW_MIX_TEST_TMPPWD_BITPOS) 


HW_MIX_TEST_DAC_CHOP_CLK_CLRMASK         equ    (~HW_MIX_TEST_DAC_CHOP_CLK_SETMASK)     

HW_MIX_TEST_DAC_DISABLE_RTZ_CLRMASK      equ    (~HW_MIX_TEST_DAC_DISABLE_RTZ_SETMASK)     

HW_MIX_TEST_DAC_MORE_AMP_I_CLRMASK       equ    (~HW_MIX_TEST_DAC_MORE_AMP_I_SETMASK) 

HW_MIX_TEST_TMP_CFG_CLRMASK              equ    (~HW_MIX_TEST_TMP_CFG_SETMASK) 

HW_MIX_TEST_TMPPWD_CLRMASK               equ    (~HW_MIX_TEST_TMPPWD_SETMASK) 

 
;C insert

;typedef union               

;{

;    struct {

;        int DAC_CHOP_CLK                   : HW_MIX_TEST_DAC_CHOP_CLK_WIDTH;

;        int RSVD0                          : HW_MIX_TEST_RSVD0_WIDTH;

;        int DAC_DISABLE_RTZ                : HW_MIX_TEST_DAC_DISABLE_RTZ_WIDTH;

;        int DAC_MORE_AMP_I                 : HW_MIX_TEST_DAC_MORE_AMP_I_WIDTH;

;        int RSVD1                          : HW_MIX_TEST_RSVD1_WIDTH;

;        int TMP_CFG                        : HW_MIX_TEST_TMP_CFG_WIDTH;

;        int TMPPWD                         : HW_MIX_TEST_TMPPWD_WIDTH;

;        int RSVD2                          : HW_MIX_TEST_RSVD2_WIDTH;


;    } B;

;    int I;
;    unsigned int U;

;} mix_test_type;

 

;#define HW_MIX_TEST      (*(volatile mix_test_type _X*) (HW_CODEC_BASEADDR+28))    /* Analog Persistent Config Register */

;endc


;///////////////////////////////////////////////////////////////////////////////
;   Reference Control Register (HW_REF_CTRL) Bit Definitions
HW_REF_CTRL_DACVBGVAL_BITPOS           equ     0
HW_REF_CTRL_ADJDAC_BITPOS              equ     4
HW_REF_CTRL_VAGVAL_BITPOS              equ     5
HW_REF_CTRL_ADJV_BITPOS                equ     9
HW_REF_CTRL_ADCREFV_BITPOS             equ     10
HW_REF_CTRL_ADJADC_BITPOS              equ     14
HW_REF_CTRL_PWRDWNS_BITPOS             equ     15
HW_REF_CTRL_BIASC_BITPOS               equ     16
HW_REF_CTRL_LWREF_BITPOS               equ     18
HW_REF_CTRL_LOW_PWR_BITPOS             equ     19 

HW_REF_CTRL_DACVBGVAL_WIDTH            equ     (4)
HW_REF_CTRL_ADJDAC_WIDTH               equ     (1)
HW_REF_CTRL_VAGVAL_WIDTH               equ     (4)
HW_REF_CTRL_ADJV_WIDTH                 equ     (1)
HW_REF_CTRL_ADCREFV_WIDTH              equ     (4)
HW_REF_CTRL_ADJADC_WIDTH               equ     (1)
HW_REF_CTRL_PWRDWNS_WIDTH              equ     (1)
HW_REF_CTRL_BIASC_WIDTH                equ     (2)
HW_REF_CTRL_LWREF_WIDTH                equ     (1)
HW_REF_CTRL_LOW_PWR_WIDTH              equ     (1) 
HW_REF_CTRL_RSVD_WIDTH                 equ     (4)

HW_REF_CTRL_DACVBGVAL_SETMASK          equ     (((1<<HW_REF_CTRL_DACVBGVAL_WIDTH)-1)<<HW_REF_CTRL_DACVBGVAL_BITPOS)
HW_REF_CTRL_ADJDAC_SETMASK             equ     (((1<<HW_REF_CTRL_ADJDAC_WIDTH)-1)<<HW_REF_CTRL_ADJDAC_BITPOS)
HW_REF_CTRL_VAGVAL_SETMASK             equ     (((1<<HW_REF_CTRL_VAGVAL_WIDTH)-1)<<HW_REF_CTRL_VAGVAL_BITPOS)
HW_REF_CTRL_ADJV_SETMASK               equ     (((1<<HW_REF_CTRL_ADJV_WIDTH)-1)<<HW_REF_CTRL_ADJV_BITPOS)
HW_REF_CTRL_ADCREFV_SETMASK            equ     (((1<<HW_REF_CTRL_ADCREFV_WIDTH)-1)<<HW_REF_CTRL_ADCREFV_BITPOS)
HW_REF_CTRL_ADJADC_SETMASK             equ     (((1<<HW_REF_CTRL_ADJADC_WIDTH)-1)<<HW_REF_CTRL_ADJADC_BITPOS)
HW_REF_CTRL_PWRDWNS_SETMASK            equ     (((1<<HW_REF_CTRL_PWRDWNS_WIDTH)-1)<<HW_REF_CTRL_PWRDWNS_BITPOS)
HW_REF_CTRL_BIASC_SETMASK              equ     (((1<<HW_REF_CTRL_BIASC_WIDTH)-1)<<HW_REF_CTRL_BIASC_BITPOS)
HW_REF_CTRL_LWREF_SETMASK              equ     (((1<<HW_REF_CTRL_LWREF_WIDTH)-1)<<HW_REF_CTRL_LWREF_BITPOS)
HW_REF_CTRL_LOW_PWR_SETMASK            equ     (((1<<HW_REF_CTRL_LOW_PWR_WIDTH)-1)<<HW_REF_CTRL_LOW_PWR_BITPOS)

HW_REF_CTRL_DACVBGVAL_CLRMASK          equ     ~HW_REF_CTRL_DACVBGVAL_SETMASK
HW_REF_CTRL_ADJDAC_CLRMASK             equ     ~HW_REF_CTRL_ADJDAC_SETMASK
HW_REF_CTRL_VAGVAL_CLRMASK             equ     ~HW_REF_CTRL_VAGVAL_SETMASK
HW_REF_CTRL_ADJV_CLRMASK               equ     ~HW_REF_CTRL_ADJV_SETMASK
HW_REF_CTRL_ADCREFV_CLRMASK            equ     ~HW_REF_CTRL_ADCREFV_SETMASK
HW_REF_CTRL_ADJADC_CLRMASK             equ     ~HW_REF_CTRL_ADJADC_SETMASK
HW_REF_CTRL_PWRDWNS_CLRMASK            equ     ~HW_REF_CTRL_PWRDWNS_SETMASK
HW_REF_CTRL_BIASC_CLRMASK              equ     ~HW_REF_CTRL_BIASC_SETMASK
HW_REF_CTRL_LWREF_CLRMASK              equ     ~HW_REF_CTRL_LWREF_SETMASK
HW_REF_CTRL_LOW_PWR_CLRMASK            equ     ~HW_REF_CTRL_LOW_PWR_SETMASK

;C insert
;typedef union               
;{
;    struct {

;        int DACVBGVAL	   : HW_REF_CTRL_DACVBGVAL_WIDTH;
;        int ADJDAC		   : HW_REF_CTRL_ADJDAC_WIDTH;
;        int VAGVAL		   : HW_REF_CTRL_VAGVAL_WIDTH;
;        int ADJV		   : HW_REF_CTRL_ADJV_WIDTH;
;        int ADCREFV       : HW_REF_CTRL_ADCREFV_WIDTH;
;        int ADJADC		   : HW_REF_CTRL_ADJADC_WIDTH;
;        int PWRDWNS       : HW_REF_CTRL_PWRDWNS_WIDTH;
;        int BIASC         : HW_REF_CTRL_BIASC_WIDTH;
;        int LWREF         : HW_REF_CTRL_LWREF_WIDTH;
;        int LOW_PWR       : HW_REF_CTRL_LOW_PWR_WIDTH;
;        int RSVD 	       : HW_REF_CTRL_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} ref_ctrl_type;

;#define HW_REF_CTRL (*(volatile ref_ctrl_type _X*) (HW_CODEC_BASEADDR+25))

;endc
 


;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;////  DAC Registers
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
HW_DAC_BASEADDR                         equ     $F800

;ASM Only
HW_DACCSR                               equ     HW_DAC_BASEADDR
HW_DACSRR                               equ     HW_DAC_BASEADDR+1
HW_DACWCR                               equ     HW_DAC_BASEADDR+2
HW_DACCPR                               equ     HW_DAC_BASEADDR+3
HW_DACMR                                equ     HW_DAC_BASEADDR+4
HW_DACBAR                               equ     HW_DAC_BASEADDR+5
HW_DACICR                               equ     HW_DAC_BASEADDR+6
;endasm


;///////////////////////////////////////////////////////////////////////////////
;  DAC Control Status Register (HW_DACCSR) Bit Definitions
HW_DACCSR_TXEN_BITPOS                   equ     0
HW_DACCSR_TXIEN_BITPOS                  equ     1
HW_DACCSR_TXI_BITPOS                    equ     2
HW_DACCSR_TXEXC_BITPOS                  equ     3
HW_DACCSR_LPBK_BITPOS                   equ     4
HW_DACCSR_DMASEL_BITPOS                 equ     5
HW_DACCSR_DAC_HIPPOP_EN_BITPOS          equ     7
HW_DACCSR_LVUP_BITPOS               equ     8
HW_DACCSR_RVUP_BITPOS                   equ     9
HW_DACCSR_VUP_BITPOS                    equ     10
HW_DACCSR_RSRVD_BITPOS                  equ     11
HW_DACCSR_CLKGT_BITPOS                  equ     23

HW_DACCSR_TXEN_WIDTH                    equ     (1)
HW_DACCSR_TXIEN_WIDTH                   equ     (1)
HW_DACCSR_TXI_WIDTH                     equ     (1)
HW_DACCSR_TXEXC_WIDTH                   equ     (1)
HW_DACCSR_LPBK_WIDTH                    equ     (1)
HW_DACCSR_DMASEL_WIDTH                  equ     (2)
HW_DACCSR_DAC_HIPPOP_EN_WIDTH           equ     (1)
HW_DACCSR_LVUP_WIDTH                    equ     (1)
HW_DACCSR_RVUP_WIDTH                    equ     (1)
HW_DACCSR_VUP_WIDTH                     equ     (1)
HW_DACCSR_RSRVD_WIDTH                   equ     (12)
HW_DACCSR_CLKGT_WIDTH                   equ     (1)

HW_DACCSR_TXEN_SETMASK                  equ     1<<HW_DACCSR_TXEN_BITPOS
HW_DACCSR_TXIEN_SETMASK                 equ     1<<HW_DACCSR_TXIEN_BITPOS
HW_DACCSR_TXI_SETMASK                   equ     1<<HW_DACCSR_TXI_BITPOS
HW_DACCSR_TXEXC_SETMASK                 equ     1<<HW_DACCSR_TXEXC_BITPOS
HW_DACCSR_LPBK_SETMASK                  equ     1<<HW_DACCSR_LPBK_BITPOS
HW_DACCSR_DMASEL_P_SETMASK              equ     ((2)<<(HW_DACCSR_DMASEL_BITPOS))
HW_DACCSR_DMASEL_Y_SETMASK              equ     (1<<(HW_DACCSR_DMASEL_BITPOS))
;         DMASEL_X has no asm setmask since X config value is 00 binary.
HW_DACCSR_DAC_HIPPOP_EN_SETMASK         equ     (1<<HW_DACCSR_DAC_HIPPOP_EN_BITPOS)
HW_DACCSR_LVUP_SETMASK                  equ     (1<<HW_DACCSR_LVUP_BITPOS)
HW_DACCSR_RVUP_SETMASK                  equ     (1<<HW_DACCSR_RVUP_BITPOS)
HW_DACCSR_VUP_SETMASK                   equ     (1<<HW_DACCSR_VUP_BITPOS)
HW_DACCSR_CLKGT_SETMASK                 equ     (1<<HW_DACCSR_CLKGT_BITPOS)

HW_DACCSR_TXEN_CLRMASK                  equ     ~HW_DACCSR_TXEN_SETMASK
HW_DACCSR_TXIEN_CLRMASK                 equ     ~HW_DACCSR_TXIEN_SETMASK
HW_DACCSR_TXI_CLRMASK                   equ     ~HW_DACCSR_TXI_SETMASK
HW_DACCSR_TXEXC_CLRMASK                 equ     ~HW_DACCSR_TXEXC_SETMASK
HW_DACCSR_LPBK_CLRMASK                  equ     ~HW_DACCSR_LPBK_SETMASK
; No asm clrmask for 2 bit bitfield DMASEL
HW_DACCSR_DAC_HIPPOP_EN_CLRMASK         equ     ~HW_DACCSR_DAC_HIPPOP_EN_SETMASK
HW_DACCSR_LVUP_CLRMASK                  equ     ~HW_DACCSR_LVUP_SETMASK
HW_DACCSR_RVUP_CLRMASK                  equ     ~HW_DACCSR_RVUP_SETMASK
HW_DACCSR_VUP_CLRMASK                   equ     ~HW_DACCSR_VUP_SETMASK
HW_DACCSR_CLKGT_CLRMASK                 equ     ~HW_DACCSR_CLKGT_SETMASK 

 
;C insert
;typedef union               
;{
;    struct {
;        int TXEN                        : HW_DACCSR_TXEN_WIDTH;
;        int TXIEN                       : HW_DACCSR_TXIEN_WIDTH;
;        int TXI                         : HW_DACCSR_TXI_WIDTH;
;        int TXEXC                       : HW_DACCSR_TXEXC_WIDTH;
;        int LPBK                        : HW_DACCSR_LPBK_WIDTH;
;        int DMASEL                      : HW_DACCSR_DMASEL_WIDTH;
;             #define HW_DACCSR_DMASEL_X_BITFIELD_VAL    0
;             #define HW_DACCSR_DMASEL_Y_BITFIELD_VAL    1
;             #define HW_DACCSR_DMASEL_P_BITFIELD_VAL    2       
;        int DAC_HIPPOP_EN               : HW_DACCSR_DAC_HIPPOP_EN_WIDTH;
;        int LVUP                        : HW_DACCSR_LVUP_WIDTH;
;        int RVUP                        : HW_DACCSR_RVUP_WIDTH;
;        int VUP                         : HW_DACCSR_VUP_WIDTH;
;        int RSVD                        : HW_DACCSR_RSRVD_WIDTH;
;        int CLKGT                       : HW_DACCSR_CLKGT_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} dac_csr_type;

;#define HW_DACCSR      (*(volatile dac_csr_type _X*) (HW_DAC_BASEADDR))

;endc



;///////////////////////////////////////////////////////////////////////////////
;  DAC Sample Rate Register (HW_DACSRR) Bit Definitions
HW_DACSRR_SR_BITPOS                     equ     0

HW_DACSRR_SR_WIDTH                      equ     (23)
HW_DACSRR_RSVD_WIDTH                    equ     (1)

;HW_DACSRR_SR_SETMASK                    equ     $FFFFFF<<HW_DACSRR_SR_BITPOS
HW_DACSRR_SR_SETMASK                   equ     (((1<<HW_DACSRR_SR_WIDTH)-1)<<HW_DACSRR_SR_BITPOS)        

HW_DACSRR_SR_CLRMASK                    equ     ~HW_DACSRR_SR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int SR                          : HW_DACSRR_SR_WIDTH;
;        int RSVD                        : HW_DACSRR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} dac_srr_type;

;#define HW_DACSRR      (*(volatile dac_srr_type _X*) (HW_DAC_BASEADDR+1))

;endc



;///////////////////////////////////////////////////////////////////////////////
;  DAC Word Count Register (HW_DACWCR) Bit Definitions
HW_DACWCR_WCR_BITPOS                    equ     0

HW_DACWCR_WCR_WIDTH                     equ     (10)
HW_DACWCR_RSVD_WIDTH                    equ     (12)

;HW_DACWCR_WCR_SETMASK                   equ     $3FF<<HW_DACWCR_WCR_BITPOS
HW_DACWCR_WCR_SETMASK                   equ     (((1<<HW_DACWCR_WCR_WIDTH)-1)<<HW_DACWCR_WCR_BITPOS)        

HW_DACWCR_WCR_CLRMASK                   equ     ~HW_DACWCR_WCR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int WCR                         : HW_DACWCR_WCR_WIDTH;
;        int RSVD                        : HW_DACWCR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} dac_wcr_type;

;#define HW_DACWCR      (*(volatile dac_wcr_type _X*) (HW_DAC_BASEADDR+2))

;endc



;///////////////////////////////////////////////////////////////////////////////
;  DAC Current Position Register (HW_DACCPR) Bit Definitions
HW_DACCPR_CPR_BITPOS                    equ     0

HW_DACCPR_CPR_WIDTH                     equ     (10)
HW_DACCPR_RSVD_WIDTH                    equ     (14)

;HW_DACCPR_CPR_SETMASK                   equ     $3FF<<HW_DACCPR_CPR_BITPOS
HW_DACCPR_CPR_SETMASK                   equ     (((1<<HW_DACCPR_CPR_WIDTH)-1)<<HW_DACCPR_CPR_BITPOS)        

HW_DACCPR_CPR_CLRMASK                   equ     ~HW_DACCPR_CPR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int CPR                         : HW_DACCPR_CPR_WIDTH;
;        int RSVD                        : HW_DACCPR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} dac_cpr_type;

;#define HW_DACCPR      (*(volatile dac_cpr_type _X*) (HW_DAC_BASEADDR+3))

;endc



;///////////////////////////////////////////////////////////////////////////////
;  DAC Modulo Register (HW_DACMR) Bit Definitions
HW_DACMR_MR_BITPOS                      equ     0
HW_DACMR_MR_WIDTH                       equ     (10)
HW_DACMR_RSVD_WIDTH                     equ     (14)

;HW_DACMR_MR_SETMASK                     equ     $3FF<<HW_DACMR_MR_BITPOS
HW_DACMR_MR_SETMASK                    equ     (((1<<HW_DACMR_MR_WIDTH)-1)<<HW_DACMR_MR_BITPOS)        

HW_DACMR_MR_CLRMASK                     equ     ~HW_DACMR_MR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int MR                         : HW_DACMR_MR_WIDTH;
;        int RSVD                       : HW_DACMR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} dac_mr_type;

;#define HW_DACMR      (*(volatile dac_mr_type _X*) (HW_DAC_BASEADDR+4))

;endc



;///////////////////////////////////////////////////////////////////////////////
;   DAC Base Address Register (HW_DACBAR) Bit Definitions
HW_DACBAR_BAR_BITPOS                    equ     0
HW_DACBAR_BAR_WIDTH                     equ     (16)
HW_DACBAR_RSVD_WIDTH                    equ     (8)

;HW_DACBAR_BAR_SETMASK                   equ     $FFFF<<HW_DACBAR_BAR_BITPOS
HW_DACBAR_BAR_SETMASK                    equ     (((1<<HW_DACBAR_BAR_WIDTH)-1)<<HW_DACBAR_BAR_BITPOS)        

HW_DACBAR_BAR_CLRMASK                   equ     ~HW_DACBAR_BAR_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int BAR                        : HW_DACBAR_BAR_WIDTH;
;        int RSVD                       : HW_DACBAR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} dac_bar_type;

;#define HW_DACBAR      (*(volatile dac_bar_type _X*) (HW_DAC_BASEADDR+5))

;endc



;///////////////////////////////////////////////////////////////////////////////
;  DAC Interrupt Control Register (HW_DACICR) Bit Definitions
HW_DACICR_IPT_BITPOS                    equ     0

HW_DACICR_IPT_WIDTH                     equ     (10)
HW_DACICR_RSVD_WIDTH                    equ     (12)

HW_DACICR_IPT_SETMASK                   equ     (((1<<HW_DACICR_IPT_WIDTH)-1)<<HW_DACICR_IPT_BITPOS)        

HW_DACICR_IPT_CLRMASK                   equ     ~HW_DACICR_IPT_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int IPT                         : HW_DACICR_IPT_WIDTH;
;        int RSVD                        : HW_DACICR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} dac_icr_type;

;#define HW_DACICR      (*(volatile dac_icr_type _X*) (HW_DAC_BASEADDR+6))

;endc





;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;////  ADC Registers
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
HW_ADC_BASEADDR                         equ     $FB00

;ASM Only
HW_ADCCSR                               equ     HW_ADC_BASEADDR
HW_ADCSRR                               equ     HW_ADC_BASEADDR+1
HW_ADCWCR                               equ     HW_ADC_BASEADDR+2
HW_ADCCPR                               equ     HW_ADC_BASEADDR+3
HW_ADCMR                                equ     HW_ADC_BASEADDR+4
HW_ADCBAR                               equ     HW_ADC_BASEADDR+5
HW_ADCIPR                               equ     HW_ADC_BASEADDR+6
;endasm


;///////////////////////////////////////////////////////////////////////////////
;  ADC Control Status Register (HW_ADCCSR) Bit Definitions
HW_ADCCSR_TXEN_BITPOS                   equ     0
HW_ADCCSR_TXIEN_BITPOS                  equ     1
HW_ADCCSR_TXI_BITPOS                    equ     2
HW_ADCCSR_TXEXC_BITPOS                  equ     3
HW_ADCCSR_LPBK_BITPOS                   equ     4
HW_ADCCSR_DMASEL_BITPOS                 equ     5
HW_ADCCSR_LPFMODE_BITPOS                equ     8
HW_ADCCSR_CLKGT_BITPOS                  equ     23

HW_ADCCSR_TXEN_WIDTH                    equ     (1)
HW_ADCCSR_TXIEN_WIDTH                   equ     (1)
HW_ADCCSR_TXI_WIDTH                     equ     (1)
HW_ADCCSR_TXEXC_WIDTH                   equ     (1)
HW_ADCCSR_LPBK_WIDTH                    equ     (1)
HW_ADCCSR_DMASEL_WIDTH                  equ     (2)
HW_ADCCSR_RSVD1_WIDTH                   equ     (1)
HW_ADCCSR_LPFMODE_WIDTH                 equ     (4)
HW_ADCCSR_RSVD2_WIDTH                   equ     (11)
HW_ADCCSR_CLKGT_WIDTH                   equ     (1)

HW_ADCCSR_TXEN_SETMASK                  equ     1<<HW_ADCCSR_TXEN_BITPOS
HW_ADCCSR_TXIEN_SETMASK                 equ     1<<HW_ADCCSR_TXIEN_BITPOS
HW_ADCCSR_TXI_SETMASK                   equ     1<<HW_ADCCSR_TXI_BITPOS
HW_ADCCSR_TXEXC_SETMASK                 equ     1<<HW_ADCCSR_TXEXC_BITPOS
HW_ADCCSR_LPBK_SETMASK                  equ     1<<HW_ADCCSR_LPBK_BITPOS
HW_ADCCSR_DMASEL_SETMASK                equ     (((1<<HW_ADCCSR_DMASEL_WIDTH)-1)<<HW_ADCCSR_DMASEL_BITPOS)        
HW_ADCCSR_LPFMODE_SETMASK                equ     (((1<<HW_ADCCSR_LPFMODE_WIDTH)-1)<<HW_ADCCSR_LPFMODE_BITPOS)        
HW_ADCCSR_CLKGT_SETMASK                 equ     1<<HW_ADCCSR_CLKGT_BITPOS

HW_ADCCSR_TXEN_CLRMASK                  equ     ~HW_ADCCSR_TXEN_SETMASK
HW_ADCCSR_TXIEN_CLRMASK                 equ     ~HW_ADCCSR_TXIEN_SETMASK
HW_ADCCSR_TXI_CLRMASK                   equ     ~HW_ADCCSR_TXI_SETMASK
HW_ADCCSR_TXEXC_CLRMASK                 equ     ~HW_ADCCSR_TXEXC_SETMASK
HW_ADCCSR_LPBK_CLRMASK                  equ     ~HW_ADCCSR_LPBK_SETMASK
HW_ADCCSR_DMASEL_CLRMASK                equ     ~HW_ADCCSR_DMASEL_SETMASK
HW_ADCCSR_LPFMODE_CLRMASK               equ     ~HW_ADCCSR_LPFMODE_SETMASK
HW_ADCCSR_CLKGT_CLRMASK                 equ     ~HW_ADCCSR_CLKGT_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int TXEN                        : HW_ADCCSR_TXEN_WIDTH;
;        int TXIEN                       : HW_ADCCSR_TXIEN_WIDTH;
;        int TXI                         : HW_ADCCSR_TXI_WIDTH;
;        int TXEXC                       : HW_ADCCSR_TXEXC_WIDTH;
;        int LPBK                        : HW_ADCCSR_LPBK_WIDTH;
;        int RSVD1                       : HW_ADCCSR_RSVD1_WIDTH;
;        int DMASEL                      : HW_ADCCSR_DMASEL_WIDTH;
;        int LPFMODE                     : HW_ADCCSR_LPFMODE_WIDTH;
;        int RSVD2                       : HW_ADCCSR_RSVD2_WIDTH;
;        int CLKGT                       : HW_ADCCSR_CLKGT_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} adc_csr_type;

;#define HW_ADCCSR (*(volatile adc_csr_type _X*) (HW_ADC_BASEADDR+0))

;endc


;///////////////////////////////////////////////////////////////////////////////
;  ADC Word Count Register (HW_ADCWCR) Bit Definitions
HW_ADCWCR_WCR_BITPOS                    equ     0

HW_ADCWCR_WCR_WIDTH                     equ     (10)
HW_ADCWCR_RSVD_WIDTH                    equ     (14)

HW_ADCWCR_WCR_SETMASK                   equ     (((1<<HW_ADCWCR_WCR_WIDTH)-1)<<HW_ADCWCR_WCR_BITPOS)        

HW_ADCWCR_WCR_CLRMASK                   equ     ~HW_ADCWCR_WCR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int WCR                         : HW_ADCWCR_WCR_WIDTH;
;        int RSVD                        : HW_ADCWCR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} adc_wcr_type;

;#define HW_ADCWCR (*(volatile adc_wcr_type _X*) (HW_ADC_BASEADDR+2))

;endc



;///////////////////////////////////////////////////////////////////////////////
;   ADC Base Address Register (HW_ADCBAR) Bit Definitions
HW_ADCBAR_BAR_BITPOS                    equ     0
HW_ADCBAR_BAR_WIDTH                     equ     (16)
HW_ADCBAR_RSVD_WIDTH                    equ     (8)

;HW_ADCBAR_BAR_SETMASK                   equ     $FFFF<<HW_ADCBAR_BAR_BITPOS
HW_ADCBAR_BAR_SETMASK                    equ     (((1<<HW_ADCBAR_BAR_WIDTH)-1)<<HW_ADCBAR_BAR_BITPOS)        

HW_ADCBAR_BAR_CLRMASK                   equ     ~HW_ADCBAR_BAR_SETMASK

;C insert
;typedef union               
;{
;    struct {
;        int BAR                        : HW_ADCBAR_BAR_WIDTH;
;        int RSVD                       : HW_ADCBAR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} adc_bar_type;

;#define HW_ADCBAR (*(volatile adc_bar_type _X*) (HW_ADC_BASEADDR+5))

;endc



 

;///////////////////////////////////////////////////////////////////////////////
;  DAC Current Position Register (HW_ADCCPR) Bit Definitions
HW_ADCCPR_CPR_BITPOS                    equ     0

HW_ADCCPR_CPR_WIDTH                     equ     (10)
HW_ADCCPR_RSVD_WIDTH                    equ     (14)

HW_ADCCPR_CPR_SETMASK                   equ     (((1<<HW_ADCCPR_CPR_WIDTH)-1)<<HW_ADCCPR_CPR_BITPOS)        

HW_ADCCPR_CPR_CLRMASK                   equ     ~HW_ADCCPR_CPR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int CPR                         : HW_ADCCPR_CPR_WIDTH;
;        int RSVD                        : HW_ADCCPR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} adc_cpr_type;

;#define HW_ADCCPR (*(volatile adc_cpr_type _X*) (HW_ADC_BASEADDR+3))

;endc


;///////////////////////////////////////////////////////////////////////////////
;  ADC Modulo Register (HW_ADCMR) Bit Definitions
HW_ADCMR_MR_BITPOS                      equ     0
HW_ADCMR_MR_WIDTH                       equ     (10)
HW_ADCMR_RSVD_WIDTH                     equ     (14)

;HW_ADCMR_MR_SETMASK                     equ     $3FF<<HW_ADCMR_MR_BITPOS
HW_ADCMR_MR_SETMASK                    equ     (((1<<HW_ADCMR_MR_WIDTH)-1)<<HW_ADCMR_MR_BITPOS)        

HW_ADCMR_MR_CLRMASK                     equ     ~HW_ADCMR_MR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int MR                         : HW_ADCMR_MR_WIDTH;
;        int RSVD                       : HW_ADCMR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} adc_mr_type;

;#define HW_ADCMR (*(volatile adc_mr_type _X*) (HW_ADC_BASEADDR+4))

;endc

;///////////////////////////////////////////////////////////////////////////////
;  ADC Sample Rate Register (HW_ADCSRR) Bit Definitions
HW_ADCSRR_SR_BITPOS                     equ     0

HW_ADCSRR_SR_WIDTH                      equ     (23)
HW_ADCSRR_RSVD_WIDTH                    equ     (1)

;HW_ADCSRR_SR_SETMASK                    equ     $FFFFFF<<HW_ADCSRR_SR_BITPOS
HW_ADCSRR_SR_SETMASK                   equ     (((1<<HW_ADCSRR_SR_WIDTH)-1)<<HW_ADCSRR_SR_BITPOS)        

HW_ADCSRR_SR_CLRMASK                    equ     ~HW_ADCSRR_SR_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int SR                          : HW_ADCSRR_SR_WIDTH;
;        int RSVD                        : HW_ADCSRR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} adc_srr_type;

;#define HW_ADCSRR (*(volatile adc_srr_type _X*) (HW_ADC_BASEADDR+1))

;endc

;///////////////////////////////////////////////////////////////////////////////
;  ADC Interrupt Control Register (HW_ADCICR) Bit Definitions
HW_ADCICR_IPT_BITPOS                    equ     0

HW_ADCICR_IPT_WIDTH                     equ     (10)
HW_ADCICR_RSVD_WIDTH                    equ     (12)

HW_ADCICR_IPT_SETMASK                   equ     (((1<<HW_ADCICR_IPT_WIDTH)-1)<<HW_ADCICR_IPT_BITPOS)        

HW_ADCICR_IPT_CLRMASK                   equ     ~HW_ADCICR_IPT_SETMASK
 
;C insert
;typedef union               
;{
;    struct {
;        int IPT                         : HW_ADCICR_IPT_WIDTH;
;        int RSVD                        : HW_ADCICR_RSVD_WIDTH;
;    } B;
;    int I;
;    unsigned int U;
;} adc_icr_type;

;#define HW_ADCICR (*(volatile adc_icr_type _X*) (HW_ADC_BASEADDR+6))

;endc

 ;*********************  REGISTER ALIAS DEFINES TO MATCH LEGACY CODE *******************************
 ;
 ; The following defines were added to match regs3410.inc definition to build SDK2XXX code without needing 
 ; to update the actual files. Only the defines needed to build SDK2.400 were added.

HW_MIXTBR_PW_ADC_RIGHT_CH_BITPOS        equ     16

HW_MIXLINE1INVR_GN_ZERO_SETMASK         equ     $808

    endif
