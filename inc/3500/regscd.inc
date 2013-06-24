;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Copyright(C) SigmaTel, Inc. 2002-2003
;;  File        : regscd.inc
;;  Description : CD-DSP Interface Registers (CDI, CDSync, CD Reed-Solomon)
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

    if !(@def(regscdinc))
regscdinc            equ         1

;C insert
;#include "types.h"
;endc


;/////////////////////////////////////////////////////////////////////////////////
;// CD Sync Registers
;/////////////////////////////////////////////////////////////////////////////////
HW_CDSYNC_BASEADDR      equ 	($F600)

;ASM Only

HW_CDSYNCCSR 	        equ     (HW_CDSYNC_BASEADDR)
HW_CDSYNCDR 	        equ     (HW_CDSYNC_BASEADDR+1)
HW_CDSYNCWCR 	        equ     (HW_CDSYNC_BASEADDR+2)  
HW_CDSYNCCPR 	        equ     (HW_CDSYNC_BASEADDR+3)  
HW_CDSYNCMR 	        equ     (HW_CDSYNC_BASEADDR+4)
HW_CDSYNCBAR 	        equ     (HW_CDSYNC_BASEADDR+5)

;endasm

;/////////////////////////////////////////////////////////////////////////////////
;  CD Sync Control Status Register (HW_CDSYNCCSR) Bit Definitions

HW_CDSYNCCSR_EN_BITPOS          equ     (0)
HW_CDSYNCCSR_CRC_BITPOS         equ     (1)
HW_CDSYNCCSR_DSCRAM_BITPOS 	    equ     (2)
HW_CDSYNCCSR_SYNCEN_BITPOS      equ     (3)
HW_CDSYNCCSR_DMAIRQEN_BITPOS    equ     (4)
HW_CDSYNCCSR_EDCIRQEN_BITPOS    equ     (5)
HW_CDSYNCCSR_LOSIRQEN_BITPOS    equ     (6)
HW_CDSYNCCSR_SYNCIRQEN_BITPOS   equ     (7)
HW_CDSYNCCSR_DMADONE_BITPOS     equ     (8)
HW_CDSYNCCSR_EDC_BITPOS         equ     (9)
HW_CDSYNCCSR_LOS_BITPOS         equ     (10)
HW_CDSYNCCSR_SYNC_BITPOS        equ     (11)
HW_CDSYNCCSR_DMAOF_BITPOS       equ	    (12)
HW_CDSYNCCSR_MODE1_BITPOS       equ     (18)
HW_CDSYNCCSR_INPUTMSB_BITPOS    equ     (19)
HW_CDSYNCCSR_OUTWRDLEN_BITPOS   equ     (20)
HW_CDSYNCCSR_OUTWRDLEN0_BITPOS  equ     (20)
HW_CDSYNCCSR_OUTWRDLEN1_BITPOS  equ     (21)
HW_CDSYNCCSR_INWRDLEN_BITPOS  	equ     (22)
HW_CDSYNCCSR_RESET_BITPOS       equ     (23)

HW_CDSYNCCSR_EN_SETMASK        	equ     (1<<HW_CDSYNCCSR_EN_BITPOS)  
HW_CDSYNCCSR_CRC_SETMASK        equ     (1<<HW_CDSYNCCSR_CRC_BITPOS) 
HW_CDSYNCCSR_DSCRAM_SETMASK 	equ     (1<<HW_CDSYNCCSR_DSCRAM_BITPOS)    
HW_CDSYNCCSR_SYNCEN_SETMASK    	equ     (1<<HW_CDSYNCCSR_SYNCEN_BITPOS)  
HW_CDSYNCCSR_DMAIRQEN_SETMASK  	equ     (1<<HW_CDSYNCCSR_DMAIRQEN_BITPOS)  
HW_CDSYNCCSR_EDCIRQEN_SETMASK  	equ     (1<<HW_CDSYNCCSR_EDCIRQEN_BITPOS)  
HW_CDSYNCCSR_LOSIRQEN_SETMASK  	equ     (1<<HW_CDSYNCCSR_LOSIRQEN_BITPOS)  
HW_CDSYNCCSR_SYNCIRQEN_SETMASK 	equ     (1<<HW_CDSYNCCSR_SYNCIRQEN_BITPOS)  
HW_CDSYNCCSR_DMADONE_SETMASK   	equ     (1<<HW_CDSYNCCSR_DMADONE_BITPOS)  
HW_CDSYNCCSR_EDC_SETMASK       	equ     (1<<HW_CDSYNCCSR_EDC_BITPOS)  
HW_CDSYNCCSR_LOS_SETMASK       	equ     (1<<HW_CDSYNCCSR_LOS_BITPOS)  
HW_CDSYNCCSR_SYNC_SETMASK      	equ     (1<<HW_CDSYNCCSR_SYNC_BITPOS)  
HW_CDSYNCCSR_DMAOF_SETMASK   	equ     (1<<HW_CDSYNCCSR_DMAOF_BITPOS)	
HW_CDSYNCCSR_MODE1_SETMASK     	equ     (1<<HW_CDSYNCCSR_MODE1_BITPOS) 
HW_CDSYNCCSR_INPUTMSB_SETMASK  	equ     (1<<HW_CDSYNCCSR_INPUTMSB_BITPOS)  
HW_CDSYNCCSR_OUTWRDLEN_SETMASK 	equ     (3<<HW_CDSYNCCSR_OUTWRDLEN_BITPOS) 
HW_CDSYNCCSR_INWRDLEN_SETMASK  	equ     (1<<HW_CDSYNCCSR_INWRDLEN_BITPOS)	
HW_CDSYNCCSR_RESET_SETMASK     	equ     (1<<HW_CDSYNCCSR_RESET_BITPOS)  

HW_CDSYNCCSR_EN_CLRMASK        	equ     (~HW_CDSYNCCSR_EN_SETMASK)
HW_CDSYNCCSR_CRC_CLRMASK       	equ     (~HW_CDSYNCCSR_CRC_SETMASK)
HW_CDSYNCCSR_DSCRAM_CLRMASK    	equ     (~HW_CDSYNCCSR_DSCRAM_SETMASK)
HW_CDSYNCCSR_SYNCEN_CLRMASK    	equ     (~HW_CDSYNCCSR_SYNCEN_SETMASK)
HW_CDSYNCCSR_DMAIRQEN_CLRMASK  	equ     (~HW_CDSYNCCSR_DMAIRQEN_SETMASK)
HW_CDSYNCCSR_EDCIRQEN_CLRMASK  	equ     (~HW_CDSYNCCSR_EDCIRQEN_SETMASK)
HW_CDSYNCCSR_LOSIRQEN_CLRMASK  	equ     (~HW_CDSYNCCSR_LOSIRQEN_SETMASK)
HW_CDSYNCCSR_SYNCIRQEN_CLRMASK 	equ     (~HW_CDSYNCCSR_SYNCIRQEN_SETMASK)
HW_CDSYNCCSR_DMADONE_CLRMASK   	equ     (~HW_CDSYNCCSR_DMADONE_SETMASK)
HW_CDSYNCCSR_EDC_CLRMASK       	equ     (~HW_CDSYNCCSR_EDC_SETMASK)
HW_CDSYNCCSR_LOS_CLRMASK        equ     (~HW_CDSYNCCSR_LOS_SETMASK)
HW_CDSYNCCSR_SYNC_CLRMASK       equ     (~HW_CDSYNCCSR_SYNC_SETMASK)
HW_CDSYNCCSR_DMAOF_CLRMASK      equ     (~HW_CDSYNCCSR_DMAOF_SETMASK)
HW_CDSYNCCSR_MODE1_CLRMASK      equ     (~HW_CDSYNCCSR_MODE1_SETMASK)
HW_CDSYNCCSR_INPUTMSB_CLRMASK   equ     (~HW_CDSYNCCSR_INPUTMSB_SETMASK)
HW_CDSYNCCSR_OUTWRDLEN_CLRMASK  equ     (~HW_CDSYNCCSR_OUTWRDLEN_SETMASK)
HW_CDSYNCCSR_INWRDLEN_CLRMASK   equ     (~HW_CDSYNCCSR_INWRDLEN_SETMASK)
HW_CDSYNCCSR_RESET_CLRMASK      equ     (~HW_CDSYNCCSR_RESET_SETMASK)


;C insert
;typedef union               /* SPI Control/Status Register (SPCSR)  */
;{
;    struct {
;       int		EN			:1;	// CDSync Enable
;		int 	CRC			:1;	// CRC checking Enable
;		int		DSCRAM		:1;	// Enable Descrambling
;		int 	SYNCEN		:1;	// Enable Sync Detection
;		int		DMAIRQEN 	:1; // Enable IRQ on DMA complete
;		int		EDCIRQEN	:1;	// Enable IRQ on EDC errors
;		int		LOSIRQEN	:1;	// Enable IRQ on Loss of Sync
;		int 	SYNCIRQEN	:1;	// Enable IRQ on CD-ROM Frame Start
;		int		DMADONE		:1;	// DMA is completed Status Bit
;		int		EDC		   	:1;	// EDC status bit
;		int		LOS			:1;	// Loss of Sync Indicator bit
;		int 	SYNC		:1;	// Sync Found Flag
;		int		DMAOF		:1;	// Indicates a DMA Overflow Exists
;		unsigned			:5;	// Reserved
;		int 	MODE1		:1;	// Force Mode1 Sectors
;		int		INPUTMSB	:1;	// Indicates which input MSB to use
;		unsigned OUTWRLEN	:2;	// Number of bytes to write in each output word
;		unsigned INWRDLEN	:1;	// Number of bytes in each input word (2 or 3)
;		int		RESET		:1;	// Soft Reset and Low Power Enable
;            } B;
;    unsigned int I;
;} cdsync_csr_type;

;#define HW_CDSYNCCSR 	(*(volatile cdsync_csr_type _X*) (HW_CDSYNC_BASEADDR))		/* CDSync Control/Status Register (SPCSR)  */
;endc



;///////////////////////////////////////////////////////
;//  CDSync Input Word Register (CDSYNCDR) bit definitions


HW_CDSYNCDR_DATA_BITPOS     equ     (0)
HW_CDSYNCDR_DATA_SETMASK    equ   	($FFFFFF<<HW_CDSYNCDR_DATA_BITPOS)
HW_CDSYNCDR_DATA_CLRMASK    equ  	(~HW_CDSYNCDR_DATA_SETMASK)  

;C insert

;typedef union     // CDSync Input Data Word Register
;{
;    struct { 
;		unsigned DATA	:24;	// Data for Input 
;		} B;
;    unsigned int I;
;} cdsync_datar_type;

;#define HW_CDSYNCDR 	(*(volatile cdsync_datar_type _X*) (HW_CDSYNC_BASEADDR+1))	
;endc

;///////////////////////////////////////////////////////
;//  CDSync Word Count Register (CDSYNCWCR) bit definitions

HW_CDSYNCWCR_COUNT_BITPOS   equ    	(0)
HW_CDSYNCWCR_COUNT_SETMASK  equ 	($1FFF<<HW_CDSYNCWCR_COUNT_BITPOS)
HW_CDSYNCWCR_COUNT_CLRMASK  equ    	(~HW_CDSYNCWCR_COUNT_SETMASK)  


;C insert
;typedef union     // CDSync Word Count Register
;{
;    struct { 
;		unsigned COUNT	:13;	// Word Count
;		unsigned		:11;	// Reserved
;		} B;
;    unsigned int I;
;} cdsync_wcr_type;
;#define HW_CDSYNCWCR 	(*(volatile cdsync_wcr_type _X*) (HW_CDSYNC_BASEADDR+2))  	/* CDSync Word Count Register */
;endc


;///////////////////////////////////////////////////////
;//  CDSync Current Position Register (CDSYNCCPR) bit definitions

HW_CDSYNCCPR_POS_BITPOS     equ    	(0)
HW_CDSYNCCPR_POS_SETMASK   	equ  	($FFF<<HW_CDSYNCCPR_POS_BITPOS)
HW_CDSYNCCPR_POS_CLRMASK    equ  	(~HW_CDSYNCCPR_POS_SETMASK)

;C insert
;typedef union     // CDSync Current Position Register
;{
;    struct { 
;		unsigned POS	:12;	// Current Position 
;		unsigned		:12;	// Reserved
;		} B;
;    unsigned int I;
;} cdsync_cpr_type;
;#define HW_CDSYNCCPR 	(*(volatile cdsync_cpr_type _X*) (HW_CDSYNC_BASEADDR+3))  	/* CDSync Current Position Register */
;endc

;///////////////////////////////////////////////////////
;//  CDSync Modulo Register (CDSYNCMODR) bit definitions

HW_CDSYNCMR_MOD_BITPOS      equ  	(0)
HW_CDSYNCMR_MOD_SETMASK   	equ  	($1FFF<<HW_CDSYNCMR_MOD_BITPOS)
HW_CDSYNCMR_MOD_CLRMASK     equ 	(~HW_CDSYNCMR_MOD_SETMASK)

;C insert
;typedef union     // CDSync Modulo Register
;{
;    struct { 
;		unsigned MOD	:13;	// Modulo
;		unsigned 		:11;	// Reserved
;		} B;
;    unsigned int I;
;} cdsync_modr_type;
;#define HW_CDSYNCMR 	(*(volatile cdsync_modr_type _X*) (HW_CDSYNC_BASEADDR+4))   /* CDSync Modulo Register */
;endc

;///////////////////////////////////////////////////////
;//  CDSync Base Address Register (CDSYNCBAR) bit definitions

HW_CDSYNCBAR_BASE_BITPOS    equ    	(0)
HW_CDSYNCBAR_BASE_SETMASK   equ   	($FFFF<<HW_CDSYNCBAR_BASE_BITPOS)
HW_CDSYNCBAR_BASE_CLRMASK   equ   	(~HW_CDSYNCBAR_BASE_SETMASK)

;C insert
;typedef union     // CDSync Base Address Register
;{
;    struct { 
;		unsigned BASE	:16;	// Base Address
;		unsigned 		:8;		// Reserved
;		} B;
;    unsigned int I;
;} cdsync_bar_type;

;#define HW_CDSYNCBAR 	(*(volatile cdsync_bar_type _X*) (HW_CDSYNC_BASEADDR+5))  	/* CDSync Base Address Register */
;endc




;/////////////////////////////////////////////////////////////////////////////////
;// Reed Solomon Registers
;/////////////////////////////////////////////////////////////////////////////////
HW_RS_BASEADDR          equ 	($F700)
;ASM only
HW_RSCSR 		        equ     (HW_RS_BASEADDR)			
HW_RSOFFSETR	        equ     (HW_RS_BASEADDR+1)
HW_RSWRDCNTR 	        equ     (HW_RS_BASEADDR+2)
HW_RSCPR 		        equ     (HW_RS_BASEADDR+3)
HW_RSMODR 		        equ     (HW_RS_BASEADDR+4)
HW_RSBAR 		        equ     (HW_RS_BASEADDR+5)
HW_RSPBAR 		        equ     (HW_RS_BASEADDR+6)
HW_RSSPANR 		        equ     (HW_RS_BASEADDR+7)
;endasm
;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Control Status Register (HW_RSCSR) Bit Definitions

HW_RSCSR_KICK_BITPOS		equ     (0)
HW_RSCSR_AUTOCORCT_BITPOS   equ     (1)   
HW_RSCSR_EVENEN_BITPOS      equ     (2)
HW_RSCSR_ODDEN_BITPOS       equ     (3)
HW_RSCSR_DONEIRQEN_BITPOS   equ     (4)   
HW_RSCSR_CORCTIRQEN_BITPOS  equ     (5)    
HW_RSCSR_ERRIRQEN_BITPOS    equ     (6)  
HW_RSCSR_DONE_BITPOS      	equ     (8)
HW_RSCSR_CORRECT_BITPOS     equ     (9) 
HW_RSCSR_ERR_BITPOS         equ	    (10)
HW_RSCSR_EVENCORCT_BITPOS   equ     (12)   
HW_RSCSR_EVENERROR_BITPOS   equ     (13)   
HW_RSCSR_ODDCORCT_BITPOS    equ     (14)  
HW_RSCSR_ODDERROR_BITPOS    equ     (15)  
HW_RSCSR_DMAWAIT_BITPOS     equ  	(16)
HW_RSCSR_RESET_BITPOS 	    equ  	(23)

HW_RSCSR_KICK_LENGTH		equ     (1)
HW_RSCSR_AUTOCORCT_LENGTH   equ     (1)   
HW_RSCSR_EVENEN_LENGTH      equ     (1)
HW_RSCSR_ODDEN_LENGTH       equ     (1)
HW_RSCSR_DONEIRQEN_LENGTH   equ     (1)   
HW_RSCSR_CORCTIRQEN_LENGTH  equ     (1)    
HW_RSCSR_ERRIRQEN_LENGTH    equ     (1)  
HW_RSCSR_DONE_LENGTH      	equ     (1)
HW_RSCSR_CORRECT_LENGTH     equ     (1) 
HW_RSCSR_ERR_LENGTH         equ	    (1)
HW_RSCSR_EVENCORCT_LENGTH   equ     (1)    
HW_RSCSR_EVENERROR_LENGTH   equ     (1)    
HW_RSCSR_ODDCORCT_LENGTH    equ     (1)   
HW_RSCSR_ODDERROR_LENGTH    equ     (1)   
HW_RSCSR_DMAWAIT_LENGTH     equ  	(4) 
HW_RSCSR_RESET_LENGTH 	    equ  	(1) 
                                            
HW_RSCSR_KICK_SETMASK      	equ  	(((1<<HW_RSCSR_KICK_LENGTH)-1)<<HW_RSCSR_KICK_BITPOS)
HW_RSCSR_AUTOCORCT_SETMASK  equ  	(((1<<HW_RSCSR_AUTOCORCT_LENGTH)-1)<<HW_RSCSR_AUTOCORCT_BITPOS)
HW_RSCSR_EVENEN_SETMASK     equ  	(((1<<HW_RSCSR_EVENEN_LENGTH)-1)<<HW_RSCSR_EVENEN_BITPOS)
HW_RSCSR_ODDEN_SETMASK      equ  	(((1<<HW_RSCSR_ODDEN_LENGTH)-1)<<HW_RSCSR_ODDEN_BITPOS)
HW_RSCSR_DONEIRQEN_SETMASK  equ  	(((1<<HW_RSCSR_DONEIRQEN_LENGTH)-1)<<HW_RSCSR_DONEIRQEN_BITPOS)
HW_RSCSR_CORCTIRQEN_SETMASK equ  	(((1<<HW_RSCSR_CORCTIRQEN_LENGTH)-1)<<HW_RSCSR_CORCTIRQEN_BITPOS)
HW_RSCSR_ERRIRQEN_SETMASK   equ  	(((1<<HW_RSCSR_ERRIRQEN_LENGTH)-1)<<HW_RSCSR_ERRIRQEN_BITPOS)
HW_RSCSR_DONE_SETMASK      	equ  	(((1<<HW_RSCSR_DONE_LENGTH)-1)<<HW_RSCSR_DONE_BITPOS)
HW_RSCSR_CORRECT_SETMASK    equ  	(((1<<HW_RSCSR_CORRECT_LENGTH)-1)<<HW_RSCSR_CORRECT_BITPOS)
HW_RSCSR_ERR_SETMASK      	equ  	(((1<<HW_RSCSR_ERR_LENGTH)-1)<<HW_RSCSR_ERR_BITPOS)
HW_RSCSR_EVENCORCT_SETMASK  equ  	(((1<<HW_RSCSR_EVENCORCT_LENGTH)-1)<<HW_RSCSR_EVENCORCT_BITPOS)
HW_RSCSR_EVENERROR_SETMASK  equ  	(((1<<HW_RSCSR_EVENERROR_LENGTH)-1)<<HW_RSCSR_EVENERROR_BITPOS)
HW_RSCSR_ODDCORCT_SETMASK   equ  	(((1<<HW_RSCSR_ODDCORCT_LENGTH)-1)<<HW_RSCSR_ODDCORCT_BITPOS)
HW_RSCSR_ODDERROR_SETMASK   equ  	(((1<<HW_RSCSR_ODDERROR_LENGTH)-1)<<HW_RSCSR_ODDERROR_BITPOS)
HW_RSCSR_DMAWAIT_SETMASK    equ  	(((1<<HW_RSCSR_DMAWAIT_LENGTH)-1)<<HW_RSCSR_DMAWAIT_BITPOS)
HW_RSCSR_RESET_SETMASK      equ  	(((1<<HW_RSCSR_RESET_LENGTH)-1)<<HW_RSCSR_RESET_BITPOS)


HW_RSCSR_KICK_CLRMASK      	equ  	(~HW_RSCSR_KICK_SETMASK)
HW_RSCSR_AUTOCORCT_CLRMASK  equ     (~HW_RSCSR_AUTOCORCT_SETMASK)
HW_RSCSR_EVENEN_CLRMASK     equ     (~HW_RSCSR_EVENEN_SETMASK)
HW_RSCSR_ODDEN_CLRMASK      equ     (~HW_RSCSR_ODDEN_SETMASK)
HW_RSCSR_DONEIRQEN_CLRMASK  equ     (~HW_RSCSR_DONEIRQEN_SETMASK)
HW_RSCSR_CORCTIRQEN_CLRMASK equ     (~HW_RSCSR_CORCTIRQEN_SETMASK)
HW_RSCSR_ERRIRQEN_CLRMASK   equ     (~HW_RSCSR_ERRIRQEN_SETMASK)
HW_RSCSR_DONE_CLRMASK      	equ     (~HW_RSCSR_DONE_SETMASK)
HW_RSCSR_CORRECT_CLRMASK    equ     (~HW_RSCSR_CORRECT_SETMASK)
HW_RSCSR_ERR_CLRMASK      	equ     (~HW_RSCSR_ERR_SETMASK)
HW_RSCSR_EVENCORCT_CLRMASK  equ     (~HW_RSCSR_EVENCORCT_SETMASK)
HW_RSCSR_EVENERROR_CLRMASK  equ     (~HW_RSCSR_EVENERROR_SETMASK)
HW_RSCSR_ODDCORCT_CLRMASK   equ     (~HW_RSCSR_ODDCORCT_SETMASK)
HW_RSCSR_ODDERROR_CLRMASK   equ     (~HW_RSCSR_ODDERROR_SETMASK)
HW_RSCSR_DMAWAIT_CLRMASK    equ     (~HW_RSCSR_DMAWAIT_SETMASK)
HW_RSCSR_RESET_CLRMASK      equ     (~HW_RSCSR_RESET_SETMASK)



;C insert
;typedef union               
;{
;    struct {
;       int		KICK		:1;	// Start processing
;		int		AUTOCORCT	:1;	// Auto Correct errors
;		int 	EVENEN		:1;	// Enable calculation of even codewords
;		int 	ODDEN		:1;	// Enable calculation of odd codewords
;		int 	DONEIRQEN	:1;	// Enable Done IRQ
;		int		CORCTIRQEN	:1;	// Enable IRQ on correctable errors
;		int		ERRIRQEN	:1;	// Enable IRQ for non-correctable errors
;		int					:1;	// Reserved
;		int		DONE		:1;	// Done Flag
;		int		CORRECT		:1;	// Corrected errors Flag
;		int		ERR			:1;	// Non-Corrected errors Flag
;		int					:1;	// Reserved
;		int		EVENCORCT	:1;	// Flag for Even error corrected
;		int		EVENERROR	:1;	// Flag For Even error non-corrected
;		int		ODDCORCT	:1;	// Flag for Odd error corrected
;		int		ODDERROR	:1;	// Flag For Odd error non-corrected
;		unsigned DMAWAIT	:4;	// Number of cycles to wait between DMA cycles
;		int					:3;	// Reserved
;		int 	RESET		:1;	// Reset /  Power Down			
;    } B;
;    unsigned int I;
;} rs_csr_type;
;#define HW_RSCSR 		(*(volatile RS_csr_type _X*) (HW_RS_BASEADDR))			/* RS Control/Status Register */
;endc




;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Error Offset Register (HW_RSOFFSETR) Bit Definitions

HW_RSOFFSETR_EVEN_BITPOS        equ     (0)
HW_RSOFFSETR_ODD_BITPOS        	equ     (12)

HW_RSOFFSETR_EVEN_SETMASK      	equ     ($FFF<<HW_RSOFFSETR_EVEN_BITPOS)
HW_RSOFFSETR_ODD_SETMASK      	equ     ($FFF<<HW_RSOFFSETR_ODD_BITPOS)

HW_RSOFFSETR_EVEN_CLRMASK      	equ     (~HW_RSOFFSETR_EVEN_SETMASK)
HW_RSOFFSETR_ODD_CLRMASK      	equ     (~HW_RSOFFSETR_ODD_SETMASK)

;C insert
;typedef union{ 
;    struct { 
;		unsigned EVEN		:12;	// Location of even error
;		unsigned ODD		:12;	// Location of odd error
;		} B;
;    unsigned int I;
;} rs_offsetr_type;
;#define HW_RSOFFSETR	(*(volatile RS_offsetr_type _X*) (HW_RS_BASEADDR+1))	/* RS Error Offset Register */
;endc



;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Word Count Register (HW_RSWRDCNTR) Bit Definitions

HW_RSWRDCNTR_WORDCNT_BITPOS         equ     (0)
HW_RSWRDCNTR_BLOCKCNT_BITPOS        equ     (12)

HW_RSWRDCNTR_WORDCNT_SETMASK      	equ     ($FFF<<HW_RSWRDCNTR_WORDCNT_BITPOS)
HW_RSWRDCNTR_BLOCKCNT_SETMASK      	equ     ($FFF<<HW_RSWRDCNTR_BLOCKCNT_BITPOS)

HW_RSWRDCNTR_WORDCNT_CLRMASK      	equ     (~HW_RSWRDCNTR_WORDCNT_SETMASK)
HW_RSWRDCNTR_BLOCKCNT_CLRMASK      	equ     (~HW_RSWRDCNTR_BLOCKCNT_SETMASK)

;C insert
;typedef union{ 
;    struct { 
;		unsigned WORDCNT	:12;	// Number of bytes per codeword
;		unsigned BLOCKCNT	:12;	// Number of codewords to process
;		} B;
;   unsigned int I;
;} rs_wrdcntr_type;
;#define HW_RSWRDCNTR 	(*(volatile RS_wrdcntr_type _X*) (HW_RS_BASEADDR+2)) 	/* RS Word Count Register */
;endc




;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Current Position Register (HW_RSCPR) Bit Definitions

HW_RSCPR_POS_BITPOS         equ     (0)

HW_RSCPR_POS_SETMASK        equ     ($FFFF<<HW_RSCPR_POS_BITPOS)

HW_RSCPR_POS_CLRMASK      	equ     (~HW_RSCPR_POS_SETMASK)

;C insert
;typedef union{ 
;    struct { 
;		unsigned POS		:16;	// Current offset within the DMA buffer
;		unsigned 			:8;		// Reserved
;		} B;
;    unsigned int I;
;} rs_cpr_type;
;#define HW_RSCPR 		(*(volatile RS_cpr_type _X*) (HW_RS_BASEADDR+3))		/* RS Current Position Register */
;endc




;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Modulo Register (HW_RSMODR) Bit Definitions

HW_RSMODR_MOD_BITPOS        equ     (0)

HW_RSMODR_MOD_SETMASK      	equ     ($FFFF<<HW_RSMODR_MOD_BITPOS)

HW_RSMODR_MOD_CLRMASK      	equ     (~HW_RSMODR_MOD_SETMASK)

;C insert
;typedef union{ 
;    struct { 
;		unsigned MOD		:16;	// Modulo offset
;		unsigned 			:8;		// Reserved
;		} B;
;    unsigned int I;
;} rs_modr_type;
;#define HW_RSMODR 		(*(volatile RS_modr_type _X*) (HW_RS_BASEADDR+4))		/* RS Modulo Register */
;endc



;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Base Addr Register (HW_RSBAR) Bit Definitions

HW_RSBAR_ADDR_BITPOS        equ     (0)

HW_RSBAR_ADDR_SETMASK      	equ     ($FFFF<<HW_RSBAR_ADDR_BITPOS)

HW_RSBAR_ADDR_CLRMASK      	equ     (~HW_RSBAR_ADDR_SETMASK)

;C insert
;typedef union{ 
;    struct { 
;		unsigned ADDR		:16;	// Base Address of non-parity portion of codeword
;		unsigned 			:8;		// Reserved
;		} B;
;    unsigned int I;
;} rs_bar_type;
;#define HW_RSBAR 		(*(volatile RS_bar_type _X*) (HW_RS_BASEADDR+5))		/* RS Base Address Register */
;endc

;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Parity Base Addr Register (HW_RSPBAR) Bit Definitions

HW_RSPBAR_ADDR_BITPOS           equ     (0)

HW_RSPBAR_ADDR_SETMASK      	equ     ($FFFF<<HW_RSPBAR_ADDR_BITPOS)

HW_RSPBAR_ADDR_CLRMASK      	equ     (~HW_RSPBAR_ADDR_SETMASK)

;C insert
;typedef union{ 
;    struct { 
;		unsigned ADDR		:16;	// Parity offset
;		unsigned 			:8;		// Reserved
;		} B;
;    unsigned int I;
;} rs_pbar_type;
;#define HW_RSPBAR 		(*(volatile RS_pbar_type _X*) (HW_RS_BASEADDR+6))		/* RS Parity Base Address Register */
;endc


;/////////////////////////////////////////////////////////////////////////////////
;//  Reed Solomon Span Register (HW_RSSPANR) Bit Definitions

HW_RSSPANR_WORDSPAN_BITPOS       equ     (0)
HW_RSSPANR_PARITYSPAN_BITPOS     equ     (8)
HW_RSSPANR_BLOCKSPAN_BITPOS      equ     (16)

HW_RSSPANR_WORDSPAN_SETMASK      equ     ($FF<<HW_RSSPANR_WORDSPAN_BITPOS)
HW_RSSPANR_PARITYSPAN_SETMASK    equ     ($FF<<HW_RSSPANR_PARITYSPAN_BITPOS)
HW_RSSPANR_BLOCKSPAN_SETMASK     equ     ($FF<<HW_RSSPANR_BLOCKSPAN_BITPOS)

HW_RSSPANR_WORDSPAN_CLRMASK      equ     (~HW_RSSPANR_WORDSPAN_SETMASK)
HW_RSSPANR_PARITYSPAN_CLRMASK    equ     (~HW_RSSPANR_PARITYSPAN_SETMASK)
HW_RSSPANR_BLOCKSPAN_CLRMASK     equ     (~HW_RSSPANR_BLOCKSPAN_SETMASK)

;C insert
;typedef union{ 
;    struct { 
;		unsigned WORDSPAN  	:8;	// Word Address Increment
;		unsigned PARITYSPAN	:8;	// Parity Address Increment
;		unsigned BLOCKSPAN  :8;	// Block Address Increment
;		} B;
;    unsigned int I;
;} rs_spanr_type;
;#define HW_RSSPANR 		(*(volatile RS_spanr_type _X*) (HW_RS_BASEADDR+7))		/* RS Span Register */
;endc





;/////////////////////////////////////////////////////////////////////////////////
;//  CD-DSP Interface (CDI) Registers
;/////////////////////////////////////////////////////////////////////////////////

HW_CDI_BASEADDR         equ 	$F280

;ASM only
HW_CDICTRLCSR 	equ 	(HW_CDI_BASEADDR)
HW_CDICTRLTMRR 	equ 	(HW_CDI_BASEADDR+1)
HW_CDICTRLTBR 	equ 	(HW_CDI_BASEADDR+2)
HW_CDICTRLDATAR equ 	(HW_CDI_BASEADDR+3)
HW_CDIPINCFGR 	equ 	(HW_CDI_BASEADDR+4)
HW_CDISI0CSR 	equ 	(HW_CDI_BASEADDR+8)
HW_CDISI0TMRR 	equ 	(HW_CDI_BASEADDR+9)
HW_CDISI0TBR 	equ 	(HW_CDI_BASEADDR+10)
HW_CDISI0DATAR 	equ 	(HW_CDI_BASEADDR+11)	 
HW_CDISI1CSR 	equ 	(HW_CDI_BASEADDR+12)
HW_CDISI1TMRR 	equ 	(HW_CDI_BASEADDR+13)
HW_CDISI1TBR 	equ 	(HW_CDI_BASEADDR+14)
HW_CDISI1DATAR 	equ 	(HW_CDI_BASEADDR+15)
;endasm
;/////////////////////////////////////////////////////////////////////////////////
;//  CD-DSP Interface Control Unit Control Status Register (HW_CDI_CTLCSR) Bit Definitions

HW_CDICTRLCSR_KICK_BITPOS    	equ    (0)
HW_CDICTRLCSR_CLKPOL_BITPOS     equ    (1)
HW_CDICTRLCSR_CLKRZ_BITPOS      equ    (2)
HW_CDICTRLCSR_LSBFIRST_BITPOS   equ    (3)
HW_CDICTRLCSR_LENGTH_BITPOS     equ    (4)
HW_CDICTRLCSR_MSBJUSTIFY_BITPOS equ    (9)
HW_CDICTRLCSR_LATCHEN_BITPOS    equ    (10)
HW_CDICTRLCSR_LATCHPOL_BITPOS   equ    (11)
HW_CDICTRLCSR_RSVD_BITPOS       equ    (12)
HW_CDICTRLCSR_IRQEN_BITPOS      equ    (20)
HW_CDICTRLCSR_TFRDONE_BITPOS    equ    (21)
HW_CDICTRLCSR_RESET_BITPOS      equ    (23)

HW_CDICTRLCSR_KICK_SETMASK     	equ     (1<<HW_CDICTRLCSR_KICK_BITPOS)
HW_CDICTRLCSR_CLKPOL_SETMASK    equ     (1<<HW_CDICTRLCSR_CLKPOL_BITPOS)
HW_CDICTRLCSR_CLKRZ_SETMASK     equ     (1<<HW_CDICTRLCSR_CLKRZ_BITPOS)
HW_CDICTRLCSR_LSBFIRST_SETMASK  equ     (1<<HW_CDICTRLCSR_LSBFIRST_BITPOS)
HW_CDICTRLCSR_LENGTH_SETMASK    equ     ($1F<<HW_CDICTRLCSR_LENGTH_BITPOS)
HW_CDICTRLCSR_MSBJUSTIFY_SETMASK equ    (1<<HW_CDICTRLCSR_MSBJUSTIFY_BITPOS)
HW_CDICTRLCSR_LATCHEN_SETMASK   equ     (1<<HW_CDICTRLCSR_LATCHEN_BITPOS)
HW_CDICTRLCSR_LATCHPOL_SETMASK  equ     (1<<HW_CDICTRLCSR_LATCHPOL_BITPOS)
HW_CDICTRLCSR_IRQEN_SETMASK     equ     (1<<HW_CDICTRLCSR_IRQEN_BITPOS)
HW_CDICTRLCSR_TFRDONE_SETMASK   equ     (1<<HW_CDICTRLCSR_TFRDONE_BITPOS)
HW_CDICTRLCSR_RESET_SETMASK     equ     (1<<HW_CDICTRLCSR_RESET_BITPOS)

HW_CDICTRLCSR_KICK_CLRMASK     		equ     (~HW_CDICTRLCSR_KICK_SETMASK)
HW_CDICTRLCSR_CLKPOL_CLRMASK     	equ     (~HW_CDICTRLCSR_CLKPOL_SETMASK)
HW_CDICTRLCSR_CLKRZ_CLRMASK      	equ     (~HW_CDICTRLCSR_CLKRZ_SETMASK)
HW_CDICTRLCSR_LSBFIRST_CLRMASK      equ     (~HW_CDICTRLCSR_LSBFIRST_SETMASK)
HW_CDICTRLCSR_LENGTH_CLRMASK      	equ     (~HW_CDICTRLCSR_LENGTH_SETMASK)
HW_CDICTRLCSR_MSBJUSTIFY_CLRMASK    equ     (~HW_CDICTRLCSR_MSBJUSTIFY_SETMASK)
HW_CDICTRLCSR_LATCHEN_CLRMASK      	equ     (~HW_CDICTRLCSR_LATCHEN_SETMASK)
HW_CDICTRLCSR_LATCHPOL_CLRMASK      equ     (~HW_CDICTRLCSR_LATCHPOL_SETMASK)
HW_CDICTRLCSR_IRQEN_CLRMASK      	equ     (~HW_CDICTRLCSR_IRQEN_SETMASK)
HW_CDICTRLCSR_TFRDONE_CLRMASK      	equ     (~HW_CDICTRLCSR_TFRDONE_SETMASK)
HW_CDICTRLCSR_RESET_CLRMASK      	equ     (~HW_CDICTRLCSR_RESET_SETMASK)


;C insert
;typedef union               /* CD-DSP Interface  Control Unit Control/Status Register  */
;{
;    struct {
;		int KICK		:1;	/* Start Transaction */
;		int CLKPOL		:1;	/* Control Clock Polarity */
;		int CLKRZ		:1; /* Control Clock RZ */
;		int LSBFIRST	:1;	/* Send Data LSB First */
;		unsigned LENGTH :5; /* Number of Bits to Transfer */
;		int MSBJUSTIFY	:1;	/* Selects if valid data is Right or Left Justified */
;		int LATCHEN		:1;	/* Enable Latch at end of transfer */
;		int LATCHPOL	:1; /* Selects Latch Polarity */
;		int 			:8;	/* Reserved */
;		int IRQEN		:1;	/* Enable the Done IRQ */
;		int TFRDONE		:1;	/* Transfer Complete Flag */
;		int				:1;	/* Reserved */
;		int RESET		:1;	/* Reset and Low Power */
;		} B;
;    unsigned int I;
;} cdi_ctrlcsr_type;

;#define HW_CDICTRLCSR 	(*(volatile cdi_ctrlcsr_type _X*) (HW_CDI_BASEADDR))		/* CDI Control Port Control/Status Register */
;endc

;/////////////////////////////////////////////////////////////////////////////////
;//  CDI Time Base Register bitfields

;// CDI Control Port Time Base Register Bit Defs
HW_CDICTRLTBR_DIVIDE_BITPOS     equ     (0)

HW_CDICTRLTBR_DIVIDE_SETMASK    equ  	($3FF<<HW_CDICTRLTBR_DIVIDE_BITPOS)

HW_CDICTRLTBR_DIVIDE_CLRMASK    equ     (~HW_CDICTRLTBR_DIVIDE_SETMASK)


;// CDI Serial Input Unit 0 Time Base Register Bit Defs
HW_CDISI0TBR_DIVIDE_BITPOS       equ     (0)

HW_CDISI0TBR_DIVIDE_SETMASK      equ     ($3FF<<HW_CDISI0TBR_DIVIDE_BITPOS)

HW_CDISI0TBR_DIVIDE_CLRMASK      equ     (~HW_CDISI0TBR_DIVIDE_SETMASK)

;// CDI Serial Input Unit 1 Time Base Register Bit Defs
HW_CDISI1TBR_DIVIDE_BITPOS       equ     (0)

HW_CDISI1TBR_DIVIDE_SETMASK      equ     ($3FF<<HW_CDISI1TBR_DIVIDE_BITPOS)

HW_CDISI1TBR_DIVIDE_CLRMASK      equ     (~HW_CDISI1TBR_DIVIDE_SETMASK)

;C insert
;typedef union
;{
;    struct { 
;		unsigned DIVIDE	:10;	// Clock Divide Value for generating the bit Clock
;		int				:14;	// Reserved
;		} B;
;    unsigned int I;
;} cdi_tbr_type;

;#define HW_CDICTRLTBR 	(*(volatile cdi_tbr_type _X*) (HW_CDI_BASEADDR+2))	  	/* CDI Control Port Time Base Register  */
;#define HW_CDISI0TBR 	(*(volatile cdi_tbr_type _X*) (HW_CDI_BASEADDR+10))		/* CDI Serial Input Unit 0 Time Base Register  */
;#define HW_CDISI1TBR 	(*(volatile cdi_tbr_type _X*) (HW_CDI_BASEADDR+14))		/* CDI Serial Input Unit 1 Time Base Register  */


;endc


;/////////////////////////////////////////////////////////////////////////////////
;//  CDI Control Port Timer Register


HW_CDICTRLTMRR_LATCHSTART_BITPOS    equ     (0)
HW_CDICTRLTMRR_LATCHWIDTH_BITPOS    equ     (8)
HW_CDICTRLTMRR_FINALDELAY_BITPOS    equ     (16)

HW_CDICTRLTMRR_LATCHSTART_SETMASK   equ     ($FF<<HW_CDICTRLTMRR_LATCHSTART_BITPOS)
HW_CDICTRLTMRR_LATCHWIDTH_SETMASK   equ     ($FF<<HW_CDICTRLTMRR_LATCHWIDTH_BITPOS)
HW_CDICTRLTMRR_FINALDELAY_SETMASK   equ     ($FF<<HW_CDICTRLTMRR_FINALDELAY_BITPOS)

HW_CDICTRLTMRR_LATCHSTART_CLRMASK      	equ     (~HW_CDICTRLTMRR_LATCHSTART_SETMASK)
HW_CDICTRLTMRR_LATCHWIDTH_CLRMASK      	equ     (~HW_CDICTRLTMRR_LATCHWIDTH_SETMASK)
HW_CDICTRLTMRR_FINALDELAY_CLRMASK      	equ     (~HW_CDICTRLTMRR_FINALDELAY_SETMASK)

;C insert
;typedef union      
;{
;    struct { 
;		unsigned LATCHSTART	:8;	// Clock Divide Value for generating the bit Clock
;		unsigned LATCHWIDTH	:8;	// Number of bit Clocks to hold the latch asserted
;		unsigned FINALDELAY :8;	// Number of bit clocks to wait before generating a done IRQ
;		} B;
;    unsigned int I;
;} cdi_ctrltmrr_type;
;#define HW_CDICTRLTMRR 	(*(volatile cdi_ctrltmrr_type _X*) (HW_CDI_BASEADDR+1))	/* CDI Control Port Timer Register */
;endc


;/////////////////////////////////////////////////////////////////////////////////
;//// Bit Defs for CDI Serial Input Units 0 and 1

HW_CDISI0TMRR_LATCHSTART_BITPOS         equ     (0)
HW_CDISI0TMRR_FINALDELAY_BITPOS         equ     (16)

HW_CDISI0TMRR_LATCHSTART_SETMASK        equ     ($FF<<HW_CDISI0TMRR_LATCHSTART_BITPOS)
HW_CDISI0TMRR_FINALDELAY_SETMASK        equ     ($FF<<HW_CDISI0TMRR_FINALDELAY_BITPOS)

HW_CDISI0TMRR_LATCHSTART_CLRMASK      	equ     (~HW_CDISI0TMRR_LATCHSTART_SETMASK)
HW_CDISI0TMRR_FINALDELAY_CLRMASK      	equ     (~HW_CDISI0TMRR_FINALDELAY_SETMASK)


HW_CDISI1TMRR_LATCHSTART_BITPOS         equ     (0)
HW_CDISI1TMRR_FINALDELAY_BITPOS         equ     (16)

HW_CDISI1TMRR_LATCHSTART_SETMASK      	equ     ($FF<<HW_CDISI1TMRR_LATCHSTART_BITPOS)
HW_CDISI1TMRR_FINALDELAY_SETMASK      	equ     ($FF<<HW_CDISI1TMRR_FINALDELAY_BITPOS)

HW_CDISI1TMRR_LATCHSTART_CLRMASK      	equ     (~HW_CDISI1TMRR_LATCHSTART_SETMASK)
HW_CDISI1TMRR_FINALDELAY_CLRMASK      	equ     (~HW_CDISI1TMRR_FINALDELAY_SETMASK)

;C insert
;typedef union     // CDI Serial Input Unit Timer Register
;{
;    struct { 
;		unsigned LATCHSTART	:8;	// Clock Divide Value for generating the bit Clock
;		unsigned 			:8;	// Reserved
;		unsigned FINALDELAY :8;	// Number of bit clocks to wait before generating a done IRQ
;		} B;
;    unsigned int I;
;} cdi_sitmrr_type;

;#define HW_CDISI0TMRR 	(*(volatile cdi_sitmrr_type _X*) (HW_CDI_BASEADDR+9)) 	/* CDI Serial Input Unit 0 Timer Register */
;#define HW_CDISI1TMRR 	(*(volatile cdi_sitmrr_type _X*) (HW_CDI_BASEADDR+13))	/* CDI Serial Input Unit 1 Timer Register */
;endc


;/////////////////////////////////////////////////////////////////////////////////
;//  CDI Data Registers (Control, Serial in 0,1)


HW_CDICTRLDATAR_DATA_BITPOS             equ     (0)

HW_CDICTRLDATAR_DATA_SETMASK      	    equ     ($FFFFFF<<HW_CDICTRLDATAR_DATA_BITPOS)

HW_CDICTRLDATAR_DATA_CLRMASK      	    equ     (~HW_CDICTRLDATAR_DATA_SETMASK)


;// Bit Defs for CDI Serial Input Unit 0
HW_CDISI0DATAR_DATA_BITPOS              equ     (0)

HW_CDISI0DATAR_DATA_SETMASK      	    equ     ($FFFFFF<<HW_CDISI0DATAR_DATA_BITPOS)

HW_CDISI0DATAR_DATA_CLRMASK      	    equ     (~HW_CDISI0DATAR_DATA_SETMASK)


;// Bit Defs for CDI Serial Input Unit 1
HW_CDISI1DATAR_DATA_BITPOS              equ     (0)

HW_CDISI1DATAR_DATA_SETMASK      	    equ     ($FFFFFF<<HW_CDISI1DATAR_DATA_BITPOS)

HW_CDISI1DATAR_DATA_CLRMASK      	    equ     (~HW_CDISI1DATAR_DATA_SETMASK)

;C insert
;typedef union     
;{
;    struct { 
;		unsigned DATA	:24;	// Data for Input or Output
;		} B;
;    unsigned int I;
;} cdi_datar_type;

;#define HW_CDICTRLDATAR (*(volatile cdi_datar_type _X*) (HW_CDI_BASEADDR+3))	/* CDI Control Unit Data Register  */
;#define HW_CDISI0DATAR 	(*(volatile cdi_datar_type _X*) (HW_CDI_BASEADDR+11))	/* CDI Serial Input Unit 0 Data Register */		 
;#define HW_CDISI1DATAR 	(*(volatile cdi_datar_type _X*) (HW_CDI_BASEADDR+15))	/* CDI Serial Input Unit 1 Data Register */

;endc


;/////////////////////////////////////////////////////////////////////////////////
;// CDI Pin Configuration Register Bit defs

HW_CDIPINCFGR_ENABLE_BITPOS		    equ     (0)
HW_CDIPINCFGR_CTL_BITPOS            equ     (1)
HW_CDIPINCFGR_SCLK_BITPOS           equ     (2)
HW_CDIPINCFGR_CTLSO_BITPOS          equ     (4)
HW_CDIPINCFGR_SYNCA_BITPOS          equ     (5)
HW_CDIPINCFGR_SENSE_BITPOS          equ     (6)
HW_CDIPINCFGR_CTRLSCLK_BITPOS       equ     (7)
HW_CDIPINCFGR_CCLK_BITPOS           equ     (8)
HW_CDIPINCFGR_CTLOUTEN_BITPOS       equ     (10)
HW_CDIPINCFGR_SCLKOUTEN_BITPOS      equ     (11)

HW_CDIPINCFGR_ENABLE_SETMASK   	    equ     (1<<HW_CDIPINCFGR_ENABLE_BITPOS)
HW_CDIPINCFGR_CTL_SETMASK      	    equ     (1<<HW_CDIPINCFGR_CTL_BITPOS)
HW_CDIPINCFGR_SCLK_SETMASK          equ     (3<<HW_CDIPINCFGR_SCLK_BITPOS)
HW_CDIPINCFGR_CTLSO_SETMASK         equ     (1<<HW_CDIPINCFGR_CTLSO_BITPOS)
HW_CDIPINCFGR_SYNCA_SETMASK         equ     (1<<HW_CDIPINCFGR_SYNCA_BITPOS)
HW_CDIPINCFGR_SENSE_SETMASK         equ     (1<<HW_CDIPINCFGR_SENSE_BITPOS)
HW_CDIPINCFGR_CTRLSCLK_SETMASK      equ     (1<<HW_CDIPINCFGR_CTRLSCLK_BITPOS)
HW_CDIPINCFGR_CCLK_SETMASK          equ     (3<<HW_CDIPINCFGR_CCLK_BITPOS)
HW_CDIPINCFGR_CTLOUTEN_SETMASK      equ     (1<<HW_CDIPINCFGR_CTLOUTEN_BITPOS)
HW_CDIPINCFGR_SCLKOUTEN_SETMASK     equ     (1<<HW_CDIPINCFGR_SCLKOUTEN_BITPOS)

HW_CDIPINCFGR_ENABLE_CLRMASK   	equ     (~HW_CDIPINCFGR_ENABLE_SETMASK)
HW_CDIPINCFGR_CTL_CLRMASK      	equ     (~HW_CDIPINCFGR_CTL_SETMASK)
HW_CDIPINCFGR_SCLK_CLRMASK      equ     (~HW_CDIPINCFGR_SCLK_SETMASK)
HW_CDIPINCFGR_CTLSO_CLRMASK     equ     (~HW_CDIPINCFGR_CTLSO_SETMASK)
HW_CDIPINCFGR_SYNCA_CLRMASK     equ     (~HW_CDIPINCFGR_SYNCA_SETMASK)
HW_CDIPINCFGR_SENSE_CLRMASK     equ     (~HW_CDIPINCFGR_SENSE_SETMASK)
HW_CDIPINCFGR_CTRLSCLK_CLRMASK  equ     (~HW_CDIPINCFGR_CTRLSCLK_SETMASK)
HW_CDIPINCFGR_CCLK_CLRMASK      equ     (~HW_CDIPINCFGR_CCLK_SETMASK)
HW_CDIPINCFGR_CTLOUTEN_CLRMASK  equ     (~HW_CDIPINCFGR_CTLOUTEN_SETMASK)
HW_CDIPINCFGR_SCLKOUTEN_CLRMASK equ     (~HW_CDIPINCFGR_SCLKOUTEN_SETMASK)

;C insert
;typedef union     // CDI Pin Configuration Register
;{
;    struct { 
;		int ENABLE		:1;	// CDI Pin Enable
;		int	CTL			:1;	// CDI Control Unit Pin Select
;		unsigned SCLK	:2;	// Pin select for CTL_SCLK
;		int CTLSO		:1;	// CTL SO Pin Select
;		int SYNCA		:1;	// SI SYNCa pin select
;		int SENSE		:1;	// CTRL SENSE Pin select
;		int CTRLSCLK	:1;	// CTRL SCLK source select
;		unsigned CCLK  	:2;	// Control Clock Select
;		int CTLOUTEN 	:1;	// Enable output drivers on Control
;		int SCLKOUTEN	:1;	// Enable output drivers on SCLK
;		int				:12; // Reserved	   
;		} B;
;    unsigned int I;
;} cdi_pincfgr_type;

;#define HW_CDIPINCFGR 	(*(volatile cdi_pincfgr_type _X*) (HW_CDI_BASEADDR+4))	/* CDI Pin Configuration Register  */
;endc

;/////////////////////////////////////////////////////////////////////////////////
;//  CD-DSP Interface Serial Input Unit Control Status Register (HW_CDI_SIxCSR) Bit Definitions

;// Serial Input Unit 0 CSR Bit Defs
HW_CDISI0CSR_KICK_BITPOS		    equ     (0)
HW_CDISI0CSR_CLKPOL_BITPOS		    equ     (1)
HW_CDISI0CSR_CLKRZ_BITPOS		    equ     (2)
HW_CDISI0CSR_LSBFIRST_BITPOS	    equ     (3)
HW_CDISI0CSR_LENGTH_BITPOS		    equ     (4)
HW_CDISI0CSR_MSBJUSTIFY_BITPOS	    equ     (9)
HW_CDISI0CSR_SCLKRZ_BITPOS		    equ     (10)
HW_CDISI0CSR_SYNCEN_BITPOS		    equ     (11)
HW_CDISI0CSR_SYNCPOL_BITPOS		    equ     (12)
HW_CDISI0CSR_CRCEN_BITPOS		    equ     (13)
HW_CDISI0CSR_CRCPOL_BITPOS		    equ     (14)
HW_CDISI0CSR_CLKSEL_BITPOS		    equ     (15)
HW_CDISI0CSR_DATASEL_BITPOS		    equ     (16)
HW_CDISI0CSR_SYNCSEL_BITPOS		    equ     (17)
HW_CDISI0CSR_IRQEN_BITPOS		    equ     (20)
HW_CDISI0CSR_DONE_BITPOS		    equ     (21)
HW_CDISI0CSR_RESET_BITPOS		    equ     (23)

HW_CDISI0CSR_KICK_SETMASK   		    equ     (1<<HW_CDISI0CSR_KICK_BITPOS)
HW_CDISI0CSR_CLKPOL_SETMASK   	    equ     (1<<HW_CDISI0CSR_CLKPOL_BITPOS)
HW_CDISI0CSR_CLKRZ_SETMASK   	    equ     (1<<HW_CDISI0CSR_CLKRZ_BITPOS)
HW_CDISI0CSR_LSBFIRST_SETMASK   	    equ     (1<<HW_CDISI0CSR_LSBFIRST_BITPOS)
HW_CDISI0CSR_LENGTH_SETMASK   	    equ     ($1F<<HW_CDISI0CSR_LENGTH_BITPOS)
HW_CDISI0CSR_MSBJUSTIFY_SETMASK      equ     (1<<HW_CDISI0CSR_MSBJUSTIFY_BITPOS)
HW_CDISI0CSR_SCLKRZ_SETMASK   	    equ     (1<<HW_CDISI0CSR_SCLKRZ_BITPOS)
HW_CDISI0CSR_SYNCEN_SETMASK   	    equ     (1<<HW_CDISI0CSR_SYNCEN_BITPOS)
HW_CDISI0CSR_SYNCPOL_SETMASK   	    equ     (1<<HW_CDISI0CSR_SYNCPOL_BITPOS)
HW_CDISI0CSR_CRCEN_SETMASK   	    equ     (1<<HW_CDISI0CSR_CRCEN_BITPOS)
HW_CDISI0CSR_CRCPOL_SETMASK   	    equ     (1<<HW_CDISI0CSR_CRCPOL_BITPOS)
HW_CDISI0CSR_CLKSEL_SETMASK   	    equ     (1<<HW_CDISI0CSR_CLKSEL_BITPOS)
HW_CDISI0CSR_DATASEL_SETMASK   	    equ     (1<<HW_CDISI0CSR_DATASEL_BITPOS)
HW_CDISI0CSR_SYNCSEL_SETMASK   	    equ     (3<<HW_CDISI0CSR_SYNCSEL_BITPOS)
HW_CDISI0CSR_IRQEN_SETMASK   	    equ     (1<<HW_CDISI0CSR_IRQEN_BITPOS)
HW_CDISI0CSR_DONE_SETMASK   		    equ     (1<<HW_CDISI0CSR_DONE_BITPOS)
HW_CDISI0CSR_RESET_SETMASK   	    equ     (1<<HW_CDISI0CSR_RESET_BITPOS)										
HW_CDISI0CSR_KICK_CLRMASK   		equ     (~HW_CDISI0CSR_KICK_SETMASK)
HW_CDISI0CSR_CLKPOL_CLRMASK   	equ     (~HW_CDISI0CSR_CLKPOL_SETMASK)
HW_CDISI0CSR_CLKRZ_CLRMASK   	equ     (~HW_CDISI0CSR_CLKRZ_SETMASK)
HW_CDISI0CSR_LSBFIRST_CLRMASK   	equ     (~HW_CDISI0CSR_LSBFIRST_SETMASK)
HW_CDISI0CSR_LENGTH_CLRMASK   	equ     (~HW_CDISI0CSR_LENGTH_SETMASK)
HW_CDISI0CSR_MSBJUSTIFY_CLRMASK  equ     (~HW_CDISI0CSR_MSBJUSTIFY_SETMASK)
HW_CDISI0CSR_SCLKRZ_CLRMASK   	equ     (~HW_CDISI0CSR_SCLKRZ_SETMASK)
HW_CDISI0CSR_SYNCEN_CLRMASK   	equ     (~HW_CDISI0CSR_SYNCEN_SETMASK)
HW_CDISI0CSR_SYNCPOL_CLRMASK   	equ     (~HW_CDISI0CSR_SYNCPOL_SETMASK)
HW_CDISI0CSR_CRCEN_CLRMASK   	equ     (~HW_CDISI0CSR_CRCEN_SETMASK)
HW_CDISI0CSR_CRCPOL_CLRMASK   	equ     (~HW_CDISI0CSR_CRCPOL_SETMASK)
HW_CDISI0CSR_CLKSEL_CLRMASK   	equ     (~HW_CDISI0CSR_CLKSEL_SETMASK)
HW_CDISI0CSR_DATASEL_CLRMASK   	equ     (~HW_CDISI0CSR_DATASEL_SETMASK)
HW_CDISI0CSR_SYNCSEL_CLRMASK   	equ     (~HW_CDISI0CSR_SYNCSEL_SETMASK)
HW_CDISI0CSR_IRQEN_CLRMASK   	equ     (~HW_CDISI0CSR_IRQEN_SETMASK)
HW_CDISI0CSR_DONE_CLRMASK   		equ     (~HW_CDISI0CSR_DONE_SETMASK)
HW_CDISI0CSR_RESET_CLRMASK   	equ     (~HW_CDISI0CSR_RESET_SETMASK)

;// Serial Input Unit 1 CSR Bit Defs
HW_CDISI1CSR_KICK_BITPOS		equ     (0)
HW_CDISI1CSR_CLKPOL_BITPOS		equ     (1)
HW_CDISI1CSR_CLKRZ_BITPOS		equ     (2)
HW_CDISI1CSR_LSBFIRST_BITPOS	equ     (3)
HW_CDISI1CSR_LENGTH_BITPOS		equ     (4)
HW_CDISI1CSR_MSBJUSTIFY_BITPOS	equ     (9)
HW_CDISI1CSR_SCLKRZ_BITPOS		equ     (10)
HW_CDISI1CSR_SYNCEN_BITPOS		equ     (11)
HW_CDISI1CSR_SYNCPOL_BITPOS		equ     (12)
HW_CDISI1CSR_CRCEN_BITPOS		equ     (13)
HW_CDISI1CSR_CRCPOL_BITPOS		equ     (14)
HW_CDISI1CSR_CLKSEL_BITPOS		equ     (15)
HW_CDISI1CSR_DATASEL_BITPOS		equ     (16)
HW_CDISI1CSR_SYNCSEL_BITPOS		equ     (17)
HW_CDISI1CSR_IRQEN_BITPOS		equ     (20)
HW_CDISI1CSR_DONE_BITPOS		equ     (21)
HW_CDISI1CSR_RESET_BITPOS		equ     (23)

HW_CDISI1CSR_KICK_SETMASK   	equ     (1<<HW_CDISI1CSR_KICK_BITPOS)
HW_CDISI1CSR_CLKPOL_SETMASK   	equ     (1<<HW_CDISI1CSR_CLKPOL_BITPOS)
HW_CDISI1CSR_CLKRZ_SETMASK   	equ     (1<<HW_CDISI1CSR_CLKRZ_BITPOS)
HW_CDISI1CSR_LSBFIRST_SETMASK   equ     (1<<HW_CDISI1CSR_LSBFIRST_BITPOS)
HW_CDISI1CSR_LENGTH_SETMASK   	equ     ($1F<<HW_CDISI1CSR_LENGTH_BITPOS)
HW_CDISI1CSR_MSBJUSTIFY_SETMASK equ     (1<<HW_CDISI1CSR_MSBJUSTIFY_BITPOS)
HW_CDISI1CSR_SCLKRZ_SETMASK   	equ     (1<<HW_CDISI1CSR_SCLKRZ_BITPOS)
HW_CDISI1CSR_SYNCEN_SETMASK   	equ     (1<<HW_CDISI1CSR_SYNCEN_BITPOS)
HW_CDISI1CSR_SYNCPOL_SETMASK   	equ     (1<<HW_CDISI1CSR_SYNCPOL_BITPOS)
HW_CDISI1CSR_CRCEN_SETMASK   	equ     (1<<HW_CDISI1CSR_CRCEN_BITPOS)
HW_CDISI1CSR_CRCPOL_SETMASK   	equ     (1<<HW_CDISI1CSR_CRCPOL_BITPOS)
HW_CDISI1CSR_CLKSEL_SETMASK   	equ     (1<<HW_CDISI1CSR_CLKSEL_BITPOS)
HW_CDISI1CSR_DATASEL_SETMASK   	equ     (1<<HW_CDISI1CSR_DATASEL_BITPOS)
HW_CDISI1CSR_SYNCSEL_SETMASK   	equ     (3<<HW_CDISI1CSR_SYNCSEL_BITPOS)
HW_CDISI1CSR_IRQEN_SETMASK   	equ     (1<<HW_CDISI1CSR_IRQEN_BITPOS)
HW_CDISI1CSR_DONE_SETMASK   	equ     (1<<HW_CDISI1CSR_DONE_BITPOS)
HW_CDISI1CSR_RESET_SETMASK   	equ     (1<<HW_CDISI1CSR_RESET_BITPOS)

HW_CDISI1CSR_KICK_CLRMASK   	equ     (~HW_CDISI1CSR_KICK_SETMASK)
HW_CDISI1CSR_CLKPOL_CLRMASK   	equ     (~HW_CDISI1CSR_CLKPOL_SETMASK)
HW_CDISI1CSR_CLKRZ_CLRMASK   	equ     (~HW_CDISI1CSR_CLKRZ_SETMASK)
HW_CDISI1CSR_LSBFIRST_CLRMASK   equ     (~HW_CDISI1CSR_LSBFIRST_SETMASK)
HW_CDISI1CSR_LENGTH_CLRMASK   	equ     (~HW_CDISI1CSR_LENGTH_SETMASK)
HW_CDISI1CSR_MSBJUSTIFY_CLRMASK equ     (~HW_CDISI1CSR_MSBJUSTIFY_SETMASK)
HW_CDISI1CSR_SCLKRZ_CLRMASK   	equ     (~HW_CDISI1CSR_SCLKRZ_SETMASK)
HW_CDISI1CSR_SYNCEN_CLRMASK   	equ     (~HW_CDISI1CSR_SYNCEN_SETMASK)
HW_CDISI1CSR_SYNCPOL_CLRMASK   	equ     (~HW_CDISI1CSR_SYNCPOL_SETMASK)
HW_CDISI1CSR_CRCEN_CLRMASK   	equ     (~HW_CDISI1CSR_CRCEN_SETMASK)
HW_CDISI1CSR_CRCPOL_CLRMASK   	equ     (~HW_CDISI1CSR_CRCPOL_SETMASK)
HW_CDISI1CSR_CLKSEL_CLRMASK   	equ     (~HW_CDISI1CSR_CLKSEL_SETMASK)
HW_CDISI1CSR_DATASEL_CLRMASK   	equ     (~HW_CDISI1CSR_DATASEL_SETMASK)
HW_CDISI1CSR_SYNCSEL_CLRMASK   	equ     (~HW_CDISI1CSR_SYNCSEL_SETMASK)
HW_CDISI1CSR_IRQEN_CLRMASK   	equ     (~HW_CDISI1CSR_IRQEN_SETMASK)
HW_CDISI1CSR_DONE_CLRMASK   	equ     (~HW_CDISI1CSR_DONE_SETMASK)
HW_CDISI1CSR_RESET_CLRMASK   	equ     (~HW_CDISI1CSR_RESET_SETMASK)

;C insert
;typedef union       
;{
;    struct {
;		int KICK		:1;	/* Start Transaction */
;		int CLKPOL		:1;	/* Clock Polarity */
;		int CLKRZ		:1; /* Clock Return to Zero select */
;		int LSBFIRST	:1;	/* Send Data LSB First */
;		unsigned LENGTH :5; /* Nubber of Bits to Transfer */
;		int MSBJUSTIFY	:1;	/* Selects if valid data is Right or Left Justified */
;		int SCLKRZ		:1;	/* SClock Return to Zero select */
;		int SYNCEN		:1; /* Sync Enable */
;		int SYNCPOL		:1;	/* Sync Polarity */
;		int CRCEN		:1;	/* Enable the CRC */
;		int CRCPOL		:1;	/* CRC Polarity */
;		int CLKSEL		:1;	/* Clock Select */
;		int DATASEL		:1;	/* Data input selector */
;		unsigned SYNCSEL :2;/* Sync Signal Selector */
;		int				:1;	/* Reserved */
;		int IRQEN		:1;	/* Interrupt Enable */
;		int DONE		:1; /* Transfer Done Flag */
;		int				:1;	/* Reserved */
;		int RESET		:1;	/* Reset and Low Power */
;		} B;
;    unsigned int I;
;} cdi_sicsr_type;

;#define HW_CDISI0CSR 	(*(volatile cdi_sicsr_type _X*) (HW_CDI_BASEADDR+8))  	/* CDI Serial Input Unit 0 Control/Status Register  */
;#define HW_CDISI1CSR 	(*(volatile cdi_sicsr_type _X*) (HW_CDI_BASEADDR+12)) 	/* CDI Serial Input Unit 1 Control/Status Register  */
;endc
    endif
