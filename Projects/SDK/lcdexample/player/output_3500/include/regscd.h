//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;  Copyright(C) SigmaTel, Inc. 2002-2003
//;  File        : regscd.inc
//;  Description : CD-DSP Interface Registers (CDI, CDSync, CD Reed-Solomon)
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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

#if !(defined(regscdinc))
#define regscdinc 1

#include "types.h"


///////////////////////////////////////////////////////////////////////////////////
//// CD Sync Registers
///////////////////////////////////////////////////////////////////////////////////
#define HW_CDSYNC_BASEADDR (0xF600)


///////////////////////////////////////////////////////////////////////////////////
//  CD Sync Control Status Register (HW_CDSYNCCSR) Bit Definitions

#define HW_CDSYNCCSR_EN_BITPOS (0)
#define HW_CDSYNCCSR_CRC_BITPOS (1)
#define HW_CDSYNCCSR_DSCRAM_BITPOS (2)
#define HW_CDSYNCCSR_SYNCEN_BITPOS (3)
#define HW_CDSYNCCSR_DMAIRQEN_BITPOS (4)
#define HW_CDSYNCCSR_EDCIRQEN_BITPOS (5)
#define HW_CDSYNCCSR_LOSIRQEN_BITPOS (6)
#define HW_CDSYNCCSR_SYNCIRQEN_BITPOS (7)
#define HW_CDSYNCCSR_DMADONE_BITPOS (8)
#define HW_CDSYNCCSR_EDC_BITPOS (9)
#define HW_CDSYNCCSR_LOS_BITPOS (10)
#define HW_CDSYNCCSR_SYNC_BITPOS (11)
#define HW_CDSYNCCSR_DMAOF_BITPOS (12)
#define HW_CDSYNCCSR_MODE1_BITPOS (18)
#define HW_CDSYNCCSR_INPUTMSB_BITPOS (19)
#define HW_CDSYNCCSR_OUTWRDLEN_BITPOS (20)
#define HW_CDSYNCCSR_OUTWRDLEN0_BITPOS (20)
#define HW_CDSYNCCSR_OUTWRDLEN1_BITPOS (21)
#define HW_CDSYNCCSR_INWRDLEN_BITPOS (22)
#define HW_CDSYNCCSR_RESET_BITPOS (23)

#define HW_CDSYNCCSR_EN_SETMASK (1<<HW_CDSYNCCSR_EN_BITPOS)  
#define HW_CDSYNCCSR_CRC_SETMASK (1<<HW_CDSYNCCSR_CRC_BITPOS) 
#define HW_CDSYNCCSR_DSCRAM_SETMASK (1<<HW_CDSYNCCSR_DSCRAM_BITPOS)    
#define HW_CDSYNCCSR_SYNCEN_SETMASK (1<<HW_CDSYNCCSR_SYNCEN_BITPOS)  
#define HW_CDSYNCCSR_DMAIRQEN_SETMASK (1<<HW_CDSYNCCSR_DMAIRQEN_BITPOS)  
#define HW_CDSYNCCSR_EDCIRQEN_SETMASK (1<<HW_CDSYNCCSR_EDCIRQEN_BITPOS)  
#define HW_CDSYNCCSR_LOSIRQEN_SETMASK (1<<HW_CDSYNCCSR_LOSIRQEN_BITPOS)  
#define HW_CDSYNCCSR_SYNCIRQEN_SETMASK (1<<HW_CDSYNCCSR_SYNCIRQEN_BITPOS)  
#define HW_CDSYNCCSR_DMADONE_SETMASK (1<<HW_CDSYNCCSR_DMADONE_BITPOS)  
#define HW_CDSYNCCSR_EDC_SETMASK (1<<HW_CDSYNCCSR_EDC_BITPOS)  
#define HW_CDSYNCCSR_LOS_SETMASK (1<<HW_CDSYNCCSR_LOS_BITPOS)  
#define HW_CDSYNCCSR_SYNC_SETMASK (1<<HW_CDSYNCCSR_SYNC_BITPOS)  
#define HW_CDSYNCCSR_DMAOF_SETMASK (1<<HW_CDSYNCCSR_DMAOF_BITPOS)	
#define HW_CDSYNCCSR_MODE1_SETMASK (1<<HW_CDSYNCCSR_MODE1_BITPOS) 
#define HW_CDSYNCCSR_INPUTMSB_SETMASK (1<<HW_CDSYNCCSR_INPUTMSB_BITPOS)  
#define HW_CDSYNCCSR_OUTWRDLEN_SETMASK (3<<HW_CDSYNCCSR_OUTWRDLEN_BITPOS) 
#define HW_CDSYNCCSR_INWRDLEN_SETMASK (1<<HW_CDSYNCCSR_INWRDLEN_BITPOS)	
#define HW_CDSYNCCSR_RESET_SETMASK (1<<HW_CDSYNCCSR_RESET_BITPOS)  

#define HW_CDSYNCCSR_EN_CLRMASK (~(WORD)HW_CDSYNCCSR_EN_SETMASK)
#define HW_CDSYNCCSR_CRC_CLRMASK (~(WORD)HW_CDSYNCCSR_CRC_SETMASK)
#define HW_CDSYNCCSR_DSCRAM_CLRMASK (~(WORD)HW_CDSYNCCSR_DSCRAM_SETMASK)
#define HW_CDSYNCCSR_SYNCEN_CLRMASK (~(WORD)HW_CDSYNCCSR_SYNCEN_SETMASK)
#define HW_CDSYNCCSR_DMAIRQEN_CLRMASK (~(WORD)HW_CDSYNCCSR_DMAIRQEN_SETMASK)
#define HW_CDSYNCCSR_EDCIRQEN_CLRMASK (~(WORD)HW_CDSYNCCSR_EDCIRQEN_SETMASK)
#define HW_CDSYNCCSR_LOSIRQEN_CLRMASK (~(WORD)HW_CDSYNCCSR_LOSIRQEN_SETMASK)
#define HW_CDSYNCCSR_SYNCIRQEN_CLRMASK (~(WORD)HW_CDSYNCCSR_SYNCIRQEN_SETMASK)
#define HW_CDSYNCCSR_DMADONE_CLRMASK (~(WORD)HW_CDSYNCCSR_DMADONE_SETMASK)
#define HW_CDSYNCCSR_EDC_CLRMASK (~(WORD)HW_CDSYNCCSR_EDC_SETMASK)
#define HW_CDSYNCCSR_LOS_CLRMASK (~(WORD)HW_CDSYNCCSR_LOS_SETMASK)
#define HW_CDSYNCCSR_SYNC_CLRMASK (~(WORD)HW_CDSYNCCSR_SYNC_SETMASK)
#define HW_CDSYNCCSR_DMAOF_CLRMASK (~(WORD)HW_CDSYNCCSR_DMAOF_SETMASK)
#define HW_CDSYNCCSR_MODE1_CLRMASK (~(WORD)HW_CDSYNCCSR_MODE1_SETMASK)
#define HW_CDSYNCCSR_INPUTMSB_CLRMASK (~(WORD)HW_CDSYNCCSR_INPUTMSB_SETMASK)
#define HW_CDSYNCCSR_OUTWRDLEN_CLRMASK (~(WORD)HW_CDSYNCCSR_OUTWRDLEN_SETMASK)
#define HW_CDSYNCCSR_INWRDLEN_CLRMASK (~(WORD)HW_CDSYNCCSR_INWRDLEN_SETMASK)
#define HW_CDSYNCCSR_RESET_CLRMASK (~(WORD)HW_CDSYNCCSR_RESET_SETMASK)


typedef union               /* SPI Control/Status Register (SPCSR)  */
{
    struct {
       int		EN			:1;	// CDSync Enable
		int 	CRC			:1;	// CRC checking Enable
		int		DSCRAM		:1;	// Enable Descrambling
		int 	SYNCEN		:1;	// Enable Sync Detection
		int		DMAIRQEN 	:1; // Enable IRQ on DMA complete
		int		EDCIRQEN	:1;	// Enable IRQ on EDC errors
		int		LOSIRQEN	:1;	// Enable IRQ on Loss of Sync
		int 	SYNCIRQEN	:1;	// Enable IRQ on CD-ROM Frame Start
		int		DMADONE		:1;	// DMA is completed Status Bit
		int		EDC		   	:1;	// EDC status bit
		int		LOS			:1;	// Loss of Sync Indicator bit
		int 	SYNC		:1;	// Sync Found Flag
		int		DMAOF		:1;	// Indicates a DMA Overflow Exists
		unsigned			:5;	// Reserved
		int 	MODE1		:1;	// Force Mode1 Sectors
		int		INPUTMSB	:1;	// Indicates which input MSB to use
		unsigned OUTWRLEN	:2;	// Number of bytes to write in each output word
		unsigned INWRDLEN	:1;	// Number of bytes in each input word (2 or 3)
		int		RESET		:1;	// Soft Reset and Low Power Enable
            } B;
    unsigned int I;
} cdsync_csr_type;
#define HW_CDSYNCCSR 	(*(volatile cdsync_csr_type _X*) (HW_CDSYNC_BASEADDR))		/* CDSync Control/Status Register (SPCSR)  */



/////////////////////////////////////////////////////////
////  CDSync Input Word Register (CDSYNCDR) bit definitions


#define HW_CDSYNCDR_DATA_BITPOS (0)
#define HW_CDSYNCDR_DATA_SETMASK (0xFFFFFF<<HW_CDSYNCDR_DATA_BITPOS)
#define HW_CDSYNCDR_DATA_CLRMASK (~(WORD)HW_CDSYNCDR_DATA_SETMASK)  

typedef union     // CDSync Input Data Word Register
{
    struct { 
		unsigned DATA	:24;	// Data for Input 
		} B;
    unsigned int I;
} cdsync_datar_type;
#define HW_CDSYNCDR 	(*(volatile cdsync_datar_type _X*) (HW_CDSYNC_BASEADDR+1))	

/////////////////////////////////////////////////////////
////  CDSync Word Count Register (CDSYNCWCR) bit definitions

#define HW_CDSYNCWCR_COUNT_BITPOS (0)
#define HW_CDSYNCWCR_COUNT_SETMASK (0x1FFF<<HW_CDSYNCWCR_COUNT_BITPOS)
#define HW_CDSYNCWCR_COUNT_CLRMASK (~(WORD)HW_CDSYNCWCR_COUNT_SETMASK)  


typedef union     // CDSync Word Count Register
{
    struct { 
		unsigned COUNT	:13;	// Word Count
		unsigned		:11;	// Reserved
		} B;
    unsigned int I;
} cdsync_wcr_type;
#define HW_CDSYNCWCR 	(*(volatile cdsync_wcr_type _X*) (HW_CDSYNC_BASEADDR+2))  	/* CDSync Word Count Register */


/////////////////////////////////////////////////////////
////  CDSync Current Position Register (CDSYNCCPR) bit definitions

#define HW_CDSYNCCPR_POS_BITPOS (0)
#define HW_CDSYNCCPR_POS_SETMASK (0xFFF<<HW_CDSYNCCPR_POS_BITPOS)
#define HW_CDSYNCCPR_POS_CLRMASK (~(WORD)HW_CDSYNCCPR_POS_SETMASK)

typedef union     // CDSync Current Position Register
{
    struct { 
		unsigned POS	:12;	// Current Position 
		unsigned		:12;	// Reserved
		} B;
    unsigned int I;
} cdsync_cpr_type;
#define HW_CDSYNCCPR 	(*(volatile cdsync_cpr_type _X*) (HW_CDSYNC_BASEADDR+3))  	/* CDSync Current Position Register */

/////////////////////////////////////////////////////////
////  CDSync Modulo Register (CDSYNCMODR) bit definitions

#define HW_CDSYNCMR_MOD_BITPOS (0)
#define HW_CDSYNCMR_MOD_SETMASK (0x1FFF<<HW_CDSYNCMR_MOD_BITPOS)
#define HW_CDSYNCMR_MOD_CLRMASK (~(WORD)HW_CDSYNCMR_MOD_SETMASK)

typedef union     // CDSync Modulo Register
{
    struct { 
		unsigned MOD	:13;	// Modulo
		unsigned 		:11;	// Reserved
		} B;
    unsigned int I;
} cdsync_modr_type;
#define HW_CDSYNCMR 	(*(volatile cdsync_modr_type _X*) (HW_CDSYNC_BASEADDR+4))   /* CDSync Modulo Register */

/////////////////////////////////////////////////////////
////  CDSync Base Address Register (CDSYNCBAR) bit definitions

#define HW_CDSYNCBAR_BASE_BITPOS (0)
#define HW_CDSYNCBAR_BASE_SETMASK (0xFFFF<<HW_CDSYNCBAR_BASE_BITPOS)
#define HW_CDSYNCBAR_BASE_CLRMASK (~(WORD)HW_CDSYNCBAR_BASE_SETMASK)

typedef union     // CDSync Base Address Register
{
    struct { 
		unsigned BASE	:16;	// Base Address
		unsigned 		:8;		// Reserved
		} B;
    unsigned int I;
} cdsync_bar_type;
#define HW_CDSYNCBAR 	(*(volatile cdsync_bar_type _X*) (HW_CDSYNC_BASEADDR+5))  	/* CDSync Base Address Register */




///////////////////////////////////////////////////////////////////////////////////
//// Reed Solomon Registers
///////////////////////////////////////////////////////////////////////////////////
#define HW_RS_BASEADDR (0xF700)
///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Control Status Register (HW_RSCSR) Bit Definitions

#define HW_RSCSR_KICK_BITPOS (0)
#define HW_RSCSR_AUTOCORCT_BITPOS (1)   
#define HW_RSCSR_EVENEN_BITPOS (2)
#define HW_RSCSR_ODDEN_BITPOS (3)
#define HW_RSCSR_DONEIRQEN_BITPOS (4)   
#define HW_RSCSR_CORCTIRQEN_BITPOS (5)    
#define HW_RSCSR_ERRIRQEN_BITPOS (6)  
#define HW_RSCSR_DONE_BITPOS (8)
#define HW_RSCSR_CORRECT_BITPOS (9) 
#define HW_RSCSR_ERR_BITPOS (10)
#define HW_RSCSR_EVENCORCT_BITPOS (12)   
#define HW_RSCSR_EVENERROR_BITPOS (13)   
#define HW_RSCSR_ODDCORCT_BITPOS (14)  
#define HW_RSCSR_ODDERROR_BITPOS (15)  
#define HW_RSCSR_DMAWAIT_BITPOS (16)
#define HW_RSCSR_RESET_BITPOS (23)

#define HW_RSCSR_KICK_LENGTH (1)
#define HW_RSCSR_AUTOCORCT_LENGTH (1)   
#define HW_RSCSR_EVENEN_LENGTH (1)
#define HW_RSCSR_ODDEN_LENGTH (1)
#define HW_RSCSR_DONEIRQEN_LENGTH (1)   
#define HW_RSCSR_CORCTIRQEN_LENGTH (1)    
#define HW_RSCSR_ERRIRQEN_LENGTH (1)  
#define HW_RSCSR_DONE_LENGTH (1)
#define HW_RSCSR_CORRECT_LENGTH (1) 
#define HW_RSCSR_ERR_LENGTH (1)
#define HW_RSCSR_EVENCORCT_LENGTH (1)    
#define HW_RSCSR_EVENERROR_LENGTH (1)    
#define HW_RSCSR_ODDCORCT_LENGTH (1)   
#define HW_RSCSR_ODDERROR_LENGTH (1)   
#define HW_RSCSR_DMAWAIT_LENGTH (4) 
#define HW_RSCSR_RESET_LENGTH (1) 

#define HW_RSCSR_KICK_SETMASK (((1<<HW_RSCSR_KICK_LENGTH)-1)<<HW_RSCSR_KICK_BITPOS)
#define HW_RSCSR_AUTOCORCT_SETMASK (((1<<HW_RSCSR_AUTOCORCT_LENGTH)-1)<<HW_RSCSR_AUTOCORCT_BITPOS)
#define HW_RSCSR_EVENEN_SETMASK (((1<<HW_RSCSR_EVENEN_LENGTH)-1)<<HW_RSCSR_EVENEN_BITPOS)
#define HW_RSCSR_ODDEN_SETMASK (((1<<HW_RSCSR_ODDEN_LENGTH)-1)<<HW_RSCSR_ODDEN_BITPOS)
#define HW_RSCSR_DONEIRQEN_SETMASK (((1<<HW_RSCSR_DONEIRQEN_LENGTH)-1)<<HW_RSCSR_DONEIRQEN_BITPOS)
#define HW_RSCSR_CORCTIRQEN_SETMASK (((1<<HW_RSCSR_CORCTIRQEN_LENGTH)-1)<<HW_RSCSR_CORCTIRQEN_BITPOS)
#define HW_RSCSR_ERRIRQEN_SETMASK (((1<<HW_RSCSR_ERRIRQEN_LENGTH)-1)<<HW_RSCSR_ERRIRQEN_BITPOS)
#define HW_RSCSR_DONE_SETMASK (((1<<HW_RSCSR_DONE_LENGTH)-1)<<HW_RSCSR_DONE_BITPOS)
#define HW_RSCSR_CORRECT_SETMASK (((1<<HW_RSCSR_CORRECT_LENGTH)-1)<<HW_RSCSR_CORRECT_BITPOS)
#define HW_RSCSR_ERR_SETMASK (((1<<HW_RSCSR_ERR_LENGTH)-1)<<HW_RSCSR_ERR_BITPOS)
#define HW_RSCSR_EVENCORCT_SETMASK (((1<<HW_RSCSR_EVENCORCT_LENGTH)-1)<<HW_RSCSR_EVENCORCT_BITPOS)
#define HW_RSCSR_EVENERROR_SETMASK (((1<<HW_RSCSR_EVENERROR_LENGTH)-1)<<HW_RSCSR_EVENERROR_BITPOS)
#define HW_RSCSR_ODDCORCT_SETMASK (((1<<HW_RSCSR_ODDCORCT_LENGTH)-1)<<HW_RSCSR_ODDCORCT_BITPOS)
#define HW_RSCSR_ODDERROR_SETMASK (((1<<HW_RSCSR_ODDERROR_LENGTH)-1)<<HW_RSCSR_ODDERROR_BITPOS)
#define HW_RSCSR_DMAWAIT_SETMASK (((1<<HW_RSCSR_DMAWAIT_LENGTH)-1)<<HW_RSCSR_DMAWAIT_BITPOS)
#define HW_RSCSR_RESET_SETMASK (((1<<HW_RSCSR_RESET_LENGTH)-1)<<HW_RSCSR_RESET_BITPOS)


#define HW_RSCSR_KICK_CLRMASK (~(WORD)HW_RSCSR_KICK_SETMASK)
#define HW_RSCSR_AUTOCORCT_CLRMASK (~(WORD)HW_RSCSR_AUTOCORCT_SETMASK)
#define HW_RSCSR_EVENEN_CLRMASK (~(WORD)HW_RSCSR_EVENEN_SETMASK)
#define HW_RSCSR_ODDEN_CLRMASK (~(WORD)HW_RSCSR_ODDEN_SETMASK)
#define HW_RSCSR_DONEIRQEN_CLRMASK (~(WORD)HW_RSCSR_DONEIRQEN_SETMASK)
#define HW_RSCSR_CORCTIRQEN_CLRMASK (~(WORD)HW_RSCSR_CORCTIRQEN_SETMASK)
#define HW_RSCSR_ERRIRQEN_CLRMASK (~(WORD)HW_RSCSR_ERRIRQEN_SETMASK)
#define HW_RSCSR_DONE_CLRMASK (~(WORD)HW_RSCSR_DONE_SETMASK)
#define HW_RSCSR_CORRECT_CLRMASK (~(WORD)HW_RSCSR_CORRECT_SETMASK)
#define HW_RSCSR_ERR_CLRMASK (~(WORD)HW_RSCSR_ERR_SETMASK)
#define HW_RSCSR_EVENCORCT_CLRMASK (~(WORD)HW_RSCSR_EVENCORCT_SETMASK)
#define HW_RSCSR_EVENERROR_CLRMASK (~(WORD)HW_RSCSR_EVENERROR_SETMASK)
#define HW_RSCSR_ODDCORCT_CLRMASK (~(WORD)HW_RSCSR_ODDCORCT_SETMASK)
#define HW_RSCSR_ODDERROR_CLRMASK (~(WORD)HW_RSCSR_ODDERROR_SETMASK)
#define HW_RSCSR_DMAWAIT_CLRMASK (~(WORD)HW_RSCSR_DMAWAIT_SETMASK)
#define HW_RSCSR_RESET_CLRMASK (~(WORD)HW_RSCSR_RESET_SETMASK)



typedef union               
{
    struct {
       int		KICK		:1;	// Start processing
		int		AUTOCORCT	:1;	// Auto Correct errors
		int 	EVENEN		:1;	// Enable calculation of even codewords
		int 	ODDEN		:1;	// Enable calculation of odd codewords
		int 	DONEIRQEN	:1;	// Enable Done IRQ
		int		CORCTIRQEN	:1;	// Enable IRQ on correctable errors
		int		ERRIRQEN	:1;	// Enable IRQ for non-correctable errors
		int					:1;	// Reserved
		int		DONE		:1;	// Done Flag
		int		CORRECT		:1;	// Corrected errors Flag
		int		ERR			:1;	// Non-Corrected errors Flag
		int					:1;	// Reserved
		int		EVENCORCT	:1;	// Flag for Even error corrected
		int		EVENERROR	:1;	// Flag For Even error non-corrected
		int		ODDCORCT	:1;	// Flag for Odd error corrected
		int		ODDERROR	:1;	// Flag For Odd error non-corrected
		unsigned DMAWAIT	:4;	// Number of cycles to wait between DMA cycles
		int					:3;	// Reserved
		int 	RESET		:1;	// Reset /  Power Down			
    } B;
    unsigned int I;
} rs_csr_type;
#define HW_RSCSR 		(*(volatile RS_csr_type _X*) (HW_RS_BASEADDR))			/* RS Control/Status Register */




///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Error Offset Register (HW_RSOFFSETR) Bit Definitions

#define HW_RSOFFSETR_EVEN_BITPOS (0)
#define HW_RSOFFSETR_ODD_BITPOS (12)

#define HW_RSOFFSETR_EVEN_SETMASK (0xFFF<<HW_RSOFFSETR_EVEN_BITPOS)
#define HW_RSOFFSETR_ODD_SETMASK (0xFFF<<HW_RSOFFSETR_ODD_BITPOS)

#define HW_RSOFFSETR_EVEN_CLRMASK (~(WORD)HW_RSOFFSETR_EVEN_SETMASK)
#define HW_RSOFFSETR_ODD_CLRMASK (~(WORD)HW_RSOFFSETR_ODD_SETMASK)

typedef union{ 
    struct { 
		unsigned EVEN		:12;	// Location of even error
		unsigned ODD		:12;	// Location of odd error
		} B;
    unsigned int I;
} rs_offsetr_type;
#define HW_RSOFFSETR	(*(volatile RS_offsetr_type _X*) (HW_RS_BASEADDR+1))	/* RS Error Offset Register */



///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Word Count Register (HW_RSWRDCNTR) Bit Definitions

#define HW_RSWRDCNTR_WORDCNT_BITPOS (0)
#define HW_RSWRDCNTR_BLOCKCNT_BITPOS (12)

#define HW_RSWRDCNTR_WORDCNT_SETMASK (0xFFF<<HW_RSWRDCNTR_WORDCNT_BITPOS)
#define HW_RSWRDCNTR_BLOCKCNT_SETMASK (0xFFF<<HW_RSWRDCNTR_BLOCKCNT_BITPOS)

#define HW_RSWRDCNTR_WORDCNT_CLRMASK (~(WORD)HW_RSWRDCNTR_WORDCNT_SETMASK)
#define HW_RSWRDCNTR_BLOCKCNT_CLRMASK (~(WORD)HW_RSWRDCNTR_BLOCKCNT_SETMASK)

typedef union{ 
    struct { 
		unsigned WORDCNT	:12;	// Number of bytes per codeword
		unsigned BLOCKCNT	:12;	// Number of codewords to process
		} B;
   unsigned int I;
} rs_wrdcntr_type;
#define HW_RSWRDCNTR 	(*(volatile RS_wrdcntr_type _X*) (HW_RS_BASEADDR+2)) 	/* RS Word Count Register */




///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Current Position Register (HW_RSCPR) Bit Definitions

#define HW_RSCPR_POS_BITPOS (0)

#define HW_RSCPR_POS_SETMASK (0xFFFF<<HW_RSCPR_POS_BITPOS)

#define HW_RSCPR_POS_CLRMASK (~(WORD)HW_RSCPR_POS_SETMASK)

typedef union{ 
    struct { 
		unsigned POS		:16;	// Current offset within the DMA buffer
		unsigned 			:8;		// Reserved
		} B;
    unsigned int I;
} rs_cpr_type;
#define HW_RSCPR 		(*(volatile RS_cpr_type _X*) (HW_RS_BASEADDR+3))		/* RS Current Position Register */




///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Modulo Register (HW_RSMODR) Bit Definitions

#define HW_RSMODR_MOD_BITPOS (0)

#define HW_RSMODR_MOD_SETMASK (0xFFFF<<HW_RSMODR_MOD_BITPOS)

#define HW_RSMODR_MOD_CLRMASK (~(WORD)HW_RSMODR_MOD_SETMASK)

typedef union{ 
    struct { 
		unsigned MOD		:16;	// Modulo offset
		unsigned 			:8;		// Reserved
		} B;
    unsigned int I;
} rs_modr_type;
#define HW_RSMODR 		(*(volatile RS_modr_type _X*) (HW_RS_BASEADDR+4))		/* RS Modulo Register */



///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Base Addr Register (HW_RSBAR) Bit Definitions

#define HW_RSBAR_ADDR_BITPOS (0)

#define HW_RSBAR_ADDR_SETMASK (0xFFFF<<HW_RSBAR_ADDR_BITPOS)

#define HW_RSBAR_ADDR_CLRMASK (~(WORD)HW_RSBAR_ADDR_SETMASK)

typedef union{ 
    struct { 
		unsigned ADDR		:16;	// Base Address of non-parity portion of codeword
		unsigned 			:8;		// Reserved
		} B;
    unsigned int I;
} rs_bar_type;
#define HW_RSBAR 		(*(volatile RS_bar_type _X*) (HW_RS_BASEADDR+5))		/* RS Base Address Register */

///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Parity Base Addr Register (HW_RSPBAR) Bit Definitions

#define HW_RSPBAR_ADDR_BITPOS (0)

#define HW_RSPBAR_ADDR_SETMASK (0xFFFF<<HW_RSPBAR_ADDR_BITPOS)

#define HW_RSPBAR_ADDR_CLRMASK (~(WORD)HW_RSPBAR_ADDR_SETMASK)

typedef union{ 
    struct { 
		unsigned ADDR		:16;	// Parity offset
		unsigned 			:8;		// Reserved
		} B;
    unsigned int I;
} rs_pbar_type;
#define HW_RSPBAR 		(*(volatile RS_pbar_type _X*) (HW_RS_BASEADDR+6))		/* RS Parity Base Address Register */


///////////////////////////////////////////////////////////////////////////////////
////  Reed Solomon Span Register (HW_RSSPANR) Bit Definitions

#define HW_RSSPANR_WORDSPAN_BITPOS (0)
#define HW_RSSPANR_PARITYSPAN_BITPOS (8)
#define HW_RSSPANR_BLOCKSPAN_BITPOS (16)

#define HW_RSSPANR_WORDSPAN_SETMASK (0xFF<<HW_RSSPANR_WORDSPAN_BITPOS)
#define HW_RSSPANR_PARITYSPAN_SETMASK (0xFF<<HW_RSSPANR_PARITYSPAN_BITPOS)
#define HW_RSSPANR_BLOCKSPAN_SETMASK (0xFF<<HW_RSSPANR_BLOCKSPAN_BITPOS)

#define HW_RSSPANR_WORDSPAN_CLRMASK (~(WORD)HW_RSSPANR_WORDSPAN_SETMASK)
#define HW_RSSPANR_PARITYSPAN_CLRMASK (~(WORD)HW_RSSPANR_PARITYSPAN_SETMASK)
#define HW_RSSPANR_BLOCKSPAN_CLRMASK (~(WORD)HW_RSSPANR_BLOCKSPAN_SETMASK)

typedef union{ 
    struct { 
		unsigned WORDSPAN  	:8;	// Word Address Increment
		unsigned PARITYSPAN	:8;	// Parity Address Increment
		unsigned BLOCKSPAN  :8;	// Block Address Increment
		} B;
    unsigned int I;
} rs_spanr_type;
#define HW_RSSPANR 		(*(volatile RS_spanr_type _X*) (HW_RS_BASEADDR+7))		/* RS Span Register */





///////////////////////////////////////////////////////////////////////////////////
////  CD-DSP Interface (CDI) Registers
///////////////////////////////////////////////////////////////////////////////////

#define HW_CDI_BASEADDR 0xF280

///////////////////////////////////////////////////////////////////////////////////
////  CD-DSP Interface Control Unit Control Status Register (HW_CDI_CTLCSR) Bit Definitions

#define HW_CDICTRLCSR_KICK_BITPOS (0)
#define HW_CDICTRLCSR_CLKPOL_BITPOS (1)
#define HW_CDICTRLCSR_CLKRZ_BITPOS (2)
#define HW_CDICTRLCSR_LSBFIRST_BITPOS (3)
#define HW_CDICTRLCSR_LENGTH_BITPOS (4)
#define HW_CDICTRLCSR_MSBJUSTIFY_BITPOS (9)
#define HW_CDICTRLCSR_LATCHEN_BITPOS (10)
#define HW_CDICTRLCSR_LATCHPOL_BITPOS (11)
#define HW_CDICTRLCSR_RSVD_BITPOS (12)
#define HW_CDICTRLCSR_IRQEN_BITPOS (20)
#define HW_CDICTRLCSR_TFRDONE_BITPOS (21)
#define HW_CDICTRLCSR_RESET_BITPOS (23)

#define HW_CDICTRLCSR_KICK_SETMASK (1<<HW_CDICTRLCSR_KICK_BITPOS)
#define HW_CDICTRLCSR_CLKPOL_SETMASK (1<<HW_CDICTRLCSR_CLKPOL_BITPOS)
#define HW_CDICTRLCSR_CLKRZ_SETMASK (1<<HW_CDICTRLCSR_CLKRZ_BITPOS)
#define HW_CDICTRLCSR_LSBFIRST_SETMASK (1<<HW_CDICTRLCSR_LSBFIRST_BITPOS)
#define HW_CDICTRLCSR_LENGTH_SETMASK (0x1F<<HW_CDICTRLCSR_LENGTH_BITPOS)
#define HW_CDICTRLCSR_MSBJUSTIFY_SETMASK (1<<HW_CDICTRLCSR_MSBJUSTIFY_BITPOS)
#define HW_CDICTRLCSR_LATCHEN_SETMASK (1<<HW_CDICTRLCSR_LATCHEN_BITPOS)
#define HW_CDICTRLCSR_LATCHPOL_SETMASK (1<<HW_CDICTRLCSR_LATCHPOL_BITPOS)
#define HW_CDICTRLCSR_IRQEN_SETMASK (1<<HW_CDICTRLCSR_IRQEN_BITPOS)
#define HW_CDICTRLCSR_TFRDONE_SETMASK (1<<HW_CDICTRLCSR_TFRDONE_BITPOS)
#define HW_CDICTRLCSR_RESET_SETMASK (1<<HW_CDICTRLCSR_RESET_BITPOS)

#define HW_CDICTRLCSR_KICK_CLRMASK (~(WORD)HW_CDICTRLCSR_KICK_SETMASK)
#define HW_CDICTRLCSR_CLKPOL_CLRMASK (~(WORD)HW_CDICTRLCSR_CLKPOL_SETMASK)
#define HW_CDICTRLCSR_CLKRZ_CLRMASK (~(WORD)HW_CDICTRLCSR_CLKRZ_SETMASK)
#define HW_CDICTRLCSR_LSBFIRST_CLRMASK (~(WORD)HW_CDICTRLCSR_LSBFIRST_SETMASK)
#define HW_CDICTRLCSR_LENGTH_CLRMASK (~(WORD)HW_CDICTRLCSR_LENGTH_SETMASK)
#define HW_CDICTRLCSR_MSBJUSTIFY_CLRMASK (~(WORD)HW_CDICTRLCSR_MSBJUSTIFY_SETMASK)
#define HW_CDICTRLCSR_LATCHEN_CLRMASK (~(WORD)HW_CDICTRLCSR_LATCHEN_SETMASK)
#define HW_CDICTRLCSR_LATCHPOL_CLRMASK (~(WORD)HW_CDICTRLCSR_LATCHPOL_SETMASK)
#define HW_CDICTRLCSR_IRQEN_CLRMASK (~(WORD)HW_CDICTRLCSR_IRQEN_SETMASK)
#define HW_CDICTRLCSR_TFRDONE_CLRMASK (~(WORD)HW_CDICTRLCSR_TFRDONE_SETMASK)
#define HW_CDICTRLCSR_RESET_CLRMASK (~(WORD)HW_CDICTRLCSR_RESET_SETMASK)


typedef union               /* CD-DSP Interface  Control Unit Control/Status Register  */
{
    struct {
		int KICK		:1;	/* Start Transaction */
		int CLKPOL		:1;	/* Control Clock Polarity */
		int CLKRZ		:1; /* Control Clock RZ */
		int LSBFIRST	:1;	/* Send Data LSB First */
		unsigned LENGTH :5; /* Number of Bits to Transfer */
		int MSBJUSTIFY	:1;	/* Selects if valid data is Right or Left Justified */
		int LATCHEN		:1;	/* Enable Latch at end of transfer */
		int LATCHPOL	:1; /* Selects Latch Polarity */
		int 			:8;	/* Reserved */
		int IRQEN		:1;	/* Enable the Done IRQ */
		int TFRDONE		:1;	/* Transfer Complete Flag */
		int				:1;	/* Reserved */
		int RESET		:1;	/* Reset and Low Power */
		} B;
    unsigned int I;
} cdi_ctrlcsr_type;
#define HW_CDICTRLCSR 	(*(volatile cdi_ctrlcsr_type _X*) (HW_CDI_BASEADDR))		/* CDI Control Port Control/Status Register */

///////////////////////////////////////////////////////////////////////////////////
////  CDI Time Base Register bitfields

//// CDI Control Port Time Base Register Bit Defs
#define HW_CDICTRLTBR_DIVIDE_BITPOS (0)

#define HW_CDICTRLTBR_DIVIDE_SETMASK (0x3FF<<HW_CDICTRLTBR_DIVIDE_BITPOS)

#define HW_CDICTRLTBR_DIVIDE_CLRMASK (~(WORD)HW_CDICTRLTBR_DIVIDE_SETMASK)


//// CDI Serial Input Unit 0 Time Base Register Bit Defs
#define HW_CDISI0TBR_DIVIDE_BITPOS (0)

#define HW_CDISI0TBR_DIVIDE_SETMASK (0x3FF<<HW_CDISI0TBR_DIVIDE_BITPOS)

#define HW_CDISI0TBR_DIVIDE_CLRMASK (~(WORD)HW_CDISI0TBR_DIVIDE_SETMASK)

//// CDI Serial Input Unit 1 Time Base Register Bit Defs
#define HW_CDISI1TBR_DIVIDE_BITPOS (0)

#define HW_CDISI1TBR_DIVIDE_SETMASK (0x3FF<<HW_CDISI1TBR_DIVIDE_BITPOS)

#define HW_CDISI1TBR_DIVIDE_CLRMASK (~(WORD)HW_CDISI1TBR_DIVIDE_SETMASK)

typedef union
{
    struct { 
		unsigned DIVIDE	:10;	// Clock Divide Value for generating the bit Clock
		int				:14;	// Reserved
		} B;
    unsigned int I;
} cdi_tbr_type;
#define HW_CDICTRLTBR 	(*(volatile cdi_tbr_type _X*) (HW_CDI_BASEADDR+2))	  	/* CDI Control Port Time Base Register  */
#define HW_CDISI0TBR 	(*(volatile cdi_tbr_type _X*) (HW_CDI_BASEADDR+10))		/* CDI Serial Input Unit 0 Time Base Register  */
#define HW_CDISI1TBR 	(*(volatile cdi_tbr_type _X*) (HW_CDI_BASEADDR+14))		/* CDI Serial Input Unit 1 Time Base Register  */


///////////////////////////////////////////////////////////////////////////////////
////  CDI Control Port Timer Register


#define HW_CDICTRLTMRR_LATCHSTART_BITPOS (0)
#define HW_CDICTRLTMRR_LATCHWIDTH_BITPOS (8)
#define HW_CDICTRLTMRR_FINALDELAY_BITPOS (16)

#define HW_CDICTRLTMRR_LATCHSTART_SETMASK (0xFF<<HW_CDICTRLTMRR_LATCHSTART_BITPOS)
#define HW_CDICTRLTMRR_LATCHWIDTH_SETMASK (0xFF<<HW_CDICTRLTMRR_LATCHWIDTH_BITPOS)
#define HW_CDICTRLTMRR_FINALDELAY_SETMASK (0xFF<<HW_CDICTRLTMRR_FINALDELAY_BITPOS)

#define HW_CDICTRLTMRR_LATCHSTART_CLRMASK (~(WORD)HW_CDICTRLTMRR_LATCHSTART_SETMASK)
#define HW_CDICTRLTMRR_LATCHWIDTH_CLRMASK (~(WORD)HW_CDICTRLTMRR_LATCHWIDTH_SETMASK)
#define HW_CDICTRLTMRR_FINALDELAY_CLRMASK (~(WORD)HW_CDICTRLTMRR_FINALDELAY_SETMASK)

typedef union      
{
    struct { 
		unsigned LATCHSTART	:8;	// Clock Divide Value for generating the bit Clock
		unsigned LATCHWIDTH	:8;	// Number of bit Clocks to hold the latch asserted
		unsigned FINALDELAY :8;	// Number of bit clocks to wait before generating a done IRQ
		} B;
    unsigned int I;
} cdi_ctrltmrr_type;
#define HW_CDICTRLTMRR 	(*(volatile cdi_ctrltmrr_type _X*) (HW_CDI_BASEADDR+1))	/* CDI Control Port Timer Register */


///////////////////////////////////////////////////////////////////////////////////
////// Bit Defs for CDI Serial Input Units 0 and 1

#define HW_CDISI0TMRR_LATCHSTART_BITPOS (0)
#define HW_CDISI0TMRR_FINALDELAY_BITPOS (16)

#define HW_CDISI0TMRR_LATCHSTART_SETMASK (0xFF<<HW_CDISI0TMRR_LATCHSTART_BITPOS)
#define HW_CDISI0TMRR_FINALDELAY_SETMASK (0xFF<<HW_CDISI0TMRR_FINALDELAY_BITPOS)

#define HW_CDISI0TMRR_LATCHSTART_CLRMASK (~(WORD)HW_CDISI0TMRR_LATCHSTART_SETMASK)
#define HW_CDISI0TMRR_FINALDELAY_CLRMASK (~(WORD)HW_CDISI0TMRR_FINALDELAY_SETMASK)


#define HW_CDISI1TMRR_LATCHSTART_BITPOS (0)
#define HW_CDISI1TMRR_FINALDELAY_BITPOS (16)

#define HW_CDISI1TMRR_LATCHSTART_SETMASK (0xFF<<HW_CDISI1TMRR_LATCHSTART_BITPOS)
#define HW_CDISI1TMRR_FINALDELAY_SETMASK (0xFF<<HW_CDISI1TMRR_FINALDELAY_BITPOS)

#define HW_CDISI1TMRR_LATCHSTART_CLRMASK (~(WORD)HW_CDISI1TMRR_LATCHSTART_SETMASK)
#define HW_CDISI1TMRR_FINALDELAY_CLRMASK (~(WORD)HW_CDISI1TMRR_FINALDELAY_SETMASK)

typedef union     // CDI Serial Input Unit Timer Register
{
    struct { 
		unsigned LATCHSTART	:8;	// Clock Divide Value for generating the bit Clock
		unsigned 			:8;	// Reserved
		unsigned FINALDELAY :8;	// Number of bit clocks to wait before generating a done IRQ
		} B;
    unsigned int I;
} cdi_sitmrr_type;
#define HW_CDISI0TMRR 	(*(volatile cdi_sitmrr_type _X*) (HW_CDI_BASEADDR+9)) 	/* CDI Serial Input Unit 0 Timer Register */
#define HW_CDISI1TMRR 	(*(volatile cdi_sitmrr_type _X*) (HW_CDI_BASEADDR+13))	/* CDI Serial Input Unit 1 Timer Register */


///////////////////////////////////////////////////////////////////////////////////
////  CDI Data Registers (Control, Serial in 0,1)


#define HW_CDICTRLDATAR_DATA_BITPOS (0)

#define HW_CDICTRLDATAR_DATA_SETMASK (0xFFFFFF<<HW_CDICTRLDATAR_DATA_BITPOS)

#define HW_CDICTRLDATAR_DATA_CLRMASK (~(WORD)HW_CDICTRLDATAR_DATA_SETMASK)


//// Bit Defs for CDI Serial Input Unit 0
#define HW_CDISI0DATAR_DATA_BITPOS (0)

#define HW_CDISI0DATAR_DATA_SETMASK (0xFFFFFF<<HW_CDISI0DATAR_DATA_BITPOS)

#define HW_CDISI0DATAR_DATA_CLRMASK (~(WORD)HW_CDISI0DATAR_DATA_SETMASK)


//// Bit Defs for CDI Serial Input Unit 1
#define HW_CDISI1DATAR_DATA_BITPOS (0)

#define HW_CDISI1DATAR_DATA_SETMASK (0xFFFFFF<<HW_CDISI1DATAR_DATA_BITPOS)

#define HW_CDISI1DATAR_DATA_CLRMASK (~(WORD)HW_CDISI1DATAR_DATA_SETMASK)

typedef union     
{
    struct { 
		unsigned DATA	:24;	// Data for Input or Output
		} B;
    unsigned int I;
} cdi_datar_type;
#define HW_CDICTRLDATAR (*(volatile cdi_datar_type _X*) (HW_CDI_BASEADDR+3))	/* CDI Control Unit Data Register  */
#define HW_CDISI0DATAR 	(*(volatile cdi_datar_type _X*) (HW_CDI_BASEADDR+11))	/* CDI Serial Input Unit 0 Data Register */		 
#define HW_CDISI1DATAR 	(*(volatile cdi_datar_type _X*) (HW_CDI_BASEADDR+15))	/* CDI Serial Input Unit 1 Data Register */


///////////////////////////////////////////////////////////////////////////////////
//// CDI Pin Configuration Register Bit defs

#define HW_CDIPINCFGR_ENABLE_BITPOS (0)
#define HW_CDIPINCFGR_CTL_BITPOS (1)
#define HW_CDIPINCFGR_SCLK_BITPOS (2)
#define HW_CDIPINCFGR_CTLSO_BITPOS (4)
#define HW_CDIPINCFGR_SYNCA_BITPOS (5)
#define HW_CDIPINCFGR_SENSE_BITPOS (6)
#define HW_CDIPINCFGR_CTRLSCLK_BITPOS (7)
#define HW_CDIPINCFGR_CCLK_BITPOS (8)
#define HW_CDIPINCFGR_CTLOUTEN_BITPOS (10)
#define HW_CDIPINCFGR_SCLKOUTEN_BITPOS (11)

#define HW_CDIPINCFGR_ENABLE_SETMASK (1<<HW_CDIPINCFGR_ENABLE_BITPOS)
#define HW_CDIPINCFGR_CTL_SETMASK (1<<HW_CDIPINCFGR_CTL_BITPOS)
#define HW_CDIPINCFGR_SCLK_SETMASK (3<<HW_CDIPINCFGR_SCLK_BITPOS)
#define HW_CDIPINCFGR_CTLSO_SETMASK (1<<HW_CDIPINCFGR_CTLSO_BITPOS)
#define HW_CDIPINCFGR_SYNCA_SETMASK (1<<HW_CDIPINCFGR_SYNCA_BITPOS)
#define HW_CDIPINCFGR_SENSE_SETMASK (1<<HW_CDIPINCFGR_SENSE_BITPOS)
#define HW_CDIPINCFGR_CTRLSCLK_SETMASK (1<<HW_CDIPINCFGR_CTRLSCLK_BITPOS)
#define HW_CDIPINCFGR_CCLK_SETMASK (3<<HW_CDIPINCFGR_CCLK_BITPOS)
#define HW_CDIPINCFGR_CTLOUTEN_SETMASK (1<<HW_CDIPINCFGR_CTLOUTEN_BITPOS)
#define HW_CDIPINCFGR_SCLKOUTEN_SETMASK (1<<HW_CDIPINCFGR_SCLKOUTEN_BITPOS)

#define HW_CDIPINCFGR_ENABLE_CLRMASK (~(WORD)HW_CDIPINCFGR_ENABLE_SETMASK)
#define HW_CDIPINCFGR_CTL_CLRMASK (~(WORD)HW_CDIPINCFGR_CTL_SETMASK)
#define HW_CDIPINCFGR_SCLK_CLRMASK (~(WORD)HW_CDIPINCFGR_SCLK_SETMASK)
#define HW_CDIPINCFGR_CTLSO_CLRMASK (~(WORD)HW_CDIPINCFGR_CTLSO_SETMASK)
#define HW_CDIPINCFGR_SYNCA_CLRMASK (~(WORD)HW_CDIPINCFGR_SYNCA_SETMASK)
#define HW_CDIPINCFGR_SENSE_CLRMASK (~(WORD)HW_CDIPINCFGR_SENSE_SETMASK)
#define HW_CDIPINCFGR_CTRLSCLK_CLRMASK (~(WORD)HW_CDIPINCFGR_CTRLSCLK_SETMASK)
#define HW_CDIPINCFGR_CCLK_CLRMASK (~(WORD)HW_CDIPINCFGR_CCLK_SETMASK)
#define HW_CDIPINCFGR_CTLOUTEN_CLRMASK (~(WORD)HW_CDIPINCFGR_CTLOUTEN_SETMASK)
#define HW_CDIPINCFGR_SCLKOUTEN_CLRMASK (~(WORD)HW_CDIPINCFGR_SCLKOUTEN_SETMASK)

typedef union     // CDI Pin Configuration Register
{
    struct { 
		int ENABLE		:1;	// CDI Pin Enable
		int	CTL			:1;	// CDI Control Unit Pin Select
		unsigned SCLK	:2;	// Pin select for CTL_SCLK
		int CTLSO		:1;	// CTL SO Pin Select
		int SYNCA		:1;	// SI SYNCa pin select
		int SENSE		:1;	// CTRL SENSE Pin select
		int CTRLSCLK	:1;	// CTRL SCLK source select
		unsigned CCLK  	:2;	// Control Clock Select
		int CTLOUTEN 	:1;	// Enable output drivers on Control
		int SCLKOUTEN	:1;	// Enable output drivers on SCLK
		int				:12; // Reserved	   
		} B;
    unsigned int I;
} cdi_pincfgr_type;
#define HW_CDIPINCFGR 	(*(volatile cdi_pincfgr_type _X*) (HW_CDI_BASEADDR+4))	/* CDI Pin Configuration Register  */

///////////////////////////////////////////////////////////////////////////////////
////  CD-DSP Interface Serial Input Unit Control Status Register (HW_CDI_SIxCSR) Bit Definitions

//// Serial Input Unit 0 CSR Bit Defs
#define HW_CDISI0CSR_KICK_BITPOS (0)
#define HW_CDISI0CSR_CLKPOL_BITPOS (1)
#define HW_CDISI0CSR_CLKRZ_BITPOS (2)
#define HW_CDISI0CSR_LSBFIRST_BITPOS (3)
#define HW_CDISI0CSR_LENGTH_BITPOS (4)
#define HW_CDISI0CSR_MSBJUSTIFY_BITPOS (9)
#define HW_CDISI0CSR_SCLKRZ_BITPOS (10)
#define HW_CDISI0CSR_SYNCEN_BITPOS (11)
#define HW_CDISI0CSR_SYNCPOL_BITPOS (12)
#define HW_CDISI0CSR_CRCEN_BITPOS (13)
#define HW_CDISI0CSR_CRCPOL_BITPOS (14)
#define HW_CDISI0CSR_CLKSEL_BITPOS (15)
#define HW_CDISI0CSR_DATASEL_BITPOS (16)
#define HW_CDISI0CSR_SYNCSEL_BITPOS (17)
#define HW_CDISI0CSR_IRQEN_BITPOS (20)
#define HW_CDISI0CSR_DONE_BITPOS (21)
#define HW_CDISI0CSR_RESET_BITPOS (23)

#define HW_CDISI0CSR_KICK_SETMASK (1<<HW_CDISI0CSR_KICK_BITPOS)
#define HW_CDISI0CSR_CLKPOL_SETMASK (1<<HW_CDISI0CSR_CLKPOL_BITPOS)
#define HW_CDISI0CSR_CLKRZ_SETMASK (1<<HW_CDISI0CSR_CLKRZ_BITPOS)
#define HW_CDISI0CSR_LSBFIRST_SETMASK (1<<HW_CDISI0CSR_LSBFIRST_BITPOS)
#define HW_CDISI0CSR_LENGTH_SETMASK (0x1F<<HW_CDISI0CSR_LENGTH_BITPOS)
#define HW_CDISI0CSR_MSBJUSTIFY_SETMASK (1<<HW_CDISI0CSR_MSBJUSTIFY_BITPOS)
#define HW_CDISI0CSR_SCLKRZ_SETMASK (1<<HW_CDISI0CSR_SCLKRZ_BITPOS)
#define HW_CDISI0CSR_SYNCEN_SETMASK (1<<HW_CDISI0CSR_SYNCEN_BITPOS)
#define HW_CDISI0CSR_SYNCPOL_SETMASK (1<<HW_CDISI0CSR_SYNCPOL_BITPOS)
#define HW_CDISI0CSR_CRCEN_SETMASK (1<<HW_CDISI0CSR_CRCEN_BITPOS)
#define HW_CDISI0CSR_CRCPOL_SETMASK (1<<HW_CDISI0CSR_CRCPOL_BITPOS)
#define HW_CDISI0CSR_CLKSEL_SETMASK (1<<HW_CDISI0CSR_CLKSEL_BITPOS)
#define HW_CDISI0CSR_DATASEL_SETMASK (1<<HW_CDISI0CSR_DATASEL_BITPOS)
#define HW_CDISI0CSR_SYNCSEL_SETMASK (3<<HW_CDISI0CSR_SYNCSEL_BITPOS)
#define HW_CDISI0CSR_IRQEN_SETMASK (1<<HW_CDISI0CSR_IRQEN_BITPOS)
#define HW_CDISI0CSR_DONE_SETMASK (1<<HW_CDISI0CSR_DONE_BITPOS)
#define HW_CDISI0CSR_RESET_SETMASK (1<<HW_CDISI0CSR_RESET_BITPOS)										
#define HW_CDISI0CSR_KICK_CLRMASK (~(WORD)HW_CDISI0CSR_KICK_SETMASK)
#define HW_CDISI0CSR_CLKPOL_CLRMASK (~(WORD)HW_CDISI0CSR_CLKPOL_SETMASK)
#define HW_CDISI0CSR_CLKRZ_CLRMASK (~(WORD)HW_CDISI0CSR_CLKRZ_SETMASK)
#define HW_CDISI0CSR_LSBFIRST_CLRMASK (~(WORD)HW_CDISI0CSR_LSBFIRST_SETMASK)
#define HW_CDISI0CSR_LENGTH_CLRMASK (~(WORD)HW_CDISI0CSR_LENGTH_SETMASK)
#define HW_CDISI0CSR_MSBJUSTIFY_CLRMASK (~(WORD)HW_CDISI0CSR_MSBJUSTIFY_SETMASK)
#define HW_CDISI0CSR_SCLKRZ_CLRMASK (~(WORD)HW_CDISI0CSR_SCLKRZ_SETMASK)
#define HW_CDISI0CSR_SYNCEN_CLRMASK (~(WORD)HW_CDISI0CSR_SYNCEN_SETMASK)
#define HW_CDISI0CSR_SYNCPOL_CLRMASK (~(WORD)HW_CDISI0CSR_SYNCPOL_SETMASK)
#define HW_CDISI0CSR_CRCEN_CLRMASK (~(WORD)HW_CDISI0CSR_CRCEN_SETMASK)
#define HW_CDISI0CSR_CRCPOL_CLRMASK (~(WORD)HW_CDISI0CSR_CRCPOL_SETMASK)
#define HW_CDISI0CSR_CLKSEL_CLRMASK (~(WORD)HW_CDISI0CSR_CLKSEL_SETMASK)
#define HW_CDISI0CSR_DATASEL_CLRMASK (~(WORD)HW_CDISI0CSR_DATASEL_SETMASK)
#define HW_CDISI0CSR_SYNCSEL_CLRMASK (~(WORD)HW_CDISI0CSR_SYNCSEL_SETMASK)
#define HW_CDISI0CSR_IRQEN_CLRMASK (~(WORD)HW_CDISI0CSR_IRQEN_SETMASK)
#define HW_CDISI0CSR_DONE_CLRMASK (~(WORD)HW_CDISI0CSR_DONE_SETMASK)
#define HW_CDISI0CSR_RESET_CLRMASK (~(WORD)HW_CDISI0CSR_RESET_SETMASK)

//// Serial Input Unit 1 CSR Bit Defs
#define HW_CDISI1CSR_KICK_BITPOS (0)
#define HW_CDISI1CSR_CLKPOL_BITPOS (1)
#define HW_CDISI1CSR_CLKRZ_BITPOS (2)
#define HW_CDISI1CSR_LSBFIRST_BITPOS (3)
#define HW_CDISI1CSR_LENGTH_BITPOS (4)
#define HW_CDISI1CSR_MSBJUSTIFY_BITPOS (9)
#define HW_CDISI1CSR_SCLKRZ_BITPOS (10)
#define HW_CDISI1CSR_SYNCEN_BITPOS (11)
#define HW_CDISI1CSR_SYNCPOL_BITPOS (12)
#define HW_CDISI1CSR_CRCEN_BITPOS (13)
#define HW_CDISI1CSR_CRCPOL_BITPOS (14)
#define HW_CDISI1CSR_CLKSEL_BITPOS (15)
#define HW_CDISI1CSR_DATASEL_BITPOS (16)
#define HW_CDISI1CSR_SYNCSEL_BITPOS (17)
#define HW_CDISI1CSR_IRQEN_BITPOS (20)
#define HW_CDISI1CSR_DONE_BITPOS (21)
#define HW_CDISI1CSR_RESET_BITPOS (23)

#define HW_CDISI1CSR_KICK_SETMASK (1<<HW_CDISI1CSR_KICK_BITPOS)
#define HW_CDISI1CSR_CLKPOL_SETMASK (1<<HW_CDISI1CSR_CLKPOL_BITPOS)
#define HW_CDISI1CSR_CLKRZ_SETMASK (1<<HW_CDISI1CSR_CLKRZ_BITPOS)
#define HW_CDISI1CSR_LSBFIRST_SETMASK (1<<HW_CDISI1CSR_LSBFIRST_BITPOS)
#define HW_CDISI1CSR_LENGTH_SETMASK (0x1F<<HW_CDISI1CSR_LENGTH_BITPOS)
#define HW_CDISI1CSR_MSBJUSTIFY_SETMASK (1<<HW_CDISI1CSR_MSBJUSTIFY_BITPOS)
#define HW_CDISI1CSR_SCLKRZ_SETMASK (1<<HW_CDISI1CSR_SCLKRZ_BITPOS)
#define HW_CDISI1CSR_SYNCEN_SETMASK (1<<HW_CDISI1CSR_SYNCEN_BITPOS)
#define HW_CDISI1CSR_SYNCPOL_SETMASK (1<<HW_CDISI1CSR_SYNCPOL_BITPOS)
#define HW_CDISI1CSR_CRCEN_SETMASK (1<<HW_CDISI1CSR_CRCEN_BITPOS)
#define HW_CDISI1CSR_CRCPOL_SETMASK (1<<HW_CDISI1CSR_CRCPOL_BITPOS)
#define HW_CDISI1CSR_CLKSEL_SETMASK (1<<HW_CDISI1CSR_CLKSEL_BITPOS)
#define HW_CDISI1CSR_DATASEL_SETMASK (1<<HW_CDISI1CSR_DATASEL_BITPOS)
#define HW_CDISI1CSR_SYNCSEL_SETMASK (3<<HW_CDISI1CSR_SYNCSEL_BITPOS)
#define HW_CDISI1CSR_IRQEN_SETMASK (1<<HW_CDISI1CSR_IRQEN_BITPOS)
#define HW_CDISI1CSR_DONE_SETMASK (1<<HW_CDISI1CSR_DONE_BITPOS)
#define HW_CDISI1CSR_RESET_SETMASK (1<<HW_CDISI1CSR_RESET_BITPOS)

#define HW_CDISI1CSR_KICK_CLRMASK (~(WORD)HW_CDISI1CSR_KICK_SETMASK)
#define HW_CDISI1CSR_CLKPOL_CLRMASK (~(WORD)HW_CDISI1CSR_CLKPOL_SETMASK)
#define HW_CDISI1CSR_CLKRZ_CLRMASK (~(WORD)HW_CDISI1CSR_CLKRZ_SETMASK)
#define HW_CDISI1CSR_LSBFIRST_CLRMASK (~(WORD)HW_CDISI1CSR_LSBFIRST_SETMASK)
#define HW_CDISI1CSR_LENGTH_CLRMASK (~(WORD)HW_CDISI1CSR_LENGTH_SETMASK)
#define HW_CDISI1CSR_MSBJUSTIFY_CLRMASK (~(WORD)HW_CDISI1CSR_MSBJUSTIFY_SETMASK)
#define HW_CDISI1CSR_SCLKRZ_CLRMASK (~(WORD)HW_CDISI1CSR_SCLKRZ_SETMASK)
#define HW_CDISI1CSR_SYNCEN_CLRMASK (~(WORD)HW_CDISI1CSR_SYNCEN_SETMASK)
#define HW_CDISI1CSR_SYNCPOL_CLRMASK (~(WORD)HW_CDISI1CSR_SYNCPOL_SETMASK)
#define HW_CDISI1CSR_CRCEN_CLRMASK (~(WORD)HW_CDISI1CSR_CRCEN_SETMASK)
#define HW_CDISI1CSR_CRCPOL_CLRMASK (~(WORD)HW_CDISI1CSR_CRCPOL_SETMASK)
#define HW_CDISI1CSR_CLKSEL_CLRMASK (~(WORD)HW_CDISI1CSR_CLKSEL_SETMASK)
#define HW_CDISI1CSR_DATASEL_CLRMASK (~(WORD)HW_CDISI1CSR_DATASEL_SETMASK)
#define HW_CDISI1CSR_SYNCSEL_CLRMASK (~(WORD)HW_CDISI1CSR_SYNCSEL_SETMASK)
#define HW_CDISI1CSR_IRQEN_CLRMASK (~(WORD)HW_CDISI1CSR_IRQEN_SETMASK)
#define HW_CDISI1CSR_DONE_CLRMASK (~(WORD)HW_CDISI1CSR_DONE_SETMASK)
#define HW_CDISI1CSR_RESET_CLRMASK (~(WORD)HW_CDISI1CSR_RESET_SETMASK)

typedef union       
{
    struct {
		int KICK		:1;	/* Start Transaction */
		int CLKPOL		:1;	/* Clock Polarity */
		int CLKRZ		:1; /* Clock Return to Zero select */
		int LSBFIRST	:1;	/* Send Data LSB First */
		unsigned LENGTH :5; /* Nubber of Bits to Transfer */
		int MSBJUSTIFY	:1;	/* Selects if valid data is Right or Left Justified */
		int SCLKRZ		:1;	/* SClock Return to Zero select */
		int SYNCEN		:1; /* Sync Enable */
		int SYNCPOL		:1;	/* Sync Polarity */
		int CRCEN		:1;	/* Enable the CRC */
		int CRCPOL		:1;	/* CRC Polarity */
		int CLKSEL		:1;	/* Clock Select */
		int DATASEL		:1;	/* Data input selector */
		unsigned SYNCSEL :2;/* Sync Signal Selector */
		int				:1;	/* Reserved */
		int IRQEN		:1;	/* Interrupt Enable */
		int DONE		:1; /* Transfer Done Flag */
		int				:1;	/* Reserved */
		int RESET		:1;	/* Reset and Low Power */
		} B;
    unsigned int I;
} cdi_sicsr_type;
#define HW_CDISI0CSR 	(*(volatile cdi_sicsr_type _X*) (HW_CDI_BASEADDR+8))  	/* CDI Serial Input Unit 0 Control/Status Register  */
#define HW_CDISI1CSR 	(*(volatile cdi_sicsr_type _X*) (HW_CDI_BASEADDR+12)) 	/* CDI Serial Input Unit 1 Control/Status Register  */
#endif

