///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbWriteRecord.c
//! \brief
//! database.c has been moved into system area to increase the speed of 
//! database reads.  Since the write does not need to be optimized it's
//! been moved back to pagable memory to help reduce the system area
//! size.
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
//! The database index that is currently in g_DB_Buffer[].
//!
extern INT g_DB_BufferIndex;
//!
//! This only holds one flag (DB_ENTRY_DIRTY) which indicates that this buffer
//! has been modified and will need to be written back to the storage device.
//!
extern INT g_DB_BufferState;

//!
//! This structure contains cached values that must be written back to the
//! non-volatile storage when a DBClose() command is issued.
//!
extern DBState g_DBState[DB_MAX_DATABASE_STORES];

//=============================================================================
//  FUNCTION: WriteDatabaseRecord
// ----------------------------------------------------------------------------
//! This function will handle the write accesses to the main database files.  
//! This will possibly flush out a buffer if it has been written to and a new
//! buffer needs to loaded into memory. 
//!
//! \param RecordIndex    
//!      The index to the record to read from the database
//! \param RecordOffset
//!      The offset in the record to read from the database.
//! \param Size
//!      The number of bytes to read out of the database.
//! \param pBuffer
//!      The buffer that will hold the data returned from the database.
//!      
//! \retval DB_ERROR_NONE
//!      The record was written to the database successfully.
//! \retval DB_ERROR_OFFSET
//!      The record offset was invalid.
//!
//=============================================================================
INT _reentrant WriteDatabaseRecord(INT RecordIndex, INT RecordOffset, INT Size, 
    void *pBuffer)
{
    INT i;
    INT BufferNum;
    DB_BUFFER_MEMORY INT *pDestBuffer;
    INT *pSourceBuffer;

    if (RecordOffset + Size >= DB_RECORD_SIZE_BYTES)
    {
        return(DB_ERROR_OFFSET);
    }

    //
    // TODO get this divide outta here!!!
    //
    pDestBuffer = g_DB_Buffer + (RecordOffset + 2)/3;
    pSourceBuffer = (INT *)pBuffer;

    BufferUpdate(RecordIndex);

    for (i=0; i<Size; i+=3)
    {
        *pDestBuffer++ = *pSourceBuffer++;
    }    

    //
    // Mark this dirty since we have written to it.
    //
    g_DB_BufferState = DB_ENTRY_DIRTY;
    
    return(DB_ERROR_NONE);
}
