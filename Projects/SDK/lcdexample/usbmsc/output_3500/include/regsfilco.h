//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;  Copyright(C) SigmaTel, Inc. 2002-2003
//;  File        : regsfilco.inc
//;  Description : FILCO IP Register definition
//; Last Edited 2.26.2003 Rod Holley
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

// The following naming conventions are followed in this file.
// All registers are named using the format...
//     HW_<module>_<regname>
// where <module> is the module name which can be any of the following...
//     FILCO
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

#if !(defined(regsfilcoinc))
#define regsfilcoinc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//   FILCO STMP Registers 
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define HW_FILCO_BASEADDR (0xFB80)

/////////////////////////////////////////////////////////////////////////////////
//  FILCO Registers
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
//  FILCO Control Status Register (HW_FILCO_CSR) Bit Definitions
#define HW_FILCO_CSR_DISABLE_BITPOS (0)
#define HW_FILCO_CSR_DMAOUT_BITPOS (2)
#define HW_FILCO_CSR_DMASAMP_BITPOS (3)
#define HW_FILCO_CSR_DMACOEF_BITPOS (6)
#define HW_FILCO_CSR_FILCOSEL_BITPOS (11)
#define HW_FILCO_CSR_IIRMODE_BITPOS (12)
#define HW_FILCO_CSR_IIRSCALE_BITPOS (13)
#define HW_FILCO_CSR_FIRMODE_BITPOS (14)
#define HW_FILCO_CSR_CLKDIV_BITPOS (16)
#define HW_FILCO_CSR_CLKGT_BITPOS (23)

#define HW_FILCO_CSR_DISABLE_WIDTH (1)
#define HW_FILCO_CSR_DMAOUT_WIDTH (2)
#define HW_FILCO_CSR_DMASAMP_WIDTH (2)
#define HW_FILCO_CSR_DMACOEF_WIDTH (2)
#define HW_FILCO_CSR_FILCOSEL_WIDTH (1)
#define HW_FILCO_CSR_IIRMODE_WIDTH (1)
#define HW_FILCO_CSR_IIRSCALE_WIDTH (1)
#define HW_FILCO_CSR_FIRMODE_WIDTH (2)
#define HW_FILCO_CSR_CLKDIV_WIDTH (4)
#define HW_FILCO_CSR_CLKGT_WIDTH (1)

#define HW_FILCO_CSR_DISABLE_SETMASK (((1<<HW_FILCO_CSR_DISABLE_WIDTH)-1)<<HW_FILCO_CSR_DISABLE_BITPOS)
#define HW_FILCO_CSR_DMAOUT_SETMASK (((1<<HW_FILCO_CSR_DMAOUT_WIDTH)-1)<<HW_FILCO_CSR_DMAOUT_BITPOS)
#define HW_FILCO_CSR_DMASAMP_SETMASK (((1<<HW_FILCO_CSR_DMASAMP_WIDTH)-1)<<HW_FILCO_CSR_DMASAMP_BITPOS)
#define HW_FILCO_CSR_DMACOEF_SETMASK (((1<<HW_FILCO_CSR_DMACOEF_WIDTH)-1)<<HW_FILCO_CSR_DMACOEF_BITPOS)
#define HW_FILCO_CSR_FILCOSEL_SETMASK (((1<<HW_FILCO_CSR_FILCOSEL_WIDTH)-1)<<HW_FILCO_CSR_FILCOSEL_BITPOS)
#define HW_FILCO_CSR_IIRMODE_SETMASK (((1<<HW_FILCO_CSR_IIRMODE_WIDTH)-1)<<HW_FILCO_CSR_IIRMODE_BITPOS)
#define HW_FILCO_CSR_IIRSCALE_SETMASK (((1<<HW_FILCO_CSR_IIRSCALE_WIDTH)-1)<<HW_FILCO_CSR_IIRSCALE_BITPOS)
#define HW_FILCO_CSR_FIRMODE_SETMASK (((1<<HW_FILCO_CSR_FIRMODE_WIDTH)-1)<<HW_FILCO_CSR_FIRMODE_BITPOS)
#define HW_FILCO_CSR_CLKDIV_SETMASK (((1<<HW_FILCO_CSR_CLKDIV_WIDTH)-1)<<HW_FILCO_CSR_CLKDIV_BITPOS)
#define HW_FILCO_CSR_CLKGT_SETMASK (((1<<HW_FILCO_CSR_CLKGT_WIDTH)-1)<<HW_FILCO_CSR_CLKGT_BITPOS)

#define HW_FILCO_CSR_DISABLE_CLRMASK (~(WORD)HW_FILCO_CSR_DISABLE_SETMASK)
#define HW_FILCO_CSR_DMAOUT_CLRMASK (~(WORD)HW_FILCO_CSR_DMAOUT_SETMASK)
#define HW_FILCO_CSR_DMASAMP_CLRMASK (~(WORD)HW_FILCO_CSR_DMASAMP_SETMASK)
#define HW_FILCO_CSR_DMACOEF_CLRMASK (~(WORD)HW_FILCO_CSR_DMACOEF_SETMASK)
#define HW_FILCO_CSR_FILCOSEL_CLRMASK (~(WORD)HW_FILCO_CSR_FILCOSEL_SETMASK)
#define HW_FILCO_CSR_IIRMODE_CLRMASK (~(WORD)HW_FILCO_CSR_IIRMODE_SETMASK)
#define HW_FILCO_CSR_IIRSCALE_CLRMASK (~(WORD)HW_FILCO_CSR_IIRSCALE_SETMASK)
#define HW_FILCO_CSR_FIRMODE_CLRMASK (~(WORD)HW_FILCO_CSR_FIRMODE_SETMASK)
#define HW_FILCO_CSR_CLKDIV_CLRMASK (~(WORD)HW_FILCO_CSR_CLKDIV_SETMASK)
#define HW_FILCO_CSR_CLKGT_CLRMASK (~(WORD)HW_FILCO_CSR_CLKGT_SETMASK)

typedef union               
{
    struct {
        int DISABLE  	: HW_FILCO_CSR_DISABLE_WIDTH ;
        int DMAOUT  	: HW_FILCO_CSR_DMAOUT_WIDTH ;
        int DMASAMP  	: HW_FILCO_CSR_DMASAMP_WIDTH ;  
        int DMACOEF  	: HW_FILCO_CSR_DMACOEF_WIDTH ;   
        int FILCOSEL  	: HW_FILCO_CSR_FILCOSEL_WIDTH ;  
        int IIRMODE  	: HW_FILCO_CSR_IIRMODE_WIDTH ;   
        int IIRSCALE  	: HW_FILCO_CSR_IIRSCALE_WIDTH ;  
        int FIRMODE  	: HW_FILCO_CSR_FIRMODE_WIDTH ;   
        int CLKDIV  	: HW_FILCO_CSR_CLKDIV_WIDTH ;    
        int CLKGT  	: HW_FILCO_CSR_CLKGT_WIDTH ;     
    } B;
    int I;
} filcocsr_type;
#define HW_FILCO_CSR      (*(volatile filcocsr_type _X*) (HW_FILCO_BASEADDR))    /* FILCO Control / Status Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_SAT) Bit Definitions
#define HW_FILCO_SAT_BITPOS (0)

#define HW_FILCO_SAT_WIDTH (8)

#define HW_FILCO_SAT_SETMASK (((1<<HW_FILCO_SAT_WIDTH)-1)<<HW_FILCO_SAT_BITPOS)

#define HW_FILCO_SAT_CLRMASK (~(WORD)HW_FILCO_SAT_SETMASK)

typedef union               
{
    struct {
        int SAT  	: HW_FILCO_SAT_WIDTH ;
    } B;
    int I;
} filcosat_type;
#define HW_FILCOSAT      (*(volatile filcosat_type _X*) (HW_FILCO_BASEADDR+1))    /* FILCO Saturation Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_CBAR) Bit Definitions
#define HW_FILCO_CBAR_BITPOS (0)

#define HW_FILCO_CBAR_WIDTH (16)

#define HW_FILCO_CBAR_SETMASK (((1<<HW_FILCO_CBAR_WIDTH)-1)<<HW_FILCO_CBAR_BITPOS)

#define HW_FILCO_CBAR_CLRMASK (~(WORD)HW_FILCO_CBAR_SETMASK)

typedef union               
{
    struct {
        int CBAR	: HW_FILCO_CBAR_WIDTH;
    } B;
    int I;
} filcocbar_type;
#define HW_FILCO_CBAR      (*(volatile filcocbar_type _X*) (HW_FILCO_BASEADDR+2))    /* FILCO Coef Base Address */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_CMR) Bit Definitions
#define HW_FILCO_CMR_BITPOS (0)

#define HW_FILCO_CMR_WIDTH (8)

#define HW_FILCO_CMR_SETMASK (((1<<HW_FILCO_CMR_WIDTH)-1)<<HW_FILCO_CMR_BITPOS)

#define HW_FILCO_CMR_CLRMASK (~(WORD)HW_FILCO_CMR_SETMASK)

typedef union               
{
    struct {
        int CMR	: HW_FILCO_CMR_WIDTH;
    } B;
    int I;
} filcocmr_type;
#define HW_FILCO_CMR      (*(volatile filcocmr_type _X*) (HW_FILCO_BASEADDR+3))    /* FILCO Coef Modulus Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_CCPR) Bit Definitions
#define HW_FILCO_CCPR_BITPOS (0)

#define HW_FILCO_CCPR_WIDTH (8)

#define HW_FILCO_CCPR_SETMASK (((1<<HW_FILCO_CCPR_WIDTH-1)<<HW_FILCO_CCPR_BITPOS))

#define HW_FILCO_CCPR_CLRMASK (~(WORD)HW_FILCO_CCPR_SETMASK)

typedef union               
{
    struct {
        int CCPR	: HW_FILCO_CCPR_WIDTH;
    } B;
    int I;
} filcoccpr_type;
#define HW_FILCO_CCPR      (*(volatile filcoccpr_type _X*) (HW_FILCO_BASEADDR+4))    /* FILCO Coef Current Position Register */


/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_S0) Bit Definitions
#define HW_FILCO_S0_BITPOS (0)

#define HW_FILCO_S0_WIDTH (24)

#define HW_FILCO_S0_SETMASK (((1<<HW_FILCO_S0_WIDTH-1)<<HW_FILCO_S0_BITPOS))

#define HW_FILCO_S0_CLRMASK (~(WORD)HW_FILCO_S0_SETMASK)

typedef union               
{
    struct {
        int S0		: HW_FILCO_S0_WIDTH;
    } B;
    int I;
} filcos0_type;
#define HW_FILCO_S0      (*(volatile filcos0_type _X*) (HW_FILCO_BASEADDR+5))    /* FILCO Sample addr S0 Register */


/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_S1) Bit Definitions
#define HW_FILCO_S1_BITPOS (0)

#define HW_FILCO_S1_WIDTH (24)

#define HW_FILCO_S1_SETMASK (((1<<HW_FILCO_S1_WIDTH-1)<<HW_FILCO_S1_BITPOS))

#define HW_FILCO_S1_CLRMASK (~(WORD)HW_FILCO_S1_SETMASK)

typedef union               
{
    struct {
        int S1		: HW_FILCO_S1_WIDTH;
    } B;
    int I;
} filcos1_type;
#define HW_FILCO_S1      (*(volatile filcos1_type _X*) (HW_FILCO_BASEADDR+6))    /* FILCO Sample addr S1 Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_S2) Bit Definitions
#define HW_FILCO_S2_BITPOS (0)

#define HW_FILCO_S2_WIDTH (24)

#define HW_FILCO_S2_SETMASK (((1<<HW_FILCO_S2_WIDTH-1)<<HW_FILCO_S2_BITPOS))

#define HW_FILCO_S2_CLRMASK (~(WORD)HW_FILCO_S2_SETMASK)

typedef union               
{
    struct {
        int S2		: HW_FILCO_S2_WIDTH;
    } B;
    int I;
} filcos2_type;
#define HW_FILCO_S2      (*(volatile filcos2_type _X*) (HW_FILCO_BASEADDR+7))    /* FILCO Sample addr S2 Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_S3) Bit Definitions
#define HW_FILCO_S3_BITPOS (0)

#define HW_FILCO_S3_WIDTH (24)

#define HW_FILCO_S3_SETMASK (((1<<HW_FILCO_S3_WIDTH-1)<<HW_FILCO_S3_BITPOS))

#define HW_FILCO_S3_CLRMASK (~(WORD)HW_FILCO_S3_SETMASK)

typedef union               
{
    struct {
        int S3		: HW_FILCO_S3_WIDTH;
    } B;
    int I;
} filcos3_type;
#define HW_FILCO_S3      (*(volatile filcos3_type _X*) (HW_FILCO_BASEADDR+8))    /* FILCO Sample addr S3 Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_S4) Bit Definitions
#define HW_FILCO_S4_BITPOS (0)

#define HW_FILCO_S4_WIDTH (24)

#define HW_FILCO_S4_SETMASK (((1<<HW_FILCO_S4_WIDTH-1)<<HW_FILCO_S4_BITPOS))

#define HW_FILCO_S4_CLRMASK (~(WORD)HW_FILCO_S4_SETMASK)

typedef union               
{
    struct {
        int S4		: HW_FILCO_S4_WIDTH;
    } B;
    int I;
} filcos4_type;
#define HW_FILCO_S4      (*(volatile filcos4_type _X*) (HW_FILCO_BASEADDR+9))    /* FILCO Sample addr S4 Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_S5) Bit Definitions
#define HW_FILCO_S5_BITPOS (0)

#define HW_FILCO_S5_WIDTH (24)

#define HW_FILCO_S5_SETMASK (((1<<HW_FILCO_S5_WIDTH)-1)<<HW_FILCO_S5_BITPOS)

#define HW_FILCO_S5_CLRMASK (~(WORD)HW_FILCO_S5_SETMASK)

typedef union               
{
    struct {
        int S5		: HW_FILCO_S5_WIDTH;
    } B;
    int I;
} filcos5_type;
#define HW_FILCO_S5      (*(volatile filcos5_type _X*) (HW_FILCO_BASEADDR+10))    /* FILCO Sample addr S5 Register */


/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_INTR) Bit Definitions
#define HW_FILCO_INTR_IRQEN_BITPOS (0)
#define HW_FILCO_INTR_IRQ_BITPOS (1)

#define HW_FILCO_INTR_IRQEN_WIDTH (1)
#define HW_FILCO_INTR_IRQ_WIDTH (1)

#define HW_FILCO_INTR_IRQEN_SETMASK (((1<<HW_FILCO_INTR_IRQEN_WIDTH)-1)<<HW_FILCO_INTR_IRQEN_BITPOS)
#define HW_FILCO_INTR_IRQ_SETMASK (((1<<HW_FILCO_INTR_IRQ_WIDTH)-1)<<HW_FILCO_INTR_IRQ_BITPOS)

#define HW_FILCO_INTR_IRQEN_CLRMASK (~(WORD)HW_FILCO_INTR_IRQEN_SETMASK)
#define HW_FILCO_INTR_IRQ_CLRMASK (~(WORD)HW_FILCO_INTR_IRQ_SETMASK)

typedef union               
{
    struct {
        int IRQEN	: HW_FILCO_INTR_IRQEN_WIDTH;
        int IRQ	: HW_FILCO_INTR_IRQ_WIDTH;
    } B;
    int I;
} filcointr_type;
#define HW_FILCO_INTR      (*(volatile filcokick_type _X*) (HW_FILCO_BASEADDR+11))    /* FILCO Interupt Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_KICK) Bit Definitions
#define HW_FILCO_KICK_KICK_BITPOS (0)
#define HW_FILCO_KICK_UNKICK_BITPOS (1)

#define HW_FILCO_KICK_KICK_WIDTH (1)
#define HW_FILCO_KICK_UNKICK_WIDTH (1)

#define HW_FILCO_KICK_KICK_SETMASK (((1<<HW_FILCO_KICK_KICK_WIDTH)-1)<<HW_FILCO_KICK_KICK_BITPOS)
#define HW_FILCO_KICK_UNKICK_SETMASK (((1<<HW_FILCO_KICK_UNKICK_WIDTH)-1)<<HW_FILCO_KICK_UNKICK_BITPOS)

#define HW_FILCO_KICK_KICK_CLRMASK (~(WORD)HW_FILCO_KICK_KICK_SETMASK)
#define HW_FILCO_KICK_UNKICK_CLRMASK (~(WORD)HW_FILCO_KICK_UNKICK_SETMASK)

typedef union               
{
    struct {
        int KICK		: HW_FILCO_KICK_KICK_WIDTH;
        int UNKICK		: HW_FILCO_KICK_UNKICK_WIDTH;
    } B;
    int I;
} filcoKICK_type;
#define HW_FILCO_KICK      (*(volatile filcoKICK_type _X*) (HW_FILCO_BASEADDR+12))    /* FILCO Sample addr KICK Register */




/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_DBAR) Bit Definitions
#define HW_FILCO_DBAR_BITPOS (0)

#define HW_FILCO_DBAR_WIDTH (16)

#define HW_FILCO_DBAR_SETMASK (((1<<HW_FILCO_DBAR_WIDTH)-1)<<HW_FILCO_DBAR_BITPOS)

#define HW_FILCO_DBAR_CLRMASK (~(WORD)HW_FILCO_DBAR_SETMASK)

typedef union               
{
    struct {
        int DBAR	: HW_FILCO_DBAR_WIDTH;
    } B;
    int I;
} filcodbar_type;
#define HW_FILCO_DBAR      (*(volatile filcodbar_type _X*) (HW_FILCO_BASEADDR+13))    /* FILCO Input Sample Base Address */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_DMR) Bit Definitions
#define HW_FILCO_DMR_BITPOS (0)

#define HW_FILCO_DMR_WIDTH (13)

#define HW_FILCO_DMR_SETMASK (((1<<HW_FILCO_DMR_WIDTH)-1)<<HW_FILCO_DMR_BITPOS)

#define HW_FILCO_DMR_CLRMASK (~(WORD)HW_FILCO_DMR_SETMASK)

typedef union               
{
    struct {
        int DMR		: HW_FILCO_DMR_WIDTH;
    } B;
    int I;
} filcodmr_type;
#define HW_FILCO_DMR      (*(volatile filcodmr_type _X*) (HW_FILCO_BASEADDR+14))    /* FILCO Input Sample Modulus Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_DCPR) Bit Definitions
#define HW_FILCO_DCPR_BITPOS (0)

#define HW_FILCO_DCPR_WIDTH (13)

#define HW_FILCO_DCPR_SETMASK (((1<<HW_FILCO_DCPR_WIDTH)-1)<<HW_FILCO_DCPR_BITPOS)

#define HW_FILCO_DCPR_CLRMASK (~(WORD)HW_FILCO_DCPR_SETMASK)

typedef union               
{
    struct {
        int DCPR		: HW_FILCO_DCPR_WIDTH;
    } B;
    int I;
} filcodcpr_type;
#define HW_FILCO_DCPR      (*(volatile filcodcpr_type _X*) (HW_FILCO_BASEADDR+15))    /* FILCO Input Sample Current Position Register */


/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_SBAR) Bit Definitions
#define HW_FILCO_SBAR_BITPOS (0)

#define HW_FILCO_SBAR_WIDTH (16)

#define HW_FILCO_SBAR_SETMASK (((1<<HW_FILCO_SBAR_WIDTH)-1)<<HW_FILCO_SBAR_BITPOS)

#define HW_FILCO_SBAR_CLRMASK (~(WORD)HW_FILCO_SBAR_SETMASK)

typedef union               
{
    struct {
        int SBAR		: HW_FILCO_SBAR_WIDTH;
    } B;
    int I;
} filcosbar_type;
#define HW_FILCO_SBAR      (*(volatile filcosbar_type _X*) (HW_FILCO_BASEADDR+16))    /* FILCO Output Sample Base Address */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_SMR) Bit Definitions
#define HW_FILCO_SMR_BITPOS (0)

#define HW_FILCO_SMR_WIDTH (13)

#define HW_FILCO_SMR_SETMASK (((1<<HW_FILCO_SMR_WIDTH)-1)<<HW_FILCO_SMR_BITPOS)

#define HW_FILCO_SMR_CLRMASK (~(WORD)HW_FILCO_SMR_SETMASK)

typedef union               
{
    struct {
        int SMR		: HW_FILCO_SMR_WIDTH;
    } B;
    int I;
} filcosmr_type;
#define HW_FILCO_SMR      (*(volatile filcosmr_type _X*) (HW_FILCO_BASEADDR+17))    /* FILCO Output Sample Modulus Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_SCPR) Bit Definitions
#define HW_FILCO_SCPR_BITPOS (0)

#define HW_FILCO_SCPR_WIDTH (13)

#define HW_FILCO_SCPR_SETMASK (((1<<HW_FILCO_SCPR_WIDTH)-1)<<HW_FILCO_SCPR_BITPOS)

#define HW_FILCO_SCPR_CLRMASK (~(WORD)HW_FILCO_SCPR_SETMASK)

typedef union               
{
    struct {
        int SCPR		: HW_FILCO_SCPR_WIDTH;
    } B;
    int I;
} filcoscpr_type;
#define HW_FILCO_SCPR      (*(volatile filcoscpr_type _X*) (HW_FILCO_BASEADDR+18))    /* FILCO Output Sample Current Position Register */




/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_IIRGAIN) Bit Definitions
#define HW_FILCO_IIRGAIN_BITPOS (0)

#define HW_FILCO_IIRGAIN_WIDTH (24)

#define HW_FILCO_IIRGAIN_SETMASK (((1<<HW_FILCO_IIRGAIN_WIDTH)-1)<<HW_FILCO_IIRGAIN_BITPOS)

#define HW_FILCO_IIRGAIN_CLRMASK (~(WORD)HW_FILCO_IIRGAIN_SETMASK)

typedef union               
{
    struct {
        int C5			:HW_FILCO_IIRGAIN_WIDTH;
    } B;
    int I;
} filcoc5_type;
#define HW_FILCO_IIRGAIN      (*(volatile filcoc5_type _X*) (HW_FILCO_BASEADDR+24))    /* FILCO Coef addr C5 Register */


/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_WCNT) Bit Definitions
#define HW_FILCO_WCNT_BITPOS (0)

#define HW_FILCO_WCNT_WIDTH (13)

#define HW_FILCO_WCNT_SETMASK (((1<<HW_FILCO_WCNT_WIDTH)-1)<<HW_FILCO_WCNT_BITPOS)

#define HW_FILCO_WCNT_CLRMASK (~(WORD)HW_FILCO_WCNT_SETMASK)

typedef union               
{
    struct {
        int WCNT		: HW_FILCO_WCNT_WIDTH;
    } B;
    int I;
} filcowcnt_type;
#define HW_FILCO_WCNT      (*(volatile filcowcnt_type _X*) (HW_FILCO_BASEADDR+27))    /* FILCO Word Count Register */


/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_ZC_STATUS) Bit Definitions
#define HW_FILCO_ZC_STATUS_EN_BITPOS (0)
#define HW_FILCO_ZC_STATUS_BITPOS (2)

#define HW_FILCO_ZC_STATUS_EN_WIDTH (2)
#define HW_FILCO_ZC_STATUS_WIDTH (2)

#define HW_FILCO_ZC_STATUS_EN_SETMASK (((1<<HW_FILCO_ZC_STATUS_EN_WIDTH)-1)<<HW_FILCO_ZC_STATUS_EN_BITPOS)
#define HW_FILCO_ZC_STATUS_SETMASK (((1<<HW_FILCO_ZC_STATUS_WIDTH)-1)<<HW_FILCO_ZC_STATUS_BITPOS)

#define HW_FILCO_ZC_STATUS_EN_CLRMASK (((~(WORD)HW_FILCO_ZC_STATUS_EN_WIDTH)-1)<<HW_FILCO_ZC_STATUS_EN_SETMASK)
#define HW_FILCO_ZC_STATUS_CLRMASK (((~(WORD)HW_FILCO_ZC_STATUS_WIDTH)-1)<<HW_FILCO_ZC_STATUS_SETMASK)

typedef union               
{
    struct {
        int ZC_STATUS_EN		: HW_FILCO_ZC_STATUS_EN_WIDTH;
        int ZC_STATUS			: HW_FILCO_ZC_STATUS_WIDTH;
    } B;
    int I;
} filcozc_status_type;
#define HW_FILCO_ZC_STATUS      (*(volatile filcozc_status_type _X*) (HW_FILCO_BASEADDR+28))    /* FILCO Zero Cross Detect Status Register */

/////////////////////////////////////////////////////////////////////////////////
//  FILCO  (HW_FILCO_ZC_GAIN) Bit Definitions
#define HW_FILCO_ZC_GAIN_BITPOS (0)

#define HW_FILCO_ZC_GAIN_WIDTH (24)

#define HW_FILCO_ZC_GAIN_SETMASK (((1<<HW_FILCO_ZC_GAIN_WIDTH)-1)<<HW_FILCO_ZC_GAIN_BITPOS)

#define HW_FILCO_ZC_GAIN_CLRMASK (~(WORD)HW_FILCO_ZC_GAIN_SETMASK)

typedef union               
{
    struct {
        int ZC_GAIN		: HW_FILCO_ZC_GAIN_WIDTH;
    } B;
    int I;
} filcozc_gain_type;
#define HW_FILCO_ZC_GAIN      (*(volatile filcozc_gain_type _X*) (HW_FILCO_BASEADDR+29))    /* FILCO Word Count Register */

#endif























