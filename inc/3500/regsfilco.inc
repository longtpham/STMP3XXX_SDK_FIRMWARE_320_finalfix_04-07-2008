;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2003
;;  File        : regsfilco.inc
;;  Description : FILCO IP Register definition
;; Last Edited 2.26.2003 Rod Holley
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; The following naming conventions are followed in this file.
; All registers are named using the format...
;     HW_<module>_<regname>
; where <module> is the module name which can be any of the following...
;     FILCO
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

    if !(@def(regsfilcoinc))
regsfilcoinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;   FILCO STMP Registers 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
HW_FILCO_BASEADDR                       equ     ($FB80)

;///////////////////////////////////////////////////////////////////////////////
;  FILCO Registers
;///////////////////////////////////////////////////////////////////////////////

;ASM Only
HW_FILCO_CSR                            equ     (HW_FILCO_BASEADDR)
HW_FILCO_SAT                            equ     (HW_FILCO_BASEADDR+1)
HW_FILCO_CBAR                           equ     (HW_FILCO_BASEADDR+2)
HW_FILCO_CMR                            equ     (HW_FILCO_BASEADDR+3)
HW_FILCO_CCPR                           equ     (HW_FILCO_BASEADDR+4)
HW_FILCO_INTR                           equ     (HW_FILCO_BASEADDR+11)
HW_FILCO_KICK                           equ     (HW_FILCO_BASEADDR+12)
HW_FILCO_DBAR                           equ     (HW_FILCO_BASEADDR+13)
HW_FILCO_DMR                            equ     (HW_FILCO_BASEADDR+14)
HW_FILCO_DCPR                           equ     (HW_FILCO_BASEADDR+15)
HW_FILCO_SBAR                           equ     (HW_FILCO_BASEADDR+16)
HW_FILCO_SMR                            equ     (HW_FILCO_BASEADDR+17)
HW_FILCO_SCPR                           equ     (HW_FILCO_BASEADDR+18)
HW_FILCO_IIRGAIN                        equ     (HW_FILCO_BASEADDR+24)
HW_FILCO_WCNT                           equ     (HW_FILCO_BASEADDR+27)
HW_FILCO_ZC_STATUS                      equ     (HW_FILCO_BASEADDR+28)
HW_FILCO_ZC_GAIN                        equ     (HW_FILCO_BASEADDR+29)
;endasm

;///////////////////////////////////////////////////////////////////////////////
;  FILCO Control Status Register (HW_FILCO_CSR) Bit Definitions
HW_FILCO_CSR_DISABLE_BITPOS             equ     (0)
HW_FILCO_CSR_DMAOUT_BITPOS              equ     (2)
HW_FILCO_CSR_DMASAMP_BITPOS             equ     (3)
HW_FILCO_CSR_DMACOEF_BITPOS             equ     (6)
HW_FILCO_CSR_FILCOSEL_BITPOS            equ     (11)
HW_FILCO_CSR_IIRMODE_BITPOS             equ     (12)
HW_FILCO_CSR_IIRSCALE_BITPOS            equ     (13)
HW_FILCO_CSR_FIRMODE_BITPOS             equ     (14)
HW_FILCO_CSR_CLKDIV_BITPOS              equ     (16)
HW_FILCO_CSR_CLKGT_BITPOS               equ     (23)

HW_FILCO_CSR_DISABLE_WIDTH              equ     (1)
HW_FILCO_CSR_DMAOUT_WIDTH               equ     (2)
HW_FILCO_CSR_DMASAMP_WIDTH              equ     (2)
HW_FILCO_CSR_DMACOEF_WIDTH              equ     (2)
HW_FILCO_CSR_FILCOSEL_WIDTH             equ     (1)
HW_FILCO_CSR_IIRMODE_WIDTH              equ     (1)
HW_FILCO_CSR_IIRSCALE_WIDTH             equ     (1)
HW_FILCO_CSR_FIRMODE_WIDTH              equ     (2)
HW_FILCO_CSR_CLKDIV_WIDTH               equ     (4)
HW_FILCO_CSR_CLKGT_WIDTH                equ     (1)

HW_FILCO_CSR_DISABLE_SETMASK            equ     (((1<<HW_FILCO_CSR_DISABLE_WIDTH)-1)<<HW_FILCO_CSR_DISABLE_BITPOS)
HW_FILCO_CSR_DMAOUT_SETMASK             equ     (((1<<HW_FILCO_CSR_DMAOUT_WIDTH)-1)<<HW_FILCO_CSR_DMAOUT_BITPOS)
HW_FILCO_CSR_DMASAMP_SETMASK            equ     (((1<<HW_FILCO_CSR_DMASAMP_WIDTH)-1)<<HW_FILCO_CSR_DMASAMP_BITPOS)
HW_FILCO_CSR_DMACOEF_SETMASK            equ     (((1<<HW_FILCO_CSR_DMACOEF_WIDTH)-1)<<HW_FILCO_CSR_DMACOEF_BITPOS)
HW_FILCO_CSR_FILCOSEL_SETMASK           equ     (((1<<HW_FILCO_CSR_FILCOSEL_WIDTH)-1)<<HW_FILCO_CSR_FILCOSEL_BITPOS)
HW_FILCO_CSR_IIRMODE_SETMASK            equ     (((1<<HW_FILCO_CSR_IIRMODE_WIDTH)-1)<<HW_FILCO_CSR_IIRMODE_BITPOS)
HW_FILCO_CSR_IIRSCALE_SETMASK           equ     (((1<<HW_FILCO_CSR_IIRSCALE_WIDTH)-1)<<HW_FILCO_CSR_IIRSCALE_BITPOS)
HW_FILCO_CSR_FIRMODE_SETMASK            equ     (((1<<HW_FILCO_CSR_FIRMODE_WIDTH)-1)<<HW_FILCO_CSR_FIRMODE_BITPOS)
HW_FILCO_CSR_CLKDIV_SETMASK             equ     (((1<<HW_FILCO_CSR_CLKDIV_WIDTH)-1)<<HW_FILCO_CSR_CLKDIV_BITPOS)
HW_FILCO_CSR_CLKGT_SETMASK              equ     (((1<<HW_FILCO_CSR_CLKGT_WIDTH)-1)<<HW_FILCO_CSR_CLKGT_BITPOS)

HW_FILCO_CSR_DISABLE_CLRMASK            equ     (~HW_FILCO_CSR_DISABLE_SETMASK)
HW_FILCO_CSR_DMAOUT_CLRMASK             equ     (~HW_FILCO_CSR_DMAOUT_SETMASK)
HW_FILCO_CSR_DMASAMP_CLRMASK            equ     (~HW_FILCO_CSR_DMASAMP_SETMASK)
HW_FILCO_CSR_DMACOEF_CLRMASK            equ     (~HW_FILCO_CSR_DMACOEF_SETMASK)
HW_FILCO_CSR_FILCOSEL_CLRMASK           equ     (~HW_FILCO_CSR_FILCOSEL_SETMASK)
HW_FILCO_CSR_IIRMODE_CLRMASK            equ     (~HW_FILCO_CSR_IIRMODE_SETMASK)
HW_FILCO_CSR_IIRSCALE_CLRMASK           equ     (~HW_FILCO_CSR_IIRSCALE_SETMASK)
HW_FILCO_CSR_FIRMODE_CLRMASK            equ     (~HW_FILCO_CSR_FIRMODE_SETMASK)
HW_FILCO_CSR_CLKDIV_CLRMASK             equ     (~HW_FILCO_CSR_CLKDIV_SETMASK)
HW_FILCO_CSR_CLKGT_CLRMASK              equ     (~HW_FILCO_CSR_CLKGT_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int DISABLE  	: HW_FILCO_CSR_DISABLE_WIDTH ;
;        int DMAOUT  	: HW_FILCO_CSR_DMAOUT_WIDTH ;
;        int DMASAMP  	: HW_FILCO_CSR_DMASAMP_WIDTH ;  
;        int DMACOEF  	: HW_FILCO_CSR_DMACOEF_WIDTH ;   
;        int FILCOSEL  	: HW_FILCO_CSR_FILCOSEL_WIDTH ;  
;        int IIRMODE  	: HW_FILCO_CSR_IIRMODE_WIDTH ;   
;        int IIRSCALE  	: HW_FILCO_CSR_IIRSCALE_WIDTH ;  
;        int FIRMODE  	: HW_FILCO_CSR_FIRMODE_WIDTH ;   
;        int CLKDIV  	: HW_FILCO_CSR_CLKDIV_WIDTH ;    
;        int CLKGT  	: HW_FILCO_CSR_CLKGT_WIDTH ;     
;    } B;
;    int I;
;} filcocsr_type;

;#define HW_FILCO_CSR      (*(volatile filcocsr_type _X*) (HW_FILCO_BASEADDR))    /* FILCO Control / Status Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_SAT) Bit Definitions
HW_FILCO_SAT_BITPOS             equ     (0)

HW_FILCO_SAT_WIDTH              equ     (8)

HW_FILCO_SAT_SETMASK            equ     (((1<<HW_FILCO_SAT_WIDTH)-1)<<HW_FILCO_SAT_BITPOS)

HW_FILCO_SAT_CLRMASK            equ     (~HW_FILCO_SAT_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int SAT  	: HW_FILCO_SAT_WIDTH ;
;    } B;
;    int I;
;} filcosat_type;

;#define HW_FILCOSAT      (*(volatile filcosat_type _X*) (HW_FILCO_BASEADDR+1))    /* FILCO Saturation Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_CBAR) Bit Definitions
HW_FILCO_CBAR_BITPOS             equ     (0)

HW_FILCO_CBAR_WIDTH              equ     (16)

HW_FILCO_CBAR_SETMASK            equ     (((1<<HW_FILCO_CBAR_WIDTH)-1)<<HW_FILCO_CBAR_BITPOS)

HW_FILCO_CBAR_CLRMASK            equ     (~HW_FILCO_CBAR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int CBAR	: HW_FILCO_CBAR_WIDTH;
;    } B;
;    int I;
;} filcocbar_type;

;#define HW_FILCO_CBAR      (*(volatile filcocbar_type _X*) (HW_FILCO_BASEADDR+2))    /* FILCO Coef Base Address */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_CMR) Bit Definitions
HW_FILCO_CMR_BITPOS             equ     (0)

HW_FILCO_CMR_WIDTH              equ     (8)

HW_FILCO_CMR_SETMASK            equ     (((1<<HW_FILCO_CMR_WIDTH)-1)<<HW_FILCO_CMR_BITPOS)

HW_FILCO_CMR_CLRMASK            equ     (~HW_FILCO_CMR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int CMR	: HW_FILCO_CMR_WIDTH;
;    } B;
;    int I;
;} filcocmr_type;

;#define HW_FILCO_CMR      (*(volatile filcocmr_type _X*) (HW_FILCO_BASEADDR+3))    /* FILCO Coef Modulus Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_CCPR) Bit Definitions
HW_FILCO_CCPR_BITPOS             equ     (0)

HW_FILCO_CCPR_WIDTH              equ     (8)

HW_FILCO_CCPR_SETMASK            equ     (((1<<HW_FILCO_CCPR_WIDTH-1)<<HW_FILCO_CCPR_BITPOS))

HW_FILCO_CCPR_CLRMASK            equ     (~HW_FILCO_CCPR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int CCPR	: HW_FILCO_CCPR_WIDTH;
;    } B;
;    int I;
;} filcoccpr_type;

;#define HW_FILCO_CCPR      (*(volatile filcoccpr_type _X*) (HW_FILCO_BASEADDR+4))    /* FILCO Coef Current Position Register */
;endc


;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_S0) Bit Definitions
HW_FILCO_S0_BITPOS             equ     (0)

HW_FILCO_S0_WIDTH              equ     (24)

HW_FILCO_S0_SETMASK            equ     (((1<<HW_FILCO_S0_WIDTH-1)<<HW_FILCO_S0_BITPOS))

HW_FILCO_S0_CLRMASK            equ     (~HW_FILCO_S0_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int S0		: HW_FILCO_S0_WIDTH;
;    } B;
;    int I;
;} filcos0_type;

;#define HW_FILCO_S0      (*(volatile filcos0_type _X*) (HW_FILCO_BASEADDR+5))    /* FILCO Sample addr S0 Register */
;endc


;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_S1) Bit Definitions
HW_FILCO_S1_BITPOS             equ     (0)

HW_FILCO_S1_WIDTH              equ     (24)

HW_FILCO_S1_SETMASK            equ     (((1<<HW_FILCO_S1_WIDTH-1)<<HW_FILCO_S1_BITPOS))

HW_FILCO_S1_CLRMASK            equ     (~HW_FILCO_S1_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int S1		: HW_FILCO_S1_WIDTH;
;    } B;
;    int I;
;} filcos1_type;

;#define HW_FILCO_S1      (*(volatile filcos1_type _X*) (HW_FILCO_BASEADDR+6))    /* FILCO Sample addr S1 Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_S2) Bit Definitions
HW_FILCO_S2_BITPOS             equ     (0)

HW_FILCO_S2_WIDTH              equ     (24)

HW_FILCO_S2_SETMASK            equ     (((1<<HW_FILCO_S2_WIDTH-1)<<HW_FILCO_S2_BITPOS))

HW_FILCO_S2_CLRMASK            equ     (~HW_FILCO_S2_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int S2		: HW_FILCO_S2_WIDTH;
;    } B;
;    int I;
;} filcos2_type;

;#define HW_FILCO_S2      (*(volatile filcos2_type _X*) (HW_FILCO_BASEADDR+7))    /* FILCO Sample addr S2 Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_S3) Bit Definitions
HW_FILCO_S3_BITPOS             equ     (0)

HW_FILCO_S3_WIDTH              equ     (24)

HW_FILCO_S3_SETMASK            equ     (((1<<HW_FILCO_S3_WIDTH-1)<<HW_FILCO_S3_BITPOS))

HW_FILCO_S3_CLRMASK            equ     (~HW_FILCO_S3_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int S3		: HW_FILCO_S3_WIDTH;
;    } B;
;    int I;
;} filcos3_type;

;#define HW_FILCO_S3      (*(volatile filcos3_type _X*) (HW_FILCO_BASEADDR+8))    /* FILCO Sample addr S3 Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_S4) Bit Definitions
HW_FILCO_S4_BITPOS             equ     (0)

HW_FILCO_S4_WIDTH              equ     (24)

HW_FILCO_S4_SETMASK            equ     (((1<<HW_FILCO_S4_WIDTH-1)<<HW_FILCO_S4_BITPOS))

HW_FILCO_S4_CLRMASK            equ     (~HW_FILCO_S4_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int S4		: HW_FILCO_S4_WIDTH;
;    } B;
;    int I;
;} filcos4_type;

;#define HW_FILCO_S4      (*(volatile filcos4_type _X*) (HW_FILCO_BASEADDR+9))    /* FILCO Sample addr S4 Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_S5) Bit Definitions
HW_FILCO_S5_BITPOS             equ     (0)

HW_FILCO_S5_WIDTH              equ     (24)

HW_FILCO_S5_SETMASK            equ     (((1<<HW_FILCO_S5_WIDTH)-1)<<HW_FILCO_S5_BITPOS)

HW_FILCO_S5_CLRMASK            equ     (~HW_FILCO_S5_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int S5		: HW_FILCO_S5_WIDTH;
;    } B;
;    int I;
;} filcos5_type;

;#define HW_FILCO_S5      (*(volatile filcos5_type _X*) (HW_FILCO_BASEADDR+10))    /* FILCO Sample addr S5 Register */
;endc


;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_INTR) Bit Definitions
HW_FILCO_INTR_IRQEN_BITPOS       equ     (0)
HW_FILCO_INTR_IRQ_BITPOS         equ     (1)

HW_FILCO_INTR_IRQEN_WIDTH        equ     (1)
HW_FILCO_INTR_IRQ_WIDTH          equ     (1)

HW_FILCO_INTR_IRQEN_SETMASK      equ     (((1<<HW_FILCO_INTR_IRQEN_WIDTH)-1)<<HW_FILCO_INTR_IRQEN_BITPOS)
HW_FILCO_INTR_IRQ_SETMASK        equ     (((1<<HW_FILCO_INTR_IRQ_WIDTH)-1)<<HW_FILCO_INTR_IRQ_BITPOS)
                                 
HW_FILCO_INTR_IRQEN_CLRMASK      equ     (~HW_FILCO_INTR_IRQEN_SETMASK)
HW_FILCO_INTR_IRQ_CLRMASK        equ     (~HW_FILCO_INTR_IRQ_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int IRQEN	: HW_FILCO_INTR_IRQEN_WIDTH;
;        int IRQ	: HW_FILCO_INTR_IRQ_WIDTH;
;    } B;
;    int I;
;} filcointr_type;

;#define HW_FILCO_INTR      (*(volatile filcokick_type _X*) (HW_FILCO_BASEADDR+11))    /* FILCO Interupt Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_KICK) Bit Definitions
HW_FILCO_KICK_KICK_BITPOS       equ     (0)
HW_FILCO_KICK_UNKICK_BITPOS     equ     (1)

HW_FILCO_KICK_KICK_WIDTH        equ     (1)
HW_FILCO_KICK_UNKICK_WIDTH      equ     (1)

HW_FILCO_KICK_KICK_SETMASK      equ     (((1<<HW_FILCO_KICK_KICK_WIDTH)-1)<<HW_FILCO_KICK_KICK_BITPOS)
HW_FILCO_KICK_UNKICK_SETMASK    equ     (((1<<HW_FILCO_KICK_UNKICK_WIDTH)-1)<<HW_FILCO_KICK_UNKICK_BITPOS)
                                 
HW_FILCO_KICK_KICK_CLRMASK      equ     (~HW_FILCO_KICK_KICK_SETMASK)
HW_FILCO_KICK_UNKICK_CLRMASK    equ     (~HW_FILCO_KICK_UNKICK_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int KICK		: HW_FILCO_KICK_KICK_WIDTH;
;        int UNKICK		: HW_FILCO_KICK_UNKICK_WIDTH;
;    } B;
;    int I;
;} filcoKICK_type;

;#define HW_FILCO_KICK      (*(volatile filcoKICK_type _X*) (HW_FILCO_BASEADDR+12))    /* FILCO Sample addr KICK Register */
;endc




;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_DBAR) Bit Definitions
HW_FILCO_DBAR_BITPOS             equ     (0)

HW_FILCO_DBAR_WIDTH              equ     (16)

HW_FILCO_DBAR_SETMASK            equ     (((1<<HW_FILCO_DBAR_WIDTH)-1)<<HW_FILCO_DBAR_BITPOS)

HW_FILCO_DBAR_CLRMASK            equ     (~HW_FILCO_DBAR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int DBAR	: HW_FILCO_DBAR_WIDTH;
;    } B;
;    int I;
;} filcodbar_type;

;#define HW_FILCO_DBAR      (*(volatile filcodbar_type _X*) (HW_FILCO_BASEADDR+13))    /* FILCO Input Sample Base Address */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_DMR) Bit Definitions
HW_FILCO_DMR_BITPOS             equ     (0)

HW_FILCO_DMR_WIDTH              equ     (13)

HW_FILCO_DMR_SETMASK            equ     (((1<<HW_FILCO_DMR_WIDTH)-1)<<HW_FILCO_DMR_BITPOS)

HW_FILCO_DMR_CLRMASK            equ     (~HW_FILCO_DMR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int DMR		: HW_FILCO_DMR_WIDTH;
;    } B;
;    int I;
;} filcodmr_type;

;#define HW_FILCO_DMR      (*(volatile filcodmr_type _X*) (HW_FILCO_BASEADDR+14))    /* FILCO Input Sample Modulus Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_DCPR) Bit Definitions
HW_FILCO_DCPR_BITPOS             equ     (0)

HW_FILCO_DCPR_WIDTH              equ     (13)

HW_FILCO_DCPR_SETMASK            equ     (((1<<HW_FILCO_DCPR_WIDTH)-1)<<HW_FILCO_DCPR_BITPOS)

HW_FILCO_DCPR_CLRMASK            equ     (~HW_FILCO_DCPR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int DCPR		: HW_FILCO_DCPR_WIDTH;
;    } B;
;    int I;
;} filcodcpr_type;

;#define HW_FILCO_DCPR      (*(volatile filcodcpr_type _X*) (HW_FILCO_BASEADDR+15))    /* FILCO Input Sample Current Position Register */
;endc


;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_SBAR) Bit Definitions
HW_FILCO_SBAR_BITPOS             equ     (0)

HW_FILCO_SBAR_WIDTH              equ     (16)

HW_FILCO_SBAR_SETMASK            equ     (((1<<HW_FILCO_SBAR_WIDTH)-1)<<HW_FILCO_SBAR_BITPOS)

HW_FILCO_SBAR_CLRMASK            equ     (~HW_FILCO_SBAR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int SBAR		: HW_FILCO_SBAR_WIDTH;
;    } B;
;    int I;
;} filcosbar_type;

;#define HW_FILCO_SBAR      (*(volatile filcosbar_type _X*) (HW_FILCO_BASEADDR+16))    /* FILCO Output Sample Base Address */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_SMR) Bit Definitions
HW_FILCO_SMR_BITPOS             equ     (0)

HW_FILCO_SMR_WIDTH              equ     (13)

HW_FILCO_SMR_SETMASK            equ     (((1<<HW_FILCO_SMR_WIDTH)-1)<<HW_FILCO_SMR_BITPOS)

HW_FILCO_SMR_CLRMASK            equ     (~HW_FILCO_SMR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int SMR		: HW_FILCO_SMR_WIDTH;
;    } B;
;    int I;
;} filcosmr_type;

;#define HW_FILCO_SMR      (*(volatile filcosmr_type _X*) (HW_FILCO_BASEADDR+17))    /* FILCO Output Sample Modulus Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_SCPR) Bit Definitions
HW_FILCO_SCPR_BITPOS             equ     (0)

HW_FILCO_SCPR_WIDTH              equ     (13)

HW_FILCO_SCPR_SETMASK            equ     (((1<<HW_FILCO_SCPR_WIDTH)-1)<<HW_FILCO_SCPR_BITPOS)

HW_FILCO_SCPR_CLRMASK            equ     (~HW_FILCO_SCPR_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int SCPR		: HW_FILCO_SCPR_WIDTH;
;    } B;
;    int I;
;} filcoscpr_type;

;#define HW_FILCO_SCPR      (*(volatile filcoscpr_type _X*) (HW_FILCO_BASEADDR+18))    /* FILCO Output Sample Current Position Register */
;endc




;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_IIRGAIN) Bit Definitions
HW_FILCO_IIRGAIN_BITPOS             equ     (0)

HW_FILCO_IIRGAIN_WIDTH              equ     (24)

HW_FILCO_IIRGAIN_SETMASK            equ     (((1<<HW_FILCO_IIRGAIN_WIDTH)-1)<<HW_FILCO_IIRGAIN_BITPOS)

HW_FILCO_IIRGAIN_CLRMASK            equ     (~HW_FILCO_IIRGAIN_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int C5			:HW_FILCO_IIRGAIN_WIDTH;
;    } B;
;    int I;
;} filcoc5_type;

;#define HW_FILCO_IIRGAIN      (*(volatile filcoc5_type _X*) (HW_FILCO_BASEADDR+24))    /* FILCO Coef addr C5 Register */
;endc


;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_WCNT) Bit Definitions
HW_FILCO_WCNT_BITPOS             equ     (0)

HW_FILCO_WCNT_WIDTH              equ     (13)

HW_FILCO_WCNT_SETMASK            equ     (((1<<HW_FILCO_WCNT_WIDTH)-1)<<HW_FILCO_WCNT_BITPOS)

HW_FILCO_WCNT_CLRMASK            equ     (~HW_FILCO_WCNT_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int WCNT		: HW_FILCO_WCNT_WIDTH;
;    } B;
;    int I;
;} filcowcnt_type;

;#define HW_FILCO_WCNT      (*(volatile filcowcnt_type _X*) (HW_FILCO_BASEADDR+27))    /* FILCO Word Count Register */
;endc


;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_ZC_STATUS) Bit Definitions
HW_FILCO_ZC_STATUS_EN_BITPOS          equ     (0)
HW_FILCO_ZC_STATUS_BITPOS             equ     (2)

HW_FILCO_ZC_STATUS_EN_WIDTH           equ     (2)
HW_FILCO_ZC_STATUS_WIDTH              equ     (2)

HW_FILCO_ZC_STATUS_EN_SETMASK         equ     (((1<<HW_FILCO_ZC_STATUS_EN_WIDTH)-1)<<HW_FILCO_ZC_STATUS_EN_BITPOS)
HW_FILCO_ZC_STATUS_SETMASK            equ     (((1<<HW_FILCO_ZC_STATUS_WIDTH)-1)<<HW_FILCO_ZC_STATUS_BITPOS)

HW_FILCO_ZC_STATUS_EN_CLRMASK         equ     (((~HW_FILCO_ZC_STATUS_EN_WIDTH)-1)<<HW_FILCO_ZC_STATUS_EN_SETMASK)
HW_FILCO_ZC_STATUS_CLRMASK            equ     (((~HW_FILCO_ZC_STATUS_WIDTH)-1)<<HW_FILCO_ZC_STATUS_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int ZC_STATUS_EN		: HW_FILCO_ZC_STATUS_EN_WIDTH;
;        int ZC_STATUS			: HW_FILCO_ZC_STATUS_WIDTH;
;    } B;
;    int I;
;} filcozc_status_type;

;#define HW_FILCO_ZC_STATUS      (*(volatile filcozc_status_type _X*) (HW_FILCO_BASEADDR+28))    /* FILCO Zero Cross Detect Status Register */
;endc

;///////////////////////////////////////////////////////////////////////////////
;  FILCO  (HW_FILCO_ZC_GAIN) Bit Definitions
HW_FILCO_ZC_GAIN_BITPOS             equ     (0)

HW_FILCO_ZC_GAIN_WIDTH              equ     (24)

HW_FILCO_ZC_GAIN_SETMASK            equ     (((1<<HW_FILCO_ZC_GAIN_WIDTH)-1)<<HW_FILCO_ZC_GAIN_BITPOS)

HW_FILCO_ZC_GAIN_CLRMASK            equ     (~HW_FILCO_ZC_GAIN_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;        int ZC_GAIN		: HW_FILCO_ZC_GAIN_WIDTH;
;    } B;
;    int I;
;} filcozc_gain_type;

;#define HW_FILCO_ZC_GAIN      (*(volatile filcozc_gain_type _X*) (HW_FILCO_BASEADDR+29))    /* FILCO Word Count Register */
;endc

    endif






















