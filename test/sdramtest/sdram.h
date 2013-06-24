/**********************************************************************
 *
 *            Copyright (c) SigmaTel Inc.
 *
 * This code is confidential and proprietary product of SigmaTel.
 * Any unauthorized use, reproduction or transfer of this
 * code is strictly prohibited.
 **********************************************************************/
#ifndef _SDRAM_H_
#define _SDRAM_H_

#include "hwequ.h"

#define SDRAMTYPE_64Mb_8 0x01
#define SDRAMTYPE_128Mb_8 0x02
#define SDRAMTIMEOUT 100


INT gpiopwrupall (void);
INT ccrinit (WORD); 
INT sdraminitregs  (INT);
INT _reentrant sdraminittrans(INT, INT, INT, INT, WORD, WORD, _X WORD*, WORD,
							  _X WORD*, WORD, WORD, WORD, WORD, WORD, INT);

INT _reentrant pollsdramtransactiondone (INT);
INT _reentrant programtimingregs (INT, INT, INT, INT, INT, INT);

#endif
