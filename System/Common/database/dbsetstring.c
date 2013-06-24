///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbsetstring.c
//! \brief
//!     The implementation of the DBSetString database API.
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
//  FUNCTION: DBSetString
// ----------------------------------------------------------------------------
//! This API is used to set a string in the record referenced by the value passed
//! in the RecordIndex parameter.  The actual string that is set is based on the
//! offset indicated by the RecordOffset parameter passed in to this API.   The
//! value passed in pSize should contain the actual size of the string that is
//! pointed to by pBuffer.  This API will not check the string size and will
//! simply store *pSize bytes into the database.
//! 
//! \param RecordIndex
//!      The index of the record that the caller wants to access.
//! \param RecordOffset
//!      The offset (in bytes) to the area in the record reserved for this string.
//! \param Size
//!      The size (in bytes) of the string pointed to by pBuffer.
//! \param pBuffer
//!      The buffer that the string will be copied from.
//! \param OffsetInBuffer, 
//!      Offset in buffer where string is present
//! \param Buffer_Memory
//!      X or Y memory.
//!
//! \retval DB_ERROR_NONE
//!      The string was successfully written into the database.
//! \retval DB_ERROR_OFFSET
//!      Invalid record offset passed in.
//! \retval DB_ERROR_SIZE
//!      The size of the string was too large.
//! \retval DB_ERROR_INDEX
//!      Invalid index passed in.
//! \retval DB_ERROR_CRITICAL
//!      The database experienced an unrecoverable error.
//!
//=============================================================================
INT _reentrant DBSetString(INT RecordIndex, INT RecordOffset, INT Size, 
                           void *pVoid)
{
    StringEntry sString;
    INT     MediaIndex;
    INT     StringIdx;
    LONG    lSeekPos;
    INT     RetVal;
    INT     NumBytesFromOffsetToNextWord;
    INT     FirstWord;
    StringAPIParams *pStringAPIParams = (StringAPIParams*)pVoid;
    
    //
    // Make sure the string is not too long.
    //
    if (Size > DB_STRING_SIZE_BYTES)
    {
        return(DB_ERROR_SIZE);
    }

    //
    // Get the media Index out of the RecordIndex.
    //
    MediaIndex = GetMediaIndex(RecordIndex);
    
    if (MediaIndex < 0)
    {
        return(DB_ERROR_INDEX);
    }

    //
    // Write out the string to the database.
    //
    RetVal = ReadDatabaseRecord(RecordIndex, RecordOffset, 6, &sString);
    if (RetVal != DB_ERROR_NONE)
    {
        return(RetVal); 
    }

    //
    // If a string is already allocated then the caller is just setting this 
    // string to a new value else.
    //
    if (DB_TAG_STRING != (sString.Size & DB_TAG_MASK))
    {
        if (0 != g_DBState[MediaIndex].TopRecord.TotalDelStrings)
        {
            //
            // Seek to the last deleted string in the index file.
            //
            Fseek(g_DBState[MediaIndex].hStString_Idx, (g_DBState[MediaIndex].TopRecord.TotalDelStrings - 1) * 3, SEEK_SET);
            
            // use a deleted string.
            Fread(g_DBState[MediaIndex].hStString_Idx, &StringIdx, 3, DEFAULT_MEMORY, -1);
            lSeekPos = StringIdx * DB_STRING_SIZE_BYTES;  
            
            //
            // We used up one of the deleted strings.
            //
            g_DBState[MediaIndex].TopRecord.TotalDelStrings--;
            sString.Index = StringIdx;
        }
        else
        {
            // use a new string.
            lSeekPos = 
                g_DBState[MediaIndex].TopRecord.TotalStrings * DB_STRING_SIZE_BYTES;
            sString.Index = g_DBState[MediaIndex].TopRecord.TotalStrings;
        }

        //
        // Update the number of strings in the database and the current position 
        // in the file.
        // It should be ok to update this up here eventhough we can fail later.
        // All errors after this indicate that the database is not working.
        //
        g_DBState[MediaIndex].TopRecord.TotalStrings++;
    }
    else
    {
        //
        // Seek to the already allocated string.
        //
        lSeekPos = sString.Index * DB_STRING_SIZE_BYTES;
    }
    
    //
    // Set up the size and tag entry for the main database file.
    //
    sString.Size = Size | DB_TAG_STRING;

    //
    // Write out the string to the database.
    //
    if (WriteDatabaseRecord(RecordIndex, RecordOffset, 6, &sString) != DB_ERROR_NONE)
    {
        return(DB_ERROR_CRITICAL); 
    }

    //
    // Seek to the current position in the string database.
    //
    if (SeekDB(g_DBState[MediaIndex].hStString_Dat, lSeekPos))
    {
        return(DB_ERROR_CRITICAL); 
    }
    
    //
    // Write the buffer out to the storage media.
    //

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
    FirstWord = 0;
    if( NumBytesFromOffsetToNextWord != 0 )
    {
        // write the bytes from first word if offset is not word aligned
        if( (NumBytesFromOffsetToNextWord == 1) /* && (Size >= 1) */)
        {
            if( pStringAPIParams->MemoryType == X_MEMORY )
                FirstWord = GetByteFromArrayInX((void _X *)pStringAPIParams->pBuffer, *(pStringAPIParams->pOffsetInBuffer));
            else
                FirstWord = GetByteFromArrayInY((void _Y *)pStringAPIParams->pBuffer, *(pStringAPIParams->pOffsetInBuffer));
        }
        else if( (NumBytesFromOffsetToNextWord == 2) /* && (Size >= 2) */)
        {
            if( pStringAPIParams->MemoryType == X_MEMORY )
            {
                FirstWord = GetByteFromArrayInX((void _X *)pStringAPIParams->pBuffer, *(pStringAPIParams->pOffsetInBuffer)+1); //read the next byte first
                FirstWord = (FirstWord << 8) | GetByteFromArrayInX((void _X *)pStringAPIParams->pBuffer, *(pStringAPIParams->pOffsetInBuffer)); //read the byte
            }
            else
            {
                FirstWord = GetByteFromArrayInY((void _Y *)pStringAPIParams->pBuffer, *(pStringAPIParams->pOffsetInBuffer)+1); //read the next byte
                FirstWord = (FirstWord << 8) | GetByteFromArrayInY((void _Y *)pStringAPIParams->pBuffer, *(pStringAPIParams->pOffsetInBuffer)); //read the byte
            }
        }
        if (NumBytesFromOffsetToNextWord != Fwrite(g_DBState[MediaIndex].hStString_Dat, 
                    (INT *)&FirstWord, NumBytesFromOffsetToNextWord, DEFAULT_MEMORY, -1))
        {
            return(DB_ERROR_CRITICAL); 
        }
    }

    // point to the right place in buffer to read string
    pStringAPIParams->pBuffer = pStringAPIParams->pBuffer + (*(pStringAPIParams->pOffsetInBuffer) / 3); //starts at the next word boundary

    // skip those bytes that are already written
    if( NumBytesFromOffsetToNextWord > 0 )
        pStringAPIParams->pBuffer = pStringAPIParams->pBuffer + 1; //starts at the next word boundary

    // write the complete string
    if ((DB_STRING_SIZE_BYTES - NumBytesFromOffsetToNextWord) != Fwrite(g_DBState[MediaIndex].hStString_Dat, 
                (INT *)pStringAPIParams->pBuffer, (DB_STRING_SIZE_BYTES - NumBytesFromOffsetToNextWord), pStringAPIParams->MemoryType, -1))
    {
        return(DB_ERROR_CRITICAL); 
    }
    
    return(DB_ERROR_NONE);
}
