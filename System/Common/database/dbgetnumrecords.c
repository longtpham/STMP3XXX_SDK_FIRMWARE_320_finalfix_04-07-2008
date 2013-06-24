///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbgetnumrecords.c
//! \brief The implementation of the DBGetNumRecords database API.
//!
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
//  FUNCTION: DBGetNumRecords
// ----------------------------------------------------------------------------
//! This API is used to return the total number of records in the database based
//! on MediaIndex.  
//! 
//! \param MediaIndex
//!      This indicates which media database to return number of objects for.
//! \param pNumRecords
//!      Pointer to the number of objects returned from this call.
//!
//! \retval DB_ERROR_NONE
//!      The number of records was returned in pNumObjects.
//! \retval DB_ERROR_MEDIA
//!      The media database number was not valid.
//! \retval DB_ERROR_NOT_INIT
//!      The database indicated is not initialized.
//!
//=============================================================================
INT _reentrant DBGetNumRecords(INT MediaIndex, INT *pNumRecords)
{
    //
    // Make sure that the index is correct.
    //
    if (MediaIndex >= DB_MAX_DATABASE_STORES)
    {
        return(DB_ERROR_MEDIA);
    }

    //
    // Make sure that the database has already been opened.
    //
    if (0 == g_DBState[MediaIndex].hStStore_Dat)
    {
        return(DB_ERROR_NOT_OPEN);
    }

    *pNumRecords = g_DBState[MediaIndex].TopRecord.TotalRecords;

    return(DB_ERROR_NONE);
}
