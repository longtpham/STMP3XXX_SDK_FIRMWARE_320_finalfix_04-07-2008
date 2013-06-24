///////////////////////////////////////////////////////////////////////////////
//!
//! \file database.c
//! \brief
//! The common functions used by the database APIs.
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

//! 
//! The global buffer and it's state.
//!
DB_BUFFER_MEMORY INT g_DB_Buffer[DB_RECORD_SIZE_WORDS+1];
//!
//! The database index that is currently in g_DB_Buffer[].
//!
INT g_DB_BufferIndex = -1;
//!
//! This only holds one flag (DB_ENTRY_DIRTY) which indicates that this buffer
//! has been modified and will need to be written back to the storage device.
//!
INT g_DB_BufferState = 0;

//!
//! This variable indicates if any changes have been made to the database.
//!
volatile INT g_DB_Dirty=0;

//!
//! This structure contains cached values that must be written back to the
//! non-volatile storage when a DBClose() command is issued.
//!
DBState g_DBState[DB_MAX_DATABASE_STORES]={0};

//=============================================================================
//  FUNCTION: GetMediaIndex
// ----------------------------------------------------------------------------
//! \brief Just return the media index from the Database index.
//!
//! \param DBIndex
//!      This needs to be the database index that was returned from a call to 
//!      DBNewRecord.
//! \retval 0 
//!     Media 0 selected.
//! \retval 1
//!     Media 1 selected. 
//!
//=============================================================================
INT _reentrant GetMediaIndex(INT DBIndex)
{
    if (DB_MEDIA_INDEX_EXTERNAL & DBIndex)
    {
        return(1);
    }
    return(0);
}

//=============================================================================
//  FUNCTION: SeekDB
// ----------------------------------------------------------------------------
//! This function just makes sure that a seek is actually successful by calling
//! Ftell as well.
//!
//! \param hFile
//!     The file handle to seek.
//! \param lPos
//!     The position to seek to.
//!
//! \retval 0 
//!     Successful seek.
//! \retval -1 
//!     Seek was unsuccessful. 
//!
//=============================================================================
INT _reentrant SeekDB(INT hFile, LONG lPos)
{
    INT fs_err = Fseek(hFile, lPos, SEEK_SET); 
    if( fs_err != FS_SUCCESS )
        return(-1);
    if (Ftell(hFile) != lPos)
    {
        
        return(-1);
    }
    return(0);
}

//=============================================================================
//  FUNCTION: ReadDatabaseRecord
// ----------------------------------------------------------------------------
//! This function will handle the accesses to the main database files.  This will
//! possibly flush out a buffer if it has been written to 
//!
//! \param RecordIndex
//!      The index to the record to read from the database.
//! \param RecordOffset
//!      The offset in the record to read from the database.
//! \param Size
//!      The number of bytes to read out of the database.
//! \param pBuffer
//!      The buffer that will hold the data returned from the database.
//!      
//! \retval DB_ERROR_NONE
//!      The record was read back from the database successfully.
//! \retval DB_ERROR_OFFSET
//!      The record offset was invalid.
//!
//=============================================================================
INT _reentrant ReadDatabaseRecord(INT RecordIndex, INT RecordOffset, INT Size, 
    void *pBuffer)
{
    INT i;
    INT BufferNum;
    INT *pDestBuffer;
    DB_BUFFER_MEMORY INT *pSourceBuffer;

    if (RecordOffset + Size > DB_RECORD_SIZE_BYTES)
    {
        return(DB_ERROR_OFFSET);
    }

    //
    // TODO get this divide outta here!!!
    //
    pSourceBuffer = g_DB_Buffer + (RecordOffset + 2)/3;
    pDestBuffer = (INT *)pBuffer;

    BufferUpdate(RecordIndex);

    for (i=0; i<Size; i+=3)
    {
        *pDestBuffer++ = *pSourceBuffer++;
    }    

    return(DB_ERROR_NONE);
}

//=============================================================================
//  FUNCTION: BufferUpdate
// ----------------------------------------------------------------------------
//! This function will make sure that the old buffer is flushed to the storage
//! an that then new buffer is loaded into the buffer.
//!
//! \param RecordIndex
//!      The record that needs to be loaded into the buffer.
//!
//! \retval SUCCESS
//!      The buffer is loaded with the record indicated by RecordIndex.
//!
//=============================================================================
INT _reentrant BufferUpdate(INT RecordIndex)
{
    INT MediaIndex;
    LONG BytesRead;
    INT FileIndex;

    //
    // Check if current record is already in memory.
    //
    if (g_DB_BufferIndex == RecordIndex)
    {
        return(SUCCESS);
    }

    //
    // Only used the offset here.
    //
    FileIndex = RecordIndex & DB_INDEX_MASK;
    
    //
    // Get the media Index out of the DBIndex.
    //
    MediaIndex = GetMediaIndex(RecordIndex);

    //
    // Write out the old buffer if it has been written to.
    //
    BufferFlush();

    //
    // Save which record is in here.
    //
    g_DB_BufferIndex = RecordIndex;
    
    //
    // Read the new buffer in.
    //
    Fseek(g_DBState[MediaIndex].hStStore_Dat, 
          DB_RECORD_SIZE_BYTES * FileIndex, SEEK_SET);
    
    //
    // If this fails to read anything in this buffer was not written out yet
    // This is not an error.
    //
    BytesRead = Fread(g_DBState[MediaIndex].hStStore_Dat, (INT *)g_DB_Buffer, 
                  DB_RECORD_SIZE_BYTES, DB_BUFFER_MEMORY_TYPE, -1);
                  
    if((BytesRead != 0) && (BytesRead != DB_RECORD_SIZE_BYTES))
    {
        DebugBuildAssert(0);
    }

               
    //
    // Mark this not dirty since we have just read it in to memory.
    //
    g_DB_BufferState &= ~DB_ENTRY_DIRTY;

    return(SUCCESS);
}

//=============================================================================
//  FUNCTION: BufferFlush
// ----------------------------------------------------------------------------
//! This function flushes the buffered data out to storage.
//!
//! \param void
//!
//! \retval SUCCESS
//!      The buffer was written out successfully. 
//! \retval DB_ERROR_CRITICAL
//!      Something went horribly wrong.
//!
//=============================================================================
INT _reentrant BufferFlush(void)
{
    INT Index;
    INT MediaIndex;

    //
    // The buffer is invalid so just return.
    //
    if (-1 == g_DB_BufferIndex)
    {
        return(SUCCESS);
    }

    //
    // The buffer was never written just return.
    //
    if (0 == (g_DB_BufferState & DB_ENTRY_DIRTY))
    {
        return(SUCCESS); 
    }
    
    Index = g_DB_BufferIndex & DB_INDEX_MASK;
    MediaIndex = GetMediaIndex(g_DB_BufferIndex);
    
    //
    // Write the buffer out to the storage media.
    //
    if (SeekDB(g_DBState[MediaIndex].hStStore_Dat, DB_RECORD_SIZE_BYTES * Index) < 0)
    {
        DebugBuildAssert(0);
        return(DB_ERROR_CRITICAL);
    }

    if (DB_RECORD_SIZE_BYTES != Fwrite(g_DBState[MediaIndex].hStStore_Dat, 
                (INT *)g_DB_Buffer, DB_RECORD_SIZE_BYTES, 
                DB_BUFFER_MEMORY_TYPE, -1))
    {
        DebugBuildAssert(0);
        return(DB_ERROR_CRITICAL);
    }

    //
    // Clear the dirty flag so that this buffer is not written again.
    //
    g_DB_BufferState &= ~DB_ENTRY_DIRTY;
    
    return(SUCCESS);
}
