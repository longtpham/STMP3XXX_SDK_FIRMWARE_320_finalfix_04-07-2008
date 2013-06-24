/**********************************************************************
 *
 *            Copyright (c) SigmaTel Inc.
 *
 * This code is confidential and proprietary product of SigmaTel.
 * Any unauthorized use, reproduction or transfer of this
 * code is strictly prohibited.
 ***********************************************************************
 *
 *
 * support.c - Support functions for testing of the SDRAM
 *
 ***********************************************************************/

#include "sdramsupport.h"
#include "sdram.h"
#include "stmp_vlsi_env.h"

/* source and destination data arrays in DSP RAM: */
_X  WORD source_data[0x400];
_X WORD   returned_data[0x400];
WORD   test_result[0x200] _at (0x200);

/* parameters used in initiation of data transaction with SDRAM */
WORD    start_byte_send _at (0x500);
WORD    start_byte_receive _at (0x501);
WORD    big_endian _at (0x502);
WORD    num_bytes _at (0x503);
WORD    extaddr_lo _at (0x504);
WORD    extaddr_hi _at (0x505);
WORD    extaddr_base_lo _at (0x506);
WORD    extaddr_base_hi _at (0x507);
WORD    extaddr_mod_lo _at (0x508);
WORD    extaddr_mod_hi _at (0x509);
WORD    sysaddr_mod _at (0x50A);
WORD    tranread _at (0x50B);
WORD    i _at (0x50C);
WORD    j _at (0x50D);
WORD    timeout _at (0x50E);
_X WORD* _X sysaddr_base;

/***********************************************************************
 * data_initialize  - initialization of source and destination 
 *					  data arrays in the DSP RAM 
 *
 * Used repeatedly after each test has been completed (after every 
 * call to test())
 **********************************************************************/
void _reentrant data_initialize(){

	/* initialize source data region: */

	for (i = 0; i < 0x400; i++)
		source_data[i] =   ((INIT_SRC_B0 + i) & 0xff)       +
			              (((INIT_SRC_B1 + i) & 0xff) << 8) +
					      (((INIT_SRC_B2 + i) & 0xff) << 16);

	/* initialize return data region to simple binary count: */

	for (i = 0; i < 0x400; i++)
		returned_data[i] = INIT_RETURN_DATA;

	return;
}


/***********************************************************************
 * init_sdram  - Initialization of SDRAM memory
 *				 Used in the initialize() only
 *
 * Time and memory intensive as it sweeps the entire SDRAM memory
 **********************************************************************/

void _reentrant init_sdram(){

	INT ii;
	for (i = 0; i < 0x1000; i++){
		for (ii = 0; ii < 4; ii++) {
		    num_bytes = 0x400;
			timeout = SDRAMTIMEOUT * num_bytes;
			tranread = 0;
			start_byte_send = 0;
			start_byte_receive = 0;
			big_endian = 0;
			extaddr_lo = i * 0x1000 + ii * 0x400;
			extaddr_hi = 0;
			extaddr_base_lo = 0;
			extaddr_base_hi = 0;
			extaddr_mod_lo = 0;
			extaddr_mod_hi = 0;
			*sysaddr_base = 0;
			sysaddr_mod = 0;

			if (sdraminittrans (SDRAMTYPE_128Mb_8,
						tranread,
						start_byte_send,
						big_endian,
						extaddr_lo,
						extaddr_hi,
						returned_data,
						num_bytes,
						sysaddr_base,
						sysaddr_mod,
						extaddr_base_lo,
						extaddr_base_hi,
						extaddr_mod_lo,
						extaddr_mod_hi,
						timeout) != 1)
				TEST_FAILED;

		}
	}
}


/***********************************************************************
 * initialize  - One-time initialization routines included here
 *
 *
 * Used for initializing the SDRAM memory, test result region, 
 * SDRAM init registers
 **********************************************************************/
void _reentrant initialize(){

	/* initialize test result region: */
	for (i = 0; i < 0x200; i++)
		test_result[i] = 0x00;

	if (gpiopwrupall() != 1)
		TEST_FAILED;

	if (sdraminitregs(CLK_DIV) != 1)
		TEST_FAILED;

	init_sdram();

	j = 0; // no of tests failed
	
	return;
}


/***********************************************************************
 * test_basic  - Test run with default values of start_byte_send,
 *				 start_byte_receive, sysaddr_mod
 *
 * 
 * Parameters  
 * byteCount - Number of Bytes to be Transfered
 * extAdrL   - Sdram External Address Low 
 **********************************************************************/
void _reentrant test_basic(WORD byteCount, WORD extAdrL){
	test(byteCount, extAdrL, 0, 0, 0);
}

/***********************************************************************
 * test  - main routine which is called repeatedly 
 *		   (once for each test run)
 *		   Two transactions take place, first a write to the SDRAM and
 *		   second, a read from the SDRAM. It is followed by verification
 *		   of data consistency.
 *	
 * Parameters
 * byteCount - Number of Bytes to be Transfered
 * extAdrL   - Sdram External Address Low 
 * sbSend    - start byte send value
 * sbRecv    - start byte receive value
 * sysMod    - Sdram DSP modulo register - offset for module access
 * 
 **********************************************************************/
void _reentrant test(WORD byteCount, WORD extAdrL, 
					 WORD sbSend, WORD sbRecv, WORD sysMod){
    num_bytes = byteCount;
	timeout = SDRAMTIMEOUT * num_bytes;
	tranread = 0;
	start_byte_send = sbSend;
	start_byte_receive = sbRecv;
	big_endian = 0;
	extaddr_lo = extAdrL;
	extaddr_hi = 0;
	extaddr_base_lo = 0;
	extaddr_base_hi = 0;
	extaddr_mod_lo = 0;
	extaddr_mod_hi = 0;
	sysaddr_base = source_data;
	sysaddr_mod = sysMod;
  
	if (sdraminittrans (SDRAMTYPE_128Mb_8,
						tranread,
						start_byte_send,
						big_endian,
						extaddr_lo,
						extaddr_hi,
						source_data,
						num_bytes,
						sysaddr_base,
						sysaddr_mod,
						extaddr_base_lo,
						extaddr_base_hi,
						extaddr_mod_lo,
						extaddr_mod_hi,
						timeout) != 1)
		TEST_FAILED;

	tranread = 1;
	sysaddr_mod = 0;

 	if (sdraminittrans (SDRAMTYPE_128Mb_8,
						tranread,
						start_byte_receive,
						big_endian,
						extaddr_lo,
						extaddr_hi,
						returned_data,
						num_bytes,
						sysaddr_base,
						sysaddr_mod,
						extaddr_base_lo,
						extaddr_base_hi,
						extaddr_mod_lo,
						extaddr_mod_hi,
						timeout) != 1)
	    TEST_FAILED;

	sysaddr_mod = sysMod; // set again to verify result

	verify_result();
	data_initialize();
	erase_used_sdram();
}


/***********************************************************************
 * erase_used_sdram - Erasing currently written data in SDRAM before
 *					  next test is carried out
 *					  It is done by overwriting currently written data
 *
 ***********************************************************************/

void _reentrant erase_used_sdram(){
	tranread = 0;

	if (sdraminittrans (SDRAMTYPE_128Mb_8,
						tranread,
						start_byte_send,
						big_endian,
						extaddr_lo,
						extaddr_hi,
						returned_data, // used because it has been just cleared
						num_bytes,
						sysaddr_base,
						sysaddr_mod,
						extaddr_base_lo,
						extaddr_base_hi,
						extaddr_mod_lo,
						extaddr_mod_hi,
						timeout) != 1)
		TEST_FAILED;

}


/***********************************************************************
 * test_vary_extAdrL - Run tests for a given bank using all addresses in
 *					   it as values for 'Sdram External Address Low' 
 *					   variable
 *					   Run tests for different values of number of bytes	
 *
 *
 * Parameters
 * bank_no			 - Bank being tested
 * row_no			 - Row being tested
 * sbSend			 - start byte send value
 * sbRecv			 - start byte receive value
 * sysMod			 - Sdram DSP modulo register; offset for module access
 * 
 ***********************************************************************/

void _reentrant test_vary_extAdrL(WORD bank_no, WORD row_no,
								  WORD sbSend, WORD sbRecv, WORD sysMod){
	// varying the extaddr_lo parameter
	WORD k, offset;
	offset = 0x400 * bank_no + 0x1000 * row_no;

	for (k = 0; k <= EOB_0_128; k++){ 
		// vary number of bytes being transferred to SDRAM
		// arbitarily selecting number of bytes to be transferred
		test(1, k + offset, sbSend, sbRecv, sysMod);
		test(2, k + offset, sbSend, sbRecv, sysMod);
		test(8, k + offset, sbSend, sbRecv, sysMod);
	   	test(50, k + offset, sbSend, sbRecv, sysMod);
	}
	return;
}

/***********************************************************************
 * test_bank - Run tests for a given bank using all combinations of
 *			   start and receive byte values
 *
 * Parameters
 * bank_no	 - Bank being tested
 * row_no	 - Row being tested
 * 
 ***********************************************************************/

void _reentrant test_bank(WORD bank_no, WORD row_no){
	// vary the start and receive byte values
	WORD ii;
	for(ii=0; ii<3; ii++){
		test_vary_extAdrL(bank_no, row_no, 0, ii, 0);
		test_vary_extAdrL(bank_no, row_no, 1, ii, 0);
		test_vary_extAdrL(bank_no, row_no, 2, ii, 0);
	}
}

/***********************************************************************
 * test_row - Test all the 4 banks in a given row
 *
 * Parameters
 * row_no	- Row being tested
 * 
 ***********************************************************************/

void _reentrant test_row(WORD row_no){
	test_bank(0,row_no);
	test_bank(1,row_no);
	test_bank(2,row_no);
	test_bank(3,row_no);
}

/***********************************************************************
 * verify_result - Called in the test function
 *				   Used to verify the results of the test by comparing
 *				   source_data[] and returned_data[] 		
 * 
 ***********************************************************************/

void _reentrant verify_result(){
	WORD temp, temp1, i;
	WORD no_words, rem_bytes;
	WORD bytes = num_bytes;

	if (sysaddr_mod != 0){
		verify_modulo();
		return;
	}

	if (start_byte_send != start_byte_receive){
		shift_bytes();
		
		// update start_byte_send and start_byte_receive
		start_byte_send = 0;
		start_byte_receive = 0;
	}

	// If start_byte_receive != 0, we check the first address location
	if (start_byte_receive != 0) {
		if (start_byte_receive == 1) {
			if(bytes == 1) {
				temp = source_data[0] & 0x00FF00;
				temp1 = returned_data[0] & 0x00FF00; 
				if (temp != temp1) {
					TEST_FAILED;
					test_result[j++] = extaddr_lo;
				}
				return;
			}
			else if (bytes > 1) {
				temp = source_data[0] & 0xFFFF00;
				temp1 = returned_data[0] & 0xFFFF00;
				if (temp != temp1) {
					TEST_FAILED;
					test_result[j++] = extaddr_lo;
					return;
				}
				bytes -= 2;
			}
		}
	
		if (start_byte_receive == 2) {
			temp = source_data[0] & 0xFF0000;
			temp1 = returned_data[0] & 0xFF0000; 
			if (temp != temp1) {
				TEST_FAILED;
				test_result[j++] = extaddr_lo;
				return;
			}
			bytes--;
		}
	
		no_words = divide(bytes, 3);
		rem_bytes = modulus(bytes, 3); 

		for (i=1; i <= no_words; i++){
			if(source_data[i] != returned_data[i]){
				TEST_FAILED;
				test_result[j++] = extaddr_lo;
				return;
			}
		}
	}
	else { // start_byte_receive = 0
		no_words = divide(num_bytes,3);
		rem_bytes = modulus(num_bytes,3); 

		for (i=0; i<no_words; i++){
			if(source_data[i] != returned_data[i]){
				TEST_FAILED;
				test_result[j++] = extaddr_lo;
				return;
			}
		}
	}
	
	if (rem_bytes == 1) { // each mem loc = 3 bytes
		temp = source_data[i] & 0xFF;
		temp1 = returned_data[i] & 0xFF;
		if (temp != temp1) {
			TEST_FAILED;
			test_result[j++] = extaddr_lo;
		}
	} 
	else if (rem_bytes == 2) { 
		temp = source_data[i] & 0xFFFF;
		temp1 = returned_data[i] & 0xFFFF;
		if (temp != temp1) {
			TEST_FAILED;
			test_result[j++] = extaddr_lo;
		}
	}
	
}


/***********************************************************************
 * shift_bytes - Used to shift bytes if the send or receive byte
 *				 values are not 0
 *
 ***********************************************************************/
void _reentrant shift_bytes(){
	INT ii;
	INT temp = 0;
	WORD temp1 = divide(num_bytes,3) + 2;

	if (start_byte_receive == 1){
			
		for(ii=0; ii<temp1; ii++){
			// save right bytes
			if (ii != 0)
				temp = returned_data[ii] & 0xFF;

			// shift data
			returned_data[ii] >>= 8;

			// copy saved bytes in the earlier memory location
			if (ii != 0)
				returned_data[ii-1] += temp * 0x10000;

		}
	}
	else if (start_byte_receive == 2){
	
		for(ii=0; ii < temp1; ii++){
			// save right bytes
			if (ii != 0)
				temp = returned_data[ii] & 0xFFFF;

			// shift data
			returned_data[ii] >>= 16;

			// copy saved bytes in the earlier memory location
			if (ii != 0)
				returned_data[ii-1] += temp * 0x100;

		}
	}


	if (start_byte_send == 1){
			
		for(ii=0; ii< temp1; ii++){
			// save right bytes
			if (ii != 0)
				temp = source_data[ii] & 0xFF;

			// shift data
			source_data[ii] >>= 8;

			// copy saved bytes in the earlier memory location
			if (ii != 0)
				source_data[ii-1] += temp * 0x10000;

		}
	}
	else if (start_byte_send == 2){
	
		for(ii=0; ii < temp1; ii++){
			// save right bytes
			if (ii != 0)
				temp = source_data[ii] & 0xFFFF;

			// shift data
			source_data[ii] >>= 16;

			// copy saved bytes in the earlier memory location
			if (ii != 0)
				source_data[ii-1] += temp * 0x100;

		}
	}

}


/***********************************************************************
 * verify_modulo - Called by the verify result function when system 
 *				   modulo functionality is used
 *
 ***********************************************************************/
void _reentrant verify_modulo(){
	WORD count;
	WORD temp1, cc, cc1, ref, rem_bytes, rem_words;
	WORD temp = 0;

	// save data from first word before shifting
	if(start_byte_send != 0) {

		if(start_byte_send == 1){
			temp = source_data[0] & 0xFF;
			temp <<= 16;
		}
		else if(start_byte_send == 2){
			temp = source_data[0] & 0xFFFF;
			temp <<= 8;
		}
	}

	shift_bytes();

	// manipulation of source data for modular comparison
	if(start_byte_send == 1){
		source_data[sysaddr_mod] &= 0xFFFF;
	} 
	else if(start_byte_send == 2){
		source_data[sysaddr_mod] &= 0xFF;
	}

	source_data[sysaddr_mod] |= temp;

	count = divide(num_bytes ,(sysaddr_mod + 1)*3);
	rem_bytes = num_bytes - (count * (sysaddr_mod + 1)*3);

	// start comparing sent and received data
	for(cc=0; cc<count; cc++){
		for(cc1=0; cc1<=sysaddr_mod; cc1++){
			if(returned_data[cc*(sysaddr_mod + 1) + cc1] != source_data[cc1]){
				TEST_FAILED;
				test_result[j++] = extaddr_lo;
				return;
			}
		}
	}

	ref = count * (sysaddr_mod + 1);
	rem_words = divide(rem_bytes,3);
	rem_bytes = modulus(rem_bytes,3);

	for(cc=0; cc<rem_words; cc++){
		if(source_data[cc] != returned_data[ref+cc]){
			TEST_FAILED;
			test_result[j++] = extaddr_lo;
			return;
		}
	}

	ref += rem_words;

	if(rem_bytes == 1){
		temp = returned_data[ref] & 0xFF;
		temp1 = source_data[rem_words] & 0xFF;
		if(temp != temp1){
			TEST_FAILED;
			test_result[j++] = extaddr_lo;
			return;
		}
	} else if (rem_bytes == 2){
		temp = returned_data[ref] & 0xFFFF;
		temp1 = source_data[rem_words] & 0xFFFF;
		if(temp != temp1){
			TEST_FAILED;
			test_result[j++] = extaddr_lo;
			return;
		}
	}

}

WORD _reentrant divide(WORD a, WORD b){
	INT count = 0;
		
	while(a >= b){
		a -= b;
		count++;
	}

	return count;
}

WORD _reentrant modulus(WORD a, WORD b){
	while (a >= b){
		a -= b;
	}
	return a;
}
