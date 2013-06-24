///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbgetblock.c
//! \brief
//!     The implementation of the DBGetBlock database API.
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
// FUNCTION: DBGetBlock
//----------------------------------------------------------------------------
//! This API is used to return a block of data from the record indicated by the 
//! RecordIndex parameter.  This can be used to get a larger block of data from 
//! the record without having to use a full string or multiple number entries.  
//! The database will not interpret this data at all and will simply return the 
//! data contained at the offset passed in as RecordOffset.
//!
//! \param RecordIndex
//!     The index of the record that the caller wants to access.
//! \param RecordOffset
//!     The offset (in bytes) into the record that the caller wants to read.
//! \param pSize
//!     A pointer to the size (in bytes) of the data to be returned in pBuffer.
//! \param pBuffer
//!     The buffer that will contain the data returned from the object.
//!
//! \retval DB_ERROR_NONE
//!     The buffer was retrieved from the the database.
//! \retval DB_ERROR_NONE
//!     The buffer was retrieved from the database.
//! \retval DB_ERROR_OFFSET
//!     Invalid record offset passed in.
//! \retval DB_ERROR_SIZE
//!     The size passed in was too large; it must be less than the record size 
//!     of the database.
//! \retval DB_ERROR_INDEX
//!     Invalid index passed in.
//!
//=============================================================================
INT _reentrant DBGetBlock(INT RecordIndex, INT RecordOffset, INT *pSize, void *pBuffer)
{
    INT RetCode;
    INT Header;
    
    //
    // Create the header.
    //
    RetCode = ReadDatabaseRecord(RecordIndex, RecordOffset, 3, &Header);
    if (RetCode != DB_ERROR_NONE)
    {
        return(RetCode);
    }

    //
    // Quick check to see if the header matches what the caller is trying
    // to read out.
    //
    if ((Header & DB_TAG_BLOCK_SIZE_MASK) > *pSize)
    {
        *pSize = Header & DB_TAG_BLOCK_SIZE_MASK;
        return(DB_ERROR_SIZE);
    }
    *pSize = Header & DB_TAG_BLOCK_SIZE_MASK;

    return(ReadDatabaseRecord(RecordIndex, RecordOffset + 3, *pSize, pBuffer));
}
