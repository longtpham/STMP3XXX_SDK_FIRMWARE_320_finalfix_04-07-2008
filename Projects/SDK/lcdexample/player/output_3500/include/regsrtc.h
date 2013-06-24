/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2004
// Filename: regsrtc.inc
// Description: Register definitions for RTC interface
/////////////////////////////////////////////////////////////////////////////////
// The following naming conventions are followed in this file.
// All registers are named using the format...
//     HW_<module>_<regname>
// where <module> is the module name which can be any of the following...
//     USB20
// (Note that when there is more than one copy of a particular module, the
// module name includes a number starting from 0 for the first instance of
// that module)
// <regname> is the specific register within that module
// We also define the following...
//     HW_<module>_<regname>_BITPOS
// which defines the starting bit (i.e. LSB) of a multi bit field
//     HW_<module>_<regname>_SETMASK
// which does something else, and
//     HW_<module>_<regname>_CLRMASK
// which does something else.
// Other rules
//     All caps
//     Numeric identifiers start at 0
#if !(defined(regsrtcinc))
#define regsrtcinc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//   RTC STMP Registers 
//   Edited 2/26/2002 J. Ferrara
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#define HW_RTC_BASEADDR (0xF500)





/////////////////////////////////////////////////////////////////////////////////

////  RTC Control Status Register (HW_RTC_CSR) Bit Definitions

#define HW_RTC_CSR_ALARMINTEN_BITPOS (0)
#define HW_RTC_CSR_ALARMINT_BITPOS (1)
#define HW_RTC_CSR_WATCHDOGEN_BITPOS (2)
#define HW_RTC_CSR_RSVD0_BITPOS (3)
#define HW_RTC_CSR_STALEREGS_BITPOS (4)
#define HW_RTC_CSR_RSVD1_BITPOS (9)
#define HW_RTC_CSR_NEWREGS_BITPOS (12)
#define HW_RTC_CSR_NEWREGS_XTALDIV_BITPOS (12)
#define HW_RTC_CSR_NEWREGS_MSCOUNT_BITPOS (13)
#define HW_RTC_CSR_NEWREGS_ALRMCOUNT_BITPOS (14)
#define HW_RTC_CSR_NEWREGS_PERSIST0_BITPOS (15)
#define HW_RTC_CSR_NEWREGS_PERSIST1_BITPOS (16)
#define HW_RTC_CSR_RSVD2_BITPOS (17)
#define HW_RTC_CSR_FORCE_BITPOS (20)
#define HW_RTC_CSR_RSVD3_BITPOS (21)
#define HW_RTC_CSR_SFTRST_BITPOS (23)										
#define HW_RTC_CSR_ALARMINTEN_WIDTH (1)
#define HW_RTC_CSR_ALARMINT_WIDTH (1)
#define HW_RTC_CSR_WATCHDOGEN_WIDTH (1)
#define HW_RTC_CSR_RSVD0_WIDTH (1)
#define HW_RTC_CSR_STALEREGS_WIDTH (5)
#define HW_RTC_CSR_RSVD1_WIDTH (3)
#define HW_RTC_CSR_NEWREGS_WIDTH (5)
#define HW_RTC_CSR_NEWREGS_XTALDIV_WIDTH (1)
#define HW_RTC_CSR_NEWREGS_MSCOUNT_WIDTH (1)
#define HW_RTC_CSR_NEWREGS_ALRMCOUNT_WIDTH (1)
#define HW_RTC_CSR_NEWREGS_PERSIST0_WIDTH (1)
#define HW_RTC_CSR_NEWREGS_PERSIST1_WIDTH (1)
#define HW_RTC_CSR_RSVD2_WIDTH (3)
#define HW_RTC_CSR_FORCE_WIDTH (1)
#define HW_RTC_CSR_RSVD3_WIDTH (2)
#define HW_RTC_CSR_SFTRST_WIDTH (1)

#define HW_RTC_CSR_ALARMINT_SETMASK (((1<<HW_RTC_CSR_ALARMINT_WIDTH)-1)<<HW_RTC_CSR_ALARMINT_BITPOS) 
#define HW_RTC_CSR_ALARMINTEN_SETMASK (((1<<HW_RTC_CSR_ALARMINTEN_WIDTH)-1)<<HW_RTC_CSR_ALARMINTEN_BITPOS) 
#define HW_RTC_CSR_WATCHDOGEN_SETMASK (((1<<HW_RTC_CSR_WATCHDOGEN_WIDTH)-1)<<HW_RTC_CSR_WATCHDOGEN_BITPOS) 
#define HW_RTC_CSR_RSVD0_SETMASK (((1<<HW_RTC_CSR_RSVD0_WIDTH)-1)<<HW_RTC_CSR_RSVD0_BITPOS) 
#define HW_RTC_CSR_STALEREGS_SETMASK (((1<<HW_RTC_CSR_STALEREGS_WIDTH)-1)<<HW_RTC_CSR_STALEREGS_BITPOS) 
#define HW_RTC_CSR_RSVD1_SETMASK (((1<<HW_RTC_CSR_RSVD1_WIDTH)-1)<<HW_RTC_CSR_RSVD1_BITPOS) 
#define HW_RTC_CSR_NEWREGS_SETMASK (((1<<HW_RTC_CSR_NEWREGS_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_BITPOS) 
#define HW_RTC_CSR_NEWREGS_XTALDIV_SETMASK (((1<<HW_RTC_CSR_NEWREGS_XTALDIV_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_XTALDIV_BITPOS)   
#define HW_RTC_CSR_NEWREGS_MSCOUNT_SETMASK (((1<<HW_RTC_CSR_NEWREGS_MSCOUNT_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_MSCOUNT_BITPOS)
#define HW_RTC_CSR_NEWREGS_ALRMCOUNT_SETMASK (((1<<HW_RTC_CSR_NEWREGS_ALRMCOUNT_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_ALRMCOUNT_BITPOS)
#define HW_RTC_CSR_NEWREGS_PERSIST0_SETMASK (((1<<HW_RTC_CSR_NEWREGS_PERSIST0_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_PERSIST0_BITPOS)
#define HW_RTC_CSR_NEWREGS_PERSIST1_SETMASK (((1<<HW_RTC_CSR_NEWREGS_PERSIST1_WIDTH)-1)<<HW_RTC_CSR_NEWREGS_PERSIST1_BITPOS)
#define HW_RTC_CSR_RSVD2_SETMASK (((1<<HW_RTC_CSR_RSVD2_WIDTH)-1)<<HW_RTC_CSR_RSVD2_BITPOS) 
#define HW_RTC_CSR_FORCE_SETMASK (((1<<HW_RTC_CSR_FORCE_WIDTH)-1)<<HW_RTC_CSR_FORCE_BITPOS) 
#define HW_RTC_CSR_RSVD3_SETMASK (((1<<HW_RTC_CSR_RSVD3_WIDTH)-1)<<HW_RTC_CSR_RSVD3_BITPOS) 
#define HW_RTC_CSR_SFTRST_SETMASK (((1<<HW_RTC_CSR_SFTRST_WIDTH)-1)<<HW_RTC_CSR_SFTRST_BITPOS) 

#define HW_RTC_CSR_ALARMINTEN_CLRMASK (~(WORD)HW_RTC_CSR_ALARMINTEN_SETMASK)
#define HW_RTC_CSR_ALARMINT_CLRMASK (~(WORD)HW_RTC_CSR_ALARMINT_SETMASK)
#define HW_RTC_CSR_WATCHDOGEN_CLRMASK (~(WORD)HW_RTC_CSR_WATCHDOGEN_SETMASK)
#define HW_RTC_CSR_RSVD0_CLRMASK (~(WORD)HW_RTC_CSR_RSVD0_SETMASK)
#define HW_RTC_CSR_STALEREGS_CLRMASK (~(WORD)HW_RTC_CSR_STALEREGS_SETMASK)
#define HW_RTC_CSR_RSVD1_CLRMASK (~(WORD)HW_RTC_CSR_RSVD1_SETMASK)
#define HW_RTC_CSR_NEWREGS_CLRMASK (~(WORD)HW_RTC_CSR_NEWREGS_SETMASK)
#define HW_RTC_CSR_NEWREGS_XTALDIV_CLRMASK (~(WORD)HW_RTC_CSR_NEWREGS_XTALDIV_SETMASK)   
#define HW_RTC_CSR_NEWREGS_MSCOUNT_CLRMASK (~(WORD)HW_RTC_CSR_NEWREGS_MSCOUNT_SETMASK)
#define HW_RTC_CSR_NEWREGS_ALRMCOUNT_CLRMASK (~(WORD)HW_RTC_CSR_NEWREGS_ALRMCOUNT_SETMASK)
#define HW_RTC_CSR_NEWREGS_PERSIST0_CLRMASK (~(WORD)HW_RTC_CSR_NEWREGS_PERSIST0_SETMASK)
#define HW_RTC_CSR_NEWREGS_PERSIST1_CLRMASK (~(WORD)HW_RTC_CSR_NEWREGS_PERSIST1_SETMASK)

#define HW_RTC_CSR_RSVD2_CLRMASK (~(WORD)HW_RTC_CSR_RSVD2_SETMASK)
#define HW_RTC_CSR_FORCE_CLRMASK (~(WORD)HW_RTC_CSR_FORCE_SETMASK)
#define HW_RTC_CSR_RSVD3_CLRMASK (~(WORD)HW_RTC_CSR_RSVD3_SETMASK)
#define HW_RTC_CSR_SFTRST_CLRMASK (~(WORD)HW_RTC_CSR_SFTRST_SETMASK)


typedef union               
{
    struct {
	 int ALARMINTEN 	: HW_RTC_CSR_ALARMINTEN_WIDTH;
	 int ALARMINT           : HW_RTC_CSR_ALARMINT_WIDTH;
	 int WATCHDOGEN         : HW_RTC_CSR_WATCHDOGEN_WIDTH;
	 int RSVD0              : HW_RTC_CSR_RSVD0_WIDTH;
	 unsigned int STALEREGS : HW_RTC_CSR_STALEREGS_WIDTH;
	 int RSVD1              : HW_RTC_CSR_RSVD1_WIDTH;
	 unsigned int NEWREGS   : HW_RTC_CSR_NEWREGS_WIDTH;
	 int RSVD2              : HW_RTC_CSR_RSVD2_WIDTH;
	 int FORCE             : HW_RTC_CSR_FORCE_WIDTH;
	 int RSVD3              : HW_RTC_CSR_RSVD3_WIDTH;
	 unsigned int SFTRST    : HW_RTC_CSR_SFTRST_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_csr_type;
#define HW_RTC_CSR      (*(volatile rtc_csr_type _X*) (HW_RTC_BASEADDR+0))    /* RTC Control / Status Register */

/////////////////////////////////////////////////////////////////////////////////

//  RTC Milliseconds Register0 (HW_RTC_MSECONDS0) Bit Definitions

#define HW_RTC_MSECONDS0_MSECONDSLOW_BITPOS (0)

#define HW_RTC_MSECONDS0_MSECONDSLOW_WIDTH (24)

#define HW_RTC_MSECONDS0_MSECONDSLOW_SETMASK (((1<<HW_RTC_MSECONDS0_MSECONDSLOW_WIDTH)-1)<<HW_RTC_MSECONDS0_MSECONDSLOW_BITPOS) 

#define HW_RTC_MSECONDS0_MSECONDSLOW_CLRMASK (~(WORD)HW_RTC_MSECONDS0_MSECONDSLOW_SETMASK)

typedef union               
{
    struct {
	 int MSECONDSLOW 	: HW_RTC_MSECONDS0_MSECONDSLOW_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_mseconds0_type;
#define HW_RTC_MSECONDS0      (*(volatile rtc_mseconds0_type _X*) (HW_RTC_BASEADDR+1))    /* RTC MSECONDS Lower Word Register */
#define HW_RTCLOWR            HW_RTC_MSECONDS0
/////////////////////////////////////////////////////////////////////////////////

//  RTC Milliseconds Register1 (HW_RTC_MSECONDS1) Bit Definitions

#define HW_RTC_MSECONDS1_MSECONDSHIGH_BITPOS (0)

#define HW_RTC_MSECONDS1_MSECONDSHIGH_WIDTH (24)

#define HW_RTC_MSECONDS1_MSECONDSHIGH_SETMASK (((1<<HW_RTC_MSECONDS1_MSECONDSHIGH_WIDTH)-1)<<HW_RTC_MSECONDS1_MSECONDSHIGH_BITPOS) 

#define HW_RTC_MSECONDS1_MSECONDSHIGH_CLRMASK (~(WORD)HW_RTC_MSECONDS1_MSECONDSHIGH_SETMASK)

typedef union               
{
    struct {
	 int MSECONDSHIGH 	: HW_RTC_MSECONDS1_MSECONDSHIGH_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_mseconds1_type;
#define HW_RTC_MSECONDS1      (*(volatile rtc_mseconds1_type _X*) (HW_RTC_BASEADDR+2))    /* RTC MSECONDS Upper Word Register */
#define HW_RTCUPR             HW_RTC_MSECONDS1

#define HW_RTC_LOWER_OFFSET 1
#define HW_RTC_UP_OFFSET 2


/////////////////////////////////////////////////////////////////////////////////

//  RTC Watchdog Count (HW_RTC_WATCHDOG) Bit Definitions

#define HW_RTC_WATCHDOG_TIMEOUTCNT_BITPOS (0)

#define HW_RTC_WATCHDOG_TIMEOUTCNT_WIDTH (24)

#define HW_RTC_WATCHDOG_TIMEOUTCNT_SETMASK (((1<<HW_RTC_WATCHDOG_TIMEOUTCNT_WIDTH)-1)<<HW_RTC_WATCHDOG_TIMEOUTCNT_BITPOS) 

#define HW_RTC_WATCHDOG_TIMEOUTCNT_CLRMASK (~(WORD)HW_RTC_WATCHDOG_TIMEOUTCNT_SETMASK)

typedef union               
{
    struct {
	 int TIMEOUTCNT 	: HW_RTC_WATCHDOG_TIMEOUTCNT_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_watchdog_type;
#define HW_RTC_WATCHDOG     (*(volatile rtc_watchdog_type _X*) (HW_RTC_BASEADDR+3))    /* RTC Watchdog Timeout Register */
/////////////////////////////////////////////////////////////////////////////////

//  RTC Alarm Register0 (HW_RTC_ALARM0) Bit Definitions

#define HW_RTC_ALARM0_ALARMLOW_BITPOS (0)

#define HW_RTC_ALARM0_ALARMLOW_WIDTH (24)

#define HW_RTC_ALARM0_ALARMLOW_SETMASK (((1<<HW_RTC_ALARM0_ALARMLOW_WIDTH)-1)<<HW_RTC_ALARM0_ALARMLOW_BITPOS) 

#define HW_RTC_ALARM0_ALARMLOW_CLRMASK (~(WORD)HW_RTC_ALARM0_ALARMLOW_SETMASK)

typedef union               
{
    struct {
	 int ALARMLOW 	: HW_RTC_ALARM0_ALARMLOW_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_alarm0_type;
#define HW_RTC_ALARM0      (*(volatile rtc_alarm0_type _X*) (HW_RTC_BASEADDR+4))    /* RTC ALARM Lower Word Register */
/////////////////////////////////////////////////////////////////////////////////

//  RTC Alarm Register1 (HW_RTC_ALARM1) Bit Definitions

#define HW_RTC_ALARM1_ALARMHIGH_BITPOS (0)

#define HW_RTC_ALARM1_ALARMHIGH_WIDTH (24)

#define HW_RTC_ALARM1_ALARMHIGH_SETMASK (((1<<HW_RTC_ALARM1_ALARMHIGH_WIDTH)-1)<<HW_RTC_ALARM1_ALARMHIGH_BITPOS) 

#define HW_RTC_ALARM1_ALARMHIGH_CLRMASK (~(WORD)HW_RTC_ALARM1_ALARMHIGH_SETMASK)

typedef union               
{
    struct {
	 int ALARMHIGH 	: HW_RTC_ALARM1_ALARMHIGH_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_alarm1_type;
#define HW_RTC_ALARM1      (*(volatile rtc_alarm1_type _X*) (HW_RTC_BASEADDR+5))    /* RTC ALARM Upper Word Register */
/////////////////////////////////////////////////////////////////////////////////

//  RTC Xtal-Clock Divide Register (HW_RTC_XTALDIVIDE) Bit Definitions

#define HW_RTC_XTALDIVIDE_DIVCOUNT_BITPOS (0)
#define HW_RTC_XTALDIVIDE_RSVD0_BITPOS (16)

#define HW_RTC_XTALDIVIDE_DIVCOUNT_WIDTH (16)
#define HW_RTC_XTALDIVIDE_RSVD0_WIDTH (8)

#define HW_RTC_XTALDIVIDE_DIVCOUNT_SETMASK (((1<<HW_RTC_XTALDIVIDE_DIVCOUNT_WIDTH)-1)<<HW_RTC_XTALDIVIDE_DIVCOUNT_BITPOS) 
#define HW_RTC_XTALDIVIDE_RSVD0_SETMASK (((1<<HW_RTC_XTALDIVIDE_RSVD0_BITPOS)-1)<<HW_RTC_XTALDIVIDE_RSVD0_BITPOS) 

#define HW_RTC_XTALDIVIDE_DIVCOUNT_CLRMASK (~(WORD)HW_RTC_XTALDIVIDE_DIVCOUNT_SETMASK)
#define HW_RTC_XTALDIVIDE_RSVD0_CLRMASK (~(WORD)HW_RTC_XTALDIVIDE_RSVD0_SETMASK)

typedef union               
{
    struct {
	 int DIVCOUNT 	: HW_RTC_XTALDIVIDE_DIVCOUNT_WIDTH;
	 int RSVD0 	: HW_RTC_XTALDIVIDE_RSVD0_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_xtaldivide_type;
#define HW_RTC_XTALDIVIDE   (*(volatile rtc_xtaldivide_type _X*) (HW_RTC_BASEADDR+6))  /* RTC Xtal-clock Pre-Divider Word Register */
/////////////////////////////////////////////////////////////////////////////////

//  RTC PERSISTENT Register0 (HW_RTC_PERSISTENT0) Bit Definitions

#define HW_RTC_PERSISTENT0_ALARMEN_BITPOS (0)
#define HW_RTC_PERSISTENT0_ALARMWAKE_EN_BITPOS (1)
#define HW_RTC_PERSISTENT0_ALARMWAKE_BITPOS (2)
#define HW_RTC_PERSISTENT0_XTALPDOWN_BITPOS (3)
#define HW_RTC_PERSISTENT0_RSVD0_BITPOS (4)

#define HW_RTC_PERSISTENT0_ALARMEN_WIDTH (1)
#define HW_RTC_PERSISTENT0_ALARMWAKE_EN_WIDTH (1)
#define HW_RTC_PERSISTENT0_ALARMWAKE_WIDTH (1)
#define HW_RTC_PERSISTENT0_XTALPDOWN_WIDTH (1)
#define HW_RTC_PERSISTENT0_RSVD0_WIDTH (21)

#define HW_RTC_PERSISTENT0_ALARMEN_SETMASK (((1<<HW_RTC_PERSISTENT0_ALARMEN_WIDTH)-1)<<HW_RTC_PERSISTENT0_ALARMEN_BITPOS) 
#define HW_RTC_PERSISTENT0_ALARMWAKE_EN_SETMASK (((1<<HW_RTC_PERSISTENT0_ALARMWAKE_EN_WIDTH)-1)<<HW_RTC_PERSISTENT0_ALARMWAKE_EN_BITPOS)
#define HW_RTC_PERSISTENT0_ALARMWAKE_SETMASK (((1<<HW_RTC_PERSISTENT0_ALARMWAKE_WIDTH)-1)<<HW_RTC_PERSISTENT0_ALARMWAKE_BITPOS) 
#define HW_RTC_PERSISTENT0_XTALPDOWN_SETMASK (((1<<HW_RTC_PERSISTENT0_XTALPDOWN_WIDTH)-1)<<HW_RTC_PERSISTENT0_XTALPDOWN_BITPOS) 
#define HW_RTC_PERSISTENT0_RSVD0_SETMASK (((1<<HW_RTC_PERSISTENT0_RSVD0_WIDTH)-1)<<HW_RTC_PERSISTENT0_RSVD0_BITPOS) 

#define HW_RTC_PERSISTENT0_ALARMEN_CLRMASK (~(WORD)HW_RTC_PERSISTENT0_ALARMEN_SETMASK)
#define HW_RTC_PERSISTENT0_ALARMWAKE_EN_CLRMASK (~(WORD)HW_RTC_PERSISTENT0_ALARMWAKE_EN_SETMASK)
#define HW_RTC_PERSISTENT0_ALARMWAKE_CLRMASK (~(WORD)HW_RTC_PERSISTENT0_ALARMWAKE_SETMASK)
#define HW_RTC_PERSISTENT0_XTALPDOWN_CLRMASK (~(WORD)HW_RTC_PERSISTENT0_XTALPDOWN_SETMASK)
#define HW_RTC_PERSISTENT0_RSVD0_CLRMASK (~(WORD)HW_RTC_PERSISTENT0_RSVD0_SETMASK)

typedef union               
{
   struct {
      int ALARMEN   : HW_RTC_PERSISTENT0_ALARMEN_WIDTH;
	   int ALARMWAKE_EN  :  HW_RTC_PERSISTENT0_ALARMWAKE_EN_WIDTH;
      int ALARMWAKE : HW_RTC_PERSISTENT0_ALARMWAKE_WIDTH;
      int XTALPDOWN : HW_RTC_PERSISTENT0_XTALPDOWN_WIDTH;
      int RSVD0     : HW_RTC_PERSISTENT0_RSVD0_WIDTH;
   } B;
    int I;
    unsigned int U;
} rtc_PERSISTENT0_type;
#define HW_RTC_PERSISTENT0   (*(volatile rtc_PERSISTENT0_type _X*) (HW_RTC_BASEADDR+7))  /* RTC PERSISTENT Register0 */

/////////////////////////////////////////////////////////////////////////////////

//  RTC PERSISTENT Register1 (HW_RTC_PERSISTENT1) Bit Definitions

#define HW_RTC_PERSISTENT1_RSVD0_BITPOS (0)

#define HW_RTC_PERSISTENT1_RSVD0_WIDTH (24)

#define HW_RTC_PERSISTENT1_RSVD0_SETMASK (((1<<HW_RTC_PERSISTENT1_RSVD0_WIDTH)-1)<<HW_RTC_PERSISTENT1_RSVD0_BITPOS) 

#define HW_RTC_PERSISTENT1_RSVD0_CLRMASK (~(WORD)HW_RTC_PERSISTENT1_RSVD0_SETMASK)


typedef union               
{
    struct {
	 int RSVD0 	: HW_RTC_PERSISTENT1_RSVD0_WIDTH;
    } B;
    int I;
    unsigned int U;
} rtc_PERSISTENT1_type;
#define HW_RTC_PERSISTENT1   (*(volatile rtc_PERSISTENT1_type _X*) (HW_RTC_BASEADDR+8))  /* RTC PERSISTENT Register1 */


#endif














