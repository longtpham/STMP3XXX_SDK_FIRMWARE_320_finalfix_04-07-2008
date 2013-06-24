/* $Header: Z:/projects/Audible/SigmaTel/AudibleOEM/STMP3XXX_SDK_PLAYER_FIRMWARE/System/common/metadata/rcs/AAMetadataOverlay.c 1.5 2005/03/03 04:30:29Z mmiu Exp $ */
/* $Name: SigmaTel_v1.0_RC4 SigmaTel_v1.0_RC3 $ */
/******************************************************************************
*                                                                             *
*                (C) Copyright Audible Inc. (1997-2005)                       *
*                         All Rights Reserved                                 *
*    This material is confidential and shall remain as such. Any unauthorized *
*   use, distribution, reproduction or storage of this material or any part   *
*   thereof is strictly prohibited.                                           *
******************************************************************************/



/*
	File:	AAMetadataOverlay.c

	Description:
		This file contains all the routines necessary for reading metadata
		from an Audible Audio (.AA) file. The intent of this routine is to
		create a code overlay to read metadata in embedded systems with tight
		memory constraints.
		
		The entry point is AudibleAA_GetMetadataOverlay() and is called with
		the same arguments as AudibleAA_GetMetadata() and
		AudibleAA_GetMetadataEmbedded() in the monolithic library.

*/

#include "AAMetadataOverlay.h"
/*
#include "AAFileAccess.h"
#include "CircularBuffer.h"
#include "CoreEncryption.h"
#include "IAACodec.h"
#include "AAFileInternalCommon.h"
*/
//#include "FillAudibleFileInfo.h"
//#include "AAOpenFile.h"
#include "AudCStandFunctions.h"


//
// These C files are included to create a single source file from the
// compiler's point-of-view. This helps on the SigmaTel platform because it
// makes it easier to locate this overlay.
//
#include "InputStreamImplCStand.c"
#include "AudCStandAllocation.c"
#include "AAFileMiscHelpers.c"
//#include "FillAudibleFileInfo.c"


// Stub for linkage
void  CircularBuffer_Destructor( CircularBuffer * p )
{
}

/*
void *Audible_memcpy( void *pDestinationBuf, const void *pSourceBuf, size_t count )

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

void *Audible_memset( void *pBuffer, int value, size_t size )

{
  unsigned char *pStartBuffer = (unsigned char *)pBuffer;
  unsigned char *pEndBuffer   = pStartBuffer + size;
  unsigned char chVal = value && 0xFF;
  while ( --pEndBuffer >= pStartBuffer )
  { *pEndBuffer = chVal;
  } 
  return pBuffer;
}

int Audible_strncmp( const char *pOneString, const char *pSecondString, size_t count )

{ size_t i;
  for( i = 0; *pOneString != 0  &&  *pSecondString != 0 && i < count; pOneString++, pSecondString++, i++ )
  { if ( *pOneString < *pSecondString )
    { return -1;
    }
    if ( *pOneString > *pSecondString )
    { return 1;
    }
  }
//--------------- Finished loop. If both are at the end - then equal  
  if ( ( *pOneString == 0  &&  *pSecondString == 0 ) || ( i == count && *pSecondString == 0) )
  { return 0;
  }
 //------------ If first string is at the end - it is larger
 if ( i == count )
  { return 1;
  }
  return *pOneString == 0 ? 1 : -1;
}

char *Audible_strncpy( char *pDestString, const char *pSourceString, size_t count )

{
  char *p = pDestString;
  while ( count-- )
  { *p++ = *pSourceString++;
  }
  return pDestString;
}
*/

static AudErrorCode MDValidateAAFileHeader( IAudibleInputStream *pFile) 

{//--------- Position is already on zero. 
//--  Read file size 
  unsigned long fileSize;
  if ( !ReadULong( pFile, &fileSize ) )      { return AUD_FILE_READ_FAIL; };
//--  Read Magic number. 
  { unsigned long magicNumber;
    if ( !ReadULong( pFile, &magicNumber ) )   { return AUD_FILE_READ_FAIL; }
  //--   Compare magic number and file size with actual one.
    { unsigned long actualfileSize = pFile->pVTable->GetFileSize( pFile );
      if ( actualfileSize != fileSize )
      { return AUD_AA_FILE_TRUNCATED;
      }
      if ( magicNumber != 0x57907536 )
      { return AUD_NOT_AUDIBLE_FILE;
      }
      return AUD_NO_ERROR; 
    }
  }  
}

static AudErrorCode MDReadAAFileSections( IAudibleInputStream *pFile, CAudibleAAInternal *pAudData ) 

{//-- Read sections. We do not read sections with ID more than MAX_SECTION_COUNT. 
//   We also do not read more than MAX_SECTION_COUNT sections
//  sectionCount = sectionCount < MAX_SECTION_COUNT < sectionCount
//--  Read number of entires in section table.
  unsigned long sectionCount; 
  if ( !ReadULong( pFile, &sectionCount ) )  { return AUD_FILE_READ_FAIL; }
//--  Read DUMMY, This way we skip 4 bytes. Use scope to get rid of extra local var.
  { unsigned long dummy;
    ReadULong( pFile, &dummy );
  }
  { unsigned short i;
    for ( i = 0; i < sectionCount; i++ )
    { unsigned long sectionID = 0;
      unsigned long sectionStarts = 0;
      unsigned long sectionLenghts   = 0;
      BOOL errSectionEntry = 
      ReadULong( pFile, &sectionID ) && ReadULong( pFile, &sectionStarts ) && ReadULong( pFile, &sectionLenghts );
      if ( !errSectionEntry )
      { return AUD_FILE_READ_FAIL;
      }
  //---- Put section ID if it fits into MAX_SECTION_COUNT
      if ( sectionID > 0 && sectionID <  MAX_SECTION_COUNT )
      { pAudData->audSections[sectionID].sectionID    = sectionID;
        pAudData->audSections[sectionID].sectionStart = sectionStarts;
        pAudData->audSections[sectionID].sectionLength   = sectionLenghts;
      }
    }
    return AUD_NO_ERROR; 
  }
}

static AudErrorCode MDReadAudibleCommonHeader( IAudibleInputStream *pFile, CAudibleCommonHeader *pCommonHead, long sectionSize )

{ if ( sectionSize != 24 )
  { return AUD_AA_FILE_BAD_FORMAT;
  }
//-------------- Now read. 
  { BOOL readRes =  
      ReadShort( pFile, &pCommonHead->versionNumber    ) && 
      ReadShort( pFile, &pCommonHead->minVersionNumber ) &&
      ReadLong ( pFile, &pCommonHead->magicNumber      ) &&
  //------------ Read information if this is partial file from larger one.   
      ReadLong( pFile, &pCommonHead->IsPartialFile )     &&
      ReadLong( pFile, &pCommonHead->sizeOfOriginalAudio ) &&
      ReadLong( pFile, &pCommonHead->segmentStart )      &&
      ReadLong( pFile, &pCommonHead->segmentEnd );       
  //----------------  
    if ( !readRes )
    { return AUD_FILE_READ_FAIL;
    }
    if ( pCommonHead->magicNumber != 0x57907536 || pCommonHead->minVersionNumber != 0 )
    { return AUD_AA_FILE_BAD_FORMAT;
    }
  //----------- If version more than 1 - means some new future format
    if ( pCommonHead->versionNumber > 1 || pCommonHead->versionNumber < 0 )
    { return AUD_AA_FILE_UNSOPPORTED_FORMAT;
    }
    return AUD_NO_ERROR;
  }
}

static AudErrorCode MDReadAudibleOptionalHeader( IAudibleInputStream *pFile, CAudibleOptionalHeader *pOptonalHead, long sectionSize )

{ pOptonalHead->dataSize = sectionSize - 4;
  pOptonalHead->pValueNamesData = (unsigned char *)Audible_malloc( pOptonalHead->dataSize );
//------------- Allocates memory for value-name pairs  
  if ( pOptonalHead->pValueNamesData == NULL )
  { return AUD_MEM_ALLOC_FAILED;
  }
//------------- Memory allocated - read from the file.  
  { BOOL readRes = ReadLong( pFile, &pOptonalHead->valueCount ) &&
                  AUD_NO_ERROR == pFile->pVTable->Read( pFile, pOptonalHead->pValueNamesData, pOptonalHead->dataSize, NULL );
    if ( !readRes )
    { return AUD_FILE_READ_FAIL;
    }
    return AUD_NO_ERROR;
  }
}

static AudErrorCode MDReadAudibleSpecificHeader( IAudibleInputStream *pFile, CAudibleSpecificHeader *pSpecificHead, long sectionSize )

{ BOOL readRes =  
    ReadUShort( pFile, &pSpecificHead->hashTableVersion )                                                             &&
    AUD_NO_ERROR == pFile->pVTable->Read( pFile, pSpecificHead->szTitleID,        sizeof( pSpecificHead->szTitleID ), NULL        ) &&
    AUD_NO_ERROR == pFile->pVTable->Read( pFile, pSpecificHead->digitalSignature, sizeof( pSpecificHead->digitalSignature ), NULL ) &&
    ReadULong( pFile, &pSpecificHead->audioSizeBytes )                                                                &&
    ReadULong( pFile, &pSpecificHead->audioTimeSeconds )                                                              &&
    ReadULong( pFile, &pSpecificHead->hashBlockSize    )                                                              &&
    ReadULong( pFile, &pSpecificHead->unused );
//----------------  
  if ( !readRes )
  { return AUD_FILE_READ_FAIL;
  }
  return AUD_NO_ERROR;
}

//CAudibleOneAudioChapter	tempChapter;
//long	dummy;
//static  char	unUsed[20];

static AudErrorCode MDReadAudibleTableOfContent( IAudibleInputStream *pFile, CAudibleAllChapters *pAudChapters, long sectionSize )

{
  int	i;

  //---- AA file consist of multiple sections, each section - multiple blocks.
  BOOL readRes = ReadULong( pFile, &pAudChapters->audioChapetsCount ); // Read count of audio chapters.
  if ( !readRes )
  { return AUD_FILE_READ_FAIL;
  }

  //---- Check for valid chapter count
  if ( pAudChapters->audioChapetsCount > AUDIO_CHAPTERS_MAX_COUNT )
  { return AUD_AA_FILE_BAD_FORMAT;
  } 

  //----------- Now allocate memory for audio chapters.
  pAudChapters->pAudioChap = (CAudibleOneAudioChapter *)Audible_malloc( pAudChapters->audioChapetsCount * sizeof( CAudibleOneAudioChapter ) );
  if ( pAudChapters->pAudioChap == NULL )
  { return AUD_MEM_ALLOC_FAILED;
  }

  //----------- Good, now iterate through all sections.
  for ( i = 0; i < pAudChapters->audioChapetsCount; i++ )
  { char unUsed[8];
    long entriesCount;
	CAudibleOneAudioChapter *p = &pAudChapters->pAudioChap[i];
    readRes = 
      ReadLong( pFile,  &p->blockCount )                                &&
      AUD_NO_ERROR == pFile->pVTable->Read( pFile, unUsed, sizeof( unUsed ), NULL )  &&
      ReadLong( pFile, &p->audioSize  )                                &&
      ReadLong( pFile,  &p->audioTimeSec )                              &&
      ReadUShort(pFile, &p->codecID )                                   &&
      ReadLong( pFile,  &entriesCount );
    if ( !readRes )
    { return AUD_FILE_READ_FAIL;
    }
	//---- For metadata, just assume one block
    p->pAudioBlockInfo = (CAudibleBlockInfo *)Audible_malloc( sizeof( CAudibleBlockInfo ) );	//p->audBlockInfo;
	if ( p->pAudioBlockInfo == NULL )
	  return AUD_MEM_ALLOC_FAILED;
    //--------------------- Now we skip time stamp table, as we know the bitrate. 
    { unsigned long curPos = pFile->pVTable->GetCurrentPos( pFile );
      curPos += entriesCount * 8;
      if ( AUD_NO_ERROR != pFile->pVTable->SetCurrentPos( pFile, curPos ) )
      { return AUD_FILE_SEEK_FAIL;
      }
    }   
  }

/*
  //---- Read one chapter to get the codecID. First skip some data we don't need right now...
  pAudChapters->pAudioChap = &tempChapter;
  if ( pFile->pVTable->Read( pFile, unUsed, 4*5, NULL ) != AUD_NO_ERROR )
    return AUD_FILE_READ_FAIL;
*/
/*
  pAudChapters->pAudioChap = &tempChapter;
  for ( i = 0; i < 5; i++ )
    if ( !ReadULong( pFile, &dummy ) )
    { return AUD_FILE_READ_FAIL;
    }
*/
/*
  //---- Read codec ID
  if ( !ReadUShort( pFile, &pAudChapters->pAudioChap[0].codecID ) )
  { return AUD_FILE_READ_FAIL;
  }
*/
/*
//----------- Now allocate memory for audio chapters.
//  pAudChapters->audioChapetsCount = minimum( AUDIO_CHAPTERS_MAX_COUNT, pAudChapters->audioChapetsCount );
  pAudChapters->pAudioChap = (CAudibleOneAudioChapter *)Audible_malloc( pAudChapters->audioChapetsCount * sizeof( CAudibleOneAudioChapter ) );
  if ( pAudChapters->pAudioChap == NULL )
  { return AUD_MEM_ALLOC_FAILED;
  }
//----------- Good, now iterates through all sections.
  { unsigned long i = 0;
    for ( i = 0; i < pAudChapters->audioChapetsCount; i++ )
    { short unUsed[4];
      long entriesCount = 0;
      readRes = 
        ReadLong( pFile,  &pAudChapters->pAudioChap[i].blockCount )                                &&
        AUD_NO_ERROR == pFile->pVTable->Read( pFile, unUsed, 2 * sizeof( unUsed ) / sizeof( unUsed[0] ), NULL )  &&
        ReadLong( pFile, &pAudChapters->pAudioChap[i].audioSize  )                                &&
        ReadLong( pFile,  &pAudChapters->pAudioChap[i].audioTimeSec )                              &&
        ReadUShort(pFile, &pAudChapters->pAudioChap[i].codecID )                                   &&
        ReadLong( pFile,  &entriesCount );
      if ( !readRes )
      { return AUD_FILE_READ_FAIL;
      }
  //---------------------  If count of blocks more than 4, we allocate memory dynamically
      if ( pAudChapters->pAudioChap[i].blockCount <= MAX_DEFAULD_AUDIOBLOCK_COUNT )
      { pAudChapters->pAudioChap[i].pAudioBlockInfo = pAudChapters->pAudioChap[i].audBlockInfo;
      }
      else
      { pAudChapters->pAudioChap[i].pAudioBlockInfo = 
          ( CAudibleBlockInfo *)Audible_malloc( pAudChapters->pAudioChap[i].blockCount * sizeof( CAudibleBlockInfo ) );
      }      
  //--------------------- Now we skip time stamp table, as we know the bitrate. 
  //    { unsigned long curPos = pFile->pVTable->GetCurrentPos( pFile );
  //      curPos += entriesCount * 8;
  //      if ( AUD_NO_ERROR != pFile->pVTable->SetCurrentPos( pFile, curPos ) )
  //      { return AUD_FILE_SEEK_FAIL;
  //      }
  //    }   
    }
    return AUD_NO_ERROR;
  }
*/

  return AUD_NO_ERROR;
}

AudErrorCode MDReadAudibleIDList( IAudibleInputStream *pFile, CAudibleGroupOrPlayer_IDList *pIDList, long sectionSize )

{ int i;
  long temp;
//  BOOL readRes = ReadLong( pFile, &pIDList->idCount );
  BOOL readRes = ReadLong( pFile, &temp );
//---------------- Fail to read - return error. 
  if ( !readRes )
  { return AUD_FILE_READ_FAIL;
  }
  pIDList->idCount = temp;
//---------------- Size of section should be equal to 4 + pIDList->idCount * 4
  if ( sectionSize != 4 + pIDList->idCount * 4 )
  { return AUD_AA_FILE_BAD_FORMAT;
  }  
//--------- If count is less than fits to embedded array - then we leave it inside, otherwise dynamically allocate  
  if ( pIDList->idCount < sizeof( pIDList->idList ) / sizeof( pIDList->idList[0] ) )
  { pIDList->pIDsArray = pIDList->idList;
  }
  else
  { pIDList->pIDsArray = (unsigned long *)Audible_malloc( pIDList->idCount );
    if ( pIDList->pIDsArray == NULL )
    { return AUD_MEM_ALLOC_FAILED;
    }
  }
//---------- Now read values one by one
  for ( i = 0; i < pIDList->idCount; i++ )
  { 
    //if ( !ReadULong( pFile, &pIDList->pIDsArray[i] ) )
	if ( !ReadULong( pFile, (unsigned long *) &temp ) )
    { return AUD_FILE_READ_FAIL;
    }
	pIDList->pIDsArray[i] = temp;
  }
  return AUD_NO_ERROR;
}

/*
static AudErrorCode MDReadAudibleImage( IAudibleInputStream *pFile, CAudibleJPEGImage *pImageInfo, long sectionSize )

{ unsigned long curPos = pFile->pVTable->GetCurrentPos( pFile );
  if ( !( ReadULong( pFile, &pImageInfo->imageSize  ) &&
          ReadULong( pFile, &pImageInfo->imageStart )
        )
     )
    { return AUD_FILE_READ_FAIL;
    }
//------------ Image size should be section size minus 8. Image start should be section position plus 8
  if ( !( pImageInfo->imageSize + 8 == sectionSize && curPos + 8 == pImageInfo->imageStart ) )
  { return AUD_AA_FILE_BAD_FORMAT;
  }
  return AUD_NO_ERROR;
}
*/



/*
INT _reentrant AudibleAA_GetMetadataOverlayEntry( INT pAudFileData, INT pFile, INT *pExtra )
{
	return AudibleAA_GetMetadataOverlay( (CAudibleAAInternal *) pAudFileData, (IAudibleInputStream *) pFile );
}
*/



/*----------------------------------------------------------------------------*\
	Description:
		This routine gathers AA file metadata information without using
		InternalOpenAAFile(). This allows a minimum of routines to be used
		for metadata gathering which is more easily overlayed in tight memory
		spaces.
		
		This routine gathers metadata information that players are normally
		interested in: Title, Author, Time, Product ID, those kind of things.
		Other data returned are codec ID, encoded bitrate, and decoded
		sampling rate.
\*----------------------------------------------------------------------------*/

//AudErrorCode AudibleAA_GetMetadataOverlay( CAudibleAAInternal *pAudFileData, IAudibleInputStream * pFile, CAudibleFileInfoEmbedded * pAudibleFileInfoEmb, long flags )
AudErrorCode _reentrant AudibleAA_GetMetadataOverlay( CAudibleAAInternal *pAudFileData, IAudibleInputStream * pFile )
{
//	CAudibleAAInternal	audFileData;
	long				internalFlags;
	AudErrorCode		errCode;
	int					secNumber;

	// Translate flags for "optional" metadata to retrieve.
	internalFlags = AUD_FILE_COMMON_HEADER_FLAG | AUD_FILE_AUDIBLE_SPEC_HEADER_FLAG | AUD_FILE_TOC_SEC_FLAG | AUD_FILE_OPTIONAL_HEADER_FLAG | AUD_FILE_GROUPID_SEC_FLAG | AUD_FILE_PLAYERID_SEC;
/*
	if ( flags & AUDIBLE_METADATA_GET_STRINGS )
		internalFlags |= AUD_FILE_OPTIONAL_HEADER_FLAG;
	if ( flags & AUDIBLE_METADATA_GET_IMAGE )
		internalFlags |= AUD_FILE_IMAGE_SEC_FLAG;
//	if ( (pImageInfo->allocatedSize == 0) || (pImageInfo->pGPEGImage == NULL) )
//		return AUD_IMAGE_MEM_NOT_ALLOCATED;
*/

	// Always start at the beginning of the file
	pFile->pVTable->SetCurrentPos( pFile, 0 );

	// Check if this is an Audible file. Exit if not.
	errCode = MDValidateAAFileHeader( pFile );
	if ( errCode != AUD_NO_ERROR )
		return errCode;

	// We have an Audible file. Read all the sections into memory all at once.
	errCode = MDReadAAFileSections( pFile, pAudFileData );
	if ( errCode != AUD_NO_ERROR )
		return errCode;

	//---------- Now reads each section.
	for ( secNumber = 1; secNumber < MAX_SECTION_COUNT; secNumber++ )
	{
		// Read section if specified in flags. Otherwise, skip it.
		if ( !(internalFlags & ( 1 << secNumber )) )
			continue;
			
		//--- Seek position to the beginning of the section 
		errCode = pFile->pVTable->SetCurrentPos( pFile, pAudFileData->audSections[secNumber].sectionStart );
		if ( errCode != AUD_NO_ERROR )
			return errCode;

		//--- So position is at the beginning of the section. Now read this section. We have long case:
		switch( secNumber )
		{
			case AUD_FILE_COMMON_HEADER:
				errCode = MDReadAudibleCommonHeader( pFile, &pAudFileData->audCommonHeader, pAudFileData->audSections[secNumber].sectionLength );
				break;
			case AUD_FILE_OPTIONAL_HEADER:
				errCode = MDReadAudibleOptionalHeader( pFile, &pAudFileData->audOptionalHeader, pAudFileData->audSections[secNumber].sectionLength );
				break;
			case AUD_FILE_AUDIBLE_SPEC_HEADER:
				errCode = MDReadAudibleSpecificHeader( pFile, &pAudFileData->audSpecificHeader, pAudFileData->audSections[secNumber].sectionLength );
				break;
			case AUD_FILE_PLAYERID_SEC:
				errCode = MDReadAudibleIDList( pFile, &pAudFileData->audPlayerIDList, pAudFileData->audSections[secNumber].sectionLength );
				break;
			case AUD_FILE_GROUPID_SEC:
				errCode = MDReadAudibleIDList( pFile, &pAudFileData->audGroupIDList, pAudFileData->audSections[secNumber].sectionLength );
				break;
			case AUD_FILE_TOC_SEC:
				errCode = MDReadAudibleTableOfContent( pFile, &pAudFileData->audAudioChapters, pAudFileData->audSections[secNumber].sectionLength );
				break;
			//case AUD_FILE_HASH_SEC            : errCode =   ; break;
			//case AUD_FILE_PROMPT_TABLE_SEC    : errCode =   ; break;
			//case AUD_FILE_PROMPT_SEC          : errCode =   ; break;
			//case AUD_FILE_AUDIO_SEC           : errCode = ReadAudibleAudioOffsets  ( pFile, &pAudFileData->audAudioChapters,  pAudFileData->audSections[secNumber].sectionLength ); break;
			//case AUD_FILE_IMAGE_SEC:
			//	errCode = MDReadAudibleImage( pFile, &pAupAudFileDatadFileData->audJPEFImage, pAudFileData->audSections[secNumber].sectionLength );
			//	break;
		}
		if ( errCode != AUD_NO_ERROR )
			return errCode;
	}

/*
	// All sections are now in memory. Now copy the data into the metadata structure.
	errCode = FillAudibleFileInfoFromInternalData(	pAudFileData, &pAudibleFileInfoEmb->meta,
													ProcessValueNamePairForAudibleInfoEmbedded,
													&pAudibleFileInfoEmb->codec, &pAudibleFileInfoEmb->audio,
													&pAudibleFileInfoEmb->image, pFile, flags );
*/

	return errCode;
}
