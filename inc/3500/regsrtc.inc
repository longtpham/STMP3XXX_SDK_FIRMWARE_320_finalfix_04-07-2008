;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2004
;
; Filename: regsrtc.inc
; Description: Register definitions for RTC interface
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
    if !(@def(regsrtcinc))
regsrtcinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   RTC STMP Registers 
;   Edited 2/26/2002 J. Ferrara
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_RTC_BASEADDR             equ     ($F500)

 

;ASM Only

 
HW_RTC_CSR                   equ     (HW_RTC_BASEADDR+0)
HW_RTC_MSECONDS0             equ     (HW_RTC_BASEADDR+1)
HW_RTC_MSECONDS1             equ     (HW_RTC_BASEADDR+2)
HW_RTC_WATCHDOG              equ     (HW_RTC_BASEADDR+3)
HW_RTC_ALARM0                equ     (HW_RTC_BASEADDR+4)
HW_RTC_ALARM1                equ     (HW_RTC_BASEADDR+5)
HW_RTC_DIVIDE                equ     (HW_RTC_BASEADDR+6)
HW_RTC_PERSIST0              equ     (HW_RTC_BASEADDR+7)
HW_RTC_PERSIST1              equ     (HW_RTC_BASEADDR+8)

;endasm
 

;///////////////////////////////////////////////////////////////////////////////

;//  RTC Control Status Register (HW_RTC_CSR) Bit Definitions

HW_RTC_CSR_ALARMINTEN_BITPOS            equ     (0)
HW_RTC_CSR_ALARMINT_BITPOS              equ     (1)
HW_RTC_CSR_WATCHDOGEN_BITPOS            equ     (2)
HW_RTC_CSR_RSVD0_BITPOS                 equ     (3)
HW_RTC_CSR_STALEREGS_BITPOS             equ     (4)
HW_RTC_CSR_RSVD1_BITPOS                 equ     (9)
HW_RTC_CSR_NEWREGS_BITPOS				equ		(12)
HW_RTC_CSR_NEWREGS_XTALDIV_BITPOS       equ     (12)
HW_RTC_CSR_NEWREGS_MSCOUNT_BITPOS		equ		(13)
HW_RTC_CSR_NEWREGS_ALRMCOUNT_BITPOS		equ		(14)
HW_RTC_CSR_NEWREGS_PERSIST0_BITPOS		equ		(15)
HW_RTC_CSR_NEWREGS_PERSIST1_BITPOS		equ 	(16)
HW_RTC_CSR_RSVD2_BITPOS                 equ     (17)
HW_RTC_CSR_FORCE_BITPOS                 equ     (20)
HW_RTC_CSR_RSVD3_BITPOS                 equ     (21)
HW_RTC_CSR_SFTRST_BITPOS                equ     (23)										
HW_RTC_CSR_ALARMINTEN_WIDTH             equ     (1)
HW_RTC_CSR_ALARMINT_WIDTH               equ     (1)
HW_RTC_CSR_WATCHDOGEN_WIDTH             equ     (1)
HW_RTC_CSR_RSVD0_WIDTH                  equ     (1)
HW_RTC_CSR_STALEREGS_WIDTH              equ     (5)
HW_RTC_CSR_RSVD1_WIDTH                  equ     (3)
HW_RTC_CSR_NEWREGS_WIDTH				equ		(5)
HW_RTC_CSR_NEWREGS_XTALDIV_WIDTH        equ     (1)
HW_RTC_CSR_NEWREGS_MSCOUNT_WIDTH		equ		(1)
HW_RTC_CSR_NEWREGS_ALRMCOUNT_WIDTH		equ		(1)
HW_RTC_CSR_NEWREGS_PERSIST0_WIDTH		equ		(1)
HW_RTC_CSR_NEWREGS_PERSIST1_WIDTH		equ 	(1)
HW_RTC_CSR_RSVD2_WIDTH                  equ     (3)
HW_RTC_CSR_FORCE_WIDTH                  equ     (1)
HW_RTC_CSR_RSVD3_WIDTH                  equ     (2)
HW_RTC_CSR_SFTRST_WIDTH                 equ     (1)

HW_RTC_CSR_ALARMINT_SETMASK             equ     (((1<<HW_RTC_CSR_ALARMINT_WIDTH)-1)<<HW_RTC_CSR_ALARMINT_BITPOS) 
HW_RTC_CSR_ALARMINTEN_SETMASK           equ     (((1<<HW_RTC_CSR_ALARMINTEN_WIDTH)-1)<<HW_RTC_CSR_ALARMINTEN_BITPOS) 
HW_RTC_CSR_WATCHDOGEN_SETMASK           equ     (((1<<HW_RTC_CSR_WATCHDOGEN_WIDTH)-1)<<HW_RTC_CSR_WATCHDOGEN_BITPOS) 
HW_RTC_CSR_RSVD0_SETMASK                equ     (((1<<HW_RTC_CSR_RSVD0_WIDTH)-1)<<HW_RTC_CSR_RSVD0_BITPOS) 
HW_RTC_CSR_STALEREGS_SETMASK            equ     (((1<<HW_RTC_CSR_STALEREGS_WIDTH)-1)<<HW_RTC_CSR_STALEREGS_BITPOS) 
HW_RTC_CSR_RSVD1_SETMASK                equ     (((1<<HW_RTC_CSR_RSVD1_WIDTH)-1)<<HW_RTC_CSR_RSVD1_BITPOS) 
HW_RTC_CSR_NEWREGS_SETMASK              equ     (((1<<HW_RTC_CSR_NEWREGS_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_BITPOS) 
HW_RTC_CSR_NEWREGS_XTALDIV_SETMASK      equ		(((1<<HW_RTC_CSR_NEWREGS_XTALDIV_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_XTALDIV_BITPOS)   
HW_RTC_CSR_NEWREGS_MSCOUNT_SETMASK		equ		(((1<<HW_RTC_CSR_NEWREGS_MSCOUNT_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_MSCOUNT_BITPOS)
HW_RTC_CSR_NEWREGS_ALRMCOUNT_SETMASK	equ		(((1<<HW_RTC_CSR_NEWREGS_ALRMCOUNT_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_ALRMCOUNT_BITPOS)
HW_RTC_CSR_NEWREGS_PERSIST0_SETMASK		equ		(((1<<HW_RTC_CSR_NEWREGS_PERSIST0_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_PERSIST0_BITPOS)
HW_RTC_CSR_NEWREGS_PERSIST1_SETMASK		equ 	(((1<<HW_RTC_CSR_NEWREGS_PERSIST1_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_PERSIST1_BITPOS)
HW_RTC_CSR_RSVD2_SETMASK                equ     (((1<<HW_RTC_CSR_RSVD2_WIDTH)-1)<<HW_RTC_CSR_RSVD2_BITPOS) 
HW_RTC_CSR_FORCE_SETMASK                equ     (((1<<HW_RTC_CSR_FORCE_WIDTH)-1)<<HW_RTC_CSR_FORCE_BITPOS) 
HW_RTC_CSR_RSVD3_SETMASK                equ     (((1<<HW_RTC_CSR_RSVD3_WIDTH)-1)<<HW_RTC_CSR_RSVD3_BITPOS) 
HW_RTC_CSR_SFTRST_SETMASK               equ     (((1<<HW_RTC_CSR_SFTRST_WIDTH)-1)<<HW_RTC_CSR_SFTRST_BITPOS) 

HW_RTC_CSR_ALARMINTEN_CLRMASK            equ    (~HW_RTC_CSR_ALARMINTEN_SETMASK)
HW_RTC_CSR_ALARMINT_CLRMASK              equ    (~HW_RTC_CSR_ALARMINT_SETMASK)
HW_RTC_CSR_WATCHDOGEN_CLRMASK            equ    (~HW_RTC_CSR_WATCHDOGEN_SETMASK)
HW_RTC_CSR_RSVD0_CLRMASK                 equ    (~HW_RTC_CSR_RSVD0_SETMASK)
HW_RTC_CSR_STALEREGS_CLRMASK             equ    (~HW_RTC_CSR_STALEREGS_SETMASK)
HW_RTC_CSR_RSVD1_CLRMASK                 equ    (~HW_RTC_CSR_RSVD1_SETMASK)
HW_RTC_CSR_NEWREGS_CLRMASK               equ    (~HW_RTC_CSR_NEWREGS_SETMASK)
HW_RTC_CSR_NEWREGS_XTALDIV_CLRMASK       equ	(~HW_RTC_CSR_NEWREGS_XTALDIV_SETMASK)   
HW_RTC_CSR_NEWREGS_MSCOUNT_CLRMASK		 equ	(~HW_RTC_CSR_NEWREGS_MSCOUNT_SETMASK)
HW_RTC_CSR_NEWREGS_ALRMCOUNT_CLRMASK	 equ	(~HW_RTC_CSR_NEWREGS_ALRMCOUNT_SETMASK)
HW_RTC_CSR_NEWREGS_PERSIST0_CLRMASK		 equ	(~HW_RTC_CSR_NEWREGS_PERSIST0_SETMASK)
HW_RTC_CSR_NEWREGS_PERSIST1_CLRMASK		 equ 	(~HW_RTC_CSR_NEWREGS_PERSIST1_SETMASK)

HW_RTC_CSR_RSVD2_CLRMASK                 equ     (~HW_RTC_CSR_RSVD2_SETMASK)
HW_RTC_CSR_FORCE_CLRMASK                 equ     (~HW_RTC_CSR_FORCE_SETMASK)
HW_RTC_CSR_RSVD3_CLRMASK                 equ     (~HW_RTC_CSR_RSVD3_SETMASK)
HW_RTC_CSR_SFTRST_CLRMASK                equ     (~HW_RTC_CSR_SFTRST_SETMASK)


;C insert

;typedef union               

;{

;    struct {
;	 int ALARMINTEN 	: HW_RTC_CSR_ALARMINTEN_WIDTH;
;	 int ALARMINT           : HW_RTC_CSR_ALARMINT_WIDTH;
;	 int WATCHDOGEN         : HW_RTC_CSR_WATCHDOGEN_WIDTH;
;	 int RSVD0              : HW_RTC_CSR_RSVD0_WIDTH;
;	 unsigned int STALEREGS : HW_RTC_CSR_STALEREGS_WIDTH;
;	 int RSVD1              : HW_RTC_CSR_RSVD1_WIDTH;
;	 unsigned int NEWREGS   : HW_RTC_CSR_NEWREGS_WIDTH;
;	 int RSVD2              : HW_RTC_CSR_RSVD2_WIDTH;
;	 int FORCE             : HW_RTC_CSR_FORCE_WIDTH;
;	 int RSVD3              : HW_RTC_CSR_RSVD3_WIDTH;
;	 unsigned int SFTRST    : HW_RTC_CSR_SFTRST_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_csr_type;

 

;#define HW_RTC_CSR      (*(volatile rtc_csr_type _X*) (HW_RTC_BASEADDR+0))    /* RTC Control / Status Register */

;endc

;///////////////////////////////////////////////////////////////////////////////

;  RTC Milliseconds Register0 (HW_RTC_MSECONDS0) Bit Definitions

HW_RTC_MSECONDS0_MSECONDSLOW_BITPOS      equ     (0)

HW_RTC_MSECONDS0_MSECONDSLOW_WIDTH       equ     (24)

HW_RTC_MSECONDS0_MSECONDSLOW_SETMASK     equ     (((1<<HW_RTC_MSECONDS0_MSECONDSLOW_WIDTH)-1)<<HW_RTC_MSECONDS0_MSECONDSLOW_BITPOS) 

HW_RTC_MSECONDS0_MSECONDSLOW_CLRMASK     equ     (~HW_RTC_MSECONDS0_MSECONDSLOW_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int MSECONDSLOW 	: HW_RTC_MSECONDS0_MSECONDSLOW_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_mseconds0_type;

 

;#define HW_RTC_MSECONDS0      (*(volatile rtc_mseconds0_type _X*) (HW_RTC_BASEADDR+1))    /* RTC MSECONDS Lower Word Register */
;#define HW_RTCLOWR            HW_RTC_MSECONDS0
;endc
;///////////////////////////////////////////////////////////////////////////////

;  RTC Milliseconds Register1 (HW_RTC_MSECONDS1) Bit Definitions

HW_RTC_MSECONDS1_MSECONDSHIGH_BITPOS      equ     (0)

HW_RTC_MSECONDS1_MSECONDSHIGH_WIDTH       equ     (24)

HW_RTC_MSECONDS1_MSECONDSHIGH_SETMASK     equ     (((1<<HW_RTC_MSECONDS1_MSECONDSHIGH_WIDTH)-1)<<HW_RTC_MSECONDS1_MSECONDSHIGH_BITPOS) 

HW_RTC_MSECONDS1_MSECONDSHIGH_CLRMASK     equ     (~HW_RTC_MSECONDS1_MSECONDSHIGH_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int MSECONDSHIGH 	: HW_RTC_MSECONDS1_MSECONDSHIGH_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_mseconds1_type;
 

;#define HW_RTC_MSECONDS1      (*(volatile rtc_mseconds1_type _X*) (HW_RTC_BASEADDR+2))    /* RTC MSECONDS Upper Word Register */
;#define HW_RTCUPR             HW_RTC_MSECONDS1
;endc

HW_RTC_LOWER_OFFSET                     equ     1
HW_RTC_UP_OFFSET                        equ     2

;ASM only
HW_RTCUPR                               equ     HW_RTC_BASEADDR+HW_RTC_UP_OFFSET
HW_RTCLOWR                              equ     HW_RTC_BASEADDR+HW_RTC_LOWER_OFFSET
HW_RTCUP                                equ     HW_RTCUPR
HW_RTCLOW                               equ     HW_RTCLOWR

HW_RTC_UPPER_SETMASK                    equ     $03ffff
HW_RTC_UPPER_CLRMASK                    equ     ~HW_RTC_UPPER_SETMASK
;endasm

;///////////////////////////////////////////////////////////////////////////////

;  RTC Watchdog Count (HW_RTC_WATCHDOG) Bit Definitions

HW_RTC_WATCHDOG_TIMEOUTCNT_BITPOS      equ     (0)

HW_RTC_WATCHDOG_TIMEOUTCNT_WIDTH       equ     (24)

HW_RTC_WATCHDOG_TIMEOUTCNT_SETMASK     equ     (((1<<HW_RTC_WATCHDOG_TIMEOUTCNT_WIDTH)-1)<<HW_RTC_WATCHDOG_TIMEOUTCNT_BITPOS) 

HW_RTC_WATCHDOG_TIMEOUTCNT_CLRMASK     equ     (~HW_RTC_WATCHDOG_TIMEOUTCNT_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int TIMEOUTCNT 	: HW_RTC_WATCHDOG_TIMEOUTCNT_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_watchdog_type;
 

;#define HW_RTC_WATCHDOG     (*(volatile rtc_watchdog_type _X*) (HW_RTC_BASEADDR+3))    /* RTC Watchdog Timeout Register */
;endc
;///////////////////////////////////////////////////////////////////////////////

;  RTC Alarm Register0 (HW_RTC_ALARM0) Bit Definitions

HW_RTC_ALARM0_ALARMLOW_BITPOS      equ     (0)

HW_RTC_ALARM0_ALARMLOW_WIDTH       equ     (24)

HW_RTC_ALARM0_ALARMLOW_SETMASK     equ     (((1<<HW_RTC_ALARM0_ALARMLOW_WIDTH)-1)<<HW_RTC_ALARM0_ALARMLOW_BITPOS) 

HW_RTC_ALARM0_ALARMLOW_CLRMASK     equ     (~HW_RTC_ALARM0_ALARMLOW_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int ALARMLOW 	: HW_RTC_ALARM0_ALARMLOW_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_alarm0_type;

 

;#define HW_RTC_ALARM0      (*(volatile rtc_alarm0_type _X*) (HW_RTC_BASEADDR+4))    /* RTC ALARM Lower Word Register */
;endc
;///////////////////////////////////////////////////////////////////////////////

;  RTC Alarm Register1 (HW_RTC_ALARM1) Bit Definitions

HW_RTC_ALARM1_ALARMHIGH_BITPOS      equ     (0)

HW_RTC_ALARM1_ALARMHIGH_WIDTH       equ     (24)

HW_RTC_ALARM1_ALARMHIGH_SETMASK     equ     (((1<<HW_RTC_ALARM1_ALARMHIGH_WIDTH)-1)<<HW_RTC_ALARM1_ALARMHIGH_BITPOS) 

HW_RTC_ALARM1_ALARMHIGH_CLRMASK     equ     (~HW_RTC_ALARM1_ALARMHIGH_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int ALARMHIGH 	: HW_RTC_ALARM1_ALARMHIGH_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_alarm1_type;
 

;#define HW_RTC_ALARM1      (*(volatile rtc_alarm1_type _X*) (HW_RTC_BASEADDR+5))    /* RTC ALARM Upper Word Register */

;endc
;///////////////////////////////////////////////////////////////////////////////

;  RTC Xtal-Clock Divide Register (HW_RTC_XTALDIVIDE) Bit Definitions

HW_RTC_XTALDIVIDE_DIVCOUNT_BITPOS      equ     (0)
HW_RTC_XTALDIVIDE_RSVD0_BITPOS         equ     (16)

HW_RTC_XTALDIVIDE_DIVCOUNT_WIDTH       equ     (16)
HW_RTC_XTALDIVIDE_RSVD0_WIDTH          equ     (8)

HW_RTC_XTALDIVIDE_DIVCOUNT_SETMASK     equ     (((1<<HW_RTC_XTALDIVIDE_DIVCOUNT_WIDTH)-1)<<HW_RTC_XTALDIVIDE_DIVCOUNT_BITPOS) 
HW_RTC_XTALDIVIDE_RSVD0_SETMASK        equ     (((1<<HW_RTC_XTALDIVIDE_RSVD0_BITPOS)-1)<<HW_RTC_XTALDIVIDE_RSVD0_BITPOS) 

HW_RTC_XTALDIVIDE_DIVCOUNT_CLRMASK     equ     (~HW_RTC_XTALDIVIDE_DIVCOUNT_SETMASK)
HW_RTC_XTALDIVIDE_RSVD0_CLRMASK        equ     (~HW_RTC_XTALDIVIDE_RSVD0_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int DIVCOUNT 	: HW_RTC_XTALDIVIDE_DIVCOUNT_WIDTH;
;	 int RSVD0 	: HW_RTC_XTALDIVIDE_RSVD0_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_xtaldivide_type;
 

;#define HW_RTC_XTALDIVIDE   (*(volatile rtc_xtaldivide_type _X*) (HW_RTC_BASEADDR+6))  /* RTC Xtal-clock Pre-Divider Word Register */
;endc
;///////////////////////////////////////////////////////////////////////////////

;  RTC PERSISTENT Register0 (HW_RTC_PERSISTENT0) Bit Definitions

HW_RTC_PERSISTENT0_ALARMEN_BITPOS        equ     (0)
HW_RTC_PERSISTENT0_ALARMWAKE_EN_BITPOS	 equ	 (1)
HW_RTC_PERSISTENT0_ALARMWAKE_BITPOS      equ     (2)
HW_RTC_PERSISTENT0_XTALPDOWN_BITPOS      equ     (3)
HW_RTC_PERSISTENT0_RSVD0_BITPOS          equ     (4)

HW_RTC_PERSISTENT0_ALARMEN_WIDTH         equ     (1)
HW_RTC_PERSISTENT0_ALARMWAKE_EN_WIDTH	 equ	 (1)
HW_RTC_PERSISTENT0_ALARMWAKE_WIDTH       equ     (1)
HW_RTC_PERSISTENT0_XTALPDOWN_WIDTH       equ     (1)
HW_RTC_PERSISTENT0_RSVD0_WIDTH           equ     (21)

HW_RTC_PERSISTENT0_ALARMEN_SETMASK       equ     (((1<<HW_RTC_PERSISTENT0_ALARMEN_WIDTH)-1)<<HW_RTC_PERSISTENT0_ALARMEN_BITPOS) 
HW_RTC_PERSISTENT0_ALARMWAKE_EN_SETMASK	 equ	 (((1<<HW_RTC_PERSISTENT0_ALARMWAKE_EN_WIDTH)-1)<<HW_RTC_PERSISTENT0_ALARMWAKE_EN_BITPOS)
HW_RTC_PERSISTENT0_ALARMWAKE_SETMASK     equ     (((1<<HW_RTC_PERSISTENT0_ALARMWAKE_WIDTH)-1)<<HW_RTC_PERSISTENT0_ALARMWAKE_BITPOS) 
HW_RTC_PERSISTENT0_XTALPDOWN_SETMASK     equ     (((1<<HW_RTC_PERSISTENT0_XTALPDOWN_WIDTH)-1)<<HW_RTC_PERSISTENT0_XTALPDOWN_BITPOS) 
HW_RTC_PERSISTENT0_RSVD0_SETMASK         equ     (((1<<HW_RTC_PERSISTENT0_RSVD0_WIDTH)-1)<<HW_RTC_PERSISTENT0_RSVD0_BITPOS) 

HW_RTC_PERSISTENT0_ALARMEN_CLRMASK       equ     (~HW_RTC_PERSISTENT0_ALARMEN_SETMASK)
HW_RTC_PERSISTENT0_ALARMWAKE_EN_CLRMASK	 equ	 (~HW_RTC_PERSISTENT0_ALARMWAKE_EN_SETMASK)
HW_RTC_PERSISTENT0_ALARMWAKE_CLRMASK     equ     (~HW_RTC_PERSISTENT0_ALARMWAKE_SETMASK)
HW_RTC_PERSISTENT0_XTALPDOWN_CLRMASK     equ     (~HW_RTC_PERSISTENT0_XTALPDOWN_SETMASK)
HW_RTC_PERSISTENT0_RSVD0_CLRMASK         equ     (~HW_RTC_PERSISTENT0_RSVD0_SETMASK)
 
;C insert

;typedef union               

;{

;   struct {
;      int ALARMEN   : HW_RTC_PERSISTENT0_ALARMEN_WIDTH;
;	   int ALARMWAKE_EN  :  HW_RTC_PERSISTENT0_ALARMWAKE_EN_WIDTH;
;      int ALARMWAKE : HW_RTC_PERSISTENT0_ALARMWAKE_WIDTH;
;      int XTALPDOWN : HW_RTC_PERSISTENT0_XTALPDOWN_WIDTH;
;      int RSVD0     : HW_RTC_PERSISTENT0_RSVD0_WIDTH;
;   } B;

;    int I;
;    unsigned int U;

;} rtc_PERSISTENT0_type;
 

;#define HW_RTC_PERSISTENT0   (*(volatile rtc_PERSISTENT0_type _X*) (HW_RTC_BASEADDR+7))  /* RTC PERSISTENT Register0 */
;endc

;///////////////////////////////////////////////////////////////////////////////

;  RTC PERSISTENT Register1 (HW_RTC_PERSISTENT1) Bit Definitions

HW_RTC_PERSISTENT1_RSVD0_BITPOS          equ     (0)

HW_RTC_PERSISTENT1_RSVD0_WIDTH           equ     (24)

HW_RTC_PERSISTENT1_RSVD0_SETMASK         equ     (((1<<HW_RTC_PERSISTENT1_RSVD0_WIDTH)-1)<<HW_RTC_PERSISTENT1_RSVD0_BITPOS) 

HW_RTC_PERSISTENT1_RSVD0_CLRMASK         equ     (~HW_RTC_PERSISTENT1_RSVD0_SETMASK)


;C insert

;typedef union               

;{

;    struct {
;	 int RSVD0 	: HW_RTC_PERSISTENT1_RSVD0_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} rtc_PERSISTENT1_type;
 
;#define HW_RTC_PERSISTENT1   (*(volatile rtc_PERSISTENT1_type _X*) (HW_RTC_BASEADDR+8))  /* RTC PERSISTENT Register1 */
;endc
 

   endif













