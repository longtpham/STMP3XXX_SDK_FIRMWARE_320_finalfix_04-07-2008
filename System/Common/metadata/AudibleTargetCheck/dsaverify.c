/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/dsaverify.c 1.4 2005/02/25 00:56:39Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/


#include "menumanager.h"
#include "resource.h"

#include "dsaverify.h"
//New scheme - code from Ron.
#include "ronsdsa.h"
#include "sha.h"
#include "AudCStandFunctions.h"


static const unsigned char stm_LightSignKey [] = { 0xb5, 0xb4, 0xe1, 0xfc, 0x21, 0x24, 0x4e, 0x23, 0x96, 0xfa, 0x8e, 0x19, 0xb2, 0xcd, 0x73, 0x2f };
static const unsigned char stm_VersionBlock [] = { 0x87, 0xf1, 0x99, 0xb4, 0xc7, 0xfe, 0xca, 0xa6 };


//******************************************************************************
// Duplicated routines
//******************************************************************************

/*
static void *Audible_memcpy( void *pDestinationBuf, const void *pSourceBuf, size_t count )

{//---------------- Copy byte by byte
  unsigned char *pCharDest = (unsigned char *)pDestinationBuf;
  unsigned char *pCharSrc  = (unsigned char *)pSourceBuf;
//  long i = 0;
//  for ( i = 0; i < count; i++ )
  while ( count-- )
  { *pCharDest++ = *pCharSrc++;
  }
  return pDestinationBuf;
}
*/

static int Audible_memcmp( const void *pBuf1, const void *pBuf2, size_t count )

{ while ( count-- )
  {
    if ( *(unsigned char *) pBuf1 != *(unsigned char *) pBuf2 )
    { if ( *(unsigned char *) pBuf1 < *(unsigned char *) pBuf2 )
      { return -1;
	  }
	  return 1;
	}
	((unsigned char *) pBuf1)++;
	((unsigned char *) pBuf2)++;
  }
  return 0;
}

//******************************************************************************

static void AAGenerateLightSignature( const void *hash, void *rawsig, unsigned char LightSignHash[SHA_HASH_OUTPUT_SIZE] )

{//----- The rule is - sha of 20 bytes hash + + stm_LightSignKey + first 20 bytes of signature == 20 last bytes of signature
  unsigned char szToHash[20 + sizeof( stm_LightSignKey ) + 20 ];
//  const unsigned char *pByteRawSig = rawsig;
//------------ Copies version block.  
  Audible_memcpy( rawsig, stm_VersionBlock, sizeof( stm_VersionBlock ) );
//------------ Copies hash bytes.
  Audible_memcpy( szToHash,                                   hash,             20 );
  Audible_memcpy( szToHash + 20,                              stm_LightSignKey, sizeof( stm_LightSignKey ) ); 
  Audible_memcpy( szToHash + 20 + sizeof( stm_LightSignKey ), rawsig,           20 );
  shsBlock( szToHash, sizeof( szToHash ), LightSignHash );
}

static int AAVerifyLightSignature( void *hash, void *rawsig )

{//---- Check the version.
  if ( 0 != Audible_memcmp( rawsig, stm_VersionBlock, sizeof( stm_VersionBlock ) ) )
  { return -1;
  }
//----- The rule is - sha of 20 bytes hash + + stm_LightSignKey + first 20 bytes of signature == 20 last bytes of signature
  { unsigned char szHash[20];
    unsigned char *pByteRawSig = rawsig;
    AAGenerateLightSignature( hash, rawsig, szHash );
    return 0 == Audible_memcmp( szHash, pByteRawSig + 20, 20 );
  }
}

int AudibleValidAndGenerateLightSign( void *hash, void *rawsig )

{//----- First we verify existing signature. If it is good - generate new one.
  if ( !AASmallVerify( hash, rawsig, metacommandsign ) )
  { return 0;
  }
//----------- Now generate one. 
  { unsigned char szHash[SHA_HASH_OUTPUT_SIZE];
    unsigned char *pRandom = szHash;
    unsigned char *pByteRawSig = rawsig;
  //----------- Init array of hash to kind of random value  
    shsBlock( (unsigned char *)&pRandom, sizeof( pRandom ), szHash );
  //----------- 20 first bytes of signature are now random. 
    Audible_memcpy( rawsig, szHash, SHA_HASH_OUTPUT_SIZE );
    AAGenerateLightSignature( hash, rawsig, pByteRawSig + SHA_HASH_OUTPUT_SIZE );
    return 1;
  }
}

int AASmallVerify( void *hash, void *rawsig, KeyType ktype )

{//------------ First we check type of the signature. Original one or later invented light one.
//  We know that hash has size 20 bytes, signature 40 bytes.
//  Run light signature verification. If it fails - then run DSA
  { int resLight = AAVerifyLightSignature( hash, rawsig );
    switch ( resLight )
    { case 1 : return 1;
      case 0 : return 0;
  //--------- -1 means another version, most probably DSA   
      default : break;
    }
  }

//------------- Choose one of keys	
  { const unsigned short *arr = 0;
	  switch (ktype)
	  { case metacommandsign  : arr = metacommandsign_public_key; break;
  //---------- Only three keys are supported, unknown key	  
	    default: return 0;
	  }
  //------------- Copy from continous bitstream onto array of short int values. 
  //        Values in this array will be the same on different CPUs	
    { unsigned short arrayHash[10], arraySignature[20];  
      { int i = 0;
        for ( i = 0; i < 10; i++ )
        { unsigned char *pHash = (unsigned char *)hash;
          unsigned short lowByte  = pHash[2*i] & 0xFF;
          unsigned short highByte = pHash[2*i+1];
          arrayHash[i] = lowByte | ( ( highByte << 8 ) & 0xFF00 );
        }
      }
      { int i = 0;
        for ( i = 0; i < 20; i++ )
        { unsigned char *pSignature = (unsigned char *)rawsig;
          unsigned short lowByte  = pSignature[2*i] & 0xFF;
          unsigned short highByte = pSignature[2*i+1];
          arraySignature[i] = lowByte | ( ( highByte << 8 ) & 0xFF00 );
        }
      }
    //---------- Now calls actual validation.    
      { //int retval = RonsSimpleDSAVerify( arrayHash, arraySignature, arr );
	    int retval = SysCallFunction( RSRC_AUDIBLE_DSA_P, RonsSimpleDSAVerifyEntry, (INT)arrayHash, (INT)arraySignature, (INT *)arr );
        return retval;
      }
    }
  }
}



