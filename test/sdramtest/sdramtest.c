/**********************************************************************
 *
 *            Copyright (c) SigmaTel Inc.
 *
 * This code is confidential and proprietary product of SigmaTel.
 * Any unauthorized use, reproduction or transfer of this
 * code is strictly prohibited.
 **********************************************************************
 * Main test program - includes series of modules that test different
 * features of the SDRAM
 **********************************************************************/
//#define  DEVICE_3410

#include "sdramsupport.h"
#include "dac.h"
#include "stmp_vlsi_env.h"

//#define MAXLCD_CONT 100

void _reentrant test1(void);
void _reentrant test2(void);
void _reentrant test3(void);
void _reentrant test4(void);
void _reentrant test5(void);
void _reentrant test6(void);
void _reentrant test7(void);

WORD _X cc;

void main ()
{
	initialize();
	data_initialize();
	start_dac();

//	test2();
	test3();	
	test4();
	test5();
	test6();	
//	test7();

	//TEST_PASSED;
    SystemHalt();
 	return;
}


// Test 1 : Testing different rows in the SDRAM
// Total number of rows in SDRAM is 0x1000
void _reentrant test1(){
	test_row(0x0);
	test_row(0xFFF);
}

// Test 2 : Write equal amount of data on either sides of the bank
void _reentrant test2(){
	for(cc=0; cc<0x400; cc++){
		test_basic(2*(cc+1), EOB_0_128 - cc);
		test_basic(2*(cc+1), EOB_3_128 - cc);
	}
}

// Test 3 : Slide window of data over the edge of last bank (into next row)
void _reentrant test3(){

	for(cc=0; cc <= 26; cc++)
		test_basic(25, EOB_3_128 - (25 - cc));
}

// Test 4 : Repeating the failure sequence for different rows
void _reentrant test4(){

	for(cc=0; cc<100; cc++)
		test_basic(7, 0xFFA + cc * 0x1000);
}

// Test 5 : Repeating the failure sequence for the same row
void _reentrant test5(){

	for(cc=0; cc<20; cc++)
		test_basic(8, 0xFFFFFA);
}

// Test 6 : Test different start and receive byte combinations
void _reentrant test6(){
	test(80, 0xFFA, 0, 1, 0);
	test(80, 0xFFA, 0, 2, 0);
	test(80, 0xFFB, 1, 0, 0);
	test(80, 0xFFB, 1, 2, 0);
	test(80, 0xFFC, 2, 0, 0);
	test(80, 0xFFC, 2, 1, 0);
}

// Test 7 : Test for system modulo operation

void _reentrant test7(){
	test(8, 0, 1, 2, 3);
	test(24,0xFFA, 0, 0, 4);
	test(28,0xFFA, 1, 2, 4);
	for(cc=0; cc<=0x400; cc++){
		test(35, 0xC00 + cc, 1, 2, 3);
	}
}

#pragma asm
Exec:
	jsr Fmain
	rts

#pragma endasm