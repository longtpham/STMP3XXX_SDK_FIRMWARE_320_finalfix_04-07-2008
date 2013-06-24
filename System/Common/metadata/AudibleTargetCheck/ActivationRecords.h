/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/AudibleTargetCheck/rcs/ActivationRecords.h 1.2 2005/01/13 00:58:01Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 SigmaTel_v1.0_RC2 SigmaTel_v1.0_RC1 SigmaTel_Beta3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



#ifndef  __ACTIVATIONRECORDS_H
#define  __ACTIVATIONRECORDS_H

#ifdef __cplusplus
extern "C" {
#endif

#define SIZEOF_DIGITAL_SIGN       40

typedef struct _CAudibleDRMRecord

{ long int  newgid;
  short int gidindex;
  long int  verno;
  unsigned char playid[AUDIBLE_PLAYER_ID_SIZE];
  char ds[SIZEOF_DIGITAL_SIGN];
} CAudibleDRMRecord;


void ConvertActivePacketToGIDStruct( const CAudibleDRMRecordData *pDRMData, CAudibleDRMRecord *pGIDStruct );
void ConvertGIDStructToActivePacket( const CAudibleDRMRecord *pGIDStruct, CAudibleDRMRecordData *pDRMData );

#ifdef __cplusplus
}      // Closing of external "C"
#endif


#endif // __ACTIVATIONRECORDS_H

