///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbsetblock.c
//! \brief
//!     The implementation of the DBSetBlock database API.
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
//  FUNCTION: DBSetBlock
// ----------------------------------------------------------------------------
//! This API is used to set a block of data in the record indicated by the 
//! RecordIndex parameter.  This can be used to set a larger block of data 
//! without having to use a full string or multiple numbers.  The database will 
//! not interpret this data at all and will simply set block of data in the 
//! record at the offset passed in as RecordOffset.
//!
//! \param RecordIndex
//!      The index of the record that the caller wants to access.
//! \param RecordOffset
//!      The offset (in bytes) into the record that the caller wants to set.
//! \param Size
//!      The size (in bytes) of the data that is being passed in the buffer 
//!      pointed to by pBuffer.
//! \param pBuffer
//!      The buffer that will contain the to data set in the record.
//! \retval DB_ERROR_NONE
//!      The buffer was written to the database.
//! \retval DB_ERROR_OFFSET
//!      Invalid record offset passed in.
//! \retval DB_ERROR_SIZE
//!      The size passed in was too large, it must be less than the record size 
//!      of the database.
//! \retval DB_ERROR_HANDLE
//!      Invalid handle passed in.
//!
//=============================================================================
INT _reentrant DBSetBlock(INT RecordIndex, INT RecordOffset, INT Size, void *pBuffer)
{
    INT RetCode;
    INT Header;
    
    // 
    // Create the header.
    //
    Header = DB_TAG_BLOCK;
    Header += Size;

    RetCode = WriteDatabaseRecord(RecordIndex, RecordOffset, 3, &Header);
    if (RetCode != DB_ERROR_NONE)
    {
        return(RetCode);
    }
    
    //
    // Now write out Size number of bytes to the Database.
    //
    return(WriteDatabaseRecord(RecordIndex, RecordOffset + 3, Size, pBuffer));
}

