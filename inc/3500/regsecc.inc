;///////////////////////////////////////////////////////////////////////////////
; Copyright(C) SigmaTel, Inc. 2000-2001
;
; Filename: regsecc.inc
; Description: Register definitions for ECC interface
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
    if !(@def(regseccinc))
regseccinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   ECC STMP Registers 
;   Last Updated 2.26.2003 J. Ferrara
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_ECC_BASEADDR             equ     ($F780)

 

;ASM Only

 
HW_ECC_CSR0                  equ     (HW_ECC_BASEADDR+0)
HW_ECC_CSR1                  equ     (HW_ECC_BASEADDR+1)
HW_ECC_RSCFG                 equ     (HW_ECC_BASEADDR+2)
HW_ECC_SSFDCCFG              equ     (HW_ECC_BASEADDR+3)
HW_ECC_BLKSTRTADDR           equ     (HW_ECC_BASEADDR+4)
HW_ECC_BLKSTRTINDEX          equ     (HW_ECC_BASEADDR+5)
HW_ECC_PARSTRTADDR           equ     (HW_ECC_BASEADDR+6)
HW_ECC_PARSTRTINDEX          equ     (HW_ECC_BASEADDR+7)
HW_ECC_LOCADDR               equ     (HW_ECC_BASEADDR+8)
HW_ECC_LOCINDEX              equ     (HW_ECC_BASEADDR+9)
HW_ECC_ERRVAL                equ     (HW_ECC_BASEADDR+10)

;endasm

;///////////////////////////////////////////////////////////////////////////////

;  ECC Control Status Register0 (HW_ECC_CSR0) Bit Definitions

HW_ECC_CSR0_ECCKICK_BITPOS              equ     (0)
HW_ECC_CSR0_AUTOCORR_BITPOS             equ     (1)
HW_ECC_CSR0_RSVD0_BITPOS                equ     (2)
HW_ECC_CSR0_DONEINTEN_BITPOS            equ     (3)
HW_ECC_CSR0_KESINTEN_BITPOS             equ     (5)
HW_ECC_CSR0_CORRINTEN_BITPOS            equ     (6)
HW_ECC_CSR0_UNCORRINTEN_BITPOS          equ     (7)
HW_ECC_CSR0_ERRINTEN_BITPOS             equ     (8)
HW_ECC_CSR0_RSVD1_BITPOS                equ     (9)
HW_ECC_CSR0_DMAWAIT_BITPOS              equ     (12)
HW_ECC_CSR0_ECCSEL_BITPOS               equ     (16)
HW_ECC_CSR0_RSVD2_BITPOS                equ     (19)
HW_ECC_CSR0_SFTRST_BITPOS               equ     (23)

HW_ECC_CSR0_ECCKICK_WIDTH               equ     (1)        
HW_ECC_CSR0_AUTOCORR_WIDTH              equ     (1)
HW_ECC_CSR0_RSVD0_WIDTH                 equ     (2)
HW_ECC_CSR0_DONEINTEN_WIDTH             equ     (1)
HW_ECC_CSR0_KESINTEN_WIDTH              equ     (1)
HW_ECC_CSR0_CORRINTEN_WIDTH             equ     (1)
HW_ECC_CSR0_UNCORRINTEN_WIDTH           equ     (1)
HW_ECC_CSR0_ERRINTEN_WIDTH              equ     (1)
HW_ECC_CSR0_RSVD1_WIDTH                 equ     (3)
HW_ECC_CSR0_DMAWAIT_WIDTH               equ     (4)
HW_ECC_CSR0_ECCSEL_WIDTH                equ     (3)
HW_ECC_CSR0_RSVD2_WIDTH                 equ     (4)
HW_ECC_CSR0_SFTRST_WIDTH                equ     (1)
   
HW_ECC_CSR0_ECCKICK_SETMASK              equ     (((1<<HW_ECC_CSR0_ECCKICK_WIDTH)-1)<<HW_ECC_CSR0_ECCKICK_BITPOS) 
HW_ECC_CSR0_AUTOCORR_SETMASK             equ     (((1<<HW_ECC_CSR0_AUTOCORR_WIDTH)-1)<<HW_ECC_CSR0_AUTOCORR_BITPOS)
HW_ECC_CSR0_RSVD0_SETMASK                equ     (((1<<HW_ECC_CSR0_RSVD0_WIDTH)-1)<<HW_ECC_CSR0_RSVD0_BITPOS) 
HW_ECC_CSR0_DONEINTEN_SETMASK            equ     (((1<<HW_ECC_CSR0_DONEINTEN_WIDTH)-1)<<HW_ECC_CSR0_DONEINTEN_BITPOS) 
HW_ECC_CSR0_KESINTEN_SETMASK             equ     (((1<<HW_ECC_CSR0_KESINTEN_WIDTH)-1)<<HW_ECC_CSR0_KESINTEN_BITPOS) 
HW_ECC_CSR0_CORRINTEN_SETMASK            equ     (((1<<HW_ECC_CSR0_CORRINTEN_WIDTH)-1)<<HW_ECC_CSR0_CORRINTEN_BITPOS) 
HW_ECC_CSR0_UNCORRINTEN_SETMASK          equ     (((1<<HW_ECC_CSR0_UNCORRINTEN_WIDTH)-1)<<HW_ECC_CSR0_UNCORRINTEN_BITPOS) 
HW_ECC_CSR0_ERRINTEN_SETMASK             equ     (((1<<HW_ECC_CSR0_ERRINTEN_WIDTH)-1)<<HW_ECC_CSR0_ERRINTEN_BITPOS) 
HW_ECC_CSR0_RSVD1_SETMASK                equ     (((1<<HW_ECC_CSR0_RSVD1_WIDTH)-1)<<HW_ECC_CSR0_RSVD1_BITPOS) 
HW_ECC_CSR0_DMAWAIT_SETMASK              equ     (((1<<HW_ECC_CSR0_DMAWAIT_WIDTH)-1)<<HW_ECC_CSR0_DMAWAIT_BITPOS) 
HW_ECC_CSR0_ECCSEL_SETMASK               equ     (((1<<HW_ECC_CSR0_ECCSEL_WIDTH)-1)<<HW_ECC_CSR0_ECCSEL_BITPOS) 
HW_ECC_CSR0_RSVD2_SETMASK                equ     (((1<<HW_ECC_CSR0_RSVD2_WIDTH)-1)<<HW_ECC_CSR0_RSVD2_BITPOS) 
HW_ECC_CSR0_SFTRST_SETMASK               equ     (((1<<HW_ECC_CSR0_SFTRST_WIDTH)-1)<<HW_ECC_CSR0_SFTRST_BITPOS) 
  
HW_ECC_CSR0_ECCKICK_CLRMASK              equ     (~HW_ECC_CSR0_ECCKICK_SETMASK)
HW_ECC_CSR0_AUTOCORR_CLRMASK             equ     (~HW_ECC_CSR0_AUTOCORR_SETMASK)
HW_ECC_CSR0_RSVD0_CLRMASK                equ     (~HW_ECC_CSR0_RSVD0_SETMASK)
HW_ECC_CSR0_DONEINTEN_CLRMASK            equ     (~HW_ECC_CSR0_DONEINTEN_SETMASK)
HW_ECC_CSR0_KESINTEN_CLRMASK             equ     (~HW_ECC_CSR0_KESINTEN_SETMASK)
HW_ECC_CSR0_CORRINTEN_CLRMASK            equ     (~HW_ECC_CSR0_CORRINTEN_SETMASK)
HW_ECC_CSR0_UNCORRINTEN_CLRMASK          equ     (~HW_ECC_CSR0_UNCORRINTEN_SETMASK)
HW_ECC_CSR0_ERRINTEN_CLRMASK             equ     (~HW_ECC_CSR0_ERRINTEN_SETMASK)
HW_ECC_CSR0_RSVD1_CLRMASK                equ     (~HW_ECC_CSR0_RSVD1_SETMASK)
HW_ECC_CSR0_DMAWAIT_CLRMASK              equ     (~HW_ECC_CSR0_DMAWAIT_SETMASK)
HW_ECC_CSR0_ECCSEL_CLRMASK               equ     (~HW_ECC_CSR0_ECCSEL_SETMASK)
HW_ECC_CSR0_RSVD2_CLRMASK                equ     (~HW_ECC_CSR0_RSVD2_SETMASK)
HW_ECC_CSR0_SFTRST_CLRMASK               equ     (~HW_ECC_CSR0_SFTRST_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int ECCKICK 	     : HW_ECC_CSR0_ECCKICK_WIDTH;
;	 int AUTOCORR	     : HW_ECC_CSR0_AUTOCORR_WIDTH;
;	 int RSVD0           : HW_ECC_CSR0_RSVD0_WIDTH;
;	 int DONEINTEN	     : HW_ECC_CSR0_DONEINTEN_WIDTH;
;	 int KESINTEN	     : HW_ECC_CSR0_KESINTEN_WIDTH;
;	 int CORRINTEN       : HW_ECC_CSR0_CORRINTEN_WIDTH;
;	 int UNCORRINTEN     : HW_ECC_CSR0_UNCORRINTEN_WIDTH;
;	 int ERRINTEN        : HW_ECC_CSR0_ERRINTEN_WIDTH;
;	 int RSVD1	     : HW_ECC_CSR0_RSVD1_WIDTH;
; 	 int DMAWAIT         : HW_ECC_CSR0_DMAWAIT_WIDTH;
;	 int ECCSEL          : HW_ECC_CSR0_ECCSEL_WIDTH;
;	 int RSVD2           : HW_ECC_CSR0_RSVD2_WIDTH;
;        int SFTRST          : HW_ECC_CSR0_SFTRST_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_csr0_type;

 

;#define HW_ECC_CSR0      (*(volatile ecc_csr0_type _X*) (HW_ECC_BASEADDR+0))    /* ECC Control / Status Register0 */

;endc
 
;//////////////////////////////////////////////////////////////////////////
;  ECC Control Status Register1 (HW_ECC_CSR1) Bit Definitions

HW_ECC_CSR1_RSVD0_BITPOS                equ     (0)
HW_ECC_CSR1_DONEINT_BITPOS              equ     (4)
HW_ECC_CSR1_KESINT_BITPOS               equ     (5)
HW_ECC_CSR1_CORRINT_BITPOS              equ     (6)
HW_ECC_CSR1_UNCORRINT_BITPOS            equ     (7)
HW_ECC_CSR1_ERRINT_BITPOS               equ     (8)
HW_ECC_CSR1_RSVD1_BITPOS                equ     (9)
HW_ECC_CSR1_ECCEXCEPTION_BITPOS         equ     (12)
HW_ECC_CSR1_ECCERRORS_BITPOS            equ     (16)
HW_ECC_CSR1_RSVD2_BITPOS                equ     (19)

HW_ECC_CSR1_RSVD0_WIDTH                 equ     (4)
HW_ECC_CSR1_DONEINT_WIDTH               equ     (1)
HW_ECC_CSR1_KESINT_WIDTH                equ     (1)
HW_ECC_CSR1_CORRINT_WIDTH               equ     (1)
HW_ECC_CSR1_UNCORRINT_WIDTH             equ     (1)
HW_ECC_CSR1_ERRINT_WIDTH                equ     (1)
HW_ECC_CSR1_RSVD1_WIDTH                 equ     (3)
HW_ECC_CSR1_ECCEXCEPTION_WIDTH          equ     (4)
HW_ECC_CSR1_ECCERRORS_WIDTH             equ     (3)
HW_ECC_CSR1_RSVD2_WIDTH                 equ     (5)
                        
HW_ECC_CSR1_RSVD0_SETMASK                equ     (((1<<HW_ECC_CSR1_RSVD0_WIDTH)-1)<<HW_ECC_CSR1_RSVD0_BITPOS) 
HW_ECC_CSR1_DONEINT_SETMASK              equ     (((1<<HW_ECC_CSR1_DONEINT_WIDTH)-1)<<HW_ECC_CSR1_DONEINT_BITPOS) 
HW_ECC_CSR1_KESINT_SETMASK               equ     (((1<<HW_ECC_CSR1_KESINT_WIDTH)-1)<<HW_ECC_CSR1_KESINT_BITPOS) 
HW_ECC_CSR1_CORRINT_SETMASK              equ     (((1<<HW_ECC_CSR1_CORRINT_WIDTH)-1)<<HW_ECC_CSR1_CORRINT_BITPOS) 
HW_ECC_CSR1_UNCORRINT_SETMASK            equ     (((1<<HW_ECC_CSR1_UNCORRINT_WIDTH)-1)<<HW_ECC_CSR1_UNCORRINT_BITPOS) 
HW_ECC_CSR1_ERRINT_SETMASK               equ     (((1<<HW_ECC_CSR1_ERRINT_WIDTH)-1)<<HW_ECC_CSR1_ERRINT_BITPOS) 
HW_ECC_CSR1_RSVD1_SETMASK                equ     (((1<<HW_ECC_CSR1_RSVD1_WIDTH)-1)<<HW_ECC_CSR1_RSVD1_BITPOS) 
HW_ECC_CSR1_ECCEXCEPTION_SETMASK         equ     (((1<<HW_ECC_CSR1_ECCEXCEPTION_WIDTH)-1)<<HW_ECC_CSR1_ECCEXCEPTION_BITPOS) 
HW_ECC_CSR1_ECCERRORS_SETMASK            equ     (((1<<HW_ECC_CSR1_ECCERRORS_WIDTH)-1)<<HW_ECC_CSR1_ECCERRORS_BITPOS) 
HW_ECC_CSR1_RSVD2_SETMASK                equ     (((1<<HW_ECC_CSR1_RSVD2_WIDTH)-1)<<HW_ECC_CSR1_RSVD2_BITPOS) 
  
HW_ECC_CSR1_RSVD0_CLRMASK                equ     (~HW_ECC_CSR1_RSVD0_SETMASK)
HW_ECC_CSR1_DONEINT_CLRMASK              equ     (~HW_ECC_CSR1_DONEINT_SETMASK)
HW_ECC_CSR1_KESINT_CLRMASK               equ     (~HW_ECC_CSR1_KESINT_SETMASK)
HW_ECC_CSR1_CORRINT_CLRMASK              equ     (~HW_ECC_CSR1_CORRINT_SETMASK)
HW_ECC_CSR1_UNCORRINT_CLRMASK            equ     (~HW_ECC_CSR1_UNCORRINT_SETMASK)
HW_ECC_CSR1_ERRINT_CLRMASK               equ     (~HW_ECC_CSR1_ERRINT_SETMASK)
HW_ECC_CSR1_RSVD1_CLRMASK                equ     (~HW_ECC_CSR1_RSVD1_SETMASK)
HW_ECC_CSR1_ECCEXCEPTION_CLRMASK         equ     (~HW_ECC_CSR1_ECCEXCEPTION_SETMASK)
HW_ECC_CSR1_ECCERRORS_CLRMASK            equ     (~HW_ECC_CSR1_ECCERRORS_SETMASK)
HW_ECC_CSR1_RSVD2_CLRMASK                equ     (~HW_ECC_CSR1_RSVD2_SETMASK)
 
;C insert

;typedef union               

;{

;    struct {
;	 int RSVD0           : HW_ECC_CSR1_RSVD0_WIDTH;
;	 int DONEINT	     : HW_ECC_CSR1_DONEINT_WIDTH;
;	 int KESINT	     : HW_ECC_CSR1_KESINT_WIDTH;
;	 int CORRINT         : HW_ECC_CSR1_CORRINT_WIDTH;
;	 int UNCORRIN        : HW_ECC_CSR1_UNCORRINT_WIDTH;
;	 int ERRINT          : HW_ECC_CSR1_ERRINT_WIDTH;
;	 int RSVD1	     : HW_ECC_CSR1_RSVD1_WIDTH;
; 	 int ECCEXCEPTION    : HW_ECC_CSR1_ECCEXCEPTION_WIDTH;
;	 int ECCERRORS       : HW_ECC_CSR1_ECCERRORS_WIDTH;
;	 int RSVD2           : HW_ECC_CSR1_RSVD2_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_csr1_type;

;#define HW_ECC_CSR1      (*(volatile ecc_csr1_type _X*) (HW_ECC_BASEADDR+1))    /* ECC Control / Status Register1 */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Reed_solomon Control Status Register (HW_ECC_RSCFG) Bit Definitions

HW_ECC_RSCFG_SYMBOLSIZE_BITPOS          equ     (0)
HW_ECC_RSCFG_BLOCKSIZE_BITPOS           equ     (4)
HW_ECC_RSCFG_RSVD0_BITPOS               equ     (13)
HW_ECC_RSCFG_BYTEEN_BITPOS              equ     (20)
HW_ECC_RSCFG_RSCFGERR_BITPOS            equ     (21)
HW_ECC_RSCFG_DECSFTRST_BITPOS           equ     (22)
HW_ECC_RSCFG_ENCSFTRST_BITPOS           equ     (23)

HW_ECC_RSCFG_SYMBOLSIZE_WIDTH           equ     (4)
HW_ECC_RSCFG_BLOCKSIZE_WIDTH            equ     (9)
HW_ECC_RSCFG_RSVD0_WIDTH                equ     (7)
HW_ECC_RSCFG_BYTEEN_WIDTH               equ     (1)
HW_ECC_RSCFG_RSCFGERR_WIDTH             equ     (1)
HW_ECC_RSCFG_DECSFTRST_WIDTH            equ     (1)
HW_ECC_RSCFG_ENCSFTRST_WIDTH            equ     (1)
                        
HW_ECC_RSCFG_SYMBOLSIZE_SETMASK         equ     (((1<<HW_ECC_RSCFG_SYMBOLSIZE_WIDTH)-1)<<HW_ECC_RSCFG_SYMBOLSIZE_BITPOS) 
HW_ECC_RSCFG_BLOCKSIZE_SETMASK          equ     (((1<<HW_ECC_RSCFG_BLOCKSIZE_WIDTH)-1)<<HW_ECC_RSCFG_BLOCKSIZE_BITPOS) 
HW_ECC_RSCFG_RSVD0_SETMASK              equ     (((1<<HW_ECC_RSCFG_RSVD0_WIDTH)-1)<<HW_ECC_RSCFG_RSVD0_BITPOS) 
HW_ECC_RSCFG_BYTEEN_SETMASK             equ     (((1<<HW_ECC_RSCFG_BYTEEN_WIDTH)-1)<<HW_ECC_RSCFG_BYTEEN_BITPOS) 
HW_ECC_RSCFG_RSCFGERR_SETMASK           equ     (((1<<HW_ECC_RSCFG_RSCFGERR_WIDTH)-1)<<HW_ECC_RSCFG_RSCFGERR_BITPOS) 
HW_ECC_RSCFG_DECSFTRST_SETMASK          equ     (((1<<HW_ECC_RSCFG_DECSFTRST_WIDTH)-1)<<HW_ECC_RSCFG_DECSFTRST_BITPOS) 
HW_ECC_RSCFG_ENCSFTRST_SETMASK          equ     (((1<<HW_ECC_RSCFG_ENCSFTRST_WIDTH)-1)<<HW_ECC_RSCFG_ENCSFTRST_BITPOS) 
  
HW_ECC_RSCFG_SYMBOLSIZE_CLRMASK         equ     (~HW_ECC_RSCFG_SYMBOLSIZE_SETMASK)
HW_ECC_RSCFG_BLOCKSIZE_CLRMASK          equ     (~HW_ECC_RSCFG_BLOCKSIZE_SETMASK)
HW_ECC_RSCFG_RSVD0_CLRMASK              equ     (~HW_ECC_RSCFG_RSVD0_SETMASK)
HW_ECC_RSCFG_BYTEEN_CLRMASK             equ     (~HW_ECC_RSCFG_BYTEEN_SETMASK)
HW_ECC_RSCFG_RSCFGERR_CLRMASK           equ     (~HW_ECC_RSCFG_RSCFGERR_SETMASK)
HW_ECC_RSCFG_DECSFTRST_CLRMASK          equ     (~HW_ECC_RSCFG_DECSFTRST_SETMASK)
HW_ECC_RSCFG_ENCSFTRST_CLRMASK          equ     (~HW_ECC_RSCFG_ENCSFTRST_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int SYMBOLSIZE      : HW_ECC_RSCFG_SYMBOLSIZE_WIDTH;
;	 int BLOCKSIZE       : HW_ECC_RSCFG_BLOCKSIZE_WIDTH;
;	 int RSVD0           : HW_ECC_RSCFG_RSVD0_WIDTH;
;	 int BYTEEN	     : HW_ECC_RSCFG_BYTEEN_WIDTH;
; 	 int RSCFGERR        : HW_ECC_RSCFG_RSCFGERR_WIDTH;
;	 int DECSFTRST       : HW_ECC_RSCFG_DECSFTRST_WIDTH;
;	 int ENCSFTRST       : HW_ECC_RSCFG_ENCSFTRST_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_rscfg_type;

;#define HW_ECC_RSCFG      (*(volatile ecc_rscfg_type _X*) (HW_ECC_BASEADDR+2))    /* ECC Reed-Solomon Control Register */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC SSFDC Control Status Register (HW_ECC_SSFDCCFG) Bit Definitions

HW_ECC_SSFDCCFG_SYMBOLSIZE_BITPOS          equ     (0)
HW_ECC_SSFDCCFG_BLOCKSIZE_BITPOS           equ     (4)
HW_ECC_SSFDCCFG_RSVD0_BITPOS               equ     (13)
HW_ECC_SSFDCCFG_SFTRST_BITPOS              equ     (23)

HW_ECC_SSFDCCFG_SYMBOLSIZE_WIDTH           equ     (4)
HW_ECC_SSFDCCFG_BLOCKSIZE_WIDTH            equ     (9)
HW_ECC_SSFDCCFG_RSVD0_WIDTH                equ     (7)
HW_ECC_SSFDCCFG_SFTRST_WIDTH               equ     (1)
                        
HW_ECC_SSFDCCFG_SYMBOLSIZE_SETMASK         equ     (((1<<HW_ECC_SSFDCCFG_SYMBOLSIZE_WIDTH)-1)<<HW_ECC_SSFDCCFG_SYMBOLSIZE_BITPOS) 
HW_ECC_SSFDCCFG_BLOCKSIZE_SETMASK          equ     (((1<<HW_ECC_SSFDCCFG_BLOCKSIZE_WIDTH)-1)<<HW_ECC_SSFDCCFG_BLOCKSIZE_BITPOS) 
HW_ECC_SSFDCCFG_RSVD0_SETMASK              equ     (((1<<HW_ECC_SSFDCCFG_RSVD0_WIDTH)-1)<<HW_ECC_SSFDCCFG_RSVD0_BITPOS) 
HW_ECC_SSFDCCFG_SFTRST_SETMASK             equ     (((1<<HW_ECC_SSFDCCFG_SFTRST_WIDTH)-1)<<HW_ECC_SSFDCCFG_SFTRST_BITPOS) 

HW_ECC_SSFDCCFG_SYMBOLSIZE_CLRMASK         equ     (~HW_ECC_SSFDCCFG_SYMBOLSIZE_SETMASK)
HW_ECC_SSFDCCFG_BLOCKSIZE_CLRMASK          equ     (~HW_ECC_SSFDCCFG_BLOCKSIZE_SETMASK)
HW_ECC_SSFDCCFG_RSVD0_CLRMASK              equ     (~HW_ECC_SSFDCCFG_RSVD0_SETMASK)
HW_ECC_SSFDCCFG_ESFTRST_CLRMASK            equ     (~HW_ECC_SSFDCCFG_SFTRST_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int SYMBOLSIZE      : HW_ECC_SSFDCCFG_SYMBOLSIZE_WIDTH;
;	 int BLOCKSIZE       : HW_ECC_SSFDCCFG_BLOCKSIZE_WIDTH;
;	 int RSVD0           : HW_ECC_SSFDCCFG_RSVD0_WIDTH;
;	 int ESFTRST         : HW_ECC_SSFDCCFG_SFTRST_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_ssfdccfg_type;

;#define HW_ECC_SSFDCCFG      (*(volatile ecc_ssfdccfg_type _X*) (HW_ECC_BASEADDR+3))    /* ECC SSFDC Control Register */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Data Block Start Address Register (HW_ECC_BLKSTRTADDR) Bit Definitions

HW_ECC_BLKSTRTADDR_ADDR_BITPOS                equ     (0)
HW_ECC_BLKSTRTADDR_RSVD0_BITPOS               equ     (16)

HW_ECC_BLKSTRTADDR_ADDR_WIDTH                 equ     (16)
HW_ECC_BLKSTRTADDR_RSVD0_WIDTH                equ     (8)

HW_ECC_BLKSTRTADDR_ADDR_SETMASK               equ     (((1<<HW_ECC_BLKSTRTADDR_ADDR_WIDTH)-1)<<HW_ECC_BLKSTRTADDR_ADDR_BITPOS)
HW_ECC_BLKSTRTADDR_RSVD0_SETMASK              equ     (((1<<HW_ECC_BLKSTRTADDR_RSVD0_WIDTH)-1)<<HW_ECC_BLKSTRTADDR_RSVD0_BITPOS) 

HW_ECC_BLKSTRTADDR_ADDR_CLRMASK               equ     (~HW_ECC_BLKSTRTADDR_ADDR_SETMASK)
HW_ECC_BLKSTRTADDR_RSVD0_CLRMASK              equ     (~HW_ECC_BLKSTRTADDR_RSVD0_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int ADDR            : HW_ECC_BLKSTRTADDR_ADDR_WIDTH;
;	 int RSVD0           : HW_ECC_BLKSTRTADDR_RSVD0_WIDTH;

;    } B;

;    int I;
;    unsigned int U;

;} ecc_blkstrtaddr_type;

;#define HW_ECC_BLKSTRTADDR      (*(volatile ecc_blkstrtaddr_type _X*) (HW_ECC_BASEADDR+4))    /* ECC Block-Start Address */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Block Start Index Register (HW_ECC_BLKSTRTINDEX) Bit Definitions

HW_ECC_BLKSTRTINDEX_INDEX_BITPOS               equ     (0)
HW_ECC_BLKSTRTINDEX_RSVD0_BITPOS               equ     (5)
HW_ECC_BLKSTRTINDEX_MEMSPACE_BITPOS            equ     (8)
HW_ECC_BLKSTRTINDEX_RSVD1_BITPOS               equ     (10)

HW_ECC_BLKSTRTINDEX_INDEX_WIDTH                equ     (5)
HW_ECC_BLKSTRTINDEX_RSVD0_WIDTH                equ     (3)             
HW_ECC_BLKSTRTINDEX_MEMSPACE_WIDTH             equ     (2)
HW_ECC_BLKSTRTINDEX_RSVD1_WIDTH                equ     (14)

HW_ECC_BLKSTRTINDEX_INDEX_SETMASK              equ     (((1<<HW_ECC_BLKSTRTINDEX_INDEX_WIDTH)-1)<<HW_ECC_BLKSTRTINDEX_INDEX_BITPOS)
HW_ECC_BLKSTRTINDEX_RSVD0_SETMASK              equ     (((1<<HW_ECC_BLKSTRTINDEX_RSVD0_WIDTH)-1)<<HW_ECC_BLKSTRTINDEX_RSVD0_BITPOS)            
HW_ECC_BLKSTRTINDEX_MEMSPACE_SETMASK           equ     (((1<<HW_ECC_BLKSTRTINDEX_MEMSPACE_WIDTH)-1)<<HW_ECC_BLKSTRTINDEX_MEMSPACE_BITPOS) 
HW_ECC_BLKSTRTINDEX_RSVD1_SETMASK              equ     (((1<<HW_ECC_BLKSTRTINDEX_RSVD1_WIDTH)-1)<<HW_ECC_BLKSTRTINDEX_RSVD1_BITPOS) 

HW_ECC_BLKSTRTINDEX_INDEX_CLRMASK              equ     (~HW_ECC_BLKSTRTINDEX_INDEX_SETMASK)
HW_ECC_BLKSTRTINDEX_RSVD0_CLRMASK              equ     (~HW_ECC_BLKSTRTINDEX_RSVD0_SETMASK)
HW_ECC_BLKSTRTINDEX_MEMSPACE_CLRMASK           equ     (~HW_ECC_BLKSTRTINDEX_MEMSPACE_SETMASK)
HW_ECC_BLKSTRTINDEX_RSVD1_CLRMASK              equ     (~HW_ECC_BLKSTRTINDEX_RSVD1_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int INDEX           : HW_ECC_BLKSTRTINDEX_INDEX_WIDTH;
;	 int RSVD1           : HW_ECC_BLKSTRTINDEX_RSVD1_WIDTH;
;	 int MEMSPACE        : HW_ECC_BLKSTRTINDEX_MEMSPACE_WIDTH;
;	 int RSVD0           : HW_ECC_BLKSTRTINDEX_RSVD0_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_blkstrtindex_type;

;#define HW_ECC_BLKSTRTINDEX      (*(volatile ecc_blkstrtindex_type _X*) (HW_ECC_BASEADDR+5))    /* ECC Block-Start Bit-Index */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Parity Start Address Register (HW_ECC_PARSTRTADDR) Bit Definitions

HW_ECC_PARSTRTADDR_ADDR_BITPOS                equ     (0)
HW_ECC_PARSTRTADDR_RSVD0_BITPOS               equ     (16)

HW_ECC_PARSTRTADDR_ADDR_WIDTH                 equ     (16)
HW_ECC_PARSTRTADDR_RSVD0_WIDTH                equ     (8)

HW_ECC_PARSTRTADDR_ADDR_SETMASK               equ     (((1<<HW_ECC_PARSTRTADDR_ADDR_WIDTH)-1)<<HW_ECC_PARSTRTADDR_ADDR_BITPOS)  
HW_ECC_PARSTRTADDR_RSVD0_SETMASK              equ     (((1<<HW_ECC_PARSTRTADDR_RSVD0_WIDTH)-1)<<HW_ECC_PARSTRTADDR_RSVD0_BITPOS) 

HW_ECC_PARSTRTADDR_ADDR_CLRMASK               equ     (~HW_ECC_PARSTRTADDR_ADDR_SETMASK)
HW_ECC_PARSTRTADDR_RSVD0_CLRMASK              equ     (~HW_ECC_PARSTRTADDR_RSVD0_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int ADDR            : HW_ECC_PARSTRTADDR_ADDR_WIDTH;
;	 int RSVD0           : HW_ECC_PARSTRTADDR_RSVD0_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_parstrtaddr_type;

;#define HW_ECC_PARSTRTADDR      (*(volatile ecc_parstrtaddr_type _X*) (HW_ECC_BASEADDR+6))    /* ECC Parity-Start Address */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Parity Start Index Register (HW_ECC_PARSTRTINDEX) Bit Definitions

HW_ECC_PARSTRTINDEX_INDEX_BITPOS               equ     (0)
HW_ECC_PARSTRTINDEX_RSVD0_BITPOS               equ     (5)
HW_ECC_PARSTRTINDEX_MEMSPACE_BITPOS            equ     (8)
HW_ECC_PARSTRTINDEX_RSVD1_BITPOS               equ     (10)

HW_ECC_PARSTRTINDEX_INDEX_WIDTH                equ     (5)
HW_ECC_PARSTRTINDEX_RSVD0_WIDTH                equ     (3)             
HW_ECC_PARSTRTINDEX_MEMSPACE_WIDTH             equ     (2)
HW_ECC_PARSTRTINDEX_RSVD1_WIDTH                equ     (14)
           
HW_ECC_PARSTRTINDEX_INDEX_SETMASK              equ     (((1<<HW_ECC_PARSTRTINDEX_INDEX_WIDTH)-1)<<HW_ECC_PARSTRTINDEX_INDEX_BITPOS)
HW_ECC_PARSTRTINDEX_RSVD0_SETMASK              equ     (((1<<HW_ECC_PARSTRTINDEX_RSVD0_WIDTH)-1)<<HW_ECC_PARSTRTINDEX_RSVD0_BITPOS)
HW_ECC_PARSTRTINDEX_MEMSPACE_SETMASK           equ     (((1<<HW_ECC_PARSTRTINDEX_MEMSPACE_WIDTH)-1)<<HW_ECC_PARSTRTINDEX_MEMSPACE_BITPOS) 
HW_ECC_PARSTRTINDEX_RSVD1_SETMASK              equ     (((1<<HW_ECC_PARSTRTINDEX_RSVD1_WIDTH)-1)<<HW_ECC_PARSTRTINDEX_RSVD1_BITPOS) 

HW_ECC_PARSTRTINDEX_INDEX_CLRMASK              equ     (~HW_ECC_PARSTRTINDEX_INDEX_SETMASK)
HW_ECC_PARSTRTINDEX_RSVD0_CLRMASK              equ     (~HW_ECC_PARSTRTINDEX_RSVD0_SETMASK)
HW_ECC_PARSTRTINDEX_MEMSPACE_CLRMASK           equ     (~HW_ECC_PARSTRTINDEX_MEMSPACE_SETMASK)
HW_ECC_PARSTRTINDEX_RSVD1_CLRMASK              equ     (~HW_ECC_PARSTRTINDEX_RSVD1_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int INDEX           : HW_ECC_PARSTRTINDEX_INDEX_WIDTH;
;	 int RSVD0           : HW_ECC_PARSTRTINDEX_RSVD0_WIDTH;
;	 int MEMSPACE        : HW_ECC_PARSTRTINDEX_MEMSPACE_WIDTH;
;	 int RSVD1           : HW_ECC_PARSTRTINDEX_RSVD1_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_parstrtindex_type;

;#define HW_ECC_PARSTRTINDEX      (*(volatile ecc_parstrtindex_type _X*) (HW_ECC_BASEADDR+7))    /* ECC Block-Start Bit-Index */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Error-Location Address Register (HW_ECC_LOCADDR) Bit Definitions

HW_ECC_LOCADDR_ADDR_BITPOS                equ     (0)
HW_ECC_LOCADDR_RSVD0_BITPOS               equ     (16)

HW_ECC_LOCADDR_ADDR_WIDTH                 equ     (16)
HW_ECC_LOCADDR_RSVD0_WIDTH                equ     (8)

HW_ECC_LOCADDR_ADDR_SETMASK               equ     (((1<<HW_ECC_LOCADDR_ADDR_WIDTH)-1)<<HW_ECC_LOCADDR_ADDR_BITPOS)               
HW_ECC_LOCADDR_RSVD0_SETMASK              equ     (((1<<HW_ECC_LOCADDR_RSVD0_WIDTH)-1)<<HW_ECC_LOCADDR_RSVD0_BITPOS) 

HW_ECC_LOCADDR_ADDR_CLRMASK               equ     (~HW_ECC_LOCADDR_ADDR_SETMASK)
HW_ECC_LOCADDR_RSVD0_CLRMASK              equ     (~HW_ECC_LOCADDR_RSVD0_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int ADDR            : HW_ECC_LOCADDR_ADDR_WIDTH;
;	 int RSVD0           : HW_ECC_LOCADDR_RSVD0_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_locaddr_type;

;#define HW_ECC_LOCADDR      (*(volatile ecc_locaddr_type _X*) (HW_ECC_BASEADDR+8))    /* ECC Parity-Start Address */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Error-Location Index Register (HW_ECC_LOCINDEX) Bit Definitions

HW_ECC_LOCINDEX_INDEX_BITPOS               equ     (0)
HW_ECC_LOCINDEX_RSVD0_BITPOS               equ     (5)
HW_ECC_LOCINDEX_MEMSPACE_BITPOS            equ     (8)
HW_ECC_LOCINDEX_RSVD1_BITPOS               equ     (10)

HW_ECC_LOCINDEX_INDEX_WIDTH                equ     (5)
HW_ECC_LOCINDEX_RSVD0_WIDTH                equ     (3)             
HW_ECC_LOCINDEX_MEMSPACE_WIDTH             equ     (2)
HW_ECC_LOCINDEX_RSVD1_WIDTH                equ     (14)
           
HW_ECC_LOCINDEX_INDEX_SETMASK              equ     (((1<<HW_ECC_LOCINDEX_INDEX_WIDTH)-1)<<HW_ECC_LOCINDEX_INDEX_BITPOS)
HW_ECC_LOCINDEX_RSVD0_SETMASK              equ     (((1<<HW_ECC_LOCINDEX_RSVD0_WIDTH)-1)<<HW_ECC_LOCINDEX_RSVD0_BITPOS)            
HW_ECC_LOCINDEX_MEMSPACE_SETMASK           equ     (((1<<HW_ECC_LOCINDEX_MEMSPACE_WIDTH)-1)<<HW_ECC_LOCINDEX_MEMSPACE_BITPOS) 
HW_ECC_LOCINDEX_RSVD1_SETMASK              equ     (((1<<HW_ECC_LOCINDEX_RSVD1_WIDTH)-1)<<HW_ECC_LOCINDEX_RSVD1_BITPOS) 

HW_ECC_LOCINDEX_INDEX_CLRMASK              equ     (~HW_ECC_LOCINDEX_INDEX_SETMASK)
HW_ECC_LOCINDEX_RSVD0_CLRMASK              equ     (~HW_ECC_LOCINDEX_RSVD0_SETMASK)
HW_ECC_LOCINDEX_MEMSPACE_CLRMASK           equ     (~HW_ECC_LOCINDEX_MEMSPACE_SETMASK)
HW_ECC_LOCINDEX_RSVD1_CLRMASK              equ     (~HW_ECC_LOCINDEX_RSVD1_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int INDEX           : HW_ECC_LOCINDEX_INDEX_WIDTH;
;	 int RSVD0           : HW_ECC_LOCINDEX_RSVD0_WIDTH;
;	 int MEMSPACE        : HW_ECC_LOCINDEX_MEMSPACE_WIDTH;
;	 int RSVD1           : HW_ECC_LOCINDEX_RSVD1_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_locindex_type;

;#define HW_ECC_LOCINDEX      (*(volatile ecc_locindex_type _X*) (HW_ECC_BASEADDR+9))    /* ECC Block-Start Bit-Index */

;endc

;//////////////////////////////////////////////////////////////////////////
;  ECC Error-Location Index Register (HW_ECC_ERRVAL) Bit Definitions

HW_ECC_ERRVAL_ERRORVALUE_BITPOS          equ     (0)
HW_ECC_ERRVAL_RSVD0_BITPOS               equ     (9)
HW_ECC_ERRVAL_BLOCKLOC_BITPOS            equ     (12)
HW_ECC_ERRVAL_RSVD1_BITPOS               equ     (21)

HW_ECC_ERRVAL_ERRORVALUE_WIDTH           equ     (9)
HW_ECC_ERRVAL_RSVD0_WIDTH                equ     (3)             
HW_ECC_ERRVAL_BLOCKLOC_WIDTH             equ     (9)
HW_ECC_ERRVAL_RSVD1_WIDTH                equ     (3)
           
HW_ECC_ERRVAL_ERRORVALUE_SETMASK         equ     (((1<<HW_ECC_ERRVAL_ERRORVALUE_WIDTH)-1)<<HW_ECC_ERRVAL_ERRORVALUE_BITPOS)
HW_ECC_ERRVAL_RSVD0_SETMASK              equ     (((1<<HW_ECC_ERRVAL_RSVD0_WIDTH)-1)<<HW_ECC_ERRVAL_RSVD0_BITPOS)            
HW_ECC_ERRVAL_BLOCKLOC_SETMASK           equ     (((1<<HW_ECC_ERRVAL_BLOCKLOC_WIDTH)-1)<<HW_ECC_ERRVAL_BLOCKLOC_BITPOS) 
HW_ECC_ERRVAL_RSVD1_SETMASK              equ     (((1<<HW_ECC_ERRVAL_RSVD1_WIDTH)-1)<<HW_ECC_ERRVAL_RSVD1_BITPOS) 

HW_ECC_ERRVAL_ERRORVALUE_CLRMASK         equ     (~HW_ECC_ERRVAL_ERRORVALUE_SETMASK)
HW_ECC_ERRVAL_RSVD0_CLRMASK              equ     (~HW_ECC_ERRVAL_RSVD0_SETMASK)
HW_ECC_ERRVAL_BLOCKLOC_CLRMASK           equ     (~HW_ECC_ERRVAL_BLOCKLOC_SETMASK)
HW_ECC_ERRVAL_RSVD1_CLRMASK              equ     (~HW_ECC_ERRVAL_RSVD1_SETMASK)

;C insert

;typedef union               

;{

;    struct {
;	 int INDEX           : HW_ECC_ERRVAL_ERRORVALUE_WIDTH;
;	 int RSVD0           : HW_ECC_ERRVAL_RSVD0_WIDTH;
;	 int MEMSPACE        : HW_ECC_ERRVAL_BLOCKLOC_WIDTH;
;	 int RSVD1           : HW_ECC_ERRVAL_RSVD1_WIDTH;
;    } B;

;    int I;
;    unsigned int U;

;} ecc_errval_type;

;#define HW_ECC_ERRVAL      (*(volatile ecc_errval_type _X*) (HW_ECC_BASEADDR+10))    /* ECC Block-Start Bit-Index */

;endc

    endif
















