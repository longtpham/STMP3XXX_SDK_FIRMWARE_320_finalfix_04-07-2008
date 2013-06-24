/**********************************************************************
 *
 *            Copyright (c) SigmaTel Inc.
 *
 * This code is confidential and proprietary product of SigmaTel.
 * Any unauthorized use, reproduction or transfer of this
 * code is strictly prohibited.
 **********************************************************************/
#ifndef _DAC_H_
#define _DAC_H_
  
void _long_interrupt(30) DAC_Refill_ISR(void);
void _long_interrupt(31) DAC_Underflow_ISR(void);
void _reentrant start_dac(void);
void _reentrant mix_init(void);
 
#endif