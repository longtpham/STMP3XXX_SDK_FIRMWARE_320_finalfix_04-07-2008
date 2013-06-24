///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbclose.c
//! \brief
//!  The implementation of the DBClose database API.
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

static _reentrant INT CloseDatabaseFiles(INT MediaIndex);

//=============================================================================
//  FUNCTION: DBClose
// ----------------------------------------------------------------------------
//! This API will insure that the database flushes its data out to the storage
//! media indicated by MediaIndex.   This call will also close out all files used
//! by the database, and the system can be shut down safely after this call is
//! made.  Since the database will keep some portion of the data in memory during
//! normal operation, it will be necessary to perform a DBClose operation if the
//! system is about to lose power.  This call may take some time to complete as
//! some checking and clean up will occur with this call.
//! This API is used to close the database based on MediaIndex.
//! 
//! \param MediaIndex
//!      This indicates which media database to close.
//!
//! \retval DB_ERROR_NONE
//!      The database was successfully closed.
//! \retval DB_ERROR_MEDIA
//!      The media database number was not valid.
//! \retval DB_ERROR_NOT_OPEN
//!      The media database number was already opened.
//! \retval DB_ERROR_CRITICAL
//!     The database could not be closed and should be considered to be in a
//!     corrupted state.
//!
//=============================================================================
INT _reentrant DBClose(INT MediaIndex)
{
    //
    // Make sure that the index is correct.
    //
    if (MediaIndex >= DB_MAX_DATABASE_STORES)
    {
        return(DB_ERROR_MEDIA);
    }

    //
    // If the main database file is not open then the database has not been 
    // opened yet.
    //
    if (0 == g_DBState[MediaIndex].hStStore_Dat)
    {
        return(DB_ERROR_NOT_OPEN);
    }

    //
    // Flush the database buffer.
    //
    BufferFlush();

    //
    // Flush the first record out to non-volatile storage.
    //
    Fseek(g_DBState[MediaIndex].hStStore_Dat, 0, SEEK_SET);
    Fwrite(g_DBState[MediaIndex].hStStore_Dat,
          (INT *)&(g_DBState[MediaIndex].TopRecord),
          sizeof(DBTopRecord)*3, DEFAULT_MEMORY,-1);

    //
    // Close out the database files.
    // 
    if (SUCCESS != CloseDatabaseFiles(MediaIndex))
    {
        return(DB_ERROR_CRITICAL);
    }
    
    //
    // Indicate that someone has touched the database.
    //
    g_DB_Dirty = FALSE;
    
    return(DB_ERROR_NONE);
}

//=============================================================================
// FUNCTION: CloseDatabaseFiles
// ----------------------------------------------------------------------------
//! Just closes out all of the database files that were opened when the 
//! database was opened. 
//!
//! \param MediaIndex 
//!     The media index to media that contains the database to be closed.
//!
//! \retval DB_ERROR
//!     Could not open all database files.
//! \retval SUCCESS
//!     Database files are open for read/write access. 
//!
//=============================================================================
static _reentrant INT CloseDatabaseFiles(INT MediaIndex)
{
    INT RetCode;

    //
    // Close out all of the database files.
    //
    RetCode = Fclose(g_DBState[MediaIndex].hStStore_Dat);
    g_DBState[MediaIndex].hStStore_Dat = 0;
    
    RetCode |= Fclose(g_DBState[MediaIndex].hStStore_Idx);
    g_DBState[MediaIndex].hStStore_Idx = 0;
    
    RetCode |= Fclose(g_DBState[MediaIndex].hStString_Dat);
    g_DBState[MediaIndex].hStString_Dat = 0;
    
    RetCode |= Fclose(g_DBState[MediaIndex].hStString_Idx);
    g_DBState[MediaIndex].hStString_Idx = 0;
    
    if (SUCCESS != RetCode)
    {
        return(DB_ERROR_CRITICAL);
    }

    return(SUCCESS);
}
