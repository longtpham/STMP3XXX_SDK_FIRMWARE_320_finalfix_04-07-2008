;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Copyright(C) SigmaTel, Inc. 2002-2003
;;; Filename    : regsspi.inc
;;; Description : Register definitions for SPI interface
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
    if !(@def(regsspiinc))
regsspiinc            equ         1

;C insert
;#include "types.h"
;endc

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   SPI STMP3500 Registers 
;   Last Updated 7.7.2003 Dave Dyches
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

HW_SPI_BASEADDR             equ     ($FFF9)


;ASM Only 

HW_SPCSR                    equ     (HW_SPI_BASEADDR)
HW_SPDR		    	        equ     (HW_SPI_BASEADDR+1)

;endasm

; /////////////////////////////////////////////
; //  SPI Control/Status Register Bit Definitions
HW_SPCSR_SPE_BITPOS         equ     (0)
HW_SPCSR_MSTR_BITPOS 	    equ     (1)
HW_SPCSR_SPIE_BITPOS    	equ     (2)
HW_SPCSR_CPOL_BITPOS        equ     (3)
HW_SPCSR_CPHA_BITPOS 	    equ     (4)
HW_SPCSR_SPIF_BITPOS 	    equ     (5)
HW_SPCSR_WCOL_BITPOS        equ     (6)
HW_SPCSR_MODF_BITPOS 	    equ     (7)
HW_SPCSR_DIV_BITPOS 	    equ     (8)
HW_SPCSR_RSVD_BITPOS 	    equ     (16)

HW_SPCSR_SPE_WIDTH        	equ     (1)
HW_SPCSR_MSTR_WIDTH 	    equ     (1)
HW_SPCSR_SPIE_WIDTH    	    equ     (1)
HW_SPCSR_CPOL_WIDTH	        equ     (1)
HW_SPCSR_CPHA_WIDTH 	    equ     (1)
HW_SPCSR_SPIF_WIDTH 	    equ     (1)
HW_SPCSR_WCOL_WIDTH         equ     (1)
HW_SPCSR_MODF_WIDTH 	    equ     (1)
HW_SPCSR_DIV_WIDTH 	        equ     (8)
HW_SPCSR_RSVD_WIDTH 	    equ     (8)

HW_SPCSR_SPE_SETMASK      	equ     (((1<<HW_SPCSR_SPE_WIDTH)-1)<<HW_SPCSR_SPE_BITPOS)
HW_SPCSR_MSTR_SETMASK    	equ     (((1<<HW_SPCSR_MSTR_WIDTH)-1)<<HW_SPCSR_MSTR_BITPOS)
HW_SPCSR_SPIE_SETMASK   	equ     (((1<<HW_SPCSR_SPIE_WIDTH)-1)<<HW_SPCSR_SPIE_BITPOS)
HW_SPCSR_CPOL_SETMASK 	    equ     (((1<<HW_SPCSR_CPOL_WIDTH)-1)<<HW_SPCSR_CPOL_BITPOS)
HW_SPCSR_CPHA_SETMASK    	equ     (((1<<HW_SPCSR_CPHA_WIDTH)-1)<<HW_SPCSR_CPHA_BITPOS)
HW_SPCSR_SPIF_SETMASK 		equ     (((1<<HW_SPCSR_SPIF_WIDTH)-1)<<HW_SPCSR_SPIF_BITPOS)
HW_SPCSR_WCOL_SETMASK       equ     (((1<<HW_SPCSR_WCOL_WIDTH)-1)<<HW_SPCSR_WCOL_BITPOS)
HW_SPCSR_MODF_SETMASK    	equ     (((1<<HW_SPCSR_MODF_WIDTH)-1)<<HW_SPCSR_MODF_BITPOS)
HW_SPCSR_DIV_SETMASK      	equ     (((1<<HW_SPCSR_DIV_WIDTH)-1)<<HW_SPCSR_DIV_BITPOS)
HW_SPCSR_RSVD_SETMASK    	equ     (((1<<HW_SPCSR_RSVD_WIDTH)-1)<<HW_SPCSR_RSVD_BITPOS)

HW_SPCSR_SPE_CLRMASK	  	equ     (~HW_SPCSR_SPE_SETMASK)
HW_SPCSR_MSTR_CLRMASK   	equ     (~HW_SPCSR_MSTR_SETMASK)
HW_SPCSR_SPIE_CLRMASK     	equ     (~HW_SPCSR_SPIE_SETMASK)
HW_SPCSR_CPOL_CLRMASK 		equ     (~HW_SPCSR_CPOL_SETMASK)
HW_SPCSR_CPHA_CLRMASK       equ     (~HW_SPCSR_CPHA_SETMASK)
HW_SPCSR_SPIF_CLRMASK      	equ     (~HW_SPCSR_SPIF_SETMASK)
HW_SPCSR_WCOL_CLRMASK      	equ     (~HW_SPCSR_WCOL_SETMASK)
HW_SPCSR_MODF_CLRMASK      	equ     (~HW_SPCSR_MODF_SETMASK)
HW_SPCSR_DIV_CLRMASK      	equ     (~HW_SPCSR_DIV_SETMASK)
HW_SPCSR_RSVD_CLRMASK       equ     (~HW_SPCSR_RSVD_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;	int SPE             	: HW_SPCSR_SPE_WIDTH;	 
;	int MSTR             	: HW_SPCSR_MSTR_WIDTH;
;	int SPIE           	    : HW_SPCSR_SPIE_WIDTH;
;	int CPOL              	: HW_SPCSR_CPOL_WIDTH;
;	int CPHA             	: HW_SPCSR_CPHA_WIDTH;
;	int SPIF               	: HW_SPCSR_SPIF_WIDTH;
;	int WCOL         	    : HW_SPCSR_WCOL_WIDTH;
;	int MODF             	: HW_SPCSR_MODF_WIDTH;
;	int DIV              	: HW_SPCSR_DIV_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} spcsr_type;
;#define HW_SPCSR      (*(volatile spcsr_type _X*) (HW_SPI_BASEADDR))    /* SPI Control / Status Register */
;endc

; /////////////////////////////////////////////
; //  SPI Data Register Bit Definitions
HW_SPDR_SPIDATA_BITPOS      equ     (0)
HW_SPDR_RSVD_BITPOS         equ     (8)

HW_SPDR_SPIDATA_WIDTH       equ     (8)
HW_SPDR_RSVD_WIDTH          equ     (16)

HW_SPDR_SPIDATA_SETMASK     equ     (((1<<HW_SPDR_SPIDATA_WIDTH)-1)<<HW_SPDR_SPIDATA_BITPOS)
HW_SPDR_RSVD_SETMASK        equ     (((1<<HW_SPDR_RSVD_WIDTH)-1)<<HW_SPDR_RSVD_BITPOS)

HW_SPDR_SPIDATA_CLRMASK     equ    	(~HW_SPDR_SPIDATA_SETMASK) 
HW_SPDR_RSVD_CLRMASK        equ     (~HW_SPDR_RSVD_SETMASK)

;C insert
;typedef union               
;{
;    struct {
;	int SPIDATA             	: HW_SPDR_SPIDATA_WIDTH;
;    } B;
;
;    int I;
;    unsigned int U;

;} spdr_type;
;#define HW_SPDR      (*(volatile spdr_type _X*) (HW_SPI_BASEADDR+1))    /* SPI Data Register */
;endc


    endif

