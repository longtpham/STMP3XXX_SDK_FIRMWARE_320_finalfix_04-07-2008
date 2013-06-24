/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/MetaFillAudibleFileInfo.c 1.5 2005/03/03 05:07:37Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



//  This file has code to fill public CAudibleFileInfo from internal CAudibleAAInternal

#include "MetaFillAudibleFileInfoDefines.h"
#include "AAFileAccess.h"
#include "CoreEncryption.h"
#include "CircularBuffer.h"
#include "IAACodec.h"
#include "AAFileInternalCommon.h"
#include "MetaFillAudibleFileInfo.h"
#include "AudCStandFunctions.h"
#include "AAFileMiscHelpers.h"


// ***************************************************************************
// DUPLICATED ROUTINES START
// ***************************************************************************

#include "AAFileMiscHelpers.c"
#include "CoreEncryption.c"

static int Audible_IsDigit( char value )

{ return value >= 0x30 && value <= 0x39;
}

static unsigned long Audible_atol( const char *lpszNumber )

{ unsigned long retVal = 0;
  for( ; Audible_IsDigit( *lpszNumber ); lpszNumber++ )
  { retVal = retVal * 10 + ( *lpszNumber - 0x30 );
  }
  return retVal;
}

// ***************************************************************************
// DUPLICATED ROUTINES END
// ***************************************************************************



/*---------------------------------------------------------------------------*\
	Name:	DecryptAudibleHeader

	Description:
		This is a slightly modified version of
		CAudibleAAInternal_DecryptAudibleHeader where the encryption strings
		from the AA file header are passed in instead of converted here.
\*---------------------------------------------------------------------------*/

BOOL DecryptAudibleHeader( CAudibleAAInternal *pThis, CAudibleEncryptionStrings *pEncStr )

{ if ( pThis->audCommonHeader.versionNumber == AUDIBLE_COMHEAD_NOT_ENCRYPTED )
  { return TRUE;
  }
//-------------
  {
  //-------------  Now get numbers from strings.
    { unsigned long  seed      = Audible_atol( pEncStr->sz_EncryptionSeed );
      unsigned short encBlocks = (unsigned short)Audible_atol( pEncStr->sz_EncryptedBlocks );
    //------------- Version of hash table. Once we read it from header, we will init it here. 
      unsigned short hashTableVersion = 0;
      char *lpszCurNumInKey = pEncStr->sz_EncryptionKey;
      pThis->audioACS.m_ScramblingKey.a = Audible_atol( lpszCurNumInKey );
      while( *lpszCurNumInKey++ != ' '  ) {;}
      pThis->audioACS.m_ScramblingKey.b = Audible_atol( lpszCurNumInKey );
      while( *lpszCurNumInKey++ != ' '  ) {;}
      pThis->audioACS.m_ScramblingKey.c = Audible_atol( lpszCurNumInKey );
      while( *lpszCurNumInKey++ != ' '  ) {;}
      pThis->audioACS.m_ScramblingKey.d = Audible_atol( lpszCurNumInKey );
    //------------- Put it all into stream. Declare array and writes all to it. 
      { unsigned char szData[128];
        CAudOutputBuffer outBuf;
        IAudibleOutputStream *pOutStr = (IAudibleOutputStream *)&outBuf;
        CAudOutputBuffer_CAudOutputBuffer( &outBuf, szData, sizeof( szData ) );
        WriteShort(pOutStr, hashTableVersion  );
        WriteLong( pOutStr, pThis->audioACS.m_ScramblingKey.a );
        WriteLong( pOutStr, pThis->audioACS.m_ScramblingKey.b );
        WriteLong( pOutStr, pThis->audioACS.m_ScramblingKey.c );
        WriteLong( pOutStr, pThis->audioACS.m_ScramblingKey.d );
        WriteShort( pOutStr,encBlocks );
      //-------------- Array of player IDs and group IDs
        { int i = 0;
          for ( i = 0; i < pThis->audPlayerIDList.idCount; i++ )
          { WriteLong( pOutStr, pThis->audPlayerIDList.pIDsArray[i] );
          }
        }
        { int i = 0;
          for ( i = 0; i < pThis->audGroupIDList.idCount; i++ )
          { WriteLong( pOutStr, pThis->audGroupIDList.pIDsArray[i] );
          }
        }

      // **************** Decrypts the stream. ****************
        { CEncryptionKey encKey;
          encKey.a = 0x77214d4b; encKey.b = 0x196a87cd;
          encKey.c = 0x520045fd; encKey.d = 0x2a51d673;
          EncryptInChainMode( &encKey, szData, pOutStr->pVTable->GetCurrentPos( pOutStr ), seed ); 

        // **************** Read all data back from stream. ****************
          { CAudInputBuffer inBuf;  
            IAudibleInputStream *pInStream = (IAudibleInputStream *)&inBuf;
            CAudInputBuffer_CAudInputBuffer( &inBuf, szData, sizeof( szData ) );
            ReadUShort(pInStream, &hashTableVersion  );
            ReadULong( pInStream, &pThis->audioACS.m_ScramblingKey.a );
            ReadULong( pInStream, &pThis->audioACS.m_ScramblingKey.b );
            ReadULong( pInStream, &pThis->audioACS.m_ScramblingKey.c );
            ReadULong( pInStream, &pThis->audioACS.m_ScramblingKey.d );
            ReadUShort(pInStream, &encBlocks );
            { int i = 0;
              for ( i = 0; i < pThis->audPlayerIDList.idCount; i++ )
              { ReadULong( pInStream, &pThis->audPlayerIDList.pIDsArray[i] );
              }
            }
            { int i = 0;
              for ( i = 0; i < pThis->audGroupIDList.idCount; i++ )
              { ReadULong( pInStream, &pThis->audGroupIDList.pIDsArray[i] );
              }
            }
            return TRUE;
          }
        }
      }
    }
  }
}



/*---------------------------------------------------------------------------*\
	Description:
		Main entry point used to be for the SigmaTel code resource to fill
		in metadata from internal data. It is now used solely to decrypt
		the AA file header info.
\*---------------------------------------------------------------------------*/

AudErrorCode FillAudibleFileInfo(int pAAInternal, int unused, CAudibleEncryptionStrings *pEncStr)
{
  // 2/17/2005 mmiu - Filling the fileInfo structure doesn't require decrypting the header
  //-------- Decrypt header.
//  if ( pFileInfoArgs->flags & AUD_FILE_DECRYPT_HEADER_FLAG )

  // 2/28/2005 mmiu -	Crap. Actually, I had it backwards. When filling in the
  //					fileInfo structure and doing a targeting check, always
  //					decrypt the header.
  DecryptAudibleHeader( (CAudibleAAInternal *) pAAInternal, pEncStr );

  return AUD_NO_ERROR;
}

