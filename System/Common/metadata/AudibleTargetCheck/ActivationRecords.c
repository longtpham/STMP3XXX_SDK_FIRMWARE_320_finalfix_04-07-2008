/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/ActivationRecords.c 1.4 2005/02/25 00:56:24Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



#include "AAFileAccess.h"
#include "AudCStandFunctions.h"
#include "ActivationRecords.h"

// ****************************************************************************************************
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


// ****************************************************************************************************
static unsigned long ReadLongFromBuf( const unsigned char *pData )

{ unsigned long first  = pData[0];
  unsigned long second = pData[1];
  unsigned long third  = pData[2];
  unsigned long fourth = pData[3];
  return (first << 24) | (second << 16) | (third << 8) | fourth;
}

static void WriteLongFromBuf( unsigned long value, unsigned char *pData )

{ pData[0] = (unsigned char)((value & 0xFF000000) >> 24);
  pData[1] = (unsigned char)((value & 0xFF0000) >> 16);
  pData[2] = (unsigned char)((value & 0xFF00) >> 8);
  pData[3] = (unsigned char)( value & 0xFF);
}

static unsigned short ReadShortFromBuf( const unsigned char *pData )

{ unsigned short first  = pData[0];
  unsigned short second = pData[1];
  return (first << 8) | second;
}

static void WriteShortFromBuf( unsigned short value, unsigned char *pData )

{ pData[0] = (value & 0xFF00) >> 8;
  pData[1] =  value & 0xFF;
}

//--------- Convertion from activation packet from raw data to Audible_gidswbytestream structure
//--------- iTunesAudibleAPI.h required for this header.
#define SERVER_ACTIVATION_PACKET_SIZE         70
#define SERVER_ACTIVATION_GROUPID_SIZE        4
#define SERVER_ACTIVATION_INDEX_SIZE          2
#define SERVER_ACTIVATION_VERSION_SIZE        4
#define SERVER_ACTIVATION_PLAYERID_SIZE       AUDIBLE_PLAYER_ID_SIZE // 20
#define SERVER_ACTIVATION_DIGITAL_SIGN_SIZE   SIZEOF_DIGITAL_SIGN


void ConvertActivePacketToGIDStruct( const CAudibleDRMRecordData *pDRMData, CAudibleDRMRecord *pGIDStruct )

{ const unsigned char *pData = pDRMData->szData;
  pGIDStruct->newgid   = ReadLongFromBuf( pData );                             pData += SERVER_ACTIVATION_GROUPID_SIZE;
  pGIDStruct->gidindex = ReadShortFromBuf( pData );                            pData += SERVER_ACTIVATION_INDEX_SIZE;
  pGIDStruct->verno    = ReadLongFromBuf( pData );                             pData += SERVER_ACTIVATION_VERSION_SIZE;
  Audible_memcpy( pGIDStruct->playid, pData, SERVER_ACTIVATION_PLAYERID_SIZE ); pData += SERVER_ACTIVATION_PLAYERID_SIZE;
  Audible_memcpy( pGIDStruct->ds, pData, SERVER_ACTIVATION_DIGITAL_SIGN_SIZE ); 
}


// 2/3/2005 mmiu - Unused function
/*
void ConvertGIDStructToActivePacket( const CAudibleDRMRecord *pGIDStruct, CAudibleDRMRecordData *pDRMData )

{ unsigned char *pData = pDRMData->szData;                     
  WriteLongFromBuf(  pGIDStruct->newgid,   pData );                             pData += SERVER_ACTIVATION_GROUPID_SIZE; 
  WriteShortFromBuf( pGIDStruct->gidindex, pData );                             pData += SERVER_ACTIVATION_INDEX_SIZE;   
  WriteLongFromBuf( pGIDStruct->verno,     pData );                             pData += SERVER_ACTIVATION_VERSION_SIZE; 
  Audible_memcpy( pData, pGIDStruct->playid, SERVER_ACTIVATION_PLAYERID_SIZE ); pData += SERVER_ACTIVATION_PLAYERID_SIZE;
  Audible_memcpy( pData, pGIDStruct->ds, SERVER_ACTIVATION_DIGITAL_SIGN_SIZE ); 
}
*/
