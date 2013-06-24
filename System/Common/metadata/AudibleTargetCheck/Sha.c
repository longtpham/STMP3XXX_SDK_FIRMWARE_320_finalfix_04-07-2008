/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/Sha.c 1.2 2005/01/13 01:02:51Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 SigmaTel_Beta3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



//#include "stdafx.h"

/*
 * SHS.C:  Reference implementation of NIST FIPS PUB 180-1.
 *   (Secure Hash Standard, revised version).
 *   Copyright 1994 by Paul C. Kocher.  All rights reserved.
 *   This code is provided without warranty of any kind.
 *   =

 */

#define UINT32	unsigned long


#include "Sha.h"

typedef struct {
  unsigned long H[5];
  unsigned long W[80];
  int lenW;
  unsigned long sizeHi,sizeLo;
} SHS_CTX;

SHS_CTX g_ctx;


void shsHashBlock(SHS_CTX *ctx);

void shsInit(SHS_CTX *ctx) {
  ctx->lenW = 0;
  ctx->sizeHi = ctx->sizeLo = 0;

  /* Initialize H with the magic constants (see FIPS180 for constants)
   */

  ctx->H[0] = 0x67452301L;
  ctx->H[1] = 0xefcdab89L;
  ctx->H[2] = 0x98badcfeL;
  ctx->H[3] = 0x10325476L;
  ctx->H[4] = 0xc3d2e1f0L;

}


void shsUpdate(SHS_CTX *ctx, unsigned char *dataIn, int len) {
  int i;

  /* Read the data into W and process blocks as they get full
   */
  for (i = 0; i < len; i++) {
    ctx->W[ctx->lenW / 4] <<= 8;
    ctx->W[ctx->lenW / 4] |= (UINT32)dataIn[i];
	ctx->W[ctx->lenW / 4] &= 0xFFFFFFFF;			// mmiu - Added mask
    if ((++ctx->lenW) % 64 == 0) {
      shsHashBlock(ctx);
      ctx->lenW = 0;
    }
    ctx->sizeLo += 8;
    ctx->sizeHi += (ctx->sizeLo < 8);
  }
}


void shsFinal(SHS_CTX *ctx, unsigned char *hashout) {
  unsigned char pad0x80 = 0x80;
  unsigned char pad0x00 = 0x00;
  unsigned char padlen[8];
  int i;

  /* Pad with a binary 1 (e.g. 0x80), then zeroes, then length
   */
  padlen[0] = (unsigned char)((ctx->sizeHi >> 24) & 255);
  padlen[1] = (unsigned char)((ctx->sizeHi >> 16) & 255);
  padlen[2] = (unsigned char)((ctx->sizeHi >> 8) & 255);
  padlen[3] = (unsigned char)((ctx->sizeHi >> 0) & 255);
  padlen[4] = (unsigned char)((ctx->sizeLo >> 24) & 255);
  padlen[5] = (unsigned char)((ctx->sizeLo >> 16) & 255);
  padlen[6] = (unsigned char)((ctx->sizeLo >> 8) & 255);
  padlen[7] = (unsigned char)((ctx->sizeLo >> 0) & 255);
  shsUpdate(ctx, &pad0x80, 1);
  while (ctx->lenW != 56)
    shsUpdate(ctx, &pad0x00, 1);
  shsUpdate(ctx, padlen, 8);

  /* Output hash
   */
  for (i = 0; i < 20; i++) {
    hashout[i] = (unsigned char)((ctx->H[i / 4] >> 24) & 0xFF);			// mmiu - Added mask
    ctx->H[i / 4] <<= 8;
  }
  shsInit(ctx);
}


void shsBlock(unsigned char *dataIn, int len, unsigned char *dataOut) {
//  SHS_CTX ctx;

  shsInit(&g_ctx);
  shsUpdate(&g_ctx, dataIn, len);
  shsFinal(&g_ctx, dataOut);
}


// mmiu - Added mask
#define SHS_ROTL(X,n) ((((X) << (n)) | ((X) >> (32-(n)))) & 0xFFFFFFFF)

void shsHashBlock(SHS_CTX *ctx) {
  int t;
  UINT32 A,B,C,D,E,TEMP;

  for (t = 16; t <= 79; t++)
    ctx->W[t] =
      SHS_ROTL(ctx->W[t-3] ^ ctx->W[t-8] ^ ctx->W[t-14] ^ ctx->W[t-16], 1);

  A = ctx->H[0];
  B = ctx->H[1];
  C = ctx->H[2];
  D = ctx->H[3];
  E = ctx->H[4];

  for (t = 0; t <= 19; t++) {
    TEMP = SHS_ROTL(A,5) + (((C^D)&B)^D) + E + ctx->W[t] + 0x5a827999L;
	TEMP &= 0xFFFFFFFF;			// mmiu - Added mask

    E = D;
    D = C;
    C = SHS_ROTL(B, 30);
    B = A;
    A = TEMP;
  }
  for (t = 20; t <= 39; t++) {
    TEMP = SHS_ROTL(A,5) + (B^C^D) + E + ctx->W[t] + 0x6ed9eba1L;
	TEMP &= 0xFFFFFFFF;			// mmiu - Added mask
    E = D;
    D = C;
    C = SHS_ROTL(B, 30);
    B = A;
    A = TEMP;
  }
  for (t = 40; t <= 59; t++) {
    TEMP = SHS_ROTL(A,5) + ((B&C)|(D&(B|C))) + E + ctx->W[t] + 0x8f1bbcdcL;
	TEMP &= 0xFFFFFFFF;			// mmiu - Added mask
    E = D;
    D = C;
    C = SHS_ROTL(B, 30);
    B = A;
    A = TEMP;
  }
  for (t = 60; t <= 79; t++) {
    TEMP = SHS_ROTL(A,5) + (B^C^D) + E + ctx->W[t] + 0xca62c1d6L;
	TEMP &= 0xFFFFFFFF;			// mmiu - Added mask
    E = D;
    D = C;
    C = SHS_ROTL(B, 30);
    B = A;
    A = TEMP;
  }

  ctx->H[0] = (ctx->H[0] + A) & 0xFFFFFFFF;
  ctx->H[1] = (ctx->H[1] + B) & 0xFFFFFFFF;
  ctx->H[2] = (ctx->H[2] + C) & 0xFFFFFFFF;
  ctx->H[3] = (ctx->H[3] + D) & 0xFFFFFFFF;
  ctx->H[4] = (ctx->H[4] + E) & 0xFFFFFFFF;
/*
  ctx->H[0] += A;
  ctx->H[1] += B;
  ctx->H[2] += C;
  ctx->H[3] += D;
  ctx->H[4] += E;
*/
}


