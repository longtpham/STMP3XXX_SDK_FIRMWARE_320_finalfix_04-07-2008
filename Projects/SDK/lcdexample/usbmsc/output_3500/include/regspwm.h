/////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2000-2001
// Filename: regspwm.inc
// Description: Register definitions for PWM interface
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
#if !(defined(regspwminc))
#define regspwminc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//   Pulse Width Modulator STMP Registers 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define HW_PWM_BASEADDR (0xFA31)


/////////////////////////////////////////////////////////////////////////////////
//  PWM CSR register (HW_PWM_CSR) Bit Definitions
#define HW_PWM_CSR_PWM0_EN_BITPOS (0)
#define HW_PWM_CSR_PWM1_EN_BITPOS (1) 
#define HW_PWM_CSR_PWM2_EN_BITPOS (2)
#define HW_PWM_CSR_PWM3_EN_BITPOS (3)
#define HW_PWM_CSR_CDIV_BITPOS (8)
#define HW_PWM_CSR_MSTR_EN_BITPOS (23)

#define HW_PWM_CSR_PWM0_EN_WIDTH (1)
#define HW_PWM_CSR_PWM1_EN_WIDTH (1) 
#define HW_PWM_CSR_PWM2_EN_WIDTH (1)
#define HW_PWM_CSR_PWM3_EN_WIDTH (1)
#define HW_PWM_CSR_CDIV_WIDTH (2)
#define HW_PWM_CSR_MSTR_EN_WIDTH (1)

#define HW_PWM_CSR_PWM0_EN_SETMASK (((1<<HW_PWM_CSR_PWM0_EN_WIDTH)-1)<<HW_PWM_CSR_PWM0_EN_BITPOS)
#define HW_PWM_CSR_PWM1_EN_SETMASK (((1<<HW_PWM_CSR_PWM1_EN_WIDTH)-1)<<HW_PWM_CSR_PWM1_EN_BITPOS)
#define HW_PWM_CSR_PWM2_EN_SETMASK (((1<<HW_PWM_CSR_PWM2_EN_WIDTH)-1)<<HW_PWM_CSR_PWM2_EN_BITPOS)
#define HW_PWM_CSR_PWM3_EN_SETMASK (((1<<HW_PWM_CSR_PWM3_EN_WIDTH)-1)<<HW_PWM_CSR_PWM3_EN_BITPOS)
#define HW_PWM_CSR_CDIV_SETMASK (((1<<HW_PWM_CSR_CDIV_WIDTH)-1)<<HW_PWM_CSR_CDIV_BITPOS)
#define HW_PWM_CSR_MSTR_EN_SETMASK (((1<<HW_PWM_CSR_MSTR_EN_WIDTH)-1)<<HW_PWM_CSR_MSTR_EN_BITPOS)

#define HW_PWM_CSR_PWM0_EN_CLRMASK (~(WORD)HW_PWM_CSR_PWM0_EN_SETMASK)
#define HW_PWM_CSR_PWM1_EN_CLRMASK (~(WORD)HW_PWM_CSR_PWM1_EN_SETMASK)
#define HW_PWM_CSR_PWM2_EN_CLRMASK (~(WORD)HW_PWM_CSR_PWM2_EN_SETMASK)
#define HW_PWM_CSR_PWM3_EN_CLRMASK (~(WORD)HW_PWM_CSR_PWM3_EN_SETMASK)
#define HW_PWM_CSR_CDIV_CLRMASK (~(WORD)HW_PWM_CSR_CDIV_SETMASK)   
#define HW_PWM_CSR_MSTR_EN_CLRMASK (~(WORD)HW_PWM_CSR_MSTR_EN_SETMASK)

typedef union               
{
    struct {
        int PWM0_EN                    :1;
        int PWM1_EN                    :1;
        int PWM2_EN                    :1;
        int PWM3_EN                    :1;
        int RSVD0                      :4;
        int CDIV                       :2;
        int RSVD1                      :13;
        int MSTR_EN                    :1;
    } B;
    int I;
} pwmcsr_type;
#define HW_PWM_CSR        (*(volatile pwmcsr_type _X*) (HW_PWM_BASEADDR))  /* PWM Control Status Register */

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 0 A  (HW_PWM_CH0AR) Bit Definitions
#define HW_PWM_CH0AR_ACTIVE_BITPOS (0)
#define HW_PWM_CH0AR_INACTIVE_BITPOS (12)

#define HW_PWM_CH0AR_ACTIVE_WIDTH (12)
#define HW_PWM_CH0AR_INACTIVE_WIDTH (12)

#define HW_PWM_CH0AR_ACTIVE_SETMASK (((1<<HW_PWM_CH0AR_ACTIVE_WIDTH)-1)<<HW_PWM_CH0AR_ACTIVE_BITPOS)
#define HW_PWM_CH0AR_INACTIVE_SETMASK (((1<<HW_PWM_CH0AR_INACTIVE_WIDTH)-1)<<HW_PWM_CH0AR_INACTIVE_BITPOS)

#define HW_PWM_CH0AR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH0AR_ACTIVE_SETMASK)
#define HW_PWM_CH0AR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH0AR_INACTIVE_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 0 B  (HW_PWM_CH0BR) Bit Definitions
#define HW_PWM_CH0BR_ACTIVE_BITPOS (0)
#define HW_PWM_CH0BR_INACTIVE_BITPOS (12)

#define HW_PWM_CH0BR_ACTIVE_WIDTH (12)
#define HW_PWM_CH0BR_INACTIVE_WIDTH (12)

#define HW_PWM_CH0BR_ACTIVE_SETMASK (((1<<HW_PWM_CH0BR_ACTIVE_WIDTH)-1)<<HW_PWM_CH0BR_ACTIVE_BITPOS)
#define HW_PWM_CH0BR_INACTIVE_SETMASK (((1<<HW_PWM_CH0BR_INACTIVE_WIDTH)-1)<<HW_PWM_CH0BR_INACTIVE_BITPOS)

#define HW_PWM_CH0BR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH0BR_ACTIVE_SETMASK)
#define HW_PWM_CH0BR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH0BR_INACTIVE_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 1 A  (HW_PWM_CH1AR) Bit Definitions
#define HW_PWM_CH1AR_ACTIVE_BITPOS (0)
#define HW_PWM_CH1AR_INACTIVE_BITPOS (12)

#define HW_PWM_CH1AR_ACTIVE_WIDTH (12)
#define HW_PWM_CH1AR_INACTIVE_WIDTH (12)

#define HW_PWM_CH1AR_ACTIVE_SETMASK (((1<<HW_PWM_CH1AR_ACTIVE_WIDTH)-1)<<HW_PWM_CH1AR_ACTIVE_BITPOS)
#define HW_PWM_CH1AR_INACTIVE_SETMASK (((1<<HW_PWM_CH1AR_INACTIVE_WIDTH)-1)<<HW_PWM_CH1AR_INACTIVE_BITPOS)

#define HW_PWM_CH1AR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH1AR_ACTIVE_SETMASK)
#define HW_PWM_CH1AR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH1AR_INACTIVE_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 1 B  (HW_PWM_CH1BR) Bit Definitions
#define HW_PWM_CH1BR_ACTIVE_BITPOS (0)
#define HW_PWM_CH1BR_INACTIVE_BITPOS (12)

#define HW_PWM_CH1BR_ACTIVE_WIDTH (12)
#define HW_PWM_CH1BR_INACTIVE_WIDTH (12)

#define HW_PWM_CH1BR_ACTIVE_SETMASK (((1<<HW_PWM_CH1BR_ACTIVE_WIDTH)-1)<<HW_PWM_CH1BR_ACTIVE_BITPOS)
#define HW_PWM_CH1BR_INACTIVE_SETMASK (((1<<HW_PWM_CH1BR_INACTIVE_WIDTH)-1)<<HW_PWM_CH1BR_INACTIVE_BITPOS)

#define HW_PWM_CH1BR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH1BR_ACTIVE_SETMASK)
#define HW_PWM_CH1BR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH1BR_INACTIVE_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 2 A  (HW_PWM_CH2AR) Bit Definitions
#define HW_PWM_CH2AR_ACTIVE_BITPOS (0)
#define HW_PWM_CH2AR_INACTIVE_BITPOS (12)

#define HW_PWM_CH2AR_ACTIVE_WIDTH (12)
#define HW_PWM_CH2AR_INACTIVE_WIDTH (12)

#define HW_PWM_CH2AR_ACTIVE_SETMASK (((1<<HW_PWM_CH2AR_ACTIVE_WIDTH)-1)<<HW_PWM_CH2AR_ACTIVE_BITPOS)
#define HW_PWM_CH2AR_INACTIVE_SETMASK (((1<<HW_PWM_CH2AR_INACTIVE_WIDTH)-1)<<HW_PWM_CH2AR_INACTIVE_BITPOS)

#define HW_PWM_CH2AR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH2AR_ACTIVE_SETMASK)
#define HW_PWM_CH2AR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH2AR_INACTIVE_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 2 B  (HW_PWM_CH2BR) Bit Definitions
#define HW_PWM_CH2BR_ACTIVE_BITPOS (0)
#define HW_PWM_CH2BR_INACTIVE_BITPOS (12)

#define HW_PWM_CH2BR_ACTIVE_WIDTH (12)
#define HW_PWM_CH2BR_INACTIVE_WIDTH (12)

#define HW_PWM_CH2BR_ACTIVE_SETMASK (((1<<HW_PWM_CH2BR_ACTIVE_WIDTH)-1)<<HW_PWM_CH2BR_ACTIVE_BITPOS)
#define HW_PWM_CH2BR_INACTIVE_SETMASK (((1<<HW_PWM_CH2BR_INACTIVE_WIDTH)-1)<<HW_PWM_CH2BR_INACTIVE_BITPOS)

#define HW_PWM_CH2BR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH2BR_ACTIVE_SETMASK)
#define HW_PWM_CH2BR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH2BR_INACTIVE_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 3 A  (HW_PWM_CH3AR) Bit Definitions
#define HW_PWM_CH3AR_ACTIVE_BITPOS (0)
#define HW_PWM_CH3AR_INACTIVE_BITPOS (12)

#define HW_PWM_CH3AR_ACTIVE_WIDTH (12)
#define HW_PWM_CH3AR_INACTIVE_WIDTH (12)

#define HW_PWM_CH3AR_ACTIVE_SETMASK (((1<<HW_PWM_CH3AR_ACTIVE_WIDTH)-1)<<HW_PWM_CH3AR_ACTIVE_BITPOS)
#define HW_PWM_CH3AR_INACTIVE_SETMASK (((1<<HW_PWM_CH3AR_INACTIVE_WIDTH)-1)<<HW_PWM_CH3AR_INACTIVE_BITPOS)

#define HW_PWM_CH3AR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH3AR_ACTIVE_SETMASK)
#define HW_PWM_CH3AR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH3AR_INACTIVE_SETMASK)

/////////////////////////////////////////////////////////////////////////////////
//  PWM CHANNEL 3 B  (HW_PWM_CH3BR) Bit Definitions
#define HW_PWM_CH3BR_ACTIVE_BITPOS (0)
#define HW_PWM_CH3BR_INACTIVE_BITPOS (12)

#define HW_PWM_CH3BR_ACTIVE_WIDTH (12)
#define HW_PWM_CH3BR_INACTIVE_WIDTH (12)

#define HW_PWM_CH3BR_ACTIVE_SETMASK (((1<<HW_PWM_CH3BR_ACTIVE_WIDTH)-1)<<HW_PWM_CH3BR_ACTIVE_BITPOS)
#define HW_PWM_CH3BR_INACTIVE_SETMASK (((1<<HW_PWM_CH3BR_INACTIVE_WIDTH)-1)<<HW_PWM_CH3BR_INACTIVE_BITPOS)

#define HW_PWM_CH3BR_ACTIVE_CLRMASK (~(WORD)HW_PWM_CH3BR_ACTIVE_SETMASK)
#define HW_PWM_CH3BR_INACTIVE_CLRMASK (~(WORD)HW_PWM_CH3BR_INACTIVE_SETMASK)

typedef union               
{
    struct {
       int ACTIVE                    :12;
       int INACTIVE                  :12;
    } B;
    int I;
} pwmchan_type;
#define HW_PWM_CH0AR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+1))  /* PWM Channel 0 A Register */
#define HW_PWM_CH0BR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+2))  /* PWM Channel 0 B Register */
#define HW_PWM_CH1AR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+3))  /* PWM Channel 1 A Register */
#define HW_PWM_CH1BR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+4))  /* PWM Channel 1 B Register */
#define HW_PWM_CH2AR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+5))  /* PWM Channel 2 A Register */
#define HW_PWM_CH2BR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+6))  /* PWM Channel 2 B Register */
#define HW_PWM_CH3AR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+7))  /* PWM Channel 3 A Register */
#define HW_PWM_CH3BR        (*(volatile pwmchan_type _X*) (HW_PWM_BASEADDR+8))  /* PWM Channel 3 B Register */

#endif




