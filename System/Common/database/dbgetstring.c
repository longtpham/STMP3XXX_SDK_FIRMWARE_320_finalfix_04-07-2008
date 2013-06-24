///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbgetstring.c
//! \brief 
//! The implementation of the DBGetString database API
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//!
//! SigmaTel, Inc.
//! Proprietary  Confidential
//!
//! This source code and the algorithms implemented therein constitute
//! confidential information and may comprise trade secrets of SigmaTel, Inc.
//! or its associates, and any use thereof is subject to the terms and
//! conditions of the Confidential Disclosure Agreement pursuant to which this
//! source code was originally received.
//!
///////////////////////////////////////////////////////////////////////////////
#include <types.h>
#include <filesystem.h>
#include <fsapi.h>
#include <fstypes.h>
#include <fserror.h>
#include <database.h>
#include <database_internal.h>
#include <stringlib.h>

//=============================================================================
//  FUNCTION: DBGetString
// ----------------------------------------------------------------------------
//! This API is used to return a string from the record referenced by the value 
//! passed in the RecordIndex parameter.  The actual string that is returned is 
//! based on the offset indicated by the RecordOffset parameter passed in to this
//!  API.   The value passed in pSize should contain the actual size of the 
//! buffer pointed to by pBuffer.  The value pointed to by pSize will be changed 
//! to the actual size of the string returned in pBuffer.
//! 
//! \param RecordIndex
//!      The index of the record that the caller wants to access.
//! \param RecordOffset
//!      The offset (in bytes) to the area in the record reserved for this 
//!      string.
//! \param pSize
//!      A pointer to the size (in bytes) of the array pointed to by pBuffer.
//! \param pVoid
//!      A pointer to StringAPIParams.
//!      pVoid->pOffsetInBuffer
//!           offset in buffer where string should be copied.
//!      pVoid->pBuffer
//!           a pointer to buffer.
//!      pVoid->MemoryType
//!           X or Y memory for pVoid->pBuffer
//!
//! \retval DB_ERROR_NONE
//!      The value in pSize is the number of valid bytes returned in the buffer
//!      pointed to by pBuffer.
//! \retval DB_ERROR_OFFSET
//!      Invalid record offset was passed in.
//! \retval DB_ERROR_INDEX
//!      Invalid index passed in.
//! \retval DB_ERROR_STRING
//!      The string requested was not set yet.
//! \retval DB_ERROR_SIZE
//!      Not enough space in pBuffer to hold the string.
//! \retval DB_ERROR_CRITICAL
//!      The database experienced an unrecoverable error.
//!
//=============================================================================
INT _reentrant DBGetString(INT RecordIndex, INT RecordOffset, INT *pSize, 
                           void *pVoid)
{
    StringEntry sString;
    INT     MediaIndex;
    INT     RetVal;
    INT     NumBytesFromOffsetToNextWord;
    INT     FirstWord;
    INT *   pBuffer;
    INT     i,j;
    StringAPIParams *pStringAPIParams = (StringAPIParams*)pVoid;
    
    //
    // Get the media Index out of the RecordIndex.
    //
    MediaIndex = GetMediaIndex(RecordIndex);
    
    if (MediaIndex < 0)
    {
        return(DB_ERROR_INDEX);
    }

    //
    // Read the string from the database.
    //
    RetVal = ReadDatabaseRecord(RecordIndex, RecordOffset, 6, &sString);
    if (RetVal != DB_ERROR_NONE)
    {
        return(RetVal); 
    }
    
    //
    // See if this is a string entry.
    //
    if (0 == (sString.Size & DB_TAG_STRING))
    {
        return(DB_ERROR_STRING);
    }          

    //
    // Get the actual index to the string.
    //
    sString.Size &= DB_TAG_STRING_SIZE_MASK;

//MLP 2-21-06
// This += 2 caused MTP compliance failures on P4S 1.21.  
// The real fix is in the for loop below.
//    sString.Size+=2;    

    //
    // Not enough space in the buffer to return the string.
    //
    if (*pSize < sString.Size)
    {
        *pSize = sString.Size;
        return(DB_ERROR_SIZE);
    }
    *pSize = sString.Size;
    
    //
    // Seek to the position of this string in the file and read it out.
    //
    if (SeekDB(g_DBState[MediaIndex].hStString_Dat, 
               sString.Index * DB_STRING_SIZE_BYTES) < 0)
    {
        return(DB_ERROR_CRITICAL); 
    }

    // the offset_in_buffer may not be word aligned, so we must first find out
    // the bytes from offset to the next word. 
    NumBytesFromOffsetToNextWord = 0;
    if( *(pStringAPIParams->pOffsetInBuffer) > 0 )
    {
        switch( *(pStringAPIParams->pOffsetInBuffer) % 3 )
        {
            case 1:
            NumBytesFromOffsetToNextWord = 2; // offset points at start of a word, 0 b'cos 
            break;                   // all 3 bytes will be used of 1st word.
            case 2:            
            NumBytesFromOffsetToNextWord = 1; //offset points at second byte of the first word.
            break;
        };
    }

    // save buffer pointer to pBuffer for later use
    pBuffer = pStringAPIParams->pBuffer;

    // move the pointer pStringAPIParams->pBuffer to start of the word.
    pStringAPIParams->pBuffer = pStringAPIParams->pBuffer + (*(pStringAPIParams->pOffsetInBuffer) / 3); //starts at the next word boundary

    // if offset is not word aligned then we read into the next word, later we
    // move the bytes back to the right offset.
    if( NumBytesFromOffsetToNextWord > 0 )
        pStringAPIParams->pBuffer = pStringAPIParams->pBuffer + 1; //starts at the next word boundary

    // read from string table
    if (Fread(g_DBState[MediaIndex].hStString_Dat, (INT *)pStringAPIParams->pBuffer, 
        sString.Size, pStringAPIParams->MemoryType, -1) != sString.Size)
    {
        return(DB_ERROR_CRITICAL); 
    }

    if( NumBytesFromOffsetToNextWord != 0 )
    {
        // move the bytes back to start from *pOffsetInBuffer
        FirstWord = pStringAPIParams->pBuffer[0];

        // now use the saved pointer in pBuffer
        pBuffer = pBuffer + (*(pStringAPIParams->pOffsetInBuffer) / 3);

        i=0;
        if( NumBytesFromOffsetToNextWord == 2 )
            i=1;
        else if( NumBytesFromOffsetToNextWord == 1 )
            i=2;

        //MLP 2-21-06
        // Changed i<sString.Size to j<sString.Size since
        // i coulde be 0, 1 or 2 when that loop starts.
        for( j=0; j<sString.Size; i++, j++ )
        {
            if( pStringAPIParams->MemoryType == X_MEMORY )
            {
                PutByteInArrayInX((void _X *)pBuffer, i, GetByteFromArrayInX((void _X *)pStringAPIParams->pBuffer, j));
            }
            else
            {
                PutByteInArrayInY((void _Y *)pBuffer, i, GetByteFromArrayInY((void _Y *)pStringAPIParams->pBuffer, j));
            }
        }
    }
    
    return(DB_ERROR_NONE);
}
