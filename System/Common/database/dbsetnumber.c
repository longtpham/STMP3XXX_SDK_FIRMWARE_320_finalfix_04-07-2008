///////////////////////////////////////////////////////////////////////////////
//!
//! \file dbsetnumber.c
//! \brief
//!     The implementation of the DBSetNumber database API.
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
//  FUNCTION: DBSetNumber
// ----------------------------------------------------------------------------
//! This API is used to set a numeric value in the record referenced by
//! RecordIndex.  The actual location that is set in the record is based on the 
//! offset indicated by the RecordOffset parameter.  The size of the data pointed
//! to by pBuffer indicates the type of data stored in the database.  The caller 
//! must insure that they have requested the correct size, otherwise extra 
//! unwanted data or to little data may be returned in the buffer.
//! 
//! \param RecordIndex
//!      The index of the record that the caller wants to access.
//! \param RecordOffset
//!      The offset (in bytes) into the record that the caller wants to set.
//! \param Size
//!      The size (in bytes) of the array pointed to by pBuffer it also indicated
//!      the type of the number being set.
//! \param pBuffer
//!      The buffer that contains the number.
//!
//! \retval DB_ERROR_NONE
//!      The number was written to the database.
//! \retval DB_ERROR_OFFSET
//!      Invalid record offset passed in.
//! \retval DB_ERROR_SIZE
//!      Invalid number size was passed in.
//! \retval DB_ERROR_INDEX
//!      Invalid index passed in.
//! \retval DB_ERROR_CRITICAL
//!      The database experienced an unrecoverable error.
//!
//=============================================================================
INT _reentrant DBSetNumber(INT RecordIndex, INT RecordOffset, INT Size, void *pBuffer)
{
    INT Buffer[2];
    INT RetVal;
    INT *pTemp;
    
    if (Size > 4)
    {
        return(DB_ERROR_SIZE);
    }
    
    pTemp = (INT *)pBuffer;
    
    // 
    // Save the first value in the buffer since we are going to modify it.
    //
    Buffer[1] = *pTemp++;

    if (Size > 3)
    {
        Buffer[0] = *pTemp & DB_TAG_NUMBER_VALUE_MASK;
    }
    else
    {
        Buffer[0] = 0;
    }
    
    Buffer[0] |= DB_TAG_NUMBER;
        
    //
    // Write this out to the file.
    //
    RetVal = WriteDatabaseRecord(RecordIndex, RecordOffset, 
                                 DB_RECORD_ENTRY_SIZE_BYTES, Buffer);
    
    return(RetVal);
}
