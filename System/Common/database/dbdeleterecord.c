///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbdeleterecord.c
//! \brief
//!  The implementation of the DBDeleteRecord database API.
//!
//! Copyright (c) SigmaTel, Inc. Unpublished
//! 
//! SigmaTel, Inc.
//! Proprietary  Confidential
//! 
//! This source code and the algorithms implemented therein constitute 
//! confidential information and may comprise trade secrets of SigmaTel, Inc. 
//! or its associates, and any unauthorized use thereof is prohibited.
//! 
///////////////////////////////////////////////////////////////////////////////
#include <types.h>
#include <filesystem.h>
#include <fsapi.h>
#include <fstypes.h>
#include <fserror.h>
#include <database.h>
#include <database_internal.h>

static _reentrant INT DeleteString(DBState *pDBState, INT StringIndex);
static _reentrant INT IndexFileAdd(INT FileHandle, INT FileIndex);

//=============================================================================
//  FUNCTION: DBDeleteRecord
// ----------------------------------------------------------------------------
//! This API is used to remove a record, indicated by index passed in 
//! RecordIndex, from the database.
//! 
//! \param RecordIndex
//!     The index of the record that the caller wants to delete.
//!
//! \retval DB_ERROR_NONE
//!     The record was removed from the database.
//! \retval DB_ERROR_INDEX
//!     Invalid index passed in, it does not exist in the database.
//!
//=============================================================================
INT _reentrant DBDeleteRecord(INT RecordIndex)
{
    INT MediaIndex;
    INT TempIndex;
    INT i;
    INT Size;
    INT RetCode;
    LONG FilePosition;
    
    //
    // Get the media Index out of the DBIndex.
    //
    MediaIndex = GetMediaIndex(RecordIndex);

    //
    // Indicate that someone has touched the database.
    //
    g_DB_Dirty = TRUE;

    //
    // Now look for strings that may need to be deleted.
    //
    BufferUpdate(RecordIndex);
    
    //
    // if any reserved bits are set this record was already deleted.
    //
    if (g_DB_Buffer[DB_RECORD_SIZE_WORDS] & 0xffff)
    {
        return(DB_ERROR_INDEX);
    }
    
    //
    // Seek to the file location and get the old handle value.
    //
    if (SUCCESS != Fseek(g_DBState[MediaIndex].hStStore_Idx, 
                        g_DBState[MediaIndex].TopRecord.PutDelRecordIdx,
                        SEEK_SET))
    {
        DebugBuildAssert(0);
    }
    
    //
    // Write the handle out to the deleted index file.
    //
    if (DB_HANDLE_SIZE != Fwrite(g_DBState[MediaIndex].hStStore_Idx, 
                 &RecordIndex, 
                 DB_HANDLE_SIZE, 
                 DEFAULT_MEMORY,-1))
    {
        DebugBuildAssert(0);
    }
    
    //
    // Check if Put needs to wrap or not.
    //
    if (g_DBState[MediaIndex].TopRecord.PutDelRecordIdx == 
         g_DBState[MediaIndex].TopRecord.LastDelRecordIdx)
    { 
        //
        // If Put needs to wrap to the beginning but Get is already there then
        // we need to make space at the end of the index file.
        //
        if (g_DBState[MediaIndex].TopRecord.GetDelRecordIdx == 0)
        {
            g_DBState[MediaIndex].TopRecord.PutDelRecordIdx += DB_HANDLE_SIZE;
            g_DBState[MediaIndex].TopRecord.LastDelRecordIdx += DB_HANDLE_SIZE;
        }
        //
        // Otherwise we need to move Put back to the beginning of the file and
        // move the last valid position back one so that it does not point to 
        // an invalid entry.
        else
        {
            g_DBState[MediaIndex].TopRecord.PutDelRecordIdx = 0;
            g_DBState[MediaIndex].TopRecord.LastDelRecordIdx -= DB_HANDLE_SIZE;
            DebugBuildAssert(g_DBState[MediaIndex].TopRecord.LastDelRecordIdx >= 0);
        }
    }
    else
    {
        g_DBState[MediaIndex].TopRecord.PutDelRecordIdx += DB_HANDLE_SIZE;
        
        //
        // If Put does not need to wrap then make sure it did not just run into 
        // the Get position.  If it did then make room in the index file and 
        // move the Put position to the end of the file.
        //
        if (g_DBState[MediaIndex].TopRecord.PutDelRecordIdx == 
            g_DBState[MediaIndex].TopRecord.GetDelRecordIdx)
        {
            g_DBState[MediaIndex].TopRecord.LastDelRecordIdx += DB_HANDLE_SIZE;
            g_DBState[MediaIndex].TopRecord.PutDelRecordIdx = 
                g_DBState[MediaIndex].TopRecord.LastDelRecordIdx;
            g_DBState[MediaIndex].TopRecord.GetDelRecordIdx = 0;
        }
    }

    g_DBState[MediaIndex].TopRecord.TotalRecords--;
    g_DBState[MediaIndex].TopRecord.TotalDelRecords++;

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

    for (i=0; i < DB_RECORD_SIZE_WORDS; )
    {
        if (g_DB_Buffer[i] & DB_TAG_STRING)
        {
            // 
            // Remove this string from the string database.
            //
            if (DeleteString(&g_DBState[MediaIndex], g_DB_Buffer[i+1]) < 0)
            {
                return(DB_ERROR_CRITICAL);
            }
            i += 2;
        }
        else if (g_DB_Buffer[i] & DB_TAG_BLOCK)
        {
            Size = g_DB_Buffer[i] & DB_TAG_BLOCK_SIZE_MASK;
            // TODO get rid of this divide!!!!
            i += Size/3 + 1;
        }
        else
        {
            i += 2;
        }
    }
    //
    // Mark this record deleted also make it dirty so it gets flushed.
    //
    g_DB_Buffer[DB_RECORD_SIZE_WORDS] = -1;
    g_DB_BufferState |= DB_ENTRY_DIRTY;
    BufferFlush();
    
    return(SUCCESS);
}

//=============================================================================
//  FUNCTION: DeleteString
// ----------------------------------------------------------------------------
//! This function adds a string to the deleted index file.
//!
//! \param pDBState
//!     Pointer to the DBState structure for the database that is being used.
//! \param StringIndex
//!     This is the index of the string to add to the deleted list.
//!
//! \retval DB_ERROR_NONE
//!     The string index was added to the list of deleted strings.
//! \retval DB_ERROR_CRITICAL
//!     Something went wrong.
//!
//=============================================================================
static _reentrant INT DeleteString(DBState *pDBState, INT StringIndex)
{
    INT RetCode;
    
    //
    // Add this record to the list of deleted records.
    // This seek is necessary since IndexFileAdd() assumes that the file is 
    // at the point where an index can be added.  This will likely go away
    // at some point.
    //
    Fseek(pDBState->hStString_Idx, 
          pDBState->TopRecord.TotalDelStrings * DB_STRING_HANDLE_SIZE, 
          SEEK_SET);

    RetCode = IndexFileAdd(pDBState->hStString_Idx, StringIndex);
    if (RetCode < 0)
    {
        return(RetCode);
    }
    
    pDBState->TopRecord.TotalStrings--;
    pDBState->TopRecord.TotalDelStrings++;
    
    return(DB_ERROR_NONE);
}

//=============================================================================
//  FUNCTION: IndexFileAdd
// ----------------------------------------------------------------------------
//! Add an entry to the index file.
//!
//! \param FileHandle 
//!     The handle for the index file to use.
//! \param  FileIndex
//!     The index that we are looking to add to the list of indexes.
//!
//! \retval DB_ERROR_CRITICAL
//!     Something really bad happened and we have no way to recover.
//! \retval DB_ERROR_NONE
//!     The index was successfully added to the index file.
//!
//=============================================================================
static _reentrant INT IndexFileAdd(INT FileHandle, INT FileIndex)
{
    INT TempIndex;
    LONG FilePosition;
    LONG lPosition;
    
    //
    // Need to see if this entry has already been deleted.
    //
    FilePosition = Ftell(FileHandle);
    lPosition = FilePosition;
    Fseek(FileHandle, 0, SEEK_SET);

    while (lPosition > 0)
    {
        if (DB_STRING_HANDLE_SIZE != Fread(FileHandle, &TempIndex, 
                    DB_STRING_HANDLE_SIZE, 
                    DEFAULT_MEMORY, -1))
        {
            return(DB_ERROR_CRITICAL);
        }
        if (TempIndex == FileIndex)
        {
            break;
        }
        lPosition -= DB_STRING_HANDLE_SIZE;
    }    

    if (0 == lPosition)
    {
        //
        // Seek back to the original position.
        //
        Fseek(FileHandle, FilePosition, SEEK_SET);

        //
        // Update deleted list.
        //
        if (DB_STRING_HANDLE_SIZE != Fwrite(FileHandle, &FileIndex, 
                DB_STRING_HANDLE_SIZE, 
                DEFAULT_MEMORY, -1))
        {
            return(DB_ERROR_CRITICAL);
        }
    }
    return(DB_ERROR_NONE);
}
