/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/CheckTargeting.c 1.3 2005/02/19 00:43:04Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



#include "CheckTargeting.h"
#include "AAFileMiscHelpers.h"
#include "AudCStandFunctions.h"
#include "FillAudibleFileInfo.h"
#include "AAOpenFile.h"
#include "ActivationRecords.h"
#include "sha.h"
#include "dsaverify.h"


//******************************************************************************
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
CAudibleDRMRecord drmRecord;
unsigned char bufferHash[SHA_HASH_OUTPUT_SIZE];

_reentrant AudErrorCode
InternalAACheckTargeting(CAudibleAAInternal *pInternalAA, CAudibleDRMRecordData *pRecords,
                         short RecordCount, unsigned char playerID[AUDIBLE_PLAYER_ID_SIZE])
{
//  CAudibleDRMRecord drmRecord;
  AudErrorCode  errCode = AUD_NO_ERROR;
  int           bAuthorised;
  int           i, j;

  //------------ If idCount is zero - files is not targeted, anybody may open it. 
  bAuthorised = pInternalAA->audGroupIDList.idCount == 0;
  for ( i = 0; i < RecordCount; i++ )
  {
    ConvertActivePacketToGIDStruct( pRecords + i, &drmRecord );

    for ( j = 0; j < pInternalAA->audGroupIDList.idCount; j++ )
    {//------------- If grooup ID in activation record and in file are the same - file authorized. 
      if ( drmRecord.newgid == pInternalAA->audGroupIDList.pIDsArray[j] &&
          0 == Audible_memcmp( drmRecord.playid, playerID, AUDIBLE_PLAYER_ID_SIZE )                                                                )
      { //------------ If there is a match between group ID in record and file - 
        //             then we check if activation record is valid itself. 
//        unsigned char bufferHash[SHA_HASH_OUTPUT_SIZE];
//------------ Make hash and check signature.
        shsBlock
        ( pRecords[i].szData, 
          ACTIVATION_PACKET_RAW_SIZE - SIZEOF_DIGITAL_SIGN, 
          bufferHash 
        );
//---------- Now checks the hash with signature. If this check fails - means that it is not valid records from Audible.        
        if ( !AASmallVerify( bufferHash, drmRecord.ds, metacommandsign ) )
        { errCode = AUD_ACTIVATION_RECORDS_SIG_FAIL;
          goto FINAL_CLEANUP; 
        }    
//------------ Match found - file authorised.          
        bAuthorised = 1;
        goto FINAL_CLEANUP;
      }
    }
  }
//------------- If no match - means file should not be opened with these activation records.  
FINAL_CLEANUP : 
  if ( !bAuthorised )
  {//------------ If other error is not set - set this error. 
    if ( errCode == AUD_NO_ERROR )
    { errCode = AUD_DEVICE_NOT_ACTIVATED_FOR_THIS_FILE;
    }
  }

  return errCode;
}



_reentrant AudErrorCode
AACheckTargeting( INT pInternalAA, INT pDRMArgs, INT *playerID )
{
	CAudibleAAInternal		*pArgInternalAA = (CAudibleAAInternal *) pInternalAA;
	CAudibleDRMRecordData	*pArgRecords = ((AudibleDRMArgs_t *) pDRMArgs)->pRecords;
	short					argRecordCount = ((AudibleDRMArgs_t *) pDRMArgs)->RecordCount;
	unsigned char			*pPlayerID = (unsigned char *) playerID;

	return InternalAACheckTargeting( pArgInternalAA, pArgRecords, argRecordCount, pPlayerID );

// 12/3/2004 mmiu - I have to use the above code or else the wrong registers are used to call InternalAACheckTargeting?!
//	return InternalAACheckTargeting( (CAudibleAAInternal *) pInternalAA, ((AudibleDRMArgs_t *) pDRMArgs)->pRecords,
//										((AudibleDRMArgs_t *) pDRMArgs)->RecordCount, (unsigned char *) playerID );
}
