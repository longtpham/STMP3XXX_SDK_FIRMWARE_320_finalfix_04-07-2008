////////////////////////////////////////////////////////////////////////////////
///
/// \file StAddNewObject.c
/// \brief This file implements StorAddNewObject API. 
/// This API is called by the handler for SendObjectInfo MTP Command
///
/// Copyright (c) SigmaTel, Inc. Unpublished
///
/// SigmaTel, Inc.
/// Proprietary  Confidential
///
/// This source code and the algorithms implemented therein constitute
/// confidential information and may comprise trade secrets of SigmaTel, Inc.
/// or its associates, and any use thereof is subject to the terms and
/// conditions of the Confidential Disclosure Agreement pursuant to which this
/// source code was originally received.
///
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//   Includes
////////////////////////////////////////////////////////////////////////////////
#include "mtp.h"
#include "filesystem.h"
#include "fsapi.h"
#include "fstypes.h"
#include "fserror.h"
#include "StError.h"
#include "ptpstring.h"
#include "StStore.h"
#include "resource.h"
#include "database.h"
#include "stringlib.h"
#include "system.h"
#include "audiblemtpobjecthandler.h"
#ifdef AUDIBLE
#include "AAMTPAccess.h"
#endif

#define POSITION_FILE_EXTENTION	0x534F50   // SOP

//////////////////////////////////////////////////////////////////
//! External Declaration
//////////////////////////////////////////////////////////////////
extern _packed BYTE _Y  g_packedTmpBuffer[];
extern SPtpString      g_TempPtpString;
extern StStoreArray    g_stor_StoreArray;

//////////////////////////////////////////////////////////////////
//! Local Functions
//////////////////////////////////////////////////////////////////
INT _reentrant GetAudiblePosFileName(WORD ObjectHandle, int PackedPathNameAddress, _packed BYTE *pTempUserBuf, StString *pTempStringBuf);
//The following functions are used for Host String manipulation.  They are copies of functions used
//by STWriteObjectData().  They were put here to prevent overlay switching.
int _reentrant iMtpGetLittleEndianINT16InY(void _Y *pBuffer, int iStartIndex);
void _reentrant iMtpPutLittleEndianInY(void _Y *pBuffer, int iStartIndex, long lValue, int iBytes);
INT _reentrant iConvStmp2HostUnicodeStr( INT * pHostStr, INT * pStmpStr, INT SizeOfStmpStrInChars );
INT _reentrant iConvHostUnicode2StmpStr( INT * pStmpStr, INT * pHostStr );

//If it's not an audible build, send back an error message.
#ifndef AUDIBLE
INT _reentrant StorAudibleCodecObjectProp(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue)
{
    return STOR_ERROR_INVALID_OBJECT_HANDLE;
}
INT _reentrant StorAudibleProductID(EnGetSet OPType, WORD ObjectHandle, StString *rValue)
{
    return STOR_ERROR_INVALID_OBJECT_HANDLE;
}    
INT _reentrant StorAudibleTitleSegment(EnGetSet OPType, WORD ObjectHandle, UINT128 *rValue)
{
    return STOR_ERROR_INVALID_OBJECT_HANDLE;
}
INT _reentrant StorAudiblePlaybackPosition(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue)
{
     return STOR_ERROR_INVALID_OBJECT_HANDLE;
}
INT _reentrant StorAudiblePlayedThrough(EnGetSet OPType, WORD ObjectHandle, UINT16 *rValue)
{
     return STOR_ERROR_INVALID_OBJECT_HANDLE;
}
INT _reentrant DeleteAudiblePosFile(WORD ObjectHandle, int PackedPathNameAddress, _packed BYTE *pTempUserBuf, StString *pTempStringBuf)
{
     return STOR_ERROR_INVALID_OBJECT_HANDLE;
}
#else
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT _reentrant StorAudibleCodecObjectProp(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue)
//!
//! This function is used as link between the MTP Object GET/SET commands and the retrieval of audible metadata.
//! 
//! \param [IN] OPType
//!     Set to GetIt to retreive object properties.  Set to SetIt to set object properties
//! \param [in] ObjectHandle
//!     Contines the object handle for which the propery is being set or retreived.
//! \param [in] rValue
//!     Contains the new rValue if OPType is set to SetIt or gets set to the current
//!     object property rValue if OPType is set to GetIt.
//!
//!  Output:   STOR_ERROR_NONE – Successful execution.
//!            STOR_ERROR_READING_FILE – Error during data retrieval.
//!            STOR_ERROR_OBJECT_READONLY - Set OPType not supported.
 
INT _reentrant StorAudibleCodecObjectProp(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue)
{
    INT error = STOR_ERROR_NONE;

    if(OPType == GetIt)
    {
        //First step is to retrieve the filename.
        error = StorObjectShortFilename(GetIt, ObjectHandle, g_packedTmpBuffer);
        if(error != STOR_ERROR_NONE)
            goto done;
        
        //Retrieve the audible metadata
        //error = SysCallFunction( RSRC_AUDIBLE_METADATA_CODEBANK, AAMTPGetCodecID, (INT)g_packedTmpBuffer, 0, (void *)rValue );
        error = AAMTPGetCodecID((INT)g_packedTmpBuffer, 0, (void *)rValue );

        if(error != AUD_NO_ERROR)
        {
            error = STOR_ERROR_READING_FILE;  //Convert all AudErrorCode to STOR_ERROR_READING_FILE
            goto done;
        } 
    }
    else
    {//SetIt
        error = STOR_ERROR_OBJECT_READONLY;  //Set is not supported so return an error.
    }
    
done:
    return error;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT _reentrant StorAudibleProductID(EnGetSet OPType, WORD ObjectHandle, StString *rValue)
//!
//! This function is used as link between the MTP Object GET/SET commands and the retrieval of audible metadata.
//! 
//! \param [IN] OPType
//!     Set to GetIt to retreive object properties.  Set to SetIt to set object properties
//! \param [in] ObjectHandle
//!     Contines the object handle for which the propery is being set or retreived.
//! \param [in] rValue
//!     Contains the new rValue if OPType is set to SetIt or gets set to the current
//!     object property rValue if OPType is set to GetIt.
//!
//!  Output:   STOR_ERROR_NONE – Successful execution.
//!            STOR_ERROR_READING_FILE – Error during data retrieval.
//!            STOR_ERROR_OBJECT_READONLY - Set OPType not supported.
 
INT _reentrant StorAudibleProductID(EnGetSet OPType, WORD ObjectHandle, StString *rValue)
{
    INT error = STOR_ERROR_NONE;
    int iStrLen;
    
    if(OPType == GetIt)
    {
        //First step is to retrieve the filename.
        error = StorObjectShortFilename(GetIt, ObjectHandle, g_packedTmpBuffer);
        if(error != STOR_ERROR_NONE)
            goto done;
        
        //Retrieve the audible metadata
        //error = SysCallFunction( RSRC_AUDIBLE_METADATA_CODEBANK, AAMTPGetProductID, (INT)g_packedTmpBuffer, 0, (void *)rValue );
        error = AAMTPGetProductID((INT)g_packedTmpBuffer, 0, (void *)rValue );
        if(error != AUD_NO_ERROR)
        {
            error = STOR_ERROR_READING_FILE;  //Convert all AudErrorCode to STOR_ERROR_READING_FILE
            goto done;
        }
        //Convert to packed 2-byte characters.
        iStrLen= strlen((UCS3 _Y *)rValue);
        iConvStmp2HostUnicodeStr((int *)rValue,(int *)rValue, iStrLen); 
    }
    else
    {//SetIt
        error = STOR_ERROR_OBJECT_READONLY;  //Set is not supported so return an error.
        goto done;
    }
    
done:
    return error;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT _reentrant StorAudibleTitleSegment(EnGetSet OPType, WORD ObjectHandle, StString *rValue)
//!
//! This function is used as link between the MTP Object GET/SET commands and the retrieval of audible metadata.
//! 
//! \param [IN] OPType
//!     Set to GetIt to retreive object properties.  Set to SetIt to set object properties
//! \param [in] ObjectHandle
//!     Contines the object handle for which the propery is being set or retreived.
//! \param [in] rValue
//!     Contains the new rValue if OPType is set to SetIt or gets set to the current
//!     object property rValue if OPType is set to GetIt.
//!
//!  Output:   STOR_ERROR_NONE – Successful execution.
//!            STOR_ERROR_READING_FILE – Error during data retrieval.
//!            STOR_ERROR_OBJECT_READONLY - Set OPType not supported.
 
INT _reentrant StorAudibleTitleSegment(EnGetSet OPType, WORD ObjectHandle, UINT128 *rValue)
{
    INT error = STOR_ERROR_NONE;

    if(OPType == GetIt)
    {
        //First step is to retrieve the filename.
        error = StorObjectShortFilename(GetIt, ObjectHandle, g_packedTmpBuffer);
        if(error != STOR_ERROR_NONE)
            goto done;
        
        //Retrieve the audible metadata
        //error = SysCallFunction( RSRC_AUDIBLE_METADATA_CODEBANK, AAMTPGetTitleSegment, (INT)g_packedTmpBuffer, 0, (void *)rValue );
        error = AAMTPGetTitleSegment((INT)g_packedTmpBuffer, 0, (void *)rValue ); 
        if(error != AUD_NO_ERROR)
        {
            error = STOR_ERROR_READING_FILE;  //Convert all AudErrorCode to STOR_ERROR_READING_FILE
            goto done;
        } 
    }
    else
    {//SetIt
        error = STOR_ERROR_OBJECT_READONLY;  //Set is not supported so return an error.
        goto done;
    }
    
done:
    return error;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT _reentrant StorAudiblePlaybackPosition(EnGetSet OPType, WORD ObjectHandle, StString *rValue)
//!
//! This function is used as link between the MTP Object GET/SET commands and the retrieval of audible metadata.
//! 
//! \param [IN] OPType
//!     Set to GetIt to retreive object properties.  Set to SetIt to set object properties
//! \param [in] ObjectHandle
//!     Contines the object handle for which the propery is being set or retreived.
//! \param [in] rValue
//!     Contains the new rValue if OPType is set to SetIt or gets set to the current
//!     object property rValue if OPType is set to GetIt.
//!
//!  Output:   STOR_ERROR_NONE – Successful execution.
//!            STOR_ERROR_READING_FILE – Error during data retrieval.
//!            STOR_ERROR_OBJECT_READONLY - Set OPType not supported.
 
INT _reentrant StorAudiblePlaybackPosition(EnGetSet OPType, WORD ObjectHandle, UINT32 *rValue)
{
    INT error = STOR_ERROR_NONE;

    if(OPType == GetIt)
    {
        //The playback position is contained inside the .pos file.
        error = GetAudiblePosFileName(ObjectHandle, (INT)g_packedTmpBuffer, (_packed BYTE *)&g_wUserScratchY, (StString *)&g_TempPtpString);
        if(error != AUD_NO_ERROR)
        {
            error = STOR_ERROR_READING_FILE;  //Convert all AudErrorCode to STOR_ERROR_READING_FILE
            goto done;
        } 
        
        //Retrieve the audible metadata
        //error = SysCallFunction( RSRC_AUDIBLE_METADATA_CODEBANK, AAMTPGetPlaybackPosition, (INT)g_packedTmpBuffer, 0, (void *)rValue );
        error = AAMTPGetPlaybackPosition((INT)g_packedTmpBuffer, 0, (void *)rValue );
    }
    else
    {//SetIt
        error = STOR_ERROR_OBJECT_READONLY;  //Set is not supported so return an error.
        goto done;
    }
    
done:
    return error;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT _reentrant StorAudiblePlayedThrough(EnGetSet OPType, WORD ObjectHandle, StString *rValue)
//!
//! This function is used as link between the MTP Object GET/SET commands and the retrieval of audible metadata.
//! 
//! \param [IN] OPType
//!     Set to GetIt to retreive object properties.  Set to SetIt to set object properties
//! \param [in] ObjectHandle
//!     Contines the object handle for which the propery is being set or retreived.
//! \param [in] rValue
//!     Contains the new rValue if OPType is set to SetIt or gets set to the current
//!     object property rValue if OPType is set to GetIt.
//!
//!  Output:   STOR_ERROR_NONE – Successful execution.
//!            STOR_ERROR_READING_FILE – Error during data retrieval.
//!            STOR_ERROR_OBJECT_READONLY - Set OPType not supported.
 
INT _reentrant StorAudiblePlayedThrough(EnGetSet OPType, WORD ObjectHandle, UINT16 *rValue)
{
    INT error = STOR_ERROR_NONE;

    if(OPType == GetIt)
    {
        //The Played Through data is contained inside the .pos file.
        error = GetAudiblePosFileName(ObjectHandle, (INT)g_packedTmpBuffer, (_packed BYTE *)&g_wUserScratchY, (StString *)&g_TempPtpString);
        
        if(error != AUD_NO_ERROR)
        {
            error = STOR_ERROR_READING_FILE;  //Convert all AudErrorCode to STOR_ERROR_READING_FILE
            goto done;
        } 
        
        //Retrieve the audible metadata
        //error = SysCallFunction( RSRC_AUDIBLE_METADATA_CODEBANK, AAMTPGetPlaybackPosition, (INT)g_packedTmpBuffer, 0, (void *)rValue );
        error = AAMTPGetPlayedThrough((INT)g_packedTmpBuffer, 0, (void *)rValue );
    }
    else
    {//SetIt
        error = STOR_ERROR_OBJECT_READONLY;  //Set is not supported so return an error.
        goto done;
    }
    
done:
    return error;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT _reentrant GetAudiblePosFileName(WORD ObjectHandle , _packed BYTE * pFilePath)
//!
//! This function is used to retrieve the file name of the .
//! 
//! \param [IN] pFilePath 
//!     Location to plce 
//! \param [in] ObjectHandle
//!     Contines the object handle for which the propery is being set or retreived.
//!
//!  Output:   STOR_ERROR_NONE – Successful execution.
//!            STOR errors from StorObjectXX calls.            

INT _reentrant GetAudiblePosFileName(WORD ObjectHandle, int PackedPathNameAddress, _packed BYTE *pTempUserBuf, StString *pTempStringBuf)
{   
    int error = STOR_ERROR_NONE;
    DWORD parent_handle = 0;
    WORD wExtension[2];
    INT str_len = 0;
    int ByteOffset = 0;
            
    //First step is to retrieve the filepath of the parent.
    error = StorObjectParent(GetIt, ObjectHandle, &parent_handle);
    if(error != STOR_ERROR_NONE)
        return error;

    if( parent_handle != 0 && parent_handle != 0xFFFFFF )
    {
        StorObjectShortFilename(GetIt, parent_handle, (_packed BYTE*)PackedPathNameAddress);
        if(error != STOR_ERROR_NONE)
            return error;
            
        packed_strcat((_packed BYTE*)PackedPathNameAddress, (_packed BYTE *)"/");
    }
    else
    {
        //Need to add MMC support.  Model 'store_number' after StWriteObjectData.c.
        packed_strcpy((_packed BYTE*)PackedPathNameAddress, g_stor_StoreArray.Entry[0]->Root);
    }
                          
    //Now get the LFN of the .POS file and pack it into a temp buffer
    error = StorObjectFileName(GetIt, ObjectHandle, (INT *)pTempStringBuf, (INT*)&ByteOffset, Y_MEMORY);
    if(error != STOR_ERROR_NONE)
        return error;

    //LFN is stored as a packed 2-byte unicode string, convert to 1 character per word.  
    iConvHostUnicode2StmpStr((INT *)pTempUserBuf, (INT *)pTempStringBuf);
        
    pack_string((UCS3 _Y *)pTempUserBuf, (_packed BYTE *)pTempStringBuf);
    
    //Add the LFN file name to the end of the parent's SFN path 
    packed_strcat((_packed BYTE*)PackedPathNameAddress, (_packed BYTE*)pTempStringBuf);
	 
    str_len = packed_strlen((_packed BYTE*)PackedPathNameAddress);
    	
    // Find the '.'
	while(str_len--) {
	    // Start at the end, go backward until the char is '.'
	    if(packed_get((_packed BYTE*)PackedPathNameAddress, str_len)=='.')
	   		break;	
    }
    
    // Append POS to PackedPathNameAddress string
	str_len++;
	packed_set((_packed BYTE*)PackedPathNameAddress, str_len, 0);
	wExtension[0] = POSITION_FILE_EXTENTION;
	wExtension[1] = 0;
    packed_strcat((_packed BYTE*)PackedPathNameAddress,(_packed BYTE*)wExtension);
    
    return error;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : INT _reentrant DeleteAudiblePosFile(WORD ObjectHandle, int PackedPathNameAddress, 
//!                                                  int * pTempUserBuf, int * pTempStringBuf)
//!
//! This function is used to delete the .pos file.
//! 
//! \param [in] ObjectHandle
//!     Contines the object handle for which the propery is being set or retreived.
//! \param [IN] PackedPathNameAddress
//!     Holds the name of the position file.
//! \param [in] pTempUserBuf
//!     Pointer to a temp. buffer
//! \param [in] rValue
//!     Pointer to a temp string buffer
//!
//!  Output:   STOR_ERROR_NONE – Successful execution.
//!            STOR errors from StorObjectXX calls.
//!            Error from Fremove
  
INT _reentrant DeleteAudiblePosFile(WORD ObjectHandle, int PackedPathNameAddress, _packed BYTE *pTempUserBuf, StString *pTempStringBuf)
{
    INT error = STOR_ERROR_NONE;
    
    _packed BYTE *pUserBuf = pTempUserBuf;   
    //error = GetAudiblePosFileName(ObjectHandle, PackedPathNameAddress, (_packed BYTE *)pTempUserBuf, (StString *)pTempStringBuf);

    error = GetAudiblePosFileName(ObjectHandle, PackedPathNameAddress, pUserBuf, pTempStringBuf);

    if(error != STOR_ERROR_NONE)
        return error;
            
    //Remove the position file
    error = Fremove( (_packed char *)PackedPathNameAddress );
        
    return error;
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
int _reentrant iMtpGetLittleEndianINT16InY(void _Y *pBuffer, int iStartIndex)
//#endif
{
    int iValue=0;
    int i;
    for(i=1;i>=0;i--)
    {
        iValue = (iValue << 8) | GetByteFromArrayInY(pBuffer,iStartIndex+i);
    }
    
    return iValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : void _reentrant ConvHostUnicode2StmpStr( INT * pStmpStr, 
//!                   INT * pHostStr )
//!  This function converts host string (2-byte character string) to stmp
//!  strings (3-byte character string). 
//! 
INT _reentrant iConvHostUnicode2StmpStr( INT * pStmpStr, INT * pHostStr )
{
    BYTE ii;
    INT num_chars;
    INT byte_offset;

    byte_offset = 0;
    num_chars = GetByteFromArrayInY(pHostStr, byte_offset);

    byte_offset = 1;
    for(ii = 0; ii < num_chars; ii++)
    {
        pStmpStr[ii] = iMtpGetLittleEndianINT16InY(pHostStr, byte_offset);
        byte_offset += 2;
    }
    pStmpStr[ii]=0x000000;
    return num_chars;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : void _reentrant MtpPutLittleEndianInY(void _Y *pBuffer, int iStartIndex, long lValue, int iBytes)
//!  This function places iBytes from lValue into pBuffer.
//! 
void _reentrant iMtpPutLittleEndianInY(void _Y *pBuffer, int iStartIndex, long lValue, int iBytes)
{
    int i;
    for(i=0;i<iBytes;i++)
    {
        PutByteInArrayInY(pBuffer,iStartIndex + i,lValue & 0xff);
        lValue=lValue>>8;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \prototype : void _reentrant ConvStmp2HostUnicodeStr( INT * pHostStr, 
//!                   INT * pStmpStr, INT SizeOfStmpStrInChars )
//!  This function converts stmp unicode string (3-byte character string) to host
//!  unicode strings (2-byte character string). 
//! 
INT _reentrant iConvStmp2HostUnicodeStr( INT * pHostStr, INT * pStmpStr, INT SizeOfStmpStrInChars )
{
    BYTE ii;

    for(ii = 0; ii < SizeOfStmpStrInChars;  ii++)
    {       
        // it is important to do this check here and not afte calling MtpPutLittleEndianINT16InY,
        // because pStmpStr[ii] may change if host and stmp strings are pointing 
        // to same buffer. we must write null too.
        if ( pStmpStr[ii] == 0x000000 )
        {
            iMtpPutLittleEndianInY(pHostStr, (ii*2)+1, (UINT16)pStmpStr[ii], 2);
            break;
        }
        iMtpPutLittleEndianInY(pHostStr, (ii*2)+1, (UINT16)pStmpStr[ii], 2);
    }
    // write the size of string at 0 offset
    PutByteInArrayInY(pHostStr, 0, ii);

    return ii-1;
}
#endif