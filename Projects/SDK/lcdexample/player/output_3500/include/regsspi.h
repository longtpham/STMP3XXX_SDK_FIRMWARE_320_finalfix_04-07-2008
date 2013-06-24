//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//;; Copyright(C) SigmaTel, Inc. 2002-2003
//;; Filename    : regsspi.inc
//;; Description : Register definitions for SPI interface
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
#if !(defined(regsspiinc))
#define regsspiinc 1

#include "types.h"

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

//   SPI STMP3500 Registers 
//   Last Updated 7.7.2003 Dave Dyches
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#define HW_SPI_BASEADDR (0xFFF9)



// /////////////////////////////////////////////
// //  SPI Control/Status Register Bit Definitions
#define HW_SPCSR_SPE_BITPOS (0)
#define HW_SPCSR_MSTR_BITPOS (1)
#define HW_SPCSR_SPIE_BITPOS (2)
#define HW_SPCSR_CPOL_BITPOS (3)
#define HW_SPCSR_CPHA_BITPOS (4)
#define HW_SPCSR_SPIF_BITPOS (5)
#define HW_SPCSR_WCOL_BITPOS (6)
#define HW_SPCSR_MODF_BITPOS (7)
#define HW_SPCSR_DIV_BITPOS (8)
#define HW_SPCSR_RSVD_BITPOS (16)

#define HW_SPCSR_SPE_WIDTH (1)
#define HW_SPCSR_MSTR_WIDTH (1)
#define HW_SPCSR_SPIE_WIDTH (1)
#define HW_SPCSR_CPOL_WIDTH (1)
#define HW_SPCSR_CPHA_WIDTH (1)
#define HW_SPCSR_SPIF_WIDTH (1)
#define HW_SPCSR_WCOL_WIDTH (1)
#define HW_SPCSR_MODF_WIDTH (1)
#define HW_SPCSR_DIV_WIDTH (8)
#define HW_SPCSR_RSVD_WIDTH (8)

#define HW_SPCSR_SPE_SETMASK (((1<<HW_SPCSR_SPE_WIDTH)-1)<<HW_SPCSR_SPE_BITPOS)
#define HW_SPCSR_MSTR_SETMASK (((1<<HW_SPCSR_MSTR_WIDTH)-1)<<HW_SPCSR_MSTR_BITPOS)
#define HW_SPCSR_SPIE_SETMASK (((1<<HW_SPCSR_SPIE_WIDTH)-1)<<HW_SPCSR_SPIE_BITPOS)
#define HW_SPCSR_CPOL_SETMASK (((1<<HW_SPCSR_CPOL_WIDTH)-1)<<HW_SPCSR_CPOL_BITPOS)
#define HW_SPCSR_CPHA_SETMASK (((1<<HW_SPCSR_CPHA_WIDTH)-1)<<HW_SPCSR_CPHA_BITPOS)
#define HW_SPCSR_SPIF_SETMASK (((1<<HW_SPCSR_SPIF_WIDTH)-1)<<HW_SPCSR_SPIF_BITPOS)
#define HW_SPCSR_WCOL_SETMASK (((1<<HW_SPCSR_WCOL_WIDTH)-1)<<HW_SPCSR_WCOL_BITPOS)
#define HW_SPCSR_MODF_SETMASK (((1<<HW_SPCSR_MODF_WIDTH)-1)<<HW_SPCSR_MODF_BITPOS)
#define HW_SPCSR_DIV_SETMASK (((1<<HW_SPCSR_DIV_WIDTH)-1)<<HW_SPCSR_DIV_BITPOS)
#define HW_SPCSR_RSVD_SETMASK (((1<<HW_SPCSR_RSVD_WIDTH)-1)<<HW_SPCSR_RSVD_BITPOS)

#define HW_SPCSR_SPE_CLRMASK (~(WORD)HW_SPCSR_SPE_SETMASK)
#define HW_SPCSR_MSTR_CLRMASK (~(WORD)HW_SPCSR_MSTR_SETMASK)
#define HW_SPCSR_SPIE_CLRMASK (~(WORD)HW_SPCSR_SPIE_SETMASK)
#define HW_SPCSR_CPOL_CLRMASK (~(WORD)HW_SPCSR_CPOL_SETMASK)
#define HW_SPCSR_CPHA_CLRMASK (~(WORD)HW_SPCSR_CPHA_SETMASK)
#define HW_SPCSR_SPIF_CLRMASK (~(WORD)HW_SPCSR_SPIF_SETMASK)
#define HW_SPCSR_WCOL_CLRMASK (~(WORD)HW_SPCSR_WCOL_SETMASK)
#define HW_SPCSR_MODF_CLRMASK (~(WORD)HW_SPCSR_MODF_SETMASK)
#define HW_SPCSR_DIV_CLRMASK (~(WORD)HW_SPCSR_DIV_SETMASK)
#define HW_SPCSR_RSVD_CLRMASK (~(WORD)HW_SPCSR_RSVD_SETMASK)

typedef union               
{
    struct {
	int SPE             	: HW_SPCSR_SPE_WIDTH;	 
	int MSTR             	: HW_SPCSR_MSTR_WIDTH;
	int SPIE           	    : HW_SPCSR_SPIE_WIDTH;
	int CPOL              	: HW_SPCSR_CPOL_WIDTH;
	int CPHA             	: HW_SPCSR_CPHA_WIDTH;
	int SPIF               	: HW_SPCSR_SPIF_WIDTH;
	int WCOL         	    : HW_SPCSR_WCOL_WIDTH;
	int MODF             	: HW_SPCSR_MODF_WIDTH;
	int DIV              	: HW_SPCSR_DIV_WIDTH;
    } B;

    int I;
    unsigned int U;
} spcsr_type;
#define HW_SPCSR      (*(volatile spcsr_type _X*) (HW_SPI_BASEADDR))    /* SPI Control / Status Register */

// /////////////////////////////////////////////
// //  SPI Data Register Bit Definitions
#define HW_SPDR_SPIDATA_BITPOS (0)
#define HW_SPDR_RSVD_BITPOS (8)

#define HW_SPDR_SPIDATA_WIDTH (8)
#define HW_SPDR_RSVD_WIDTH (16)

#define HW_SPDR_SPIDATA_SETMASK (((1<<HW_SPDR_SPIDATA_WIDTH)-1)<<HW_SPDR_SPIDATA_BITPOS)
#define HW_SPDR_RSVD_SETMASK (((1<<HW_SPDR_RSVD_WIDTH)-1)<<HW_SPDR_RSVD_BITPOS)

#define HW_SPDR_SPIDATA_CLRMASK (~(WORD)HW_SPDR_SPIDATA_SETMASK) 
#define HW_SPDR_RSVD_CLRMASK (~(WORD)HW_SPDR_RSVD_SETMASK)

typedef union               
{
    struct {
	int SPIDATA             	: HW_SPDR_SPIDATA_WIDTH;
    } B;

    int I;
    unsigned int U;
} spdr_type;
#define HW_SPDR      (*(volatile spdr_type _X*) (HW_SPI_BASEADDR+1))    /* SPI Data Register */


#endif


