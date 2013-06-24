////////////////////////////////////////////////////////////////////////////////
// Copyright(C) SigmaTel, Inc. 2003-2005
//
// Filename:     sectordef.h
// Description:  Contains structs, prototypes, equates for the NAND Hal & other 
//               routines.
////////////////////////////////////////////////////////////////////////////////

#ifndef _SECTORDEF_H
#define _SECTORDEF_H

    #define RAM_SECTOR_TOTAL_SIZE                 (528)
    #define RAM_SECTOR_DATA_SIZE                  (512)
    #define RAM_SECTOR_REDUNDANT_SIZE             (16)


    #define MMC_SECTOR_TOTAL_SIZE                 (528)
    #define MMC_SECTOR_DATA_SIZE                  (512)
    #define MMC_SECTOR_REDUNDANT_SIZE             (16)


    #define SSFDC_SECTOR_TOTAL_SIZE                 (528)
    #define SSFDC_SECTOR_DATA_SIZE                  (512)
    #define SSFDC_SECTOR_REDUNDANT_SIZE             (16)

 // Define large sector macros as small ones so sdk3.x fits in ram and mtp with small 
 // sector flash gets the performance boosts from optimized nand timings, etc. 
 // TOVERIFY: Comment out the next macro def when you want large sector support as 
 // in sdk2.6 and you can find the free data RAM. Or uncomment if you want to save data ram
 // and only support 512 byte sectors and their associated flash types. 
//#define LARGE_SECTOR_DEFS_AS_SMALL_SECTOR
#ifdef LARGE_SECTOR_DEFS_AS_SMALL_SECTOR 
    #define LARGE_SECTOR_TOTAL_SIZE                 (528)
    #define LARGE_SECTOR_DATA_SIZE                  (512)
    #define LARGE_SECTOR_REDUNDANT_SIZE             (16)
#else
    // Real large sector defines to use when large sector flash support is desired.
    #define LARGE_SECTOR_TOTAL_SIZE                 (2112)
    #define LARGE_SECTOR_DATA_SIZE                  (2048)
    #define LARGE_SECTOR_REDUNDANT_SIZE             (64)
#endif
    
    // round up to the nearest WORD, then add one to pad for the ECC DMA bug
    #define SECTOR_BUFFER_ALLOC_SIZE(a)             (((a+2)/3)+1)


    // These are moved here from chkdsk.h     TOVERIFY. 
    #define SEC_512_BYTES_PER_SECTOR                        SSFDC_SECTOR_DATA_SIZE
	#define SEC_512_BYTES_TOTAL_SIZE						SSFDC_SECTOR_TOTAL_SIZE
	#define SEC_512_WORDS_TOTAL_SIZE						176
    #define SEC_512_BYTES_CLUSTER_ENTRY_PER_SECTOR_SHIFT    12
    #define SEC_512_BYTES_DIR_REC_PER_SECTOR                16
    #define SEC_512_BYTES_WORDS_PER_SECTOR                  177
    #define SEC_2K_BYTES_PER_SECTOR                         LARGE_SECTOR_DATA_SIZE
	#define SEC_2K_BYTES_TOTAL_SIZE							LARGE_SECTOR_TOTAL_SIZE
    #define SEC_2K_BYTES_CLUSTER_ENTRY_PER_SECTOR_SHIFT     12
    #define SEC_2K_BYTES_DIR_REC_PER_SECTOR                 64
    #define SEC_2K_BYTES_WORDS_PER_SECTOR                   705
    #define SEC_2K_BYTES_WORDS_PER_SECTOR_NO_RA             683

    // Next 5 lines imported from nandsystemdrivewritesector.c
    #define RA_ATTR_MEMORY_OFFSET           682
    #define NUMBER_OF_WORDS_IN_512BYTES     171
    #define NUM_ECC_DATA_IN_WORDS           3
    #define SEC_512_WORD1_RA_SWAP           171
    #define SEC_512_WORD2_RA_SWAP           173

#endif  // _SECTORDEF_H

