/**********************************************************************
 *
 *            Copyright (c) SigmaTel Inc.
 *
 * This code is confidential and proprietary product of SigmaTel.
 * Any unauthorized use, reproduction or transfer of this
 * code is strictly prohibited.
 **********************************************************************/
#ifndef _SUPPORT_H_
#define _SUPPORT_H_

#include "hwequ.h"

#define  INIT_RETURN_DATA     0x888888
#define  INIT_SRC_B0          0xaa
#define  INIT_SRC_B1          0xee
#define  INIT_SRC_B2          0x33
#define  CLK_DIV              1

#define  EOB_0_64             0x1FF
#define  EOB_1_64             0x3FF
#define  EOB_2_64             0x5FF
#define  EOB_3_64             0x7FF
#define  EOF_64               0x7FFFFF

#define  EOB_0_128            0x3FF
#define  EOB_1_128            0x7FF
#define  EOB_2_128            0xBFF
#define  EOB_3_128            0xFFF
#define  EOF_128              0xFFFFFF



/* support functions */
void _reentrant data_initialize(void);
void _reentrant initialize(void);
void _reentrant init_sdram(void);

void _reentrant test(WORD, WORD, 
					 WORD, WORD, WORD);
void _reentrant test_vary_extAdrL(WORD, WORD, 
								  WORD, WORD, WORD);
void _reentrant test_bank(WORD, WORD);
void _reentrant test_row(WORD);
void _reentrant test_basic(WORD, WORD);

void _reentrant erase_used_sdram(void);
void _reentrant verify_result(void);
void _reentrant shift_bytes(void);
void _reentrant verify_modulo(void);

WORD _reentrant divide(WORD, WORD);
WORD _reentrant modulus(WORD, WORD);

#endif