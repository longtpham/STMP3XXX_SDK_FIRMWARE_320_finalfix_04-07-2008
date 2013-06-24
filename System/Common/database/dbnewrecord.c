///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbnewrecord.c
//! \brief
//!  The implementation of the DBNewRecord database API.
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

//=============================================================================
//  FUNCTION: DBNewRecord
// ----------------------------------------------------------------------------
//! This API is used allocate a new record in the database.  The MediaIndex
//! parameter is used to indicate which media that this object will reside on.
//! This allows for removable media to have its own database that travels with
//! the media.
//! If a record is created via this API but never has a value written into it
//! then it will be still have space reserved for it by the database when the
//! DBClose() is called.
//! This API is used allocate a new record in the database.  The MediaIndex 
//! parameter is used to indicate which media that this object will reside on.  
//! This allows for removable media to have its own database that travels with 
//! the media.
//! 
//! \param MediaIndex
//!      This indicates which media database this record will be allocated for.
//! \param pRecordIndex
//!      An index to the newly allocated record will be returned in the index
//!      value pointed to by this parameter.
//!
//! \retval DB_ERROR_NONE
//!      The new record was created in the database.
//! \retval DB_ERROR_MEDIA
//!      The media database number was not valid.
//!
//=============================================================================
INT _reentrant DBNewRecord (INT MediaIndex, INT *pRecordIndex)
{
    INT DeletedIndex;
    INT i;
    DB_BUFFER_MEMORY INT *Buffer;

    //
    // Make sure that the index is correct.
    //
    if (MediaIndex >= DB_MAX_DATABASE_STORES)
    {
        return(DB_ERROR_MEDIA);
    }

    //
    // Indicate that someone has touched the database.
    //
    g_DB_Dirty = TRUE;

    //
    // If we have given out more than 2^14 records, then we are done.
    //
    if (g_DBState[MediaIndex].TopRecord.TotalRecords >= 0x4000)
    {
        return(DB_ERROR_INDEX);
    }

    //
    // Increment the total number of records.
    //
    g_DBState[MediaIndex].TopRecord.TotalRecords++;
    
    //
    // If there are no deleted records to use, use a new one.
    //
    if (g_DBState[MediaIndex].TopRecord.TotalDelRecords == 0)
    {
        //
        // Increment the total number of records and return the new one.
        //
        *pRecordIndex = g_DBState[MediaIndex].TopRecord.TotalRecords;
    }
    else
    {
        //
        // Seek to the file location and get the old handle value.
        //
        if (SUCCESS != Fseek(g_DBState[MediaIndex].hStStore_Idx, 
                            g_DBState[MediaIndex].TopRecord.GetDelRecordIdx,
                            SEEK_SET))
        {
            DebugBuildAssert(0);
        }
        if (DB_HANDLE_SIZE != Fread(g_DBState[MediaIndex].hStStore_Idx, 
                     pRecordIndex, 
                     DB_HANDLE_SIZE, 
                     DEFAULT_MEMORY,-1))
        {
            DebugBuildAssert(0);
        }

        //
        // Increment the use cound and make sure the upper bit is clear, this 
        // will cause a wrap around in the counting.
        //
        *pRecordIndex += DB_COUNT_ADD;
        *pRecordIndex &= (~DB_MEDIA_INDEX_EXTERNAL);
        
        //
        // Decrement the number of valid deleted records in the store.idx file.
        //
        g_DBState[MediaIndex].TopRecord.TotalDelRecords--;

        //
        // If Get position has moved to the end then we need to wrap it back 
        // to the beginning.
        //
        if (g_DBState[MediaIndex].TopRecord.GetDelRecordIdx ==
            g_DBState[MediaIndex].TopRecord.LastDelRecordIdx)
        {
            g_DBState[MediaIndex].TopRecord.GetDelRecordIdx = 0;
            g_DBState[MediaIndex].TopRecord.LastDelRecordIdx = 
                g_DBState[MediaIndex].TopRecord.PutDelRecordIdx;
        }
        else
        {
            //
            // If put is at the end of the list then we need to move it back to 
            // the Get position since this entry is now now valid.  We need to
            // start putting newly deleted entries here to prevent a "hole" from
            // forming in the index file.
            //
            if (g_DBState[MediaIndex].TopRecord.PutDelRecordIdx ==
                g_DBState[MediaIndex].TopRecord.LastDelRecordIdx)
            {
                g_DBState[MediaIndex].TopRecord.PutDelRecordIdx =
                    g_DBState[MediaIndex].TopRecord.GetDelRecordIdx;
                g_DBState[MediaIndex].TopRecord.LastDelRecordIdx -= 
                    DB_HANDLE_SIZE;
                DebugBuildAssert(g_DBState[MediaIndex].TopRecord.LastDelRecordIdx >=0);
            }
            
            //
            // If Last just got backed up into the Get position then we need to
            // move Get back to the beginning of the list.  If it was already 
            // at the beginning of the list then that should be ok since 
            // we are now out of delete entries.
            // Otherwise if Get is not now at the last position move it up 
            // one position.
            //
            if (g_DBState[MediaIndex].TopRecord.GetDelRecordIdx == 
                g_DBState[MediaIndex].TopRecord.LastDelRecordIdx)
            {
                g_DBState[MediaIndex].TopRecord.GetDelRecordIdx = 0;   
            }
            else
            {
                g_DBState[MediaIndex].TopRecord.GetDelRecordIdx += 
                    DB_HANDLE_SIZE;
            }
            DebugBuildAssert(g_DBState[MediaIndex].TopRecord.GetDelRecordIdx <= 
                    g_DBState[MediaIndex].TopRecord.LastDelRecordIdx);
        }
        
        //
        // There are now no valid entries in the file so reset to the beginning.
        //
        if (g_DBState[MediaIndex].TopRecord.GetDelRecordIdx ==
            g_DBState[MediaIndex].TopRecord.PutDelRecordIdx)
        {
            DebugBuildAssert(g_DBState[MediaIndex].TopRecord.TotalDelRecords == 0);
            g_DBState[MediaIndex].TopRecord.GetDelRecordIdx = 0;
            g_DBState[MediaIndex].TopRecord.PutDelRecordIdx = 0;
            g_DBState[MediaIndex].TopRecord.LastDelRecordIdx = 0;
        }
    }
   
    if (g_DBState[MediaIndex].TopRecord.GetDelRecordIdx > 
        g_DBState[MediaIndex].TopRecord.PutDelRecordIdx)
    {
        DebugBuildAssert(g_DBState[MediaIndex].TopRecord.LastDelRecordIdx - 
         g_DBState[MediaIndex].TopRecord.GetDelRecordIdx + DB_HANDLE_SIZE +
         g_DBState[MediaIndex].TopRecord.PutDelRecordIdx == 
         g_DBState[MediaIndex].TopRecord.TotalDelRecords * DB_HANDLE_SIZE);
    }
    else
    {
        DebugBuildAssert((g_DBState[MediaIndex].TopRecord.PutDelRecordIdx - 
                         g_DBState[MediaIndex].TopRecord.GetDelRecordIdx) == 
                         g_DBState[MediaIndex].TopRecord.TotalDelRecords * DB_HANDLE_SIZE);
    }

    //
    // Set the media index that we are using, this can only be internal or
    // external.
    //
    if (0 != MediaIndex)
    {
        *pRecordIndex |= DB_MEDIA_INDEX_EXTERNAL;
    }

    //
    // Flush out the previous buffer as we are about to create a new one.
    //
    BufferFlush();

    //
    // Fill the buffer with 0.
    //
    Buffer = g_DB_Buffer;
    for (i=0; i < DB_RECORD_SIZE_WORDS+1; i++)
    {
        *Buffer++=0;
    }

    //
    // This data is now the current buffer.
    //
    g_DB_BufferIndex = *pRecordIndex;
    g_DB_BufferState = DB_ENTRY_DIRTY;
    
    return(DB_ERROR_NONE);
}
