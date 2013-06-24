/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/ronsdsa.c 1.3 2005/02/19 00:43:06Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



/*
 *   Major modifications and simplications made by Paul Kocher to 
 *   minimize code size and maximize efficiency for DSA signature
 *   verification.
 *
 *   Derived from CryptoLib 1.1  (Copyright notices moved to end.)
 *
 *   New functions added:
 *       SimpleDSAVerify              Check a signature. Returns 0=bad 1=good.
 
 
 */
#include "ronsdsa.h"







void Simple_Multiply	(unsigned short *product, unsigned short *a, const unsigned short *b, int len);
void Simple_PolishReduce(unsigned short *a, const unsigned short *b,int len,unsigned short hi);
void Simple_Reduce		(unsigned short *a, const unsigned short *b,int b_len);
void Simple_ModMult		(unsigned short *product, unsigned short *a
						, const unsigned short *b, const unsigned short *p,int len);

void Simple_ModExp		(unsigned short *a, const unsigned short *x,unsigned short *e
						, const unsigned short *p,int len,int eBits);


/********************************************************************/

/*
	Description:
		This is the interface when using SysCallFunction().
*/

int _reentrant
RonsSimpleDSAVerifyEntry( INT rawHash, INT rawSig, INT *rawPubKey )
{
	return RonsSimpleDSAVerify( (unsigned short*) rawHash, (unsigned short *) rawSig, (unsigned short *) rawPubKey );
}

/********************************************************************/

int
RonsSimpleDSAVerify	(unsigned short rawHash[10], unsigned short rawSig[20]
				, const unsigned short rawPubKey[RAW_PUBKEY_BYTES/2])
{
	int	 i;
	unsigned short	ux[10];
	unsigned short	s1[KEY_BITS/16];			// 32 words
	unsigned short	s2[KEY_BITS/16];			// 32 words

#define RAWHASH           (rawHash)
#define RAWSIG_R          (rawSig)
#define RAWSIG_W          (rawSig+10)
#define RAWPUBKEY_Q       (rawPubKey)
#define RAWPUBKEY_P       (rawPubKey+10)
#define RAWPUBKEY_G       (rawPubKey+10+KEY_BITS/16)
#define RAWPUBKEY_Y       (rawPubKey+10+2*KEY_BITS/16)


	/* Compute u1 = (w * md) mod q */
	Simple_ModMult(ux, RAWSIG_W, RAWHASH, RAWPUBKEY_Q, 10);

	/* Compute s1 = g^u1 mod p */
	Simple_ModExp(s1, RAWPUBKEY_G, ux, RAWPUBKEY_P, KEY_BITS/16, 160);

	/* Compute u2 = (w * r) mod q */
	Simple_ModMult(ux, RAWSIG_W, RAWSIG_R, RAWPUBKEY_Q, 10);

	/* Compute s2 = y^u2 mod p */
	Simple_ModExp(s2, RAWPUBKEY_Y, ux, RAWPUBKEY_P, KEY_BITS/16, 160);

	/* Compute s1 = s1 * s2 mod p */
	Simple_ModMult(s1, s1, s2, RAWPUBKEY_P, KEY_BITS/16);

	/* Compute s1 = s1 mod q */
	for (i = KEY_BITS/16 - 20; i > 0; i -= 10)
		Simple_Reduce(s1+i, RAWPUBKEY_Q, 10);		/* s1=s1 mod q */

	Simple_Reduce(s1, RAWPUBKEY_Q, 10);

	/* Return 1 (success) iff s1 = r, else return 0 */
	for (i = 0; i < 10; i++) {
		if (s1[i] != RAWSIG_R[i])
			return (0);
	}
	return (1);
}


/*
 *  Compute a*b, where each are len unsigned short's in length.
 *  Note: a and b must be different (cannot be same buffer)
 *
 *  lA1 - a[j] * b[k]
 *  lA2 - accumulate LSW of lA1 and all of lA3
 *  lA3 - accumulate MSW of lA1
 */

void
Simple_Multiply(unsigned short *product, unsigned short *a, const unsigned short *b, int len)
{
	int	 i
		,j
		,k;

	unsigned long	lA1;
	unsigned long	lA2;
	unsigned long	lA3;

	lA2 = 0;
	for (i = 0; i < ( 2 * len) ; i++) {
		j = 0;      /* min */
		k = i;      /* max */
		if (k >= len) {
			k = len - 1;
			j = i - k;
		}
		lA3 = 0;

		while ( 1 ) { 
			if ((k < 0) || (j>=len))
				break; 
			lA1	= (unsigned long) a[j];
			lA1	= (lA1 * b[k]) & 0xFFFFFFFF;			// mmiu - Added & 0xFFFFFFFF for SigmaTel
			lA2	= ( ( lA1 & 0x0000FFFFL ) + lA2 ) /*& 0xFFFFFFFF*/;		// mmiu - Added masks
			lA3	= ( lA3 + (lA1 >> 16) ) /*& 0xFFFFFFFF*/;				// mmiu - Added mask
			j++;
			k--;
		}

		product[i] = lA2 & 0xFFFF;
		lA2 = (lA2 >> 16) & 0x0000FFFF;						// mmiu - Added mask
		lA2 = (lA2 + lA3) /*& 0xFFFFFFFF*/;
	}
}

/*
 *  Compute a = a mod b, where a is 2*b_len unsigned short's in length.
 *  IMPORTANT: The most significant digit of B can't be very small.
 *     (This is checked by MakeRawPublicKey and MakeRawSignature.)
 */

void
Simple_Reduce(unsigned short *a, const unsigned short *b, int b_len)
{
	unsigned long	 subMult
					,lA1
					,bmax;
	int i,j;
	unsigned short	tmp;

	bmax = (unsigned long)( b[ b_len -1 ] ) + 1;
	Simple_PolishReduce( a + b_len, b, b_len, 0);
	
	for (i = b_len-1; i >= 0; i--) {

		/* subMult is less than or equal to the multiple of a to subtract */
		subMult = (((((unsigned long)a[b_len+i]) << 16) + a[b_len+i-1]) & 0xFFFFFFFF) / bmax;		// mmiu - Added mask

		/* subtract subMult*b from a[i] */
		if (subMult) {
			lA1 = 0;

			for (j = 0; j < b_len; j++) {
				lA1 = (lA1 + subMult * b[j]) & 0xFFFFFFFF;		// mmiu - Added mask
				tmp = a[i+j];
//				if ( tmp < ((unsigned short) lA1))
				if ( tmp < (unsigned short) (lA1 & 0xFFFF) )	// mmiu - Force 16-bit arithmetic
					lA1 = lA1 + 0x10000L;						// mmiu - 32-bit mask unnecessary

//				a[i+j] = tmp - ((unsigned short) lA1);
				a[i+j] = (tmp - (unsigned short) (lA1 & 0xFFFF)) & 0xFFFF;	// mmiu - Force 16-bit arithmetic
				lA1 = lA1 >> 16;
			}

			tmp = a[ i + b_len ];
//			tmp = tmp - (unsigned short) lA1;
			tmp = (tmp - (unsigned short) (lA1 & 0xFFFF)) & 0xFFFF;			// mmiu - Force 16-bit arithmetic
			a[ i + b_len ] = tmp;
		}
		Simple_PolishReduce(a + i, b, b_len, a[i + b_len]);
		a[i+b_len] = 0;
	}
}


/* Subtract b from a until a < b. */
void 
Simple_PolishReduce(unsigned short *a, const unsigned short *b, int len, unsigned short hi) 
{
	int		 i;
	long	 lAccum		= 0x0L
			,lBorrow	= 0x0L;

	while (1) {
		if (hi == 0) {
			for (i = len-1; i >= 0; i--) {
				if (a[i] < b[i]) 
					return;
				if (a[i] > b[i])
					break;
			}
		}

		for (i = 0; i < len; i++) {
			lAccum	= (a[i] - lBorrow) - b[i];
//			a[i]	= (unsigned short)(lAccum & 0xFFFF);
			a[i]	= lAccum & 0xFFFF;								// mmiu - Removed cast
			if ( lAccum < 0L ) 
				lBorrow	= 1 ;
			else
				lBorrow	= 0 ;
		}
		hi = hi - (unsigned short) lBorrow ;
	}
}


/*
 *  Compute a = x^e mod p, where numbers are of size len (though a 
 *    must be a buffer of size 2*len) and the exponent is eBits long.
 *
 *  This function is optimized to use as little RAM as possible.
 *  Note that the destination cannot equal any of the parameters.
 */

void
Simple_ModExp( unsigned short *a, const unsigned short *x, unsigned short *e, const unsigned short *p, int len, int eBits)
{
	int i;

	for (i = 1; i < len; i++)
		a[i] = 0;
	a[0] = 1;

	for (i = eBits-1; i >= 0; i--) {
		/* compute a = a^2 mod p */
		Simple_ModMult(a, a, a, p, len);

		/* If exponent bit is set, compute a = a * b mod p. */
		if ((e[i/16] >> (i % 16)) & 1)
			Simple_ModMult(a, a, x, p, len);
	}
}


void
Simple_ModMult(unsigned short *product, unsigned short *a, const unsigned short *b, const unsigned short *p, int len) 
{
	int		 i;
	unsigned short	scratch[2*KEY_BITS/16];		// 64 words

	Simple_Multiply(scratch, a, b, len);
	Simple_Reduce(scratch, p, len);
	for (i = 0; i < len; i++)
		product[i] = scratch[i];
}
