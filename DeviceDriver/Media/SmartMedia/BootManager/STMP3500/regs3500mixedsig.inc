;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2003
;;  File        : regs3500mixedsig.inc
;;  Description : Mixed Signal IP Register definition
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; The following naming conventions are followed in this file.
; All registers are named using the format...
;     HW_<module>_<regname>
; where <module> is the module name which can be any of the following...
;     SYSTEM
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

    if !(@def(regs3500mixedsiginc))
regs3500mixedsiginc    equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   SYSTEM STMP Registers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   SYSTEM STMP Registers
;	Last Edited 2.19.2003 M. May
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_CODEC_BASEADDR             equ     $FA00
HW_DCDC_BASEADDR              equ     $FA0C
HW_LRADC_BASEADDR             equ     $FA20
HW_DAC_BASEADDR               equ     $F800
HW_ADC_BASEADDR               equ     $FB00

;ASM Only
HW_MIXTBR                     equ     HW_CODEC_BASEADDR+3      ; Codec/Mixer test bit register
HW_MIXMASTERVR                equ     HW_CODEC_BASEADDR+4      ; Mixer master volume register
HW_MIXMICINVR                 equ     HW_CODEC_BASEADDR+5      ; Mixer mic-in volume register
HW_MIXLINE1INVR               equ     HW_CODEC_BASEADDR+6      ; Mixer line-in volume register
HW_MIXLINE2INVR               equ     HW_CODEC_BASEADDR+7      ; Mixer fm-in volume register
HW_MIXDACINVR                 equ     HW_CODEC_BASEADDR+8      ; Mixer DAC volume register
HW_MIXRECSELR                 equ     HW_CODEC_BASEADDR+9      ; Mixer ADC select register
HW_MIXADCGAINR                equ     HW_CODEC_BASEADDR+10     ; Mixer ADC gain register
HW_MIXPWRDNR                  equ     HW_CODEC_BASEADDR+11     ; Mixer power down register
HW_REF_CTRL                   equ     HW_CODEC_BASEADDR+25     ; Reference voltage control register
HW_HPCR                       equ     (HW_CODEC_BASEADDR+21)
HW_PERSIST_CFG                equ     (HW_CODEC_BASEADDR+27)
HW_MIX_TEST                   equ     (HW_CODEC_BASEADDR+28)
HW_DCDC1_CTRL0                equ     (HW_DCDC_BASEADDR)
HW_DCDC1_CTRL1                equ     (HW_DCDC_BASEADDR+1)
HW_DCDC_VDDIO                 equ     (HW_DCDC_BASEADDR+2)
HW_DCDC_VDDD                  equ     (HW_DCDC_BASEADDR+3)
HW_DCDC_VDDA                  equ     (HW_DCDC_BASEADDR+4)
HW_DCDC2_CTRL0                equ     (HW_DCDC_BASEADDR+5)
HW_DCDC2_CTRL1                equ     (HW_DCDC_BASEADDR+6)
HW_SPEED                      equ     (HW_DCDC_BASEADDR+7)
HW_DCDCTBR                    equ     (HW_DCDC_BASEADDR+8)
HW_VDD5V_PWR_CHARGE           equ     (HW_DCDC_BASEADDR+17)
HW_BATT_CTRL                  equ     (HW_LRADC_BASEADDR)
HW_BATT_THRSH                 equ     (HW_LRADC_BASEADDR+1)
HW_BATT_RESULT                equ     (HW_LRADC_BASEADDR+2)
HW_LRADC1_CTRL                equ     (HW_LRADC_BASEADDR+3)
HW_LRADC1_THRSH               equ     (HW_LRADC_BASEADDR+4)
HW_LRADC1_RESULT              equ     (HW_LRADC_BASEADDR+5)
HW_LRADC2_CTRL                equ     (HW_LRADC_BASEADDR+6)
HW_LRADC2_THRSH               equ     (HW_LRADC_BASEADDR+7)
HW_LRADC2_RESULT              equ     (HW_LRADC_BASEADDR+8)
HW_DACCSR                     equ     HW_DAC_BASEADDR
HW_DACSRR                     equ     HW_DAC_BASEADDR+1
HW_DACWCR                     equ     HW_DAC_BASEADDR+2
HW_DACCPR                     equ     HW_DAC_BASEADDR+3
HW_DACMR                      equ     HW_DAC_BASEADDR+4
HW_DACBAR                     equ     HW_DAC_BASEADDR+5
HW_DACIPR                     equ     HW_DAC_BASEADDR+6
HW_ADCCSR                     equ     HW_ADC_BASEADDR
HW_ADCSRR                     equ     HW_ADC_BASEADDR+1
HW_ADCWCR                     equ     HW_ADC_BASEADDR+2
HW_ADCCPR                     equ     HW_ADC_BASEADDR+3
HW_ADCMR                      equ     HW_ADC_BASEADDR+4
HW_ADCBAR                     equ     HW_ADC_BASEADDR+5
HW_ADCIPR                     equ     HW_ADC_BASEADDR+6
;endasm


;///////////////////////////////////////////////////////////////////////////////

;  headphone Control Register (HW_HPCR) Bit Definitions
HW_HPCR_ITEST_BITPOS                 equ     (0)
HW_HPCR_POP_BITPOS                   equ     (4)
HW_HPCR_PWD_BITPOS                   equ     (8)
HW_HPCR_CLASSAB_BITPOS               equ     (9)
HW_HPCR_NOCAP_BITPOS                 equ     (10)
HW_HPCR_SHORTMODE_BITPOS             equ     (12)
HW_HPCR_SHORT_LVLADJ_BITPOS          equ     (16)
HW_HPCR_CHOP_CLK_BITPOS              equ     (20)

HW_HPCR_ITEST_WIDTH                  equ     (4)
HW_HPCR_POP_WIDTH                    equ     (3)
HW_HPCR_RSVD0_WIDTH                  equ     (1)
HW_HPCR_PWD_WIDTH                    equ     (1)
HW_HPCR_CLASSAB_WIDTH                equ     (1)
HW_HPCR_NOCAP_WIDTH                  equ     (1)
HW_HPCR_RSVD1_WIDTH                  equ     (1)
HW_HPCR_SHORTMODE_WIDTH              equ     (4)
HW_HPCR_SHORT_LVLADJ_WIDTH           equ     (2)
HW_HPCR_RSVD2_WIDTH                  equ     (2)
HW_HPCR_CHOP_CLK_WIDTH               equ     (2)
HW_HPCR_RSVD3_WIDTH                  equ     (2)

HW_HPCR_ITEST_SETMASK                equ     (((1<<HW_HPCR_ITEST_WIDTH)-1)<<HW_HPCR_ITEST_BITPOS)
HW_HPCR_POP_SETMASK                  equ     (((1<<HW_HPCR_POP_WIDTH)-1)<<HW_HPCR_POP_BITPOS)
HW_HPCR_PWD_SETMASK                  equ     (((1<<HW_HPCR_PWD_WIDTH)-1)<<HW_HPCR_PWD_BITPOS)
HW_HPCR_CLASSAB_SETMASK              equ     (((1<<HW_HPCR_CLASSAB_WIDTH)-1)<<HW_HPCR_CLASSAB_BITPOS)
HW_HPCR_NOCAP_SETMASK                equ     (((1<<HW_HPCR_NOCAP_WIDTH)-1)<<HW_HPCR_NOCAP_BITPOS)
HW_HPCR_SHORTMODE_SETMASK            equ     (((1<<HW_HPCR_SHORTMODE_WIDTH)-1)<<HW_HPCR_SHORTMODE_BITPOS)
HW_HPCR_SHORT_LVLADJ_SETMASK         equ     (((1<<HW_HPCR_SHORT_LVLADJ_WIDTH)-1)<<HW_HPCR_SHORT_LVLADJ_BITPOS)
HW_HPCR_CHOP_CLK_SETMASK             equ     (((1<<HW_HPCR_CHOP_CLK_WIDTH)-1)<<HW_HPCR_CHOP_CLK_BITPOS)

HW_HPCR_ITEST_CLRMASK                equ    (~HW_HPCR_ITEST_SETMASK)
HW_HPCR_POP_CLRMASK                  equ    (~HW_HPCR_POP_SETMASK)
HW_HPCR_PWD_CLRMASK                  equ    (~HW_HPCR_PWD_SETMASK)
HW_HPCR_CLASSAB_CLRMASK              equ    (~HW_HPCR_CLASSAB_SETMASK)
HW_HPCR_NOCAP_CLRMASK                equ    (~HW_HPCR_NOCAP_SETMASK)
HW_HPCR_SHORTMODE_CLRMASK            equ    (~HW_HPCR_SHORTMODE_SETMASK)
HW_HPCR_SHORT_LVLADJ_CLRMASK         equ    (~HW_HPCR_SHORT_LVLADJ_SETMASK)
HW_HPCR_CHOP_CLK_CLRMASK             equ    (~HW_HPCR_CHOP_CLK_SETMASK)

;C insert

;typedef union

;{

;    struct {

;        int ITEST               : HW_HPCR_ITEST_WIDTH;
;        int POP                 : HW_HPCR_POP_WIDTH;
;        int RSVD0               : HW_HPCR_RSVD0_WIDTH;
;        int PWD                 : HW_HPCR_PWD_WIDTH;
;        int CLASSAB             : HW_HPCR_CLASSAB_WIDTH;
;        int NOCAP               : HW_HPCR_NOCAP_WIDTH;
;        int RSVD1               : HW_HPCR_RSVD1_WIDTH;
;        int SHORTMODE           : HW_HPCR_SHORTMODE_WIDTH;
;        int SHORT_LVLADJ        : HW_HPCR_SHORT_LVLADJ_WIDTH;
;        int RSVD2               : HW_HPCR_RSVD2_WIDTH;
;        int CHOP_CLK            : HW_HPCR_CHOP_CLK_WIDTH;
;        int RSVD3               : HW_HPCR_RSVD3_WIDTH;
;    } B;

;    int I;

;} speed_type;

;#define HW_HPCR      (*(volatile speed_type _X*) (HW_CODEC_BASEADDR+21))    /* Headphone Control Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Analog Persistent Config (HW_PERSIST_CFG) Bit Definitions
HW_PERSIST_CFG_XTAL_TRIM_ENABLE_BITPOS      equ     (0)
HW_PERSIST_CFG_XTAL_BIAS_DOWN0_BITPOS       equ     (1)
HW_PERSIST_CFG_XTAL_BIAS_DOWN1_BITPOS       equ     (2)
HW_PERSIST_CFG_XTAL_TRIM0_BITPOS            equ     (3)
HW_PERSIST_CFG_XTAL_TRIM1_BITPOS            equ     (4)
HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_BITPOS     equ     (5)
HW_PERSIST_CFG_LOW_BATT_ENABLE_BITPOS       equ     (6)
HW_PERSIST_CFG_LOW_BATTERY_TYPE_BITPOS      equ     (7)
HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_BITPOS       equ     (8)
HW_PERSIST_CFG_AUTO_RESTART_BITPOS          equ     (9)
HW_PERSIST_CFG_UPDATE_BITPOS                equ     (10)
HW_PERSIST_CFG_AUTO_START_STAT_BITPOS       equ     (11)
HW_PERSIST_CFG_AUTO_RESTART_STAT_BITPOS     equ     (12)

HW_PERSIST_CFG_XTAL_TRIM_ENABLE_WIDTH       equ     (1)
HW_PERSIST_CFG_XTAL_BIAS_DOWN0_WIDTH        equ     (1)
HW_PERSIST_CFG_XTAL_BIAS_DOWN1_WIDTH        equ     (1)
HW_PERSIST_CFG_XTAL_TRIM0_WIDTH             equ     (1)
HW_PERSIST_CFG_XTAL_TRIM1_WIDTH             equ     (1)
HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_WIDTH      equ     (1)
HW_PERSIST_CFG_LOW_BATT_ENABLE_WIDTH        equ     (1)
HW_PERSIST_CFG_LOW_BATTERY_TYPE_WIDTH       equ     (1)
HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_WIDTH     equ     (1)
HW_PERSIST_CFG_AUTO_RESTART_WIDTH           equ     (1)
HW_PERSIST_CFG_UPDATE_WIDTH                 equ     (1)
HW_PERSIST_CFG_AUTO_START_STAT_WIDTH        equ     (1)
HW_PERSIST_CFG_AUTO_RESTART_STAT_WIDTH      equ     (1)
HW_PERSIST_CFG_RSVD0_WIDTH                  equ     (11)

HW_PERSIST_CFG_XTAL_TRIM_ENABLE_SETMASK     equ     (((1<<HW_PERSIST_CFG_XTAL_TRIM_ENABLE_WIDTH)-1)<<HW_PERSIST_CFG_XTAL_TRIM_ENABLE_BITPOS)
HW_PERSIST_CFG_XTAL_BIAS_DOWN0_SETMASK      equ     (((1<<HW_PERSIST_CFG_XTAL_BIAS_DOWN0_WIDTH)-1)<<HW_PERSIST_CFG_XTAL_BIAS_DOWN0_BITPOS)
HW_PERSIST_CFG_XTAL_BIAS_DOWN1_SETMASK      equ     (((1<<HW_PERSIST_CFG_XTAL_BIAS_DOWN1_WIDTH)-1)<<HW_PERSIST_CFG_XTAL_BIAS_DOWN1_BITPOS)
HW_PERSIST_CFG_XTAL_TRIM0_SETMASK           equ     (((1<<HW_PERSIST_CFG_XTAL_TRIM0_WIDTH)-1)<<HW_PERSIST_CFG_XTAL_TRIM0_BITPOS)
HW_PERSIST_CFG_XTAL_TRIM1_SETMASK           equ     (((1<<HW_PERSIST_CFG_XTAL_TRIM1_WIDTH)-1)<<HW_PERSIST_CFG_XTAL_TRIM1_BITPOS)
HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_SETMASK    equ     (((1<<HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_WIDTH)-1)<<HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_BITPOS)
HW_PERSIST_CFG_LOW_BATT_ENABLE_SETMASK      equ     (((1<<HW_PERSIST_CFG_LOW_BATT_ENABLE_WIDTH)-1)<<HW_PERSIST_CFG_LOW_BATT_ENABLE_BITPOS)
HW_PERSIST_CFG_LOW_BATTERY_TYPE_SETMASK     equ     (((1<<HW_PERSIST_CFG_LOW_BATTERY_TYPE_WIDTH)-1)<<HW_PERSIST_CFG_LOW_BATTERY_TYPE_BITPOS)
HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_SETMASK     equ     (((1<<HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_WIDTH)-1)<<HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_BITPOS)
HW_PERSIST_CFG_AUTO_RESTART_SETMASK         equ     (((1<<HW_PERSIST_CFG_AUTO_RESTART_WIDTH)-1)<<HW_PERSIST_CFG_AUTO_RESTART_BITPOS)
HW_PERSIST_CFG_UPDATE_SETMASK               equ     (((1<<HW_PERSIST_CFG_UPDATE_WIDTH)-1)<<HW_PERSIST_CFG_UPDATE_BITPOS)
HW_PERSIST_CFG_AUTO_START_STAT_SETMASK      equ     (((1<<HW_PERSIST_CFG_AUTO_START_STAT_WIDTH)-1)<<HW_PERSIST_CFG_AUTO_START_STAT_BITPOS)
HW_PERSIST_CFG_AUTO_RESTART_STAT_SETMASK    equ     (((1<<HW_PERSIST_CFG_AUTO_RESTART_STAT_WIDTH)-1)<<HW_PERSIST_CFG_AUTO_RESTART_STAT_BITPOS)

HW_PERSIST_CFG_XTAL_TRIM_ENABLE_CLRMASK     equ    (~HW_PERSIST_CFG_XTAL_TRIM_ENABLE_SETMASK)
HW_PERSIST_CFG_XTAL_BIAS_DOWN0_CLRMASK      equ    (~HW_PERSIST_CFG_XTAL_BIAS_DOWN0_SETMASK)
HW_PERSIST_CFG_XTAL_BIAS_DOWN1_CLRMASK      equ    (~HW_PERSIST_CFG_XTAL_BIAS_DOWN1_SETMASK)
HW_PERSIST_CFG_XTAL_TRIM0_CLRMASK           equ    (~HW_PERSIST_CFG_XTAL_TRIM0_SETMASK)
HW_PERSIST_CFG_XTAL_TRIM1_CLRMASK           equ    (~HW_PERSIST_CFG_XTAL_TRIM1_SETMASK)
HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_CLRMASK    equ    (~HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_SETMASK)
HW_PERSIST_CFG_LOW_BATT_ENABLE_CLRMASK      equ    (~HW_PERSIST_CFG_LOW_BATT_ENABLE_SETMASK)
HW_PERSIST_CFG_LOW_BATTERY_TYPE_CLRMASK     equ    (~HW_PERSIST_CFG_LOW_BATTERY_TYPE_SETMASK)
HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_CLRMASK     equ    (~HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_SETMASK)
HW_PERSIST_CFG_AUTO_RESTART_CLRMASK         equ    (~HW_PERSIST_CFG_AUTO_RESTART_SETMASK)
HW_PERSIST_CFG_UPDATE_CLRMASK               equ    (~HW_PERSIST_CFG_UPDATE_SETMASK)
HW_PERSIST_CFG_AUTO_START_STAT_CLRMASK      equ    (~HW_PERSIST_CFG_AUTO_START_STAT_SETMASK)
HW_PERSIST_CFG_AUTO_RESTART_STAT_CLRMASK    equ    (~HW_PERSIST_CFG_AUTO_RESTART_STAT_SETMASK)

;C insert

;typedef union

;{

;    struct {
;        int XTAL_TRIM_ENABLE               : HW_PERSIST_CFG_XTAL_TRIM_ENABLE_WIDTH;
;        int XTAL_BIAS_DOWN0                : HW_PERSIST_CFG_XTAL_BIAS_DOWN0_WIDTH;
;        int XTAL_BIAS_DOWN1                : HW_PERSIST_CFG_XTAL_BIAS_DOWN1_WIDTH;
;        int XTAL_TRIM0                     : HW_PERSIST_CFG_XTAL_TRIM0_WIDTH;
;        int XTAL_TRIM1                     : HW_PERSIST_CFG_XTAL_TRIM1_WIDTH;
;        int SLEEP_XTAL_ENABLE              : HW_PERSIST_CFG_SLEEP_XTAL_ENABLE_WIDTH;
;        int LOW_BATT_ENABLE                : HW_PERSIST_CFG_LOW_BATT_ENABLE_WIDTH;
;        int LOW_BATTERY_TYPE               : HW_PERSIST_CFG_LOW_BATTERY_TYPE_WIDTH;
;        int DISABLE_USB_AUTOSTART_TYPE     : HW_PERSIST_CFG_DISABLE_USB_AUTOSTART_TYPE_WIDTH;
;        int AUTO_RESTART                   : HW_PERSIST_CFG_AUTO_RESTART_WIDTH;
;        int UPDATE                         : HW_PERSIST_CFG_UPDATE_WIDTH;
;        int AUTO_START_STAT                : HW_PERSIST_CFG_AUTO_START_STAT_WIDTH;
;        int AUTO_RESTART_STAT              : HW_PERSIST_CFG_AUTO_RESTART_STAT_WIDTH;
;        int RSVD0                          : HW_PERSIST_CFG_RSVD0_WIDTH;
;    } B;

;    int I;

;} persist_cfg_type;

;#define HW_PERSIST_CFG      (*(volatile persist_cfg_type _X*) (HW_CODEC_BASEADDR+27))    /* Analog Persistent Config Register */

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

;} mix_test_type;

;#define HW_MIX_TEST      (*(volatile mix_test_type _X*) (HW_CODEC_BASEADDR+28))    /* Analog Persistent Config Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  DcDc#1 Control Register (HW_DCDC1_CTRL0) Bit Definitions
HW_DCDC1_CTRL0_PLEVBU_WIDTH                 equ     (5)
HW_DCDC1_CTRL0_RSVD0_WIDTH                  equ     (3)
HW_DCDC1_CTRL0_PLEVBO_WIDTH                 equ     (5)
HW_DCDC1_CTRL0_RSVD1_WIDTH                  equ     (3)
HW_DCDC1_CTRL0_NLEV_WIDTH                   equ     (5)
HW_DCDC1_CTRL0_RSVD2_WIDTH                  equ     (3)

HW_DCDC1_CTRL0_PLEVBU_BITPOS                equ     (0)
HW_DCDC1_CTRL0_PLEVBO_BITPOS                equ     (8)
HW_DCDC1_CTRL0_NLEV_BITPOS                  equ     (16)

HW_DCDC1_CTRL0_PLEVBU_SETMASK               equ     (((1<<HW_DCDC1_CTRL0_PLEVBU_BITPOS)-1)<<HW_DCDC1_CTRL0_PLEVBU_BITPOS)
HW_DCDC1_CTRL0_PLEVBO_SETMASK               equ     (((1<<HW_DCDC1_CTRL0_PLEVBO_BITPOS)-1)<<HW_DCDC1_CTRL0_PLEVBO_BITPOS)
HW_DCDC1_CTRL0_NLEV_SETMASK                 equ     (((1<<HW_DCDC1_CTRL0_NLEV_BITPOS)-1)<<HW_DCDC1_CTRL0_NLEV_BITPOS)

HW_DCDC1_CTRL0_PLEVBU_CLRMASK               equ    (~HW_DCDC1_CTRL0_PLEVBU_SETMASK)
HW_DCDC1_CTRL0_PLEVBO_CLRMASK               equ    (~HW_DCDC1_CTRL0_PLEVBO_SETMASK)
HW_DCDC1_CTRL0_NLEV_CLRMASK                 equ    (~HW_DCDC1_CTRL0_NLEV_SETMASK)

;C insert

;typedef union

;{

;    struct {
;        int PLEVBU           : HW_DCDC1_CTRL0_PLEVBU_WIDTH;
;        int RSVD0            : HW_DCDC1_CTRL0_RSVD0_WIDTH;
;        int PLEVBO           : HW_DCDC1_CTRL0_PLEVBO_WIDTH;
;        int RSVD1            : HW_DCDC1_CTRL0_RSVD1_WIDTH;
;        int NLEV             : HW_DCDC1_CTRL0_NLEV_WIDTH;
;        int RSVD2            : HW_DCDC1_CTRL0_RSVD2_WIDTH;
;    } B;

;    int I;

;} dcdc1_ctrl0_type;

;#define HW_DCDC1_CTRL0      (*(volatile dcdc1_ctrl0_type _X*) (HW_DCDC_BASEADDR))    /* DcDc#1 Limit Level Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  DCDc#1 Control Register1 (HW_DCDC1_CTRL1) Bit Definitions
HW_DCDC1_CTRL1_C_BITPOS                     equ     (0)
HW_DCDC1_CTRL1_R_BITPOS                     equ     (4)
HW_DCDC1_CTRL1_FFOR_BITPOS                  equ     (8)
HW_DCDC1_CTRL1_PFMCTRL_BITPOS               equ     (12)

HW_DCDC1_CTRL1_C_WIDTH                      equ     (4)
HW_DCDC1_CTRL1_R_WIDTH                      equ     (4)
HW_DCDC1_CTRL1_FFOR_WIDTH                   equ     (3)
HW_DCDC1_CTRL1_RSVD0_WIDTH                  equ     (1)
HW_DCDC1_CTRL1_PFMCTRL_WIDTH                equ     (12)

HW_DCDC1_CTRL1_C_SETMASK                    equ     (((1<<HW_DCDC1_CTRL1_C_BITPOS)-1)<<HW_DCDC1_CTRL1_C_BITPOS)
HW_DCDC1_CTRL1_R_SETMASK                    equ     (((1<<HW_DCDC1_CTRL1_R_BITPOS)-1)<<HW_DCDC1_CTRL1_R_BITPOS)
HW_DCDC1_CTRL1_FFOR_SETMASK                 equ     (((1<<HW_DCDC1_CTRL1_FFOR_BITPOS)-1)<<HW_DCDC1_CTRL1_FFOR_BITPOS)
HW_DCDC1_CTRL1_PFMCTRL_SETMASK              equ     (((1<<HW_DCDC1_CTRL1_PFMCTRL_BITPOS)-1)<<HW_DCDC1_CTRL1_PFMCTRL_BITPOS)

HW_DCDC1_CTRL1_C_CLRMASK                    equ    (~HW_DCDC1_CTRL1_C_SETMASK)
HW_DCDC1_CTRL1_R_CLRMASK                    equ    (~HW_DCDC1_CTRL1_R_SETMASK)
HW_DCDC1_CTRL1_FFOR_CLRMASK                 equ    (~HW_DCDC1_CTRL1_FFOR_SETMASK)
HW_DCDC1_CTRL1_PFMCTRL_CLRMASK              equ    (~HW_DCDC1_CTRL1_PFMCTRL_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int C                : HW_DCDC1_CTRL1_C_WIDTH;
;        int R                : HW_DCDC1_CTRL1_R_WIDTH;
;        int FFOR             : HW_DCDC1_CTRL1_FFOR_WIDTH;
;        int RSVD0            : HW_DCDC1_CTRL1_RSVD0_WIDTH;
;        int PFMCTRL          : HW_DCDC1_CTRL1_FFOR_WIDTH;
;    } B;

;    int I;

;} dcdc1_ctrl1_type;

;#define HW_DCDC1_CTRL1      (*(volatile dcdc1_ctrl1_type _X*) (HW_DCDC_BASEADDR+1))    /* DcDc#1 Ctrl #1 Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  DcDc VDDIO Register (HW_DCDC_VDDIO) Bit Definitions
HW_DCDC_VDDIO_VOLTAGE_LEVEL_BITPOS         equ     (0)
HW_DCDC_VDDIO_BROWNOUT_LEVEL_BITPOS        equ     (8)
HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS       equ     (16)
HW_DCDC_VDDIO_BROWNOUT_STATUS_BITPOS       equ     (20)

HW_DCDC_VDDIO_VOLTAGE_LEVEL_WIDTH          equ     (5)
HW_DCDC_VDDIO_RSVD0_WIDTH                  equ     (3)
HW_DCDC_VDDIO_BROWNOUT_LEVEL_WIDTH         equ     (5)
HW_DCDC_VDDIO_RSVD1_WIDTH                  equ     (3)
HW_DCDC_VDDIO_BROWNOUT_ENABLE_WIDTH        equ     (1)
HW_DCDC_VDDIO_RSVD2_WIDTH                  equ     (3)
HW_DCDC_VDDIO_BROWNOUT_STATUS_WIDTH        equ     (1)
HW_DCDC_VDDIO_RSVD3_WIDTH                  equ     (3)

HW_DCDC_VDDIO_VOLTAGE_LEVEL_SETMASK        equ     (((1<<HW_DCDC_VDDIO_VOLTAGE_LEVEL_BITPOS)-1)<<HW_DCDC_VDDIO_VOLTAGE_LEVEL_BITPOS)
HW_DCDC_VDDIO_BROWNOUT_LEVEL_SETMASK       equ     (((1<<HW_DCDC_VDDIO_BROWNOUT_LEVEL_BITPOS)-1)<<HW_DCDC_VDDIO_BROWNOUT_LEVEL_BITPOS)
HW_DCDC_VDDIO_BROWNOUT_ENABLE_SETMASK      equ     (((1<<HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS)-1)<<HW_DCDC_VDDIO_BROWNOUT_ENABLE_BITPOS)
HW_DCDC_VDDIO_BROWNOUT_STATUS_SETMASK      equ     (((1<<HW_DCDC_VDDIO_BROWNOUT_STATUS_BITPOS)-1)<<HW_DCDC_VDDIO_BROWNOUT_STATUS_BITPOS)

HW_DCDC_VDDIO_VOLTAGE_LEVEL_CLRMASK        equ    (~HW_DCDC_VDDIO_VOLTAGE_LEVEL_SETMASK)
HW_DCDC_VDDIO_BROWNOUT_LEVEL_CLRMASK       equ    (~HW_DCDC_VDDIO_BROWNOUT_LEVEL_SETMASK)
HW_DCDC_VDDIO_BROWNOUT_ENABLE_CLRMASK      equ    (~HW_DCDC_VDDIO_BROWNOUT_ENABLE_SETMASK)
HW_DCDC_VDDIO_BROWNOUT_STATUS_CLRMASK      equ    (~HW_DCDC_VDDIO_BROWNOUT_STATUS_SETMASK)

;C insert

;typedef union
;{
;    struct {
;        int VOLTAGE_LEVEL    : HW_DCDC_VDDIO_VOLTAGE_LEVEL_WIDTH;
;        int RSVD0            : HW_DCDC_VDDIO_RSVD0_WIDTH;
;        int BROWNOUT_LEVEL   : HW_DCDC_VDDIO_BROWNOUT_LEVEL_WIDTH;
;        int RSVD1            : HW_DCDC_VDDIO_RSVD1_WIDTH;
;        int BROWNOUT_ENABLE  : HW_DCDC_VDDIO_BROWNOUT_ENABLE_WIDTH;
;        int RSVD2            : HW_DCDC_VDDIO_RSVD2_WIDTH;
;        int BROWNOUT_STATUS  : HW_DCDC_VDDIO_BROWNOUT_STATUS_WIDTH;
;        int RSVD3            : HW_DCDC_VDDIO_RSVD3_WIDTH;
;    } B;

;    int I;

;} dcdc_vddio_type;

;#define HW_DCDC_VDDIO      (*(volatile dcdc_vddio_type _X*) (HW_DCDC_BASEADDR+2))    /* DcDc VDDIO Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  DcDc VDDD Register (HW_DCDC_VDDD) Bit Definitions
HW_DCDC_VDDD_VOLTAGE_LEVEL_BITPOS         equ     (0)
HW_DCDC_VDDD_BROWNOUT_LEVEL_BITPOS        equ     (8)
HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS       equ     (16)
HW_DCDC_VDDD_BROWNOUT_STATUS_BITPOS       equ     (20)

HW_DCDC_VDDD_VOLTAGE_LEVEL_WIDTH          equ     (5)
HW_DCDC_VDDD_RSVD0_WIDTH                  equ     (3)
HW_DCDC_VDDD_BROWNOUT_LEVEL_WIDTH         equ     (5)
HW_DCDC_VDDD_RSVD1_WIDTH                  equ     (3)
HW_DCDC_VDDD_BROWNOUT_ENABLE_WIDTH        equ     (1)
HW_DCDC_VDDD_RSVD2_WIDTH                  equ     (3)
HW_DCDC_VDDD_BROWNOUT_STATUS_WIDTH        equ     (1)
HW_DCDC_VDDD_RSVD3_WIDTH                  equ     (3)

HW_DCDC_VDDD_VOLTAGE_LEVEL_SETMASK        equ     (((1<<HW_DCDC_VDDD_VOLTAGE_LEVEL_BITPOS)-1)<<HW_DCDC_VDDD_VOLTAGE_LEVEL_BITPOS)
HW_DCDC_VDDD_BROWNOUT_LEVEL_SETMASK       equ     (((1<<HW_DCDC_VDDD_BROWNOUT_LEVEL_BITPOS)-1)<<HW_DCDC_VDDD_BROWNOUT_LEVEL_BITPOS)
HW_DCDC_VDDD_BROWNOUT_ENABLE_SETMASK      equ     (((1<<HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS)-1)<<HW_DCDC_VDDD_BROWNOUT_ENABLE_BITPOS)
HW_DCDC_VDDD_BROWNOUT_STATUS_SETMASK      equ     (((1<<HW_DCDC_VDDD_BROWNOUT_STATUS_BITPOS)-1)<<HW_DCDC_VDDD_BROWNOUT_STATUS_BITPOS)

HW_DCDC_VDDD_VOLTAGE_LEVEL_CLRMASK        equ    (~HW_DCDC_VDDD_VOLTAGE_LEVEL_SETMASK)
HW_DCDC_VDDD_BROWNOUT_LEVEL_CLRMASK       equ    (~HW_DCDC_VDDD_BROWNOUT_LEVEL_SETMASK)
HW_DCDC_VDDD_BROWNOUT_ENABLE_CLRMASK      equ    (~HW_DCDC_VDDD_BROWNOUT_ENABLE_SETMASK)
HW_DCDC_VDDD_BROWNOUT_STATUS_CLRMASK      equ    (~HW_DCDC_VDDD_BROWNOUT_STATUS_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int VOLTAGE_LEVEL    : HW_DCDC_VDDD_VOLTAGE_LEVEL_WIDTH;
;        int RSVD0            : HW_DCDC_VDDD_RSVD0_WIDTH;
;        int BROWNOUT_LEVEL   : HW_DCDC_VDDD_BROWNOUT_LEVEL_WIDTH;
;        int RSVD1            : HW_DCDC_VDDD_RSVD1_WIDTH;
;        int BROWNOUT_ENABLE  : HW_DCDC_VDDD_BROWNOUT_ENABLE_WIDTH;
;        int RSVD2            : HW_DCDC_VDDD_RSVD2_WIDTH;
;        int BROWNOUT_STATUS  : HW_DCDC_VDDD_BROWNOUT_STATUS_WIDTH;
;        int RSVD3            : HW_DCDC_VDDD_RSVD3_WIDTH;
;    } B;

;    int I;

;} dcdc_vddd_type;

;#define HW_DCDC_VDDD      (*(volatile dcdc_vddd_type _X*) (HW_DCDC_BASEADDR+3))    /* DcDc VDDD Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  DcDc Vdda Register (HW_DCDC_VDDA) Bit Definitions
HW_DCDC_VDDA_VOLTAGE_LEVEL_BITPOS         equ     (0)
HW_DCDC_VDDA_BROWNOUT_LEVEL_BITPOS        equ     (8)
HW_DCDC_VDDA_BROWNOUT_ENABLE_BITPOS       equ     (1)
HW_DCDC_VDDA_BROWNOUT_STATUS_BITPOS       equ     (20)

HW_DCDC_VDDA_VOLTAGE_LEVEL_WIDTH          equ     (5)
HW_DCDC_VDDA_RSVD0_WIDTH                  equ     (3)
HW_DCDC_VDDA_BROWNOUT_LEVEL_WIDTH         equ     (5)
HW_DCDC_VDDA_RSVD1_WIDTH                  equ     (3)
HW_DCDC_VDDA_BROWNOUT_ENABLE_WIDTH        equ     (1)
HW_DCDC_VDDA_RSVD2_WIDTH                  equ     (3)
HW_DCDC_VDDA_BROWNOUT_STATUS_WIDTH        equ     (1)
HW_DCDC_VDDA_RSVD3_WIDTH                  equ     (3)

HW_DCDC_VDDA_VOLTAGE_LEVEL_SETMASK        equ     (((1<<HW_DCDC_VDDA_VOLTAGE_LEVEL_BITPOS)-1)<<HW_DCDC_VDDA_VOLTAGE_LEVEL_BITPOS)
HW_DCDC_VDDA_BROWNOUT_LEVEL_SETMASK       equ     (((1<<HW_DCDC_VDDA_BROWNOUT_LEVEL_BITPOS)-1)<<HW_DCDC_VDDA_BROWNOUT_LEVEL_BITPOS)
HW_DCDC_VDDA_BROWNOUT_ENABLE_SETMASK      equ     (((1<<HW_DCDC_VDDA_BROWNOUT_ENABLE_BITPOS)-1)<<HW_DCDC_VDDA_BROWNOUT_ENABLE_BITPOS)
HW_DCDC_VDDA_BROWNOUT_STATUS_SETMASK      equ     (((1<<HW_DCDC_VDDA_BROWNOUT_STATUS_BITPOS)-1)<<HW_DCDC_VDDA_BROWNOUT_STATUS_BITPOS)
HW_DCDC_VDDA_VOLTAGE_LEVEL_CLRMASK        equ    (~HW_DCDC_VDDA_VOLTAGE_LEVEL_SETMASK)
HW_DCDC_VDDA_BROWNOUT_LEVEL_CLRMASK       equ    (~HW_DCDC_VDDA_BROWNOUT_LEVEL_SETMASK)
HW_DCDC_VDDA_BROWNOUT_ENABLE_CLRMASK      equ    (~HW_DCDC_VDDA_BROWNOUT_ENABLE_SETMASK)
HW_DCDC_VDDA_BROWNOUT_STATUS_CLRMASK      equ    (~HW_DCDC_VDDA_BROWNOUT_STATUS_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int VOLTAGE_LEVEL    : HW_DCDC_VDDA_VOLTAGE_LEVEL_WIDTH;
;        int RSVD0            : HW_DCDC_VDDA_RSVD0_WIDTH;
;        int BROWNOUT_LEVEL   : HW_DCDC_VDDA_BROWNOUT_LEVEL_WIDTH;
;        int RSVD1            : HW_DCDC_VDDA_RSVD1_WIDTH;
;        int BROWNOUT_ENABLE  : HW_DCDC_VDDA_BROWNOUT_ENABLE_WIDTH;
;        int RSVD2            : HW_DCDC_VDDA_RSVD2_WIDTH;
;        int BROWNOUT_STATUS  : HW_DCDC_VDDA_BROWNOUT_STATUS_WIDTH;
;        int RSVD3            : HW_DCDC_VDDA_RSVD3_WIDTH;
;    } B;

;    int I;

;} dcdc_vdda_type;

;#define HW_DCDC_VDDA      (*(volatile dcdc_vdda_type _X*) (HW_DCDC_BASEADDR+4))    /* DcDc VDDA Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  DcDc#2 Control Register 0 (HW_DCDC2_CTRL0) Bit Definitions
HW_DCDC2_CTRL0_PLEVBU_BITPOS                equ     (0)
HW_DCDC2_CTRL0_PLEVBO_BITPOS                equ     (8)
HW_DCDC2_CTRL0_NLEV_BITPOS                  equ     (16)

HW_DCDC2_CTRL0_PLEVBU_WIDTH                 equ     (5)
HW_DCDC2_CTRL0_RSVD0_WIDTH                  equ     (3)
HW_DCDC2_CTRL0_PLEVBO_WIDTH                 equ     (5)
HW_DCDC2_CTRL0_RSVD1_WIDTH                  equ     (3)
HW_DCDC2_CTRL0_NLEV_WIDTH                   equ     (5)
HW_DCDC2_CTRL0_RSVD2_WIDTH                  equ     (3)

HW_DCDC2_CTRL0_PLEVBU_SETMASK               equ     (((1<<HW_DCDC2_CTRL0_PLEVBU_BITPOS)-1)<<HW_DCDC2_CTRL0_PLEVBU_BITPOS)
HW_DCDC2_CTRL0_PLEVBO_SETMASK               equ     (((1<<HW_DCDC2_CTRL0_PLEVBO_BITPOS)-1)<<HW_DCDC2_CTRL0_PLEVBO_BITPOS)
HW_DCDC2_CTRL0_NLEV_SETMASK                 equ     (((1<<HW_DCDC2_CTRL0_NLEV_BITPOS)-1)<<HW_DCDC2_CTRL0_NLEV_BITPOS)

HW_DCDC2_CTRL0_PLEVBU_CLRMASK               equ    (~HW_DCDC2_CTRL0_PLEVBU_SETMASK)
HW_DCDC2_CTRL0_PLEVBO_CLRMASK               equ    (~HW_DCDC2_CTRL0_PLEVBO_SETMASK)
HW_DCDC2_CTRL0_NLEV_CLRMASK                 equ    (~HW_DCDC2_CTRL0_NLEV_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int PLEVBU           : HW_DCDC2_CTRL0_PLEVBU_WIDTH;
;        int RSVD0            : HW_DCDC2_CTRL0_RSVD0_WIDTH;
;        int PLEVBO           : HW_DCDC2_CTRL0_PLEVBO_WIDTH;
;        int RSVD1            : HW_DCDC2_CTRL0_RSVD1_WIDTH;
;        int NLEV             : HW_DCDC2_CTRL0_NLEV_WIDTH;
;        int RSVD2            : HW_DCDC2_CTRL0_RSVD2_WIDTH;
;    } B;
;    int I;
;} dcdc2_ctrl0_type;

;#define HW_DCDC2_CTRL0      (*(volatile dcdc2_ctrl0_type _X*) (HW_DCDC_BASEADDR+5))    /* DcDc#2 Limit Level Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  DcDc#2 Control Register #1 (HW_DCDC2_CTRL1) Bit Definitions
HW_DCDC2_CTRL1_C_BITPOS                     equ     (0)
HW_DCDC2_CTRL1_R_BITPOS                     equ     (4)
HW_DCDC2_CTRL1_FFOR_BITPOS                  equ     (8)
HW_DCDC2_CTRL1_PFMCTRL_BITPOS               equ     (12)

HW_DCDC2_CTRL1_C_WIDTH                      equ     (4)
HW_DCDC2_CTRL1_R_WIDTH                      equ     (4)
HW_DCDC2_CTRL1_FFOR_WIDTH                   equ     (3)
HW_DCDC2_CTRL1_RSVD0_WIDTH                  equ     (1)
HW_DCDC2_CTRL1_PFMCTRL_WIDTH                equ     (12)

HW_DCDC2_CTRL1_C_SETMASK                    equ     (((1<<HW_DCDC2_CTRL1_C_BITPOS)-1)<<HW_DCDC2_CTRL1_C_BITPOS)
HW_DCDC2_CTRL1_R_SETMASK                    equ     (((1<<HW_DCDC2_CTRL1_R_BITPOS)-1)<<HW_DCDC2_CTRL1_R_BITPOS)
HW_DCDC2_CTRL1_FFOR_SETMASK                 equ     (((1<<HW_DCDC2_CTRL1_FFOR_BITPOS)-1)<<HW_DCDC2_CTRL1_FFOR_BITPOS)
HW_DCDC2_CTRL1_PFMCTRL_SETMASK              equ     (((1<<HW_DCDC2_CTRL1_PFMCTRL_BITPOS)-1)<<HW_DCDC2_CTRL1_PFMCTRL_BITPOS)

HW_DCDC2_CTRL1_C_CLRMASK                    equ    (~HW_DCDC2_CTRL1_C_SETMASK)
HW_DCDC2_CTRL1_R_CLRMASK                    equ    (~HW_DCDC2_CTRL1_R_SETMASK)
HW_DCDC2_CTRL1_FFOR_CLRMASK                 equ    (~HW_DCDC2_CTRL1_FFOR_SETMASK)
HW_DCDC2_CTRL1_PFMCTRL_CLRMASK              equ    (~HW_DCDC2_CTRL1_PFMCTRL_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int C                : HW_DCDC2_CTRL1_C_WIDTH;
;        int R                : HW_DCDC2_CTRL1_R_WIDTH;
;        int FFOR             : HW_DCDC2_CTRL1_FFOR_WIDTH;
;        int RSVD0            : HW_DCDC2_CTRL1_RSVD0_WIDTH;
;        int PFMCTRL          : HW_DCDC2_CTRL1_FFOR_WIDTH;
;    } B;
;    int I;
;} dcdc2_ctrl1_type;

;#define HW_DCDC2_CTRL1      (*(volatile dcdc2_ctrl1_type _X*) (HW_DCDC_BASEADDR+6))    /* DcDc#2 Ctrl Register #1 */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Speed Monitoring Register (HW_SPEED) Bit Definitions
HW_SPEED_OSC1_PWRUP_BITPOS            equ     (0)
HW_SPEED_OSC1_START_COUNT_BITPOS      equ     (1)
HW_SPEED_RINGOSC1_BITPOS              equ     (4)
HW_SPEED_OSC2_PWRUP_BITPOS            equ     (12)
HW_SPEED_OSC2_START_COUNT_BITPOS      equ     (13)
HW_SPEED_RINGOSC2_BITPOS              equ     (16)

HW_SPEED_OSC1_PWRUP_WIDTH             equ     (1)
HW_SPEED_OSC1_START_COUNT_WIDTH       equ     (1)
HW_SPEED_RSVD0_WIDTH                  equ     (2)
HW_SPEED_RINGOSC1_WIDTH               equ     (7)
HW_SPEED_RSVD1_WIDTH                  equ     (1)
HW_SPEED_OSC2_PWRUP_WIDTH             equ     (1)
HW_SPEED_OSC2_START_COUNT_WIDTH       equ     (1)
HW_SPEED_RSVD2_WIDTH                  equ     (2)
HW_SPEED_RINGOSC2_WIDTH               equ     (7)
HW_SPEED_RSVD3_WIDTH                  equ     (1)

HW_SPEED_OSC1_PWRUP_SETMASK           equ     (((1<<HW_SPEED_OSC1_PWRUP_BITPOS)-1)<<HW_SPEED_OSC1_PWRUP_BITPOS)
HW_SPEED_OSC1_START_COUNT_SETMASK     equ     (((1<<HW_SPEED_OSC1_START_COUNT_BITPOS)-1)<<HW_SPEED_OSC1_START_COUNT_BITPOS)
HW_SPEED_RINGOSC1_SETMASK             equ     (((1<<HW_SPEED_RINGOSC1_BITPOS)-1)<<HW_SPEED_RINGOSC1_BITPOS)
HW_SPEED_OSC2_PWRUP_SETMASK           equ     (((1<<HW_SPEED_OSC2_PWRUP_BITPOS)-1)<<HW_SPEED_OSC2_PWRUP_BITPOS)
HW_SPEED_OSC2_START_COUNT_SETMASK     equ     (((1<<HW_SPEED_OSC2_START_COUNT_BITPOS)-1)<<HW_SPEED_OSC2_START_COUNT_BITPOS)
HW_SPEED_RINGOSC2_SETMASK             equ     (((1<<HW_SPEED_RINGOSC2_BITPOS)-1)<<HW_SPEED_RINGOSC2_BITPOS)

HW_SPEED_OSC1_PWRUP_CLRMASK        equ    (~HW_SPEED_OSC1_PWRUP_SETMASK)
HW_SPEED_OSC1_START_COUNT_CLRMASK  equ    (~HW_SPEED_OSC1_START_COUNT_SETMASK)
HW_SPEED_RINGOSC1_CLRMASK          equ    (~HW_SPEED_RINGOSC1_SETMASK)
HW_SPEED_OSC2_PWRUP_CLRMASK        equ    (~HW_SPEED_OSC2_PWRUP_SETMASK)
HW_SPEED_OSC2_START_COUNT_CLRMASK  equ    (~HW_SPEED_OSC2_START_COUNT_SETMASK)
HW_SPEED_RINGOSC2_CLRMASK          equ    (~HW_SPEED_RINGOSC2_SETMASK)

;C insert

;typedef union
;{
;    struct {
;        int OSC1_PWRUP          : HW_SPEED_OSC1_PWRUP_WIDTH;
;        int OSC1_START_COUNT    : HW_SPEED_OSC1_START_COUNT_WIDTH;
;        int RSVD0               : HW_SPEED_RSVD0_WIDTH;
;        int RINGOSC1            : HW_SPEED_RINGOSC1_WIDTH;
;        int RSVD1               : HW_SPEED_RSVD1_WIDTH;
;        int OSC2_PWRUP          : HW_SPEED_OSC2_PWRUP_WIDTH;
;        int OSC2_START_COUNT    : HW_SPEED_OSC2_START_COUNT_WIDTH;
;        int RSVD2               : HW_SPEED_RSVD2_WIDTH;
;        int RINGOSC2            : HW_SPEED_RINGOSC2_WIDTH;
;        int RSVD3               : HW_SPEED_RSVD3_WIDTH;
;    } B;
;    int I;

;} speed_type;

;#define HW_SPEED      (*(volatile speed_type _X*) (HW_DCDC_BASEADDR+7))    /* Speed Measurement Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Analog Persistent Config (HW_VDD5V_PWR_CHARGE) Bit Definitions
HW_VDD5V_PWR_CHARGE_BATT_CURRENT_BITPOS      equ     (0)
HW_VDD5V_PWR_CHARGE_PWD_BITPOS               equ     (8)
HW_VDD5V_PWR_CHARGE_RES_BITPOS               equ     (9)
HW_VDD5V_PWR_CHARGE_NIMH_BITPOS              equ     (10)
HW_VDD5V_PWR_CHARGE_LI_TYPE_BITPOS           equ     (11)
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS    equ     (12)
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS  equ     (13)
HW_VDD5V_PWR_CHARGE_DCANA_LP_BITPOS          equ     (14)
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_BITPOS      equ     (15)
HW_VDD5V_PWR_CHARGE_DRV_BATT_BITPOS          equ     (16)
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_BITPOS        equ     (17)
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_BITPOS     equ     (20)
HW_VDD5V_PWR_CHARGE_BATT_INFO_BITPOS         equ     (21)

HW_VDD5V_PWR_CHARGE_BATT_CURRENT_WIDTH       equ     (6)
HW_VDD5V_PWR_CHARGE_RSVD0_WIDTH              equ     (2)
HW_VDD5V_PWR_CHARGE_PWD_WIDTH                equ     (1)
HW_VDD5V_PWR_CHARGE_RES_WIDTH                equ     (1)
HW_VDD5V_PWR_CHARGE_NIMH_WIDTH               equ     (1)
HW_VDD5V_PWR_CHARGE_LI_TYPE_WIDTH            equ     (1)
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_WIDTH     equ     (1)
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_WIDTH   equ     (1)
HW_VDD5V_PWR_CHARGE_DCANA_LP_WIDTH           equ     (1)
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_WIDTH       equ     (1)
HW_VDD5V_PWR_CHARGE_DRV_BATT_WIDTH           equ     (1)
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_WIDTH         equ     (1)
HW_VDD5V_PWR_CHARGE_RSVD1_WIDTH              equ     (2)
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_WIDTH      equ     (1)
HW_VDD5V_PWR_CHARGE_BATT_INFO_WIDTH          equ     (2)
HW_VDD5V_PWR_CHARGE_RSVD2_WIDTH              equ     (3)

HW_VDD5V_PWR_CHARGE_BATT_CURRENT_SETMASK     equ     (((1<<HW_VDD5V_PWR_CHARGE_BATT_CURRENT_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_BATT_CURRENT_BITPOS)
HW_VDD5V_PWR_CHARGE_PWD_SETMASK              equ     (((1<<HW_VDD5V_PWR_CHARGE_PWD_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_PWD_BITPOS)
HW_VDD5V_PWR_CHARGE_RES_SETMASK              equ     (((1<<HW_VDD5V_PWR_CHARGE_RES_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_RES_BITPOS)
HW_VDD5V_PWR_CHARGE_NIMH_SETMASK             equ     (((1<<HW_VDD5V_PWR_CHARGE_NIMH_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_NIMH_BITPOS)
HW_VDD5V_PWR_CHARGE_LI_TYPE_SETMASK          equ     (((1<<HW_VDD5V_PWR_CHARGE_LI_TYPE_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_LI_TYPE_BITPOS)
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_SETMASK   equ     (((1<<HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_BITPOS)
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_SETMASK equ     (((1<<HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_BITPOS)
HW_VDD5V_PWR_CHARGE_DCANA_LP_SETMASK         equ     (((1<<HW_VDD5V_PWR_CHARGE_DCANA_LP_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_DCANA_LP_BITPOS)
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_SETMASK     equ     (((1<<HW_VDD5V_PWR_CHARGE_TEST_USBREGS_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_TEST_USBREGS_BITPOS)
HW_VDD5V_PWR_CHARGE_DRV_BATT_SETMASK         equ     (((1<<HW_VDD5V_PWR_CHARGE_DRV_BATT_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_DRV_BATT_BITPOS)
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_SETMASK       equ     (((1<<HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_BITPOS)
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_SETMASK    equ     (((1<<HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_BITPOS)
HW_VDD5V_PWR_CHARGE_BATT_INFO_SETMASK        equ     (((1<<HW_VDD5V_PWR_CHARGE_BATT_INFO_BITPOS)-1)<<HW_VDD5V_PWR_CHARGE_BATT_INFO_BITPOS)

HW_VDD5V_PWR_CHARGE_BATT_CURRENT_CLRMASK     equ    (~HW_VDD5V_PWR_CHARGE_BATT_CURRENT_SETMASK)
HW_VDD5V_PWR_CHARGE_PWD_CLRMASK              equ    (~HW_VDD5V_PWR_CHARGE_PWD_SETMASK)
HW_VDD5V_PWR_CHARGE_RES_CLRMASK              equ    (~HW_VDD5V_PWR_CHARGE_RES_SETMASK)
HW_VDD5V_PWR_CHARGE_NIMH_CLRMASK             equ    (~HW_VDD5V_PWR_CHARGE_NIMH_SETMASK)
HW_VDD5V_PWR_CHARGE_LI_TYPE_CLRMASK          equ    (~HW_VDD5V_PWR_CHARGE_LI_TYPE_SETMASK)
HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_CLRMASK   equ    (~HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_SETMASK)
HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_CLRMASK equ    (~HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_SETMASK)
HW_VDD5V_PWR_CHARGE_DCANA_LP_CLRMASK         equ    (~HW_VDD5V_PWR_CHARGE_DCANA_LP_SETMASK)
HW_VDD5V_PWR_CHARGE_TEST_USBREGS_CLRMASK     equ    (~HW_VDD5V_PWR_CHARGE_TEST_USBREGS_SETMASK)
HW_VDD5V_PWR_CHARGE_DRV_BATT_CLRMASK         equ    (~HW_VDD5V_PWR_CHARGE_DRV_BATT_SETMASK)
HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_CLRMASK       equ    (~HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_SETMASK)
HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_CLRMASK    equ    (~HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_SETMASK)
HW_VDD5V_PWR_CHARGE_BATT_INFO_CLRMASK        equ    (~HW_VDD5V_PWR_CHARGE_BATT_INFO_SETMASK)

;C insert
;typedef union
;{
;    struct {
;        int BATT_CURRENT               : HW_VDD5V_PWR_CHARGE_BATT_CURRENT_WIDTH;
;        int RSVD0                      : HW_VDD5V_PWR_CHARGE_RSVD0_WIDTH;
;        int PWD                        : HW_VDD5V_PWR_CHARGE_PWD_WIDTH;
;        int RES                        : HW_VDD5V_PWR_CHARGE_RES_WIDTH;
;        int NIMH                       : HW_VDD5V_PWR_CHARGE_NIMH_WIDTH;
;        int LI_TYPE                    : HW_VDD5V_PWR_CHARGE_LI_TYPE_WIDTH;
;        int DISABLE_ILIMIT             : HW_VDD5V_PWR_CHARGE_DISABLE_ILIMIT_WIDTH;
;        int PWDN_ON_IOBRNOUT           : HW_VDD5V_PWR_CHARGE_PWDN_ON_IOBRNOUT_WIDTH;
;        int DCANA_LP                   : HW_VDD5V_PWR_CHARGE_DCANA_LP_WIDTH;
;        int TEST_USBREGS               : HW_VDD5V_PWR_CHARGE_TEST_USBREGS_WIDTH;
;        int DRV_BATT                   : HW_VDD5V_PWR_CHARGE_DRV_BATT_WIDTH;
;        int SWCHRG_BAT                 : HW_VDD5V_PWR_CHARGE_SWCHRG_BAT_WIDTH;
;        int RSVD1                      : HW_VDD5V_PWR_CHARGE_RSVD1_WIDTH;
;        int VDD5V_PRESENT              : HW_VDD5V_PWR_CHARGE_VDD5V_PRESENT_WIDTH;
;        int RSVD2                      : HW_VDD5V_PWR_CHARGE_RSVD2_WIDTH;
;    } B;

;    int I;

;} usb_pwr_charge_type;

;#define HW_VDD5V_PWR_CHARGE      (*(volatile usb_pwr_charge_type _X*) (HW_DCDC_BASEADDR+16))    /* Analog Persistent Config Register */
;endc

;///////////////////////////////////////////////////////////////////////////////

;  Battery LRADC Control Register (HW_BATT_CTRL) Bit Definitions
HW_BATT_CTRL_INPUT_OFFSET_BITPOS          equ     (0)
HW_BATT_CTRL_HALF_CMP_PWR_BITPOS          equ     (8)
HW_BATT_CTRL_INPUT_DIV2_BITPOS            equ     (9)
HW_BATT_CTRL_CLEAR_BITPOS                 equ     (10)
HW_BATT_CTRL_PWD_BITPOS                   equ     (11)
HW_BATT_CTRL_CLK_DIV_BITPOS               equ     (12)
HW_BATT_CTRL_REF_VAL_BITPOS               equ     (16)

HW_BATT_CTRL_INPUT_OFFSET_WIDTH           equ     (7)
HW_BATT_CTRL_RSVD0_WIDTH                  equ     (1)
HW_BATT_CTRL_HALF_CMP_PWR_WIDTH           equ     (1)
HW_BATT_CTRL_INPUT_DIV2_WIDTH             equ     (1)
HW_BATT_CTRL_CLEAR_WIDTH                  equ     (1)
HW_BATT_CTRL_PWD_WIDTH                    equ     (1)
HW_BATT_CTRL_CLK_DIV_WIDTH                equ     (2)
HW_BATT_CTRL_RSVD1_WIDTH                  equ     (2)
HW_BATT_CTRL_REF_VAL_WIDTH                equ     (2)
HW_BATT_CTRL_RSVD2_WIDTH                  equ     (6)

HW_BATT_CTRL_INPUT_OFFSET_SETMASK         equ     (((1<<HW_BATT_CTRL_INPUT_OFFSET_WIDTH)-1)<<HW_BATT_CTRL_INPUT_OFFSET_BITPOS)
HW_BATT_CTRL_HALF_CMP_PWR_SETMASK         equ     (((1<<HW_BATT_CTRL_HALF_CMP_PWR_WIDTH)-1)<<HW_BATT_CTRL_HALF_CMP_PWR_BITPOS)
HW_BATT_CTRL_INPUT_DIV2_SETMASK           equ     (((1<<HW_BATT_CTRL_INPUT_DIV2_WIDTH)-1)<<HW_BATT_CTRL_INPUT_DIV2_BITPOS)
HW_BATT_CTRL_CLEAR_SETMASK                equ     (((1<<HW_BATT_CTRL_CLEAR_WIDTH)-1)<<HW_BATT_CTRL_CLEAR_BITPOS)
HW_BATT_CTRL_PWD_SETMASK                  equ     (((1<<HW_BATT_CTRL_PWD_WIDTH)-1)<<HW_BATT_CTRL_PWD_BITPOS)
HW_BATT_CTRL_CLK_DIV_SETMASK              equ     (((1<<HW_BATT_CTRL_CLK_DIV_WIDTH)-1)<<HW_BATT_CTRL_CLK_DIV_BITPOS)
HW_BATT_CTRL_REF_VAL_SETMASK              equ     (((1<<HW_BATT_CTRL_REF_VAL_WIDTH)-1)<<HW_BATT_CTRL_REF_VAL_BITPOS)

HW_BATT_CTRL_INPUT_OFFSET_CLRMASK         equ    (~HW_BATT_CTRL_INPUT_OFFSET_SETMASK)
HW_BATT_CTRL_HALF_CMP_PWR_CLRMASK         equ    (~HW_BATT_CTRL_HALF_CMP_PWR_SETMASK)
HW_BATT_CTRL_INPUT_DIV2_CLRMASK           equ    (~HW_BATT_CTRL_INPUT_DIV2_SETMASK)
HW_BATT_CTRL_CLEAR_CLRMASK                equ    (~HW_BATT_CTRL_CLEAR_SETMASK)
HW_BATT_CTRL_PWD_CLRMASK                  equ    (~HW_BATT_CTRL_PWD_SETMASK)
HW_BATT_CTRL_CLK_DIV_CLRMASK              equ    (~HW_BATT_CTRL_CLK_DIV_SETMASK)
HW_BATT_CTRL_REF_VAL_CLRMASK              equ    (~HW_BATT_CTRL_REF_VAL_SETMASK)

;C insert
;typedef union
;{
;    struct {
;        int INPUT_OFFSET                 : HW_BATT_CTRL_INPUT_OFFSET_WIDTH;
;        int RSVD0                        : HW_BATT_CTRL_RSVD0_WIDTH;
;        int HALF_CMP_PWR                 : HW_BATT_CTRL_HALF_CMP_PWR_WIDTH;
;        int INPUT_DIV2                   : HW_BATT_CTRL_INPUT_DIV2_WIDTH;
;        int CLEAR                        : HW_BATT_CTRL_CLEAR_WIDTH;
;        int PWD                          : HW_BATT_CTRL_PWD_WIDTH;
;        int CLK_DIV                      : HW_BATT_CTRL_CLK_DIV_WIDTH;
;        int RSVD1                        : HW_BATT_CTRL_RSVD1_WIDTH;
;        int REF_VAL                      : HW_BATT_CTRL_REF_VAL_WIDTH;
;        int RSVD2                        : HW_BATT_CTRL_RSVD2_WIDTH;
;    } B;
;    int I;

;} batt_ctrl_type;

;#define HW_BATT_CTRL      (*(volatile batt_ctrl_type _X*) (HW_LRADC_BASEADDR))    /* Battery LRADC Control Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Battery LRADC Threshold Register (HW_BATT_THRSH) Bit Definitions
HW_BATT_THRSH_MIN_THRESHOLD_BITPOS          equ     (0)
HW_BATT_THRSH_MAX_THRESHOLD_BITPOS          equ     (12)

HW_BATT_THRSH_MIN_THRESHOLD_WIDTH           equ     (9)
HW_BATT_THRSH_RSVD0_WIDTH                   equ     (3)
HW_BATT_THRSH_MAX_THRESHOLD_WIDTH           equ     (9)
HW_BATT_THRSH_RSVD1_WIDTH                   equ     (3)

HW_BATT_THRSH_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_THRSH_MIN_THRESHOLD_WIDTH)-1)<<HW_BATT_THRSH_MIN_THRESHOLD_BITPOS)
HW_BATT_THRSH_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_THRSH_MAX_THRESHOLD_WIDTH)-1)<<HW_BATT_THRSH_MAX_THRESHOLD_BITPOS)

HW_BATT_THRSH_MIN_THRESHOLD_CLRMASK         equ    (~HW_BATT_THRSH_MIN_THRESHOLD_SETMASK)
HW_BATT_THRSH_MAX_THRESHOLD_CLRMASK         equ    (~HW_BATT_THRSH_MAX_THRESHOLD_SETMASK)

;C insert

;typedef union
;{
;    struct {
;        int MIN_THRESHOLD                 : HW_BATT_THRSH_MIN_THRESHOLD_WIDTH;
;        int RSVD0                        : HW_BATT_THRSH_RSVD0_WIDTH;
;        int MAX_THRESHOLD                 : HW_BATT_THRSH_MAX_THRESHOLD_WIDTH;
;        int RSVD1                        : HW_BATT_THRSH_RSVD1_WIDTH;
;    } B;
;    int I;

;} batt_thrsh_type;

;#define HW_BATT_THRSH      (*(volatile batt_thrsh_type _X*) (HW_LRADC_BASEADDR+1))    /* Battery LRADC Threshold Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  Battery LRADC Result Register (HW_BATT_RESULT) Bit Definitions
HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_BITPOS          equ     (1)
HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_BITPOS          equ     (1)
HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_BITPOS          equ     (1)
HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_BITPOS          equ     (1)
HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_BITPOS          equ     (1)
HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_BITPOS          equ     (1)
HW_BATT_RESULT_DATA_OUT_BITPOS                       equ     (8)

HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH           equ     (1)
HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH           equ     (1)
HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH           equ     (1)
HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH           equ     (1)
HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH           equ     (1)
HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH           equ     (1)
HW_BATT_RESULT_RSVD0_WIDTH                           equ     (2)
HW_BATT_RESULT_DATA_OUT_WIDTH                        equ     (9)
HW_BATT_RESULT_RSVD1_WIDTH                           equ     (7)

HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH)-1)<<HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_BITPOS)
HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH)-1)<<HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_BITPOS)
HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH)-1)<<HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_BITPOS)
HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH)-1)<<HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_BITPOS)
HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH)-1)<<HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_BITPOS)
HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH)-1)<<HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_BITPOS)
HW_BATT_RESULT_DATA_OUT_SETMASK                      equ     (((1<<HW_BATT_RESULT_DATA_OUT_WIDTH)-1)<<HW_BATT_RESULT_DATA_OUT_BITPOS)

HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_CLRMASK         equ    (~HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_SETMASK)
HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_CLRMASK         equ    (~HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_SETMASK)
HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_CLRMASK         equ    (~HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_SETMASK)
HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_CLRMASK         equ    (~HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_SETMASK)
HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_CLRMASK         equ    (~HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_SETMASK)
HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_CLRMASK         equ    (~HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_SETMASK)
HW_BATT_RESULT_DATA_OUT_CLRMASK                      equ    (~HW_BATT_RESULT_DATA_OUT_SETMASK)

;C insert

;typedef union
;{
;    struct {
;        int DATA_LT_MIN_THRESHOLD             : HW_BATT_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH;
;        int DATA_EQ_MIN_THRESHOLD             : HW_BATT_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH;
;        int DATA_GT_MIN_THRESHOLD             : HW_BATT_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH;
;        int DATA_LT_MAX_THRESHOLD             : HW_BATT_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH;
;        int DATA_EQ_MAX_THRESHOLD             : HW_BATT_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH;
;        int DATA_GT_MAX_THRESHOLD             : HW_BATT_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH;
;        int RSVD0                             : HW_BATT_RESULT_RSVD0_WIDTH;
;        int DATA_OUT                          : HW_BATT_RESULT_DATA_OUT_WIDTH;
;        int RSVD1                             : HW_BATT_RESULT_RSVD1_WIDTH;
;    } B;
;    int I;

;} batt_result_type;

;#define HW_BATT_RESULT      (*(volatile batt_result_type _X*) (HW_LRADC_BASEADDR+2))    /* Battery LRADC Result Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  LRADC1 Control Register (HW_LRADC1_CTRL) Bit Definitions
HW_LRADC1_CTRL_INPUT_OFFSET_WIDTH           equ     (7)
HW_LRADC1_CTRL_RSVD0_WIDTH                  equ     (1)
HW_LRADC1_CTRL_HALF_CMP_PWR_WIDTH           equ     (1)
HW_LRADC1_CTRL_INPUT_DIV2_WIDTH             equ     (1)
HW_LRADC1_CTRL_CLEAR_WIDTH                  equ     (1)
HW_LRADC1_CTRL_PWD_WIDTH                    equ     (1)
HW_LRADC1_CTRL_CLK_DIV_WIDTH                equ     (2)
HW_LRADC1_CTRL_RSVD1_WIDTH                  equ     (2)
HW_LRADC1_CTRL_REF_VAL_WIDTH                equ     (2)
HW_LRADC1_CTRL_RSVD2_WIDTH                  equ     (6)

HW_LRADC1_CTRL_INPUT_OFFSET_BITPOS          equ     (0)
HW_LRADC1_CTRL_HALF_CMP_PWR_BITPOS          equ     (8)
HW_LRADC1_CTRL_INPUT_DIV2_BITPOS            equ     (9)
HW_LRADC1_CTRL_CLEAR_BITPOS                 equ     (10)
HW_LRADC1_CTRL_PWD_BITPOS                   equ     (11)
HW_LRADC1_CTRL_CLK_DIV_BITPOS               equ     (12)
HW_LRADC1_CTRL_REF_VAL_BITPOS               equ     (16)

HW_LRADC1_CTRL_INPUT_OFFSET_SETMASK         equ     (((1<<HW_LRADC1_CTRL_INPUT_OFFSET_WIDTH)-1)<<HW_LRADC1_CTRL_INPUT_OFFSET_BITPOS)
HW_LRADC1_CTRL_HALF_CMP_PWR_SETMASK         equ     (((1<<HW_LRADC1_CTRL_HALF_CMP_PWR_WIDTH)-1)<<HW_LRADC1_CTRL_HALF_CMP_PWR_BITPOS)
HW_LRADC1_CTRL_INPUT_DIV2_SETMASK           equ     (((1<<HW_LRADC1_CTRL_INPUT_DIV2_WIDTH)-1)<<HW_LRADC1_CTRL_INPUT_DIV2_BITPOS)
HW_LRADC1_CTRL_CLEAR_SETMASK                equ     (((1<<HW_LRADC1_CTRL_CLEAR_WIDTH)-1)<<HW_LRADC1_CTRL_CLEAR_BITPOS)
HW_LRADC1_CTRL_PWD_SETMASK                  equ     (((1<<HW_LRADC1_CTRL_PWD_WIDTH)-1)<<HW_LRADC1_CTRL_PWD_BITPOS)
HW_LRADC1_CTRL_CLK_DIV_SETMASK              equ     (((1<<HW_LRADC1_CTRL_CLK_DIV_WIDTH)-1)<<HW_LRADC1_CTRL_CLK_DIV_BITPOS)
HW_LRADC1_CTRL_REF_VAL_SETMASK              equ     (((1<<HW_LRADC1_CTRL_REF_VAL_WIDTH)-1)<<HW_LRADC1_CTRL_REF_VAL_BITPOS)

HW_LRADC1_CTRL_INPUT_OFFSET_CLRMASK         equ    (~HW_LRADC1_CTRL_INPUT_OFFSET_SETMASK)
HW_LRADC1_CTRL_HALF_CMP_PWR_CLRMASK         equ    (~HW_LRADC1_CTRL_HALF_CMP_PWR_SETMASK)
HW_LRADC1_CTRL_INPUT_DIV2_CLRMASK           equ    (~HW_LRADC1_CTRL_INPUT_DIV2_SETMASK)
HW_LRADC1_CTRL_CLEAR_CLRMASK                equ    (~HW_LRADC1_CTRL_CLEAR_SETMASK)
HW_LRADC1_CTRL_PWD_CLRMASK                  equ    (~HW_LRADC1_CTRL_PWD_SETMASK)
HW_LRADC1_CTRL_CLK_DIV_CLRMASK              equ    (~HW_LRADC1_CTRL_CLK_DIV_SETMASK)
HW_LRADC1_CTRL_REF_VAL_CLRMASK              equ    (~HW_LRADC1_CTRL_REF_VAL_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int INPUT_OFFSET                 : HW_LRADC1_CTRL_INPUT_OFFSET_WIDTH;
;        int RSVD0                        : HW_LRADC1_CTRL_RSVD0_WIDTH;
;        int HALF_CMP_PWR                 : HW_LRADC1_CTRL_HALF_CMP_PWR_WIDTH;
;        int INPUT_DIV2                   : HW_LRADC1_CTRL_INPUT_DIV2_WIDTH;
;        int CLEAR                        : HW_LRADC1_CTRL_CLEAR_WIDTH;
;        int PWD                          : HW_LRADC1_CTRL_PWD_WIDTH;
;        int CLK_DIV                      : HW_LRADC1_CTRL_CLK_DIV_WIDTH;
;        int RSVD1                        : HW_LRADC1_CTRL_RSVD1_WIDH;
;        int REF_VAL                      : HW_LRADC1_CTRL_REF_VAL_WIDTH;
;        int RSVD2                        : HW_LRADC1_CTRL_RSVD2_WIDTH;
;    } B;
;    int I;
;} lradc1_ctrl_type;

;#define HW_LRADC1_CTRL      (*(volatile lradc1_ctrl_type _X*) (HW_LRADC_BASEADDR+3))    /* LRADC1 Control Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  LRADC1 Threshold Register (HW_LRADC1_THRSH) Bit Definitions
HW_LRADC1_THRSH_MIN_THRESHOLD_BITPOS          equ     (0)
HW_LRADC1_THRSH_MAX_THRESHOLD_BITPOS          equ     (12)

HW_LRADC1_THRSH_MIN_THRESHOLD_WIDTH           equ     (9)
HW_LRADC1_THRSH_RSVD0_WIDTH                   equ     (3)
HW_LRADC1_THRSH_MAX_THRESHOLD_WIDTH           equ     (9)
HW_LRADC1_THRSH_RSVD1_WIDTH                   equ     (3)

HW_LRADC1_THRSH_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_THRSH_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC1_THRSH_MIN_THRESHOLD_BITPOS)
HW_LRADC1_THRSH_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_THRSH_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC1_THRSH_MAX_THRESHOLD_BITPOS)

HW_LRADC1_THRSH_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_THRSH_MIN_THRESHOLD_SETMASK)
HW_LRADC1_THRSH_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_THRSH_MAX_THRESHOLD_SETMASK)

;C insert
;typedef union
;{
;    struct {
;        int MIN_THRESHOLD                 : HW_LRADC1_THRSH_MIN_THRESHOLD_WIDTH;
;        int RSVD0                         : HW_LRADC1_THRSH_RSVD0_WIDTH;
;        int MAX_THRESHOLD                 : HW_LRADC1_THRSH_MAX_THRESHOLD_WIDTH;
;        int RSVD1                         : HW_LRADC1_THRSH_RSVD1_WIDTH;
;    } B;

;    int I;

;} lradc1_thrsh_type;

;#define HW_LRADC1_THRSH      (*(volatile lradc1_thrsh_type _X*) (HW_LRADC_BASEADDR+4))    /* LRADC1 Threshold Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  LRADC1 Result Register (HW_LRADC1_RESULT) Bit Definitions
HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_BITPOS          equ     (1)
HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_BITPOS          equ     (1)
HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_BITPOS          equ     (1)
HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_BITPOS          equ     (1)
HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_BITPOS          equ     (1)
HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_BITPOS          equ     (1)
HW_LRADC1_RESULT_DATA_OUT_BITPOS                       equ     (8)

HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH           equ     (1)
HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH           equ     (1)
HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH           equ     (1)
HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH           equ     (1)
HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH           equ     (1)
HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH           equ     (1)
HW_LRADC1_RESULT_RSVD0_WIDTH                           equ     (2)
HW_LRADC1_RESULT_DATA_OUT_WIDTH                        equ     (9)
HW_LRADC1_RESULT_RSVD1_WIDTH                           equ     (7)

HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_BITPOS)
HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_BITPOS)
HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_BITPOS)
HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_BITPOS)
HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_BITPOS)
HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_BITPOS)
HW_LRADC1_RESULT_DATA_OUT_SETMASK                      equ     (((1<<HW_LRADC1_RESULT_DATA_OUT_WIDTH)-1)<<HW_LRADC1_RESULT_DATA_OUT_BITPOS)

HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_SETMASK)
HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_SETMASK)
HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_SETMASK)
HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_SETMASK)
HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_SETMASK)
HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_SETMASK)
HW_LRADC1_RESULT_DATA_OUT_CLRMASK                      equ    (~HW_LRADC1_RESULT_DATA_OUT_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int DATA_LT_MIN_THRESHOLD             : HW_LRADC1_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH;
;        int DATA_EQ_MIN_THRESHOLD             : HW_LRADC1_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH;
;        int DATA_GT_MIN_THRESHOLD             : HW_LRADC1_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH;
;        int DATA_LT_MAX_THRESHOLD             : HW_LRADC1_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH;
;        int DATA_EQ_MAX_THRESHOLD             : HW_LRADC1_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH;
;        int DATA_GT_MAX_THRESHOLD             : HW_LRADC1_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH;
;        int RSVD0                             : HW_LRADC1_RESULT_RSVD0_WIDTH;
;        int DATA_OUT                          : HW_LRADC1_RESULT_DATA_OUT_WIDTH;
;        int RSVD1                             : HW_LRADC1_RESULT_RSVD1_WIDTH;
;    } B;

;    int I;

;} lradc1_result_type;

;#define HW_LRADC1_RESULT      (*(volatile lradc1_result_type _X*) (HW_LRADC_BASEADDR+5))    /* LRADC1 Result Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  LRADC2_CTRL Control Register (HW_LRADC2_CTRL) Bit Definitions
HW_LRADC2_CTRL_INPUT_OFFSET_BITPOS          equ     (0)
HW_LRADC2_CTRL_HALF_CMP_PWR_BITPOS          equ     (8)
HW_LRADC2_CTRL_INPUT_DIV2_BITPOS            equ     (9)
HW_LRADC2_CTRL_CLEAR_BITPOS                 equ     (10)
HW_LRADC2_CTRL_PWD_BITPOS                   equ     (11)
HW_LRADC2_CTRL_CLK_DIV_BITPOS               equ     (12)
HW_LRADC2_CTRL_REF_VAL_BITPOS               equ     (16)

HW_LRADC2_CTRL_INPUT_OFFSET_WIDTH           equ     (7)
HW_LRADC2_CTRL_RSVD0_WIDTH                  equ     (1)
HW_LRADC2_CTRL_HALF_CMP_PWR_WIDTH           equ     (1)
HW_LRADC2_CTRL_INPUT_DIV2_WIDTH             equ     (1)
HW_LRADC2_CTRL_CLEAR_WIDTH                  equ     (1)
HW_LRADC2_CTRL_PWD_WIDTH                    equ     (1)
HW_LRADC2_CTRL_CLK_DIV_WIDTH                equ     (2)
HW_LRADC2_CTRL_RSVD1_WIDTH                  equ     (2)
HW_LRADC2_CTRL_REF_VAL_WIDTH                equ     (2)
HW_LRADC2_CTRL_RSVD2_WIDTH                  equ     (6)

HW_LRADC2_CTRL_INPUT_OFFSET_SETMASK         equ     (((1<<HW_LRADC2_CTRL_INPUT_OFFSET_WIDTH)-1)<<HW_LRADC2_CTRL_INPUT_OFFSET_BITPOS)
HW_LRADC2_CTRL_HALF_CMP_PWR_SETMASK         equ     (((1<<HW_LRADC2_CTRL_HALF_CMP_PWR_WIDTH)-1)<<HW_LRADC2_CTRL_HALF_CMP_PWR_BITPOS)
HW_LRADC2_CTRL_INPUT_DIV2_SETMASK           equ     (((1<<HW_LRADC2_CTRL_INPUT_DIV2_WIDTH)-1)<<HW_LRADC2_CTRL_INPUT_DIV2_BITPOS)
HW_LRADC2_CTRL_CLEAR_SETMASK                equ     (((1<<HW_LRADC2_CTRL_CLEAR_WIDTH)-1)<<HW_LRADC2_CTRL_CLEAR_BITPOS)
HW_LRADC2_CTRL_PWD_SETMASK                  equ     (((1<<HW_LRADC2_CTRL_PWD_WIDTH)-1)<<HW_LRADC2_CTRL_PWD_BITPOS)
HW_LRADC2_CTRL_CLK_DIV_SETMASK              equ     (((1<<HW_LRADC2_CTRL_CLK_DIV_WIDTH)-1)<<HW_LRADC2_CTRL_CLK_DIV_BITPOS)
HW_LRADC2_CTRL_REF_VAL_SETMASK              equ     (((1<<HW_LRADC2_CTRL_REF_VAL_WIDTH)-1)<<HW_LRADC2_CTRL_REF_VAL_BITPOS)

HW_LRADC2_CTRL_INPUT_OFFSET_CLRMASK         equ    (~HW_LRADC2_CTRL_INPUT_OFFSET_SETMASK)
HW_LRADC2_CTRL_HALF_CMP_PWR_CLRMASK         equ    (~HW_LRADC2_CTRL_HALF_CMP_PWR_SETMASK)
HW_LRADC2_CTRL_INPUT_DIV2_CLRMASK           equ    (~HW_LRADC2_CTRL_INPUT_DIV2_SETMASK)
HW_LRADC2_CTRL_CLEAR_CLRMASK                equ    (~HW_LRADC2_CTRL_CLEAR_SETMASK)
HW_LRADC2_CTRL_PWD_CLRMASK                  equ    (~HW_LRADC2_CTRL_PWD_SETMASK)
HW_LRADC2_CTRL_CLK_DIV_CLRMASK              equ    (~HW_LRADC2_CTRL_CLK_DIV_SETMASK)
HW_LRADC2_CTRL_REF_VAL_CLRMASK              equ    (~HW_LRADC2_CTRL_REF_VAL_SETMASK)

;C insert

;typedef union

;{
;    struct {
;        int INPUT_OFFSET                 : HW_LRADC2_CTRL_INPUT_OFFSET_WIDTH;
;        int RSVD0                        : HW_LRADC2_CTRL_RSVD0_WIDTH;
;        int HALF_CMP_PWR                 : HW_LRADC2_CTRL_HALF_CMP_PWR_WIDTH;
;        int INPUT_DIV2                   : HW_LRADC2_CTRL_INPUT_DIV2_WIDTH;
;        int CLEAR                        : HW_LRADC2_CTRL_CLEAR_WIDTH;
;        int PWD                          : HW_LRADC2_CTRL_PWD_WIDTH;
;        int CLK_DIV                      : HW_LRADC2_CTRL_CLK_DIV_WIDTH;
;        int RSVD1                        : HW_LRADC2_CTRL_RSVD1_WIDTH;
;        int REF_VAL                      : HW_LRADC2_CTRL_REF_VAL_WIDTH;
;        int RSVD2                        : HW_LRADC2_CTRL_RSVD2_WIDTH;
;    } B;

;    int I;

;} lradc2_ctrl_type;

;#define HW_LRADC2_CTRL      (*(volatile lradc2_ctrl_type _X*) (HW_LRADC_BASEADDR+6))    /* LRADC2_CTRL Control Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  LRADC2 Threshold Register (HW_LRADC2_THRSH) Bit Definitions
HW_LRADC2_THRSH_MIN_THRESHOLD_BITPOS          equ     (0)
HW_LRADC2_THRSH_MAX_THRESHOLD_BITPOS          equ     (12)

HW_LRADC2_THRSH_MIN_THRESHOLD_WIDTH           equ     (9)
HW_LRADC2_THRSH_RSVD0_WIDTH                   equ     (3)
HW_LRADC2_THRSH_MAX_THRESHOLD_WIDTH           equ     (9)
HW_LRADC2_THRSH_RSVD1_WIDTH                   equ     (3)

HW_LRADC2_THRSH_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_THRSH_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC2_THRSH_MIN_THRESHOLD_BITPOS)
HW_LRADC2_THRSH_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_THRSH_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC2_THRSH_MAX_THRESHOLD_BITPOS)

HW_LRADC2_THRSH_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_THRSH_MIN_THRESHOLD_SETMASK)
HW_LRADC2_THRSH_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_THRSH_MAX_THRESHOLD_SETMASK)

;C insert

;typedef union
;{
;    struct {
;        int MIN_THRESHOLD                 : HW_LRADC2_THRSH_MIN_THRESHOLD_WIDTH;
;        int RSVD0                         : HW_LRADC2_THRSH_RSVD0_WIDTH;
;        int MAX_THRESHOLD                 : HW_LRADC2_THRSH_MAX_THRESHOLD_WIDTH;
;        int RSVD1                         : HW_LRADC2_THRSH_RSVD1_WIDTH;
;    } B;

;    int I;

;} lradc2_thrsh_type;

;#define HW_LRADC2_THRSH      (*(volatile lradc2_thrsh_type _X*) (HW_LRADC_BASEADDR+7))    /* LRADC2 Threshold Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  LRADC2 Result Register (HW_LRADC2_RESULT) Bit Definitions
HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_BITPOS          equ     (1)
HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_BITPOS          equ     (1)
HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_BITPOS          equ     (1)
HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_BITPOS          equ     (1)
HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_BITPOS          equ     (1)
HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_BITPOS          equ     (1)
HW_LRADC2_RESULT_DATA_OUT_BITPOS                       equ     (8)

HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH           equ     (1)
HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH           equ     (1)
HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH           equ     (1)
HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH           equ     (1)
HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH           equ     (1)
HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH           equ     (1)
HW_LRADC2_RESULT_RSVD0_WIDTH                           equ     (2)
HW_LRADC2_RESULT_DATA_OUT_WIDTH                        equ     (9)
HW_LRADC2_RESULT_RSVD1_WIDTH                           equ     (7)

HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_BITPOS)
HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_BITPOS)
HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH)-1)<<HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_BITPOS)
HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_BITPOS)
HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_BITPOS)
HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_SETMASK         equ     (((1<<HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH)-1)<<HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_BITPOS)
HW_LRADC2_RESULT_DATA_OUT_SETMASK                      equ     (((1<<HW_LRADC2_RESULT_DATA_OUT_WIDTH)-1)<<HW_LRADC2_RESULT_DATA_OUT_BITPOS)

HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_SETMASK)
HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_SETMASK)
HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_SETMASK)
HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_SETMASK)
HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_SETMASK)
HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_CLRMASK         equ    (~HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_SETMASK)
HW_LRADC2_RESULT_DATA_OUT_CLRMASK                      equ    (~HW_LRADC2_RESULT_DATA_OUT_SETMASK)

;C insert

;typedef union
;{
;    struct {
;        int DATA_LT_MIN_THRESHOLD             : HW_LRADC2_RESULT_DATA_LT_MIN_THRESHOLD_WIDTH;
;        int DATA_EQ_MIN_THRESHOLD             : HW_LRADC2_RESULT_DATA_EQ_MIN_THRESHOLD_WIDTH;
;        int DATA_GT_MIN_THRESHOLD             : HW_LRADC2_RESULT_DATA_GT_MIN_THRESHOLD_WIDTH;
;        int DATA_LT_MAX_THRESHOLD             : HW_LRADC2_RESULT_DATA_LT_MAX_THRESHOLD_WIDTH;
;        int DATA_EQ_MAX_THRESHOLD             : HW_LRADC2_RESULT_DATA_EQ_MAX_THRESHOLD_WIDTH;
;        int DATA_GT_MAX_THRESHOLD             : HW_LRADC2_RESULT_DATA_GT_MAX_THRESHOLD_WIDTH;
;        int RSVD0                             : HW_LRADC2_RESULT_RSVD0_WIDTH;
;        int DATA_OUT                          : HW_LRADC2_RESULT_DATA_OUT_WIDTH;
;        int RSVD1                             : HW_LRADC2_RESULT_RSVD1_WIDTH;
;    } B;
;    int I;

;} lradc2_result_type;

;#define HW_LRADC2_RESULT      (*(volatile lradc2_result_type _X*) (HW_LRADC_BASEADDR+8))    /* LRADC2 Result Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  DAC Registers
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;  DAC Control Status Register (HW_DACCSR) Bit Definitions
HW_DACCSR_TXEN_BITPOS                   equ     0
HW_DACCSR_TXIEN_BITPOS                  equ     1
HW_DACCSR_TXI_BITPOS                    equ     2
HW_DACCSR_TXEXC_BITPOS                  equ     3
HW_DACCSR_LPBK_BITPOS                   equ     4
HW_DACCSR_DMASEL_BITPOS                 equ     5
HW_DACCSR_LVUP_BITPOS                   equ     8
HW_DACCSR_RVUP_BITPOS                   equ     9
HW_DACCSR_VUP_BITPOS                    equ     10
HW_DACCSR_CLKGT_BITPOS                  equ     23

HW_DACCSR_TXEN_SETMASK                  equ     1<<HW_DACCSR_TXEN_BITPOS
HW_DACCSR_TXIEN_SETMASK                 equ     1<<HW_DACCSR_TXIEN_BITPOS
HW_DACCSR_TXI_SETMASK                   equ     1<<HW_DACCSR_TXI_BITPOS
HW_DACCSR_TXEXC_SETMASK                 equ     1<<HW_DACCSR_TXEXC_BITPOS
HW_DACCSR_LPBK_SETMASK                  equ     1<<HW_DACCSR_LPBK_BITPOS
HW_DACCSR_DMASEL_SETMASK                equ     $3<<HW_DACCSR_DMASEL_BITPOS
HW_DACCSR_LVUP_SETMASK                  equ     1<<HW_DACCSR_LVUP_BITPOS
HW_DACCSR_RVUP_SETMASK                  equ     1<<HW_DACCSR_RVUP_BITPOS
HW_DACCSR_VUP_SETMASK                   equ     1<<HW_DACCSR_VUP_BITPOS
HW_DACCSR_CLKGT_SETMASK                 equ     1<<HW_DACCSR_CLKGT_BITPOS

HW_DACCSR_TXEN_CLRMASK                  equ     ~HW_DACCSR_TXEN_SETMASK
HW_DACCSR_TXIEN_CLRMASK                 equ     ~HW_DACCSR_TXIEN_SETMASK
HW_DACCSR_TXI_CLRMASK                   equ     ~HW_DACCSR_TXI_SETMASK
HW_DACCSR_TXEXC_CLRMASK                 equ     ~HW_DACCSR_TXEXC_SETMASK
HW_DACCSR_DMASEL_CLRMASK                equ     ~HW_DACCSR_DMASEL_SETMASK
HW_DACCSR_LPBK_CLRMASK                  equ     ~HW_DACCSR_LPBK_SETMASK
HW_DACCSR_LVUP_CLRMASK                  equ     ~HW_DACCSR_LVUP_SETMASK
HW_DACCSR_RVUP_CLRMASK                  equ     ~HW_DACCSR_RVUP_SETMASK
HW_DACCSR_VUP_CLRMASK                   equ     ~HW_DACCSR_VUP_SETMASK
HW_DACCSR_CLKGT_CLRMASK                 equ     ~HW_DACCSR_CLKGT_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  DAC Sample Rate Register (HW_DACSRR) Bit Definitions
HW_DACSRR_SR_BITPOS                     equ     0

HW_DACSRR_SR_SETMASK                    equ     $FFFFFF<<HW_DACSRR_SR_BITPOS

HW_DACSRR_SR_CLRMASK                    equ     ~HW_DACSRR_SR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  DAC Word Count Register (HW_DACWCR) Bit Definitions
HW_DACWCR_WCR_BITPOS                    equ     0

HW_DACWCR_WCR_SETMASK                   equ     $3FF<<HW_DACWCR_WCR_BITPOS

HW_DACWCR_WCR_CLRMASK                   equ     ~HW_DACWCR_WCR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  DAC Current Position Register (HW_DACCPR) Bit Definitions
HW_DACCPR_CPR_BITPOS                    equ     0

HW_DACCPR_CPR_SETMASK                   equ     $3FF<<HW_DACCPR_CPR_BITPOS

HW_DACCPR_CPR_CLRMASK                   equ     ~HW_DACCPR_CPR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  DAC Modulo Register (HW_DACMR) Bit Definitions
HW_DACMR_MR_BITPOS                      equ     0

HW_DACMR_MR_SETMASK                     equ     $3FF<<HW_DACMR_MR_BITPOS

HW_DACMR_MR_CLRMASK                     equ     ~HW_DACMR_MR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   DAC Base Address Register (HW_DACBAR) Bit Definitions
HW_DACBAR_BAR_BITPOS                    equ     0

HW_DACBAR_BAR_SETMASK                   equ     $FFFF<<HW_DACBAR_BAR_BITPOS

HW_DACBAR_BAR_CLRMASK                   equ     ~HW_DACBAR_BAR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   DAC Interrupt Point Register (HW_DACIPR) Bit Definitions
HW_DACIPR_IPR_BITPOS                    equ     0

HW_DACIPR_IPR_SETMASK                   equ     $3FF<<HW_DACIPR_IPR_BITPOS

HW_DACIPR_IPR_CLRMASK                   equ     ~HW_DACIPR_IPR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  ADC Registers
;///////////////////////////////////////////////////////////////////////////////
;///////////////////////////////////////////////////////////////////////////////
;  ADC Control Status Register (HW_ADCCSR) Bit Definitions
HW_ADCCSR_RXEN_BITPOS                   equ     0
HW_ADCCSR_RXIEN_BITPOS                  equ     1
HW_ADCCSR_RXI_BITPOS                    equ     2
HW_ADCCSR_RXEXC_BITPOS                  equ     3
HW_ADCCSR_LPBK_BITPOS                   equ     4
HW_ADCCSR_DMASEL_BITPOS                 equ     5
HW_ADCCSR_LPFMODE_BITPOS                equ     8
HW_ADCCSR_CLKGT_BITPOS                  equ     23

HW_ADCCSR_RXEN_SETMASK                  equ     1<<HW_ADCCSR_RXEN_BITPOS
HW_ADCCSR_RXIEN_SETMASK                 equ     1<<HW_ADCCSR_RXIEN_BITPOS
HW_ADCCSR_RXI_SETMASK                   equ     1<<HW_ADCCSR_RXI_BITPOS
HW_ADCCSR_RXEXC_SETMASK                 equ     1<<HW_ADCCSR_RXEXC_BITPOS
HW_ADCCSR_LPBK_SETMASK                  equ     1<<HW_ADCCSR_LPBK_BITPOS
HW_ADCCSR_DMASEL_SETMASK                equ     $3<<HW_ADCCSR_DMASEL_BITPOS
HW_ADCCSR_LPFMODE_SETMASK               equ     $F<<HW_ADCCSR_LPFMODE_BITPOS
HW_ADCCSR_CLKGT_SETMASK                 equ     1<<HW_ADCCSR_CLKGT_BITPOS

HW_ADCCSR_RXEN_CLRMASK                  equ     ~HW_ADCCSR_RXEN_SETMASK
HW_ADCCSR_RXIEN_CLRMASK                 equ     ~HW_ADCCSR_RXIEN_SETMASK
HW_ADCCSR_RXI_CLRMASK                   equ     ~HW_ADCCSR_RXI_SETMASK
HW_ADCCSR_RXEXC_CLRMASK                 equ     ~HW_ADCCSR_RXEXC_SETMASK
HW_ADCCSR_LPBK_CLRMASK                  equ     ~HW_ADCCSR_LPBK_SETMASK
HW_ADCCSR_DMASEL_CLRMASK                equ     ~HW_ADCCSR_DMASEL_SETMASK
HW_ADCCSR_LPFMODE_CLRMASK               equ     ~HW_ADCCSR_LPFMODE_SETMASK
HW_ADCCSR_CLKGT_CLRMASK                 equ     ~HW_ADCCSR_CLKGT_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  ADC Sample Rate Register (HW_ADCSRR) Bit Definitions
HW_ADCSRR_SR_BITPOS                     equ     0

HW_ADCSRR_SR_SETMASK                    equ     $FFFFFF<<HW_ADCSRR_SR_BITPOS

HW_ADCSRR_SR_CLRMASK                    equ     ~HW_ADCSRR_SR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  ADC Word Count Register (HW_ADCWCR) Bit Definitions
HW_ADCWCR_WCR_BITPOS                    equ     0

HW_ADCWCR_WCR_SETMASK                   equ     $3FF<<HW_ADCWCR_WCR_BITPOS

HW_ADCWCR_WCR_CLRMASK                   equ     ~HW_ADCWCR_WCR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  ADC Current Position Register (HW_ADCCPR) Bit Definitions
HW_ADCCPR_CPR_BITPOS                    equ     0

HW_ADCCPR_CPR_SETMASK                   equ     $3FF<<HW_ADCCPR_CPR_BITPOS

HW_ADCCPR_CPR_CLRMASK                   equ     ~HW_ADCCPR_CPR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;  ADC Modulo Register (HW_ADCMR) Bit Definitions
HW_ADCMR_MR_BITPOS                      equ     0

HW_ADCMR_MR_SETMASK                     equ     $3FF<<HW_ADCMR_MR_BITPOS

HW_ADCMR_MR_CLRMASK                     equ     ~HW_ADCMR_MR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   ADC Base Address Register (HW_ADCBAR) Bit Definitions
HW_ADCBAR_BAR_BITPOS                    equ     0

HW_ADCBAR_BAR_SETMASK                   equ     $FFFF<<HW_ADCBAR_BAR_BITPOS

HW_ADCBAR_BAR_CLRMASK                   equ     ~HW_ADCBAR_BAR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   ADC Interrupt Point Register (HW_ADCIPR) Bit Definitions
HW_ADCIPR_IPR_BITPOS                    equ     0

HW_ADCIPR_IPR_SETMASK                   equ     $3FF<<HW_ADCIPR_IPR_BITPOS

HW_ADCIPR_IPR_CLRMASK                   equ     ~HW_ADCIPR_IPR_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   Codec/Mixer Test Register (HW_MIXTBR) Bit Definitions
HW_MIXTBR_RESERVED_BITPOS               equ     0
HW_MIXTBR_PLL_ZERO_VT_BITPOS            equ     1
HW_MIXTBR_DIFF_2_POS_BITPOS             equ     2
HW_MIXTBR_INV_ADC_CLK_BITPOS            equ     3
HW_MIXTBR_SLOW_ADC_DITHER_BITPOS        equ     4
HW_MIXTBR_RAISE_CP_I_BITPOS             equ     5
HW_MIXTBR_LOWER_CP_I_BITPOS             equ     6
HW_MIXTBR_INV_DAC_CLK_BITPOS            equ     7
HW_MIXTBR_LOWER_XTAL_BIAS_I_BITPOS      equ     8
HW_MIXTBR_DIS_FAST_FEDGE_PSWITCH_BITPOS equ     9
HW_MIXTBR_DOUBLE_FREF_PLL_BITPOS        equ     10
HW_MIXTBR_RAISE_VCO_BITPOS              equ     11
HW_MIXTBR_DIS_XTAL_IBIAS_BITPOS         equ     12
HW_MIXTBR_EN_BG_IBIAS_BITPOS            equ     13
HW_MIXTBR_DIS_ADC_DITHER_BITPOS         equ     14
HW_MIXTBR_MICBIAS_LSBITPOS              equ     15
HW_MIXTBR_PW_ADC_RIGHT_CH_BITPOS        equ     16
HW_MIXTBR_MICBIAS_MSBITPOS              equ     17
HW_MIXTBR_EN_ZERO_DET_BITPOS            equ     18
HW_MIXTBR_UPDATE_DA_VOL_BITPOS          equ     19
HW_MIXTBR_UPDATE_FM_VOL_BITPOS          equ     20
HW_MIXTBR_UPDATE_LINE_VOL_BITPOS        equ     21
HW_MIXTBR_UPDATE_MIC_VOL_BITPOS         equ     22
HW_MIXTBR_UPDATE_MSTR_VOL_BITPOS        equ     23

HW_MIXTBR_RESERVED_SETMASK              equ     1<<HW_MIXTBR_RESERVED_BITPOS
HW_MIXTBR_PLL_ZERO_VT_SETMASK           equ     1<<HW_MIXTBR_PLL_ZERO_VT_BITPOS
HW_MIXTBR_DIFF_2_POS_SETMASK            equ     1<<HW_MIXTBR_DIFF_2_POS_BITPOS
HW_MIXTBR_INV_ADC_CLK_BITPOS_SETMASK    equ     1<<HW_MIXTBR_INV_ADC_CLK_BITPOS
HW_MIXTBR_SLOW_ADC_DITHER_SETMASK       equ     1<<HW_MIXTBR_SLOW_ADC_DITHER_BITPOS
HW_MIXTBR_RAISE_CP_I_SETMASK            equ     1<<HW_MIXTBR_RAISE_CP_I_BITPOS
HW_MIXTBR_LOWER_CP_I_SETMASK            equ     1<<HW_MIXTBR_LOWER_CP_I_BITPOS
HW_MIXTBR_INV_DAC_CLK_SETMASK           equ     1<<HW_MIXTBR_INV_DAC_CLK_BITPOS
HW_MIXTBR_LOWER_XTAL_BIAS_I_SETMASK     equ     1<<HW_MIXTBR_LOWER_XTAL_BIAS_I_BITPOS
HW_MIXTBR_DIS_FAST_FEDGE_PSWITCH_SETMASK equ    1<<HW_MIXTBR_DIS_FAST_FEDGE_PSWITCH_BITPOS
HW_MIXTBR_DOUBLE_FREF_PLL_SETMASK       equ     1<<HW_MIXTBR_DOUBLE_FREF_PLL_BITPOS
HW_MIXTBR_RAISE_VCO_SETMASK             equ     1<<HW_MIXTBR_RAISE_VCO_BITPOS
HW_MIXTBR_DIS_XTAL_IBIAS_SETMASK        equ     1<<HW_MIXTBR_DIS_XTAL_IBIAS_BITPOS
HW_MIXTBR_EN_BG_IBIAS_SETMASK           equ     1<<HW_MIXTBR_EN_BG_IBIAS_BITPOS
HW_MIXTBR_DIS_ADC_DITHER_SETMASK        equ     1<<HW_MIXTBR_DIS_ADC_DITHER_BITPOS
HW_MIXTBR_MICBIAS_LSB_SETMASK           equ     1<<HW_MIXTBR_MICBIAS_LSBITPOS
HW_MIXTBR_MICBIAS_MSB_SETMASK           equ     1<<HW_MIXTBR_MICBIAS_MSBITPOS
HW_MIXTBR_PW_ADC_RIGHT_CH_SETMASK       equ     1<<HW_MIXTBR_PW_ADC_RIGHT_CH_BITPOS
HW_MIXTBR_EN_ZERO_DET_SETMASK           equ     1<<HW_MIXTBR_EN_ZERO_DET_BITPOS
HW_MIXTBR_UPDATE_DA_VOL_SETMASK         equ     1<<HW_MIXTBR_UPDATE_DA_VOL_BITPOS
HW_MIXTBR_UPDATE_FM_VOL_SETMASK         equ     1<<HW_MIXTBR_UPDATE_FM_VOL_BITPOS
HW_MIXTBR_UPDATE_LINE_VOL_SETMASK       equ     1<<HW_MIXTBR_UPDATE_LINE_VOL_BITPOS
HW_MIXTBR_UPDATE_MIC_VOL_SETMASK        equ     1<<HW_MIXTBR_UPDATE_MIC_VOL_BITPOS
HW_MIXTBR_UPDATE_MSTR_VOL_SETMASK       equ     1<<HW_MIXTBR_UPDATE_MSTR_VOL_BITPOS

HW_MIXTBR_RESERVED_CLRMASK              equ     ~HW_MIXTBR_RESERVED_SETMASK
HW_MIXTBR_PLL_ZERO_VT_CLRMASK           equ     ~HW_MIXTBR_PLL_ZERO_VT_SETMASK
HW_MIXTBR_DIFF_2_POS_CLRMASK            equ     ~HW_MIXTBR_DIFF_2_POS_SETMASK
HW_MIXTBR_INV_ADC_CLK_BITPOS_CLRMASK    equ     ~HW_MIXTBR_INV_ADC_CLK_BITPOS_SETMASK
HW_MIXTBR_SLOW_ADC_DITHER_CLRMASK       equ     ~HW_MIXTBR_SLOW_ADC_DITHER_SETMASK
HW_MIXTBR_RAISE_CP_I_CLRMASK            equ     ~HW_MIXTBR_RAISE_CP_I_SETMASK
HW_MIXTBR_LOWER_CP_I_CLRMASK            equ     ~HW_MIXTBR_LOWER_CP_I_SETMASK
HW_MIXTBR_INV_DAC_CLK_CLRMASK           equ     ~HW_MIXTBR_INV_DAC_CLK_SETMASK
HW_MIXTBR_LOWER_XTAL_BIAS_I_CLRMASK     equ     ~HW_MIXTBR_LOWER_XTAL_BIAS_I_SETMASK
HW_MIXTBR_DIS_FAST_FEDGE_PSWITCH_CLRMASK equ    ~HW_MIXTBR_DIS_FAST_FEDGE_PSWITCH_SETMASK
HW_MIXTBR_DOUBLE_FREF_PLL_CLRMASK       equ     ~HW_MIXTBR_DOUBLE_FREF_PLL_SETMASK
HW_MIXTBR_RAISE_VCO_CLRMASK             equ     ~HW_MIXTBR_RAISE_VCO_SETMASK
HW_MIXTBR_DIS_XTAL_IBIAS_CLRMASK        equ     ~HW_MIXTBR_DIS_XTAL_IBIAS_SETMASK
HW_MIXTBR_EN_BG_IBIAS_CLRMASK           equ     ~HW_MIXTBR_EN_BG_IBIAS_SETMASK
HW_MIXTBR_DIS_ADC_DITHER_CLRMASK        equ     ~HW_MIXTBR_DIS_ADC_DITHER_SETMASK
HW_MIXTBR_MICBIAS_LSB_CLRMASK           equ     ~HW_MIXTBR_MICBIAS_LSB_SETMASK
HW_MIXTBR_MICBIAS_MSB_CLRMASK           equ     ~HW_MIXTBR_MICBIAS_MSB_SETMASK
HW_MIXTBR_PW_ADC_RIGHT_CH_CLRMASK       equ     ~HW_MIXTBR_PW_ADC_RIGHT_CH_SETMASK
HW_MIXTBR_EN_ZERO_DET_CLRMASK           equ     ~HW_MIXTBR_EN_ZERO_DET_SETMASK
HW_MIXTBR_UPDATE_DA_VOL_CLRMASK         equ     ~HW_MIXTBR_UPDATE_DA_VOL_SETMASK
HW_MIXTBR_UPDATE_FM_VOL_CLRMASK         equ     ~HW_MIXTBR_UPDATE_FM_VOL_SETMASK
HW_MIXTBR_UPDATE_LINE_VOL_CLRMASK       equ     ~HW_MIXTBR_UPDATE_LINE_VOL_SETMASK
HW_MIXTBR_UPDATE_MIC_VOL_CLRMASK        equ     ~HW_MIXTBR_UPDATE_MIC_VOL_SETMASK
HW_MIXTBR_UPDATE_MSTR_VOL_CLRMASK       equ     ~HW_MIXTBR_UPDATE_MSTR_VOL_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   Generic Volume Register (HW_MIXMASTERVR) Bit Definitions
HW_MIXVOLUMER_MR_BITPOS                 equ     0
HW_MIXVOLUMER_ML_BITPOS                 equ     8
HW_MIXVOLUMER_MUTE_BITPOS               equ     15

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

HW_MIXMASTERVR_MR_SETMASK               equ      $1F<<HW_MIXMASTERVR_MR_BITPOS
HW_MIXMASTERVR_ML_SETMASK               equ      $1F<<HW_MIXMASTERVR_ML_BITPOS
HW_MIXMASTERVR_MUTE_SETMASK             equ      1<<HW_MIXMASTERVR_MUTE_BITPOS

HW_MIXMASTERVR_MR_CLRMASK               equ      ~HW_MIXMASTERVR_MR_SETMASK
HW_MIXMASTERVR_ML_CLRMASK               equ      ~HW_MIXMASTERVR_ML_SETMASK
HW_MIXMASTERVR_MUTE_CLRMASK             equ      ~HW_MIXMASTERVR_MUTE_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   Mixer Mic In Volume Register (HW_MIXMICINVR) Bit Definitions
HW_MIXMICINVR_GN_BITPOS                 equ       0
HW_MIXMICINVR_P20DB_BITPOS              equ       6
HW_MIXMICINVR_MUTE_BITPOS               equ       15

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


;///////////////////////////////////////////////////////////////////////////////
;   Mixer Line1 In Volume Register (HW_MIXLINE1INVR) Bit Definitions
HW_MIXLINE1INVR_MR_BITPOS               equ     0
HW_MIXLINE1INVR_ML_BITPOS               equ     8
HW_MIXLINE1INVR_MUTE_BITPOS             equ     15

HW_MIXLINE1INVR_MR_SETMASK              equ     $1F<<HW_MIXLINE1INVR_MR_BITPOS
HW_MIXLINE1INVR_ML_SETMASK              equ     $1F<<HW_MIXLINE1INVR_ML_BITPOS
HW_MIXLINE1INVR_MUTE_SETMASK            equ     1<<HW_MIXLINE1INVR_MUTE_BITPOS

HW_MIXLINE1INVR_MR_CLRMASK              equ     ~HW_MIXLINE1INVR_MR_SETMASK
HW_MIXLINE1INVR_ML_CLRMASK              equ     ~HW_MIXLINE1INVR_ML_SETMASK
HW_MIXLINE1INVR_MUTE_CLRMASK            equ     ~HW_MIXLINE1INVR_MUTE_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   Mixer Line2 In Volume Register (HW_MIXLINE2INVR) Bit Definitions
HW_MIXLINE2INVR_MR_BITPOS               equ     0
HW_MIXLINE2INVR_ML_BITPOS               equ     8
HW_MIXLINE2INVR_MUTE_BITPOS             equ     15

HW_MIXLINE2INVR_MR_SETMASK              equ     $1F<<HW_MIXLINE2INVR_MR_BITPOS
HW_MIXLINE2INVR_ML_SETMASK              equ     $1F<<HW_MIXLINE2INVR_ML_BITPOS
HW_MIXLINE2INVR_MUTE_SETMASK            equ     1<<HW_MIXLINE2INVR_MUTE_BITPOS

HW_MIXLINE2INVR_MR_CLRMASK              equ     ~HW_MIXLINE2INVR_MR_SETMASK
HW_MIXLINE2INVR_ML_CLRMASK              equ     ~HW_MIXLINE2INVR_ML_SETMASK
HW_MIXLINE2INVR_MUTE_CLRMASK            equ     ~HW_MIXLINE2INVR_MUTE_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   Mixer DAC In Volume Register (HW_MIXDACINVR) Bit Definitions
HW_MIXDACINVR_MR_BITPOS                 equ     0
HW_MIXDACINVR_ML_BITPOS                 equ     8
HW_MIXDACINVR_MUTE_BITPOS               equ     15

HW_MIXDACINVR_MR_SETMASK                equ     $1F<<HW_MIXDACINVR_MR_BITPOS
HW_MIXDACINVR_ML_SETMASK                equ     $1F<<HW_MIXDACINVR_ML_BITPOS
HW_MIXDACINVR_MUTE_SETMASK              equ     1<<HW_MIXDACINVR_MUTE_BITPOS

HW_MIXDACINVR_MR_CLRMASK                equ     ~HW_MIXDACINVR_MR_SETMASK
HW_MIXDACINVR_ML_CLRMASK                equ     ~HW_MIXDACINVR_ML_SETMASK
HW_MIXDACINVR_MUTE_CLRMASK              equ     ~HW_MIXDACINVR_MUTE_SETMASK


;///////////////////////////////////////////////////////////////////////////////
;   Mixer Record Select Register (HW_MIXRECSELR) Bit Definitions
HW_MIXRECSELR_SR_BITPOS                 equ     0
HW_MIXRECSELR_SL_BITPOS                 equ     8
HW_MIXRECSELR_X_BITPOS                  equ     11

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


;///////////////////////////////////////////////////////////////////////////////
;   Mixer ADC In Gain Register (HW_MIXADCGAINR) Bit Definitions
HW_MIXADCGAINR_GR_BITPOS                equ     0
HW_MIXADCGAINR_GL_BITPOS                equ     8
HW_MIXADCGAINR_MUTE_BITPOS              equ     15

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


;///////////////////////////////////////////////////////////////////////////////
;   Mixer Power Down Register (HW_MIXPWRDNR) Bit Definitions
HW_MIXPWRDNR_PR_BITPOS                  equ     9
HW_MIXPWRDNR_PR1_BITPOS                 equ     10
HW_MIXPWRDNR_PR2_BITPOS                 equ     11

HW_MIXPWRDNR_PR_SETMASK                 equ     7<<HW_MIXPWRDNR_PR_BITPOS

HW_MIXPWRDNR_PR_CLRMASK                 equ     ~HW_MIXPWRDNR_PR_SETMASK

  endif















