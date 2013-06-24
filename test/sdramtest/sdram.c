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
 * sdram.c - access/control functions for the SDRAM memory interface 
 * peripheral
 *
 ***********************************************************************/
//#define  DEVICE_3410

/* Comment out foll defn to disable software workaround */
#define  SOFT_WORKAROUND

#include "sdram.h"

/***********************************************************************
 * SDRAMInitregs 
 * Sdram clock divide - Programs the sdram clock rate this is divided by
 *                      1 to 15 of the system clock (dclk)
 **********************************************************************
 * Sdram Interrrupt Enable 
 * Sdram Enable   

 * Sdram Gpio Control - Since the SDRAM address and data bus is shared
 *                      with the EMC which is also the default. This 
 *                      has to be switched to SDRAM.

 **********************************************************************/
INT gpiopwrupall (void) {
	HW_GP0PWR.I = (unsigned int)(0xFFFFFF & 0xFFFFFF); // warning removed: changed cast from INT to unsigned int Nov24,03
	HW_GP1PWR.I = (unsigned int)(0xFFFFFF & 0xFFFFFF);
	HW_GP2PWR.I = (unsigned int)(0xFFFFFF & 0xFFFFFF);
	HW_GP3PWR.I = (unsigned int)(0xFFFFFF & 0xFFFFFF);

	return(1); 
}

INT ccrinit (WORD freq_const) {

	HW_CCR.B.PDIV = freq_const ;
	HW_CCR.B.CKRST = 1;
	HW_CCR.B.PLLEN = 1;
	HW_CCR.B.CKSRC = 1;

	return(1);
}

INT sdraminitregs ( INT sdramclkdiv) {

	HW_SDRAM_CSR.B.SDRAMEN = 1;
	HW_SDRAM_CSR.B.IE = 1;
	HW_SDRAM_CSR.B.SDRAM = 1;
	HW_SDRAM_CSR.B.DIV = sdramclkdiv;

	return(1); /* pass */
}



/***********************************************************************
 * SDRAMInitTrans - initiate a SDRAM  device transaction
 *
 * Parameters 
 * Sdram Type     - Programs the width of the address bus to differentiate
 *                  between 64Mb, 128Mb, or 256Mb SDRAMS
 * Sdram Read         - 0 = transaction is write; read otherwise
 * Sdram External Address Low 
 * Sdram External Address High  
 * Sdram Internal System Address

 * NumBytes              - Number of Bytes to be Transfered
 * Sdram DSP modulo base address register
 * Sdram DSP modulo register - offset for module access
 * Sdram External SDRAM modulo base address register Low
 * Sdram External SDRAM modulo base address register High
 * Sdram External SDRAM module address Low
 * Sdram External SDRAM module address High
 * TimeoutCount - timeout counter (how many PIO polls before giving up)
 *
 **********************************************************************
 * Currently 
 * Unprogrammed  -Mode currently stmp3410 only supports full page burst
 *               sequential, cas latency = 2
 *               -Timings SDRAM timings which could optimize speed 
 **********************************************************************/

/* Tests should not fail if 'SOFT_WORKAROUND' is defined */

INT _reentrant sdraminittrans ( INT sdramtype,
                     INT sdramread,
                     INT startbyte,
                     INT bigendian,
                     WORD sdramextaddrlo,
                     WORD sdramextaddrhi,
                     _X WORD* sdramsysaddr,
                     WORD numbytes,
                     _X WORD* sdramsysmodbase,
                     WORD sdramsysmod,
                     WORD sdramextmodbaselo,
                     WORD sdramextmodbasehi,
                     WORD sdramextmodlo,
                     WORD sdramextmodhi,
					 INT  TimeoutCount
) {

	WORD temp=0;
	INT rembytes=0;

	HW_SDRAM_TYPE.I = sdramtype & 0x03;  
	HW_SDRAM_CSR.B.RNW = sdramread;
	HW_SDRAM_CSR.B.SBYTE = startbyte;
	HW_SDRAM_CSR.B.BIGE = bigendian;
	HW_SDRAM_ADDR1 = ((WORD) sdramextaddrlo) & 0xFFFFFF;
	HW_SDRAM_ADDR2 = ((WORD) sdramextaddrhi) & 0xF;
	HW_SDRAM_SYSADDR = ((WORD) sdramsysaddr) & 0xFFFF;

#ifdef SOFT_WORKAROUND 
	temp = HW_SDRAM_ADDR1 % 0x1000;
	if( numbytes > (0x1000 - temp) ){
		rembytes = numbytes - (0x1000 - temp);
		numbytes = 0x1000 - temp;
	}
#endif
	
	HW_SDRAM_SIZE = numbytes;
	HW_SDRAM_BAR = ((WORD) sdramsysmodbase) & 0xFFFF;
	HW_SDRAM_MR = ((WORD) sdramsysmod) & 0xFFFF;
	HW_SDRAM_DBAR1 = ((WORD) sdramextmodbaselo) & 0xFFFFFF;
	HW_SDRAM_DBAR2 = ((WORD) sdramextmodbasehi) & 0xF;
	HW_SDRAM_DMR1 = sdramextmodlo;
	HW_SDRAM_DMR2 = sdramextmodhi;

	HW_SDRAM_CSR.B.KICK = 1;
	/* poll until transaction is done */
	if (pollsdramtransactiondone(TimeoutCount) != 1) return(0);
	
#ifdef SOFT_WORKAROUND 
	/* Check if another transaction needed */
	while(rembytes > 0){
		if (rembytes > 0x1000) 
			HW_SDRAM_SIZE = 0x1000;
		else 
			HW_SDRAM_SIZE = rembytes;

		rembytes -= HW_SDRAM_SIZE;

		HW_SDRAM_CSR.B.KICK = 1;
		/* poll until transaction is done */
		if (pollsdramtransactiondone(TimeoutCount) != 1) return(0);
	}
#endif

	return(1); /* pass */

}

/***********************************************************************
 * programtimingregs - Program SDRAM timing parameters
 *
 * Parameters
 *   Initialization delay - Micron recommends 100us , Samsung recommends
 *                          200us. This programs the number of cycles of
 *                          sdram clock. INIT
 *   Precharge delay  -     Time from precharge to next command. 
 *                          This programs the number of cycles of
 *                          sdram clock. TRP
 *   Refresh delay    -     Delay from refresh command to any next command
 *                          This programs the number of cycles of
 *                          sdram clock. TRFC
 *   Exit self refresh delay - Time from the exit of self refresh to
 *                             the next command (most likely Refresh)
 *                          This programs the number of cycles of
 *                          sdram clock. TXSR
 *   Refresh period   -     Every 64ms all rows (4096) sdram has to be refreshed
 *                          Thus averaging 15us for refresh. This programs
 *                          the interval for refresh.
 *                          This programs the number of cycles of
 *                          sdram clock. TREF
 *   Active delay     -     Active sets the row for access. This programs
 *                          the delay from active to the next command. 
 *                          This programs the number of cycles of
 *                          sdram clock. TRCD
 * Returns
 *   pass (1) or fail (0) indicator
 **********************************************************************/

INT _reentrant programtimingregs( INT init, INT trp, INT trfc, INT txsr, INT tref, INT trcd) {

	HW_SDRAM_TIMER1.B.INIT = init;
	HW_SDRAM_TIMER1.B.TRP = trp;
	HW_SDRAM_TIMER1.B.TRFC = trfc;
	HW_SDRAM_TIMER2.B.TXSR = txsr;
	HW_SDRAM_TIMER2.B.TREF = tref;
	HW_SDRAM_TIMER2.B.TRCD = trcd;

	return (1);
}


/***********************************************************************
 * PollTransactionDone - poll completion of an external memory
 * transaction
 *
 * Parameters
 *   TimeoutCount - timeout counter (how many PIO polls before giving
 *                  up)
 *
 * Returns
 *   pass (1) or fail (0) indicator
 **********************************************************************/
INT _reentrant pollsdramtransactiondone (INT TimeoutCount) {

	do {
		TimeoutCount--;
		if (!TimeoutCount)
			return(TimeoutCount); 

	} while(HW_SDRAM_CSR.B.KICK == 1) ; /*While transaction in progress */

	/* Wait for the transfer to complete (IRQP == 1) %%Will spin forever if it doesn't complete */
	if (HW_SDRAM_CSR.B.ISTAT == 1)
		HW_SDRAM_CSR.B.ISTAT = 1;

    return(1);
}